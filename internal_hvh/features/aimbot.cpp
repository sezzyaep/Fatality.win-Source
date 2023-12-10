#include "../include_cheat.h"

void aimbot::run()
{
	stop = stop_type::stop_none;
	has_shot = false;

	if ( !aimbot_helpers::get_current_autostop() )
		keep_stop = stop_type::stop_none;

	if ( vars::legit_enabled() || ( *interfaces::game_rules_proxy() )->m_bFreezePeriod() )
	{
		targets.clear();
		last_target = -1;
		keep_stop = stop_type::stop_none;
		return;
	}

	const auto new_weapon = local_weapon;;
	const auto weapon_changed = weapon != new_weapon;
	weapon = new_weapon;

	if ( !weapon || weapon->is_knife() && !vars::aim.knife_bot->get<bool>() )
	{
		targets.clear();
		last_target = -1;
		keep_stop = stop_type::stop_none;
		return;
	}

	select_targets();
	if ( targets.empty() )
	{
		last_target = -1;
		if ( !has_dormant_target() )
		{
			keep_stop = stop_type::stop_none;
			return;
		}
	}

	if ( weapon_changed )
	{
		for ( auto& target : targets )
			target.hitboxes.clear();

		keep_stop = stop_type::stop_none;
	}


	auto revolver_pass = false;
	if ( weapon && weapon->get_weapon_id() == WEAPON_REVOLVER && config::current_config( weapon )->autorevolver->get<bool>() && weapon->get_clip1() > 0 )
		revolver_pass = prediction::get_pred_info( globals::current_cmd->command_number ).postpone_fire_ready_time > interfaces::globals()->curtime || interfaces::globals()->curtime < weapon->get_next_secondary_attack() - ticks_to_time( 3 );

	if ( !prediction::can_fire() && !revolver_pass )
	{
		const auto previous_target = last_target > 0 ? globals::get_player( last_target ) : nullptr;
		if ( weapon->in_reload() || last_target > 0 && ( !previous_target || !previous_target->get_alive() ) /*&& !antiaim::check_server_hittable()*/ )
		{
			for ( auto& target : targets )
				target.hitboxes.clear();

			keep_stop = stop_type::stop_none;
		}

		if ( !prediction::can_fire() )
		{
			const auto is_singleshot = weapon->get_weapon_id() == WEAPON_SSG08 || weapon->get_weapon_id() == WEAPON_AWP || weapon->get_weapon_id() == WEAPON_REVOLVER || misc::retract_peek;
			if ( !config::current_config( weapon )->autostop_inbetween->get<bool>() || misc::retract_peek )
				keep_stop = stop_type::stop_none;

			if ( misc::retract_peek )
				last_target = -1;

			if ( is_singleshot && tickbase::to_shift > 0 )
				keep_stop = stop_type::stop_none;
		}
		else
		{
			prediction::take_shot( false );
			prediction::take_secondary_shot( false );
			globals::current_cmd->buttons &= ~IN_ATTACK;
			if ( !weapon || weapon->is_secondary_attack_weapon() )
				globals::current_cmd->buttons &= ~IN_ATTACK2;
		}

		return;
	}

	if ( !prediction::had_attack )
	{
		if ( vars::aim.fake_duck->get<bool>() && !*globals::send_packet )
			return;

		prediction::run_prediction();

		const auto doubletap_ready = ( tickbase::fast_fire || tickbase::hide_shot ) && tickbase::compute_current_limit() > 2;

		const auto choke_limit = vars::aa.choke_mode->get<int>() ? vars::aa.choke_limit->get<int>() : vars::aa.fake->get<bool>() && vars::aa.enabled->get<bool>() ? 1 : 0;
		const auto max_choke = std::min( choke_limit, clamp( sv_maxusrcmdprocessticks - 1 - tickbase::compute_current_limit(), 0, sv_maxusrcmdprocessticks ) );
		if ( vars::aim.extend_peek->get<bool>() && !doubletap_ready && !*globals::send_packet && !weapon->is_knife() && !vars::misc.in_peek_assist() && local_player->get_velocity().Length() > 5.f && interfaces::client_state()->chokedcommands < max_choke - 4 && last_target < 1 /*&& !antiaim::check_server_hittable()*/ ) // only delay shot without dt on as you can adjust autostop for more ticks
			return;

		const auto should_delay = weapon->get_weapon_id() != WEAPON_REVOLVER && vars::aim.extend_peek->get<bool>() && doubletap_ready && !weapon->is_knife() && local_player->get_velocity().Length() > 30.f && last_target < 1;
		if ( !vars::misc.in_peek_assist() && should_delay && animations::lag.first - local_player->get_tickbase() > 5 /*&& !antiaim::check_server_hittable()*/ ) // prevent shooting while in peek exploit to get better shot
			return;
	}

	auto target_command = globals::current_cmd->command_number - std::min( sv_maxusrcmdprocessticks / 2 - 1, time_to_ticks( interfaces::globals()->curtime - weapon->get_next_primary_attack() ) ); // closest to standing in fakeduck
	if ( !vars::aim.fake_duck->get<bool>() || target_command <= globals::first_cmd->command_number )
		target_command = globals::current_cmd->command_number;

	const auto initial_stop = vars::misc.in_peek_assist() ? stop_type::stop_full : stop_type::stop_slide;

	CUserCmd command{};
	if ( aimbot_helpers::get_current_autostop() )
	{
		const auto cmd = &interfaces::input()->m_pCommands[ target_command % 150 ];
		command = *cmd;

		if ( initial_stop == stop_type::stop_full )
			aimbot_helpers::stop_to_speed( 0.f, cmd );
		else
			aimbot_helpers::slide_stop( cmd );

		prediction::run_prediction( cmd->command_number );
	}

	keep_stop = stop_type::stop_none;

	const auto old_weapon = weapon;
	prediction::run_prediction( target_command );
	weapon = local_weapon;
	if ( old_weapon != weapon )
	{
		targets.clear();
		last_target = -1;
		keep_stop = stop_type::stop_none;
		return;
	}

	player_log::filter_records();
	stop = scan_targets( &interfaces::input()->m_pCommands[ target_command % 150 ] );
	if ( has_shot )
		stop = initial_stop;

	if ( initial_stop != stop && command.command_number > 0 )
	{
		const auto cmd = &interfaces::input()->m_pCommands[ command.command_number % 150 ];
		const auto buttons = cmd->buttons;
		*cmd = command;
		cmd->buttons = buttons;

		if ( stop != stop_type::stop_none )
		{
			if ( stop == stop_type::stop_full )
				aimbot_helpers::stop_to_speed( 0.f, cmd );
			else
				aimbot_helpers::slide_stop( cmd );
		}

		prediction::run_prediction( cmd->command_number );
	}

	keep_stop = stop;
}

