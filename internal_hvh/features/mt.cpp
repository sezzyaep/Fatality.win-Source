#include "../include_cheat.h"

namespace detail
{
	call_queue callqueue;

	size_t call_queue::calculate_remaining_tls_slots() const
	{
		static constexpr auto max_threads = 32;

		auto highest_index = 1;
		while ( highest_index < 128 && reinterpret_cast< uint8_t* >( make_offset( "tier0.dll", sig_tls_slots ) )[ highest_index ] )
			highest_index++;

		return clamp( max_threads - highest_index, 0, max_threads );
	}

#pragma clang section text=".discard"

	__declspec( noinline ) void call_queue::init()
	{
		// locate tls initializer entry of the game.
		allocate_thread_id_ = reinterpret_cast< allocate_thread_id_t >( make_offset( "tier0.dll", sig_allocate_thread_id ) );
		free_thread_id_ = reinterpret_cast< free_thread_id_t >( make_offset( "tier0.dll", sig_free_thread_id ) );

		auto str = std::to_string( std::thread::hardware_concurrency() ) + " " + std::to_string( calculate_remaining_tls_slots() ) + "\n";
		util::print_log( str.c_str() );

		auto max_threads = std::thread::hardware_concurrency();
		if ( max_threads == 20 )
			--max_threads;

		count_ = std::min( static_cast< size_t >( floorf( 0.5f * max_threads ) ) - 2, calculate_remaining_tls_slots() );
		count_ = clamp( count_, 2, 7 );

		// create enough threads to maintain the tracer.
		for ( auto i = 0u; i < count_; i++ )
			threads.emplace_back( &call_queue::process_queue, this, i );

		erase_end;
	}

#pragma clang section text=""

	// Caller is responsible for making sure this is synchronized!
	// NOTE: This function is blocking.
	void call_queue::perform( std::vector<std::shared_ptr<queue_element>>& calls )
	{
		if ( calls.empty() )
			return;

		// add new calls to dispatch.
		std::unique_lock lock( queue_mtx_ );

		queue_.insert( queue_.end(), calls.begin(), calls.end() );
		to_run_ = queue_.size();

		// work until the calls are completed.
		for ( ;; )
		{
			// retrieve a new job.
			if ( queue_.empty() )
			{
				while ( to_run_ > 0 )
					_mm_pause();

				break;
			}

			const auto current = std::move( queue_.back() );
			queue_.pop_back();
			lock.unlock();

			queue_cond_.notify_all();

			current->call();

			// decrease remaining calls.
			--to_run_;

			lock.lock();
		}
	}

	void call_queue::process_queue( int index )
	{
		//SetThreadAffinityMask( GetCurrentThread(), 1 << ( index + 1 ) );

		// allocate tls storage for the current thread.
		allocate_thread_id_();

		// process the actual queue.
		for ( ;; )
		{
			// retrieve a new job.
			std::unique_lock lock( queue_mtx_ );
			queue_cond_.wait( lock, [this] { return !queue_.empty() || decomissioned; } );
			if ( decomissioned )
				break;

			const auto current = std::move( queue_.back() );
			queue_.pop_back();
			lock.unlock();

			queue_cond_.notify_all();

			current->call();

			// decrease remaining calls.
			--to_run_;
		}


		free_thread_id_();
	}

#ifndef RELEASE
	void call_queue::decomission()
	{
		std::unique_lock lock( queue_mtx_ );
		if ( decomissioned )
			return;

		decomissioned = true;
		lock.unlock();
		queue_cond_.notify_all();

		for ( auto& thread : threads )
			thread.join();
	}
#endif
}