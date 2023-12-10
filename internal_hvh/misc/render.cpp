#include <fstream>
#include "../menu/menu_include.h"
#include "../smallest_pixel.h"
#include "../menu/menu.h"
#include "../josefin_sans.h"
#include "../jost.h"

namespace fonts
{
	std::shared_ptr<c_font> esp = nullptr;
	std::shared_ptr<c_font> lby = nullptr;

	std::shared_ptr<c_font> title_font = nullptr;
	std::shared_ptr<c_font> glitch_font = nullptr;
	std::shared_ptr<c_font> tab_font = nullptr;
	std::shared_ptr<c_font> child_title = nullptr;
	std::shared_ptr<c_font> control = nullptr;
	std::shared_ptr<c_font> control_unicode = nullptr;

	void init()
	{
		if ( esp )
			return;

		_( esp_small_name, "Smallest Pixel-7" );
		esp = std::make_shared<c_font>( esp_small_name, 11, 400, false, 0x4FF );
		_( lby_name, "Verdana" );
		lby = std::make_shared<c_font>( lby_name, 27, 800 );

		_( Josefin, "Josefin Sans SemiBold" );
		title_font = std::make_shared<c_font>( Josefin, 16, 400 );
		tab_font = std::make_shared<c_font>( Josefin, 22, 600 );
		glitch_font = std::make_shared<c_font>( lby_name, 25, 700 );
		child_title = std::make_shared<c_font>( Josefin, 18, 600 );
		control = std::make_shared<c_font>( Josefin, 18, 400 );
		_( uni_name, "Jost Medium" );
		control_unicode = std::make_shared<c_font>( uni_name, 16, 400, false, 0x4FF );

		render::fonts.push_back( fonts::esp );
		render::fonts.push_back( fonts::lby );

		render::fonts.push_back( fonts::title_font );
		render::fonts.push_back( fonts::glitch_font );
		render::fonts.push_back( fonts::tab_font );
		render::fonts.push_back( fonts::child_title );
		render::fonts.push_back( fonts::control );
		render::fonts.push_back( fonts::control_unicode );
	}
}

__attribute__( ( section( "discard" ) ) ) void load_font_from_resource( char* arr, const size_t size )
{
	DWORD n_fonts;
	render::mem_font_hadles.push_back( imp( "gdi32.dll", AddFontMemResourceEx )( arr, size, nullptr, &n_fonts ) );
}

render::command::command( layer* l )
{
	buffer = l;
	// pre-set everything
	DWORD res;
	if ( dev->GetRenderState( D3DRS_SCISSORTESTENABLE, &res ); !res )
		clip = { ( long ) port.X, ( long ) port.Y, ( long ) port.X + ( long ) port.Width, ( long ) port.Y + ( long ) port.Height };
	else
		dev->GetScissorRect( &clip );

	anti_alias = aa_state.empty() ? false : aa_state.back();

	texture = override_texture;
}

void render::load_fonts()
{
	std::vector<unsigned char> v;

	load_font_from_resource( reinterpret_cast< char* >( smallest_pixel.data() ), smallest_pixel.size() );
	smallest_pixel.clear();
	auto vec = std::vector<unsigned char>( smallest_pixel );
	smallest_pixel.swap( vec );

	load_font_from_resource( reinterpret_cast< char* >( josefin_sans.data() ), josefin_sans.size() );
	josefin_sans.clear();
	vec = std::vector<unsigned char>( josefin_sans );
	josefin_sans.swap( vec );

	load_font_from_resource( reinterpret_cast< char* >( jost.data() ), jost.size() );
	jost.clear();
	vec = std::vector<unsigned char>( jost );
	jost.swap( vec );

	erase_end;
}

void render::remove_fonts()
{
	for ( auto& handle : mem_font_hadles )
		imp( "gdi32.dll", RemoveFontMemResourceEx )( handle );
}

void render::init( IDirect3DDevice9* dev )
{
	if ( m_init )
		return;

	// Init fonts
	init_device_objects( dev );
	m_init = true;
}

void render::limit( Vector2D pos, Vector2D size, bool override )
{
	RECT rec;
	rec.left = static_cast< LONG >( pos.x );
	rec.top = static_cast< LONG >( pos.y );
	rec.right = static_cast< LONG >( pos.x + size.x + 1 );
	rec.bottom = static_cast< LONG >( pos.y + size.y + 1 );

	if ( !override && !m_scissor_records.empty() )
	{
		auto& last_record = m_scissor_records.back();
		rec.left = clamp( rec.left, last_record.left, last_record.right );
		rec.right = clamp( rec.right, last_record.left, last_record.right );
		rec.top = clamp( rec.top, last_record.top, last_record.bottom );
		rec.bottom = clamp( rec.bottom, last_record.top, last_record.bottom );
	}

	m_scissor_records.push_back( rec );

	dev->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
	dev->SetScissorRect( &rec );
}

void render::limit_lua( Vector2D pos, Vector2D to, bool intersect )
{
	RECT rec;
	rec.left = static_cast< LONG >( pos.x );
	rec.top = static_cast< LONG >( pos.y );
	rec.right = static_cast< LONG >( to.x );
	rec.bottom = static_cast< LONG >( to.y );

	if ( intersect && !m_scissor_records.empty() )
	{
		auto& last_record = m_scissor_records.back();
		rec.left = clamp( rec.left, last_record.left, last_record.right );
		rec.right = clamp( rec.right, last_record.left, last_record.right );
		rec.top = clamp( rec.top, last_record.top, last_record.bottom );
		rec.bottom = clamp( rec.bottom, last_record.top, last_record.bottom );
	}

	m_scissor_records.push_back( rec );

	dev->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
	dev->SetScissorRect( &rec );
}

