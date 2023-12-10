#pragma once

#include "intrinsics.h"

class Quaternion				// same data-layout as engine's vec4_t,
{								//		which is a float[4]
public:
	inline Quaternion( void ) {}
	inline Quaternion( float ix, float iy, float iz, float iw ) : x( ix ), y( iy ), z( iz ), w( iw ) {}

	inline void Init( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f ) { x = ix; y = iy; z = iz; w = iw; }

	float* Base() { return ( float* ) this; }
	const float* Base() const { return ( float* ) this; }

	float x{}, y{}, z{}, w{};
};

class ALIGN16 QuaternionAligned : public Quaternion
{
public:
	inline QuaternionAligned( void ) {};
	inline QuaternionAligned( float X, float Y, float Z, float W )
	{
		Init( X, Y, Z, W );
	}
public:
	explicit QuaternionAligned( const Quaternion& vOther )
	{
		Init( vOther.x, vOther.y, vOther.z, vOther.w );
	}

	QuaternionAligned& operator=( const Quaternion& vOther )
	{
		Init( vOther.x, vOther.y, vOther.z, vOther.w );
		return *this;
	}
};

namespace math
{

	__forceinline void fast_sqrt( float* __restrict p_out, float* __restrict p_in )
	{
		_mm_store_ss( p_out, _mm_sqrt_ss( _mm_load_ss( p_in ) ) );
		// compiles to movss, sqrtss, movss
	}

	__forceinline void fast_rsqrt( float a, float* out )
	{
		const auto xx = _mm_load_ss( &a );
		auto xr = _mm_rsqrt_ss( xx );
		auto xt = _mm_mul_ss( xr, xr );
		xt = _mm_mul_ss( xt, xx );
		xt = _mm_sub_ss( _mm_set_ss( 3.f ), xt );
		xt = _mm_mul_ss( xt, _mm_set_ss( 0.5f ) );
		xr = _mm_mul_ss( xr, xt );
		_mm_store_ss( out, xr );
	}

	__forceinline float fast_vec_normalize( Vector& vec )
	{
		const auto sqrlen = vec.LengthSqr() + 1.0e-10f;
		float invlen;
		fast_rsqrt( sqrlen, &invlen );
		vec.x *= invlen;
		vec.y *= invlen;
		vec.z *= invlen;
		return sqrlen * invlen;
	}

	__forceinline float angle_diff( float destAngle, float srcAngle )
	{
		float delta;

		delta = fmodf( destAngle - srcAngle, 360.0f );
		if ( destAngle > srcAngle )
		{
			if ( delta >= 180 )
				delta -= 360;
		}
		else
		{
			if ( delta <= -180 )
				delta += 360;
		}
		return delta;
	}
	__forceinline void vector_transform( const Vector& in1, const matrix3x4_t& in2, Vector& out )
	{
		out.x = in1.Dot( in2.m_flMatVal[ 0 ] ) + in2.m_flMatVal[ 0 ][ 3 ];
		out.y = in1.Dot( in2.m_flMatVal[ 1 ] ) + in2.m_flMatVal[ 1 ][ 3 ];
		out.z = in1.Dot( in2.m_flMatVal[ 2 ] ) + in2.m_flMatVal[ 2 ][ 3 ];
	}

	__forceinline matrix3x4_t rotation_matrix( const Vector& direction, const Vector& up = Vector( 0, 1, 0 ) )
	{
		matrix3x4_t out{};

		auto xaxis = up.Cross( direction );
		auto yaxis = direction.Cross( xaxis );
		fast_vec_normalize( xaxis );
		fast_vec_normalize( yaxis );

		out[ 0 ][ 0 ] = xaxis.x;
		out[ 0 ][ 1 ] = yaxis.x;
		out[ 0 ][ 2 ] = direction.x;

		out[ 1 ][ 0 ] = xaxis.y;
		out[ 1 ][ 1 ] = yaxis.y;
		out[ 1 ][ 2 ] = direction.y;

		out[ 2 ][ 0 ] = xaxis.z;
		out[ 2 ][ 1 ] = yaxis.z;
		out[ 2 ][ 2 ] = direction.z;

		return out;
	}

