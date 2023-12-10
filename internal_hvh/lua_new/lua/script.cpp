#include "../include_cheat.h"

using namespace evo::ren;

bool lua::script::initialize( bool is_script )
{
	// set panic function
	l.set_panic_func( api_def::panic );

	// hide poor globals first
	hide_globals();

	// register global functions
	register_globals();

	// register namespaces
	register_namespaces();

	// register types
	register_types();

	// attempt loading file
	if ( is_script && !l.load_file( file.c_str() ) )
	{
		lua::helpers::error( XOR_STR( "syntax_error" ), l.get_last_error_description().c_str() );
		return false;
	}

	return true;
}

void lua::script::unload()
{
	g_menu->draw_mutex.lock();

	// remove invisible controls
	for ( const auto& e : hidden_elements )
	{
		if ( --helpers::hidden_elements[ e ] == 0 )
		{
			e->set_visibility( true );
		}
	}

	// resolve parent and erase element, freeing it
	for ( const auto& e : gui_elements )
	{
		// remove from parent
		for ( const auto& sub : e->get_controls() )
			sub->set_parent( nullptr );

		e->get_parent()->remove_control( e );
	}

	g_menu->draw_mutex.unlock();

	// erase callbacks
	/*for ( const auto& e : gui_callbacks )
	{
		if ( const auto& c = ctx->find( e ) )
		{
			if ( c->universal_callbacks.find( id ) != c->universal_callbacks.end() )
				c->universal_callbacks[ id ].clear();
		}
	}*/

	for ( const auto& f : fonts )
	{
		if ( draw.has_font( f ) )
			draw.forget_font( f, true );
	}

	for ( const auto& t : textures )
	{
		if ( draw.has_texture( t ) )
			draw.forget_texture( t, true );
	}

	for ( const auto& a : animators )
	{
		if ( draw.has_anim( a ) )
			draw.forget_anim( a );
	}

	for ( const auto& f : materials )
	{
		f->DecrementReferenceCount();
	}

	if ( helpers::timers.contains( id ) )
		helpers::timers[ id ].clear();

	fonts.clear();
	textures.clear();
	gui_elements.clear();
	hidden_elements.clear();

	is_running = false;
}

bool lua::script::call_main()
{
	// taking off!
	if ( !l.run() )
	{
		lua::helpers::error( XOR_STR( "runtime_error" ), l.get_last_error_description().c_str() );
		return false;
	}

	// find all forwards
	find_forwards();
	return true;
}

void lua::script::find_forwards()
{
	// declare list of all possible forwards
	const std::vector<std::string> forward_list = {
		XOR_STR( "on_paint" ),
		XOR_STR( "on_paint_traverse" ),
		XOR_STR( "on_frame_stage_notify" ),
		XOR_STR( "on_setup_move" ),
		XOR_STR( "on_run_command" ),
		XOR_STR( "on_create_move" ),
		XOR_STR( "on_level_init" ),
		XOR_STR( "on_do_post_screen_space_events" ),
		XOR_STR( "on_input" ),
		XOR_STR( "on_game_event" ),
		XOR_STR( "on_shutdown" ),
		XOR_STR( "on_config_load" ),
		XOR_STR( "on_config_save" ),
		XOR_STR( "on_shot_registered" ),
		XOR_STR( "on_console_input" ),
		XOR_STR( "on_esp_flags" ),
		XOR_STR( "on_draw_model_execute" ),
	};

	// iterate through the list and check if we find any functions
	for ( const auto& f : forward_list )
	{
		l.get_global( f.c_str() );
		if ( l.is_function( -1 ) )
			forwards[ fnv1a_rt( f.c_str() ) ] = f;

		l.pop( 1 );
	}
}

void lua::script::create_forward( const char* n )
{
	forwards[ fnv1a_rt( n ) ] = n;
}

bool lua::script::has_forward( uint32_t hash )
{
	return forwards.find( hash ) != forwards.end();
}

void lua::script::call_forward( uint32_t hash, const std::function<int( state& )>& arg_callback, int ret, const std::function<void( state& )>& callback )
{
	// check if forward exists
	if ( !is_running || !has_forward( hash ) )
		return;

	// call the function
	l.get_global( forwards[ hash ].c_str() );
	if ( l.is_nil( -1 ) )
	{
		l.pop( 1 );
		return;
	}

	if ( !l.call( arg_callback ? arg_callback( l ) : 0, ret ) )
	{
		did_error = true;
		lua::helpers::error( XOR_STR( "runtime_error" ), l.get_last_error_description().c_str() );
		if ( const auto f = api.find_script_file( id ); f.has_value() )
			f->get().should_unload = true;

		return;
	}

	if ( ret )
	{
		if ( l.get_stack_top() < ret )
		{
			did_error = true;
			lua::helpers::error( XOR_STR( "runtime_error" ), XOR_STR( "Not enough return values." ) );
			if ( const auto f = api.find_script_file( id ); f.has_value() )
				f->get().should_unload = true;

			return;
		}

		if ( callback )
			callback( l );

		l.pop( l.get_stack_top() );
	}
}

