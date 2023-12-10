#include <intrin.h>

#include "../include_cheat.h"

void __fastcall hook::send_weapon_anim( C_WeaponCSBaseGun* weapon, uint32_t edx, uint32_t act )
{
	const auto owner = reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity_from_handle( weapon->get_owner_ent() ) );
	if ( owner && owner->is_player() && owner->get_alive() && owner->EntIndex() == interfaces::engine()->GetLocalPlayer() )
	{
		const auto& context = owner->get_command_context();
		auto& animation = prediction::get_pred_info( context.cmd.command_number - 1 ).animation;
		const auto fresh = interfaces::input()->m_pVerifiedCommands[ context.cmd.command_number % 150 ].m_crc ==
			*reinterpret_cast< int32_t* >( &interfaces::globals()->interval_per_tick );

		if ( fresh && ( act == 192 || act == 194 || act == 831 || act == 220 || act == 221 ) )
			owner->is_looking_at_weapon() = owner->is_holding_looking_at_weapon() = false;

		if ( animation.sequence == context.cmd.command_number - 1 && ( fresh || context.cmd.command_number <= interfaces::client_state()->lastoutgoingcommand ) )
			animation.addon.vm = act;

	}

	original( send_weapon_anim )( weapon, edx, act );
}

void __fastcall hook::fire_event( C_CSPlayer* player, uint32_t edx, Vector* origin, Vector* angles, int event, const char* options )
{
	original( fire_event )( player, edx, origin, angles, event, options );

	if ( player->EntIndex() == interfaces::engine()->GetLocalPlayer() )
	{
		const auto& context = player->get_command_context();
		auto& info = prediction::get_pred_info( context.cmd.command_number - 1 );
		if ( info.sequence == context.cmd.command_number - 1 )
			info.ev = event;
	}
}

void __fastcall hook::do_animation_events( C_CSPlayer* player, uint32_t edx, CStudioHdr* hdr )
{
	if ( player->EntIndex() != interfaces::engine()->GetLocalPlayer() )
		original( do_animation_events )( player, edx, hdr );
}

void __fastcall hook::do_animation_events_animating( C_BaseAnimating* animating, uint32_t edx, CStudioHdr* hdr )
{
	animating->get_prev_reset_events_parity() = animating->get_reset_events_parity();
	original( do_animation_events_animating )( animating, edx, hdr );
}

bool __fastcall hook::deploy( C_WeaponCSBaseGun* wpn, uint32_t edx )
{
	const auto world_model = reinterpret_cast< C_BaseWeaponWorldModel* >( interfaces::entity_list()->get_client_entity_from_handle( wpn->get_world_model_handle() ) );
	if ( world_model )
		world_model->get_effects() |= EF_NODRAW;

	return original( deploy )( wpn, edx );
}

int __fastcall hook::get_default_fov( C_CSPlayer* player, uintptr_t edx )
{
	if ( interfaces::prediction()->InPrediction() || !vars::misc.fov_enabled->get<bool>() )
		return original( get_default_fov )( player, edx );

	return static_cast< int32_t >( vars::misc.fov->get<float>() );
}

double __fastcall hook::get_fov( C_CSPlayer* player, uintptr_t edx )
{
	if ( prediction::computing )
		return original( get_fov )( player, edx );

	const auto backup_fov = player->get_fov();
	const auto backup_fov_start = player->get_fov_start();

	const auto get_modified_fov = [player] ( int fov_in ) -> int
	{
		if ( !vars::misc.fov_enabled->get<bool>() )
			return fov_in;

		constexpr auto default_fov = 90.f;

		const auto weapon = globals::get_weapon( player->get_active_weapon() );
		if ( !weapon || fov_in == default_fov || fov_in == 0 || local_player->get_observer_mode() == OBS_MODE_IN_EYE )
			return vars::misc.fov->get<int>();

		if ( fov_in == weapon->get_wpn_data()->zoom_fov1 )
		{
			if ( vars::misc.fov_scope->get<int>() && weapon->get_wpn_data()->zoom_levels > 1 )
			{
				const auto diff = default_fov - float( fov_in );
				return ( float( fov_in ) + diff * ( vars::misc.fov_scope->get<float>() * 0.01f ) ) / default_fov * vars::misc.fov->get<float>();
			}

			return float( fov_in ) / default_fov * vars::misc.fov->get<float>();
		}

		if ( fov_in == weapon->get_wpn_data()->zoom_fov2 )
		{
			if ( vars::misc.fov_scope->get<int>() && weapon->get_wpn_data()->zoom_levels > 1 )
				return float( weapon->get_wpn_data()->zoom_fov1 ) / default_fov * vars::misc.fov->get<float>();

			return float( fov_in ) / default_fov * vars::misc.fov->get<float>();
		}

		return fov_in;
	};

	player->get_fov() = get_modified_fov( backup_fov );
	player->get_fov_start() = get_modified_fov( backup_fov_start );

	const auto ret = original( get_fov )( player, edx );
	player->get_fov() = backup_fov;
	player->get_fov_start() = backup_fov_start;
	return ret;
}

