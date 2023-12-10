#pragma once

class Color
{
public:
	Color()
	{
		*( ( int* ) this ) = 0;
	}
	Color( int _r, int _g, int _b )
	{
		SetColor( _r, _g, _b, 255 );
	}
	Color( int _r, int _g, int _b, int _a )
	{
		SetColor( _r, _g, _b, _a );
	}
	Color( float _rgb[ 3 ] )
	{
		SetColor( ( int ) ( _rgb[ 0 ] * 255.0f ), ( int ) ( _rgb[ 1 ] * 255.0f ), ( int ) ( _rgb[ 2 ] * 255.0f ), 255 );
	}
	Color( D3DCOLOR _argb )
	{
		_cl[ 0 ] = ( ( ( _argb ) & 0x00ff0000 ) >> 16 );
		_cl[ 1 ] = ( ( ( _argb ) & 0x0000ff00 ) >> 8 );
		_cl[ 2 ] = ( ( _argb ) & 0x000000ff );
		_cl[ 3 ] = ( ( ( _argb ) & 0xff000000 ) >> 24 );
	}
	Color( uintptr_t _rgb )
	{
		SetRawColor( _rgb );
	}
	Color( float _rgb[ 3 ], float _alpha )
	{
		SetColor( ( int ) ( _rgb[ 0 ] * 255.0f ), ( int ) ( _rgb[ 1 ] * 255.0f ), ( int ) ( _rgb[ 2 ] * 255.0f ), ( int ) _alpha );
	}

	Color( Color _cl, float _alpha )
	{
		SetColor( ( int ) _cl.r(), ( int ) _cl.g(), ( int ) _cl.b(), ( int ) _alpha );
	}

	void SetColor( int _r, int _g, int _b, int _a = 255 )
	{
		_cl[ 0 ] = ( unsigned char ) _r;
		_cl[ 1 ] = ( unsigned char ) _g;
		_cl[ 2 ] = ( unsigned char ) _b;
		_cl[ 3 ] = ( unsigned char ) _a;
	}

	void GetColor( int& _r, int& _g, int& _b, int& _a ) const
	{
		_r = _cl[ 0 ];
		_g = _cl[ 1 ];
		_b = _cl[ 2 ];
		_a = _cl[ 3 ];
	}

	void SetRawColor( int color32 )
	{
		*( ( int* ) this ) = color32;
	}

	int GetRawColor() const
	{
		return *( ( int* ) this );
	}

	D3DCOLOR GetD3DColor() const
	{
		return ( ( int ) ( ( ( ( _cl[ 3 ] ) & 0xff ) << 24 ) | ( ( ( _cl[ 0 ] ) & 0xff ) << 16 ) | ( ( ( _cl[ 1 ] ) & 0xff ) << 8 ) | ( ( _cl[ 2 ] ) & 0xff ) ) );
	}

	inline int r() const { return _cl[ 0 ]; }
	inline int g() const { return _cl[ 1 ]; }
	inline int b() const { return _cl[ 2 ]; }
	inline int a() const { return _cl[ 3 ]; }

	inline float rBase() const { return _cl[ 0 ] / 255.0f; }
	inline float gBase() const { return _cl[ 1 ] / 255.0f; }
	inline float bBase() const { return _cl[ 2 ] / 255.0f; }
	inline float aBase() const { return _cl[ 3 ] / 255.0f; }

	unsigned char& operator[]( int index )
	{
		return _cl[ index ];
	}

	const unsigned char& operator[]( int index ) const
	{
		return _cl[ index ];
	}

	bool operator == ( const Color& rhs ) const
	{
		return ( *( ( int* ) this ) == *( ( int* ) &rhs ) );
	}

	bool operator != ( const Color& rhs ) const
	{
		return !( operator==( rhs ) );
	}

	Color& operator=( const Color& rhs )
	{
		SetRawColor( rhs.GetRawColor() );
		return *this;
	}

	std::array<float, 3> Base()
	{
		std::array<float, 3> clr{};

		clr[ 0 ] = _cl[ 0 ] / 255.0f;
		clr[ 1 ] = _cl[ 1 ] / 255.0f;
		clr[ 2 ] = _cl[ 2 ] / 255.0f;

		return clr;
	}

	std::array<float, 4> BaseAlpha()
	{
		std::array<float, 4> clr{};

		clr[ 0 ] = _cl[ 0 ] / 255.0f;
		clr[ 1 ] = _cl[ 1 ] / 255.0f;
		clr[ 2 ] = _cl[ 2 ] / 255.0f;
		clr[ 3 ] = _cl[ 3 ] / 255.0f;

		return clr;
	}

	float hue() const
	{
		float red = _cl[ 0 ] / 255.0f;
		float green = _cl[ 1 ] / 255.0f;
		float blue = _cl[ 2 ] / 255.0f;

		float mx = std::max( red, std::max( green, blue ) );
		float mn = std::min( red, std::min( green, blue ) );
		if ( mx == mn )
			return 0.f;

		float delta = mx - mn;

		float hue = 0.f;
		if ( mx == red )
			hue = ( green - blue ) / delta;
		else if ( mx == green )
			hue = 2.f + ( blue - red ) / delta;
		else
			hue = 4.f + ( red - green ) / delta;

		hue *= 60.f;
		if ( hue < 0.f )
			hue += 360.f;

		return hue / 360.f;
	}

