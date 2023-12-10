//
// Created by panzerfaust on 9/19/2020.
//

#include <ren/layer.h>
#include <ren/misc.h>
#include <ren/adapter.h>

using namespace evo::ren;

layer& layer::add_quad_filled( const vec2& a, const vec2& b, const vec2& c, const vec2& d, const color& col )
{
	const auto uv = get_uv().value_or( rect( 0.f, 0.f, 1.f, 1.f ) );
	const auto rot = get_rot().value_or( 0.f );
	const auto center = get_center( { a, b, c, d } );

	const auto 	_a = rotate_point( rot, a, center ),
		_b = rotate_point( rot, b, center ),
		_c = rotate_point( rot, c, center ),
		_d = rotate_point( rot, d, center );

	command cmd( this );
	cmd.add_vertex(
			vertex( _a, col, uv.tl() ),
			vertex( _b, col, uv.tr() ),
			vertex( _c, col, uv.br() ),
			vertex( _d, col, uv.bl() )
	);
	cmd.add_index( 0, 1, 2, 0, 2, 3 );

	add_command( cmd );
	return *this;
}

layer& layer::add_empty()
{
	command cmd( this );
	cmd.add_vertex( vertex( { 0.f, 0.f }, 0 ) );
	cmd.add_index( 0, 0, 0 );

	add_command( cmd );
	return *this;
}

layer& layer::add_rect_filled( const rect& r, const color& c )
{
	if ( r.width() == 0.f || r.height() == 0.f )
		return *this;

	return add_quad_filled( r.tl(), r.tr(), r.br(), r.bl(), c );
}

