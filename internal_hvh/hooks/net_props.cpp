#include "../include_cheat.h"

void __cdecl hook::layer_sequence( CRecvProxyData* pData, void* entity, void* output )
{
	const auto layer = reinterpret_cast< C_AnimationLayer* >( entity );
	if ( !layer->m_pOwner )
		return original( layer_sequence )( pData, entity, output );

	auto& log = player_log::get_log( layer->m_pOwner->EntIndex() );
	log.player = layer->m_pOwner;
	const auto index = layer - &layer->m_pOwner->get_anim_layers()[ 0 ];
	if ( layer->m_pOwner->EntIndex() == interfaces::engine()->GetLocalPlayer() && prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.sequence == interfaces::client_state()->command_ack )
	{
		auto& pred_info = prediction::get_pred_info( interfaces::client_state()->command_ack );
		const auto act = log.player->get_sequence_act( pData->m_Value.m_Int );
		if ( index == 1 )
		{
			if ( act == 962 )
				pred_info.animation.addon.swing_left = false;
		}
		else if ( index == 3 )
		{
			if ( pred_info.animation.layers[ index ].m_nSequence != pData->m_Value.m_Int )
				pred_info.animation.addon.in_idle = act == 979 || act == 980;
		}
		else if ( index == 7 )
			pred_info.animation.state.strafe_sequence = pData->m_Value.m_Int;

		pred_info.animation.layers[ index ].m_nSequence = pData->m_Value.m_Int;
	}
	else
		log.server_layers[ index ].m_nSequence = pData->m_Value.m_Int;

	if ( vars::legit_enabled() && layer->m_pOwner->EntIndex() != interfaces::engine()->GetLocalPlayer() )
		return original( layer_sequence )( pData, entity, output );
}

void __cdecl hook::layer_cycle( CRecvProxyData* pData, void* entity, void* output )
{
	const auto layer = reinterpret_cast< C_AnimationLayer* >( entity );
	if ( !layer->m_pOwner )
		return original( layer_cycle )( pData, entity, output );

	auto& log = player_log::get_log( layer->m_pOwner->EntIndex() );
	log.player = layer->m_pOwner;
	const auto index = layer - &layer->m_pOwner->get_anim_layers()[ 0 ];
	if ( layer->m_pOwner->EntIndex() == interfaces::engine()->GetLocalPlayer() && prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.sequence == interfaces::client_state()->command_ack )
	{
		auto& animation = prediction::get_pred_info( interfaces::client_state()->command_ack ).animation;
		animation.layers[ index ].m_flCycle = pData->m_Value.m_Float;
		if ( index == 6 )
			animation.state.feet_cycle = pData->m_Value.m_Float;
		else if ( index == 7 )
			animation.state.strafe_change_cycle = pData->m_Value.m_Float;
	}
	else
		log.server_layers[ index ].m_flCycle = pData->m_Value.m_Float;

	if ( vars::legit_enabled() && layer->m_pOwner->EntIndex() != interfaces::engine()->GetLocalPlayer() )
		return original( layer_cycle )( pData, entity, output );
}

void __cdecl hook::layer_playback_rate( CRecvProxyData* pData, void* entity, void* output )
{
	const auto layer = reinterpret_cast< C_AnimationLayer* >( entity );
	if ( !layer->m_pOwner )
		return original( layer_playback_rate )( pData, entity, output );

	auto& log = player_log::get_log( layer->m_pOwner->EntIndex() );
	log.player = layer->m_pOwner;
	const auto index = layer - &layer->m_pOwner->get_anim_layers()[ 0 ];
	if ( layer->m_pOwner->EntIndex() == interfaces::engine()->GetLocalPlayer() && prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.sequence == interfaces::client_state()->command_ack )
		prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.layers[ index ].m_flPlaybackRate = pData->m_Value.m_Float;
	else
		log.server_layers[ index ].m_flPlaybackRate = pData->m_Value.m_Float;

	if ( vars::legit_enabled() && layer->m_pOwner->EntIndex() != interfaces::engine()->GetLocalPlayer() )
		return original( layer_playback_rate )( pData, entity, output );
}

