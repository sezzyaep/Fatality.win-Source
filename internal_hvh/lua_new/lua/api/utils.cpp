#include <filesystem>

#include "../include_cheat.h"
#include <../misc/aes256.h>
#include <wininet.h>

using namespace lua;

int api_def::utils::random_int( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::number },
		{ ltd::number }
	} );

	if ( !r )
	{
		s.error( XOR( "usage: random_int(min, max)" ) );
		return 0;
	}

	s.push( globals::random_int( s.get_integer( 1 ), s.get_integer( 2 ) ) );
	return 1;
}

int api_def::utils::random_float( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::number },
		{ ltd::number }
	} );

	if ( !r )
	{
		s.error( XOR( "usage: random_float(min, max)" ) );
		return 0;
	}

	s.push( globals::random_float( s.get_float( 1 ), s.get_float( 2 ) ) );
	return 1;
}

int api_def::utils::flags( lua_State* l )
{
	runtime_state s( l );

	uint32_t result{};
	for ( auto i = 1; i <= s.get_stack_top(); ++i )
	{
		if ( s.is_number( i ) )
			result |= s.get_integer( i );
	}

	s.push( static_cast< int >( result ) );
	return 1;
}

int api_def::utils::find_interface( lua_State* l )
{
	runtime_state s( l );

	if ( !vars::lua.allow_insecure->get<bool>() )
	{
		s.error( XOR_STR( "find_interface is not available with Allow insecure disabled" ) );
		return 0;
	}

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::string }
	} );

	if ( !r )
	{
		s.error( XOR( "usage: find_interface(module, name)" ) );
		return 0;
	}

	const auto res = ( int ) helpers::get_interface( fnv1a_rt( s.get_string( 1 ) ), fnv1a_rt( s.get_string( 2 ) ) );

	if ( res == -1 )
		s.push_nil();
	else
		s.push( res );

	return 1;
}

int api_def::utils::find_pattern( lua_State* l )
{
	runtime_state s( l );

	if ( !vars::lua.allow_insecure->get<bool>() )
	{
		s.error( XOR_STR( "find_pattern is not available with Allow insecure disabled" ) );
		return 0;
	}

	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::string },
		{ ltd::number, true }
	} );

	if ( !r )
	{
		s.error( XOR( "usage: find_pattern(module, pattern, offset = 0)" ) );
		return 0;
	}

	const auto v = s.get_stack_top() >= 3 ? s.get_integer( 3 ) : 0;

	try
	{
		const auto result = helpers::find_pattern( helpers::find_module( fnv1a_rt( s.get_string( 1 ) ) ), s.get_string( 2 ), v );
		if ( result.error == helpers::scan_result::found )
		{
			s.push( ( int ) result.address );
			return 1;
		}

		switch ( result.error )
		{
			case helpers::scan_result::not_found:
				s.error( XOR_STR( "pattern: not found" ) );
				break;
			case helpers::scan_result::too_large_deref:
				s.error( util::format( XOR_STR( "pattern: too large address to get (max 4 bytes) at %d" ), result.error_pos ) );
				break;
			case helpers::scan_result::too_small_deref:
				s.error( util::format( XOR_STR( "pattern: too small address to get (min 1 byte) at %d" ), result.error_pos ) );
				break;
			case helpers::scan_result::not_a_power:
				s.error( XOR_STR( "pattern: offset is not a power of 2" ) );
				break;
			case helpers::scan_result::exception:
				s.error( XOR_STR( "pattern: exception while reading memory" ) );
				break;
			case helpers::scan_result::parse_error:
				s.error( util::format( XOR_STR( "pattern: parse error at %d" ), result.error_pos ) );
				break;
			default: break;
		}
	}
	catch ( ... )
	{
		s.error( XOR_STR( "pattern: unknown exception" ) );
	}

	return 0;
}

