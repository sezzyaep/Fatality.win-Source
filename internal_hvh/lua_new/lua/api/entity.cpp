#include "../include_cheat.h"

int lua::api_def::entities::index( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	if ( s.is_number( 2 ) )
	{
		const auto key = s.get_integer( 2 );

		auto ent = interfaces::entity_list()->get_client_entity( key );

		if ( !ent )
			return 0;

		s.create_user_object_ptr( XOR_STR( "csgo.entity" ), ent );

		return 1;
	}

	return 0;
}

int lua::api_def::entities::get_entity( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	if ( !s.is_number( 1 ) )
	{
		s.error( XOR_STR( "usage: entities.get_entity(number)" ) );
		return 0;
	}

	auto ent = interfaces::entity_list()->get_client_entity( s.get_integer( 1 ) );

	if ( !ent )
		return 0;

	s.create_user_object_ptr( XOR_STR( "csgo.entity" ), ent );

	return 1;
}

int lua::api_def::entities::get_entity_from_handle( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	if ( !s.is_number( 1 ) )
	{
		s.error( XOR_STR( "usage: entities.get_entity_from_handle(number)" ) );
		return 0;
	}

	auto ent = interfaces::entity_list()->get_client_entity_from_handle( s.get_integer( 1 ) );

	if ( !ent )
		return 0;

	s.create_user_object_ptr( XOR_STR( "csgo.entity" ), ent );

	return 1;
}

int lua::api_def::entities::for_each( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	if ( !s.is_function( 1 ) )
	{
		s.error( XOR_STR( "usage: entities.for_each(function)" ) );
		return 0;
	}

	const auto me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	const auto func_ref = s.registry_add();

	interfaces::entity_list()->for_each( [&] ( C_BaseEntity* ent )
	{
		s.registry_get( func_ref );
		s.create_user_object_ptr( XOR_STR( "csgo.entity" ), ent );
		if ( !s.call( 1, 0 ) )
		{
			me->did_error = true;
			lua::helpers::error( XOR_STR( "runtime_error" ), s.get_last_error_description().c_str() );
			if ( const auto f = api.find_script_file( me->id ); f.has_value() )
				f->get().should_unload = true;

			return;
		}
	} );

	s.registry_remove( func_ref );

	return 0;
}

int lua::api_def::entities::for_each_z( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	if ( !s.is_function( 1 ) )
	{
		s.error( XOR_STR( "usage: entities.for_each_z(function)" ) );
		return 0;
	}

	const auto me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	const auto func_ref = s.registry_add();

	interfaces::entity_list()->for_each_z( [&] ( C_BaseEntity* ent )
	{
		s.registry_get( func_ref );
		s.create_user_object_ptr( XOR_STR( "csgo.entity" ), ent );
		if ( !s.call( 1, 0 ) )
		{
			me->did_error = true;
			lua::helpers::error( XOR_STR( "runtime_error" ), s.get_last_error_description().c_str() );
			if ( const auto f = api.find_script_file( me->id ); f.has_value() )
				f->get().should_unload = true;

			return;
		}
	} );

	s.registry_remove( func_ref );

	return 0;
}

int lua::api_def::entities::for_each_player( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	if ( !s.is_function( 1 ) )
	{
		s.error( XOR_STR( "usage: entities.for_each_player(function)" ) );
		return 0;
	}

	const auto me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	const auto func_ref = s.registry_add();

	interfaces::entity_list()->for_each_player( [&] ( C_CSPlayer* ent )
	{
		s.registry_get( func_ref );
		s.create_user_object_ptr( XOR_STR( "csgo.entity" ), ent );

		if ( !s.call( 1, 0 ) )
		{
			me->did_error = true;
			lua::helpers::error( XOR_STR( "runtime_error" ), s.get_last_error_description().c_str() );
			if ( const auto f = api.find_script_file( me->id ); f.has_value() )
				f->get().should_unload = true;

			return;
		}
	} );

	s.registry_remove( func_ref );

	return 0;
}

