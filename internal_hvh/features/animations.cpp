#include "../include_cheat.h"

float animations::get_yaw_modifier( C_CSPlayer* player, const Vector& velocity, const float duckamt, const CCSGOPlayerAnimState* state )
{
	const auto wpn = globals::get_weapon( player->get_active_weapon() );
	const auto speed = std::min( velocity.Length2D(), 260.f );
	const auto max_movement_speed = wpn ? std::max( wpn->get_max_speed(), .001f ) : 260.f;
	const auto movement_speed = clamp( speed / ( max_movement_speed * .520f ), 0.f, 1.f );
	const auto ducking_speed = clamp( speed / ( max_movement_speed * .340f ), 0.f, 1.f );

	auto yaw_modifier = ( ( ( state->ground_fraction * -.3f ) - .2f ) * movement_speed ) + 1.f;

	if ( duckamt > 0.f )
	{
		const auto ducking_modifier = clamp( ducking_speed, 0.f, 1.f );
		yaw_modifier += ( ( duckamt * ducking_modifier ) * ( .5f - yaw_modifier ) );
	}

	return yaw_modifier;
}

void animations::calculate_sim_ticks( lag_record_t* record, C_CSPlayer* player, lag_record_t* previous )
{
	auto& log = player_log::get_log( player->EntIndex() );

	record->m_lagamt = clamp( record->m_lagamt, 1, sv_maxusrcmdprocessticks );

	if ( !previous )
		return;

	const auto& start = previous->m_layers[ 11 ];
	const auto& end = record->m_layers[ 11 ];
	auto cycle = start.m_flCycle;
	auto playback_rate = start.m_flPlaybackRate;

	record->m_ignore = true;

	for ( auto i = 1; i <= 64; i++ )
	{
		const auto cycle_delta = playback_rate * ticks_to_time( 1 );
		cycle += cycle_delta;

		if ( cycle >= 1.f )
			playback_rate = end.m_flPlaybackRate;

		cycle -= static_cast< float >( static_cast< int >( cycle ) );

		if ( cycle < 0.f )
			cycle += 1.f;

		if ( cycle > 1.f )
			cycle -= 1.f;

		if ( fabsf( cycle - end.m_flCycle ) <= .5f * cycle_delta )
		{
			if ( ( record->m_simtime < previous->m_simtime || log.m_simtime_int && record->m_simtime == previous->m_simtime ) && !previous->m_dormant )
			{
				record->m_lagamt = i + 1;
				record->m_tickbase_shift = true;
			}
			else
				record->m_lagamt = i;

			record->m_ignore = false;
		}
	}

	if ( record->m_ignore )
	{
		record->m_lagamt = 1;
		record->m_tickbase_shift = true;
	}

	if ( !log.m_simtime_int && !record->m_ignore && record->m_simtime == previous->m_simtime )
	{
		const auto standing = record->m_layers[ 6 ].m_flWeight == 0.f && record->m_layers[ 6 ].m_flPlaybackRate < 0.000001f && record->m_flags & FL_ONGROUND;
		if ( !standing && log.m_ground_accel_linear_frac_last_time_stamp == interfaces::client_state()->command_ack && fabsf( log.m_ground_accel_linear_frac_last_time - record->m_simtime ) < 1.f )
			record->m_simtime = log.m_ground_accel_linear_frac_last_time;
		else
		{
			record->m_unknown = true;
			record->m_simtime += ticks_to_time( record->m_lagamt );

			if ( log.m_unknown_misses )
				record->m_ignore = true;
		}

	}


	/*log.adjust_next = 0;

	if ( record->m_lagamt > record->m_last_server_tick - previous->m_last_server_tick )
		log.adjust_next = record->m_lagamt - ( record->m_last_server_tick - previous->m_last_server_tick );*/


	// continue the interpolation if the update has been postponed.
	if ( player->get_simtime() != record->m_simtime )
	{
		player->on_simulation_time_changing( player->get_simtime(), record->m_simtime );
		player->get_simtime() = record->m_simtime;
	}

	record->m_velocity = ( player->get_origin() - *player->get_old_origin() ) * ( 1.f / ticks_to_time( record->m_lagamt ) );

	if ( record->m_flags & FL_ONGROUND || previous->m_flags & FL_ONGROUND ) // yes this is correct
		record->m_velocity.z = 0.f;
}

