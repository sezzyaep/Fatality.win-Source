
#include "../include_cheat.h"

namespace lua::gui_helpers
{
	std::mutex cache_mutex{};
	std::unordered_map<uint32_t, std::weak_ptr<control>> cached_items{};

	template <typename T = control>
	inline std::shared_ptr<T> find_control( runtime_state& s, const std::string& path )
	{
		cache_mutex.lock();
		const auto hash = fnv1a_rt_tol( path );
		if ( cached_items.contains( hash ) )
		{
			if ( !cached_items[ hash ].expired() )
			{
				cache_mutex.unlock();
				return std::static_pointer_cast< T >( cached_items[ hash ].lock() );
			}

			cached_items.erase( hash );
		}

		cache_mutex.unlock();

		const auto split_path = util::split( path, ">" );
		if ( split_path.size() < 3 )
		{
			g_menu->draw_mutex.unlock();
			s.error( util::format( XOR_STR( "container was not found: %s" ), path.c_str() ) );
			return {};
		}

		auto position = 0;

		for ( auto& tab : g_menu->get_tabs() )
		{
			_u( name_s, tab->get_name() );
			if ( !util::compare_nocase( split_path[ position ], name_s ) )
				continue;

			position++;

			for ( auto& subtab : tab->get_subtabs() )
			{
				_u( name_s, subtab->get_name() );

				// LEGIT
				switch ( fnv1a_rt( name_s ) )
				{
					case FNV1A( "usp_silencer" ):
						name_s = xorstr_( "pistol" );
						break;
					case FNV1A( "m4a1_silencer" ):
						name_s = xorstr_( "rifle" );
						break;
					case FNV1A( "awp" ):
						name_s = xorstr_( "sniper" );
						break;
					case FNV1A( "mp5sd" ):
						name_s = xorstr_( "smg" );
						break;
					case FNV1A( "mag7" ):
						name_s = xorstr_( "shotgun" );
						break;
					case FNV1A( "m249" ):
						name_s = xorstr_( "lmg" );
						break;
					default:
						break;
				}

				if ( tab->has_sub_tabs() && !util::compare_nocase( split_path[ position ], name_s ) )
					continue;

				if ( tab->has_sub_tabs() )
					position++;

				std::shared_ptr<sub_tab> target = subtab;
				if ( !subtab->get_subtabs().empty() )
				{
					if ( split_path.size() == position + 3 )
					{
						for ( auto& subsubtab : subtab->get_subtabs() )
						{
							_u( name_s, subsubtab->get_name() );

							// RAGE
							switch ( fnv1a_rt( name_s ) )
							{
								case FNV1A( "scar20" ):
									name_s = xorstr_( "autosniper" );
									break;
								case FNV1A( "revolver" ):
									name_s = xorstr_( "heavy_pistol" );
									break;
								case FNV1A( "usp_silencer" ):
									name_s = xorstr_( "pistol" );
									break;
								case FNV1A( "ak47" ):
									name_s = xorstr_( "other" );
									break;
								default:
									break;
							}

							if ( !util::compare_nocase( split_path[ position ], name_s ) )
								continue;

							position++;

							target = subsubtab;
						}
					}
				}

				if ( split_path.size() != position + 2 )
				{
					g_menu->draw_mutex.unlock();
					s.error( util::format( XOR_STR( "container was not found: %s" ), path.c_str() ) );
					return {};
				}

				for ( auto& child : target->get_children() )
				{
					_u( name_s, child->get_name() );
					if ( !util::compare_nocase( split_path[ position ], name_s ) )
						continue;

					position++;

					for ( auto& curr : child->get_controls() )
					{
						_u( name2, curr->get_name() );

						if ( util::compare_nocase( split_path[ position ], name2 ) )
						{
							cache_mutex.lock();
							cached_items[ hash ] = curr;
							cache_mutex.unlock();
							return std::static_pointer_cast< T >( cached_items[ hash ].lock() );
						}

					}

					g_menu->draw_mutex.unlock();
					s.error( util::format( XOR_STR( "control was not found: %s" ), split_path[ position ].c_str() ) );
					return {};
				}
			}
		}

		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "container was not found: %s" ), path.c_str() ) );
		return {};
	}

	inline bool add_item( runtime_state& s, const std::shared_ptr<control>& control, const std::string& path )
	{
		const auto split_path = util::split( path, ">" );
		if ( split_path.size() < 2 )
		{
			g_menu->draw_mutex.unlock();
			s.error( util::format( XOR_STR( "container was not found: %s" ), path.c_str() ) );
			return false;
		}

		auto position = 0;
		for ( auto& tab : g_menu->get_tabs() )
		{
			_u( name_s, tab->get_name() );
			if ( !util::compare_nocase( split_path[ position ], name_s ) )
				continue;

			position++;

			for ( auto& subtab : tab->get_subtabs() )
			{
				_u( name_s, subtab->get_name() );

				// LEGIT
				switch ( fnv1a_rt( name_s ) )
				{
					case FNV1A( "usp_silencer" ):
						name_s = xorstr_( "pistol" );
						break;
					case FNV1A( "m4a1_silencer" ):
						name_s = xorstr_( "rifle" );
						break;
					case FNV1A( "awp" ):
						name_s = xorstr_( "sniper" );
						break;
					case FNV1A( "mp5sd" ):
						name_s = xorstr_( "smg" );
						break;
					case FNV1A( "mag7" ):
						name_s = xorstr_( "shotgun" );
						break;
					case FNV1A( "m249" ):
						name_s = xorstr_( "lmg" );
						break;
					default:
						break;
				}

				if ( tab->has_sub_tabs() && !util::compare_nocase( split_path[ position ], name_s ) )
					continue;

				if ( tab->has_sub_tabs() )
					position++;

				std::shared_ptr<sub_tab> target = subtab;
				if ( !subtab->get_subtabs().empty() )
				{
					if ( split_path.size() == position + 2 )
					{
						for ( auto& subsubtab : subtab->get_subtabs() )
						{
							_u( name_s, subsubtab->get_name() );

							// RAGE
							switch ( fnv1a_rt( name_s ) )
							{
								case FNV1A( "scar20" ):
									name_s = xorstr_( "autosniper" );
									break;
								case FNV1A( "revolver" ):
									name_s = xorstr_( "heavy_pistol" );
									break;
								case FNV1A( "usp_silencer" ):
									name_s = xorstr_( "pistol" );
									break;
								case FNV1A( "ak47" ):
									name_s = xorstr_( "other" );
									break;
								default:
									break;
							}

							if ( !util::compare_nocase( split_path[ position ], name_s ) )
								continue;

							position++;

							target = subsubtab;
						}
					}
				}

				if ( split_path.size() != position + 1 )
				{
					g_menu->draw_mutex.unlock();
					s.error( util::format( XOR_STR( "container was not found: %s" ), path.c_str() ) );
					return false;
				}

				for ( auto& child : target->get_children() )
				{
					_u( name_s, child->get_name() );
					if ( !util::compare_nocase( split_path[ position ], name_s ) )
						continue;

					for ( auto& curr : child->get_controls() )
					{
						_u( name1, control->get_name() );
						_u( name2, curr->get_name() );

						if ( util::compare_nocase( name1, name2 ) )
						{
							g_menu->draw_mutex.unlock();
							s.error( util::format( XOR_STR( "control id is already in use: %s" ), name1.c_str() ) );
							return false;
						}
					}

					child->add_control( control );
					return true;
				}
			}
		}

		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "container was not found: %s" ), path.c_str() ) );
		return false;
	}
}

