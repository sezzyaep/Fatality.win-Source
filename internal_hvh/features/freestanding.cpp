#include "../include_cheat.h"

void freestanding::update_hotkeys( ClientFrameStage_t stage )
{
	if ( stage != FRAME_RENDER_START || !local_player || !local_player->get_alive() )
		return;

	if ( !vars::aa.manual->get<bool>() )
	{
		direction = -1;
		return;
	}

	auto current = -1;

	static auto last_left = false;
	static auto last_right = false;
	static auto last_back = false;

	if ( vars::aa.manual_left->get<bool>() != last_left )
	{
		vars::aa.manual_right->set<bool>( false );
		vars::aa.manual_back->set<bool>( false );
	}
	else if ( vars::aa.manual_right->get<bool>() != last_right )
	{
		vars::aa.manual_left->set<bool>( false );
		vars::aa.manual_back->set<bool>( false );
	}
	else if ( vars::aa.manual_back->get<bool>() != last_back )
	{
		vars::aa.manual_right->set<bool>( false );
		vars::aa.manual_left->set<bool>( false );
	}

	last_left = vars::aa.manual_left->get<bool>();
	last_right = vars::aa.manual_right->get<bool>();
	last_back = vars::aa.manual_back->get<bool>();

	if ( vars::aa.manual_left->get<bool>() )
		current = 1;
	if ( vars::aa.manual_right->get<bool>() )
		current = 2;
	if ( vars::aa.manual_back->get<bool>() )
		current = 0;

	direction = current;
}

void freestanding::do_jitter( float& yaw )
{
	auto jitter_range = vars::aa.jitter_range->get<int>() * 0.5f;

	static auto flip = false;
	static auto add = 0.f;

	if ( vars::aa.jitter_random->get<int>() )
	{
		globals::random_seed( globals::current_cmd->command_number % 255 );
		jitter_range = globals::random_float( -jitter_range, jitter_range );
		flip = true;
	}

	add = flip ? jitter_range : -jitter_range;
	flip = !flip;
	yaw += add;
};

bool freestanding::override( float& yaw )
{
	if ( !vars::aa.manual->get<bool>() )
		return false;

	QAngle angles{};
	interfaces::engine()->GetViewAngles( angles );

	switch ( direction )
	{
		case 0:
		{
			antiaim::real_direction = 0.f;
			yaw = math::normalize_float( angles.y + 180.f );
			break;
		}
		case 1:
		{
			antiaim::real_direction = -90.f;
			yaw = math::normalize_float( angles.y + 90.f );
			break;
		}
		case 2:
		{
			antiaim::real_direction = 90.f;
			yaw = math::normalize_float( angles.y - 90.f );
			break;
		}
		default: return false;
	}

	if ( vars::aa.jitter->get<bool>() )
		do_jitter( yaw );

	return true;
}

bool freestanding::get_real( float& yaw )
{
	antiaim::real_direction = 0.f;

	get_targets();

	if ( players.empty() )
		return override( yaw );

	const auto ret = freestand( yaw );

	if ( override( yaw ) )
		return true;

	return ret;
}

bool IsVisible( Vector start, Vector end, C_CSPlayer* filter, C_CSPlayer* ent )
{
	trace_t trace;
	globals::util_trace_line( start, end, MASK_SHOT_HULL, filter, &trace );

	return trace.m_pEnt == ent;
}

void freestanding::get_targets()
{
	players.clear();

	QAngle viewangles{};
	interfaces::engine()->GetViewAngles( viewangles );

	auto needed_fov = FLT_MAX;
	for ( const auto ent : interfaces::entity_list()->get_players() )
	{
		if ( ent == local_player || !ent->get_alive() || !ent->is_enemy() )
			continue;

		auto& log = player_log::get_log( ent->EntIndex() );
		if ( log.record.empty() )
			continue;

		if ( ent->IsDormant() && !( interfaces::client_state()->get_last_server_tick() >= log.last_server_tick && interfaces::client_state()->get_last_server_tick() < log.last_server_tick + time_to_ticks( 5.f ) ) )
			continue;

		const auto fov = math::get_fov( viewangles, math::calc_angle( local_player->get_eye_pos(), sound_esp::get_position( ent->EntIndex() ) + Vector{ 0.f, 0.f, 64.f } ) );
		if ( fov < needed_fov || fov < 18.f )
		{
			if ( needed_fov >= 18.f && fov < 18.f || fov >= 18.f )
				players.clear();
			needed_fov = fov;
			players.push_back( ent );
		}
	}

	//if ( players.empty() )
	//	return;

	//C_CSPlayer* target = nullptr;
	//auto best_dist = FLT_MAX;
	//for ( auto& player : players )
	//{
	//	const auto dist = player->get_origin().Dist( local_player->get_origin() );
	//	if ( dist < best_dist )
	//	{
	//		best_dist = dist;
	//		target = player;
	//	}
	//}

	//players.clear();
	//players.push_back( target );
}