void render::reset_limit()
{
	if ( !m_scissor_records.empty() )
		m_scissor_records.pop_back();

	/*
	 * If we have a scissor rect from earlier, use that one
	 */
	if ( !m_scissor_records.empty() )
	{
		dev->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
		dev->SetScissorRect( &m_scissor_records.back() );
	}
	else
		dev->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
}

void render::line( Vector2D from, Vector2D to, Color color )
{
	const auto col = color.direct();

	command cmd( &render_layer );
	cmd.add_vertex(
		vertex{ from.x, from.y, 0.0f, 1.0f, col },
			vertex{ to.x, to.y, 0.0f, 1.0f, col }
	);
	cmd.add_index( 0, 1 );
	//render_layer.commands.emplace_back( cmd );
}

void render::line_gradient( Vector2D from, Vector2D to, Color color, Color color2 )
{
	const auto col = color.direct();
	const auto col2 = color2.direct();

	command cmd( &render_layer );
	cmd.add_vertex(
		vertex{ from.x, from.y, 0.0f, 1.0f, col },
			vertex{ to.x, to.y, 0.0f, 1.0f, col2 }
	);
	cmd.add_index( 0, 1 );
	//render_layer.commands.emplace_back( cmd );
}

void render::rect( Vector2D from, Vector2D size, Color color )
{
	const auto to = from + size;
	const auto col = color.direct();

	command cmd( &render_layer );
	cmd.add_vertex(
	vertex{ from.x, from.y, 0.0f, 1.0f, col },
	vertex{ to.x, from.y, 0.0f, 1.0f, col },
	vertex{ to.x, to.y, 0.0f, 1.0f, col },
	vertex{ from.x, to.y, 0.0f, 1.0f, col },
	vertex{ from.x, from.y, 0.0f, 1.0f, col }
	);
	cmd.add_index( 0, 1, 2, 3, 4 );
	//render_layer.commands.emplace_back( cmd );
}

void render::rect_linear_gradient( Vector2D from, Vector2D size, Color color1, Color color2, bool horizontal )
{
	const auto to = from + size;
	const auto col1 = color1.direct();
	const auto col2 = color2.direct();

	vertex vert[ 5 ] =
	{
		{ from.x, from.y, 0.0f, 1.0f, col2 },
	{ to.x, from.y, 0.0f, 1.0f, horizontal ? col2 : col1 },
	{ to.x, to.y, 0.0f, 1.0f, col2 },
	{ from.x, to.y, 0.0f, 1.0f, horizontal ? col1 : col2 },
	{ from.x, from.y, 0.0f, 1.0f, col1 }
	};

	dev->SetTexture( 0, nullptr );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );
	dev->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, &vert, sizeof( vertex ) );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );
}

void render::rect_filled( Vector2D from, Vector2D size, Color color )
{
	const auto to = from + size;
	const auto col = color.direct();

	command cmd( &render_layer );
	cmd.add_vertex(
		vertex{ from.x, from.y, 0.0f, 1.0f, col },
	vertex{ to.x, from.y, 0.0f, 1.0f, col },
	vertex{ from.x, to.y, 0.0f, 1.0f, col },
	vertex{ to.x, to.y, 0.0f, 1.0f, col }
	);

	cmd.add_index( 0, 1, 2, 0, 2, 3 );
	render_layer.commands.emplace_back( cmd );
}

void render::rect_filled_linear_gradient( Vector2D from, Vector2D size, Color color1, Color color2, bool horizontal )
{
	const auto to = from + size;
	const auto col1 = color1.direct();
	const auto col2 = color2.direct();

	vertex vert[ 4 ] =
	{
		{ from.x, from.y, 0.0f, 1.0f, col1 },
	{ to.x, from.y, 0.0f, 1.0f, horizontal ? col2 : col1 },
	{ from.x, to.y, 0.0f, 1.0f, horizontal ? col1 : col2 },
	{ to.x, to.y, 0.0f, 1.0f, col2 }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &vert, sizeof( vertex ) );
}

void render::rect_filled_radial_gradient_test( Vector2D from, Vector2D size, Color color1, Color color2 )
{
	const auto to = from + size;
	const auto center = from + size / 2.0f;

	const auto col1 = color1.direct();
	const auto col2 = color2.direct();
	build_lookup_table();

	vertex vert[ points + 3 ] = {};

	for ( auto i = 2; i <= points; i++ )
		vert[ i ] =
	{
		center.x + size.x * 0.5f * lookup_table[ i ].x,
		center.y - size.y * 0.5f * lookup_table[ i ].y,
		0.0f,
		1.0f,
		col1
	};

	vert[ 0 ] = { to.x - 2, to.y - 2, 0.0f, 1.0f, col2 };
	vert[ 1 ] = { from.x + 2, from.y + 2, 0.0f, 1.0f, col2 };
	vert[ points + 1 ] = vert[ 2 ];

	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );
	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, points, &vert, sizeof( vertex ) );
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );
}

