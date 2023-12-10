#include "../include_cheat.h"
#include <intrin.h>

bool __fastcall hook::animating_setup_bones( void* ecx, void* edx, matrix3x4_t* out, int max_bones, int mask, float time )
{
	if ( interfaces::prediction()->InPrediction() || globals::in_cm || CGameMovement::computing )
	{
		if ( out )
		{
			const auto animating = reinterpret_cast< C_BaseAnimating* const >( reinterpret_cast< uintptr_t >( ecx ) - 4 );
			if ( !animating->get_bone_accessor()->m_pBones || !animating->get_model_ptr() )
				return false;

			memcpy( out, animating->get_bone_accessor()->m_pBones, std::min( max_bones, animating->get_model_ptr()->numbones() ) * sizeof( matrix3x4_t ) );
		}

		return true;
	}

	return original( animating_setup_bones )( ecx, edx, out, max_bones, mask, time );
}

bool __fastcall hook::setup_bones( void* ecx, void* edx, matrix3x4_t* out, int max_bones, int mask, float time )
{
	const auto player = reinterpret_cast< C_CSPlayer* const >( reinterpret_cast< uintptr_t >( ecx ) - 4 );

	if ( interfaces::prediction()->InPrediction() || util::get_ebp()[ 1 ] == make_offset( "client.dll", sig_ret_hitbox_to_world_transforms ) )
		return true;

	mask |= BONE_USED_BY_ANYTHING;

	player->get_lod_data().flags = 0;
	player->get_lod_data().frame = 0;

	original( reevauluate_anim_lod )( player, 0, mask );

	const auto flags = player->get_lod_data().flags;
	player->get_lod_data().flags |= 0x04; //ANIMLODFLAG_INVISIBLE

	if ( vars::legit_enabled() && player->EntIndex() != interfaces::engine()->GetLocalPlayer() )
		return original( setup_bones )( ecx, edx, out, max_bones, mask, time );

	const auto backup_viewoffset = player->get_view_offset();
	const auto backup_maxs = player->GetCollideable()->OBBMaxs();
	const auto backup_groundentity = player->get_ground_entity();
	const auto backup_collision_viewheight = player->get_collision_viewheight();
	const auto backup_collision_bounds_change_time = player->get_collision_bounds_change_time();
	const auto backup_eye_angles = player->get_eye_angles();

	auto& log = player_log::get_log( player->EntIndex() );
	if ( !player->get_predictable() )
	{
		if ( const auto eye_watcher = player->get_var_mapping().find( FNV1A( "C_CSPlayer::m_iv_angEyeAngles" ) ) )
			eye_watcher->RestoreToLastNetworked();

		if ( !log.record.empty() )
			player->get_eye_angles().z = lag_record_t::get_resolver_roll( log.get_dir() );

		player->get_collision_viewheight() = player->get_abs_origin().z + log.m_view_delta;
		player->get_collision_bounds_change_time() = log.m_unpred_change_time;
	}
	else
	{
		auto& current_info = prediction::get_pred_info( animations::most_recent.second );
		if ( current_info.sequence == animations::most_recent.second )
		{
			if ( vars::misc.goblin_mode->get<bool>() )
			{
				player->get_collision_bounds_change_time() = interfaces::globals()->curtime + 0.02f;
				player->get_collision_viewheight() = player->get_ground_entity() == 0xFFFFFFFF ? player->get_abs_origin().z + 57.f : player->get_abs_origin().z + 56.f - ( current_info.view_delta - 54.f ) * 0.5f;
				player->get_ground_entity() = interfaces::entity_list()->get_client_entity( 0 )->GetRefEHandle();
			}
			else
			{
				player->get_collision_bounds_change_time() = current_info.undpred_change_time;
				player->get_collision_viewheight() = player->get_abs_origin().z + current_info.view_delta;
			}

			//player->get_ground_entity() = current_info.groundentity;
			interfaces::prediction()->SetLocalViewAngles( current_info.animation.eye_angles );
		}
	}

	const auto state = player->get_anim_state();
	CBaseHandle rollback_weapon = {};

	if ( state )
	{
		state->first_foot_plant = true;
		if ( player->EntIndex() == interfaces::engine()->GetLocalPlayer() )
			player->set_abs_angles( { 0.f, state->foot_yaw, 0.f } );
	}

	if ( player->get_alive() && !player->is_ragdoll() && state )
	{
		const auto weapons = player->get_weapons();
		for ( size_t i = 0; weapons[ i ] != INVALID_EHANDLE_INDEX; i++ )
		{
			const auto weapon = reinterpret_cast< C_WeaponCSBaseGun* >( interfaces::entity_list()->get_client_entity_from_handle( weapons[ i ] ) );
			if ( weapon == state->weapon )
			{
				rollback_weapon = player->get_active_weapon();
				player->get_active_weapon() = weapons[ i ];
				break;
			}
		}
	}

	const auto ret = original( setup_bones )( ecx, edx, out, max_bones, mask, time );

	player->get_view_offset() = backup_viewoffset;
	player->GetCollideable()->OBBMaxs() = backup_maxs;
	player->get_ground_entity() = backup_groundentity;
	player->get_collision_viewheight() = backup_collision_viewheight;
	player->get_collision_bounds_change_time() = backup_collision_bounds_change_time;
	player->get_eye_angles() = backup_eye_angles;

	if ( rollback_weapon )
		player->get_active_weapon() = rollback_weapon;

	player->get_lod_data().flags = flags;
	return ret;
}

