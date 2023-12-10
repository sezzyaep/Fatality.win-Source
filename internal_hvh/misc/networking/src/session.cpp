#include <system_error>
#include <ws2tcpip.h>
#include "../include_cheat.h"

#include "session.h"
#include <data.h>

namespace networking
{
	namespace proxy
	{
		__declspec( noinline ) int WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData )
		{
			return ::WSAStartup( wVersionRequested, lpWSAData );
		}

		__declspec( noinline ) int getaddrinfo( PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA* pHints, PADDRINFOA* ppResult )
		{
			return ::getaddrinfo( pNodeName, pServiceName, pHints, ppResult );
		}

		__declspec( noinline ) SOCKET socket( int af, int type, int protocol )
		{
			return ::socket( af, type, protocol );
		}

		__declspec( noinline ) int setsockopt( SOCKET s, int level, int optname, const char* optval, int optlen )
		{
			return ::setsockopt( s, level, optname, optval, optlen );
		}

		__declspec( noinline ) int connect( SOCKET s, const sockaddr* name, int namelen )
		{
			return ::connect( s, name, namelen );
		}

		__declspec( noinline ) int closesocket( SOCKET s )
		{
			return ::closesocket( s );
		}

		__declspec( noinline ) void freeaddrinfo( PADDRINFOA pAddrInfo )
		{
			::freeaddrinfo( pAddrInfo );
		}

