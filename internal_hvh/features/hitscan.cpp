#include <random>
#include <stack>

#include "../include_cheat.h"
#include "mt.h"

aimbot::aimpoint_t hitscan::hitscan_record( aimbot::target_data_t* target, lag_record_t& record, int safety, lag_record_t* extra, Vector eyepos )
{
	std::array<circular_buffer<aimbot::aimpoint_t, 78>, HITBOX_MAX> hitbox_points = {};
	std::vector<std::shared_ptr<detail::call_queue::queue_element>> hitbox_traces = {};
	aimbot::aimpoint_t aimpoint{};

	const auto player = globals::get_player( record.m_index );
	auto log = &player_log::get_log( record.m_index );
	const auto current_state = log->get_dir( &record );
	const auto weapon = local_weapon;

	const auto model = player->get_model();
	if ( !model )
		return aimpoint;

	const auto studio_hdr = interfaces::model_info()->GetStudioModel( model );
	if ( !studio_hdr )
		return aimpoint;

	const auto config = config::current_config( weapon );
	const auto is_zeus = weapon->get_weapon_id() == WEAPON_TASER;
	const auto is_knife = !is_zeus && weapon->get_weapon_type() == WEAPONTYPE_KNIFE;
	const auto moving = record.m_velocity.Length() > 4.f;
	const auto on_ground = record.m_flags & FL_ONGROUND;
	const auto weapon_data = weapon->get_wpn_data();

	const auto spread_rad = aimbot_helpers::get_lowest_inaccuracy( weapon );
	const auto corner_rad = DEG2RAD( 90.f - RAD2DEG( spread_rad ) );
	const auto dist = record.m_origin.Dist( local_player->get_origin() );
	const auto spread_radius = ( dist / sinf( corner_rad ) ) * sinf( spread_rad );

	auto baim = vars::aim.baim_on_key->get<bool>();
	if ( config->baim.air->get<bool>() && !( record.m_flags & FL_ONGROUND ) && player->get_move_type() != MOVETYPE_LADDER
		|| config->baim.moving->get<bool>() && record.m_velocity.Length2D() > 4.f
		|| config->baim.standing->get<bool>() && record.m_velocity.Length2D() <= 4.f )
		baim = true;

	if ( vars::aim.headshot_only() )
		baim = false;

	for ( auto i = 0u; i < HITBOX_MAX; i++ )
	{
		if ( !is_knife && !is_zeus && !enabled_hitboxes[ i ] )
			continue;

		if ( !is_knife && !is_zeus && baim && ( i == HITBOX_HEAD || i == HITBOX_LEFT_FOOT || i == HITBOX_RIGHT_FOOT ) )
			continue;

		if ( !is_knife && ( i == HITBOX_RIGHT_FOOT ||
			i == HITBOX_LEFT_FOOT ||
			i == HITBOX_LEFT_UPPER_ARM ||
			i == HITBOX_RIGHT_UPPER_ARM ||
			i == HITBOX_RIGHT_CALF ||
			i == HITBOX_LEFT_CALF ) &&
			( moving && config->ignore_limbs->get<bool>() || log->m_shots > 4 && config->fallback_mode->get<int>() != 0 || !on_ground && config->force_safe.air->get<bool>() ) )
			continue;

		if ( is_knife && i != HITBOX_UPPER_CHEST && i != HITBOX_PELVIS && ( i != HITBOX_LEFT_FOOT && i != HITBOX_RIGHT_FOOT || !moving ) )
			continue;

		if ( is_zeus && i != HITBOX_UPPER_CHEST && i != HITBOX_PELVIS && i != HITBOX_BODY )
			continue;

		const auto calc_hitbox = [&, i] () -> void
		{
			multipoint_hitbox( hitbox_points[ i ], i, studio_hdr, record, eyepos, spread_radius, is_knife, is_zeus, config, is_zeus ? penetration::safety_full : safety, extra, weapon_data );
			if ( hitbox_points[ i ].empty() )
				return;

			for ( auto j = 0; j < hitbox_points[ i ].size(); j++ )
			{
				auto& point = hitbox_points[ i ][ j ];

				point.hitchance = 0.f;

				can_hit( local_player, penetration::pen_data( &record, current_state, player->get_player_info().fakeplayer ? 0 : point.safety, extra ), eyepos, &point, point.damage );
			}
		};

		//calc_hitbox();
		hitbox_traces.emplace_back( std::make_shared<detail::call_queue::queue_element>( calc_hitbox ) );
	}

	detail::callqueue.perform( hitbox_traces );

	auto has_safepoint = false;
	//auto has_hitchance = false;
	const auto limb_safety = config::current_config( weapon )->extra_safety.limbs->get<bool>() ? penetration::safety_full : config::current_config( weapon )->force_safe.limbs->get<bool>() ? penetration::safety_no_roll : penetration::safety_none;
	auto best_shots_to_kill = 100;

	auto lethal_safety = penetration::safety_none;
	auto lethal_baim = false;
	auto highest_damage = 0;

	const auto minimum_dmg = static_cast< int >( config->mindmg_override_enabled->get< bool >() ? config->mindmg_override->get< float >() : config->mindmg->get< float >() );
	const auto additional_damage = std::max( 0, minimum_dmg - 100 );
	for ( const auto& trace : hitbox_points )
		for ( auto i = 0; i < trace.size(); i++ )
		{
			const auto& trace_aimpoint = trace[ i ];

			if ( vars::aim.headshot_only() )
				continue;

			if ( trace_aimpoint.hitbox != HITBOX_BODY && trace_aimpoint.hitbox != HITBOX_PELVIS && trace_aimpoint.hitbox != HITBOX_UPPER_CHEST )
				continue;

			if ( trace_aimpoint.no_pen_damage < player->get_predicted_health() + additional_damage )
				continue;

			lethal_baim = config::current_config( weapon )->baim.lethal->get<bool>() && !vars::aim.headshot_only();
			lethal_safety = config::current_config( weapon )->extra_safety.lethal->get<bool>() ? penetration::safety_full : config::current_config( weapon )->force_safe.lethal->get<bool>() ? penetration::safety_no_roll : penetration::safety_none;
		}

	if ( !has_safepoint && safety == penetration::safety_full )
		safety = penetration::safety_no_roll;

	if ( !has_safepoint && lethal_safety == penetration::safety_full )
		lethal_safety = penetration::safety_no_roll;


	for ( const auto& trace : hitbox_points )
		for ( auto i = 0; i < trace.size(); i++ )
		{
			const auto& trace_aimpoint = trace[ i ];

			if ( trace_aimpoint.safety < lethal_safety )
				continue;

			if ( lethal_baim && trace_aimpoint.hitbox != HITBOX_BODY && trace_aimpoint.hitbox != HITBOX_PELVIS && trace_aimpoint.hitbox != HITBOX_UPPER_CHEST )
				continue;

			if ( trace_aimpoint.safety == penetration::safety_full && trace_aimpoint.no_pen_damage >= 1 )
				has_safepoint = true;

			if ( trace_aimpoint.damage > highest_damage )
				highest_damage = trace_aimpoint.damage;

			if ( trace_aimpoint.hitgroup == HITGROUP_STOMACH && trace_aimpoint.no_pen_damage > highest_damage )
				highest_damage = trace_aimpoint.no_pen_damage;
		}

#if defined(ALPHA) || !defined(RELEASE)
	if ( !visuals::players[ record.m_index ].debug.target_sp && lethal_safety > 0 )
		visuals::players[ record.m_index ].debug.target_sp = lethal_safety > 0;
	if ( !visuals::players[ record.m_index ].debug.target_vsp && lethal_safety == penetration::safety_full )
		visuals::players[ record.m_index ].debug.target_vsp = true;

	visuals::players[ record.m_index ].debug.reason = XOR_STR( "NO HITPOINT" );
	visuals::players[ record.m_index ].debug.dmg = 0;
#endif

	std::string extra_info = xorstr_( "\navailable:\n" );

	for ( auto& trace : hitbox_points )
	{
		for ( auto i = 0; i < trace.size(); i++ )
		{
			auto& trace_aimpoint = trace[ i ];

			trace_aimpoint.player_health = player->get_predicted_health();

			if ( vars::aim.headshot_only() && trace_aimpoint.hitbox != HITBOX_HEAD )
				continue;

			if ( safety == penetration::safety_full && trace_aimpoint.safety < penetration::safety_full )
				continue;

			if ( safety == penetration::safety_no_roll && trace_aimpoint.safety < penetration::safety_no_roll )
				continue;

			if ( trace_aimpoint.safety < lethal_safety )
				continue;

			if ( lethal_safety == penetration::safety_full && trace_aimpoint.hitbox != HITBOX_BODY && trace_aimpoint.hitbox != HITBOX_PELVIS && trace_aimpoint.hitbox != HITBOX_UPPER_CHEST )
				continue;

			if ( lethal_baim && trace_aimpoint.hitbox != HITBOX_BODY && trace_aimpoint.hitbox != HITBOX_PELVIS && trace_aimpoint.hitbox != HITBOX_UPPER_CHEST )
				continue;

			if ( is_zeus && ( trace_aimpoint.safety < penetration::safety_full && has_safepoint || trace_aimpoint.safety < penetration::safety_no_roll ) )
				continue;

			const auto is_limb = trace_aimpoint.hitbox == HITBOX_RIGHT_FOOT ||
				trace_aimpoint.hitbox == HITBOX_LEFT_FOOT ||
				trace_aimpoint.hitbox == HITBOX_LEFT_UPPER_ARM ||
				trace_aimpoint.hitbox == HITBOX_RIGHT_UPPER_ARM ||
				trace_aimpoint.hitbox == HITBOX_RIGHT_CALF ||
				trace_aimpoint.hitbox == HITBOX_LEFT_CALF;

			if ( is_limb && ( trace_aimpoint.safety < limb_safety || limb_safety != penetration::safety_none && ( limb_safety == penetration::safety_full ? trace_aimpoint.safety_size < 5.f : 0 ) ) )
				continue;

			const auto mindmg = config::current_config( weapon )->get_mindmg( player, highest_damage );

			auto hitbox = trace_aimpoint.hitbox;
			if ( trace_aimpoint.hitbox == HITBOX_PELVIS )
				hitbox = HITBOX_BODY;

			if ( trace_aimpoint.hitbox == HITBOX_UPPER_CHEST )
				hitbox = HITBOX_CHEST;

			if ( trace_aimpoint.no_pen_damage >= mindmg && !is_knife && !is_zeus && !target->hitboxes[ hitbox ] && trace_aimpoint.safety != penetration::safety_none )
				target->hitboxes[ hitbox ] = true;

			if ( !is_knife && trace_aimpoint.damage < 1 )
				continue;

#if defined(ALPHA) || !defined(RELEASE)
			visuals::players[ record.m_index ].debug.reason = XOR_STR( "NO DAMAGE" );
			if ( trace_aimpoint.damage > visuals::players[ record.m_index ].debug.dmg )
			{
				visuals::players[ record.m_index ].debug.dmg = trace_aimpoint.damage;
			}
#endif

			if ( is_zeus && trace_aimpoint.damage < 100 && trace_aimpoint.damage < player->get_predicted_health() )
				continue;

			if ( is_knife )
			{
				if ( trace_aimpoint.damage > aimpoint.damage )
					aimpoint = trace_aimpoint;

				continue;
			}

#if defined(ALPHA) || !defined(RELEASE)
			visuals::players[ record.m_index ].debug.target_dmg = mindmg;
			visuals::players[ record.m_index ].debug.dmg = trace_aimpoint.damage;
#endif
			if ( trace_aimpoint.damage < mindmg )
				continue;

			const auto overwrite_point = [&] ()
			{
				extra_info += "[ " + std::to_string( trace_aimpoint.hitbox ) + " " + std::to_string( trace_aimpoint.damage ) + " " + std::to_string( trace_aimpoint.safety ) + " " + std::to_string( trace_aimpoint.safety_size ) + " " + std::to_string( trace_aimpoint.center_dist ) + " ]\n";

				if ( aimpoint.damage < 1 && trace_aimpoint.damage >= 1 )
				{
					const auto is_singleshot = weapon->get_weapon_id() == WEAPON_SSG08 || weapon->get_weapon_id() == WEAPON_AWP || weapon->get_weapon_id() == WEAPON_REVOLVER || misc::retract_peek;
					const auto doubletap_ready = tickbase::fast_fire && !is_singleshot && tickbase::compute_current_limit() > 2;

					const auto health = player->get_predicted_health() + ( trace_aimpoint.penetration_points.size() > 1 ? 5 : 0 );
					const auto shots_to_kill = static_cast< int >( ceilf( health / fminf( floorf( trace_aimpoint.damage * ( doubletap_ready ? 2.f : 1.f ) ), health ) ) );

					best_shots_to_kill = shots_to_kill;
					aimpoint = trace_aimpoint;
					return false;
				}

				// prefer safe point
				if ( aimpoint.safety >= penetration::safety_no_roll && trace_aimpoint.safety < penetration::safety_no_roll && ( config::current_config( weapon )->fallback_mode->get<int>() == 2 || config::current_config( weapon )->fallback_mode->get<int>() == 1 && log->m_shots ) )
					return false;

				const auto is_singleshot = weapon->get_weapon_id() == WEAPON_SSG08 || weapon->get_weapon_id() == WEAPON_AWP || weapon->get_weapon_id() == WEAPON_REVOLVER || misc::retract_peek;
				const auto doubletap_ready = tickbase::fast_fire && !is_singleshot && tickbase::compute_current_limit() > 2;

				const auto health = player->get_predicted_health() + ( trace_aimpoint.penetration_points.size() > 1 ? 5 : 0 );
				const auto health_aimpoint = player->get_predicted_health() + ( aimpoint.penetration_points.size() > 1 ? 5 : 0 );

				const auto shots_to_kill = static_cast< int >( ceilf( health / fminf( floorf( trace_aimpoint.damage * ( doubletap_ready ? 2.f : 1.f ) ), health ) ) );
				const auto shots_to_kill_aimpoint = static_cast< int >( ceilf( health_aimpoint / fminf( floorf( aimpoint.damage * ( doubletap_ready ? 2.f : 1.f ) ), health_aimpoint ) ) );

				if ( trace_aimpoint.damage + 5 < aimpoint.damage && shots_to_kill_aimpoint > 2 )
					return false;

				if ( shots_to_kill >= best_shots_to_kill * 2 )
					return false;

				/*if ( aimpoint.safety >= penetration::safety_no_roll && trace_aimpoint.safety >= penetration::safety_no_roll )
				{
					const auto shots_to_kill = static_cast< int >( ceilf( health / fminf( floorf( trace_aimpoint.safe_point_damage * ( doubletap_ready ? 2.f : 1.f ) ), health ) ) );
					const auto shots_to_kill_aimpoint = static_cast< int >( ceilf( health_aimpoint / fminf( floorf( aimpoint.safe_point_damage * ( doubletap_ready ? 2.f : 1.f ) ), health_aimpoint ) ) );

					if ( shots_to_kill >= shots_to_kill_aimpoint * 2 )
						return false;
				}*/

				// has lethal safe point
				if ( aimpoint.safety >= penetration::safety_no_roll && shots_to_kill_aimpoint == 1 && trace_aimpoint.safety < penetration::safety_no_roll )
					return false;

				if ( aimpoint.safety >= penetration::safety_no_roll && trace_aimpoint.safety < penetration::safety_no_roll && shots_to_kill_aimpoint == shots_to_kill )
					return false;

				if ( trace_aimpoint.safety_size < aimpoint.safety_size && shots_to_kill_aimpoint <= shots_to_kill && trace_aimpoint.safety == aimpoint.safety )
					return false;

				if ( shots_to_kill_aimpoint <= shots_to_kill && trace_aimpoint.safety < aimpoint.safety )
					return false;

				const auto hitbox = player->get_model_ptr()->m_pStudioHdr->pHitbox( trace_aimpoint.hitbox, 0 );
				const auto hitbox_aimpoint = player->get_model_ptr()->m_pStudioHdr->pHitbox( aimpoint.hitbox, 0 );
				if ( shots_to_kill == shots_to_kill_aimpoint && hitbox->radius != -1.f && hitbox_aimpoint->radius == -1.f )
				{
					best_shots_to_kill = shots_to_kill;
					aimpoint = trace_aimpoint;
					return false;
				}

				if ( fabsf( trace_aimpoint.safety_size - aimpoint.safety_size ) <= 5.f && shots_to_kill_aimpoint <= shots_to_kill )
				{
					const auto volume = std::min( 1600.f, M_PI * powf( hitbox->radius, 2 ) * ( 4.f / 3.f * hitbox->radius + ( hitbox->bbmax - hitbox->bbmin ).Length() ) );
					const auto volume_aimpoint = std::min( 1600.f, M_PI * powf( hitbox_aimpoint->radius, 2 ) * ( 4.f / 3.f * hitbox_aimpoint->radius + ( hitbox_aimpoint->bbmax - hitbox_aimpoint->bbmin ).Length() ) );

					if ( volume < volume_aimpoint - 2.f )
						return false;

					if ( fabsf( volume - volume_aimpoint ) < 2.f && trace_aimpoint.hitbox > aimpoint.hitbox )
						return false;

					if ( trace_aimpoint.center_dist > aimpoint.center_dist + 0.3f && fabsf( volume - volume_aimpoint ) < 2.f )
						return false;
				}

				if ( shots_to_kill < best_shots_to_kill )
					best_shots_to_kill = shots_to_kill;

				aimpoint = trace_aimpoint;
				return false;
			};

			if ( overwrite_point() )
				break;
		}
	}

	if ( has_safepoint )
		aimpoint.has_safe_point = true;

	extra_info += xorstr_( "chosen: \n" );
	extra_info += "[ " + std::to_string( aimpoint.hitbox ) + " " + std::to_string( aimpoint.damage ) + " " + std::to_string( aimpoint.safety ) + " " + std::to_string( aimpoint.safety_size ) + " " + std::to_string( aimpoint.center_dist ) + " ]\n";
	aimpoint.extra_info = extra_info;
	aimpoint.highest_damage = highest_damage;

	return aimpoint;
}

