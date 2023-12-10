#include "../include_cheat.h"

void antiaim::run_lag()
{
	to_choke = 0;
	antiaim_called = 0;

	if ( ( *interfaces::game_rules_proxy() )->m_bFreezePeriod() )
		return;

	if ( tickbase::to_shift > 0 )
	{
		if ( vars::aa.enabled->get<bool>() )
			antiaim_called = globals::current_cmd->command_number;

		return;
	}
	if ( !vars::aa.enabled->get<bool>() && !vars::aa.choke_mode->get<int>() )
		return;

	if ( vars::aa.enabled->get<bool>() )
		antiaim_called = globals::current_cmd->command_number;

	fake_lag();
}

/*std::make_shared<combobox>( _w( "Mega lean type" ), vars::aa.lean_type, combobox_style::flags::none, std::vector<combo_item_t>
{
	{ _w( "None" ), 0 },
	{ _w( "Independent" ), 1 },
	{ _w( "Extend fake" ), 2 },
	{ _w( "Freestand" ), 3 },
	{ _w( "Freestand opposite" ), 4 },
	{ _w( "Jitter" ), 5 },
} ),*/

float antiaim::get_lean( float target_direction, float target_angle )
{
	if ( !vars::aa.lean_type->get<int>() )
		return 0.f;

	if ( vars::aa.lean_type->get<int>() == 1 )
		return vars::aa.lean_amount->get<float>();

	if ( vars::aa.lean_type->get<int>() == 2 )
		return target_direction == 0.f ? vars::aa.lean_amount->get<float>() : fabsf( vars::aa.lean_amount->get<float>() ) * -signum( target_direction );

	if ( vars::aa.lean_type->get<int>() == 6 )
		return target_direction == 0.f ? vars::aa.lean_amount->get<float>() : fabsf( vars::aa.lean_amount->get<float>() ) * signum( target_direction );

	if ( vars::aa.lean_type->get<int>() == 3 )
		return fake_direction == 0.f ? vars::aa.lean_amount->get<float>() : fabsf( vars::aa.lean_amount->get<float>() ) * -signum( math::normalize_float( fake_direction - target_angle ) );

	if ( vars::aa.lean_type->get<int>() == 4 )
		return fake_direction == 0.f ? vars::aa.lean_amount->get<float>() : fabsf( vars::aa.lean_amount->get<float>() ) * signum( math::normalize_float( fake_direction - target_angle ) );

	static bool flip = false;

	if ( vars::aa.lean_type->get<int>() == 5 )
	{
		flip = !flip;
		return flip ? vars::aa.lean_amount->get<float>() : -vars::aa.lean_amount->get<float>();
	}

	return 0.f;
}

void antiaim::run()
{
	fake_direction = 0.f;

	const auto is_defusing = globals::current_cmd->buttons & IN_USE && check_in_defuse_range();

	if ( !get_antiaim_state() )
		return;

	globals::random_seed( globals::current_cmd->command_number % 255 );

	auto yaw = globals::current_cmd->viewangles.y;
	if ( vars::aa.yaw->get<int>() == 3 && !is_defusing )
		yaw = globals::random_float( -180.f, 180.f );

	if ( vars::aa.yaw->get<int>() == 2 )
		yaw = 0;

	if ( vars::aa.yaw->get<int>() == 1 )
	{
		QAngle va{};
		interfaces::engine()->GetViewAngles( va );
		yaw = va.y + 180.f;
	}

	if ( vars::aa.at_target->get<bool>() && !is_defusing )
		do_at_target( yaw );

	if ( vars::aa.spin->get<bool>() && !is_defusing )
		do_spin( yaw );

	if ( vars::aa.jitter->get<bool>() && !is_defusing )
		do_jitter( yaw );

	if ( ( !vars::aa.edge->get<bool>() || !do_edge( yaw ) ) && !is_defusing )
		freestanding::get_real( yaw );

	in_flip = !in_flip;

	do_add( yaw );

	auto pitch = 0.f;
	var( weapon_recoil_scale );
	switch ( vars::aa.pitch->get<int>() )
	{
		case 1:
			pitch = !globals::nospread ? 89.f : -540.f - local_player->get_aim_punch().x * weapon_recoil_scale->get_float();
			break;
		case 2:
			pitch = !globals::nospread ? -89.f : 540.f - local_player->get_aim_punch().x * weapon_recoil_scale->get_float();
			break;
		case 3:
			pitch = !globals::nospread ? 0.f : 1080.f - local_player->get_aim_punch().x * weapon_recoil_scale->get_float();
			break;
		default:
			pitch = globals::current_cmd->viewangles.x;
			break;
	}

	target_angle = { pitch, math::normalize_float( yaw ), 0.f };
	//if ( vars::aa.pitch->get<int>() && !globals::nospread && in_flip ) // DO NOT REMOVE
	//	target_angle.x -= signum( pitch ) * 0.01f;
}


