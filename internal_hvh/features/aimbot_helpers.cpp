#include "../include_cheat.h"

void aimbot_helpers::no_recoil( CUserCmd* cmd )
{
	if ( vars::legit.enabled->get<bool>() )
		return;

	if ( !vars::aim.enabled->get<bool>() && !globals::nospread )
		return;

	var( weapon_recoil_scale );
	cmd->viewangles -= local_player->get_aim_punch() * weapon_recoil_scale->get_float();
}

void aimbot_helpers::no_visual_recoil( CViewSetup& v_view )
{
	if ( !local_player || !vars::aim.enabled->get<bool>() || interfaces::input()->m_fCameraInThirdPerson || !local_player->get_alive() || vars::legit.enabled->get<bool>() )
		return;

	v_view.angles -= local_player->get_aim_punch() * 0.9f;

	if ( vars::aim.punch->get<bool>() )
		v_view.angles -= local_player->get_view_punch();
}

void aimbot_helpers::stop_to_speed( float speed, CMoveData* mv, C_CSPlayer* player )
{
	CGameMovement::friction( mv, player );
	CGameMovement::check_parameters( mv, player );

	auto mv_cp = *mv;
	CGameMovement::walk_move( &mv_cp, player );
	if ( mv_cp.m_vecVelocity.Length2D() <= speed )
		return;

	if ( mv->m_vecVelocity.Length2D() > speed )
	{
		QAngle ang{};
		math::vector_angles( mv->m_vecVelocity * -1.f, ang );
		ang.y = math::normalize_float( mv->m_vecViewAngles.y - ang.y );

		Vector forward{};
		math::angle_vectors( ang, &forward );
		forward = forward.To2D().Normalized();

		const auto target_move_len = std::min( mv->m_vecVelocity.Length2D() - speed, mv->m_flMaxSpeed );
		mv->m_flForwardMove = forward.x * target_move_len;
		mv->m_flSideMove = forward.y * target_move_len;
		mv->m_flUpMove = 0.f;
		return;
	}

	Vector forward, right;
	math::angle_vectors( mv->m_vecViewAngles, &forward, &right );
	forward.z = right.z = 0.f;

	VectorNormalize( forward );
	VectorNormalize( right );

	Vector wishdir( forward.x * mv->m_flForwardMove + right.x * mv->m_flSideMove, forward.y * mv->m_flForwardMove + right.y * mv->m_flSideMove, 0.f );
	VectorNormalize( wishdir );

	const auto currentspeed = mv->m_vecVelocity.Dot( wishdir );
	const auto target_accelspeed = speed - mv->m_vecVelocity.Length2D();
	const auto target_move_len = std::min( currentspeed + target_accelspeed, mv->m_flMaxSpeed );

	const auto speed_squared = ( mv->m_flForwardMove * mv->m_flForwardMove ) + ( mv->m_flSideMove * mv->m_flSideMove ) + ( mv->m_flUpMove * mv->m_flUpMove );
	const auto ratio = target_move_len / sqrt( speed_squared );
	if ( ratio < 1.f )
	{
		mv->m_flForwardMove *= ratio;
		mv->m_flSideMove *= ratio;
		mv->m_flUpMove *= ratio;
	}
}