void aimbot::calculate_hittable_enemies()
{
	if ( vars::legit_enabled() )
		return;

	std::vector<penetration::awall_trace_t> points;

	for ( const auto player : interfaces::entity_list()->get_players() )
	{
		if ( !player->get_alive() || !player->is_enemy() || local_player == player || player->get_immunity() || player->IsDormant() /*|| player->get_render_clr() != local_player->get_render_clr()*/ )
			continue;

		auto& log = player_log::get_log( player->EntIndex() );
		if ( log.record.empty() || log.record.back().m_dormant || log.m_enter_pvs )
			continue;

		log.is_hittable = hitscan::check_enemy_hittable( player );
	}

#if defined(ALPHA) || !defined(RELEASE)
	for ( const auto player : interfaces::entity_list()->get_players() )
	{
		auto& log = player_log::get_log( player->EntIndex() );
		if ( log.record.empty() || log.record.back().m_dormant )
		{
			visuals::players[ player->EntIndex() ].debug = {};
			visuals::players[ player->EntIndex() ].debug.reason = XOR_STR( "NO RECORDS" );
		}
		else if ( log.m_enter_pvs )
		{
			visuals::players[ player->EntIndex() ].debug = {};
			visuals::players[ player->EntIndex() ].debug.reason = XOR_STR( "ENTER PVS" );
		}
		else if ( !log.is_hittable )
		{
			visuals::players[ player->EntIndex() ].debug = {};
			visuals::players[ player->EntIndex() ].debug.reason = XOR_STR( "NOT HITTABLE" );
		}

	}
#endif
}

void aimbot::select_targets()
{
	QAngle angles{};
	interfaces::engine()->GetViewAngles( angles );
	angles.x = 0.f;

	for ( auto i = 1; i <= interfaces::globals()->maxClients; i++ )
	{
		const auto player = globals::get_player( i );
		if ( !player || !player->get_alive() || !player->is_enemy() || local_player == player || player->get_immunity() || player->IsDormant() )
		{
			targets.erase( std::ranges::remove_if( targets, [i] ( const target_data_t& target ) { return target.index == i; } ).begin(), targets.end() );
			continue;
		}

		auto& log = player_log::get_log( i );
		if ( log.record.empty() || log.record.back().m_dormant || log.m_enter_pvs || !log.is_hittable )
		{
			targets.erase( std::ranges::remove_if( targets, [i] ( const target_data_t& target ) { return target.index == i; } ).begin(), targets.end() );
			continue;
		}

		if ( vars::aim.fov->get<float>() <= 179.f )
		{
			auto forward = player->get_origin() - local_player->get_abs_origin();
			QAngle player_ang;
			math::vector_angles( forward, player_ang );
			if ( fabsf( math::normalize_float( angles.y - player_ang.y ) ) > vars::aim.fov->get<float>() )
			{
				targets.erase( std::ranges::remove_if( targets, [i] ( const target_data_t& target ) { return target.index == i; } ).begin(), targets.end() );
				continue;
			}
		}

		target_data_t* elem = nullptr;
		const auto& find_res = std::ranges::find_if( targets, [i] ( const target_data_t& target ) { return target.index == i; } );
		if ( find_res == targets.end() )
			elem = &targets.emplace_back( i );
		else
			elem = &*find_res;

		elem->set( player );
	}
}

