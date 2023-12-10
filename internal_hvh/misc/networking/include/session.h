#pragma once

#include <cstdint>
#include <vector>
#include <mutex>

#include "data.h"
#include "../src/crypto/ssl.h"

namespace networking
{
	class session
	{
	public:
		session( const char* server, uint16_t port, const std::string& token );
		bool has_data();
		bool is_connected();
		bool read( std::string& s );
		bool write( const std::vector<unsigned char>& s );
		int update( const std::vector<unsigned char>& update );
		bool try_write();
		bool try_read( std::vector<std::vector<unsigned char>>& ret );
		std::vector<std::vector<unsigned char>> process( bool& success );

		struct
		{
			uint8_t shared_key[ 32 ];
			uint8_t nonce[ 24 ];
			bool has_key = false;
		} key{};
		std::atomic_bool closed = false;

		inline void close()
		{
			if ( !closed.load() )
			{
				closed = true;
				ssl_->shutdown( true );
				imp( "ws2_32.dll", closesocket )( socket_ );
			}

		}

		inline ~session()
		{
			close();
		}


	private:
		std::shared_ptr<crypto::ssl> ssl_;
		std::shared_ptr<crypto::ssl_context> ctx_;

		SOCKET socket_;
		std::vector<std::vector<unsigned char>> updates_;
		std::mutex mutex;
		std::mutex networking_mutex;
	};

	inline int init_status = 0;

}