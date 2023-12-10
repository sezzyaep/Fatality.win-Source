#include "../include_cheat.h"

bool lua::state::load_file( const char* name )
{
	const auto result = luaL_loadfile( l, name );
	if ( result )
	{
		handle_error( result );
		return false;
	}

	return true;
}

void lua::state::set_panic_func( c_function func )
{
	lua_atpanic( l, func );
}

bool lua::state::run()
{
	const auto result = lua_pcall( l, 0, 0, 0 );
	if ( result )
	{
		handle_error( result );
		return false;
	}

	return true;
}

bool lua::state::load_string( const std::string& str )
{
	const auto result = luaL_loadstring( l, str.c_str() );
	if ( result )
	{
		handle_error( result );
		return false;
	}

	return true;
}

void lua::state::handle_error( int result )
{
	switch ( result )
	{
		case LUA_ERRSYNTAX:
			// set the last error plus read error message from the stack
			last_error = se_syntax_error;
			last_error_description = lua_tostring( l, -1 );
			break;
		case LUA_ERRMEM:
			// that's unfortunate
			last_error = se_memory_error;
			break;
		case LUA_ERRRUN:
			// basically do the same as for syntax error
			last_error = se_runtime_error;
			last_error_description = lua_tostring( l, -1 );
			break;
		case LUA_ERRERR:
			last_error = se_error_handler_error;
			break;
		default:
			// most likely not implemented
			last_error = se_unknown;
			break;
	}

	// pop error from the stack
	lua_pop( l, 1 );
}

void lua::state::print_stack_dump()
{
	interfaces::cvar()->ConsoleColorPrintf( Color::White(), XOR_STR( "--- STACK TRACE DUMP ---\n" ) );
	for ( auto i = lua_gettop( l ); i >= 1; --i )
	{
		interfaces::cvar()->ConsoleColorPrintf( Color::White(), XOR_STR( "#%i:\ttypename %s => " ), i, lua_typename( l, lua_type( l, i ) ) );

		switch ( lua_type( l, i ) )
		{
			case LUA_TNUMBER:
				interfaces::cvar()->ConsoleColorPrintf( Color::White(), XOR_STR( "%g\n" ), lua_tonumber( l, i ) );
				break;
			case LUA_TSTRING:
				interfaces::cvar()->ConsoleColorPrintf( Color::White(), XOR_STR( "%s\n" ), lua_tostring( l, i ) );
				break;
			case LUA_TBOOLEAN:
				interfaces::cvar()->ConsoleColorPrintf( Color::White(), XOR_STR( "%s\n" ), lua_toboolean( l, i ) ? XOR_STR( "true" ) : XOR_STR( "false" ) );
				break;
			case LUA_TNIL:
				interfaces::cvar()->ConsoleColorPrintf( Color::White(), XOR_STR( "nil\n" ) );
				break;
			default:
				interfaces::cvar()->ConsoleColorPrintf( Color::White(), XOR_STR( "pointer %p\n" ), lua_topointer( l, i ) );
				break;
		}
	}

	interfaces::cvar()->ConsoleColorPrintf( Color::White(), XOR_STR( "--- END STACK TRACE DUMP ---\n" ) );
}

void lua::state::unset_global( const char* name )
{
	// set global to nil, basically "unsetting" it
	lua_pushnil( l );
	lua_setglobal( l, name );
}

void lua::state::set_global_function( const char* name, lua::c_function func )
{
	lua_pushcfunction( l, func );
	lua_setglobal( l, name );
}

void lua::state::error( const std::string& err )
{
	try
	{
		luaL_error( l, XOR_STR( "%s" ), err.c_str() );
	}
	catch ( ... ) {}
}

int lua::state::get_stack_top()
{
	return lua_gettop( l );
}

double lua::state::get_number( int pos )
{
	return lua_tonumber( l, pos );
}

float lua::state::get_float( int pos )
{
	return static_cast< float >( get_number( pos ) );
}

const char* lua::state::get_string( int pos )
{
	return lua_tostring( l, pos );
}

