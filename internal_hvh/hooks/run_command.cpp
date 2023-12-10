#include "../include_cheat.h"
#include "intrin.h"

auto cmdnum = 0;

void __fastcall hook::run_command( void* ecx, void* edx, C_BasePlayer* player, CUserCmd* cmd, IMoveHelper* move_helper )
{
	prediction::set_last_command_number( cmd->command_number );

	const auto& last_info = prediction::get_pred_info( cmd->command_number - 1 );
	auto& current_info = prediction::get_pred_info( cmd->command_number );

	if ( last_info.sequence == cmd->command_number - 1 )
	{
		local_player->get_collision_viewheight() = last_info.collision_viewheight;
		local_player->get_collision_bounds_change_time() = last_info.collision_bounds_change_time;
	}

	// adjust for tickbase shift.
	tickbase::on_runcmd( cmd, player->get_tickbase() );

	auto zoom_level = -1;

	// write back postpone time.
	const auto wpn = globals::get_weapon( player->get_active_weapon() );
	if ( wpn )
	{
		wpn->get_postpone_fire_ready_time() = current_info.postpone_fire_ready_time;

		if ( cmd->hasbeenpredicted )
			wpn->get_last_client_fire_bullet_time() = interfaces::globals()->curtime;
		else
		{
			zoom_level = wpn->get_zoom_level();
			wpn->get_last_client_fire_bullet_time() = interfaces::globals()->curtime - 1.f;
		}

	}

	/*if ( !prediction::computing )
		lua::api.callback( FNV1A( "on_run_command" ), [cmd] ( lua::state& s ) -> int
	{
		s.create_user_object_ptr( XOR_STR( "csgo.user_cmd" ), &cmd );
		return 1;
	} );*/

	original( run_command )( ecx, edx, player, cmd, move_helper );

	// play scope sound.
	if ( zoom_level > -1 && wpn == globals::get_weapon( player->get_active_weapon() ) && !wpn->in_reload() )
	{
		if ( zoom_level < wpn->get_zoom_level() )
		{
			const auto in_sound = wpn->get_zoom_in_sound();
			if ( in_sound && in_sound[ 0 ] )
				player->emit_sound( in_sound );
		}
		else if ( zoom_level > wpn->get_zoom_level() )
		{
			const auto out_sound = wpn->get_zoom_out_sound();
			if ( out_sound && out_sound[ 0 ] )
				player->emit_sound( out_sound );
		}
	}

	const auto new_wpn = globals::get_weapon( player->get_active_weapon() );

	if ( current_info.sequence == cmd->command_number )
	{
		current_info.velocity = player->get_velocity();
		current_info.origin = player->get_origin();
		current_info.buttons = cmd->buttons;
		current_info.viewoffset_z = player->get_view_offset().z;
		current_info.maxs = player->GetCollideable()->OBBMaxs();
		current_info.groundentity = player->get_ground_entity();
		current_info.wpn = player->get_active_weapon();
		current_info.last_shot = new_wpn ? new_wpn->get_last_shot_time() : 0.f;
		current_info.collision_viewheight = local_player->get_collision_viewheight();
		current_info.collision_bounds_change_time = local_player->get_collision_bounds_change_time();
		current_info.tick_base = player->get_tickbase();

		const auto& log = player_log::get_log( player->EntIndex() );
		current_info.view_delta = log.m_view_delta;
		current_info.undpred_change_time = log.m_unpred_change_time;
	}
}

void __fastcall hook::process_movement( void* ecx, void* edx, C_CSPlayer* player, CMoveData* data )
{
	data->m_bGameCodeMovedPlayer = false;
	original( process_movement )( ecx, edx, player, data );
}

