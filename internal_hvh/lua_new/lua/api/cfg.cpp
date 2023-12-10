#include "../include_cheat.h"

// get_int(): int
int lua::api_def::cfg_value::get_int( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( { { ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_int(): int" ) );
		return 0;
	}

	const auto cfg_item = s.user_data_ptr<value>( 1 );
	s.push( cfg_item->get_int() );
	return 1;
}

// get_bool(): bool
int lua::api_def::cfg_value::get_bool( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( { { ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_bool(): bool" ) );
		return 0;
	}

	const auto cfg_item = s.user_data_ptr<value>( 1 );
	s.push( cfg_item->get_bool() );
	return 1;
}

// get_float(): number
int lua::api_def::cfg_value::get_float( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( { { ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_float(): number" ) );
		return 0;
	}

	const auto cfg_item = s.user_data_ptr<value>( 1 );
	s.push( cfg_item->get_float() );
	return 1;
}

// get_color(): color
int lua::api_def::cfg_value::get_color( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( { { ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_color(): color" ) );
		return 0;
	}

	const auto cfg_item = s.user_data_ptr<value>( 1 );

	s.create_table();
	s.set_field( XOR( "r" ), cfg_item->get_color().r() );
	s.set_field( XOR( "g" ), cfg_item->get_color().g() );
	s.set_field( XOR( "b" ), cfg_item->get_color().b() );
	s.set_field( XOR( "a" ), cfg_item->get_color().a() );
	return 1;
}

// get_string(): string
int lua::api_def::cfg_value::get_string( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( { { ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_string(): string" ) );
		return 0;
	}

	const auto cfg_item = s.user_data_ptr<value>( 1 );
	s.push( cfg_item->get_string().c_str() );
	return 1;
}

// set_int(int)
int lua::api_def::cfg_value::set_int( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( {
		{ ltd::user_data },
		{ ltd::number } } ) )
	{
		s.error( XOR_STR( "usage: obj:set_int(int)" ) );
		return 0;
	}

	const auto cfg_item = s.user_data_ptr<value>( 1 );
	cfg_item->set_int( s.get_integer( 2 ) );
	return 0;
}

// set_bool(bool)
int lua::api_def::cfg_value::set_bool( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( {
		{ ltd::user_data },
		{ ltd::boolean } } ) )
	{
		s.error( XOR_STR( "usage: obj:set_bool(bool)" ) );
		return 0;
	}

	const auto cfg_item = s.user_data_ptr<value>( 1 );
	cfg_item->set_bool( s.get_boolean( 2 ) );
	return 0;
}

// set_float(number)
int lua::api_def::cfg_value::set_float( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( {
		{ ltd::user_data },
		{ ltd::number } } ) )
	{
		s.error( XOR_STR( "usage: obj:set_float(number)" ) );
		return 0;
	}

	const auto cfg_item = s.user_data_ptr<value>( 1 );
	cfg_item->set_float( s.get_float( 2 ) );
	return 0;
}

// set_color(color)
int lua::api_def::cfg_value::set_color( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( {
		{ ltd::user_data },
		{ ltd::table } } ) )
	{
		s.error( XOR_STR( "usage: obj:set_color(color)" ) );
		return 0;
	}

	const auto cfg_item = s.user_data_ptr<value>( 1 );
	cfg_item->set_color( color( s.get_field_integer( XOR_STR( "r" ), 2 ),
		s.get_field_integer( XOR_STR( "g" ), 2 ),
		s.get_field_integer( XOR_STR( "b" ), 2 ),
		s.get_field_integer( XOR_STR( "a" ), 2 ) ) );

	return 0;
}

// set_string(string)
int lua::api_def::cfg_value::set_string( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( {
		{ ltd::user_data },
		{ ltd::string } } ) )
	{
		s.error( XOR_STR( "usage: obj:set_string(string)" ) );
		return 0;
	}

	const auto cfg_item = s.user_data_ptr<value>( 1 );
	cfg_item->set_string( s.get_string( 2 ) );
	return 0;
}