void aimbot_helpers::stop_to_speed( float speed, CUserCmd* cmd )
{
	if ( ( speed == 0.f || prediction::unpred_move.Length() == 0.f ) && prediction::get_pred_info( cmd->command_number - 1 ).velocity.Length() < 1.1f )
	{
		// micromove
		if ( local_player->get_flags() & FL_ONGROUND && !( cmd->buttons & IN_JUMP ) && vars::aa.fake->get<bool>() && vars::aa.enabled->get<bool>() && cmd->command_number > 1 && prediction::get_pred_info( cmd->command_number - 1 ).m_flags & FL_ONGROUND )
		{
			auto forwardmove = interfaces::client_state()->chokedcommands % 2 ? 1.01f : -1.01f;
			forwardmove /= local_player->get_duck_amt() * .34f + 1.f - local_player->get_duck_amt();

			cmd->forwardmove = forwardmove;
			cmd->sidemove = 0.f;
		}

		return;
	}

	prediction::run_prediction( cmd->command_number - 1 );

	CMoveData data = interfaces::game_movement()->setup_move( local_player, cmd );
	stop_to_speed( speed, &data );

	cmd->forwardmove = data.m_flForwardMove;
	cmd->sidemove = data.m_flSideMove;

	const auto forwardmove = cmd->command_number % 2 ? 1.01f : -1.01f;

	CGameMovement::walk_move( &data, local_player );
	if ( vars::aa.fake->get<bool>() && local_player->get_velocity().Length() <= 1.10f && data.m_vecVelocity.Length() <= 1.10f && local_player->get_flags() & FL_ONGROUND
	&& vars::aa.enabled->get<bool>() )
	{
		cmd->forwardmove = forwardmove;
		cmd->sidemove = 0.f;
	}

	const auto factor = local_player->get_duck_amt() * .34f + 1.f - local_player->get_duck_amt();
	cmd->forwardmove /= factor;
	cmd->sidemove /= factor;

	prediction::run_prediction( cmd->command_number );
}


bool aimbot_helpers::autorevolver( C_WeaponCSBaseGun* weapon )
{
	if ( tickbase::to_recharge > 0 )
		return false;

	if ( weapon->get_weapon_id() == WEAPON_REVOLVER && config::current_config( weapon )->autorevolver->get<bool>() && weapon->get_clip1() > 0 )
	{
		if ( prediction::get_pred_info( globals::current_cmd->command_number ).postpone_fire_ready_time > interfaces::globals()->curtime )
			prediction::take_shot();
		else if ( interfaces::globals()->curtime < weapon->get_next_secondary_attack() - ticks_to_time( 3 ) )
			globals::current_cmd->buttons |= IN_ATTACK2;
	}

	return prediction::can_fire();
}

float aimbot_helpers::get_lowest_inaccuracy( C_WeaponCSBaseGun* weapon )
{
	const auto id = weapon->get_weapon_id();
	const auto config = config::current_config( weapon );

	const auto scoped = ( config->autoscope->get<bool>() || weapon->get_zoom_level() ) && ( id == WEAPON_AWP || id == WEAPON_G3SG1 || id == WEAPON_SCAR20 || id == WEAPON_SSG08 );

	const auto crouched = local_player->get_flags() & FL_DUCKING;
	const auto wpn_data = weapon->get_wpn_data();

	if ( crouched )
		return scoped ? wpn_data->flinaccuracycrouchalt : wpn_data->flinaccuracycrouch;

	return scoped ? wpn_data->flinaccuracystandalt : wpn_data->flinaccuracystand;
}

bool aimbot_helpers::highest_accuracy( C_WeaponCSBaseGun* weapon, bool check_landing )
{
	const auto id = weapon->get_weapon_id();
	const auto config = config::current_config( weapon );

	constexpr auto round_acc = [] ( float accuracy ) { return floorf( accuracy * 170.f ) / 170.f; };
	constexpr auto round_acc2 = [] ( float accuracy ) { return floorf( accuracy * 300.f ) / 300.f; };

	const auto scoped = ( config->autoscope->get<bool>() || weapon->get_zoom_level() ) && ( id == WEAPON_AWP || id == WEAPON_G3SG1 || id == WEAPON_SCAR20 || id == WEAPON_SSG08 );

	const auto inair = !( local_player->get_flags() & FL_ONGROUND );
	const auto crouched = local_player->get_flags() & FL_DUCKING;
	const auto wpn_data = weapon->get_wpn_data();

	if ( check_landing && local_player->get_anim_state()->in_hit_ground )
		return true;

	if ( config->index == 1 && config->scout_jumpshot->get<bool>() && inair )
		return weapon->get_inaccuracy() < 0.009f;

	if ( id == WEAPON_REVOLVER )
	{
		return interfaces::globals()->curtime > weapon->get_next_secondary_attack() - ticks_to_time( 7 );
	}

	if ( crouched )
	{
		if ( round_acc2( weapon->get_inaccuracy() ) <= ( scoped ? wpn_data->flinaccuracycrouchalt : wpn_data->flinaccuracycrouch ) )
			return true;
	}
	else
	{
		if ( round_acc( weapon->get_inaccuracy() ) <= ( scoped ? wpn_data->flinaccuracystandalt : wpn_data->flinaccuracystand ) )
			return true;
	}

	return false;
}