int lua::api_def::entities::for_each_player_z( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	if ( !s.is_function( 1 ) )
	{
		s.error( XOR_STR( "usage: entities.for_each_player_z(function)" ) );
		return 0;
	}

	auto func_ref = s.registry_add();

	interfaces::entity_list()->for_each_player_z( [&] ( C_CSPlayer* ent )
	{
		s.registry_get( func_ref );
		s.create_user_object_ptr( XOR_STR( "csgo.entity" ), ent );

		if ( !s.call( 1, 0 ) )
			s.error( s.get_last_error_description() );
	} );

	s.registry_remove( func_ref );

	return 0;
}

bool lua::api_def::entity::is_sane( C_BaseEntity* ent )
{
	return std::find( interfaces::entity_list()->begin(), interfaces::entity_list()->end(), ent ) != interfaces::entity_list()->end();
}

int lua::api_def::entity::get_index( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_BaseEntity>( s, XOR_STR( "usage: ent:get_index()" ) );

	if ( !ent || !is_sane( ent ) )
		return 0;

	s.push( ent->EntIndex() );
	return 1;
}

int lua::api_def::entity::is_valid( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_CSPlayer>( s, XOR_STR( "usage: ent:is_valid()" ) );

	if ( !ent || !is_sane( ent ) )
		return 0;

	s.push( ent->is_player() && ent->is_valid() );

	return 1;
}

int lua::api_def::entity::is_alive( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_CSPlayer>( s, XOR_STR( "usage: ent:is_alive()" ) );

	if ( !ent || !is_sane( ent ) )
		return 0;

	s.push( ent->is_player() && ent->get_alive() );

	return 1;
}

int lua::api_def::entity::is_dormant( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_CSPlayer>( s, XOR_STR( "usage: ent:is_dormant()" ) );

	if ( !ent || !is_sane( ent ) )
		return 0;

	s.push( ent->IsDormant() );

	return 1;
}

int lua::api_def::entity::is_player( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_CSPlayer>( s, XOR_STR( "usage: ent:is_player()" ) );

	if ( !ent || !is_sane( ent ) )
		return 0;

	s.push( ent->is_player() );

	return 1;
}

int lua::api_def::entity::is_enemy( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_CSPlayer>( s, XOR_STR( "usage: ent:is_enemy()" ) );

	if ( !ent || !is_sane( ent ) )
		return 0;

	s.push( ent->is_player() && ent->is_enemy() );

	return 1;
}

int lua::api_def::entity::get_class( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_CSPlayer>( s, XOR_STR( "usage: ent:get_class()" ) );

	if ( !ent || !is_sane( ent ) )
		return 0;

	s.push( ent->GetClientClass()->m_pNetworkName );

	return 1;
}

int lua::api_def::entity::get_hitbox_position( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	if ( !s.check_arguments( {
		{ ltd::user_data },
		{ ltd::number } } ) )
	{
		s.error( XOR_STR( "usage: ent:get_hitbox_position(number)" ) );
		return 0;
	}

	const auto ent = *reinterpret_cast< C_CSPlayer** >( s.get_user_data( 1 ) );

	if ( !ent || !is_sane( ent ) )
		return 0;


	const auto hitbox_pos = ent->get_hitbox_pos( s.get_integer( 2 ) );
	if ( hitbox_pos.IsZero() || !hitbox_pos.IsValid() )
		return 0;

	s.push( hitbox_pos.x );
	s.push( hitbox_pos.y );
	s.push( hitbox_pos.z );

	return 3;
}

int lua::api_def::entity::get_eye_position( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_CSPlayer>( s, XOR_STR( "usage: ent:get_eye_position()" ) );

	if ( !ent || !is_sane( ent ) )
		return 0;

	const auto eye_pos = ent->get_eye_pos();

	s.push( eye_pos.x );
	s.push( eye_pos.y );
	s.push( eye_pos.z );

	return 3;
}

int lua::api_def::entity::get_player_info( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_CSPlayer>( s, XOR_STR( "usage: ent:get_player_info()" ) );

	if ( !ent || !is_sane( ent ) || !ent->is_player() )
		return 0;

	const auto player_info = ent->get_player_info();

	s.create_table();
	{
		s.set_field( XOR( "name" ), player_info.name );
		s.set_field( XOR( "user_id" ), player_info.userid );
		s.set_field( XOR( "steam_id" ), player_info.guid );
		s.set_field( XOR( "steam_id64" ), std::to_string( player_info.xuid ).c_str() );
		s.set_field( XOR( "steam_id64_low" ), player_info.xuidlow );
		s.set_field( XOR( "steam_id64_high" ), player_info.xuidhigh );
	}

	return 1;
}

