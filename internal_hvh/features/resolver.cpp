#include <random>
#include "../include_cheat.h"

void resolver::resolve( C_CSPlayer* player, lag_record_t* record, lag_record_t* previous )
{
	if ( !player->is_enemy() )
		return;

	if ( !record->m_shot )
		pitch_resolve( record );

	yaw_resolve( record, previous );
}

void resolver::post_animate( C_CSPlayer* player, lag_record_t* record )
{
	const auto log = &player_log::get_log( player->EntIndex() );

	if ( vars::aim.resolver_mode->get<int>() )
		for ( auto& mode : log->m_mode )
			for ( auto& side : mode.m_side )
				if ( side.m_current_dir > resolver_direction::resolver_max )
				{
					side.m_current_dir = resolver_direction::resolver_networked;
					log->m_unknown_shot = true;
					log->m_unknown = true;
				}

	if ( !player->is_enemy() || player->get_player_info().fakeplayer )
		log->m_mode[ resolver_mode::resolver_shot ].m_side = log->m_mode[ resolver_mode::resolver_default ].m_side = log->m_mode[ resolver_mode::resolver_flip ].m_side = {};

	record->m_resolver_mode = record->m_shot ? resolver_mode::resolver_shot : log->m_current_mode;
	record->m_resolver_side = log->m_current_side;

	if ( !record->m_shot )
	{
		const auto cureye = record->m_eye_angles;
		if ( fabsf( cureye.x ) >= 60.f )
			log->m_last_unusual_pitch = interfaces::globals()->curtime;
		else
			log->m_last_zero_pitch = interfaces::globals()->curtime;
	}

	if ( log->m_unknown_shot && log->m_mode[ log->m_current_mode ].m_side[ log->m_current_side ].m_current_dir > resolver_direction::resolver_networked )
		log->m_mode[ resolver_mode::resolver_shot ].m_side[ log->m_current_side ].m_current_dir = log->m_mode[ log->m_current_mode ].m_side[ log->m_current_side ].m_current_dir;
}

bool resolver::extrapolate_record( int ticks, lag_record_t& outrecord, const bool simple )
{
	if ( !ticks )
	{
		outrecord.setup_matrices();
		return true;
	}

	const auto player = globals::get_player( outrecord.m_index );

	const auto backup_lby = player->get_lby();
	const auto backup_layers = player->get_anim_layers();
	const auto backup_state = *player->get_anim_state();
	const auto backup_poses = player->get_pose_params();
	const auto backup_angle = player->get_abs_rotation();

	const auto backup_abs_origin = player->get_abs_origin();
	const auto backup_flags = player->get_flags();
	const auto backup_groundentity = player->get_ground_entity();
	const auto backup_move_type = player->get_move_type();
	const auto backup_velocity = player->get_velocity();
	const auto backup_ducking = player->get_ducking();

	outrecord.m_velocity = outrecord.m_calculated_velocity;

	player->get_velocity().z = outrecord.m_calculated_velocity.z;

	auto new_previous = std::make_unique<lag_record_t>();
	*new_previous = outrecord;
	new_previous->m_extrapolated = true;
	auto& log = player_log::get_log( outrecord.m_index );

	if ( simple )
	{
		process_move_changes_t backup_pm{};
		backup_pm.store( player );

		const auto original_record = log.record.back();
		const auto p1 = log.record.size() > 1 ? &log.record[ log.record.size() - 2 ] : nullptr;
		const auto p2 = log.record.size() > 2 ? &log.record[ log.record.size() - 3 ] : nullptr;

		int prev_buttons = 0;

		Vector predicted_vel_change{}, record_vel_change{};
		if ( p1 && p2 )
		{
			const auto p1_vel_change = ( p1->m_calculated_velocity - p2->m_calculated_velocity ) / p1->m_lagamt;
			record_vel_change = ( original_record.m_calculated_velocity - p1->m_calculated_velocity ) / original_record.m_lagamt;
			predicted_vel_change = record_vel_change - p1_vel_change;
		}

		const auto speed = original_record.m_velocity.Length2D();

		CUserCmd cmd{};
		for ( auto i = 0; i < ticks; i++ )
		{
			QAngle predicted_vel_change_ang;
			math::vector_angles( player->get_velocity() + predicted_vel_change, predicted_vel_change_ang );
			cmd.viewangles.y = predicted_vel_change_ang.y;
			cmd.viewangles.x = 0;

			cmd.forwardmove = speed > 5.f ? 450.f : ( i % 2 ? 1.01f : -1.01f );
			cmd.sidemove = 0.f;

			if ( original_record.m_duckamt > 0.f )
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

			player->set_abs_origin( data.m_vecAbsOrigin );
			player->get_velocity() = data.m_vecVelocity;

			if ( i == ticks - 1 )
				outrecord.m_origin = data.m_vecAbsOrigin;
		}

		backup_pm.restore( player );
		player->set_abs_origin( backup_abs_origin );
		player->get_flags() = backup_flags;
		player->get_ground_entity() = backup_groundentity;
		player->get_move_type() = backup_move_type;
		player->get_velocity() = backup_velocity;
		player->get_ducking() = backup_ducking;

		return true;
	}

	new_previous->m_velocity = outrecord.m_calculated_velocity;
	outrecord.m_simtime += interfaces::globals()->interval_per_tick * ticks;
	outrecord.m_lagamt = ticks;
	animations::update_player_animations( &outrecord, player, new_previous.get() );

	player->get_lby() = backup_lby;
	player->get_anim_layers() = backup_layers;
	*player->get_anim_state() = backup_state;
	player->get_pose_params() = backup_poses;
	player->set_abs_angles( backup_angle );
	player->get_velocity() = backup_velocity;

	for ( auto& state : outrecord.m_state )
		state.m_setup_tick = -1;
	outrecord.setup_matrices( resolver_direction::resolver_invalid, true );

	//aimbot_helpers::draw_debug_hitboxes( player, outrecord.matrix( player_log::get_log( outrecord.m_index ).get_dir( outrecord.m_shot, outrecord.m_resolver_mode ) ), -1, interfaces::globals()->interval_per_tick * 2 );

	/*for ( auto j = resolver_direction::resolver_networked; j < resolver_direction::resolver_direction_max; j++ )
	{
		if ( j == resolver_direction::resolver_min || j == resolver_direction::resolver_max )
		{
			aimbot_helpers::draw_debug_hitboxes( player, outrecord.matrix( j ), -1, interfaces::globals()->interval_per_tick * 2, Color::Blue( 100 ) );
		}

		if ( j == resolver_direction::resolver_min_min || j == resolver_direction::resolver_max_max )
		{
			aimbot_helpers::draw_debug_hitboxes( player, outrecord.matrix( j ), -1, interfaces::globals()->interval_per_tick * 2, Color::Green( 100 ) );
		}

		if ( j == resolver_direction::resolver_min_extra )
		{
			aimbot_helpers::draw_debug_hitboxes( player, outrecord.matrix( j ), -1, interfaces::globals()->interval_per_tick * 2, Color::Red( 100 ) );
		}

	}*/

	return true;
}

