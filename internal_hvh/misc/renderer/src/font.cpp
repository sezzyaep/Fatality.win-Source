#include <ren/font.h>

#include <fstream>
#include <stdexcept>

#include <deps/stb/stb_rect_pack.h>
#include <deps/stb/stb_truetype.h>

#include "../cthash.h"

using namespace evo::ren;

font::font( const std::string& path, float size, uint32_t flg, uint32_t from, uint32_t to )
{
	height = size;
	flags = flg;
	mins = from;
	maxs = to;

	fnt = new stbtt_fontinfo{};

	std::ifstream file( path, std::ios::binary | std::ios::in );
	if ( !file.is_open() )
	{
		if ( path.find( '/' ) == std::string::npos )
		{
			char win_dir_arr[ 256 ]{};
			GetSystemWindowsDirectoryA( win_dir_arr, 256 );
			file = std::ifstream( win_dir_arr + std::string( xorstr_( "/fonts/" ) ) + path, std::ios::binary | std::ios::in );
		}

		if ( !file.is_open() )
			throw std::runtime_error( xorstr_( "Failed to open font file" ) );
	}

	file.seekg( 0, file.end );
	const auto sz = file.tellg();
	file.seekg( 0, file.beg );

	auto mem = ( char* ) malloc( sz );
	file.read( mem, sz );
	file.close();

	font_data = ( uint8_t* ) malloc( sz );
	font_size = sz;

	memcpy( font_data, mem, sz );
	free( mem );

	if ( !stbtt_InitFont( fnt, font_data, 0 ) )
		throw std::runtime_error( xorstr_( "Failed to load font file" ) );
}

font::font( void* mem, size_t sz, float size, uint32_t flg, uint32_t from, uint32_t to )
{
	height = size;
	flags = flg;
	mins = from;
	maxs = to;

	fnt = new stbtt_fontinfo{};

	font_data = ( uint8_t* ) malloc( sz );
	font_size = sz;

	memcpy( font_data, mem, sz );

	if ( !stbtt_InitFont( fnt, font_data, 0 ) )
		throw std::runtime_error( xorstr_( "Failed to load font file" ) );
}

font::~font()
{
	destroy();
	free( font_data );
	delete fnt;
}

void font::create()
{
	if ( !glyphs.empty() )
		return;

	struct glyph_data
	{
		uint32_t codepoint{};
		uint32_t glyph_index{};
	};

	std::vector<glyph_data> cp_available{};
	for ( auto n = mins; n < maxs; n++ )
	{
		if ( auto idx = stbtt_FindGlyphIndex( fnt, n ) )
			cp_available.emplace_back( glyph_data{ n, ( uint32_t ) idx } );
	}

	scale = stbtt_ScaleForPixelHeight( fnt, height );

	int a{}, d{}, g{};
	stbtt_GetFontVMetrics( fnt, &a, &d, &g );

	ascent = a * scale;
	descent = d * scale;
	line_gap = g * scale;

	for ( const auto& c : cp_available )
	{
		glyphs[ c.codepoint ] = glyph( c.glyph_index, as<font>() );
		glyphs[ c.codepoint ].create();
	}
}

void font::destroy()
{
	if ( glyphs.empty() )
		return;

	for ( auto& g : glyphs )
		g.second.destroy();

	glyphs.clear();
}

float font::get_advance( uint32_t a, uint32_t b )
{
	const auto i = kerning_index( a, b );
	if ( kerning_cache.find( i ) != kerning_cache.end() )
		return kerning_cache[ i ];

	// i don't trust this one.
	const auto adv = stbtt_GetCodepointKernAdvance( fnt, a, b );
	kerning_cache[ i ] = adv < 0 ? 0 : adv; // are you high?

	return kerning_cache[ i ];
}

