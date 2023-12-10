#include "../include_cheat.h"

void math::angle_matrix( const QAngle& angles, const Vector& position, matrix3x4_t& matrix )
{
	angle_matrix( angles, matrix );
	matrix_set_column( position, 3, matrix );
}

void math::angle_matrix( const QAngle& angles, matrix3x4_t& matrix )
{
	float sr, sp, sy, cr, cp, cy;

	sin_cos( DEG2RAD( angles.x ), &sp, &cp );
	sin_cos( DEG2RAD( angles.y ), &sy, &cy );
	sin_cos( DEG2RAD( angles.z ), &sr, &cr );

	// matrix = (YAW * PITCH) * ROLL
	matrix[ 0 ][ 0 ] = cp * cy;
	matrix[ 1 ][ 0 ] = cp * sy;
	matrix[ 2 ][ 0 ] = -sp;

	// NOTE: Do not optimize this to reduce multiplies! optimizer bug will screw this up.
	matrix[ 0 ][ 1 ] = sr * sp * cy + cr * -sy;
	matrix[ 1 ][ 1 ] = sr * sp * sy + cr * cy;
	matrix[ 2 ][ 1 ] = sr * cp;
	matrix[ 0 ][ 2 ] = ( cr * sp * cy + -sr * -sy );
	matrix[ 1 ][ 2 ] = ( cr * sp * sy + -sr * cy );
	matrix[ 2 ][ 2 ] = cr * cp;

	matrix[ 0 ][ 3 ] = 0.0f;
	matrix[ 1 ][ 3 ] = 0.0f;
	matrix[ 2 ][ 3 ] = 0.0f;
}

void math::matrix_angles( const matrix3x4_t& matrix, QAngle& angles )
{
	float forward[ 3 ];
	float left[ 3 ];
	float up[ 3 ];

	//
	// Extract the basis vectors from the matrix. Since we only need the Z
	// component of the up vector, we don't get X and Y.
	//
	forward[ 0 ] = matrix[ 0 ][ 0 ];
	forward[ 1 ] = matrix[ 1 ][ 0 ];
	forward[ 2 ] = matrix[ 2 ][ 0 ];
	left[ 0 ] = matrix[ 0 ][ 1 ];
	left[ 1 ] = matrix[ 1 ][ 1 ];
	left[ 2 ] = matrix[ 2 ][ 1 ];
	up[ 2 ] = matrix[ 2 ][ 2 ];

	float xyDist = sqrtf( forward[ 0 ] * forward[ 0 ] + forward[ 1 ] * forward[ 1 ] );

	// enough here to get angles?
	if ( xyDist > 0.001f )
	{
		// (yaw)	y = ATAN( forward.y, forward.x );		-- in our space, forward is the X axis
		angles[ 1 ] = RAD2DEG( atan2f( forward[ 1 ], forward[ 0 ] ) );

		// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
		angles[ 0 ] = RAD2DEG( atan2f( -forward[ 2 ], xyDist ) );

		// (roll)	z = ATAN( left.z, up.z );
		angles[ 2 ] = RAD2DEG( atan2f( left[ 2 ], up[ 2 ] ) );
	}
	else	// forward is mostly Z, gimbal lock-
	{
		// (yaw)	y = ATAN( -left.x, left.y );			-- forward is mostly z, so use right for yaw
		angles[ 1 ] = RAD2DEG( atan2f( -left[ 0 ], left[ 1 ] ) );

		// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
		angles[ 0 ] = RAD2DEG( atan2f( -forward[ 2 ], xyDist ) );

		// Assume no roll in this case as one degree of freedom has been lost (i.e. yaw == roll)
		angles[ 2 ] = 0;
	}
}

