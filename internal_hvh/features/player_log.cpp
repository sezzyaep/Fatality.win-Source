#include "../include_cheat.h"

void player_log::run_log( const ClientFrameStage_t stage )
{
	if ( !interfaces::engine()->IsInGame() )
	{
		queued_esp_data.clear();

		for ( auto i = 1; i <= interfaces::globals()->maxClients; i++ )
			logs[ i ].record.reserve( 0 );
	}

	if ( stage != FRAME_NET_UPDATE_END )
		return;

	send_esp_data();

	if ( !local_player )
		return;

	for ( auto i = 1; i <= interfaces::globals()->maxClients; i++ )
	{
		if ( !( logs[ i ].player = globals::get_player( i ) ) )
			logs[ i ].record.reserve( 0 );
	}
}

void player_log::queue_esp_data( C_CSPlayer* player )
{
	if ( !vars::misc.shared_esp->get<bool>() )
		return;

	if ( !vars::misc.shared_esp_with_enemy->get<bool>() && !player->is_enemy() )
		return;

	const auto original_size = queued_esp_data.size();
	auto ent_index = player->EntIndex();
	queued_esp_data.erase( std::ranges::remove_if( queued_esp_data, [&ent_index] ( const std::pair<int, shared_esp_data>& data ) { return data.first == ent_index; } ).begin(), queued_esp_data.end() );

	auto subtracted = false;
	const auto weapon = globals::get_weapon( player->get_active_weapon() );
	auto weapon_id = weapon ? weapon->get_weapon_id() : WEAPON_NONE;
	if ( weapon_id > WEAPON_BUMPMINE )
	{
		subtracted = true;
		weapon_id -= 400;
	}

	weapon_id = clamp( weapon_id, 0, 255 );

	shared_esp_data packet;
	packet.identifier = subtracted ? 0x7FFB : 0x7FFA;
	packet.user_id = player->get_player_info().userid;
	packet.weapon_id = weapon_id;
	packet.pos = player->get_origin();
	packet.server_tick = interfaces::client_state()->get_last_server_tick();

	if ( original_size == queued_esp_data.size() )
		queued_esp_data.emplace_back( player->EntIndex(), packet );
	else
		queued_esp_data.emplace_front( player->EntIndex(), packet );

	voice_usable_fields data;
	memcpy( &data, &packet, sizeof( packet ) );
	shared_data::send_data_msg( data );
}

void player_log::send_esp_data()
{
	static auto last_tick = 0;
	if ( last_tick == interfaces::client_state()->get_last_server_tick() )
		return;

	last_tick = interfaces::client_state()->get_last_server_tick();

	if ( queued_esp_data.empty() )
		return;

	const auto& packet = queued_esp_data.front();

	voice_usable_fields data;
	memcpy( &data, &packet, sizeof( packet ) );
	shared_data::send_data_msg( data );

	queued_esp_data.pop_front();
}

void player_log::parse_esp_data( const int client, const voice_usable_fields& data )
{
	const auto fatality_esp_packet = *reinterpret_cast< const shared_esp_data* >( &data );
	if ( fatality_esp_packet.identifier != 0x7FFA && fatality_esp_packet.identifier != 0x7FFB )
		return;

	const auto player = globals::get_player( interfaces::engine()->GetPlayerForUserID( fatality_esp_packet.user_id ) );
	if ( !player || !player->IsDormant() || !player->is_enemy() )
		return;

	const auto sound_player = sound_esp::get_sound_player( player->EntIndex() );
	if ( sound_player->last_update_tick >= fatality_esp_packet.server_tick )
		return;

	if ( sound_player->origin.Dist( fatality_esp_packet.pos ) > 0.2f )
		sound_player->lerp_start = interfaces::globals()->realtime;

	sound_player->origin = fatality_esp_packet.pos;
	sound_player->weapon_id = fatality_esp_packet.weapon_id + ( fatality_esp_packet.identifier == 0x7FFB ? 400 : 0 );

	if ( sound_player->lerped_origin.IsZero() )
		sound_player->lerped_origin = sound_player->origin;

	if ( sound_player->last_update_tick < interfaces::client_state()->get_last_server_tick() - 3 && vars::visuals.disable_dormancy_interp->get<bool>() )
	{
		sound_player->lerp_start = 0.f;
		sound_player->lerped_origin = sound_player->origin;
	}

	sound_player->updated = true;
	sound_player->last_update_tick = fatality_esp_packet.server_tick;
}