void antiaim::run_on_send()
{
	prediction::run_prediction( globals::current_cmd->command_number );

	run();
	auto player = local_player;

	const auto is_defusing = globals::current_cmd->buttons & IN_USE && check_in_defuse_range();

	prediction::run_prediction( interfaces::client_state()->lastoutgoingcommand );

	const auto to_animate = clamp( globals::current_cmd->command_number - globals::first_cmd->command_number, 0, 18 );
	auto target_direction = 0.f;

	if ( vars::aa.fake->get<bool>() )
		target_direction = vars::aa.fake_amount->get<float>();

	const auto diff = math::normalize_float( interfaces::input()->m_pCommands[ interfaces::client_state()->lastoutgoingcommand % 150 ].viewangles.y - target_angle.y );


	if ( vars::aa.freestand_fake->get<int>() && fake_direction != 0.f && !is_defusing )
	{
		if ( math::normalize_float( fake_direction - target_angle.y ) < 0.f )
			target_direction = -fabsf( target_direction );
		else
			target_direction = fabsf( target_direction );

		if ( vars::aa.freestand_fake->get<int>() == 2 )
			target_direction = -target_direction;
	}

	if ( in_flip && vars::aa.jitter->get<bool>() && vars::aa.flip_fake->get<bool>() && !is_defusing )
		target_direction = -target_direction;

	const auto freeze_period = ( *interfaces::game_rules_proxy() )->m_bFreezePeriod();

	for ( auto i = 0; i <= to_animate; i++ )
	{
		const auto cmd = &interfaces::input()->m_pCommands[ ( globals::first_cmd->command_number + i ) % 150 ];
		auto& info = prediction::get_pred_info( cmd->command_number );
		animations::restore_animation( cmd->command_number - 1 );
		interfaces::globals()->curtime += interfaces::globals()->interval_per_tick;

		const auto target_delta = ( target_direction / 100.f );

		auto [predicted, l] = animations::predict_animation_state( player );
		animations::local_record.m_yaw_modifier = animations::get_yaw_modifier( player, predicted.velocity, predicted.duck_amount, player->get_anim_state() );
		const auto min_max = target_delta > 0.f ? predicted.aim_yaw_max : predicted.aim_yaw_min;
		const auto min_max_inv = target_delta <= 0.f ? predicted.aim_yaw_max : predicted.aim_yaw_min;
		const auto body_yaw = min_max * animations::local_record.m_yaw_modifier * fabsf( target_delta );

		auto final_add = 0.f;
		if ( vars::aa.fake->get<bool>() && vars::aa.compensate_angle->get<float>() != 0.f )
			final_add = target_delta > 0.f ? 0.f : target_delta * min_max * 0.5f * vars::aa.compensate_angle->get<float>() * 0.01f;

		auto should_apply_antiaim = !freeze_period && cmd->command_number != globals::shot_command && vars::aa.enabled->get<bool>();

		if ( info.movetype == MOVETYPE_NOCLIP || info.movetype == MOVETYPE_LADDER || cmd->buttons & IN_USE && !check_in_defuse_range() )
			should_apply_antiaim = false;

		if ( should_apply_antiaim && std::get<bool>( check_knife_round() ) )
		{
			cmd->viewangles.y = std::get<float>( check_knife_round() );
			should_apply_antiaim = false;
		}

		const auto wpn = local_weapon;
		if ( wpn && wpn->is_grenade() && ( info.throwtime == 0.f || interfaces::globals()->curtime > info.throwtime ) && prediction::get_pred_info( cmd->command_number - 1 ).throwtime > 0.f )
		{
			should_apply_antiaim = false;
			/*if ( vars::misc.compensate_grenade->get<bool>() )
			{
				prediction::run_prediction( cmd->command_number );

				Vector direction{};
				math::angle_vectors( cmd->viewangles, &direction );

				const auto throw_strength = clamp< float >( wpn->get_throw_strength(), 0.f, 1.f );
				auto new_dir = direction * ( clamp< float >( wpn->get_wpn_data()->flthrowvelocity * 0.9f, 15.f, 750.f ) * ( throw_strength * 0.7f + 0.3f ) ) + local_player->get_velocity() * 1.25f;

				math::fast_vec_normalize( new_dir );

				QAngle new_angle{};
				math::vector_angles( new_dir, new_angle );
				cmd->viewangles.y += ( cmd->viewangles.y - new_angle.y );
			}*/
		}

		if ( antiaim_called && should_apply_antiaim )
			cmd->viewangles = target_angle + QAngle( 0, final_add, 0 );

		if ( Vector pos{}; should_apply_antiaim && cmd->buttons & IN_USE && check_in_defuse_range( &pos ) )
		{
			if ( vars::aa.enabled->get<bool>() )
			{
				const auto bomb_angles = math::calc_angle( player->get_eye_pos(), pos );
				if ( cmd->viewangles.x < 50.f || bomb_angles.x < 50.f )
					cmd->viewangles = bomb_angles;
			}

			should_apply_antiaim = false;
		}

		if ( should_apply_antiaim && i != to_animate && vars::aa.fake->get<bool>() )
		{
			auto intermediate = 0.f;

			if ( fabsf( target_delta ) <= .999f && fabsf( min_max * animations::local_record.m_yaw_modifier ) + fabsf( body_yaw ) + fabsf( min_max_inv * animations::local_record.m_yaw_modifier ) < 180.f )
			{
				intermediate = body_yaw;

				const auto delta = math::angle_diff( math::normalize_float( cmd->viewangles.y + intermediate ),
					player->get_anim_state()->foot_yaw );

				if ( fabsf( delta ) >= 2.5f )
					intermediate += ( delta > 0.f ? predicted.aim_yaw_max : predicted.aim_yaw_min ) * animations::local_record.m_yaw_modifier;
			}
			else if ( cmd->command_number % 2 != ( interfaces::client_state()->lastoutgoingcommand + interfaces::client_state()->chokedcommands + 1 ) % 2 )
			{
				static constexpr auto flop_tolerance = 6.f;
				static constexpr auto flip_range = -152.f;

				const auto opposite = math::normalize_float( flip_range - fabsf( min_max_inv ) );
				intermediate += opposite + ( fabsf( min_max_inv ) - fabsf( body_yaw ) ) * .75f;
				const auto delta = math::angle_diff( math::normalize_float( cmd->viewangles.y + intermediate * -signum( target_delta ) ), player->get_anim_state()->foot_yaw );

				if ( fabsf( delta ) <= opposite )
					intermediate = 180.f + -signum( target_delta ) * ( fabsf( min_max_inv ) + flop_tolerance );
				else
					intermediate *= -signum( target_delta );
			}

			cmd->viewangles.y = math::normalize_float( cmd->viewangles.y + intermediate );
		}

		const auto bak_choked = to_lag = interfaces::client_state()->chokedcommands; // fix lagamt for lua
		interfaces::client_state()->chokedcommands = i;
		run_antiaim = should_apply_antiaim;
		lua::api.callback( FNV1A( "on_run_command" ), [cmd] ( lua::state& s ) -> int
		{
			s.create_user_object_ptr( XOR_STR( "csgo.user_cmd" ), cmd );
			return 1;
		} );
		run_antiaim = false;
		interfaces::client_state()->chokedcommands = bak_choked;

		// holdaim
		if ( cmd->command_number > globals::shot_command && globals::shot_command >= globals::first_cmd->command_number )
			cmd->viewangles = interfaces::input()->m_pCommands[ globals::shot_command % 150 ].viewangles;

		cmd->viewangles.y = math::normalize_float( cmd->viewangles.y );
		if ( !globals::nospread )
			cmd->viewangles.Clamp();

		fix_movement( cmd );

		if ( ( i == to_animate || is_defusing ) && should_apply_antiaim && vars::aa.enabled->get<bool>() )
		{
			auto roll_dir = target_direction;
			if ( in_flip && vars::aa.jitter->get<bool>() && vars::aa.flip_lean->get<bool>() && !is_defusing )
				roll_dir = -roll_dir;

			const auto roll_amt = get_lean( roll_dir, target_angle.y );
			if ( vars::aa.ensure_lean->get<bool>() )
			{
				cmd->viewangles.z = roll_amt;

				fix_movement( cmd );

				current_lean = fabsf( cmd->viewangles.z );
			}
			else if ( fabsf( roll_amt ) > 0.f )
			{
				CMoveData orig_mv = interfaces::game_movement()->setup_move( player, cmd );
				CGameMovement::check_parameters( &orig_mv, player );
				CGameMovement::friction( &orig_mv, player );
				CGameMovement::walk_move( &orig_mv, player );

				for ( auto j = static_cast< int >( fabsf( roll_amt ) ); j >= 0; --j )
				{
					cmd->viewangles.z = j * signum( roll_amt );
					fix_movement( cmd );

					CMoveData mv = interfaces::game_movement()->setup_move( player, cmd );
					CGameMovement::check_parameters( &mv, player );
					CGameMovement::friction( &mv, player );
					CGameMovement::walk_move( &mv, player );

					if ( mv.m_vecVelocity.Length2D() >= orig_mv.m_vecVelocity.Length2D() - 0.000001f )
						break;
				}

				current_lean = fabsf( cmd->viewangles.z );
			}
		}
		else if ( i == to_animate )
			current_lean = 0.f;

		if ( info.movetype != MOVETYPE_LADDER && vars::aa.enabled->get<bool>() )
		{
			cmd->buttons &= ~( IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT | IN_BACK );
			if ( vars::aa.strafing->get<int>() == 0 )
			{
				if ( cmd->forwardmove > 0.f )
					cmd->buttons |= IN_FORWARD;
				else if ( cmd->forwardmove < 0.f )
					cmd->buttons |= IN_BACK;
				if ( cmd->sidemove > 0.f )
					cmd->buttons |= IN_MOVERIGHT;
				else if ( cmd->sidemove < 0.f )
					cmd->buttons |= IN_MOVELEFT;
			}
			else if ( vars::aa.strafing->get<int>() > 1 )
			{
				interfaces::prediction()->SetLocalViewAngles( cmd->viewangles );
				const auto [pred, l] = animations::predict_animation_state( player );
				Vector forward{}, right{}, up{};
				math::angle_vectors( QAngle( 0.f, pred.foot_yaw, 0.f ), &forward, &right, &up );
				right.NormalizeInPlace();
				const auto to_forward_dot = pred.last_accel_speed.Dot( forward );
				const auto to_right_dot = pred.last_accel_speed.Dot( right );

				const auto strafe_forward = to_forward_dot < -.55f;
				const auto strafe_backwards = to_forward_dot > .55f;
				const auto strafe_right = to_right_dot < -.63f;
				const auto strafe_left = to_right_dot > .63f;

				if ( cmd->command_number % 2 != ( interfaces::client_state()->lastoutgoingcommand + interfaces::client_state()->chokedcommands + 1 ) % 2 )
				{
					if ( player->get_anim_state()->strafe_sequence != -1 || player->get_anim_state()->strafe_change_weight <= 0.f )
						cmd->buttons |= strafe_forward ? IN_FORWARD : strafe_backwards ? IN_BACK : strafe_right ? IN_MOVERIGHT : strafe_left ? IN_MOVELEFT : 0;
				}
			}
		}

		aimbot::fix_shot( cmd );

		interfaces::input()->m_pVerifiedCommands[ cmd->command_number % 150 ].m_crc = *reinterpret_cast< int32_t* >( &interfaces::globals()->interval_per_tick );
		prediction::run_prediction( cmd->command_number );

		misc::write_tick( cmd->command_number );
	}

	animations::local_record.~lag_record_t();
	new( &animations::local_record ) lag_record_t( player );
	animations::local_record.m_simtime = interfaces::globals()->curtime;
	animations::local_record.m_state[ resolver_direction::resolver_networked ].m_abs_ang = { 0.f, animations::local_record.m_state[ resolver_direction::resolver_networked ].m_animstate.foot_yaw, 0.f };

	if ( player->get_tickbase() > animations::lag.first )
		animations::hittable_record = animations::local_record;

	/*if ( tickbase::skip_next_adjust )
	{
		bone_setup::handle_bone_setup( animations::hittable_record, resolver_networked );
		aimbot_helpers::draw_debug_hitboxes( local_player, animations::hittable_record.matrix( resolver_networked ), -1, 5.f, Color::Lime() );
	}*/

	//bone_setup::handle_bone_setup( animations::local_record, resolver_networked );
	//aimbot_helpers::draw_debug_hitboxes( local_player, animations::local_record.matrix( 0 ), -1, 0.2f, Color::LightBlue() );

	to_lag = 0; // lua
	in_fakelag = fakelag_next;
	fakelag_next = false;
	in_skip_animation = false;
	if ( !in_fakelag && player->get_tickbase() > animations::lag.first )
		in_onpeek = false;
}