bool hitscan::get_best_hitpoint( aimbot::target_data_t* target, lag_record_t& record, aimbot::aimpoint_t& ret, lag_record_t* extra, Vector eyepos )
{
	const auto player = globals::get_player( record.m_index );
	const auto log = &player_log::get_log( record.m_index );
	const auto bot = player->get_player_info().fakeplayer;
	const auto config = config::current_config( local_weapon );

	auto safety = penetration::safety_none;

	if ( !bot && ( config->force_safe.air->get<bool>() && !( record.m_flags & FL_ONGROUND ) && player->get_move_type() != MOVETYPE_LADDER
		|| config->force_safe.moving->get<bool>() && record.m_velocity.Length2D() > 4.f
		|| config->force_safe.standing->get<bool>() && record.m_velocity.Length2D() <= 4.f
		|| config->fallback_mode->get<int>() && log->m_shots > 1 )
		|| record.m_extrapolated && ( record.m_extrapolate_amt || record.m_velocity.Length() > 2.f ) )
		safety = penetration::safety_no_roll;

	if ( !bot && ( config->extra_safety.air->get<bool>() && !( record.m_flags & FL_ONGROUND ) && player->get_move_type() != MOVETYPE_LADDER
		|| config->extra_safety.moving->get<bool>() && record.m_velocity.Length2D() > 4.f
		|| config->extra_safety.standing->get<bool>() && record.m_velocity.Length2D() <= 4.f
		|| config->fallback_mode->get<int>() && log->m_shots > 3 )
		|| vars::aim.fallback_on_key->get<bool>() )
		safety = penetration::safety_full;

	if ( !bot && ( !globals::nospread && ( interfaces::globals()->curtime - log->m_last_unusual_pitch < 0.5f && !record.m_shot && fabsf( record.m_eye_angles.x ) < 60.f
		/*|| interfaces::globals()->curtime - log->m_last_zero_pitch < 0.3f && fabsf( record.m_eye_angles.x ) >= 60.f*/ )
		|| record.m_extrapolated && ( record.m_extrapolate_amt || record.m_velocity.Length() > 2.f ) && ( !ConVar::cl_lagcompensation || !ConVar::cl_predict ) )
		|| record.m_pitch_jitter )
		safety = penetration::safety_full;

#if defined(ALPHA) || !defined(RELEASE)
	visuals::players[ record.m_index ].debug.target_sp = safety > penetration::safety_none;
	visuals::players[ record.m_index ].debug.target_vsp = safety == penetration::safety_full;
#endif

	ret = hitscan_record( target, record, safety, extra, eyepos );

	return ret.damage >= 1;
}