bool lua::state::get_boolean( int pos )
{
	return lua_toboolean( l, pos );
}

void lua::state::create_table()
{
	lua_newtable( l );
}

void lua::state::set_field( const char* name, bool v )
{
	lua_pushboolean( l, v );
	lua_setfield( l, -2, name );
}

void lua::state::set_field( const char* name, int v )
{
	lua_pushinteger( l, v );
	lua_setfield( l, -2, name );
}

void lua::state::set_field( const char* name, double v )
{
	lua_pushnumber( l, v );
	lua_setfield( l, -2, name );
}

void lua::state::set_global( const char* name )
{
	lua_setglobal( l, name );
}

void lua::state::set_field( const char* name )
{
	lua_setfield( l, -2, name );
}

void lua::state::set_field( const char* name, lua::c_function v )
{
	lua_pushcfunction( l, v );
	lua_setfield( l, -2, name );
}

void lua::state::set_field( const char* name, const char* v )
{
	lua_pushstring( l, v );
	lua_setfield( l, -2, name );
}

void lua::state::set_table( const char* v )
{
	lua_pushstring( l, v );
	lua_settable( l, -2 );
}

void lua::state::set_i( int n, bool v )
{
	lua_pushboolean( l, v );
	lua_rawseti( l, -2, n );
}

void lua::state::set_i( int n, int v )
{
	lua_pushinteger( l, v );
	lua_rawseti( l, -2, n );
}

void lua::state::set_i( int n, double v )
{
	lua_pushnumber( l, v );
	lua_rawseti( l, -2, n );
}

void lua::state::set_i( int n )
{
	lua_rawseti( l, -2, n );
}

void lua::state::set_i( int n, lua::c_function v )
{
	lua_pushcfunction( l, v );
	lua_rawseti( l, -2, n );
}

void lua::state::set_i( int n, const char* v )
{
	lua_pushstring( l, v );
	lua_rawseti( l, -2, n );
}

void lua::state::create_namespace( const char* name, const std::vector<function_desc>& funcs )
{
	create_table();

	for ( const auto& func : funcs )
		set_field( func.name, func.func );

	set_global( name );
}

void lua::state::extend_namespace( const char* name, const std::vector<function_desc>& funcs )
{
	get_global( name );

	for ( const auto& func : funcs )
		set_field( func.name, func.func );

	set_global( name );
}

void lua::state::create_type( const char* name, const std::vector<function_desc>& funcs )
{
	// point __index to ourselves
	luaL_newmetatable( l, name );
	lua_pushvalue( l, -1 );
	lua_setfield( l, -2, XOR_STR( "__index" ) );

	// register fields
	for ( const auto& func : funcs )
		set_field( func.name, func.func );

	// pop metatable from the stack
	lua_pop( l, 1 );
}

void lua::state::unset_in_table( const char* table, const char* name )
{
	// get global table
	lua_getglobal( l, table );

	// unset field
	lua_pushnil( l );
	lua_setfield( l, -2, name );

	// pop global table
	lua_pop( l, 1 );
}

void lua::state::push_nil()
{
	lua_pushnil( l );
}

void lua::state::push( int v )
{
	lua_pushinteger( l, v );
}

void lua::state::push( double v )
{
	lua_pushnumber( l, v );
}

void lua::state::push( const char* v )
{
	lua_pushstring( l, v );
}

void lua::state::push( const std::string& v )
{
	lua_pushlstring( l, v.c_str(), v.size() );
}

void lua::state::push( bool v )
{
	lua_pushboolean( l, v );
}

void lua::state::push( lua::c_function v )
{
	lua_pushcfunction( l, v );
}

void lua::state::push( void* v )
{
	lua_pushlightuserdata( l, v );
}

int lua::state::get_integer( int pos )
{
	return lua_tointeger( l, pos );
}

lua::c_function lua::state::get_c_function( int pos )
{
	return lua_tocfunction( l, pos );
}

void* lua::state::get_user_data( int pos )
{
	return lua_touserdata( l, pos );
}