void render::rect_filled_radial_gradient( Vector2D from, Vector2D size, Color color1, Color color2 )
{
	const auto center = from + size / 2.0f;

	const auto col1 = color1.direct();
	const auto col2 = color2.direct();
	build_lookup_table();

	vertex vert[ points + 2 ] = {};

	for ( auto i = 1; i <= points; i++ )
		vert[ i ] =
	{
		center.x + size.x * lookup_table[ i ].x,
		center.y - size.y * lookup_table[ i ].y,
		0.0f,
		1.0f,
		col1
	};

	vert[ 0 ] = { center.x, center.y, 0.0f, 1.0f, col2 };
	vert[ points + 1 ] = vert[ 1 ];

	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );
	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, points, &vert, sizeof( vertex ) );
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );
}


void render::rect_filled_diamond_gradient( Vector2D from, Vector2D size, Color color1, Color color2 )
{
	const auto to = from + size;
	const auto col1 = color1.direct();
	const auto col2 = color2.direct();

	vertex vert[ 6 ] =
	{
		{ ( from.x + to.x ) / 2.0f, ( from.y + to.y ) / 2.0f, 0.0f, 1.0f, col2 },
	{ from.x, from.y, 0.0f, 1.0f, col1 },
	{ to.x, from.y, 0.0f, 1.0f, col1 },
	{ to.x, to.y, 0.0f, 1.0f, col1 },
	{ from.x, to.y, 0.0f, 1.0f, col1 },
	{ from.x, from.y, 0.0f, 1.0f, col1 }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 5, &vert, sizeof( vertex ) );
}

void render::rect_filled_hue( Vector2D from, Vector2D size )
{
	vertex vert[ 128 + 2 ] = {};

	auto new_hue = 0.f;
	for ( auto i = 1; i <= 128; i++ )
	{
		if ( new_hue >= 1.f )
			new_hue = 0.f;

		new_hue += 1.f / 128.f;
		new_hue = clamp( new_hue, 0.f, 1.f );

		auto temp = util::hsb_to_rgbcol( new_hue, 1.f, 1.f, 255 );
		const auto col = D3DCOLOR_RGBA( temp.r(), temp.g(), temp.b(), 255 );

		vert[ i ] =
		{
			from.x + size.x / 128 * i,
			from.y + size.y,
			0.0f,
			1.0f,
			col
		};
	}

	vert[ 0 ] = vert[ 1 ];
	vert[ 128 + 1 ] = vert[ 128 ];

	dev->SetTexture( 0, NULL );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 128, &vert, sizeof( vertex ) );
}

void render::triangle( Vector2D pos1, Vector2D pos2, Vector2D pos3, Color color )
{
	vertex vert[ 4 ] =
	{
		{ pos1.x, pos1.y, 0.0f, 1.0f, color.direct() },
	{ pos2.x, pos2.y, 0.0f, 1.0f, color.direct() },
	{ pos3.x, pos3.y, 0.0f, 1.0f, color.direct() },
	{ pos1.x, pos1.y, 0.0f, 1.0f, color.direct() }
	};


	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );
	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_LINESTRIP, 3, &vert, sizeof( vertex ) );
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );
}

void render::triangle_filled( Vector2D pos1, Vector2D pos2, Vector2D pos3, Color color )
{
	vertex vert[ 3 ] =
	{
		{ pos1.x, pos1.y, 0.0f, 1.0f, color.direct() },
	{ pos2.x, pos2.y, 0.0f, 1.0f, color.direct() },
	{ pos3.x, pos3.y, 0.0f, 1.0f, color.direct() }
	};


	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );
	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 1, &vert, sizeof( vertex ) );
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );
}

void render::triangle_filled_linear_gradient( Vector2D pos1, Vector2D pos2, Vector2D pos3, Color color1, Color color2, Color color3 )
{
	const auto col1 = color1.direct();
	const auto col2 = color2.direct();
	const auto col3 = color3.direct();

	vertex vert[ 4 ] =
	{
		{ pos1.x, pos1.y, 0.0f, 1.0f, col1 },
	{ pos2.x, pos2.y, 0.0f, 1.0f, col2 },
	{ pos3.x, pos3.y, 0.0f, 1.0f, col3 }
	};

	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );
	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, points, &vert, sizeof( vertex ) );
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );
}

void render::circle( Vector2D center, float radius, Color color )
{
	const auto col = color.direct();
	build_lookup_table();

	vertex vert[ points + 1 ] = {};

	for ( auto i = 0; i <= points; i++ )
		vert[ i ] =
	{
		center.x + radius * lookup_table[ i ].x,
		center.y - radius * lookup_table[ i ].y,
		0.0f,
		1.0f,
		col
	};

	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );
	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_LINESTRIP, points, &vert, sizeof( vertex ) );
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );
}

void render::circle_progress( Vector2D center, float radius, Color color, float progress )
{
	const auto col = color.direct();
	build_lookup_table();

	const auto num_points = clamp( static_cast< int >( progress * points ), 0, points );

	vertex vert[ points + 1 ] = {};

	for ( auto i = 0; i <= points; i++ )
		vert[ points - i ] =
	{
		center.x + radius * lookup_table[ ( i + 16 ) % ( points + 1 ) ].x,
		center.y - radius * lookup_table[ ( i + 16 ) % ( points + 1 ) ].y,
		0.0f,
		1.0f,
		col
	};

	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );
	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_LINESTRIP, num_points, &vert, sizeof( vertex ) );
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );
}

