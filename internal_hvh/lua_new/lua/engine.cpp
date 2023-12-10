#include <filesystem>
#include "../include_cheat.h"
#include <fstream>

lua::engine::engine()
{
	selected_load = new value();
	selected_unload = new value();
}

void lua::engine::refresh_scripts()
{
	std::lock_guard lock( list_mutex );

	if ( !std::filesystem::exists( XOR_STR( "fatality/scripts" ) ) )
		std::filesystem::create_directories( XOR_STR( "fatality/scripts" ) );
	if ( !std::filesystem::exists( XOR_STR( "fatality/scripts/lib" ) ) )
		std::filesystem::create_directories( XOR_STR( "fatality/scripts/lib" ) );

	// remove scripts that are no longer in the list or are marked for unload
	std::lock_guard _lock( access_mutex );
	for ( const auto& script : scripts )
	{
		const auto p = std::find_if( script_files.begin(), script_files.end(), [&] ( const script_file& f )
		{
			return f.make_id() == script.first;
		} );

		if ( p != script_files.end() )
		{
			if ( p->should_unload )
				scripts[ script.first ]->unload();

			continue;
		}

		scripts[ script.first ]->unload();
	}

	script_files.clear();
	loaded_scripts.clear();
	listed_scripts.clear();

	// TODO: template this?
	for ( auto& f : std::filesystem::directory_iterator( XOR_STR( "fatality/scripts" ) ) )
	{
		if ( f.is_directory() || f.path().extension() != XOR_STR( ".lua" ) )
			continue;

		script_file file{
			st_script,
			f.path().filename().replace_extension( XOR_STR( "" ) ).string()
		};

		try
		{
			file.parse_metadata();
			script_files.emplace_back( file );
		}
		catch ( ... )
		{
			continue;
		}

		const auto id = file.make_id();
		if ( auto find_res = scripts.find( id ); find_res != scripts.end() )
		{
			if ( find_res->second->is_running )
				loaded_scripts.emplace_back( _w2( file.name ) );
			else
				listed_scripts.emplace_back( _w2( file.name ) );
		}
		else
			listed_scripts.emplace_back( _w2( file.name ) );
	}

	for ( auto& f : std::filesystem::directory_iterator( XOR_STR( "fatality/scripts/lib" ) ) )
	{
		if ( f.is_directory() || f.path().extension() != XOR_STR( ".lua" ) )
			continue;

		script_file file{
			st_library,
			f.path().filename().replace_extension( XOR_STR( "" ) ).string()
		};

		try
		{
			file.parse_metadata();
			script_files.emplace_back( file );
		}
		catch ( ... )
		{
			continue;
		}
	}

	if ( !std::filesystem::exists( XOR_STR( "fatality/autoload" ) ) )
		return;

	std::ifstream al_file( XOR_STR( "fatality/autoload" ), std::ios::binary );
	if ( !al_file.is_open() )
		return;

	while ( !al_file.eof() )
	{
		uint32_t alh{};
		al_file.read( reinterpret_cast< char* >( &alh ), sizeof( alh ) );
		autoload.emplace_back( alh );
	}
}

void lua::engine::run_autoload()
{
	for ( const auto& f_id : autoload )
	{
		const auto file = std::find_if( script_files.begin(), script_files.end(), [f_id] ( const script_file& f )
		{
			return f.make_id() == f_id;
		} );

		if ( file != script_files.end() )
			run_script( *file, false );
	}
}

void lua::engine::enable_autoload( const script_file& file )
{
	if ( std::find( autoload.begin(), autoload.end(), file.make_id() ) != autoload.end() )
		return;

	autoload.emplace_back( file.make_id() );
	write_autoload();
}

void lua::engine::disable_autoload( const script_file& file )
{
	if ( const auto f = std::find( autoload.begin(), autoload.end(), file.make_id() ); f != autoload.end() )
		autoload.erase( f );

	write_autoload();
}

bool lua::engine::is_autoload_enabled( const script_file& file )
{
	return std::find( autoload.begin(), autoload.end(), file.make_id() ) != autoload.end();
}

void lua::engine::write_autoload()
{
	if ( std::filesystem::exists( XOR_STR( "fatality/autoload" ) ) )
		std::filesystem::remove( XOR_STR( "fatality/autoload" ) );

	std::ofstream al_file( XOR_STR( "fatality/autoload" ), std::ios::binary );
	if ( !al_file.is_open() )
		return;

	// eliminate same scripts from the list
	std::vector<uint32_t> fixed_list{};
	for ( const auto& alh : autoload )
	{
		if ( std::find( fixed_list.begin(), fixed_list.end(), alh ) == fixed_list.end() )
			fixed_list.emplace_back( alh );
	}

	autoload = fixed_list;
	for ( const auto& alh : autoload )
		al_file.write( reinterpret_cast< const char* >( &alh ), sizeof( alh ) );
}

