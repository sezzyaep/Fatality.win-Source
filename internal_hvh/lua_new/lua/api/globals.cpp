#include "../include_cheat.h"

int lua::api_def::panic( lua_State* l )
{
	runtime_state s( l );

	imp( "user32.dll", MessageBoxA )( nullptr, s.get_string( -1 ), XOR_STR( "Critical error" ), MB_OK | MB_ICONWARNING | MB_SYSTEMMODAL );
	return 0;
}

int lua::api_def::print( lua_State* l )
{
	runtime_state s( l );
	if ( !s.get_stack_top() )
	{
		s.error( XOR_STR( "print() requires at least one argument" ) );
		return 0;
	}

	std::string output{};

	auto n = 1;
	do
	{
		if ( s.is_boolean( n ) )
			output += s.get_boolean( n ) ? XOR_STR( "true" ) : XOR_STR( "false" );
		else if ( s.is_string( n ) )
			output += s.get_string( n );
		else if ( s.is_number( n ) )
			output += std::to_string( s.get_number( n ) );
	} while ( ++n <= s.get_stack_top() );

	size_t last_pos{};
	while ( ( last_pos = output.find( XOR_STR( "%" ), last_pos ) ) != std::string::npos )
	{
		output = output.replace( last_pos, 1, XOR_STR( "%%" ) );
		last_pos += 2;
	}

	lua::helpers::print( output.c_str() );
	return 0;
}

int lua::api_def::require( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: require(lib)" ) );
		return 0;
	}

	const auto me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "sandbox problem" ) );
		return 0;
	}

	// attempt loading lib
	script_file file{ st_library, s.get_string( 1 ) };

	s.load_file( file.get_file_path().c_str() );
	if ( !s.call( 0, 1 ) )
		s.error( s.get_last_error_description() );

	return 1;
}

int lua::api_def::loadfile( lua_State* l )
{
	runtime_state s( l );

	if ( !vars::lua.allow_insecure->get<bool>() )
	{
		s.error( XOR_STR( "allow insecure is disabled" ) );
		return 0;
	}

	const auto r = s.check_arguments( {
		{ ltd::string }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: loadfile(path)" ) );
		return 0;
	}

	const auto me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "sandbox problem" ) );
		return 0;
	}

	// check if it is a proprietary script
	std::string content;
#ifdef CSGO_SECURE
	if ( me->remote.is_proprietary )
		content = network::decrypt_script( s.get_string( 1 ), me->remote.id, me->remote.last_update );
	else
#endif
	{
		// read from file
		std::ifstream file( s.get_string( 1 ) );
		if ( !file.is_open() )
		{
			s.error( XOR_STR( "file not found" ) );
			return 0;
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		content = buffer.str();
	}

	if ( !s.load_string( content ) )
	{
		s.error( s.get_last_error_description() );
		return 0;
	}

	return 1;
}

int lua::api_def::stub_new_index( lua_State* l )
{
	runtime_state s( l );
	s.error( XOR_STR( "access violation: overriding fields is forbidden" ) );

	return 0;
}

int lua::api_def::stub_index( lua_State* l )
{
	runtime_state s( l );
	return 0;
}

int lua::api_def::server_index( lua_State* l )
{
	if ( !interfaces::engine()->IsInGame() )
		return 0;

	runtime_state s( l );

	const auto key = fnv1a_rt( s.get_as_string( 2 ).c_str() );
	switch ( key )
	{
		case FNV1A( "map_name" ):
			s.push( interfaces::client_state()->m_szLevelName );
			return 1;
		case FNV1A( "address" ):
			s.push( interfaces::client_state()->m_NetChannel->remote_address );
			return 1;
		case FNV1A( "max_players" ):
			s.push( interfaces::globals()->maxClients );
			return 1;
		default: return 0;
	}
}

int lua::api_def::fatality_index( lua_State* l )
{
	runtime_state s( l );

	const auto key = fnv1a_rt( s.get_as_string( 2 ).c_str() );
	switch ( key )
	{
		//TODO: implement username
		case FNV1A( "username" ):
			s.push( globals::user_info.name );
			return 1;
		case FNV1A( "lag_ticks" ):
			s.push( interfaces::client_state()->chokedcommands );
			return 1;
		case FNV1A( "to_lag" ):
			s.push( antiaim::to_lag );
			return 1;
		case FNV1A( "can_fastfire" ):
			if ( interfaces::engine()->IsInGame() && local_player && local_player->get_alive() )
				s.push( tickbase::fast_fire && tickbase::compute_current_limit() > 0 );
			else
				s.push( 0 );
			return 1;
		case FNV1A( "in_fakeduck" ):
			s.push( vars::aim.fake_duck->get<bool>() );
			return 1;
		case FNV1A( "in_slowwalk" ):
			s.push( vars::aim.slide->get<bool>() );
			return 1;
		case FNV1A( "allow_insecure" ):
			s.push( vars::lua.allow_insecure->get<bool>() );
			return 1;
		case FNV1A( "desync" ):
			s.push( clamp( animations::local_record.m_yaw_modifier, 0.f, 1.f ) );
			return 1;
		case FNV1A( "shot_command" ):
			s.push( globals::shot_command );
			return 1;
		case FNV1A( "run_antiaim" ):
			s.push( antiaim::run_antiaim );
			return 1;
		default: return 0;
	}
}

int lua::api_def::global_vars_index( lua_State* l )
{
	runtime_state s( l );

	const auto key = fnv1a_rt( s.get_as_string( 2 ).c_str() );
	switch ( key )
	{
		case FNV1A( "realtime" ):
			s.push( interfaces::globals()->realtime );
			return 1;
		case FNV1A( "framecount" ):
			s.push( interfaces::globals()->framecount );
			return 1;
		case FNV1A( "curtime" ):
			s.push( interfaces::globals()->curtime );
			return 1;
		case FNV1A( "frametime" ):
			s.push( interfaces::globals()->frametime );
			return 1;
		case FNV1A( "tickcount" ):
			s.push( interfaces::globals()->tickcount );
			return 1;
		case FNV1A( "interval_per_tick" ):
			s.push( interfaces::globals()->interval_per_tick );
			return 1;
		default: return 0;
	}
}

int lua::api_def::game_rules_index( lua_State* l )
{
	runtime_state s( l );

	if ( !*interfaces::game_rules_proxy() )
		return 0;

	const auto key = fnv1a_rt( s.get_as_string( 2 ).c_str() );
	switch ( key )
	{
		case FNV1A( "is_valve_server" ):
			s.push( ( *interfaces::game_rules_proxy() )->m_bIsValveDS() );
			return 1;
		case FNV1A( "is_freeze_period" ):
			s.push( ( *interfaces::game_rules_proxy() )->m_bFreezePeriod() );
			return 1;
		default: return 0;
	}
}