void aimbot::get_best_target( final_target_t& final_target )
{
	cycle_data.tick_targets.clear();

	const auto find_res = std::ranges::find_if( targets, [&] ( const target_data_t& target ) { return target.player == cycle_data.last_shootable_target; } );
	if ( find_res != targets.end() )
		cycle_data.tick_targets.push_back( &*find_res );
	else
		cycle_data.last_shootable_target = nullptr;


	const auto target_amount = vars::aim.low_perf_targets->get<bool>() ? 1 : cycle_data.last_shootable_target ? 2 : 1;
	for ( auto i = 0u; i < targets.size() && cycle_data.tick_targets.size() < target_amount; i++ )
	{
		if ( targets[ i ].player == cycle_data.last_shootable_target || std::ranges::find( cycle_data.last_targets, targets[ i ].player ) != cycle_data.last_targets.end() )
			continue;

		cycle_data.tick_targets.push_back( &targets[ i ] );
	}

	if ( cycle_data.tick_targets.empty() || cycle_data.last_shootable_target && cycle_data.tick_targets.size() == 1 )
	{
		cycle_data.last_targets.clear();

		for ( auto i = 0u; i < targets.size() && cycle_data.tick_targets.size() < target_amount; i++ )
		{
			if ( targets[ i ].player != cycle_data.last_shootable_target )
				cycle_data.tick_targets.push_back( &targets[ i ] );
		}
	}

	cycle_data.last_shootable_target = nullptr;
	std::vector<final_target_t> final_targets{};
	const auto config = config::current_config( local_weapon );

	static auto calc_target = [config] ( target_data_t* target ) -> std::vector<final_target_t>
	{
		std::vector<lag_record_t*> scan_records{};
		std::vector<final_target_t> out{};

		get_scan_records( target->player, scan_records );

		for ( auto it = scan_records.begin(); it != scan_records.end(); ++it )
		{
			auto& record = **it;

			record.setup_matrices();

			aimpoint_t point = {};
#if defined(ALPHA) || !defined(RELEASE)
			visuals::players[ record.m_index ].debug.reason = {};
#endif
			if ( !hitscan::get_best_hitpoint( target, record, point ) )
				continue;

			out.emplace_back( target->player, *it, point );
		}

		if ( !scan_records.empty() && out.empty() && config->predictive_autostop->get<bool>() && !vars::aim.fake_duck->get<bool>() && prediction::unpred_vel.Length2D() > 20.f )
		{
			aimpoint_t point = {};
			if ( !hitscan::get_best_hitpoint( target, *scan_records.front(), point, nullptr, local_player->get_eye_pos() + prediction::unpred_vel * interfaces::globals()->interval_per_tick * 3.f ) )
				return out;

			out.emplace_back( target->player, scan_records.front(), point, true );
			return out;
		}

#if defined(ALPHA) || !defined(RELEASE)
		visuals::players[ target->index ].debug.reason = {};
#endif
		return out;
	};

	for ( auto it = cycle_data.last_targets.begin(); it != cycle_data.last_targets.end(); )
	{
		if ( std::ranges::find_if( targets, [it] ( const target_data_t& target ) { return target.player == *it; } ) == targets.end() )
		{
			it = cycle_data.last_targets.erase( it );
			continue;
		}

		++it;
	}

#if defined(ALPHA) || !defined(RELEASE)
	for ( const auto& target : cycle_data.tick_targets )
	{
		visuals::players[ target->index ].debug.scan = true;
		visuals::players[ target->index ].debug.reason.clear();
	}
#endif

	for ( const auto target : cycle_data.tick_targets )
	{
		cycle_data.last_targets.push_back( target->player );
		target->hitboxes.clear();
		std::ranges::copy( calc_target( target ), std::back_inserter( final_targets ) );
	}

	for ( const auto target : cycle_data.tick_targets )
	{
		auto& log = player_log::get_log( target->index );

		if ( log.need_extrapolate )
		{
			log.need_extrapolate = false;

			if ( !lagcomp::run_extrapolation( target->player ) )
				continue;

#ifdef _DEBUG
			if ( vars::aim.anti_exploit->get<bool>() )
				player_log::anim_debug_info[ target->player->EntIndex() ].get_or_put( time_to_ticks( log.extrapolated_record.m_simtime ) ).emplace_client( log.extrapolated_record.matrix( log.get_dir( &log.extrapolated_record ) ) );
#endif

			lag_record_t* extra = nullptr;
			if ( log.extrapolated_record.m_extrapolate_amt > 0 && log.record.size() > 1 && !log.record[ log.record.size() - 2 ].m_dormant )
			{
				extra = &log.record[ log.record.size() - 2 ];

				const auto backup_origin = extra->m_origin;
				extra->m_origin = log.extrapolated_record.m_origin;
				for ( auto& state : extra->m_state )
					state.m_setup_tick = -1;
				extra->setup_matrices();
				for ( auto& state : extra->m_state )
					state.m_setup_tick = -1;
				extra->m_origin = backup_origin;
			}

			aimpoint_t point = {};
			if ( !hitscan::get_best_hitpoint( target, log.extrapolated_record, point, extra ) )
			{
				if ( config->predictive_autostop->get<bool>() && final_targets.empty() && !vars::aim.fake_duck->get<bool>() && prediction::unpred_vel.Length2D() > 20.f )
				{
					point = {};
					if ( hitscan::get_best_hitpoint( target, log.extrapolated_record, point, extra, local_player->get_eye_pos() + prediction::unpred_vel * interfaces::globals()->interval_per_tick * 3.f ) )
						final_targets.emplace_back( final_target_t{ target->player, &log.extrapolated_record, point, true } );
				}

				continue;
			}

			final_targets.emplace_back( final_target_t{ target->player, &log.extrapolated_record, point } );
		}
	}

	for ( const auto& target : final_targets )
	{
		if ( target.point.damage < 1.f )
			continue;

		if ( !final_target.record )
		{
			final_target = target;
			continue;
		}

		if ( final_target.predictive && !target.predictive )
		{
			final_target = target;
			continue;
		}

		const auto is_singleshot = weapon->get_weapon_id() == WEAPON_SSG08 || weapon->get_weapon_id() == WEAPON_AWP || weapon->get_weapon_id() == WEAPON_REVOLVER || misc::retract_peek;
		const auto doubletap_ready = tickbase::fast_fire && !is_singleshot && tickbase::compute_current_limit() > 2;

		const auto health = target.player->get_predicted_health() + ( target.point.penetration_points.size() > 1 ? 5 : 0 );
		const auto health_aimpoint = target.player->get_predicted_health() + ( final_target.point.penetration_points.size() > 1 ? 5 : 0 );

		const auto shots_to_kill = static_cast< int >( ceilf( health / fminf( target.point.damage * ( doubletap_ready ? 2.f : 1.f ), health ) ) );
		const auto shots_to_kill_aimpoint = static_cast< int >( ceilf( health_aimpoint / fminf( final_target.point.damage * ( doubletap_ready ? 2.f : 1.f ), health_aimpoint ) ) );

		if ( final_target.record && target.record && target.record->m_index == last_target && globals::nospread ) // cycle in nospread
			continue;

		if ( target.point.safety >= penetration::safety_no_roll && ( final_target.point.safety < penetration::safety_no_roll || shots_to_kill < shots_to_kill_aimpoint ) )
		{
			final_target = target;
			continue;
		}

		if ( final_target.record && target.record && shots_to_kill_aimpoint == shots_to_kill && ( target.point.safety >= penetration::safety_no_roll && final_target.point.safety >= penetration::safety_no_roll || target.point.safety < penetration::safety_no_roll && final_target.point.safety < penetration::safety_no_roll || final_target.point.safety == target.point.safety ) )
		{
			if ( target.record->m_server_tick > final_target.record->m_server_tick )
			{
				final_target = target;
				continue;
			}
		}

		if ( shots_to_kill < shots_to_kill_aimpoint )
			final_target = target;
	}
}

