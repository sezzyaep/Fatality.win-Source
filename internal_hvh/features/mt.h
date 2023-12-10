#pragma once
#include <atomic>

namespace detail
{
	typedef int( __cdecl* allocate_thread_id_t )( );
	typedef void( __cdecl* free_thread_id_t )( );

	struct spinlock
	{
		std::atomic<bool> lock_ = { false };

		void lock() noexcept
		{
			for ( ;; )
			{
				// Optimistically assume the lock is free on the first try
				if ( !lock_.exchange( true, std::memory_order_acquire ) )
				{
					return;
				}
				// Wait for lock to be released without generating cache misses
				while ( lock_.load( std::memory_order_relaxed ) )
				{
					// Issue X86 PAUSE or ARM YIELD instruction to reduce contention between
					// hyper-threads
					_mm_pause();
				}
			}
		}

		bool try_lock() noexcept
		{
			// First do a relaxed load to check if lock is free in order to prevent
			// unnecessary cache misses if someone does while(!try_lock())
			return !lock_.load( std::memory_order_relaxed ) &&
				!lock_.exchange( true, std::memory_order_acquire );
		}

		void unlock() noexcept
		{
			lock_.store( false, std::memory_order_release );
		}
	};

	class call_queue
	{
	public:
		struct queue_element
		{
			queue_element( std::function<void()> call ) : call( std::move( call ) ) {}
			std::function<void()> call;
		};


		call_queue() = default;
		void init();

		void perform( std::vector<std::shared_ptr<queue_element>>& calls );

		void process_queue( int index );
#ifndef RELEASE
		void decomission();
#endif
		std::vector<std::thread> threads = {};
		bool decomissioned = {};
		int count_ = {};
	private:
		allocate_thread_id_t allocate_thread_id_ = {};
		free_thread_id_t free_thread_id_ = {};

		size_t calculate_remaining_tls_slots() const;
		std::vector<std::shared_ptr<queue_element>> queue_ = {};
		std::atomic<int> to_run_ = 0;
		spinlock queue_mtx_ = {};
		std::condition_variable_any queue_cond_ = {};
	};

	extern call_queue callqueue;
}
