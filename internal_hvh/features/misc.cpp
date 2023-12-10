#include "../include_cheat.h"
#include <experimental/map>

void misc::bhop( bool post_prediction )
{
	var( sv_autobunnyhopping );

	const auto player = local_player;

	if ( post_prediction )
	{
		if ( vars::misc.bhop->get<bool>() && !sv_autobunnyhopping->get_bool() && globals::current_cmd->buttons & IN_JUMP && player->get_flags() & FL_ONGROUND )
		{
			globals::current_cmd->buttons &= ~IN_JUMP;
			prediction::run_prediction();
		}
		else if ( vars::misc.edge_jump->get<bool>() && player->get_flags() & FL_ONGROUND )
		{
			auto mv = interfaces::game_movement()->setup_move( player, globals::current_cmd );
			const auto backup_frametime = interfaces::globals()->frametime;
			interfaces::globals()->frametime = interfaces::globals()->interval_per_tick;
			const auto data = interfaces::game_movement()->process_movement( player, &mv );
			interfaces::globals()->frametime = backup_frametime;

			if ( !( data.flags & FL_ONGROUND ) )
				globals::current_cmd->buttons |= IN_JUMP;
		}
		return;
	}

	prediction::attempted_jump = globals::current_cmd->buttons & IN_JUMP;

	if ( !vars::misc.bhop->get<bool>() || sv_autobunnyhopping->get_bool() )
		return;

	constexpr MoveType_t bad_mts[] = { MOVETYPE_LADDER, MOVETYPE_NOCLIP, MOVETYPE_OBSERVER };
	const auto in_water = player->get_water_level() >= 2;
	const auto bad_mt = std::ranges::find( bad_mts, player->get_move_type() ) != std::end( bad_mts );

	if ( !( player->get_flags() & FL_ONGROUND ) && !bad_mt && !in_water )
		globals::current_cmd->buttons &= ~IN_JUMP;

	if ( !prediction::attempted_jump && !( player->get_flags() & FL_ONGROUND ) && tickbase::force_choke )
	{
		auto mv = interfaces::game_movement()->setup_move( player, globals::current_cmd );
		const auto backup_frametime = interfaces::globals()->frametime;
		interfaces::globals()->frametime = interfaces::globals()->interval_per_tick;
		const auto data = interfaces::game_movement()->process_movement( player, &mv );
		interfaces::globals()->frametime = backup_frametime;

		 if ( data.flags & FL_ONGROUND )
			globals::current_cmd->buttons |= IN_DUCK;

	}
}

void misc::remove_smoke( const ClientFrameStage_t stage )
{
	if ( stage != FRAME_RENDER_START || !vars::misc.remove_smoke->get<bool>() )
		return;

	*reinterpret_cast< int* >( make_offset( "client.dll", sig_smoke_count ) ) = 0;
}

void misc::remove_flash( const ClientFrameStage_t stage )
{
	if ( stage != FRAME_RENDER_START || !vars::misc.remove_flash->get<bool>() || !local_player )
		return;

	local_player->get_flash_alpha() = 0.f;
}

void misc::reveal_ranks()
{
	if ( !vars::misc.reveal_ranks->get<bool>() || !( globals::current_cmd->buttons & IN_SCORE ) )
		return;

	float empty[ 3 ] = { 0, 0, 0 };

	reinterpret_cast< bool( __cdecl* )( float* ) >( make_offset( "client.dll", sig_reveal_ranks ) )( empty );
}

