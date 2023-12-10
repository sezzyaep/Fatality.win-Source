#pragma once

template<typename T>
class msg_queue
{
public:
	// preventing implicit generation of move constructor 
	~msg_queue() {}

	void post( T&& msg )
	{
		std::lock_guard<std::mutex> lock( mtx );
		queue.clear();
		queue.push_front( std::move( msg ) );
	}

	bool recv( T& msg )
	{
		std::unique_lock<std::mutex> lock( mtx );
		if ( queue.empty() )
			return false;

		msg = queue.back();
		return true;
	}

	bool try_recv( T& msg )
	{
		if ( !mtx.try_lock() )
			return true;

		if ( queue.empty() )
		{
			mtx.unlock();
			msg = {};
			return false;
		}

		msg = queue.back();
		mtx.unlock();
		return true;
	}

	unsigned int size()
	{
		std::lock_guard<std::mutex> lock( mtx );
		return queue.size();
	}

	void clear()
	{
		std::lock_guard<std::mutex> lock( mtx );
		queue.clear();
	}

private:
	std::deque<T> queue;
	std::mutex mtx;
};
