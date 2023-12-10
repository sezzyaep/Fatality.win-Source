
#ifndef LUA_STATE_H
#define LUA_STATE_H

#include <lua.hpp>
#include <string>
#include <vector>

namespace lua
{
	enum state_error
	{
		se_ok,
		se_syntax_error,
		se_memory_error,
		se_runtime_error,
		se_error_handler_error,
		se_unknown
	};

	enum class lua_type_def
	{
		none = LUA_TNONE,
		nil = LUA_TNIL,
		boolean = LUA_TBOOLEAN,
		light_user_data = LUA_TLIGHTUSERDATA,
		number = LUA_TNUMBER,
		string = LUA_TSTRING,
		table = LUA_TTABLE,
		function = LUA_TFUNCTION,
		user_data = LUA_TUSERDATA,
		thread = LUA_TTHREAD
	};

	using ltd = lua_type_def;
	using c_function = lua_CFunction;

	class argument_desc
	{
	public:
		lua_type_def expected_type{};
		bool is_optional{};
	};

	class function_desc
	{
	public:
		const char* name{};
		c_function func{};
	};

	class state
	{
	public:
		state( lua_State* s = nullptr )
		{
			// prevent issues with runtime_state
			if ( s )
			{
				l = s;
				return;
			}

			l = luaL_newstate();

			// only load what we want
			luaopen_base( l );
			luaopen_bit( l );
			luaopen_math( l );
			luaopen_string( l );
			luaopen_table( l );

			// special way of loading ffi
			luaopen_ffi( l );
			lua_setglobal( l, "ffi" );
		}

		~state()
		{
			if ( should_destroy_state )
				lua_close( l );
		}

		lua_State* get_state()
		{
			return l;
		}

		void set_panic_func( c_function func );

		// loaders
		bool load_file( const char* name );
		bool load_string( const std::string& str );

		// runtime
		bool run();
		void error( const std::string& err );
		bool call( int args, int result );

		// global
		void set_global_function( const char* name, c_function func );
		void set_global( const char* name );
		void get_global( const char* name );

		// metatables
		void create_metatable( const char* name );
		void set_metatable();

		// hiding util
		void unset_global( const char* name );
		void unset_in_table( const char* table, const char* name );

		// tables
		void create_table();
		void set_field( const char* name );
		void set_field( const char* name, int v );
		void set_field( const char* name, bool v );
		void set_field( const char* name, double v );
		void set_field( const char* name, c_function v );
		void set_field( const char* name, const char* v );

		void set_table( const char* v );

		void set_i( int n );
		void set_i( int n, int v );
		void set_i( int n, bool v );
		void set_i( int n, double v );
		void set_i( int n, c_function v );
		void set_i( int n, const char* v );

		int get_field_integer( const char* name, int pos = -2 );
		void get_field( const char* name, int pos = -2 );
		int get_len( int pos = -2 );

		std::vector<int> table_to_int_array( int pos );
		std::vector<std::string> table_to_string_array( int pos );
		std::vector<void*> table_to_user_data_array( int pos );

		template<typename T>
		std::vector<T> table_to_object_array( int pos )
		{
			std::vector<T> ret;
			for ( const auto& v : table_to_user_data_array( pos ) )
				ret.emplace_back( *( T* ) v );
			return ret;
		}

		std::vector<std::string> get_array_string( int pos );

		// registry
		int registry_add();
		void registry_get( int ref );
		void registry_remove( int ref );

		// debug
		void print_stack_dump();

		// stack
		int get_stack_top();

		bool is_function( int pos );
		bool is_number( int pos );
		bool is_boolean( int pos );
		bool is_string( int pos );
		bool is_table( int pos );
		bool is_nil( int pos );

		int get_integer( int pos );
		double get_number( int pos );
		float get_float( int pos );
		const char* get_string( int pos );
		bool get_boolean( int pos );
		c_function get_c_function( int pos );
		void* get_user_data( int pos );
		std::string get_as_string( int pos );

		template<typename T>
		T& user_data( int pos )
		{
			return *( T* ) get_user_data( pos );
		}

		template<typename T>
		T* user_data_ptr( int pos )
		{
			return *( T** ) get_user_data( pos );
		}

		void push( int v );
		void push( double v );
		void push( const char* v );
		void push( const std::string& v );
		void push( bool v );
		void push( c_function v );
		void push( void* v );
		void push_nil();

		void pop( int n );

		// util
		void create_namespace( const char* name, const std::vector<function_desc>& funcs );
		void extend_namespace( const char* name, const std::vector<function_desc>& funcs );
		void create_type( const char* name, const std::vector<function_desc>& funcs );

		void set_integers_for_table( const char* name, const std::vector<std::pair<const char*, int>>& pairs );

		// user types
		template<typename T, typename... Args>
		T* create_user_object_managed( const char* metatable_name, Args... args )
		{
			auto ptr = ( T** ) lua_newuserdata( l, sizeof( T ) );
			*ptr = new T( args... );

			luaL_getmetatable( l, metatable_name );
			lua_setmetatable( l, -2 );

			return *ptr;
		}

		template<typename T>
		void create_user_object( const char* metatable_name, T* val )
		{
			create_user_object_internal( metatable_name, val, sizeof( T ) );
		}

		template<typename T>
		void create_user_object_ptr( const char* metatable_name, T* val )
		{
			auto v = &val;
			create_user_object_internal( metatable_name, v, sizeof( T* ) );
		}

		// errors
		[[nodiscard]]
		state_error get_last_error() const
		{
			return last_error;
		}

		[[nodiscard]]
		std::string get_last_error_description() const
		{
			return last_error_description;
		}

	protected:
		void create_user_object_internal( const char* mt_name, void* val, size_t size );
		void handle_error( int result );

		state_error last_error{ se_ok };
		std::string last_error_description{};

		bool should_destroy_state{ true };
		lua_State* l{};
	};

	class runtime_state : public state
	{
	public:
		explicit runtime_state( lua_State* s ) : state( s )
		{
			should_destroy_state = false;
		}

		// arguments
		bool check_arguments( const std::vector<argument_desc>& desc, bool relaxed = false );
		bool check_argument_type( int pos, lua_type_def expected_type );
	};
}

#endif // LUA_STATE_H