void misc::clan_tag()
{
	static auto was_on = false;

	if ( !vars::misc.clantag->get<bool>() )
	{
		if ( was_on )
			reinterpret_cast< int( __fastcall* )( const char*, const char* ) >( make_offset_multi( "engine.dll", sig_set_clantag ) )( "", "" );
		was_on = false;
		return;
	}

	was_on = true;

	static auto old_string = std::string();

	const auto ticks = time_to_ticks( ticks_to_time( interfaces::client_state()->get_last_server_tick() + 163 ) + interfaces::engine()->GetNetChannelInfo()->GetLatency( FLOW_OUTGOING ) );
	const auto offset = ( ticks / 20 ) % 40;

	auto current_string = std::string();

	if ( offset < 10 )
	{
		switch ( offset )
		{
			case 0:
				current_string = _r( "" );
				break;
			case 1:
				current_string = _r( "|" );
				break;
			case 2:
				current_string = _r( "f|" );
				break;
			case 3:
				current_string = _r( "fa|" );
				break;
			case 4:
				current_string = _r( "fat|" );
				break;
			case 5:
				current_string = _r( "fata|" );
				break;
			case 6:
				current_string = _r( "fatal|" );
				break;
			case 7:
				current_string = _r( "fatali|" );
				break;
			case 8:
				current_string = _r( "fatalit|" );
				break;
			case 9:
				current_string = _r( "fatality|" );
				break;
		}

	}
	else if ( offset < 20 )
	{
		switch ( offset - 10 )
		{
			case 0:
				current_string = _r( "fatality|" );
				break;
			case 1:
				current_string = _r( "fatality" );
				break;
			case 2:
				current_string = _r( "fatality" );
				break;
			case 3:
				current_string = _r( "fatality|" );
				break;
			case 4:
				current_string = _r( "fatality|" );
				break;
			case 5:
				current_string = _r( "fatality" );
				break;
			case 6:
				current_string = _r( "fatality" );
				break;
			case 7:
				current_string = _r( "fatality|" );
				break;
			case 8:
				current_string = _r( "fatality|" );
				break;
			case 9:
				current_string = _r( "fatality" );
				break;
		}

	}
	else if ( offset < 30 )
	{
		switch ( offset - 20 )
		{
			case 0:
				current_string = _r( "fatalit|" );
				break;
			case 1:
				current_string = _r( "fatali|" );
				break;
			case 2:
				current_string = _r( "fatal|" );
				break;
			case 3:
				current_string = _r( "fata|" );
				break;
			case 4:
				current_string = _r( "fat|" );
				break;
			case 5:
				current_string = _r( "fa|" );
				break;
			case 6:
				current_string = _r( "f|" );
				break;
			case 7:
				current_string = _r( "|" );
				break;
			case 8:
				current_string = _r( "|" );
				break;
		}
	}
	else
		switch ( offset - 30 )
		{
			case 0:
				current_string = _r( "\4\4\4" );
				break;
			case 1:
				current_string = _r( "" );
				break;
			case 2:
				current_string = _r( "|" );
				break;
			case 3:
				current_string = _r( "|" );
				break;
			case 4:
				current_string = _r( "" );
				break;
			case 5:
				current_string = _r( "" );
				break;
			case 6:
				current_string = _r( "|" );
				break;
			case 7:
				current_string = _r( "|" );
				break;
			case 8:
				current_string = _r( "" );
				break;
		}

	if ( old_string != current_string )
		reinterpret_cast< int( __fastcall* )( const char*, const char* ) >( make_offset_multi( "engine.dll", sig_set_clantag ) )( current_string.c_str(), current_string.c_str() );

	old_string = current_string;
}

void misc::play_freezetime_music()
{
#if 0
	_( techno_overpass, "ambient/misc/techno_overpass.wav" );

	static auto played = false;
	if ( *interfaces::game_rules_proxy() && ( *interfaces::game_rules_proxy() )->m_flRestartRoundTime() && false )
	{
		if ( !played )
			interfaces::surface()->PlaySound( techno_overpass );

		played = true;

	}
	else if ( played )
	{
		interfaces::engine_sound()->StopAllSounds( false );
		played = false;
}

#endif
}

