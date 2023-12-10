#include "../include_cheat.h"
#include <fstream>

inline std::vector<listbox_item_t> menu_items = {};
inline std::vector<std::string> weapon_menu_items{};
inline std::vector<listbox_item_t> list_items = {};

bool inventorychanger::is_knife( const int idx )
{
	return ( idx >= WEAPON_KNIFE_BAYONET && idx <= WEAPON_KNIFE_SKELETON || idx == WEAPON_KNIFE_T || idx == WEAPON_KNIFE );
}

bool inventorychanger::is_glove( const int idx )
{
	return idx >= GLOVE_STUDDED_BLOODHOUND && idx <= GLOVE_HYDRA || idx == GLOVE_BROKEN_FANG;
}

bool inventorychanger::same_item( const int idx, const int idx2 )
{
	return idx == idx2 || is_glove( idx ) && is_glove( idx2 ) || is_knife( idx ) && is_knife( idx2 );
}

#pragma clang section text=".discard"

__declspec( noinline ) void inventorychanger::fill_weapon_list()
{
	for ( auto& item : skinchanger::weapon_infos )
	{
		weapon_menu_items.push_back( item.second.name );
		weapon_menu_items_data.push_back( item.first );
	}

	erase_end;
}

#pragma clang section text=""

auto get_color_for_rarity = [] ( int32_t rarity ) -> color
{
	static const std::array rarity_colors = {
		color( 190, 195, 205 ),
		color( 100, 150, 225 ),
		color( 75, 105, 205 ),
		color( 135, 70, 255 ),
		color( 210, 45, 230 ),
		color( 235, 75, 75 ),
		color( 200, 170, 5 ),
		color( 135, 105, 10 ),
	};

	return rarity_colors[ rarity == 7 ? rarity : rarity - 1 ];
};

void inventorychanger::fill_skin_list( std::string& search )
{
	static const auto find_case_insensitive = [] ( std::string data, std::string search, size_t pos = 0 )
	{
		std::transform( data.begin(), data.end(), data.begin(), ::tolower );
		std::transform( search.begin(), search.end(), search.begin(), ::tolower );

		return data.find( search, pos );
	};

	static auto last_selected_weapon = -1;
	static auto last_show_all_skins = false;
	static auto last_search = std::string{};

	if ( weapon_menu_items_data.empty() )
		return;

	// backup old selected value for user experience
	const auto selected_weapon = weapon_menu_items_data[ vars::inventory.definition_index->get<int>() ];
	const auto selected_paint_kit = vars::inventory.paint_kit->get<int>() < menu_items_data.size() ? menu_items_data[ vars::inventory.paint_kit->get<int>() ].paint_kit : 0;
	const auto show_all_skins = vars::inventory.show_all_skins->get<bool>();

	if ( last_selected_weapon == selected_weapon && last_search == search && last_show_all_skins == show_all_skins )
		return;

	last_selected_weapon = selected_weapon;
	last_search = search;
	last_show_all_skins = show_all_skins;

	menu_items_data.clear();
	menu_items.clear();

	if ( !items.empty() )
	{
		const auto& item = items[ vars::menu.inventory_list_idx->get<int>() ];
		if ( item.definition_index == selected_weapon )
		{
			auto pk = skinchanger::find_paintkit( item.paint_kit );
			_u( name, pk.name );
			if ( find_case_insensitive( name, search ) != std::string::npos )
				menu_items_data.emplace_back( pk );
		}
	}

	// for emplacing selected paintkit not in list
	auto move = !menu_items_data.empty();

	// loop all entities with skins
	for ( auto& weapons : skinchanger::weapon_infos )
	{
		// skip skins not meant for the selected entity
		if ( weapons.first != selected_weapon && ( !show_all_skins || weapons.first >= static_cast< int >( GLOVE_BROKEN_FANG ) ) )
			continue;

		// loop all paint kits for entity
		for ( auto& skin : weapons.second.paint_kits )
		{
			_u( name, skin.name );

			// skip fucked up ones
			if ( name.empty() )
				continue;

			if ( move && items[ vars::menu.inventory_list_idx->get<int>() ].paint_kit == skin.paint_kit )
			{
				move = false;
				menu_items_data.erase( menu_items_data.begin() );
			}

			if ( find_case_insensitive( name, search ) != std::string::npos )
				menu_items_data.emplace_back( skin );
		}
	}

	if ( show_all_skins && selected_weapon < static_cast< int >( GLOVE_BROKEN_FANG ) )
	{
		auto comp = [] ( const skinchanger::paint_kit_data_s& lhs, const skinchanger::paint_kit_data_s& rhs ) { return lhs.paint_kit == rhs.paint_kit; };
		auto pred = [] ( const skinchanger::paint_kit_data_s& lhs, const skinchanger::paint_kit_data_s& rhs ) { return lhs.paint_kit < rhs.paint_kit; };
		std::ranges::sort( menu_items_data, pred );
		const auto last = std::ranges::unique( menu_items_data, comp ).begin();
		menu_items_data.erase( last, menu_items_data.end() );
	}

	auto start = menu_items_data.begin();
	if ( move )
		++start;


	// sort by order (rarity) and alphabetically
	std::sort( start, menu_items_data.end(), [] ( const skinchanger::paint_kit_data_s& l, const skinchanger::paint_kit_data_s& r )
	{
		if ( l.rarity < r.rarity ) return false;
		if ( l.rarity > r.rarity ) return true;

		_u( lname, l.name );
		_u( rname, r.name );
		if ( lname < rname ) return true;
		return false;
	} );

	auto index = 0;
	auto has_found = false;
	for ( auto& el : menu_items_data )
	{
		if ( !has_found && el.paint_kit == selected_paint_kit )
		{
			has_found = true;
			vars::inventory.paint_kit->set( index );
			paint_kit_listbox.lock()->adjust_scroll();
		}

		menu_items.emplace_back( el.name, menu_items.size(), get_color_for_rarity( el.rarity ) );
		++index;
	}

	if ( !has_found )
	{
		vars::inventory.paint_kit->set( 0 );
		paint_kit_listbox.lock()->adjust_scroll();
	}


	if ( auto lb = paint_kit_listbox.lock() )
		lb->reset_scroll();
}