void animations::update_player_animations( lag_record_t* record, C_CSPlayer* player, lag_record_t* previous )
{
	auto& log = player_log::get_log( player->EntIndex() );
	const auto state = player->get_anim_state();
	if ( !state )
		return;

	record->m_shot = false;

	if ( record->m_velocity.Length() > 200.f && record->m_flags & FL_ONGROUND )
		log.m_shots_spread = 0;

	const auto weapon = globals::get_weapon( player->get_active_weapon() );
	if ( previous && !previous->m_extrapolated && weapon && !weapon->is_grenade() && weapon->get_last_shot_time() > previous->m_simtime && weapon->get_last_shot_time() <= record->m_simtime )
		record->m_shot = true;

	if ( !record->m_velocity.IsValid() )
		record->m_velocity = Vector( 0.f, 0.f, 0.f );

	//fix standing velocity
	if ( !record->m_extrapolated && record->m_layers[ 6 ].m_flWeight == 0.f && record->m_layers[ 6 ].m_flPlaybackRate < 0.000001f && record->m_flags & FL_ONGROUND )
		record->m_velocity = Vector( 0.f, 0.f, 0.f );

	// is player a bot?
	const auto bot = player->get_player_info().fakeplayer;
	auto fake = !bot && player->is_enemy() && local_player->get_alive() && record->m_lagamt > 1 && !vars::legit_enabled();

	if ( fake && ( !previous || !previous->m_extrapolated ) )
		resolver::resolve( player, record, previous );

	const auto no_data = !previous || previous->m_extrapolated;
	const auto to_simulate = record->m_lagamt;
	if ( !previous )
		previous = record;

	if ( !no_data && record->m_lagamt > 1 && !previous->m_extrapolated && record->m_layers[ 11 ].m_flPlaybackRate == previous->m_layers[ 11 ].m_flPlaybackRate && weapon == state->weapon && record->m_flags & FL_ONGROUND )
	{
		const auto speed = std::min( record->m_velocity.Length2D(), 260.f );
		const auto max_movement_speed = weapon ? std::max( weapon->get_max_speed(), .001f ) : 260.f;
		const auto portion = clamp( 1.f - record->m_layers[ 11 ].m_flWeight, 0.f, 1.f );
		const auto vel = ( portion * .35f + .55f ) * max_movement_speed;
		if ( ( portion > 0.f && portion < 1.f )
			|| ( portion == 0.f && speed > vel )
			|| ( portion == 1.f && speed < vel ) )
		{
			auto new_vel = record->m_velocity;
			new_vel.x = record->m_velocity.x / speed * vel;
			new_vel.y = record->m_velocity.y / speed * vel;
			if ( new_vel.IsValid() )
				record->m_velocity = new_vel;
		}
	}

	struct direction_data_t
	{
		void store( C_CSPlayer* player )
		{
			abs_origin = player->get_abs_origin();
			layers = player->get_anim_layers();
			state = *player->get_anim_state();
			abs_angle = player->get_abs_angles();
			eye_angle = player->get_eye_angles();
			lby = player->get_lby();
			flags = player->get_flags();
			strafing = player->get_strafing();
			ground_entity = player->get_ground_entity();
			velocity_modifier = player->get_velocity_modifier();
			velocity = player->get_velocity();
			poses = player->get_pose_params();
			ground_accel_linear_frac_last_time = player->get_ground_accel_linear_frac_last_time();
			origin = player->get_origin();
			pm.store( player );
		}

		void write( C_CSPlayer* player, bool backup = true ) const
		{

			player->get_anim_layers() = layers;
			*player->get_anim_state() = state;
			player->set_abs_angles( abs_angle );
			player->get_eye_angles() = eye_angle;
			player->get_lby() = lby;
			player->get_strafing() = strafing;
			player->get_pose_params() = poses;
			if ( backup )
			{
				player->get_ground_accel_linear_frac_last_time() = ground_accel_linear_frac_last_time;
				player->get_ground_entity() = ground_entity;
				player->get_flags() = flags;
				player->set_abs_origin( abs_origin );
				player->get_velocity_modifier() = velocity_modifier;
				player->get_velocity() = velocity;
				player->get_origin() = origin;
				pm.restore( player );
			}

		}

		Vector abs_origin;
		std::array< C_AnimationLayer, 13 > layers;
		CCSGOPlayerAnimState state;
		QAngle abs_angle;
		QAngle eye_angle;
		float lby;
		int flags;
		bool strafing;
		CBaseHandle ground_entity;
		float velocity_modifier;
		Vector velocity;
		Vector origin;
		std::array< float, 24 > poses;
		float ground_accel_linear_frac_last_time;
		process_move_changes_t pm;
	};

	enum_array<resolver_direction, direction_data_t, resolver_direction::resolver_direction_max > direction_data{};

	// player backup
	auto intermediary_record = std::make_unique<lag_record_t>( player );

	direction_data_t backup;
	backup.store( player );

	CUserCmd cmd{};

	player->set_abs_origin( previous->m_origin );
	player->get_velocity() = previous->m_velocity;
	player->get_velocity().z = previous->m_calculated_velocity.z;
	player->get_flags() = previous->m_flags;
	player->get_velocity_modifier() = previous->m_velocity_modifier;
	player->get_ground_entity() = previous->m_flags & FL_ONGROUND ? interfaces::entity_list()->get_client_entity( 0 )->GetRefEHandle() : 0;
	player->get_duck_amt() = previous->m_duckamt;

	auto backup_curtime = interfaces::globals()->curtime;
	auto backup_frametime = interfaces::globals()->frametime;

	int prev_buttons = 0;

	for ( auto i = 0; i < to_simulate; i++ )
	{
		const auto sent_tick = i == to_simulate - 1;

		interfaces::globals()->curtime = intermediary_record->m_simtime = record->m_simtime - ticks_to_time( to_simulate - i - 1 );
		interfaces::globals()->frametime = interfaces::globals()->interval_per_tick;

		const auto speed = record->m_velocity.Length2D();

		if ( !no_data && to_simulate > 1 )
		{
			cmd.viewangles.y = math::calc_angle( previous->m_origin, record->m_origin ).y;
			cmd.viewangles.x = 0;
			cmd.forwardmove = record->m_velocity.Length2D();
			cmd.sidemove = 0.f;

			if ( speed < previous->m_velocity.Length2D() && player->get_flags() & FL_ONGROUND && record->m_flags & FL_ONGROUND )
			{
				CMoveData data = interfaces::game_movement()->setup_move( player, &cmd );
				aimbot_helpers::stop_to_speed( speed, &data, player );
				cmd.forwardmove = data.m_flForwardMove;
				cmd.sidemove = data.m_flSideMove;
			}

			if ( player->get_duck_amt() < record->m_duckamt || record->m_duckamt == 1.f )
				cmd.buttons |= IN_DUCK;

			if ( player->get_duck_amt() > record->m_duckamt )
				cmd.buttons &= ~IN_DUCK;

			else if ( player->get_duck_amt() > 0.f )
				cmd.buttons |= IN_DUCK;

			if ( i == 0 )
			{
				if ( previous->m_duckamt > 0.f && record->m_obb_maxs.z == 54.f )
					player->get_ducking() = true;
				else
					player->get_ducking() = false;

				if ( previous->m_duckamt == 1.f )
				{
					player->get_ducked() = true;
					player->get_ducking() = false;
				}
				else
					player->get_ducked() = false;

				prev_buttons = cmd.buttons;

				if ( previous->m_flags & FL_ONGROUND && !( record->m_flags & FL_ONGROUND ) )
					cmd.buttons |= IN_JUMP;

				if ( !( previous->m_flags & FL_ONGROUND ) )
					prev_buttons |= IN_JUMP;

			}

			CMoveData data = interfaces::game_movement()->setup_move( player, &cmd );
			data.m_nOldButtons = prev_buttons;
			const auto ret = interfaces::game_movement()->process_movement( player, &data );
			prev_buttons = data.m_nButtons;
			ret.restore( player );

			if ( !( previous->m_flags & FL_ONGROUND ) && !( record->m_flags & FL_ONGROUND ) && player->get_flags() & FL_ONGROUND )
				cmd.buttons |= IN_JUMP;
			else
				cmd.buttons &= ~IN_JUMP;

			player->get_origin() = data.m_vecAbsOrigin;
			player->set_abs_origin( data.m_vecAbsOrigin );
			player->get_velocity() = data.m_vecVelocity;

			if ( sent_tick )
			{
				player->get_flags() = record->m_flags;
				player->set_abs_origin( record->m_origin );

				if ( record->m_layers[ 11 ].m_flPlaybackRate == previous->m_layers[ 11 ].m_flPlaybackRate && weapon == state->weapon && record->m_flags & FL_ONGROUND )
				{
					auto old_vel = player->get_velocity();

					const auto speed = std::min( old_vel.Length2D(), 260.f );
					const auto max_movement_speed = weapon ? std::max( weapon->get_max_speed(), .001f ) : 260.f;
					const auto portion = clamp( 1.f - record->m_layers[ 11 ].m_flWeight, 0.f, 1.f );
					const auto vel = ( portion * .35f + .55f ) * max_movement_speed;
					if ( ( portion > 0.f && portion < 1.f )
						|| ( portion == 0.f && speed > vel )
						|| ( portion == 1.f && speed < vel ) )
					{
						old_vel.x = old_vel.x / speed * vel;
						old_vel.y = old_vel.y / speed * vel;
						if ( old_vel.IsValid() )
							player->get_velocity() = old_vel;
					}
				}
			}
		}
		else if ( previous->m_extrapolated )
		{
			const auto& original_record = log.record.back();
			const auto p1 = log.record.size() > 1 ? &log.record[ log.record.size() - 2 ] : nullptr;
			const auto p2 = log.record.size() > 2 ? &log.record[ log.record.size() - 3 ] : nullptr;

			Vector predicted_vel_change{}, record_vel_change{};
			if ( p1 && p2 )
			{
				const auto p1_vel_change = ( p1->m_calculated_velocity - p2->m_calculated_velocity ) / p1->m_lagamt;
				record_vel_change = ( original_record.m_calculated_velocity - p1->m_calculated_velocity ) / original_record.m_lagamt;
				predicted_vel_change = record_vel_change - p1_vel_change;
			}

			QAngle predicted_vel_change_ang;
			math::vector_angles( player->get_velocity() + predicted_vel_change, predicted_vel_change_ang );
			cmd.viewangles.y = predicted_vel_change_ang.y;
			cmd.viewangles.x = 0;

			cmd.forwardmove = speed > 5.f ? 450.f : ( i % 2 ? 1.01f : -1.01f );
			cmd.sidemove = 0.f;

			if ( record->m_duckamt > 0.f )
				cmd.buttons |= IN_DUCK;
			else
				cmd.buttons &= ~IN_DUCK;

			if ( i == 0 )
			{
				if ( player->get_duck_amt() > 0.f )
					player->get_ducking() = true;
				else
					player->get_ducking() = false;

				if ( player->get_duck_amt() == 1.f )
				{
					player->get_ducked() = true;
					player->get_ducking() = false;
				}
				else
					player->get_ducked() = false;

				prev_buttons = cmd.buttons;

				if ( !( player->get_flags() & FL_ONGROUND ) )
					prev_buttons |= IN_JUMP;
			}

			if ( !( player->get_flags() & FL_ONGROUND ) )
			{
				QAngle vel_ang;
				math::vector_angles( player->get_velocity(), vel_ang );

				if ( fabsf( math::normalize_float( vel_ang.y - predicted_vel_change_ang.y ) ) > 20.f )
				{
					cmd.forwardmove = 0.f;
					cmd.sidemove = fabsf( vel_ang.y - predicted_vel_change_ang.y ) > 0.f ? 450.f : -450.f;
				}
			}
			else if ( p1 && speed < p1->m_velocity.Length2D() - 5.f * original_record.m_lagamt || speed < 5.f )
			{
				CMoveData data = interfaces::game_movement()->setup_move( player, &cmd );
				aimbot_helpers::stop_to_speed( 1.01f, &data, player );
				cmd.forwardmove = data.m_flForwardMove;
				cmd.sidemove = data.m_flSideMove;
			}
			else if ( p1 && speed < p1->m_velocity.Length2D() + 5.f * original_record.m_lagamt && speed > 5.f )
			{
				CMoveData data = interfaces::game_movement()->setup_move( player, &cmd );
				aimbot_helpers::stop_to_speed( ( player->get_velocity() + predicted_vel_change ).Length2D(), &data, player );
				cmd.forwardmove = data.m_flForwardMove;
				cmd.sidemove = data.m_flSideMove;
			}

			CMoveData data = interfaces::game_movement()->setup_move( player, &cmd );
			data.m_nOldButtons = prev_buttons;
			const auto ret = interfaces::game_movement()->process_movement( player, &data );
			prev_buttons = data.m_nButtons;
			ret.restore( player );

			if ( p1 )
			{
				if ( !( p1->m_flags & FL_ONGROUND ) && !( original_record.m_flags & FL_ONGROUND ) && player->get_flags() & FL_ONGROUND )
					cmd.buttons |= IN_JUMP;
				else
					cmd.buttons &= ~IN_JUMP;
			}

			player->get_origin() = data.m_vecAbsOrigin;
			player->set_abs_origin( data.m_vecAbsOrigin );
			player->get_velocity() = data.m_vecVelocity;

			if ( sent_tick )
				record->m_origin = data.m_vecAbsOrigin;

			player->get_eye_angles() = record->m_eye_angles;
		}
		else
			player->get_velocity() = record->m_velocity;

		if ( !no_data && record->m_lagamt == 1 && record->m_obb_maxs.z == previous->m_obb_maxs.z )
			player->get_velocity() = record->m_velocity;

		if ( !player->get_velocity().IsValid() )
			player->get_velocity() = record->m_velocity;

		if ( sent_tick && record->m_layers[ 6 ].m_flPlaybackRate <= 0.f && record->m_flags & FL_ONGROUND )
			player->get_velocity() = {};

		//lerp duckamt
		//if ( !previous->m_extrapolated )
			//player->get_duck_amt() = math::lerp( previous->m_duckamt, record->m_duckamt, lerp_amt );

		for ( auto j = resolver_direction::resolver_networked; j < resolver_direction::resolver_max_extra; j++ )
		{
			if ( ( bot || !player->is_enemy() ) && j != resolver_direction::resolver_networked )
				continue;

			if ( i == 0 )
			{
				player->get_anim_layers() = previous->m_layers;
				*state = previous->m_state[ j ].m_animstate;
				intermediary_record->m_state[ j ].m_animstate.copy_meta( state );
				intermediary_record->addon = previous->m_state[ j ].addon;
				state->feet_cycle = previous->m_layers[ 6 ].m_flCycle;
				state->feet_weight = previous->m_layers[ 6 ].m_flWeight;
				state->strafe_sequence = previous->m_layers[ 7 ].m_nSequence;
				state->strafe_change_weight = previous->m_layers[ 7 ].m_flWeight;
				state->strafe_change_cycle = previous->m_layers[ 7 ].m_flCycle;
				state->acceleration_weight = previous->m_layers[ 12 ].m_flWeight;
				player->set_abs_angles( previous->m_state[ j ].m_abs_ang );
				player->get_lby() = previous->m_lby;
				player->get_strafing() = previous->m_strafing;
				player->get_pose_params() = previous->m_state[ j ].m_poses;
			}
			else
			{
				direction_data[ j ].write( player, false );
				intermediary_record->m_state[ j ].m_animstate.copy_meta( state );
				intermediary_record->addon = previous->m_state[ j ].addon;
			}

			const auto current_state = &record->m_state[ j ];

			if ( !sent_tick )
			{
				current_state->m_simulated_yaw = record->m_eye_angles.y;

				if ( j == resolver_direction::resolver_networked )
				{
					auto [p, l] = predict_animation_state( player );
					record->m_yaw_modifier = get_yaw_modifier( player, p.velocity, p.duck_amount, &p );
				}

				// simulate fake yaw in choked ticks
				if ( fake && j != resolver_direction::resolver_networked )
					current_state->m_simulated_yaw = resolver::get_resolver_angle( *record, j, record->m_eye_angles.y );

				// holdaim
				if ( record->m_shot && intermediary_record->m_simtime >= weapon->get_last_shot_time() )
					current_state->m_simulated_yaw = record->m_eye_angles.y;

				player->get_eye_angles().y = current_state->m_simulated_yaw;
			}
			else
			{
				if ( !previous->m_extrapolated )
				{
					player->get_duck_amt() = record->m_duckamt;
					player->get_flags() = record->m_flags;
					player->set_abs_origin( record->m_origin );
					player->get_lby() = record->m_lby;
					player->get_strafing() = record->m_strafing;
					player->get_velocity_modifier() = record->m_velocity_modifier;
				}

				player->get_eye_angles() = record->m_eye_angles;
			}

			update_animations( intermediary_record.get(), previous->m_extrapolated ? &cmd : nullptr, record->m_tickbase_shift && i == 0 );

			if ( ( !no_data || previous->m_extrapolated ) && j != resolver_direction::resolver_networked && sent_tick && ( !player->get_strafing() || fabsf( previous->m_layers[ 7 ].m_flWeight - record->m_layers[ 7 ].m_flWeight ) < 0.1f && record->m_layers[ 7 ].m_flWeight > 0.f && record->m_layers[ 7 ].m_flWeight < 1.f ) && player->get_flags() & FL_ONGROUND && player->get_velocity().Length2D() > 0.1f )
				player->get_pose_params()[ 0 ] = ( math::normalize_float( state->move_yaw + 180.f ) + 180.f ) / 360.f;

			// force middle | check for NOT holdaim
			if ( !sent_tick && fake && j == resolver_direction::resolver_zero && !( record->m_shot && intermediary_record->m_simtime >= weapon->get_last_shot_time() ) )
				state->foot_yaw = current_state->m_simulated_yaw;

			if ( !sent_tick && fake && ( j == resolver_direction::resolver_max || j == resolver_direction::resolver_min ) && !( record->m_shot && intermediary_record->m_simtime >= weapon->get_last_shot_time() ) )
				state->foot_yaw = current_state->m_simulated_yaw;

			if ( j != resolver_direction::resolver_networked && ( player->get_velocity().Length2D() <= 0.1f || sent_tick ) )
				player->get_lby() = record->m_lby;

			direction_data[ j ].store( player );

			// store updated data
			current_state->m_poses = player->get_pose_params();
			current_state->m_animstate = *player->get_anim_state();
			current_state->m_abs_ang = { 0.f, player->get_anim_state()->foot_yaw, 0.f };
			current_state->m_own_layers = player->get_anim_layers();
			current_state->addon = intermediary_record->addon;
		}

		record->m_calculated_velocity = player->get_velocity();
	}

	interfaces::globals()->curtime = backup_curtime;
	interfaces::globals()->frametime = backup_frametime;

	// restore
	backup.write( player );

	record->m_state[ resolver_direction::resolver_max_extra ] = record->m_state[ resolver_direction::resolver_max_max ] = record->m_state[ resolver_direction::resolver_max ];
	record->m_state[ resolver_direction::resolver_min_extra ] = record->m_state[ resolver_direction::resolver_min_min ] = record->m_state[ resolver_direction::resolver_min ];

	if ( previous->m_extrapolated )
		return;

	resolver::post_animate( player, record );

	// restore to resolved state
	record->m_setup_dir = log.get_dir( record );
	const auto resolved_state = &record->m_state[ log.get_dir( record ) ];
	player->get_anim_layers() = record->m_layers;
	player->get_origin() = record->m_origin;
	player->get_lby() = record->m_lby;
	*player->get_anim_state() = resolved_state->m_animstate;
	player->get_pose_params() = resolved_state->m_poses;
	original( hook::set_abs_angles )( player, nullptr, resolved_state->m_abs_ang );
	record->m_abs_ang = resolved_state->m_abs_ang;
	original( hook::on_latch_interp_var )( player, nullptr, 1 );
}