void hitscan::multipoint_hitbox( circular_buffer<aimbot::aimpoint_t, 78>& points, int index, const studiohdr_t* studio_hdr, lag_record_t& record, const Vector& eyepos, float spread_radius, bool is_knife, bool is_zeus, const weaponconfig_t* config, int safety, lag_record_t* extra, CCSWeaponData* weapon_data )
{
	auto& log = player_log::get_log( record.m_index );
	const auto hitbox = studio_hdr->pHitbox( index, 0 );
	if ( !hitbox )
		return;

	const auto player = globals::get_player( record.m_index );
	const auto is_bot = player->get_player_info().fakeplayer;

	if ( is_bot )
		safety = penetration::safety_none;

	const auto resolve_dir = log.get_dir( &record );
	const auto out = record.matrix( resolve_dir )[ hitbox->bone ];

	Vector vmin{}, vmax{};
	math::vector_transform( hitbox->bbmin, out, vmin );
	math::vector_transform( hitbox->bbmax, out, vmax );

	auto center = ( vmin + vmax ) * 0.5f;

	const auto cur_angles = math::calc_angle( eyepos, center );
	Vector forward{};
	math::angle_vectors( cur_angles, &forward );

	if ( is_knife )
	{
		const auto radius = hitbox->radius == -1 ? 3.f : hitbox->radius;
		const auto back = forward * -radius * 0.975f;
		const auto point = points.emplace_back( center + back, center, index, hitbox->group );
		point->safety = penetration::safety_full;
		return;
	}

	auto dist_scaled_dmg = static_cast< float >( weapon_data->idamage );
	dist_scaled_dmg *= pow( weapon_data->flrangemodifier, eyepos.Dist( center ) / 500.f );
	auto max_body_damage = dist_scaled_dmg;
	penetration::scale_damage( player, HITGROUP_STOMACH, weapon_data, max_body_damage, !is_zeus );

	auto scaled_damage = dist_scaled_dmg;
	penetration::scale_damage( player, hitbox->group, weapon_data, scaled_damage, !is_zeus );
	const auto minimum_damage = static_cast< int >( scaled_damage ) - 1;

	if ( static_cast< int >( scaled_damage ) + 1 < config->get_mindmg( player, static_cast< int >( max_body_damage ) ) )
		return;

	if ( hitbox->radius <= 0.f )
	{
		if ( safety == penetration::safety_none )
		{
			const auto point = points.emplace_back( center, center, index, hitbox->group );
			point->no_pen_damage = minimum_damage;
			point->center_dist = 0.f;
			point->safety = is_bot ? penetration::safety_full : penetration::safety_none;
			point->safety_size = 0.f;
		}

		return;
	}

	auto rs = 0.975f;

	log.m_shots = std::max( 0, log.m_shots );
	log.m_shots_spread = std::max( 0, log.m_shots_spread );

	if ( hitbox->group == HITGROUP_RIGHTLEG || hitbox->group == HITGROUP_LEFTLEG )
		rs *= 0.8f;

	auto total_shots = log.m_shots_spread;
	if ( log.m_shots > 2 )
		total_shots += log.m_shots - 2;

	if ( !globals::nospread )
		rs *= 1.f - clamp( total_shots * 0.2f, 0.f, 0.8f );

	auto ps = is_zeus ? 0.f : config->pointscale->get<float>();

	rs *= .5f + .5f * clamp( ps, 0.f, 100.f ) * .01f;

	if ( !ConVar::cl_lagcompensation || !ConVar::cl_predict )
		rs *= 0.8f;

	rs -= spread_radius * 0.1f;

	if ( !is_zeus && !pointscale_hitboxes[ index ] )
		rs = 0.f;

	rs = clamp( rs, 0.f, 0.975f );

	auto use_extra = extra && record.m_extrapolate_amt;
	auto safe_legs = record.m_velocity.Length2D() < 1.f || !( record.m_flags & FL_ONGROUND );

	circular_buffer<Vector> projected_points[ 3 ][ static_cast< int >( resolver_direction::resolver_direction_max ) ];
	circular_buffer<Vector> mapped_points[ 3 ][ static_cast< int >( resolver_direction::resolver_direction_max ) ];
	auto orig_center = center;

	auto fw = forward;
	const auto n = fw;
	const auto u = forward.Cross( Vector( 0, 0, 1 ) ).Normalize();
	const auto v = u.Cross( n ).Normalize();
	for ( auto i = 0; i < 3; i++ )
	{
		auto is_extra_record = i == 2;
		auto is_extra_matrix = i == 1;

		if ( !use_extra && is_extra_record )
			continue;

		if ( !record.m_last_ang_differs && is_extra_matrix )
			continue;

		for ( auto j = resolver_direction::resolver_networked; j < resolver_direction::resolver_direction_max; j++ )
		{
			if ( is_bot && j != resolver_direction::resolver_networked )
				continue;

			if ( !( j == resolve_dir && !is_extra_record
				|| j == resolver_direction::resolver_networked && !is_extra_record && !is_extra_matrix
				|| j == resolver_direction::resolver_min
				|| j == resolver_direction::resolver_max
				|| j == resolver_direction::resolver_min_min
				|| j == resolver_direction::resolver_max_max
				|| j == resolver_direction::resolver_min_extra && !is_extra_record
				|| j == resolver_direction::resolver_max_extra && !is_extra_record ) )
				continue;

			const auto out2 = is_extra_record ? extra->matrix( j )[ hitbox->bone ] : is_extra_matrix ? record.extra_matrix( j )[ hitbox->bone ] : record.matrix( j )[ hitbox->bone ];
			Vector vmin2{};
			Vector vmax2{};
			math::vector_transform( hitbox->bbmin, out2, vmin2 );
			math::vector_transform( hitbox->bbmax, out2, vmax2 );

			auto& proj = projected_points[ i ][ static_cast< int >( j ) ];
			proj.reserve( 24 );

			const auto add_circle_points = [&] ( const Vector& dir )
			{
				proj.emplace_back( vmin2 + dir );
				proj.emplace_back( vmax2 + dir );
			};

			const auto right = forward.Cross( Vector( 0, 0, 1 ) ) * hitbox->radius;
			add_circle_points( right );
			const auto top = Vector( 0, 0, 1 ) * hitbox->radius;
			add_circle_points( top );
			const auto left = forward.Cross( Vector( 0, 0, -1 ) ) * hitbox->radius;
			add_circle_points( left );
			const auto bot = Vector( 0, 0, -1 ) * hitbox->radius;
			add_circle_points( bot );

			add_circle_points( math::lerp( right, top, 0.375 ).Normalize() * hitbox->radius );
			add_circle_points( math::lerp( right, top, 0.625 ).Normalize() * hitbox->radius );
			add_circle_points( math::lerp( right, bot, 0.375 ).Normalize() * hitbox->radius );
			add_circle_points( math::lerp( right, bot, 0.625 ).Normalize() * hitbox->radius );

			add_circle_points( math::lerp( left, top, 0.375 ).Normalize() * hitbox->radius );
			add_circle_points( math::lerp( left, top, 0.625 ).Normalize() * hitbox->radius );
			add_circle_points( math::lerp( left, bot, 0.375 ).Normalize() * hitbox->radius );
			add_circle_points( math::lerp( left, bot, 0.625 ).Normalize() * hitbox->radius );

			for ( auto k = 0u; k < proj.size(); k++ )
			{
				auto& p = proj[ k ];
				auto v = p - orig_center;
				auto dist = v.x * fw.x + v.y * fw.y + v.z * fw.z;
				p = p - fw * dist;
			}

			auto& mapped = mapped_points[ i ][ static_cast< int >( j ) ];
			mapped.resize( 24 );

			const auto p0 = projected_points[ 0 ][ static_cast< int >( resolver_direction::resolver_networked ) ][ 0 ];
			for ( size_t k = 0; k < proj.size(); k++ )
			{
				auto& p = proj[ k ];
				mapped_points[ i ][ static_cast< int >( j ) ][ k ] = Vector{ ( p - p0 ).Dot( u ), ( p - p0 ).Dot( v ), 0.f };
			}

			poly_intersect::graham_scan( mapped_points[ i ][ static_cast< int >( j ) ] );
		}
	}

	const auto point_to_world = [&] ( const Vector& p )
	{
		const auto p0 = projected_points[ 0 ][ static_cast< int >( resolver_direction::resolver_networked ) ][ 0 ];
		const auto x = Vector( p0[ 0 ], u[ 0 ], v[ 0 ] ).Dot( Vector( 1, p.x, p.y ) );
		const auto y = Vector( p0[ 1 ], u[ 1 ], v[ 1 ] ).Dot( Vector( 1, p.x, p.y ) );
		const auto z = Vector( p0[ 2 ], u[ 2 ], v[ 2 ] ).Dot( Vector( 1, p.x, p.y ) );
		return Vector( x, y, z );
	};

	const auto extra_head_calc = [&] ( const Vector& top, const Vector& bot )
	{
		if ( index == HITBOX_HEAD )
		{
			Vector point;

			for ( auto i = 0u; i < 6; i++ )
			{
				point = math::lerp( top, bot, i / 6.f );

				const auto check_hit = [&] ()
				{
					const auto studio_model = interfaces::model_info()->GetStudioModel( player->get_model() );
					const auto hitbox_set = studio_model->pHitboxSet( player->get_hitbox_set() );

					matrix3x4_t* hitboxbones[ 128 ];
					for ( auto j = 0; j < 128; j++ )
						hitboxbones[ j ] = &record.matrix( resolve_dir )[ j ];

					auto direction = point - eyepos;
					math::fast_vec_normalize( direction );

					CGameTrace enter_trace;
					Ray_t ray;
					ray.Init( eyepos, point + direction * ( weapon_data->range + penetration::ray_extension ) );
					return penetration::trace_to_studio_csgo_hitgroups_priority( &ray, player->get_model_ptr(), hitbox_set, hitboxbones, MASK_SHOT_HULL | CONTENTS_HITBOX, &record.m_origin, player->get_model_scale(), &enter_trace ) && enter_trace.hitgroup == hitbox->group;
				};

				if ( !check_hit() )
					break;
			}

			return ( top + point ) / 2.f;
		}

		return top;
	};

	const auto extract_points = [&] ( poly_intersect::convex_polygon& intersect, int safety ) -> bool
	{
		if ( intersect.size() < 3 )
			return false;

		Vector rightmost, topmost, bottommost;
		Vector leftmost = rightmost = topmost = bottommost = intersect[ 0 ];

		for ( auto i = 0u; i < intersect.size(); i++ )
		{
			auto& p = intersect[ i ];
			if ( p.x < leftmost.x - 0.01f )
				leftmost = p;

			else if ( p.x > rightmost.x + 0.01f )
				rightmost = p;

			if ( p.y > topmost.y + 0.01f )
				topmost = p;

			else if ( p.y < bottommost.y - 0.01f )
				bottommost = p;
		}

		center = ( leftmost + rightmost + bottommost + topmost ) / 4.f;
		std::array intersection{ point_to_world( math::lerp( center, leftmost, rs ) ), point_to_world( math::lerp( center, rightmost, rs ) ), index == HITBOX_HEAD ? point_to_world( math::lerp( center, topmost, rs ) ) : point_to_world( center ) };

		const auto new_center = extra_head_calc( intersection.back(), point_to_world( math::lerp( center, bottommost, rs ) ) );
		auto safety_size = safety == penetration::safety_none ? 0.f : poly_intersect::area( intersect );
		if ( index == HITBOX_HEAD )
			safety_size *= 0.1f;
		else if ( index == HITBOX_UPPER_CHEST )
			safety_size *= 0.2f;

		if ( rs < 0.2f || index == HITBOX_HEAD )
		{
			const auto point = points.emplace_back( new_center, new_center, index, hitbox->group );
			point->no_pen_damage = minimum_damage;
			point->center_dist = 0.f;
			point->safety = is_bot ? penetration::safety_full : safety;
			point->safety_size = is_bot ? 10.f : safety_size;
		}

		if ( rs >= 0.2f )
			for ( auto& p : intersection )
			{
				const auto point = points.emplace_back( p, new_center, index, hitbox->group );
				point->no_pen_damage = minimum_damage;
				point->center_dist = p.DistToSqr( new_center );
				point->safety = is_bot ? penetration::safety_full : safety;
				point->safety_size = is_bot ? 10.f : safety_size;
			}

		return true;
	};



	if ( safety == penetration::safety_none && ( index != HITBOX_HEAD || !config::current_config( local_weapon )->baim.high_risk->get<bool>() || is_bot || vars::aim.headshot_only() ) )
	{
		auto intersect = record.m_last_ang_differs ? poly_intersect::get_intersection_poly( mapped_points[ 0 ][ static_cast< int >( resolve_dir ) ], mapped_points[ 1 ][ static_cast< int >( resolve_dir ) ] ) : mapped_points[ 0 ][ static_cast< int >( resolve_dir ) ];
		if ( !extract_points( intersect, penetration::safety_none ) )
			return;
	}

	if ( is_bot || ( index == HITBOX_LEFT_CALF || index == HITBOX_RIGHT_CALF || index == HITBOX_LEFT_FOOT || index == HITBOX_RIGHT_FOOT ) && !safe_legs )
		return;

	rs *= 0.8f;

	auto intersect = mapped_points[ 0 ][ static_cast< int >( resolver_direction::resolver_min ) ];
	intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 0 ][ static_cast< int >( resolver_direction::resolver_max ) ] );

	if ( use_extra )
	{
		intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 2 ][ static_cast< int >( resolver_direction::resolver_min ) ] );
		intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 2 ][ static_cast< int >( resolver_direction::resolver_max ) ] );
	}

	if ( record.m_last_ang_differs )
	{
		intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 1 ][ static_cast< int >( resolver_direction::resolver_min ) ] );
		intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 1 ][ static_cast< int >( resolver_direction::resolver_max ) ] );
	}

	if ( safety != penetration::safety_full && !extract_points( intersect, penetration::safety_no_roll ) )
		return;

	intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 0 ][ static_cast< int >( resolver_direction::resolver_min_min ) ] );
	intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 0 ][ static_cast< int >( resolver_direction::resolver_max_max ) ] );

	intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 0 ][ static_cast< int >( resolver_direction::resolver_min_extra ) ] );
	intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 0 ][ static_cast< int >( resolver_direction::resolver_max_extra ) ] );

	if ( record.m_last_ang_differs )
	{
		intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 1 ][ static_cast< int >( resolver_direction::resolver_min_min ) ] );
		intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 1 ][ static_cast< int >( resolver_direction::resolver_max_max ) ] );

		intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 1 ][ static_cast< int >( resolver_direction::resolver_min_extra ) ] );
		intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 1 ][ static_cast< int >( resolver_direction::resolver_max_extra ) ] );
	}

	if ( use_extra )
	{
		intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 2 ][ static_cast< int >( resolver_direction::resolver_min_min ) ] );
		intersect = poly_intersect::get_intersection_poly( intersect, mapped_points[ 2 ][ static_cast< int >( resolver_direction::resolver_max_max ) ] );
	}

	extract_points( intersect, penetration::safety_full );
}

