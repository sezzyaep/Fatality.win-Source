#include "../include_cheat.h"
#include "ssl_context.h"
#include "ssl.h"

using namespace crypto;

ssl_context::ssl_context()
{
	SSL_library_init();
	SSLeay_add_all_algorithms();
	SSL_load_error_strings();
}

ssl_context::~ssl_context()
{
	if ( ctx )
		SSL_CTX_free( ctx );

	EVP_cleanup();
}

bool ssl_context::create_server()
{
	if ( !create( TLSv1_2_server_method() ) )
		return false;

	config();

	if ( !SSL_CTX_use_certificate_file( ctx, cert.c_str(), SSL_FILETYPE_PEM ) )
	{
		ssl_print_error( XOR_STR( "Failed to set certificate" ) );
		return false;
	}

	if ( !SSL_CTX_use_PrivateKey_file( ctx, pk.c_str(), SSL_FILETYPE_PEM ) )
	{
		ssl_print_error( XOR_STR( "Failed to set private key" ) );
		return false;
	}

	return true;
}

bool ssl_context::create_client()
{
	if ( !create( TLSv1_2_client_method() ) )
		return false;

	config();
	return true;
}

bool ssl_context::create( const SSL_METHOD* method )
{
	ctx = SSL_CTX_new( method );
	if ( !ctx )
	{
		ssl_print_error( XOR_STR( "Failed to create context" ) );
		return false;
	}

	return true;
}

void ssl_context::config() const
{
	SSL_CTX_set_ecdh_auto( ctx, 1 );
	SSL_CTX_set_mode( ctx, SSL_MODE_AUTO_RETRY );
	SSL_CTX_set_mode( ctx, SSL_MODE_RELEASE_BUFFERS );
}