#pragma clang section text=".discard"

__declspec( noinline ) void aimbot_helpers::build_seed_table()
{
	if ( !precomputed_seeds.empty() )
		return;

	for ( auto i = 0; i < 255; i++ )
	{
		globals::random_seed( i + 1 );

		const auto pi_seed = globals::random_float( 0.f, 2.f * M_PI );

		precomputed_seeds.emplace_back( globals::random_float( 0.f, 1.f ), sin( pi_seed ), cos( pi_seed ) );
	}

	erase_end;
}

#pragma clang section text=""

std::tuple<float, float> aimbot_helpers::calc_hc( std::vector<std::shared_ptr<intersection>>& intersections, QAngle vangles, lag_record_t& record )
{
	const auto player = globals::get_player( record.m_index );
	auto weapon = local_weapon;
	if ( !weapon )
		return { 0.f, 0.f };

	Vector forward{}, right{}, up{};
	const auto eyepos = local_player->get_eye_pos();
	auto inaccuracy = weapon->get_inaccuracy();

	const auto angles = vangles.Clamp();
	const auto range = weapon->get_wpn_data()->range;
	math::angle_vectors( angles, &forward, &right, &up );
	math::fast_vec_normalize( forward );
	math::fast_vec_normalize( right );
	math::fast_vec_normalize( up );

	auto hits = 0;
	auto hits_roll = 0;
	auto i = 0;

	Vector v_spread = {};
	std::tuple<float, float, float>* seed = nullptr;
	float spread_x = {}, spread_y = {}, seed_inaccuracy = {};

	while ( i < 32 )
	{
		seed = &precomputed_seeds[ i ];
		seed_inaccuracy = std::get<0>( *seed ) * inaccuracy;
		spread_x = std::get<2>( *seed ) * seed_inaccuracy;
		spread_y = std::get<1>( *seed ) * seed_inaccuracy;
		v_spread = ( forward + right * spread_x + up * spread_y );

		math::fast_vec_normalize( v_spread );

		QAngle spread_view{};
		Vector end{};

		math::vector_angles( v_spread, spread_view );
		spread_view.NormalizeNoClamp();
		math::angle_vectors( angles - ( spread_view - angles ), &end );
		math::fast_vec_normalize( end );

		trace_t tr;
		Ray_t ray;
		const auto trace_end = eyepos + ( end * range );

		ray.Init( eyepos, trace_end );

		auto missed_roll = false;
		auto missed = false;
		for ( auto& cur : intersections )
		{
			if ( cur->roll && missed_roll )
				continue;

			cur->trace_hitboxes( ray, tr );
			if ( tr.m_pEnt != player )
			{
				if ( cur->roll )
					missed_roll = true;
				else
				{
					missed_roll = missed = true;
					break;
				}
			}
		}

		if ( !missed )
			hits++;

		if ( !missed_roll )
			hits_roll++;

		i++;
	}

	return { static_cast< float >( hits ) * 3.125f, static_cast< float >( hits_roll ) * 3.125f };
}

