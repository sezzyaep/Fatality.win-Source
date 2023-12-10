#include "../include_cheat.h"

void clear_ref_counted_vector( CUtlVector<IRefCounted*>& vec )
{
	for ( auto& elem : vec )
	{
		if ( elem )
		{
			elem->unreference();
			elem = nullptr;
		}
	}
	vec.RemoveAll();
}

static CreateClientClassFn get_wearable_create_fn()
{
	auto clazz = interfaces::client()->GetAllClasses();

	_( CEconWearable, "CEconWearable" );

	while ( strcmp( clazz->m_pNetworkName, CEconWearable.c_str() ) )
		clazz = clazz->m_pNext;

	return clazz->m_pCreateFn;
}

void skinchanger::on_level_shudown()
{
	const auto our_glove = reinterpret_cast< C_BaseAttributableItem* >( interfaces::entity_list()->get_client_entity_from_handle( glove_handle ) );
	for ( const auto ent : *interfaces::entity_list() )
	{
		if ( ent->GetClientClass()->m_ClassID != ClassId::CEconWearable || ent != our_glove )
			continue;

		interfaces::client_leaf_system()->RemoveRenderable( ent->RenderHandle() );
		ent->SetDestroyedOnRecreateEntities();
		ent->Release();
		glove_handle = 0;
		break;
	}
}

void skinchanger::spawn_glove( const ClientFrameStage_t stage )
{
	if ( stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START )
		return;

	const auto wearables = local_player->get_wearables();

	auto glove = reinterpret_cast< C_BaseAttributableItem* >( interfaces::entity_list()->get_client_entity_from_handle( wearables[ 0 ] ) );
	const auto our_glove = reinterpret_cast< C_BaseAttributableItem* >( interfaces::entity_list()->get_client_entity_from_handle( glove_handle ) );

	if ( glove != our_glove && our_glove )
	{
		if ( glove )
		{
			interfaces::client_leaf_system()->RemoveRenderable( our_glove->RenderHandle() );
			our_glove->SetDestroyedOnRecreateEntities();
			our_glove->Release();
			glove_handle = 0;
		}
		else
		{
			wearables[ 0 ] = glove_handle;
			glove = our_glove;
		}
	}

	if ( !glove )
	{
		static auto create_wearable_fn = get_wearable_create_fn();

		const auto entry = interfaces::entity_list()->get_highest_entity_index() + 1;
		const auto serial = rand() % 0x1000;

		create_wearable_fn( entry, serial );
		glove = reinterpret_cast< C_BaseAttributableItem* >( interfaces::entity_list()->get_client_entity( entry ) );

		static const Vector new_pos = { 10000.f, 10000.f, 10000.f };
		glove->set_abs_origin( new_pos );
		//*reinterpret_cast< int* >( uintptr_t( glove ) + 0x64 ) = -1; // fix Invalid EconItemView message // shows gloves at ground
		glove_handle = wearables[ 0 ] = entry | serial << 16;

		interfaces::client_leaf_system()->CreateRenderableHandle( glove );
	}

	apply_glove();

	if ( glove )
	{
		for ( const auto ent : *interfaces::entity_list() )
		{
			if ( ent->GetClientClass()->m_ClassID != ClassId::CEconWearable || ent == glove )
				continue;

			if ( reinterpret_cast< C_BaseAttributableItem* >( ent )->get_account_id() == local_player->get_player_info().xuidlow )
			{
				interfaces::client_leaf_system()->RemoveRenderable( ent->RenderHandle() );
				ent->SetDestroyedOnRecreateEntities();
				ent->Release();
			}
		}
	}
}