void antiaim::fake_lag()
{
	static auto last_ducked = false;
	static Vector last_velocity{};

	if ( tickbase::force_choke )
	{
		to_choke = sv_maxusrcmdprocessticks;
		*globals::send_packet = false;
		return;
	}

	if ( tickbase::force_unchoke )
	{
		to_choke = 0;
		*globals::send_packet = true;
		return;
	}

	if ( tickbase::post_shift )
	{
		to_choke = clamp( sv_maxusrcmdprocessticks - 1 - tickbase::compute_current_limit(), 0, sv_maxusrcmdprocessticks );
		*globals::send_packet = interfaces::client_state()->chokedcommands >= to_choke;
		return;
	}

	const auto weapon = local_weapon;
	const auto velocity = local_player->get_velocity();

	auto can_skip_animation = false;

	const auto choke_limit = vars::aa.choke_mode->get<int>() ? vars::aa.choke_limit->get<int>() : vars::aa.fake->get<bool>() && vars::aa.enabled->get<bool>() ? 1 : 0;
	to_choke = std::min( choke_limit, 1 );

	const auto backup_animations = animations::animation_copy( 0 );

	animations::restore_animation( globals::current_cmd->command_number - 1 );
	const auto& c = *local_player->get_anim_state();

	if ( velocity.Length() >= 2.f )
	{
		auto last_vel = last_velocity;
		last_vel.NormalizeInPlace();
		auto new_vel = local_player->get_velocity();
		new_vel.NormalizeInPlace();
		const auto dot = new_vel.Dot( last_vel );

		can_skip_animation = ( dot > .1f && dot < .9995f );

		if ( vars::aa.fake->get<bool>() )
			can_skip_animation |= weapon && c.speed > weapon->get_max_speed() - 2.5f;
	}
	else if ( velocity.Length2D() >= 2.f && choke_limit > 2 && c.speed > weapon->get_max_speed() - 12.5f && tickbase::is_ready() )
		to_choke = 1;

	auto [p, l] = animations::predict_animation_state( local_player );

	if ( c.speed < 1.05f && p.speed > 1.05f )
		in_skip_animation = true;

	if ( fabsf( c.speed - p.speed ) > 10.f && c.speed < 100.f && !vars::aim.slide->get<bool>() )
		in_skip_animation = true;

	for ( auto i = 0u; i < local_player->get_anim_layers().size(); i++ )
	{
		const auto& current = local_player->get_anim_layers()[ i ];
		const auto& next = l[ i ];

		if ( can_skip_animation
			|| current.m_nSequence != next.m_nSequence
			|| fabsf( current.m_flCycle - next.m_flCycle ) > .1f
			|| !current.has_animation_completed() && next.has_animation_completed()
			|| fabsf( current.m_flPlaybackRate - next.m_flPlaybackRate ) > .01f )
		{
			in_skip_animation = true;
			break;
		}
	}

	last_velocity = local_player->get_velocity();

	backup_animations.restore( local_player );

	interfaces::prediction()->SetLocalViewAngles( globals::current_cmd->viewangles );

	if ( in_skip_animation )
		in_fakelag = true;

	if ( in_fakelag )
		to_choke = choke_limit;

	if ( vars::aa.choke_mode->get<int>() == 2 )
	{
		if ( ( local_player->get_origin() - resolver::last_origin ).Length2DSqr() > 4096.f )
			to_choke = 1;
	}

	const auto ducked = globals::current_cmd->buttons & IN_DUCK;
	if ( !in_onpeek && !in_fakelag && local_player->get_flags() & FL_ONGROUND && !ducked && last_ducked )
	{
		globals::current_cmd->buttons |= IN_DUCK;
		to_choke = 0;
		fakelag_next = true;
		started_fakelag = true;
	}
	last_ducked = globals::current_cmd->buttons & IN_DUCK;


	if ( !vars::aa.choke_mode->get<int>() && !vars::aa.enabled->get<bool>() )
		to_choke = 0;

	static auto notarget = false;
	const auto had_target = !notarget;
	if ( !in_onpeek && on_peek_fakelag( notarget ) && !had_target )
	{
		to_choke = 0;
		fakelag_next = true;
		started_fakelag = true;
		in_onpeek = true;
	}

	if ( ( tickbase::fast_fire || tickbase::hide_shot ) && tickbase::compute_current_limit() > 3 && ( !to_choke || interfaces::client_state()->chokedcommands ) && local_player->get_tickbase() >= animations::lag.first )
	{
		tickbase::skip_next_adjust = true;
		to_choke = 0;
	}

	to_choke = std::min( to_choke, clamp( sv_maxusrcmdprocessticks - 1 - tickbase::compute_current_limit(), 0, sv_maxusrcmdprocessticks ) );

	/*if ( ( tickbase::fast_fire || tickbase::hide_shot ) && globals::shot_command == globals::current_cmd->command_number - 1 && local_player->get_tickbase() <= animations::lag.first )
		to_choke = 0;*/

	*globals::send_packet = interfaces::client_state()->chokedcommands >= to_choke;
}