	__forceinline float distance_point_to_line( const Vector& a, const Vector& b, const Vector& c )
	{
		const auto l = c.Length();
		const auto t = ( a - b ).Dot( c ) / ( l * l );

		if ( t <= FLT_EPSILON )
			return FLT_MAX;

		const auto x = b + c * t;
		return ( a - x ).Length();
	}

	__forceinline QAngle calc_angle( const Vector src, const Vector dst )
	{
		static Vector vec_zero = { 0.0f, 0.0f, 0.0f };
		static QAngle ang_zero = { 0.0f, 0.0f, 0.0f };

		const auto delta = src - dst;
		if ( delta == vec_zero )
			return ang_zero;

		/*_( ntdll, "ntdll.dll" );
		static auto addr = DWORD64();
		if ( !addr )
		{
			std::wstring w;
			std::copy( ntdll, ntdll.c_str() + strlen( ntdll.c_str() ), back_inserter( w ) );
			addr = GetModuleHandle64( const_cast< wchar_t* >( w.c_str() ) );
			if ( addr )
				addr += 0x50;
		}
		auto res = 0.f;*/
//#ifdef RELEASE
//		auto temp = SIZE_T();
//		ReadProcessMemory64( HANDLE( -1 ), addr, reinterpret_cast< void* >( &res ), sizeof( int ), &temp );
//#else
//		res = 57.295779513082f;
//#endif

		auto res = 57.295779513082f;

		const auto len = delta.Length();

		if ( delta.z == 0.0f && len == 0.0f )
			return ang_zero;

		if ( delta.y == 0.0f && delta.x == 0.0f )
			return ang_zero;

		QAngle angles{};

		angles.x = ( asinf( delta.z / delta.Length() ) * res );
		angles.y = ( atanf( delta.y / delta.x ) * res );

		angles.z = 0.0f;
		if ( delta.x >= 0.0f ) { angles.y += 180.0f; }

		return angles.Clamp();
	}

	__forceinline float fast_atan2f( const float y, const float x )
	{

		static constexpr auto pi_float = 3.14159265f;
		static constexpr auto piby2_float = 1.5707963f;

		if ( x == 0.0f )
		{
			if ( y > 0.0f ) return piby2_float;
			if ( y == 0.0f ) return 0.0f;
			return -piby2_float;
		}
		float atan;
		const auto z = y / x;
		if ( fabs( z ) < 1.0f )
		{
			atan = z / ( 1.0f + 0.28f * z * z );
			if ( x < 0.0f )
			{
				if ( y < 0.0f ) return atan - pi_float;
				return atan + pi_float;
			}
		}
		else
		{
			atan = piby2_float - z / ( z * z + 0.28f );
			if ( y < 0.0f ) return atan - pi_float;
		}
		return atan;
	}

	__forceinline void vector_angles( const Vector& forward, QAngle& angles, Vector* up = nullptr )
	{
		Vector  left;
		float   len, up_z, pitch, yaw, roll;

		// get 2d length.
		len = forward.Length2D();

		if ( up && len > 0.001f )
		{
			pitch = RAD2DEG( std::atan2( -forward.z, len ) );
			yaw = RAD2DEG( std::atan2( forward.y, forward.x ) );

			// get left direction vector using cross product.
			left = ( *up ).Cross( forward ).Normalized();

			// calculate up_z.
			up_z = ( left.y * forward.x ) - ( left.x * forward.y );

			// calculate roll.
			roll = RAD2DEG( std::atan2( left.z, up_z ) );
		}

		else
		{
			if ( len > 0.f )
			{
				// calculate pitch and yaw.
				pitch = RAD2DEG( std::atan2( -forward.z, len ) );
				yaw = RAD2DEG( std::atan2( forward.y, forward.x ) );
				roll = 0.f;
			}

			else
			{
				pitch = ( forward.z > 0.f ) ? -90.f : 90.f;
				yaw = 0.f;
				roll = 0.f;
			}
		}

		// set out angles.
		angles = { pitch, yaw, roll };
	}

