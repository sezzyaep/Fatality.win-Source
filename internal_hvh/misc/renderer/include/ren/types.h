#ifndef EV0LVE_RENDERER_TYPES_H
#define EV0LVE_RENDERER_TYPES_H

#include <cmath>
#include <cstdint>
#include <algorithm>

namespace evo::ren
{
	class vec2
	{
	public:
		vec2() = default;
		explicit vec2( float a ) : x( a ), y( a )
		{}
		vec2( float a, float b ) : x( a ), y( b )
		{}

		operator float* ( )
		{
			return &x;
		}
		bool operator==( const vec2& o ) const
		{
			return x == o.x && y == o.y;
		}
		bool operator==( const float o ) const
		{
			return x == y && y == o;
		}
		bool operator!=( const vec2& o ) const
		{
			return !( *this == o );
		}
		bool operator!=( const float o ) const
		{
			return!( *this == o );
		}
		bool operator>( const vec2& o ) const
		{
			return x > o.x && y > o.y;
		}
		bool operator>( const float o ) const
		{
			return x > o && y > o;
		}
		bool operator<( const vec2& o ) const
		{
			return o > *this;
		}
		bool operator<( const float o ) const
		{
			return vec2( o ) > *this;
		}
		bool operator>=( const vec2& o ) const
		{
			return x >= o.x && y >= o.y;
		}
		bool operator>=( const float o ) const
		{
			return x >= o && y >= o;
		}
		bool operator<=( const vec2& o ) const
		{
			return o >= *this;
		}
		bool operator<=( const float o ) const
		{
			return vec2( o, o ) >= *this;
		}
		vec2 operator+( const vec2& o ) const
		{
			return vec2( x + o.x, y + o.y );
		}
		vec2 operator+( const float o ) const
		{
			return vec2( x + o, y + o );
		}
		vec2 operator-() const
		{
			return vec2( -x, -y );
		}
		vec2 operator-( const vec2& o ) const
		{
			return *this + ( -o );
		}
		vec2 operator-( const float o ) const
		{
			return *this + ( -o );
		}
		vec2 operator*( const vec2& o ) const
		{
			return vec2( x * o.x, y * o.y );
		}
		vec2 operator*( const float o ) const
		{
			return vec2( x * o, y * o );
		}
		vec2 operator/( const vec2& o ) const
		{
			return vec2( x / o.x, y / o.y );
		}
		vec2 operator/( const float o ) const
		{
			return vec2( x / o, y / o );
		}
		void operator+=( const vec2& o )
		{
			x += o.x;
			y += o.y;
		}
		void operator+=( const float o )
		{
			x += o;
			y += o;
		}
		void operator-=( const vec2& o )
		{
			x -= o.x;
			y -= o.y;
		}
		void operator-=( const float o )
		{
			x -= o;
			y -= o;
		}
		void operator*=( const vec2& o )
		{
			x *= o.x;
			y *= o.y;
		}
		void operator*=( const float o )
		{
			x *= o;
			y *= o;
		}
		void operator/=( const vec2& o )
		{
			x /= o.x;
			y /= o.y;
		}
		void operator/=( const float o )
		{
			x /= o;
			y /= o;
		}

		vec2 floor() const
		{
			return vec2( floorf( x ), floorf( y ) );
		}
		vec2 ceil() const
		{
			return vec2( ceilf( x ), ceilf( y ) );
		}
		vec2 round() const
		{
			return vec2( roundf( x ), roundf( y ) );
		}
		vec2& floor()
		{
			x = floorf( x );
			y = floorf( y );
			return *this;
		}
		vec2& ceil()
		{
			x = ceilf( x );
			y = ceilf( y );
			return *this;
		}
		vec2& round()
		{
			x = roundf( x );
			y = roundf( y );
			return *this;
		}
		float len_sqr() const
		{
			return x * x + y * y;
		}
		float len() const
		{
			return sqrtf( len_sqr() );
		}
		bool is_zero() const
		{
			return *this == 0.f;
		}

		float x{}, y{};
	};

