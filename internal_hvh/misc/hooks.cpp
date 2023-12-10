#include "../include_cheat.h"

namespace hook
{

	namespace proxy
	{
		__declspec( noinline ) ConVar* mat_force_tonemap_scale()
		{
			var( mat_force_tonemap_scale );
			return mat_force_tonemap_scale;
		}

		__declspec( noinline ) void* sv_cheats()
		{
			var( sv_cheats );
			return sv_cheats;
		}

		__declspec( noinline ) void* developer()
		{
			var( developer );
			return developer;
		}

		__declspec( noinline ) void* weapon_debug_spread_show()
		{
			var( weapon_debug_spread_show );
			return weapon_debug_spread_show;
		}

		__declspec( noinline ) void* sv_party_mode()
		{
			var( sv_party_mode );
			return sv_party_mode;
		}

		__declspec( noinline ) void* studio_render()
		{
			return interfaces::studio_render()( );
		}

		__declspec( noinline ) void* engine()
		{
			return interfaces::engine()( );
		}

		__declspec( noinline ) uintptr_t getmodulehandleexa()
		{
			return memory::get_proc_address( memory::get_module_handle( FNV1A( "kernel32.dll" ) ), FNV1A( "GetModuleHandleExA" ) );
		}
	}

#pragma clang section text=".discard"
	__declspec( noinline ) void init()
	{
		VIRTUALIZER_LION_RED_START;

		// vac
		make_hook_direct( get_module_handle_ex_a, proxy::getmodulehandleexa() );

		// functions
		make_hook( ent_info_list_link_before, "client.dll", hook_ent_info_list_link_before );
		make_hook( start_sound_immediate, "engine.dll", hook_start_sound_immediate );
		make_hook( set_bodygroup, "client.dll", hook_set_bodygroup );
		make_hook( modify_eye_pos, "client.dll", hook_modify_eye_pos );
		make_hook( wnd_proc, "inputsystem.dll", hook_wnd_proc );
		make_hook( create_move, "client.dll", hook_create_move );
		make_hook( cl_move_naked, "engine.dll", hook_cl_move );
		make_hook( frame_stage_notify, "client.dll", hook_frame_stage_notify );
		make_hook( on_latch_interp_var, "client.dll", hook_on_latch_interp_var );
		make_hook( draw_model_execute, "engine.dll", hook_draw_model_execute );
		make_hook( render_view, "client.dll", hook_render_view );
		make_hook( level_init_post_entity, "client.dll", hook_level_init_post_entity );
		make_hook( level_init_pre_entity, "client.dll", hook_level_init_pre_entity );
		make_hook( draw_3d_debug_overlays, "engine.dll", hook_draw_3d_debug_overlays );
		//make_hook_dbg( render_glow_boxes, sig( "client.dll", "53 8B DC 83 EC ? 83 E4 ? 83 C4 ? 55 8B 6B ? 89 6C ? ? 8B EC 83 EC ? 56 57 8B F9 89 7D ? 8B 4F" ) );
		make_hook( shader_api_draw_mesh, "shaderapidx9.dll", hook_shader_api_draw_mesh );
		make_hook( is_box_visible, "engine.dll", hook_is_box_visible );
		make_hook( scene_end, "engine.dll", hook_scene_end );
		make_hook( calc_renderable_world_space_aabb_bloated, "client.dll", hook_calc_renderable_world_space_aabb_bloated );
		make_hook( add_renderables_to_render_lists, "client.dll", hook_add_renderables_to_render_lists );
		make_hook( do_post_screen_space_effects, "client.dll", hook_do_post_screen_space_effects );
		make_hook( obb_change_callback, "client.dll", hook_obb_change_callback );
		make_hook( server_cmd_key_values, "engine.dll", hook_server_cmd_key_values );
		make_hook( set_abs_angles, "client.dll", sig_set_abs_angles );
		make_hook_direct( draw_model_studio, ( *reinterpret_cast< uintptr_t** >( proxy::studio_render() ) )[ 29 ] );
		make_hook( present, "gameoverlayrenderer.dll", hook_present );
		make_hook( reset, "gameoverlayrenderer.dll", hook_reset );
		make_hook( send_net_msg, "engine.dll", hook_send_net_msg );
		make_hook( override_view, "client.dll", hook_override_view );
		make_hook( paint_traverse, "vgui2.dll", hook_paint_traverse );
		make_hook( find_material, "materialsystem.dll", hook_find_material );
		make_hook( emit_sound, "engine.dll", hook_emit_sound );
		make_hook( is_connected, "engine.dll", hook_is_connected );
		make_hook( lock_cursor, "vguimatsurface.dll", hook_lock_cursor );
		make_hook( draw_set_color, "vguimatsurface.dll", hook_draw_set_color );
		make_hook( setup_bones, "client.dll", hook_setup_bones );
		make_hook( animating_setup_bones, "client.dll", hook_animating_setup_bones );
		make_hook( reevauluate_anim_lod, "client.dll", hook_reevauluate_anim_lod );
		make_hook( post_data_update, "client.dll", hook_post_data_update );
		make_hook( maintain_sequence_transitions, "client.dll", hook_maintain_sequence_transitions );
		make_hook( trace_ray, "engine.dll", hook_trace_ray );
		make_hook( run_command, "client.dll", hook_run_command );
		make_hook( process_movement, "client.dll", hook_process_movement );
		make_hook( transfer_data, "client.dll", hook_transfer_data );
		make_hook( perform_prediction, "client.dll", hook_perform_prediction );
		make_hook( packet_start, "engine.dll", hook_packet_start );
		make_hook( level_shutdown, "client.dll", hook_level_shutdown );
		make_hook( physics_simulate, "client.dll", hook_physics_simulate );
		make_hook( estimate_abs_velocity, "client.dll", hook_estimate_abs_velocity );
		make_hook( send_weapon_anim, "client.dll", hook_send_weapon_anim );
		make_hook( fire_event, "client.dll", hook_fire_event );
		make_hook( do_animation_events, "client.dll", hook_do_animation_events );
		make_hook( do_animation_events_animating, "client.dll", hook_do_animation_events_animating );
		make_hook( deploy, "client.dll", hook_deploy );
		make_hook( get_fov, "client.dll", hook_get_fov );
		make_hook( get_default_fov, "client.dll", hook_get_default_fov );
		make_hook( calc_view, "client.dll", hook_calc_view );
		make_hook( scale_mouse, "client.dll", hook_scale_mouse );
		make_hook( start_sound, "client.dll", hook_start_sound );
		make_hook( convar_network_change_callback, "engine.dll", hook_convar_network_change_callback );
		make_hook( update_clientside_anim, "client.dll", hook_update_clientside_anim );
		make_hook( extract_occluded_renderables, "client.dll", hook_extract_occluded_renderables );
		make_hook( SVCMsg_VoiceData, "engine.dll", hook_sv_msg_voicedata );
		make_hook( hud_reticle_process_input, "client.dll", hook_hud_reticle_process_input );
		make_hook( update_interp_vars, "client.dll", hook_update_interp_vars );
		make_hook( particle_draw_model, "client.dll", hook_particle_draw_model );

		// netprops
		make_hook( effects, "client.dll", hook_effects );
		make_hook( sequence, "client.dll", hook_sequence );
		make_hook( weapon_handle, "client.dll", hook_weapon_handle );
		make_hook( simulation_time, "client.dll", hook_simulation_time );
		make_hook( general_float, "client.dll", hook_general_float );
		make_hook( general_vec, "client.dll", hook_general_vec );
		make_hook( general_int, "client.dll", hook_general_int );
		make_hook( general_bool, "client.dll", hook_general_bool );
		make_hook( layer_sequence, "client.dll", hook_layer_sequence );
		make_hook( layer_cycle, "client.dll", hook_layer_cycle );
		make_hook( layer_playback_rate, "client.dll", hook_layer_playback_rate );
		make_hook( layer_weight, "client.dll", hook_layer_weight );
		make_hook( layer_weight_delta_rate, "client.dll", hook_layer_weight_delta_rate );
		make_hook( layer_order, "client.dll", hook_layer_order );

		// convars
		make_hook_virt( convar_get_float_engine, 12, proxy::mat_force_tonemap_scale() );
		make_hook_virt( convar_get_int_engine, 13, proxy::sv_cheats() );
		make_hook_virt( convar_get_int_engine, 13, proxy::developer() );
		make_hook_virt( convar_get_int_client, 13, proxy::weapon_debug_spread_show() );
		make_hook_virt( convar_get_int_client, 13, proxy::sv_party_mode() );

		// lua
		make_hook_direct( client_cmd_unrestricted, ( *reinterpret_cast< uintptr_t** >( proxy::engine() ) )[ 114 ] );

#ifdef _DEBUG
		// debug
		make_hook_direct( run_command_server, sig( "server.dll", "55 8B EC 83 E4 F8 83 EC 2C 53 56 8B 75 08 8B D9 57" ) );
#endif

#ifndef RELEASE
#endif

#ifndef RELEASE
		auto index = 0;
#endif
		for ( auto& item : hook_list )
		{
			item.second.apply();
#ifndef RELEASE
			index++;
#endif
		}

		VIRTUALIZER_LION_RED_END;

		erase_end;
	}

#ifndef RELEASE
	void unhook()
	{
		for ( auto& item : hook_list )
		{
			item.second.hook_.reset();
		}
	}
#endif
#pragma clang section text=""
}