	float saturation() const
	{
		float red = _cl[ 0 ] / 255.0f;
		float green = _cl[ 1 ] / 255.0f;
		float blue = _cl[ 2 ] / 255.0f;

		float mx = std::max( red, std::max( green, blue ) );
		float mn = std::min( red, std::min( green, blue ) );

		float delta = mx - mn;

		if ( mx == 0.f )
			return delta;

		return delta / mx;
	}

	float brightness() const
	{
		float red = _cl[ 0 ] / 255.0f;
		float green = _cl[ 1 ] / 255.0f;
		float blue = _cl[ 2 ] / 255.0f;

		return std::max( red, std::max( green, blue ) );
	}

	static Color FromHSB( float hue, float saturation, float brightness )
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - ( int ) h;
		float p = brightness * ( 1.0f - saturation );
		float q = brightness * ( 1.0f - saturation * f );
		float t = brightness * ( 1.0f - ( saturation * ( 1.0f - f ) ) );

		if ( h < 1 )
		{
			return Color(
				( unsigned char ) ( brightness * 255 ),
				( unsigned char ) ( t * 255 ),
				( unsigned char ) ( p * 255 )
			);
		}
		else if ( h < 2 )
		{
			return Color(
				( unsigned char ) ( q * 255 ),
				( unsigned char ) ( brightness * 255 ),
				( unsigned char ) ( p * 255 )
			);
		}
		else if ( h < 3 )
		{
			return Color(
				( unsigned char ) ( p * 255 ),
				( unsigned char ) ( brightness * 255 ),
				( unsigned char ) ( t * 255 )
			);
		}
		else if ( h < 4 )
		{
			return Color(
				( unsigned char ) ( p * 255 ),
				( unsigned char ) ( q * 255 ),
				( unsigned char ) ( brightness * 255 )
			);
		}
		else if ( h < 5 )
		{
			return Color(
				( unsigned char ) ( t * 255 ),
				( unsigned char ) ( p * 255 ),
				( unsigned char ) ( brightness * 255 )
			);
		}
		else
		{
			return Color(
				( unsigned char ) ( brightness * 255 ),
				( unsigned char ) ( p * 255 ),
				( unsigned char ) ( q * 255 )
			);
		}
	}

	D3DCOLOR direct() const
	{
		return D3DCOLOR_ARGB( a(), r(), g(), b() );
	}

	Color fade_to( const Color& other, float lerp ) const
	{
		return Color( r() + lerp * ( other.r() - r() ), g() + lerp * ( other.g() - g() ), b() + lerp * ( other.b() - b() ), a() + lerp * ( other.a() - a() ) );
	}

	static Color Red( const int alpha = 255 ) { return Color( 255, 0, 0, alpha ); }
	static Color Green( const int alpha = 255 ) { return Color( 0, 255, 0, alpha ); }
	static Color Blue( const int alpha = 255 ) { return Color( 0, 0, 255, alpha ); }
	static Color LightBlue( const int alpha = 255 ) { return Color( 100, 100, 255, alpha ); }
	static Color Grey( const int alpha = 255 ) { return Color( 128, 128, 128, alpha ); }
	static Color DarkGrey( const int alpha = 255 ) { return Color( 45, 45, 45, alpha ); }
	static Color Black( const int alpha = 255 ) { return Color( 0, 0, 0, alpha ); }
	static Color White( const int alpha = 255 ) { return Color( 255, 255, 255, alpha ); }
	static Color Purple( const int alpha = 255 ) { return Color( 220, 0, 220, alpha ); }
	static Color Pink( const int alpha = 255 ) { return Color( 255, 105, 180, alpha ); }
	static Color Lime( const int alpha = 255 ) { return Color( 160, 255, 0, alpha ); }


	static Color BlueAccent( const int alpha = 255 ) { return Color( 0, 153, 204, alpha ); }
	static Color RedAccent( const int alpha = 255 ) { return Color( 170, 33, 33, alpha ); }

	//Menu
	static Color Background() { return Color( 55, 55, 55 ); }
	static Color FrameBorder() { return Color( 80, 80, 80 ); }
	static Color MainText() { return Color( 230, 230, 230 ); }
	static Color HeaderText() { return Color( 49, 124, 230 ); }
	static Color CurrentTab() { return Color( 55, 55, 55 ); }
	static Color Tabs() { return Color( 23, 23, 23 ); }
	static Color Highlight() { return Color( 49, 124, 230 ); }
	static Color ElementBorder() { return Color( 0, 0, 0 ); }
	static Color SliderScroll() { return Color( 78, 143, 230 ); }


private:
	unsigned char _cl[ 4 ]{};
};