float aimbot_helpers::calc_penhc( aimbot::final_target_t* target, const bool skip_full, const bool maximum )
{
	const auto angles = math::calc_angle( local_player->get_eye_pos(), target->point.point ).Clamp();

	auto weapon = local_weapon;
	if ( !weapon )
		return 0.f;

	if ( globals::nospread )
		return 100.f;

	const auto true_highest_acc = maximum ? true : highest_accuracy( weapon );
	if ( !skip_full && true_highest_acc )
		return 100.f;

	Vector forward{}, right{}, up{};
	const auto eyepos = local_player->get_eye_pos();
	auto inaccuracy = maximum ? get_lowest_inaccuracy( weapon ) : weapon->get_inaccuracy();

	const auto range = weapon->get_wpn_data()->range;
	math::angle_vectors( angles, &forward, &right, &up );
	math::fast_vec_normalize( forward );
	math::fast_vec_normalize( right );
	math::fast_vec_normalize( up );

	auto hits = 0;
	auto i = 0;

	auto& log = player_log::get_log( target->record->m_index );
	const auto state = log.get_dir( target->record );

	Vector v_spread = {};
	std::tuple<float, float, float>* seed = nullptr;
	float spread_x = {}, spread_y = {}, seed_inaccuracy = {};

	std::vector<std::shared_ptr<detail::call_queue::queue_element>> traces;
	traces.reserve( 64 );
	std::vector<aimbot::aimpoint_t> points = {};
	points.reserve( 64 );

	auto health = target->player->get_predicted_health();
	auto mindmg = config::current_config( weapon )->get_mindmg( target->player, target->point.highest_damage );
	const auto is_revolver = weapon->get_weapon_id() == WEAPON_REVOLVER;

	const auto highest_acc = maximum ? true : local_player->get_velocity().Length() <= ( is_revolver ? 180.f : weapon->get_max_speed() ) / 3.f;

	while ( i < 64 )
	{
		seed = &precomputed_seeds[ i ];
		seed_inaccuracy = std::get<0>( *seed ) * inaccuracy;
		spread_x = std::get<2>( *seed ) * seed_inaccuracy;
		spread_y = std::get<1>( *seed ) * seed_inaccuracy;
		v_spread = ( forward + right * spread_x + up * spread_y );

		math::fast_vec_normalize( v_spread );

		QAngle spread_view{};
		Vector end{};

		math::vector_angles( v_spread, spread_view );
		spread_view.NormalizeNoClamp();
		math::angle_vectors( angles - ( spread_view - angles ), &end );
		math::fast_vec_normalize( end );

		const auto trace_end = eyepos + end * range;

		aimbot::aimpoint_t aimpoint;
		aimpoint.hitbox = -1;
		aimpoint.point = trace_end;

		points.emplace_back( aimpoint );

		i++;
	}

	auto calc_hitbox = [&] ( std::vector<aimbot::aimpoint_t>& aimpoints ) -> void
	{
		for ( auto& point : aimpoints )
			can_hit( local_player, penetration::pen_data( target->record, state ), eyepos, &point, point.damage, true );
	};

	traces.reserve( detail::callqueue.count_ );
	const auto count_per_thread = static_cast< int >( std::ceil( static_cast< float >( points.size() ) / detail::callqueue.count_ ) );
	std::vector<std::vector<aimbot::aimpoint_t>> temp( detail::callqueue.count_ );
	auto vecnum = 0;
	for ( auto i = 0u; i < points.size(); i++ )
	{
		if ( i % count_per_thread == 0 )
			temp[ vecnum ].resize( count_per_thread );
		temp[ vecnum ][ i % count_per_thread ] = points[ i ];
		if ( i % count_per_thread == count_per_thread - 1 )
		{
			traces.emplace_back( std::make_shared<detail::call_queue::queue_element>( [calc_hitbox, &temp, vecnum] { return calc_hitbox( temp[ vecnum ] ); } ) );
			vecnum++;
		}

		else if ( i == points.size() - 1 )
		{
			temp.resize( vecnum + 1 );
			temp[ vecnum ].resize( i % count_per_thread + 1 );
			traces.emplace_back( std::make_shared<detail::call_queue::queue_element>( [calc_hitbox, &temp, vecnum] { return calc_hitbox( temp[ vecnum ] ); } ) );
		}
	}

	detail::callqueue.perform( traces );

	auto is_zeus = weapon->is_zeus();

	for ( const auto& point_vec : temp )
		for ( const auto& point : point_vec )
		{
			if ( point.damage < 1 )
				continue;

			if ( !highest_acc && !is_zeus && point.hitbox != target->point.hitbox )
				continue;

			if ( !true_highest_acc && !is_zeus && point.hitgroup != target->point.hitgroup )
				continue;

			if ( true_highest_acc || point.damage >= mindmg || point.damage >= ( point.penetration_points.size() > 1 ? health + 5 : health ) )
				hits++;
		}

	return std::min( static_cast< float >( hits * weapon->get_wpn_data()->ibullets ) * 1.5625f, 100.f );
}

