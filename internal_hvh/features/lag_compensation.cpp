#include "../include_cheat.h"

lag_record_t::lag_record_t( C_CSPlayer* player )
{
	m_recv_time = interfaces::globals()->curtime;
	m_index = player->EntIndex();
	m_valid = true;
	m_origin = player->get_origin();
	m_abs_origin = player->get_abs_origin();
	m_layers = player->get_anim_layers();
	m_poses = player->get_pose_params();
	m_old_simtime = player->get_oldsimtime();
	m_simtime = player->get_simtime();
	m_duckamt = player->get_duck_amt();
	m_lby = player->get_lby();
	m_eye_angles = *player->eye_angles_virtual();
	m_eye_angles.NormalizeNoClamp();
	m_abs_ang = player->get_abs_angles();
	do_not_set = false;
	m_strafing = player->get_strafing();
	m_dormant = player->IsDormant();
	m_velocity_modifier = player->get_velocity_modifier();
	for ( auto& state : m_state )
	{
		state.m_abs_ang = player->get_abs_angles();
		state.m_poses = m_poses;
	}

	m_flags = player->get_flags();
	m_ground_entity = player->get_ground_entity();
	m_lagamt = time_to_ticks( m_simtime - m_old_simtime );
	m_velocity = ( player->get_origin() - *player->get_old_origin() ) * ( 1.f / ticks_to_time( m_lagamt ) );
	if ( m_flags & FL_ONGROUND )
		m_velocity.z = 0.f;
	if ( !m_velocity.IsValid() )
		m_velocity = Vector( 0.f, 0.f, 0.f );
	m_shot = false;
	const auto collideable = player->GetCollideable();
	m_obb_maxs = collideable->OBBMaxs();
	m_obb_mins = collideable->OBBMins();
	m_net_time_deviation = ticks_to_time( interfaces::client_state()->get_last_server_tick() ) - m_simtime;
	m_server_tick = interfaces::client_state()->get_last_server_tick();
	if ( player->get_anim_state() )
	{
		for ( auto& state : m_state )
		{
			state.m_animstate = *player->get_anim_state();
			state.m_abs_ang = { 0.f, state.m_animstate.foot_yaw, 0.f };
		}
		has_animstate = true;
	}
	m_tickbase_shift = false;

	m_collision_viewheight = player->get_collision_viewheight();
	m_collision_change_time = player->get_collision_bounds_change_time();
}

void lag_record_t::setup_matrices( resolver_direction dir, bool extrapolated )
{
	if ( dir != resolver_direction::resolver_invalid )
	{
		if ( extrapolated || this->m_state[ dir ].m_setup_tick != interfaces::globals()->tickcount )
			C_CSPlayer::setup_bones( *this, dir, extrapolated );

		return;
	}

	const auto player = globals::get_player( this->m_index );
	for ( auto j = resolver_direction::resolver_networked; j < resolver_direction::resolver_max_extra; j++ )
	{
		if ( player->get_player_info().fakeplayer && j != resolver_direction::resolver_networked )
			continue;

		if ( extrapolated || this->m_state[ j ].m_setup_tick != interfaces::globals()->tickcount )
			C_CSPlayer::setup_bones( *this, j, extrapolated );
	}
}

bool lag_record_t::can_delay_shot() const
{
	const auto rtt = misc::get_latency();
	return this->m_real_lag > time_to_ticks( rtt ) && ConVar::cl_lagcompensation && ConVar::cl_predict;
}

int lag_record_t::ticks_behind( int lag ) const
{
	return clamp( interfaces::client_state()->get_last_server_tick() - this->m_server_tick, 0, sv_maxusrcmdprocessticks );
}

bool lag_record_t::delay_shot() const
{
	const auto rtt = misc::get_latency();
	return time_to_ticks( rtt ) + ticks_behind() > this->m_real_lag;
}

bool lag_record_t::breaking_lc() const
{
	return this->m_breaking_lc && !this->m_dormant || !ConVar::cl_lagcompensation || !ConVar::cl_predict;
}