void render::circle_filled( Vector2D center, float radius, Color color )
{
	const auto col = color.direct();
	build_lookup_table();

	vertex vert[ points + 1 ] = {};

	for ( auto i = 0; i <= points; i++ )
		vert[ i ] =
	{
		center.x + radius * lookup_table[ i ].x,
		center.y - radius * lookup_table[ i ].y,
		0.0f,
		1.0f,
		col
	};

	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );
	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, points, &vert, sizeof( vertex ) );
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );
}

void render::circle_filled_radial_gradient( Vector2D center, float radius, Color color1, Color color2 )
{
	const auto col1 = color1.direct();
	const auto col2 = color2.direct();
	build_lookup_table();

	vertex vert[ points + 2 ] = {};

	for ( auto i = 1; i <= points; i++ )
		vert[ i ] =
	{
		center.x + radius * lookup_table[ i ].x,
		center.y - radius * lookup_table[ i ].y,
		0.0f,
		1.0f,
		col1
	};

	vert[ 0 ] = { center.x, center.y, 0.0f, 1.0f, col2 };
	vert[ points + 1 ] = vert[ 1 ];

	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );
	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, points, &vert, sizeof( vertex ) );
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );
}

void render::circle_filled_rainbow( Vector2D center, float radius, float* hue, float brightness, float alpha, float alpha2 )
{
	if ( hue )
	{
		if ( *hue >= 1.f )
			*hue = 0.f;

		*hue += ( 1.f / 5.f ) * interfaces::globals()->frametime;
	}

	vertex vert[ 128 + 2 ] = {};

	auto new_hue = hue ? *hue : 0.f;
	for ( auto i = 1; i <= 128; i++ )
	{
		if ( new_hue >= 1.f )
			new_hue = 0.f;

		new_hue += 1.f / 128.f;
		new_hue = clamp( new_hue, 0.f, 1.f );

		auto temp = util::hsb_to_rgbcol( new_hue, 1.f, brightness, 255 );
		auto col = D3DCOLOR_RGBA( temp.r(), temp.g(), temp.b(), static_cast< int >( alpha ) );

		vert[ i ] =
		{
			center.x + radius * cos( D3DX_PI * ( i / ( 128 / 2.0f ) ) ),
			center.y + radius * sin( D3DX_PI * ( i / ( 128 / 2.0f ) ) ),
			0.0f,
			1.0f,
			col
		};
	}

	vert[ 0 ] = { center.x, center.y, 0.0f, 1.0f, static_cast< uint32_t >( Color( util::hsb_to_rgbcol( 0.f, 0.f, brightness, 255 ), alpha2 ).GetD3DColor() ) };
	vert[ 128 + 1 ] = vert[ 1 ];

	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );
	dev->SetTexture( 0, NULL );
	dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 128, &vert, sizeof( vertex ) );
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );
}

void render::filled_triangle( POINT pos1, POINT pos2, POINT pos3, Color color )
{
	vertex vert[ 3 ] =
	{
		{ ( float ) pos1.x, ( float ) pos1.y, 0.0f, 1.0f, color.direct() },
	{ ( float ) pos2.x, ( float ) pos2.y, 0.0f, 1.0f, color.direct() },
	{ ( float ) pos3.x, ( float ) pos3.y, 0.0f, 1.0f, color.direct() }
	};


	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );
	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 1, &vert, sizeof( vertex ) );
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );
}

void render::filled_box( const int x, const int y, const int w, const int h, const Color color )
{
	vertex vertices2[ 4 ] = { { ( float ) x, ( float ) y + h, 0.0f, 1.0f, color.direct() }, { ( float ) x, ( float ) y, 0.0f, 1.0f, color.direct() }, { ( float ) ( x + w ), ( float ) ( y + h ), 0.0f, 1.0f, color.direct() }, { ( float ) ( x + w ), ( float ) y, 0.0f, 1.0f, color.direct() } };

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertices2, sizeof( vertex ) );
}

void render::filled_box_outlined( const int x, const int y, const int w, const int h, const Color color, const Color outline, const int thickness )
{
	filled_box( x, y, w, h, color );
	bordered_box( x, y, w, h, outline, thickness );
}

void render::bordered_box( const int x, const int y, const int w, const int h, const Color color, const int thickness )
{
	filled_box( x, y, w, thickness, color );
	filled_box( x, y, thickness, h, color );
	filled_box( x + w - thickness, y, thickness, h, color );
	filled_box( x, y + h - thickness, w, thickness, color );
}

void render::bordered_box_outlined( const int x, const int y, const int w, const int h, const Color color, const Color outline, const int thickness )
{
	bordered_box( x, y, w, h, outline, thickness );
	bordered_box( x + thickness, y + thickness, w - ( thickness * 2 ), h - ( thickness * 2 ), color, thickness );
	bordered_box( x + ( thickness * 2 ), y + ( thickness * 2 ), w - ( thickness * 4 ), h - ( thickness * 4 ), outline, thickness );
}

void render::text( Vector2D pos, std::wstring string, Color color, std::shared_ptr<c_font>& font, uint8_t flags )
{
	font->draw_string( std::roundf( pos.x ), std::roundf( pos.y ), color.direct(), string.c_str(), flags );
}

void render::text( Vector2D pos, std::string string, Color color, std::shared_ptr<c_font>& font, uint8_t flags )
{
	text( pos, std::wstring( string.begin(), string.end() ), color, font, flags );
}