int api_def::utils::get_weapon_info( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::number }
	} );

	if ( !r )
	{
		s.error( XOR( "usage: get_weapon_info(item_definition_index)" ) );
		return 0;
	}

	const auto n = s.get_integer( 1 );
	if ( !n )
		return 0;

	const auto weapon_data = interfaces::weapon_system()->GetWpnData( n );
	if ( !weapon_data )
		return 0;

	s.create_table();
	{
		s.set_field( XOR( "console_name" ), weapon_data->consolename );
		s.set_field( XOR( "max_clip1" ), weapon_data->maxclip1 );
		s.set_field( XOR( "max_clip2" ), weapon_data->imaxclip2 );
		s.set_field( XOR( "world_model" ), weapon_data->szworldmodel );
		s.set_field( XOR( "view_model" ), weapon_data->szviewmodel );
		s.set_field( XOR( "weapon_type" ), weapon_data->weapontype );
		s.set_field( XOR( "weapon_price" ), weapon_data->iweaponprice );
		s.set_field( XOR( "kill_reward" ), weapon_data->ikillaward );
		s.set_field( XOR( "cycle_time" ), weapon_data->cycle_time );
		s.set_field( XOR( "is_full_auto" ), weapon_data->bfullauto );
		s.set_field( XOR( "damage" ), weapon_data->idamage );
		s.set_field( XOR( "range" ), weapon_data->range );
		s.set_field( XOR( "range_modifier" ), weapon_data->flrangemodifier );
		s.set_field( XOR( "throw_velocity" ), weapon_data->flthrowvelocity );
		s.set_field( XOR( "has_silencer" ), weapon_data->bhassilencer );
		s.set_field( XOR( "max_player_speed" ), weapon_data->flmaxplayerspeed );
		s.set_field( XOR( "max_player_speed_alt" ), weapon_data->flmaxplayerspeedalt );
		s.set_field( XOR( "zoom_fov1" ), weapon_data->zoom_fov1 );
		s.set_field( XOR( "zoom_fov2" ), weapon_data->zoom_fov2 );
		s.set_field( XOR( "zoom_levels" ), weapon_data->zoom_levels );
	}

	return 1;
}

int api_def::utils::create_timer( lua_State* l, bool run )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::number },
		{ ltd::function }
	} );

	if ( !r )
	{
		s.error( XOR( "usage: new_timer(delay, function)" ) );
		return 0;
	}

	const auto& me = lua::api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	const auto ref = s.registry_add();
	const auto tmr = std::make_shared<helpers::timed_callback>( s.get_float( 1 ), [ref, l, me] ()
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

	helpers::timers[ me->id ].emplace_back( tmr );

	if ( run )
	{
		tmr->run_once();
		return 0;
	}

	// create lua usertype
	std::weak_ptr<helpers::timed_callback> obj{ tmr };
	s.create_user_object<decltype( obj )>( XOR_STR( "utils.timer" ), &obj );

	return 1;
}

int api_def::utils::new_timer( lua_State* l )
{
	return create_timer( l );
}

int api_def::utils::run_delayed( lua_State* l )
{
	return create_timer( l, true );
}