void misc::preserve_deathnotices( ClientFrameStage_t stage )
{
	if ( stage != FRAME_NET_UPDATE_START || !interfaces::engine()->IsInGame() || !local_player )
		return;

	static auto spawntime = 0.f;
	static auto status = false;

	auto set = false;
	if ( spawntime != local_player->get_spawn_time() || status != vars::misc.preservedn->get<bool>() )
	{
		set = true;
		status = vars::misc.preservedn->get<bool>();
		spawntime = local_player->get_spawn_time();
	}

	if ( set )
	{
		_( CCSGO_HudDeathNotice, "CCSGO_HudDeathNotice" );
		const auto death_notice = reinterpret_cast< uintptr_t >( interfaces::hud()->FindElement( CCSGO_HudDeathNotice.c_str() ) );
		const auto notice_element = reinterpret_cast< uintptr_t* >( death_notice - 0x14 );
		if ( !death_notice || !notice_element )
			return;

		static auto byte_150825F6 = reinterpret_cast< uintptr_t* >( make_offset_once( "client.dll", sig_deathnotice_check ) );
		if ( !*byte_150825F6 )
			return;

		const auto lifetime = reinterpret_cast< float* >( death_notice + 0x50 );
		status ? *lifetime = FLT_MAX : *lifetime = 1.5f;
		reinterpret_cast< int( __thiscall* )( void* ) >( make_offset( "client.dll", sig_clear_deathnotices ) )( notice_element );
	}
}