void render::text_radial( Vector2D pos, std::wstring string, Color color1, Color color2, std::shared_ptr<c_font>& font, uint8_t flags )
{
	font->draw_radial( std::roundf( pos.x ), std::roundf( pos.y ), color1.direct(), color2.direct(), string.c_str(), flags );
}

void render::text_radial( Vector2D pos, std::string string, Color color1, Color color2, std::shared_ptr<c_font>& font, uint8_t flags )
{
	text_radial( pos, std::wstring( string.begin(), string.end() ), color1, color2, font, flags );
}

Vector2D render::get_text_size( std::wstring string, std::shared_ptr<c_font>& font )
{
	SIZE size;
	font->get_text_extent( string.c_str(), &size );
	return Vector2D( static_cast< float >( size.cx ), static_cast< float >( size.cy ) );
}

Vector2D render::get_text_size( std::string string, std::shared_ptr<c_font>& font )
{
	return get_text_size( std::wstring( string.begin(), string.end() ), font );
}

void render::scale( Vector2D& vec )
{
	vec.x = ( vec.x + 1.f ) * port.Width / 2.f;
	vec.y = ( -vec.y + 1.f ) * port.Height / 2.f;
}

Vector2D render::get_center()
{
	return Vector2D( static_cast< float >( port.X + port.Width / 2.f ), static_cast< float >( port.Y + port.Height / 2.f ) );
}

D3DVIEWPORT9 render::get_dimensions()
{
	return port;
}

void render::init_device_objects( IDirect3DDevice9* dev )
{
	render::dev = dev;
	dev->GetViewport( &port );

	fonts::init();

	for ( const auto& font : fonts )
	{
		font->init_device_objects( dev );
		font->restore_device_objects();
	}

	for ( const auto& font : lua_fonts )
	{
		if ( font.first > 0 )
		{
			font.second->init_device_objects( dev );
			font.second->restore_device_objects();
		}
	}

	if ( !entity_rendering::sprit_tex )
		imp( "d3dx9_43.dll", D3DXCreateSprite )( render::dev, const_cast< LPD3DXSPRITE* >( &entity_rendering::sprit_tex ) );

	if ( !m_sprite_texture )
		imp( "d3dx9_43.dll", D3DXCreateSprite )( render::dev, const_cast< LPD3DXSPRITE* >( &m_sprite_texture ) );

	if ( !texture_object )
	{
		auto pixel = 0xFFFFFFFFu;
		if ( FAILED( dev->CreateTexture( 1, 1, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture_object, nullptr ) ) )
			return;

		D3DLOCKED_RECT r{};
		if ( FAILED( texture_object->LockRect( 0, &r, nullptr, 0 ) ) )
			return;

		// swap colors to correct rendering
		for ( auto y = 0; y < 1; y++ )
		{
			for ( auto x = 0; x < 1; x++ )
			{
				auto c = reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( &pixel ) + ( x + y * 1 ) * 4 );
				c[ 0 ] ^= c[ 2 ];
				c[ 2 ] ^= c[ 0 ];
				c[ 0 ] ^= c[ 2 ];
			}
		}

		if ( r.Pitch == 4 )
			memcpy( r.pBits, &pixel, 4 * 1 );
		else
		{
			for ( auto y = 0; y < 1; y++ )
				memcpy( ( char* ) r.pBits + r.Pitch * y, ( char* ) &pixel + 4 * y, 4 );
		}

		texture_object->UnlockRect( 0 );
	}
}

void render::invalidate_device_objects()
{
	dev = nullptr;

	SAFE_RELEASE( entity_rendering::sprit_tex );
	SAFE_RELEASE( m_sprite_texture );
	SAFE_RELEASE( texture_object );

	for ( const auto& font : fonts )
		font->invalidate_device_objects();

	for ( const auto& font : lua_fonts )
		font.second->invalidate_device_objects();

	m_weapon_icons.clear();
	m_custom_icons.clear();
}

void render::setup_render_state( IDirect3DDevice9* device )
{
	dev = device;
	dev->SetVertexShader( nullptr );
	dev->SetPixelShader( nullptr );
	dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
	dev->SetRenderState( D3DRS_LIGHTING, false );
	dev->SetRenderState( D3DRS_FOGENABLE, false );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	dev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	dev->SetRenderState( D3DRS_SCISSORTESTENABLE, true );
	dev->SetRenderState( D3DRS_ZWRITEENABLE, false );
	dev->SetRenderState( D3DRS_STENCILENABLE, false );

	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, false );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, false );

	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	dev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, true );
	dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	dev->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
	dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	dev->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );

	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, false );
	dev->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );

	dev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	dev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	dev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
}

void render::build_lookup_table()
{
	if ( !lookup_table.empty() )
		return;

	for ( auto i = 0; i <= points; i++ )
		lookup_table.emplace_back(
			std::cos( 2.f * D3DX_PI * ( i / static_cast< float >( points ) ) ),
			std::sin( 2.f * D3DX_PI * ( i / static_cast< float >( points ) ) )
		);
}

