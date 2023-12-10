#include "../include_cheat.h"

Vector legit::closest_hitbox_pos( const player_log_t& log, lag_record_t* record, float& bestdist )
{
	mstudiobbox_t* bestbox = nullptr;

	static constexpr std::array hitboxes =
	{
		HITBOX_LEFT_CALF,
		HITBOX_RIGHT_CALF,
		HITBOX_CHEST,
		HITBOX_PELVIS,
		HITBOX_HEAD
	};

	var( weapon_recoil_scale );

	const auto model = log.player->get_model_ptr();
	if ( !model )
		return Vector( 0.f, 0.f, 0.f );

	const auto studio_model = model->m_pStudioHdr;
	if ( !studio_model )
		return Vector( 0.f, 0.f, 0.f );

	const auto hitbox_set = studio_model->pHitboxSet( log.player->get_hitbox_set() );
	if ( !hitbox_set )
		return Vector( 0.f, 0.f, 0.f );

	prediction::run_prediction( globals::current_cmd->command_number );

	QAngle angles{};
	interfaces::engine()->GetViewAngles( angles );

	for ( auto& hitbox_index : hitboxes )
	{
		const auto box = hitbox_set->pHitbox( hitbox_index );
		if ( !box )
			continue;

		const auto tempdist = math::get_fov( angles + local_player->get_aim_punch() * weapon_recoil_scale->get_float(), math::calc_angle( local_player->get_eye_pos(), Vector( record->matrix( record->m_setup_dir )[ box->bone ][ 0 ][ 3 ], record->matrix( record->m_setup_dir )[ box->bone ][ 1 ][ 3 ], record->matrix( record->m_setup_dir )[ box->bone ][ 2 ][ 3 ] ) ) );
		if ( tempdist < bestdist )
		{
			bestdist = tempdist;
			bestbox = box;
		}
	}

	if ( !bestbox )
		return Vector( 0.f, 0.f, 0.f );

	const auto center = ( ( bestbox->bbmin + bestbox->bbmax ) * 0.5f );
	Vector tmp{};
	math::vector_transform( center, record->matrix( record->m_setup_dir )[ bestbox->bone ], tmp );

	return tmp;
}

void legit::run_triggerbot()
{
	if ( !vars::legit.enabled->get<bool>() || !prediction::can_fire() || prediction::has_shot() )
	{
		trigger_time = 0.f;
		return;
	}

	const auto weapon = globals::get_weapon( local_player->get_active_weapon() );
	if ( !weapon )
	{
		trigger_time = 0.f;
		return;
	}

	const auto config = config::current_legit_config( weapon );
	if ( !config || !config->triggerbot->get<bool>() )
	{
		trigger_time = 0.f;
		return;
	}

	if ( config->triggerbot_only_in_scope->get<bool>() && config->index == 2 && !local_player->get_scoped() )
	{
		trigger_time = 0.f;
		return;
	}

	if ( trigger_time > 0.f )
	{
		if ( trigger_time < config->reaction_time->get<float>() * 0.001f )
			trigger_time += interfaces::globals()->interval_per_tick;
		else
		{
			trigger_time = 0.f;
			prediction::take_shot();
		}

		return;
	}

	QAngle angles{};
	interfaces::engine()->GetViewAngles( angles );

	Vector dir;
	math::angle_vectors( angles, &dir );
	math::fast_vec_normalize( dir );

	const auto startpos = local_player->get_eye_pos();
	const auto endpos = startpos + dir * weapon->get_wpn_data()->range;

	constexpr auto ray_extension = 40.f;

	trace_t enter_trace;
	globals::util_trace_line( startpos, endpos, MASK_SHOT_HULL | CONTENTS_HITBOX, local_player, &enter_trace );
	if ( enter_trace.m_pEnt && enter_trace.m_pEnt->is_player() && reinterpret_cast< C_CSPlayer* >( enter_trace.m_pEnt )->is_enemy() )
		globals::util_clip_trace_to_player( startpos, endpos + dir * ray_extension, MASK_SHOT_HULL | CONTENTS_HITBOX, enter_trace.m_pEnt, &enter_trace );

	if ( enter_trace.hitgroup == HITGROUP_GEAR || !enter_trace.m_pEnt || !enter_trace.m_pEnt->is_player() || !reinterpret_cast< C_CSPlayer* >( enter_trace.m_pEnt )->is_enemy() || vars::legit.disable_flash->get<bool>() && local_player->get_flash_amount() > 115.f || vars::legit.disable_smoke->get<bool>() && globals::line_goes_through_smoke( startpos, enter_trace.endpos ) )
		trigger_time = 0.f;
	else
	{
		if ( config->reaction_time->get<float>() == 0.f )
			prediction::take_shot();
		else
			trigger_time = interfaces::globals()->interval_per_tick;
	}

}