bool antiaim::started_peek_fakelag()
{
	auto weapon = local_weapon;
	if ( weapon && weapon->get_weapon_id() == WEAPON_REVOLVER )
	{
		started_fakelag = false;
		return false;
	}

	if ( !fakelag_next )
		started_fakelag = false;
	else
	{
		//aimbot_helpers::draw_debug_hitboxes( local_player, hitscan::local_record.matrix( resolver_direction::resolver_networked ), -1, 5.f, Color::LightBlue() );
		const auto ret = started_fakelag;
		started_fakelag = false;
		return ret;
	}

	return false;
}

void antiaim::fake_duck()
{
	static auto first_unduck = false;
	static auto first_duck = true;

	if ( tickbase::force_choke || tickbase::to_adjust || tickbase::compute_current_limit() > 0 )
		return;

	const auto to_choke = std::min( sv_maxusrcmdprocessticks - 1, 14 );

	if ( !vars::aim.fake_duck->get<bool>() )
	{
		if ( first_unduck && globals::current_cmd->buttons & IN_DUCK && local_player->get_duck_amt() != 1.f )
			*globals::send_packet = interfaces::client_state()->chokedcommands >= to_choke;
		else
			first_unduck = false;

		first_duck = true;
		return;
	}

	globals::current_cmd->buttons |= IN_DUCK;

	prediction::run_prediction( globals::current_cmd->command_number );

	if ( globals::restricted_unduck )
		return;

	fakeduck_called = interfaces::globals()->curtime;

	if ( first_duck && local_player->get_duck_amt() < 0.8f )
		return;

	if ( first_duck && interfaces::client_state()->chokedcommands )
	{
		*globals::send_packet = true;
		globals::current_cmd->buttons &= ~IN_DUCK;
		globals::current_cmd->buttons |= IN_BULLRUSH;

		prediction::run_prediction( globals::current_cmd->command_number );
		first_duck = false;
		return;
	}

	const auto needed_ticks = static_cast< int >( ceilf( ( 1.f - local_player->get_duck_amt() ) / ( interfaces::globals()->interval_per_tick * local_player->get_duck_speed() ) ) );
	if ( needed_ticks <= to_choke - interfaces::client_state()->chokedcommands + 1 )
		globals::current_cmd->buttons &= ~IN_DUCK;

	*globals::send_packet = interfaces::client_state()->chokedcommands >= to_choke;

	globals::current_cmd->buttons |= !( globals::current_cmd->buttons & IN_DUCK ) ? IN_BULLRUSH : 0;

	prediction::run_prediction( globals::current_cmd->command_number );

	first_duck = false;
	first_unduck = true;
}