bool animations::local_bone_setup( lag_record_t& record, bool ignore_check )
{
	static auto old_cmdnum = 0;

	if ( !ignore_check && old_cmdnum == record.m_cmdnum )
	{
		if ( vars::visuals.chams.local.fake.interpolate->get<bool>() || antiaim::to_choke < 2 && !record.m_ignore )
			move_matrix( record.matrix( resolver_direction::resolver_networked ), record.m_abs_origin, local_player->get_abs_origin() );

		return true;
	}

	old_cmdnum = record.m_cmdnum;

	auto view = *local_player->eye_angles_virtual();
	QAngle angles = { record.m_eye_angles.x, record.m_eye_angles.y, 0.f };
	interfaces::prediction()->SetLocalViewAngles( angles );
	const auto ret = bone_setup::handle_bone_setup( record, resolver_direction::resolver_networked, true );
	interfaces::prediction()->SetLocalViewAngles( view );
	return ret;
}

void animations::move_matrix( matrix3x4_t* matrix, Vector& orig_pos, const Vector new_pos )
{
	for ( auto i = 0; i < 128; i++ )
	{
		const auto matrix_delta = math::matrix_get_origin( matrix[ i ] ) - orig_pos;
		math::matrix_set_origin( matrix_delta + new_pos, matrix[ i ] );
	}

	orig_pos = new_pos;
}

