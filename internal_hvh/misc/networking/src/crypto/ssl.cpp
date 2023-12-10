#include "../include_cheat.h"
#include <openssl/err.h>
#include "ssl.h"

#include <thread>
#include <chrono>

using namespace crypto;

void crypto::ssl_print_error( const std::string& message )
{
	util::print_log( XOR( "SSL error occured: %s\n" ), message.c_str() );
	ERR_print_errors_fp( stdout );
}

ssl::ssl( const std::shared_ptr<ssl_context>& ctx, uintptr_t sock )
{
	std::lock_guard _lock( obj_mtx );

	obj = SSL_new( ctx->get() );
	if ( !obj )
	{
		ssl_print_error( XOR( "Failed to create SSL object" ) );
		return;
	}

	SSL_set_fd( obj, sock );
}

ssl::~ssl()
{
	shutdown( false );
}

bool ssl::accept()
{
	return do_async_call( SSL_accept, XOR( "Failed to accept SSL socket" ) );
}

bool ssl::connect()
{
	return do_async_call( SSL_connect, XOR( "Failed to connect to SSL socket" ) );
}

bool ssl::send( unsigned char* buf, size_t len, size_t* r )
{
	return do_async_call( SSL_write, XOR( "Failed to write to SSL socket" ), buf, len, r );
}

bool ssl::recv( char* buf, size_t len, size_t* r )
{
	return do_async_call( SSL_read, XOR( "Failed to read from SSL socket" ), buf, len, r );
}

bool ssl::do_async_call( const std::function<int( SSL* )> fn, const std::string& err_msg, size_t* r )
{
	std::lock_guard _lock( obj_mtx );
	if ( !is_valid() ) return false;

	auto h = fn( obj );
	if ( h <= 0 )
	{
		auto e = SSL_get_error( obj, h );
		while ( e == SSL_ERROR_WANT_READ || e == SSL_ERROR_WANT_WRITE )
		{
			h = fn( obj );
			e = h <= 0 ? SSL_get_error( obj, h ) : 0;

			using namespace std::chrono_literals;
			std::this_thread::sleep_for( 10ms );
			std::this_thread::yield();
		}

		if ( e > 0 )
		{
			if ( e == SSL_ERROR_SYSCALL )
			{
				ERR_print_errors_fp( stdout );
				return false;
			}

			if ( e == SSL_ERROR_ZERO_RETURN )
				return false;

			log_debug_fmt( "SSL error occured: %i (return code %i)", e, h );
			if ( !err_msg.empty() )
				ssl_print_error( err_msg );

			return false;
		}
	}

	if ( r )
		*r = h;

	return true;
}

bool ssl::do_async_call( const std::function<int( SSL*, void*, int )> fn, const std::string& err_msg, void* buf, size_t len, size_t* r )
{
	std::lock_guard _lock( obj_mtx );
	if ( !is_valid() ) return false;

	log_debug_fmt( "Calling IO SSL function (buf 0x%X, len %i)\n", buf, len );

	auto h = fn( obj, buf, len );
	if ( h <= 0 )
	{
		auto e = SSL_get_error( obj, h );
		while ( e == SSL_ERROR_WANT_READ || e == SSL_ERROR_WANT_WRITE )
		{
			h = fn( obj, buf, len );
			e = h <= 0 ? SSL_get_error( obj, h ) : 0;

			using namespace std::chrono_literals;
			std::this_thread::sleep_for( 10ms );
			std::this_thread::yield();
		}

		if ( e > 0 )
		{
			if ( e == SSL_ERROR_SYSCALL )
			{
				ERR_print_errors_fp( stdout );
				return false;
			}

			if ( e == SSL_ERROR_ZERO_RETURN )
				return false;

			util::print_log( "SSL error occured: %i (return code %i)\n", e, h );
			if ( !err_msg.empty() )
				ssl_print_error( err_msg );

			return false;
		}
	}

	if ( r )
		*r = h;

	log_debug_fmt( "IO SSL call finished, return code %i\n", h );
	return true;
}

bool ssl::has_data()
{
	char buf[ 1 ]; size_t r{};
	return do_async_call( SSL_peek, XOR( "Failed to peek into SSL socket" ), buf, 1, &r ) && r > 0;
}

void ssl::shutdown( bool sd )
{
	if ( obj )
	{
		if ( sd )
			do_async_call( SSL_shutdown );

		std::lock_guard _lock( obj_mtx );
		SSL_free( obj );
		obj = nullptr;
	}
}
