#include "../include_cheat.h"
#include <intrin.h>

#ifdef _DEBUG
void draw_server_hitboxes( uintptr_t player, int32_t n )
{
	static uintptr_t fn = sig_rel( "server.dll", "E8 ? ? ? ? F6 83 ? ? ? ? ? 0F 84 ? ? ? ? 33 FF" );
	float duration = interfaces::globals()->interval_per_tick * 2;
	__asm {
		movss xmm1, duration
		mov ecx, player
		push n
		call fn
	}
}

using get_server_ent_t = uintptr_t( __fastcall* )( int32_t );
get_server_ent_t get_server_ent = ( get_server_ent_t ) sig( "server.dll", "85 C9 7E 32 A1" );
#endif

void __stdcall cl_create_move( int sequence_number, float input_sample_frametime, bool active, bool& sendpacket )
{
	const auto cmd = globals::current_cmd = &interfaces::input()->m_pCommands[ sequence_number % 150 ];
	globals::send_packet = &sendpacket;

	original( hook::create_move )( sequence_number, input_sample_frametime, active );

	static auto anim_cmdnum = 1;
	if ( !interfaces::client_state()->chokedcommands )
		anim_cmdnum = cmd->command_number;

	globals::first_cmd = &interfaces::input()->m_pCommands[ anim_cmdnum % 150 ];
	globals::in_cm = true;
	antiaim::wish_angles[ sequence_number % 150 ] = cmd->viewangles;

	if ( !interfaces::engine()->IsConnected() )
	{
		globals::in_cm = false;
		aimbot::targets.clear();
		antiaim::reset();
		animations::fake_record = {};
		misc::has_position = false;
		animations::local_record = {};
		prediction::clear_pred_info();
		animations::lag = {};
		animations::most_recent = {};
		return;
	}

	if ( !tickbase::force_choke )
	{
		var( weapon_accuracy_nospread );
		globals::nospread = weapon_accuracy_nospread->get_bool();

		var( r_jiggle_bones );
		r_jiggle_bones->set_value( vars::legit_enabled() && !vars::aa.enabled->get<bool>() );

		var( cl_foot_contact_shadows );
		cl_foot_contact_shadows->set_int( !vars::visuals.disable_postprocessing->get<bool>() );

		var( fog_enable );
		fog_enable->set_int( !vars::visuals.disable_postprocessing->get<bool>() );

		var( fog_enableskybox );
		fog_enableskybox->set_int( !vars::visuals.disable_postprocessing->get<bool>() );

		var( fog_override );
		fog_override->set_int( 1 );

		var( cl_clock_correction );
		cl_clock_correction->set_int( 0 );

		misc::buybot();
		misc::clan_tag();
		misc::play_freezetime_music();
		misc::reveal_ranks();

		aimbot_helpers::manage_lagcomp();
	}

	lua::api.callback( FNV1A( "on_setup_move" ), [cmd, sendpacket] ( lua::state& s ) -> int
	{
		s.create_user_object_ptr( XOR_STR( "csgo.user_cmd" ), cmd );
		s.push( sendpacket );
		return 2;
	} );

	if ( !local_player || !local_player->get_alive() || !local_player->get_model_ptr() || !local_player->get_anim_state() )
	{
		globals::in_cm = false;
		aimbot::targets.clear();
		antiaim::reset();
		animations::fake_record = {};
		misc::has_position = false;
		animations::local_record = {};
		prediction::clear_pred_info();
		animations::lag = {};
		animations::most_recent = {};
		globals::restricted_unduck = false;
		sendpacket = true;
		return;
	}

	if ( vars::inventory.autoselect_weapon->get<bool>() && g_menu->is_open() )
	{
		if ( const auto weap = local_weapon )
		{
			auto wpn_idx = 0;
			for ( const auto& curr : inventorychanger::weapon_menu_items_data )
			{
				if ( curr == weap->get_weapon_id() )
					break;

				wpn_idx++;
			}

			if ( wpn_idx < inventorychanger::weapon_menu_items_data.size() )
			{
				vars::inventory.definition_index->set( wpn_idx );
				inventorychanger::weapons_listbox.lock()->adjust_scroll();
			}

		}
	}

	if ( !( cmd->buttons & IN_DUCK ) && vars::misc.instant_unduck->get<bool>() && !globals::restricted_unduck )
		cmd->buttons |= IN_BULLRUSH;

	static auto prev_spawntime = 0.f;
	if ( prev_spawntime != local_player->get_spawn_time() )
	{
		antiaim::reset();
		animations::fake_record = {};
		misc::has_position = false;
		animations::local_record = {};
		prediction::clear_pred_info();
		animations::most_recent = {};
	}
	prev_spawntime = local_player->get_spawn_time();

	const auto weapon = local_weapon;
	if ( weapon && weapon->get_weapon_id() == WEAPON_REVOLVER && ( config::current_config( weapon )->autorevolver->get<bool>() || vars::aim.enabled->get<bool>() || vars::aa.enabled->get<bool>() ) )
		cmd->buttons &= ~IN_ATTACK2;

	misc::peek_assist();
	autostrafer::strafe();
	misc::bhop();

	prediction::start( sequence_number );

	misc::bhop( true );

	if ( !tickbase::force_choke )
		tickbase::adjust_limit_dynamic();

	*globals::send_packet = !tickbase::force_choke;

	//if ( vars::visuals.esp.flags.hit->get<bool>() && !tickbase::force_choke && ( vars::aa.enabled->get<bool>() || vars::aa.choke_mode->get<int>() ) )
	//	antiaim::check_hittable();

	if ( vars::aim.enabled->get<bool>() && prediction::can_fire() )
		aimbot::calculate_hittable_enemies();

	antiaim::run_lag();

	antiaim::fake_duck();

	resolver::on_createmove();

	if ( weapon && vars::aim.enabled->get<bool>() && ( weapon->is_shootable() || weapon->is_secondary_attack_weapon() ) && weapon->get_weapon_id() != WEAPON_REVOLVER )
	{
		if ( !prediction::can_fire() )
		{
			prediction::take_shot( false );
			prediction::take_secondary_shot( false );
			cmd->buttons &= ~( IN_ATTACK | IN_ATTACK2 );
		}
	}

#if defined(ALPHA) || !defined(RELEASE)
	for ( auto i = 1; i <= interfaces::globals()->maxClients; i++ )
		visuals::players[ i ].debug.scan = false;
#endif

	aimbot::run();

	const auto force_stop = prediction::unpred_move.Length() == 0.f && prediction::unpred_flags & FL_ONGROUND && !( cmd->buttons & IN_JUMP ) && ( vars::aim.enabled->get<bool>() || vars::legit.quick_stop->get<bool>() );
	const auto new_stop = force_stop ? aimbot::stop_type::stop_full : vars::aim.slide->get<bool>() || vars::aim.fake_duck->get<bool>() ? aimbot::stop_type::stop_slide : aimbot::stop_type::stop_none;
	if ( aimbot::stop == aimbot::stop_type::stop_none && aimbot::keep_stop == aimbot::stop_type::stop_none && new_stop == aimbot::stop_type::stop_none )
	{
		cmd->forwardmove = prediction::unpred_move.x;
		cmd->sidemove = prediction::unpred_move.y;

		if ( tickbase::to_shift == 0 )
		{
			prediction::run_prediction( globals::current_cmd->command_number );

			// prevent move layer from reaching highest weight...
			if ( weapon && local_player->get_flags() & FL_ONGROUND && !( cmd->buttons & IN_JUMP ) && vars::aa.fake->get<bool>() && vars::aa.enabled->get<bool>()
			&& cmd->command_number > 1 && prediction::get_pred_info( cmd->command_number - 1 ).m_flags & FL_ONGROUND )
			{
				const auto is_revolver = weapon->get_weapon_id() == WEAPON_REVOLVER;
				const auto jitter = fabs( sin( fmodf( interfaces::globals()->curtime, .4f ) / .2f * M_PI ) );
				const auto amnt = local_player->get_velocity().Length2D() / 25.f;
				aimbot_helpers::stop_to_speed( std::max( ( is_revolver ? 180.f : weapon->get_max_speed() ) - 0.1f - amnt * jitter, 0.f ), cmd );
			}
		}
	}

	const auto dont_run_autostop = cmd->command_number == globals::shot_command;
	if ( aimbot::stop == aimbot::stop_type::stop_none && !dont_run_autostop && ( aimbot::keep_stop != aimbot::stop_type::stop_none || new_stop != aimbot::stop_type::stop_none ) )
	{
		const auto target_stop = new_stop != aimbot::stop_type::stop_none ? new_stop : aimbot::keep_stop;

		if ( target_stop == aimbot::stop_type::stop_full )
			aimbot_helpers::stop_to_speed( 0.f, cmd );
		else
			aimbot_helpers::slide_stop( cmd );
	}

	antiaim::fix_movement( cmd );

	legit::run_triggerbot();
	legit::run_backtrack();

	aimbot::manual_shot();

	static int shift_back = -1;
	if ( prediction::has_shot() && tickbase::holds_tick_base_weapon() && tickbase::to_shift == 0 )
		shift_back = 3;

	if ( shift_back != -1 )
	{
		if ( misc::retract_peek && tickbase::fast_fire )
		{
			if ( local_player->get_tickbase() > animations::lag.first - 2 || --shift_back < 0 )
			{
				shift_back = -1;
				tickbase::attempt_shift_back( sendpacket );
			}
		}
		else
		{
			shift_back = -1;
			tickbase::attempt_shift_back( sendpacket );
		}
	}

	if ( !globals::nospread )
		cmd->viewangles.Clamp();

	antiaim::fix_movement( cmd );

	if ( *globals::send_packet )
	{
		// make sure we cache defuse range with newest tick
		antiaim::check_in_defuse_range();

		tickbase::on_send_command();
		prediction::finish();
		antiaim::run_on_send();
		resolver::set_local_info();
	}

	if ( *globals::send_packet )
		animations::build_fake_animation();

	lua::api.callback( FNV1A( "on_create_move" ), [cmd, sendpacket] ( lua::state& s ) -> int
	{
		s.create_user_object_ptr( XOR_STR( "csgo.user_cmd" ), cmd );
		s.push( sendpacket );
		return 2;
	} );

	globals::in_cm = false;

	prediction::finish( sendpacket );

	prediction::evaluate_shots();

	cmd->upmove = clamp( cmd->upmove, -320.f, 320.f );

	misc::write_tick( cmd->command_number );

	tickbase::on_finish_command( sendpacket );

	//util::print_dev_console( true, Color::Lime(), "%d %d %d\n", time_to_ticks( interfaces::engine()->GetNetChannelInfo()->GetAvgLatency( FLOW_INCOMING ) + interfaces::engine()->GetNetChannelInfo()->GetAvgLatency( FLOW_OUTGOING ) ), time_to_ticks( interfaces::engine()->GetNetChannelInfo()->GetLatency( FLOW_INCOMING ) + interfaces::engine()->GetNetChannelInfo()->GetLatency( FLOW_OUTGOING ) ), time_to_ticks( misc::get_latency() ) );

#ifdef _DEBUG
#if 0
	if ( const auto bot = get_server_ent( 2 ) )
		util::print_dev_console( true, Color::Lime(), "%d - %.4f\n", time_to_ticks( *( float* ) ( bot + 0x3AF4 ) ), *( float* ) ( bot + 0x3AF0 ) );

	if ( const auto bot = globals::get_player( 2 ) )
		util::print_dev_console( true, Color::RedAccent(), "%d - %.4f\n", time_to_ticks( bot->get_collision_bounds_change_time() ), bot->get_collision_viewheight() );
#endif
#if 0
	if ( const auto local = get_server_ent( 1 ) )
		draw_server_hitboxes( local, 1 );
#endif
#endif
}