Vector animations::get_headpos()
{
	if ( globals::in_cm )
		bone_setup::handle_bone_setup( aimbot::shot_record, resolver_direction::resolver_networked );

	_( head0, "head_0" );
	return math::matrix_get_origin( aimbot::shot_record.matrix( resolver_direction::resolver_networked )[ reinterpret_cast< int( __thiscall* )( void*, const char* ) >( make_offset_multi( "client.dll", sig_lookup_bone ) )( local_player, head0.c_str() ) ] );
}


class modifier_table_t
{
private:
	MFUNC( find( uint16_t* symbol, char* lookup ), uint16_t* ( __thiscall* )( void*, uint16_t*, char* ), make_offset( "client.dll", sig_modifier_table_find ) )( symbol, lookup );
	MFUNC( add_string( uint16_t* symbol, char* lookup ), uint16_t* ( __thiscall* )( void*, uint16_t*, char* ), make_offset( "client.dll", sig_modifier_table_add_string ) )( symbol, lookup );

public:
	uint16_t find_or_insert( char* lookup )
	{
		uint16_t symbol = 0xFFFF;
		find( &symbol, lookup );
		if ( symbol == 0xFFFF )
			symbol = *add_string( &symbol, lookup );

		return symbol;
	}
};

void build_activity_modifiers( C_CSPlayer* player, std::vector<uint16_t>& modifiers, bool uh = false )
{
	modifiers.clear();

	const auto add_modifier = [&modifiers] ( std::string name )
	{
		char lookup[ 32 ];
		strcpy_s( lookup, sizeof( lookup ), name.c_str() );
		const auto sym = reinterpret_cast< modifier_table_t* >( make_offset( "client.dll", sig_modifier_table ) )->find_or_insert( lookup );

		if ( sym != 0xFFFF )
			modifiers.push_back( sym );
	};

	const auto state = player->get_anim_state();

	add_modifier( state->get_active_weapon_prefix() );

	_( moving, "moving" );
	_( crouch, "crouch" );
	_( underhand, "underhand" );

	if ( state->running_speed > .25f ) // running speed.
		add_modifier( moving );

	if ( state->duck_amount > .55f )
		add_modifier( crouch );

	if ( uh )
		add_modifier( underhand );
}

void animations::build_fake_animation()
{
	const auto player = local_player;

	if ( player != player->get_anim_state()->player )
		return;

	const auto backup_animations = animation_copy( 0 );

	prediction::run_prediction();

	// store off real animation.
	const auto real_layers = player->get_anim_layers();

	// store off buttons ( for fake jumping animation )
	const auto buttons = globals::current_cmd->buttons;

	// check if jumped within choked cycle
	for ( size_t i = 0; i <= interfaces::client_state()->chokedcommands; i++ )
		if ( interfaces::input()->m_pCommands[ ( globals::current_cmd->command_number - i ) % 150 ].buttons & IN_JUMP )
		{
			globals::current_cmd->buttons |= IN_JUMP;
			break;
		}

	// emplace last fake.
	if ( fake_record.has_animstate )
	{
		const auto bak = *player->get_anim_state();
		*player->get_anim_state() = fake_record.m_state[ resolver_direction::resolver_networked ].m_animstate;
		bak.copy_meta( player->get_anim_state() );
		player->get_anim_layers() = fake_record.m_layers;
		player->get_pose_params() = fake_record.m_state[ resolver_direction::resolver_networked ].m_poses;
		player->get_anim_state()->feet_cycle = player->get_anim_layers()[ 6 ].m_flCycle;
		player->get_anim_state()->feet_weight = player->get_anim_layers()[ 6 ].m_flWeight;
	}

	fake_record.m_index = player->EntIndex();
	fake_record.m_simtime = time_to_ticks( local_player->get_tickbase() );
	fake_record.m_ignore = fake_record.m_simtime <= fake_record.m_old_simtime;
	fake_record.m_eye_angles = globals::current_cmd->viewangles;
	if ( !fake_record.m_ignore ) //fix for non lag compensated fake chams
	{
		fake_record.m_old_simtime = fake_record.m_simtime;
		fake_record.m_cmdnum = globals::current_cmd->command_number;
	}

	// animate fake.
	update_animations( &fake_record, globals::current_cmd, false );

	// selectively merge in the server layers now.
	for ( auto i = 0u; i < local_player->get_anim_layers().size(); i++ )
	{
		auto& client = local_player->get_anim_layers()[ i ];
		auto& real = real_layers[ i ];

		client.m_nOrder = real.m_nOrder;

		if ( i == 1 || ( i > 7 && i < 12 ) )
			client = real;
	}

	// store fake record.
	fake_record.has_animstate = true;
	fake_record.m_state[ resolver_direction::resolver_networked ].m_animstate = *player->get_anim_state();
	fake_record.m_layers = player->get_anim_layers();
	fake_record.m_state[ resolver_direction::resolver_networked ].m_poses = player->get_pose_params();
	fake_record.m_state[ resolver_direction::resolver_networked ].m_abs_ang = { 0.f, player->get_anim_state()->foot_yaw, 0.f };
	fake_record.m_origin = player->get_origin();
	if ( !fake_record.m_ignore ) //fix for non lag compensated fake chams
		fake_record.m_abs_origin = player->get_abs_origin();

	fake_record.m_state[ resolver_direction::resolver_networked ].m_setup_tick = -1;

	local_bone_setup( fake_record );

	globals::current_cmd->buttons = buttons;

	backup_animations.restore( player );
}