void aimbot_helpers::check_corner_hitpoint( aimbot::final_target_t& target )
{
	if ( target.point.center_dist < 1.f )
		return;

	const auto weapon = local_weapon;
	const auto player = target.player;

	const auto shot_pos = local_player->get_eye_pos();

	auto new_point = target.point;

	const auto is_singleshot = weapon->get_weapon_id() == WEAPON_SSG08 || weapon->get_weapon_id() == WEAPON_AWP || weapon->get_weapon_id() == WEAPON_REVOLVER || misc::retract_peek;
	const auto doubletap_ready = tickbase::fast_fire && !is_singleshot && tickbase::compute_current_limit() > 2;
	const auto target_health = player->get_predicted_health() + ( target.point.penetration_points.size() > 1 ? 5 : 0 );
	const auto target_shots_to_kill = static_cast< int >( ceilf( target_health / fminf( floorf( target.point.damage * ( doubletap_ready ? 2.f : 1.f ) ), target_health ) ) );

	for ( auto i = 0u; i < 6; i++ )
	{
		auto point2 = target.point;
		point2.penetration_points.clear();
		point2.point = math::lerp( target.point.point, target.point.center, i / 6.f );

		auto ret_point2 = can_hit( local_player, penetration::pen_data( target.record, player_log::get_log( target.record->m_index ).get_dir( target.record ), target.point.safety ), shot_pos, &point2, point2.damage );
		const auto mindmg = config::current_config( local_weapon )->get_mindmg( target.player, target.point.highest_damage );
		if ( point2.damage < mindmg )
		{
			point2.damage = 0;
			ret_point2 = false;
		}

		const auto health = player->get_predicted_health() + ( point2.penetration_points.size() > 1 ? 5 : 0 );
		const auto shots_to_kill = static_cast< int >( ceilf( health / fminf( floorf( point2.damage * ( doubletap_ready ? 2.f : 1.f ) ), health ) ) );

		if ( ret_point2 && shots_to_kill <= target_shots_to_kill )
			new_point = point2;
	}

	if ( new_point.point == target.point.point )
		return;

	new_point.penetration_points.clear();
	new_point.point = math::lerp( new_point.point, target.point.point, 0.5f );

	auto ret_new_point = can_hit( local_player, penetration::pen_data( target.record, player_log::get_log( target.record->m_index ).get_dir( target.record ), target.point.safety ), shot_pos, &new_point, new_point.damage );
	const auto mindmg = config::current_config( local_weapon )->get_mindmg( target.player, target.point.highest_damage );
	if ( new_point.damage < mindmg )
	{
		new_point.damage = 0;
		ret_new_point = false;
	}

	const auto health = player->get_predicted_health() + ( new_point.penetration_points.size() > 1 ? 5 : 0 );
	const auto shots_to_kill = static_cast< int >( ceilf( health / fminf( floorf( new_point.damage * ( doubletap_ready ? 2.f : 1.f ) ), health ) ) );

	if ( !ret_new_point || shots_to_kill > target_shots_to_kill )
		return;

	target.point = new_point;
}

bool aimbot_helpers::get_current_autostop()
{
	const auto weapon = local_weapon;
	if ( !weapon )
		return false;

	if ( !vars::aim.enabled->get<bool>() )
		return false;

	if ( globals::nospread )
		return false;

	const auto is_zeus = weapon->get_weapon_id() == WEAPON_TASER;
	const auto is_knife = !is_zeus && weapon->get_weapon_type() == WEAPONTYPE_KNIFE;
	const auto config = config::current_config( weapon );
	const auto in_air = !( local_player->get_flags() & FL_ONGROUND ) || !( prediction::unpred_flags & FL_ONGROUND ) && prediction::attempted_jump;

	if ( is_knife || is_zeus )
		return false;

	if ( !config->autostop->get<int>() )
		return false;

	if ( in_air && ( !config->scout_jumpshot->get<bool>() || local_player->get_velocity().Length2D() > 120.f ) )
		return false;

	return config->autostop->get<int>();
}