bool lagcomp::run_extrapolation( const C_CSPlayer* player, const bool simple )
{
	auto& log = player_log::get_log( player->EntIndex() );
	const auto& first = log.record.back();

	if ( ConVar::cl_lagcompensation && ConVar::cl_predict && ( first.m_simtime < log.m_highest_simtime || first.m_simtime == log.m_highest_simtime && first.m_ignore ) )
	{
		for ( auto i = log.record.size() - 1; i >= 0; i-- )
		{
			const auto record = &log.record[ i ];
			if ( record->m_simtime == log.m_highest_simtime )
			{
				record->setup_matrices();
				log.extrapolated_record = *record;
				log.extrapolated_record.m_breaking_lc = record->breaking_lc();
				log.extrapolated_record.m_shot_info.breaking_lc = log.extrapolated_record.m_breaking_lc;
				log.extrapolated_record.m_extrapolated = false;
				log.extrapolated_record.m_to_server_position = true;
				log.extrapolated_record.m_valid = true;
				log.extrapolated_record.m_shot_info.has_info = true;
				log.extrapolated_record.m_shot_info.delay_shot = false;
				log.extrapolated_record.m_shot_info.extrapolated = false;
				log.extrapolated_record.m_shot_info.backtrack_ticks = 0;
				return true;
			}
		}

		return false;
	}

	const auto record = &log.record.back();

	const auto rtt = misc::get_latency();

	const auto lag = std::max( 1, record->m_lagamt );

	log.extrapolated_record = *record;
	log.extrapolated_record.m_breaking_lc = record->breaking_lc();
	log.extrapolated_record.m_shot_info.breaking_lc = log.extrapolated_record.m_breaking_lc;

	const auto possible_future_tick = interfaces::client_state()->get_last_server_tick() + 1 + time_to_ticks( rtt ) + 8;
	const auto max_future_ticks = possible_future_tick - time_to_ticks( log.extrapolated_record.m_simtime + get_lerp_time() );
	const auto ticks_behind = record->ticks_behind( lag );

	auto to_server_position = log.extrapolated_record.m_breaking_lc;

	if ( !to_server_position )
	{
		for ( auto i = 1; i <= max_future_ticks; i++ )
		{
			log.extrapolated_record.m_lagamt = clamp( ( i + ticks_behind ) / lag * lag, 0, 64 );
			if ( valid_simtime( record->m_simtime + ticks_to_time( log.extrapolated_record.m_lagamt ) ) )
				break;
		}

		if ( !valid_simtime( record->m_simtime + ticks_to_time( log.extrapolated_record.m_lagamt ) ) )
			to_server_position = true;
	}

	const auto server_position = clamp( ( time_to_ticks( rtt ) + ticks_behind ) / lag * lag, 0, 64 );
	if ( to_server_position || server_position < log.extrapolated_record.m_lagamt )
		log.extrapolated_record.m_lagamt = server_position;

	log.extrapolated_record.m_tickbase_shift = false;

	resolver::extrapolate_record( log.extrapolated_record.m_lagamt, log.extrapolated_record, simple );

	log.extrapolated_record.m_extrapolate_amt = log.extrapolated_record.m_lagamt;

	if ( log.extrapolated_record.m_lagamt > max_future_ticks )
		log.extrapolated_record.m_simtime = ticks_to_time( possible_future_tick - 2 ) - get_lerp_time();

	log.extrapolated_record.m_extrapolated = true;
	log.extrapolated_record.m_to_server_position = to_server_position;
	log.extrapolated_record.m_valid = true;
	log.extrapolated_record.m_lagamt = lag;
	log.extrapolated_record.m_shot_info.has_info = true;
	log.extrapolated_record.m_shot_info.delay_shot = false;
	log.extrapolated_record.m_shot_info.extrapolated = true;
	log.extrapolated_record.m_shot_info.backtrack_ticks = time_to_ticks( log.m_highest_simtime - log.extrapolated_record.m_simtime );

	return true;
}