void animations::play_additional_animations( CUserCmd* const cmd, const CCSGOPlayerAnimState& pred_state, lag_record_t* record, int seed )
{
	if ( !cmd )
		return;

	const auto player = globals::get_player( record->m_index );
	const auto wpn = globals::get_weapon( player->get_active_weapon() );
	const auto full = record == &local_record;

	const auto s = player->get_anim_state();
	const auto p = &pred_state;

	build_activity_modifiers( player, record->addon.activity_modifiers );

   // CCSGOPlayerAnimState::DoAnimationEvent
	if ( wpn && wpn->get_weapon_type() == WEAPONTYPE_KNIFE && wpn->get_next_primary_attack() + .4f < interfaces::globals()->curtime )
		record->addon.swing_left = true;

	  // CCSGOPlayerAnimState::DoAnimationEvent
	if ( cmd->buttons & IN_JUMP && !( player->get_flags() & FL_ONGROUND ) && s->on_ground )
	{
		player->try_initiate_animation( 4, 985, record->addon.activity_modifiers, seed );
		record->addon.in_jump = true;
	}

	const auto modifiers = record->addon.activity_modifiers;
	auto& action = player->get_anim_layers()[ 1 ];
	switch ( record->addon.vm )
	{
	// ACT_VM_RELEASE, ACT_VM_THROW, ACT_VM_PRIMARYATTACK, ACT_VM_PRIMARYATTACK_SILENCED
		case 219:
			build_activity_modifiers( player, record->addon.activity_modifiers, true );
		case 190:
		case 192:
		case 477:
			if ( wpn && wpn->get_weapon_id() != WEAPON_C4 )
				player->try_initiate_animation( 1, 961, record->addon.activity_modifiers, seed );
			if ( record->addon.vm == 219 )
				record->addon.activity_modifiers = modifiers;
			break;
		  // ACT_VM_HITCENTER, ACT_VM_MISSCENTER
		case 200:
		case 206:
			player->try_initiate_animation( 1, !record->addon.swing_left ? 961 : 962, record->addon.activity_modifiers, seed );
			record->addon.swing_left = !record->addon.swing_left;
			break;
		  // ACT_VM_SWINGHIT
		case 211:
			player->try_initiate_animation( 1, 963, record->addon.activity_modifiers, seed );
			record->addon.swing_left = !record->addon.swing_left;
			break;
		  // ACT_VM_SECONDARYATTACK
		case 193:
			player->try_initiate_animation( 1, 964, record->addon.activity_modifiers, seed );
			break;
		  // ACT_VM_HITCENTER2, ACT_VM_MISSCENTER2
		case 201:
		case 207:
			player->try_initiate_animation( 1, 964, record->addon.activity_modifiers, seed );
			record->addon.swing_left = true;
			break;
		  // ACT_VM_SWINGHARD
		case 209:
			player->try_initiate_animation( 1, 965, record->addon.activity_modifiers, seed );
			record->addon.swing_left = true;
			break;
		  // ACT_VM_RELOAD, ACT_SECONDARY_VM_RELOAD
		case 194:
		case 831:
			if ( wpn && wpn->get_weapon_type() == WEAPONTYPE_SHOTGUN && wpn->get_weapon_id() != WEAPON_MAG7 )
				player->try_initiate_animation( 1, 969, record->addon.activity_modifiers, seed );
			else
				player->try_initiate_animation( 1, 967, record->addon.activity_modifiers, seed );
			break;
		  // ACT_SHOTGUN_RELOAD_START
		case 264:
			player->try_initiate_animation( 1, 968, record->addon.activity_modifiers, seed );
			break;
		  // ACT_SHOTGUN_RELOAD_FINISH
		case 265:
			player->try_initiate_animation( 1, 970, record->addon.activity_modifiers, seed );
			break;
		  // ACT_VM_PULLPIN
		case 191:
			player->try_initiate_animation( 1, 971, record->addon.activity_modifiers, seed );
			break;
		  // ACT_VM_DRAW, ACT_VM_EMPTY_DRAW, ACT_VM_DRAW_SILENCED
		case 183:
		case 224:
		case 481:
			if ( wpn && player->get_sequence_act( action.m_nSequence ) == 972
			  && action.m_flCycle < .15f )
				record->addon.in_deploy_rate_limit = true;
			else
				player->try_initiate_animation( 1, 972, record->addon.activity_modifiers, seed );
			break;
	}

	// CCSGOPlayerAnimState::SetupVelocity
	auto& layer3 = player->get_anim_layers()[ 3 ];

	if ( !record->addon.in_idle && p->speed <= 0.f && s->standing_time <= 0.f && s->on_ground
		&& !s->on_ladder && !s->in_hit_ground && s->stutter_step < 50.f )
	{
		player->try_initiate_animation( 3, 980, record->addon.activity_modifiers, seed );
		record->addon.in_idle = true;
	}

	const auto update_time = fmaxf( 0.f, p->last_update - s->last_update );
	const auto layer3_act = player->get_sequence_act( layer3.m_nSequence );
	if ( layer3_act == 980 || layer3_act == 979 )
	{
		if ( record->addon.in_idle && p->ducking_speed <= .25f )
		{
			layer3.increment_layer_cycle_weight_rate_generic( update_time, player->get_model_ptr() );
			record->addon.in_idle = !layer3.has_sequence_completed( update_time );
		}
		else
		{
			const auto weight = layer3.m_flWeight;
			layer3.m_flWeight = math::approach( 0.f, weight, update_time * 5.f );
			layer3.set_layer_weight_rate( update_time, weight );
			record->addon.in_idle = false;
		}
	}

	if ( p->speed <= 1.f && s->on_ground && !s->on_ladder && !s->in_hit_ground
		&& fabsf( math::angle_diff( s->foot_yaw, p->foot_yaw ) ) / update_time > 120.f )
	{
		player->try_initiate_animation( 3, 979, record->addon.activity_modifiers, seed );
		record->addon.in_idle = true;
	}

	record->addon.adjust_weight = layer3.m_flWeight;
	layer3.m_flWeight = 0.f;

	if ( player->get_predictable() )
	{
		Vector forward{}, right{}, up{};
		math::angle_vectors( QAngle( 0.f, p->foot_yaw, 0.f ), &forward, &right, &up );
		right.NormalizeInPlace();
		const auto to_forward_dot = p->last_accel_speed.Dot( forward );
		const auto to_right_dot = p->last_accel_speed.Dot( right );

		const auto move_right = ( cmd->buttons & IN_MOVERIGHT ) != 0;
		const auto move_left = ( cmd->buttons & IN_MOVELEFT ) != 0;
		const auto move_forward = ( cmd->buttons & IN_FORWARD ) != 0;
		const auto move_backwards = ( cmd->buttons & IN_BACK ) != 0;
		const auto strafe_forward = p->running_speed >= .65f && move_forward && !move_backwards && to_forward_dot < -.55f;
		const auto strafe_backwards = p->running_speed >= .65f && move_backwards && !move_forward && to_forward_dot > .55f;
		const auto strafe_right = p->running_speed >= .73f && move_right && !move_left && to_right_dot < -.63f;
		const auto strafe_left = p->running_speed >= .73f && move_left && !move_right && to_right_dot > .63f;

		player->get_strafing() = strafe_forward || strafe_backwards || strafe_right || strafe_left;
	}

	const auto swapped_ground = s->on_ground != p->on_ground || s->on_ladder != p->on_ladder;

	if ( p->on_ground )
	{
		auto& layer5 = player->get_anim_layers()[ 5 ];

		if ( !s->in_hit_ground && swapped_ground )
			player->try_initiate_animation( 5, s->air_time > 1.f ? 989 : 988, record->addon.activity_modifiers, seed );

		if ( p->in_hit_ground && player->get_sequence_act( layer5.m_nSequence ) != 987 )
			record->addon.in_jump = false;

		if ( !p->in_hit_ground && !record->addon.in_jump && p->ladder_speed <= 0.f )
			layer5.m_flWeight = 0.f;
	}
	else if ( swapped_ground && !record->addon.in_jump )
		player->try_initiate_animation( 4, 986, record->addon.activity_modifiers, seed );

	// CCSGOPlayerAnimState::SetupAliveLoop
	auto& alive = player->get_anim_layers()[ 11 ];
	if ( player->get_sequence_act( alive.m_nSequence ) == 981 )
	{
		if ( p->weapon && p->weapon != p->last_weapon )
		{
			const auto cycle = alive.m_flCycle;
			player->try_initiate_animation( 11, 981, record->addon.activity_modifiers, seed );
			alive.m_flCycle = cycle;
		}
		else if ( !alive.has_sequence_completed( update_time ) )
			alive.m_flWeight = 1.f - clamp( ( p->walk_speed - .55f ) / .35f, 0.f, 1.f );
	}

	const auto world_model = wpn ? reinterpret_cast< C_BaseWeaponWorldModel* >( interfaces::entity_list()->get_client_entity_from_handle( wpn->get_world_model_handle() ) ) : nullptr;

	 // CCSGOPlayerAnimState::SetUpWeaponAction
	auto increment = true;
	if ( wpn && full && record->addon.in_deploy_rate_limit && player->get_sequence_act( action.m_nSequence ) == 972 )
	{
		if ( world_model )
			world_model->get_effects() |= EF_NODRAW;

		if ( action.m_flCycle >= .15f )
		{
			record->addon.in_deploy_rate_limit = false;
			player->try_initiate_animation( 1, 972, record->addon.activity_modifiers, seed );
			increment = false;
		}
	}

	auto& recrouch = player->get_anim_layers()[ 2 ];
	auto recrouch_weight = 0.f;

	if ( action.m_flWeight > 0.f )
	{
		if ( recrouch.m_nSequence <= 0 )
		{
			_( r, "recrouch_generic" );
			const auto seq = player->lookup_sequence( r.c_str() );
			recrouch.m_flPlaybackRate = player->get_layer_sequence_cycle_rate( &recrouch, seq );
			recrouch.m_nSequence = seq;
			recrouch.m_flCycle = recrouch.m_flWeight = 0.f;
		}

		auto has_modifier = false;
		_( c, "crouch" );
		const auto& seqdesc = player->get_model_ptr()->get_seq_desc( action.m_nSequence );
		for ( auto i = 0; i < seqdesc->numactivitymodifiers; i++ )
			if ( !strcmp( seqdesc->get_activity_modifier( i )->get_name(), c.c_str() ) )
			{
				has_modifier = true;
				break;
			}

		if ( has_modifier )
		{
			if ( p->duck_amount < 1.f )
				recrouch_weight = action.m_flWeight * ( 1.f - p->duck_amount );
		}
		else if ( p->duck_amount > 0.f )
			recrouch_weight = action.m_flWeight * p->duck_amount;
	}
	else if ( recrouch.m_flWeight > 0.f )
		recrouch_weight = math::approach( 0.f, recrouch.m_flWeight, 4.f * update_time );

	recrouch.m_flWeight = clamp( recrouch_weight, 0.f, 1.f );

	if ( increment )
	{
		action.increment_layer_cycle( update_time, false );
		action.set_layer_ideal_weight_from_sequence_cycle( player->get_model_ptr() );
		action.set_layer_weight_rate( p->last_increment, action.m_flWeight );

		action.m_flCycle = clamp( action.m_flCycle - p->last_increment * action.m_flPlaybackRate, 0.f, 1.f );
		action.m_flWeight = clamp( action.m_flWeight - p->last_increment * action.m_flWeightDeltaRate, .0000001f, 1.f );
	}

	record->addon.vm = 0;
}