		__declspec( noinline ) int ioctlsocket( SOCKET s, long cmd, u_long* argp )
		{
			return ::ioctlsocket( s, cmd, argp );
		}
	}

	__attribute__( ( always_inline ) ) session::session( const char* server, uint16_t port, const std::string& token ) : socket_( INVALID_SOCKET )
	{
		log( "c1\n" );

		WSADATA data;
		if ( proxy::WSAStartup( MAKEWORD( 2, 2 ), &data ) != 0 )
			util::error_close( xorstr_( "Failed to connect 0x00001040" ) );

		addrinfo hints = { 0 };
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		log( "c2\n" );

		addrinfo* info;
		if ( proxy::getaddrinfo( server, std::to_string( port ).c_str(), &hints, &info ) != 0 )
			util::error_close( xorstr_( "Failed to connect 0x00001041" ) );

		for ( auto ptr = info; ptr != nullptr; ptr = ptr->ai_next )
		{
			log( "c3\n" );
			socket_ = proxy::socket( ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol );
			if ( socket_ == INVALID_SOCKET )
				util::error_close( xorstr_( "Failed to connect 0x00001042" ) );

			bool reuse_addr{ true };
			uint32_t timeout{ 5000 };
			linger opt_linger{ 0, 0 };

			proxy::setsockopt( socket_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast< char* >( &reuse_addr ), sizeof( bool ) );
			proxy::setsockopt( socket_, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast< char* >( &timeout ), sizeof( uint32_t ) );
			proxy::setsockopt( socket_, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast< char* >( &timeout ), sizeof( uint32_t ) );
			proxy::setsockopt( socket_, SOL_SOCKET, SO_LINGER, reinterpret_cast< char* >( &opt_linger ), sizeof( linger ) );

			log( "c4\n" );
			if ( proxy::connect( socket_, ptr->ai_addr, ( int ) ptr->ai_addrlen ) == 0 )
				break;

			log( "c4.5\n" );
			proxy::closesocket( socket_ );
			socket_ = INVALID_SOCKET;
		}

		log_debug( "c5\n" );
		proxy::freeaddrinfo( info );

		if ( socket_ == INVALID_SOCKET )
			util::error_close( xorstr_( "Failed to connect 0x00001043" ) );

		u_long v = 1;
		log_debug( "c6\n" );
		if ( FAILED( proxy::ioctlsocket( socket_, FIONBIO, &v ) ) )
		{
			proxy::closesocket( socket_ );
			util::error_close( xorstr_( "Failed to connect 0x00001044" ) );
		}

		// init ssl
		ctx_ = std::make_shared<crypto::ssl_context>();
		if ( !ctx_->create_client() )
		{
			proxy::closesocket( socket_ );
			util::error_close( xorstr_( "Failed to connect 0x00001045" ) );
		}

		ssl_ = std::make_shared<crypto::ssl>( ctx_, socket_ );
		if ( !ssl_->is_valid() )
		{
			proxy::closesocket( socket_ );
			util::error_close( xorstr_( "Failed to connect 0x00001046" ) );
		}

		if ( !ssl_->connect() )
		{
			ssl_->shutdown( true );
			proxy::closesocket( socket_ );
			util::error_close( xorstr_( "Failed to connect 0x00001047" ) );
		}


		log( "cs\n" );

		// Send initial client info.
		const auto buf = token.data();
		if ( !ssl_->send( ( unsigned char* ) buf, static_cast< int >( token.length() ) + 1 ) )
		{
			ssl_->shutdown( true );
			proxy::closesocket( socket_ );
			util::error_close( xorstr_( "Failed to connect 0x00001048" ) );
		}

		log_debug( "sent key\n" );
	}

	template<class T>
	T my_ntoh( unsigned char* buf )
	{
		const auto s = sizeof( T );
		T value = 0;
		for ( unsigned i = 0; i < s; i++ )
			value |= buf[ i ] << CHAR_BIT * ( s - 1 - i );
		return value;
	}

	bool session::has_data()
	{
		char buf[ 1 ];

		const auto e = recv( socket_, buf, 1, MSG_PEEK );
		if ( e == SOCKET_ERROR )
			return false;

		if ( !e )
		{
			close();
			return false;
		}

		return true;
	}

	bool session::is_connected()
	{
		if ( closed.load() || !ssl_->is_valid() )
			return false;

		if ( !has_data() )
		{
			const auto error = WSAGetLastError( );
			if ( error > WSABASEERR && ( error == WSAECONNRESET || error == WSAECONNABORTED || error != WSAEWOULDBLOCK ) )
			{
				close();
				return false;
			}
		}

		return true;
	}

	bool session::read( std::string& s )
	{
		char buf[ 2048 ];

		size_t ret{};
		if ( !ssl_->recv( buf, 4, &ret ) || ret != 4 )
			return false;

		const auto packet_size = *reinterpret_cast< uint32_t* >( buf );

		do
		{
			memset( buf, 0, sizeof( buf ) );

			const auto to_recv = std::min( packet_size - s.size(), sizeof( buf ) );

			size_t actual{};
			if ( !ssl_->recv( buf, to_recv, &actual ) )
			{
				if ( !is_connected() )
					break;
			}

			s.append( buf, actual );

			if ( packet_size == s.size() || actual == 0 )
				break;

		} while ( true );

		if ( s.size() != packet_size )
		{
			s.clear();
			return false;
		}

		return true;
	}

	bool session::write( const std::vector<unsigned char>& s )
	{
		std::vector<unsigned char> buf;
		buf.resize( 4 + s.size() );
		*reinterpret_cast< uint32_t* >( buf.data() ) = s.size();
		memcpy( buf.data() + 4, s.data(), s.size() );

		size_t total_written{};

		while ( total_written < buf.size() )
		{
			constexpr auto buf_size = 2048u;
			const auto to_send = std::min( buf.size() - total_written, buf_size );
			size_t written{};

			if ( !ssl_->send( buf.data() + total_written, to_send, &written ) )
			{
				if ( !is_connected() )
					break;
			}

			if ( written < to_send )
				return false;

			total_written += written;
		}

		return true;
	}

	int session::update( const std::vector<unsigned char>& update )
	{
		int size = 0;
		log_debug( "mutex.lock\n" );
		mutex.lock();
		log_debug( "updates_.push_back\n" );
		updates_.emplace_back( update );
		size = static_cast< int >( updates_.size() );
		log_debug( "mutex.unlock\n" );
		mutex.unlock();
		log_debug( "return\n" );
		return size;
	}

	bool session::try_write()
	{
		std::lock_guard _lock( mutex );

		try
		{
			// test if we are still connected & valid
			if ( !is_connected() )
				return false;

			for ( const auto& m : updates_ )
			{
				/*if ( !write( gzip::compress( data.c_str(), data.size() ) ) )*/
				log_debug_fmt( "sending datapack, size: %d\n", m.size() );
				if ( !write( m ) )
				{
					if ( !is_connected() )
						return false;
				}
			}
		}
		catch ( const std::exception& ex )
		{
			log_debug_fmt( "Exception in %s: %s", XOR( __FUNCTION__ ), ex.what() );
		}

		updates_.clear();
		return true;
	}

	bool session::try_read( std::vector<std::vector<unsigned char>>& ret )
	{
		using namespace nlohmann;

		try
		{
			if ( !has_data() )
				return is_connected();

			do
			{
				std::string raw{};
				if ( !read( raw ) )
					return is_connected();

				ret.emplace_back( raw.begin(), raw.end() );
			} while ( SSL_pending( ssl_->obj ) >= 4 );
		}
		catch ( const std::exception& ex )
		{
			log_debug_fmt( "Exception #1 in %s: %s", XOR( __FUNCTION__ ), ex.what() );
		}

		return true;
	}

	std::vector<std::vector<unsigned char>> session::process( bool& success )
	{
		success = false;

		// Send our updates.
		if ( !try_write() )
			return {};

		std::vector<std::vector<unsigned char>> ret;
		// Receive incoming updates.
		if ( !try_read( ret ) )
			return {};

		success = true;

		return ret;
	}
}