	class vec4
	{
	public:
		vec4() = default;
		explicit vec4( float a ) : x( a ), y( a ), z( a ), w( a )
		{}
		vec4( const vec2& a, const vec2& b ) : x( a.x ), y( a.y ), z( b.x ), w( b.y )
		{}
		vec4( float a, float b, float c, float d ) : x( a ), y( b ), z( c ), w( d )
		{}

		operator float* ( )
		{
			return &x;
		}
		bool operator==( const vec4& o ) const
		{
			return x == o.x && y == o.y && z == o.z && w == o.w;
		}
		bool operator==( const float o ) const
		{
			return x == y && y == z && z == w && w == o;
		}
		bool operator !=( const vec4& o ) const
		{
			return !( *this == o );
		}
		bool operator !=( const float o ) const
		{
			return!( *this == o );
		}
		bool operator>( const vec4& o ) const
		{
			return x > o.x && y > o.y && z > o.z && w > o.w;
		}
		bool operator>( const float o ) const
		{
			return x > o && y > o && z > o && w > o;
		}
		bool operator<( const vec4& o ) const
		{
			return o > *this;
		}
		bool operator<( const float o ) const
		{
			return vec4( o ) > *this;
		}
		bool operator>=( const vec4& o ) const
		{
			return x >= o.x && y >= o.y && z >= o.z && w >= o.w;
		}
		bool operator>=( const float o ) const
		{
			return x >= o && y >= o && z >= o && w >= o;
		}
		bool operator<=( const vec4& o ) const
		{
			return o >= *this;
		}
		bool operator<=( const float o ) const
		{
			return vec4( o ) >= *this;
		}
		vec4 operator+( const vec4& o ) const
		{
			return vec4( x + o.x, y + o.y, z + o.z, w + o.w );
		}
		vec4 operator+( const float o ) const
		{
			return vec4( x + o, y + o, z + o, w + o );
		}
		vec4 operator-() const
		{
			return vec4( -x, -y, -z, -w );
		}
		vec4 operator-( const vec4& o ) const
		{
			return *this + ( -o );
		}
		vec4 operator-( const float o ) const
		{
			return *this + ( -o );
		}
		vec4 operator*( const vec4& o ) const
		{
			return vec4( x * o.x, y * o.y, z * o.z, w * o.w );
		}
		vec4 operator*( const float o ) const
		{
			return vec4( x * o, y * o, z * o, w * o );
		}
		vec4 operator/( const vec4& o ) const
		{
			return vec4( x / o.x, y / o.y, z / o.z, w / o.w );
		}
		vec4 operator/( const float o ) const
		{
			return vec4( x / o, y / o, z / o, w / o );
		}
		void operator+=( const vec4& o )
		{
			x += o.x;
			y += o.y;
			z += o.z;
			w += o.w;
		}
		void operator+=( const float o )
		{
			x += o;
			y += o;
			z += o;
			w += o;
		}
		void operator-=( const vec4& o )
		{
			x -= o.x;
			y -= o.y;
			z -= o.z;
			w -= o.w;
		}
		void operator-=( const float o )
		{
			x -= o;
			y -= o;
			z -= o;
			w -= o;
		}
		void operator*=( const vec4& o )
		{
			x *= o.x;
			y *= o.y;
			z *= o.z;
			w *= o.w;
		}
		void operator*=( const float o )
		{
			x *= o;
			y *= o;
			z *= o;
			w *= o;
		}
		void operator/=( const vec4& o )
		{
			x /= o.x;
			y /= o.y;
			z /= o.z;
			w /= o.w;
		}
		void operator/=( const float o )
		{
			x /= o;
			y /= o;
			z /= o;
			w /= o;
		}

		vec4& floor()
		{
			x = floorf( x );
			y = floorf( y );
			z = floorf( z );
			w = floorf( w );
			return *this;
		}
		vec4& ceil()
		{
			x = ceilf( x );
			y = ceilf( y );
			z = ceilf( z );
			w = ceilf( w );
			return *this;
		}
		vec4& round()
		{
			x = roundf( x );
			y = roundf( y );
			z = roundf( z );
			w = roundf( w );
			return *this;
		}
		float len_sqr() const
		{
			return x * x + y * y + z * z + w * w;
		}
		float len() const
		{
			return sqrtf( len_sqr() );
		}
		bool is_zero() const
		{
			return *this == 0.f;
		}