void lua::state::get_global( const char* name )
{
	lua_getglobal( l, name );
}

bool lua::state::call( int args, int result )
{
	const auto e = lua_pcall( l, args, result, 0 );
	if ( e )
	{
		handle_error( e );
		return false;
	}

	return true;
}

void lua::state::pop( int n )
{
	lua_pop( l, n );
}

bool lua::state::is_function( int pos )
{
	return lua_isfunction( l, pos );
}

bool lua::state::is_number( int pos )
{
	return lua_isnumber( l, pos );
}

bool lua::state::is_string( int pos )
{
	return lua_isstring( l, pos );
}

bool lua::state::is_table( int pos )
{
	return lua_istable( l, pos );
}

bool lua::state::is_boolean( int pos )
{
	return lua_isboolean( l, pos );
}

bool lua::state::is_nil( int pos )
{
	return lua_isnil( l, pos );
}

void lua::state::set_integers_for_table( const char* name, const std::vector<std::pair<const char*, int>>& pairs )
{
	get_global( name );
	for ( const auto& [field, val] : pairs )
		set_field( field, val );
	pop( 1 );
}

int lua::state::get_field_integer( const char* name, int pos )
{
	lua_getfield( l, pos, name );
	const auto v = lua_tointeger( l, -1 );
	lua_pop( l, 1 );

	return v;
}

void lua::state::get_field( const char* name, int pos )
{
	lua_getfield( l, pos, name );
}

int lua::state::get_len( int pos )
{
	return lua_objlen( l, pos );
}

std::vector<std::string> lua::state::get_array_string( int pos )
{
	const auto len = get_len( pos );

	std::vector<std::string> ret;

	for ( int i = 0; i < len; i++ )
	{
		lua_pushinteger( l, i + 1 );
		lua_gettable( l, pos );

		auto v = std::string();
		if ( lua_isstring( l, -1 ) )
			v = lua_tostring( l, -1 );

		ret.emplace_back( v );

		lua_pop( l, 1 );
	}

	return ret;
}

std::vector<int> lua::state::table_to_int_array( int pos )
{
	std::vector<int> arr;

	lua_pushnil( l );
	while ( lua_next( l, pos ) != 0 )
	{
		arr.emplace_back( lua_tointeger( l, -1 ) );
		lua_pop( l, 1 );
	}

	return arr;
}

std::vector<std::string> lua::state::table_to_string_array( int pos )
{
	std::vector<std::string> arr;

	lua_pushnil( l );
	while ( lua_next( l, pos ) != 0 )
	{
		arr.emplace_back( lua_tostring( l, -1 ) );
		lua_pop( l, 1 );
	}

	return arr;
}

std::vector<void*> lua::state::table_to_user_data_array( int pos )
{
	std::vector<void*> arr;

	lua_pushnil( l );
	while ( lua_next( l, pos ) != 0 )
	{
		arr.emplace_back( lua_touserdata( l, -1 ) );
		lua_pop( l, 1 );
	}

	return arr;
}

void lua::state::create_user_object_internal( const char* mt_name, void* val, size_t size )
{
	// create object
	auto data = lua_newuserdata( l, size );
	memcpy( data, val, size );

	// set object's metatable
	luaL_getmetatable( l, mt_name );
	lua_setmetatable( l, -2 );
}

std::string lua::state::get_as_string( int pos )
{
	size_t sz{};
	const auto str = lua_tolstring( l, pos, &sz );

	return sz && str ? str : XOR_STR( "" );
}

void lua::state::create_metatable( const char* name )
{
	luaL_newmetatable( l, name );
}

void lua::state::set_metatable()
{
	lua_setmetatable( l, -2 );
}

int lua::state::registry_add()
{
	return luaL_ref( l, LUA_REGISTRYINDEX );
}

void lua::state::registry_get( int ref )
{
	lua_rawgeti( l, LUA_REGISTRYINDEX, ref );
}

void lua::state::registry_remove( int ref )
{
	luaL_unref( l, LUA_REGISTRYINDEX, ref );
}
