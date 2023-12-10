#include "../include_cheat.h"

int lua::api_def::input::is_key_down( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::number }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: is_key_down(key)" ) );
		return 0;
	}

	s.push( g_input->get_key( s.get_integer( 1 ) ).pressed || g_input->get_key( s.get_integer( 1 ) ).held );
	return 1;
}

int lua::api_def::input::get_cursor_pos( lua_State* l )
{
	runtime_state s( l );

	const auto pos = g_input->get_mouse().pos;
	s.push( static_cast< int >( pos.x ) );
	s.push( static_cast< int >( pos.y ) );

	return 2;
}