void hitscan::create_local_record( bool extrapolate )
{
	prediction::run_prediction( globals::current_cmd->command_number );

	auto backup_angle = *local_player->eye_angles_virtual();

	animations::predict_animation_state( local_record, local_player, extrapolate ? 3 : 0, true );
	interfaces::prediction()->SetLocalViewAngles( local_record.m_eye_angles );
	interfaces::globals()->curtime = local_record.m_simtime;
	bone_setup::handle_bone_setup( local_record, resolver_direction::resolver_zero );
	const auto bak_state = local_record.m_state[ resolver_direction::resolver_zero ];

	animations::predict_animation_state( local_record, local_player, extrapolate ? 3 : 0 );
	interfaces::prediction()->SetLocalViewAngles( local_record.m_eye_angles );
	interfaces::globals()->curtime = local_record.m_simtime;
	bone_setup::handle_bone_setup( local_record, resolver_direction::resolver_networked );

	local_record.m_state[ resolver_direction::resolver_zero ] = bak_state;

	//aimbot_helpers::draw_debug_hitboxes( local_player, local_record.matrix( resolver_direction::resolver_networked ), -1, interfaces::globals()->interval_per_tick * 2, Color::LightBlue() );
	//aimbot_helpers::draw_debug_hitboxes( local_player, local_record.matrix( resolver_direction::resolver_zero ), -1, interfaces::globals()->interval_per_tick * 2, Color::Red() );

	interfaces::prediction()->SetLocalViewAngles( backup_angle );
}