void misc::buybot()
{
	static std::vector<std::string> buybot_primary = { _w( "none" ), _w( "ak47" ), _w( "m4a1" ), _w( "sg556" ), _w( "awp" ), _w( "ssg08" ), _w( "g3sg1" ), _w( "scar20" ), _w( "negev" ) };
	static std::vector<short> buybot_primary_idx = { -1, WEAPON_AK47, WEAPON_M4A1, WEAPON_SG556, WEAPON_AWP, WEAPON_SSG08, WEAPON_G3SG1, WEAPON_SCAR20, WEAPON_NEGEV };
	static std::vector<int> buybot_primary_prices = { 0, 2700, 3100, 2750, 4750, 1600, 5000, 5000, 1700 };

	static std::vector<std::string> buybot_secondary = { _w( "none" ), _w( "deagle" ), _w( "elite" ), _w( "tec9" ), _w( "p250" ), _w( "fiveseven" ) };
	static std::vector<short> buybot_secondary_idx = { -1, WEAPON_DEAGLE, WEAPON_ELITE, WEAPON_TEC9, WEAPON_P250, WEAPON_FIVESEVEN };
	static std::vector<int> buybot_secondary_prices = { 0, 600, 400, 500, 300, 500 };

	static std::vector<std::string> buybot_equip = { _w( "none" ), _w( "vest" ), _w( "vesthelm" ), _w( "defuser" ), _w( "taser" ) };
	static std::vector<short> buybot_equip_idx = { -1, -1, -1, -1, WEAPON_TASER };
	static std::vector<int> buybot_equip_prices = { 0, 650, 350, 400, 200 };

	constexpr auto print_buy = [] ( std::string str )
	{
		if ( !vars::misc.buylog->get<bool>() )
			return;

		_( fatality, "[fatality] " );
		_( bought, "bought " );
		_( sn, "%s\n" );
		interfaces::cvar()->ConsoleColorPrintf( Color( 235, 5, 90, 255 ), fatality.c_str() );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), bought.c_str() );
		util::print_dev_console( true, Color( 173, 216, 230, 255 ), sn.c_str(), str.c_str() );
	};

	constexpr auto has_weapon = [] ( const int index )
	{
		const auto weapons = local_player->get_weapons();
		for ( size_t i = 0; weapons[ i ] != INVALID_EHANDLE_INDEX; i++ )
		{
			const auto weapon = reinterpret_cast< C_BaseCombatWeapon* >( interfaces::entity_list()->get_client_entity_from_handle( weapons[ i ] ) );
			if ( !weapon || weapon->get_owner_ent() == -1 )
				continue;

			if ( weapon->get_weapon_id() == index )
				return true;

			if ( index == WEAPON_M4A1 && weapon->get_weapon_id() == WEAPON_M4A1_SILENCER )
				return true;

			if ( index == WEAPON_DEAGLE && weapon->get_weapon_id() == WEAPON_REVOLVER )
				return true;
		}

		return false;
	};

	static auto skip_first = true;

	if ( !local_player->get_alive() )
		return;

	if ( skip_first )
	{
		skip_first = false;
		return;
	}

	static auto spawntime = 0.f;

	if ( spawntime == local_player->get_spawn_time() )
	{
		skip_first = true;
		return;
	}

	spawntime = local_player->get_spawn_time();

	const auto ct = local_player->get_team() == 3;
	auto index = vars::misc.buybot.primary->get<int>();

	if ( index > 1 )
		index++;

	if ( index > 5 )
		index++;

	if ( ct && ( index == 1 || index == 6 ) )
		index++;

	const auto index_secondary = vars::misc.buybot.pistol->get<int>();
	auto armor_price = 0;

	auto current_money = local_player->get_money();

	if ( vars::misc.buybot.kevlar_helmet->get<int>() > 0 )
		armor_price += buybot_equip_prices[ 1 ];
	if ( vars::misc.buybot.kevlar_helmet->get<int>() == 2 )
		armor_price += buybot_equip_prices[ 2 ];

	if ( local_player->get_armor() > 20 && local_player->get_helmet() )
		armor_price = 0;

	if ( local_player->get_armor() > 20 )
		armor_price = armor_price -= buybot_equip_prices[ 1 ];

	auto bought_fullarmor = false;
	auto bought_primary = false;
	if ( index > 0 && ( index != 3 || !ct ) && current_money >= buybot_primary_prices[ index ] + armor_price && !has_weapon( buybot_primary_idx[ index ] ) )
	{
		_( buy, "buy " );
		_( buys, "buy m4a1-s" );
		_u( str, buybot_primary[ index ] );
		std::string buystr( buy );
		buystr.append( str );
		interfaces::engine()->ClientCmd_Unrestricted( buystr.c_str() );
		if ( ct && index == 1 )
			interfaces::engine()->ClientCmd_Unrestricted( buys.c_str() );

		print_buy( str.c_str() );

		current_money -= buybot_primary_prices[ index ];
		bought_primary = true;
	}

	if ( ( current_money >= buybot_secondary_prices[ index_secondary ] + armor_price || bought_primary ) && vars::misc.buybot.kevlar_helmet->get<int>() && ( local_player->get_armor() < 20 || !local_player->get_helmet() && vars::misc.buybot.kevlar_helmet->get<int>() == 2 ) )
	{
		_( buystr, "buy vest" );
		_( buystr2, "buy vesthelm" );
		_( str, "kevlar" );
		_( str2, "kevlar/helmet" );

		interfaces::engine()->ClientCmd_Unrestricted( buystr.c_str() );

		if ( vars::misc.buybot.kevlar_helmet->get<int>() == 2 )
			interfaces::engine()->ClientCmd_Unrestricted( buystr2.c_str() );


		if ( vars::misc.buybot.kevlar_helmet->get<int>() == 2 )
			print_buy( str2 );
		else
			print_buy( str );

		current_money -= armor_price;

		bought_fullarmor = true;
	}

	if ( index_secondary > 0 && current_money >= buybot_secondary_prices[ index_secondary ] && !has_weapon( buybot_secondary_idx[ index_secondary ] ) )
	{
		_( buy, "buy " );
		_u( str, buybot_secondary[ index_secondary ] );
		std::string buystr( buy );
		buystr.append( str );
		interfaces::engine()->ClientCmd_Unrestricted( buystr.c_str() );

		print_buy( str.c_str() );

		current_money -= buybot_secondary_prices[ index_secondary ];
	}

	if ( !bought_fullarmor && current_money >= buybot_equip_prices[ 1 ] && vars::misc.buybot.kevlar_helmet->get<int>() )
	{
		_( buystr, "buy vest" );
		_( str, "kevlar" );
		interfaces::engine()->ClientCmd_Unrestricted( buystr.c_str() );
		print_buy( str );
	}

	if ( vars::misc.buybot.grenades->get<bool>() )
	{
		_( buystr1, "buy hegrenade" );
		_( buystr2, "buy smokegrenade" );
		_( buystr3, "buy molotov" );
		_( str1, "hegrenade" );
		_( str2, "smokegrenade" );
		_( str3, "molotov" );

		interfaces::engine()->ClientCmd_Unrestricted( buystr1.c_str() );
		print_buy( str1 );
		interfaces::engine()->ClientCmd_Unrestricted( buystr2.c_str() );
		print_buy( str2 );
		interfaces::engine()->ClientCmd_Unrestricted( buystr3.c_str() );
		print_buy( str3 );
	}

	if ( vars::misc.buybot.defuser->get<bool>() && ct && !local_player->has_defuser() && current_money >= buybot_equip_prices[ 3 ] )
	{
		_( buystr, "buy defuser" );
		_( str, "defuser" );
		interfaces::engine()->ClientCmd_Unrestricted( buystr.c_str() );
		print_buy( str );

		current_money -= buybot_equip_prices[ 3 ];
	}

	if ( vars::misc.buybot.taser->get<bool>() && current_money >= buybot_equip_prices[ 4 ] && !has_weapon( buybot_equip_idx[ 4 ] ) )
	{
		_( buystr, "buy taser" );
		_( str, "taser" );
		interfaces::engine()->ClientCmd_Unrestricted( buystr.c_str() );
		print_buy( str );
	}
}