void resolver::pitch_resolve( lag_record_t* record )
{
	const auto& log = player_log::get_log( record->m_index );

	if ( globals::nospread )
	{
		if ( log.nospread.m_pitch_cycle % 2 && log.nospread.m_can_fake )
		{
			record->m_eye_angles.x = -record->m_eye_angles.x;
		}
	}

	record->m_pitch_cycle = log.nospread.m_pitch_cycle;
}


float resolver::get_resolver_angle( const lag_record_t& record, resolver_direction direction, float eye_angle )
{
	switch ( direction )
	{
		case resolver_direction::resolver_max:
			return math::normalize_float( eye_angle + record.m_state[ direction ].m_animstate.aim_yaw_max * record.m_yaw_modifier * 2.f );
		case resolver_direction::resolver_min:
			return math::normalize_float( eye_angle + record.m_state[ direction ].m_animstate.aim_yaw_min * record.m_yaw_modifier * 2.f );
		default:
			return eye_angle;
	}
}

void resolver::yaw_resolve( const lag_record_t* record, const lag_record_t* previous )
{
	if ( record->m_shot || ( previous && previous->m_shot ) )
		return;

	auto& log = player_log::get_log( record->m_index );

	const auto delta = !previous ? 0.f : math::normalize_float( record->m_eye_angles.y - previous->m_eye_angles.y );

	const auto use_nonflip = delta < -30.f;
	const auto use_flip = delta > 30.f;

	const auto previous_mode = log.m_current_mode;

	if ( ( log.m_current_mode == resolver_mode::resolver_flip && use_nonflip || log.m_current_mode == resolver_mode::resolver_default && use_flip ) )
		log.m_current_mode = static_cast< resolver_mode >( !static_cast< int >( log.m_current_mode ) );
	else if ( fabsf( delta ) > 170.f )
		log.m_current_mode = static_cast< resolver_mode >( !static_cast< int >( log.m_current_mode ) );

	if ( previous_mode != log.m_current_mode )
		log.m_last_flip_tick = interfaces::client_state()->get_last_server_tick();

	if ( interfaces::client_state()->get_last_server_tick() - log.m_last_flip_tick > time_to_ticks( 1.1f ) )
		log.m_current_mode = resolver_mode::resolver_default;
}

void resolver::on_createmove()
{
	if ( tickbase::force_choke )
		return;

	std::vector<std::shared_ptr<detail::call_queue::queue_element>> calls;


	static Vector last_eyepos = {};
	const auto eyepos = local_player->get_eye_pos();

	for ( const auto player : interfaces::entity_list()->get_players() )
	{
		auto& log = player_log::get_log( player->EntIndex() );
		if ( player->IsDormant() || !player->is_enemy() || log.record.empty() || player->get_player_info().fakeplayer || !log.is_hittable )
			continue;

		auto& newest = log.record.back();

		if ( fabsf( eyepos.Length() - last_eyepos.Length() ) > 2.f )
			newest.m_did_wall_detect = false;

		if ( newest.m_did_wall_detect )
			continue;

		wall_detect( &newest );
	}

	last_eyepos = eyepos;
}