int lua::api_def::gui::add_checkbox( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::string },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: add_checkbox(name, path): cfg.value" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	const auto hash = fnv1a_rt_tol( std::string( s.get_string( 1 ) ) + s.get_string( 2 ) );
	auto item = config_init::add( hash );
	const auto new_control = std::make_shared<checkbox>( _w2( std::string( s.get_string( 1 ) ) ), item );

	if ( !lua::gui_helpers::add_item( s, new_control, s.get_string( 2 ) ) )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "could not add checkbox: " ), s.get_string( 1 ) ) );
		return 0;
	}

	// register in the script
	me->add_gui_element( new_control );

	g_menu->draw_mutex.unlock();

	s.create_user_object<decltype( item )>( XOR_STR( "cfg.value" ), &item );
	return 1;
}

int lua::api_def::gui::add_slider( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::string },
		{ ltd::number },
		{ ltd::number },
		{ ltd::number },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: add_slider(name, path, min, max, step): cfg.value" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	const auto hash = fnv1a_rt_tol( std::string( s.get_string( 1 ) ) + s.get_string( 5 ) );
	auto item = config_init::add( hash );
	const auto new_control = std::make_shared<slider>( _w2( std::string( s.get_string( 1 ) ) ), item, static_cast< float >( s.get_integer( 3 ) ), static_cast< float >( s.get_integer( 4 ) ), slider_style::flags::none, _w( "%.1f" ), static_cast< float >( s.get_integer( 5 ) ) );

	if ( !lua::gui_helpers::add_item( s, new_control, s.get_string( 2 ) ) )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "could not add slider: " ), s.get_string( 1 ) ) );
		return 0;
	}

	// register in the script
	me->add_gui_element( new_control );

	g_menu->draw_mutex.unlock();

	s.create_user_object<decltype( item )>( XOR_STR( "cfg.value" ), &item );
	return 1;
}