void math::matrix_multiply( matrix3x4_t& in1, matrix3x4_t& in2, matrix3x4_t& out )
{
	out[ 0 ][ 0 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 0 ] +
		in1[ 0 ][ 2 ] * in2[ 2 ][ 0 ];
	out[ 0 ][ 1 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 1 ] +
		in1[ 0 ][ 2 ] * in2[ 2 ][ 1 ];
	out[ 0 ][ 2 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 2 ] +
		in1[ 0 ][ 2 ] * in2[ 2 ][ 2 ];
	out[ 0 ][ 3 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 3 ] +
		in1[ 0 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 0 ][ 3 ];
	out[ 1 ][ 0 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 0 ] +
		in1[ 1 ][ 2 ] * in2[ 2 ][ 0 ];
	out[ 1 ][ 1 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 1 ] +
		in1[ 1 ][ 2 ] * in2[ 2 ][ 1 ];
	out[ 1 ][ 2 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 2 ] +
		in1[ 1 ][ 2 ] * in2[ 2 ][ 2 ];
	out[ 1 ][ 3 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 3 ] +
		in1[ 1 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 1 ][ 3 ];
	out[ 2 ][ 0 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 0 ] +
		in1[ 2 ][ 2 ] * in2[ 2 ][ 0 ];
	out[ 2 ][ 1 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 1 ] +
		in1[ 2 ][ 2 ] * in2[ 2 ][ 1 ];
	out[ 2 ][ 2 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 2 ] +
		in1[ 2 ][ 2 ] * in2[ 2 ][ 2 ];
	out[ 2 ][ 3 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 3 ] +
		in1[ 2 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 2 ][ 3 ];
}

void math::rotate_matrix( const QAngle& angles, matrix3x4_t& matrix )
{
	QAngle orig_angles{};
	Vector orig_origin{};
	matrix_get_column( matrix, 3, orig_origin );
	matrix_angles( matrix, orig_angles );

	angle_matrix( angles, matrix );
	matrix_set_column( orig_origin, 3, matrix );
}

void math::axis_angle_quaternion( const Vector& axis, float angle, Quaternion& q )
{
	float sa, ca;

	sin_cos( DEG2RAD( angle ) * 0.5f, &sa, &ca );

	q.x = axis.x * sa;
	q.y = axis.y * sa;
	q.z = axis.z * sa;
	q.w = ca;
}

void math::matrix_set_column( const Vector& in, int column, matrix3x4_t& out )
{
	out[ 0 ][ column ] = in.x;
	out[ 1 ][ column ] = in.y;
	out[ 2 ][ column ] = in.z;
}

void math::matrix_get_column( const matrix3x4_t& in, int column, Vector& out )
{
	out.x = in[ 0 ][ column ];
	out.y = in[ 1 ][ column ];
	out.z = in[ 2 ][ column ];
}

void math::matrix_set_origin( Vector pos, matrix3x4_t& matrix )
{
	for ( size_t i{}; i < 3; ++i )
	{
		matrix[ i ][ 3 ] = pos[ i ];
	}
}

Vector math::matrix_get_origin( const matrix3x4_t& src )
{
	return Vector( src[ 0 ][ 3 ], src[ 1 ][ 3 ], src[ 2 ][ 3 ] );
}


void math::vector_scale( const Vector& in, float scale, Vector& out )
{
	out[ 0 ] = in[ 0 ] * scale;
	out[ 1 ] = in[ 1 ] * scale;
	out[ 2 ] = in[ 2 ] * scale;
}

void math::vector_scale( const float* in, float scale, float* out )
{
	out[ 0 ] = in[ 0 ] * scale;
	out[ 1 ] = in[ 1 ] * scale;
	out[ 2 ] = in[ 2 ] * scale;
}

void math::vector_ma( const Vector& start, float scale, const Vector& direction, Vector& dest )
{
	dest[ 0 ] = start[ 0 ] + direction[ 0 ] * scale;
	dest[ 1 ] = start[ 1 ] + direction[ 1 ] * scale;
	dest[ 2 ] = start[ 2 ] + direction[ 2 ] * scale;
}

