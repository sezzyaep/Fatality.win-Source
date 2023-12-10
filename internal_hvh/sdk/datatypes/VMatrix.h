#pragma once

struct VMatrix
{
	VMatrix() = default;

	float m[ 4 ][ 4 ];

	inline float* operator[]( int i )
	{
		return m[ i ];
	}

	inline const float* operator[]( int i ) const
	{
		return m[ i ];
	}

	inline const matrix3x4_t& as_3x4() const
	{
		return *( ( const matrix3x4_t* ) this );
	}
};