__forceinline void normalize_move( float& forward, float& side, float& up )
{
	auto dir = Vector( forward, side, up );

	if ( dir.x > 450.f || dir.x < -450.f )
	{
		const auto old = dir.x;
		dir.x = clamp( dir.x, -450.f, 450.f );
		dir.y *= dir.x / old;
		dir.z *= dir.x / old;
	}

	if ( dir.y > 450.f || dir.y < -450.f )
	{
		const auto old = dir.y;
		dir.y = clamp( dir.y, -450.f, 450.f );
		dir.x *= dir.y / old;
		dir.z *= dir.y / old;
	}

	if ( dir.z > 320.f || dir.z < -320.f )
	{
		const auto old = dir.z;
		dir.z = clamp( dir.z, -320.f, 320.f );
		dir.x *= dir.z / old;
		dir.y *= dir.z / old;
	}

	forward = clamp( dir.x, -450.f, 450.f );
	side = clamp( dir.y, -450.f, 450.f );
	up = clamp( dir.z, -320.f, 320.f );
}

void antiaim::fix_movement( CUserCmd* cmd )
{
	auto& wish_angle = wish_angles[ cmd->command_number % 150 ];
	if ( cmd->viewangles == wish_angle )
		return;

	Vector view_fwd{}, view_right{}, view_up{}, cmd_fwd{}, cmd_right{}, cmd_up{};
	math::angle_vectors( wish_angle, &view_fwd, &view_right, &view_up );
	math::angle_vectors( cmd->viewangles, &cmd_fwd, &cmd_right, &cmd_up );

	if ( view_fwd[ 2 ] != 0 )
	{
		view_fwd[ 2 ] = 0;
		VectorNormalize( view_fwd );
	}

	if ( view_right[ 2 ] != 0 )
	{
		view_right[ 2 ] = 0;
		VectorNormalize( view_right );
	}

	if ( cmd_fwd[ 2 ] != 0 )
	{
		cmd_fwd[ 2 ] = 0;
		VectorNormalize( cmd_fwd );
	}

	if ( cmd_right[ 2 ] != 0 )
	{
		cmd_right[ 2 ] = 0;
		VectorNormalize( cmd_right );
	}

	const auto dir = view_fwd * cmd->forwardmove + view_right * cmd->sidemove;
	const auto denom = cmd_right.y * cmd_fwd.x - cmd_right.x * cmd_fwd.y;

	cmd->sidemove = ( cmd_fwd.x * dir.y - cmd_fwd.y * dir.x ) / denom;
	cmd->forwardmove = ( cmd_right.y * dir.x - cmd_right.x * dir.y ) / denom;

	normalize_move( cmd->forwardmove, cmd->sidemove, cmd->upmove );
	wish_angle = cmd->viewangles;


	//auto& wish_angle = wish_angles[ cmd->command_number % 150 ];
	//if ( cmd->viewangles == wish_angle )
	//	return;

	//Vector forward, right, up;
	//math::angle_vectors( wish_angle, &forward, &right, &up ); // Determine movement angles

	//// Copy movement amounts
	//auto const fmove = cmd->forwardmove;
	//auto const smove = cmd->sidemove;

	//forward.z = right.z = 0.f;

	//VectorNormalize( forward );
	//VectorNormalize( right );

	//const Vector wishvel( forward.x * fmove + right.x * smove, forward.y * fmove + right.y * smove, 0.f );

	//Vector dir_pair_forward, dir_pair_right;
	//math::angle_vectors( cmd->viewangles, &dir_pair_forward, &dir_pair_right );
	//dir_pair_forward = dir_pair_forward.To2D().Normalized();
	//dir_pair_right = dir_pair_right.To2D().Normalized();

	//Vector target_out_forward, target_out_right;
	//math::angle_vectors( QAngle( 0.f, wish_angle.y, 0.f ), &target_out_forward, &target_out_right );

	//auto scaled_forward = ( dir_pair_forward * wishvel );
	//auto scaled_right = ( dir_pair_right * wishvel );
	//cmd->forwardmove = scaled_forward.x * target_out_forward.y - scaled_forward.y * target_out_forward.x;
	//cmd->sidemove = ( scaled_right.x * target_out_right.y - scaled_right.y * target_out_right.x );

	//normalize_move( cmd->forwardmove, cmd->sidemove, cmd->upmove );
	//wish_angle = cmd->viewangles;
}