int lua::api_def::gui::add_combo( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::string },
		{ ltd::table }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: add_combo(name, path, {item, ...}): cfg.value" ) );
		return 0;
	}

	const auto len = s.get_len( 3 );
	if ( len < 1 )
	{
		s.error( XOR_STR( "add_combo requires at least one item" ) );
		return 0;
	}

		// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	std::vector<combo_item_t> items{};
	auto count = 0;
	for ( const auto raw_items = s.get_array_string( 3 ); auto & cur : raw_items )
		items.emplace_back( combo_item_t{ _w2( cur ), count++ } );

	const auto hash = fnv1a_rt_tol( std::string( s.get_string( 1 ) ) + s.get_string( 2 ) );
	auto item = config_init::add( hash );
	const auto new_control = std::make_shared<combobox>( _w2( std::string( s.get_string( 1 ) ) ), item, combobox_style::flags::none, items );

	if ( !lua::gui_helpers::add_item( s, new_control, s.get_string( 2 ) ) )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "could not add combo: " ), s.get_string( 1 ) ) );
		return 0;
	}

	// register in the script
	me->add_gui_element( new_control );

	g_menu->draw_mutex.unlock();

	s.create_user_object<decltype( item )>( XOR_STR( "cfg.value" ), &item );
	return 1;
}

int lua::api_def::gui::add_multi_combo( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::string },
		{ ltd::table }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: add_multi_combo(name, path, {item, ...}): cfg.value..." ) );
		return 0;
	}


	const auto len = s.get_len( 3 );
	if ( len < 1 )
	{
		s.error( XOR_STR( "add_multi_combo requires at least one item" ) );
		return 0;
	}

		// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	std::vector<combo_item_t> items{};
	for ( const auto raw_items = s.get_array_string( 3 ); auto & cur : raw_items )
	{
		const auto hash = fnv1a_rt_tol( std::string( s.get_string( 1 ) ) + s.get_string( 2 ) + cur );
		items.emplace_back( combo_item_t{ _w2( cur ), config_init::add( hash ) } );
	}

	const auto new_control = std::make_shared<combobox>( _w2( std::string( s.get_string( 1 ) ) ), nullptr, combobox_style::flags::multi_select, items );

	if ( !lua::gui_helpers::add_item( s, new_control, s.get_string( 2 ) ) )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "could not add multi_combo: " ), s.get_string( 1 ) ) );
		return 0;
	}

	// register in the script
	me->add_gui_element( new_control );

	g_menu->draw_mutex.unlock();

	auto amt = 0;
	for ( auto& cur : items )
	{
		s.create_user_object<decltype( cur.m_value )>( XOR_STR( "cfg.value" ), &cur.m_value );
		amt++;
	}
	return amt;
}

int lua::api_def::gui::add_button( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::string },
		{ ltd::function },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: add_button(name, path, function)" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	// save up function in the registry
	const auto ref = s.registry_add();

	const auto new_control = std::make_shared<button>( _w2( std::string( s.get_string( 1 ) ) ), [ref, l, me] ()
	{
		runtime_state s( l );
		s.registry_get( ref );

		if ( !s.is_nil( -1 ) )
		{
			if ( !s.call( 0, 0 ) )
			{
				me->did_error = true;
				lua::helpers::error( XOR_STR( "runtime_error" ), s.get_last_error_description().c_str() );
				if ( const auto f = api.find_script_file( me->id ); f.has_value() )
					f->get().should_unload = true;

				return;
			}
		}
		else
			s.pop( 1 );
	} );

	if ( !lua::gui_helpers::add_item( s, new_control, s.get_string( 2 ) ) )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "could not add button: " ), s.get_string( 1 ) ) );
		return 0;
	}

	// register in the script
	me->add_gui_element( new_control );

	g_menu->draw_mutex.unlock();
	return 0;
}