aimbot::stop_type aimbot::scan_targets( CUserCmd* cmd )
{
	final_target_t target{};

	hitscan::filter_hitboxes();
	get_best_target( target );

	if ( !target.player )
		scan_dormant_targets( target );

	if ( !target.player )
	{
		last_target = -1;
		return stop_type::stop_none;
	}

#if defined(ALPHA) || !defined(RELEASE)
	visuals::players[ target.player->EntIndex() ].debug.reason = XOR_STR( "TARGETING" );
	visuals::players[ target.player->EntIndex() ].debug.dmg = target.point.damage;
	visuals::players[ target.player->EntIndex() ].debug.sp = target.point.safety >= penetration::safety_no_roll;
	visuals::players[ target.player->EntIndex() ].debug.vsp = target.point.safety == penetration::safety_full;
#endif

	const auto full_stop = vars::misc.in_peek_assist() && !weapon->is_knife();

	cycle_data.last_shootable_target = target.player;

	const auto is_knife = weapon->is_knife();
	if ( !is_knife )
		aimbot_helpers::check_corner_hitpoint( target );

	const auto max_hitchance = aimbot_helpers::check_max_hitchance( target );
	if ( !max_hitchance )
	{
#if defined(ALPHA) || !defined(RELEASE)
		visuals::players[ target.player->EntIndex() ].debug.hc = target.point.hitchance;
		visuals::players[ target.player->EntIndex() ].debug.reason = XOR_STR( "MAX HITCHANCE" );
#endif
		return stop_type::stop_none;
	}

	if ( max_hitchance && config::current_config( weapon )->autoscope->get<bool>() && !globals::nospread && !weapon->get_zoom_level() && ( weapon->get_weapon_id() == WEAPON_SSG08 || weapon->get_weapon_id() == WEAPON_AWP || weapon->get_weapon_id() == WEAPON_SCAR20 || weapon->get_weapon_id() == WEAPON_G3SG1 || weapon->get_weapon_id() == WEAPON_AUG || weapon->get_weapon_id() == WEAPON_SG556 ) )
	{
		cmd->buttons |= IN_ATTACK2;
		return aimbot_helpers::should_autostop( target ) ? stop_type::stop_slide : stop_type::stop_none;
	}

	if ( target.predictive )
		return aimbot_helpers::should_autostop( target ) ? stop_type::stop_slide : stop_type::stop_none;

	//interfaces::debug_overlay()->AddBoxOverlay( target.point.point, Vector( 0.1f, 0.1f, 0.1f ), Vector( -0.1f, -0.1f, -0.1f ), Vector( 0.f, 0.f, 0.f ), 0.f, 255.f, 0.f, 255.f, 0.1f );

#if defined(ALPHA) || !defined(RELEASE)
	visuals::players[ target.player->EntIndex() ].debug.dmg = target.point.damage;
	visuals::players[ target.player->EntIndex() ].debug.sp = target.point.safety >= penetration::safety_no_roll;
	visuals::players[ target.player->EntIndex() ].debug.vsp = target.point.safety == penetration::safety_full;
#endif

	if ( !aimbot_helpers::check_hitchance( target ) && !prediction::has_shot() ) // let autostop run ( dont do any autostopping in fakeduck )
	{
#if defined(ALPHA) || !defined(RELEASE)
		visuals::players[ target.player->EntIndex() ].debug.hc = target.point.hitchance;
		visuals::players[ target.player->EntIndex() ].debug.reason = XOR_STR( "HITCHANCE" );
#endif
		return aimbot_helpers::should_autostop( target ) ? full_stop ? stop_type::stop_full : stop_type::stop_slide : stop_type::stop_none;
	}

#if defined(ALPHA) || !defined(RELEASE)
	visuals::players[ target.player->EntIndex() ].debug.hc = target.point.hitchance;
#endif
	if ( target.record->breaking_lc() && target.record->can_delay_shot() && target.record->delay_shot() ) // make sure not to shoot delay shotted tick when we cant even shoot at it, we just want to autostop
	{
#if defined(ALPHA) || !defined(RELEASE)
		visuals::players[ target.player->EntIndex() ].debug.reason = XOR_STR( "DELAY SHOT" );
#endif
		return full_stop ? stop_type::stop_full : stop_type::stop_slide;
	}

	if ( weapon->get_weapon_id() == WEAPON_REVOLVER && !aimbot_helpers::autorevolver( weapon ) )
	{
#if defined(ALPHA) || !defined(RELEASE)
		visuals::players[ target.player->EntIndex() ].debug.reason = XOR_STR( "AUTOREVOLVER" );
#endif
		return full_stop ? stop_type::stop_full : stop_type::stop_slide;
	}

	if ( cmd->buttons & IN_ATTACK2 && !is_knife && weapon->get_weapon_id() != WEAPON_REVOLVER ) // scope fix
	{
#if defined(ALPHA) || !defined(RELEASE)
		visuals::players[ target.player->EntIndex() ].debug.reason = XOR_STR( "SCOPING" );
#endif
		return stop_type::stop_slide;
	}

	if ( !is_knife && weapon->get_weapon_id() != WEAPON_TASER && ( !prediction::can_fire() || !prediction::has_shot() && !vars::aim.autofire->get<bool>() ) )
	{
#if defined(ALPHA) || !defined(RELEASE)
		visuals::players[ target.player->EntIndex() ].debug.reason = XOR_STR( "CANT FIRE" );
#endif
		return stop_type::stop_slide;
	}

	prediction::run_prediction( cmd->command_number );

	weapon = local_weapon;
	if ( !weapon )
		return stop_type::stop_none;

	const auto aim_angles = math::calc_angle( local_player->get_eye_pos(), target.point.point );
	const auto tickcount = lagcomp::fix_tickcount( target.record->m_simtime );
	target.record->m_shot_dir = player_log::get_log( target.record->m_index ).get_dir( target.record );
	auto shot = resolver::shot_t( local_player->get_eye_pos(), cmd->command_number, interfaces::globals()->tickcount, false, target.point.penetration_points, target.player->EntIndex(), target.record, target.point.hitgroup, target.point.point, target.point.damage, weapon->get_inaccuracy(), target.point.hitchance, local_player->get_aim_punch(), target.point.safety );

	auto stab = false;
	auto run_knife = false;
	if ( is_knife )// autoknife
	{
		run_knife = true;
		if ( !knifebot::trace_knife( shot.hitpos, &shot.record, stab ) )
		{
#if defined(ALPHA) || !defined(RELEASE)
			visuals::players[ target.player->EntIndex() ].debug.reason = XOR_STR( "KNIFEBOT" );
#endif
			return stop_type::stop_none;
		}
	}

	shot.record.m_shot_info.hitchance = target.point.hitchance;
	shot.record.m_shot_info.target_dmg = target.point.damage;
	shot.record.m_shot_info.safety = target.point.safety;
	shot.record.m_shot_info.extra_info = target.point.extra_info;

	*globals::send_packet = !tickbase::force_choke;
	if ( !*globals::send_packet )
		globals::choked_shot_command = cmd->command_number;

	if ( vars::misc.peek_assist->get<bool>() )
		misc::retract();

	last_target = shot.record.m_index;

	cmd->tick_count = tickcount;
	cmd->viewangles = aim_angles;

	const auto secondary = run_knife && stab;
	if ( cmd == globals::current_cmd )
		secondary ? prediction::take_secondary_shot() : prediction::take_shot();
	else
	{
		cmd->buttons |= secondary ? IN_ATTACK2 : IN_ATTACK;
		globals::choked_shot_command = cmd->command_number;
	}

#if defined(ALPHA) || !defined(RELEASE)
	visuals::players[ target.player->EntIndex() ].debug.reason = XOR_STR( "SHOOTING" );
#endif

	globals::shot_command = cmd->command_number;

	add_shot( shot );

	has_shot = true;

	//util::print_dev_console( true, Color::Lime(), "%.0f %.0f\n", target.point.hitchance, target.point.hitchance_roll );

	//aimbot_helpers::draw_debug_hitboxes( target.player, shot.record.matrix( target.record->m_shot_dir ), -1, 4.f, Color::Lime() );

	return stop_type::stop_slide; // irrelevant return value as its not altered when the aimbot shot
}