bool lua::engine::run_script( const script_file& file, bool sounds )
{
	if ( file.type == st_library )
		return false;

	std::lock_guard _lock( access_mutex );

	// make sure file still exists
	if ( !std::filesystem::exists( file.get_file_path() ) )
	{
		lua::helpers::error( XOR_STR( "Script error" ), XOR_STR( "File not found." ) );

		return false;
	}

	// delete previously running script
	const auto id = file.make_id();
	if ( scripts.find( id ) != scripts.end() )
		scripts.erase( id );

	// create script (lua will auto-initialize)
	const auto s = std::make_shared<script>();
	s->name = file.name;
	s->id = id;
	s->type = file.type;
	s->file = file.get_file_path();

	// add to the list
	scripts[ s->id ] = s;

	// init the script
	if ( !s->initialize() || !s->call_main() )
	{
		// show the error message to user
		lua::helpers::error( XOR_STR( "Script error" ), XOR_STR( "Unable to initialize script." ) );

		s->is_running = false;
		s->unload();

		scripts.erase( s->id );
		return false;
	}

	// mark as running
	s->is_running = true;

	if ( sounds )
		EMIT_SUCCESS_SOUND();

	return true;
}

void lua::engine::stop_script( const script_file& file )
{
	std::lock_guard _lock( access_mutex );

	// "unload" script (lua state will be auto-erased)
	const auto id = file.make_id();
	if ( const auto s = scripts.find( id ); s != scripts.end() )
	{
		if ( !s->second->did_error )
			s->second->call_forward( FNV1A( "on_shutdown" ) );
		s->second->is_running = false;
		s->second->unload();
		scripts.erase( id );
	}
}

void lua::engine::for_each_script_name( const std::function<void( script_file& )>& fn )
{
	for ( auto& f : script_files )
	{
		if ( f.type != st_library )
			fn( f );
	}
}

std::optional<std::reference_wrapper<lua::script_file>> lua::engine::find_script_file( uint32_t id )
{
	const auto p = std::ranges::find_if( script_files, [id] ( const script_file& f )
	{
		return f.make_id() == id;
	} );

	if ( p != script_files.end() )
		return std::ref( *p );

	return {};
}

void lua::engine::on_con_command( const std::string& params )
{
	script script;
	script.name = _r( "con_command" );
	script.id = FNV1A( "con_command" ) + fnv1a_rt( params.c_str() );
	script.type = st_script;

	if ( !script.initialize( false ) )
	{
		lua::helpers::error( XOR_STR( "Script error" ), XOR_STR( "Unable to initialize script." ) );
		return;
	}

	if ( !script.l.load_string( params.c_str() ) )
	{
		lua::helpers::error( XOR_STR( "syntax_error" ), script.l.get_last_error_description().c_str() );
	}
	else
	{
		if ( !script.call_main() )
		{
			lua::helpers::error( XOR_STR( "Script error" ), XOR_STR( "Unable to initialize script." ) );
			return;
		}
	}
}

void lua::engine::callback( uint32_t id, const std::function<int( state& )>& arg_callback, int ret, const std::function<void( state& )>& callback )
{
	try
	{
		std::lock_guard _lock( access_mutex );
		for ( const auto& [_, s] : scripts )
		{
			if ( s->is_running )
				s->call_forward( id, arg_callback, ret, callback );
		}
	}
	catch ( ... ) {}
}

void lua::engine::create_callback( const char* n )
{
	std::lock_guard _lock( access_mutex );
	for ( const auto& [_, s] : scripts )
	{
		if ( !s->has_forward( fnv1a_rt( n ) ) )
			s->create_forward( n );
	}
}

void lua::engine::stop_all()
{
	std::lock_guard _lock( access_mutex );
	scripts.clear();
}

void lua::engine::run_timers()
{
	std::lock_guard _lock( access_mutex );
	for ( const auto& [_, s] : scripts )
	{
		if ( s->is_running )
			s->run_timers();
	}
}

lua::script_t lua::engine::find_by_state( lua_State* state )
{
	for ( const auto& [_, s] : scripts )
	{
		if ( s->l.get_state() == state )
			return s;
	}

	return nullptr;
}

lua::script_t lua::engine::find_by_id( uint32_t id )
{
	for ( const auto& [_, s] : scripts )
	{
		if ( _ == id )
			return s;
	}

	return nullptr;
}

uint32_t lua::script_file::make_id() const
{
	return fnv1a_rt( name.c_str() ) ^ type;
}

std::string lua::script_file::get_file_path() const
{
	std::string base_path{ XOR_STR( "fatality/" ) };
	switch ( type )
	{
		case st_script:
			base_path += XOR_STR( "scripts/" );
			break;
		case st_remote:
			base_path += XOR_STR( "scripts/remote/" );
			break;
		case st_library:
			base_path += XOR_STR( "scripts/lib/" );
			break;
		default:
			return XOR_STR( "" );
	}

	return base_path + name + XOR_STR( ".lua" );
}

void lua::script_file::parse_metadata()
{
	std::ifstream file( get_file_path() );
	if ( !file.is_open() )
		return;

	auto runs = 4;

	std::string line{};
	while ( std::getline( file, line ) )
	{
		if ( runs-- == 0 )
			break;

		// check our own shebang notation
		if ( line.find( XOR_STR( "--." ) ) != 0 )
			continue;

		// remove shebang
		line = line.erase( 0, 3 );

		// split in parts
		const auto parts = util::split( line, XOR_STR( " " ) );
		if ( parts.empty() )
			continue;

		const auto item = fnv1a_rt( parts[ 0 ].c_str() );
		switch ( item )
		{
			case FNV1A( "name" ):
				metadata.name = line.size() > 4 ? line.substr( 5 ) : std::string();
				break;
			case FNV1A( "description" ):
				metadata.description = line.size() > 11 ? line.substr( 12 ) : std::string();
				break;
			case FNV1A( "author" ):
				metadata.author = line.size() > 6 ? line.substr( 7 ) : std::string();
				break;
			default: break;
		}
	}
}