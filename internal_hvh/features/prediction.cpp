#include "../include_cheat.h"
#include <intrin.h>

void prediction::start( int sequence_nr )
{
	auto& cur_info = pred_info[ sequence_nr % 150 ];

	if ( sequence_nr != cur_info.sequence )
	{
		cur_info.reset();
		cur_info.sequence = sequence_nr;
	}

	orig_curtime = interfaces::globals()->curtime;
	orig_frametime = interfaces::globals()->frametime;

	unpred_abs_origin = local_player->get_abs_origin();
	unpred_vel = local_player->get_velocity();
	unpred_flags = local_player->get_flags();
	unpred_move = { globals::current_cmd->forwardmove, globals::current_cmd->sidemove };

	predict_post_pone_time( globals::current_cmd );

	predict_can_fire( globals::current_cmd );

	run_prediction( sequence_nr );

	orig_predicted = interfaces::prediction()->get_predicted_commands();
	initial_vel = local_player->get_velocity();
	cur_info.movetype = local_player->get_move_type();
	cur_info.tick_base = local_player->get_tickbase();

	if ( const auto wpn = local_weapon )
		aimbot_helpers::autorevolver( wpn );
}

void prediction::finish( bool sendpacket )
{
	interfaces::globals()->curtime = orig_curtime;
	interfaces::globals()->frametime = orig_frametime;

	const auto frame = interfaces::client_state()->lastoutgoingcommand + interfaces::client_state()->chokedcommands + 1;
	auto last = interfaces::client_state()->last_command_ack + orig_predicted;

	interfaces::input()->m_pCommands[ frame % 150 ].hasbeenpredicted = false;

	if ( sendpacket && globals::shot_command > interfaces::client_state()->lastoutgoingcommand && globals::shot_command < frame && globals::shot_command == globals::choked_shot_command )
	{
		interfaces::input()->m_pCommands[ globals::shot_command % 150 ].hasbeenpredicted = false;
		last = globals::shot_command;
	}

	const auto weapon = local_weapon;
	if ( ( had_attack || had_secondary_attack ) && !has_shot_this_cycle && ( can_shoot || weapon && weapon->get_weapon_id() == WEAPON_REVOLVER ) )
	{
		last = std::min( last, globals::current_cmd->command_number );
		if ( had_attack )
			globals::current_cmd->buttons |= IN_ATTACK;
		if ( had_secondary_attack )
			globals::current_cmd->buttons |= IN_ATTACK2;
	}

	interfaces::prediction()->get_predicted_commands() = clamp( last - interfaces::client_state()->last_command_ack - 1, 0, interfaces::prediction()->get_predicted_commands() );
}

void prediction::run_prediction( int command_number )
{
	if ( interfaces::client_state()->m_nDeltaTick <= 0 )
		return;

	if ( !last_update_command_number || globals::current_cmd->command_number - last_update_command_number - 2 > 148 )
		return;

	const auto verified_cmd = &interfaces::input()->m_pVerifiedCommands[ command_number % 150 ];
	const auto checksum = verified_cmd->m_crc;
	const auto force_run = checksum == *reinterpret_cast< int32_t* >( &interfaces::globals()->interval_per_tick );

	if ( !force_run )
		misc::write_tick( command_number );

	const auto rerun = verified_cmd->m_crc != checksum || force_run;

	for ( auto i = interfaces::client_state()->lastoutgoingcommand + 1; i <= command_number; i++ )
		interfaces::input()->m_pCommands[ i % 150 ].hasbeenpredicted = true;

	if ( rerun )
		interfaces::prediction()->get_predicted_commands() = clamp( command_number - interfaces::client_state()->last_command_ack, 0, interfaces::prediction()->get_predicted_commands() );

	computing = true;
	interfaces::prediction()->m_flLastServerWorldTimeStamp = 0.f;
	interfaces::prediction()->Update( interfaces::client_state()->m_nDeltaTick, true, interfaces::client_state()->last_command_ack, command_number );
	computing = false;

	interfaces::globals()->curtime = tickbase::get_adjusted_time();
	interfaces::globals()->frametime = interfaces::globals()->interval_per_tick;

	const auto wpn = local_weapon;
	const auto player = local_player;
	pred_info[ command_number % 150 ].throwtime = wpn && wpn->is_grenade() ? wpn->get_throw_time() : 0.f;
	pred_info[ command_number % 150 ].m_flags = player->get_flags();

	player->get_collision_bounds_change_time() = pred_info[ command_number % 150 ].collision_bounds_change_time;
	player->get_collision_viewheight() = pred_info[ command_number % 150 ].collision_viewheight;
}

