#pragma once

struct matrix3x3_t
{
	matrix3x3_t() = default;
	matrix3x3_t(
		float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22 )
	{
		m_flMatVal[ 0 ][ 0 ] = m00;	m_flMatVal[ 0 ][ 1 ] = m01; m_flMatVal[ 0 ][ 2 ] = m02;
		m_flMatVal[ 1 ][ 0 ] = m10;	m_flMatVal[ 1 ][ 1 ] = m11; m_flMatVal[ 1 ][ 2 ] = m12;
		m_flMatVal[ 2 ][ 0 ] = m20;	m_flMatVal[ 2 ][ 1 ] = m21; m_flMatVal[ 2 ][ 2 ] = m22;
	}

	matrix3x3_t(
		float* m0,
		float* m1,
		float* m2 )
	{
		m_flMatVal[ 0 ][ 0 ] = m0[ 0 ];	m_flMatVal[ 0 ][ 1 ] = m0[ 1 ]; m_flMatVal[ 0 ][ 2 ] = m0[ 2 ];
		m_flMatVal[ 1 ][ 0 ] = m1[ 0 ];	m_flMatVal[ 1 ][ 1 ] = m1[ 1 ]; m_flMatVal[ 1 ][ 2 ] = m1[ 2 ];
		m_flMatVal[ 2 ][ 0 ] = m2[ 0 ];	m_flMatVal[ 2 ][ 1 ] = m2[ 1 ]; m_flMatVal[ 2 ][ 2 ] = m2[ 2 ];
	}

	float *operator[]( int i ) { return m_flMatVal[ i ]; }
	const float *operator[]( int i ) const { return m_flMatVal[ i ]; }
	float *Base() { return &m_flMatVal[ 0 ][ 0 ]; }
	const float *Base() const { return &m_flMatVal[ 0 ][ 0 ]; }
	__forceinline matrix3x3_t transpose() const
	{
		return matrix3x3_t( m_flMatVal[ 0 ][ 0 ], m_flMatVal[ 1 ][ 0 ], m_flMatVal[ 2 ][ 0 ],
							m_flMatVal[ 0 ][ 1 ], m_flMatVal[ 1 ][ 1 ], m_flMatVal[ 2 ][ 1 ],
							m_flMatVal[ 0 ][ 2 ], m_flMatVal[ 1 ][ 2 ], m_flMatVal[ 2 ][ 2 ] );
	}

	typedef double tfScalar;
	__forceinline static const matrix3x3_t &getIdentity()
	{
		static const matrix3x3_t identityMatrix( tfScalar( 1.0 ), tfScalar( 0.0 ), tfScalar( 0.0 ),
												 tfScalar( 0.0 ), tfScalar( 1.0 ), tfScalar( 0.0 ),
												 tfScalar( 0.0 ), tfScalar( 0.0 ), tfScalar( 1.0 ) );
		return identityMatrix;
	}

	__forceinline tfScalar tdotx( const float* v ) const
	{
		return m_flMatVal[ 0 ][ 0 ] * v[ 0 ] + m_flMatVal[ 1 ][ 0 ] * v[ 1 ] + m_flMatVal[ 2 ][ 0 ] * v[ 2 ];
	}
	__forceinline tfScalar tdoty( const float* v ) const
	{
		return m_flMatVal[ 0 ][ 1 ] * v[ 0 ] + m_flMatVal[ 1 ][ 1 ] * v[ 1 ] + m_flMatVal[ 2 ][ 1 ] * v[ 2 ];
	}
	__forceinline tfScalar tdotz( const float* v ) const
	{
		return m_flMatVal[ 0 ][ 2 ] * v[ 0 ] + m_flMatVal[ 1 ][ 2 ] * v[ 1 ] + m_flMatVal[ 2 ][ 2 ] * v[ 2 ];
	}

	__forceinline matrix3x3_t operator*( const matrix3x3_t& m2 ) const
	{
		return matrix3x3_t(
			m2.tdotx( m_flMatVal[ 0 ] ), m2.tdoty( m_flMatVal[ 0 ] ), m2.tdotz( m_flMatVal[ 0 ] ),
			m2.tdotx( m_flMatVal[ 1 ] ), m2.tdoty( m_flMatVal[ 1 ] ), m2.tdotz( m_flMatVal[ 1 ] ),
			m2.tdotx( m_flMatVal[ 2 ] ), m2.tdoty( m_flMatVal[ 2 ] ), m2.tdotz( m_flMatVal[ 2 ] ) );
	}

	__forceinline Vector operator*( const Vector& v )
	{
		return Vector( Vector( m_flMatVal[ 0 ] ).Dot( v ), Vector( m_flMatVal[ 1 ] ).Dot( v ), Vector( m_flMatVal[ 2 ] ).Dot( v ) );
	}

	float m_flMatVal[ 3 ][ 3 ]{};
};