bool hitscan::hitscan_self( C_CSPlayer* enemy, bool extrapolate, lag_record_t& record )
{
	constexpr auto is_weapon = [] ( ClassId id )
	{
		return id >= ClassId::CWeaponAug && id <= ClassId::CWeaponXM1014 || id == ClassId::CAK47 || id == ClassId::CDEagle;
	};

	const auto weapon = globals::get_weapon( enemy->get_active_weapon() );
	if ( !weapon || !is_weapon( weapon->GetClientClass()->m_ClassID ) )
		return false;

	const auto sound_pos = sound_esp::get_position( enemy->EntIndex(), false );
	auto eye_pos = enemy->IsDormant() ? sound_pos + Vector( 0.f, 0.f, 64.f ) : enemy->get_eye_pos( false );
	auto exe_pos_extrap = eye_pos;
	if ( extrapolate && enemy->get_velocity().Length2D() > 20.f )
		exe_pos_extrap += enemy->get_velocity() * interfaces::globals()->interval_per_tick * 6;

	if ( enemy->get_origin().Dist( record.m_origin ) > weapon->get_wpn_data()->range )
		return false;

	std::vector<aimbot::aimpoint_t> points = {};
	points.reserve( 10 );

	const auto target = local_player;
	const auto health = target->get_health();
	const auto wpn_data = weapon->get_wpn_data();

	if ( enemy->get_velocity().Length() < 2.f )
		extrapolate = false;

	if ( extrapolate )
	{
		multipoint_self( record, exe_pos_extrap, points );
		if ( !points.empty() )
		{
			for ( auto& point : points )
			{
				if ( penetration::can_hit( enemy, {}, exe_pos_extrap, &point, point.damage ) )
				{
					auto dmg = static_cast< float >( point.damage );
					penetration::scale_damage( target, point.hitgroup, wpn_data, dmg, true );
					if ( dmg >= std::min( health, 5 ) )
						return true;
				}
			}
		}
	}

	points.clear();
	multipoint_self( record, eye_pos, points );
	if ( !points.empty() )
	{
		for ( auto& point : points )
		{
			if ( penetration::can_hit( enemy, {}, eye_pos, &point, point.damage ) )
			{
				auto dmg = static_cast< float >( point.damage );
				penetration::scale_damage( target, point.hitgroup, wpn_data, dmg, true );
				if ( dmg >= std::min( health, 5 ) )
					return true;
			}
		}
	}

	return false;
}