	void cross_product( const Vector v1, const Vector v2, Vector& result );
	__forceinline void cross_product( const float* v1, const float* v2, float* cross )
	{
		cross[ 0 ] = v1[ 1 ] * v2[ 2 ] - v1[ 2 ] * v2[ 1 ];
		cross[ 1 ] = v1[ 2 ] * v2[ 0 ] - v1[ 0 ] * v2[ 2 ];
		cross[ 2 ] = v1[ 0 ] * v2[ 1 ] - v1[ 1 ] * v2[ 0 ];
	}

	__forceinline void vector_angles( const Vector& forward, const Vector& pseudoup, QAngle& angles )
	{
		Vector left{};

		cross_product( pseudoup, forward, left );
		fast_vec_normalize( left );

		float xyDist = sqrtf( forward[ 0 ] * forward[ 0 ] + forward[ 1 ] * forward[ 1 ] );

		// enough here to get angles?
		if ( xyDist > 0.001f )
		{
			// (yaw)	y = ATAN( forward.y, forward.x );		-- in our space, forward is the X axis
			angles[ 1 ] = RAD2DEG( atan2f( forward[ 1 ], forward[ 0 ] ) );

			// The engine does pitch inverted from this, but we always end up negating it in the DLL
			// UNDONE: Fix the engine to make it consistent
			// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
			angles[ 0 ] = RAD2DEG( atan2f( -forward[ 2 ], xyDist ) );

			float up_z = ( left[ 1 ] * forward[ 0 ] ) - ( left[ 0 ] * forward[ 1 ] );

			// (roll)	z = ATAN( left.z, up.z );
			angles[ 2 ] = RAD2DEG( atan2f( left[ 2 ], up_z ) );
		}
		else	// forward is mostly Z, gimbal lock-
		{
			// (yaw)	y = ATAN( -left.x, left.y );			-- forward is mostly z, so use right for yaw
			angles[ 1 ] = RAD2DEG( atan2f( -left[ 0 ], left[ 1 ] ) ); //This default originally copied from the "void MatrixAngles( const matrix3x4_t& matrix, float *angles )" code, and it's 180 degrees off, negated the values and it all works now (Dave Kircher)

																	  // The engine does pitch inverted from this, but we always end up negating it in the DLL
																	  // UNDONE: Fix the engine to make it consistent
																	  // (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
			angles[ 0 ] = RAD2DEG( atan2f( -forward[ 2 ], xyDist ) );

			// Assume no roll in this case as one degree of freedom has been lost (i.e. yaw == roll)
			angles[ 2 ] = 0;
		}
	}

	inline void sin_cos( float radians, float* sine, float* cosine )
	{
		*sine = sin( radians );
		*cosine = cos( radians );
	}

	__forceinline void angle_vectors( const QAngle& angles, Vector* forward = nullptr, Vector* right = nullptr, Vector* up = nullptr )
	{
		float sr, sp, sy, cr, cp, cy;

		sin_cos( DEG2RAD( angles.x ), &sp, &cp );
		sin_cos( DEG2RAD( angles.y ), &sy, &cy );
		sin_cos( DEG2RAD( angles.z ), &sr, &cr );

		if ( forward )
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if ( right )
		{
			right->x = ( -1 * sr * sp * cy + -1 * cr * -sy );
			right->y = ( -1 * sr * sp * sy + -1 * cr * cy );
			right->z = -1 * sr * cp;
		}

		if ( up )
		{
			up->x = ( cr * sp * cy + -sr * -sy );
			up->y = ( cr * sp * sy + -sr * cy );
			up->z = cr * cp;
		}
	}