bool freestanding::freestand( float& yaw )
{
	if ( !vars::aa.freestand->get<bool>() && !vars::aa.freestand_fake->get<int>() && vars::aa.lean_type->get<int>() != 3 && vars::aa.lean_type->get<int>() != 4 )
		return false;

	static auto get_rotated_pos = [] ( Vector start, float rotation, float distance )
	{
		const auto rad = DEG2RAD( rotation );
		start.x += cos( rad ) * distance;
		start.y += sin( rad ) * distance;

		return start;
	};

	antiaim::real_direction = antiaim::fake_direction = 0.f;
	QAngle direction_at_target;
	auto has_direction = false;

	for ( auto target : players )
	{
		const auto eye_pos = local_player->get_eye_pos();
		const auto target_position = sound_esp::get_position( target->EntIndex() ) + Vector( 0.f, 0.f, 66.f );
		const auto target_angle = math::calc_angle( eye_pos, target_position );

		const auto weapon_info = interfaces::weapon_system()->GetWpnData( WEAPON_AWP );

		Vector forward{};
		math::angle_vectors( target_angle, &forward );

		const auto local_pos_left = get_rotated_pos( eye_pos, math::normalize_float( target_angle.y - 90.f ), 25.f );
		const auto local_pos_right = get_rotated_pos( eye_pos, math::normalize_float( target_angle.y + 90.f ), 25.f );

		const auto local_half_pos_left = get_rotated_pos( eye_pos, math::normalize_float( target_angle.y - 90.f ), 12.f );
		const auto local_half_pos_right = get_rotated_pos( eye_pos, math::normalize_float( target_angle.y + 90.f ), 12.f );

		const auto enemy_pos_left = get_rotated_pos( target_position, math::normalize_float( target_angle.y - 90.f ), 25.f );
		const auto enemy_pos_right = get_rotated_pos( target_position, math::normalize_float( target_angle.y + 90.f ), 25.f );

		const auto compare = [&weapon_info, &target] ( const Vector& from_left, const Vector& from_right, const Vector& left, const Vector& right, const bool check = false, const bool check2 = false ) -> int
		{
			auto pen_weapon = *weapon_info;
			if ( check )
				pen_weapon.idamage = 200;

			aimbot::aimpoint_t aimpoint_left{};
			aimpoint_left.point = left;
			can_hit( target, penetration::pen_data( {}, {}, {}, {}, &pen_weapon ), from_left, &aimpoint_left, aimpoint_left.damage );

			if ( check && aimpoint_left.damage > 0 && !aimpoint_left.penetration_points.empty() && aimpoint_left.penetration_points.front().Dist( from_left ) < aimpoint_left.penetration_points.front().Dist( left ) )
				return 1;

			if ( check2 )
				return aimpoint_left.damage > 0;

			aimbot::aimpoint_t aimpoint_right{};
			aimpoint_right.point = right;
			can_hit( target, penetration::pen_data( {}, {}, {}, {}, &pen_weapon ), from_right, &aimpoint_right, aimpoint_right.damage );

			if ( check )
			{
				if ( aimpoint_right.damage > 0 && !aimpoint_right.penetration_points.empty() && aimpoint_right.penetration_points.front().Dist( from_right ) < aimpoint_right.penetration_points.front().Dist( right ) )
					return 1;
			}
			else
			{
				if ( !aimpoint_left.damage && aimpoint_right.damage > 1 )
					return 1;

				if ( !aimpoint_right.damage && aimpoint_left.damage > 1 )
					return 2;

				if ( aimpoint_right.damage > 1 && aimpoint_left.damage > 1 )
					return 3;
			}

			return 0;
		};

		if ( !has_direction )
		{
			if ( const auto res = compare( target_position, target_position, local_pos_left, local_pos_right ) )
			{
				if ( res == 3 )
					antiaim::real_direction = 0.f;
				else
				{
					direction_at_target = target_angle;
					antiaim::real_direction = res == 1 ? -90.f : 90.f;
				}
					

				has_direction = true;
			}
			else if ( const auto res = compare( enemy_pos_left, enemy_pos_right, local_pos_left, local_pos_right ) )
			{
				if ( res == 3 )
					antiaim::real_direction = 0.f;
				else
				{
					direction_at_target = target_angle;
					antiaim::real_direction = res == 1 ? -90.f : 90.f;
				}
					

				has_direction = true;
			}
		}

		if ( antiaim::real_direction != 0.f )
		{
			antiaim::fake_direction = antiaim::real_direction;
		}

		if ( antiaim::real_direction != 0.f && compare( target_position, {}, antiaim::real_direction < 0.f ? local_half_pos_left : local_half_pos_right, {}, false, true ) )
		{
			antiaim::real_direction = 0.f;
			has_direction = true;
		}

		if ( antiaim::real_direction != 0.f && compare( antiaim::real_direction < 0.f ? enemy_pos_right : enemy_pos_left, {}, antiaim::real_direction < 0.f ? local_half_pos_left : local_half_pos_right, {}, false, true ) )
		{
			antiaim::real_direction = 0.f;
			has_direction = true;
		}

		if ( antiaim::real_direction != 0.f && compare( enemy_pos_left, enemy_pos_right, local_half_pos_right, local_half_pos_left, true ) )
		{
			antiaim::real_direction = 0.f;
			has_direction = true;
		}
	}


	if ( !vars::aa.freestand->get<bool>() )
		antiaim::real_direction = 0.f;

	if ( antiaim::real_direction != 0.f )
		yaw = math::normalize_float( direction_at_target.y + antiaim::real_direction );

	if ( antiaim::real_direction == antiaim::fake_direction && antiaim::fake_direction != 0.f )
		antiaim::fake_direction = math::normalize_float( direction_at_target.y - 180.f );
	else if ( antiaim::fake_direction != 0.f )
		antiaim::fake_direction = math::normalize_float( direction_at_target.y + antiaim::fake_direction );

	if ( vars::aa.jitter->get<bool>() && antiaim::real_direction != 0.f )
		do_jitter( yaw );

	return antiaim::real_direction != 0.f;
}
