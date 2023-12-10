#include "../include_cheat.h"

namespace lua::api_def::panorama
{
	bool value_to_lua( runtime_state& s, v8::value_t* val )
	{
		int _unk;
		if ( val->is_number_object() || val->is_number() )
			s.push( val->number_value() );
		else if ( val->is_boolean() || val->is_boolean_object() )
			s.push( val->boolean_value() );
		else if ( val->is_string() || val->is_string_object() )
		{
			v8::utf8_string_t str;
			str.ctor( val );
			s.push( std::string( str.value, str.length ) );
			str.dtor();
		}
		else if ( val->is_object() )
		{
			// not going to do this yet.
			if ( val->is_function() )
				return false;

			const auto obj = static_cast< v8::object_t* >( val->to_object( &_unk )->value );
			if ( !obj )
				return false;

			if ( val->is_array() )
			{
				const auto arr = reinterpret_cast< v8::array_t* >( obj );

				s.create_table();
				for ( int i{}; i < arr->length(); ++i )
				{
					const auto v = arr->get( &_unk, i )->value;
					if ( !v )
						continue;

					// push the value to the stack
					if ( !value_to_lua( s, v ) )
						continue;

					s.set_i( i + 1 );
				}
			}
			else
			{
				const auto obj_names = static_cast< v8::array_t* >( obj->get_property_names( &_unk )->value );
				if ( !obj_names )
					return false;

				s.create_table();
				for ( int i{}; i < obj_names->length(); ++i )
				{
					const auto obj_name = obj_names->get( &_unk, i )->value;
					if ( !obj_name || !obj_name->is_string() && !obj_name->is_string_object() )
						continue;

					const auto v = obj->get( &_unk, obj_name )->value;
					if ( !v )
						continue;

					// push the value
					if ( !value_to_lua( s, v ) )
						continue;

					v8::utf8_string_t str;
					str.ctor( obj_name );
					s.set_field( str.value );
					str.dtor();
				}
			}
		}

		return true;
	}

	int eval( lua_State* l )
	{
		runtime_state s( l );

		if ( !vars::lua.allow_insecure->get<bool>() )
		{
			s.error( XOR_STR( "eval is not available with Allow insecure disabled" ) );
			return 0;
		}

		const auto r = s.check_arguments( {
			{ ltd::string }
		}, true );

		if ( !r )
		{
			s.error( XOR_STR( "usage: panorama.run_script(script)" ) );
			return 0;
		}

		const auto ui_engine = interfaces::panoroama_ui_engine()->get_ui_engine();
		const auto me = api.find_by_state( l );
		if ( !me )
		{
			s.error( XOR_STR( "sandbox problem" ) );
			return 0;
		}

		const auto find_panel = [ui_engine] ( uint32_t id ) -> ui_panel_t*
		{
			for ( auto i = 0; i < ui_engine->panels.m_Memory.m_nAllocationCount; i++ )
			{
				const auto& panel = ui_engine->panels.Element( i );
				if ( !ui_engine->is_valid_panel_ptr( panel.panel ) )
					continue;

				if ( std::string( panel.panel->get_id() ).empty() )
					continue;

				if ( fnv1a_rt( panel.panel->get_id() ) == id )
					return panel.panel;
			}

			return nullptr;
		};

		// get js context
		ui_panel_t* js_context{};
		if ( s.get_stack_top() == 2 && s.is_string( 2 ) )
		{
			js_context = find_panel( fnv1a_rt( s.get_string( 2 ) ) );
			if ( !js_context )
			{
				s.error( util::format( XOR_STR( "panorama: specified panel not found: %s" ), s.get_string( 2 ) ) );
				return 0;
			}
		}

		if ( !js_context )
			js_context = find_panel( FNV1A( "CSGOJsRegistration" ) );

		if ( !js_context || !ui_engine->is_valid_panel_ptr( js_context ) )
		{
			s.error( XOR_STR( "panorama: unable to find panel" ) );
			return 0;
		}

		v8::handle_scope_t scope;
		v8::try_catch_t try_catch;

		const auto isolate = ui_engine->get_isolate();
		isolate->enter();
		scope.enter( isolate );

		const auto ctx = v8::handle_scope_t::create_handle( isolate, **ui_engine->get_panel_context( js_context->get_js_context_parent() ) );
		ctx->enter();

		// compile in try/catch.
		try_catch.enter( isolate );
		const auto compiled = ui_engine->compile( js_context, util::format( XOR_STR( "(function(){%s})()" ), s.get_string( 1 ) ).c_str() );
		try_catch.exit();

		if ( !compiled->value )
		{
			ctx->exit();
			scope.exit();
			isolate->exit();
			return 0;
		}

		// run script.
		int ret_count{}, _unk;

		const auto val = ui_engine->run_compiled_script( &_unk, js_context, compiled->value )->value;
		if ( !val )
		{
			ctx->exit();
			scope.exit();
			isolate->exit();
			return 0;
		}

		// deduct return params.
		if ( value_to_lua( s, val ) )
			ret_count = 1;

		ctx->exit();
		scope.exit();
		isolate->exit();

		return ret_count;
	}
}