void resolver::wall_detect( lag_record_t* record )
{
	auto& log = player_log::get_log( record->m_index );

	const auto should_change_desync = !record->m_shot && log.m_unknown && !record->m_did_wall_detect && record->m_lagamt >= 1;

	const auto weapon = local_weapon;
	if ( !weapon || !weapon->is_gun() )
		return;

	const auto player = globals::get_player( record->m_index );

	const auto hdr = player->get_model_ptr();
	if ( !hdr )
		return;

	const auto studio_hdr = hdr->m_pStudioHdr;
	if ( !studio_hdr )
		return;

	const auto hitbox_set = studio_hdr->pHitboxSet( player->get_hitbox_set() );
	if ( !hitbox_set )
		return;

	const auto hitbox = hitbox_set->pHitbox( HITBOX_HEAD );
	if ( !hitbox )
		return;

	record->m_did_wall_detect = true;

	auto get_rotated_pos = [] ( Vector start, const float rotation, const float distance )
	{
		const auto rad = DEG2RAD( rotation );
		start.x += cos( rad ) * distance;
		start.y += sin( rad ) * distance;

		return start;
	};


	const auto eye_pos = record->m_origin + Vector( 0.f, 0.f, 60.f );
	const auto target_position = current_eye;
	const auto target_angle = math::calc_angle( eye_pos, target_position );

	const auto weapon_info = interfaces::weapon_system()->GetWpnData( WEAPON_AWP );

	const auto local_pos_left = get_rotated_pos( eye_pos, math::normalize_float( target_angle.y - 90.f ), 25.f );
	const auto local_pos_right = get_rotated_pos( eye_pos, math::normalize_float( target_angle.y + 90.f ), 25.f );

	const auto local_half_pos_left = get_rotated_pos( eye_pos, math::normalize_float( target_angle.y - 90.f ), 12.f );
	const auto local_half_pos_right = get_rotated_pos( eye_pos, math::normalize_float( target_angle.y + 90.f ), 12.f );

	const auto enemy_pos_left = get_rotated_pos( target_position, math::normalize_float( target_angle.y - 90.f ), 25.f );
	const auto enemy_pos_right = get_rotated_pos( target_position, math::normalize_float( target_angle.y + 90.f ), 25.f );

	const auto compare = [&player, &weapon_info] ( const Vector& from_left, const Vector& from_right, const Vector& left, const Vector& right, const bool check = false ) -> int
	{
		auto pen_weapon = *weapon_info;
		if ( !check )
			pen_weapon.idamage = 200;

		aimbot::aimpoint_t aimpoint_left{};
		aimpoint_left.point = left;
		can_hit( player, penetration::pen_data( {}, {}, {}, {}, &pen_weapon ), from_left, &aimpoint_left, aimpoint_left.damage );

		if ( check )
			return aimpoint_left.damage > 0;

		aimbot::aimpoint_t aimpoint_right{};
		aimpoint_right.point = right;
		can_hit( player, penetration::pen_data( {}, {}, {}, {}, &pen_weapon ), from_right, &aimpoint_right, aimpoint_right.damage );

		if ( !aimpoint_left.damage && aimpoint_right.damage )
			return 1;

		if ( !aimpoint_right.damage && aimpoint_left.damage )
			return 2;

		return 0;
	};


	auto goal_dir = -1;

	if ( const auto res = compare( local_pos_left, local_pos_right, enemy_pos_left, enemy_pos_right ); res && !compare( eye_pos, eye_pos, res == 1 ? enemy_pos_left : enemy_pos_right, enemy_pos_right, true ) )
	{
		goal_dir = res == 1 ? 1 : 2;
	}
	else if ( const auto res = compare( local_pos_left, local_pos_right, enemy_pos_right, enemy_pos_left ); res && !compare( eye_pos, eye_pos, res == 1 ? enemy_pos_left : enemy_pos_right, enemy_pos_right, true ) )
	{
		goal_dir = res == 1 ? 1 : 2;
	}

	if ( goal_dir != -1 && compare( goal_dir == 1 ? local_half_pos_left : local_half_pos_right, target_position, target_position, target_position, true ) )
	{
		goal_dir = -1;
	}

	if ( goal_dir == -1 )
		return;

	log.m_wall_detect_ang = math::normalize_float( target_angle.y + ( goal_dir == 1 ? -90.f : 90.f ) );

	auto closest_state = resolver_direction::resolver_invalid;

	if ( should_change_desync )
	{
		record->setup_matrices();

		auto closest_angle = FLT_MAX;
		for ( auto i = resolver_direction::resolver_networked; i < resolver_direction::resolver_max_extra; i++ )
		{
			auto& state = record->m_state[ i ];

			const auto pos = Vector( state.m_matrix[ hitbox->bone ][ 0 ][ 3 ], state.m_matrix[ hitbox->bone ][ 1 ][ 3 ], state.m_matrix[ hitbox->bone ][ 2 ][ 3 ] );
			const auto angle = math::calc_angle( record->m_origin, pos );
			const auto diff = fabsf( math::normalize_float( angle.y - log.m_wall_detect_ang ) );
			if ( diff < closest_angle )
			{
				closest_angle = diff;
				closest_state = i;
			}
		}
	}

	log.m_current_side = goal_dir == 1 ? resolver_side::resolver_left : resolver_side::resolver_right;

	if ( closest_state != resolver_direction::resolver_invalid )
		log.m_mode[ log.m_current_mode ].m_side[ log.m_current_side ].m_current_dir = closest_state;

}

void resolver::add_shot( shot_t& shot )
{
	shots.emplace_back( shot );
}

void resolver::update_missed_shots( const ClientFrameStage_t& stage )
{
	if ( stage != FRAME_NET_UPDATE_END )
		return;

	auto it = shots.begin();
	while ( it != shots.end() )
	{
		const auto shot = *it;
		if ( shot.tick + time_to_ticks( 1.f ) < interfaces::globals()->tickcount || shot.tick - 10 > interfaces::globals()->tickcount )
		{
			it = shots.erase( it );
		}
		else
		{
			++it;
		}
	}

	auto it2 = current_shots.begin();
	while ( it2 != current_shots.end() )
	{
		const auto shot = *it2;
		if ( shot.tick + time_to_ticks( 1.f ) < interfaces::globals()->tickcount || shot.tick - 10 > interfaces::globals()->tickcount )
		{
			it2 = current_shots.erase( it2 );
		}
		else
		{
			++it2;
		}
	}
}