void __cdecl hook::layer_weight( CRecvProxyData* pData, void* entity, void* output )
{
	auto layer = reinterpret_cast< C_AnimationLayer* >( entity );
	if ( !layer->m_pOwner )
		return original( layer_weight )( pData, entity, output );

	auto& log = player_log::get_log( layer->m_pOwner->EntIndex() );
	log.player = layer->m_pOwner;
	const auto index = layer - &layer->m_pOwner->get_anim_layers()[ 0 ];
	if ( layer->m_pOwner->EntIndex() == interfaces::engine()->GetLocalPlayer() && prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.sequence == interfaces::client_state()->command_ack )
	{
		auto& animation = prediction::get_pred_info( interfaces::client_state()->command_ack ).animation;
		animation.layers[ index ].m_flWeight = pData->m_Value.m_Float;
		if ( index == 6 )
			animation.state.feet_weight = pData->m_Value.m_Float;
		else if ( index == 7 )
			animation.state.strafe_change_weight = pData->m_Value.m_Float;
	}
	else
		log.server_layers[ index ].m_flWeight = pData->m_Value.m_Float;

	if ( vars::legit_enabled() && layer->m_pOwner->EntIndex() != interfaces::engine()->GetLocalPlayer() )
		return original( layer_weight )( pData, entity, output );
}

void __cdecl hook::layer_weight_delta_rate( CRecvProxyData* pData, void* entity, void* output )
{
	const auto layer = reinterpret_cast< C_AnimationLayer* >( entity );
	if ( !layer->m_pOwner )
		return original( layer_weight_delta_rate )( pData, entity, output );

	auto& log = player_log::get_log( layer->m_pOwner->EntIndex() );
	log.player = layer->m_pOwner;
	const auto index = layer - &layer->m_pOwner->get_anim_layers()[ 0 ];
	if ( layer->m_pOwner->EntIndex() == interfaces::engine()->GetLocalPlayer() && prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.sequence == interfaces::client_state()->command_ack )
		prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.layers[ index ].m_flWeightDeltaRate = pData->m_Value.m_Float;
	else
		log.server_layers[ index ].m_flWeightDeltaRate = pData->m_Value.m_Float;

	if ( vars::legit_enabled() && layer->m_pOwner->EntIndex() != interfaces::engine()->GetLocalPlayer() )
		return original( layer_weight_delta_rate )( pData, entity, output );
}

void __cdecl hook::layer_order( CRecvProxyData* pData, void* entity, void* output )
{
	const auto layer = reinterpret_cast< C_AnimationLayer* >( entity );
	if ( !layer->m_pOwner )
		return original( layer_order )( pData, entity, output );

	auto& log = player_log::get_log( layer->m_pOwner->EntIndex() );
	log.player = layer->m_pOwner;
	const auto index = layer - &layer->m_pOwner->get_anim_layers()[ 0 ];
	if ( layer->m_pOwner->EntIndex() == interfaces::engine()->GetLocalPlayer() && prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.sequence == interfaces::client_state()->command_ack )
	{
		if ( index == 1 )
		{
			const auto offset = 0x7A + ( pData->m_Value.m_Int == 7 ? 1 : 8 );
			prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.state.layer_order_preset = *reinterpret_cast< uintptr_t* >( make_offset_multi( "client.dll", sig_update_layer_order_preset ) + offset );
		}
		prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.layers[ index ].m_nOrder = pData->m_Value.m_Int;
	}

	else
		log.server_layers[ index ].m_nOrder = pData->m_Value.m_Int;

	if ( vars::legit_enabled() && layer->m_pOwner->EntIndex() != interfaces::engine()->GetLocalPlayer() )
		return original( layer_order )( pData, entity, output );
}