void lua::script::hide_globals()
{
	l.unset_global( XOR_STR( "getfenv" ) );			// allows getting environment table
	if ( !vars::lua.allow_insecure->get<bool>() )
	{
		l.unset_global( XOR_STR( "pcall" ) );			// allows raw calling
		l.unset_global( XOR_STR( "xpcall" ) );			// ^
	}

	l.unset_global( XOR_STR( "loadfile" ) );
	if ( !vars::lua.allow_insecure->get<bool>() )
	{
		// allows direct file load
		l.unset_global( XOR_STR( "load" ) );				// allows direct load
		l.unset_global( XOR_STR( "loadstring" ) );          // allows direct string load
		l.unset_global( XOR_STR( "dofile" ) );			// same as loadfile but also executes it
	}

	l.unset_global( XOR_STR( "gcinfo" ) );			// garbage collector info
	l.unset_global( XOR_STR( "collectgarbage" ) );	// forces GC to collect
	l.unset_global( XOR_STR( "newproxy" ) );			// proxy functions
	l.unset_global( XOR_STR( "coroutine" ) );		// allows managing coroutines
	l.unset_global( XOR_STR( "setfenv" ) );			// allows replacing environment table

	if ( !vars::lua.allow_insecure->get<bool>() )
	{
		l.unset_global( XOR_STR( "rawget" ) );			// bypasses metatables
		l.unset_global( XOR_STR( "rawset" ) );			// ^
		l.unset_global( XOR_STR( "rawequal" ) );			// ^
	}

	l.unset_in_table( XOR_STR( "ffi" ), XOR_STR( "C" ) );		// useless without load()
	l.unset_global( XOR_STR( "_G" ) );				// disable global table loop
	l.unset_in_table( XOR_STR( "ffi" ), XOR_STR( "load" ) );	// allows loading DLLs
	l.unset_in_table( XOR_STR( "ffi" ), XOR_STR( "gc" ) );		// forces GC to collect
	l.unset_in_table( XOR_STR( "ffi" ), XOR_STR( "fill" ) );	// memory setting

	l.unset_in_table( XOR_STR( "string" ), XOR_STR( "dump" ) );	// useless without load()
}

