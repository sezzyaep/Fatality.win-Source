#include "../include_cheat.h"

namespace lua::api_def::mat
{
	int index( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
			return 0;

		switch ( fnv1a_rt( s.get_string( 2 ) ) )
		{
			case FNV1A( "modulate" ):
				s.push( modulate );
				return 1;
			case FNV1A( "set_flag" ):
				s.push( set_flag );
				return 1;
			case FNV1A( "get_flag" ):
				s.push( get_flag );
				return 1;
			case FNV1A( "find_var" ):
				s.push( find_var );
				return 1;
			case FNV1A( "get_name" ):
				s.push( get_name );
				return 1;
			case FNV1A( "get_group" ):
				s.push( get_group );
				return 1;
		}

		uint32_t tok;
		const auto var = s.user_data_ptr<IMaterial>( 1 )->FindVarFast( s.get_string( 2 ), &tok );
		if ( !var )
			return 0;

		s.create_user_object_ptr( XOR_STR( "csgo.material_var" ), var );
		return 1;
	}

	int gc( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data } } ) )
			return 0;

		//s.user_data_ptr<IMaterial>( 1 )->DecrementReferenceCount();
		return 0;
	}

	int get_float( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data } } ) )
			return 0;

		s.push( s.user_data_ptr<IMaterialVar>( 1 )->get_float() );
		return 1;
	}

	int set_float( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::number } } ) )
		{
			s.error( XOR_STR( "usage: material_var:set_float(val)" ) );
			return 0;
		}

		s.user_data_ptr<IMaterialVar>( 1 )->set_float_vaue( s.get_float( 2 ) );
		return 0;
	}

	int get_int( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data } } ) )
			return 0;

		s.push( s.user_data_ptr<IMaterialVar>( 1 )->get_int() );
		return 1;
	}

	int set_int( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::number } } ) )
		{
			s.error( XOR_STR( "usage: material_var:set_int(val)" ) );
			return 0;
		}

		s.user_data_ptr<IMaterialVar>( 1 )->set_int_value( s.get_integer( 2 ) );
		return 0;
	}

	int get_string( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data } } ) )
			return 0;

		s.push( s.user_data_ptr<IMaterialVar>( 1 )->get_string() );
		return 1;
	}

	int set_string( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: material_var:set_string(val)" ) );
			return 0;
		}

		s.user_data_ptr<IMaterialVar>( 1 )->set_string( s.get_string( 2 ) );
		return 0;
	}

	int get_vector( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data } } ) )
			return 0;

		const auto var = s.user_data_ptr<IMaterialVar>( 1 );
		const auto vec = var->get_vector();
		const auto vec_sz = var->get_vector_size();

		for ( int i{}; i < vec_sz; ++i )
			s.push( vec[ i ] );

		return vec_sz;
	}

	int set_vector( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::number }, { ltd::number } }, true ) )
		{
			s.error( XOR_STR( "usage: material_var:set_vector(x, y, z?, w?)" ) );
			return 0;
		}

		const auto var = s.user_data_ptr<IMaterialVar>( 1 );
		var->set_vector_component( s.get_float( 2 ), 0 );
		var->set_vector_component( s.get_float( 3 ), 1 );
		if ( s.get_stack_top() >= 4 )
			var->set_vector_component( s.get_float( 4 ), 2 );
		if ( s.get_stack_top() >= 5 )
			var->set_vector_component( s.get_float( 5 ), 3 );

		return 0;
	}

	int modulate( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::table } } ) )
		{
			s.error( XOR_STR( "usage: material:modulate(color)" ) );
			return 0;
		}

		const auto col = extract_color( s, 2 );
		const auto mat = s.user_data_ptr<IMaterial>( 1 );
		mat->AlphaModulate( col.value.w );
		mat->ColorModulate( col.value.x, col.value.y, col.value.z );
		return 0;
	}

	int set_flag( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::number }, { ltd::boolean } } ) )
		{
			s.error( XOR_STR( "usage: material:set_flag(flag, state)" ) );
			return 0;
		}

		s.user_data_ptr<IMaterial>( 1 )->SetMaterialVarFlag( ( MaterialVarFlags_t ) s.get_integer( 2 ), s.get_boolean( 3 ) );
		return 0;
	}

	int get_flag( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::number } } ) )
		{
			s.error( XOR_STR( "usage: material:get_flag(flag)" ) );
			return 0;
		}

		s.push( s.user_data_ptr<IMaterial>( 1 )->GetMaterialVarFlag( ( MaterialVarFlags_t ) s.get_integer( 2 ) ) );
		return 1;
	}

	int find_var( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: material:find_var(name)" ) );
			return 0;
		}

		uint32_t tok;
		const auto var = s.user_data_ptr<IMaterial>( 1 )->FindVarFast( s.get_string( 2 ), &tok );
		if ( !var )
			return 0;

		s.create_user_object_ptr( XOR_STR( "csgo.material_var" ), var );
		return 1;
	}

	int get_name( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data } } ) )
			return 0;

		s.push( s.user_data_ptr<IMaterial>( 1 )->GetName() );
		return 1;
	}

	int get_group( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data } } ) )
			return 0;

		s.push( s.user_data_ptr<IMaterial>( 1 )->GetTextureGroupName() );
		return 1;
	}

	int create( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::string }, { ltd::string }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: mat.create(name, type, kv)" ) );
			return 0;
		}

		const auto me = api.find_by_state( l );
		if ( !me )
		{
			s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
			return 0;
		}

		const auto kv = KeyValues::create( s.get_string( 2 ) );
		KeyValues::load_from_buffer( kv, s.get_string( 1 ), s.get_string( 3 ) );
		const auto mat = interfaces::material_system()->CreateMaterial( s.get_string( 1 ), kv );
		if ( !mat )
		{
			kv->~KeyValues();
			s.error( XOR_STR( "failed to create material" ) );
			return 0;
		}

		//IncrementReferenceCount already called by CreateMaterial
		me->add_material( mat );
		s.create_user_object_ptr( XOR_STR( "csgo.material" ), mat );
		return 1;
	}

	int find( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::string }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: mat.find(name, texture)" ) );
			return 0;
		}

		const auto me = api.find_by_state( l );
		if ( !me )
		{
			s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
			return 0;
		}

		const auto mat = interfaces::material_system()->FindMaterial( s.get_string( 1 ), s.get_string( 2 ) );
		if ( !mat )
			return 0;

		mat->IncrementReferenceCount();
		me->add_material( mat );
		s.create_user_object_ptr( XOR_STR( "csgo.material" ), mat );
		return 1;
	}

	int for_each_material( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::function } } ) )
		{
			s.error( XOR_STR( "usage: mat.for_each_material(callback)" ) );
			return 0;
		}

		const auto me = api.find_by_state( l );
		if ( !me )
		{
			s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
			return 0;
		}

		const auto fn = s.registry_add();
		for ( auto i = interfaces::material_system()->FirstMaterial(); i != interfaces::material_system()->InvalidMaterial(); i = interfaces::material_system()->NextMaterial( i ) )
		{
			const auto mat = interfaces::material_system()->GetMaterial( i );
			if ( !mat )
				continue;

			s.registry_get( fn );
			s.create_user_object_ptr( XOR_STR( "csgo.material" ), mat );
			if ( !s.call( 1, 0 ) )
			{
				me->did_error = true;
				helpers::error( XOR_STR( "runtime_error" ), s.get_last_error_description().c_str() );
				if ( const auto f = api.find_script_file( me->id ); f.has_value() )
					f->get().should_unload = true;

				return 0;
			}
		}

		s.registry_remove( fn );
		return 0;
	}

	int override_material( lua_State* l )
	{
		lua::runtime_state s( l );
		if ( !s.check_arguments( { { lua::ltd::user_data } } )
		&& !s.check_arguments( { { lua::ltd::nil } } ) )
		{
			s.error( XOR_STR( "usage: override_material(mat/nil)" ) );
			return 0;
		}

		if ( s.check_argument_type( 1, lua::ltd::nil ) )
			interfaces::model_render()->ForcedMaterialOverride();
		else
			interfaces::model_render()->ForcedMaterialOverride( s.user_data_ptr<IMaterial>( 1 ) );

		return 0;
	}
}