void animations::predict_animation_state( lag_record_t& record, C_CSPlayer* player, int times, bool force_angle )
{
	const auto backup_state = *player->get_anim_state();
	const auto backup_layers = player->get_anim_layers();
	const auto backup_poses = player->get_pose_params();
	const auto backup_abs_origin = player->get_abs_origin();
	const auto backup_curtime = interfaces::globals()->curtime;
	const auto backup_velocity = player->get_velocity();
	auto backup_eye_angles = *player->eye_angles_virtual();
	const auto backup_strafing = player->get_strafing();

	if ( player->get_anim_state()->last_update <= 0.f || player->get_anim_state()->player != player )
		times = 0;

	const auto backup_animations = animation_copy( 0 );

	restore_animation( most_recent.second );

	auto target_direction = 0.f;

	const auto bak_fake_direction = antiaim::fake_direction;
	const auto bak_real_direction = antiaim::real_direction;
	const auto bak_in_flip = antiaim::in_flip;
	const auto bak_target_angle = antiaim::target_angle;

	if ( !force_angle )
		antiaim::run();

	if ( vars::aa.fake->get<bool>() )
		target_direction = vars::aa.fake_amount->get<float>();

	if ( vars::aa.freestand_fake->get<int>() && antiaim::fake_direction != 0.f )
	{
		if ( math::normalize_float( antiaim::fake_direction - antiaim::target_angle.y ) < 0.f )
			target_direction = -fabsf( target_direction );
		else
			target_direction = fabsf( target_direction );

		if ( vars::aa.freestand_fake->get<int>() == 2 )
			target_direction = -target_direction;
	}

	if ( antiaim::in_flip && vars::aa.jitter->get<bool>() && vars::aa.flip_fake->get<bool>() )
		target_direction = -target_direction;

	const auto target_delta = ( target_direction / 100.f );
	QAngle angles;

	interfaces::globals()->curtime = tickbase::get_adjusted_time() - ticks_to_time( interfaces::client_state()->chokedcommands );

	if ( ( tickbase::fast_fire || tickbase::hide_shot ) && tickbase::compute_current_limit() )
		interfaces::globals()->curtime += ticks_to_time( tickbase::compute_current_limit() );

	auto should_apply_antiaim = vars::aa.enabled->get<bool>() && !( globals::current_cmd->buttons & IN_USE );

	CMoveData data;
	if ( times )
		for ( auto i = 0; i < interfaces::client_state()->chokedcommands + times; i++ )
		{
			if ( i <= interfaces::client_state()->chokedcommands )
			{
				player->set_abs_origin( prediction::get_pred_info( interfaces::client_state()->lastoutgoingcommand + 1 + i ).origin );
				player->get_velocity() = prediction::get_pred_info( interfaces::client_state()->lastoutgoingcommand + 1 + i ).velocity;
				data = interfaces::game_movement()->setup_move( player, &interfaces::input()->m_pCommands[ ( interfaces::client_state()->lastoutgoingcommand + 1 + i ) % 150 ] );
			}
			else
			{
				interfaces::game_movement()->process_movement( player, &data );
				player->get_velocity() = data.m_vecVelocity;
			}

			if ( i > interfaces::client_state()->chokedcommands + 1 )
			{
				player->set_abs_origin( data.m_vecAbsOrigin );
				continue;
			}

			if ( !force_angle )
			{
				if ( player->get_anim_state()->last_framecount >= interfaces::globals()->framecount )
					player->get_anim_state()->last_framecount = interfaces::globals()->framecount - 1;

				auto [predicted, l] = predict_animation_state( player );
				local_record.m_yaw_modifier = get_yaw_modifier( player, predicted.velocity, predicted.duck_amount, player->get_anim_state() );
				const auto min_max = target_delta > 0.f ? predicted.aim_yaw_max : predicted.aim_yaw_min;
				const auto min_max_inv = target_delta <= 0.f ? predicted.aim_yaw_max : predicted.aim_yaw_min;
				const auto body_yaw = min_max * animations::local_record.m_yaw_modifier * fabsf( target_delta );

				auto final_add = 0.f;
				if ( vars::aa.fake->get<bool>() && vars::aa.compensate_angle->get<float>() != 0.f )
					final_add = target_delta > 0.f ? 0.f : target_delta * min_max * 0.5f * vars::aa.compensate_angle->get<float>() * 0.01f;

				angles = !should_apply_antiaim ? globals::current_cmd->viewangles : antiaim::target_angle + QAngle( 0, final_add, 0 );

				if ( should_apply_antiaim && i != interfaces::client_state()->chokedcommands + 1 && vars::aa.fake->get<bool>() )
				{
					auto intermediate = 0.f;

					if ( fabsf( target_delta ) <= .999f && fabsf( min_max * animations::local_record.m_yaw_modifier ) + fabsf( body_yaw ) + fabsf( min_max_inv * animations::local_record.m_yaw_modifier ) < 180.f )
					{
						intermediate = body_yaw;

						const auto delta = math::angle_diff( math::normalize_float( angles.y + intermediate ),
							local_player->get_anim_state()->foot_yaw );

						if ( fabsf( delta ) >= 2.5f )
							intermediate += ( delta > 0.f ? predicted.aim_yaw_max : predicted.aim_yaw_min ) * animations::local_record.m_yaw_modifier;
					}
					else if ( i % 2 != ( interfaces::client_state()->chokedcommands + 1 ) % 2 )
					{
						static constexpr auto flop_tolerance = 6.f;
						static constexpr auto flip_range = -152.f;

						const auto opposite = math::normalize_float( flip_range - fabsf( min_max_inv ) );
						intermediate += opposite + ( fabsf( min_max_inv ) - fabsf( body_yaw ) ) * .75f;
						const auto delta = math::angle_diff( math::normalize_float( angles.y + intermediate * -signum( target_delta ) ), local_player->get_anim_state()->foot_yaw );

						if ( fabsf( delta ) <= opposite )
							intermediate = 180.f + -signum( target_delta ) * ( fabsf( min_max_inv ) + flop_tolerance );
						else
							intermediate *= -signum( target_delta );
					}

					angles.y = math::normalize_float( angles.y + intermediate );
				}

				if ( should_apply_antiaim )
				{
					player->get_strafing() = false;

					if ( vars::aa.strafing->get<int>() > 1 && predicted.running_speed >= .65f )
					{
						if ( i % 2 != ( interfaces::client_state()->chokedcommands + 1 ) % 2 )
						{
							if ( player->get_anim_state()->strafe_sequence != -1 || player->get_anim_state()->strafe_change_weight <= 0.f )
								player->get_strafing() = true;
						}
					}
				}

				player->get_anim_state()->update( angles );

				if ( should_apply_antiaim && i == interfaces::client_state()->chokedcommands + 1 )
				{
					auto roll_dir = target_direction;
					if ( antiaim::in_flip && vars::aa.jitter->get<bool>() && vars::aa.flip_lean->get<bool>() )
						roll_dir = -roll_dir;

					const auto roll_amt = antiaim::get_lean( roll_dir, antiaim::target_angle.y );
					if ( vars::aa.ensure_lean->get<bool>() )
					{
						angles.z = roll_amt;
					}
					else if ( fabsf( roll_amt ) > 0.f )
					{
						angles.z = interfaces::input()->m_pCommands[ interfaces::client_state()->lastoutgoingcommand % 150 ].viewangles.z;
					}
				}

			}

			player->set_abs_origin( data.m_vecAbsOrigin );
			interfaces::globals()->curtime += interfaces::globals()->interval_per_tick;
		}

	antiaim::fake_direction = bak_fake_direction;
	antiaim::real_direction = bak_real_direction;
	antiaim::in_flip = bak_in_flip;
	antiaim::target_angle = bak_target_angle;

	record.~lag_record_t();
	new( &record ) lag_record_t( player );
	if ( times )
	{
		if ( !force_angle )
			record.m_eye_angles = angles;
		record.m_origin = data.m_vecAbsOrigin;
	}

	record.m_simtime = interfaces::globals()->curtime;

	backup_animations.restore( player );

	*player->get_anim_state() = backup_state;
	player->get_anim_layers() = backup_layers;
	player->get_pose_params() = backup_poses;
	player->set_abs_origin( backup_abs_origin );
	player->get_velocity() = backup_velocity;
	player->get_strafing() = backup_strafing;
	interfaces::globals()->curtime = backup_curtime;
	interfaces::prediction()->SetLocalViewAngles( backup_eye_angles );
}

