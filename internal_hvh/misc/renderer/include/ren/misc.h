#ifndef EV0LVE_RENDERER_MISC_H
#define EV0LVE_RENDERER_MISC_H

#include <cstdint>
#include <string>
#include <ren/font.h>

namespace evo::ren
{
	constexpr auto pi = 3.14159265358979323846;

	inline static float deg2rad( float d )
	{
		return d * ( ( float ) pi / 180.f );
	}

	inline static float rad2deg( float r )
	{
		return r * ( 180.f / ( float ) pi );
	}

	inline static bool get_bit( uint32_t N, uint32_t V )
	{
		return ( V >> N ) & 1;
	}

	inline static void set_bit( uint32_t N, uint32_t& V )
	{
		V |= 1 << N;
	}

	inline static void unset_bit( uint32_t N, uint32_t& V )
	{
		V &= ~( 1 << N );
	}

	inline static std::string utf8_encode( uint32_t codepoint )
	{
		static const auto set_octet = [] ( const uint32_t w, int offset, int x, uint32_t& o )
		{
			for ( auto i = 0; i < ( 6 - x ); i++ )
			{
				if ( get_bit( i + offset, w ) )
					set_bit( i, o );
			}

			for ( auto i = 0; i < x; i++ )
				set_bit( 6 - i, o );

			set_bit( 7, o );
		};

		std::string str{};
		if ( codepoint < 0x80 )
			str += static_cast< char >( codepoint );
		else if ( codepoint < 0x800 )
		{
			uint32_t octet_1{};
			uint32_t octet_2{};

			set_octet( codepoint, 0, 0, octet_2 );
			set_octet( codepoint, 6, 1, octet_1 );

			str += static_cast< char >( octet_1 );
			str += static_cast< char >( octet_2 );
		}
		else if ( codepoint < 0x10000 )
		{
			uint32_t octet_1{};
			uint32_t octet_2{};
			uint32_t octet_3{};

			set_octet( codepoint, 0, 0, octet_3 );
			set_octet( codepoint, 6, 0, octet_2 );
			set_octet( codepoint, 12, 2, octet_1 );

			str += static_cast< char >( octet_1 );
			str += static_cast< char >( octet_2 );
			str += static_cast< char >( octet_3 );
		}
		else if ( codepoint < 0x110000 )
		{
			uint32_t octet_1{};
			uint32_t octet_2{};
			uint32_t octet_3{};
			uint32_t octet_4{};

			set_octet( codepoint, 0, 0, octet_4 );
			set_octet( codepoint, 6, 0, octet_3 );
			set_octet( codepoint, 12, 0, octet_2 );
			set_octet( codepoint, 24, 3, octet_1 );

			str += static_cast< char >( octet_1 );
			str += static_cast< char >( octet_2 );
			str += static_cast< char >( octet_3 );
			str += static_cast< char >( octet_4 );
		}

		return str;
	}

	inline static uint8_t utf8_decode( const char* str, uint32_t& codepoint )
	{
		static auto set_octet = [] ( char _, uint32_t& c, uint32_t o, uint32_t m )
		{
			for ( auto i = 0; i < m; i++ )
			{
				if ( get_bit( i, _ ) )
					set_bit( i + o, c );
			}
		};

		codepoint = 0;
		if ( !get_bit( 7, str[ 0 ] ) )
		{
			codepoint = ( uint8_t ) str[ 0 ];
			return 1;
		}

		if ( !get_bit( 6, str[ 0 ] ) )
			return 1;

		if ( !get_bit( 5, str[ 0 ] ) )
		{
			set_octet( str[ 1 ], codepoint, 0, 6 );
			set_octet( str[ 0 ], codepoint, 6, 5 );

			return 2;
		}

		if ( !get_bit( 4, str[ 0 ] ) )
		{
			set_octet( str[ 2 ], codepoint, 0, 6 );
			set_octet( str[ 1 ], codepoint, 6, 6 );
			set_octet( str[ 0 ], codepoint, 12, 4 );

			return 3;
		}

		if ( !get_bit( 3, str[ 0 ] ) )
		{
			set_octet( str[ 3 ], codepoint, 0, 6 );
			set_octet( str[ 2 ], codepoint, 6, 6 );
			set_octet( str[ 1 ], codepoint, 12, 6 );
			set_octet( str[ 0 ], codepoint, 18, 3 );

			return 4;
		}

		return 1;
	}

	inline static vec2 text_size( const std::shared_ptr<font_base>& f, const std::string& str )
	{
		vec2 size{ 0.f, ( float ) f->height };
		float max_width{ -1.f };

		auto last_c = 0; auto is_first = true;
		for ( auto i = 0; i < str.length(); is_first = false )
		{
			if ( str[ i ] == ' ' || str[ i ] == '\t' )
			{
				size.x += f->height * 0.25f * ( str[ i ] == ' ' ? 1 : 4 );
				i++;

				last_c = 0;
				continue;
			}

			if ( str[ i ] == '\n' )
			{
				size.y += f->height + f->descent + f->line_gap;
				if ( max_width < size.x )
				{
					max_width = size.x;
					size.x = 0.f;
				}

				i++;

				last_c = 0;
				continue;
			}

			uint32_t codepoint{};
			i += utf8_decode( str.data() + i, codepoint );

			const auto g = f->find( codepoint );
			if ( !g.bmp )
				continue;

			if ( is_first )
				size.x += g.first_offset.x;

			size.x += g.offset.x + g.size.x + f->get_advance( last_c, codepoint ) + g.extra_offset.x;
			last_c = codepoint;
		}

		if ( max_width > -1.f )
			size.x = max_width;

		size.y -= f->descent + f->line_gap;
		return size.round();
	}

	inline static vec2 rotate_point( const float f, const vec2& p, const vec2& c )
	{
		return c + vec2(
				cosf( f ) * ( p.x - c.x ) - sinf( f ) * ( p.y - c.y ),
				sinf( f ) * ( p.x - c.x ) + cosf( f ) * ( p.y - c.y )
		);
	}

	inline static vec2 get_center( const std::vector<vec2>& ps )
	{
		vec2 c{};
		for ( auto& p : ps )
			c += p;

		return c / ps.size();
	}
}

#endif //EV0LVE_RENDERER_MISC_H