bool antiaim::on_peek_fakelag( bool& notarget )
{
	notarget = false;

	if ( check_hittable() )
	{
		//aimbot_helpers::draw_debug_hitboxes( local_player, hitscan::local_record.matrix( resolver_direction::resolver_networked ), -1, 1.f, Color::LightBlue() );
		return true;
	}
		

	notarget = true;

	return false;
}

void antiaim::do_spin( float& yaw )
{
	const auto speed = vars::aa.spin_speed->get<int>() / 60.f * ( 360.f / time_to_ticks( 1.f ) );

	static auto add = 0.f;

	if ( add > vars::aa.spin_range->get<int>() )
	{
		add = 0.f;
	}

	add += speed * ( 1 + interfaces::client_state()->chokedcommands );
	yaw += add - vars::aa.spin_range->get<int>() * 0.5f;
}

void antiaim::do_add( float& yaw )
{
	if ( !vars::aa.add_enabled->get<bool>() )
		return;

	const auto add = vars::aa.add->get<int>();

	yaw += add;
}

void antiaim::do_jitter( float& yaw )
{
	auto jitter_range = vars::aa.jitter_range->get<int>() * 0.5f;

	static auto add = 0.f;

	if ( vars::aa.jitter_random->get<bool>() )
		jitter_range = globals::random_float( -jitter_range, jitter_range );

	add = in_flip || vars::aa.jitter_random->get<bool>() ? jitter_range : -jitter_range;

	yaw += add;
}