void render::filled_cube( const Vector origin, const int width, const int height, Color outline, Color filling )
{
	const auto difw = static_cast< float >( width / 2 );
	const auto difh = static_cast< float >( height / 2 );
	const Vector box_vectors[ 8 ] =
	{
		Vector( origin.x - difw, origin.y - difh, origin.z - difw ),
		Vector( origin.x - difw, origin.y - difh, origin.z + difw ),
		Vector( origin.x + difw, origin.y - difh, origin.z + difw ),
		Vector( origin.x + difw, origin.y - difh, origin.z - difw ),
		Vector( origin.x - difw, origin.y + difh, origin.z - difw ),
		Vector( origin.x - difw, origin.y + difh, origin.z + difw ),
		Vector( origin.x + difw, origin.y + difh, origin.z + difw ),
		Vector( origin.x + difw, origin.y + difh, origin.z - difw ),
	};

	static Vector vec0, vec1, vec2, vec3,
		vec4, vec5, vec6, vec7;

	if ( math::world_to_screen( box_vectors[ 0 ], vec0 ) &&
		 math::world_to_screen( box_vectors[ 1 ], vec1 ) &&
		 math::world_to_screen( box_vectors[ 2 ], vec2 ) &&
		 math::world_to_screen( box_vectors[ 3 ], vec3 ) &&
		 math::world_to_screen( box_vectors[ 4 ], vec4 ) &&
		 math::world_to_screen( box_vectors[ 5 ], vec5 ) &&
		 math::world_to_screen( box_vectors[ 6 ], vec6 ) &&
		 math::world_to_screen( box_vectors[ 7 ], vec7 ) )
	{


		Vector2D lines[ 12 ][ 2 ];
		lines[ 0 ][ 0 ] = vec0.Base();
		lines[ 0 ][ 1 ] = vec1.Base();
		lines[ 1 ][ 0 ] = vec1.Base();
		lines[ 1 ][ 1 ] = vec2.Base();
		lines[ 2 ][ 0 ] = vec2.Base();
		lines[ 2 ][ 1 ] = vec3.Base();
		lines[ 3 ][ 0 ] = vec3.Base();
		lines[ 3 ][ 1 ] = vec0.Base();


		// top of box
		lines[ 4 ][ 0 ] = vec4.Base();
		lines[ 4 ][ 1 ] = vec5.Base();
		lines[ 5 ][ 0 ] = vec5.Base();
		lines[ 5 ][ 1 ] = vec6.Base();
		lines[ 6 ][ 0 ] = vec6.Base();
		lines[ 6 ][ 1 ] = vec7.Base();
		lines[ 7 ][ 0 ] = vec7.Base();
		lines[ 7 ][ 1 ] = vec4.Base();


		lines[ 8 ][ 0 ] = vec0.Base();
		lines[ 8 ][ 1 ] = vec4.Base();

		lines[ 9 ][ 0 ] = vec1.Base();
		lines[ 9 ][ 1 ] = vec5.Base();

		lines[ 10 ][ 0 ] = vec2.Base();
		lines[ 10 ][ 1 ] = vec6.Base();

		lines[ 11 ][ 0 ] = vec3.Base();
		lines[ 11 ][ 1 ] = vec7.Base();

		const auto draw_polys = [&] ( const std::vector<std::vector<Vector2D>>& vecs ) -> void
		{
			for ( auto it : vecs )
			{
				vertex vert[ 4 ] =
				{
					{ it[ 0 ].x, it[ 0 ].y, 0.0f, 1.0f, filling.direct() },
				{ it[ 1 ].x, it[ 1 ].y, 0.0f, 1.0f, filling.direct() },
				{ it[ 2 ].x, it[ 2 ].y, 0.0f, 1.0f, filling.direct() },
				{ it[ 3 ].x, it[ 3 ].y, 0.0f, 1.0f, filling.direct() }
				};

				dev->SetTexture( 0, nullptr );
				dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vert, sizeof( vertex ) );
			}
		};

		const std::vector<Vector2D> front = { vec0.Base(), vec1.Base(), vec3.Base(), vec2.Base() };
		const std::vector<Vector2D> back = { vec4.Base(), vec5.Base(), vec7.Base(), vec6.Base() };
		const std::vector<Vector2D> left = { vec3.Base(), vec2.Base(), vec7.Base(), vec6.Base() };
		const std::vector<Vector2D> right = { vec0.Base(), vec1.Base(), vec4.Base(), vec5.Base() };
		const std::vector<Vector2D> top = { vec2.Base(), vec1.Base(), vec6.Base(), vec5.Base() };
		const std::vector<Vector2D> bottom = { vec0.Base(), vec4.Base(), vec3.Base(), vec7.Base() };

		const std::vector<std::vector<Vector2D>> vectors{ front, back, left, right, top, bottom };

		draw_polys( vectors );

		for ( auto& curr : lines )
			line( curr[ 0 ], curr[ 1 ], outline );
	}
}

void render::rect_filled_multi_gradient( Vector2D from, Vector2D size, Color col_top_left, Color col_top_right, Color col_bot_left, Color col_bot_right )
{
	const auto to = from + size;
	const auto col1 = col_top_left.direct();
	const auto col2 = col_top_right.direct();
	const auto col3 = col_bot_left.direct();
	const auto col4 = col_bot_right.direct();

	vertex vert[ 4 ] =
	{
		{ from.x, from.y, 0.0f, 1.0f, col1 },
	{ to.x, from.y, 0.0f, 1.0f, col2 },
	{ from.x, to.y, 0.0f, 1.0f, col3 },
	{ to.x, to.y, 0.0f, 1.0f, col4 }
	};

	dev->SetTexture( 0, nullptr );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &vert, sizeof( vertex ) );
}