void __cdecl hook::general_float( CRecvProxyData* pData, void* entity, void* output )
{
	if ( !pData->m_pRecvProp || !pData->m_pRecvProp->m_pVarName )
		return original( general_float )( pData, entity, output );

	const auto var_name = fnv1a_rt( pData->m_pRecvProp->m_pVarName );
	if ( var_name == FNV1A( "m_flPrevCycle" ) )
	{
		auto layer = reinterpret_cast< C_AnimationLayer* >( entity );
		if ( !layer->m_pOwner )
			return original( general_float )( pData, entity, output );

		auto& log = player_log::get_log( layer->m_pOwner->EntIndex() );
		log.player = layer->m_pOwner;
		const auto index = layer - &layer->m_pOwner->get_anim_layers()[ 0 ];
		if ( layer->m_pOwner->EntIndex() == interfaces::engine()->GetLocalPlayer() && prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.sequence == interfaces::client_state()->command_ack )
			prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.layers[ index ].m_flPrevCycle = pData->m_Value.m_Float;
		else
			log.server_layers[ index ].m_flPrevCycle = pData->m_Value.m_Float;

		if ( vars::legit_enabled() && layer->m_pOwner->EntIndex() != interfaces::engine()->GetLocalPlayer() )
			return original( general_float )( pData, entity, output );

		return;
	}

	if ( var_name == FNV1A( "m_nTickBase" ) )
	{
		const auto player = reinterpret_cast< C_CSPlayer* >( entity );
		if ( player->EntIndex() != interfaces::engine()->GetLocalPlayer() && player->IsDormant() )
		{
			auto& log = player_log::get_log( player->EntIndex() );
			log.m_enter_pvs = true;
		}

		if ( player->EntIndex() == interfaces::engine()->GetLocalPlayer() )
		{
			auto& info = prediction::get_pred_info( interfaces::client_state()->command_ack );
			info.animation.state.last_update = ticks_to_time( pData->m_Value.m_Int - 1 );
			/*if ( pData->m_Value.m_Int == info.tick_base && info.sequence == interfaces::client_state()->command_ack && interfaces::client_state()->command_ack != interfaces::client_state()->last_command_ack )
				tickbase::server_limit = interfaces::client_state()->m_ClockDriftMgr.m_nServerTick - pData->m_Value.m_Int + 2;*/

			if ( pData->m_Value.m_Int != info.tick_base && info.sequence == interfaces::client_state()->command_ack && interfaces::client_state()->command_ack != interfaces::client_state()->last_command_ack )
			{

				info.tickbase.limit = clamp( info.tickbase.limit - ( info.tick_base - static_cast< int >( pData->m_Value.m_Int ) ), 0, sv_maxusrcmdprocessticks );
				//util::print_dev_console( true, Color::White(), "adjusted limit by - %d   new limit %d\n", info.tick_base - pData->m_Value.m_Int, info.tickbase.limit );

			//tickbase::limit = std::min( tickbase::limit - ( info.tick_base - static_cast< int >( pData->m_Value.m_Int ) ), 0 );
			}

			//util::print_dev_console( true, Color::White(), "tickbase: %d\n", pData->m_Value.m_Int );

			if ( animations::lag.second == interfaces::client_state()->command_ack )
				animations::lag.first = pData->m_Value.m_Int;
		}
	}

	if ( var_name == FNV1A( "m_flGroundAccelLinearFracLastTime" ) )
	{
		const auto player = reinterpret_cast< C_CSPlayer* >( entity );
		if ( player->EntIndex() == interfaces::engine()->GetLocalPlayer() )
			return original( general_float )( pData, entity, output );

		auto& log = player_log::get_log( player->EntIndex() );
		log.m_ground_accel_linear_frac_last_time_stamp = interfaces::client_state()->command_ack;
		log.m_ground_accel_linear_frac_last_time = pData->m_Value.m_Float;

		return original( general_float )( pData, entity, output );
	}

	if ( var_name == FNV1A( "m_flLowerBodyYawTarget" ) )
	{
		const auto player = reinterpret_cast< C_CSPlayer* >( entity );
		if ( player->EntIndex() != interfaces::engine()->GetLocalPlayer() )
			return original( general_float )( pData, entity, output );

		if ( prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.sequence == interfaces::client_state()->command_ack )
			prediction::get_pred_info( interfaces::client_state()->command_ack ).animation.lower_body_yaw_target = pData->m_Value.m_Float;

		return;
	}

	if ( var_name == FNV1A( "m_flThirdpersonRecoil" ) )
	{
		const auto player = reinterpret_cast< C_CSPlayer* >( entity );
		if ( player->EntIndex() != interfaces::engine()->GetLocalPlayer() )
			return original( general_float )( pData, entity, output );

		var( weapon_recoil_scale );
		if ( weapon_recoil_scale->get_float() > 0.f )
			player->get_aim_punch().x = pData->m_Value.m_Float / weapon_recoil_scale->get_float();
	}

	if ( var_name == FNV1A( "m_vecViewOffset[2]" ) )
	{
		const auto player = reinterpret_cast< C_CSPlayer* >( entity );
		if ( player->EntIndex() != interfaces::engine()->GetLocalPlayer() )
			return original( general_float )( pData, entity, output );

		if ( fabsf( pData->m_Value.m_Float - prediction::get_pred_info( interfaces::client_state()->command_ack ).viewoffset_z ) < .375f )
			pData->m_Value.m_Float = prediction::get_pred_info( interfaces::client_state()->command_ack ).viewoffset_z;
	}

	if ( var_name == FNV1A( "m_angEyeAngles[0]" ) )
	{
		const auto player = reinterpret_cast< C_CSPlayer* >( entity );
		if ( player && player->is_player() && !player->get_player_info().fakeplayer )
		{
			auto& log = player_log::get_log( player->EntIndex() );
			log.nospread.m_can_fake = fabsf( fabsf( pData->m_Value.m_Float ) - 180.f ) < 13.f;
			log.m_proxy_pitch = pData->m_Value.m_Float;

			pData->m_Value.m_Float = std::max( -90.f, std::min( math::normalize_float( pData->m_Value.m_Float ), 90.f ) );
		}
	}

	const auto is_addon_junk = var_name == FNV1A( "m_iAddonBits" )
		|| var_name == FNV1A( "m_iPrimaryAddon" )
		|| var_name == FNV1A( "m_iSecondaryAddon" );

	const auto is_fov_junk = var_name == FNV1A( "m_flFOVTime" );

	if ( is_addon_junk || is_fov_junk )
	{
		const auto player = reinterpret_cast< C_CSPlayer* >( entity );
		if ( player->EntIndex() == interfaces::engine()->GetLocalPlayer() && player->get_alive() )
			return;
	}

	return original( general_float )( pData, entity, output );
}

