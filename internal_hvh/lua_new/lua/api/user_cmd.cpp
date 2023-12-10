#include "../include_cheat.h"

int lua::api_def::user_cmd::get_command_number( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( { { ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_command_number()" ) );
		return 0;
	}

	const auto cmd = *reinterpret_cast< CUserCmd** >( s.get_user_data( 1 ) );
	s.push( cmd->command_number );

	return 1;
}

int lua::api_def::user_cmd::get_view_angles( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( { { ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_view_angles()" ) );
		return 0;
	}

	const auto cmd = *reinterpret_cast< CUserCmd** >( s.get_user_data( 1 ) );
	s.push( cmd->viewangles.x );
	s.push( cmd->viewangles.y );
	s.push( cmd->viewangles.z );

	return 3;
}

int lua::api_def::user_cmd::get_move( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( { { ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_move()" ) );
		return 0;
	}

	const auto cmd = *reinterpret_cast< CUserCmd** >( s.get_user_data( 1 ) );
	s.push( cmd->forwardmove );
	s.push( cmd->sidemove );

	return 2;
}

int lua::api_def::user_cmd::get_buttons( lua_State* l )
{
	runtime_state s( l );
	if ( !s.check_arguments( { { ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_buttons()" ) );
		return 0;
	}

	const auto cmd = *reinterpret_cast< CUserCmd** >( s.get_user_data( 1 ) );
	s.push( cmd->buttons );

	return 1;
}

int lua::api_def::user_cmd::set_view_angles( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::user_data },
		{ ltd::number },
		{ ltd::number },
		{ ltd::number },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: obj:set_view_angles(x, y, z)" ) );
		return 0;
	}

	const auto cmd = *reinterpret_cast< CUserCmd** >( s.get_user_data( 1 ) );
	cmd->viewangles = QAngle( s.get_float( 2 ), s.get_float( 3 ), s.get_float( 4 ) );

	return 0;
}

int lua::api_def::user_cmd::set_move( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::user_data },
		{ ltd::number },
		{ ltd::number },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: obj:set_move(forward_move, side_move)" ) );
		return 0;
	}

	const auto cmd = *reinterpret_cast< CUserCmd** >( s.get_user_data( 1 ) );
	cmd->forwardmove = s.get_float( 2 );
	cmd->sidemove = s.get_float( 3 );

	return 0;
}

int lua::api_def::user_cmd::set_buttons( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::user_data },
		{ ltd::number },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: obj:set_buttons(buttons)" ) );
		return 0;
	}

	const auto cmd = *reinterpret_cast< CUserCmd** >( s.get_user_data( 1 ) );
	cmd->buttons = s.get_integer( 2 );

	return 0;
}