void lua::script::register_namespaces()
{
	l.create_namespace( XOR_STR( "render" ), {
		{ XOR_STR( "color" ), api_def::render::color },
		{ XOR_STR( "rect_filled" ), api_def::render::rect_filled },
		{ XOR_STR( "rect" ), api_def::render::rect },
		{ XOR_STR( "rect_filled_rounded" ), api_def::render::rect_filled_rounded },
		{ XOR_STR( "text" ), api_def::render::text },
		{ XOR_STR( "get_screen_size" ), api_def::render::get_screen_size },
		{ XOR_STR( "create_font" ), api_def::render::create_font },
		{ XOR_STR( "create_font_gdi" ), api_def::render::create_font_gdi },
		{ XOR_STR( "get_text_size" ), api_def::render::get_text_size },
		{ XOR_STR( "circle" ), api_def::render::circle },
		{ XOR_STR( "circle_filled" ), api_def::render::circle_filled },
		{ XOR_STR( "create_texture" ), api_def::render::create_texture },
		{ XOR_STR( "create_texture_svg" ), api_def::render::create_texture_svg },
		{ XOR_STR( "create_texture_bytes" ), api_def::render::create_texture_bytes },
		{ XOR_STR( "create_texture_rgba" ), api_def::render::create_texture_rgba },
		{ XOR_STR( "create_texture_stream" ), api_def::render::create_texture_stream },
		{ XOR_STR( "create_font_stream" ), api_def::render::create_font_stream },
		{ XOR_STR( "push_texture" ), api_def::render::push_texture },
		{ XOR_STR( "pop_texture" ), api_def::render::pop_texture },
		{ XOR_STR( "get_texture_size" ), api_def::render::get_texture_size },
		{ XOR_STR( "create_animator_float" ), api_def::render::create_animator_float },
		{ XOR_STR( "create_animator_color" ), api_def::render::create_animator_color },
		{ XOR_STR( "push_clip_rect" ), api_def::render::push_clip_rect },
		{ XOR_STR( "pop_clip_rect" ), api_def::render::pop_clip_rect },
		{ XOR_STR( "push_uv" ), api_def::render::push_uv },
		{ XOR_STR( "pop_uv" ), api_def::render::pop_uv },
		{ XOR_STR( "rect_filled_multicolor" ), api_def::render::rect_filled_multicolor },
		{ XOR_STR( "line" ), api_def::render::line },
		{ XOR_STR( "line_multicolor" ), api_def::render::line_multicolor },
		{ XOR_STR( "triangle" ), api_def::render::triangle },
		{ XOR_STR( "triangle_filled" ), api_def::render::triangle_filled },
		{ XOR_STR( "triangle_filled_multicolor" ), api_def::render::triangle_filled_multicolor },
		{ XOR_STR( "esp_flag" ), api_def::render::esp_flag },
	} );

	l.set_integers_for_table( XOR_STR( "render" ), {
		// alignment
		{ XOR_STR( "align_left" ), 0 },
		{ XOR_STR( "align_top" ), 0 },
		{ XOR_STR( "align_center" ), 1 },
		{ XOR_STR( "align_right" ), 2 },
		{ XOR_STR( "align_bottom" ), 2 },
		{ XOR_STR( "top_left" ), 1 },
		{ XOR_STR( "top_right" ), 2 },
		{ XOR_STR( "bottom_left" ), 4 },
		{ XOR_STR( "bottom_right" ), 8 },
		{ XOR_STR( "top" ), 1 | 2 },
		{ XOR_STR( "bottom" ), 4 | 8 },
		{ XOR_STR( "left" ), 1 | 4 },
		{ XOR_STR( "right" ), 2 | 8 },
		{ XOR_STR( "all" ), 1 | 2 | 4 | 8 },

		// font flags
		{ XOR_STR( "font_flag_shadow" ), 1 },
		{ XOR_STR( "font_flag_outline" ), 2 },

		// default fonts
		{ XOR_STR( "font_tab" ), FNV1A( "tab_font" ) },
		{ XOR_STR( "font_indicator" ), FNV1A( "lby" ) },
		{ XOR_STR( "font_control" ), FNV1A( "control" ) },
		{ XOR_STR( "font_esp" ), FNV1A( "esp" ) },

		// easing
		{ XOR_STR( "linear" ), 0 },
		{ XOR_STR( "ease_in" ), 1 },
		{ XOR_STR( "ease_out" ), 2 },
		{ XOR_STR( "ease_in_out" ), 3 },
		{ XOR_STR( "elastic_in" ), 4 },
		{ XOR_STR( "elastic_out" ), 5 },
		{ XOR_STR( "elastic_in_out" ), 6 },
		{ XOR_STR( "bounce_in" ), 7 },
		{ XOR_STR( "bounce_out" ), 8 },
		{ XOR_STR( "bounce_in_out" ), 9 },

		// default textures
		/*{ XOR_STR( "texture_logo_head" ), GUI_HASH( "gui_logo_head" ) },
		{ XOR_STR( "texture_logo_stripes" ), GUI_HASH( "gui_logo_stripes" ) },
		{ XOR_STR( "texture_pattern" ), GUI_HASH( "gui_pattern" ) },
		{ XOR_STR( "texture_pattern_group" ), GUI_HASH( "gui_pattern_group" ) },
		{ XOR_STR( "texture_cursor" ), GUI_HASH( "gui_cursor" ) },
		{ XOR_STR( "texture_icon_up" ), GUI_HASH( "gui_icon_up" ) },
		{ XOR_STR( "texture_icon_down" ), GUI_HASH( "gui_icon_down" ) },
		{ XOR_STR( "texture_icon_clear" ), GUI_HASH( "gui_icon_clear" ) },
		{ XOR_STR( "texture_icon_copy" ), GUI_HASH( "gui_icon_copy" ) },
		{ XOR_STR( "texture_icon_paste" ), GUI_HASH( "gui_icon_paste" ) },
		{ XOR_STR( "texture_icon_alert" ), GUI_HASH( "gui_icon_alert" ) },
		{ XOR_STR( "texture_icon_rage" ), GUI_HASH( "gui_icon_rage" ) },
		{ XOR_STR( "texture_icon_legit" ), GUI_HASH( "gui_icon_legit" ) },
		{ XOR_STR( "texture_icon_visuals" ), GUI_HASH( "gui_icon_visuals" ) },
		{ XOR_STR( "texture_icon_misc" ), GUI_HASH( "gui_icon_misc" ) },
		{ XOR_STR( "texture_icon_scripts" ), GUI_HASH( "gui_icon_scripts" ) },
		{ XOR_STR( "texture_icon_skins" ), GUI_HASH( "gui_icon_skins" ) },
		{ XOR_STR( "texture_icon_cloud" ), GUI_HASH( "gui_icon_cloud" ) },
		{ XOR_STR( "texture_icon_file" ), GUI_HASH( "gui_icon_file" ) },
		{ XOR_STR( "texture_avatar" ), GUI_HASH( "user_avatar" ) },*/
	} );

	l.create_namespace( XOR_STR( "utils" ), {
		{ XOR_STR( "random_int" ), api_def::utils::random_int },
		{ XOR_STR( "random_float" ), api_def::utils::random_float },
		{ XOR_STR( "flags" ), api_def::utils::flags },
		{ XOR_STR( "find_interface" ), api_def::utils::find_interface },
		{ XOR_STR( "find_pattern" ), api_def::utils::find_pattern },
		{ XOR_STR( "get_weapon_info" ), api_def::utils::get_weapon_info },
		{ XOR_STR( "new_timer" ), api_def::utils::new_timer },
		{ XOR_STR( "run_delayed" ), api_def::utils::run_delayed },
		{ XOR_STR( "world_to_screen" ), api_def::utils::world_to_screen },
		{ XOR_STR( "get_rtt" ), api_def::utils::get_rtt },
		{ XOR_STR( "get_time" ), api_def::utils::get_time },
		{ XOR_STR( "http_get" ), api_def::utils::http_get },
		{ XOR_STR( "http_post" ), api_def::utils::http_post },
		{ XOR_STR( "json_encode" ), api_def::utils::json_encode },
		{ XOR_STR( "json_decode" ), api_def::utils::json_decode },
		{ XOR_STR( "set_clan_tag" ), api_def::utils::set_clan_tag },
		{ XOR_STR( "trace" ), api_def::utils::trace },
		{ XOR_STR( "trace_bullet" ), api_def::utils::trace_bullet },
		{ XOR_STR( "scale_damage" ), api_def::utils::scale_damage },
		{ XOR_STR( "load_file" ), api_def::utils::load_file },
		{ XOR_STR( "print_console" ), api_def::utils::print_console },
		{ XOR_STR( "print_dev_console" ), api_def::utils::print_dev_console },
		{ XOR_STR( "error_print" ), api_def::utils::error_print },
		{ XOR_STR( "aes256_encrypt" ), api_def::utils::aes256_encrypt },
		{ XOR_STR( "aes256_decrypt" ), api_def::utils::aes256_decrypt },
		{ XOR_STR( "base64_encode" ), api_def::utils::base64_encode },
		{ XOR_STR( "base64_decode" ), api_def::utils::base64_decode },
		{ XOR_STR( "get_unix_time" ), api_def::utils::get_unix_time },
	} );

	l.create_namespace( XOR_STR( "client" ), {
		{ XOR_STR( "create_interface" ), api_def::utils::find_interface },
		{ XOR_STR( "find_signature" ), api_def::utils::find_pattern },
	} );

	l.create_namespace( XOR_STR( "gui" ), {
		{ XOR_STR( "add_checkbox" ), api_def::gui::add_checkbox },
		{ XOR_STR( "add_slider" ), api_def::gui::add_slider },
		{ XOR_STR( "add_combo" ), api_def::gui::add_combo },
		{ XOR_STR( "add_multi_combo" ), api_def::gui::add_multi_combo },
		{ XOR_STR( "add_button" ), api_def::gui::add_button },
		{ XOR_STR( "add_textbox" ), api_def::gui::add_textbox },
		{ XOR_STR( "add_listbox" ), api_def::gui::add_listbox },
		{ XOR_STR( "add_keybind" ), api_def::gui::add_keybind },
		{ XOR_STR( "add_colorpicker" ), api_def::gui::add_colorpicker },
		{ XOR_STR( "get_keybind" ), api_def::gui::get_keybind },
		{ XOR_STR( "get_listbox_items" ), api_def::gui::get_listbox_items },
		{ XOR_STR( "set_listbox_items" ), api_def::gui::set_listbox_items },
		{ XOR_STR( "get_combo_items" ), api_def::gui::get_combo_items },
		{ XOR_STR( "set_combo_items" ), api_def::gui::set_combo_items },
		{ XOR_STR( "get_config_item" ), api_def::gui::get_config_item },
		{ XOR_STR( "is_menu_open" ), api_def::gui::is_menu_open },
		{ XOR_STR( "get_menu_rect" ), api_def::gui::get_menu_rect },
		{ XOR_STR( "set_visible" ), api_def::gui::set_visible },
	} );

	l.set_integers_for_table( XOR_STR( "gui" ), {
		{ XOR_STR( "always_on" ), 0 },
		{ XOR_STR( "hold" ), 1 },
		{ XOR_STR( "toggle" ), 2 },
		{ XOR_STR( "force_off" ), 3 },
	} );

	l.create_namespace( XOR_STR( "input" ), {
		{ XOR_STR( "is_key_down" ), api_def::input::is_key_down },
		{ XOR_STR( "get_cursor_pos" ), api_def::input::get_cursor_pos },
	} );

	l.extend_namespace( XOR_STR( "math" ), {
		{ XOR_STR( "vec3" ), api_def::math::vec3_new },
		{ XOR_STR( "vector_angles" ), api_def::math::vector_angles },
		{ XOR_STR( "angle_vectors" ), api_def::math::angle_vectors }
	} );
}