		float x{}, y{}, z{}, w{};
	};

	class rect
	{
	public:
		rect() = default;
		rect( const vec4& v ) : mins( v.x, v.y ), maxs( v.z, v.w )
		{}
		explicit rect( const vec2& v ) : mins( v ), maxs( v )
		{}
		rect( float x, float y ) : mins( x, y ), maxs( x, y )
		{}
		rect( const vec2& mi, const vec2& ma ) : mins( mi ), maxs( ma )
		{}
		rect( float x1, float y1, float x2, float y2 ) : mins( x1, y1 ), maxs( x2, y2 )
		{}

		float width() const
		{
			return maxs.x - mins.x;
		}
		rect width( float v ) const
		{
			return rect( mins.x, mins.y, mins.x + v, maxs.y );
		}
		float height() const
		{
			return maxs.y - mins.y;
		}
		rect height( float v ) const
		{
			return rect( mins.x, mins.y, maxs.x, mins.y + v );
		}
		vec2 size() const
		{
			return maxs - mins;
		}
		rect size( const vec2& s ) const
		{
			return rect( mins, mins + s );
		}

		rect translate( const vec2& v ) const
		{
			return rect( mins + v, maxs + v );
		}
		rect& translate( const vec2& v )
		{
			mins += v;
			maxs += v;
			return *this;
		}
		rect& floor()
		{
			mins.floor();
			maxs.floor();
			return *this;
		}
		rect& ceil()
		{
			mins.ceil();
			maxs.ceil();
			return *this;
		}
		rect& round()
		{
			mins.round();
			maxs.round();
			return *this;
		}
		bool is_zero() const
		{
			return mins.is_zero() && maxs.is_zero();
		}

		rect shrink( float f ) const
		{
			return rect( mins + f, maxs - f );
		}
		rect expand( float f ) const
		{
			return shrink( -f );
		}
		rect margin_left( float f ) const
		{
			return rect( *this ).translate( vec2( f, 0 ) );
		}
		rect margin_top( float f ) const
		{
			return rect( *this ).translate( vec2( 0, f ) );
		}
		rect margin_right( float f ) const
		{
			return margin_left( -f );
		}
		rect margin_bottom( float f ) const
		{
			return margin_top( -f );
		}
		rect padding_left( float f ) const
		{
			return rect( mins + vec2( f, 0 ), maxs );
		}
		rect padding_top( float f ) const
		{
			return rect( mins + vec2( 0, f ), maxs );
		}
		rect padding_right( float f ) const
		{
			return rect( mins, maxs - vec2( f, 0 ) );
		}
		rect padding_bottom( float f ) const
		{
			return rect( mins, maxs - vec2( 0, f ) );
		}

		rect intersect( const rect& r ) const
		{
			return rect( vec2( r.mins.x < mins.x ? mins.x : r.mins.x, r.mins.y < mins.y ? mins.y : r.mins.y ),
						vec2( r.maxs.x > maxs.x ? maxs.x : r.maxs.x, r.maxs.y > maxs.y ? maxs.y : r.maxs.y ) );
		}
		bool contains( const rect& r ) const
		{
			return r.mins >= mins && r.maxs <= maxs;
		}
		bool contains( const vec2& r ) const
		{
			return contains( rect( r, r ) );
		}

		vec2 tl() const
		{
			return mins;
		}
		vec2 tr() const
		{
			return vec2( maxs.x, mins.y );
		}
		vec2 br() const
		{
			return maxs;
		}
		vec2 bl() const
		{
			return vec2( mins.x, maxs.y );
		}
		vec2 center() const
		{
			return mins + size() * 0.5f;
		}
		vec2 circle( float r ) const
		{
			return center() + vec2( width() * 0.5f * cosf( r ), height() * 0.5f * sinf( r ) );
		}

		vec2 mins{}, maxs{};
	};