bool hitscan::check_hittable_by_enemy( C_CSPlayer* enemy, bool extrapolate )
{
	constexpr auto is_weapon = [] ( ClassId id )
	{
		return id >= ClassId::CWeaponAug && id <= ClassId::CWeaponXM1014 || id == ClassId::CAK47 || id == ClassId::CDEagle;
	};

	const auto weapon = globals::get_weapon( enemy->get_active_weapon() );
	if ( !weapon || !is_weapon( weapon->GetClientClass()->m_ClassID ) )
		return false;

	static auto get_rotated_pos = [] ( Vector start, float rotation, float distance ) -> Vector
	{
		const auto rad = DEG2RAD( rotation );
		start.x += cos( rad ) * distance;
		start.y += sin( rad ) * distance;

		return start;
	};

	const auto model = local_player->get_model();
	if ( !model )
		return false;

	const auto studio_hdr = interfaces::model_info()->GetStudioModel( model );
	if ( !studio_hdr )
		return false;

	const auto hitbox = studio_hdr->pHitbox( HITBOX_HEAD, 0 );

	const auto mat = local_record.matrix( resolver_direction::resolver_networked )[ hitbox->bone ];

	Vector vmax{};
	math::vector_transform( hitbox->bbmax, mat, vmax );

	const auto scanzpos = vmax += Vector( 0, 0, 1 ) * hitbox->radius;
	const auto sound_pos = sound_esp::get_position( enemy->EntIndex(), false );
	const auto eye_pos = enemy->IsDormant() ? sound_pos + Vector( 0.f, 0.f, 64.f ) : enemy->get_eye_pos( false );
	auto exe_pos_extrap = eye_pos;
	if ( extrapolate )
		exe_pos_extrap += enemy->get_velocity() * interfaces::globals()->interval_per_tick * 6;

	const auto index = enemy->EntIndex();

	const auto target_position = Vector( local_record.m_origin.x, local_record.m_origin.y, scanzpos.z );

	if ( target_position.Dist( eye_pos ) > weapon->get_wpn_data()->range )
		return false;

	const auto target_angle = math::calc_angle( eye_pos, target_position );

	if ( enemy->get_velocity().Length() < 2.f )
		extrapolate = false;

	return penetration::check_hittable( { index, eye_pos, get_rotated_pos( target_position, math::normalize_float( target_angle.y + 90.f ), 30.f ) } )
		|| penetration::check_hittable( { index, eye_pos, get_rotated_pos( target_position, math::normalize_float( target_angle.y - 90.f ), 30.f ) } )
		|| extrapolate && penetration::check_hittable( { index, exe_pos_extrap, get_rotated_pos( target_position, math::normalize_float( target_angle.y + 90.f ), 30.f ) } )
		|| extrapolate && penetration::check_hittable( { index, exe_pos_extrap, get_rotated_pos( target_position, math::normalize_float( target_angle.y - 90.f ), 30.f ) } );
}