void lua::script::register_globals()
{
	l.set_global_function( XOR_STR( "print" ), api_def::print );
	l.set_global_function( XOR_STR( "require" ), api_def::require );
	l.set_global_function( XOR_STR( "loadfile" ), api_def::loadfile );

	l.create_table();
	{
		l.set_field( XOR_STR( "save" ), api_def::database::save );
		l.set_field( XOR_STR( "load" ), api_def::database::load );

		l.create_metatable( XOR_STR( "database" ) );
		l.set_field( XOR_STR( "__newindex" ), api_def::stub_new_index );
		l.set_field( XOR_STR( "__index" ), api_def::stub_index );
		l.set_metatable();

	}
	l.set_global( XOR_STR( "database" ) );

	l.create_table();
	{
		l.set_field( XOR_STR( "is_in_game" ), api_def::engine::is_in_game );
		l.set_field( XOR_STR( "exec" ), api_def::engine::exec );
		l.set_field( XOR_STR( "get_local_player" ), api_def::engine::get_local_player );
		l.set_field( XOR_STR( "get_view_angles" ), api_def::engine::get_view_angles );
		l.set_field( XOR_STR( "get_player_for_user_id" ), api_def::engine::get_player_for_user_id );
		l.set_field( XOR_STR( "get_player_info" ), api_def::engine::get_player_info );
		l.set_field( XOR_STR( "set_view_angles" ), api_def::engine::set_view_angles );

		l.create_metatable( XOR_STR( "engine" ) );
		l.set_field( XOR_STR( "__newindex" ), api_def::stub_new_index );
		l.set_field( XOR_STR( "__index" ), api_def::stub_index );
		l.set_metatable();

	}
	l.set_global( XOR_STR( "engine" ) );

	l.create_table();
	{
		l.set_field( XOR_STR( "get_entity" ), api_def::entities::get_entity );
		l.set_field( XOR_STR( "get_entity_from_handle" ), api_def::entities::get_entity_from_handle );
		l.set_field( XOR_STR( "for_each" ), api_def::entities::for_each );
		l.set_field( XOR_STR( "for_each_z" ), api_def::entities::for_each_z );
		l.set_field( XOR_STR( "for_each_player" ), api_def::entities::for_each_player );
		l.set_field( XOR_STR( "for_each_player_z" ), api_def::entities::for_each_player_z );

		l.create_metatable( XOR_STR( "entities" ) );
		l.set_field( XOR_STR( "__newindex" ), api_def::stub_new_index );
		l.set_field( XOR_STR( "__index" ), api_def::entities::index );
		l.set_metatable();

	}
	l.set_global( XOR_STR( "entities" ) );

	l.create_table();
	{
		l.create_table();
		{
			l.create_metatable( XOR_STR( "fatality" ) );
			l.set_field( XOR_STR( "__newindex" ), api_def::stub_new_index );
			l.set_field( XOR_STR( "__index" ), api_def::fatality_index );
			l.set_metatable();
		}
		l.set_field( XOR_STR( "fatality" ) );

		l.create_table();
		{
			l.create_metatable( XOR_STR( "server" ) );
			l.set_field( XOR_STR( "__newindex" ), api_def::stub_new_index );
			l.set_field( XOR_STR( "__index" ), api_def::server_index );
			l.set_metatable();
		}
		l.set_field( XOR_STR( "server" ) );
	}
	l.set_global( XOR_STR( "info" ) );

	l.create_table();
	{
		l.create_metatable( XOR_STR( "cvar" ) );
		l.set_field( XOR_STR( "__newindex" ), api_def::stub_new_index );
		l.set_field( XOR_STR( "__index" ), api_def::cvar::index );
		l.set_metatable();
	}
	l.set_global( XOR_STR( "cvar" ) );

	l.create_table();
	{
		l.create_metatable( XOR_STR( "global_vars" ) );
		l.set_field( XOR_STR( "__newindex" ), api_def::stub_new_index );
		l.set_field( XOR_STR( "__index" ), api_def::global_vars_index );
		l.set_metatable();
	}
	l.set_global( XOR_STR( "global_vars" ) );

	l.create_table();
	{
		l.create_metatable( XOR_STR( "game_rules" ) );
		l.set_field( XOR_STR( "__newindex" ), api_def::stub_new_index );
		l.set_field( XOR_STR( "__index" ), api_def::game_rules_index );
		l.set_metatable();
	}
	l.set_global( XOR_STR( "game_rules" ) );

	l.create_namespace( XOR_STR( "mat" ), {
		{ XOR_STR( "create" ), api_def::mat::create },
		{ XOR_STR( "find" ), api_def::mat::find },
		{ XOR_STR( "for_each_material" ), api_def::mat::for_each_material },
		{ XOR_STR( "override_material" ), api_def::mat::override_material },
	} );

	l.set_integers_for_table( XOR_STR( "mat" ), {
		{ XOR_STR( "var_debug" ), MATERIAL_VAR_DEBUG },
		{ XOR_STR( "var_no_debug_override" ), MATERIAL_VAR_NO_DEBUG_OVERRIDE },
		{ XOR_STR( "var_no_draw" ), MATERIAL_VAR_NO_DRAW },
		{ XOR_STR( "var_use_in_fillrate_mode" ), MATERIAL_VAR_USE_IN_FILLRATE_MODE },
		{ XOR_STR( "var_vertexcolor" ), MATERIAL_VAR_VERTEXCOLOR },
		{ XOR_STR( "var_vertexalpha" ), MATERIAL_VAR_VERTEXALPHA },
		{ XOR_STR( "var_selfillum" ), MATERIAL_VAR_SELFILLUM },
		{ XOR_STR( "var_additive" ), MATERIAL_VAR_ADDITIVE },
		{ XOR_STR( "var_alphatest" ), MATERIAL_VAR_ALPHATEST },
		{ XOR_STR( "var_znearer" ), MATERIAL_VAR_ZNEARER },
		{ XOR_STR( "var_model" ), MATERIAL_VAR_MODEL },
		{ XOR_STR( "var_flat" ), MATERIAL_VAR_FLAT },
		{ XOR_STR( "var_nocull" ), MATERIAL_VAR_NOCULL },
		{ XOR_STR( "var_nofog" ), MATERIAL_VAR_NOFOG },
		{ XOR_STR( "var_ignorez" ), MATERIAL_VAR_IGNOREZ },
		{ XOR_STR( "var_decal" ), MATERIAL_VAR_DECAL },
		{ XOR_STR( "var_envmapsphere" ), MATERIAL_VAR_ENVMAPSPHERE },
		{ XOR_STR( "var_envmapcameraspace" ), MATERIAL_VAR_ENVMAPCAMERASPACE },
		{ XOR_STR( "var_basealphaenvmapmask" ), MATERIAL_VAR_BASEALPHAENVMAPMASK },
		{ XOR_STR( "var_translucent" ), MATERIAL_VAR_TRANSLUCENT },
		{ XOR_STR( "var_normalmapalphaenvmapmask" ), MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK },
		{ XOR_STR( "var_needs_software_skinning" ), MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING },
		{ XOR_STR( "var_opaquetexture" ), MATERIAL_VAR_OPAQUETEXTURE },
		{ XOR_STR( "var_envmapmode" ), MATERIAL_VAR_ENVMAPMODE },
		{ XOR_STR( "var_suppress_decals" ), MATERIAL_VAR_SUPPRESS_DECALS },
		{ XOR_STR( "var_halflambert" ), MATERIAL_VAR_HALFLAMBERT },
		{ XOR_STR( "var_wireframe" ), MATERIAL_VAR_WIREFRAME },
		{ XOR_STR( "var_allowalphatocoverage" ), MATERIAL_VAR_ALLOWALPHATOCOVERAGE },
		{ XOR_STR( "var_alpha_modified_by_proxy" ), MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY },
		{ XOR_STR( "var_vertexfog" ), MATERIAL_VAR_VERTEXFOG },
	} );

	l.create_table();
	{
		l.set_field( XOR_STR( "frame_undefined" ), -1 );
		l.set_field( XOR_STR( "frame_start" ), 0 );
		l.set_field( XOR_STR( "frame_net_update_start" ), 1 );
		l.set_field( XOR_STR( "frame_net_update_postdataupdate_start" ), 2 );
		l.set_field( XOR_STR( "frame_net_update_postdataupdate_end" ), 3 );
		l.set_field( XOR_STR( "frame_net_update_end" ), 4 );
		l.set_field( XOR_STR( "frame_render_start" ), 5 );
		l.set_field( XOR_STR( "frame_render_end" ), 6 );
		l.set_field( XOR_STR( "in_attack" ), IN_ATTACK );
		l.set_field( XOR_STR( "in_jump" ), IN_JUMP );
		l.set_field( XOR_STR( "in_duck" ), IN_DUCK );
		l.set_field( XOR_STR( "in_forward" ), IN_FORWARD );
		l.set_field( XOR_STR( "in_back" ), IN_BACK );
		l.set_field( XOR_STR( "in_use" ), IN_USE );
		l.set_field( XOR_STR( "in_left" ), IN_LEFT );
		l.set_field( XOR_STR( "in_move_left" ), IN_MOVELEFT );
		l.set_field( XOR_STR( "in_right" ), IN_RIGHT );
		l.set_field( XOR_STR( "in_move_right" ), IN_MOVERIGHT );
		l.set_field( XOR_STR( "in_attack2" ), IN_ATTACK2 );
		l.set_field( XOR_STR( "in_score" ), IN_SCORE );
	}
	l.set_global( XOR_STR( "csgo" ) );
}

