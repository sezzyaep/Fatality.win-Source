#include "../include_cheat.h"

int lua::api_def::timer::start( lua_State* l )
{
	runtime_state s( l );

	if ( !s.check_arguments( {
		{ ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: tmr:start()" ) );
		return 0;
	}

	const auto obj = *reinterpret_cast< std::weak_ptr<helpers::timed_callback>* >( s.get_user_data( 1 ) );

	if ( const auto tmr = obj.lock(); tmr )
	{
		if ( !tmr )
			return 0;

		tmr->start();
	}
	return 0;
}

int lua::api_def::timer::stop( lua_State* l )
{
	runtime_state s( l );

	if ( !s.check_arguments( {
		{ ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: tmr:stop()" ) );
		return 0;
	}

	const auto obj = *reinterpret_cast< std::weak_ptr<helpers::timed_callback>* >( s.get_user_data( 1 ) );

	if ( const auto tmr = obj.lock(); tmr )
	{
		if ( !tmr )
			return 0;

		tmr->stop();
	}
	return 0;
}

int lua::api_def::timer::run_once( lua_State* l )
{
	runtime_state s( l );

	if ( !s.check_arguments( {
		{ ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: tmr:run_once()" ) );
		return 0;
	}

	const auto obj = *reinterpret_cast< std::weak_ptr<helpers::timed_callback>* >( s.get_user_data( 1 ) );

	if ( const auto tmr = obj.lock(); tmr )
	{
		if ( !tmr )
			return 0;

		tmr->run_once();
	}
	return 0;
}

int lua::api_def::timer::set_delay( lua_State* l )
{
	runtime_state s( l );

	if ( !s.check_arguments( {
		{ ltd::user_data },
		{ ltd::number } } ) )
	{
		s.error( XOR_STR( "usage: tmr:set_delay(delay)" ) );
		return 0;
	}

	const auto obj = *reinterpret_cast< std::weak_ptr<helpers::timed_callback>* >( s.get_user_data( 1 ) );

	if ( const auto tmr = obj.lock(); tmr )
	{
		if ( !tmr )
			return 0;

		tmr->set_delay( s.get_float( 2 ) );
	}
	return 0;
}

int lua::api_def::timer::is_active( lua_State* l )
{
	runtime_state s( l );

	if ( !s.check_arguments( {
		{ ltd::user_data } } ) )
	{
		s.error( XOR_STR( "usage: tmr:is_active()" ) );
		return 0;
	}

	const auto obj = *reinterpret_cast< std::weak_ptr<helpers::timed_callback>* >( s.get_user_data( 1 ) );

	if ( const auto tmr = obj.lock(); tmr )
	{
		if ( !tmr )
			return 0;

		s.push( tmr->is_active() );
		return 1;
	}
	return 0;
}