__declspec( naked ) void __stdcall hook::create_move( int sequence_number, float input_sample_frametime, bool active )
{
	__asm
	{
		push ebx				// push sendpacket from ebx
		push esp				// push address of sendpackt
		push[ esp + 0x14 ]
		push[ esp + 0x14 ]
		push[ esp + 0x14 ]
		call cl_create_move
		pop ebx
		ret 0x0C
	}
}

void original_cl_move( float accumulated_extra_sample, bool final_tick )
{
	auto orig_func = original( hook::cl_move_naked );

	__asm
	{
		mov cl, final_tick
		movss xmm0, accumulated_extra_sample
		mov eax, [ orig_func ]
		call eax
	}
}

uintptr_t client_state = 0;
uintptr_t jumpto = 0;
__declspec( naked ) void rest_of_clmove()
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		sub     esp, 164h
		push    ebx
		push    esi
		push    edi
		mov     edi, [ client_state ]
		mov     eax, [ jumpto ]
		jmp     eax
	}
}

void __stdcall cl_move( const float accumulated_extra_sample, const bool final_tick )
{
	if ( interfaces::client_state()->m_nSignonState != SIGNONSTATE_FULL || !local_player || !local_player->get_alive() )
	{
		original_cl_move( accumulated_extra_sample, final_tick );
		globals::sent_commands.push_back( interfaces::client_state()->lastoutgoingcommand );
		tickbase::post_shift = false;
		return;
	}

	interfaces::client_state()->process_sockets();
	interfaces::prediction()->Update( interfaces::client_state()->m_nDeltaTick, true, interfaces::client_state()->last_command_ack, interfaces::client_state()->lastoutgoingcommand + interfaces::client_state()->chokedcommands );

	const auto out_sequence = interfaces::client_state()->lastoutgoingcommand;

	const auto recharge = tickbase::to_recharge > 0 && !interfaces::client_state()->chokedcommands;
	if ( recharge )
	{
		util::print_dev_console( true, Color::Lime(), "recharging %d\n", tickbase::to_recharge );

		tickbase::to_recharge--;

		const auto current_tick = interfaces::client_state()->lastoutgoingcommand + 1;

		interfaces::input()->m_pCommands[ current_tick % 150 ] = interfaces::input()->m_pCommands[ out_sequence % 150 ];
		interfaces::input()->m_pCommands[ current_tick % 150 ].tick_count = INT_MAX;
		interfaces::input()->m_pCommands[ current_tick % 150 ].command_number = current_tick;
		misc::write_tick( current_tick );

		globals::current_cmd = &interfaces::input()->m_pCommands[ current_tick % 150 ];

		tickbase::on_recharge( globals::current_cmd );
		tickbase::compute_current_limit();
	}
	else
	{
		tickbase::to_adjust = 0;
		tickbase::force_choke = false;
		tickbase::force_unchoke = false;
		original_cl_move( accumulated_extra_sample, final_tick );

		if ( tickbase::to_shift > 0 )
		{
			for ( auto i = 0; i < tickbase::to_shift; i++ )
			{
				tickbase::force_choke = true;
				original_cl_move( accumulated_extra_sample, final_tick );
			}

			tickbase::to_shift = 0;
			tickbase::post_shift = true;
			interfaces::prediction()->get_predicted_commands() = clamp( interfaces::client_state()->lastoutgoingcommand + 1 - interfaces::client_state()->last_command_ack, 0, interfaces::prediction()->get_predicted_commands() );
		}

		if ( out_sequence == interfaces::client_state()->lastoutgoingcommand && interfaces::client_state()->m_NetChannel )
		{
			interfaces::client_state()->m_NetChannel->m_nOutSequenceNr--;
			interfaces::client_state()->m_NetChannel->m_nChokedPackets = 0;
		}
	}

	if ( out_sequence == interfaces::client_state()->lastoutgoingcommand && interfaces::client_state()->m_NetChannel )
	{
		const auto backup_outgoing = interfaces::client_state()->lastoutgoingcommand;
		const auto backup_choked = interfaces::client_state()->chokedcommands;

		/*client_state = reinterpret_cast< uintptr_t >( interfaces::client_state() );
		jumpto = make_offset( "engine.dll", sig_cl_move_rest );
		rest_of_clmove();*/
		interfaces::client_state()->chokedcommands = 0;
		interfaces::client_state()->m_NetChannel->SendDatagram();
		--interfaces::client_state()->m_NetChannel->m_nOutSequenceNr;

		interfaces::client_state()->chokedcommands = backup_choked;
		if ( !recharge )
			interfaces::client_state()->lastoutgoingcommand = backup_outgoing;
	}
	else
	{
		tickbase::post_shift = false;
		tickbase::compute_current_limit( interfaces::client_state()->lastoutgoingcommand );
		globals::sent_commands.push_back( interfaces::client_state()->lastoutgoingcommand );
	}


}

__declspec( naked ) void hook::cl_move_naked()
{
	__asm
	{
		push ebp
		mov ebp, esp
		movzx eax, cl
		push eax
		push eax
		movss[ esp ], xmm0
		call cl_move
		pop ebp
		ret
	}
}

void __stdcall hook::scale_mouse( int slot, float* mouse_x, float* mouse_y )
{
	original( scale_mouse )( slot, mouse_x, mouse_y );

	if ( interfaces::engine()->IsInGame() )
		legit::run_aim( mouse_x, mouse_y );
}