void math::vector_i_transform( const Vector& in1, const matrix3x4_t& in2, Vector& out )
{
	float in1_t[ 3 ];

	in1_t[ 0 ] = in1[ 0 ] - in2[ 0 ][ 3 ];
	in1_t[ 1 ] = in1[ 1 ] - in2[ 1 ][ 3 ];
	in1_t[ 2 ] = in1[ 2 ] - in2[ 2 ][ 3 ];

	out[ 0 ] = in1_t[ 0 ] * in2[ 0 ][ 0 ] + in1_t[ 1 ] * in2[ 1 ][ 0 ] + in1_t[ 2 ] * in2[ 2 ][ 0 ];
	out[ 1 ] = in1_t[ 0 ] * in2[ 0 ][ 1 ] + in1_t[ 1 ] * in2[ 1 ][ 1 ] + in1_t[ 2 ] * in2[ 2 ][ 1 ];
	out[ 2 ] = in1_t[ 0 ] * in2[ 0 ][ 2 ] + in1_t[ 1 ] * in2[ 1 ][ 2 ] + in1_t[ 2 ] * in2[ 2 ][ 2 ];
}

void math::vector_rotate( const Vector& in1, const matrix3x4_t& in2, Vector& out )
{
	vector_rotate( &in1.x, in2, &out.x );
}

void math::vector_rotate( const float* in1, const matrix3x4_t& in2, float* out )
{
	float x = dot_product( in1, in2[ 0 ] );
	float y = dot_product( in1, in2[ 1 ] );
	float z = dot_product( in1, in2[ 2 ] );

	out[ 0 ] = x;
	out[ 1 ] = y;
	out[ 2 ] = z;
}

float math::dot_product( const Vector v1, const Vector v2 )
{
	return v1[ 0 ] * v2[ 0 ] + v1[ 1 ] * v2[ 1 ] + v1[ 2 ] * v2[ 2 ];
}

void math::cross_product( const Vector v1, const Vector v2, Vector& result )
{
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
}

void math::matrix_copy( const matrix3x4_t& in, matrix3x4_t& out )
{
	memcpy( out.Base(), in.Base(), sizeof( float ) * 3 * 4 );
}

const VMatrix& world_to_screen_matrix()
{
	return *reinterpret_cast< VMatrix* >( make_offset( "client.dll", sig_view_matrix ) + 176 );
}

bool math::screen_transform( const Vector& in, Vector& out )
{
	const auto& w2_s_matrix = world_to_screen_matrix();
	out.x = w2_s_matrix[ 0 ][ 0 ] * in[ 0 ] + w2_s_matrix[ 0 ][ 1 ] * in[ 1 ] + w2_s_matrix[ 0 ][ 2 ] * in[ 2 ] + w2_s_matrix[ 0 ][ 3 ];
	out.y = w2_s_matrix[ 1 ][ 0 ] * in[ 0 ] + w2_s_matrix[ 1 ][ 1 ] * in[ 1 ] + w2_s_matrix[ 1 ][ 2 ] * in[ 2 ] + w2_s_matrix[ 1 ][ 3 ];
	out.z = 0.0f;

	const auto w = w2_s_matrix[ 3 ][ 0 ] * in.x + w2_s_matrix[ 3 ][ 1 ] * in.y + w2_s_matrix[ 3 ][ 2 ] * in.z + w2_s_matrix[ 3 ][ 3 ];

	if ( w < 0.001f )
	{
		out.x *= 100000;
		out.y *= 100000;
		return false;
	}

	const auto invw = 1.0f / w;
	out.x *= invw;
	out.y *= invw;

	return true;
}

bool math::world_to_screen( const Vector& in, Vector& out )
{
	const auto result = screen_transform( in, out );

	int w, h;
	interfaces::engine()->GetScreenSize( w, h );

	out.x = ( w / 2.0f ) + ( out.x * w ) / 2.0f;
	out.y = ( h / 2.0f ) - ( out.y * h ) / 2.0f;

	return result;
}

float math::get_fov( const QAngle& angles, const QAngle& target )
{
	Vector ang{}, aim{};

	angle_vectors( angles, &aim );
	angle_vectors( target, &ang );

	return RAD2DEG( acos( aim.Dot( ang ) / aim.LengthSqr() ) );
}

float math::approach_angle( float target, float value, float speed )
{
	target = anglemod( target );
	value = anglemod( value );

	auto delta = target - value;

	// Speed is assumed to be positive
	if ( speed < 0 )
		speed = -speed;

	if ( delta < -180 )
		delta += 360;
	else if ( delta > 180 )
		delta -= 360;

	if ( delta > speed )
		value += speed;
	else if ( delta < -speed )
		value -= speed;
	else
		value = target;

	return value;
}