bool hitscan::check_enemy_hittable( C_CSPlayer* enemy )
{
	static auto get_rotated_pos = [] ( Vector start, float rotation, float distance )
	{
		const auto rad = DEG2RAD( rotation );
		start.x += cos( rad ) * distance;
		start.y += sin( rad ) * distance;

		return start;
	};

	const auto index = enemy->EntIndex();
	auto eye_pos = local_player->get_eye_pos();
	if ( vars::aim.fake_duck->get<bool>() )
		eye_pos.z = local_player->get_origin().z + 64.f;
	auto& log = player_log::get_log( enemy->EntIndex() );
	// log cant be empty here
	const auto weapon = local_weapon;
	if ( !weapon || enemy->get_origin().Dist( eye_pos ) > weapon->get_wpn_data()->range )
		return false;

	std::vector<lag_record_t*> records;
	aimbot::get_scan_records( enemy, records, true );
	[&]
	{
		if ( !log.need_extrapolate )
			return;

		log.need_extrapolate = false;

		if ( lagcomp::run_extrapolation( enemy, true ) )
			records.emplace_back( &log.extrapolated_record );
	}( );

	auto hittable = false;

	for ( const auto& record : records )
	{
		const auto target_position = Vector( record->m_origin.x, record->m_origin.y, record->m_origin.z + 75.f );
		const auto target_angle = math::calc_angle( eye_pos, target_position );

		if ( penetration::check_hittable( { index, eye_pos, get_rotated_pos( target_position, math::normalize_float( target_angle.y + 90.f ), 30.f ) } )
		|| penetration::check_hittable( { index, eye_pos, get_rotated_pos( target_position, math::normalize_float( target_angle.y - 90.f ), 30.f ) } ) )
		{
			hittable = true;
			break;
		}

	}

	if ( !hittable && !records.empty() && config::current_config( weapon )->predictive_autostop->get<bool>() && !vars::aim.fake_duck->get<bool>() && prediction::unpred_vel.Length2D() > 20.f )
	{
		const auto& record = records.front();
		eye_pos = local_player->get_eye_pos() + prediction::unpred_vel * interfaces::globals()->interval_per_tick * 3.f;
		const auto target_position = Vector( record->m_origin.x, record->m_origin.y, record->m_origin.z + 75.f );
		const auto target_angle = math::calc_angle( eye_pos, target_position );

		if ( penetration::check_hittable( { index, eye_pos, get_rotated_pos( target_position, math::normalize_float( target_angle.y + 90.f ), 30.f ) } )
		|| penetration::check_hittable( { index, eye_pos, get_rotated_pos( target_position, math::normalize_float( target_angle.y - 90.f ), 30.f ) } ) )
			hittable = true;
	}

	return hittable;
}


