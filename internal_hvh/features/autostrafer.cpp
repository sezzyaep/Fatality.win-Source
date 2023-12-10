#include "../include_cheat.h"

void autostrafer::strafe()
{
	static auto last_pressed = 0;
	static auto last_buttons = 0;

	const auto check_button = [&] ( const int button )
	{
		if ( globals::current_cmd->buttons & button && ( !( last_buttons & button )
			|| button & IN_MOVELEFT && !( last_pressed & IN_MOVERIGHT )
			|| button & IN_MOVERIGHT && !( last_pressed & IN_MOVELEFT )
			|| button & IN_FORWARD && !( last_pressed & IN_BACK )
			|| button & IN_BACK && !( last_pressed & IN_FORWARD ) ) )
		{
			if ( vars::misc.autostrafe_easy_strafe->get<bool>() )
			{
				if ( button & IN_MOVELEFT )
					last_pressed &= ~IN_MOVERIGHT;
				else if ( button & IN_MOVERIGHT )
					last_pressed &= ~IN_MOVELEFT;
				else if ( button & IN_FORWARD )
					last_pressed &= ~IN_BACK;
				else if ( button & IN_BACK )
					last_pressed &= ~IN_FORWARD;
			}

			last_pressed |= button;
		}
		else if ( !( globals::current_cmd->buttons & button ) )
			last_pressed &= ~button;
	};

	check_button( IN_MOVELEFT );
	check_button( IN_MOVERIGHT );
	check_button( IN_FORWARD );
	check_button( IN_BACK );

	last_buttons = globals::current_cmd->buttons;

	if ( !local_player->get_alive() || local_player->get_water_level() >= 2 || local_player->get_move_type() == MOVETYPE_NOCLIP || local_player->get_move_type() == MOVETYPE_LADDER )
		return;

	if ( !( globals::current_cmd->buttons & IN_JUMP ) && !vars::misc.autostrafe_jump_release->get<bool>() || globals::current_cmd->buttons & IN_SPEED )
		return;

	if ( !vars::misc.autostrafe_movement_keys->get<bool>() && !vars::misc.autostrafe_viewangles->get<bool>() )
		return;

	const auto weapon = local_weapon;
	const auto js = weapon && ( config::current_config( weapon )->index == 1 && config::current_config( weapon )->scout_jumpshot->get<bool>() && local_player->get_velocity().Length2D() < 50.f );
	const auto throwing_nade = weapon && weapon->is_grenade() && ticks_to_time( local_player->get_tickbase() ) > prediction::get_pred_info( globals::current_cmd->command_number - 1 ).throwtime && prediction::get_pred_info( globals::current_cmd->command_number - 1 ).throwtime > 0.f;

	if ( js || throwing_nade )
		return;

	auto mv = interfaces::game_movement()->setup_move( local_player, globals::current_cmd );

	const auto backup_frametime = interfaces::globals()->frametime;
	interfaces::globals()->frametime = interfaces::globals()->interval_per_tick;
	const auto data = interfaces::game_movement()->process_movement( local_player, &mv );
	interfaces::globals()->frametime = backup_frametime;

	if ( data.flags & FL_ONGROUND )
		return;

	auto yaw = math::normalize_float( globals::current_cmd->viewangles.y );

	if ( vars::misc.autostrafe_movement_keys->get<bool>() )
	{
		auto offset = 0.f;
		if ( last_pressed & IN_MOVELEFT )
			offset += 90.f;
		if ( last_pressed & IN_MOVERIGHT )
			offset -= 90.f;
		if ( last_pressed & IN_FORWARD )
			offset *= 0.5f;
		else if ( last_pressed & IN_BACK )
			offset = ( -offset * 0.5f ) + 180.f;

		yaw += offset;

		globals::current_cmd->forwardmove = 0.f;
		globals::current_cmd->sidemove = 0.f;

		if ( !vars::misc.autostrafe_viewangles->get<bool>() && offset == 0.f )
			return;
	}

	if ( globals::current_cmd->sidemove != 0.0f || globals::current_cmd->forwardmove != 0.0f )
		return;

	auto calc = [&] ( const float target_yaw )
	{
		auto cmd_copy = *globals::current_cmd;
		cmd_copy.viewangles.y = target_yaw;
		cmd_copy.forwardmove = 450.f;
		cmd_copy.sidemove = 0.f;

		auto mv = interfaces::game_movement()->setup_move( local_player, &cmd_copy );
		const auto player = local_player;

		CGameMovement::check_parameters( &mv, player );
		CGameMovement::air_move( &mv, player );

		return mv.m_vecVelocity.Length2D();

	};

	auto consider_accel = [&] ( float yaw, float& ref, float& res )
	{
		const auto cmp = calc( math::normalize_float( yaw ) );
		if ( cmp >= ref )
		{
			ref = cmp;
			res = math::normalize_float( yaw );
		}
	};

	const auto velocity = local_player->get_velocity();
	auto velocity_angle = RAD2DEG( std::atan2f( velocity.y, velocity.x ) );
	if ( velocity_angle < 0.0f )
		velocity_angle += 360.0f;

	if ( velocity_angle < 0.0f )
		velocity_angle += 360.0f;

	velocity_angle -= floorf( velocity_angle / 360.0f + 0.5f ) * 360.0f;

	auto res = 0.f;
	auto ref = -1.f;
	for ( auto i = 0; i <= 110.f; i++ )
		consider_accel( velocity_angle + i, ref, res );

	const auto speed = velocity.Length2D();
	const auto ideal = math::normalize_float( res - velocity_angle );

	auto correct = ideal;

	bool did_avoid = false;
	if ( vars::misc.avoid_walls->get<bool>() )
	{
		const auto player = local_player;
		const auto pos = player->get_origin();
		const auto collideable = player->GetCollideable();

		Vector forward{};
		math::angle_vectors( { 0.f, yaw, 0.f }, &forward );
		auto trace_to = pos + forward * speed * interfaces::globals()->interval_per_tick * 5.f;

		Ray_t ray;
		trace_t pm;
		ray.Init( pos, trace_to, collideable->OBBMins(), collideable->OBBMaxs() );
		CTraceFilterSimpleNoPlayers filter( nullptr, COLLISION_GROUP_PLAYER_MOVEMENT );
		interfaces::engine_trace()->TraceRay( ray, MASK_PLAYERSOLID, &filter, &pm );

		if ( pm.fraction < 1.f && fabs( pm.plane.normal.z ) < 0.0001f )
		{
			constexpr auto clip_velocity = [] ( Vector in, Vector& normal, Vector& out )
			{
				const auto backoff = math::dot_product( in, normal );
				for ( auto i = 0; i < 3; i++ )
					out[ i ] = in[ i ] - normal[ i ] * backoff;
				// iterate once to make sure we aren't still moving through the plane
				auto adjust = math::dot_product( out, normal );
				if ( adjust < 0.0f )
				{
					adjust = std::min( adjust, -0.03125f );
					out -= ( normal * adjust );
				}
			};

			Vector vel_out{};
			clip_velocity( pos + forward, pm.plane.normal, vel_out );

			QAngle out_ang{};
			math::vector_angles( vel_out, out_ang );
			const auto wall_hit_angle = fabsf( math::normalize_float( out_ang.y - yaw ) );

			if ( fabsf( math::normalize_float( wall_hit_angle - 90.f ) ) > 15.f )
			{
				if ( wall_hit_angle > 90.f )
					out_ang.y += 180.f;

				ray.Init( pm.endpos + forward * 250.f * interfaces::globals()->interval_per_tick, Vector( pm.endpos.x, pm.endpos.y, pm.endpos.z + local_player->get_step_size() + 0.03125f ) + forward * 250.f * interfaces::globals()->interval_per_tick, collideable->OBBMins(), collideable->OBBMaxs() );
				interfaces::engine_trace()->TraceRay( ray, MASK_PLAYERSOLID, &filter, &pm );

				// Only step up as high as we have headroom to do so.	
				if ( !pm.startsolid || pm.allsolid )
				{
					did_avoid = true;
					yaw = math::normalize_float( out_ang.y ) + math::normalize_float( out_ang.y - velocity_angle ) * 0.5f;
				}
			}
		}
	}

	const auto velocity_delta = math::normalize_float( yaw - velocity_angle );
	correct += fabsf( velocity_delta ) * 0.3f * ( 100.f - ( !did_avoid ? vars::misc.autostrafe_smoothing->get<float>() : 50 ) ) * 0.01f;

	if ( fabsf( velocity_delta ) > 170.f && speed > 80.f && !did_avoid || velocity_delta > 0.f )
		yaw = velocity_angle + correct;
	else if ( velocity_delta <= 0.f )
		yaw = velocity_angle - correct;

	auto& wish_angle = antiaim::wish_angles[ globals::current_cmd->command_number % 150 ];
	wish_angle.y = math::normalize_float( yaw );
	globals::current_cmd->sidemove = 0.f;
	globals::current_cmd->forwardmove = 450.f;

	antiaim::fix_movement( globals::current_cmd );
}