void skinchanger::apply_glove()
{
	const auto glove = reinterpret_cast< C_BaseAttributableItem* >( interfaces::entity_list()->get_client_entity_from_handle( local_player->get_wearables()[ 0 ] ) );
	if ( !glove )
		return;

	auto& items = inventorychanger::items;
	inventorychanger::inventory_item_t* inv_item = nullptr;
	for ( auto& item : items )
		if ( item.enabled() && inventorychanger::is_glove( item.definition_index ) )
		{
			inv_item = &item;
			break;
		}

	if ( !inv_item )
		return;

	glove->get_item_id_high() = -1;

	glove->get_fallback_paint_kit() = inv_item->paint_kit;
	glove->get_fallback_seed() = inv_item->seed;
	glove->get_fallback_stat_trak() = inv_item->stat_trak;
	glove->get_fallback_wear() = clamp( inv_item->wear * 0.01f, FLT_MIN, 1.f );

	local_player->get_body() = 1;

	if ( models.contains( inv_item->definition_index ) && glove->get_item_definition_index() != inv_item->definition_index )
	{
		glove->get_item_definition_index() = inv_item->definition_index;
		_u( model_name, models[ inv_item->definition_index ].world_display_model );
		glove->set_model_idx( interfaces::model_info()->GetModelIndex( model_name.c_str() ) );
		glove->PreDataUpdate( DATA_UPDATE_CREATED );
		reinterpret_cast< int( __thiscall* )( void*, void* ) >( make_offset( "client.dll", sig_equip_glove ) )( glove, local_player );
		if ( inv_item->update )
			interfaces::client_state()->force_full_update();
	}

	inv_item->update = false;
}

struct hud_weapons_t
{
	int32_t* get_weapon_count()
	{
		return reinterpret_cast< int32_t* >( reinterpret_cast< uintptr_t >( this ) + 0x80 );
	}
};

void skinchanger::update_hud()
{
	const auto hud_weapon_selection = reinterpret_cast< uintptr_t >( interfaces::hud()->FindElement( XOR_STR( "CCSGO_HudWeaponSelection" ) ) );
	if ( !hud_weapon_selection )
		return;

	const auto hud_weapons = reinterpret_cast< hud_weapons_t* >( hud_weapon_selection - 0xA0 );

	if ( !hud_weapons )
		return;

	if ( !*hud_weapons->get_weapon_count() )
		return;

	for ( int32_t j = 0; j < *hud_weapons->get_weapon_count(); j++ )
		j = reinterpret_cast< int32_t( __thiscall* )( void*, int32_t ) >( make_offset( "client.dll", sig_clear_hud_weapon_icon ) )( hud_weapons, j );
}