void hitscan::multipoint_self( lag_record_t& record, Vector& eyepos, std::vector<aimbot::aimpoint_t>& points )
{
	const auto model = local_player->get_model();
	if ( !model )
		return;

	const auto studio_hdr = interfaces::model_info()->GetStudioModel( model );
	if ( !studio_hdr )
		return;

	const auto calc = [&] ( size_t i, bool second = false )
	{
		const auto hitbox = studio_hdr->pHitbox( i, 0 );

		auto mat = record.matrix( second ? resolver_direction::resolver_zero : resolver_direction::resolver_networked )[ hitbox->bone ];

		Vector vmin{}, vmax{};
		math::vector_transform( hitbox->bbmax, mat, vmax );
		math::vector_transform( hitbox->bbmin, mat, vmin );

		const auto orig_center = ( vmin + vmax ) * 0.5f;

		const auto cur_angles = math::calc_angle( eyepos, orig_center );

		Vector forward{};
		math::angle_vectors( cur_angles, &forward );

		circular_buffer<Vector> proj( 24 );
		circular_buffer<Vector> mapped_points( 24 );
		mapped_points.resize( 24 );

		auto fw = forward;
		const auto n = fw;
		const auto u = forward.Cross( Vector( 0, 0, 1 ) ).Normalize();
		const auto v = u.Cross( n ).Normalize();

		const auto add_circle_points = [&] ( const Vector& dir )
		{
			proj.emplace_back( vmin + dir );
			proj.emplace_back( vmax + dir );
		};

		const auto right = forward.Cross( Vector( 0, 0, 1 ) ) * hitbox->radius;
		add_circle_points( right );
		const auto top = Vector( 0, 0, 1 ) * hitbox->radius;
		add_circle_points( top );
		const auto left = forward.Cross( Vector( 0, 0, -1 ) ) * hitbox->radius;
		add_circle_points( left );
		const auto bot = Vector( 0, 0, -1 ) * hitbox->radius;
		add_circle_points( bot );

		add_circle_points( math::lerp( right, top, 0.375 ).Normalize() * hitbox->radius );
		add_circle_points( math::lerp( right, top, 0.625 ).Normalize() * hitbox->radius );
		add_circle_points( math::lerp( right, bot, 0.375 ).Normalize() * hitbox->radius );
		add_circle_points( math::lerp( right, bot, 0.625 ).Normalize() * hitbox->radius );

		add_circle_points( math::lerp( left, top, 0.375 ).Normalize() * hitbox->radius );
		add_circle_points( math::lerp( left, top, 0.625 ).Normalize() * hitbox->radius );
		add_circle_points( math::lerp( left, bot, 0.375 ).Normalize() * hitbox->radius );
		add_circle_points( math::lerp( left, bot, 0.625 ).Normalize() * hitbox->radius );

		for ( auto k = 0u; k < proj.size(); k++ )
		{
			auto& p = proj[ k ];
			auto v = p - orig_center;
			auto dist = v.x * fw.x + v.y * fw.y + v.z * fw.z;
			p = p - fw * dist;
		}

		const auto p0 = proj[ 0 ];
		for ( size_t k = 0; k < proj.size(); k++ )
		{
			auto& p = proj[ k ];
			mapped_points[ k ] = Vector{ ( p - p0 ).Dot( u ), ( p - p0 ).Dot( v ), 0.f };
		}

		Vector rightmost, topmost;
		Vector leftmost = rightmost = topmost = mapped_points[ 0 ];
		for ( auto j = 0u; j < proj.size(); j++ )
		{
			auto& p = mapped_points[ j ];
			if ( p.x < leftmost.x - 0.01f )
				leftmost = p;

			else if ( p.x > rightmost.x + 0.01f )
				rightmost = p;

			if ( p.y > topmost.y + 0.01f )
				topmost = p;
		}

		const auto point_to_world = [&] ( const Vector& p )
		{
			const auto p0 = proj[ 0 ];
			const auto x = Vector( p0[ 0 ], u[ 0 ], v[ 0 ] ).Dot( Vector( 1, p.x, p.y ) );
			const auto y = Vector( p0[ 1 ], u[ 1 ], v[ 1 ] ).Dot( Vector( 1, p.x, p.y ) );
			const auto z = Vector( p0[ 2 ], u[ 2 ], v[ 2 ] ).Dot( Vector( 1, p.x, p.y ) );
			return Vector( x, y, z );
		};

		std::vector temp_points{ leftmost, rightmost };
		if ( i == HITBOX_HEAD )
			temp_points.emplace_back( topmost );

		std::vector<Vector> intersection;

		std::ranges::transform( temp_points, std::back_inserter( intersection ), [&] ( const Vector& p )
		{
			return point_to_world( p );
		} );

		for ( auto& p : intersection )
			points.emplace_back( p, p, i, hitbox->group );
	};

	calc( HITBOX_HEAD );
	calc( HITBOX_HEAD, true );
	calc( HITBOX_RIGHT_UPPER_ARM );
	calc( HITBOX_LEFT_UPPER_ARM );
	calc( HITBOX_RIGHT_CALF );
	calc( HITBOX_LEFT_CALF );
	calc( HITBOX_RIGHT_CALF, true );
	calc( HITBOX_LEFT_CALF, true );
	calc( HITBOX_PELVIS );
}

void hitscan::filter_hitboxes()
{
	const auto weapon = local_weapon;

	const auto config = config::current_config( weapon );

	for ( size_t i = 0; i < enabled_hitboxes.size(); i++ )
	{
		switch ( i )
		{
			// 78
			case HITBOX_HEAD: // 12
			{
				enabled_hitboxes[ i ] = config->hitboxes.head->get<bool>() || vars::aim.headshot_only();
				break;
			}
			case HITBOX_UPPER_CHEST: // 9
			{
				enabled_hitboxes[ i ] = config->hitboxes.chest->get<bool>() && !vars::aim.headshot_only();
				break;
			}
			case HITBOX_BODY: // 9
			case HITBOX_PELVIS: // 9
			{
				enabled_hitboxes[ i ] = config->hitboxes.stomach->get<bool>() && !vars::aim.headshot_only();
				break;
			}
			case HITBOX_RIGHT_UPPER_ARM: //9
			case HITBOX_LEFT_UPPER_ARM: //9
			{
				enabled_hitboxes[ i ] = config->hitboxes.arms->get<bool>() && !vars::aim.headshot_only();
				break;
			}
			case HITBOX_LEFT_CALF: // 3
			case HITBOX_RIGHT_CALF: // 3
			{
				enabled_hitboxes[ i ] = config->hitboxes.legs->get<bool>() && !vars::aim.headshot_only();
				break;
			}
			case HITBOX_LEFT_FOOT: // 3
			case HITBOX_RIGHT_FOOT: // 3
			{
				enabled_hitboxes[ i ] = config->hitboxes.feet->get<bool>() && !vars::aim.headshot_only();
				break;
			}
			default:
				enabled_hitboxes[ i ] = false;
				break;
		}
	}

	for ( size_t i = 0; i < pointscale_hitboxes.size(); i++ )
	{
		switch ( i )
		{
			case HITBOX_HEAD:
			{
				pointscale_hitboxes[ i ] = config->pointscales.head->get<bool>();
				break;
			}
			case HITBOX_UPPER_CHEST:
			{
				pointscale_hitboxes[ i ] = config->pointscales.chest->get<bool>();
				break;
			}
			case HITBOX_BODY:
			case HITBOX_PELVIS:
			{
				pointscale_hitboxes[ i ] = config->pointscales.stomach->get<bool>();
				break;
			}
			case HITBOX_RIGHT_UPPER_ARM:
			case HITBOX_LEFT_UPPER_ARM:
			{
				pointscale_hitboxes[ i ] = config->pointscales.arms->get<bool>();
				break;
			}
			case HITBOX_LEFT_CALF:
			case HITBOX_RIGHT_CALF:
			{
				pointscale_hitboxes[ i ] = config->pointscales.legs->get<bool>();
				break;
			}
			case HITBOX_LEFT_FOOT:
			case HITBOX_RIGHT_FOOT:
			{
				pointscale_hitboxes[ i ] = config->pointscales.feet->get<bool>();
				break;
			}
			default:
				pointscale_hitboxes[ i ] = false;
				break;
		}
	}
}