std::pair<CCSGOPlayerAnimState, std::array<C_AnimationLayer, 13>> animations::predict_animation_state( C_CSPlayer* player )
{
	const auto backup_state = *player->get_anim_state();
	const auto backup_layers = player->get_anim_layers();
	const auto backup_poses = player->get_pose_params();

	if ( player->get_anim_state()->last_framecount >= interfaces::globals()->framecount )
		player->get_anim_state()->last_framecount = interfaces::globals()->framecount - 1;

	const auto angles = *player->eye_angles_virtual();
	player->get_anim_state()->update( angles );
	const auto pred = *player->get_anim_state();
	const auto layers = player->get_anim_layers();

	*player->get_anim_state() = backup_state;
	player->get_anim_layers() = backup_layers;
	player->get_pose_params() = backup_poses;

	return { pred, layers };
}

void animations::update_animations( lag_record_t* record, CUserCmd* const cmd, bool tickbase_drift )
{
	const auto player = globals::get_player( record->m_index );

	if ( player != player->get_anim_state()->player )
		return;

	// make a backup of globals.
	const auto backup_frametime = interfaces::globals()->frametime;
	const auto backup_curtime = interfaces::globals()->curtime;

	if ( player->get_anim_state()->last_framecount >= interfaces::globals()->framecount )
		player->get_anim_state()->last_framecount = interfaces::globals()->framecount - 1;

	// fixes for networked players.
	if ( player != local_player )
	{
		interfaces::globals()->frametime = interfaces::globals()->interval_per_tick;
		interfaces::globals()->curtime = record->m_simtime;

		if ( tickbase_drift )
			player->get_anim_state()->last_update = interfaces::globals()->curtime + interfaces::globals()->interval_per_tick;
		else
			player->get_anim_state()->last_update = interfaces::globals()->curtime - interfaces::globals()->interval_per_tick;
	}

	if ( cmd && player->get_anim_state()->last_update != interfaces::globals()->curtime )
		play_additional_animations( cmd, predict_animation_state( player ).first, record, cmd->command_number );

	// update animations.
	const auto angles = *player->eye_angles_virtual();
	player->get_anim_state()->update( angles );

	if ( cmd && player == local_player )
	{
		// update lower body yaw target, simtime and prevent adjust from blending too far into the future.
		const auto state = player->get_anim_state();
		if ( state->speed > .1f || state->speed_z > 100.f )
		{
			player->get_lby() = state->eye_yaw;
			record->addon.next_lby_update = interfaces::globals()->curtime + .22f;
		}
		else if ( interfaces::globals()->curtime > record->addon.next_lby_update && fabsf( math::angle_diff( state->foot_yaw, state->eye_yaw ) ) > 35.f )
		{
			player->get_lby() = state->eye_yaw;
			record->addon.next_lby_update = interfaces::globals()->curtime + 1.1f;
		}

		player->get_anim_layers()[ 3 ].m_flWeight = record->addon.adjust_weight;
	}

	// restore globals.
	interfaces::globals()->frametime = backup_frametime;
	interfaces::globals()->curtime = backup_curtime;
}

