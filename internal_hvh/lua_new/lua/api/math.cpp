#include "../include_cheat.h"

using namespace lua;
namespace lua::api_def::math
{
	int vec3_new( lua_State* l )
	{
		runtime_state s( l );

		if ( s.get_stack_top() > 3 )
		{
			s.error( XOR_STR( "usage: vec3(x?, y?, z?)" ) );
			return 0;
		}

		Vector vec{
			s.get_stack_top() >= 1 ? s.get_float( 1 ) : 0.f,
			s.get_stack_top() >= 2 ? s.get_float( 2 ) : 0.f,
			s.get_stack_top() >= 3 ? s.get_float( 3 ) : 0.f,
		};

		s.create_user_object( XOR_STR( "vec3" ), &vec );
		return 1;
	}

	int vec3_length( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data } } );
		if ( !r )
		{
			s.error( XOR_STR( "usage: obj:length()" ) );
			return 0;
		}

		const auto len = s.user_data<Vector>( 1 ).Length();
		s.push( len );
		return 1;
	}

	int vec3_length2d( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data } } );
		if ( !r )
		{
			s.error( XOR_STR( "usage: obj:length2d()" ) );
			return 0;
		}

		const auto len = s.user_data<Vector>( 1 ).Length2D();
		s.push( len );
		return 1;
	}

	int vec3_dot( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data }, { ltd::user_data } } );
		if ( !r )
		{
			s.error( XOR_STR( "usage: obj:dot(vec3 other)" ) );
			return 0;
		}

		const auto dot = s.user_data<Vector>( 1 ).Dot( s.user_data<Vector>( 2 ) );
		s.push( dot );
		return 1;
	}

	int vec3_cross( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data }, { ltd::user_data } } );
		if ( !r )
		{
			s.error( XOR_STR( "usage: obj:cross(vec3 other)" ) );
			return 0;
		}

		auto vec = s.user_data<Vector>( 1 ).Cross( s.user_data<Vector>( 2 ) );
		s.create_user_object( XOR_STR( "vec3" ), &vec );
		return 1;
	}

	int vec3_normalize( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data } } );
		if ( !r )
		{
			s.error( XOR_STR( "usage: obj:normalize()" ) );
			return 0;
		}

		auto& vec = s.user_data<Vector>( 1 );
		vec.NormalizeInPlace();

		s.create_user_object( XOR_STR( "vec3" ), &vec );
		return 1;
	}

	int vec3_add( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data }, { ltd::user_data } } );
		const auto r2 = s.check_arguments( { { ltd::user_data }, { ltd::number } } );
		if ( !r && !r2 )
		{
			s.error( XOR_STR( "usage: obj + vec3/number" ) );
			return 0;
		}

		auto vec = s.user_data<Vector>( 1 );
		if ( s.is_number( 2 ) )
			vec += s.get_float( 2 );
		else
			vec += s.user_data<Vector>( 2 );

		s.create_user_object( XOR_STR( "vec3" ), &vec );
		return 1;
	}

	int vec3_sub( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data }, { ltd::user_data } } );
		const auto r2 = s.check_arguments( { { ltd::user_data }, { ltd::number } } );
		if ( !r && !r2 )
		{
			s.error( XOR_STR( "usage: obj - vec3/number" ) );
			return 0;
		}

		auto vec = s.user_data<Vector>( 1 );
		if ( s.is_number( 2 ) )
			vec -= s.get_float( 2 );
		else
			vec -= s.user_data<Vector>( 2 );

		s.create_user_object( XOR_STR( "vec3" ), &vec );
		return 1;
	}

	int vec3_mul( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data }, { ltd::user_data } } );
		const auto r2 = s.check_arguments( { { ltd::user_data }, { ltd::number } } );
		if ( !r && !r2 )
		{
			s.error( XOR_STR( "usage: obj * vec3/number" ) );
			return 0;
		}

		auto vec = s.user_data<Vector>( 1 );
		if ( s.is_number( 2 ) )
			vec *= s.get_float( 2 );
		else
			vec *= s.user_data<Vector>( 2 );

		s.create_user_object( XOR_STR( "vec3" ), &vec );
		return 1;
	}

	int vec3_div( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data }, { ltd::user_data } } );
		const auto r2 = s.check_arguments( { { ltd::user_data }, { ltd::number } } );
		if ( !r && !r2 )
		{
			s.error( XOR_STR( "usage: obj / vec3/number" ) );
			return 0;
		}

		auto vec = s.user_data<Vector>( 1 );
		if ( s.is_number( 2 ) )
			vec /= s.get_float( 2 );
		else
			vec /= s.user_data<Vector>( 2 );

		s.create_user_object( XOR_STR( "vec3" ), &vec );
		return 1;
	}

	int vec3_calc_angle( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data }, { ltd::user_data } } );
		if ( !r )
		{
			s.error( XOR_STR( "usage: obj:calc_angle(vec3 other): vec3" ) );
			return 0;
		}

		const auto res = ::math::calc_angle( s.user_data<Vector>( 1 ), s.user_data<Vector>( 2 ) );
		auto ret = Vector( res.x, res.y, res.z );

		s.create_user_object( XOR_STR( "vec3" ), &ret );
		return 1;
	}

	int vec3_dist( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data }, { ltd::user_data } } );
		if ( !r )
		{
			s.error( XOR_STR( "usage: obj:dist(vec3 other): number" ) );
			return 0;
		}

		const auto res = s.user_data<Vector>( 1 ).Dist( s.user_data<Vector>( 2 ) );
		s.push( res );
		return 1;
	}

	int vec3_dist2d( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data }, { ltd::user_data } } );
		if ( !r )
		{
			s.error( XOR_STR( "usage: obj:dist2d(vec3 other): number" ) );
			return 0;
		}

		const auto res = s.user_data<Vector>( 1 ).To2D().Dist( s.user_data<Vector>( 2 ).To2D() );
		s.push( res );
		return 1;
	}

	int vec3_to2d( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data } } );
		if ( !r )
		{
			s.error( XOR_STR( "usage: obj:to2d(): vec3" ) );
			return 0;
		}

		auto vec = s.user_data<Vector>( 1 ).To2D();
		s.create_user_object( XOR_STR( "vec3" ), &vec );
		return 1;
	}

	int vec3_index( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string } } ) )
		{
			s.error( XOR_STR( "wtf are you doing" ) );
			return 0;
		}

		const auto vec = s.user_data<Vector>( 1 );
		switch ( fnv1a_rt( s.get_string( 2 ) ) )
		{
			case FNV1A( "x" ):
				s.push( vec.x );
				return 1;
			case FNV1A( "y" ):
				s.push( vec.y );
				return 1;
			case FNV1A( "z" ):
				s.push( vec.z );
				return 1;
			case FNV1A( "length" ):
				s.push( vec3_length );
				return 1;
			case FNV1A( "length2d" ):
				s.push( vec3_length2d );
				return 1;
			case FNV1A( "dot" ):
				s.push( vec3_dot );
				return 1;
			case FNV1A( "cross" ):
				s.push( vec3_cross );
				return 1;
			case FNV1A( "normalize" ):
				s.push( vec3_normalize );
				return 1;
			case FNV1A( "calc_angle" ):
				s.push( vec3_calc_angle );
				return 1;
			case FNV1A( "dist" ):
				s.push( vec3_dist );
				return 1;
			case FNV1A( "dist2d" ):
				s.push( vec3_dist2d );
				return 1;
			case FNV1A( "to2d" ):
				s.push( vec3_to2d );
				return 1;
			case FNV1A( "unpack" ):
				s.push( vec3_unpack );
				return 1;
			default:
				return 0;
		}
	}

	int vec3_new_index( lua_State* l )
	{
		runtime_state s( l );
		if ( !s.check_arguments( { { ltd::user_data }, { ltd::string }, { ltd::number } } ) )
		{
			s.error( XOR_STR( "second operand should be a number" ) );
			return 0;
		}

		auto& vec = s.user_data<Vector>( 1 );
		switch ( fnv1a_rt( s.get_string( 2 ) ) )
		{
			case FNV1A( "x" ):
				vec.x = s.get_float( 3 ); break;
			case FNV1A( "y" ):
				vec.y = s.get_float( 3 ); break;
			case FNV1A( "z" ):
				vec.z = s.get_float( 3 ); break;
		}

		return 0;
	}

	int vector_angles( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data } } );
		if ( !r )
		{
			s.error( XOR_STR( "usage: vector_angles(vec3 forward) vec3" ) );
			return 0;
		}

		const auto vec = s.user_data<Vector>( 1 );

		QAngle angles;
		::math::vector_angles( vec, angles );
		s.create_user_object( XOR_STR( "vec3" ), &angles );
		return 1;
	}

	int angle_vectors( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data } } );
		if ( !r )
		{
			s.error( XOR_STR( "usage: angle_vectors(vec3 angle) vec3, vec3, vec3" ) );
			return 0;
		}

		const auto vec = s.user_data<QAngle>( 1 );

		Vector forward, right, up;
		::math::angle_vectors( vec, &forward, &right, &up );

		s.create_user_object( XOR_STR( "vec3" ), &forward );
		s.create_user_object( XOR_STR( "vec3" ), &right );
		s.create_user_object( XOR_STR( "vec3" ), &up );
		return 3;
	}

	int vec3_unpack( lua_State* l )
	{
		runtime_state s( l );
		const auto r = s.check_arguments( { { ltd::user_data } } );
		if ( !r )
		{
			s.error( XOR_STR( "usage: obj:unpack() number, number, number" ) );
			return 0;
		}

		const auto vec = s.user_data<Vector>( 1 );

		s.push( vec.x );
		s.push( vec.y );
		s.push( vec.z );
		return 3;
	}
}