void render::render_texture( LPDIRECT3DTEXTURE9 texture, const Vector2D& pos, std::size_t width, std::size_t height, const Color& color, uint8_t flags )
{
	if ( !m_sprite_texture )
		imp( "d3dx9_43.dll", D3DXCreateSprite )( dev, const_cast< LPD3DXSPRITE* >( &m_sprite_texture ) );

	D3DXMATRIX world;
	D3DXMATRIX rotation;
	D3DXMATRIX scale;
	D3DXMATRIX translation;
	D3DXMatrixIdentity( &world );

	D3DXMatrixScaling( &scale, 1.f, 1.f, 1.f );
	D3DXMatrixRotationYawPitchRoll( &rotation, 0.f, 0.f, 0.f );
	D3DXMatrixTranslation( &translation, 0.f, 0.f, 0.f );
	world = rotation * scale * translation;

	/*
	* Prepare and render the texture
	*/

	m_sprite_texture->SetTransform( &world );
	m_sprite_texture->Begin( D3DXSPRITE_ALPHABLEND );

	auto new_pos = D3DXVECTOR3( pos.x, pos.y, 0.f );

	if ( flags & centered_x )
	{
		new_pos.x -= static_cast< float >( width ) * 0.5f;
		new_pos.x = std::roundf( new_pos.x );
	}
	else if ( flags & right_aligned )
	{
		new_pos.x -= static_cast< float >( width );
		new_pos.x = std::roundf( new_pos.x );
	}

	if ( flags & centered_y )
	{
		new_pos.y -= static_cast< float >( height ) * 0.5f;
		new_pos.y = std::roundf( new_pos.y );
	}


	if ( flags & outline )
	{
		auto shadowcol = D3DCOLOR_ARGB( static_cast< int >( 0.8f * ( color.GetD3DColor() >> 24 & 255 ) ), 0, 0, 0 );

		auto temp = D3DXVECTOR3( new_pos.x, new_pos.y, 0.f );
		m_sprite_texture->Draw( texture, NULL, NULL, &temp, shadowcol );
		temp = D3DXVECTOR3( new_pos.x + 1.f, new_pos.y + 1.f, 0.f );
		m_sprite_texture->Draw( texture, NULL, NULL, &temp, shadowcol );
		temp = D3DXVECTOR3( new_pos.x + 1.f, new_pos.y - 1.f, 0.f );
		m_sprite_texture->Draw( texture, NULL, NULL, &temp, shadowcol );
		temp = D3DXVECTOR3( new_pos.x - 1.f, new_pos.y + 1.f, 0.f );
		m_sprite_texture->Draw( texture, NULL, NULL, &temp, shadowcol );
	}
	else if ( flags & drop_shadow )
	{
		auto shadowcol = D3DCOLOR_ARGB( static_cast< int >( 0.8f * ( color.GetD3DColor() >> 24 & 255 ) ), 0, 0, 0 );

		auto temp = D3DXVECTOR3( new_pos.x - 1.f, new_pos.y, 0.f );
		m_sprite_texture->Draw( texture, NULL, NULL, &temp, shadowcol );
		temp = D3DXVECTOR3( new_pos.x, new_pos.y + 1.f, 0.f );
		m_sprite_texture->Draw( texture, NULL, NULL, &temp, shadowcol );
	}

	auto temp = D3DXVECTOR3( new_pos.x, new_pos.y, 0.f );
	m_sprite_texture->Draw( texture, NULL, NULL, &temp, color.GetD3DColor() );
	m_sprite_texture->End();
}

void render::space_texture( const Vector2D& pos, const Vector2D& size )
{
	rect_filled( pos, size, gui_color::custom( Color( 17, 15, 35 ) ) );
}


Color render::get_pixel_color( const Vector2D& pos, bool render )
{
	static auto ret = Color( 0, 0, 0, 255 );

	if ( !render )
		return ret;

	if ( !g_menu->get_focussed_control() || !g_menu->get_focussed_control()->get_focussed_control() || !g_menu->get_focussed_control()->get_focussed_control()->is_colorpicker() )
		return ret;

	const auto is_fullscreen = [] ( HWND window ) -> bool
	{
		RECT a, b;
		GetWindowRect( window, &a );
		GetWindowRect( GetDesktopWindow(), &b );
		return ( a.left == b.left &&
				 a.top == b.top &&
				 a.right == b.right &&
				 a.bottom == b.bottom );
	};

	/*
	* Grab whole screen texture
	*/
	auto screen = GetDC( is_fullscreen( init::window ) ? NULL : GetForegroundWindow() );

	/*
	* Grab our wished pixel
	*/
	auto pixel = GetPixel( screen, pos.x, pos.y );

	/*
	* Release the texture
	*/
	ReleaseDC( NULL, screen );

	/*
	* Return color
	*/
	ret = Color( GetRValue( pixel ), GetGValue( pixel ), GetBValue( pixel ) );

	return ret;
}

Vector2D render::weapon_size( std::string& name, const std::size_t height )
{
	const auto hash = fnv1a_rt( name ) + height;

	m_draw_icon_mutex.lock();

	auto icon = m_weapon_icons.find( hash );
	if ( icon == m_weapon_icons.end() )
		icon = m_weapon_icons.insert_or_assign( hash, std::make_shared<weapon_icon_t>( name, height ) ).first;

	if ( !icon->second->texture.ptexture )
	{
		m_weapon_icons.erase( icon );
		m_draw_icon_mutex.unlock();
		return {};
	}

	m_draw_icon_mutex.unlock();

	return  { static_cast< float >( icon->second->texture.width ), static_cast< float >( icon->second->texture.height ) };
}