void misc::write_tick( int cmdnum )
{
	const auto cmd_ver = &interfaces::input()->m_pVerifiedCommands[ cmdnum % 150 ];
	const auto cmd = &interfaces::input()->m_pCommands[ cmdnum % 150 ];

	cmd->headangles = cmd->viewangles;
	cmd->buttons2 = cmd->buttons;
	cmd_ver->m_cmd = *cmd;
	cmd_ver->m_crc = reinterpret_cast< uint32_t( __thiscall* )( CUserCmd* ) >( make_offset( "client.dll", sig_get_checksum ) )( &cmd_ver->m_cmd );
}

void misc::custom_render_options( ClientFrameStage_t stage )
{
	if ( stage != FRAME_START || true )
		return;

	var( r_modelAmbientMin );
	if ( vars::visuals.chams.model_brightness->get<bool>() && fabsf( vars::visuals.chams.model_brightness_amt->get<float>() * 0.01f - r_modelAmbientMin->get_float() ) >= 0.01f )
	{
		r_modelAmbientMin->unlock();
		r_modelAmbientMin->set_value( vars::visuals.chams.model_brightness_amt->get<float>() * 0.05f );
	}
	else
		r_modelAmbientMin->set_value( 0.f );
}

void misc::peek_assist()
{
	if ( !vars::misc.peek_assist->get<bool>() )
	{
		has_position = false;
		retract_peek = false;
		has_released = false;
		cant_reach = false;
		return;
	}

	const auto current_pos = local_player->get_origin();

	if ( !has_position && local_player->get_flags() & FL_ONGROUND )
	{
		movement_position = current_pos;
		has_position = true;
		has_released = false;
	}

	if ( !retract_peek && vars::misc.peek_assist_retreat_on_release->get<bool>() )
	{
		if ( cant_reach && ( globals::current_cmd->forwardmove != 0.f || globals::current_cmd->sidemove != 0.f ) )
			cant_reach = false;

		if ( !cant_reach && globals::current_cmd->forwardmove == 0.f && globals::current_cmd->sidemove == 0.f )
		{
			retract_peek = true;
			peek_position = current_pos;
		}
	}

	if ( retract_peek && has_position && !cant_reach )
	{
		if ( !tickbase::force_choke )
		{
			if ( globals::current_cmd->forwardmove == 0.f && globals::current_cmd->sidemove == 0.f )
				has_released = true;

			if ( has_released && ( globals::current_cmd->forwardmove != 0.f || globals::current_cmd->sidemove != 0.f ) )
			{
				retract_peek = false;
				has_released = false;
				return;
			}
		}

		globals::current_cmd->forwardmove = 450.f;
		globals::current_cmd->sidemove = globals::current_cmd->upmove = 0.f;
		antiaim::wish_angles[ globals::current_cmd->command_number % 150 ].y = math::calc_angle( current_pos, movement_position ).y;
		antiaim::fix_movement();

		auto start = interfaces::game_movement()->setup_move( local_player, globals::current_cmd );
		auto data = start;

		cant_reach = false;

		data = start;
		for ( auto i = 0; i < time_to_ticks( 1.f ); i++ )
		{
			data.m_flForwardMove = globals::current_cmd->forwardmove;
			data.m_flSideMove = globals::current_cmd->sidemove;
			aimbot_helpers::stop_to_speed( 0.f, &data );
			CGameMovement::walk_move( &data, local_player );
			data.m_vecAbsOrigin += data.m_vecVelocity * interfaces::globals()->interval_per_tick;

			if ( data.m_vecVelocity.Length() < .1f )
				break;
		}

		if ( ( data.m_vecAbsOrigin - peek_position ).Length() > ( peek_position - movement_position ).Length() - 1.1f )
		{
			data = start;
			aimbot_helpers::stop_to_speed( 0.f, &data );
			globals::current_cmd->forwardmove = data.m_flForwardMove;
			globals::current_cmd->sidemove = data.m_flSideMove;

			CGameMovement::walk_move( &data, local_player );
			if ( data.m_vecVelocity.Length() < 1.1f )
			{
				const auto weapon = local_weapon;
				const auto wpn_info = weapon ? interfaces::weapon_system()->GetWpnData( weapon->get_weapon_id() ) : nullptr;
				if ( !wpn_info || weapon->get_next_primary_attack() - interfaces::globals()->curtime <= 0.2f || !vars::misc.peek_assist->get<bool>() )
				{
					has_released = false;
					retract_peek = false;
				}
			}
		}
	}
}