void lua::script::register_types()
{
	l.create_type( XOR_STR( "render.anim_float" ), {
		{ XOR_STR( "direct" ), api_def::render::anim_float_direct },
		{ XOR_STR( "get_value" ), api_def::render::anim_float_get_value },
	} );

	l.create_type( XOR_STR( "render.anim_color" ), {
		{ XOR_STR( "direct" ), api_def::render::anim_color_direct },
		{ XOR_STR( "get_value" ), api_def::render::anim_color_get_value },
	} );

	l.create_type( XOR_STR( "render.esp_flag" ), {} );

	l.create_type( XOR_STR( "csgo.entity" ), {
		{ XOR_STR( "get_index" ), api_def::entity::get_index },
		{ XOR_STR( "is_valid" ), api_def::entity::is_valid },
		{ XOR_STR( "is_alive" ), api_def::entity::is_alive },
		{ XOR_STR( "is_dormant" ), api_def::entity::is_dormant },
		{ XOR_STR( "is_player" ), api_def::entity::is_player },
		{ XOR_STR( "is_enemy" ), api_def::entity::is_enemy },
		{ XOR_STR( "get_class" ), api_def::entity::get_class },
		{ XOR_STR( "get_prop" ), api_def::entity::get_prop },
		{ XOR_STR( "set_prop" ), api_def::entity::set_prop },
		{ XOR_STR( "get_hitbox_position" ), api_def::entity::get_hitbox_position },
		{ XOR_STR( "get_eye_position" ), api_def::entity::get_eye_position },
		{ XOR_STR( "get_player_info" ), api_def::entity::get_player_info },
		{ XOR_STR( "get_move_type" ), api_def::entity::get_move_type },
		{ XOR_STR( "get_bbox" ), api_def::entity::get_bbox },
		{ XOR_STR( "get_weapon" ), api_def::entity::get_weapon },
		{ XOR_STR( "get_esp_alpha" ), api_def::entity::get_esp_alpha },
	} );

	l.create_type( XOR_STR( "csgo.cvar" ), {
		{ XOR_STR( "get_int" ), api_def::cvar::get_int },
		{ XOR_STR( "get_float" ), api_def::cvar::get_float },
		{ XOR_STR( "set_int" ), api_def::cvar::set_int },
		{ XOR_STR( "set_float" ), api_def::cvar::set_float },
		{ XOR_STR( "get_string" ), api_def::cvar::get_string },
		{ XOR_STR( "set_string" ), api_def::cvar::set_string },
	} );

	l.create_type( XOR_STR( "csgo.event" ), {
		{ XOR_STR( "__index" ), api_def::event::index },
		{ XOR_STR( "__newindex" ), api_def::stub_new_index },
	} );

	l.create_type( XOR_STR( "csgo.user_cmd" ), {
		{ XOR_STR( "get_command_number" ), api_def::user_cmd::get_command_number },
		{ XOR_STR( "get_view_angles" ), api_def::user_cmd::get_view_angles },
		{ XOR_STR( "get_move" ), api_def::user_cmd::get_move },
		{ XOR_STR( "get_buttons" ), api_def::user_cmd::get_buttons },
		{ XOR_STR( "set_view_angles" ), api_def::user_cmd::set_view_angles },
		{ XOR_STR( "set_move" ), api_def::user_cmd::set_move },
		{ XOR_STR( "set_buttons" ), api_def::user_cmd::set_buttons },
	} );

	l.create_type( XOR_STR( "csgo.material" ), {
		{ XOR_STR( "__index" ), api_def::mat::index },
		{ XOR_STR( "__gc" ), api_def::mat::gc },
	} );

	l.create_type( XOR_STR( "csgo.material_var" ), {
		{ XOR_STR( "get_float" ), api_def::mat::get_float },
		{ XOR_STR( "set_float" ), api_def::mat::set_float },
		{ XOR_STR( "get_int" ), api_def::mat::get_int },
		{ XOR_STR( "set_int" ), api_def::mat::set_int },
		{ XOR_STR( "get_string" ), api_def::mat::get_string },
		{ XOR_STR( "set_string" ), api_def::mat::set_string },
		{ XOR_STR( "get_vector" ), api_def::mat::get_vector },
		{ XOR_STR( "set_vector" ), api_def::mat::set_vector },
	} );

	l.create_namespace( XOR_STR( "panorama" ), {
		{ XOR_STR( "eval" ), api_def::panorama::eval }
	} );

	l.create_namespace( XOR_STR( "fs" ), {
		{ XOR_STR( "read" ), api_def::fs::read },
		{ XOR_STR( "read_stream" ), api_def::fs::read_stream },
		{ XOR_STR( "write" ), api_def::fs::write },
		{ XOR_STR( "write_stream" ), api_def::fs::write_stream },
		{ XOR_STR( "remove" ), api_def::fs::remove },
		{ XOR_STR( "exists" ), api_def::fs::exists },
		{ XOR_STR( "is_file" ), api_def::fs::is_file },
		{ XOR_STR( "is_dir" ), api_def::fs::is_dir },
		{ XOR_STR( "create_dir" ), api_def::fs::create_dir },
	} );

	l.create_namespace( XOR_STR( "zip" ), {
		{ XOR_STR( "create" ), api_def::zip::create },
		{ XOR_STR( "open" ), api_def::zip::open },
		{ XOR_STR( "open_stream" ), api_def::zip::open_stream },
	} );

	l.create_type( XOR_STR( "utils.timer" ), {
		{ XOR_STR( "start" ), api_def::timer::start },
		{ XOR_STR( "stop" ), api_def::timer::stop },
		{ XOR_STR( "run_once" ), api_def::timer::run_once },
		{ XOR_STR( "set_delay" ), api_def::timer::set_delay },
		{ XOR_STR( "is_active" ), api_def::timer::is_active }
	} );

	l.create_type( XOR_STR( "vec3" ), {
		{ XOR_STR( "__index" ), api_def::math::vec3_index },
		{ XOR_STR( "__newindex" ), api_def::math::vec3_new_index },
		{ XOR_STR( "__add" ), api_def::math::vec3_add },
		{ XOR_STR( "__sub" ), api_def::math::vec3_sub },
		{ XOR_STR( "__mul" ), api_def::math::vec3_mul },
		{ XOR_STR( "__div" ), api_def::math::vec3_div },
		{ XOR_STR( "calc_angle" ), api_def::math::vec3_calc_angle },
		{ XOR_STR( "unpack" ), api_def::math::vec3_unpack },
	} );

	l.create_type( XOR_STR( "cfg.value" ), {
		{ XOR_STR( "get_int" ), api_def::cfg_value::get_int },
		{ XOR_STR( "get_bool" ), api_def::cfg_value::get_bool },
		{ XOR_STR( "get_float" ), api_def::cfg_value::get_float },
		{ XOR_STR( "get_color" ), api_def::cfg_value::get_color },
		{ XOR_STR( "get_string" ), api_def::cfg_value::get_string },
		{ XOR_STR( "set_int" ), api_def::cfg_value::set_int },
		{ XOR_STR( "set_bool" ), api_def::cfg_value::set_bool },
		{ XOR_STR( "set_float" ), api_def::cfg_value::set_float },
		{ XOR_STR( "set_color" ), api_def::cfg_value::set_color },
		{ XOR_STR( "set_string" ), api_def::cfg_value::set_string },
	} );

	l.create_type( XOR_STR( "zip" ), {
		{ XOR_STR( "__gc" ), api_def::zip::gc },
		{ XOR_STR( "get_files" ), api_def::zip::get_files },
		{ XOR_STR( "read" ), api_def::zip::read },
		{ XOR_STR( "read_stream" ), api_def::zip::read_stream },
		{ XOR_STR( "write" ), api_def::zip::write },
		{ XOR_STR( "write_stream" ), api_def::zip::write_stream },
		{ XOR_STR( "save" ), api_def::zip::save },
		{ XOR_STR( "exists" ), api_def::zip::exists },
		{ XOR_STR( "extract" ), api_def::zip::extract },
		{ XOR_STR( "extract_all" ), api_def::zip::extract_all },
		} );
}