void aimbot::manual_shot()
{
	weapon = local_weapon;
	if ( !weapon || !weapon->is_shootable() && !weapon->is_secondary_attack_weapon() )
		return;

	if ( has_shot || !prediction::has_shot() )
		return;

	if ( tickbase::to_shift )
	{
		prediction::take_shot( false );
		return;
	}

	prediction::run_prediction( globals::current_cmd->command_number );

	if ( weapon->get_weapon_id() == WEAPON_REVOLVER && !aimbot_helpers::autorevolver( weapon ) )
		return;

	aimbot_helpers::no_recoil( globals::current_cmd );

	var( weapon_recoil_scale );
	auto angles = globals::current_cmd->viewangles + local_player->get_aim_punch() * weapon_recoil_scale->get_float();
	angles.Clamp();

	aimpoint_t aimpoint{};
	aimpoint.hitbox = -2;
	Vector forward{};
	math::angle_vectors( angles, &forward );

	int damage;
	if ( legit::shot_record )
	{
		auto& log = player_log::get_log( legit::shot_index );

		legit::shot_record->m_shot_dir = log.get_dir( legit::shot_record );

		aimpoint.point = local_player->get_eye_pos() + forward * legit::shot_record->m_origin.Dist( local_player->get_eye_pos() );
		can_hit( local_player, penetration::pen_data( legit::shot_record, legit::shot_record->m_shot_dir ), local_player->get_eye_pos(), &aimpoint, damage, true );
	}
	else
	{
		aimpoint.point = local_player->get_eye_pos() + forward * local_weapon->get_wpn_data()->range;
		can_hit( local_player, penetration::pen_data(), local_player->get_eye_pos(), &aimpoint, damage );
	}

	aimpoint.damage = damage;
	auto shot = resolver::shot_t( local_player->get_eye_pos(), globals::current_cmd->command_number, interfaces::globals()->tickcount, prediction::had_secondary_attack, aimpoint.penetration_points, legit::shot_index, legit::shot_record, -1, aimpoint.point, aimpoint.damage, weapon->get_inaccuracy(), aimpoint.hitchance );

	globals::shot_command = globals::current_cmd->command_number;

	add_shot( shot );

	if ( !vars::aim.fake_duck->get<bool>() )
		*globals::send_packet = true;

	if ( vars::misc.peek_assist->get<bool>() )
		misc::retract();
}