void resolver::hurt_listener( IGameEvent* game_event, record_shot_info_t& shot_info )
{
	_( attacker_s, "attacker" );
	_( userid_s, "userid" );
	_( hitgroup_s, "hitgroup" );
	_( dmg_health_s, "dmg_health" );

	const auto attacker = interfaces::engine()->GetPlayerForUserID( game_event->GetInt( attacker_s.c_str() ) );
	const auto victim = interfaces::engine()->GetPlayerForUserID( game_event->GetInt( userid_s.c_str() ) );
	const auto hitgroup = game_event->GetInt( hitgroup_s.c_str() );
	const auto damage = game_event->GetInt( dmg_health_s.c_str() );

	if ( attacker != interfaces::engine()->GetLocalPlayer() )
		return;

	if ( victim == interfaces::engine()->GetLocalPlayer() )
		return;

	const auto player = globals::get_player( victim );
	if ( !player || !player->is_enemy() )
		return;

	if ( unapproved_shots.empty() )
		return;

	for ( auto& shot : unapproved_shots )
	{
		if ( !shot.hurt && shot.enemy_index == victim )
		{
			shot.hurt = true;
			shot.hitinfo.victim = victim;
			shot.hitinfo.hitgroup = hitgroup;
			shot.hitinfo.damage = damage;
			shot_info = shot.record.m_shot_info;
			return;
		}
	}
}

resolver::shot_t* resolver::closest_shot( int tickcount )
{
	shot_t* closest_shot = nullptr;
	for ( auto& shot : shots )
	{
		closest_shot = &shot;
		break;
	}

	return closest_shot;
}

bool resolver::record_shot( IGameEvent* game_event )
{
	_( userid_s, "userid" );

	const auto userid = interfaces::engine()->GetPlayerForUserID( game_event->GetInt( userid_s.c_str() ) );
	const auto player = globals::get_player( userid );

	if ( player != local_player )
		return false;

	const auto shot = closest_shot( interfaces::globals()->tickcount - time_to_ticks( interfaces::engine()->GetNetChannelInfo()->GetLatency( FLOW_OUTGOING ) ) );
	if ( !shot )
		return false;

	current_shots.push_front( *shot );
	shots.pop_front();
	current_hitposes.clear();

	return true;
}

void resolver::listener( IGameEvent* game_event )
{
	_( weapon_fire, "weapon_fire" );
	static auto last_tickcount = 0;

	if ( !strcmp( game_event->GetName(), weapon_fire.c_str() ) )
	{
		if ( record_shot( game_event ) )
			last_tickcount = 0;
		return;
	}

	if ( current_shots.empty() )
		return;

	_( userid_s, "userid" );

	const auto userid = interfaces::engine()->GetPlayerForUserID( game_event->GetInt( userid_s.c_str() ) );
	const auto player = globals::get_player( userid );
	if ( !player || player != local_player )
		return;

	_( x, "x" );
	_( y, "y" );
	_( z, "z" );

	const Vector pos( game_event->GetFloat( x.c_str() ), game_event->GetFloat( y.c_str() ), game_event->GetFloat( z.c_str() ) );

	const auto shot = &current_shots[ 0 ];

	static auto counter = 0;

	if ( last_tickcount == interfaces::globals()->tickcount )
		counter++;
	else
	{
		current_hitposes.clear();
		counter = 0;
	}

	if ( counter )
		unapproved_shots.pop_front();

	current_hitposes.push_back( pos );
	shot->hitposes = current_hitposes;
	unapproved_shots.emplace_back( *shot );

	last_tickcount = interfaces::globals()->tickcount;
}

Vector resolver::get_closest_hitpos( const shot_t& shot, const Vector& pos )
{
	Vector closest = {};
	auto last_dist = FLT_MAX;
	for ( auto& hitpos : shot.hitposes )
	{
		const auto dist = hitpos.Dist( pos );
		if ( dist < last_dist )
		{
			last_dist = dist;
			closest = hitpos;
		}
	}

	return closest;
}

Vector resolver::get_closest_penetrationpos( const shot_t& shot, const Vector& pos )
{
	Vector closest = {};
	auto last_dist = FLT_MAX;
	for ( auto& hitpos : shot.penetration_points )
	{
		const auto dist = hitpos.Dist( pos );
		if ( dist < last_dist )
		{
			last_dist = dist;
			closest = hitpos;
		}
	}

	return closest;
}