void glyph::create()
{
	if ( is_created )
		return;

	int w{}, h{};
	int x_o{}, y_o{};

	const auto orig_bmp = stbtt_GetGlyphBitmap( owner->get_font_info(), owner->scale, owner->scale, glyph_index, &w, &h,
											   &x_o, &y_o );
	if ( !orig_bmp )
	{
		is_created = true;
		return;
	}

	const int has_shadow = !!( owner->flags & ff_shadow );
	const int has_outline = !!( owner->flags & ff_outline );

	const auto size_ext = min( has_shadow + has_outline * 2, 2 );

	size = vec2( w, h ) + static_cast< float >( size_ext );
	offset = vec2( x_o, y_o );
	extra_offset = vec2( -static_cast< float >( size_ext ) );
	first_offset = vec2( -static_cast< float >( has_outline ) );

	const auto bmp_size = size.x * size.y * 4;

	auto rgba = new uint8_t[ bmp_size ];
	memset( rgba, 0, bmp_size );

	for ( auto y = 0; y < h; y++ )
	{
		for ( auto x = 0; x < w; x++ )
		{
			const auto m_x = x + has_outline;
			const auto m_y = y + has_outline;

			auto data = &rgba[ ( m_x + ( m_y * ( uint32_t ) size.x ) ) * 4 ];
			data[ 0 ] = data[ 1 ] = data[ 2 ] = 0xFF;
			data[ 3 ] = orig_bmp[ x + ( y * w ) ];
		}
	}

	if ( has_shadow )
		apply_effect_shadow( rgba, ( uint32_t ) size.x, ( uint32_t ) size.y );
	if ( has_outline )
		apply_effect_outline( rgba, ( uint32_t ) size.x, ( uint32_t ) size.y );

	bmp = std::make_shared<texture>( rgba, ( uint32_t ) size.x, ( uint32_t ) size.y, ( uint32_t ) size.x * 4 );

	delete[] rgba;
	stbtt_FreeBitmap( orig_bmp, nullptr );

	is_created = true;
}

void glyph::destroy()
{
	if ( !is_created )
		return;

	if ( bmp )
		bmp->destroy();

	is_created = false;
}

void glyph_base::apply_effect_shadow( uint8_t* buffer, uint32_t w, uint32_t h )
{
	auto temp_buf = new uint8_t[ w * h * 4 ]{};
	for ( auto y = 0; y < h - 1; y++ )
	{
		for ( auto x = 0; x < w - 1; x++ )
		{
			pixel o_px( &buffer[ ( y * w + x ) * 4 ] );
			pixel n_px( &temp_buf[ ( ( y + 1 ) * w + ( x + 1 ) ) * 4 ] );

			n_px.write( 0, 0, 0, o_px.alpha() );
		}
	}

	for ( auto y = 0; y < h; y++ )
	{
		for ( auto x = 0; x < w; x++ )
		{
			pixel r_px( &buffer[ ( y * w + x ) * 4 ] );
			pixel s_px( &temp_buf[ ( y * w + x ) * 4 ] );

			s_px.blend( r_px, r_px );
		}
	}

	delete[] temp_buf;
}

void glyph_base::apply_effect_outline( uint8_t* buffer, uint32_t w, uint32_t h )
{
	const auto scale_pixel = [] ( uint8_t* buf, uint32_t x, uint32_t y, uint32_t w, uint8_t a )
	{
		if ( a < 2 ) return;

		auto black = new uint8_t[ 4 ]{ 0, 0, 0, 0 };
		black[ 3 ] = a / 2;

		pixel b( black );
		for ( auto j = y - 1; j < y + 2; j++ )
		{
			for ( auto i = x - 1; i < x + 2; i++ )
			{
				pixel px( &buf[ ( j * w + i ) * 4 ] );
				b.blend( px, px );
			}
		}

		delete[] black;
	};

	auto temp_buf = new uint8_t[ w * h * 4 ]{};
	for ( auto y = 1; y < h - 1; y++ )
	{
		for ( auto x = 1; x < w - 1; x++ )
		{
			pixel px( &buffer[ ( y * w + x ) * 4 ] );
			scale_pixel( temp_buf, x, y, w, px.alpha() );
		}
	}

	for ( auto y = 0; y < h; y++ )
	{
		for ( auto x = 0; x < w; x++ )
		{
			pixel r_px( &buffer[ ( y * w + x ) * 4 ] );
			pixel s_px( &temp_buf[ ( y * w + x ) * 4 ] );

			s_px.blend( r_px, r_px );
		}
	}

	delete[] temp_buf;
}