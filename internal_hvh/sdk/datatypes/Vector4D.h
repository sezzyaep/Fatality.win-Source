#pragma once

class Vector4D
{
public:
	Vector4D() = default;
	Vector4D( float x, float y, float z, float w ) : x( x ), y( y ), z( z ), w( w ) {}

	// Members
	float x, y, z, w;

	inline float* Base();
	inline float const* Base() const;

	float operator[]( int i ) const;
	float& operator[]( int i );
};

inline float* Vector4D::Base()
{
	return ( float* ) this;
}

inline float const* Vector4D::Base() const
{
	return ( float const* ) this;
}

inline float& Vector4D::operator[]( int i )
{
	Assert( ( i >= 0 ) && ( i < 4 ) );
	return ( ( float* ) this )[ i ];
}

inline float Vector4D::operator[]( int i ) const
{
	Assert( ( i >= 0 ) && ( i < 4 ) );
	return ( ( float* ) this )[ i ];
}