void render::draw_weapon( const Vector2D& pos, const std::size_t height, const Color& color, const std::string& name, uint8_t flags )
{
	if ( name.empty() )
		return;

	const auto hash = fnv1a_rt( name ) + height;

	m_draw_icon_mutex.lock();

	auto icon = m_weapon_icons.find( hash );
	if ( icon == m_weapon_icons.end() )
		icon = m_weapon_icons.insert_or_assign( hash, std::make_shared<weapon_icon_t>( name, height ) ).first;

	if ( !icon->second->texture.ptexture )
	{
		m_weapon_icons.erase( icon );
		m_draw_icon_mutex.unlock();
		return;
	}

	m_draw_icon_mutex.unlock();

	render_texture( icon->second->texture.ptexture, pos, icon->second->texture.width, icon->second->texture.height, color, flags );
}

void render::draw_custom( const Vector2D& pos, const std::size_t height, const Color& color, const std::string& name, uint8_t flags )
{
	if ( name.empty() )
		return;

	const auto hash = fnv1a_rt( name ) + height;

	m_draw_icon_mutex.lock();

	auto icon = m_custom_icons.find( hash );
	if ( icon == m_custom_icons.end() )
	{
		_u( data, m_icon_alias.at( fnv1a_rt( name ) ) );
		icon = m_custom_icons.insert_or_assign( hash, std::make_shared<custom_icon_t>( data, height ) ).first;
	}

	if ( !icon->second->texture.ptexture )
	{
		m_custom_icons.erase( icon );
		m_draw_icon_mutex.unlock();
		return;
	}

	m_draw_icon_mutex.unlock();

	render_texture( icon->second->texture.ptexture, pos, icon->second->texture.width, icon->second->texture.height, color, flags );
}

void render::draw_stream()
{
	IDirect3DStateBlock9* state{};
	if ( FAILED( dev->CreateStateBlock( D3DSBT_PIXELSTATE, &state ) ) )
		return;

	dev->SetPixelShader( nullptr );
	dev->SetVertexShader( nullptr );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_LIGHTING, false );
	dev->SetRenderState( D3DRS_ZENABLE, false );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	dev->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	dev->SetRenderState( D3DRS_SCISSORTESTENABLE, true );
	dev->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	dev->SetRenderState( D3DRS_FOGENABLE, false );
	dev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	dev->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	dev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	dev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	static D3DMATRIX mat_identity = { { {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		} } };
	D3DMATRIX mat_projection = { { {
			2.f / port.Width, 0.0f, 0.0f, 0.0f,
			0.0f, 2.f / -port.Height, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			-1.f, 1.f, 0.5f, 1.f
		} } };

	dev->SetTransform( D3DTS_WORLD, &mat_identity );
	dev->SetTransform( D3DTS_VIEW, &mat_identity );
	dev->SetTransform( D3DTS_PROJECTION, &mat_projection );
	dev->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );

	if ( auto actual_vb_size = render_layer.vertices.size() * sizeof( vertex_dx9 ); !vertex_buffer || vb_size < actual_vb_size )
	{
		if ( vertex_buffer )
			vertex_buffer->Release();
		if ( vb_size < actual_vb_size )
			vb_size = actual_vb_size + 5000;

		if ( FAILED( dev->CreateVertexBuffer(
			vb_size,
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
			D3DPOOL_DEFAULT,
			&vertex_buffer,
			nullptr
			) ) )
			return;
	}

	if ( auto actual_ib_size = render_layer.indices.size() * sizeof( uint16_t ); !index_buffer || ib_size < actual_ib_size )
	{
		if ( index_buffer )
			index_buffer->Release();
		if ( ib_size < actual_ib_size )
			ib_size = actual_ib_size + 5000;

		if ( FAILED( dev->CreateIndexBuffer(
			ib_size,
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&index_buffer,
			nullptr
			) ) )
			return;
	}

	char* vertices{};
	char* indices{};

	if ( FAILED( vertex_buffer->Lock( 0, vb_size, ( void** ) &vertices, D3DLOCK_DISCARD ) ) )
		return;

	if ( FAILED( index_buffer->Lock( 0, ib_size, ( void** ) &indices, D3DLOCK_DISCARD ) ) )
		return;

	size_t i_offset{};
	for ( auto& v : render_layer.vertices )
	{
		*( vertex_dx9* ) vertices = vertex_dx9( v );
		vertices += sizeof( vertex_dx9 );
	}

	const auto i_size = render_layer.indices.size() * sizeof( uint16_t );
	memcpy( indices + i_offset, render_layer.indices.data(), i_size );
	i_offset += i_size;

	vertex_buffer->Unlock();
	index_buffer->Unlock();

	dev->SetStreamSource( 0, vertex_buffer, 0, sizeof( vertex_dx9 ) );
	dev->SetIndices( index_buffer );

	size_t v_offset{};
	for ( auto& c : render_layer.commands )
	{
		dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, c.anti_alias );
		dev->SetTexture( 0, c.texture ? reinterpret_cast< IDirect3DTexture9* >( c.texture ) : texture_object );
		dev->SetScissorRect( &c.clip );
		dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, v_offset, 0, c.vertices, i_offset, c.indices / 3 );

		v_offset += c.vertices;
		i_offset += c.indices;
	}

	state->Apply();
	state->Release();
}