#include <filesystem>

#include "../include_cheat.h"

#undef min
#undef max
#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#include <../misc/zip_file.hpp>

namespace lua::api_def::zip
{
	using namespace miniz_cpp;

	int read( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: zip:read(path)" ) );
			return 0;
		}

		const auto z = s.user_data_ptr<zip_file>( 1 );
		if ( !z->has_file( s.get_string( 2 ) ) )
		{
			s.error( XOR_STR( "file not found" ) );
			return 0;
		}

		s.push( z->read( s.get_string( 2 ) ) );
		return 1;
	}

	int read_stream( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: zip:read_stream(path)" ) );
			return 0;
		}

		const auto z = s.user_data_ptr<zip_file>( 1 );
		if ( !z->has_file( s.get_string( 2 ) ) )
		{
			s.error( XOR_STR( "file not found" ) );
			return 0;
		}

		const auto f = z->read( s.get_string( 2 ) );
		s.create_table();
		for ( int i{}; i < f.size(); ++i )
			s.set_i( i + 1, f[ i ] );

		return 1;
	}

	int write( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: zip:write(path, data)" ) );
			return 0;
		}

		s.user_data_ptr<zip_file>( 1 )->write( s.get_string( 2 ), s.get_string( 3 ) );
		return 0;
	}

	int write_stream( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string }, { ltd::table } } ) )
		{
			s.error( XOR_STR( "usage: zip:write_stream(path, data)" ) );
			return 0;
		}

		std::string data;
		for ( const auto& b : s.table_to_int_array( 3 ) )
		{
			if ( ( uint32_t ) b > 255 )
			{
				s.error( XOR_STR( "invalid byte value" ) );
				return 0;
			}

			data += ( char ) b;
		}

		s.user_data_ptr<zip_file>( 1 )->write( s.get_string( 2 ), data );
		return 0;
	}

	int save( lua_State* l )
	{
		runtime_state s( l );
		if ( !vars::lua.allow_insecure->get<bool>() )
		{
			s.error( XOR_STR( "save is not available with Allow insecure disabled" ) );
			return 0;
		}

		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: zip:save(path)" ) );
			return 0;
		}

		s.user_data_ptr<zip_file>( 1 )->save( s.get_string( 2 ) );
		return 0;
	}

	int get_files( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data } } ) )
		{
			s.error( XOR_STR( "usage: zip:get_files()" ) );
			return 0;
		}

		s.create_table(); int n{ 1 };

		const auto z = s.user_data_ptr<zip_file>( 1 );
		for ( const auto& i : z->infolist() )
		{
			s.create_table();
			s.set_field( XOR_STR( "filename" ), i.filename.c_str() );
			{
				s.create_table();
				s.set_field( XOR_STR( "year" ), i.date_time.year );
				s.set_field( XOR_STR( "month" ), i.date_time.month );
				s.set_field( XOR_STR( "day" ), i.date_time.day );
				s.set_field( XOR_STR( "hours" ), i.date_time.hours );
				s.set_field( XOR_STR( "minutes" ), i.date_time.minutes );
				s.set_field( XOR_STR( "seconds" ), i.date_time.seconds );
				s.set_field( XOR_STR( "date_time" ) );
			}
			s.set_field( XOR_STR( "comment" ), i.comment.c_str() );
			s.set_field( XOR_STR( "compress_size" ), ( int ) i.compress_size );
			s.set_field( XOR_STR( "file_size" ), ( int ) i.file_size );
			s.set_i( n++ );
		}

		return 1;
	}

	int exists( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: zip:exists(path)" ) );
			return 0;
		}

		s.push( s.user_data_ptr<zip_file>( 1 )->has_file( s.get_string( 2 ) ) );
		return 1;
	}

	int extract( lua_State* l )
	{
		runtime_state s( l );
		if ( !vars::lua.allow_insecure->get<bool>() )
		{
			s.error( XOR_STR( "extract is not available with Allow insecure disabled" ) );
			return 0;
		}

		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: zip:extract(path, dest)" ) );
			return 0;
		}

		const auto z = s.user_data_ptr<zip_file>( 1 );
		if ( !z->has_file( s.get_string( 2 ) ) || !std::filesystem::exists( s.get_string( 3 ) ) )
		{
			s.error( XOR_STR( "path or dest does not exist" ) );
			return 0;
		}

		z->extract( s.get_string( 2 ), s.get_string( 3 ) );
		return 0;
	}

	int extract_all( lua_State* l )
	{
		runtime_state s( l );
		if ( !vars::lua.allow_insecure->get<bool>() )
		{
			s.error( XOR_STR( "extract_all is not available with Allow insecure disabled" ) );
			return 0;
		}

		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: zip:extract_all(path, dest)" ) );
			return 0;
		}

		if ( !std::filesystem::exists( s.get_string( 2 ) ) )
		{
			s.error( XOR_STR( "dest does not exist" ) );
			return 0;
		}

		s.user_data_ptr<zip_file>( 1 )->extractall( s.get_string( 2 ) );
		return 0;
	}

	int gc( lua_State* l )
	{
		runtime_state s( l );
		s.user_data_ptr<zip_file>( 1 )->~zip_file();
		return 0;
	}

	int create( lua_State* l )
	{
		runtime_state s( l );

		s.create_user_object_managed<zip_file>( XOR_STR( "zip" ) );
		return 1;
	}

	int open( lua_State* l )
	{
		runtime_state s( l );
		if ( !vars::lua.allow_insecure->get<bool>() )
		{
			s.error( XOR_STR( "open is not available with Allow insecure disabled" ) );
			return 0;
		}

		if ( !s.check_arguments( { { ltd::string } } ) )
		{
			s.error( XOR_STR( "usage: zip.open(path)" ) );
			return 0;
		}

		const auto z = s.create_user_object_managed<zip_file>( XOR_STR( "zip" ) );
		try
		{
			z->load( s.get_string( 1 ) );
			return 1;
		}
		catch ( const std::exception& e )
		{
			s.error( e.what() );
			return 0;
		}
	}

	int open_stream( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::table } } ) )
		{
			s.error( XOR_STR( "usage: zip.open(data)" ) );
			return 0;
		}

		std::vector<unsigned char> data;
		for ( const auto& b : s.table_to_int_array( 1 ) )
		{
			if ( ( uint32_t ) b > 255 )
			{
				s.error( XOR_STR( "invalid byte value" ) );
				return 0;
			}

			data.push_back( ( unsigned char ) b );
		}

		const auto z = s.create_user_object_managed<zip_file>( XOR_STR( "zip" ) );
		try
		{
			z->load( data );
			return 1;
		}
		catch ( const std::exception& e )
		{
			s.error( e.what() );
			return 0;
		}
	}
}