bool aimbot_helpers::should_autostop( const aimbot::final_target_t& final_target )
{
	const auto weapon = local_weapon;

	const auto is_zeus = weapon->get_weapon_id() == WEAPON_TASER;
	const auto is_knife = !is_zeus && weapon->get_weapon_type() == WEAPONTYPE_KNIFE;
	const auto config = config::current_config( weapon );

	if ( is_knife || is_zeus )
		return false;

	if ( !get_current_autostop() )
		return false;

	if ( final_target.point.damage >= final_target.player->get_predicted_health() || !config->autostop_lethal->get<bool>() )
		return true;

	return false;
}

float aimbot_helpers::get_final_hitchance( C_WeaponCSBaseGun* weapon, const int index )
{
	if ( weapon->get_weapon_id() == WEAPON_TASER )
		return vars::aim.zeus_hc->get<float>();

	const auto config = config::current_config( weapon );

	if ( config->hitchance->get<float>() < 1.f )
		return config->hitchance->get<float>();

	static constexpr auto shot_limit = 2;
	const auto& log = player_log::get_log( index );

	if ( log.m_shots_spread <= shot_limit )
		return config->hitchance->get<float>();

	auto hitchance_cfg = config->hitchance->get<float>();

	const auto diff = 100.f - config->hitchance->get<float>();

	hitchance_cfg += std::fmaxf( 5.f, ( log.m_shots_spread - shot_limit ) * ( diff / 6.f ) );
	hitchance_cfg = clamp( hitchance_cfg, 0.f, 100.f );

	return hitchance_cfg;
}

bool aimbot_helpers::check_max_hitchance( aimbot::final_target_t& final_target )
{
	const auto weapon = local_weapon;

	if ( globals::nospread )
		return true;

	if ( final_target.record->m_dormant )
		return true;

	const auto config = config::current_config( weapon );

	const auto inair = !( local_player->get_flags() & FL_ONGROUND );
	const auto is_zeus = weapon->is_zeus();

	if ( weapon->is_knife() )
		return true;

	if ( config->index == 1 && config->scout_jumpshot->get<bool>() && inair )
		if ( weapon->get_inaccuracy() < 0.009f )
			return true;

	const auto hitchance_cfg = is_zeus ? vars::aim.zeus_hc->get<float>() : get_final_hitchance( weapon, final_target.record->m_index );
	const auto hitchance = calc_penhc( &final_target, !final_target.record->m_dormant, true );

	final_target.point.hitchance = hitchance;

	if ( hitchance < std::min( hitchance_cfg, 83.f ) )
		return false;

	return true;
}

bool aimbot_helpers::check_hitchance( aimbot::final_target_t& final_target )
{
	const auto weapon = local_weapon;

	if ( globals::nospread )
		return true;

	const auto config = config::current_config( weapon );

	const auto inair = !( local_player->get_flags() & FL_ONGROUND );
	const auto is_zeus = weapon->is_zeus();

	if ( weapon->is_knife() )
		return true;

	const auto hitchance_cfg = is_zeus ? vars::aim.zeus_hc->get<float>() : get_final_hitchance( weapon, final_target.record->m_index );
	auto hitchance = calc_penhc( &final_target, !final_target.record->m_dormant );

	final_target.point.hitchance = hitchance;

	if ( config->index == 1 && config->scout_jumpshot->get<bool>() && inair )
	{
		if ( weapon->get_inaccuracy() < 0.009f )
			hitchance = 100.f;
	}

	if ( hitchance < hitchance_cfg && !( highest_accuracy( weapon ) && hitchance >= std::min( hitchance_cfg, 83.f ) ) )
		return false;

	return true;
}