int lua::api_def::gui::add_textbox( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::string },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: add_textbox(name, path): cfg.value" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	const auto hash = fnv1a_rt_tol( std::string( s.get_string( 1 ) ) + s.get_string( 2 ) );
	auto item = config_init::add( hash );
	const auto new_control = std::make_shared<text_box>( _w2( std::string( s.get_string( 1 ) ) ), item );

	if ( !lua::gui_helpers::add_item( s, new_control, s.get_string( 2 ) ) )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "could not add textbox: " ), s.get_string( 1 ) ) );
		return 0;
	}

	// register in the script
	me->add_gui_element( new_control );

	g_menu->draw_mutex.unlock();

	s.create_user_object<decltype( item )>( XOR_STR( "cfg.value" ), &item );
	return 1;
}

int lua::api_def::gui::add_listbox( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::string },
		{ ltd::number },
		{ ltd::boolean },
		{ ltd::table },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: add_listbox(name, path, items_to_show, search_bar, {item, ...}): cfg.value" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	const auto len = s.get_len( 5 );
	if ( len < 1 )
	{
		s.error( XOR_STR( "listbox requires at least one item" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	std::vector<std::string> items{};
	for ( const auto raw_items = s.get_array_string( 5 ); auto & cur : raw_items )
		items.emplace_back( _w2( cur ) );

	const auto hash = fnv1a_rt_tol( std::string( s.get_string( 1 ) ) + s.get_string( 2 ) );
	auto item = config_init::add( hash );
	const auto new_control = std::make_shared<listbox>( _w2( std::string( s.get_string( 1 ) ) ), item, items, s.get_integer( 3 ), s.get_boolean( 4 ) ? listbox_style::flags::search_bar : listbox_style::flags::none );

	if ( !lua::gui_helpers::add_item( s, new_control, s.get_string( 2 ) ) )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "could not add listbox: " ), s.get_string( 1 ) ) );
		return 0;
	}

	// register in the script
	me->add_gui_element( new_control );

	g_menu->draw_mutex.unlock();

	s.create_user_object<decltype( item )>( XOR_STR( "cfg.value" ), &item );
	return 1;
}

int lua::api_def::gui::add_keybind( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: add_keybind(path)" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	const auto item = lua::gui_helpers::find_control<control>( s, s.get_string( 1 ) );
	if ( !item )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "control was not found: " ), s.get_string( 1 ) ) );
		return 0;
	}

	if ( !item->can_add_subcontrols() )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "this control type cannot have subcontrols: " ), s.get_string( 1 ) ) );
		return 0;
	}

	if ( item->is_combo() )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "this control type cannot have keybinds: " ), s.get_string( 1 ) ) );
		return 0;
	}

	if ( std::ranges::find( me->get_gui_elements(), item ) == me->get_gui_elements().end() )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "you cannot add sub controls unowned controls" ) ) );
		return 0;
	}

	if ( item->get_controls().size() >= 3 )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "controls cannot have more than three sub controls: " ), s.get_string( 1 ) ) );
		return 0;
	}

	for ( const auto& cur : item->get_controls() )
	{
		if ( cur->is_keybind() )
		{
			g_menu->draw_mutex.unlock();
			s.error( util::format( XOR_STR( "this control already has a keybind: " ), s.get_string( 1 ) ) );
			return 0;
		}
	}

	_u( name, item->get_name() );

	const auto hash = fnv1a_rt_tol( std::string( s.get_string( 1 ) ) + name + xorstr_( "_key" ) );
	auto key_item = config_init::add( hash, nullptr, 0.0, config_init::add( hash + FNV1A( ".key" ) ), config_init::add( hash + FNV1A( ".key_temp" ) ) );
	const auto new_control = std::make_shared<key_bind>( item->get_var(), key_item );

	new_control->set_parent( item );
	item->add_control( new_control );

	g_menu->draw_mutex.unlock();
	return 0;
}