void player_log::run_player( C_CSPlayer* player )
{
	auto& curlog = logs[ player->EntIndex() ];
	curlog.player = player;
	if ( curlog.xuid != player->get_player_info().xuid || player->get_spawn_time() != curlog.m_spawntime )
	{
		if ( curlog.xuid != player->get_player_info().xuid )
			curlog.nospread = {};

		curlog.reset( player );
		curlog.m_spawntime = player->get_spawn_time();
		curlog.xuid = player->get_player_info().xuid;

		const auto sound_player = sound_esp::get_sound_player( player->EntIndex() );
		sound_player->lerped_origin = {};
		sound_player->lerp_start = 0.f;
	}

	if ( local_player && !local_player->get_alive() )
	{
		curlog.m_current_side = resolver_side::resolver_left;
		curlog.m_current_mode = resolver_mode::resolver_default;
	}

	if ( !player->get_alive() || player == local_player )
	{
		if ( !curlog.m_was_reset )
		{
			curlog.reset( player );
			curlog.m_was_reset = true;
		}

		if ( !player->get_alive() && player != local_player && ( player->get_observer_mode() == OBS_MODE_IN_EYE || player->get_observer_mode() == OBS_MODE_CHASE ) && player->get_origin().Length2DSqr() > 1.f )
		{
			if ( const auto observer = player->get_observer() )
			{
				if ( observer->GetClientClass()->m_ClassID == ClassId::CCSPlayer && observer->IsDormant() )
				{
					const auto sound_player = sound_esp::get_sound_player( observer->EntIndex() );

					if ( sound_player->origin.Dist( player->get_origin() ) > 0.2f )
						sound_player->lerp_start = interfaces::globals()->realtime;

					sound_player->origin = player->get_origin();

					if ( sound_player->lerped_origin.IsZero() )
						sound_player->lerped_origin = sound_player->origin;

					sound_player->updated = true;
					sound_player->observerindex = player->EntIndex();

					if ( sound_player->last_update_tick < interfaces::client_state()->get_last_server_tick() - 3 && vars::visuals.disable_dormancy_interp->get<bool>() )
					{
						sound_player->lerp_start = 0.f;
						sound_player->lerped_origin = sound_player->origin;
					}

					sound_player->last_update_tick = interfaces::client_state()->get_last_server_tick();
				}
			}
		}

		curlog.record.reserve( 0 );

		return;
	}

	curlog.m_was_reset = false;

	if ( !player->is_enemy() )
	{
		curlog.record.reserve( 2 );
		curlog.record.clear_all_but_last();
	}
	else
		curlog.record.reserve( time_to_ticks( 0.8f ) );

	if ( !player->get_tickbase() )
		curlog.m_enter_pvs = false;

	if ( curlog.m_enter_pvs )
	{
		player->get_eflags() |= EFL_DIRTY_ABSVELOCITY | EFL_DIRTY_ABSTRANSFORM;
		curlog.m_enter_pvs = false;
		const auto state = player->get_anim_state();
		if ( player->get_tickbase() < interfaces::client_state()->get_last_server_tick() + 3 )
			player->get_simtime() = state->last_update = ticks_to_time( player->get_tickbase() );
		state->velocity = player->get_velocity();
		state->velocity.z = 0.f;
		state->feet_cycle = curlog.server_layers[ 6 ].m_flCycle;
		state->feet_weight = curlog.server_layers[ 6 ].m_flWeight;
		state->strafe_sequence = curlog.server_layers[ 7 ].m_nSequence;
		state->strafe_change_weight = curlog.server_layers[ 7 ].m_flWeight;
		state->strafe_change_cycle = curlog.server_layers[ 7 ].m_flCycle;
		state->on_ground = player->get_flags() & FL_ONGROUND;
		state->on_ladder = player->get_move_type() == MOVETYPE_LADDER;
		if ( !state->on_ground )
			state->air_time = curlog.server_layers[ 4 ].m_flCycle / curlog.server_layers[ 4 ].m_flPlaybackRate;

		if ( curlog.record.size() >= curlog.record.max_size_ )
			curlog.record.pop_front();

		const auto current = curlog.record.emplace_back( player );
		current->m_layers = curlog.server_layers;
		current->m_dormant = current->m_ignore = true;
		return;
	}

	if ( player->IsDormant() )
		return;

	if ( !curlog.record.empty() )
	{
		const auto last_record = &curlog.record.back();
		auto equals = true;

		for ( auto i = 0u; i < last_record->m_layers.size(); i++ )
			if ( !( last_record->m_layers[ i ] == curlog.server_layers[ i ] ) )
			{
				equals = false;
				break;
			}

		if ( equals )
			return;


		curlog.m_last_eye_angle = last_record->m_eye_angles;
	}

	for ( auto i = curlog.record.size() - 1; i >= 0; i-- )
	{
		const auto& record = curlog.record[ i ];
		if ( record.m_simtime < ticks_to_time( interfaces::client_state()->get_last_server_tick() ) - 1.f )
		{
			const auto records_left = i + 1;
			curlog.record.current_ += records_left;
			curlog.record.size_ -= records_left;
			break;
		}
	}

	if ( curlog.last_recv_data != interfaces::client_state()->get_last_server_tick() )
	{
		queue_esp_data( player );
		curlog.last_recv_data = interfaces::client_state()->get_last_server_tick();
	}

	curlog.m_dormant_misses = 0;
	curlog.extrapolated_record.m_valid = false;
	curlog.last_server_tick = interfaces::client_state()->get_last_server_tick();

	if ( curlog.record.size() >= curlog.record.max_size_ )
		curlog.record.pop_front();

	const auto current = curlog.record.emplace_back( player );

	current->m_layers = curlog.server_layers;

	if ( vars::legit_enabled() )
		return;

	const auto previous = curlog.record.size() > 1 ? &curlog.record[ curlog.record.size() - 2 ] : nullptr;

	// count jittering pitch for dumb reasons.
	if ( curlog.pitch_prev == 0x42b1fa80 && *reinterpret_cast< uint32_t* >( &player->get_eye_angles().x ) == 0x42b1fd50 )
		curlog.pitch_alt = std::max( curlog.pitch_alt + 1, 4 );
	else if ( curlog.pitch_prev != 0x42b1fd50 || *reinterpret_cast< uint32_t* >( &player->get_eye_angles().x ) != 0x42b1fa80 )
		curlog.pitch_alt = 0;
	curlog.pitch_prev = *reinterpret_cast< uint32_t* >( &player->get_eye_angles().x );
	if ( curlog.pitch_alt >= 4 )
		current->do_not_set = true;

	animations::calculate_sim_ticks( current, player, previous );

	if ( current->m_lagamt > 64 || current->m_lagamt <= 0 )
	{
		curlog.record.pop_back();
		return;
	}

	animations::update_player_animations( current, player, previous );

	// update bounds
	if ( curlog.m_maxs_z != FLT_MAX )
	{
		curlog.m_view_delta = curlog.m_maxs_z;
		player->get_collision_viewheight() = player->get_origin().z + curlog.m_maxs_z;
		player->get_collision_bounds_change_time() = player->get_simtime() - ceilf( current->m_lagamt / 2.f ) * interfaces::globals()->interval_per_tick;
		curlog.m_unpred_change_time = globals::interpolated_curtime;
		curlog.m_maxs_z = FLT_MAX;
	}

	player->get_velocity() = current->m_calculated_velocity;
	player->get_eflags() |= EFL_DIRTY_ABSVELOCITY | EFL_DIRTY_ABSTRANSFORM;

	player->get_blend_tick() = 0;

	if ( curlog.m_highest_simtime > current->m_simtime + ticks_to_time( 16 ) )
		curlog.m_highest_simtime = 0.f;

	if ( current->m_simtime <= curlog.m_highest_simtime )
		current->m_ignore = true;

	current->m_breaking_lc = lagcomp::is_breaking_lagcomp( player, current );

	current->m_real_lag = lagcomp::get_real_lag( player, current );

	curlog.m_highest_simtime = std::max( curlog.m_highest_simtime, current->m_simtime );

#ifdef _DEBUG
	if ( !vars::aim.anti_exploit->get<bool>() )
	{
		current->setup_matrices();
		anim_debug_info[ player->EntIndex() ].get_or_put( time_to_ticks( current->m_simtime ) ).emplace_client( current->matrix( curlog.get_dir( current ) ) );
	}
#endif
}

player_log_t& player_log::get_log( const int index )
{
	return logs[ index ];
}

void player_log::filter_records()
{
	for ( auto i = 1; i <= interfaces::globals()->maxClients; i++ )
	{
		auto& curlog = logs[ i ];
		auto& record = curlog.record;
		if ( record.empty() )
		{
			continue;
		}

		if ( !curlog.player )
		{
			record.clear();
			continue;
		}

		for ( auto j = record.size() - 1; j >= 0; j-- )
		{
			auto& currecord = record[ j ];
			if ( currecord.m_ignore )
			{
				currecord.m_valid = false;
				continue;
			}

			if ( currecord.m_lagamt >= 18 )
			{
				currecord.m_valid = false;
				continue;
			}

			currecord.m_valid = lagcomp::valid_simtime( currecord.m_simtime );
		}
	}
}