void __fastcall hook::modify_eye_pos( void* ecx, void* edx, Vector* pos )
{
	if ( !aimbot::shot_record.m_shot )
	{
		if ( antiaim::in_fakeduck() && !globals::in_cm )
			pos->z = local_player->get_abs_origin().z + 64.f;

		return;
	}

	const auto& animstate = aimbot::shot_record.m_state[ resolver_direction::resolver_networked ].m_animstate;
	if ( ( animstate.in_hit_ground || animstate.duck_amount != 0.f ) && local_player->get_ground_entity() != 0xFFFFFFFF )
	{
		auto bone_pos = animations::get_headpos();

		bone_pos.z += 1.7f;

		if ( pos->z > bone_pos.z )
		{
			auto some_factor = 0.f;

			const auto delta = ( *pos ).z - bone_pos.z;
			const auto some_offset = ( delta - 4.f ) / 6.f;

			if ( some_offset >= 0.f )
				some_factor = std::fminf( some_offset, 1.f );

			pos->z += ( bone_pos.z - pos->z ) * ( some_factor * some_factor * 3.f - some_factor * some_factor * 2.f * some_factor );
		}
	}
}

void __fastcall hook::obb_change_callback( C_CSPlayer* player, uint32_t edx, Vector* old_mins, Vector* mins, Vector* old_maxs, Vector* maxs )
{
	if ( CGameMovement::computing )
		return original( obb_change_callback )( player, edx, old_mins, mins, old_maxs, maxs );

	if ( player->get_predictable() && interfaces::prediction()->InPrediction() )
	{
		auto& log = player_log::get_log( player->EntIndex() );
		log.m_view_delta = old_maxs->z;
		player->get_collision_viewheight() = player->get_data_origin().z + old_maxs->z;
		player->get_collision_bounds_change_time() = interfaces::globals()->curtime;
		log.m_unpred_change_time = prediction::orig_curtime;
	}
	else if ( !player->get_predictable() )
	{
		player_log::get_log( player->EntIndex() ).m_maxs_z = old_maxs->z;
		if ( vars::legit_enabled() )
			return original( obb_change_callback )( player, edx, old_mins, mins, old_maxs, maxs );
	}
}

void __fastcall hook::set_abs_angles( void* ecx, void* edx, QAngle& angles )
{
	const auto player = reinterpret_cast< C_CSPlayer* const >( ecx );

	if ( reinterpret_cast< uintptr_t >( _ReturnAddress() ) == make_offset( "client.dll", sig_animstate_update_setabsagles ) && ( !vars::legit_enabled() || player->EntIndex() == interfaces::engine()->GetLocalPlayer() ) )
		return;

	original( set_abs_angles )( ecx, edx, angles );
}

void __fastcall hook::calc_view( C_CSPlayer* player, uint32_t edx, Vector* eye, QAngle* ang, float* znear, float* zfar, float* fov )
{
	const auto weapon = globals::get_weapon( player->get_active_weapon() );
	const auto ironsight = weapon ? &weapon->get_iron_sight_controller() : nullptr;
	const auto backup_ironsight = ironsight ? *ironsight : 0;

	if ( vars::misc.fov_enabled->get<bool>() && ironsight )
		*ironsight = 0;

	original( calc_view )( player, edx, eye, ang, znear, zfar, fov );

	if ( vars::misc.fov_enabled->get<bool>() && ironsight )
		*ironsight = backup_ironsight;
}


void __fastcall hook::update_clientside_anim( C_CSPlayer* player, void* edx )
{
	if ( player->get_anim_state() && ( !vars::legit_enabled() || player == local_player ) )
	{
		player->get_anim_state()->player = nullptr;
		original( update_clientside_anim )( player, edx );
		player->get_anim_state()->player = player;
	}
	else
		original( update_clientside_anim )( player, edx );
}