bool antiaim::in_fakeduck()
{
	return fakeduck_called > interfaces::globals()->curtime - 0.4f && fakeduck_called < interfaces::globals()->curtime + 1.f;
}

bool antiaim::do_edge( float& yaw )
{
	return false;
}

void antiaim::do_at_target( float& yaw )
{
	C_CSPlayer* target = nullptr;
	QAngle target_angle{};

	QAngle original_viewangles{};
	interfaces::engine()->GetViewAngles( original_viewangles );

	auto lowest_fov = FLT_MAX;
	for ( const auto player : interfaces::entity_list()->get_players() )
	{
		if ( !player->get_alive() || !player->is_enemy() || player == local_player )
			continue;

		if ( player->IsDormant() && ( player->get_simtime() > interfaces::globals()->curtime || player->get_simtime() + 5.f < interfaces::globals()->curtime ) )
			continue;

		auto enemy_pos = player->get_origin();
		enemy_pos.z += 64.f;

		const auto angle = math::calc_angle( local_player->get_eye_pos(), enemy_pos );
		const auto fov = math::get_fov( original_viewangles, angle );

		if ( fov < lowest_fov )
		{
			target = player;
			lowest_fov = fov;
			target_angle = angle;
		}
	}

	if ( !target )
		return;

	yaw = math::normalize_float( target_angle.y + 180.f );
}

bool antiaim::check_hittable()
{
	static auto hittable = false;

	static auto last_checked = 0;

	if ( last_checked == globals::current_cmd->command_number || tickbase::force_choke )
		return hittable;

	last_checked = globals::current_cmd->command_number;

	hittable = false;

	hitscan::create_local_record( true );

	std::vector<C_CSPlayer*> players;
	for ( const auto player : interfaces::entity_list()->get_players() )
	{
		if ( !player->get_alive() || !player->is_enemy() || player == local_player )
			continue;

		auto& log = player_log::get_log( player->EntIndex() );
		if ( log.record.empty() )
			continue;

		if ( player->IsDormant() && !( interfaces::client_state()->get_last_server_tick() >= log.last_server_tick && interfaces::client_state()->get_last_server_tick() < log.last_server_tick + time_to_ticks( 5.f ) ) )
			continue;

		players.emplace_back( player );

		/*log.hitinfo.hittable = hitscan::check_hittable_by_enemy( player, true );
		log.hitinfo.unhittable = !log.hitinfo.hittable;

		if ( log.hitinfo.unhittable )
			continue;

		if ( ( log.hitinfo.hittable = hitscan::hitscan_self( player, true, hitscan::local_record ) ) )
			return hittable = true;*/
	}

	const auto calc_player = [] ( C_CSPlayer* player )
	{
		auto& log = player_log::get_log( player->EntIndex() );
		log.hitinfo.hittable = hitscan::check_hittable_by_enemy( player, true );
		log.hitinfo.unhittable = !log.hitinfo.hittable;

		if ( log.hitinfo.unhittable )
			return false;

		return log.hitinfo.hittable = hitscan::hitscan_self( player, true, hitscan::local_record );
	};

	std::vector<std::shared_ptr<detail::call_queue::queue_element>> calls;
	calls.reserve( players.size() );
	for ( auto player : players )
		calls.emplace_back( std::make_shared<detail::call_queue::queue_element>( [player, &calc_player] { calc_player( player ); } ) );

	detail::callqueue.perform( calls );

	for ( const auto player : players )
	{
		const auto& log = player_log::get_log( player->EntIndex() );
		if ( log.hitinfo.hittable )
			return hittable = true;
	}

	return hittable = false;
}

