#include "menu_include.h"
#include "elements/control.h"
#include <ren/adapter_dx9.h>

extern std::vector<listbox_item_t> menu_items;
extern std::vector<std::string> weapon_menu_items;
extern std::vector<listbox_item_t> list_items;

using namespace evo::ren;

menu* g_menu = new menu();

menu::menu() : m_init( false ), m_mouse_mode(), m_keyboard_control( nullptr ), m_focus_control( nullptr ), m_focused_scissor_record(), m_alpha( 0 ), m_open( false ) {}

void save_config()
{
	config::save( vars::menu.slot->get<int>() );

	_u( config_name_u, config::config_name );
	const auto config = config_name_u + std::to_string( vars::menu.slot->get<int>() );

	lua::api.callback( FNV1A( "on_config_save" ), [&] ( lua::state& s ) -> int
	{
		s.push( config.c_str() );
		return 1;
	} );
}

void load_config()
{
	config::load( vars::menu.slot->get<int>() );

	_u( config_name_u, config::config_name );
	const auto config = config_name_u + std::to_string( vars::menu.slot->get<int>() );

	lua::api.callback( FNV1A( "on_config_load" ), [&] ( lua::state& s ) -> int
	{
		s.push( config.c_str() );
		return 1;
	} );

	std::lock_guard lock( lua::api.list_mutex );

	for ( auto& item : lua::api.loaded_scripts )
	{
		_u( name, item );
		const auto hash = fnv1a_rt( xorstr_( "lua_" ) + name );
		if ( !config_init::exists( hash ) || !config_init::get_item_direct( hash )->get<bool>() )
		{
			_( LUA, "[lua] " );
			_( unload, "unloading script %s\n" );

			interfaces::cvar()->ConsoleColorPrintf( Color( 171, 34, 210, 255 ), LUA.c_str() );
			util::print_dev_console( true, Color( 255, 255, 255, 255 ), unload.c_str(), name.c_str() );

			lua::script_file file{
				lua::st_script,
				name
			};

			lua::api.stop_script( file );
		}
	}

	for ( auto& item : lua::api.listed_scripts )
	{
		_u( name, item );
		const auto hash = fnv1a_rt( xorstr_( "lua_" ) + name );
		if ( config_init::exists( hash ) && config_init::get_item_direct( hash )->get<bool>() )
		{
			_( LUA, "[lua] " );
			_( load, "loading script %s\n" );

			interfaces::cvar()->ConsoleColorPrintf( Color( 171, 34, 210, 255 ), LUA.c_str() );
			util::print_dev_console( true, Color( 255, 255, 255, 255 ), load.c_str(), name.c_str() );

			lua::script_file file{
				lua::st_script,
				name
			};

			lua::api.run_script( file );
		}
	}
}

void lua_load_script()
{
	_( lua, "lua//" );

	if ( lua::api.selected_load->get<int>() < lua::api.listed_scripts.size() && lua::api.selected_load->get<int>() >= 0 )
	{
		_u( name, lua::api.listed_scripts[ lua::api.selected_load->get<int>() ] );

		_( LUA, "[lua] " );
		_( load, "loading script %s\n" );

		interfaces::cvar()->ConsoleColorPrintf( Color( 171, 34, 210, 255 ), LUA.c_str() );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), load.c_str(), name.c_str() );

		lua::script_file file{
			lua::st_script,
			name
		};

		lua::api.run_script( file );
	}
}

void lua_unload_script()
{
	_( lua, "lua//" );

	if ( lua::api.selected_unload->get<int>() < lua::api.loaded_scripts.size() && lua::api.selected_unload->get<int>() >= 0 )
	{
		_u( name, lua::api.loaded_scripts[ lua::api.selected_unload->get<int>() ] );

		_( LUA, "[lua] " );
		_( unload, "unloading script %s\n" );

		interfaces::cvar()->ConsoleColorPrintf( Color( 171, 34, 210, 255 ), LUA.c_str() );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), unload.c_str(), name.c_str() );

		lua::script_file file{
			lua::st_script,
			name
		};

		lua::api.stop_script( file );
	}
}

void lua_reload_script()
{
	_( lua, "lua//" );

	if ( lua::api.selected_unload->get<int>() < lua::api.loaded_scripts.size() && lua::api.selected_unload->get<int>() >= 0 )
	{
		_u( name, lua::api.loaded_scripts[ lua::api.selected_unload->get<int>() ] );

		_( LUA, "[lua] " );
		_( unload, "reloading script %s\n" );

		interfaces::cvar()->ConsoleColorPrintf( Color( 171, 34, 210, 255 ), LUA.c_str() );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), unload.c_str(), name.c_str() );

		lua::script_file file{
			lua::st_script,
			name
		};

		lua::api.stop_script( file );
		lua::api.run_script( file );
	}
}

void inventory_add()
{
	inventorychanger::add();
}

void inventory_save()
{
	inventorychanger::save( true );
}

void inventory_rm()
{
	inventorychanger::remove();
}

void inventory_reload()
{
	inventorychanger::load();
}

void menu::reset()
{
	m_windows.clear();
	m_focused_control_pos = {};
	m_focus_control = nullptr;

	m_init = false;
}

void menu::set_focused_scissor( rect focused_scissor_record )
{
	m_focused_scissor_record = focused_scissor_record;
}

std::vector<std::shared_ptr<tab>>& menu::get_tabs()
{
	return m_windows[ 0 ]->get_tabs();
}

const std::vector<combo_item_t> player_model_combo
{

	{ _w( "Default" ), 0 },
		// leet
{ _w( "The Elite Mr. Muhlik | Elite Crew" ), 1 },
{ _w( "Prof. Shahmat | Elite Crew" ), 2 },
{ _w( "Osiris | Elite Crew" ), 3 },
{ _w( "Ground Rebel | Elite Crew" ), 4 },
{ _w( "Mr. Muhlik | Elite Crew" ), 5 },

	// fbi
{ _w( "Special Agent Ava | FBI" ), 6 },
{ _w( "Michael Syfers | FBI Sniper" ), 7 },
{ _w( "Markus Delrow | FBI HRT" ), 8 },
{ _w( "Operator | FBI SWAT" ), 9 },

	// st6
{ _w( "Seal Team 6 Soldier | NSWC SEAL" ), 10 },
{ _w( "'Two Times' McCoy | USAF TACP" ), 11 },
{ _w( "Buckshot | NSWC SEAL" ), 12 },
{ _w( "3rd Commando Company | KSK" ), 13 },
{ _w( "Lt. Commander Ricksaw | NSWC SEAL" ), 14 },
{ _w( "'Blueberries' Buckshot | NSWC SEAL" ), 15 },
{ _w( "'Two Times' McCoy | TACP Cavalry" ), 16 },
{ _w( "Primeiro Tenente | Brazilian 1st Battalion" ), 17 },

	// swat
{ _w( "Cmdr. Mae 'Dead Cold' Jamison | SWAT" ), 18 },
{ _w( "1st Lieutenant Farlow | SWAT" ), 19 },
{ _w( "John 'Van Healen' Kask | SWAT" ), 20 },
{ _w( "Bio-Haz Specialist | SWAT" ), 21 },
{ _w( "Sergeant Bombson | SWAT" ), 22 },
{ _w( "Chem-Haz Specialist | SWAT" ), 23 },
{ _w( "Lieutenant 'Tree Hugger' Farlow | SWAT" ), 24 },

	// balkan
{ _w( "Maximus | Sabre" ), 25 },
{ _w( "Dragomir | Sabre" ), 26 },
{ _w( "'The Doctor' Romanov | Sabre" ), 27 },
{ _w( "Rezan The Ready | Sabre" ), 28 },
{ _w( "Blackwolf | Sabre" ), 29 },
{ _w( "Rezan the Redshirt | Sabre" ), 30 },
{ _w( "Dragomir | Sabre Footsoldier" ), 31 },

	// sas
{ _w( "D Squadron Officer | NZSAS" ), 32 },
{ _w( "B Squadron Officer | SAS" ), 33 },

	// phoenix
{ _w( "Soldier | Phoenix" ), 34 },
{ _w( "Enforcer | Phoenix" ), 35 },
{ _w( "Slingshot | Phoenix" ), 36 },
{ _w( "Street Soldier | Phoenix" ), 37 },

	// professional
{ _w( "Sir Bloody Miami Darryl | The Professionals" ), 38 },
{ _w( "Sir Bloody Silent Darryl | The Professionals" ), 39 },
{ _w( "Sir Bloody Skullhead Darryl | The Professionals" ), 40 },
{ _w( "Sir Bloody Darryl Royale | The Professionals" ), 41 },
{ _w( "Sir Bloody Loudmouth Darryl | The Professionals" ), 42 },
{ _w( "Bloody Darryl The Strapped | The Professionals" ), 43 },
{ _w( "Safecracker Voltzmann | The Professionals" ), 44 },
{ _w( "Little Kev | The Professionals" ), 45 },
{ _w( "Number K | The Professionals" ), 46 },
{ _w( "Getaway Sally | The Professionals" ), 47 },

	// professional
{ _w( "Trapper | Guerrilla Warfare" ), 48 },
{ _w( "Trapper Aggressor | Guerrilla Warfare" ), 49 },
{ _w( "Vypa Sista of the Revolution | Guerrilla Warfare" ), 50 },
{ _w( "Col. Mangos Dabisi | Guerrilla Warfare" ), 51 },
{ _w( "Arno The Overgrown | Guerrilla Warfare" ), 52 },
{ _w( "'Medium Rare' Crasswater | Guerrilla Warfare" ), 53 },
{ _w( "Crasswater The Forgotten | Guerrilla Warfare" ), 54 },
{ _w( "Elite Trapper Solman | Guerrilla Warfare" ), 55 },

	// diver
{ _w( "Cmdr. Davida 'Goggles' Fernandez | SEAL Frogman" ), 56 },
{ _w( "Cmdr. Frank 'Wet Sox' Baroud | SEAL Frogman" ), 57 },
{ _w( "Lieutenant Rex Krikey | SEAL Frogman" ), 58 },

	// gendarmerie
{ _w( "Sous-Lieutenant Medic | Gendarmerie Nationale" ), 59 },
{ _w( "Chem-Haz Capitaine | Gendarmerie Nationale" ), 60 },
{ _w( "Chef d'Escadron Rouchard | Gendarmerie Nationale" ), 61 },
{ _w( "Aspirant | Gendarmerie Nationale" ), 62 },
{ _w( "Officer Jacques Beltram | Gendarmerie Nationale" ), 63 },

{ _w( "Danger Zone A" ), 64 },
{ _w( "Danger Zone B" ), 65 },
{ _w( "Danger Zone C" ), 66 },

{ _w( "Pirate" ), 67 },
{ _w( "Pirate A" ), 68 },
{ _w( "Pirate B" ), 69 },
{ _w( "Pirate C" ), 70 },
{ _w( "Pirate D" ), 71 },
{ _w( "Separatist " ), 72 },
{ _w( "Separatist A" ), 73 },
{ _w( "Separatist B" ), 74 },
{ _w( "Separatist C" ), 75 },
{ _w( "Separatist D" ), 76 },
{ _w( "Heavy Phoenix" ), 77 },
{ _w( "Phoenix A" ), 78 },
{ _w( "Phoenix B" ), 79 },
{ _w( "Phoenix C" ), 80 },
{ _w( "Phoenix D" ), 81 },
{ _w( "Leet A" ), 82 },
{ _w( "Leet B" ), 83 },
{ _w( "Leet C" ), 84 },
{ _w( "Leet D" ), 85 },
{ _w( "Leet E" ), 86 },
{ _w( "Professional" ), 87 },
{ _w( "Professional 1" ), 88 },
{ _w( "Professional 2" ), 89 },
{ _w( "Professional 3" ), 90 },
{ _w( "Professional 4" ), 91 },
};

const std::vector<combo_item_t> player_mask_combo
{
	{ _w( "Default" ), 0 },
	{ _w( "Hoxton" ), 1 },
	{ _w( "Porcelain Doll" ), 2 },
	{ _w( "Skull" ), 3 },
	{ _w( "Samurai" ), 4 },
	{ _w( "Evil Clown" ), 5 },
	{ _w( "Wolf" ), 6 },
	{ _w( "Sheep" ), 7 },
	{ _w( "Bunny Gold" ), 8 },
	{ _w( "Anaglyph" ), 9 },
	{ _w( "Porcelain Doll Kabuki" ), 10 },
	{ _w( "Dallas" ), 11 },
	{ _w( "Pumpkin" ), 12 },
	{ _w( "Sheep Bloody" ), 13 },
	{ _w( "Devil Plastic" ), 14 },
	{ _w( "Boar" ), 15 },
	{ _w( "Chains" ), 16 },
	{ _w( "Tiki" ), 17 },
	{ _w( "Bunny" ), 18 },
	{ _w( "Sheep Gold" ), 19 },
	{ _w( "Zombie Fortune Plastic" ), 20 },
	{ _w( "Chicken" ), 21 },
	{ _w( "Skull Gold" ), 22 },
	{ _w( "TF2 Demoman" ), 23 },
	{ _w( "TF2 Engineer" ), 24 },
	{ _w( "TF2 Heavy" ), 25 },
	{ _w( "TF2 Medic" ), 26 },
	{ _w( "TF2 Pyro" ), 27 },
	{ _w( "TF2 Scout" ), 28 },
	{ _w( "TF2 Sniper" ), 29 },
	{ _w( "TF2 Soldier" ), 30 },
	{ _w( "TF2 Spy" ), 31 }
};