int lua::api_def::entity::get_prop( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	if ( !s.check_arguments( {
		{ ltd::user_data },
		{ ltd::string },
		{ ltd::number, true } } ) )
	{
		s.error( XOR_STR( "usage: ent:get_prop(name, index (optional))" ) );
		return 0;
	}

	auto index = 0;

	// 3 IS 3!
	if ( s.is_number( 3 ) )
		index = s.get_integer( 3 );

	auto ent = *reinterpret_cast< C_BaseEntity** >( s.get_user_data( 1 ) );

	if ( !ent || !is_sane( ent ) )
		return 0;

	const auto client_class = ent->GetClientClass();
	if ( fnv1a_rt( client_class->m_pNetworkName ) == FNV1A( "CCSGameRulesProxy" ) )
		ent = interfaces::game_rules_proxy() ? reinterpret_cast< C_BaseEntity* >( *interfaces::game_rules_proxy() ) : nullptr;

	auto prop_name = std::string( s.get_string( 2 ) );
	const auto name_hash = fnv1a_rt( prop_name.c_str() );

	const auto ref = helpers::get_netvar( prop_name.c_str(), client_class->m_pRecvTable );

	if ( !ref.offset )
		return 0;

	const auto addr = uint32_t( ent ) + ref.offset;
	switch ( ref.type )
	{
		default:
		case send_prop_type::dpt_int:
			if ( prop_name.find( XOR_STR( "m_b" ) ) != std::string::npos )
				s.push( reinterpret_cast< bool* >( addr )[ index ] );
			else if ( prop_name.find( XOR_STR( "m_f" ) ) != std::string::npos && name_hash != FNV1A( "m_fFlags" ) )
				s.push( reinterpret_cast< float* >( addr )[ index ] );
			else if ( prop_name.find( XOR_STR( "m_v" ) ) != std::string::npos ||
				prop_name.find( XOR_STR( "m_ang" ) ) != std::string::npos )
			{
				s.push( reinterpret_cast< float* >( addr )[ index ] );
				s.push( reinterpret_cast< float* >( addr )[ index + 1 ] );
				s.push( reinterpret_cast< float* >( addr )[ index + 2 ] );
				return 3;
			}
			else if ( prop_name.find( XOR_STR( "m_psz" ) ) != std::string::npos ||
				prop_name.find( XOR_STR( "m_sz" ) ) != std::string::npos )
				s.push( reinterpret_cast< const char* >( addr ) );
			else
			{
				if ( name_hash == FNV1A( "m_iItemDefinitionIndex" ) )
					s.push( reinterpret_cast< short* >( addr )[ index ] );
				else
					s.push( reinterpret_cast< int* >( addr )[ index ] );
			}

			return 1;
		case send_prop_type::dpt_float:
			s.push( reinterpret_cast< float* >( addr )[ index ] );
			return 1;
		case send_prop_type::dpt_vector:
			s.push( reinterpret_cast< float* >( addr )[ index * 3 ] );
			s.push( reinterpret_cast< float* >( addr )[ index * 3 + 1 ] );
			s.push( reinterpret_cast< float* >( addr )[ index * 3 + 2 ] );
			return 3;
		case send_prop_type::dpt_vectorxy:
			s.push( reinterpret_cast< float* >( addr )[ index * 2 ] );
			s.push( reinterpret_cast< float* >( addr )[ index * 2 + 1 ] );
			return 2;
		case send_prop_type::dpt_string:
			s.push( reinterpret_cast< const char* >( addr ) );
			return 1;
		case send_prop_type::dpt_int64:
		{
			const auto var = reinterpret_cast< int64_t* >( addr )[ index ];
			s.push( std::to_string( var ).c_str() );
			return 1;
		}
	}
}