void aimbot_helpers::slide_stop( CUserCmd* cmd )
{
	const auto weapon = local_weapon;

	if ( !weapon )
		return;

	cmd->sidemove = prediction::unpred_move.y;
	cmd->forwardmove = prediction::unpred_move.x;

	const auto is_revolver = weapon->get_weapon_id() == WEAPON_REVOLVER;

	// prevent move layer from reaching highest weight...
	if ( local_player->get_flags() & FL_ONGROUND && !( cmd->buttons & IN_JUMP ) && vars::aa.fake->get<bool>() && vars::aa.enabled->get<bool>()
	&& cmd->command_number > 1 && prediction::get_pred_info( cmd->command_number - 1 ).m_flags & FL_ONGROUND )
	{
		const auto jitter = fabs( sin( fmodf( interfaces::globals()->curtime, .4f ) / .2f * M_PI ) );
		const auto amnt = local_player->get_velocity().Length2D() / 25.f;

		stop_to_speed( std::max( ( is_revolver ? 180.f : weapon->get_max_speed() ) / 3.f - 0.1f - amnt * jitter, 0.f ), cmd );
	}
	else
		stop_to_speed( ( is_revolver ? 180.f : weapon->get_max_speed() ) / 3.f - 0.1f, cmd );
}

void aimbot_helpers::draw_debug_hitboxes( C_CSPlayer* player, matrix3x4_t* matrices, int hitbox, float duration, Color color )
{
	studiohdr_t* pStudioModel = player->get_model_ptr()->m_pStudioHdr;

	if ( !pStudioModel )
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet( player->get_hitbox_set() );

	if ( !pHitboxSet )
		return;

	for ( int i = 0; i < pHitboxSet->numhitboxes; i++ )
	{
		mstudiobbox_t* pHitbox = pHitboxSet->pHitbox( i );

		if ( !pHitbox )
			continue;

		if ( hitbox != -1 && hitbox != pHitbox->group )
			continue;

		matrix3x4_t temp{};
		math::angle_matrix( pHitbox->rotation, temp );
		auto out = math::concat_transforms( matrices[ pHitbox->bone ], temp );

		Vector vmin{}, vmax{};
		math::vector_transform( pHitbox->bbmin, out, vmin );
		math::vector_transform( pHitbox->bbmax, out, vmax );

		if ( pHitbox->radius > -1 )
		{
			interfaces::debug_overlay()->AddCapsuleOverlay( vmin, vmax, pHitbox->radius, color.r(), color.g(), color.b(), color.a(), duration, false, true );
		}
		else
		{
			Vector max = pHitbox->bbmax;
			Vector min = pHitbox->bbmin;

			Vector points[] = { Vector( min.x, min.y, min.z ),
				Vector( min.x, max.y, min.z ),
				Vector( max.x, max.y, min.z ),
				Vector( max.x, min.y, min.z ),
				Vector( max.x, max.y, max.z ),
				Vector( min.x, max.y, max.z ),
				Vector( min.x, min.y, max.z ),
				Vector( max.x, min.y, max.z ) };
			Vector pointsTransformed[ 8 ];
			for ( auto i = 0; i < 8; i++ ) //68, 75, 13, 41
			{
				points[ i ].x *= 0.9f;
				points[ i ].y *= 0.9f;
				points[ i ].z *= 0.9f;
				math::vector_transform( points[ i ], out, pointsTransformed[ i ] );
			}

			for ( int i = 0; i < 3; i++ )
			{
				interfaces::debug_overlay()->AddLineOverlayAlpha( pointsTransformed[ i ], pointsTransformed[ i + 1 ], color.r(), color.g(), color.b(), color.a(), true, duration );
			}
			interfaces::debug_overlay()->AddLineOverlayAlpha( pointsTransformed[ 0 ], pointsTransformed[ 3 ], color.r(), color.g(), color.b(), color.a(), true, duration );

			for ( int i = 4; i < 7; i++ )
			{
				interfaces::debug_overlay()->AddLineOverlayAlpha( pointsTransformed[ i ], pointsTransformed[ i + 1 ], color.r(), color.g(), color.b(), color.a(), true, duration );
			}
			interfaces::debug_overlay()->AddLineOverlayAlpha( pointsTransformed[ 4 ], pointsTransformed[ 7 ], color.r(), color.g(), color.b(), color.a(), true, duration );

			interfaces::debug_overlay()->AddLineOverlayAlpha( pointsTransformed[ 0 ], pointsTransformed[ 6 ], color.r(), color.g(), color.b(), color.a(), true, duration );

			interfaces::debug_overlay()->AddLineOverlayAlpha( pointsTransformed[ 1 ], pointsTransformed[ 5 ], color.r(), color.g(), color.b(), color.a(), true, duration );

			interfaces::debug_overlay()->AddLineOverlayAlpha( pointsTransformed[ 2 ], pointsTransformed[ 4 ], color.r(), color.g(), color.b(), color.a(), true, duration );

			interfaces::debug_overlay()->AddLineOverlayAlpha( pointsTransformed[ 3 ], pointsTransformed[ 7 ], color.r(), color.g(), color.b(), color.a(), true, duration );
		}
	}
}