namespace lua::gui_helpers
{
	template <typename T = control>
	std::shared_ptr<T> find_control( runtime_state& s, const std::string& path );
}

void inventorychanger::update_menu_items()
{
	// sort alphabetically
	std::sort( items.begin(), items.end(), [] ( const inventory_item_t& l, const inventory_item_t& r )
	{
		if ( l.definition_index > r.definition_index ) return false;
		if ( l.definition_index < r.definition_index ) return true;

		//if ( static_cast< int >( l.enabled_ct ) + l.enabled_t > static_cast< int >( r.enabled_ct ) + r.enabled_t ) return true;
		//if ( static_cast< int >( l.enabled_ct ) + l.enabled_t < static_cast< int >( r.enabled_ct ) + r.enabled_t ) return false;

		if ( skinchanger::find_paintkit( l.paint_kit ).rarity > skinchanger::find_paintkit( r.paint_kit ).rarity ) return true;
		return false;
	} );

	list_items.clear();

	for ( auto& item : items )
	{
		auto paint_kit = skinchanger::find_paintkit( item.paint_kit );

		_u( name, skinchanger::weapon_infos[ item.definition_index ].name );
		_u( kit, paint_kit.name );

		name.append( xorstr_( " (" ) ).append( kit ).append( xorstr_( ")" ) );
		if ( item.enabled_ct )
			name.append( xorstr_( " [CT]" ) );
		if ( item.enabled_t )
			name.append( xorstr_( " [T]" ) );

		list_items.emplace_back( _w2( name ), list_items.size(), get_color_for_rarity( paint_kit.rarity ) );
	}
}