int lua::api_def::gui::add_colorpicker( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::boolean },
		{ ltd::table, true },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: add_colorpicker(path, alpha_bar, default_col = white): cfg.value" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	const auto item = lua::gui_helpers::find_control<control>( s, s.get_string( 1 ) );
	if ( !item )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "control was not found: " ), s.get_string( 1 ) ) );
		return 0;
	}

	if ( !item->can_add_subcontrols() )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "this control type cannot have subcontrols: " ), s.get_string( 1 ) ) );
		return 0;
	}

	if ( std::ranges::find( me->get_gui_elements(), item ) == me->get_gui_elements().end() )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "you cannot add sub controls unowned controls" ) ) );
		return 0;
	}

	if ( item->get_controls().size() >= 3 )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "controls cannot have more than three sub controls: " ), s.get_string( 1 ) ) );
		return 0;
	}

	_u( name, item->get_name() );
	const auto hash = fnv1a_rt_tol( std::string( s.get_string( 1 ) ) + name + xorstr_( "_col_" ) + std::to_string( item->get_controls().size() ) );

	auto default_col = color::white();
	if ( s.get_stack_top() == 3 )
		default_col = extract_color( s, 3 );
	const auto exists = config_init::exists( hash );

	auto color_item = config_init::add( hash );
	if ( !exists )
		color_item->set_color( default_col );
	if ( !s.get_boolean( 2 ) )
		color_item->set_color( color( color_item->get_color(), 255.f ) );

	const auto new_control = std::make_shared<color_picker>( color_item, !s.get_boolean( 2 ) ? colorpicker_style::flags::no_alpha_bar : colorpicker_style::flags::none );

	new_control->set_parent( item );
	item->add_control( new_control );

	g_menu->draw_mutex.unlock();

	s.create_user_object<decltype( color_item )>( XOR_STR( "cfg.value" ), &color_item );
	return 1;
}

int lua::api_def::gui::get_keybind( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: get_keybind(path): number : number" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	const auto item = lua::gui_helpers::find_control<control>( s, s.get_string( 1 ) );
	if ( !item )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "control was not found: " ), s.get_string( 1 ) ) );
		return 0;
	}

	if ( !item->can_add_subcontrols() || item->is_combo() )
	{
		s.error( util::format( XOR_STR( "this control does not hold a keybind: " ), s.get_string( 1 ) ) );
		return 0;
	}

	for ( const auto& cur : item->get_controls() )
	{
		if ( cur->is_keybind() )
		{
			s.push( std::static_pointer_cast< key_bind >( cur )->get_key() );
			s.push( std::static_pointer_cast< key_bind >( cur )->get_mode() );
			return 2;
		}
	}

	s.error( util::format( XOR_STR( "this control does not hold a keybind: " ), s.get_string( 1 ) ) );
	return 0;
}

int lua::api_def::gui::get_listbox_items( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: get_listbox_items(path): {item, ...}" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	const auto item = lua::gui_helpers::find_control<listbox>( s, s.get_string( 1 ) );
	if ( !item )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "control was not found: " ), s.get_string( 1 ) ) );
		return 0;
	}

	if ( !item->is_listbox() )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "control is not a listbox: " ), s.get_string( 1 ) ) );
		return 0;
	}

	s.create_table();
	{
		auto index = 1;
		for ( auto& cur : item->get_items() )
		{
			_u( str, cur );
			s.set_i( index++, str.c_str() );
		}
	}

	g_menu->draw_mutex.unlock();

	return 1;
}

int lua::api_def::gui::set_listbox_items( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::table },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: set_listbox_items(path, {item, ...})" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	const auto item = lua::gui_helpers::find_control<listbox>( s, s.get_string( 1 ) );
	if ( !item )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "control was not found: " ), s.get_string( 1 ) ) );
		return 0;
	}

	if ( !item->is_listbox() )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "control is not a listbox: " ), s.get_string( 1 ) ) );
		return 0;
	}

	if ( std::ranges::find( me->get_gui_elements(), item ) == me->get_gui_elements().end() )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "you cannot set items of unowned controls" ) ) );
		return 0;
	}

	std::vector<std::string> items{};
	for ( const auto raw_items = s.get_array_string( 2 ); auto & cur : raw_items )
		items.emplace_back( _w2( cur ) );

	item->set_items( items );

	g_menu->draw_mutex.unlock();

	return 0;
}