int lagcomp::fix_tickcount( const float& simtime )
{
	return time_to_ticks( simtime + get_lerp_time() );
}

float lagcomp::get_lerp_time()
{
	static auto ret = 0.f;
	static auto last_tick = 0;
	if ( interfaces::globals()->tickcount == last_tick )
		return ret;

	var( cl_updaterate );
	var( cl_interp );
	var( cl_interp_ratio );

	const auto update_rate = cl_updaterate->get_int();
	const auto interp_ratio = cl_interp_ratio->get_float();

	auto lerp = interp_ratio / update_rate;

	if ( lerp <= cl_interp->get_float() )
		lerp = cl_interp->get_float();

	ret = roundf( lerp * 1000.0f ) / 1000.0f;

	last_tick = interfaces::globals()->tickcount;

	return ret;
}

void lagcomp::extrapolate( C_CSPlayer* player, Vector& origin, Vector& velocity, Vector base_velocity, int& flags, bool wasonground )
{
	var( sv_gravity );
	var( sv_jump_impulse );

	if ( !( flags & FL_ONGROUND ) )
	{
		velocity.z -= interfaces::globals()->interval_per_tick * sv_gravity->get_float();
		velocity.z += interfaces::globals()->interval_per_tick * base_velocity.z;
	}
	else if ( !wasonground )
		velocity.z = sv_jump_impulse->get_float() - interfaces::globals()->interval_per_tick * sv_gravity->get_float();

	const Vector mins = player->GetCollideable()->OBBMins();
	const Vector max = player->GetCollideable()->OBBMaxs();

	const Vector src = origin;
	Vector end = src + ( velocity * interfaces::globals()->interval_per_tick );

	Ray_t ray;
	ray.Init( src, end, mins, max );

	trace_t trace;
	CTraceFilterNoPlayers filter;

	interfaces::engine_trace()->TraceRay( ray, MASK_PLAYERSOLID, &filter, &trace );

	if ( trace.fraction != 1.f )
	{
		for ( int i = 0; i < 2; i++ )
		{
			velocity -= trace.plane.normal * velocity.Dot( trace.plane.normal );

			const float dot = velocity.Dot( trace.plane.normal );
			if ( dot < 0.f )
			{
				velocity.x -= dot * trace.plane.normal.x;
				velocity.y -= dot * trace.plane.normal.y;
				velocity.z -= dot * trace.plane.normal.z;
			}

			end = trace.endpos + ( velocity * ( interfaces::globals()->interval_per_tick * ( 1.f - trace.fraction ) ) );

			ray.Init( trace.endpos, end, mins, max );
			interfaces::engine_trace()->TraceRay( ray, MASK_PLAYERSOLID, &filter, &trace );

			if ( trace.fraction == 1.f )
				break;
		}
	}

	origin = trace.endpos;
	end = trace.endpos;
	end.z -= 2.f;

	ray.Init( origin, end, mins, max );
	interfaces::engine_trace()->TraceRay( ray, MASK_PLAYERSOLID, &filter, &trace );

	flags &= ~FL_ONGROUND;

	if ( trace.DidHit() && trace.plane.normal.z > 0.7f )
		flags |= FL_ONGROUND;
}