void inventorychanger::update_selected( const bool force )
{
	if ( vars::inventory.autoselect_weapon->get<bool>() && interfaces::engine()->IsInGame() )
	{
		weapons_listbox.lock()->deactivate();
		weapons_listbox.lock()->get_search().clear();
	}
	else
		weapons_listbox.lock()->activate();

	if ( items.empty() )
		return;

	if ( vars::menu.inventory_list_idx->get<int>() >= items.size() )
		vars::menu.inventory_list_idx->set( items.size() - 1 );

	static auto oldidx = -1;

	if ( vars::menu.inventory_list_idx->get<int>() == oldidx && !force )
		return;

	oldidx = vars::menu.inventory_list_idx->get<int>();

	const auto& item = items[ vars::menu.inventory_list_idx->get<int>() ];

	if ( !vars::inventory.autoselect_weapon->get<bool>() || !interfaces::engine()->IsInGame() )
	{
		auto wpn_idx = 0;
		for ( const auto& curr : weapon_menu_items_data )
		{
			if ( curr == item.definition_index )
				break;

			wpn_idx++;
		}

		vars::inventory.definition_index->set( wpn_idx );
		weapons_listbox.lock()->adjust_scroll();
	}

	auto search_term = std::string{};
	auto search = paint_kit_listbox.lock()->get_search();
	if ( !search.empty() )
		search_term = std::string( search.begin(), search.end() );

	fill_skin_list( search_term );

	auto kit_idx = 0;
	for ( const auto& curr : menu_items_data )
	{
		if ( curr.paint_kit == item.paint_kit )
			break;

		kit_idx++;
	}

	if ( kit_idx < menu_items_data.size() )
		vars::inventory.paint_kit->set( kit_idx );
	else
		vars::inventory.paint_kit->set( 0 );
	paint_kit_listbox.lock()->adjust_scroll();

	vars::inventory.stat_trak->set( item.stat_trak );
	vars::inventory.seed->set( item.seed );
	vars::inventory.wear->set( item.wear );
	vars::inventory.enabled_ct->set( item.enabled_ct );
	vars::inventory.enabled_t->set( item.enabled_t );
}

void inventorychanger::add()
{
	inventory_item_t item;
	item.definition_index = weapon_menu_items_data[ vars::inventory.definition_index->get<int>() ];
	item.paint_kit = menu_items_data[ vars::inventory.paint_kit->get<int>() ].paint_kit;
	item.stat_trak = vars::inventory.stat_trak->get<int>();
	item.seed = vars::inventory.seed->get<int>();
	item.wear = vars::inventory.wear->get<float>();
	item.enabled_ct = vars::inventory.enabled_ct->get<bool>();
	item.enabled_t = vars::inventory.enabled_t->get<bool>();
	item.update = true;

	if ( item.enabled_ct || item.enabled_t )
	{
		for ( auto& curr : items )
		{
			if ( ( item.enabled_ct && curr.enabled_ct || item.enabled_t && curr.enabled_t ) && same_item( curr.definition_index, item.definition_index ) )
			{
				if ( item.enabled_ct )
					curr.enabled_ct = false;

				if ( item.enabled_t )
					curr.enabled_t = false;
			}
		}
	}

	items.push_back( item );

	update_menu_items();

	vars::menu.inventory_list_idx->set<int>( std::ranges::find( items, item ) - items.begin() );

	update_selected( true );

	save();
}

void inventorychanger::set( const int index, const bool force )
{
	if ( index >= items.size() )
		return;

	const auto item = &items[ index ];

	if ( !force && vars::inventory.autoselect_weapon->get<bool>() && interfaces::engine()->IsInGame() && item->definition_index != weapon_menu_items_data[ vars::inventory.definition_index->get<int>() ] )
		return;

	item->definition_index = weapon_menu_items_data[ vars::inventory.definition_index->get<int>() ];
	item->paint_kit = menu_items_data[ vars::inventory.paint_kit->get<int>() ].paint_kit;
	item->seed = vars::inventory.seed->get<int>();
	item->wear = vars::inventory.wear->get<int>();
	item->stat_trak = vars::inventory.stat_trak->get<int>();
	item->enabled_ct = vars::inventory.enabled_ct->get<bool>();
	item->enabled_t = vars::inventory.enabled_t->get<bool>();
	item->update = true;

	if ( item->enabled_ct || item->enabled_t )
	{
		for ( auto& curr : items )
		{
			if ( ( item->enabled_ct && curr.enabled_ct || item->enabled_t && curr.enabled_t ) && same_item( curr.definition_index, item->definition_index ) && &curr != item )
			{
				if ( item->enabled_ct )
					curr.enabled_ct = false;

				if ( item->enabled_t )
					curr.enabled_t = false;
			}
		}
	}

	const auto it = items.begin() + index;

	update_menu_items();

	vars::menu.inventory_list_idx->set<int>( std::distance( items.begin(), it ) );
}