void aimbot::get_scan_records( C_CSPlayer* const player, std::vector<lag_record_t*>& ret, const bool ignore_extra )
{
	lag_record_t* uncrouched = nullptr;
	lag_record_t* shot = nullptr;
	lag_record_t* first = nullptr;
	lag_record_t* distant = nullptr;
	lag_record_t* angle_diff = nullptr;

	auto& log = player_log::get_log( player->EntIndex() );

	auto max_dist = 0.f;
	auto max_diff = 0.f;

	auto extrap = true;
	auto breaking_lc = false;
	const auto using_exploit = log.record.back().m_simtime < log.m_highest_simtime&& log.record.back().m_velocity.Length2DSqr() > 240.f * 240.f;

	for ( auto i = log.record.size() - 1; i >= 0; i-- )
	{
		auto& record = log.record[ i ];

		if ( !ConVar::cl_lagcompensation || !ConVar::cl_predict || breaking_lc )
			break;

		if ( record.m_ignore )
			continue;

		breaking_lc = record.breaking_lc();

		if ( record.m_lagamt > 17 || !record.m_valid )
			continue;

		if ( ( record.breaking_lc() || using_exploit ) && record.can_delay_shot() && record.delay_shot() )
		{
			extrap = false;
			break;
		}

		if ( record.breaking_lc() && !record.can_delay_shot() )
			break;

		auto add_shot = [&] ( lag_record_t*& into )
		{
			into = &record;
			into->m_shot_info.has_info = true;
			into->m_shot_info.breaking_lc = record.breaking_lc();
			into->m_shot_info.delay_shot = into->m_shot_info.breaking_lc && record.can_delay_shot();
			into->m_shot_info.extrapolated = false;
			into->m_shot_info.backtrack_ticks = time_to_ticks( log.m_highest_simtime - record.m_simtime );
		};

		if ( !first )
		{
			add_shot( first );

			if ( record.m_duckamt < 0.1f )
				uncrouched = first;

			if ( record.m_shot )
				shot = first;

			continue;
		}

		if ( !uncrouched && record.m_duckamt < 0.1f )
		{
			add_shot( uncrouched );

			continue;
		}

		if ( !shot && record.m_shot )
		{
			add_shot( shot );

			continue;
		}

		const auto dist = record.m_origin.Dist( first->m_origin );
		if ( dist > 8.f && dist > max_dist )
		{
			max_dist = dist;
			add_shot( distant );
		}

		const auto diff = fabsf( math::normalize_float( record.m_eye_angles.y - first->m_eye_angles.y ) );
		if ( diff > 8.f && diff > max_diff )
		{
			max_diff = diff;
			add_shot( angle_diff );
		}
	}

	if ( first )
		ret.emplace_back( first );

	if ( shot && first != shot && !ignore_extra )
		ret.emplace_back( shot );

	if ( distant && first != distant )
		ret.emplace_back( distant );

	if ( angle_diff && first != angle_diff && !vars::aim.low_perf_scan_accuracy->get<bool>() )
		ret.emplace_back( angle_diff );

	if ( uncrouched && first != uncrouched && !ignore_extra && !vars::aim.low_perf_scan_accuracy->get<bool>() )
		ret.emplace_back( uncrouched );

#if defined(ALPHA) || !defined(RELEASE)
	visuals::players[ player->EntIndex() ].debug.extrapolate = false;

#endif

	if ( log.record.empty() || log.record.back().m_dormant || first || !extrap )
	{
#if defined(ALPHA) || !defined(RELEASE)
		if ( !first && !ignore_extra )
			visuals::players[ player->EntIndex() ].debug.reason.clear();
#endif

		return;
	}

#if defined(ALPHA) || !defined(RELEASE)
	visuals::players[ player->EntIndex() ].debug.extrapolate = true;
#endif
	//if ( !log.m_last_valid || ConVar::cl_lagcompensation && ConVar::cl_predict && interfaces::client_state()->get_last_server_tick() - log.m_last_valid > time_to_ticks( 1.f ) )

	log.need_extrapolate = true;
}