	__forceinline matrix3x4_t angle_matrix( const QAngle& ang )
	{
		matrix3x4_t result{};

		float sr, sp, sy, cr, cp, cy;
		sin_cos( DEG2RAD( ang.x ), &sp, &cp );
		sin_cos( DEG2RAD( ang.y ), &sy, &cy );
		sin_cos( DEG2RAD( ang.z ), &sr, &cr );

		result[ 0 ][ 0 ] = cp * cy;
		result[ 1 ][ 0 ] = cp * sy;
		result[ 2 ][ 0 ] = -sp;

		const auto crcy = cr * cy;
		const auto crsy = cr * sy;
		const auto srcy = sr * cy;
		const auto srsy = sr * sy;
		result[ 0 ][ 1 ] = sp * srcy - crsy;
		result[ 1 ][ 1 ] = sp * srsy + crcy;
		result[ 2 ][ 1 ] = sr * cp;

		result[ 0 ][ 2 ] = ( sp * crcy + srsy );
		result[ 1 ][ 2 ] = ( sp * crsy - srcy );
		result[ 2 ][ 2 ] = cr * cp;

		return result;
	}

	__forceinline matrix3x4_t angle_matrix( const QAngle& ang, const Vector& pos )
	{
		auto result = angle_matrix( ang );
		result[ 0 ][ 3 ] = pos.x;
		result[ 1 ][ 3 ] = pos.y;
		result[ 2 ][ 3 ] = pos.z;
		return result;
	}

	__forceinline matrix3x4_t matrix_invert( const matrix3x4_t& in )
	{
		matrix3x4_t out{};
		out[ 0 ][ 0 ] = in[ 0 ][ 0 ];
		out[ 0 ][ 1 ] = in[ 1 ][ 0 ];
		out[ 0 ][ 2 ] = in[ 2 ][ 0 ];

		out[ 1 ][ 0 ] = in[ 0 ][ 1 ];
		out[ 1 ][ 1 ] = in[ 1 ][ 1 ];
		out[ 1 ][ 2 ] = in[ 2 ][ 1 ];

		out[ 2 ][ 0 ] = in[ 0 ][ 2 ];
		out[ 2 ][ 1 ] = in[ 1 ][ 2 ];
		out[ 2 ][ 2 ] = in[ 2 ][ 2 ];

		const Vector tmp( in[ 0 ][ 3 ], in[ 1 ][ 3 ], in[ 2 ][ 3 ] );

		out[ 0 ][ 3 ] = -tmp.Dot( out[ 0 ] );
		out[ 1 ][ 3 ] = -tmp.Dot( out[ 1 ] );
		out[ 2 ][ 3 ] = -tmp.Dot( out[ 2 ] );

		return out;
	}

	void angle_matrix( const QAngle& angles, const Vector& position, matrix3x4_t& matrix );
	void angle_matrix( const QAngle& angles, matrix3x4_t& matrix );
	void matrix_angles( const matrix3x4_t& matrix, QAngle& angles );
	void matrix_multiply( matrix3x4_t& in1, matrix3x4_t& in2, matrix3x4_t& out );
	void rotate_matrix( const QAngle& angles, matrix3x4_t& matrix );
	void axis_angle_quaternion( const Vector& axis, float angle, Quaternion& q );
	void matrix_set_column( const Vector& in, int column, matrix3x4_t& out );
	void matrix_get_column( const matrix3x4_t& in, int column, Vector& out );
	void matrix_set_origin( Vector pos, matrix3x4_t& matrix );
	Vector matrix_get_origin( const matrix3x4_t& src );
	void matrix_copy( const matrix3x4_t& in, matrix3x4_t& out );

	void vector_scale( const Vector& in, float scale, Vector& out );
	void vector_scale( const float* in, float scale, float* out );

	__forceinline float normalize_float( float angle )
	{
		auto revolutions = angle / 360.f;
		if ( angle > 180.f || angle < -180.f )
		{
			revolutions = round( abs( revolutions ) );
			if ( angle < 0.f )
				angle = ( angle + 360.f * revolutions );
			else
				angle = ( angle - 360.f * revolutions );
			return angle;
		}
		return angle;
	}

	template < typename t >
	t lerp( const t& t1, const t& t2, float progress )
	{
		return t1 + ( t2 - t1 ) * progress;
	}