void skinchanger::run( const ClientFrameStage_t stage )
{
	spawn_glove( stage );

	if ( stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START )
		return;

	model_changer();

	if ( !local_player || !local_player->get_alive() || !interfaces::engine()->IsInGame() )
		return;

	const auto weapons = local_player->get_weapons();
	for ( size_t i = 0; weapons[ i ] != INVALID_EHANDLE_INDEX; i++ )
	{
		const auto weapon = reinterpret_cast< C_WeaponCSBaseGun* >( interfaces::entity_list()->get_client_entity_from_handle( weapons[ i ] ) );
		if ( !weapon || weapon->get_owner_ent() == -1 )
			continue;

		const auto item = reinterpret_cast< C_BaseAttributableItem* >( weapon );

		const auto info = local_player->get_player_info();

		if ( info.xuidlow != item->get_xuid_low() )
			continue;

		if ( info.xuidhigh != item->get_xuid_high() )
			continue;

		auto& items = inventorychanger::items;
		inventorychanger::inventory_item_t* inv_item = nullptr;
		for ( auto& cur : items )
		{
			if ( cur.enabled() && ( cur.definition_index == item->get_item_definition_index() || inventorychanger::is_knife( cur.definition_index ) && inventorychanger::is_knife( weapon->get_weapon_id() ) ) )
				inv_item = &cur;

			if ( !cur.enabled() && cur.update )
			{
				cur.update = false;
				interfaces::client_state()->force_full_update();
			}
		}

		if ( !inv_item )
			continue;

		const auto item_changed = inv_item->definition_index != item->get_item_definition_index();

		if ( item_changed && models.contains( inv_item->definition_index ) )
		{
			item->get_item_definition_index() = inv_item->definition_index;
			_u( model_name, models.at( inv_item->definition_index ).base_display_model );
			weapon->set_model_idx( interfaces::model_info()->GetModelIndex( model_name.c_str() ) );
		}

		item->get_item_id_high() = -1;
		item->get_fallback_paint_kit() = inv_item->paint_kit;
		item->get_fallback_seed() = inv_item->seed;
		item->get_fallback_wear() = clamp( inv_item->wear * 0.01f, FLT_MIN, 1.f );

		const auto target_stattrak = inv_item->stat_trak == 10000 ? clamp( ( *interfaces::player_resource() )->get_kills( interfaces::engine()->GetLocalPlayer() ), 0, 999999 ) : inv_item->stat_trak;
		if ( item->get_fallback_stat_trak() != target_stattrak || inv_item->update )
		{
			item->get_fallback_stat_trak() = target_stattrak;
			if ( inv_item->update )
			{
				item->get_custom_material_initialized() = inv_item->paint_kit <= 0;
				clear_ref_counted_vector( item->get_custom_materials() );
				clear_ref_counted_vector( weapon->cmbat_get_econ_item_view()->get_custom_materials() );
				clear_ref_counted_vector( weapon->cmbat_get_econ_item_view()->get_visual_data_processors() );
				if ( item_changed )
				{
					interfaces::client_state()->force_full_update();
					update_hud();
				}
			}
			item->PostDataUpdate( DATA_UPDATE_CREATED );
			item->OnDataChanged( DATA_UPDATE_CREATED );
		}

		inv_item->update = false;

		if ( !item_changed || !weapon->is_knife() )
			continue;

		const auto weapon_info = interfaces::weapon_system()->GetWpnData( weapon->get_weapon_id() );
		if ( !weapon_info )
			continue;

		const auto world_model_weapon = reinterpret_cast< C_BaseWeaponWorldModel* >( interfaces::entity_list()->get_client_entity_from_handle( weapon->get_world_model_handle() ) );
		if ( world_model_weapon )
			world_model_weapon->set_model_idx( interfaces::model_info()->GetModelIndex( weapon_info->szworldmodel ) );
	}

	const auto view_model_handle = local_player->get_view_model();
	if ( view_model_handle == INVALID_EHANDLE_INDEX )
		return;

	const auto view_model = reinterpret_cast< C_BaseViewModel* >( interfaces::entity_list()->get_client_entity_from_handle( view_model_handle ) );
	if ( !view_model )
		return;

	const auto view_model_weapon_handle = view_model->get_weapon();
	if ( view_model_weapon_handle == INVALID_EHANDLE_INDEX )
		return;

	const auto view_model_weapon = reinterpret_cast< C_BaseAttributableItem* >( interfaces::entity_list()->get_client_entity_from_handle( view_model_weapon_handle ) );
	if ( !view_model_weapon )
		return;

	if ( reinterpret_cast< C_WeaponCSBaseGun* >( view_model_weapon )->is_knife() && models.contains( view_model_weapon->get_item_definition_index() ) )
	{
		const auto& override_model = models.at( view_model_weapon->get_item_definition_index() ).base_display_model;
		_u( model_name, override_model );

		const auto new_model_index = interfaces::model_info()->GetModelIndex( model_name.c_str() );

		if ( view_model->get_model_idx() != new_model_index )
			for ( auto seq = 0; seq < 16; seq++ )
				original_activity[ seq ] = view_model->get_sequence_act( seq );

		view_model->set_model_idx( new_model_index );
	}
}