int lua::api_def::gui::get_combo_items( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: get_combo_items(path): {item, ...}" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	const auto item = lua::gui_helpers::find_control<combobox>( s, s.get_string( 1 ) );
	if ( !item )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "control was not found: " ), s.get_string( 1 ) ) );
		return 0;
	}

	if ( !item->is_combo() )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "control is not a combo: " ), s.get_string( 1 ) ) );
		return 0;
	}

	s.create_table();
	{
		auto index = 1;
		for ( auto& cur : item->get_items() )
		{
			_u( str, cur.m_name );
			s.set_i( index++, str.c_str() );
		}
	}

	g_menu->draw_mutex.unlock();

	return 1;
}

int lua::api_def::gui::set_combo_items( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::table },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: set_combo_items(path, {item, ...})" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	const auto item = lua::gui_helpers::find_control<combobox>( s, s.get_string( 1 ) );
	if ( !item )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "control was not found: " ), s.get_string( 1 ) ) );
		return 0;
	}

	if ( !item->is_combo() || !item->get_var() )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "control is not a combo: " ), s.get_string( 1 ) ) );
		return 0;
	}

	if ( std::ranges::find( me->get_gui_elements(), item ) == me->get_gui_elements().end() )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "you cannot set items of unowned controls" ) ) );
		return 0;
	}

	std::vector<combo_item_t> items{};
	auto count = 0;
	for ( const auto raw_items = s.get_array_string( 2 ); auto & cur : raw_items )
		items.emplace_back( combo_item_t{ _w2( cur ), count++ } );

	item->set_items( items );

	g_menu->draw_mutex.unlock();
	return 0;
}

int lua::api_def::gui::get_config_item( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: get_config_item(path): cfg.value" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	const auto item = lua::gui_helpers::find_control( s, s.get_string( 1 ) );
	if ( !item )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "control was not found" ) ) );
		return 0;
	}

	if ( item->is_combo() && !item->get_var() )
	{
		const auto combo = std::static_pointer_cast< combobox >( item );
		auto amt = 0;
		for ( auto& cur : combo->get_items() )
		{
			s.create_user_object<decltype( cur.m_value )>( XOR_STR( "cfg.value" ), &cur.m_value );
			amt++;
		}

		g_menu->draw_mutex.unlock();
		return amt;
	}

	auto var = item->get_var();
	if ( !var )
	{
		g_menu->draw_mutex.unlock();
		s.error( util::format( XOR_STR( "failed to get config item: " ), s.get_string( 1 ) ) );
		return 0;
	}

	g_menu->draw_mutex.unlock();

	s.create_user_object<decltype( var )>( XOR_STR( "cfg.value" ), &var );
	return 1;
}

int lua::api_def::gui::is_menu_open( lua_State* l )
{
	runtime_state s( l );
	s.push( g_menu->is_open() );

	return 1;
}

int lua::api_def::gui::get_menu_rect( lua_State* l )
{
	runtime_state s( l );

	if ( g_menu->m_windows.empty() )
		return 0;

	const auto pos = g_menu->m_windows.front()->get_pos();
	const auto size = g_menu->m_windows.front()->get_size();
	s.push( pos.x );
	s.push( pos.y );
	s.push( pos.x + size.x );
	s.push( pos.y + size.y );

	return 4;
}

int lua::api_def::gui::set_visible( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::boolean },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: set_visible(path, bool)" ) );
		return 0;
	}

	// find our script beforehand
	const auto& me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	g_menu->draw_mutex.lock();

	if ( const auto curr = gui_helpers::find_control( s, s.get_string( 1 ) ) )
	{
		if ( std::ranges::find( me->get_gui_elements(), curr ) != me->get_gui_elements().end() )
		{
			curr->set_visibility( s.get_boolean( 2 ) );
			g_menu->draw_mutex.unlock();
			return 0;
		}

		if ( s.get_boolean( 2 ) )
		{
			if ( std::ranges::find( me->get_hidden_elements(), curr ) != me->get_hidden_elements().end() )
			{
				if ( --helpers::hidden_elements[ curr ] == 0 )
					curr->set_visibility( true );
			}
		}
		else if ( std::ranges::find( me->get_hidden_elements(), curr ) == me->get_hidden_elements().end() )
		{
			++helpers::hidden_elements[ curr ];
			curr->set_visibility( false );
			me->get_hidden_elements().push_back( curr );
		}

		g_menu->draw_mutex.unlock();
		return 0;
	}

	g_menu->draw_mutex.unlock();
	s.error( util::format( XOR_STR( "control was not found" ) ) );
	return 0;
}