int __fastcall hook::transfer_data( CPredictionCopy* copy, void* edx, char* operation, int entindex, datamap_t* dmap )
{
	if ( reinterpret_cast< uintptr_t >( _ReturnAddress() ) != make_offset( "client.dll", sig_ret_to_post_network_data_received ) )
		return original( transfer_data )( copy, edx, operation, entindex, dmap );

	const flattenedoffsets_t& flat = dmap->m_pOptimizedDataMap->m_Info[ PC_NETWORKED_ONLY ].m_Flat;
	const typedescription_t* base = &flat.m_Flattened[ 0 ];

	for ( int i = 0; i < flat.m_Flattened.Count(); ++i )
	{
		const auto field = &base[ i ];
		if ( field->flags & FTYPEDESC_NOERRORCHECK || field->fieldType != FIELD_FLOAT && field->fieldType != FIELD_INTEGER )
			continue;

		auto& out_data = *reinterpret_cast< int* >( copy->m_pDest + field->flatOffset[ copy->m_nDestOffsetIndex ] ); //pred
		auto& in_data = *reinterpret_cast< int* >( copy->m_pSrc + field->flatOffset[ copy->m_nSrcOffsetIndex ] ); //net

		const auto name = fnv1a_rt( field->fieldName );

		if ( field->fieldType == FIELD_INTEGER )
		{
			if ( name == FNV1A( "m_nAnimationParity" ) )
				out_data = in_data;

			if ( name == FNV1A( "m_iShotsFired" ) )
			{
				auto diff = out_data - in_data;
				if ( diff > 0 && diff <= 2 )
				{
					while ( diff > 0 )
					{
						if ( !resolver::unapproved_shots.empty() )
							resolver::unapproved_shots.pop_back();

						--diff;
					}
				}
			}
		}

		const auto& out_data_f = *reinterpret_cast< const float* >( copy->m_pDest + field->flatOffset[ copy->m_nDestOffsetIndex ] ); //pred
		const auto& in_data_f = *reinterpret_cast< const float* >( copy->m_pSrc + field->flatOffset[ copy->m_nSrcOffsetIndex ] ); //net

		const auto differs = out_data_f != in_data_f;
		if ( differs && field->fieldType == FIELD_FLOAT )
		{
			if ( name == FNV1A( "m_fLastShotTime" ) )
			{
				const auto wpn = reinterpret_cast< C_WeaponCSBaseGun* >( interfaces::entity_list()->get_client_entity( entindex ) );
				const auto player = wpn ? reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity_from_handle( wpn->get_owner_ent() ) ) : nullptr;

				if ( player && player->EntIndex() == interfaces::engine()->GetLocalPlayer() && prediction::get_pred_info( interfaces::client_state()->command_ack ).wpn == wpn->GetRefEHandle() )
				{
					const auto info = interfaces::weapon_system()->GetWpnData( reinterpret_cast< C_BaseAttributableItem* >( wpn )->get_item_definition_index() );
					if ( info )
					{
						wpn->get_next_primary_attack() = in_data_f + info->cycle_time;
						wpn->get_next_secondary_attack() = in_data_f + info->cycle_time_alt;
					}
				}
			}
		}
	}

	return original( transfer_data )( copy, edx, operation, entindex, dmap );
}

bool __fastcall hook::perform_prediction( uintptr_t prediction, uint32_t edx, int32_t slot, C_CSPlayer* player, bool recv_update, int32_t ack, int32_t out )
{
	const auto step_sounds = *interfaces::game_rules_proxy() ? &( *interfaces::game_rules_proxy() )->get_play_all_step_sounds_on_server() : nullptr;
	auto ret = false;
	if ( step_sounds )
	{
		const auto bak = *step_sounds;
		*step_sounds = false;
		ret = original( perform_prediction )( prediction, edx, slot, player, recv_update, ack, out );;
		*step_sounds = bak;
	}
	else
		ret = original( perform_prediction )( prediction, edx, slot, player, recv_update, ack, out );
	player->get_final_predicted_tick() = out;
	return ret;
}

//unused
bool __fastcall hook::MsgFunc_TextMsg( void* ecx, void* edx, void* msg )
{
	const auto msg_dest = *reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( msg ) + 0x18 );
	if ( msg_dest != 2 )
		return original( MsgFunc_TextMsg )( ecx, edx, msg );

	auto text = **reinterpret_cast< uintptr_t*** >( reinterpret_cast< uintptr_t >( msg ) + 0x8 );
	if ( text[ 5 ] >= 0x10u )
		text = reinterpret_cast< uintptr_t* >( *text );

	const auto str = reinterpret_cast< const char* >( text );
	if ( playerperf::handle_message( str ) )
		return true;

	return original( MsgFunc_TextMsg )( ecx, edx, msg );
}

#ifdef _DEBUG
void server_setupbones( void* player, matrix3x4_t* bones )
{
	//ccsplayer::setupbones
	using fn_t = void( __thiscall* )( void* thisptr, matrix3x4_t* bones, int32_t mask );
	static auto fn = ( fn_t ) sig( "server.dll", "55 8B EC 83 E4 F8 51 53 FF 75 0C 8B D9 FF 75 08" );

	fn( player, bones, BONE_USED_BY_ATTACHMENT | BONE_USED_BY_HITBOX );
}

using get_server_ent_t = uintptr_t( __fastcall* )( int32_t );
extern get_server_ent_t get_server_ent;

std::vector<CUserCmd> commands{};
constexpr auto ticks_to_choke = 13;
auto last_command_number = 0;

void __fastcall hook::run_command_server( void* ecx, void* edx, C_BasePlayer* player, CUserCmd* cmd, IMoveHelper* move_helper )
{
	if ( reinterpret_cast< uintptr_t >( player ) == get_server_ent( 2 ) )
	{
		if ( cmd->command_number == last_command_number )
		{
			commands.push_back( *cmd );
			return;
		}

		for ( auto& command : commands )
			original( run_command_server )( ecx, edx, player, &command, move_helper );

		commands.clear();

		last_command_number = cmd->command_number;
	}

	original( run_command_server )( ecx, edx, player, cmd, move_helper );

	ALIGN32 matrix3x4_t bones[ 256 ];
	server_setupbones( player, bones );

	auto index = -1;
	for ( size_t i = 1; i < 65; i++ )
	{
		if ( get_server_ent( i ) == reinterpret_cast< uintptr_t >( player ) )
		{
			index = i;
			break;
		}
	}

	if ( index != -1 )
		player_log::anim_debug_info[ index ].get_or_put( *reinterpret_cast< uint32_t* >( reinterpret_cast< uintptr_t >( player ) + 4296 ) - 1 ).emplace_server( bones );

}
#endif
