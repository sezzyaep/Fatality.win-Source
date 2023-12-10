#include "../include_cheat.h"

void __fastcall hook::physics_simulate( C_CSPlayer* const player, const uint32_t edx )
{
	auto& context = player->get_command_context();
	if ( context.cmd.tick_count == INT_MAX )
	{
		const auto backup_animations = animations::animation_copy( 0 );

		auto& current = prediction::get_pred_info( context.cmd_number ).animation;
		const auto& before = prediction::get_pred_info( context.cmd_number - 1 ).animation;
		before.restore( player );
		current = context.cmd_number;
		current.sequence = context.cmd_number;
		current.addon.vm = 0;

		backup_animations.restore( player );
		return;
	}

	const auto needs_processing = context.needs_processing
		&& player->get_simulation_tick() != interfaces::globals()->tickcount && player->get_alive() && player->get_model_ptr();

	const auto viewmodel = reinterpret_cast< C_BaseViewModel* >( interfaces::entity_list()->get_client_entity_from_handle( player->get_view_model() ) );
	const auto view_time = viewmodel ? viewmodel->get_anim_time() : 0.f;
	const auto fov_time = player->get_fov_time();

	original( physics_simulate )( player, edx );

	if ( viewmodel && viewmodel->get_anim_time() != view_time )
		viewmodel->get_anim_time() = ticks_to_time( context.cmd_number );

	if ( player->get_fov_time() != fov_time )
		player->get_fov_time() = ticks_to_time( context.cmd_number );

	var( weapon_recoil_scale );
	player->get_tp_recoil() = player->get_aim_punch().x * weapon_recoil_scale->get_float();

	if ( needs_processing )
	{
		const auto fresh = interfaces::input()->m_pVerifiedCommands[ context.cmd.command_number % 150 ].m_crc ==
			*reinterpret_cast< int32_t* >( &interfaces::globals()->interval_per_tick );

		if ( fresh && context.cmd.buttons & ( IN_ATTACK | IN_ATTACK2 | IN_RELOAD ) )
			player->is_looking_at_weapon() = player->is_holding_looking_at_weapon() = false;

		const auto backup_animations = animations::animation_copy( 0 );

		const auto sequence = player->get_sequence();
		const auto cycle = player->get_cycle();
		const auto playback_rate = player->get_playback_rate();
		auto eye_ang = *player->eye_angles_virtual();
		interfaces::prediction()->SetLocalViewAngles( context.cmd.viewangles );
		animations::merge_local_animation( &context.cmd, player );
		interfaces::prediction()->SetLocalViewAngles( eye_ang );
		player->get_sequence() = sequence;
		player->get_cycle() = cycle;
		player->get_playback_rate() = playback_rate;

		if ( !fresh )
			backup_animations.restore( player );
		else
			player->get_blend_tick() = 0;

		if ( fresh )
			original( do_animation_events )( player, 0, player->get_model_ptr() );

		const auto wpn = globals::get_weapon( player->get_active_weapon() );
		const auto world_model = wpn ? reinterpret_cast< C_BaseWeaponWorldModel* >( interfaces::entity_list()->get_client_entity_from_handle( wpn->get_world_model_handle() ) ) : nullptr;

		const auto& info = prediction::get_pred_info( context.cmd.command_number - 1 );
		if ( world_model && info.sequence == context.cmd.command_number - 1 && info.ev == 52 )
			world_model->get_effects() &= ~EF_NODRAW;

		// failsafe to show active world model if it fails to unhide by the time deploy is complete
		if ( wpn && world_model )
		{
			const auto act = wpn->get_sequence_act( wpn->get_sequence() );
			if ( act != 183 && act != 224 )
				world_model->get_effects() &= ~EF_NODRAW;
		}

		if ( fresh )
			animations::update_addon_bits( player );
	}
}