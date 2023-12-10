#include "../include_cheat.h"
#include <intrin.h>

static auto do_sequence_remapping( CRecvProxyData* data, C_BaseViewModel* entity ) -> void
{
	if ( !local_player )
		return;

	if ( !local_player->get_alive() )
		return;

	const auto owner = reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity_from_handle( entity->get_owner() ) );
	if ( owner != local_player )
		return;

	const auto item = reinterpret_cast< C_BaseAttributableItem* >( interfaces::entity_list()->get_client_entity_from_handle( entity->get_weapon() ) );
	if ( !item )
		return;

	const auto weapon = reinterpret_cast< C_WeaponCSBaseGun* >( interfaces::entity_list()->get_client_entity_from_handle( entity->get_weapon() ) );

	auto& items = inventorychanger::items;
	inventorychanger::inventory_item_t* inv_item = nullptr;
	for ( auto& cur : items )
	{
		if ( cur.enabled() && cur.definition_index == item->get_item_definition_index() && weapon->is_knife() )
		{
			inv_item = &cur;
			break;
		}
	}

	auto& sequence = data->m_Value.m_Int;

	if ( !inv_item )
		return;

	if ( skinchanger::models.contains( item->get_item_definition_index() ) )
	{
		auto& override_model = skinchanger::models.at( item->get_item_definition_index() ).base_display_model;
		_u( model_name, override_model );

		const auto new_model_index = interfaces::model_info()->GetModelIndex( model_name.c_str() );

		if ( entity->get_model_idx() != new_model_index )
		{
			for ( auto seq = 0; seq < entity->get_model_ptr()->get_numseq(); seq++ )
				skinchanger::original_activity[ seq ] = entity->get_sequence_act( seq );

			entity->set_model_idx( new_model_index );

			if ( skinchanger::models.contains( inv_item->definition_index ) )
			{
				item->get_item_definition_index() = inv_item->definition_index;
				_u( model_name, skinchanger::models.at( inv_item->definition_index ).base_display_model );
				weapon->set_model_idx( interfaces::model_info()->GetModelIndex( model_name.c_str() ) );
			}
		}
	}

	// what act did the original knife trigger
	const auto wanted_act = skinchanger::original_activity[ sequence ];

	std::vector<int> possible_sequences;

	// loop through all sequences
	for ( auto i = 0; i < entity->get_model_ptr()->get_numseq(); i++ )
	{
		// get act for sequence
		const auto cur_act = entity->get_sequence_act( i );

		// wanted and current act are the same. second check is for lookat animations (default lookat has act -1)
		if ( wanted_act != -1 && cur_act == wanted_act || cur_act == 213 && wanted_act == -1 )
		{
			possible_sequences.emplace_back( i );
			if ( possible_sequences.size() == 1 )
				for ( auto j = 0; j < 5; j++ )
					possible_sequences.emplace_back( i );
		}
	}

	// we found a valid sequence
	if ( !possible_sequences.empty() )
	{
		globals::random_seed( interfaces::globals()->tickcount );
		// we found multiple sequences, pick a random one
		if ( possible_sequences.size() > 1 )
			sequence = possible_sequences[ globals::random_int( 0, static_cast< int >( possible_sequences.size() ) - 1 ) ];
		else
			sequence = possible_sequences[ 0 ];
	}
}

void __cdecl hook::weapon_handle( CRecvProxyData* data, void* entity, void* output )
{
	if ( !local_player || !local_player->get_alive() )
		return original( weapon_handle )( data, entity, output );

	const auto view_model = reinterpret_cast< C_BaseCombatWeapon* >( entity );

	if ( data->m_Value.m_Int == 0x1FFFFF )
		*reinterpret_cast< int32_t* >( output ) = -1;
	else
		*reinterpret_cast< int32_t* >( output ) = data->m_Value.m_Int & 0x7FF | ( data->m_Value.m_Int >> 11 << 16 );

	if ( prediction::get_pred_info( interfaces::client_state()->command_ack ).wpn != *reinterpret_cast< int32_t* >( output ) )
	{
		view_model->get_cycle() = view_model->get_cycle_offset() = 0.f;
		view_model->get_anim_time() = ticks_to_time( interfaces::client_state()->command_ack );
	}
}

void __cdecl hook::sequence( CRecvProxyData* data, void* entity, void* output )
{
	if ( !local_player || !local_player->get_alive() )
		return original( sequence )( data, entity, output );

	const auto view_model = reinterpret_cast< C_BaseViewModel* >( entity );

	do_sequence_remapping( data, view_model );

	const auto act = view_model->get_sequence_act( data->m_Value.m_Int );

	if ( act == view_model->get_sequence_act( *reinterpret_cast< int32_t* >( output ) ) )
	{
		data->m_Value.m_Int = *reinterpret_cast< int32_t* >( output );
		return;
	}
	

	*reinterpret_cast< int32_t* >( output ) = data->m_Value.m_Int;
	// we do not use the cycle offset. instead, we make the animation start align.
	view_model->get_cycle_offset() = 0.f;

	if ( view_model->lookup_sequence( _r( "lookat01" ) ) == data->m_Value.m_Int
		|| act == 213 ) // ACT_VM_IDLE_LOWERED
		view_model->get_anim_time() = ticks_to_time( interfaces::client_state()->command_ack );
}

int __fastcall hook::set_bodygroup( void* ecx, void* edx, int group, int value )
{
	if ( reinterpret_cast< uintptr_t >( _ReturnAddress() ) == make_offset( "client.dll", sig_ret_to_add_viewmodel_stattrak ) )
		return 0;

	return original( set_bodygroup )( ecx, edx, group, value );
}

void __cdecl hook::effects( CRecvProxyData* data, void* entity, void* output )
{
	const auto ent = reinterpret_cast< C_BaseWeaponWorldModel* >( entity );

	const auto old = ent->get_effects();
	original( effects )( data, entity, output );

	// client-controlled.
	if ( ent->GetClientClass()->m_ClassID == ClassId::CBaseWeaponWorldModel )
	{
		const auto parent = globals::get_weapon( ent->get_parent() );
		const auto owner = parent ? reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity_from_handle( parent->get_owner_ent() ) ) : nullptr;

		if ( owner && owner->is_player() && owner->get_alive() && owner->EntIndex() == interfaces::engine()->GetLocalPlayer() )
			ent->get_effects() = ( ent->get_effects() & ~EF_NODRAW ) | ( old & EF_NODRAW );
	}
}