void skinchanger::model_changer()
{
	if ( !local_player || !local_player->get_alive() || !interfaces::engine()->IsInGame() )
		return;

	const auto new_model = local_player->get_team() == 2 ? vars::visuals.model_t->get<int>() : vars::visuals.model_ct->get<int>();

	if ( !new_model )
	{
		if ( server_model_idx != local_player->get_model_idx() && server_model_idx != -1 )
			local_player->set_model_idx( server_model_idx );
		return;
	}


	_u( model_name, player_model_list.at( new_model ) );
	auto new_model_index = interfaces::model_info()->GetModelIndex( model_name.c_str() );

	if ( new_model_index && local_player->get_model_idx() == new_model_index )
		return;

	util::precache_model( model_name, false );
	new_model_index = interfaces::model_info()->GetModelIndex( model_name.c_str() );

	if ( const auto arm_cfg = reinterpret_cast< viewmodel_arm_config * ( __thiscall* )( const char* ) >( make_offset( "client.dll", sig_get_player_viewmodel_arm_config_for_player_model ) )( model_name.c_str() ) )
	{
		if ( arm_cfg->glove_model && arm_cfg->glove_model[ 0 ] )
			util::precache_model( arm_cfg->glove_model, false );
		if ( arm_cfg->sleeve_model && arm_cfg->sleeve_model[ 0 ] )
			util::precache_model( arm_cfg->sleeve_model, false );
	}

	local_player->set_model_idx( new_model_index );
}

skinchanger::paint_kit_data_s skinchanger::find_paintkit( int32_t paint_kit )
{
	// loop all entities with skins
	for ( auto& weapons : weapon_infos )
	{
		// loop all paint kits for entity
		for ( auto& skin : weapons.second.paint_kits )
		{
			_u( name, skin.name );

			// skip fucked up ones
			if ( name.empty() )
				continue;

			if ( paint_kit == skin.paint_kit )
				return skin;
		}
	}

	return {};
}

#pragma clang section text=".discard"

__declspec( noinline ) void skinchanger::parse_weapon_names()
{
	for ( int id = 0; id <= static_cast< int >( WEAPON_KNIFE_SKELETON ); id++ )
	{
		if ( !weapon_infos.contains( id ) )
			continue;

		const auto weapon_data = interfaces::weapon_system()->GetWpnData( id );

		if ( !weapon_data )
			continue;

		std::wstring localized_name = interfaces::localize()->FindSafe( weapon_data->szhudname );

		weapon_infos[ id ].item_name = _w2( std::string( weapon_data->consolename ) );

		if ( !localized_name.empty() )
			weapon_infos[ id ].name = _w2( util::utf8_encode( localized_name ) );
	}

	erase_end;
}