void __declspec( noinline ) inventorychanger::load()
{
	auto is_file_existant = [] ( const char* file_name )
	{
		std::ifstream infile( file_name, std::ios::binary | std::ios::ate );
		return infile.good() && infile.tellg();
	};

	_( inventory, "skins" );

	if ( !is_file_existant( inventory.c_str() ) )
		return;

	std::ifstream file( inventory );

	std::string line;

	items.clear();
	list_items.clear();
	while ( file.good() )
	{
		std::getline( file, line );
		std::string definition_index, paint_kit, seed, stat_trak, wear, enabled_ct, enabled_t;

		if ( line.empty() )
			break;

		size_t pos = line.find( '|' );
		if ( pos != std::string::npos )
		{
			highest_id = ( uint64_t ) std::atoll( line.substr( 0, pos ).c_str() );
			line.erase( 0, pos + 1 );
		}

		pos = line.find( '#' );
		if ( pos == std::string::npos )
		{
			log( "[WARNING] corrupted inventory item removed\n" );
			continue;
		}
		definition_index = line.substr( 0, pos );
		line.erase( 0, pos + 1 );

		pos = line.find( '#' );
		if ( pos == std::string::npos )
		{
			log( "[WARNING] corrupted inventory item removed\n" );
			continue;
		}
		paint_kit = line.substr( 0, pos );
		line.erase( 0, pos + 1 );

		pos = line.find( '#' );
		if ( pos == std::string::npos )
		{
			log( "[WARNING] corrupted inventory item removed\n" );
			continue;
		}
		seed = line.substr( 0, pos );
		line.erase( 0, pos + 1 );

		pos = line.find( '#' );
		if ( pos == std::string::npos )
		{
			log( "[WARNING] corrupted inventory item removed\n" );
			continue;
		}
		stat_trak = line.substr( 0, pos );
		line.erase( 0, pos + 1 );

		pos = line.find( '#' );
		if ( pos == std::string::npos )
		{
			log( "[WARNING] corrupted inventory item removed\n" );
			continue;
		}
		wear = line.substr( 0, pos );
		line.erase( 0, pos + 1 );

		pos = line.find( '#' );
		if ( pos == std::string::npos )
		{
			log( "[WARNING] corrupted inventory item removed\n" );
			continue;
		}
		enabled_ct = line.substr( 0, pos );
		line.erase( 0, pos + 1 );

		enabled_t = line;

		inventory_item_t item{};
		item.definition_index = std::atoi( definition_index.c_str() );
		item.paint_kit = std::atoi( paint_kit.c_str() );
		item.seed = std::atoi( seed.c_str() );
		item.stat_trak = std::atoi( stat_trak.c_str() );
		item.wear = std::atof( wear.c_str() );
		item.enabled_ct = std::atoi( enabled_ct.c_str() );
		item.enabled_t = std::atoi( enabled_t.c_str() );
		item.update = false;

		items.push_back( item );
	}

	file.close();

	update_menu_items();
}

void inventorychanger::save( const bool force )
{
	set( vars::menu.inventory_list_idx->get<int>(), force );

	_( inventory, "skins" );

	std::string outstr;
	outstr.append( std::to_string( highest_id ) + '|' );

	for ( const auto& item : items )
		outstr.append( std::to_string( item.definition_index ) + '#' ).append( std::to_string( item.paint_kit ) + '#' ).append( std::to_string( item.seed ) + '#' ).append( std::to_string( item.stat_trak ) + '#' ).append( std::to_string( item.wear ) + '#' ).append( std::to_string( item.enabled_ct ) + '#' ).append( std::to_string( item.enabled_t ) + '\n' );

	std::ofstream file( inventory );
	file.clear();
	file.write( outstr.c_str(), outstr.size() );
	file.close();
}

void inventorychanger::remove()
{
	if ( items.empty() )
		return;

	if ( vars::menu.inventory_list_idx->get<int>() >= items.size() )
		return;

	items.erase( items.begin() + vars::menu.inventory_list_idx->get<int>() );
	list_items.erase( list_items.begin() + vars::menu.inventory_list_idx->get<int>() );

	if ( vars::menu.inventory_list_idx->get<int>() > 0 )
		vars::menu.inventory_list_idx->set<int>( vars::menu.inventory_list_idx->get<int>() - 1 );

	update_selected( true );

	save();

	interfaces::client_state()->force_full_update();
}