void animations::restore_animation( const int32_t sequence )
{
	const auto player = local_player;
	auto& info = prediction::get_pred_info( sequence );
	auto& animation = info.animation;
	if ( animation.sequence == sequence )
	{
		const auto bak = *player->get_anim_state();
		*player->get_anim_state() = animation.state;
		bak.copy_meta( player->get_anim_state() );
		player->get_anim_layers() = animation.layers;
		player->get_pose_params() = animation.poses;
		interfaces::prediction()->SetLocalViewAngles( animation.eye_angles );

		if ( info.sequence == sequence )
		{
			player->get_view_offset().z = info.viewoffset_z;
			player->GetCollideable()->OBBMaxs() = info.maxs;
			player->get_ground_entity() = info.groundentity;
			player->get_collision_viewheight() = info.collision_viewheight;
			player->get_collision_bounds_change_time() = info.collision_bounds_change_time;
		}
	}
}

void animations::merge_local_animation( CUserCmd* cmd, C_CSPlayer* player )
{
	local_record.m_index = player->EntIndex();

	auto& crc = interfaces::input()->m_pVerifiedCommands[ cmd->command_number % 150 ].m_crc;
	const auto initial = crc == *reinterpret_cast< int32_t* >( &interfaces::globals()->interval_per_tick );

	if ( !initial && cmd->command_number <= interfaces::client_state()->lastoutgoingcommand )
	{
		if ( player->get_tickbase() > lag.first )
			lag = std::make_pair( player->get_tickbase(), interfaces::client_state()->lastoutgoingcommand );

		most_recent = std::make_pair( player->get_tickbase(), interfaces::client_state()->lastoutgoingcommand );
	}

	auto& current = prediction::get_pred_info( cmd->command_number ).animation;
	const auto& before = prediction::get_pred_info( cmd->command_number - 1 ).animation;

	if ( current.sequence != cmd->command_number || crc != current.checksum )
	{
		const auto in = &interfaces::input()->m_pCommands[ cmd->command_number % 150 ];

		if ( in->command_number == cmd->command_number && before.sequence == cmd->command_number - 1 )
		{
			before.restore( player );
			update_animations( &local_record, in, false );
		}
		else if ( current.sequence == cmd->command_number )
		{
			current.restore( player );
			player->get_strafing() = current.is_strafing;
		}
		else
		{
			const auto backup_crt = crc;
			crc = *reinterpret_cast< int32_t* >( &interfaces::globals()->interval_per_tick );
			player->get_lby() = 0.f;
			player->get_anim_layers() = {};
			player->get_anim_state()->player = player;
			player->get_anim_state()->reset();
			update_animations( &local_record, in, false );
			player->get_anim_state()->last_update = interfaces::globals()->curtime + interfaces::globals()->interval_per_tick;
			update_animations( &local_record, in, false );
			crc = backup_crt;
		}

		current = animation_copy( cmd->command_number );
	}
	else
	{
		player->get_strafing() = current.is_strafing;
	}
}

void animations::update_addon_bits( C_CSPlayer* const player )
{
	_( c4, "weapon_c4" );
	_( taser, "weapon_taser" );
	_( shield, "weapon_shield" );

	const auto wpn = globals::get_weapon( player->get_active_weapon() );
	const auto world_model = wpn ? reinterpret_cast< C_BaseWeaponWorldModel* >( interfaces::entity_list()->get_client_entity_from_handle( wpn->get_world_model_handle() ) ) : nullptr;

	int32_t new_bits = 0;
	const auto weapon_visible = world_model ? !( world_model->get_effects() & EF_NODRAW ) : true;

	auto flash_bang = player->get_ammo_count( FNV1A( "AMMO_TYPE_FLASHBANG" ) );
	if ( weapon_visible && wpn && wpn->get_weapon_id() == WEAPON_FLASHBANG )
		flash_bang--;

	if ( flash_bang >= 1 )
		new_bits |= 0x1;

	if ( flash_bang >= 2 )
		new_bits |= 0x2;

	if ( wpn && player->get_ammo_count( FNV1A( "AMMO_TYPE_HEGRENADE" ) )
	  && ( wpn->get_weapon_id() != WEAPON_HEGRENADE || !weapon_visible ) )
		new_bits |= 0x4;

	if ( player->get_ammo_count( FNV1A( "AMMO_TYPE_SMOKEGRENADE" ) )
	  && ( wpn && wpn->get_weapon_id() != WEAPON_SMOKEGRENADE || !weapon_visible ) )
		new_bits |= 0x8;

	if ( player->get_ammo_count( FNV1A( "AMMO_TYPE_DECOY" ) )
	  && ( wpn && wpn->get_weapon_id() != WEAPON_DECOY || !weapon_visible ) )
		new_bits |= 0x200;

	if ( player->get_ammo_count( FNV1A( "AMMO_TYPE_TAGRENADE" ) )
	  && ( wpn && wpn->get_weapon_id() != WEAPON_TAGRENADE || !weapon_visible ) )
		new_bits |= 0x1000;

	if ( player->owns_this_type( c4.c_str() )
	  && ( wpn && wpn->get_weapon_id() != WEAPON_C4 || !weapon_visible ) )
		new_bits |= 0x10;

	if ( player->owns_this_type( shield.c_str() )
	  && ( wpn && wpn->get_weapon_id() != WEAPON_SHIELD || !weapon_visible ) )
		new_bits |= 0x2000;

	if ( player->has_defuser() )
		new_bits |= 0x20;

	const auto weapon = player->get_slot( 0 );
	if ( weapon && ( weapon != wpn || !weapon_visible ) )
	{
		new_bits |= 0x40;
		player->get_primary_addon() = weapon->get_weapon_id();
	}
	else
		player->get_primary_addon() = WEAPON_NONE;

	const auto secondary = player->get_slot( 1 );
	if ( secondary && ( secondary != wpn || !weapon_visible ) )
	{
		new_bits |= 0x80;
		if ( secondary->get_weapon_id() == WEAPON_ELITE )
			new_bits |= 0x100;
		player->get_secondary_addon() = secondary->get_weapon_id();
	}
	else if ( secondary && secondary->get_weapon_id() == WEAPON_ELITE )
	{
		new_bits |= 0x100;
		player->get_secondary_addon() = secondary->get_weapon_id();
	}
	else
		player->get_secondary_addon() = WEAPON_NONE;

	const auto knife = player->get_slot( 2 );
	if ( knife && ( knife != wpn || !weapon_visible ) )
		new_bits |= 0x400;

	if ( player->EntIndex() == interfaces::engine()->GetLocalPlayer() )
		skinchanger::on_addon_bits( player, new_bits );

	player->get_addon_bits() = new_bits;
}

#ifdef DEBUG
void animations::replace_server_model( bool start )
{
	auto player = local_player;

	static model_t* bak_model = nullptr;
	static CStudioHdr* bak_hdr = nullptr;

	if ( start && skinchanger::server_model_idx != -1 )
	{
		bak_model = player->get_model();
		bak_hdr = player->get_model_ptr();
	}
	else if ( start )
	{
		bak_model = nullptr;
		bak_hdr = nullptr;
	}

	static auto lock_ = reinterpret_cast< void( __thiscall* )( void* ) >( sig_rel( "client.dll", "E8 ? ? ? ? 8D 47 FC" ) );
	static auto unlock_ = reinterpret_cast< void( __thiscall* )( void* ) >( sig( "client.dll", "56 57 8B F9 8B B7 ? ? ? ? 85 F6 0F 84 ? ? ? ? 8B 4E 08" ) );

	if ( bak_model && bak_hdr )
	{
		if ( !start )
		{
			if ( player->get_model_ptr() != bak_hdr )
				unlock_( player );
			player->get_model() = bak_model;
			player->get_model_ptr() = bak_hdr;
		}
		else if ( const auto model = interfaces::model_info()->GetModel( skinchanger::server_model_idx ) )
		{
			util::precache_model( player->GetModel()->name, false );
			player->get_model() = const_cast< model_t* >( model );
			lock_( player );
		}
	}
}
#endif
