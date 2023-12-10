#include "../include_cheat.h"

namespace lua::api_def::event
{
	int index( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: event.get_..." ) );
			return 0;
		}

		switch ( fnv1a_rt( s.get_string( 2 ) ) )
		{
			case FNV1A( "get_name" ):
				s.push( get_name );
				return 1;
			case FNV1A( "get_bool" ):
				s.push( get_bool );
				return 1;
			case FNV1A( "get_int" ):
				s.push( get_int );
				return 1;
			case FNV1A( "get_float" ):
				s.push( get_float );
				return 1;
			case FNV1A( "get_string" ):
				s.push( get_string );
				return 1;
		}

		const auto ev = s.user_data_ptr<IGameEvent>( 1 );
		const auto key = ev->kv->find_key( s.get_string( 2 ) );
		if ( !key )
			return 0;

		switch ( key->data_type )
		{
			case KeyValues::TYPE_STRING:
				s.push( key->s_value );
				return 1;
			case KeyValues::TYPE_INT:
				s.push( key->i_value );
				return 1;
			case KeyValues::TYPE_FLOAT:
				s.push( key->f_value );
				return 1;
		}

		return 0;
	}
}

int lua::api_def::event::get_name( lua_State* l )
{
	runtime_state s( l );

	if ( !s.check_arguments( { { ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_name()" ) );
		return 0;
	}

	const auto ev = *reinterpret_cast< IGameEvent** >( s.get_user_data( 1 ) );
	s.push( ev->GetName() );
	return 1;
}

int lua::api_def::event::get_bool( lua_State* l )
{
	runtime_state s( l );

	if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_bool(key_name)" ) );
		return 0;
	}

	const auto ev = *reinterpret_cast< IGameEvent** >( s.get_user_data( 1 ) );
	s.push( ev->get_bool( s.get_string( 2 ) ) );
	return 1;
}

int lua::api_def::event::get_int( lua_State* l )
{
	runtime_state s( l );

	if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_int(key_name)" ) );
		return 0;
	}

	const auto ev = *reinterpret_cast< IGameEvent** >( s.get_user_data( 1 ) );
	s.push( ev->get_int( s.get_string( 2 ) ) );
	return 1;
}

int lua::api_def::event::get_float( lua_State* l )
{
	runtime_state s( l );

	if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_float(key_name)" ) );
		return 0;
	}

	const auto ev = *reinterpret_cast< IGameEvent** >( s.get_user_data( 1 ) );
	s.push( ev->get_float( s.get_string( 2 ) ) );
	return 1;
}

int lua::api_def::event::get_string( lua_State* l )
{
	runtime_state s( l );

	if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
	{
		s.error( XOR_STR( "usage: obj:get_string(key_name)" ) );
		return 0;
	}

	const auto ev = *reinterpret_cast< IGameEvent** >( s.get_user_data( 1 ) );
	s.push( ev->get_string( s.get_string( 2 ) ).c_str() );
	return 1;
}