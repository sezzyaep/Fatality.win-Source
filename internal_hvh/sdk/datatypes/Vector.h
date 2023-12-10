#pragma once

class Vector
{
public:
	float x, y, z;
	Vector() = default;
	Vector( float X, float Y, float Z );
	Vector( float xyz[ 3 ] );
	Vector( float val );
	void Init( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f );
	bool IsValid() const;
	float operator[]( int i ) const;
	float& operator[]( int i );
	inline void Zero();
	inline void Rotate2D( const float& f );
	bool operator==( const Vector& v ) const;
	bool operator!=( const Vector& v ) const;
	__forceinline Vector& operator+=( const Vector& v );
	__forceinline Vector& operator-=( const Vector& v );
	__forceinline Vector& operator*=( const Vector& v );
	__forceinline Vector& operator*=( float s );
	__forceinline Vector& operator/=( const Vector& v );
	__forceinline Vector& operator/=( float s );
	__forceinline Vector& operator+=( float fl );
	__forceinline Vector& operator-=( float fl );
	__forceinline Vector	operator+( float fl );
	__forceinline Vector	operator-( float fl );
	inline float	Length() const;
	__forceinline float LengthSqr( void ) const
	{
		CHECK_VALID( *this );
		return ( this->x * this->x + this->y * this->y + this->z * this->z );
	}
	bool IsZero( float tolerance = 0.01f ) const
	{
		return ( x > -tolerance && x < tolerance&&
				 y > -tolerance && y < tolerance&&
				 z > -tolerance && z < tolerance );
	}
	float	NormalizeInPlace();
	Vector	Normalize() const;
	float	NormalizeL();
	inline Vector Normalized() const;
	inline Vector To2D() const;
	__forceinline float	DistToSqr( const Vector& vOther ) const;
	__forceinline float	Dist( const Vector& vOther ) const;
	Vector	Cross( const Vector& vOther ) const;
	__forceinline float	Dot( const Vector& vOther ) const;
	__forceinline float	Dot( const float* fOther ) const;
	float	Length2D( void ) const;
	float	Length2DSqr( void ) const;
	Vector	Angle( Vector* up = 0 );
	Vector& operator=( const Vector& vOther );
	Vector	operator-( const Vector& v ) const;
	Vector	operator+( const Vector& v ) const;
	Vector	operator*( const Vector& v ) const;
	Vector	operator/( const Vector& v ) const;
	Vector	operator*( float fl ) const;
	Vector	operator/( float fl ) const;
	// Base address...
	float* Base();
	float const* Base() const;
	float get_x() const
	{
		return x;
	}

	void set_x( const float _x )
	{
		x = _x;
	}

	float get_y() const
	{
		return y;
	}

	void set_y( const float _y )
	{
		y = _y;
	}

	float get_z() const
	{
		return z;
	}

	void set_z( const float _z )
	{
		z = _z;
	}

	bool to_screen();
};

//===============================================
inline void Vector::Init( float ix, float iy, float iz )
{
	x = ix; y = iy; z = iz;
	CHECK_VALID( *this );
}