int lua::api_def::entity::set_prop( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	if ( !s.check_arguments( {
		{ ltd::user_data },
		{ ltd::string },
		{ ltd::number } }, true ) )
	{
		s.error( XOR_STR( "usage: ent:set_prop(name, index, value(s))" ) );
		return 0;
	}

	auto index = 0;

	// 3 IS 3!
	if ( s.is_number( 3 ) )
		index = s.get_integer( 3 );

	const auto ent = *reinterpret_cast< C_BaseEntity** >( s.get_user_data( 1 ) );

	if ( !ent || !is_sane( ent ) )
		return 0;

	const auto client_class = ent->GetClientClass();

	auto prop_name = std::string( s.get_string( 2 ) );

	const auto ref = helpers::get_netvar( prop_name.c_str(), client_class->m_pRecvTable );

	if ( !ref.offset )
		return 0;

	const auto addr = uint32_t( ent ) + ref.offset;

	switch ( ref.type )
	{
		default:
		case send_prop_type::dpt_int:
			if ( s.is_number( 4 ) )
				reinterpret_cast< int* >( addr )[ index ] = s.get_integer( 4 );
			else if ( s.is_boolean( 4 ) )
				reinterpret_cast< bool* >( addr )[ index ] = s.get_boolean( 4 );
			break;
		case send_prop_type::dpt_float:
			if ( s.is_number( 4 ) )
				reinterpret_cast< float* >( addr )[ index ] = s.get_float( 4 );
			break;
		case send_prop_type::dpt_vector:
			if ( s.is_number( 4 ) )
				reinterpret_cast< float* >( addr )[ index ] = s.get_float( 4 );
			if ( s.is_number( 5 ) )
				reinterpret_cast< float* >( addr )[ index ] = s.get_float( 5 );
			if ( s.is_number( 6 ) )
				reinterpret_cast< float* >( addr )[ index ] = s.get_float( 6 );
			break;
		case send_prop_type::dpt_vectorxy:
			if ( s.is_number( 4 ) )
				reinterpret_cast< float* >( addr )[ index ] = s.get_float( 4 );
			if ( s.is_number( 5 ) )
				reinterpret_cast< float* >( addr )[ index ] = s.get_float( 5 );
			break;
		case send_prop_type::dpt_string:
		case send_prop_type::dpt_int64:
			s.error( "Not implemented" );
			break;
	}
	return 0;
}

int lua::api_def::entity::get_move_type( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_CSPlayer>( s, XOR_STR( "usage: ent:get_move_type()" ) );

	if ( !ent || !is_sane( ent ) || !ent->is_player() )
		return 0;

	s.push( ent->get_move_type() );

	return 1;
}

int lua::api_def::entity::get_bbox( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_CSPlayer>( s, XOR_STR( "usage: ent:get_bbox() x1, y1, x2, y2" ) );

	if ( !ent || !is_sane( ent ) || !ent->is_player() )
		return 0;

	const auto& info = visuals::players[ ent->EntIndex() ];
	if ( !info.valid )
		return 0;

	const auto tl = ( info.top.x - info.width );
	const auto br = ( info.bot.x + info.width );

	s.push( tl );
	s.push( info.top.y );
	s.push( br );
	s.push( info.bot.y );
	return 4;
}

int lua::api_def::entity::get_weapon( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_CSPlayer>( s, XOR_STR( "usage: ent:get_weapon(): entity" ) );

	if ( !ent || !is_sane( ent ) || !ent->is_player() )
		return 0;

	const auto weapon = globals::get_weapon( ent->get_active_weapon() );
	if ( !weapon || !is_sane( weapon ) )
		return 0;

	s.create_user_object_ptr( XOR_STR( "csgo.entity" ), weapon );
	return 1;
}

int lua::api_def::entity::get_esp_alpha( lua_State* l )
{
	runtime_state s( l );
	if ( !interfaces::engine()->IsInGame() )
		return 0;

	const auto ent = extract_type<C_CSPlayer>( s, XOR_STR( "usage: ent:get_esp_alpha()" ) );
	if ( !ent || !is_sane( ent ) || !ent->is_player() )
		return 0;

	s.push( visuals::players[ ent->EntIndex() ].alpha );
	return 1;
}