void misc::ragdoll_slowpath( ClientFrameStage_t stage )
{
	if ( !interfaces::engine()->IsInGame() )
		return;

	if ( stage != FRAME_NET_UPDATE_END )
		return;

	for ( const auto ent : *interfaces::entity_list() )
	{
		if ( ent->GetClientClass()->m_ClassID == ClassId::CCSRagdoll )
		{
			const auto ragdoll = reinterpret_cast< C_CSRagdoll* >( ent );
			const auto player = reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity_from_handle( ragdoll->get_player_handle() ) );

			if ( player && ( vars::visuals.chams.enemy.ragdoll.type->get<bool>() && player->is_enemy() || vars::visuals.chams.team.ragdoll.type->get<bool>() && !player->is_enemy() ) )
				ragdoll->get_use_fastpath() = false;
		}
	}
}

void misc::fix_prediction( ClientFrameStage_t stage )
{
	if ( !interfaces::engine()->IsInGame() || !local_player )
		return;

	if ( stage != FRAME_NET_UPDATE_END )
		return;

	for ( auto i = interfaces::client_state()->command_ack; i <= interfaces::client_state()->lastoutgoingcommand; i++ )
		prediction::get_pred_info( i ).animation.checksum = 0;

	if ( !local_player->get_alive() )
		return;

	const auto& pred_info = prediction::get_pred_info( interfaces::client_state()->command_ack );

	if ( pred_info.animation.sequence == interfaces::client_state()->command_ack && pred_info.tick_base == local_player->get_tickbase() )
		if ( pred_info.buttons & IN_BULLRUSH && !( local_player->get_old_buttons() & IN_BULLRUSH ) )
			globals::restricted_unduck = true;
}

float misc::get_latency()
{
	return interfaces::engine()->GetNetChannelInfo()->GetAvgLatency( FLOW_OUTGOING ) + interfaces::engine()->GetNetChannelInfo()->GetAvgLatency( FLOW_INCOMING );
}