void legit::run_backtrack()
{
	shot_record = nullptr;
	shot_index = -1;

	if ( vars::legit.backtrack->get<float>() == 0.f )
		return;

	if ( aimbot::has_shot || !prediction::has_shot() )
		return;

	auto bestdist = FLT_MAX;
	lag_record_t* bestrecord = nullptr;
	C_CSPlayer* bestplayer = nullptr;
	player_log::filter_records();

	for ( const auto player : interfaces::entity_list()->get_players() )
	{
		if ( !player->get_alive() || player->IsDormant() || !player->is_enemy() || player == local_player || player->get_immunity() )
			continue;

		auto playerbestdist = FLT_MAX;
		lag_record_t* playerbestrecord = nullptr;

		auto& log = player_log::get_log( player->EntIndex() );
		if ( !log.player )
			continue;

		for ( auto i = log.record.size() - 1; i >= 0; i-- )
		{
			auto& current = log.record[ i ];
			if ( !current.m_valid || !current.m_has_visual_matrix )
				continue;

			auto tempdist = FLT_MAX;
			memcpy( current.matrix( current.m_setup_dir ), current.m_visual_matrix, sizeof( current.m_visual_matrix ) );
			auto pos = current.m_abs_origin;
			animations::move_matrix( current.matrix( current.m_setup_dir ), pos, current.m_origin );
			closest_hitbox_pos( log, &current, tempdist );

			if ( tempdist < playerbestdist )
			{
				playerbestdist = tempdist;
				playerbestrecord = &current;
			}
			else
			{
				break;
			}
		}

		if ( playerbestrecord && playerbestdist < bestdist )
		{
			bestdist = playerbestdist;
			bestrecord = playerbestrecord;
			bestplayer = player;
		}
	}

	if ( !bestplayer || bestdist > 30.f )
		return;

	bestrecord->setup_matrices( bestrecord->m_setup_dir );

	//aimedrecord = bestrecord;
	globals::current_cmd->tick_count = lagcomp::fix_tickcount( bestrecord->m_simtime );
	shot_record = bestrecord;
	shot_index = bestrecord->m_index;
}

bool visible( Vector start, Vector end, C_CSPlayer* filter, C_CSPlayer* ent )
{
	if ( vars::legit.disable_flash->get<bool>() && local_player->get_flash_amount() > 115.f )
		return false;

	trace_t trace;
	globals::util_trace_line( start, end, MASK_SHOT_HULL, filter, &trace );
	const auto ret = trace.m_pEnt == ent;
	if ( ret && vars::legit.disable_smoke->get<bool>() && globals::line_goes_through_smoke( start, end ) )
		return false;

	return ret;
}