void aimbot::fix_shot( CUserCmd* const cmd )
{
	if ( globals::shot_command != cmd->command_number )
		return;

	if ( resolver::shots.empty() || !weapon )
		return;

	prediction::run_prediction( cmd->command_number );

	auto& shot = resolver::shots.back();
	const auto manual_shot = shot.hitgroup == -1;

	animations::restore_animation( globals::shot_command - 1 );
	interfaces::prediction()->SetLocalViewAngles( cmd->viewangles );

	shot_record.~lag_record_t();
	new( &shot_record ) lag_record_t( local_player );
	shot_record.m_state[ resolver_direction::resolver_networked ].m_poses[ 12 ] = cmd->viewangles.x / 178.f + 0.5f;
	shot_record.m_state[ resolver_direction::resolver_networked ].m_abs_ang = { 0.f, local_player->get_anim_state()->foot_yaw, 0.f };

	const auto orig_shotpos = shot.shotpos;
	shot_record.m_shot = true;
	shot.original_shotpos = shot.shotpos = local_player->get_eye_pos();
	shot_record.m_shot = false;

	const auto corrected_pos = shot.shotpos.Dist( orig_shotpos ) >= 0.001f;
	if ( corrected_pos && !weapon->is_secondary_attack_weapon() )
	{
		if ( !manual_shot )
		{
			const auto shot_angle = math::calc_angle( shot.shotpos, shot.hitpos );
			Vector forward{};
			math::angle_vectors( shot_angle, &forward );
			math::fast_vec_normalize( forward );

			aimpoint_t point{};
			point.point = shot.hitpos + forward * 10.f;
			point.hitbox = -1;

			auto damage = 0;

			const auto ret = can_hit( local_player, penetration::pen_data( &shot.record, shot.record.m_shot_dir, penetration::safety_none, nullptr, &shot.weapon_data ), shot.shotpos, &point, damage, true );

			shot.penetration_points = point.penetration_points;
			cmd->viewangles = shot_angle;

			if ( !ret )
			{
				cmd->buttons &= ~IN_ATTACK;
				prediction::take_shot( false );
				globals::shot_command = 0;
				cmd->viewangles = globals::current_cmd->viewangles;
				if ( !resolver::shots.empty() )
					resolver::shots.pop_back();
				tickbase::revert_shift_back();
				*globals::send_packet = true;
				return;
			}
		}
		else
		{
			var( weapon_recoil_scale );
			Vector forward{};
			const auto shot_angle = cmd->viewangles + local_player->get_aim_punch() * weapon_recoil_scale->get_float();
			math::angle_vectors( shot_angle, &forward );
			math::fast_vec_normalize( forward );

			aimpoint_t aimpoint{};
			aimpoint.point = shot.shotpos + forward * weapon->get_wpn_data()->range;

			auto damage = 0;
			can_hit( local_player, penetration::pen_data(), shot.shotpos, &aimpoint, damage );
			shot.penetration_points = aimpoint.penetration_points;
			shot.hitpos = shot.target_hitpos = aimpoint.point;
		}
	}

	if ( vars::misc.impacts->get<bool>() )
	{
		const auto col1 = Color( vars::misc.impacts_color->get<D3DCOLOR>() );
		for ( auto& point : shot.penetration_points )
			interfaces::debug_overlay()->AddBoxOverlay( point, Vector( -1.25f, -1.25f, -1.25f ), Vector( 1.25f, 1.25f, 1.25f ), QAngle( 0, 0, 0 ), col1.r(), col1.g(), col1.b(), 100, 4 );
	}

	if ( !manual_shot )
		aimbot_helpers::no_recoil( cmd );

	aimbot_helpers::remove_spread( cmd );

	if ( !globals::nospread )
		cmd->viewangles.Clamp();

	antiaim::fix_movement( cmd );
}