int api_def::utils::world_to_screen( lua_State* l )
{
	runtime_state s( l );

	const auto r = s.check_arguments( {
		{ ltd::number },
		{ ltd::number },
		{ ltd::number },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: world_to_screen(x, y, z): x, y | nil" ) );
		return 0;
	}

	Vector in{ s.get_float( 1 ), s.get_float( 2 ), s.get_float( 3 ) }, out{};
	if ( !::math::world_to_screen( in, out ) )
		return 0;

	s.push( out.x );
	s.push( out.y );
	return 2;
}

int api_def::utils::get_rtt( lua_State* l )
{
	runtime_state s( l );

	if ( !interfaces::engine()->IsInGame() )
	{
		s.push( 0.f );
		return 0;
	}

	s.push( misc::get_latency() );
	return 1;
}

int api_def::utils::get_time( lua_State* l )
{
	runtime_state s( l );

	const auto time = std::time( nullptr );
	tm tm;
	localtime_s( &tm, &time );

	s.create_table();
	s.set_field( XOR( "sec" ), tm.tm_sec );
	s.set_field( XOR( "min" ), tm.tm_min );
	s.set_field( XOR( "hour" ), tm.tm_hour );
	s.set_field( XOR( "month_day" ), tm.tm_mday );
	s.set_field( XOR( "month" ), tm.tm_mon + 1 );
	s.set_field( XOR( "year" ), tm.tm_year + 1900 );
	s.set_field( XOR( "week_day" ), tm.tm_wday + 1 );
	s.set_field( XOR( "year_day" ), tm.tm_yday + 1 );

	return 1;
}

int api_def::utils::http_get( lua_State* l )
{
	runtime_state s( l );
	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::string },
		{ ltd::function },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: utils.http_get(url, headers, callback(response))" ) );
		return 0;
	}

	if ( !vars::lua.allow_insecure->get<bool>() )
	{
		s.error( XOR_STR( "http_get is not available with Allow insecure disabled" ) );
		return 0;
	}

	const auto me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	const auto url = helpers::parse_uri( s.get_string( 1 ) );
	const auto headers = std::string( s.get_string( 2 ) );
	const auto cbk = s.registry_add();

	if ( !url )
	{
		s.error( XOR_STR( "invalid url specified" ) );
		return 0;
	}

	// run this in a separate thread to avoid blocking.
	std::thread( [l, cbk, url, headers, me] ()
	{
		const auto inet = InternetOpenA( XOR_STR( "fatality" ), INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0 );
		if ( !inet )
			return;

		const auto conn = InternetConnectA( inet, url->host.c_str(), url->port, nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 1 );
		if ( !conn )
		{
			InternetCloseHandle( inet );
			return;
		}

		const auto req = HttpOpenRequestA( conn, XOR_STR( "GET" ), url->path.c_str(),
			nullptr, nullptr, nullptr, INTERNET_FLAG_KEEP_CONNECTION | ( url->is_secure ? INTERNET_FLAG_SECURE : 0 ), 1 );
		if ( !req )
		{
			InternetCloseHandle( conn );
			InternetCloseHandle( inet );
			return;
		}

		runtime_state s( l );
		if ( HttpSendRequestA( req, headers.c_str(), headers.size(), nullptr, 0 ) )
		{
			std::string result;

			char buffer[ 1024 ]{}; unsigned long bytes_read{};
			while ( InternetReadFile( req, buffer, 1024, &bytes_read ) && bytes_read )
			{
				result += std::string( buffer, bytes_read );
				memset( buffer, 0, 1024 );
			}

			InternetCloseHandle( req );
			InternetCloseHandle( conn );
			InternetCloseHandle( inet );

			s.registry_get( cbk );
			s.push( result );
			if ( !s.call( 1, 0 ) )
			{
				me->did_error = true;
				lua::helpers::error( XOR_STR( "runtime_error" ), s.get_last_error_description().c_str() );
				if ( const auto f = api.find_script_file( me->id ); f.has_value() )
					f->get().should_unload = true;

				return;
			}
		}

		s.registry_remove( cbk );
	} ).detach();

	return 0;
}

int api_def::utils::http_post( lua_State* l )
{
	runtime_state s( l );
	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::string },
		{ ltd::string },
		{ ltd::function },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: utils.http_post(url, headers, body, callback(response))" ) );
		return 0;
	}

	if ( !vars::lua.allow_insecure->get<bool>() )
	{
		s.error( XOR_STR( "http_post is not available with Allow insecure disabled" ) );
		return 0;
	}

	const auto me = api.find_by_state( l );
	if ( !me )
	{
		s.error( XOR_STR( "FATAL: could not find the script. Did it escape the sandbox?" ) );
		return 0;
	}

	const auto url = helpers::parse_uri( s.get_string( 1 ) );
	const auto headers = std::string( s.get_string( 2 ) );
	const auto body = std::string( s.get_string( 3 ) );
	const auto cbk = s.registry_add();

	if ( !url )
	{
		s.error( XOR_STR( "invalid url specified" ) );
		return 0;
	}

	// run this in a separate thread to avoid blocking.
	std::thread( [l, cbk, url, headers, body, me] ()
	{
		const auto inet = InternetOpenA( XOR_STR( "fatality" ), INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0 );
		if ( !inet )
			return;

		const auto conn = InternetConnectA( inet, url->host.c_str(), url->port, nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 1 );
		if ( !conn )
		{
			InternetCloseHandle( inet );
			return;
		}

		const auto req = HttpOpenRequestA( conn, XOR_STR( "POST" ), url->path.c_str(),
		nullptr, nullptr, nullptr, INTERNET_FLAG_KEEP_CONNECTION | ( url->is_secure ? INTERNET_FLAG_SECURE : 0 ), 1 );
		if ( !req )
		{
			InternetCloseHandle( conn );
			InternetCloseHandle( inet );
			return;
		}

		runtime_state s( l );
		if ( HttpSendRequestA( req, headers.c_str(), headers.size(), ( void* ) body.c_str(), body.size() ) )
		{
			std::string result;

			char buffer[ 1024 ]{}; unsigned long bytes_read{};
			while ( InternetReadFile( req, buffer, 1024, &bytes_read ) && bytes_read )
			{
				result += std::string( buffer, bytes_read );
				memset( buffer, 0, 1024 );
			}

			InternetCloseHandle( req );
			InternetCloseHandle( conn );
			InternetCloseHandle( inet );

			s.registry_get( cbk );
			s.push( result );
			if ( !s.call( 1, 0 ) )
			{
				me->did_error = true;
				lua::helpers::error( XOR_STR( "runtime_error" ), s.get_last_error_description().c_str() );
				if ( const auto f = api.find_script_file( me->id ); f.has_value() )
					f->get().should_unload = true;

				return;
			}
		}

		s.registry_remove( cbk );
	} ).detach();

	return 0;
}