void aimbot_helpers::remove_spread( CUserCmd* cmd )
{
	if ( !globals::nospread )
		return;

	const auto weapon = local_weapon;
	if ( !weapon )
		return;

	var( weapon_accuracy_nospread );
	if ( weapon_accuracy_nospread->get_bool() )
		return;

	globals::random_seed( ( cmd->random_seed & 255 ) + 1 );

	auto fRand1 = globals::random_float( 0.0f, 1.0f );
	const auto fRandPi1 = globals::random_float( 0.0f, 2.0f * M_PI );
	auto fRand2 = globals::random_float( 0.0f, 1.f );

	if ( weapon->get_weapon_id() == WEAPON_REVOLVER && cmd->buttons & IN_ATTACK2 && !( cmd->buttons & IN_ATTACK ) )
	{
		fRand1 = 1.f - fRand1 * fRand1;
	}
	else if ( weapon->get_weapon_id() == WEAPON_NEGEV && weapon->get_recoil_index() < 3.f )
	{
		for ( auto i = 3; i > weapon->get_recoil_index(); i-- )
		{
			fRand1 *= fRand1;
			fRand2 *= fRand2;
		}

		fRand1 = 1.f - fRand1;
	}

	const auto flRandInaccuracy = fRand1 * weapon->get_inaccuracy();

	const auto flSpreadX = cos( fRandPi1 ) * flRandInaccuracy;
	const auto flSpreadY = sin( fRandPi1 ) * flRandInaccuracy;

	cmd->viewangles.x += RAD2DEG( atan( sqrt( flSpreadX * flSpreadX + flSpreadY * flSpreadY ) ) );
	cmd->viewangles.z = -RAD2DEG( atan2f( flSpreadX, flSpreadY ) );
}

void aimbot_helpers::manage_lagcomp()
{
	_( jointeam, "jointeam 1" );
	_( jointeam_s, "jointeam " );
	_( on, "cl_lagcompensation 1" );
	_( off, "cl_lagcompensation 0" );
	_( extra, " 1" );

	var( cl_lagcompensation );
	//var( cl_predict );

	static auto previous_team = -1;
	static auto rtt_ticks = 14;

	const auto nci = interfaces::engine()->GetNetChannelInfo();
	const auto rtt = nci->GetLatency( FLOW_INCOMING ) + nci->GetLatency( FLOW_OUTGOING );

	  // switch us to spectator when we need a change.
	if ( interfaces::globals()->tickcount % std::max( 14, rtt_ticks ) == 0 )
	{
		if ( !vars::aim.anti_exploit->get<bool>() != ConVar::cl_lagcompensation )
		{
			if ( !local_player || local_player->get_team() != 1 )
			{
				previous_team = local_player->get_team();
				interfaces::engine()->ClientCmd( jointeam.c_str() );
			}
			else
			{
				cl_lagcompensation->enforce_sent_value( ConVar::cl_lagcompensation, !vars::aim.anti_exploit->get<bool>() );
				//cl_predict->enforce_sent_value( ConVar::cl_predict, !vars::aim.anti_exploit->get<bool>() );
			}
		}
		else
		{
			if ( previous_team != -1 )
				interfaces::engine()->ClientCmd( ( jointeam_s + std::to_string( previous_team ) + extra ).c_str() );

			previous_team = -1;

			rtt_ticks = time_to_ticks( rtt ) * 2;
		}
	}
}