__declspec( noinline ) void skinchanger::parse_kits()
{
	auto sys = *reinterpret_cast< econ_item_system** >( make_offset( "client.dll", sig_econ_item_system ) );
	if ( !sys )
		return;

	auto item_schema = sys->get_item_schema_interface();

	if ( !item_schema )
		return;

	auto items = &item_schema->items_sorted;

	for ( int iterator = 0; iterator < items->max_element(); ++iterator )
	{
		if ( !items->is_valid_index( iterator ) )
			continue;

		auto elem = items->element( iterator );

		if ( !elem || !elem->kv_item )
			continue;

		auto prefabKv = elem->kv_item->find_key( XOR_STR( "prefab" ) );

		if ( !prefabKv )
			continue;

		const auto prefabHash = fnv1a_rt( static_cast< const char* >( prefabKv->get_ptr() ) );

		// NOTE: add knife, glove and player models into models
		// melee & weapon_melee_prefab = default knives, why does valve have 2 types of prefabs for the default knives...
		// melee_unusual = special knives
		// hands_paintable = gloves
		// customplayertradable = agents

		if ( prefabHash != FNV1A( "melee" )
			&& prefabHash != FNV1A( "weapon_melee_prefab" )
			&& prefabHash != FNV1A( "melee_unusual" )
			&& prefabHash != FNV1A( "hands_paintable" ) )
			continue;

		std::string localized_name = util::utf8_encode( interfaces::localize()->FindSafe( elem->item_base_name ) );

		if ( prefabHash == FNV1A( "hands_paintable" ) )
		{
			glove_def_indices.emplace_back( int32_t( elem->def_index ) );

			weapon_infos[ int32_t( elem->def_index ) ].item_name = _w2( std::string( elem->item_base_name ) );
			weapon_infos[ int32_t( elem->def_index ) ].name = _w2( localized_name );
		}

		models[ int32_t( elem->def_index ) ] = { _w2( std::string( elem->definition_name ) ), _w2( std::string( elem->base_display_model ) ),
			elem->world_display_model ? _w2( std::string( elem->world_display_model ) ) : std::string() };
	}

	std::unordered_map<int32_t, std::vector<std::string>> knife_skins;

	auto* alternate_icon_data = &item_schema->alternate_icons;

	// parse knife skins
	for ( int i = 0; i < alternate_icon_data->max_element(); ++i )
	{
		if ( !alternate_icon_data->is_valid_index( i ) )
			continue;

		auto elem = &alternate_icon_data->element( i );

		if ( !elem || !elem->simple_name.Get() || !elem->simple_name.Get()[ 0 ] )
			continue;

		std::string name = elem->simple_name.Get();

		const auto slash_pos = name.rfind( '/' );

		if ( slash_pos != std::string::npos )
			name.erase( 0, slash_pos + 1 );

		// each skin exists 3 times (light, medium, heavy) in that list, we only need one
		if ( name.find( XOR_STR( "light" ) ) == std::string::npos )
			continue;

		// erase "_light" from the end
		name.erase( name.length() - 6, 6 );

		auto item_it = std::find_if(
				models.begin(),
				models.end(),
				[name] ( const auto& first )
		{
			if ( first.first == WEAPON_KNIFE
				|| first.first == WEAPON_KNIFE_T )
				return false;

			_u( str, first.second.definition_name );
			return strstr( name.data(), str.data() ) != nullptr;
		}
		);

		if ( item_it == models.end() )
			continue;

		_u( str, item_it->second.definition_name );
		name.erase( 0, str.length() + 1 );

		knife_skins[ item_it->first ].emplace_back( _w2( name ) );
	}

	std::unordered_map<uint32_t, std::pair<int32_t, std::string>> skin_ids;

	auto paint_kits = &item_schema->paint_kits;

	for ( auto i = 0; i < paint_kits->max_element(); ++i )
	{
		if ( !paint_kits->is_valid_index( i ) )
			continue;

		auto elem = paint_kits->element( i );

		if ( !elem || !elem->name.Get() )
			continue;

		std::string skin_name = elem->name.Get();
		std::string localized_name = util::utf8_encode( interfaces::localize()->FindSafe( elem->description_tag.Get() ) );

		if ( skin_name.find( XOR_STR( "ruby" ) ) != std::string::npos )
			localized_name.append( XOR_STR( " Ruby" ) );

		if ( skin_name.find( XOR_STR( "sapphire" ) ) != std::string::npos )
			localized_name.append( XOR_STR( " Sapphire" ) );

		if ( skin_name.find( XOR_STR( "emerald" ) ) != std::string::npos )
			localized_name.append( XOR_STR( " Emerald" ) );

		if ( skin_name.find( XOR_STR( "blackpearl" ) ) != std::string::npos )
			localized_name.append( XOR_STR( " Blackpearl" ) );

		const auto phasePos = skin_name.find( XOR_STR( "phase" ) );

		// NOTE: this thing converts the ascii number to a number and back to a widestring to append it
		if ( phasePos != std::string::npos )
			localized_name.append( XOR_STR( " Phase " ) + std::to_string( std::strtol( skin_name.substr( phasePos + 5, phasePos + 6 ).data(), nullptr, 10 ) ) );

		skin_ids.emplace( std::pair<uint32_t, std::pair<int32_t, std::string>>( elem->id, std::pair<int32_t, std::string>( elem->rarity, _w2( localized_name ) ) ) );
	}

	auto item_sets = &item_schema->item_sets;

	for ( auto i = 0; i < item_sets->max_element(); ++i )
	{
		if ( !item_sets->is_valid_index( i ) )
			continue;

		auto item_set = &item_sets->element( i );

		if ( !item_set )
			continue;

		for ( auto y = 0; y < item_set->item_entries.Count(); ++y )
		{
			const auto& item = item_set->item_entries[ y ];

			if ( item.item_def <= 0 || item.item_def > size_t( GLOVE_HYDRA )
				|| item.item_def > size_t( WEAPON_KNIFE_SKELETON ) && item.item_def < size_t( GLOVE_STUDDED_BLOODHOUND ) && item.item_def != GLOVE_BROKEN_FANG )
				continue;

			weapon_infos[ int32_t( item.item_def ) ].paint_kits.emplace_back(
					paint_kit_data_s{ item.paint_kit, skin_ids[ item.paint_kit ].first, skin_ids[ item.paint_kit ].second } );
		}
	}

	// NOTE: apply fixes
	if ( !skin_ids.empty() )
	{
		// NOTE: howl fix
		weapon_infos[ size_t( WEAPON_M4A1 ) ].paint_kits.emplace_back( paint_kit_data_s{ 309, 7, skin_ids[ 309 ].second } );

		// NOTE: all of those listed weapons have fucked rarities
		constexpr std::array items_to_fix =
		{
			WEAPON_DEAGLE,
			WEAPON_GLOCK,
			WEAPON_AK47,
			WEAPON_AWP,
			WEAPON_M4A1,
			WEAPON_HKP2000,
			WEAPON_M4A1_SILENCER,
			WEAPON_USP_SILENCER
		};

		for ( const auto& id : items_to_fix )
		{
			for ( auto& skin : weapon_infos[ size_t( id ) ].paint_kits )
			{
				if ( skin.rarity > 0 && skin.rarity < 6 )
					skin.rarity++;
			}
		}

		// NOTE: knife fix
		for ( auto& wep : knife_skins )
		{
			// NOTE: also add vanilla knives
			if ( wep.first <= int32_t( WEAPON_KNIFE_SKELETON ) )
				weapon_infos[ wep.first ].paint_kits.emplace_back( paint_kit_data_s{ 0, 6, _w( "-" ) } );

			for ( auto& skinName : wep.second )
			{
				_u( str, skinName );
				for ( auto i = 0; i < paint_kits->max_element(); ++i )
				{
					if ( !paint_kits->is_valid_index( i ) )
						continue;

					auto elem = paint_kits->element( i );

					if ( !elem || !elem->name.Get() )
						continue;

					if ( str == elem->name.Get() )
					{
						weapon_infos[ wep.first ].paint_kits.emplace_back( paint_kit_data_s{ elem->id, 6, skin_ids[ elem->id ].second } );
						break;
					}
				}
			}
		}
	}

	erase_end;
}

#pragma clang section text=""

void skinchanger::on_addon_bits( C_CSPlayer* player, int32_t& bits )
{
	if ( const auto new_mask = local_player->get_team() == 2 ? vars::visuals.mask_t->get<int>() : vars::visuals.mask_ct->get<int>() )
	{
		auto& model_name = player_mask_list_raw.at( new_mask );
		auto& current = *reinterpret_cast< const char** >( make_offset( "client.dll", sig_client_side_addons_mask ) );

		if ( fnv1a_rt( current ) != fnv1a_rt( model_name )
			|| !interfaces::entity_list()->get_client_entity_from_handle( player->get_assassination_target_addon() ) )
		{
			util::precache_model( model_name, false );
			current = model_name.c_str();
			player->get_addon_bits() = bits;
			player->update_addon_models( true );
		}

		bits |= 0x10000;
	}
}
