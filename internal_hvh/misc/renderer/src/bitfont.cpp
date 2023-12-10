#include <stdexcept>
#include <ren/bitfont.h>
#include "../cthash.h"

using namespace gui;
using namespace evo::ren;

bitfont::bitfont( const std::string& nam, float size, uint32_t wght, uint32_t flg, uint32_t from, uint32_t to )
{
	name = nam;
	height = size;
	flags = flg;
	weight = wght;
	mins = from;
	maxs = to;
}

bitfont::~bitfont()
{
	destroy();

	if ( win_font )
		DeleteObject( win_font );
	if ( device )
		DeleteDC( device );
	if ( dib )
		DeleteObject( dib );
}

void bitfont::create()
{
	if ( !glyphs.empty() )
		return;

	device = CreateCompatibleDC( nullptr );
	if ( !device )
		throw std::runtime_error( xorstr_( "Couldn't create font device context" ) );

	SetMapMode( device, MM_TEXT );

	win_font = CreateFontA( height, 0, 0, 0, 400, false, false, false,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, flags & ff_antialias ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str() );

	if ( !win_font )
		throw std::runtime_error( xorstr_( "Couldn't create font" ) );

	SelectObject( device, win_font );
	SetTextAlign( device, TA_LEFT | TA_TOP | TA_UPDATECP );

	if ( !GetTextMetricsA( device, &metric ) )
		throw std::runtime_error( xorstr_( "Couldn't get font metrics" ) );

	const auto shadow_offset = flags & ff_shadow ? 1 : 0;
	const auto outline_size = flags & ff_outline ? 1 : 0;

	height = metric.tmHeight + shadow_offset + outline_size * 2;
	ascent = metric.tmAscent;

	bitmap_size[ 0 ] = metric.tmMaxCharWidth + outline_size * 2;
	bitmap_size[ 1 ] = metric.tmHeight + shadow_offset + outline_size * 2;

	BITMAPINFOHEADER bmp{};
	bmp.biSize = sizeof( bmp );
	bmp.biWidth = bitmap_size[ 0 ];
	bmp.biHeight = -bitmap_size[ 1 ];
	bmp.biPlanes = 1;
	bmp.biBitCount = 32;
	bmp.biCompression = BI_RGB;

	dib = CreateDIBSection( device, ( BITMAPINFO* ) &bmp, DIB_RGB_COLORS, &buffer, nullptr, 0 );
	SelectObject( device, dib );
	SelectObject( device, win_font );

	for ( auto i = mins; i <= maxs; i++ )
	{
		ABC abc{};
		if ( !glyph_exits( i ) || !GetCharABCWidthsW( device, i, i, &abc ) )
			continue;

		glyphs[ i ] = bitglyph( i, as<bitfont>(), abc );
		glyphs[ i ].create();
	}
}

void bitfont::destroy()
{
	if ( glyphs.empty() )
		return;

	for ( auto& g : glyphs )
		g.second.destroy();

	glyphs.clear();
}

float bitfont::get_advance( uint32_t a, uint32_t b )
{
	// TODO: right now this is only used for small fonts, so ignore kerning for now.
	auto sum = 0.f;

	if ( !!( flags & ff_outline ) )
		sum -= 2.f;

	if ( !!( flags & ff_shadow ) )
		sum -= 1.f;

	return sum;
}

bool bitfont::glyph_exits( uint32_t g )
{
	wchar_t str[] = { wchar_t( g ), 0 };
	WORD tmp;
	return GetGlyphIndicesW( device, str, 1, &tmp, GGI_MARK_NONEXISTING_GLYPHS ) != GDI_ERROR && tmp < 0xFFFF;
}

void bitglyph::create()
{
	if ( is_created )
		return;

	const auto outline_size = !!( owner->flags & ff_outline ) ? 1 : 0;
	const auto shadow_offset = !!( owner->flags & ff_shadow ) ? 1 : 0;
	const auto w = abc.abcB + ( outline_size * 2.f + 1 ) + shadow_offset;
	const auto h = size_t( owner->height );
	const auto tex_buffer_size = w * h * 4;
	const wchar_t wch = glyph_index;

	auto tex_buffer = reinterpret_cast< uint8_t* >( malloc( tex_buffer_size ) );
	memset( tex_buffer, 0, tex_buffer_size );
	SelectObject( owner->device, owner->win_font );

	SetBkColor( owner->device, RGB( 0, 0, 0 ) );
	SetTextColor( owner->device, RGB( 255, 255, 255 ) );
	SetBkMode( owner->device, OPAQUE );
	MoveToEx( owner->device, 2 * outline_size - abc.abcA, 0, nullptr );

	RECT tmp{ 0, 0, long( w ), long( h ) };
	ExtTextOutW( owner->device, 0, 0, ETO_OPAQUE, &tmp, nullptr, 0, nullptr );
	ExtTextOutW( owner->device, 0, 0, 0, nullptr, &wch, 1, nullptr );

	SetBkMode( owner->device, TRANSPARENT );

	for ( auto j = 0; j < h; j++ )
	{
		for ( auto i = 0; i < w; i++ )
		{
			const auto src = &reinterpret_cast< uint8_t* >( owner->buffer )[ ( i + j * owner->bitmap_size[ 0 ] ) * 4 ];
			auto dst = &tex_buffer[ ( i + j * size_t( w ) ) * 4 ];

			uint8_t r, g, b;
			if ( wch == '\t' )
				r = g = b = 0;
			else
			{
				r = src[ 0 ];
				g = src[ 1 ];
				b = src[ 2 ];
			}

			dst[ 0 ] = r;
			dst[ 1 ] = g;
			dst[ 2 ] = b;
			dst[ 3 ] = uint8_t( ( float ) r * 0.34f + ( float ) g * 0.55f + ( float ) b * 0.11f );
		}
	}

	if ( !!( owner->flags & ff_shadow ) )
		apply_effect_shadow( tex_buffer, w, h );

	if ( !!( owner->flags & ff_outline ) )
		apply_effect_outline( tex_buffer, w, h );

	size = { float( w ), float( h ) };
	offset.y -= .85f * owner->height;
	bmp = std::make_shared<texture>( tex_buffer, uint32_t( w ), uint32_t( h ), uint32_t( w * 4 ) );
	free( tex_buffer );
	is_created = true;
}

void bitglyph::destroy()
{
	if ( !is_created )
		return;

	if ( bmp )
		bmp->destroy();

	is_created = false;
}