void resolver::approve_shots( const ClientFrameStage_t& stage )
{
	if ( stage != FRAME_NET_UPDATE_END )
		return;

	for ( auto& shot : unapproved_shots )
	{
		if ( shot.hitposes.empty() )
			continue;

		auto end = shot.hitposes[ shot.hitposes.size() - 1 ];

		if ( vars::misc.impacts->get<bool>() )
		{
			auto col2 = Color( vars::misc.impacts_color2->get<D3DCOLOR>() );

			for ( auto& point : shot.hitposes )
				interfaces::debug_overlay()->AddBoxOverlay( point, Vector( -1.25f, -1.25f, -1.25f ), Vector( 1.25f, 1.25f, 1.25f ), QAngle( 0, 0, 0 ), col2.r(), col2.g(), col2.b(), 180, 4 );
		}

		if ( local_player && local_player->get_alive() && prediction::get_pred_info( shot.cmdnum ).sequence == shot.cmdnum )
		{
			auto new_origin = prediction::get_pred_info( shot.cmdnum ).origin;
			shot.shotpos.x = new_origin.x;
			shot.shotpos.y = new_origin.y;
		}

		const auto angles = math::calc_angle( shot.shotpos, end );
		Vector direction{};
		math::angle_vectors( angles, &direction );

		if ( shot.record.m_index == -1 )
		{
			if ( shot.hurt )
			{
				if ( shot.penetration_points.empty() )
					continue;

				shot.hitpos = get_closest_hitpos( shot, shot.penetration_points[ shot.penetration_points.size() - 1 ] );
			}

			Vector zerovec = {};
			lua::api.callback( FNV1A( "on_shot_registered" ), [&] ( lua::state& state )
			{
				state.create_table();
				state.set_field( XOR_STR( "manual" ), true );
				state.set_field( XOR_STR( "secure" ), false );
				state.set_field( XOR_STR( "very_secure" ), false );
				state.set_field( XOR_STR( "result" ), shot.hurt ? XOR_STR( "hit" ) : XOR_STR( "miss" ) );
				state.set_field( XOR_STR( "target" ), -1 );
				state.set_field( XOR_STR( "tick" ), shot.tick );
				state.set_field( XOR_STR( "backtrack" ), 0 );
				state.set_field( XOR_STR( "hitchance" ), -1 );
				state.set_field( XOR_STR( "client_hitgroup" ), -1 );
				state.set_field( XOR_STR( "client_damage" ), -1 );
				state.set_field( XOR_STR( "server_hitgroup" ), shot.hitinfo.hitgroup );
				state.set_field( XOR_STR( "server_damage" ), shot.hitinfo.damage );
				state.create_user_object<decltype( shot.shotpos )>( XOR_STR( "vec3" ), &shot.shotpos );
				state.set_field( XOR_STR( "shotpos" ) );
				state.create_user_object<decltype( zerovec )>( XOR_STR( "vec3" ), &zerovec );
				state.set_field( XOR_STR( "client_hitpos" ) );
				state.create_user_object<decltype( shot.hitpos )>( XOR_STR( "vec3" ), shot.hurt ? &shot.hitpos : &zerovec );
				state.set_field( XOR_STR( "server_hitpos" ) );
				state.create_table();
				{
					auto index = 1;
					for ( auto cur : shot.penetration_points )
					{
						state.create_user_object<decltype( cur )>( XOR_STR( "vec3" ), &cur );
						state.set_i( index++ );
					}
				}
				state.set_field( XOR_STR( "client_impacts" ) );
				state.create_table();
				{
					auto index = 1;
					for ( auto cur : shot.hitposes )
					{
						state.create_user_object<decltype( cur )>( XOR_STR( "vec3" ), &cur );
						state.set_i( index++ );
					}
				}
				state.set_field( XOR_STR( "server_impacts" ) );
				return 1;
			} );

			if ( shot.hurt )
			{
				const auto player = globals::get_player( shot.hitinfo.victim );
				if ( player )
				{
					add_hit( hitmarker::hitmarker_t( interfaces::globals()->realtime, shot.hitinfo.victim, shot.hitinfo.damage, shot.hitinfo.hitgroup, shot.hitpos ) );

					if ( vars::visuals.beams.local.enabled->get<bool>() && vars::visuals.beams.enabled->get<bool>() )
						add_local_beam( beams::impact_info_t( interfaces::globals()->curtime, shot.shotpos, end, interfaces::engine()->GetLocalPlayer(), Color( vars::visuals.beams.local.color->get<D3DCOLOR>() ) ) );
					continue;
				}
			}

			if ( vars::visuals.beams.local.enabled->get<bool>() && vars::visuals.beams.enabled->get<bool>() )
				add_local_beam( beams::impact_info_t( interfaces::globals()->curtime, shot.shotpos, end, interfaces::engine()->GetLocalPlayer(), Color( vars::visuals.beams.local.color->get<D3DCOLOR>() ) ) );
			continue;
		}

		auto hitpos = get_closest_hitpos( shot, shot.hitgroup != -1 ? shot.hitpos : shot.record.m_origin );

		auto player = globals::get_player( shot.enemy_index );
		if ( vars::visuals.chams.enemy.shot_record.type->get<int>() && player )
			chams::add_ghost( player, &shot.record );

		if ( !player )
		{
			// maybe add shot info

			shot.hitpos = hitpos;
			if ( shot.hurt )
			{
				add_hit( hitmarker::hitmarker_t( interfaces::globals()->realtime, shot.hitinfo.victim, shot.hitinfo.damage, shot.hitinfo.hitgroup, hitpos ) );

				if ( vars::visuals.beams.local.enabled->get<bool>() && vars::visuals.beams.enabled->get<bool>() && !beams::beam_exists( local_player, interfaces::globals()->curtime ) )
					add_local_beam( beams::impact_info_t( interfaces::globals()->curtime, shot.shotpos, end, interfaces::engine()->GetLocalPlayer(), Color( vars::visuals.beams.local.color->get<D3DCOLOR>() ) ) );
			}
			else if ( vars::visuals.beams.local.enabled->get<bool>() && vars::visuals.beams.enabled->get<bool>() )
				add_local_beam( beams::impact_info_t( interfaces::globals()->curtime, shot.shotpos, end, interfaces::engine()->GetLocalPlayer(), Color( vars::visuals.beams.local.color->get<D3DCOLOR>() ) ) );

			continue;
		}

		if ( !local_player || !local_player->get_alive() || !local_weapon )
			continue;

		shot.hitpos = shot.hitposes[ shot.hitposes.size() - 1 ] + direction * 1000.f;

		auto& log = player_log::get_log( shot.enemy_index );
		auto data = penetration::pen_data( &shot.record, shot.record.m_shot_dir, false, nullptr, &shot.weapon_data );

		if ( shot.record.m_shot_info.extrapolated && !log.record.empty() && !log.record.back().m_dormant )
		{
			//aimbot_helpers::draw_debug_hitboxes( player, log.record.back().matrix( shot.record.m_shot_state ), -1, 5.f, Color( 0, 255, 255, 255 ) );
			data = penetration::pen_data( &log.record.back(), shot.record.m_shot_dir, false, nullptr, &shot.weapon_data );
		}

		aimbot::aimpoint_t aimpoint;
		aimpoint.hitbox = -1;
		aimpoint.point = end;

		auto damage = 0;
		auto new_data = data;
		if ( can_hit( local_player, new_data, shot.shotpos, &aimpoint, damage, true ) )
		{
			hitpos = get_closest_hitpos( shot, aimpoint.point );
			shot.hitpos = hitpos;
			shot.hit = true;
			shot.hit_originally = true;
		}

		const auto deltavec = Vector( shot.original_shotpos.x - shot.shotpos.x, shot.original_shotpos.y - shot.shotpos.y, 0 );
		const auto corrected_pos = fabsf( deltavec.x ) >= 0.001f || fabsf( deltavec.y ) >= 0.001f;

		if ( corrected_pos )
		{
			auto damage2 = 0;
			shot.hit_originally = can_hit( local_player, data, shot.original_shotpos, &aimpoint, damage2, true );
		}

		if ( shot.record.m_shot_info.extrapolated )
		{
			//aimbot_helpers::draw_debug_hitboxes( player, shot.record.matrix( shot.record.m_shot_state ), -1, 5.f, Color( 255, 255, 255, 255 ) );

			auto damage2 = 0;
			shot.hit_extrapolation = can_hit( local_player, penetration::pen_data( &shot.record, shot.record.m_shot_dir, false, nullptr, &shot.weapon_data ), shot.shotpos, &aimpoint, damage2, true );
		}

		if ( vars::visuals.beams.local.enabled->get<bool>() && vars::visuals.beams.enabled->get<bool>() )
			add_local_beam( beams::impact_info_t( interfaces::globals()->curtime, shot.shotpos, end, interfaces::engine()->GetLocalPlayer(), Color( vars::visuals.beams.local.color->get<D3DCOLOR>() ) ) );

		if ( shot.hurt )
			add_hit( hitmarker::hitmarker_t( interfaces::globals()->realtime, shot.hitinfo.victim, shot.hitinfo.damage, shot.hitinfo.hitgroup, hitpos ) );

		if ( shot.hitgroup == -1 )
			continue;

		Vector zerovec = {};

		lua::api.callback( FNV1A( "on_shot_registered" ), [&] ( lua::state& state )
		{
			state.create_table();
			state.set_field( XOR_STR( "manual" ), shot.hitgroup == -1 );
			state.set_field( XOR_STR( "secure" ), shot.safety >= penetration::safety_no_roll );
			state.set_field( XOR_STR( "very_secure" ), shot.safety >= penetration::safety_full );
			state.set_field( XOR_STR( "result" ), shot.hurt ? XOR_STR( "hit" ) : shot.hit ? XOR_STR( "resolve" ) : shot.hit_extrapolation ? ( !ConVar::cl_lagcompensation || !ConVar::cl_predict ) ? XOR_STR( "anti-exploit" ) : XOR_STR( "extrapolation" ) : shot.hit_originally ? XOR_STR( "server correction" ) : XOR_STR( "spread" ) );
			state.set_field( XOR_STR( "target" ), shot.enemy_index );
			state.set_field( XOR_STR( "tick" ), shot.tick );
			state.set_field( XOR_STR( "backtrack" ), shot.record.m_shot_info.backtrack_ticks );
			state.set_field( XOR_STR( "hitchance" ), shot.record.m_shot_info.hitchance );
			state.set_field( XOR_STR( "client_hitgroup" ), shot.hitgroup );
			state.set_field( XOR_STR( "client_damage" ), shot.damage );
			state.set_field( XOR_STR( "server_hitgroup" ), shot.hitinfo.hitgroup );
			state.set_field( XOR_STR( "server_damage" ), shot.hitinfo.damage );
			state.create_user_object<decltype( shot.shotpos )>( XOR_STR( "vec3" ), &shot.shotpos );
			state.set_field( XOR_STR( "shotpos" ) );
			state.create_user_object<decltype( end )>( XOR_STR( "vec3" ), &end );
			state.set_field( XOR_STR( "client_hitpos" ) );
			state.create_user_object<decltype( shot.hitpos )>( XOR_STR( "vec3" ), shot.hurt ? &shot.hitpos : &zerovec );
			state.set_field( XOR_STR( "server_hitpos" ) );
			state.create_table();
			{
				auto index = 1;
				for ( auto cur : shot.penetration_points )
				{
					state.create_user_object<decltype( cur )>( XOR_STR( "vec3" ), &cur );
					state.set_i( index++ );
				}
			}
			state.set_field( XOR_STR( "client_impacts" ) );
			state.create_table();
			{
				auto index = 1;
				for ( auto cur : shot.hitposes )
				{
					state.create_user_object<decltype( cur )>( XOR_STR( "vec3" ), &cur );
					state.set_i( index++ );
				}
			}
			state.set_field( XOR_STR( "server_impacts" ) );
			return 1;
		} );

		if ( player->get_player_info().fakeplayer )
		{
			calc_missed_shots( &shot );

			continue;
		}

		if ( vars::legit_enabled() )
			continue;

		get_brute_angle( &shot );

		calc_missed_shots( &shot );
	}

	current_shots.clear();
	unapproved_shots.clear();
	current_hitposes.clear();
}

