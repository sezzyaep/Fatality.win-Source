#include <filesystem>

#include "../include_cheat.h"

namespace lua::api_def::fs
{
	int read( lua_State* l )
	{
		runtime_state s( l );
		if ( !vars::lua.allow_insecure->get<bool>() )
		{
			s.error( XOR_STR( "read is not available with Allow insecure disabled" ) );
			return 0;
		}

		if ( !s.check_arguments( { { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: fs.read(path)" ) );
			return 0;
		}

		std::ifstream file( s.get_string( 1 ), std::ios::binary );
		if ( !file.is_open() )
		{
			s.error( XOR_STR( "failed to open file" ) );
			return 0;
		}

		std::string contents;
		file.seekg( 0, std::ios::end );
		contents.resize( file.tellg() );
		file.seekg( 0, std::ios::beg );
		file.read( &contents[ 0 ], contents.size() );
		file.close();

		s.push( contents );
		return 1;
	}

	int read_stream( lua_State* l )
	{
		runtime_state s( l );
		if ( !vars::lua.allow_insecure->get<bool>() )
		{
			s.error( XOR_STR( "read_stream is not available with Allow insecure disabled" ) );
			return 0;
		}

		if ( !s.check_arguments( { { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: fs.read_stream(path)" ) );
			return 0;
		}

		std::ifstream file( s.get_string( 1 ), std::ios::binary );
		if ( !file.is_open() )
		{
			s.error( XOR_STR( "failed to open file" ) );
			return 0;
		}

		// read to vector
		std::vector<char> contents;
		file.seekg( 0, std::ios::end );
		contents.resize( file.tellg() );
		file.seekg( 0, std::ios::beg );
		file.read( &contents[ 0 ], contents.size() );
		file.close();

		s.create_table();
		for ( int i{}; i < contents.size(); ++i )
			s.set_i( i + 1, contents[ i ] );

		return 1;
	}

	int write( lua_State* l )
	{
		runtime_state s( l );
		if ( !vars::lua.allow_insecure->get<bool>() )
		{
			s.error( XOR_STR( "write is not available with Allow insecure disabled" ) );
			return 0;
		}

		if ( !s.check_arguments( { { ltd::string }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: fs.write(path, data)" ) );
			return 0;
		}

		std::ofstream file( s.get_string( 1 ), std::ios::binary );
		if ( !file.is_open() )
		{
			s.error( XOR_STR( "failed to open file" ) );
			return 0;
		}

		file << s.get_string( 2 );
		file.close();

		return 0;
	}

	int write_stream( lua_State* l )
	{
		runtime_state s( l );
		if ( !vars::lua.allow_insecure->get<bool>() )
		{
			s.error( XOR_STR( "write_stream is not available with Allow insecure disabled" ) );
			return 0;
		}

		if ( !s.check_arguments( { { ltd::string }, { ltd::table } } ) )
		{
			s.error( XOR_STR( "usage: fs.write_stream(path, data)" ) );
			return 0;
		}

		std::ofstream file( s.get_string( 1 ), std::ios::binary );
		if ( !file.is_open() )
		{
			s.error( XOR_STR( "failed to open file" ) );
			return 0;
		}

		const auto bytes = s.table_to_int_array( 2 );
		for ( const auto& byte : bytes )
		{
			if ( ( uint32_t ) byte > 255 )
			{
				s.error( XOR_STR( "invalid byte value" ) );
				return 0;
			}

			file << ( char ) byte;
		}

		file.close();
		return 0;
	}

	int remove( lua_State* l )
	{
		runtime_state s( l );
		if ( !vars::lua.allow_insecure->get<bool>() )
		{
			s.error( XOR_STR( "remove is not available with Allow insecure disabled" ) );
			return 0;
		}

		if ( !s.check_arguments( { { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: fs.remove(path)" ) );
			return 0;
		}

		std::filesystem::remove_all( s.get_string( 1 ) );
		return 0;
	}

	int exists( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: fs.exists(path)" ) );
			return 0;
		}

		s.push( std::filesystem::exists( s.get_string( 1 ) ) );
		return 1;
	}

	int is_file( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: fs.is_file(path)" ) );
			return 0;
		}

		s.push( std::filesystem::is_regular_file( s.get_string( 1 ) ) );
		return 1;
	}

	int is_dir( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: fs.is_dir(path)" ) );
			return 0;
		}

		s.push( std::filesystem::is_directory( s.get_string( 1 ) ) );
		return 1;
	}

	int create_dir( lua_State* l )
	{
		runtime_state s( l );
		if ( !vars::lua.allow_insecure->get<bool>() )
		{
			s.error( XOR_STR( "create_dir is not available with Allow insecure disabled" ) );
			return 0;
		}

		if ( !s.check_arguments( { { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: fs.create_dir(path)" ) );
			return 0;
		}

		std::filesystem::create_directories( s.get_string( 1 ) );
		return 0;
	}
}