inline bool Vector::IsValid() const
{
	return std::isfinite( x ) && std::isfinite( y ) && std::isfinite( z );
}
//===============================================
inline Vector::Vector( float X, float Y, float Z )
{
	x = X; y = Y; z = Z;
	CHECK_VALID( *this );
}
//===============================================
inline Vector::Vector( float xyz[ 3 ] )
{
	x = xyz[ 0 ]; y = xyz[ 1 ]; z = xyz[ 2 ];
}
//===============================================
inline Vector::Vector( float val )
{
	x = val; y = val; z = val;
}
//===============================================
inline void Vector::Zero()
{
	x = y = z = 0.0f;
}
//===============================================
inline void Vector::Rotate2D( const float& f )
{
	float _x, _y;

	float s, c;

	float r = f * M_PI / 180.0f;
	s = sin( r );
	c = cos( r );

	_x = x;
	_y = y;

	x = ( _x * c ) - ( _y * s );
	y = ( _x * s ) + ( _y * c );
}
//===============================================
inline void VectorClear( Vector& a )
{
	a.x = a.y = a.z = 0.0f;
}
//===============================================
inline Vector& Vector::operator=( const Vector& vOther )
{
	CHECK_VALID( vOther );
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}
//===============================================
inline float& Vector::operator[]( int i )
{
	Assert( ( i >= 0 ) && ( i < 3 ) );
	return ( ( float* ) this )[ i ];
}
//===============================================
inline float Vector::operator[]( int i ) const
{
	Assert( ( i >= 0 ) && ( i < 3 ) );
	return ( ( float* ) this )[ i ];
}
//===============================================
inline bool Vector::operator==( const Vector& src ) const
{
	CHECK_VALID( src );
	CHECK_VALID( *this );
	return ( src.x == x ) && ( src.y == y ) && ( src.z == z );
}
//===============================================
inline bool Vector::operator!=( const Vector& src ) const
{
	CHECK_VALID( src );
	CHECK_VALID( *this );
	return ( src.x != x ) || ( src.y != y ) || ( src.z != z );
}
//===============================================
__forceinline void VectorCopy( const Vector& src, Vector& dst )
{
	CHECK_VALID( src );
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}
//===============================================
__forceinline  Vector& Vector::operator+=( const Vector& v )
{
	CHECK_VALID( *this );
	CHECK_VALID( v );
	x += v.x; y += v.y; z += v.z;
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator-=( const Vector& v )
{
	CHECK_VALID( *this );
	CHECK_VALID( v );
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator*=( float fl )
{
	x *= fl;
	y *= fl;
	z *= fl;
	CHECK_VALID( *this );
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator*=( const Vector& v )
{
	CHECK_VALID( v );
	x *= v.x;
	y *= v.y;
	z *= v.z;
	CHECK_VALID( *this );
	return *this;
}
//===============================================
__forceinline Vector& Vector::operator+=( float fl )
{
	x += fl;
	y += fl;
	z += fl;
	CHECK_VALID( *this );
	return *this;
}
//===============================================
__forceinline Vector& Vector::operator-=( float fl )
{
	x -= fl;
	y -= fl;
	z -= fl;
	CHECK_VALID( *this );
	return *this;
}

//===============================================
__forceinline Vector Vector::operator+( float fl )
{
	return Vector( x + fl, y + fl, z + fl );
}

//===============================================
__forceinline Vector Vector::operator-( float fl )
{
	return Vector( x - fl, y - fl, z - fl );
}
//===============================================
__forceinline  Vector& Vector::operator/=( float fl )
{
	Assert( fl != 0.0f );
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	CHECK_VALID( *this );
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator/=( const Vector& v )
{
	CHECK_VALID( v );
	Assert( v.x != 0.0f && v.y != 0.0f && v.z != 0.0f );
	x /= v.x;
	y /= v.y;
	z /= v.z;
	CHECK_VALID( *this );
	return *this;
}
//===============================================
inline float Vector::Length( void ) const
{
	CHECK_VALID( *this );

	float root = 0.0f;

	float sqsr = x * x + y * y + z * z;

	__asm sqrtss xmm0, sqsr
	__asm movss root, xmm0

	return root;
}
//===============================================
inline float Vector::Length2D( void ) const
{
	CHECK_VALID( *this );

	float root = 0.0f;

	float sqst = x * x + y * y;

	__asm
	{
		sqrtss xmm0, sqst
		movss root, xmm0
	}

	return root;
}
//===============================================
inline float Vector::Length2DSqr( void ) const
{
	return ( x * x + y * y );
}
//===============================================
inline Vector Vector::Angle( Vector* up )
{
	if ( !x && !y )
		return Vector( 0, 0, 0 );

	float roll = 0;

	if ( up )
	{
		Vector left = ( *up ).Cross( *this );
		roll = atan2f( left.z, ( left.y * x ) - ( left.x * y ) ) * 180.0f / M_PI;
	}

	return Vector( atan2f( -z, sqrt2( x * x + y * y ) ) * 180.0f / M_PI, atan2f( y, x ) * 180.0f / M_PI, roll );
}
//===============================================
inline Vector CrossProduct( const Vector& a, const Vector& b )
{
	return Vector( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
}
//===============================================
float Vector::DistToSqr( const Vector& vOther ) const
{
	Vector delta{};

	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;

	return delta.LengthSqr();
}

float Vector::Dist( const Vector& vOther ) const
{
	Vector delta{};

	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;

	return delta.Length();
}

inline Vector Vector::Cross( const Vector& vOther ) const
{
	return CrossProduct( *this, vOther );
}

inline Vector Vector::Normalize() const
{
	Vector vector = *this;
	vector.NormalizeInPlace();
	return vector;
}

inline float Vector::NormalizeL()
{
	Vector vector = *this;
	return vector.NormalizeInPlace();
}


//===============================================
__forceinline float VectorNormalize( Vector& vec )
{
	const auto sqrlen = vec.LengthSqr() + 1.0e-10f;
	float invlen;
	const auto xx = _mm_load_ss( &sqrlen );
	auto xr = _mm_rsqrt_ss( xx );
	auto xt = _mm_mul_ss( xr, xr );
	xt = _mm_mul_ss( xt, xx );
	xt = _mm_sub_ss( _mm_set_ss( 3.f ), xt );
	xt = _mm_mul_ss( xt, _mm_set_ss( 0.5f ) );
	xr = _mm_mul_ss( xr, xt );
	_mm_store_ss( &invlen, xr );
	vec.x *= invlen;
	vec.y *= invlen;
	vec.z *= invlen;
	return sqrlen * invlen;
}
//===============================================
__forceinline float VectorNormalize( float* v )
{
	return VectorNormalize( *( reinterpret_cast< Vector* >( v ) ) );
}

//===============================================
inline float Vector::NormalizeInPlace()
{
	return VectorNormalize( *this );
}
//===============================================
inline Vector Vector::Normalized() const
{
	Vector norm = *this;
	VectorNormalize( norm );
	return norm;
}

//===============================================
inline Vector Vector::To2D() const
{
	Vector vec = *this;
	vec.z = 0.f;
	return vec;
}
//===============================================
inline Vector Vector::operator+( const Vector& v ) const
{
	Vector res{};
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

//===============================================
inline Vector Vector::operator-( const Vector& v ) const
{
	Vector res{};
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}
//===============================================
inline Vector Vector::operator*( float fl ) const
{
	Vector res{};
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}
//===============================================
inline Vector Vector::operator*( const Vector& v ) const
{
	Vector res{};
	res.x = x * v.x;
	res.y = y * v.y;
	res.z = z * v.z;
	return res;
}
//===============================================
inline Vector Vector::operator/( float fl ) const
{
	Vector res{};
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}
//===============================================
inline Vector Vector::operator/( const Vector& v ) const
{
	Vector res{};
	res.x = x / v.x;
	res.y = y / v.y;
	res.z = z / v.z;
	return res;
}
//===============================================
__forceinline float Vector::Dot( const Vector& vOther ) const
{
	const Vector& a = *this;

	return( a.x * vOther.x + a.y * vOther.y + a.z * vOther.z );
}
//===============================================
inline float Vector::Dot( const float* fOther ) const
{
	const Vector& a = *this;

	return( a.x * fOther[ 0 ] + a.y * fOther[ 1 ] + a.z * fOther[ 2 ] );
}
//===============================================
inline float* Vector::Base()
{
	return ( float* ) this;
}
//===============================================
inline float const* Vector::Base() const
{
	return ( float const* ) this;
}