void resolver::get_brute_angle( shot_t* shot )
{
	if ( !local_player || !local_player->get_alive() || !local_weapon || shot->record.m_dormant )
		return;

	const auto player = globals::get_player( shot->enemy_index );
	if ( !player || !player->get_alive() )
		return;

	const auto hdr = player->get_model_ptr();
	if ( !hdr )
		return;

	const auto studio_hdr = hdr->m_pStudioHdr;
	if ( !studio_hdr )
		return;

	const auto hitbox_set = studio_hdr->pHitboxSet( player->get_hitbox_set() );
	if ( !hitbox_set )
		return;

	const auto hitbox = hitbox_set->pHitbox( HITBOX_HEAD );
	if ( !hitbox )
		return;

	const auto log = &player_log::get_log( shot->enemy_index );

	if ( vars::aim.resolver_mode->get<int>() )
		for ( auto& mode : log->m_mode )
			for ( auto& side : mode.m_side )
				if ( side.m_current_dir > resolver_direction::resolver_max )
				{
					side.m_current_dir = resolver_direction::resolver_networked;
					log->m_unknown_shot = true;
					log->m_unknown = true;
				}

	const auto use_front = shot->record.m_shot_info.extrapolated && !log->record.empty() && !log->record.back().m_dormant;
	const auto target_record = use_front ? &log->record.back() : &shot->record;
	if ( use_front )
		target_record->setup_matrices();

	const auto state = shot->record.m_shot_dir;
	const auto current_mode = target_record->m_shot ? resolver_mode::resolver_shot : shot->record.m_resolver_mode;
	const auto current_side = shot->record.m_resolver_side;

	const auto cureye = shot->record.m_eye_angles;
	const auto legit = fabsf( cureye.x ) < 60.f && ( shot->record.m_lagamt < 4 || shot->record.m_lagamt > 18 ) && !shot->record.m_shot && log->m_unknown;

	Vector shot_dir = {};
	const auto shot_angle = math::calc_angle( shot->shotpos, shot->hitpos );
	math::angle_vectors( shot_angle, &shot_dir );

	const auto end = shot->hitpos + shot_dir * 15.f;

	if ( !shot->hurt )
	{
		enum_array<resolver_direction, bool, resolver_direction::resolver_direction_max> new_blacklist = {};

		for ( auto i = resolver_direction::resolver_networked; i < ( vars::aim.resolver_mode->get<int>() ? resolver_direction::resolver_max_extra : resolver_direction::resolver_direction_max ); i++ )
		{
			//aimbot_helpers::draw_debug_hitboxes( player, shot->record.matrix( i ), -1, 3.f, Color( ( int ) i * 60, 255, 255, 255 ) );

			aimbot::aimpoint_t aimpoint{};
			aimpoint.hitbox = -2;
			aimpoint.point = end;

			auto damage = 0;
			can_hit( local_player, penetration::pen_data( target_record, i, false, nullptr, &shot->weapon_data ), shot->shotpos, &aimpoint, damage, true );

			if ( damage > 1.f )
				new_blacklist[ i ] = log->m_mode[ current_mode ].m_side[ current_side ].m_blacklist[ i ] = true;
		}

		if ( shot->hit )
		{
			auto furthest_angle = -FLT_MAX;
			auto furthest_dir = resolver_direction::resolver_invalid;

			const auto target_pos = Vector( target_record->m_state[ state ].m_matrix[ hitbox->bone ][ 0 ][ 3 ], target_record->m_state[ state ].m_matrix[ hitbox->bone ][ 1 ][ 3 ], target_record->m_state[ state ].m_matrix[ hitbox->bone ][ 2 ][ 3 ] );
			const auto target_state_ang = math::calc_angle( target_record->m_origin, target_pos );

			for ( auto i = resolver_direction::resolver_networked; i < ( vars::aim.resolver_mode->get<int>() ? resolver_direction::resolver_max_extra : resolver_direction::resolver_direction_max ); i++ )
			{
				if ( log->m_mode[ current_mode ].m_side[ current_side ].m_blacklist[ i ] )
					continue;

				const auto pos = Vector( target_record->m_state[ i ].m_matrix[ hitbox->bone ][ 0 ][ 3 ], target_record->m_state[ i ].m_matrix[ hitbox->bone ][ 1 ][ 3 ], target_record->m_state[ i ].m_matrix[ hitbox->bone ][ 2 ][ 3 ] );
				const auto angle = math::calc_angle( target_record->m_origin, pos );
				const auto diff = fabsf( math::normalize_float( angle.y - target_state_ang.y ) );
				if ( diff > furthest_angle )
				{
					furthest_dir = i;
					furthest_angle = diff;
				}
			}

			if ( furthest_dir != resolver_direction::resolver_invalid )
			{
				log->m_mode[ current_mode ].m_side[ current_side ].m_current_dir = furthest_dir;
			}
			else
			{
				log->m_mode[ current_mode ].m_side[ current_side ].m_current_dir = resolver_direction::resolver_networked;
				if ( log->nospread.m_can_fake && log->nospread.m_pitch_cycle % 2 == shot->record.m_pitch_cycle % 2 )
					log->nospread.m_pitch_cycle++;

				if ( globals::nospread )
					new_blacklist = {};
				else if ( new_blacklist[ resolver_direction::resolver_networked ] )
				{
					furthest_angle = -FLT_MAX;
					for ( auto i = resolver_direction::resolver_networked; i < ( vars::aim.resolver_mode->get<int>() ? resolver_direction::resolver_max_extra : resolver_direction::resolver_direction_max ); i++ )
					{
						if ( new_blacklist[ i ] )
							continue;

						const auto pos = Vector( target_record->m_state[ i ].m_matrix[ hitbox->bone ][ 0 ][ 3 ], target_record->m_state[ i ].m_matrix[ hitbox->bone ][ 1 ][ 3 ], target_record->m_state[ i ].m_matrix[ hitbox->bone ][ 2 ][ 3 ] );
						const auto angle = math::calc_angle( target_record->m_origin, pos );
						const auto diff = fabsf( math::normalize_float( angle.y - target_state_ang.y ) );
						if ( diff > furthest_angle )
						{
							log->m_mode[ current_mode ].m_side[ current_side ].m_current_dir = i;
							furthest_angle = diff;
						}
					}
				}

				log->m_mode[ current_mode ].m_side[ current_side ].m_blacklist = new_blacklist;
			}
		}
	}
	else if ( !legit )
	{
		enum_array<resolver_direction, bool, resolver_direction::resolver_direction_max> new_blacklist = {};
		enum_array<resolver_direction, float, resolver_direction::resolver_direction_max> hit_dist{};
		hit_dist.fill( FLT_MAX );

		for ( auto i = resolver_direction::resolver_networked; i < ( vars::aim.resolver_mode->get<int>() ? resolver_direction::resolver_max_extra : resolver_direction::resolver_direction_max ); i++ )
		{
			//aimbot_helpers::draw_debug_hitboxes( player, shot->record.matrix, -1, 3.f, Color( ( int ) i * 60, 255, 255, 255 ) );

			aimbot::aimpoint_t aimpoint{};
			aimpoint.hitbox = -2;
			aimpoint.point = end;

			auto damage = 0;
			can_hit( local_player, penetration::pen_data( target_record, i, false, nullptr, &shot->weapon_data ), shot->shotpos, &aimpoint, damage, true );

			if ( damage < 1.f || aimpoint.hitgroup != shot->hitinfo.hitgroup )
				new_blacklist[ i ] = log->m_mode[ current_mode ].m_side[ current_side ].m_blacklist[ i ] = true;
			else
			{
				const auto hitpos = get_closest_hitpos( *shot, aimpoint.point );
				hit_dist[ i ] = aimpoint.point.Dist( hitpos );
			}
		}

		auto blacklist_full = true;
		auto blacklist_invalid = false;

		const auto prev_state = shot->record.m_shot_dir;

		auto closest = FLT_MAX;
		for ( auto i = resolver_direction::resolver_networked; i < ( vars::aim.resolver_mode->get<int>() ? resolver_direction::resolver_max_extra : resolver_direction::resolver_direction_max ); i++ )
		{
			if ( !log->m_mode[ current_mode ].m_side[ current_side ].m_blacklist[ i ] )
				blacklist_full = false;

			auto& cur = hit_dist[ i ];

			if ( log->m_mode[ current_mode ].m_side[ current_side ].m_blacklist[ i ] && cur != FLT_MAX )
				blacklist_invalid = true;

			if ( cur < closest && hit_dist[ prev_state ] > 0.1f )
			{
				log->m_mode[ current_mode ].m_side[ current_side ].m_current_dir = i;
				closest = cur;
			}
		}

		if ( blacklist_full || blacklist_invalid )
		{
			log->m_mode[ current_mode ].m_side[ current_side ].m_blacklist = new_blacklist;
		}
	}

	if ( log->m_mode[ current_mode ].m_side[ current_side ].m_current_dir != state || shot->hurt )
	{
		if ( current_mode == resolver_mode::resolver_shot )
			log->m_unknown_shot = false;
		else
			log->m_unknown = false;
	}

	if ( log->m_unknown && player->get_alive() && log->m_mode[ current_mode ].m_side[ current_side ].m_current_dir != state && current_mode != resolver_mode::resolver_shot && state != resolver_direction::resolver_networked )
	{
		const auto current_state = log->m_mode[ current_mode ].m_side[ current_side ].m_current_dir;
		if ( current_state == resolver_direction::resolver_min_min || current_state == resolver_direction::resolver_max_max || current_state == resolver_direction::resolver_min_extra || current_state == resolver_direction::resolver_max_extra )
			log->m_mode[ static_cast< resolver_mode >( ( static_cast< int >( current_mode ) + 1 ) % 2 ) ].m_side[ current_side ].m_current_dir = current_state == resolver_direction::resolver_min_min || current_state == resolver_direction::resolver_min_extra ? resolver_direction::resolver_max_max : resolver_direction::resolver_min_min;
		else
			log->m_mode[ static_cast< resolver_mode >( ( static_cast< int >( current_mode ) + 1 ) % 2 ) ].m_side[ current_side ].m_current_dir = current_state == resolver_direction::resolver_min ? resolver_direction::resolver_max : resolver_direction::resolver_min;
	}
}