void __fastcall hook::reevauluate_anim_lod( C_CSPlayer* player, uint32_t edx, uint32_t bone_mask )
{
	// stub
}

void __fastcall hook::update_interp_vars( C_BaseAnimating* animating, void* edx )
{
	const auto bak = animating->get_clientside_animation();
	if ( animating->is_player() && !vars::legit_enabled() || animating->EntIndex() == interfaces::engine()->GetLocalPlayer() )
		animating->get_clientside_animation() = false;
	original( update_interp_vars )( animating, edx );
	animating->get_clientside_animation() = bak;
}

void __fastcall hook::on_latch_interp_var( C_BaseEntity* ent, void* edx, int flags )
{
	const auto player = reinterpret_cast< C_CSPlayer* >( ent );
	if ( player->EntIndex() == interfaces::engine()->GetLocalPlayer() )
	{
		if ( flags & ( 1 << 5 ) && prediction::computing )
			return;

		const auto backup_time = interfaces::globals()->curtime;
		if ( player->is_player() )
			interfaces::globals()->curtime = ticks_to_time( player->get_command_context().cmd_number );
		if ( player->GetClientClass()->m_ClassID == ClassId::CPredictedViewModel )
		{
			const auto owner = reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity_from_handle( reinterpret_cast< C_BaseViewModel* >( player )->get_owner() ) );
			if ( owner && owner->is_player() )
				interfaces::globals()->curtime = ticks_to_time( owner->get_command_context().cmd_number );
		}

		original( on_latch_interp_var )( ent, edx, flags );
		interfaces::globals()->curtime = backup_time;
		return;
	}

	if ( !player->is_player() || !( flags & ( 1 << 1 ) ) )
		return original( on_latch_interp_var )( ent, edx, flags );

	auto& curlog = player_log::get_log( ent->EntIndex() );

	var( sv_clockcorrection_msecs );
	var( cl_interp );

	const auto clock_correction = clamp( sv_clockcorrection_msecs->get_float() / 1000.0f, 0.0f, 1.0f );
	const auto original_simtime = player->get_simtime();

	player->get_simtime() = clamp( player->get_simtime() + curlog.m_add, interfaces::globals()->curtime - clock_correction + cl_interp->get_float(), interfaces::globals()->curtime + clock_correction + cl_interp->get_float() );

	original( on_latch_interp_var )( ent, edx, flags );

	player->get_simtime() = original_simtime;
	curlog.m_add = interfaces::globals()->curtime - player->get_simtime() + cl_interp->get_float();

}

void __fastcall hook::post_data_update( void* ecx, void* edx, int type )
{
	const auto player = reinterpret_cast< C_CSPlayer* >( reinterpret_cast< uintptr_t >( ecx ) - 8 );

	player_log::run_player( player );

	if ( vars::misc.radar->get<bool>() && player->is_enemy() && !( interfaces::game_rules_proxy() && ( *interfaces::game_rules_proxy() )->m_bIsValveDS() ) )
		player->get_spotted() = true;

	*player->get_old_origin() = player->get_data_origin();
	*reinterpret_cast< QAngle* >( reinterpret_cast< uintptr_t >( player->get_old_origin() ) + sizeof( Vector ) ) = player->get_data_rotation();

	const auto bak = player->get_clientside_animation();
	if ( !vars::legit_enabled() || player->EntIndex() == interfaces::engine()->GetLocalPlayer() )
		player->get_clientside_animation() = false;
	original( post_data_update )( ecx, edx, type );
	player->get_clientside_animation() = bak;
}

void __fastcall hook::maintain_sequence_transitions( C_BaseAnimating* ent, uintptr_t edx, uintptr_t bone_setup, Vector* pos, Quaternion* q )
{
	float cycle;
	__asm movss cycle, xmm2;

	const auto flags = reinterpret_cast< uint32_t* >( reinterpret_cast< uintptr_t( __thiscall* )( CStudioHdr*, int32_t ) >( make_offset( "client.dll", sig_get_sequence_flags ) )( ent->get_model_ptr(), ent->get_sequence() ) + 12 );

	if ( local_player && local_player->get_alive() && ent->GetClientClass()->m_ClassID == ClassId::CPredictedViewModel && ent->get_new_sequence_parity() != ent->get_prev_new_sequence_parity() && !( *flags & 1 ) )
	{
		auto view_model = static_cast< C_BaseViewModel* >( ent );

		const auto player = reinterpret_cast< C_CSPlayer* const >(
			interfaces::entity_list()->get_client_entity_from_handle( view_model->get_owner() ) );

		if ( player && player->is_holding_looking_at_weapon() )
		{
			const auto dt = interfaces::globals()->curtime - ticks_to_time( interfaces::globals()->tickcount );
			ent->get_cycle_offset() = -1.f * ( ( ticks_to_time( interfaces::client_state()->command_ack ) + dt - ent->get_anim_time() ) * ent->get_sequence_cycle_rate( ent->get_model_ptr(), ent->get_sequence() ) );
			cycle = 0.f;
		}
		else
			ent->get_cycle_offset() = 0.f;

		*flags |= 1;
		__asm movss xmm2, cycle;
		original( maintain_sequence_transitions )( ent, edx, bone_setup, pos, q );
		*flags &= ~1;
	}
	else
	{
		__asm movss xmm2, cycle;
		original( maintain_sequence_transitions )( ent, edx, bone_setup, pos, q );
	}
}