void lua::script::add_gui_element( const std::shared_ptr<control> elem )
{
	gui_elements.push_back( elem );
	elem->run_input();
}

const std::vector<std::shared_ptr<control>>& lua::script::get_gui_elements()
{
	return gui_elements;
}

std::vector<std::shared_ptr<control>>& lua::script::get_hidden_elements()
{
	return hidden_elements;
}

void lua::script::add_gui_element_with_callback( uint32_t e_id )
{
	if ( std::find( gui_callbacks.begin(), gui_callbacks.end(), e_id ) == gui_callbacks.end() )
		gui_callbacks.emplace_back( e_id );
}

void lua::script::add_font( uint32_t _id )
{
	fonts.emplace_back( _id );
}

void lua::script::add_texture( uint32_t _id )
{
	textures.emplace_back( _id );
}

void lua::script::add_animator( uint32_t _id )
{
	animators.emplace_back( _id );
}

void lua::script::add_material( IMaterial* mat )
{
	materials.emplace_back( mat );
}

void lua::script::run_timers() const
{
	if ( !is_running || lua::helpers::timers[ id ].empty() )
		return;

	auto& my_timers = lua::helpers::timers[ id ];

	// loop through all existing callbacks
	for ( auto it = my_timers.begin(); it != my_timers.end();)
	{
		// get current callback
		const auto timer = *it;

		// skip if inactive
		if ( !timer->is_active() )
		{
			++it;
			continue;
		}

		// we waited long enough
		if ( timer->cycle_completed() )
		{
			// run callback
			const auto cbk = timer->get_callback();

			if ( cbk )
				cbk();
		}

		// should be deleted
		if ( timer->should_delete() )
		{
			// erase callback
			it = my_timers.erase( it );
		}
		else
			++it;
	}
}