void prediction::set_last_command_number( int num )
{
	last_update_command_number = num;
}

void prediction::predict_post_pone_time( CUserCmd* cmd )
{
	const auto wpn = local_weapon;
	if ( !wpn || !last_update_command_number || globals::current_cmd->command_number - last_update_command_number - 2 > 148 )
		return;

	const auto last_cmd = &interfaces::input()->m_pCommands[ ( cmd->command_number - 1 ) % 150 ];
	run_prediction( cmd->command_number - 1 );
	const auto last_tick = local_player->get_tickbase() - 1;

	wpn->get_postpone_fire_ready_time() = get_pred_info( cmd->command_number - 1 ).postpone_fire_ready_time;

	if ( wpn->get_weapon_id() != WEAPON_REVOLVER || wpn->in_reload() ||
		local_player->get_next_attack() > ticks_to_time( last_tick ) ||
		wpn->get_next_primary_attack() > ticks_to_time( last_tick ) ||
		!( last_cmd->buttons & IN_ATTACK ) && !( last_cmd->buttons & IN_ATTACK2 ) )
	{
		wpn->get_postpone_fire_ready_time() = FLT_MAX;
		prone_delay = -1;
	}
	else if ( last_cmd->buttons & IN_ATTACK )
	{
		if ( prone_delay == -1 )
			prone_delay = last_tick + time_to_ticks( prone_time );

		if ( prone_delay <= last_tick )
			wpn->get_postpone_fire_ready_time() = ticks_to_time( prone_delay ) + post_delay;
	}

	get_pred_info( cmd->command_number ).postpone_fire_ready_time = wpn->get_postpone_fire_ready_time();
}

void prediction::predict_can_fire( CUserCmd* const cmd )
{
	if ( !interfaces::client_state()->chokedcommands )
		reset_shot();

	can_shoot = false;

	auto wpn = local_weapon;
	if ( !wpn || wpn->is_grenade() )
		return;

	const auto last_shot_time = wpn->get_last_shot_time();
	const auto next_seondary_attack = wpn->get_next_secondary_attack();
	const auto clip = wpn->get_clip1();
	had_attack = !!( cmd->buttons & IN_ATTACK );
	had_secondary_attack = wpn->is_secondary_attack_weapon() && !!( cmd->buttons & IN_ATTACK2 );
	if ( wpn->is_shootable() || wpn->is_knife() && vars::aim.knife_bot->get<bool>() || wpn->is_zeus() && vars::aim.zeus_bot->get<bool>() )
		cmd->buttons |= IN_ATTACK;

	run_prediction( cmd->command_number );

	const auto wpn2 = local_weapon;
	if ( wpn != wpn2 || ( wpn->is_shootable() || wpn->is_knife() && vars::aim.knife_bot->get<bool>() || wpn->is_zeus() && vars::aim.zeus_bot->get<bool>() ) )
	{
		cmd->buttons &= ~IN_ATTACK;
		if ( wpn->is_knife() )
			cmd->buttons &= ~IN_ATTACK2;
	}

	if ( wpn == wpn2 && ( wpn->is_shootable() && clip > 0 || wpn->is_secondary_attack_weapon() ) )
		can_shoot = last_shot_time != wpn->get_last_shot_time() || wpn->is_secondary_attack_weapon() && had_secondary_attack && wpn->get_next_secondary_attack() != next_seondary_attack;

	if ( has_shot_this_cycle && wpn == wpn2 && ( wpn->is_shootable() || wpn->is_secondary_attack_weapon() || wpn->is_zeus() ) )
		can_shoot = false;
}

prediction::pred_info_t& prediction::get_pred_info( const int cmd )
{
	return pred_info[ cmd % 150 ];
}

void prediction::clear_pred_info()
{
	for ( auto& elem : pred_info ) elem.reset();
}

void prediction::take_shot( const bool shot )
{
	had_attack = shot;
}

void prediction::take_secondary_shot( const bool shot )
{
	had_secondary_attack = shot;
}

bool prediction::has_shot()
{
	return can_shoot && ( had_attack || had_secondary_attack );
}

bool prediction::can_fire()
{
	return can_shoot && !has_shot_this_cycle;
}

void prediction::evaluate_shots()
{
	if ( can_shoot && ( had_attack || had_secondary_attack ) ) has_shot_this_cycle = true;
}

void prediction::reset_shot()
{
	has_shot_this_cycle = false;
}

void prediction::set_forced_shot()
{
	has_shot_this_cycle = true;
}