void resolver::calc_missed_shots( shot_t* shot )
{
	const auto log = &player_log::get_log( shot->enemy_index );
	if ( shot->record.m_dormant )
	{
		if ( shot->hurt )
		{
			const auto sound_player = sound_esp::get_sound_player( shot->enemy_index );
			sound_player->last_update_tick = interfaces::client_state()->get_last_server_tick();
			sound_player->updated = true;
			log->m_dormant_misses = 0;
		}
		else
		{
			log->m_dormant_misses++;
			interfaces::cvar()->ConsoleColorPrintf( Color( 235, 5, 90, 255 ), xorstr_( "[fatality] " ) );
			util::print_dev_console( true, Color( 255, 255, 255, 255 ), xorstr_( "miss due to dormant aimbot\n" ) );
		}
		return;
	}

	if ( shot->hurt && globals::nospread && shot->hitinfo.hitgroup == HITGROUP_HEAD && !shot->record.m_shot )
		log->nospread.m_pitch_cycle = 0;

	const auto player = globals::get_player( shot->enemy_index );

	if ( shot->hurt )
		return;

	if ( shot->hit && player && player->get_alive() )
	{
		if ( shot->record.m_unknown )
			log->m_unknown_misses++;

		log->m_shots++;
	}


	if ( shot->hit )
		return;

	log->m_shots_spread++;

	_( fatality, "[fatality] " );
	_( missed, "miss due to spread\n" );
	_( missed2, "miss due to server correction\n" );
	_( missed3, "miss due to extrapolation\n" );
	_( missed3_2, "miss due to anti-exploit\n" );

	interfaces::cvar()->ConsoleColorPrintf( Color( 235, 5, 90, 255 ), fatality.c_str() );

	if ( shot->hit_extrapolation )
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), ( !ConVar::cl_lagcompensation || !ConVar::cl_predict ) ? missed3_2.c_str() : missed3.c_str() );
	else if ( shot->hit_originally )
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), missed2.c_str() );
	else
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), missed.c_str() );
}

void resolver::set_local_info()
{
	last_origin_diff = local_player->get_origin() - last_origin;
	last_eye = local_player->get_eye_pos();
	last_origin = local_player->get_origin();
	current_eye = local_player->get_eye_pos();
}