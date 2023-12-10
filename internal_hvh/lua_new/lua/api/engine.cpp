#include "../include_cheat.h"

int lua::api_def::engine::is_in_game( lua_State* l )
{
	runtime_state s( l );
	s.push( interfaces::engine()->IsInGame() );

	return 1;
}

int lua::api_def::engine::get_local_player( lua_State* l )
{
	runtime_state s( l );
	s.push( static_cast< int >( interfaces::engine()->GetLocalPlayer() ) );

	return 1;
}

int lua::api_def::engine::exec( lua_State* l )
{
	using namespace helpers;
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: exec(cmd)" ) );
		return 0;
	}

	// check if any poor commands were used
	for ( const auto& cv : util::parse_cvars( s.get_string( 1 ) ) )
	{
		ConCommand cmd( cv.c_str() );

		const auto hash = fnv1a_rt( cmd.m_pArgvBuffer );
		if ( std::find( blocked_vars.begin(), blocked_vars.end(), hash ) != blocked_vars.end() )
		{
			s.error( XOR_STR( "cannot execute " ) + cv + XOR_STR( " because it is restricted" ) );
			return 0;
		}

		if ( std::find( risky_vars.begin(), risky_vars.end(), hash ) != risky_vars.end() )
		{
			if ( !vars::lua.allow_insecure->get<bool>() )
			{
				s.error( XOR_STR( "cannot execute " ) + cv + XOR_STR( " because it is restricted. Enable 'Allow unsafe scripts' to allow this command" ) );
				return 0;
			}
		}
	}

	original( hook::client_cmd_unrestricted )( interfaces::engine()( ), 0, s.get_string( 1 ), nullptr );

	return 0;
}

int lua::api_def::engine::get_view_angles( lua_State* l )
{
	runtime_state s( l );

	QAngle va;
	interfaces::engine()->GetViewAngles( va );
	s.push( va.x );
	s.push( va.y );
	s.push( va.z );

	return 3;
}

int lua::api_def::engine::get_player_for_user_id( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::number }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: get_player_for_user_id(user_id)" ) );
		return 0;
	}

	s.push( interfaces::engine()->GetPlayerForUserID( s.get_integer( 1 ) ) );
	return 1;
}

int lua::api_def::engine::get_player_info( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::number }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: get_player_info(id)" ) );
		return 0;
	}

	player_info_t info{};
	interfaces::engine()->GetPlayerInfo( s.get_integer( 1 ), &info );

	s.create_table();
	{
		s.set_field( XOR( "name" ), info.name );
		s.set_field( XOR( "user_id" ), info.userid );
		s.set_field( XOR( "steam_id" ), info.guid );
		s.set_field( XOR( "steam_id64" ), std::to_string( info.xuid ).c_str() );
		s.set_field( XOR( "steam_id64_low" ), info.xuidlow );
		s.set_field( XOR( "steam_id64_high" ), info.xuidhigh );
	}

	return 1;
}

int lua::api_def::engine::set_view_angles( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::number },
		{ ltd::number },
	}, true ); // can be relaxed so possible to do something like cmd:get_view_angles() as argument

	if ( !r )
	{
		s.error( XOR_STR( "usage: set_view_angles(x, y)" ) );
		return 0;
	}

	QAngle ang{ s.get_float( 1 ), s.get_float( 2 ), 0.f };
	interfaces::engine()->SetViewAngles( ang );
	return 0;
}