layer& layer::add_text( const std::shared_ptr<font_base>& f, const vec2& p, const std::string& str, const color& col, char ah, char av )
{
	auto pos = p * adapter->dpi;
	if ( ah > 0 || av > 0 )
	{
		const auto size = text_size( f, str );
		if ( ah == align_center )
			pos -= vec2( size.x * 0.5f, 0.f );
		else if ( ah == align_right )
			pos -= vec2( size.x, 0.f );

		if ( av == align_center )
			pos -= vec2( 0.f, size.y * 0.5f );
		else if ( av == align_bottom )
			pos -= vec2( 0.f, size.y );
	}

	auto offset = pos; auto last_c = 0; auto is_first = true;
	for ( auto i = 0; i < str.length(); is_first = false )
	{
		if ( str[ i ] == ' ' || str[ i ] == '\t' )
		{
			offset.x += f->height * 0.25f * ( str[ i ] == ' ' ? 1 : 4 );
			i++;

			last_c = 0;
			continue;
		}

		if ( str[ i ] == '\n' )
		{
			offset.y += f->height + f->descent + f->line_gap;
			offset.x = pos.x;
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
			offset.x += g.first_offset.x;

		const auto k = f->get_advance( last_c, codepoint );
		const auto r = rect( offset + vec2( k + g.offset.x, g.offset.y + f->ascent ) ).size( g.size ).round();

		command cmd( this, false );
		cmd.texture = g.bmp;
		cmd.no_dpi = true;
		cmd.add_vertex(
			vertex( r.tl(), col, { 0.f, 0.f } ),
			vertex( r.tr(), col, { 1.f, 0.f } ),
			vertex( r.br(), col, { 1.f, 1.f } ),
			vertex( r.bl(), col, { 0.f, 1.f } )
		);

		cmd.add_index( 0, 1, 2, 0, 2, 3 );
		add_command( cmd );

		offset.x += g.size.x + k + g.offset.x + g.extra_offset.x;
		last_c = codepoint;
	}

	return *this;
}

layer& layer::add_line( const vec2& a, const vec2& b, const color& col, float thickness )
{
	if ( thickness <= 0.f )
		return *this;

	auto dx = b.x - a.x;
	auto dy = b.y - a.y;

	const auto inv = 1.f / sqrtf( dx * dx + dy * dy );
	dx *= inv * ( thickness * 0.5f );
	dy *= inv * ( thickness * 0.5f );

	command cmd( this );
	cmd.add_vertex( vertex( vec2( a.x + dy, a.y - dx ), col ) );
	cmd.add_vertex( vertex( vec2( b.x + dy, b.y - dx ), col ) );
	cmd.add_vertex( vertex( vec2( b.x - dy, b.y + dx ), col ) );
	cmd.add_vertex( vertex( vec2( a.x - dy, a.y + dx ), col ) );
	cmd.add_index( 0, 1, 2, 0, 2, 3 );

	add_command( cmd );
	return *this;
}

layer& layer::add_rect( const rect& r, const color& c, float thickness )
{
	if ( r.width() == 0.f || r.height() == 0.f )
		return *this;

	return add_quad( r.tl() + 0.5f, r.tr() + vec2( -0.49f, 0.49f ), r.br() - 0.49f, r.bl() - vec2( -0.49f, 0.49f ), c, thickness );
}

layer& layer::add_circle_filled( const vec2& c, float r, const color& col, int segments, float fill, float rot )
{
	if ( r == 0.f || segments <= 0 || fill <= 0.f )
		return *this;

	build_lookup_table( segments );

	const auto has_uv = get_uv().has_value();

	const auto uv = get_uv().value_or( rect( 0.f, 0.f, 1.f, 1.f ) );
	command cmd( this );
	cmd.add_vertex( vertex( c, col, uv.center() ) );

	const auto num_points = std::clamp( static_cast< int >( fill * segments ), 0, segments );
	const auto offset = static_cast< int >( rot * 0.00277777778f * static_cast< float >( segments ) ) % segments;

	auto index = 1;
	for ( auto i = offset + 1; i < num_points + offset + 1; i++, index += 2 )
	{
		const auto idx = ( i - 1 ) % segments;
		const auto idx2 = i % segments;

		const auto pos1 = c + vec2( r * lookup_table[ segments ][ idx ].x, r * lookup_table[ segments ][ idx ].y );
		const auto pos2 = c + vec2( r * lookup_table[ segments ][ idx2 ].x, r * lookup_table[ segments ][ idx2 ].y );

		cmd.add_vertex( vertex( pos1, col, has_uv ? uv.circle( 2.f * 3.14159265358979323846 * ( idx / static_cast< float >( segments ) ) ) : vec2{ 0.f, 1.f } ) );
		cmd.add_vertex( vertex( pos2, col, has_uv ? uv.circle( 2.f * 3.14159265358979323846 * ( idx2 / static_cast< float >( segments ) ) ) : vec2{ 0.f, 1.f } ) );
		cmd.add_index( 0, index, index + 1 );
	}

	add_command( cmd );
	return *this;
}

layer& layer::add_circle( const vec2& c, float r, const color& col, float thickness, int segments, float fill, float rot )
{
	if ( r == 0.f || segments <= 0 || fill <= 0.f )
		return *this;

	build_lookup_table( segments );

	const auto num_points = std::clamp( static_cast< int >( fill * segments ), 0, segments );

	const auto offset = static_cast< int >( rot * 0.00277777778f * static_cast< float >( segments ) ) % segments;
	auto prev_pos = vec2{};
	for ( auto i = offset; i < num_points + offset + 1; i++ )
	{
		const auto idx = i % segments;
		const auto pos = c + vec2( r * lookup_table[ segments ][ idx ].x, r * lookup_table[ segments ][ idx ].y );
		if ( i == offset )
		{
			prev_pos = pos;
			continue;
		}

		add_line( pos, prev_pos, col, thickness );
		prev_pos = pos;
	}

	return *this;
}

layer& layer::add_rect_filled_linear_gradient( const rect& r, const color color1, const color color2, const bool horizontal )
{
	if ( r.width() == 0.f || r.height() == 0.f )
		return *this;

	const std::array cs = { color1, horizontal ? color2 : color1, color2, horizontal ? color1 : color2 };

	return add_quad_filled_multicolor( r.tl(), r.tr(), r.br(), r.bl(), cs );
}

layer& layer::add_rect_filled_radial_gradient( const rect& r, const color col, const color col2 )
{
	if ( r.width() == 0.f || r.height() == 0.f )
		return *this;

	constexpr auto segments = 64;

	const auto c = r.center();

	const auto uv = get_uv().value_or( rect( 0.f, 0.f, 1.f, 1.f ) );
	const auto delta = ( float ) pi * 2.f / ( float ) segments;

	command cmd( this );
	cmd.add_vertex( vertex( c, col2, uv.center() ) );

	auto offset = 0.f;
	auto index = 1;
	for ( auto i = 0; i < segments; i++ )
	{
		const auto pos_a = c + vec2( r.size().x * cosf( offset - delta * 0.5f ), r.size().y * sinf( offset - delta * 0.5f ) );
		const auto pos_b = c + vec2( r.size().x * cosf( offset + delta * 0.5f ), r.size().y * sinf( offset + delta * 0.5f ) );

		cmd.add_vertex( vertex( pos_a, col, uv.circle( offset - delta * 0.5f ) ) );
		cmd.add_vertex( vertex( pos_b, col, uv.circle( offset + delta * 0.5f ) ) );
		cmd.add_index( 0, index, index + 1 );

		offset += delta;
		index += 2;
	}

	add_command( cmd );
	return *this;
}

layer& layer::add_rect_filled_multicolor( const rect& r, const std::array<color, 4>& cs )
{
	if ( r.width() == 0.f || r.height() == 0.f )
		return *this;

	return add_quad_filled_multicolor( r.tl(), r.tr(), r.br(), r.bl(), cs );
}

layer& layer::add_quad_filled_multicolor( const vec2& a, const vec2& b, const vec2& c, const vec2& d, const std::array<color, 4>& cs )
{
	const auto uv = get_uv().value_or( rect( 0.f, 0.f, 1.f, 1.f ) );

	command cmd( this );
	cmd.add_vertex( vertex( a, cs[ 0 ], uv.tl() ), vertex( b, cs[ 1 ], uv.tr() ), vertex( c, cs[ 2 ], uv.br() ), vertex( d, cs[ 3 ], uv.bl() ) );
	cmd.add_index( 0, 1, 2, 0, 2, 3 );

	add_command( cmd );
	return *this;
}

layer& layer::add_line_multicolor( const vec2& a, const vec2& b, const std::array<color, 2>& cs, float thickness )
{
	if ( thickness <= 0.f )
		return *this;

	auto dx = b.x - a.x;
	auto dy = b.y - a.y;

	const auto inv = 1.f / sqrtf( dx * dx + dy * dy );
	dx *= inv * ( thickness * 0.5f );
	dy *= inv * ( thickness * 0.5f );

	command cmd( this );
	cmd.add_vertex( vertex( vec2( a.x + dy, a.y - dx ), cs[ 0 ] ) );
	cmd.add_vertex( vertex( vec2( b.x + dy, b.y - dx ), cs[ 1 ] ) );
	cmd.add_vertex( vertex( vec2( b.x - dy, b.y + dx ), cs[ 1 ] ) );
	cmd.add_vertex( vertex( vec2( a.x - dy, a.y + dx ), cs[ 0 ] ) );
	cmd.add_index( 0, 1, 2, 0, 2, 3 );

	add_command( cmd );
	return *this;
}

layer& layer::add_quad( const vec2& a, const vec2& b, const vec2& c, const vec2& d, const color& col, float thickness )
{
	add_line( a, b, col, thickness );
	add_line( a, d, col, thickness );
	add_line( b, c, col, thickness );
	add_line( d, c, col, thickness );

	return *this;
}

layer& layer::add_quad_multicolor( const vec2& a, const vec2& b, const vec2& c, const vec2& d, const std::array<color, 4>& cs, float thickness )
{
	add_line_multicolor( a, b, { cs[ 0 ], cs[ 1 ] }, thickness );
	add_line_multicolor( a, d, { cs[ 0 ], cs[ 3 ] }, thickness );
	add_line_multicolor( b, c, { cs[ 1 ], cs[ 2 ] }, thickness );
	add_line_multicolor( d, c, { cs[ 3 ], cs[ 2 ] }, thickness );

	return *this;
}

layer& layer::add_rect_multicolor( const rect& r, const std::array<color, 4>& cs, float thickness )
{
	if ( r.width() == 0.f || r.height() == 0.f )
		return *this;

	return add_quad_multicolor( r.tl() + 0.5f, r.tr() + vec2( -0.49f, 0.49f ), r.br() - 0.49f, r.bl() - vec2( -0.49f, 0.49f ), cs, thickness );
}

layer& layer::add_circle_filled_multicolor( const vec2& c, float r, const std::array<color, 2>& cs, int segments, float fill, float rot )
{
	if ( r == 0.f || segments <= 0 || fill <= 0.f )
		return *this;

	build_lookup_table( segments );

	const auto has_uv = get_uv().has_value();

	const auto uv = get_uv().value_or( rect( 0.f, 0.f, 1.f, 1.f ) );
	command cmd( this );
	cmd.add_vertex( vertex( c, cs[ 0 ], uv.center() ) );

	const auto num_points = std::clamp( static_cast< int >( fill * segments ), 0, segments );
	const auto offset = static_cast< int >( rot * 0.00277777778f * static_cast< float >( segments ) ) % segments;

	auto index = 1;
	for ( auto i = offset + 1; i < num_points + offset + 1; i++, index += 2 )
	{
		const auto idx = ( i - 1 ) % segments;
		const auto idx2 = i % segments;

		const auto pos1 = c + vec2( r * lookup_table[ segments ][ idx ].x, r * lookup_table[ segments ][ idx ].y );
		const auto pos2 = c + vec2( r * lookup_table[ segments ][ idx2 ].x, r * lookup_table[ segments ][ idx2 ].y );

		cmd.add_vertex( vertex( pos1, cs[ 1 ], has_uv ? uv.circle( 2.f * 3.14159265358979323846 * ( idx / static_cast< float >( segments ) ) ) : vec2{ 0.f, 1.f } ) );
		cmd.add_vertex( vertex( pos2, cs[ 1 ], has_uv ? uv.circle( 2.f * 3.14159265358979323846 * ( idx2 / static_cast< float >( segments ) ) ) : vec2{ 0.f, 1.f } ) );
		cmd.add_index( 0, index, index + 1 );
	}

	add_command( cmd );
	return *this;
}

layer& layer::add_triangle( const vec2& a, const vec2& b, const vec2& c, const color& col, float thickness )
{
	add_line( a, b, col, thickness );
	add_line( b, c, col, thickness );
	add_line( a, c, col, thickness );

	return *this;
}

layer& layer::add_triangle_filled( const vec2& a, const vec2& b, const vec2& c, const color& col )
{
	const auto uv = get_uv().value_or( rect( 0.f, 0.f, 1.f, 1.f ) );
	const auto rot = get_rot().value_or( 0.f );
	const auto center = get_center( { a, b, c } );

	const auto 	_a = rotate_point( rot, a, center ),
		_b = rotate_point( rot, b, center ),
		_c = rotate_point( rot, c, center );

	command cmd( this );
	cmd.add_vertex( vertex( _b, col, vec2( uv.width() * 0.5f, uv.mins.y ) ), vertex( _a, col, uv.bl() ), vertex( _c, col, uv.br() ) );
	cmd.add_index( 0, 1, 2 );

	add_command( cmd );
	return *this;
}

layer& layer::add_triangle_multicolor( const vec2& a, const vec2& b, const vec2& c, const std::array<color, 3>& cs, float thickness )
{
	add_line_multicolor( a, b, { cs[ 0 ], cs[ 1 ] }, thickness );
	add_line_multicolor( b, c, { cs[ 1 ], cs[ 2 ] }, thickness );
	add_line_multicolor( a, c, { cs[ 0 ], cs[ 2 ] }, thickness );

	return *this;
}

layer& layer::add_triangle_filled_multicolor( const vec2& a, const vec2& b, const vec2& c, const std::array<color, 3>& cs )
{
	const auto uv = get_uv().value_or( rect( 0.f, 0.f, 1.f, 1.f ) );
	const auto rot = get_rot().value_or( 0.f );
	const auto center = get_center( { a, b, c } );

	const auto 	_a = rotate_point( rot, a, center ),
		_b = rotate_point( rot, b, center ),
		_c = rotate_point( rot, c, center );

	command cmd( this );
	cmd.add_vertex( vertex( _b, cs[ 1 ], vec2( uv.width() * 0.5f, uv.mins.y ) ), vertex( _a, cs[ 0 ], uv.bl() ), vertex( _c, cs[ 2 ], uv.br() ) );
	cmd.add_index( 0, 1, 2 );

	add_command( cmd );
	return *this;
}

layer& layer::add_shadow( const rect& r, char dir, float a )
{
	if ( dir == dir_left )
	{
		add_rect_filled_multicolor( r, {
			color( 0.f, 0.f, 0.f, 0.f ),
			color( 0.f, 0.f, 0.f, a ),
			color( 0.f, 0.f, 0.f, a ),
			color( 0.f, 0.f, 0.f, 0.f )
		} );
	}

	if ( dir == dir_right )
	{
		add_rect_filled_multicolor( r, {
			color( 0.f, 0.f, 0.f, a ),
			color( 0.f, 0.f, 0.f, 0.f ),
			color( 0.f, 0.f, 0.f, 0.f ),
			color( 0.f, 0.f, 0.f, a )
		} );
	}

	if ( dir == dir_up )
	{
		add_rect_filled_multicolor( r, {
			color( 0.f, 0.f, 0.f, 0.f ),
			color( 0.f, 0.f, 0.f, 0.f ),
			color( 0.f, 0.f, 0.f, a ),
			color( 0.f, 0.f, 0.f, a )
		} );
	}

	if ( dir == dir_down )
	{
		add_rect_filled_multicolor( r, {
			color( 0.f, 0.f, 0.f, a ),
			color( 0.f, 0.f, 0.f, a ),
			color( 0.f, 0.f, 0.f, 0.f ),
			color( 0.f, 0.f, 0.f, 0.f )
		} );
	}

	return *this;
}

layer& layer::add_shadow( const rect& r, float radius, float a, bool bg )
{
	if ( bg )
		add_rect_filled( r, color( 0.f, 0.f, 0.f, a ) );

	add_rect_filled_multicolor( rect( r.mins - vec2( radius, 0.f ), r.mins + vec2( 0.f, r.height() ) ), {
		color( 0.f, 0.f, 0.f, 0.f ),
		color( 0.f, 0.f, 0.f, a ),
		color( 0.f, 0.f, 0.f, a ),
		color( 0.f, 0.f, 0.f, 0.f )
	} );
	add_rect_filled_multicolor( rect( r.mins + vec2( r.width(), 0.f ), r.mins + vec2( r.width() + radius, r.height() ) ), {
		color( 0.f, 0.f, 0.f, a ),
		color( 0.f, 0.f, 0.f, 0.f ),
		color( 0.f, 0.f, 0.f, 0.f ),
		color( 0.f, 0.f, 0.f, a )
	} );
	add_rect_filled_multicolor( rect( r.mins - vec2( 0.f, radius ), r.mins + vec2( r.width(), 0.f ) ), {
		color( 0.f, 0.f, 0.f, 0.f ),
		color( 0.f, 0.f, 0.f, 0.f ),
		color( 0.f, 0.f, 0.f, a ),
		color( 0.f, 0.f, 0.f, a )
	} );
	add_rect_filled_multicolor( rect( r.maxs - vec2( r.width(), 0.f ), r.maxs + vec2( 0.f, radius ) ), {
		color( 0.f, 0.f, 0.f, a ),
		color( 0.f, 0.f, 0.f, a ),
		color( 0.f, 0.f, 0.f, 0.f ),
		color( 0.f, 0.f, 0.f, 0.f )
	} );

	add_circle_filled_multicolor(
			r.tl(),
			radius,
			{ color( 0.f, 0.f, 0.f, a ), color::black_transparent() },
			4,
			0.25f,
			180.f
	);
	add_circle_filled_multicolor(
			r.tr(),
			radius,
			{ color( 0.f, 0.f, 0.f, a ), color::black_transparent() },
			4,
			0.25f,
			270.f
	);
	add_circle_filled_multicolor(
			r.br(),
			radius,
			{ color( 0.f, 0.f, 0.f, a ), color::black_transparent() },
			4,
			0.25f,
			0.f
	);
	add_circle_filled_multicolor(
			r.bl(),
			radius,
			{ color( 0.f, 0.f, 0.f, a ), color::black_transparent() },
			4,
			0.25f,
			90.f
	);

	return *this;
}

layer& layer::add_rect_rounded( const rect& r, const color& c, float f, char rnd, float thickness )
{
	if ( rnd & rnd_tl )
		add_circle( r.mins + f - 1.f, f, c, thickness, 64, 0.25f, 180.f );
	if ( rnd & rnd_tr )
		add_circle( r.mins + vec2( r.width() - f, f - 1.f ), f, c, thickness, 64, 0.25f, 270.f );
	if ( rnd & rnd_br )
		add_circle( r.maxs - f, f, c, thickness, 64, 0.25f, 0.f );
	if ( rnd & rnd_bl )
		add_circle( r.mins + vec2( f - 1.f, r.height() - f ), f, c, thickness, 64, 0.25f, 90.f );

	add_line(
			r.mins + vec2( rnd & rnd_tl ? f - 1.f : 0.f, 0.f ),
			r.mins + vec2( r.width() - ( rnd & rnd_tr ? f : 0.f ), 0.f ),
			c,
			thickness
	);
	add_line(
			r.mins + vec2( r.width(), rnd & rnd_tr ? f - 1.f : 0.f ),
			r.maxs - vec2( 0.f, rnd & rnd_br ? f : 0.f ),
			c,
			thickness
	);
	add_line(
			r.mins + vec2( rnd & rnd_bl ? f - 1.f : 0.f, r.height() ),
			r.maxs - vec2( rnd & rnd_br ? f : 0.f, 0.f ),
			c,
			thickness
	);
	add_line(
			r.mins + vec2( 0.f, rnd & rnd_tl ? f - 1.f : 0.f ),
			r.mins + vec2( 0.f, r.height() - ( rnd & rnd_bl ? f : 0.f ) ),
			c,
			thickness
	);

	return *this;
}

layer& layer::add_rect_filled_rounded( const rect& r, const color& c, float f, char rnd )
{
	if ( rnd & rnd_tl )
		add_circle_filled( r.mins + f, f, c, 24, 0.25f, 180.f );

	if ( rnd & rnd_tr )
		add_circle_filled( r.mins + vec2( r.width() - f, f ), f, c, 24, 0.25f, 270.f );

	if ( rnd & rnd_br )
		add_circle_filled( r.maxs - f, f, c, 24, 0.25f, 0.f );

	if ( rnd & rnd_bl )
		add_circle_filled( r.mins + vec2( f, r.height() - f ), f, c, 24, 0.25f, 90.f );

	add_rect_filled( rect( r.mins + f, r.maxs - f ), c );
	add_rect_filled( rect( r.mins + vec2( rnd & rnd_tl ? f : 0.f, 0.f ), r.mins + vec2( r.width() - ( rnd & rnd_tr ? f : 0.f ), f ) ), c );
	add_rect_filled( rect( r.mins + vec2( r.width() - f, f ), r.maxs - vec2( 0.f, f ) ), c );
	add_rect_filled( rect( r.mins + vec2( rnd & rnd_bl ? f : 0.f, r.height() - f ), r.maxs - vec2( rnd & rnd_br ? f : 0.f, 0.f ) ), c );
	add_rect_filled( rect( r.mins + vec2( 0.f, f ), r.mins + vec2( f, r.height() - f ) ), c );

	return *this;
}

void layer::add_command( const command& cmd )
{
	commands.emplace_back( cmd );
}