	template < typename t >
	t lerp_angle( const t& t1, const t& t2, float progress )
	{
		return math::normalize_float( t1 + ( t2 - t1 ) * progress );
	}

	void vector_ma( const Vector& start, float scale, const Vector& direction, Vector& dest );
	void vector_i_transform( const Vector& in1, const matrix3x4_t& in2, Vector& out );
	void vector_rotate( const Vector& in1, const matrix3x4_t& in2, Vector& out );
	void vector_rotate( const float* in1, const matrix3x4_t& in2, float* out );
	float dot_product( const Vector v1, const Vector v2 );
	__forceinline float dot_product( const float* v1, const float* v2 )
	{
		return v1[ 0 ] * v2[ 0 ] + v1[ 1 ] * v2[ 1 ] + v1[ 2 ] * v2[ 2 ];
	}

	bool screen_transform( const Vector& in, Vector& out );
	bool world_to_screen( const Vector& in, Vector& out );

	__forceinline static float get_fov( const QAngle& current, const Vector& start, const Vector& end )
	{
		Vector currentAim, targetAim = ( end - start ).Normalized();
		angle_vectors( current, &currentAim );
		return RAD2DEG( acos( targetAim.Dot( currentAim ) ) );
	}

	float get_fov( const QAngle& angles, const QAngle& target );

	float approach_angle( float target, float value, float speed );

	__forceinline float approach( float target, float value, float speed )
	{
		float delta = target - value;

		if ( delta > speed )
			value += speed;
		else if ( delta < -speed )
			value -= speed;
		else
			value = target;

		return value;
	}

	__forceinline float anglemod( float a )
	{
		a = ( 360.f / 65536 ) * ( static_cast< int >( a * ( 65536.f / 360.0f ) ) & 65535 );
		return a;
	}


	__forceinline float distance_to_ray( const Vector& pos, const Vector& rayStart, const Vector& rayEnd, float* along = NULL, Vector* pointOnRay = NULL )
	{
		Vector to = pos - rayStart;
		Vector dir = rayEnd - rayStart;
		float length = dir.NormalizeInPlace();

		float rangeAlong = dir.Dot( to );
		if ( along )
		{
			*along = rangeAlong;
		}

		float range;

		if ( rangeAlong < 0.0f )
		{
			// off start point
			range = -( pos - rayStart ).Length();

			if ( pointOnRay )
			{
				*pointOnRay = rayStart;
			}
		}
		else if ( rangeAlong > length )
		{
			// off end point
			range = -( pos - rayEnd ).Length();

			if ( pointOnRay )
			{
				*pointOnRay = rayEnd;
			}
		}
		else // within ray bounds
		{
			Vector onRay = rayStart + dir * rangeAlong;
			range = ( pos - onRay ).Length();

			if ( pointOnRay )
			{
				*pointOnRay = onRay;
			}
		}

		return range;
	}

	__forceinline int try_remap( float val, float& out, float a, float b, float c, float d )
	{
		float clamped_value = ( val - a ) / ( b - a );
		//clamped_value = clamp(clamped_value, 0.0f, 1.0f);
		if ( clamped_value <= 0.f )
			return 1;

		if ( clamped_value >= 1.f )
			return 2;

		out = c + ( d - c ) * clamped_value;

		return 0;
	}

	__forceinline matrix3x4_t quaternion_matrix( const QuaternionAligned& q, const Vector& pos )
	{
		matrix3x4_t result{};
		result[ 0 ][ 0 ] = 1.f - 2.f * q.y * q.y - 2.f * q.z * q.z;
		result[ 1 ][ 0 ] = 2.f * q.x * q.y + 2.f * q.w * q.z;
		result[ 2 ][ 0 ] = 2.f * q.x * q.z - 2.f * q.w * q.y;

		result[ 0 ][ 1 ] = 2.f * q.x * q.y - 2.f * q.w * q.z;
		result[ 1 ][ 1 ] = 1.f - 2.f * q.x * q.x - 2.f * q.z * q.z;
		result[ 2 ][ 1 ] = 2.f * q.y * q.z + 2.f * q.w * q.x;

		result[ 0 ][ 2 ] = 2.f * q.x * q.z + 2.f * q.w * q.y;
		result[ 1 ][ 2 ] = 2.f * q.y * q.z - 2.f * q.w * q.x;
		result[ 2 ][ 2 ] = 1.f - 2.f * q.x * q.x - 2.f * q.y * q.y;

		result[ 0 ][ 3 ] = pos.x;
		result[ 1 ][ 3 ] = pos.y;
		result[ 2 ][ 3 ] = pos.z;
		return result;
	}