int api_def::utils::json_encode( lua_State* l )
{
	runtime_state s( l );
	const auto r = s.check_arguments( {
		{ ltd::table }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: utils.json_encode(table)" ) );
		return 0;
	}

	const auto res = helpers::parse_table( l, 1 );
	s.push( res.dump().c_str() );
	return 1;
}

int api_def::utils::json_decode( lua_State* l )
{
	runtime_state s( l );
	const auto r = s.check_arguments( {
		{ ltd::string }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: utils.json_decode(json)" ) );
		return 0;
	}

	auto data = nlohmann::json::parse( s.get_string( 1 ), nullptr, false );
	if ( data.is_discarded() || data.is_null() )
	{
		s.error( XOR_STR( "invalid json string" ) );
		return 0;
	}
	return helpers::load_table( l, data );
}

int api_def::utils::set_clan_tag( lua_State* l )
{
	runtime_state s( l );
	const auto r = s.check_arguments( {
		{ ltd::string }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: utils.set_clan_tag(tag)" ) );
		return 0;
	}

	reinterpret_cast< int( __fastcall* )( const char*, const char* ) >( make_offset_multi( "engine.dll", sig_set_clantag ) )( s.get_string( 1 ), s.get_string( 1 ) );
	return 0;
}


int api_def::utils::trace( lua_State* l )
{
	runtime_state s( l );
	const auto r = s.check_arguments( {
		{ ltd::user_data },
		{ ltd::user_data },
		{ ltd::number }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: utils.trace(from, to, skip_ent_index): trace_t" ) );
		return 0;
	}

	if ( !interfaces::engine()->IsInGame() )
	{
		s.error( XOR_STR( "attempted to call trace while not in game" ) );
		return 0;
	}

	const auto from = s.user_data<Vector>( 1 );
	const auto to = s.user_data<Vector>( 2 );
	const auto skip_ent_index = s.get_integer( 3 );

	CTraceFilterSimple filter1( globals::get_player( skip_ent_index ) );
	CTraceFilterSimpleNoPlayers filter2( nullptr, 0 );
	CTraceFilter* filter = &filter1;
	if ( skip_ent_index == -1 )
		filter = &filter2;
	trace_t res;
	Ray_t ray;
	ray.Init( from, to );
	interfaces::engine_trace()->TraceRay( ray, MASK_SHOT, filter, &res );

	s.create_table();
	{
		s.create_user_object<decltype( res.endpos )>( XOR_STR( "vec3" ), &res.endpos );
		s.set_field( XOR( "endpos" ) );
		s.set_field( XOR( "fraction" ), res.fraction );
		s.set_field( XOR( "allsolid" ), res.allsolid );
		s.set_field( XOR( "startsolid" ), res.startsolid );
		s.set_field( XOR( "fractionleftsolid" ), res.fractionleftsolid );
		s.create_user_object<decltype( res.plane.normal )>( XOR_STR( "vec3" ), &res.plane.normal );
		s.set_field( XOR( "plane_normal" ) );
		s.set_field( XOR( "plane_dist" ), res.plane.dist );
		s.set_field( XOR( "contents" ), res.contents );
		s.set_field( XOR( "disp_flags" ), res.dispFlags );
		s.set_field( XOR( "surface_name" ), res.surface.name );
		s.set_field( XOR( "surface_props" ), res.surface.surfaceProps );
		s.set_field( XOR( "surface_flags" ), res.surface.flags );
		s.set_field( XOR( "ent_index" ), res.m_pEnt ? res.m_pEnt->EntIndex() : -1 );
		s.set_field( XOR( "hitbox" ), res.hitbox );
		s.set_field( XOR( "hitgroup" ), res.hitgroup );
	}

	return 1;
}

int api_def::utils::trace_bullet( lua_State* l )
{
	runtime_state s( l );
	const auto r = s.check_arguments( {
		{ ltd::number },
		{ ltd::user_data },
		{ ltd::user_data }
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: utils.trace_bullet(item_definition_index, from, to): damage, trace_t" ) );
		return 0;
	}

	if ( !interfaces::engine()->IsInGame() )
	{
		s.error( XOR_STR( "attempted to call trace_bullet while not in game" ) );
		return 0;
	}

	const auto weapon_id = s.get_integer( 1 );
	const auto from = s.user_data<Vector>( 2 );
	const auto to = s.user_data<Vector>( 3 );

	trace_t res{};
	int damage{};
	penetration::can_hit( weapon_id, from, to, damage, res );

	s.push( damage );
	s.create_table();
	{
		s.create_user_object<decltype( res.endpos )>( XOR_STR( "vec3" ), &res.endpos );
		s.set_field( XOR( "endpos" ) );
		s.set_field( XOR( "fraction" ), res.fraction );
		s.set_field( XOR( "allsolid" ), res.allsolid );
		s.set_field( XOR( "startsolid" ), res.startsolid );
		s.set_field( XOR( "fractionleftsolid" ), res.fractionleftsolid );
		s.create_user_object<decltype( res.plane.normal )>( XOR_STR( "vec3" ), &res.plane.normal );
		s.set_field( XOR( "plane_normal" ) );
		s.set_field( XOR( "plane_dist" ), res.plane.dist );
		s.set_field( XOR( "contents" ), res.contents );
		s.set_field( XOR( "disp_flags" ), res.dispFlags );
		s.set_field( XOR( "surface_name" ), res.surface.name );
		s.set_field( XOR( "surface_props" ), res.surface.surfaceProps );
		s.set_field( XOR( "surface_flags" ), res.surface.flags );
		s.set_field( XOR( "ent_index" ), res.m_pEnt ? res.m_pEnt->EntIndex() : -1 );
		s.set_field( XOR( "hitbox" ), res.hitbox );
		s.set_field( XOR( "hitgroup" ), res.hitgroup );
	}

	return 2;
}

int api_def::utils::scale_damage( lua_State* l )
{
	runtime_state s( l );
	const auto r = s.check_arguments( {
		{ ltd::number },
		{ ltd::number },
		{ ltd::number },
		{ ltd::number },
		{ ltd::boolean },
		{ ltd::boolean },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: utils.scale_damage(damage, item_definition_index, hit_group, armor, heavy_armor, helmet): damage" ) );
		return 0;
	}

	const auto damage = s.get_integer( 1 );
	const auto weapon_id = s.get_integer( 2 );
	const auto hit_group = s.get_integer( 3 );
	const auto armor = s.get_float( 4 );
	const auto heavy_armor = s.get_boolean( 5 );
	const auto helmet = s.get_boolean( 6 );

	auto current_damage = static_cast< float >( damage );
	penetration::scale_damage( weapon_id, hit_group, armor, heavy_armor, helmet, current_damage );

	s.push( static_cast< int >( current_damage ) );
	return 1;
}

int api_def::utils::load_file( lua_State* l )
{
	runtime_state s( l );
	const auto r = s.check_arguments( {
		{ ltd::string },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: utils.load_file(path): string" ) );
		return 0;
	}

	const auto sz = GetCurrentDirectoryA( 0, nullptr );
	std::string path( " ", sz );

	GetCurrentDirectoryA( sz, path.data() );
	path.pop_back(); // get rid of null terminator
	path += xorstr_( "\\" );
	path += s.get_string( 1 );

	if ( !std::filesystem::is_regular_file( path ) )
	{
		s.error( XOR_STR( "could not open file" ) );
		return 0;
	}

	std::ifstream file( path, std::ios::in | std::ios::binary );
	if ( !file.is_open() )
		return {};

	const std::string content{ std::istreambuf_iterator<char>( file ), std::istreambuf_iterator<char>() };

	file.close();

	s.push( content );
	return 1;
}

int api_def::utils::print_console( lua_State* l )
{
	runtime_state s( l );
	const auto r = s.check_arguments( {
		{ ltd::string },
		{ ltd::table, true },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: utils.print_console(string, color = white)" ) );
		return 0;
	}

	auto col = color::white();
	if ( s.get_stack_top() == 2 )
		col = lua::extract_color( s, 2 );

	interfaces::cvar()->ConsoleColorPrintf( Color( col ), s.get_string( 1 ) );
	return 0;
}

int api_def::utils::print_dev_console( lua_State* l )
{
	runtime_state s( l );
	const auto r = s.check_arguments( {
		{ ltd::string },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: utils.print_dev_console(string)" ) );
		return 0;
	}

	console_log::m_in_print = true;
	reinterpret_cast< void( __stdcall* )( const Color&, char const* ) >( make_offset_multi( "engine.dll", sig_print_dev_console ) )( Color( 255, 255, 255, 255 ), s.get_string( 1 ) );
	console_log::m_in_print = false;
	return 0;
}

int api_def::utils::error_print( lua_State* l )
{
	runtime_state s( l );
	const auto r = s.check_arguments( {
		{ ltd::string },
	} );

	if ( !r )
	{
		s.error( XOR_STR( "usage: utils.error_print(string)" ) );
		return 0;
	}

	lua::helpers::error( xorstr_( "error" ), s.get_string( 1 ) );
	EMIT_ERROR_SOUND();
	return 0;
}

namespace lua::api_def::utils
{
	int aes256_encrypt( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::string }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: utils.aes256_encrypt(key, data)" ) );
			return 0;
		}

		std::string lua_key{ s.get_string( 1 ) }, lua_data{ s.get_string( 2 ) };
		std::vector<uint8_t> key, data, enc;
		key.resize( lua_key.size() );
		data.resize( lua_data.size() );
		std::copy( lua_key.begin(), lua_key.end(), key.begin() );
		std::copy( lua_data.begin(), lua_data.end(), data.begin() );

		Aes256::encrypt( key, data, enc );

		std::string enc_str;
		enc_str.resize( enc.size() );
		std::copy( enc.begin(), enc.end(), enc_str.begin() );

		s.push( enc_str );
		return 1;
	}

	int aes256_decrypt( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::string }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: utils.aes256_decrypt(key, data)" ) );
			return 0;
		}

		std::string lua_key{ s.get_string( 1 ) }, lua_data{ s.get_string( 2 ) };
		std::vector<uint8_t> key, data, dec;
		key.resize( lua_key.size() );
		data.resize( lua_data.size() );
		std::copy( lua_key.begin(), lua_key.end(), key.begin() );
		std::copy( lua_data.begin(), lua_data.end(), data.begin() );

		Aes256::decrypt( key, data, dec );

		std::string dec_str;
		dec_str.resize( dec.size() );
		std::copy( dec.begin(), dec.end(), dec_str.begin() );

		s.push( dec_str );
		return 1;
	}

	int base64_encode( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: utils.base64_encode(data)" ) );
			return 0;
		}

		s.push( util::base64_encode( s.get_string( 1 ) ) );
		return 1;
	}

	int base64_decode( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: utils.base64_decode(data)" ) );
			return 0;
		}

		try
		{
			s.push( util::base64_decode( s.get_string( 1 ) ) );
			return 1;
		}
		catch ( const std::exception& e )
		{
			s.error( e.what() );
			return 0;
		}
	}

	int get_unix_time( lua_State* l )
	{
		runtime_state s( l );
		s.push( ( double ) util::get_unix_time() );
		return 1;
	}
}