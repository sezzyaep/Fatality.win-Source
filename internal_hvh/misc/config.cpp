#include "../include_cheat.h"
#include <fstream>
#include "../cthash.h"

std::vector ignore_items
{
	FNV1A( "inventory.definition_index" ),
	FNV1A( "inventory.paint_kit" ),
	FNV1A( "inventory.wear" ),
	FNV1A( "inventory.seed" ),
	FNV1A( "inventory.stat_trak" ),
	FNV1A( "menu.inventory_list_idx" ),
	FNV1A( "menu.slot" ),
	FNV1A( "menu.preview_type" ),
};

bool config::load( const int slot )
{
	_u( config_name_u, config_name );
	const auto config = config_name_u + std::to_string( slot );
	auto input_file = std::ifstream( config );

	if ( !input_file.good() )
	{
		auto& cfg = config_init::get_cfg();
		for ( auto& item : cfg )
			if ( item.second && std::ranges::find( ignore_items, item.first ) == ignore_items.end() )
				item.second->set_default();

		return false;
	}

	try
	{
		std::string line;
		while ( input_file.good() )
		{
			std::getline( input_file, line );
			if ( line.empty() )
				break;

			std::string key, val, str_val {};

			const auto idx = line.find( '#' );
			key = line.substr( 0, idx );
			val = line.substr( idx + 1, line.size() - idx - 1 );

			const auto id2 = val.find( '$' );
			if ( id2 != std::string::npos )
			{
				const auto new_val = val.substr( 0, idx - 1 );
				str_val = val.substr( idx, val.size() - idx );
				val = new_val;
			}

			const auto hash = std::stoll( key );

			const auto found = std::ranges::find( ignore_items, hash ) != ignore_items.end();
			if ( !found && config_init::get_item_direct( hash ) )
			{
				config_init::get_item_direct( hash )->set<double>( std::stod( val ) );
				config_init::get_item_direct( hash )->set_string( str_val );
			}
		}

		input_file.close();
	}
	catch ( ... )
	{

		interfaces::cvar()->ConsoleColorPrintf( Color( 235, 5, 90, 255 ), xorstr_( "[fatality] " ) );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), xorstr_( "failed to load config\n" ) );
		EMIT_ERROR_SOUND();

		auto& cfg = config_init::get_cfg();
		for ( auto& item : cfg )
			if ( item.second && std::ranges::find( ignore_items, item.first ) == ignore_items.end() )
				item.second->set_default();

		return false;
	}


	_( loaded, "loaded config %s %d\n" );
	_( fatality, "[fatality] " );

	interfaces::cvar()->ConsoleColorPrintf( Color( 235, 5, 90, 255 ), fatality.c_str() );
	util::print_dev_console( true, Color( 255, 255, 255, 255 ), loaded.c_str(), config_name_u.c_str(), slot );

	return true;
}

bool config::save( const int slot )
{
	_u( config_name_u, config_name );
	const auto config = config_name_u + std::to_string( slot );

	std::string outstr;

	auto& cfg_map = config_init::get_cfg();

	for ( auto& item : cfg_map )
	{
		if ( std::ranges::find( ignore_items, item.first ) == ignore_items.end() )
		{
			outstr.append( std::string( std::to_string( item.first ) + '#' + std::to_string( item.second->get<double>() ) ) );
			if ( !item.second->get_string().empty() )
				outstr += '$' + item.second->get_string();
			outstr += '\n';
		}
	}

	auto output_file = std::ofstream( config );
	if ( !output_file.good() )
		return false;
	output_file.write( outstr.c_str(), outstr.size() );
	output_file.close();

	_( loaded, "saved %s %d\n" );
	_( fatality, "[fatality] " );

	interfaces::cvar()->ConsoleColorPrintf( Color( 235, 5, 90, 255 ), fatality.c_str() );
	util::print_dev_console( true, Color( 255, 255, 255, 255 ), loaded.c_str(), config_name_u.c_str(), slot );

	return true;
}

weaponconfig_t* config::current_config( C_BaseCombatWeapon* weapon )
{
	constexpr auto get_config_index = [] ( const int weaponindex )
	{
		switch ( weaponindex )
		{
			case WEAPON_G3SG1:
			case WEAPON_SCAR20:
				return 0;
			case WEAPON_SSG08:
				return 1;
			case WEAPON_AWP:
				return 2;
			case WEAPON_DEAGLE:
			case WEAPON_REVOLVER:
				return 3;
			case WEAPON_CZ75A:
			case WEAPON_ELITE:
			case WEAPON_FIVESEVEN:
			case WEAPON_GLOCK:
			case WEAPON_HKP2000:
			case WEAPON_P250:
			case WEAPON_USP_SILENCER:
			case WEAPON_TEC9:
				return 4;
			default:
				return 5;
		}
	};

	return &weapon_config[ get_config_index( weapon->get_weapon_id() ) ];
}

weaponconfig_legit_t* config::current_legit_config( C_BaseCombatWeapon* weapon )
{
	constexpr auto get_config_index = [] ( const int weaponindex )
	{
		switch ( weaponindex )
		{
			case WEAPON_DEAGLE:
			case WEAPON_ELITE:
			case WEAPON_FIVESEVEN:
			case WEAPON_GLOCK:
			case WEAPON_P250:
			case WEAPON_HKP2000:
			case WEAPON_TEC9:
			case WEAPON_REVOLVER:
			case WEAPON_USP_SILENCER:
				return 0;
			case WEAPON_AK47:
			case WEAPON_AUG:
			case WEAPON_FAMAS:
			case WEAPON_GALILAR:
			case WEAPON_M4A1:
			case WEAPON_M4A1_SILENCER:
			case WEAPON_SG556:
				return 1;
			case WEAPON_AWP:
			case WEAPON_G3SG1:
			case WEAPON_SCAR20:
			case WEAPON_SSG08:
				return 2;
			case WEAPON_MAC10:
			case WEAPON_P90:
			case WEAPON_MP5SD:
			case WEAPON_UMP45:
			case WEAPON_BIZON:
			case WEAPON_MP7:
			case WEAPON_MP9:
				return 3;
			case WEAPON_XM1014:
			case WEAPON_MAG7:
			case WEAPON_SAWEDOFF:
			case WEAPON_NOVA:
				return 4;
			case WEAPON_M249:
			case WEAPON_NEGEV:
				return 5;
			default:
				return 6;
		}
	};

	const auto index = get_config_index( weapon->get_weapon_id() );
	if ( index == 6 )
		return nullptr;

	return &weapon_config_legit[ index ];
}