std::pair<bool, float> antiaim::check_knife_round()
{
	static auto last_checked = 0;
	static auto knife_round = false;
	static auto knife_ang = 0.f;

	if ( last_checked == globals::current_cmd->command_number )
		return { knife_round, knife_ang };

	last_checked = globals::current_cmd->command_number;

	prediction::run_prediction( globals::current_cmd->command_number );

	const auto local_has_knife = local_weapon && local_weapon->is_knife();
	knife_round = false;

	auto closest_dist = FLT_MAX;
	for ( const auto player : interfaces::entity_list()->get_players() )
	{
		if ( !player->get_alive() || !player->is_enemy() || local_player == player || player->IsDormant() )
			continue;

		const auto dist = player->get_origin().Dist( local_player->get_origin() );
		if ( dist > 600.f )
			continue;

		if ( !local_has_knife && dist > 200.f )
			continue;

		if ( dist < closest_dist )
		{

			closest_dist = dist;
			knife_ang = math::calc_angle( local_player->get_origin(), player->get_origin() ).y;
		}

		knife_round = true;

		const auto weapon = globals::get_weapon( player->get_active_weapon() );
		if ( weapon && !weapon->is_knife() )
		{
			knife_round = false;
			return { knife_round, 0.f };
		}
	}

	return { knife_round, knife_ang };
}

bool antiaim::check_in_defuse_range( Vector* pos )
{
	static auto last_checked = 0;
	static auto in_defuse_range = false;
	static Vector cached_pos = {};

	if ( last_checked == globals::current_cmd->command_number )
	{
		if ( pos )
			*pos = cached_pos;

		return in_defuse_range;
	}

	last_checked = globals::current_cmd->command_number;

	in_defuse_range = false;

	if ( local_player->get_team() != 3 )
		return false;

	for ( const auto bomb : *interfaces::entity_list() )
	{
		if ( bomb->GetClientClass()->m_ClassID != ClassId::CPlantedC4 || reinterpret_cast< C_PlantedC4* >( bomb )->get_defused() )
			continue;

		cached_pos = bomb->get_origin();
		if ( pos )
			*pos = cached_pos;

		in_defuse_range = cached_pos.Dist( local_player->get_origin() ) < 70.f;

		break;
	}

	return in_defuse_range;
}

bool antiaim::get_antiaim_state()
{
	return antiaim_called == globals::current_cmd->command_number;
}

float antiaim::get_wish_speed( CUserCmd* const cmd )
{
	auto max_speed = local_player->get_max_speed();
	auto forwardmove = cmd->forwardmove, sidemove = cmd->sidemove, upmove = cmd->upmove;
	const auto speed_squared = forwardmove * forwardmove + sidemove * sidemove + upmove * upmove;

	if ( cmd->buttons & IN_SPEED )
	{
		const auto speed = local_player->get_velocity().Length();
		if ( speed < max_speed * .52f + 25.f )
			max_speed *= .52f;
	}

	auto factor = 1.0f;
	if ( local_player->get_surface_props() )
		factor = local_player->get_surface_props()->game.maxspeedfactor;

	if ( local_player->get_flags() & FL_ONGROUND )
		factor *= local_player->get_velocity_modifier();

	max_speed *= factor;

	if ( local_player->get_stamina() > 0.f )
	{
		auto scale = clamp( 1.f - local_player->get_stamina() / 100.f, 0.f, 1.f );
		scale *= scale;
		max_speed *= scale;
	}

	if ( ( speed_squared != 0.f ) && ( speed_squared > max_speed * max_speed ) )
	{
		const auto ratio = max_speed / sqrtf( speed_squared );
		forwardmove *= ratio;
		sidemove *= ratio;
		upmove *= ratio;
	}

	Vector fwd{}, right{};
	math::angle_vectors( cmd->viewangles, &fwd, &right );
	fwd.z = right.z = 0.f;
	fwd.NormalizeInPlace();
	right.NormalizeInPlace();
	const Vector wish_vel( fwd.x * forwardmove + right.x * sidemove, fwd.y * forwardmove + right.y * sidemove, 0.f );

	auto speed = wish_vel.Length2D();
	if ( speed != 0.0f && speed > max_speed )
		speed = max_speed;

	return speed;
}