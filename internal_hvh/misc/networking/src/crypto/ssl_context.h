#pragma once

#include <openssl/ssl.h>
#include <string>

namespace crypto
{
	// should be instantinated only once
	class ssl_context
	{
	public:
		ssl_context();
		~ssl_context();

		void set_certificate( const std::string& path )
		{
			cert = path;
		}

		void set_private_key( const std::string& path )
		{
			pk = path;
		}

		bool create_server();
		bool create_client();

		SSL_CTX* get()
		{
			return ctx;
		}

	private:
		bool create( const SSL_METHOD* method );
		void config() const;

		SSL_CTX* ctx{};

		std::string cert{};
		std::string pk{};
	};
}