	class color
	{
	public:
		color() = default;
		color( int r, int g, int b, int a = 255 ) :
			value( ( float ) r / 255.f, ( float ) g / 255.f, ( float ) b / 255.f, ( float ) a / 255.f )
		{
			clamp();
		}

		color( float r, float g, float b, float a = 1.f ) : value( r, g, b, a )
		{
			clamp();
		}

		color( float r, float g, float b, float a, bool legacy ) : value( r / 255.f, g / 255.f, b / 255.f, a / 255.f )
		{
			clamp();
		}

		color( const color& col, float a ) : value( col.r() / 255.f, col.g() / 255.f, col.b() / 255.f, a / 255.f )
		{}

		color( uint32_t c ) : color(
		( int ) ( ( ( c ) & 0x00ff0000 ) >> 16 ),
		( int ) ( ( ( c ) & 0x0000ff00 ) >> 8 ),
		( int ) ( ( c ) & 0x000000ff ),
		( int ) ( ( ( c ) & 0xff000000 ) >> 24 ) )
		{}

		color( const vec4& v ) : value( v )
		{
			clamp();
		}

		operator vec4() const
		{
			return value;
		}
		operator uint32_t() const
		{
			return rgba();
		}

		bool operator==( const color& other ) const
		{
			return other.value == value;
		}

		unsigned long d3d() const
		{
			return bgra();
		}

		uint32_t rgba() const
		{
			return 	( uint32_t ) ( value.x * 255.f ) |
				( uint32_t ) ( value.y * 255.f ) << 8 |
				( uint32_t ) ( value.z * 255.f ) << 16 |
				( uint32_t ) ( value.w * 255.f ) << 24;
		}
		uint32_t argb() const
		{
			return 	( uint32_t ) ( value.w * 255.f ) |
				( uint32_t ) ( value.x * 255.f ) << 8 |
				( uint32_t ) ( value.y * 255.f ) << 16 |
				( uint32_t ) ( value.z * 255.f ) << 24;
		}
		uint32_t bgra() const
		{
			return 	( uint32_t ) ( value.z * 255.f ) |
				( uint32_t ) ( value.y * 255.f ) << 8 |
				( uint32_t ) ( value.x * 255.f ) << 16 |
				( uint32_t ) ( value.w * 255.f ) << 24;
		}
		uint32_t abgr() const
		{
			return 	( uint32_t ) ( value.w * 255.f ) |
				( uint32_t ) ( value.z * 255.f ) << 8 |
				( uint32_t ) ( value.y * 255.f ) << 16 |
				( uint32_t ) ( value.z * 255.f ) << 24;
		}

		color mod_a( float v ) const
		{
			return mod_a( ( int ) ( v * 255.f ) );
		}
		color mod_a( int v ) const
		{
			return color( *this ).a( std::clamp( v, 0, ( int ) a() ) );
		}

		uint8_t r() const
		{
			return value.x * 255.f;
		}
		uint8_t g() const
		{
			return value.y * 255.f;
		}
		uint8_t b() const
		{
			return value.z * 255.f;
		}
		uint8_t a() const
		{
			return value.w * 255.f;
		}
		color& r( float v )
		{
			value.x = v;
			return this->clamp();
		}
		color& g( float v )
		{
			value.y = v;
			return this->clamp();
		}
		color& b( float v )
		{
			value.z = v;
			return this->clamp();
		}
		color& a( float v )
		{
			value.w = v;
			return this->clamp();
		}
		color& r( int v )
		{
			return r( ( float ) v / 255.f );
		}
		color& g( int v )
		{
			return g( ( float ) v / 255.f );
		}
		color& b( int v )
		{
			return b( ( float ) v / 255.f );
		}
		color& a( int v )
		{
			return a( ( float ) v / 255.f );
		}

