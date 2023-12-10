#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <functional>
#include <mutex>

#include "ssl_context.h"

namespace crypto
{
	void ssl_print_error( const std::string& message );

	class ssl
	{
	public:
		ssl( const std::shared_ptr<ssl_context>& ctx, uintptr_t sock );
		~ssl();

		void shutdown( bool sd );

		bool accept();
		bool connect();
		bool send( unsigned char* buf, size_t len, size_t* r = nullptr );
		bool recv( char* buf, size_t len, size_t* r = nullptr );
		bool has_data();

		bool is_valid() const { return obj != nullptr; }

	public:
		bool do_async_call( const std::function<int( SSL* )> fn, const std::string& err_msg = "", size_t* r = nullptr );
		bool do_async_call( const std::function<int( SSL*, void*, int )> fn, const std::string& err_msg, void* buf, size_t len, size_t* r = nullptr );

		std::mutex obj_mtx{};
		SSL* obj{};
	};
}