	__forceinline matrix3x4_t concat_transforms( const matrix3x4_t& in1, const matrix3x4_t& in2 )
	{
		auto last_mask = *( m128* ) ( &component_mask[ 3 ] );
		auto row_a0 = load_simd( in1[ 0 ] );
		auto row_a1 = load_simd( in1[ 1 ] );
		auto row_a2 = load_simd( in1[ 2 ] );
		auto row_b0 = load_simd( in2[ 0 ] );
		auto row_b1 = load_simd( in2[ 1 ] );
		auto row_b2 = load_simd( in2[ 2 ] );

		auto a0 = splat_x( row_a0 );
		auto a1 = splat_y( row_a0 );
		auto a2 = splat_z( row_a0 );
		auto mul_00 = mul_simd( a0, row_b0 );
		auto mul_01 = mul_simd( a1, row_b1 );
		auto mul_02 = mul_simd( a2, row_b2 );

		auto temp = add_simd( mul_01, mul_02 );
		auto out0 = add_simd( mul_00, temp );

		a0 = splat_x( row_a1 );
		a1 = splat_y( row_a1 );
		a2 = splat_z( row_a1 );

		auto mul10 = mul_simd( a0, row_b0 );
		auto mul11 = mul_simd( a1, row_b1 );
		auto mul12 = mul_simd( a2, row_b2 );
		temp = add_simd( mul11, mul12 );
		auto out1 = add_simd( mul10, temp );

		a0 = splat_x( row_a2 );
		a1 = splat_y( row_a2 );
		a2 = splat_z( row_a2 );
		auto mul_20 = mul_simd( a0, row_b0 );
		auto mul_21 = mul_simd( a1, row_b1 );
		auto mul_22 = mul_simd( a2, row_b2 );
		temp = add_simd( mul_21, mul_22 );
		auto out2 = add_simd( mul_20, temp );

		a0 = v_and( row_a0, last_mask );
		a1 = v_and( row_a1, last_mask );
		a2 = v_and( row_a2, last_mask );
		out0 = add_simd( out0, a0 );
		out1 = add_simd( out1, a1 );
		out2 = add_simd( out2, a2 );

		matrix3x4_t res{};
		store_simd( res[ 0 ], out0 );
		store_simd( res[ 1 ], out1 );
		store_simd( res[ 2 ], out2 );
		return res;
	}

	__forceinline void vector_vectors( const Vector& forward, Vector& right, Vector& up )
	{
		Vector tmp{};

		if ( fabs( forward[ 0 ] ) < 1e-6 && fabs( forward[ 1 ] ) < 1e-6 )
		{
			// pitch 90 degrees up/down from identity
			right[ 0 ] = 0;
			right[ 1 ] = -1;
			right[ 2 ] = 0;
			up[ 0 ] = -forward[ 2 ];
			up[ 1 ] = 0;
			up[ 2 ] = 0;
		}
		else
		{
			tmp[ 0 ] = 0; tmp[ 1 ] = 0; tmp[ 2 ] = 1.0;
			cross_product( forward, tmp, right );
			VectorNormalize( right );
			cross_product( right, forward, up );
			VectorNormalize( up );
		}
	}

	__forceinline void vector_matrix( const Vector& forward, matrix3x4_t& matrix )
	{
		Vector right{}, up{};
		vector_vectors( forward, right, up );

		matrix_set_column( forward, 0, matrix );
		matrix_set_column( Vector( -1 ) * right, 1, matrix );
		matrix_set_column( up, 2, matrix );
	}
};