void legit::run_aim( float* mouse_x, float* mouse_y )
{
	if ( !vars::legit.enabled->get<bool>() || !local_player || !local_player->get_alive() )
	{
		last_punch = {};
		return;
	}

	const auto weapon = globals::get_weapon( local_player->get_active_weapon() );
	if ( !weapon || weapon->in_reload() )
	{
		last_punch = {};
		return;
	}

	run_rcs( mouse_x, mouse_y, weapon );

	const auto config = config::current_legit_config( weapon );
	if ( !config || !config->magnet->get<bool>() )
		return;

	if ( config->magnet_only_in_scope->get<bool>() && config->index == 2 && !local_player->get_scoped() )
		return;

	const auto get_target = [&] () -> legit_target_t
	{
		const auto current_pos = local_player->get_eye_pos();
		QAngle current_angle;
		interfaces::engine()->GetViewAngles( current_angle );

		legit_target_t best_match;
		best_match.index = 0;
		best_match.fov = FLT_MAX;
		for ( const auto player : interfaces::entity_list()->get_players() )
		{
			const auto i = player->EntIndex();
			if ( !player->get_alive() || player->IsDormant() || !player->is_enemy() || player == local_player || player->get_immunity() )
			{
				visible_time[ i ] = 0;
				continue;
			}

			auto& log = player_log::get_log( i );
			if ( !log.player )
			{
				visible_time[ i ] = 0;
				continue;
			}

			if ( log.record.empty() )
			{
				visible_time[ i ] = 0;
				continue;
			}

			auto is_visible = false;
			for ( auto j = HITBOX_HEAD; j <= HITBOX_UPPER_CHEST; j++ )
			{
				if ( j == HITBOX_HEAD && config->magnet_hitboxes.head->get<bool>()
				|| j == HITBOX_UPPER_CHEST && config->magnet_hitboxes.chest->get<bool>()
				|| j == HITBOX_BODY && config->magnet_hitboxes.body->get<bool>()
				|| j == HITBOX_PELVIS && config->magnet_hitboxes.pelvis->get<bool>() )
				{
					const auto target_pos = player->get_hitbox_pos( j );
					if ( !visible( current_pos, target_pos, local_player, player ) )
						continue;

					is_visible = true;

					if ( visible_time[ i ] < config->target_delay->get<float>() * 0.001f )
						continue;

					const auto fov = math::get_fov( current_angle, current_pos, target_pos );
					if ( fov > config->magnet_fov->get<float>() * 0.5f )
						continue;

					if ( fov < best_match.fov )
					{
						best_match.index = i;
						best_match.fov = fov;
						best_match.hitbox = j;
					}
				}
			}
			if ( is_visible )
			{
				visible_time[ i ] += interfaces::globals()->curtime - target_time;
				visible_time[ i ] = clamp( visible_time[ i ], 0.f, 1.f );
			}
			else
				visible_time[ i ] = 0;

		}

		return best_match;
	};

	static auto prev_tickcount = 0;
	if ( interfaces::globals()->tickcount != prev_tickcount )
	{
		target = get_target();
		prev_tickcount = interfaces::globals()->tickcount;
		target_time = interfaces::globals()->curtime;
	}

	if ( !target.index )
		return;

	const auto player = globals::get_player( target.index );
	if ( !player )
		return;

	QAngle current_angle;
	interfaces::engine()->GetViewAngles( current_angle );

	const auto current_pos = local_player->get_eye_pos();
	const auto target_pos = player->get_hitbox_pos( target.hitbox );

	if ( target_pos.IsZero() )
		return;

	const auto fov = math::get_fov( current_angle, current_pos, target_pos );
	if ( fov > config->magnet_fov->get<float>() * 0.5f )
		return;

	const auto new_fov = math::get_fov( current_angle + pixels_to_angle( { *mouse_x, *mouse_y } ), current_pos, target_pos );
	if ( new_fov < 0.05f )
		return;

	const auto dist_to_fov = config->magnet_fov->get<float>() * 0.5f - fov;

	var( weapon_recoil_scale );
	auto aim_angle = math::calc_angle( current_pos, target_pos );
	aim_angle -= local_player->get_aim_punch() * weapon_recoil_scale->get_float();

	smooth( aim_angle, current_angle, config->magnet_speed->get<float>() * clamp( dist_to_fov * 0.5f, 0.f, 1.f ) );

	const auto move_angle = pixels_to_angle( { *mouse_x, *mouse_y } );
	auto view_delta = aim_angle - current_angle;
	view_delta.Clamp();

	if ( !view_delta.IsValid() )
		return;

	QAngle delta;

	const auto delta_x = std::abs( move_angle.x ) * ( config->limit_factor->get<float>() * 0.01f ) + ( 1.f - config->limit_factor->get<float>() * 0.01f ) * std::abs( view_delta.x );
	const auto delta_y = std::abs( move_angle.y ) * ( config->limit_factor->get<float>() * 0.01f ) + ( 1.f - config->limit_factor->get<float>() * 0.01f ) * std::abs( view_delta.y );

	delta.x = clamp( view_delta.x, -delta_x, delta_x );
	delta.y = clamp( view_delta.y, -delta_y, delta_y );

	const auto pixels = angle_to_pixels( delta );

	*mouse_x = pixels.x + ( 1.f - config->override_factor->get<float>() * 0.01f ) * *mouse_x;
	*mouse_y = pixels.y + ( 1.f - config->override_factor->get<float>() * 0.01f ) * *mouse_y;
}

void legit::run_rcs( float* mouse_x, float* mouse_y, C_WeaponCSBaseGun* weapon )
{
	const auto config = config::current_legit_config( weapon );
	if ( !config || !config->rcs->get<bool>() || weapon->get_weapon_id() == WEAPON_AWP || weapon->get_weapon_id() == WEAPON_SSG08 || weapon->get_weapon_id() == WEAPON_REVOLVER )
	{
		last_punch = {};
		return;
	}

	var( weapon_recoil_scale );

	const auto new_punch = local_player->get_aim_punch() * weapon_recoil_scale->get_float();

	auto view_delta = new_punch - last_punch;

	last_punch = new_punch;

	view_delta *= config->rcs_speed->get<float>() * 0.01f;
	view_delta.Clamp();

	const auto pixels = angle_to_pixels( view_delta );

	*mouse_x -= pixels.x;
	*mouse_y -= pixels.y;
}


void legit::smooth( QAngle& dest_ang, const QAngle& cur_ang, const float speed )
{
	auto delta = dest_ang - cur_ang;
	delta.Clamp();

	if ( delta.Length() == 0.f )
		return;

	const auto delta_norm = delta.Length() > 1.f ? delta.Normalize() * 1.f : delta;

	dest_ang = cur_ang + delta_norm * interfaces::globals()->frametime * speed;
	dest_ang.Clamp();
}

Vector2D legit::angle_to_pixels( const QAngle& angle )
{
	var( m_yaw );
	var( m_pitch );
	return Vector2D( -angle.y / m_yaw->get_float(), angle.x / m_pitch->get_float() );
}

QAngle legit::pixels_to_angle( const Vector2D& pixel )
{
	var( m_yaw );
	var( m_pitch );

	auto result = QAngle( -pixel.y * m_yaw->get_float(), pixel.x * m_pitch->get_float(), 0.f );
	result.Clamp();
	return result;
}