		uint16_t h() const
		{
			const auto _max = fmax( value.x, fmax( value.y, value.z ) );
			const auto _min = fmin( value.x, fmin( value.y, value.z ) );
			const auto d = _max - _min;

			auto _h = 0U;
			if ( _max == _min )
				_h = 0U;
			if ( _max == value.x )
				_h = ( uint16_t ) ( 60 * ( ( value.y - value.z ) / d ) + 360 ) % 360;
			if ( _max == value.y )
				_h = ( uint16_t ) ( 60 * ( ( value.z - value.x ) / d ) + 120 ) % 360;
			if ( _max == value.z )
				_h = ( uint16_t ) ( 60 * ( ( value.x - value.y ) / d ) + 240 ) % 360;

			return _h;
		}
		float s() const
		{
			const auto _max = fmax( value.x, fmax( value.y, value.z ) );
			const auto _min = fmin( value.x, fmin( value.y, value.z ) );
			const auto d = _max - _min;

			return _max != 0.f ? d / _max : 0.f;
		}
		float v() const
		{
			return fmax( value.x, fmax( value.y, value.z ) );
		}
		color& hsv( uint16_t _h, float _s, float _v, float a = -1.f )
		{
			const auto hi = ( ( uint32_t ) floorf( ( float ) _h / 60.f ) ) % 6;
			const auto f = ( float ) _h / 60.f - floorf( ( float ) _h / 60.f );

			const auto _a = ( int ) ( _v * 255.f );
			const auto _p = ( int ) ( _a * ( 1.f - _s ) );
			const auto _q = ( int ) ( _a * ( 1.f - f * _s ) );
			const auto _t = ( int ) ( _a * ( 1.f - ( 1.f - f ) * _s ) );

			if ( hi == 0 ) value = vec4( _a / 255.f, _t / 255.f, _p / 255.f, value.w );
			if ( hi == 1 ) value = vec4( _q / 255.f, _a / 255.f, _p / 255.f, value.w );
			if ( hi == 2 ) value = vec4( _p / 255.f, _a / 255.f, _t / 255.f, value.w );
			if ( hi == 3 ) value = vec4( _p / 255.f, _q / 255.f, _a / 255.f, value.w );
			if ( hi == 4 ) value = vec4( _t / 255.f, _p / 255.f, _a / 255.f, value.w );
			if ( hi == 5 ) value = vec4( _a / 255.f, _p / 255.f, _q / 255.f, value.w );

			if ( a > -1.f )
				value.w = a;

			return this->clamp();
		}

		color& clamp()
		{
			value.x = std::clamp( value.x, 0.f, 1.f );
			value.y = std::clamp( value.y, 0.f, 1.f );
			value.z = std::clamp( value.z, 0.f, 1.f );
			value.w = std::clamp( value.w, 0.f, 1.f );
			return *this;
		}

		vec4 value{};

	public:
		static color white() { return color( 255, 255, 255, 255 ); }
		static color white_transparent() { return color( 255, 255, 255, 0 ); }
		static color black() { return color( 0, 0, 0, 255 ); }
		static color black_transparent() { return color( 0, 0, 0, 0 ); }

		static color gray( float brightness, float alpha = 1.f )
		{
			return color( brightness, brightness, brightness, alpha );
		}

		static color percent( int p, int alpha = 255 )
		{
			return color(
					p < 50 ? 200 : static_cast< uint32_t >( floorf( 200.f - ( p * 2.f - 100.f ) * 200.f / 100.f ) ),
					p > 50 ? 200 : static_cast< uint32_t >( floorf( ( p * 2.f ) * 200.f / 100.f ) ),
					13, alpha
			);
		}

		static color percent( float p, float alpha = 1.f )
		{
			return percent( static_cast< int >( p * 100.f ), static_cast< int >( alpha * 255.f ) );
		}

		static color interpolate( const color& a, const color& b, float i )
		{
			return color( a.value.x + ( b.value.x - a.value.x ) * i,
				a.value.y + ( b.value.y - a.value.y ) * i,
				a.value.z + ( b.value.z - a.value.z ) * i,
				a.value.w + ( b.value.w - a.value.w ) * i );
		}
	};

	class rect_legacy : public rect
	{
	public:
		rect_legacy( const vec2& mi, const vec2& wh ) : rect( mi, mi + wh ) {}
	};
}

#endif //EV0LVE_RENDERER_TYPES_H