bool aimbot::has_dormant_target()
{
	if ( !vars::aim.dormant->get<bool>() )
		return false;

	if ( weapon->is_knife() )
		return false;

	const auto weapon = local_weapon;
	const auto max_range = weapon->get_wpn_data()->range;
	const auto origin = local_player->get_origin();

	for ( const auto player : interfaces::entity_list()->get_players() )
	{
		const auto i = player->EntIndex();
		if ( !player->get_alive() || !player->is_enemy() || local_player == player || player->get_immunity() || !player->IsDormant() || !visuals::players[ i ].valid )
			continue;

		auto& log = player_log::get_log( i );
		const auto sound_player = sound_esp::get_sound_player( i );
		if ( !log.m_has_matrix || !( interfaces::client_state()->get_last_server_tick() >= sound_player->last_update_tick && interfaces::client_state()->get_last_server_tick() < sound_player->last_update_tick + time_to_ticks( 5.f ) ) || log.m_dormant_misses > 1 )
			continue;

		if ( sound_player->origin.Dist( origin ) > max_range + 5.f )
			continue;

		return true;
	}

	return false;
}

void aimbot::scan_dormant_targets( final_target_t& final_target )
{
	std::vector<final_target_t> targets = {};
	const auto weapon = local_weapon;

	if ( !vars::aim.dormant->get<bool>() )
		return;

	if ( weapon->is_knife() )
		return;

	const auto eyepos = local_player->get_eye_pos();
	const auto max_range = weapon->get_wpn_data()->range;
	const auto origin = local_player->get_origin();

	for ( const auto player : interfaces::entity_list()->get_players() )
	{
		const auto i = player->EntIndex();
		if ( !player->get_alive() || !player->is_enemy() || local_player == player || player->get_immunity() || !player->IsDormant() || !visuals::players[ i ].valid )
			continue;

		auto& log = player_log::get_log( i );
		const auto sound_player = sound_esp::get_sound_player( i );
		if ( !log.m_has_matrix || !( interfaces::client_state()->get_last_server_tick() >= sound_player->last_update_tick && interfaces::client_state()->get_last_server_tick() < sound_player->last_update_tick + time_to_ticks( 5.f ) ) || log.m_dormant_misses > 1 )
			continue;

		if ( sound_player->origin.Dist( origin ) > max_range + 5.f )
			continue;

		auto& log_ = player_log::get_log( i );
		animations::move_matrix( log_.dormant_record.matrix( resolver_direction::resolver_networked ), log_.dormant_record.m_origin, sound_player->origin );

		const auto target_point = log_.dormant_record.m_origin + Vector( 0.f, 0.f, 48.f );
		final_target_t target = { player, &log_.dormant_record, aimpoint_t( target_point, target_point, HITBOX_BODY, HITGROUP_STOMACH ) };

		can_hit( local_player, penetration::pen_data( &log_.dormant_record, resolver_direction::resolver_networked ), eyepos, &target.point, target.point.damage, true );

		if ( target.point.damage > config::current_config( weapon )->get_mindmg( player ) )
		{
			final_target = target;
			return;
		}
	}
}