bool lagcomp::valid_simtime( const float& simtime, const float time )
{
	const auto nci = interfaces::engine()->GetNetChannelInfo();
	if ( !nci )
		return false;

	if ( !ConVar::cl_lagcompensation || !ConVar::cl_predict )
		return false;

	const auto last_server_tick = interfaces::client_state()->get_last_server_tick();

	const auto rtt = misc::get_latency();
	const auto possible_future_tick = last_server_tick + time_to_ticks( rtt ) + 8;

	float correct = 0;
	correct += rtt;
	correct += get_lerp_time();

	var( sv_maxunlag );
	const auto deadtime = static_cast< int >( ticks_to_time( last_server_tick ) + rtt - sv_maxunlag->get_float() );
	if ( simtime <= static_cast< float >( deadtime ) || time_to_ticks( simtime + get_lerp_time() ) > possible_future_tick )
		return false;

	correct = clamp( correct, 0.f, sv_maxunlag->get_float() );
	const auto delta_time = correct - ( time - simtime );
	const auto delta_time1 = correct - ( time - interfaces::globals()->interval_per_tick - simtime );
	const auto delta_time2 = correct - ( time + interfaces::globals()->interval_per_tick - simtime );

	if ( vars::legit.enabled->get<bool>() && vars::legit.backtrack->get<float>() != 0.f )
	{
		if ( interfaces::globals()->curtime - simtime > vars::legit.backtrack->get<float>() / 1000.f )
			return false;
	}

	if ( ( tickbase::fast_fire || tickbase::hide_shot ) && tickbase::compute_current_limit() > 2 )
		return fabsf( delta_time1 ) < 0.2f && fabsf( delta_time2 ) < 0.2f;

	return fabsf( delta_time ) < 0.2f;
}

int lagcomp::get_real_lag( const C_CSPlayer* player, const lag_record_t* current )
{
	auto& log = player_log::get_log( player->EntIndex() );
	if ( log.record.size() < 2 )
		return false;

	const auto& server_tick = current->m_server_tick;

	for ( auto i = log.record.size() - 2; i >= 0; i-- )
	{
		const auto record = &log.record[ i ];
		if ( record->m_ignore )
			continue;

		return server_tick - record->m_server_tick;
	}

	return current->m_lagamt;
}

bool lagcomp::is_breaking_lagcomp( const C_CSPlayer* player, const lag_record_t* current )
{
	static constexpr auto teleport_distance_sqr = 64 * 64;

	auto& log = player_log::get_log( player->EntIndex() );
	if ( log.record.size() < 2 )
		return false;

	const auto& previous = log.record[ log.record.size() - 2 ];
	const auto force_breaking_lc = previous.m_ignore && current->m_ignore && ( !( previous.m_flags & FL_ONGROUND ) || !( current->m_flags & FL_ONGROUND ) );

	const auto& origin = current->m_origin;

	for ( auto i = log.record.size() - 2; i >= 0; i-- )
	{
		const auto record = &log.record[ i ];
		if ( record->m_ignore )
			continue;

		const auto delta = record->m_origin - origin;
		if ( delta.Length2DSqr() > teleport_distance_sqr || force_breaking_lc && ( delta.Length2DSqr() > 3000 || record->m_velocity.Length2DSqr() > 250 * 250 || previous.m_velocity.Length2DSqr() > 250 * 250 ) )
			return true;

		break;
	}

	return false;
}

bool lagcomp::is_breaking_lagcomp( const C_CSPlayer* player )
{
	static constexpr auto teleport_distance_sqr = 64 * 64;

	auto& log = player_log::get_log( player->EntIndex() );
	if ( log.record.size() < 2 )
		return false;

	const auto& first = log.record.back();
	const auto force_breaking_lc = first.m_simtime < log.m_highest_simtime || first.m_ignore && !( first.m_flags & FL_ONGROUND );

	Vector origin{};

	auto first_lagcomp = true;
	for ( auto i = log.record.size() - 1; i >= 0; i-- )
	{
		const auto record = &log.record[ i ];
		if ( record->m_ignore )
			continue;

		if ( first_lagcomp )
		{
			origin = record->m_origin;
			first_lagcomp = false;
			continue;
		}

		const auto delta = record->m_origin - origin;
		if ( delta.Length2DSqr() > teleport_distance_sqr || force_breaking_lc && ( delta.Length2DSqr() > 3000 || record->m_velocity.Length2DSqr() > 250 * 250 || first.m_velocity.Length2DSqr() > 250 * 250 ) )
			return true;

		break;
	}

	return false;
}