void menu::init()
{
	if ( m_init )
		return;

	std::shared_ptr<sub_tab> ptr;
	std::shared_ptr<weapon_tab> ptr2;
	std::shared_ptr<weapon_tab> ptr3;
	std::shared_ptr<weapon_tab> ptr4;
	std::shared_ptr<weapon_tab> ptr5;
	std::shared_ptr<weapon_tab> ptr6;
	std::shared_ptr<weapon_tab> ptr7;
	std::shared_ptr<sub_tab> ptr9;
	std::shared_ptr<child> ptr8;
	std::shared_ptr<child> ptr10;
	std::shared_ptr<child> ptr11;

	auto menu_window = std::make_shared< window >( _w( "FATALITY" ), vec2( 800, 650 ), vec2( 100, 100 ), window_style::flags::none, std::vector<std::shared_ptr<tab>>
	{std::make_shared<tab>( _w( "RAGE" ), std::vector<std::shared_ptr<sub_tab>>
	{ptr = std::make_shared<sub_tab>( _w( "AIMBOT" ), std::vector<std::shared_ptr<sub_tab>>{ std::make_shared<weapon_tab>( _w( "scar20" ), std::vector<std::shared_ptr<child>>
	{std::make_shared<child>( _w( "Auto" ), vec2( 16, 16 ), vec2( 235, 446 ), std::vector<std::shared_ptr<control>>
	{
		std::make_shared<slider>( _w( "Minimum hit chance" ), vars::weapon_config.autosniper.hitchance, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),
			std::make_shared<slider>( _w( "Pointscale" ), vars::weapon_config.autosniper.pointscale, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
			std::make_shared<slider>( _w( "Minimum damage" ), vars::weapon_config.autosniper.mindmg, 0.f, 125.f, slider_style::flags::none, _w( "%.f hp" ) ),
			std::make_shared<checkbox>( _w( "Override" ), vars::weapon_config.autosniper.mindmg_override_enabled )->add_control( std::make_shared<key_bind>( vars::weapon_config.autosniper.mindmg_override_enabled, vars::key.mindmg_override_enabled_auto ) ),
			std::make_shared<slider>( _w( "Minimum damage override" ), vars::weapon_config.autosniper.mindmg_override, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f hp" ) ),
			//std::make_shared<slider>( _w( "Damage accuracy" ), vars::weapon_config.autosniper.damage_accuracy, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
			std::make_shared<combobox>( _w( "Hitboxes" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
		{
			{ _w( "Head" ), vars::weapon_config.autosniper.hitboxes.head },
			{ _w( "Chest" ), vars::weapon_config.autosniper.hitboxes.chest },
			{ _w( "Stomach" ), vars::weapon_config.autosniper.hitboxes.stomach },
			{ _w( "Arms" ), vars::weapon_config.autosniper.hitboxes.arms },
			{ _w( "Legs" ), vars::weapon_config.autosniper.hitboxes.legs },
			{ _w( "Feet" ), vars::weapon_config.autosniper.hitboxes.feet },
		} ),
			std::make_shared<combobox>( _w( "Multipoint" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
			{
				{ _w( "Head" ), vars::weapon_config.autosniper.pointscales.head },
				{ _w( "Chest" ), vars::weapon_config.autosniper.pointscales.chest },
				{ _w( "Stomach" ), vars::weapon_config.autosniper.pointscales.stomach },
				{ _w( "Arms" ), vars::weapon_config.autosniper.pointscales.arms },
				{ _w( "Legs" ), vars::weapon_config.autosniper.pointscales.legs },
				{ _w( "Feet" ), vars::weapon_config.autosniper.pointscales.feet },
			} ),
	} ),
		std::make_shared<child>( _w( "Extra" ), vec2( 267, 16 ), vec2( 235, 446 ), std::vector<std::shared_ptr<control>>
				{
					std::make_shared<checkbox>( _w( "Autostop" ), vars::weapon_config.autosniper.autostop )->add_control( std::make_shared<key_bind>( vars::weapon_config.autosniper.autostop, vars::key.autostop ) ),
						std::make_shared<combobox>( _w( "Autostop settings" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
					{
						{ _w( "Between shots" ), vars::weapon_config.autosniper.autostop_inbetween },
						{ _w( "Early" ), vars::weapon_config.autosniper.predictive_autostop },
						{ _w( "Only when lethal" ), vars::weapon_config.autosniper.autostop_lethal },
					} ),
						std::make_shared<checkbox>( _w( "Autoscope" ), vars::weapon_config.autosniper.autoscope ),
							std::make_shared<combobox>( _w( "Safepoint mode" ), vars::weapon_config.autosniper.fallback_mode, combobox_style::flags::none, std::vector<combo_item_t>
						{
							{ _w( "Off" ), 0 },
							{ _w( "Adaptive" ), 1 },
							{ _w( "Prefer safepoint" ), 2 },
						} ),
							std::make_shared<combobox>( _w( "Safety" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
							{
								{ _w( "Lethal" ), vars::weapon_config.autosniper.force_safe.lethal },
								{ _w( "In air target" ), vars::weapon_config.autosniper.force_safe.air },
								{ _w( "Moving target" ), vars::weapon_config.autosniper.force_safe.moving },
								{ _w( "Standing target" ), vars::weapon_config.autosniper.force_safe.standing },
								{ _w( "Limbs" ), vars::weapon_config.autosniper.force_safe.limbs },
							} ),
								std::make_shared<combobox>( _w( "Extra safety" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
								{
									{ _w( "Lethal" ), vars::weapon_config.autosniper.extra_safety.lethal },
									{ _w( "In air target" ), vars::weapon_config.autosniper.extra_safety.air },
									{ _w( "Moving target" ), vars::weapon_config.autosniper.extra_safety.moving },
									{ _w( "Standing target" ), vars::weapon_config.autosniper.extra_safety.standing },
									{ _w( "Limbs" ), vars::weapon_config.autosniper.extra_safety.limbs },
								} ),
									std::make_shared<combobox>( _w( "Bodyaim" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
									{
										{ _w( "Lethal" ), vars::weapon_config.autosniper.baim.lethal },
										{ _w( "In air target" ), vars::weapon_config.autosniper.baim.air },
										{ _w( "Moving target" ), vars::weapon_config.autosniper.baim.moving },
										{ _w( "Standing target" ), vars::weapon_config.autosniper.baim.standing },
										{ _w( "High risk" ), vars::weapon_config.autosniper.baim.high_risk },
									} ),

										std::make_shared<checkbox>( _w( "Ignore limbs when target moving" ), vars::weapon_config.autosniper.ignore_limbs ),
											//std::make_shared<checkbox>( _w( "Refine shot" ), vars::weapon_config.autosniper.refine_shot ),
				} ),
	} ), std::make_shared<weapon_tab>( _w( "ssg08" ), std::vector<std::shared_ptr<child>>
										{std::make_shared<child>( _w( "Scout" ), vec2( 16, 16 ), vec2( 235, 446 ), std::vector<std::shared_ptr<control>>
										{
											std::make_shared<slider>( _w( "Minimum hit chance" ), vars::weapon_config.scout.hitchance, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),
												std::make_shared<slider>( _w( "Pointscale" ), vars::weapon_config.scout.pointscale, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
												std::make_shared<slider>( _w( "Minimum damage" ), vars::weapon_config.scout.mindmg, 0.f, 125.f, slider_style::flags::none, _w( "%.f hp" ) ),
												std::make_shared<checkbox>( _w( "Override" ), vars::weapon_config.scout.mindmg_override_enabled )->add_control( std::make_shared<key_bind>( vars::weapon_config.scout.mindmg_override_enabled, vars::key.mindmg_override_enabled_scout ) ),
												std::make_shared<slider>( _w( "Minimum damage override" ), vars::weapon_config.scout.mindmg_override, 0.f, 125.f, slider_style::flags::no_operators, _w( "%.f hp" ) ),
												//std::make_shared<slider>( _w( "Damage accuracy" ), vars::weapon_config.scout.damage_accuracy, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
												std::make_shared<combobox>( _w( "Hitboxes" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
											{
												{ _w( "Head" ), vars::weapon_config.scout.hitboxes.head },
												{ _w( "Chest" ), vars::weapon_config.scout.hitboxes.chest },
												{ _w( "Stomach" ), vars::weapon_config.scout.hitboxes.stomach },
												{ _w( "Arms" ), vars::weapon_config.scout.hitboxes.arms },
												{ _w( "Legs" ), vars::weapon_config.scout.hitboxes.legs },
												{ _w( "Feet" ), vars::weapon_config.scout.hitboxes.feet },
											} ),
												std::make_shared<combobox>( _w( "Multipoint" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
												{
													{ _w( "Head" ), vars::weapon_config.scout.pointscales.head },
													{ _w( "Chest" ), vars::weapon_config.scout.pointscales.chest },
													{ _w( "Stomach" ), vars::weapon_config.scout.pointscales.stomach },
													{ _w( "Arms" ), vars::weapon_config.scout.pointscales.arms },
													{ _w( "Legs" ), vars::weapon_config.scout.pointscales.legs },
													{ _w( "Feet" ), vars::weapon_config.scout.pointscales.feet },
												} ),
										} ), std::make_shared<child>( _w( "Extra" ), vec2( 267, 16 ), vec2( 235, 446 ), std::vector<std::shared_ptr<control>>
													{
														std::make_shared<checkbox>( _w( "Autostop" ), vars::weapon_config.scout.autostop )->add_control( std::make_shared<key_bind>( vars::weapon_config.scout.autostop, vars::key.autostop ) ),
															std::make_shared<combobox>( _w( "Autostop settings" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
														{
															{ _w( "Between shots" ), vars::weapon_config.scout.autostop_inbetween },
															{ _w( "Early" ), vars::weapon_config.scout.predictive_autostop },
															{ _w( "Only when lethal" ), vars::weapon_config.scout.autostop_lethal },
														} ),
															std::make_shared<checkbox>( _w( "Autoscope" ), vars::weapon_config.scout.autoscope ),
																std::make_shared<combobox>( _w( "Safepoint mode" ), vars::weapon_config.scout.fallback_mode, combobox_style::flags::none, std::vector<combo_item_t>
															{
																{ _w( "Off" ), 0 },
																{ _w( "Adaptive" ), 1 },
																{ _w( "Prefer safepoint" ), 2 },
															} ),
																std::make_shared<combobox>( _w( "Safety" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																{
																	{ _w( "Lethal" ), vars::weapon_config.scout.force_safe.lethal },
																	{ _w( "In air target" ), vars::weapon_config.scout.force_safe.air },
																	{ _w( "Moving target" ), vars::weapon_config.scout.force_safe.moving },
																	{ _w( "Standing target" ), vars::weapon_config.scout.force_safe.standing },
																	{ _w( "Limbs" ), vars::weapon_config.scout.force_safe.limbs },
																} ),
																	std::make_shared<combobox>( _w( "Extra safety" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																	{
																		{ _w( "Lethal" ), vars::weapon_config.scout.extra_safety.lethal },
																		{ _w( "In air target" ), vars::weapon_config.scout.extra_safety.air },
																		{ _w( "Moving target" ), vars::weapon_config.scout.extra_safety.moving },
																		{ _w( "Standing target" ), vars::weapon_config.scout.extra_safety.standing },
																		{ _w( "Limbs" ), vars::weapon_config.scout.extra_safety.limbs },
																	} ),
																		std::make_shared<combobox>( _w( "Bodyaim" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																		{
																			{ _w( "Lethal" ), vars::weapon_config.scout.baim.lethal },
																			{ _w( "In air target" ), vars::weapon_config.scout.baim.air },
																			{ _w( "Moving target" ), vars::weapon_config.scout.baim.moving },
																			{ _w( "Standing target" ), vars::weapon_config.scout.baim.standing },
																			{ _w( "High risk" ), vars::weapon_config.scout.baim.high_risk },
																		} ),

																			std::make_shared<checkbox>( _w( "Ignore limbs when target moving" ), vars::weapon_config.scout.ignore_limbs ),
																				std::make_shared<checkbox>( _w( "Jumpscout" ), vars::weapon_config.scout.scout_jumpshot ),
																				//std::make_shared<checkbox>( _w( "Refine shot" ), vars::weapon_config.scout.refine_shot ),
													} ),
										} ), std::make_shared<weapon_tab>( _w( "awp" ), std::vector<std::shared_ptr<child>>
																			{std::make_shared<child>( _w( "AWP" ), vec2( 16, 16 ), vec2( 235, 446 ), std::vector<std::shared_ptr<control>>
																			{
																				std::make_shared<slider>( _w( "Minimum hit chance" ), vars::weapon_config.awp.hitchance, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),
																					std::make_shared<slider>( _w( "Pointscale" ), vars::weapon_config.awp.pointscale, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																					std::make_shared<slider>( _w( "Minimum damage" ), vars::weapon_config.awp.mindmg, 0.f, 125.f, slider_style::flags::none, _w( "%.f hp" ) ),
																					std::make_shared<checkbox>( _w( "Override" ), vars::weapon_config.awp.mindmg_override_enabled )->add_control( std::make_shared<key_bind>( vars::weapon_config.awp.mindmg_override_enabled, vars::key.mindmg_override_enabled_awp ) ),
																					std::make_shared<slider>( _w( "Minimum damage override" ), vars::weapon_config.awp.mindmg_override, 0.f, 125.f, slider_style::flags::no_operators, _w( "%.f hp" ) ),
																					//std::make_shared<slider>( _w( "Damage accuracy" ), vars::weapon_config.awp.damage_accuracy, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																					std::make_shared<combobox>( _w( "Hitboxes" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																				{
																					{ _w( "Head" ), vars::weapon_config.awp.hitboxes.head },
																					{ _w( "Chest" ), vars::weapon_config.awp.hitboxes.chest },
																					{ _w( "Stomach" ), vars::weapon_config.awp.hitboxes.stomach },
																					{ _w( "Arms" ), vars::weapon_config.awp.hitboxes.arms },
																					{ _w( "Legs" ), vars::weapon_config.awp.hitboxes.legs },
																					{ _w( "Feet" ), vars::weapon_config.awp.hitboxes.feet },
																				} ),
																					std::make_shared<combobox>( _w( "Multipoint" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																					{
																						{ _w( "Head" ), vars::weapon_config.awp.pointscales.head },
																						{ _w( "Chest" ), vars::weapon_config.awp.pointscales.chest },
																						{ _w( "Stomach" ), vars::weapon_config.awp.pointscales.stomach },
																						{ _w( "Arms" ), vars::weapon_config.awp.pointscales.arms },
																						{ _w( "Legs" ), vars::weapon_config.awp.pointscales.legs },
																						{ _w( "Feet" ), vars::weapon_config.awp.pointscales.feet },
																					} ),
																			} ), std::make_shared<child>( _w( "Extra" ), vec2( 267, 16 ), vec2( 235, 446 ), std::vector<std::shared_ptr<control>>
																						{
																							std::make_shared<checkbox>( _w( "Autostop" ), vars::weapon_config.awp.autostop )->add_control( std::make_shared<key_bind>( vars::weapon_config.awp.autostop, vars::key.autostop ) ),
																								std::make_shared<combobox>( _w( "Autostop settings" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																							{
																								{ _w( "Between shots" ), vars::weapon_config.awp.autostop_inbetween },
																								{ _w( "Early" ), vars::weapon_config.awp.predictive_autostop },
																								{ _w( "Only when lethal" ), vars::weapon_config.awp.autostop_lethal },
																							} ),
																								std::make_shared<checkbox>( _w( "Autoscope" ), vars::weapon_config.awp.autoscope ),
																									std::make_shared<combobox>( _w( "Safepoint mode" ), vars::weapon_config.awp.fallback_mode, combobox_style::flags::none, std::vector<combo_item_t>
																								{
																									{ _w( "Off" ), 0 },
																									{ _w( "Adaptive" ), 1 },
																									{ _w( "Prefer safepoint" ), 2 },
																								} ),
																									std::make_shared<combobox>( _w( "Safety" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																									{
																										{ _w( "Lethal" ), vars::weapon_config.awp.force_safe.lethal },
																										{ _w( "In air target" ), vars::weapon_config.awp.force_safe.air },
																										{ _w( "Moving target" ), vars::weapon_config.awp.force_safe.moving },
																										{ _w( "Standing target" ), vars::weapon_config.awp.force_safe.standing },
																										{ _w( "Limbs" ), vars::weapon_config.awp.force_safe.limbs },
																									} ),
																										std::make_shared<combobox>( _w( "Extra safety" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																										{
																											{ _w( "Lethal" ), vars::weapon_config.awp.extra_safety.lethal },
																											{ _w( "In air target" ), vars::weapon_config.awp.extra_safety.air },
																											{ _w( "Moving target" ), vars::weapon_config.awp.extra_safety.moving },
																											{ _w( "Standing target" ), vars::weapon_config.awp.extra_safety.standing },
																											{ _w( "Limbs" ), vars::weapon_config.awp.extra_safety.limbs },
																										} ),
																											std::make_shared<combobox>( _w( "Bodyaim" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																											{
																												{ _w( "Lethal" ), vars::weapon_config.awp.baim.lethal },
																												{ _w( "In air target" ), vars::weapon_config.awp.baim.air },
																												{ _w( "Moving target" ), vars::weapon_config.awp.baim.moving },
																												{ _w( "Standing target" ), vars::weapon_config.awp.baim.standing },
																												{ _w( "High risk" ), vars::weapon_config.awp.baim.high_risk },
																											} ),

																												std::make_shared<checkbox>( _w( "Ignore limbs when target moving" ), vars::weapon_config.awp.ignore_limbs ),
																													//std::make_shared<checkbox>( _w( "Refine shot" ), vars::weapon_config.awp.refine_shot ),
																						} ),
																			} ), std::make_shared<weapon_tab>( _w( "revolver" ), std::vector<std::shared_ptr<child>>
																												{std::make_shared<child>( _w( "Heavy pistols" ), vec2( 16, 16 ), vec2( 235, 446 ), std::vector<std::shared_ptr<control>>
																												{
																													std::make_shared<slider>( _w( "Minimum hit chance" ), vars::weapon_config.heavy_pistol.hitchance, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),
																														std::make_shared<slider>( _w( "Pointscale" ), vars::weapon_config.heavy_pistol.pointscale, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																														std::make_shared<slider>( _w( "Minimum damage" ), vars::weapon_config.heavy_pistol.mindmg, 0.f, 125.f, slider_style::flags::none, _w( "%.f hp" ) ),
																														std::make_shared<checkbox>( _w( "Override" ), vars::weapon_config.heavy_pistol.mindmg_override_enabled )->add_control( std::make_shared<key_bind>( vars::weapon_config.heavy_pistol.mindmg_override_enabled, vars::key.mindmg_override_enabled_heavy_pistol ) ),
																														std::make_shared<slider>( _w( "Minimum damage override" ), vars::weapon_config.heavy_pistol.mindmg_override, 0.f, 125.f, slider_style::flags::no_operators, _w( "%.f hp" ) ),
																														//std::make_shared<slider>( _w( "Damage accuracy" ), vars::weapon_config.heavy_pistol.damage_accuracy, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																														std::make_shared<combobox>( _w( "Hitboxes" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																													{
																														{ _w( "Head" ), vars::weapon_config.heavy_pistol.hitboxes.head },
																														{ _w( "Chest" ), vars::weapon_config.heavy_pistol.hitboxes.chest },
																														{ _w( "Stomach" ), vars::weapon_config.heavy_pistol.hitboxes.stomach },
																														{ _w( "Arms" ), vars::weapon_config.heavy_pistol.hitboxes.arms },
																														{ _w( "Legs" ), vars::weapon_config.heavy_pistol.hitboxes.legs },
																														{ _w( "Feet" ), vars::weapon_config.heavy_pistol.hitboxes.feet },
																													} ),
																														std::make_shared<combobox>( _w( "Multipoint" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																														{
																															{ _w( "Head" ), vars::weapon_config.heavy_pistol.pointscales.head },
																															{ _w( "Chest" ), vars::weapon_config.heavy_pistol.pointscales.chest },
																															{ _w( "Stomach" ), vars::weapon_config.heavy_pistol.pointscales.stomach },
																															{ _w( "Arms" ), vars::weapon_config.heavy_pistol.pointscales.arms },
																															{ _w( "Legs" ), vars::weapon_config.heavy_pistol.pointscales.legs },
																															{ _w( "Feet" ), vars::weapon_config.heavy_pistol.pointscales.feet },
																														} ),
																												} ), std::make_shared<child>( _w( "Extra" ), vec2( 267, 16 ), vec2( 235, 446 ), std::vector<std::shared_ptr<control>>
																															{
																																std::make_shared<checkbox>( _w( "Autostop" ), vars::weapon_config.heavy_pistol.autostop )->add_control( std::make_shared<key_bind>( vars::weapon_config.heavy_pistol.autostop, vars::key.autostop ) ),
																																	std::make_shared<combobox>( _w( "Autostop settings" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																{
																																	{ _w( "Between shots" ), vars::weapon_config.heavy_pistol.autostop_inbetween },
																																	{ _w( "Early" ), vars::weapon_config.heavy_pistol.predictive_autostop },
																																	{ _w( "Only when lethal" ), vars::weapon_config.heavy_pistol.autostop_lethal },
																																} ),
																																	std::make_shared<combobox>( _w( "Safepoint mode" ), vars::weapon_config.heavy_pistol.fallback_mode, combobox_style::flags::none, std::vector<combo_item_t>
																																	{
																																		{ _w( "Off" ), 0 },
																																		{ _w( "Adaptive" ), 1 },
																																		{ _w( "Prefer safepoint" ), 2 },
																																	} ),
																																		std::make_shared<combobox>( _w( "Safety" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																		{
																																			{ _w( "Lethal" ), vars::weapon_config.heavy_pistol.force_safe.lethal },
																																			{ _w( "In air target" ), vars::weapon_config.heavy_pistol.force_safe.air },
																																			{ _w( "Moving target" ), vars::weapon_config.heavy_pistol.force_safe.moving },
																																			{ _w( "Standing target" ), vars::weapon_config.heavy_pistol.force_safe.standing },
																																			{ _w( "Limbs" ), vars::weapon_config.heavy_pistol.force_safe.limbs },
																																		} ),
																																			std::make_shared<combobox>( _w( "Extra safety" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																			{
																																				{ _w( "Lethal" ), vars::weapon_config.heavy_pistol.extra_safety.lethal },
																																				{ _w( "In air target" ), vars::weapon_config.heavy_pistol.extra_safety.air },
																																				{ _w( "Moving target" ), vars::weapon_config.heavy_pistol.extra_safety.moving },
																																				{ _w( "Standing target" ), vars::weapon_config.heavy_pistol.extra_safety.standing },
																																				{ _w( "Limbs" ), vars::weapon_config.heavy_pistol.extra_safety.limbs },
																																			} ),
																																				std::make_shared<combobox>( _w( "Bodyaim" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																				{
																																					{ _w( "Lethal" ), vars::weapon_config.heavy_pistol.baim.lethal },
																																					{ _w( "In air target" ), vars::weapon_config.heavy_pistol.baim.air },
																																					{ _w( "Moving target" ), vars::weapon_config.heavy_pistol.baim.moving },
																																					{ _w( "Standing target" ), vars::weapon_config.heavy_pistol.baim.standing },
																																					{ _w( "High risk" ), vars::weapon_config.heavy_pistol.baim.high_risk },
																																				} ),

																																					std::make_shared<checkbox>( _w( "Ignore limbs when target moving" ), vars::weapon_config.heavy_pistol.ignore_limbs ),
																																						std::make_shared<checkbox>( _w( "Autorevolver" ), vars::weapon_config.heavy_pistol.autorevolver ),
																																						//std::make_shared<checkbox>( _w( "Refine shot" ), vars::weapon_config.heavy_pistol.refine_shot ),
																															} ),
																												} ), std::make_shared<weapon_tab>( _w( "usp_silencer" ), std::vector<std::shared_ptr<child>>
																																					{std::make_shared<child>( _w( "Pistols" ), vec2( 16, 16 ), vec2( 235, 446 ), std::vector<std::shared_ptr<control>>
																																					{
																																						std::make_shared<slider>( _w( "Minimum hit chance" ), vars::weapon_config.pistol.hitchance, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),
																																							std::make_shared<slider>( _w( "Pointscale" ), vars::weapon_config.pistol.pointscale, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																							std::make_shared<slider>( _w( "Minimum damage" ), vars::weapon_config.pistol.mindmg, 0.f, 125.f, slider_style::flags::none, _w( "%.f hp" ) ),
																																							std::make_shared<checkbox>( _w( "Override" ), vars::weapon_config.pistol.mindmg_override_enabled )->add_control( std::make_shared<key_bind>( vars::weapon_config.pistol.mindmg_override_enabled, vars::key.mindmg_override_enabled_pistol ) ),
																																							std::make_shared<slider>( _w( "Minimum damage override" ), vars::weapon_config.pistol.mindmg_override, 0.f, 125.f, slider_style::flags::no_operators, _w( "%.f hp" ) ),
																																							//std::make_shared<slider>( _w( "Damage accuracy" ), vars::weapon_config.pistol.damage_accuracy, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																							std::make_shared<combobox>( _w( "Hitboxes" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																						{
																																							{ _w( "Head" ), vars::weapon_config.pistol.hitboxes.head },
																																							{ _w( "Chest" ), vars::weapon_config.pistol.hitboxes.chest },
																																							{ _w( "Stomach" ), vars::weapon_config.pistol.hitboxes.stomach },
																																							{ _w( "Arms" ), vars::weapon_config.pistol.hitboxes.arms },
																																							{ _w( "Legs" ), vars::weapon_config.pistol.hitboxes.legs },
																																							{ _w( "Feet" ), vars::weapon_config.pistol.hitboxes.feet },
																																						} ),
																																							std::make_shared<combobox>( _w( "Multipoint" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																							{
																																								{ _w( "Head" ), vars::weapon_config.pistol.pointscales.head },
																																								{ _w( "Chest" ), vars::weapon_config.pistol.pointscales.chest },
																																								{ _w( "Stomach" ), vars::weapon_config.pistol.pointscales.stomach },
																																								{ _w( "Arms" ), vars::weapon_config.pistol.pointscales.arms },
																																								{ _w( "Legs" ), vars::weapon_config.pistol.pointscales.legs },
																																								{ _w( "Feet" ), vars::weapon_config.pistol.pointscales.feet },
																																							} ),
																																					} ), std::make_shared<child>( _w( "Extra" ), vec2( 267, 16 ), vec2( 235, 446 ), std::vector<std::shared_ptr<control>>
																																								{
																																									std::make_shared<checkbox>( _w( "Autostop" ), vars::weapon_config.pistol.autostop )->add_control( std::make_shared<key_bind>( vars::weapon_config.pistol.autostop, vars::key.autostop ) ),
																																										std::make_shared<combobox>( _w( "Autostop settings" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																									{
																																										{ _w( "Between shots" ), vars::weapon_config.pistol.autostop_inbetween },
																																										{ _w( "Early" ), vars::weapon_config.pistol.predictive_autostop },
																																										{ _w( "Only when lethal" ), vars::weapon_config.pistol.autostop_lethal },
																																									} ),
																																										std::make_shared<combobox>( _w( "Safepoint mode" ), vars::weapon_config.pistol.fallback_mode, combobox_style::flags::none, std::vector<combo_item_t>
																																										{
																																											{ _w( "Off" ), 0 },
																																											{ _w( "Adaptive" ), 1 },
																																											{ _w( "Prefer safepoint" ), 2 },
																																										} ),
																																											std::make_shared<combobox>( _w( "Safety" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																											{
																																												{ _w( "Lethal" ), vars::weapon_config.pistol.force_safe.lethal },
																																												{ _w( "In air target" ), vars::weapon_config.pistol.force_safe.air },
																																												{ _w( "Moving target" ), vars::weapon_config.pistol.force_safe.moving },
																																												{ _w( "Standing target" ), vars::weapon_config.pistol.force_safe.standing },
																																												{ _w( "Limbs" ), vars::weapon_config.pistol.force_safe.limbs },
																																											} ),
																																												std::make_shared<combobox>( _w( "Extra safety" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																												{

																																													{ _w( "Lethal" ), vars::weapon_config.pistol.extra_safety.lethal },
																																													{ _w( "In air target" ), vars::weapon_config.pistol.extra_safety.air },
																																													{ _w( "Moving target" ), vars::weapon_config.pistol.extra_safety.moving },
																																													{ _w( "Standing target" ), vars::weapon_config.pistol.extra_safety.standing },
																																													{ _w( "Limbs" ), vars::weapon_config.pistol.extra_safety.limbs },
																																												} ),
																																													std::make_shared<combobox>( _w( "Bodyaim" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																													{
																																														{ _w( "Lethal" ), vars::weapon_config.pistol.baim.lethal },
																																														{ _w( "In air target" ), vars::weapon_config.pistol.baim.air },
																																														{ _w( "Moving target" ), vars::weapon_config.pistol.baim.moving },
																																														{ _w( "Standing target" ), vars::weapon_config.pistol.baim.standing },
																																														{ _w( "High risk" ), vars::weapon_config.pistol.baim.high_risk },
																																													} ),

																																														std::make_shared<checkbox>( _w( "Ignore limbs when target moving" ), vars::weapon_config.pistol.ignore_limbs ),
																																															//std::make_shared<checkbox>( _w( "Refine shot" ), vars::weapon_config.pistol.refine_shot ),
																																								} ),
																																					} ), std::make_shared<weapon_tab>( _w( "ak47" ), std::vector<std::shared_ptr<child>>
																																														{std::make_shared<child>( _w( "Other" ), vec2( 16, 16 ), vec2( 235, 446 ), std::vector<std::shared_ptr<control>>
																																														{
																																															std::make_shared<slider>( _w( "Minimum hit chance" ), vars::weapon_config.other.hitchance, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),
																																																std::make_shared<slider>( _w( "Pointscale" ), vars::weapon_config.other.pointscale, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																std::make_shared<slider>( _w( "Minimum damage" ), vars::weapon_config.other.mindmg, 0.f, 125.f, slider_style::flags::none, _w( "%.f hp" ) ),
																																																std::make_shared<checkbox>( _w( "Override" ), vars::weapon_config.other.mindmg_override_enabled )->add_control( std::make_shared<key_bind>( vars::weapon_config.other.mindmg_override_enabled, vars::key.mindmg_override_enabled_other ) ),
																																																std::make_shared<slider>( _w( "Minimum damage override" ), vars::weapon_config.other.mindmg_override, 0.f, 125.f, slider_style::flags::no_operators, _w( "%.f hp" ) ),
																																																//std::make_shared<slider>( _w( "Damage accuracy" ), vars::weapon_config.other.damage_accuracy, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																std::make_shared<combobox>( _w( "Hitboxes" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																															{
																																																{ _w( "Head" ), vars::weapon_config.other.hitboxes.head },
																																																{ _w( "Chest" ), vars::weapon_config.other.hitboxes.chest },
																																																{ _w( "Stomach" ), vars::weapon_config.other.hitboxes.stomach },
																																																{ _w( "Arms" ), vars::weapon_config.other.hitboxes.arms },
																																																{ _w( "Legs" ), vars::weapon_config.other.hitboxes.legs },
																																																{ _w( "Feet" ), vars::weapon_config.other.hitboxes.feet },
																																															} ),
																																																std::make_shared<combobox>( _w( "Multipoint" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																{
																																																	{ _w( "Head" ), vars::weapon_config.other.pointscales.head },
																																																	{ _w( "Chest" ), vars::weapon_config.other.pointscales.chest },
																																																	{ _w( "Stomach" ), vars::weapon_config.other.pointscales.stomach },
																																																	{ _w( "Arms" ), vars::weapon_config.other.pointscales.arms },
																																																	{ _w( "Legs" ), vars::weapon_config.other.pointscales.legs },
																																																	{ _w( "Feet" ), vars::weapon_config.other.pointscales.feet },
																																																} ),
																																														} ), std::make_shared<child>( _w( "Extra" ), vec2( 267, 16 ), vec2( 235, 446 ), std::vector<std::shared_ptr<control>>
																																																	{
																																																		std::make_shared<checkbox>( _w( "Autostop" ), vars::weapon_config.other.autostop )->add_control( std::make_shared<key_bind>( vars::weapon_config.other.autostop, vars::key.autostop ) ),
																																																			std::make_shared<combobox>( _w( "Autostop settings" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																		{
																																																			{ _w( "Between shots" ), vars::weapon_config.other.autostop_inbetween },
																																																			{ _w( "Early" ), vars::weapon_config.other.predictive_autostop },
																																																			{ _w( "Only when lethal" ), vars::weapon_config.other.autostop_lethal },
																																																		} ),
																																																			std::make_shared<checkbox>( _w( "Autoscope" ), vars::weapon_config.other.autoscope ),
																																																				std::make_shared<combobox>( _w( "Safepoint mode" ), vars::weapon_config.other.fallback_mode, combobox_style::flags::none, std::vector<combo_item_t>
																																																			{
																																																				{ _w( "Off" ), 0 },
																																																				{ _w( "Adaptive" ), 1 },
																																																				{ _w( "Prefer safepoint" ), 2 },
																																																			} ),
																																																				std::make_shared<combobox>( _w( "Safety" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																				{
																																																					{ _w( "Lethal" ), vars::weapon_config.other.force_safe.lethal },
																																																					{ _w( "In air target" ), vars::weapon_config.other.force_safe.air },
																																																					{ _w( "Moving target" ), vars::weapon_config.other.force_safe.moving },
																																																					{ _w( "Standing target" ), vars::weapon_config.other.force_safe.standing },
																																																					{ _w( "Limbs" ), vars::weapon_config.other.force_safe.limbs },
																																																				} ),
																																																					std::make_shared<combobox>( _w( "Extra safety" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																					{
																																																						{ _w( "Lethal" ), vars::weapon_config.other.extra_safety.lethal },
																																																						{ _w( "In air target" ), vars::weapon_config.other.extra_safety.air },
																																																						{ _w( "Moving target" ), vars::weapon_config.other.extra_safety.moving },
																																																						{ _w( "Standing target" ), vars::weapon_config.other.extra_safety.standing },
																																																						{ _w( "Limbs" ), vars::weapon_config.other.extra_safety.limbs },
																																																					} ),
																																																						std::make_shared<combobox>( _w( "Bodyaim" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																						{
																																																							{ _w( "Lethal" ), vars::weapon_config.other.baim.lethal },
																																																							{ _w( "In air target" ), vars::weapon_config.other.baim.air },
																																																							{ _w( "Moving target" ), vars::weapon_config.other.baim.moving },
																																																							{ _w( "Standing target" ), vars::weapon_config.other.baim.standing },
																																																							{ _w( "High risk" ), vars::weapon_config.other.baim.high_risk },
																																																						} ),

																																																							std::make_shared<checkbox>( _w( "Ignore limbs when target moving" ), vars::weapon_config.other.ignore_limbs ),
																																																								//std::make_shared<checkbox>( _w( "Refine shot" ), vars::weapon_config.other.refine_shot ),
																																																	} ), } ), } ),
		std::make_shared<sub_tab>( _w( "ANTI-AIM" ), std::vector<std::shared_ptr<child>>
																																																							{std::make_shared<child>( _w( "Angles" ), vec2( 16, 16 ), vec2( 235, 486 ), std::vector<std::shared_ptr<control>>
																																																							{std::make_shared<checkbox>( _w( "Anti-aim" ), vars::aa.enabled )->add_control( std::make_shared<key_bind>( vars::aa.enabled, vars::key.antiaim ) ),
																																																								std::make_shared<combobox>( _w( "Pitch" ), vars::aa.pitch, combobox_style::flags::none, std::vector<combo_item_t>
																																																							{
																																																								{ _w( "None" ), 0 },
																																																								{ _w( "Down" ), 1 },
																																																								{ _w( "Up" ), 2 },
																																																								{ _w( "Zero" ), 3 },
																																																							} ),
																																																								std::make_shared<combobox>( _w( "Yaw" ), vars::aa.yaw, combobox_style::flags::none, std::vector<combo_item_t>
																																																								{
																																																									{ _w( "None" ), 0 },
																																																									{ _w( "Backwards" ), 1 },
																																																									{ _w( "Zero" ), 2 },
																																																									{ _w( "Random" ), 3 },
																																																								} ),
																																																									std::make_shared<checkbox>( _w( "Yaw add" ), vars::aa.add_enabled ),
																																																										std::make_shared<slider>( _w( "Add" ), vars::aa.add, -180.f, 180.f, slider_style::flags::none, _w( "%.f" ) )->add_dependency( { vars::aa.add_enabled, 1.0 } ),
																																																										std::make_shared<checkbox>( _w( "Freestand" ), vars::aa.freestand )->add_control( std::make_shared<key_bind>( vars::aa.freestand, vars::key.freestanding ) ),
																																																										std::make_shared<checkbox>( _w( "At fov target" ), vars::aa.at_target ),
																																																										std::make_shared<checkbox>( _w( "Spin" ), vars::aa.spin ),
																																																										std::make_shared<slider>( _w( "Spin range" ), vars::aa.spin_range, 0.f, 360.f, slider_style::flags::none, _w( "%.f" ) )->add_dependency( { vars::aa.spin, 1.0 } ),
																																																										std::make_shared<slider>( _w( "Spin speed" ), vars::aa.spin_speed, 0.f, 360.f, slider_style::flags::none, _w( "%.f rpm" ) )->add_dependency( { vars::aa.spin, 1.0 } ),
																																																										std::make_shared<checkbox>( _w( "Jitter" ), vars::aa.jitter ),
																																																										std::make_shared<checkbox>( _w( "Random" ), vars::aa.jitter_random )->add_dependency( { vars::aa.jitter, 1.0 } ),
																																																										std::make_shared<slider>( _w( "Jitter range" ), vars::aa.jitter_range, 0.f, 360.f, slider_style::flags::none, _w( "%.f" ) )->add_dependency( { vars::aa.jitter, 1.0 } ),
																																																										std::make_shared<checkbox>( _w( "Antiaim override" ), vars::aa.manual ),
																																																										std::make_shared<checkbox>( _w( "Back" ), vars::aa.manual_back )->add_control( std::make_shared<key_bind>( vars::aa.manual_back, vars::key.aa_back ) ),
																																																										std::make_shared<checkbox>( _w( "Left" ), vars::aa.manual_left )->add_control( std::make_shared<key_bind>( vars::aa.manual_left, vars::key.aa_left ) ),
																																																										std::make_shared<checkbox>( _w( "Right" ), vars::aa.manual_right )->add_control( std::make_shared<key_bind>( vars::aa.manual_right, vars::key.aa_right ) ),
																																																							} ),
																																												std::make_shared<child>( _w( "Fakelag" ), vec2( 267, 16 ), vec2( 235, 90 ), std::vector<std::shared_ptr<control>>
																																																									{
																																																										std::make_shared<combobox>( _w( "Mode" ), vars::aa.choke_mode, combobox_style::flags::none, std::vector<combo_item_t>
																																																										{
																																																											{ _w( "None" ), 0 },
																																																											{ _w( "Always on" ), 1 },
																																																											{ _w( "Adaptive" ), 2 },
																																																										} ),
																																																											std::make_shared<slider>( _w( "Limit" ), vars::aa.choke_limit, 1.f, 14.f, slider_style::flags::none, _w( "%.f" ) )->add_dependency( { vars::aa.choke_mode, 0, true } ),

																																																									} ),
																																																										std::make_shared<child>( _w( "Desync" ), vec2( 267, 122 ), vec2( 235, 380 ), std::vector<std::shared_ptr<control>>
																																																											{
																																																												std::make_shared<checkbox>( _w( "Fake" ), vars::aa.fake ),
																																																													std::make_shared<slider>( _w( "Fake amount" ), vars::aa.fake_amount, -100.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) )->add_dependency( { vars::aa.fake, 1.0 } ),
																																																													std::make_shared<slider>( _w( "Compensate angle" ), vars::aa.compensate_angle, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) )->add_dependency( { vars::aa.fake, 1.0 } ),
																																																													std::make_shared<combobox>( _w( "Freestand fake" ), vars::aa.freestand_fake, combobox_style::flags::none, std::vector<combo_item_t>
																																																												{
																																																													{ _w( "None" ), 0 },
																																																													{ _w( "Normal" ), 1 },
																																																													{ _w( "Opposite" ), 2 },
																																																												} ),
																																																													std::make_shared<checkbox>( _w( "Flip fake with jitter" ), vars::aa.flip_fake ),
																																																														std::make_shared<combobox>( _w( "Leg slide" ), vars::aa.strafing, combobox_style::flags::none, std::vector<combo_item_t>
																																																													{
																																																														{ _w( "Default" ), 0 },
																																																														{ _w( "Never slide" ), 1 },
																																																														{ _w( "Always slide" ), 2 }
																																																													} ),
																																																														std::make_shared<combobox>( _w( "Roll lean" ), vars::aa.lean_type, combobox_style::flags::none, std::vector<combo_item_t>
																																																														{
																																																															{ _w( "None" ), 0 },
																																																															{ _w( "Static" ), 1 },
																																																															{ _w( "Extend fake" ), 2 },
																																																															{ _w( "Invert fake" ), 6 },
																																																															{ _w( "Freestand" ), 3 },
																																																															{ _w( "Freestand opposite" ), 4 },
																																																															{ _w( "Jitter" ), 5 },
																																																														} ),
																																																															std::make_shared<slider>( _w( "Lean amount" ), vars::aa.lean_amount, -50.f, 50.f, slider_style::flags::none, _w( "%.f deg" ) )->add_dependency( { vars::aa.lean_type, 0.0, true } ),
																																																																std::make_shared<checkbox>( _w( "Ensure lean" ), vars::aa.ensure_lean )->add_control( std::make_shared<key_bind>( vars::aa.ensure_lean, vars::key.ensure_lean ) ),
																																																																std::make_shared<checkbox>( _w( "Flip lean with jitter" ), vars::aa.flip_lean ),


																																																																/*std::make_shared<combobox>( _w( "Shot Antiaim" ), vars::aa.shot, combobox_style::flags::none, std::vector<combo_item_t>
																																																															{
																																																																{ _w( "Auto" ), 0 },
																																																																{ _w( "Neutral" ), 3 },
																																																																{ _w( "Opposite" ), 1 },
																																																																{ _w( "Same side" ), 2 },
																																																															} ),*/



																																																											} ),


																																																							} ),
	} ),
	std::make_shared<tab>( _w( "LEGIT" ), std::vector<std::shared_ptr<sub_tab>>
																																																															{ptr2 = std::make_shared<weapon_tab>( _w( "usp_silencer" ), std::vector<std::shared_ptr<child>>
																																																															{std::make_shared<child>( _w( "Aimhelper" ), vec2( 16, 16 ), vec2( 235, 478 ), std::vector<std::shared_ptr<control>>
																																																															{
																																																																std::make_shared<checkbox>( _w( "Magnet" ), vars::legit.weapon_config.pistol.magnet )->add_control( std::make_shared<key_bind>( vars::legit.weapon_config.pistol.magnet, vars::key.magnet ) ),
																																																																	std::make_shared<combobox>( _w( "Magnet hitboxes" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																																{
																																																																	{ _w( "Head" ), vars::legit.weapon_config.pistol.magnet_hitboxes.head },
																																																																	{ _w( "Chest" ), vars::legit.weapon_config.pistol.magnet_hitboxes.chest },
																																																																	{ _w( "Body" ), vars::legit.weapon_config.pistol.magnet_hitboxes.body },
																																																																	{ _w( "Pelvis" ), vars::legit.weapon_config.pistol.magnet_hitboxes.pelvis },
																																																																} ),
																																																																	std::make_shared<slider>( _w( "Magnet fov" ), vars::legit.weapon_config.pistol.magnet_fov, 1.f, 50.f, slider_style::flags::none, _w( "%.f deg" ) ),
																																																																		std::make_shared<slider>( _w( "Magnet speed" ), vars::legit.weapon_config.pistol.magnet_speed, 1.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																		std::make_shared<slider>( _w( "Target delay" ), vars::legit.weapon_config.pistol.target_delay, 10.f, 500.f, slider_style::flags::none, _w( "%.f ms" ) ),
																																																																		std::make_shared<slider>( _w( "Limit percent" ), vars::legit.weapon_config.pistol.limit_factor, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																		std::make_shared<slider>( _w( "Override mouse percent" ), vars::legit.weapon_config.pistol.override_factor, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),

																																																															} ),
																																																																std::make_shared<child>( _w( "Assists" ), vec2( 267, 16 ), vec2( 235, 478 ), std::vector<std::shared_ptr<control>>
																																																																	{
																																																																		std::make_shared<checkbox>( _w( "Triggerbot" ), vars::legit.weapon_config.pistol.triggerbot )->add_control( std::make_shared<key_bind>( vars::legit.weapon_config.pistol.triggerbot, vars::key.triggerbot ) ),
																																																																			std::make_shared<slider>( _w( "Reaction time" ), vars::legit.weapon_config.pistol.reaction_time, 0.f, 500.f, slider_style::flags::none, _w( "%.f ms" ) ),
																																																																			std::make_shared<checkbox>( _w( "Recoil control" ), vars::legit.weapon_config.pistol.rcs ),
																																																																			std::make_shared<slider>( _w( "Recoil control speed" ), vars::legit.weapon_config.pistol.rcs_speed, 1.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),
																																																																	} ),
																																																															} ), ptr3 = std::make_shared<weapon_tab>( _w( "m4a1_silencer" ), std::vector<std::shared_ptr<child>>
																																																																		{std::make_shared<child>( _w( "Aimhelper" ), vec2( 16, 16 ), vec2( 235, 478 ), std::vector<std::shared_ptr<control>>
																																																																		{
																																																																			std::make_shared<checkbox>( _w( "Magnet" ), vars::legit.weapon_config.rifle.magnet )->add_control( std::make_shared<key_bind>( vars::legit.weapon_config.rifle.magnet, vars::key.magnet ) ),
																																																																				std::make_shared<combobox>( _w( "Magnet hitboxes" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																																			{
																																																																				{ _w( "Head" ), vars::legit.weapon_config.rifle.magnet_hitboxes.head },
																																																																				{ _w( "Chest" ), vars::legit.weapon_config.rifle.magnet_hitboxes.chest },
																																																																				{ _w( "Body" ), vars::legit.weapon_config.rifle.magnet_hitboxes.body },
																																																																				{ _w( "Pelvis" ), vars::legit.weapon_config.rifle.magnet_hitboxes.pelvis },
																																																																			} ),
																																																																				std::make_shared<slider>( _w( "Magnet fov" ), vars::legit.weapon_config.rifle.magnet_fov, 1.f, 50.f, slider_style::flags::none, _w( "%.f deg" ) ),
																																																																					std::make_shared<slider>( _w( "Magnet speed" ), vars::legit.weapon_config.rifle.magnet_speed, 1.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																					std::make_shared<slider>( _w( "Target delay" ), vars::legit.weapon_config.rifle.target_delay, 10.f, 500.f, slider_style::flags::none, _w( "%.f ms" ) ),
																																																																					std::make_shared<slider>( _w( "Limit percent" ), vars::legit.weapon_config.rifle.limit_factor, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																					std::make_shared<slider>( _w( "Override mouse percent" ), vars::legit.weapon_config.rifle.override_factor, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),

																																																																		} ),
																																																																std::make_shared<child>( _w( "Assists" ), vec2( 267, 16 ), vec2( 235, 478 ), std::vector<std::shared_ptr<control>>
																																																																				{
																																																																					std::make_shared<checkbox>( _w( "Triggerbot" ), vars::legit.weapon_config.rifle.triggerbot )->add_control( std::make_shared<key_bind>( vars::legit.weapon_config.rifle.triggerbot, vars::key.triggerbot ) ),
																																																																						std::make_shared<slider>( _w( "Reaction time" ), vars::legit.weapon_config.rifle.reaction_time, 0.f, 500.f, slider_style::flags::none, _w( "%.f ms" ) ),
																																																																						std::make_shared<checkbox>( _w( "Recoil control" ), vars::legit.weapon_config.rifle.rcs ),
																																																																						std::make_shared<slider>( _w( "Recoil control speed" ), vars::legit.weapon_config.rifle.rcs_speed, 1.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),
																																																																				} ),
																																																																		} ), ptr4 = std::make_shared<weapon_tab>( _w( "awp" ), std::vector<std::shared_ptr<child>>
																																																																					{std::make_shared<child>( _w( "Aimhelper" ), vec2( 16, 16 ), vec2( 235, 478 ), std::vector<std::shared_ptr<control>>
																																																																					{
																																																																						std::make_shared<checkbox>( _w( "Magnet" ), vars::legit.weapon_config.sniper.magnet )->add_control( std::make_shared<key_bind>( vars::legit.weapon_config.sniper.magnet, vars::key.magnet ) ),
																																																																							std::make_shared<combobox>( _w( "Magnet hitboxes" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																																						{
																																																																							{ _w( "Head" ), vars::legit.weapon_config.sniper.magnet_hitboxes.head },
																																																																							{ _w( "Chest" ), vars::legit.weapon_config.sniper.magnet_hitboxes.chest },
																																																																							{ _w( "Body" ), vars::legit.weapon_config.sniper.magnet_hitboxes.body },
																																																																							{ _w( "Pelvis" ), vars::legit.weapon_config.sniper.magnet_hitboxes.pelvis },
																																																																						} ),
																																																																							std::make_shared<checkbox>( _w( "Only in scope" ), vars::legit.weapon_config.sniper.magnet_only_in_scope ),
																																																																								std::make_shared<slider>( _w( "Magnet fov" ), vars::legit.weapon_config.sniper.magnet_fov, 1.f, 50.f, slider_style::flags::none, _w( "%.f deg" ) ),
																																																																								std::make_shared<slider>( _w( "Magnet speed" ), vars::legit.weapon_config.sniper.magnet_speed, 1.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																								std::make_shared<slider>( _w( "Target delay" ), vars::legit.weapon_config.sniper.target_delay, 10.f, 500.f, slider_style::flags::none, _w( "%.f ms" ) ),
																																																																								std::make_shared<slider>( _w( "Limit percent" ), vars::legit.weapon_config.sniper.limit_factor, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																								std::make_shared<slider>( _w( "Override mouse percent" ), vars::legit.weapon_config.sniper.override_factor, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),

																																																																					} ),
																																																												std::make_shared<child>( _w( "Assists" ), vec2( 267, 16 ), vec2( 235, 478 ), std::vector<std::shared_ptr<control>>
																																																																							{
																																																																								std::make_shared<checkbox>( _w( "Triggerbot" ), vars::legit.weapon_config.sniper.triggerbot )->add_control( std::make_shared<key_bind>( vars::legit.weapon_config.sniper.triggerbot, vars::key.triggerbot ) ),
																																																																									std::make_shared<checkbox>( _w( "Only in scope" ), vars::legit.weapon_config.sniper.triggerbot_only_in_scope ),
																																																																									std::make_shared<slider>( _w( "Reaction time" ), vars::legit.weapon_config.sniper.reaction_time, 0.f, 500.f, slider_style::flags::none, _w( "%.f ms" ) ),
																																																																									std::make_shared<checkbox>( _w( "Recoil control" ), vars::legit.weapon_config.sniper.rcs ),
																																																																									std::make_shared<slider>( _w( "Recoil control speed" ), vars::legit.weapon_config.sniper.rcs_speed, 1.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),
																																																																							} ),
																																																																					} ), ptr5 = std::make_shared<weapon_tab>( _w( "mp5sd" ), std::vector<std::shared_ptr<child>>
																																																																								{std::make_shared<child>( _w( "Aimhelper" ), vec2( 16, 16 ), vec2( 235, 478 ), std::vector<std::shared_ptr<control>>
																																																																								{
																																																																									std::make_shared<checkbox>( _w( "Magnet" ), vars::legit.weapon_config.smg.magnet )->add_control( std::make_shared<key_bind>( vars::legit.weapon_config.smg.magnet, vars::key.magnet ) ),
																																																																										std::make_shared<combobox>( _w( "Magnet hitboxes" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																																									{
																																																																										{ _w( "Head" ), vars::legit.weapon_config.smg.magnet_hitboxes.head },
																																																																										{ _w( "Chest" ), vars::legit.weapon_config.smg.magnet_hitboxes.chest },
																																																																										{ _w( "Body" ), vars::legit.weapon_config.smg.magnet_hitboxes.body },
																																																																										{ _w( "Pelvis" ), vars::legit.weapon_config.smg.magnet_hitboxes.pelvis },
																																																																									} ),
																																																																										std::make_shared<slider>( _w( "Magnet fov" ), vars::legit.weapon_config.smg.magnet_fov, 1.f, 50.f, slider_style::flags::none, _w( "%.f deg" ) ),
																																																																											std::make_shared<slider>( _w( "Magnet speed" ), vars::legit.weapon_config.smg.magnet_speed, 1.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																											std::make_shared<slider>( _w( "Target delay" ), vars::legit.weapon_config.smg.target_delay, 10.f, 500.f, slider_style::flags::none, _w( "%.f ms" ) ),
																																																																											std::make_shared<slider>( _w( "Limit percent" ), vars::legit.weapon_config.smg.limit_factor, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																											std::make_shared<slider>( _w( "Override mouse percent" ), vars::legit.weapon_config.smg.override_factor, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),

																																																																								} ),
																																																								std::make_shared<child>( _w( "Assists" ), vec2( 267, 16 ), vec2( 235, 478 ), std::vector<std::shared_ptr<control>>
																																																																										{
																																																																											std::make_shared<checkbox>( _w( "Triggerbot" ), vars::legit.weapon_config.smg.triggerbot )->add_control( std::make_shared<key_bind>( vars::legit.weapon_config.smg.triggerbot, vars::key.triggerbot ) ),
																																																																												std::make_shared<slider>( _w( "Reaction time" ), vars::legit.weapon_config.smg.reaction_time, 0.f, 500.f, slider_style::flags::none, _w( "%.f ms" ) ),
																																																																												std::make_shared<checkbox>( _w( "Recoil control" ), vars::legit.weapon_config.smg.rcs ),
																																																																												std::make_shared<slider>( _w( "Recoil control speed" ), vars::legit.weapon_config.smg.rcs_speed, 1.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),
																																																																										} ),
																																																																								} ), ptr6 = std::make_shared<weapon_tab>( _w( "mag7" ), std::vector<std::shared_ptr<child>>
																																																																											{std::make_shared<child>( _w( "Aimhelper" ), vec2( 16, 16 ), vec2( 235, 478 ), std::vector<std::shared_ptr<control>>
																																																																											{
																																																																												std::make_shared<checkbox>( _w( "Magnet" ), vars::legit.weapon_config.shotgun.magnet )->add_control( std::make_shared<key_bind>( vars::legit.weapon_config.shotgun.magnet, vars::key.magnet ) ),
																																																																													std::make_shared<combobox>( _w( "Magnet hitboxes" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																																												{
																																																																													{ _w( "Head" ), vars::legit.weapon_config.shotgun.magnet_hitboxes.head },
																																																																													{ _w( "Chest" ), vars::legit.weapon_config.shotgun.magnet_hitboxes.chest },
																																																																													{ _w( "Body" ), vars::legit.weapon_config.shotgun.magnet_hitboxes.body },
																																																																													{ _w( "Pelvis" ), vars::legit.weapon_config.shotgun.magnet_hitboxes.pelvis },
																																																																												} ),
																																																																													std::make_shared<slider>( _w( "Magnet fov" ), vars::legit.weapon_config.shotgun.magnet_fov, 1.f, 50.f, slider_style::flags::none, _w( "%.f deg" ) ),
																																																																														std::make_shared<slider>( _w( "Magnet speed" ), vars::legit.weapon_config.shotgun.magnet_speed, 1.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																														std::make_shared<slider>( _w( "Target delay" ), vars::legit.weapon_config.shotgun.target_delay, 10.f, 500.f, slider_style::flags::none, _w( "%.f ms" ) ),
																																																																														std::make_shared<slider>( _w( "Limit percent" ), vars::legit.weapon_config.shotgun.limit_factor, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																														std::make_shared<slider>( _w( "Override mouse percent" ), vars::legit.weapon_config.shotgun.override_factor, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),

																																																																											} ),
																																																				std::make_shared<child>( _w( "Assists" ), vec2( 267, 16 ), vec2( 235, 478 ), std::vector<std::shared_ptr<control>>
																																																																													{
																																																																														std::make_shared<checkbox>( _w( "Triggerbot" ), vars::legit.weapon_config.shotgun.triggerbot )->add_control( std::make_shared<key_bind>( vars::legit.weapon_config.shotgun.triggerbot, vars::key.triggerbot ) ),
																																																																															std::make_shared<slider>( _w( "Reaction time" ), vars::legit.weapon_config.shotgun.reaction_time, 0.f, 500.f, slider_style::flags::none, _w( "%.f ms" ) ),
																																																																															std::make_shared<checkbox>( _w( "Recoil control" ), vars::legit.weapon_config.shotgun.rcs ),
																																																																															std::make_shared<slider>( _w( "Recoil control speed" ), vars::legit.weapon_config.shotgun.rcs_speed, 1.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),
																																																																													} ),
																																																																											} ), ptr7 = std::make_shared<weapon_tab>( _w( "m249" ), std::vector<std::shared_ptr<child>>
																																																																														{std::make_shared<child>( _w( "Aimhelper" ), vec2( 16, 16 ), vec2( 235, 478 ), std::vector<std::shared_ptr<control>>
																																																																														{
																																																																															std::make_shared<checkbox>( _w( "Magnet" ), vars::legit.weapon_config.lmg.magnet )->add_control( std::make_shared<key_bind>( vars::legit.weapon_config.lmg.magnet, vars::key.magnet ) ),
																																																																																std::make_shared<combobox>( _w( "Magnet hitboxes" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																																															{
																																																																																{ _w( "Head" ), vars::legit.weapon_config.lmg.magnet_hitboxes.head },
																																																																																{ _w( "Chest" ), vars::legit.weapon_config.lmg.magnet_hitboxes.chest },
																																																																																{ _w( "Body" ), vars::legit.weapon_config.lmg.magnet_hitboxes.body },
																																																																																{ _w( "Pelvis" ), vars::legit.weapon_config.lmg.magnet_hitboxes.pelvis },
																																																																															} ),
																																																																																std::make_shared<slider>( _w( "Magnet fov" ), vars::legit.weapon_config.lmg.magnet_fov, 1.f, 50.f, slider_style::flags::none, _w( "%.f deg" ) ),
																																																																																	std::make_shared<slider>( _w( "Magnet speed" ), vars::legit.weapon_config.lmg.magnet_speed, 1.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																																	std::make_shared<slider>( _w( "Target delay" ), vars::legit.weapon_config.lmg.target_delay, 10.f, 500.f, slider_style::flags::none, _w( "%.f ms" ) ),
																																																																																	std::make_shared<slider>( _w( "Limit percent" ), vars::legit.weapon_config.lmg.limit_factor, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																																	std::make_shared<slider>( _w( "Override mouse percent" ), vars::legit.weapon_config.lmg.override_factor, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),

																																																																														} ),
																																																std::make_shared<child>( _w( "Assists" ), vec2( 267, 16 ), vec2( 235, 478 ), std::vector<std::shared_ptr<control>>
																																																																																{
																																																																																	std::make_shared<checkbox>( _w( "Triggerbot" ), vars::legit.weapon_config.lmg.triggerbot )->add_control( std::make_shared<key_bind>( vars::legit.weapon_config.lmg.triggerbot, vars::key.triggerbot ) ),
																																																																																		std::make_shared<slider>( _w( "Reaction time" ), vars::legit.weapon_config.lmg.reaction_time, 0.f, 500.f, slider_style::flags::none, _w( "%.f ms" ) ),
																																																																																		std::make_shared<checkbox>( _w( "Recoil control" ), vars::legit.weapon_config.lmg.rcs ),
																																																																																		std::make_shared<slider>( _w( "Recoil control speed" ), vars::legit.weapon_config.lmg.rcs_speed, 1.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) ),
																																																																																} ), } ), }, true ),

																																																																std::make_shared<tab>( _w( "VISUALS" ), std::vector<std::shared_ptr<sub_tab>>
																																																																																	{

																																																																																		std::make_shared<sub_tab>( _w( "ESP" ), std::vector<std::shared_ptr<child>>
																																																																																		{std::make_shared<child>( _w( "Player" ), vec2( 16, 16 ), vec2( 235, 486 ), std::vector<std::shared_ptr<control>>
																																																																																		{std::make_shared<checkbox>( _w( "Esp" ), vars::visuals.esp.enabled )->add_control( std::make_shared<key_bind>( vars::visuals.esp.enabled, vars::key.esp ) ),
																																																																																			std::make_shared<checkbox>( _w( "Box" ), vars::visuals.esp.box )->add_control( std::make_shared<color_picker>( vars::visuals.esp.box_color, colorpicker_style::flags::no_alpha_bar ) ),
																																																																																			std::make_shared<checkbox>( _w( "Skeleton" ), vars::visuals.esp.skeleton )->add_control( std::make_shared<color_picker>( vars::visuals.esp.skeleton_color ) ),
																																																																																			std::make_shared<checkbox>( _w( "History skeleton" ), vars::visuals.esp.history_skeleton )->add_control( std::make_shared<color_picker>( vars::visuals.esp.history_skeleton_color ) ),
																																																																																			std::make_shared<checkbox>( _w( "Name" ), vars::visuals.esp.name )->add_control( std::make_shared<color_picker>( vars::visuals.esp.name_color, colorpicker_style::flags::no_alpha_bar ) ),
																																																																																			std::make_shared<checkbox>( _w( "Weapon" ), vars::visuals.esp.weapon )->add_control( std::make_shared<color_picker>( vars::visuals.esp.weapon_color, colorpicker_style::flags::no_alpha_bar ) ),
																																																																																			std::make_shared<checkbox>( _w( "Weapon icons" ), vars::visuals.esp.weapon_icon )->add_control( std::make_shared<color_picker>( vars::visuals.esp.weapon_icon_color, colorpicker_style::flags::no_alpha_bar ) ),
																																																																																			std::make_shared<checkbox>( _w( "Health" ), vars::visuals.esp.health )->add_control( std::make_shared<color_picker>( vars::visuals.esp.health_color_1, colorpicker_style::flags::no_alpha_bar ) )->add_control( std::make_shared<color_picker>( vars::visuals.esp.health_color_2, colorpicker_style::flags::no_alpha_bar ) ),
																																																																																			std::make_shared<combobox>( _w( "Health style" ), vars::visuals.esp.health_style, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																		{
																																																																																			{ _w( "Solid" ), 0 },
																																																																																			{ _w( "Gradient" ), 1 },
																																																																																		} )->add_dependency( { vars::visuals.esp.health, 1.0 } ),
																																																																																				std::make_shared<checkbox>( _w( "Armor" ), vars::visuals.esp.armor )->add_control( std::make_shared<color_picker>( vars::visuals.esp.armor_color, colorpicker_style::flags::no_alpha_bar ) ),
																																																																																				std::make_shared<checkbox>( _w( "Ammo" ), vars::visuals.esp.ammo )->add_control( std::make_shared<color_picker>( vars::visuals.esp.ammo_color, colorpicker_style::flags::no_alpha_bar ) ),
																																																																																				std::make_shared<checkbox>( _w( "Pingspike" ), vars::visuals.esp.ping )->add_control( std::make_shared<color_picker>( vars::visuals.esp.ping_color, colorpicker_style::flags::no_alpha_bar ) ),
																																																																																				std::make_shared<combobox>( _w( "Info" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																																																			{
																																																																																				{ _w( "Money" ), vars::visuals.esp.flags.money },
																																																																																				{ _w( "Armor" ), vars::visuals.esp.flags.armor },
																																																																																				{ _w( "Hittable" ), vars::visuals.esp.flags.hit },
																																																																																				{ _w( "Defuser" ), vars::visuals.esp.flags.defuser },
																																																																																				{ _w( "Taser" ), vars::visuals.esp.flags.taser },
																																																																																				{ _w( "Bomb" ), vars::visuals.esp.flags.bomb },
																																																																																				{ _w( "Scoped" ), vars::visuals.esp.flags.scoped },
																																																																																				{ _w( "LC" ), vars::visuals.esp.flags.lc },
																																																																																				{ _w( "Fake duck" ), vars::visuals.esp.flags.fd },
																																																																																				{ _w( "Exploiting" ), vars::visuals.esp.flags.xp },
																																																																																			} ),
																																																																																				std::make_shared<checkbox>( _w( "Sound" ), vars::visuals.esp.sound_esp )->add_control( std::make_shared<color_picker>( vars::visuals.esp.sound_esp_color ) ),
																																																																																					std::make_shared<checkbox>( _w( "Glow" ), vars::visuals.glow.enabled ),
																																																																																					std::make_shared<checkbox>( _w( "Team glow" ), vars::visuals.glow.team.enabled )->add_control( std::make_shared<color_picker>( vars::visuals.glow.team.color ) ),
																																																																																					std::make_shared<checkbox>( _w( "Enemy glow" ), vars::visuals.glow.enemy.enabled )->add_control( std::make_shared<color_picker>( vars::visuals.glow.enemy.color ) ),
																																																																																					std::make_shared<checkbox>( _w( "Local glow" ), vars::visuals.glow.local.enabled )->add_control( std::make_shared<color_picker>( vars::visuals.glow.local.color ) ),
																																																																																					std::make_shared<checkbox>( _w( "Shared esp" ), vars::misc.shared_esp ),
																																																																																					std::make_shared<checkbox>( _w( "Share esp with enemies" ), vars::misc.shared_esp_with_enemy ),
																																																																																					std::make_shared<combobox>( _w( "Animation disablers" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																																																				{
																																																																																					{ _w( "Dormant" ), vars::visuals.disable_dormancy_interp },
																																																																																					{ _w( "Model fade in/out" ), vars::visuals.disable_model_fade },
																																																																																					{ _w( "ESP fade in/out" ), vars::visuals.disable_esp_fade },
																																																																																					{ _w( "Healthbar" ), vars::visuals.disable_health_bar_interp },
																																																																																					{ _w( "Thirdperson" ), vars::visuals.disable_thirdperson_interp },
																																																																																				} ),

																																																																																		} ),

																																																																																		std::make_shared<child>( _w( "World" ), vec2( 267, 16 ), vec2( 235, 486 ), std::vector<std::shared_ptr<control>>
																																																																																					{
																																																																																						std::make_shared<combobox>( _w( "Esp" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																																																						{
																																																																																							{ _w( "Weapons" ), vars::visuals.esp.world.weapons },
																																																																																							{ _w( "Grenades" ), vars::visuals.esp.world.grenades },
																																																																																							{ _w( "C4" ), vars::visuals.esp.world.c4 },
																																																																																							{ _w( "Danger-Zone" ), vars::visuals.esp.world.danger_zone },
																																																																																						} )->add_control( std::make_shared<color_picker>( vars::visuals.esp.world.color, colorpicker_style::flags::no_alpha_bar | colorpicker_style::flags::open_left ) ),
																																																																																								std::make_shared<checkbox>( _w( "Weapon icons" ), vars::visuals.esp.world.weapon_icon )->add_dependency( { vars::visuals.esp.world.weapons, 1.0 } ),
																																																																																								std::make_shared<combobox>( _w( "Glow" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																																																							{
																																																																																								{ _w( "Weapons" ), vars::visuals.glow.world.weapons },
																																																																																								{ _w( "Grenades" ), vars::visuals.glow.world.grenades },
																																																																																								{ _w( "C4" ), vars::visuals.glow.world.c4 },
																																																																																							} )->add_control( std::make_shared<color_picker>( vars::visuals.glow.world.color, colorpicker_style::flags::open_left ) ),
																																																																																					} ),
																																																																																						std::make_shared<child>( _w( "Preview" ), vec2( 518, 16 ), vec2( 235, 370 ), std::vector<std::shared_ptr<control>>{
																																																																																									std::make_shared<combobox>( "", vars::menu.preview_type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																									{
																																																																																										{ _w( "Enemy" ), 0 },
																																																																																										{ _w( "Team" ), 1 },
																																																																																										{ _w( "Local" ), 2 },
																																																																																									} )
																																																																																								} ),
																																																																																		} ),
																																																																																			std::make_shared<sub_tab>( _w( "MODELS" ), std::vector<std::shared_ptr<child>>
																																																																																										{std::make_shared<child>( _w( "General" ), vec2( 16, 16 ), vec2( 235, 57 ), std::vector<std::shared_ptr<control>>
																																																																																										{
																																																																																											std::make_shared<checkbox>( _w( "Chams" ), vars::visuals.chams.enabled )->add_control( std::make_shared<key_bind>( vars::visuals.chams.enabled, vars::key.chams ) ),
																																																																																												std::make_shared<checkbox>( _w( "Skip occlusion" ), vars::visuals.chams.extended ),
																																																																																												/*std::make_shared<checkbox>( _w( "Custom bloom scale" ), vars::visuals.chams.custom_bloom ),
																																																																																												std::make_shared<slider>( _w( "Scale" ), vars::visuals.chams.custom_bloom_scale, 0.f, 100.f, slider_style::flags::none, _w( "%.f" ) )->add_dependency( { vars::visuals.chams.custom_bloom, 1.0 } ),
																																																																																												std::make_shared<checkbox>( _w( "Custom model brightness" ), vars::visuals.chams.model_brightness ),
																																																																																												std::make_shared<slider>( _w( "Amount" ), vars::visuals.chams.model_brightness_amt, 0.f, 100.f, slider_style::flags::none, _w( "%.f" ) )->add_dependency( { vars::visuals.chams.model_brightness, 1.0 } ),*/

																																																																																										} ),
																																																																																			std::make_shared<child>( _w( "Team" ), vec2( 267, 265 ), vec2( 235, 237 ), std::vector<std::shared_ptr<control>>
																																																																																											{
																																																																																												std::make_shared<combobox>( _w( "Player" ), vars::visuals.chams.team.type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																												{
																																																																																													{ _w( "Off" ), 0 },
																																																																																													{ _w( "Normal" ), 1 },
																																																																																													{ _w( "Flat" ), 2 },
																																																																																													{ _w( "Rim Glow" ), 3 },
																																																																																													{ _w( "Contour" ), 4 },
																																																																																													{ _w( "Flow" ), 5 },
																																																																																													{ _w( "Acrylic" ), 6 },
																																																																																													{ _w( "Legacy" ), 7 },
																																																																																												} )->add_control( std::make_shared<color_picker>( vars::visuals.chams.team.color, colorpicker_style::flags::open_left ) )->add_control( std::make_shared<color_picker>( vars::visuals.chams.team.color2, colorpicker_style::flags::no_alpha_bar | colorpicker_style::flags::open_left )->add_dependency( { vars::visuals.chams.team.type, 4.0 } )->add_dependency( { vars::visuals.chams.team.type, 3.0 } ) ),
																																																																																														std::make_shared<checkbox>( _w( "Ignore z" ), vars::visuals.chams.team.hidden )->add_control( std::make_shared<color_picker>( vars::visuals.chams.team.hidden_color, colorpicker_style::flags::open_left ) ),
																																																																																														std::make_shared<combobox>( _w( "Ragdoll" ), vars::visuals.chams.team.ragdoll.type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																													{
																																																																																														{ _w( "Off" ), 0 },
																																																																																														{ _w( "Normal" ), 1 },
																																																																																														{ _w( "Flat" ), 2 },
																																																																																														{ _w( "Rim Glow" ), 3 },
																																																																																														{ _w( "Contour" ), 4 },
																																																																																														{ _w( "Flow" ), 5 },
																																																																																														{ _w( "Acrylic" ), 6 },
																																																																																														{ _w( "Legacy" ), 7 },
																																																																																													} )->add_control( std::make_shared<color_picker>( vars::visuals.chams.team.ragdoll.color, colorpicker_style::flags::open_left ) )->add_control( std::make_shared<color_picker>( vars::visuals.chams.team.ragdoll.color2, colorpicker_style::flags::no_alpha_bar | colorpicker_style::flags::open_left )->add_dependency( { vars::visuals.chams.team.ragdoll.type, 4.0 } )->add_dependency( { vars::visuals.chams.team.ragdoll.type, 3.0 } ) ),
																																																																																															std::make_shared<combobox>( _w( "Attachments" ), vars::visuals.chams.team.attachment.type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																														{
																																																																																															{ _w( "Off" ), 0 },
																																																																																															{ _w( "Normal" ), 1 },
																																																																																															{ _w( "Flat" ), 2 },
																																																																																															{ _w( "Rim Glow" ), 3 },
																																																																																															{ _w( "Contour" ), 4 },
																																																																																															{ _w( "Flow" ), 5 },
																																																																																															{ _w( "Acrylic" ), 6 },
																																																																																															{ _w( "Legacy" ), 7 },
																																																																																														} )->add_control( std::make_shared<color_picker>( vars::visuals.chams.team.attachment.color, colorpicker_style::flags::open_left ) )->add_control( std::make_shared<color_picker>( vars::visuals.chams.team.attachment.color2, colorpicker_style::flags::no_alpha_bar | colorpicker_style::flags::open_left )->add_dependency( { vars::visuals.chams.team.attachment.type, 4.0 } )->add_dependency( { vars::visuals.chams.team.attachment.type, 3.0 } ) ),
																																																																																																std::make_shared<checkbox>( _w( "Ignore z" ), vars::visuals.chams.team.attachment.hidden )->add_dependency( { vars::visuals.chams.team.attachment.type, 0.0, true } ),
																																																																																											} ),
																																																																																												std::make_shared<child>( _w( "Enemy" ), vec2( 267, 16 ), vec2( 235, 233 ), std::vector<std::shared_ptr<control>>
																																																																																															{
																																																																																																std::make_shared<combobox>( _w( "Player" ), vars::visuals.chams.enemy.type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																{
																																																																																																	{ _w( "Off" ), 0 },
																																																																																																	{ _w( "Normal" ), 1 },
																																																																																																	{ _w( "Flat" ), 2 },
																																																																																																	{ _w( "Rim Glow" ), 3 },
																																																																																																	{ _w( "Contour" ), 4 },
																																																																																																	{ _w( "Flow" ), 5 },
																																																																																																	{ _w( "Acrylic" ), 6 },
																																																																																																	{ _w( "Legacy" ), 7 },
																																																																																																} )->add_control( std::make_shared<color_picker>( vars::visuals.chams.enemy.color, colorpicker_style::flags::open_left ) )->add_control( std::make_shared<color_picker>( vars::visuals.chams.enemy.color2, colorpicker_style::flags::no_alpha_bar | colorpicker_style::flags::open_left )->add_dependency( { vars::visuals.chams.enemy.type, 4.0 } )->add_dependency( { vars::visuals.chams.enemy.type, 3.0 } ) ),
																																																																																																		std::make_shared<checkbox>( _w( "Ignore z" ), vars::visuals.chams.enemy.hidden )->add_control( std::make_shared<color_picker>( vars::visuals.chams.enemy.hidden_color, colorpicker_style::flags::open_left ) ),
																																																																																																		std::make_shared<checkbox>( _w( "History" ), vars::visuals.chams.history.enabled )->add_control( std::make_shared<color_picker>( vars::visuals.chams.history.color, colorpicker_style::flags::open_left ) ),
																																																																																																		std::make_shared<combobox>( _w( "Shot record" ), vars::visuals.chams.enemy.shot_record.type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																	{
																																																																																																		{ _w( "Off" ), 0 },
																																																																																																		{ _w( "Normal" ), 1 },
																																																																																																		{ _w( "Flat" ), 2 },
																																																																																																		{ _w( "Rim Glow" ), 3 },
																																																																																																		{ _w( "Contour" ), 4 },
																																																																																																		{ _w( "Flow" ), 5 },
																																																																																																		{ _w( "Acrylic" ), 6 },
																																																																																																		{ _w( "Legacy" ), 7 },
																																																																																																	} )->add_control( std::make_shared<color_picker>( vars::visuals.chams.enemy.shot_record.color, colorpicker_style::flags::open_left ) )->add_control( std::make_shared<color_picker>( vars::visuals.chams.enemy.shot_record.color2, colorpicker_style::flags::no_alpha_bar | colorpicker_style::flags::open_left )->add_dependency( { vars::visuals.chams.enemy.shot_record.type, 4.0 } )->add_dependency( { vars::visuals.chams.enemy.shot_record.type, 3.0 } ) ),
																																																																																																			std::make_shared<combobox>( _w( "Ragdoll" ), vars::visuals.chams.enemy.ragdoll.type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																		{
																																																																																																			{ _w( "Off" ), 0 },
																																																																																																			{ _w( "Normal" ), 1 },
																																																																																																			{ _w( "Flat" ), 2 },
																																																																																																			{ _w( "Rim Glow" ), 3 },
																																																																																																			{ _w( "Contour" ), 4 },
																																																																																																			{ _w( "Flow" ), 5 },
																																																																																																			{ _w( "Acrylic" ), 6 },
																																																																																																			{ _w( "Legacy" ), 7 },
																																																																																																		} )->add_control( std::make_shared<color_picker>( vars::visuals.chams.enemy.ragdoll.color, colorpicker_style::flags::open_left ) )->add_control( std::make_shared<color_picker>( vars::visuals.chams.enemy.ragdoll.color2, colorpicker_style::flags::no_alpha_bar | colorpicker_style::flags::open_left )->add_dependency( { vars::visuals.chams.enemy.ragdoll.type, 4.0 } )->add_dependency( { vars::visuals.chams.enemy.ragdoll.type, 3.0 } ) ),
																																																																																																				std::make_shared<combobox>( _w( "Attachments" ), vars::visuals.chams.enemy.attachment.type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																			{
																																																																																																				{ _w( "Off" ), 0 },
																																																																																																				{ _w( "Normal" ), 1 },
																																																																																																				{ _w( "Flat" ), 2 },
																																																																																																				{ _w( "Rim Glow" ), 3 },
																																																																																																				{ _w( "Contour" ), 4 },
																																																																																																				{ _w( "Flow" ), 5 },
																																																																																																				{ _w( "Acrylic" ), 6 },
																																																																																																				{ _w( "Legacy" ), 7 },
																																																																																																			} )->add_control( std::make_shared<color_picker>( vars::visuals.chams.enemy.attachment.color, colorpicker_style::flags::open_left ) )->add_control( std::make_shared<color_picker>( vars::visuals.chams.enemy.attachment.color2, colorpicker_style::flags::no_alpha_bar | colorpicker_style::flags::open_left )->add_dependency( { vars::visuals.chams.enemy.attachment.type, 4.0 } )->add_dependency( { vars::visuals.chams.enemy.attachment.type, 3.0 } ) ),
																																																																																																					std::make_shared<checkbox>( _w( "Ignore z" ), vars::visuals.chams.enemy.attachment.hidden )->add_dependency( { vars::visuals.chams.enemy.attachment.type, 0.0, true } ),
																																																																																															} ),
																																																																																																std::make_shared<child>( _w( "Local" ), vec2( 16, 89 ), vec2( 235, 413 ), std::vector<std::shared_ptr<control>>
																																																																																																				{
																																																																																																					std::make_shared<combobox>( _w( "Type" ), vars::visuals.chams.local.type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																					{
																																																																																																						{ _w( "Off" ), 0 },
																																																																																																						{ _w( "Normal" ), 1 },
																																																																																																						{ _w( "Flat" ), 2 },
																																																																																																						{ _w( "Rim Glow" ), 3 },
																																																																																																						{ _w( "Contour" ), 4 },
																																																																																																						{ _w( "Flow" ), 5 },
																																																																																																						{ _w( "Acrylic" ), 6 },
																																																																																																						{ _w( "Legacy" ), 7 },
																																																																																																					} )->add_control( std::make_shared<color_picker>( vars::visuals.chams.local.color ) )->add_control( std::make_shared<color_picker>( vars::visuals.chams.local.color2, colorpicker_style::flags::no_alpha_bar )->add_dependency( { vars::visuals.chams.local.type, 4.0 } )->add_dependency( { vars::visuals.chams.local.type, 3.0 } ) ),
																																																																																																							std::make_shared<combobox>( _w( "Fake type" ), vars::visuals.chams.local.fake.type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																						{
																																																																																																							{ _w( "Off" ), 0 },
																																																																																																							{ _w( "Normal" ), 1 },
																																																																																																							{ _w( "Flat" ), 2 },
																																																																																																							{ _w( "Rim Glow" ), 3 },
																																																																																																							{ _w( "Contour" ), 4 },
																																																																																																							{ _w( "Flow" ), 5 },
																																																																																																							{ _w( "Acrylic" ), 6 },
																																																																																																							{ _w( "Legacy" ), 7 },
																																																																																																						} )->add_control( std::make_shared<color_picker>( vars::visuals.chams.local.fake.color ) )->add_control( std::make_shared<color_picker>( vars::visuals.chams.local.fake.color2, colorpicker_style::flags::no_alpha_bar )->add_dependency( { vars::visuals.chams.local.fake.type, 4.0 } )->add_dependency( { vars::visuals.chams.local.fake.type, 3.0 } ) ),
																																																																																																								std::make_shared<checkbox>( _w( "Interpolate" ), vars::visuals.chams.local.fake.interpolate ),
																																																																																																								std::make_shared<checkbox>( _w( "Pulsate" ), vars::visuals.chams.local.fake.pulsate ),
																																																																																																								std::make_shared<combobox>( _w( "Arms" ), vars::visuals.chams.local.arms.type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																							{
																																																																																																								{ _w( "Off" ), 0 },
																																																																																																								{ _w( "Normal" ), 1 },
																																																																																																								{ _w( "Flat" ), 2 },
																																																																																																								{ _w( "Rim Glow" ), 3 },
																																																																																																								{ _w( "Contour" ), 4 },
																																																																																																								{ _w( "Flow" ), 5 },
																																																																																																								{ _w( "Acrylic" ), 6 },
																																																																																																								{ _w( "Legacy" ), 7 },
																																																																																																							} )->add_control( std::make_shared<color_picker>( vars::visuals.chams.local.arms.color ) )->add_control( std::make_shared<color_picker>( vars::visuals.chams.local.arms.color2, colorpicker_style::flags::no_alpha_bar )->add_dependency( { vars::visuals.chams.local.arms.type, 4.0 } )->add_dependency( { vars::visuals.chams.local.arms.type, 3.0 } ) ),
																																																																																																									std::make_shared<checkbox>( _w( "Hide sleeves" ), vars::visuals.chams.local.arms.hide_sleeves ),
																																																																																																									std::make_shared<combobox>( _w( "Attachments" ), vars::visuals.chams.local.attachment.type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																								{
																																																																																																									{ _w( "Off" ), 0 },
																																																																																																									{ _w( "Normal" ), 1 },
																																																																																																									{ _w( "Flat" ), 2 },
																																																																																																									{ _w( "Rim Glow" ), 3 },
																																																																																																									{ _w( "Contour" ), 4 },
																																																																																																									{ _w( "Flow" ), 5 },
																																																																																																									{ _w( "Acrylic" ), 6 },
																																																																																																									{ _w( "Legacy" ), 7 },
																																																																																																								} )->add_control( std::make_shared<color_picker>( vars::visuals.chams.local.attachment.color ) )->add_control( std::make_shared<color_picker>( vars::visuals.chams.local.attachment.color2, colorpicker_style::flags::no_alpha_bar )->add_dependency( { vars::visuals.chams.local.attachment.type, 4.0 } )->add_dependency( { vars::visuals.chams.local.attachment.type, 3.0 } ) ),
																																																																																																										std::make_shared<combobox>( _w( "Agent T" ), vars::visuals.model_t, combobox_style::flags::none, player_model_combo ),
																																																																																																										std::make_shared<combobox>( _w( "Agent CT" ), vars::visuals.model_ct, combobox_style::flags::none, player_model_combo ),
																																																																																																										std::make_shared<combobox>( _w( "Mask T" ), vars::visuals.mask_t, combobox_style::flags::none, player_mask_combo ),
																																																																																																										std::make_shared<combobox>( _w( "Mask CT" ), vars::visuals.mask_ct, combobox_style::flags::none, player_mask_combo ),
																																																																																																				} ),
																																																																																																					std::make_shared<child>( _w( "Preview" ), vec2( 518, 16 ), vec2( 235, 370 ), std::vector<std::shared_ptr<control>>{
																																																																																																										std::make_shared<combobox>( "", vars::menu.preview_type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																										{
																																																																																																											{ _w( "Enemy" ), 0 },
																																																																																																											{ _w( "Team" ), 1 },
																																																																																																											{ _w( "Local" ), 2 },
																																																																																																										} )
																																																																																																									} ),
																																																																																										} ),
																																																																																											std::make_shared<sub_tab>( _w( "MISC" ), std::vector<std::shared_ptr<child>>
																																																																																																											{
																																																																																																												std::make_shared<child>( _w( "Local" ), vec2( 16, 16 ), vec2( 235, 486 ), std::vector<std::shared_ptr<control>>
																																																																																																												{
																																																																																																													//std::make_shared<checkbox>( _w( "Flashlight" ), vars::visuals.flashlight )->add_control( std::make_shared<key_bind>( vars::visuals.flashlight, vars::key.flashlight ) ),
																																																																																																														//std::make_shared<slider>( _w( "Flashlight radius" ), vars::visuals.flashlight_radius, 10.f, 100.f, slider_style::flags::none, _w( "%.f" ) )->add_dependency( { vars::visuals.flashlight, 1.0 } ),
																																																																																																													std::make_shared<checkbox>( _w( "Thirdperson" ), vars::misc.thirdperson )->add_control( std::make_shared<key_bind>( vars::misc.thirdperson, vars::key.thirdperson ) ),
																																																																																																														std::make_shared<checkbox>( _w( "Force when dead" ), vars::misc.thirdperson_dead )->add_dependency( { vars::misc.thirdperson, 1.0 } ),
																																																																																																														std::make_shared<checkbox>( _w( "Disable on grenade" ), vars::misc.thirdperson_grenade )->add_dependency( { vars::misc.thirdperson, 1.0 } ),
																																																																																																														std::make_shared<slider>( _w( "Distance" ), vars::misc.thirdperson_dist, 40.f, 180.f, slider_style::flags::none, _w( "%.f" ) )->add_dependency( { vars::misc.thirdperson, 1.0 } ),
																																																																																																														std::make_shared<slider>( _w( "Transparency in scope" ), vars::misc.transparency_in_scope, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f %%" ) )->add_dependency( { vars::misc.thirdperson, 1.0 } ),
																																																																																																														std::make_shared<checkbox>( _w( "Fov changer" ), vars::misc.fov_enabled ),
																																																																																																														std::make_shared<slider>( _w( "Fov" ), vars::misc.fov, 60.f, 140.f, slider_style::flags::none, _w( "%.f" ) )->add_dependency( { vars::misc.fov_enabled, 1.0 } ),
																																																																																																														std::make_shared<slider>( _w( "Remove zoom" ), vars::misc.fov_scope, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) )->add_dependency( { vars::misc.fov_enabled, 1.0 } ),
																																																																																																														std::make_shared<checkbox>( _w( "Viewmodel fov changer" ), vars::misc.vm_fov_enabled ),
																																																																																																														std::make_shared<slider>( _w( "Viewmodel fov" ), vars::misc.vm_fov, 60.f, 140.f, slider_style::flags::none, _w( "%.f" ) )->add_dependency( { vars::misc.vm_fov_enabled, 1.0 } ),
																																																																																																														std::make_shared<slider>( _w( "Aspect ratio" ), vars::misc.aspect_ratio, 0.f, 200.f, slider_style::flags::none, _w( "%.f" ) ),
																																																																																																														//std::make_shared<checkbox>( _w( "Visualize local lagcomp" ), vars::aim.visualize_tickbase ),
																																																																																																														std::make_shared<checkbox>( _w( "Goblin mode" ), vars::misc.goblin_mode ),
																																																																																																														std::make_shared<checkbox>( _w( "Remove visual punch" ), vars::aim.punch ),
																																																																																																														std::make_shared<combobox>( _w( "Remove scope" ), vars::misc.remove_scope, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																													{
																																																																																																														{ _w( "Off" ), 0 },
																																																																																																														{ _w( "Static" ), 1 },
																																																																																																														{ _w( "Dynamic" ), 2 },
																																																																																																														{ _w( "Gradient static" ), 3 },
																																																																																																														{ _w( "Gradient dynamic" ), 4 },

																																																																																																													} )->add_control( std::make_shared<color_picker>( vars::misc.scope_line_color )->add_dependency( { vars::misc.remove_scope, 3.0 } )->add_dependency( { vars::misc.remove_scope, 4.0 } ) ),
																																																																																																															std::make_shared<slider>( _w( "Line size" ), vars::misc.scope_line_size, 5.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) )->add_dependency( { vars::misc.remove_scope, 3.0 } )->add_dependency( { vars::misc.remove_scope, 4.0 } ),
																																																																																																															std::make_shared<checkbox>( _w( "Invert" ), vars::misc.scope_line_invert )->add_dependency( { vars::misc.remove_scope, 3.0 } )->add_dependency( { vars::misc.remove_scope, 4.0 } ),

																																																																																																															std::make_shared<checkbox>( _w( "Penetration crosshair" ), vars::visuals.autowall_crosshair ),
																																																																																																															std::make_shared<checkbox>( _w( "Force crosshair" ), vars::misc.force_crosshair ),
																																																																																																															std::make_shared<combobox>( _w( "Spread circle" ), vars::visuals.spread_circle_type, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																														{
																																																																																																															{ _w( "Off" ), 0 },
																																																																																																															{ _w( "Gradient" ), 1 },
																																																																																																															{ _w( "Rainbow" ), 2 },
																																																																																																															{ _w( "Rainbow ( rotating )" ), 3 },
																																																																																																														} )->add_control( std::make_shared<color_picker>( vars::visuals.spread_circle_color1 )->add_dependency( { vars::visuals.spread_circle_type, 1.0 } ) )->add_control( std::make_shared<color_picker>( vars::visuals.spread_circle_color2 )->add_dependency( { vars::visuals.spread_circle_type, 1.0 } ) ),



																																																																																																												} ),
																																																																																																													std::make_shared<child>( _w( "Various" ), vec2( 267, 16 ), vec2( 235, 486 ), std::vector<std::shared_ptr<control>>
																																																																																																															{
																																																																																																																std::make_shared<checkbox>( _w( "Darkmode" ), vars::visuals.dark )->add_control( std::make_shared<color_picker>( vars::visuals.dark_color, colorpicker_style::flags::no_alpha_bar ) )->add_control( std::make_shared<color_picker>( vars::visuals.prop_color, colorpicker_style::flags::no_alpha_bar ) ),
																																																																																																																	std::make_shared<slider>( _w( "Darkmode amount" ), vars::visuals.darkness, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) )->add_dependency( { vars::visuals.dark, 1.0 } ),
																																																																																																																	std::make_shared<slider>( _w( "Prop Transparency" ), vars::visuals.prop_transparency, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ),
																																																																																																																	std::make_shared<combobox>( _w( "Skybox" ), vars::visuals.skybox, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																																{
																																																																																																																	{ _w( "Default" ), 0 },
																																																																																																																	{ _w( "sky_csgo_cloudy01" ), 1 },
																																																																																																																	{ _w( "sky_csgo_night02" ), 2 },
																																																																																																																	{ _w( "sky_csgo_night02b" ), 3 },
																																																																																																																	{ _w( "cs_baggage_skybox" ), 4 },
																																																																																																																	{ _w( "cs_tibet" ), 5 },
																																																																																																																	{ _w( "vietnam" ), 6 },
																																																																																																																	{ _w( "sky_lunacy" ), 7 },
																																																																																																																	{ _w( "embassy" ), 8 },
																																																																																																																	{ _w( "italy" ), 9 },
																																																																																																																	{ _w( "jungle" ), 10 },
																																																																																																																	{ _w( "office" ), 11 },
																																																																																																																	{ _w( "sky_cs15_daylight01_hdr" ), 12 },
																																																																																																																	{ _w( "sky_cs15_daylight02_hdr" ), 13 },
																																																																																																																	{ _w( "sky_day02_05" ), 14 },
																																																																																																																	{ _w( "nukeblank" ), 15 },
																																																																																																																	{ _w( "dustblank" ), 16 },
																																																																																																																	{ _w( "sky_venice" ), 17 },
																																																																																																																	{ _w( "sky_cs15_daylight03_hdr" ), 18 },
																																																																																																																	{ _w( "sky_cs15_daylight04_hdr" ), 19 },
																																																																																																																	{ _w( "vertigo" ), 20 },
																																																																																																																	{ _w( "vertigoblue_hdr" ), 21 },
																																																																																																																	{ _w( "sky_dust" ), 22 },
																																																																																																																	{ _w( "sky_hr_aztec" ), 23 },
																																																																																																																} )->add_control( std::make_shared<color_picker>( vars::visuals.skybox_color, colorpicker_style::flags::no_alpha_bar ) ),
																																																																																																																		std::make_shared<checkbox>( _w( "Disable post-processing" ), vars::visuals.disable_postprocessing ),
																																																																																																																		std::make_shared<combobox>( _w( "Hitmarker" ), vars::misc.hitmarker, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																																	{
																																																																																																																		{ _w( "None" ), 0 },
																																																																																																																		{ _w( "World" ), 1 },
																																																																																																																		{ _w( "Crosshair" ), 2 },
																																																																																																																	} ),


																																																																																																																			//std::make_shared<slider>( _w( "Volume" ), vars::misc.hitsound_volume, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) )->add_dependency( { vars::misc.hitsound_select, 0.0, true } ),
																																																																																																																			std::make_shared<checkbox>( _w( "Show Impacts" ), vars::misc.impacts )->add_control( std::make_shared<color_picker>( vars::misc.impacts_color, colorpicker_style::flags::no_alpha_bar ) )->add_control( std::make_shared<color_picker>( vars::misc.impacts_color2, colorpicker_style::flags::no_alpha_bar ) ),
																																																																																																																			std::make_shared<checkbox>( _w( "Remove blur" ), vars::misc.remove_blur ),
																																																																																																																			std::make_shared<checkbox>( _w( "Remove smoke" ), vars::misc.remove_smoke ),
																																																																																																																			std::make_shared<checkbox>( _w( "Remove flash" ), vars::misc.remove_flash ),
																																																																																																																			std::make_shared<checkbox>( _w( "OOF arrows" ), vars::visuals.out_of_fov )->add_control( std::make_shared<color_picker>( vars::visuals.out_of_fov_col ) ),
																																																																																																																			std::make_shared<slider>( _w( "Distance" ), vars::visuals.out_of_fov_dst, 1.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) )->add_dependency( { vars::visuals.out_of_fov, 1.0 } ),
																																																																																																																			std::make_shared<slider>( _w( "Size" ), vars::visuals.out_of_fov_size, 1.f, 50.f, slider_style::flags::none, _w( "%.fpx" ) )->add_dependency( { vars::visuals.out_of_fov, 1.0 } ),



																																																																																																															} ),

																																																																																																																std::make_shared<child>( _w( "Beams" ), vec2( 518, 16 ), vec2( 235, 370 ), std::vector<std::shared_ptr<control>>
																																																																																																																		{
																																																																																																																			std::make_shared<checkbox>( _w( "Enabled" ), vars::visuals.beams.enabled ),
																																																																																																																				std::make_shared<slider>( _w( "Width" ), vars::visuals.beams.width, 2.f, 20.f, slider_style::flags::none, _w( "%.f" ) ),
																																																																																																																				std::make_shared<checkbox>( _w( "Team" ), vars::visuals.beams.team.enabled )->add_control( std::make_shared<color_picker>( vars::visuals.beams.team.color ) ),
																																																																																																																				std::make_shared<checkbox>( _w( "Enemy" ), vars::visuals.beams.enemy.enabled )->add_control( std::make_shared<color_picker>( vars::visuals.beams.enemy.color ) ),
																																																																																																																				std::make_shared<checkbox>( _w( "Local" ), vars::visuals.beams.local.enabled )->add_control( std::make_shared<color_picker>( vars::visuals.beams.local.color ) ),
																																																																																																																		} ),

																																																																																																											} ),

																																																																																	} ),
																																																																																		std::make_shared<tab>( _w( "MISC" ), std::vector<std::shared_ptr<sub_tab>>
																																																																																																																			{

																																																																																																																				std::make_shared<sub_tab>( _w( "" ), std::vector<std::shared_ptr<child>>
																																																																																																																				{
																																																																																																																					std::make_shared<child>( _w( "Various" ), vec2( 16, 16 ), vec2( 235, 518 ), std::vector<std::shared_ptr<control>>
																																																																																																																					{
																																																																																																																						std::make_shared<checkbox>( _w( "Bypass sv_pure" ), vars::misc.sv_pure ),
																																																																																																																							std::make_shared<checkbox>( _w( "Auto-accept" ), vars::misc.autoaccept ),
																																																																																																																							std::make_shared<checkbox>( _w( "Radar" ), vars::misc.radar ),
																																																																																																																							std::make_shared<checkbox>( _w( "Impostor rank revealer" ), vars::misc.reveal_ranks ),
																																																																																																																							std::make_shared<checkbox>( _w( "Hurt log" ), vars::misc.hurtlog ),
																																																																																																																							std::make_shared<checkbox>( _w( "Additional info" ), vars::misc.hurtlog_info )->add_dependency( { vars::misc.hurtlog, 1.0 } ),
																																																																																																																							std::make_shared<checkbox>( _w( "Buy log" ), vars::misc.buylog ),
																																																																																																																							std::make_shared<combobox>( _w( "Hitsound" ), vars::misc.hitsound_select, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																																						{
																																																																																																																							{ _w( "None" ), 0 },
																																																																																																																							{ _w( "Arena switch" ), 4 },
																																																																																																																							{ _w( "Bell" ), 1 },
																																																																																																																							{ _w( "Cod" ), 2 },
																																																																																																																							{ _w( "Fatality" ), 3 },
																																																																																																																							{ _w( "Bepis" ), 5 },
																																																																																																																							{ _w( "Phonk" ), 6 },
																																																																																																																							{ _w( "Sparkle" ), 7 },
																																																																																																																							{ _w( "Cock" ), 8 },
																																																																																																																						} ),
																																																																																																																							std::make_shared<checkbox>( _w( "Preserve deathnotices" ), vars::misc.preservedn ),
																																																																																																																								std::make_shared<combobox>( _w( "Indicators" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																																																																																							{
																																																																																																																								{ _w( "Manual anti-aim" ), vars::aa.indicator },
																																																																																																																								{ _w( "Freestanding" ), vars::aa.fs_indicator },
																																																																																																																								{ _w( "LC" ), vars::aa.lc_indicator },
																																																																																																																								{ _w( "Double tap" ), vars::aa.dt_indicator },
																																																																																																																								{ _w( "Hide shot" ), vars::aa.st_indicator },
																																																																																																																								{ _w( "Damage override" ), vars::aa.do_indicator },
																																																																																																																								{ _w( "Force safety" ), vars::aa.safe_indicator },
																																																																																																																								{ _w( "Force bodyaim" ), vars::aa.baim_indicator },
																																																																																																																								{ _w( "Headshot only" ), vars::aa.hsonly_indicator },

																																																																																																																							} ),
																																																																																																																								std::make_shared<checkbox>( _w( "Clan tag" ), vars::misc.clantag ),
																																																																																																																									std::make_shared<slider>( _w( "Ragdoll thrust" ), vars::visuals.ragdoll_thrust, 1.f, 1000.f, slider_style::flags::none, _w( "%.f %% " ) ),
																																																																																																																									std::make_shared<checkbox>( _w( "Party mode" ), vars::misc.birthday_mode ),
																																																																																																																									std::make_shared<checkbox>( _w( "Grenade prediction" ), vars::misc.grenade_tracer )->add_control( std::make_shared<color_picker>( vars::misc.tracer_color ) )->add_control( std::make_shared<color_picker>( vars::misc.box_color ) ),
																																																																																																																									std::make_shared<checkbox>( _w( "Grenade proximity warning" ), vars::misc.grenade_helper ),
#if defined(ALPHA) || !defined(RELEASE)
																																																																																																																									std::make_shared<checkbox>( _w( "Debug info" ), vars::misc.debug_info ),
#endif


																																																																																																																					} ),
																																																																																																																						std::make_shared<child>( _w( "Movement" ), vec2( 267, 16 ), vec2( 235, 518 ), std::vector<std::shared_ptr<control>>
																																																																																																																								{
																																																																																																																									std::make_shared<checkbox>( _w( "Bhop" ), vars::misc.bhop ),
																																																																																																																										std::make_shared<checkbox>( _w( "Edge jump" ), vars::misc.edge_jump )->add_control( std::make_shared<key_bind>( vars::misc.edge_jump, vars::key.edge_jump ) ),
																																																																																																																										std::make_shared<combobox>( _w( "Autostrafer" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
																																																																																																																									{
																																																																																																																										{ _w( "Viewangles" ), vars::misc.autostrafe_viewangles },
																																																																																																																										{ _w( "Movement keys" ), vars::misc.autostrafe_movement_keys },
																																																																																																																										{ _w( "Easy strafe" ), vars::misc.autostrafe_easy_strafe },
																																																																																																																										{ _w( "On jump release" ), vars::misc.autostrafe_jump_release },
																																																																																																																									} ),
																																																																																																																										std::make_shared<slider>( _w( "Autostrafer smoothing" ), vars::misc.autostrafe_smoothing, 0.f, 100.f, slider_style::flags::no_operators, _w( "%.f" ) ),
																																																																																																																											std::make_shared<checkbox>( _w( "Autostrafer avoid walls" ), vars::misc.avoid_walls ),
																																																																																																																											std::make_shared<checkbox>( _w( "Peek Assist" ), vars::misc.peek_assist )->add_control( std::make_shared<key_bind>( vars::misc.peek_assist, vars::key.peek_assist ) )->add_control( std::make_shared<color_picker>( vars::misc.peek_assist_col2, colorpicker_style::flags::no_alpha_bar ) )->add_control( std::make_shared<color_picker>( vars::misc.peek_assist_col, colorpicker_style::flags::no_alpha_bar ) ),
																																																																																																																											std::make_shared<checkbox>( _w( "Retreat on release" ), vars::misc.peek_assist_retreat_on_release ),
																																																																																																																									//std::make_shared<checkbox>( _w( "Circlestrafer" ), vars::misc.circlestrafe )->add_control( std::make_shared<key_bind>( vars::misc.circlestrafe, vars::key.circlestrafe ) ),
																																																																																																																											std::make_shared<checkbox>( _w( "Instant unduck" ), vars::misc.instant_unduck ),
																																																																																																																											std::make_shared<checkbox>( _w( "Slide" ), vars::aim.slide )->add_control( std::make_shared<key_bind>( vars::aim.slide, vars::key.slide ) ),
																																																																																																																											std::make_shared<checkbox>( _w( "Fake duck" ), vars::aim.fake_duck )->add_control( std::make_shared<key_bind>( vars::aim.fake_duck, vars::key.fake_duck ) ),
																																																																																																																											//std::make_shared<checkbox>( _w( "Compensate throwable" ), vars::misc.compensate_grenade ),
																																																																																																																								} ),
																																																																																																																									std::make_shared<child>( _w( "Buy bot" ), vec2( 518, 16 ), vec2( 235, 402 ), std::vector<std::shared_ptr<control>>
																																																																																																																										{
																																																																																																																											std::make_shared<combobox>( _w( "Primary" ), vars::misc.buybot.primary, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																																											{
																																																																																																																												{ _w( "None" ), 0 },
																																																																																																																												{ _w( "AK47 / M4A1" ), 1 },
																																																																																																																												{ _w( "SG556" ), 2 },
																																																																																																																												{ _w( "AWP" ), 3 },
																																																																																																																												{ _w( "SSG08" ), 4 },
																																																																																																																												{ _w( "G3SG1 / SCAR20" ), 5 },
																																																																																																																												{ _w( "Negev" ), 6 },
																																																																																																																											} ),
																																																																																																																												std::make_shared<combobox>( _w( "Secondary" ), vars::misc.buybot.pistol, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																																												{
																																																																																																																													{ _w( "None" ), 0 },
																																																																																																																													{ _w( "Deagle / R8" ), 1 },
																																																																																																																													{ _w( "Dual berettas" ), 2 },
																																																																																																																													{ _w( "TEC9" ), 3 },
																																																																																																																													{ _w( "P250" ), 4 },
																																																																																																																													{ _w( "Five Seven" ), 5 },
																																																																																																																												} ),
																																																																																																																													std::make_shared<combobox>( _w( "Armor" ), vars::misc.buybot.kevlar_helmet, combobox_style::flags::none, std::vector<combo_item_t>
																																																																																																																													{
																																																																																																																														{ _w( "None" ), 0 },
																																																																																																																														{ _w( "Kevlar" ), 1 },
																																																																																																																														{ _w( "Kevlar / Helmet" ), 2 },
																																																																																																																													} ),
																																																																																																																														std::make_shared<checkbox>( _w( "Defuser" ), vars::misc.buybot.defuser ),
																																																																																																																															std::make_shared<checkbox>( _w( "Taser" ), vars::misc.buybot.taser ),
																																																																																																																															std::make_shared<checkbox>( _w( "Grenades" ), vars::misc.buybot.grenades ),

																																																																																																																										} ),
																																																																																																																				} ),
																																																																																																																			} ),

																																																																																																																				std::make_shared<tab>( _w( "LUA" ), std::vector<std::shared_ptr<sub_tab>>
																																																																																																																														{ptr9 = std::make_shared<sub_tab>( _w( "" ), std::vector<std::shared_ptr<child>>
																																																																																																																														{
																																																																																																																															std::make_shared<child>( _w( "Tab A" ), vec2( 16, 16 ), vec2( 235, 518 ), std::vector<std::shared_ptr<control>>
																																																																																																																															{

																																																																																																																															} ),
																																																																																																																																std::make_shared<child>( _w( "Tab B" ), vec2( 267, 16 ), vec2( 235, 518 ), std::vector<std::shared_ptr<control>>
																																																																																																																															{

																																																																																																																															} ),

																																																																																																																														} ),
																																																																																																																														} ),
																																																																																																																															std::make_shared<tab>( _w( "SKINS" ), std::vector<std::shared_ptr<sub_tab>>
																																																																																																																															{std::make_shared<sub_tab>( _w( "" ), std::vector<std::shared_ptr<child>>
																																																																																																																															{
																																																																																																																																ptr11 = std::make_shared<child>( _w( "Weapons" ), vec2( 16, 16 ), vec2( 235, 300 ), std::vector<std::shared_ptr<control>>
																																																																																																																																{
																																																																																																																																	std::make_shared<checkbox>( _w( "Autoselect held weapon" ), vars::inventory.autoselect_weapon ),
																																																																																																																																		std::make_shared<pad>( -3 ),

																																																																																																																																} ),
																																																																																																																																	ptr8 = std::make_shared<child>( _w( "Paint kit" ), vec2( 267, 16 ), vec2( 235, 518 ), std::vector<std::shared_ptr<control>>
																																																																																																																																	{} ),
																																																																																																																																		ptr10 = std::make_shared<child>( _w( "Options" ), vec2( 16, 332 ), vec2( 235, 202 ), std::vector<std::shared_ptr<control>>
																																																																																																																																	{} ),
																																																																																																																																		std::make_shared<child>( _w( "Inventory" ), vec2( 518, 16 ), vec2( 235, 402 ), std::vector<std::shared_ptr<control>>
																																																																																																																																	{
																																																																																																																																		/*std::make_shared<pad>( -1 ),*/
																																																																																																																																		std::make_shared<listbox>( "", vars::menu.inventory_list_idx, &list_items, 19 ),
																																																																																																																																			std::make_shared<pad>( 2 ),
																																																																																																																																			std::make_shared<button>( _w( "Remove" ), inventory_rm ),
																																																																																																																																			std::make_shared<button>( _w( "Reload" ), inventory_reload ),
																																																																																																																																			std::make_shared<pad>( -7 ),
																																																																																																																																	} ),
																																																																																																																															} ),
																																																																																																																															} ),


	} );

	menu_window->config_child = std::make_shared<child>( _w( "Config" ), vec2( 518, 402 ), vec2( 235, 100 ), std::vector<std::shared_ptr<control>>
	{
		std::make_shared<combobox>( _w( "Slot" ), vars::menu.slot, combobox_style::flags::none, std::vector<combo_item_t>
		{
			{ _w( "Slot 1" ), 1 },
			{ _w( "Slot 2" ), 2 },
			{ _w( "Slot 3" ), 3 },
			{ _w( "Slot 4" ), 4 },
			{ _w( "Slot 5" ), 5 },
			{ _w( "Slot 6" ), 6 },
		} ),
			std::make_shared<pad>( 3 ),
				std::make_shared<button>( _w( "Load config" ), load_config ),
				std::make_shared<button>( _w( "Save config" ), save_config ),
				std::make_shared<pad>( -8 ),
	}, menu_window );

	ptr->add_child( std::make_shared<child>( _w( "Aimbot" ), vec2( 518, 16 ), vec2( 235, 370 ), std::vector<std::shared_ptr<control>>
	{std::make_shared<checkbox>( _w( "Aimbot" ), vars::aim.enabled )->add_control( std::make_shared<key_bind>( vars::aim.enabled, vars::key.aimbot ) ),
		std::make_shared<pad>( -7 )->add_dependency( { vars::aim.enabled, 1.0 } ),
		std::make_shared<slider>( _w( "Maximum fov" ), vars::aim.fov, 0.f, 180.f, slider_style::flags::none, _w( "%.f deg" ) )->add_dependency( { vars::aim.enabled, 1.0 } ),
		std::make_shared<checkbox>( _w( "Visualize aimbot" ), vars::aim.visualize_aimbot )->add_control( std::make_shared<color_picker>( vars::aim.visualize_color_safe ) )->add_control( std::make_shared<color_picker>( vars::aim.visualize_color ) ),
		std::make_shared<checkbox>( _w( "Anti-exploit" ), vars::aim.anti_exploit ),
		std::make_shared<checkbox>( _w( "Target dormant" ), vars::aim.dormant )->add_control( std::make_shared<key_bind>( vars::aim.dormant, vars::key.dormant ) ),
		std::make_shared<checkbox>( _w( "Autofire" ), vars::aim.autofire ),
		std::make_shared<checkbox>( _w( "Hide shot" ), vars::aim.silent )->add_control( std::make_shared<key_bind>( vars::aim.silent, vars::key.silent ) ),
		std::make_shared<checkbox>( _w( "Double tap" ), vars::aim.doubletap )->add_control( std::make_shared<key_bind>( vars::aim.doubletap, vars::key.doubletap ) ),
		//std::make_shared<checkbox>( _w( "Immediate recharge" ), vars::aim.immediate_recharge ),
		std::make_shared<checkbox>( _w( "Delay shot" ), vars::aim.extend_peek ),
		std::make_shared<checkbox>( _w( "Force extra safety" ), vars::aim.fallback_on_key )->add_control( std::make_shared<key_bind>( vars::aim.fallback_on_key, vars::key.fallback ) ),
		std::make_shared<checkbox>( _w( "Force bodyaim" ), vars::aim.baim_on_key )->add_control( std::make_shared<key_bind>( vars::aim.baim_on_key, vars::key.baim ) ),
		std::make_shared<checkbox>( _w( "Headshot only" ), vars::aim.hsmode )->add_control( std::make_shared<key_bind>( vars::aim.hsmode, vars::key.hsmode ) ),
		std::make_shared<checkbox>( _w( "Knifebot" ), vars::aim.knife_bot ),
		std::make_shared<checkbox>( _w( "Zeusbot" ), vars::aim.zeus_bot ),
		std::make_shared<slider>( _w( "Zeus hitchance" ), vars::aim.zeus_hc, 0.f, 100.f, slider_style::flags::none, _w( "%.f%%" ) )->add_dependency( { vars::aim.zeus_bot, 1.0 } ),
		std::make_shared<combobox>( _w( "Resolver mode" ), vars::aim.resolver_mode, combobox_style::flags::none, std::vector<combo_item_t>
	{
		{ _w( "Roll" ), 0 },
		{ _w( "Roll disabled" ), 1 },
	} ),
		std::make_shared<combobox>( _w( "Low performance mode" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
		{
			{ _w( "Limit targets" ), vars::aim.low_perf_targets },
			{ _w( "Limit scan accuracy" ), vars::aim.low_perf_scan_accuracy },
		} ),
			//std::make_shared<combobox>( _w( "Hitchance mode" ), vars::aim.hitchance_mode, combobox_style::flags::none, std::vector<combo_item_t>
			//{
			//	{ _w( "Normal" ), 0 },
			//	{ _w( "Hitgroup" ), 1 },
			//	{ _w( "Hitbox" ), 2 },
			//} ),
			//	std::make_shared<combobox>( _w( "Hitchance mode max accuracy" ), vars::aim.hitchance_mode_maxacc, combobox_style::flags::none, std::vector<combo_item_t>
			//	{
			//		{ _w( "Normal" ), 0 },
			//		{ _w( "Hitgroup" ), 1 },
			//		{ _w( "Hitbox" ), 2 },
			//	} ),
#ifndef RELEASE
				std::make_shared<button>( _w( "UNLOAD" ), init::unload_cheat ),
#endif
	} ) );


	const auto legit_child = std::make_shared<child>( _w( "General" ), vec2( 518, -24 ), vec2( 235, 402 ), std::vector<std::shared_ptr<control>>
	{std::make_shared<checkbox>( _w( "Enabled" ), vars::legit.enabled ),
	std::make_shared<combobox>( _w( "Disablers" ), nullptr, combobox_style::flags::multi_select, std::vector<combo_item_t>
	{
		{ _w( "Smoked" ), vars::legit.disable_smoke },
		{ _w( "Flashed" ), vars::legit.disable_flash },
	} ),
		std::make_shared<slider>( _w( "Backtrack" ), vars::legit.backtrack, 0.f, 400.f, slider_style::flags::none, _w( "%.f ms" ) ),
			std::make_shared<checkbox>( _w( "Quick stop" ), vars::legit.quick_stop ),
	} );

	ptr2->add_child( legit_child );
	ptr3->add_child( std::make_shared<child>( *legit_child ) );
	ptr4->add_child( std::make_shared<child>( *legit_child ) );
	ptr5->add_child( std::make_shared<child>( *legit_child ) );
	ptr6->add_child( std::make_shared<child>( *legit_child ) );
	ptr7->add_child( std::make_shared<child>( *legit_child ) );


	const auto lua_child = std::make_shared<child>( _w( "Lua" ), vec2( 518, 16 ), vec2( 235, 402 ), std::vector<std::shared_ptr<control>>
	{
		std::make_shared<listbox>( lua::api.selected_load, &lua::api.listed_scripts, 8 ),
			std::make_shared<button>( _w( "Load" ), lua_load_script ),
			std::make_shared<listbox>( lua::api.selected_unload, &lua::api.loaded_scripts, 7 ),
			std::make_shared<button>( _w( "Unload" ), lua_unload_script ),
			std::make_shared<button>( _w( "Reload" ), lua_reload_script ),
			std::make_shared<checkbox>( _w( "Allow unsafe scripts" ), vars::lua.allow_insecure ),
			std::make_shared<checkbox>( _w( "Load with config" ), nullptr ),
	} );

	auto listbox_ptr = std::make_shared<listbox>( "", vars::inventory.paint_kit, &menu_items, 27, listbox_style::flags::search_bar );
	auto weapon_listbox_ptr = std::make_shared<listbox>( vars::inventory.definition_index, &weapon_menu_items, 14, listbox_style::flags::search_bar );

	inventorychanger::paint_kit_listbox = listbox_ptr;
	inventorychanger::weapons_listbox = weapon_listbox_ptr,

		ptr11->add_control( weapon_listbox_ptr );
	ptr8->add_control( std::make_shared<checkbox>( _w( "Show all skins" ), vars::inventory.show_all_skins ) );
	ptr8->add_control( std::make_shared<pad>( -3 ) );
	ptr8->add_control( listbox_ptr );
	ptr8->add_control( std::make_shared<pad>( -5 ) );
	ptr10->add_control( std::make_shared<slider>( _w( "Seed" ), vars::inventory.seed, 0.f, 1000.f, slider_style::flags::none, _w( "%.f" ) ) );
	ptr10->add_control( std::make_shared<slider>( _w( "Wear" ), vars::inventory.wear, 0.f, 100.f, slider_style::flags::none, _w( "%.f %%" ) ) );
	ptr10->add_control( std::make_shared<slider>( _w( "Stattrak" ), vars::inventory.stat_trak, -1.f, 10000.f, slider_style::flags::none, _w( "%.f kills" ) ) );
	ptr10->add_control( std::make_shared<pad>( 2 ) );
	ptr10->add_control( std::make_shared<checkbox>( _w( "Equip CT" ), vars::inventory.enabled_ct ) );
	ptr10->add_control( std::make_shared<checkbox>( _w( "Equip T" ), vars::inventory.enabled_t ) );
	ptr10->add_control( std::make_shared<button>( _w( "Add" ), inventory_add ) );
	ptr10->add_control( std::make_shared<button>( _w( "Save" ), inventory_save ) );
	ptr10->add_control( std::make_shared<pad>( -8 ) );


	ptr9->add_child( lua_child );

	m_windows.push_back( menu_window );

	m_init = true;

	run_input();

	m_open = true;
}

void menu::run()
{
	/*
	* Add all items
	*/
	init();

	/*
	* Handle menu alpha when opening/closing
	*/
	run_alpha();

	/*
	 * Render background (darkening of back)
	 */

	//render_background();

	/*
	* Dont render the menu if it's not visible
	*/
	if ( m_alpha <= 0.f )
		return;

	lua::api.refresh_scripts();
//
//	static auto particles = new particle_drawer( adapter.get()->display.x, adapter.get()->display.y, 400, 150.f );
//
//#ifndef _DEBUG
//	particles->draw( 200.f * m_alpha );
//#endif

/*
 * Reset mouse mode, it may be modified by controls
 */
	set_mouse_mode( window_style::mouse_mode::regular );

	/*
	* Go through every registered window and render it
	*/
	for ( const auto& window : m_windows )
		window->run_render();

	if ( g_menu->get_focussed_control() && !m_focused_control_pos.is_zero() )
	{
		auto& d = draw.get_layer( layer::foreground );

		d.push_clip( m_focused_scissor_record );
		g_menu->get_focussed_control()->run_render( m_focused_control_pos );
		d.pop_clip();
	}

//	particles->draw_foreground( 250.f * m_alpha );

	/*
	* Render mouse at last so it's over everything else
	*/
	render_mouse();
}

void menu::run_input()
{
	/*
	* Go through every registered window and handle
	* the input for it
	*/
	for ( const auto& window : m_windows )
		window->run_input();
}

void menu::run_hotkey_input()
{
	/*
	* Go through every registered window and handle
	* the input for it
	*/
	for ( const auto& window : m_windows )
		window->run_hotkey_input();
}

float BezierBlend( float t )
{
	return t * t * ( 3.0f - 2.0f * t );
}

void menu::run_alpha()
{
	//VAL_SMOOTHER( m_alpha, 0.f, 1.f, m_open, 1.f / 0.3f );
	static auto time = 0.f;

	if ( m_open )
		time += interfaces::globals()->frametime * 2.3f;
	else
		time -= interfaces::globals()->frametime * 2.3f;

	time = clamp( time, 0.f, 1.f );

	m_alpha = BezierBlend( time );

	m_alpha = clamp( m_alpha, 0.f, 1.f );

}

void menu::toggle()
{
	/*
	 * Null the focussed control so controls
	 * that require mouse position don't get
	 * messed up
	 */
	for ( auto& window : m_windows )
		window->reset_state();

	m_open = !m_open;
}

bool menu::is_open() const
{
	return m_open;
}

float menu::get_alpha() const
{
	return m_alpha;
}

color menu::get_main_color() const
{
	return m_main_color;
}

vec2 menu::get_style( const e_menu_style style ) const
{
	switch ( style )
	{
		case e_menu_style::window_padding:
			return { 16, 16 };
		case e_menu_style::tab_area_size:
			return { 0, 54 };
		case e_menu_style::control_inner_padding:
			return { 5, 2 };
		case e_menu_style::control_padding:
			return { 6, 4 };
		case e_menu_style::child_padding:
			return { 10, 10 };
		case e_menu_style::tab_padding:
			return { 14, 8 };
		case e_menu_style::sub_control_padding:
			return { 4, 4 };
		case e_menu_style::popup_padding:
			return { 6, 2 };
		default:
			break;
	}

	return { 0, 0 };
}

control* menu::get_focussed_control() const
{
	return m_focus_control;
}

void menu::set_focussed_control( control* control )
{
	m_focus_control = control;
}

void menu::reset_focussed_control()
{
	m_focus_control = nullptr;
}

void menu::set_mouse_mode( window_style::mouse_mode mouse_mode )
{
	m_mouse_mode = mouse_mode;
}

void menu::set_keyboard_control( control* access )
{
	m_keyboard_control = access;
}

control* menu::get_keyboard_control()
{
	return m_keyboard_control;
}

void menu::set_mouse_box_color( color col )
{
	m_color_box_mouse = col;
}

void menu::render_mouse() const
{
	const auto info = g_input->get_mouse();

	auto col = color( 25, 25, 25 );

	auto& d = draw.get_layer( layer::foreground );

	if ( m_mouse_mode > window_style::mouse_mode::regular )
	{
			/*
			* Inner
			*/
		d.add_rect_filled( rect_legacy( { info.pos.x + 8, info.pos.y - 25 }, { 40, 25 } ), m_color_box_mouse );

		/*
		* Outline
		*/
		d.add_rect( rect_legacy( { info.pos.x + 8, info.pos.y - 25 }, { 40, 25 } ), gui_color::palette::border() );

		if ( m_mouse_mode == window_style::mouse_mode::color_hint )
		{
			/*
			* Draw hint that you can pick colors outside of box
			*/
			_( hint, "GRAB COLOR WITH RIGHT CLICK" );
			d.add_text( draw.get_font( FNV1A( "title_font" ) ), { info.pos.x + 8, info.pos.y - 35 }, hint, gui_color::text( 180 ), 0, layer::align_center );
		}

		return;
	}

	d.add_rect_filled( rect_legacy( { info.pos.x + 1, info.pos.y }, { 1, 17 } ), col );

	for ( auto i = 0; i < 11; i++ )
	{
		d.add_rect_filled( rect_legacy( { info.pos.x + 2 + i, info.pos.y + 1 + i }, { 1, 1 } ), col );
	}

	d.add_rect_filled( rect_legacy( { info.pos.x + 8, info.pos.y + 12 }, { 5, 1 } ), col );
	d.add_rect_filled( rect_legacy( { info.pos.x + 8, info.pos.y + 13 }, { 1, 1 } ), col );
	d.add_rect_filled( rect_legacy( { info.pos.x + 9, info.pos.y + 14 }, { 1, 2 } ), col );
	d.add_rect_filled( rect_legacy( { info.pos.x + 10, info.pos.y + 16 }, { 1, 2 } ), col );
	d.add_rect_filled( rect_legacy( { info.pos.x + 8, info.pos.y + 18 }, { 2, 1 } ), col );
	d.add_rect_filled( rect_legacy( { info.pos.x + 7, info.pos.y + 16 }, { 1, 2 } ), col );
	d.add_rect_filled( rect_legacy( { info.pos.x + 6, info.pos.y + 14 }, { 1, 2 } ), col );
	d.add_rect_filled( rect_legacy( { info.pos.x + 5, info.pos.y + 13 }, { 1, 1 } ), col );
	d.add_rect_filled( rect_legacy( { info.pos.x + 4, info.pos.y + 14 }, { 1, 1 } ), col );
	d.add_rect_filled( rect_legacy( { info.pos.x + 3, info.pos.y + 15 }, { 1, 1 } ), col );
	d.add_rect_filled( rect_legacy( { info.pos.x + 2, info.pos.y + 16 }, { 1, 1 } ), col );

	col = gui_color::palette::main_pink();

	for ( auto i = 0; i < 4; i++ )
	{
		d.add_rect_filled( rect_legacy( { info.pos.x + 2 + i, info.pos.y + 2 + i }, { 1, static_cast< float >( 14 - ( i * 2 ) ) } ), col );
	}

	d.add_rect_filled( rect_legacy( { info.pos.x + 6, info.pos.y + 6 }, { 1, 8 } ), col );
	d.add_rect_filled( rect_legacy( { info.pos.x + 7, info.pos.y + 7 }, { 1, 9 } ), col );

	for ( auto i = 0; i < 4; i++ )
	{
		d.add_rect_filled( rect_legacy( { info.pos.x + 8 + i, static_cast< float >( info.pos.y + 8 + i ) }, { 1, static_cast< float >( 4 - i ) } ), col );
	}

	d.add_rect_filled( rect_legacy( { info.pos.x + 8, info.pos.y + 14 }, { 1, 4 } ), col );
	d.add_rect_filled( rect_legacy( { info.pos.x + 9, info.pos.y + 16 }, { 1, 2 } ), col );

	//else if ( m_mouse_mode == window_style::mouse_mode::pipette )
	//{
	//	/*
	//	 * Lower Area 
	//	 */
	//	render::rect_filled( { info.pos.x + 1, info.pos.y }, { 1, 1 }, color );
	//	render::rect_filled( { info.pos.x + 1, info.pos.y - 1 }, { 1, 1 }, color ); // https://i.imgur.com/B5J2wDt.png
	//	render::rect_filled( { info.pos.x + 2, info.pos.y }, { 1, 1 }, color );

	//	render::rect_filled( { info.pos.x + 2, info.pos.y - 1 }, { 1, 1 }, color );
	//	render::rect_filled( { info.pos.x + 2, info.pos.y - 2 }, { 1, 1 }, color );
	//	render::rect_filled( { info.pos.x + 2, info.pos.y - 3 }, { 1, 1 }, color ); // https://i.imgur.com/qst9puu.png
	//	render::rect_filled( { info.pos.x + 3, info.pos.y - 1 }, { 1, 1 }, color );
	//	render::rect_filled( { info.pos.x + 4, info.pos.y - 1 }, { 1, 1 }, color );

	//	/*
	//	 * Middle Area
	//	 */
	//	render::rect_filled( { info.pos.x + 4, info.pos.y - 3 }, { 1, 1 }, color );

	//	/*
	//	 * Left Side
	//	 */		
	//	render::rect_filled( { info.pos.x + 4, info.pos.y - 4 }, { 1, 1 }, color );
	//	render::rect_filled( { info.pos.x + 3, info.pos.y - 4 }, { 1, 1 }, color ); // https://i.imgur.com/cmKrJ68.png
	//	render::rect_filled( { info.pos.x + 4, info.pos.y - 5 }, { 1, 1 }, color );
	//	render::rect_filled( { info.pos.x + 4, info.pos.y - 6 }, { 1, 1 }, color );

	//	for( auto i = 0; i < 8; i++ )
	//		render::rect_filled( { info.pos.x + 5 + i, info.pos.y - 7 - i }, { 1, 1 }, color ); // https://i.imgur.com/D7sOaOm.png

	//	/*
	//	 * Right Side
	//	 */
	//	render::rect_filled( { info.pos.x + 5, info.pos.y - 3 }, { 1, 1 }, color );
	//	render::rect_filled( { info.pos.x + 5, info.pos.y - 2 }, { 1, 1 }, color ); // https://i.imgur.com/AjLlS0P.png
	//	render::rect_filled( { info.pos.x + 6, info.pos.y - 3 }, { 1, 1 }, color );
	//	render::rect_filled( { info.pos.x + 7, info.pos.y - 3 }, { 1, 1 }, color );

	//	for ( auto i = 0; i < 8; i++ )
	//		render::rect_filled( { info.pos.x + 8 + i, info.pos.y - 4 - i }, { 1, 1 }, color ); // https://i.imgur.com/D7sOaOm.png
	//}
}

void menu::render_background() const
{
	draw.get_layer( layer::foreground ).add_rect_filled( rect_legacy( { 0, 0 }, { adapter.get()->display.x, adapter.get()->display.y } ), Color( 5, 5, 5, 150 * m_alpha ).direct() );
	//render::rect_filled_diamond_gradient( { 0, 0 }, { static_cast< float >( render::get_dimensions().Width ), static_cast< float >( render::get_dimensions().Height ) }, gui_color::palette::main_pink( 0 ), gui_color::palette::main_pink( 200 ) );
}


void menu::set_focused_control_pos( const vec2& pos )
{
	m_focused_control_pos = pos;
}