void __cdecl hook::general_vec( CRecvProxyData* pData, void* entity, void* output )
{
	if ( !pData->m_pRecvProp || !pData->m_pRecvProp->m_pVarName )
		return original( general_vec )( pData, entity, output );

	const auto var_name = fnv1a_rt( pData->m_pRecvProp->m_pVarName );
	if ( var_name == FNV1A( "m_aimPunchAngle" ) )
	{
		auto o = reinterpret_cast< Vector* >( output );
		o->y = pData->m_Value.m_Vector.y;
		o->z = pData->m_Value.m_Vector.z;
		return;
	}

	if ( var_name == FNV1A( "m_vecForce" ) || var_name == FNV1A( "m_vecRagdollVelocity" ) )
		pData->m_Value.m_Vector *= vars::visuals.ragdoll_thrust->get<float>() * 0.01f;



	return original( general_vec )( pData, entity, output );
}

void __cdecl hook::general_int( CRecvProxyData* pData, void* entity, void* output )
{
	if ( !pData->m_pRecvProp || !pData->m_pRecvProp->m_pVarName )
		return original( general_int )( pData, entity, output );

	const auto var_name = fnv1a_rt( pData->m_pRecvProp->m_pVarName );
	if ( var_name == FNV1A( "m_nModelIndex" ) )
	{
		const auto player = reinterpret_cast< C_CSPlayer* >( entity );
		if ( player->EntIndex() == interfaces::engine()->GetLocalPlayer() )
			skinchanger::server_model_idx = pData->m_Value.m_Int;
	}

	return original( general_int )( pData, entity, output );
}

void __cdecl hook::general_bool( CRecvProxyData* pData, void* entity, void* output )
{
	if ( !pData->m_pRecvProp || !pData->m_pRecvProp->m_pVarName )
		return original( general_bool )( pData, entity, output );

	const auto var_name = fnv1a_rt( pData->m_pRecvProp->m_pVarName );
	if ( var_name == FNV1A( "m_bDidSmokeEffect" ) )
	{
		if ( vars::misc.remove_smoke->get<bool>() )
		{
			*reinterpret_cast< bool* >( reinterpret_cast< uintptr_t >( output ) + 0x1 ) = true;
			return;
		}
	}

	return original( general_bool )( pData, entity, output );
}

void __cdecl hook::simulation_time( CRecvProxyData* pData, void* entity, void* output )
{
	const auto player = reinterpret_cast< C_CSPlayer* >( entity );
	if ( !player->is_player() || player->EntIndex() == interfaces::engine()->GetLocalPlayer() )
		return original( simulation_time )( pData, entity, output );

	auto& log = player_log::get_log( player->EntIndex() );
	log.m_simtime_int = pData->m_Value.m_Int;
	if ( !log.m_simtime_int )
		return;

	return original( simulation_time )( pData, entity, output );
}