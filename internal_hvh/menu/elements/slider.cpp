#include "../menu_include.h"

using namespace evo::ren;

slider::slider( std::string name, value* var, const float min, const float max, const uint8_t flags, std::string format, const float step )
	: m_min( min ), m_max( max ), m_step( step ), m_dragging( false ), m_format( std::move( format ) ), m_flags( flags )
{
	m_name = std::move( name );
	m_var = var;
	m_string_size_multiplier = 1.75f;
	m_fraction = 0.f;
	m_in_slider_area = false;
}

void slider::run_render( vec2& pos )
{
	if ( !should_render() )
		return;

	m_pos = pos;

	handle_hover_progress();

	auto& d = draw.get_layer( layer::foreground );
	/*
	* Name of the slider
	*/
	_u( name, m_name );
	d.add_text( draw.get_font( FNV1A( "control" ) ), m_pos + vec2{ 0, 5 }, name, gui_color::text() );

	/*
	 * Store position where operators will be drawn
	 */
	m_operator_pos = { floorf( m_pos.x + m_size.x - g_menu->get_style( e_menu_style::window_padding ).x ), floorf( m_pos.y + m_string_size.y / 2 + 2 ) };

	/*
	* Start drawing the actual slider below the text
	*/
	if ( !name.empty() )
	{
		m_pos.y += m_string_size.y;
	}


	/*
	* Inner
	*/
	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 1, m_pos.y + 1 }, { m_size.x - 1, m_string_size.y * .65f } ), gui_color::control_top(), gui_color::control_bot() );

	/*
	* Border
	*/
	d.add_rect( rect_legacy( { m_pos.x, m_pos.y }, { m_size.x, ceil( m_string_size.y * .75f ) + 1 } ), gui_color::palette::border() );
	d.add_rect( rect_legacy( { m_pos.x, m_pos.y }, { m_size.x, ceil( m_string_size.y * .75f ) + 1 } ), m_dragging ? gui_color::palette::main_pink() : gui_color::palette::main_pink( m_hover_progress * 255 ) );

	/*
	 * Slider bounds
	 */
	const auto slider_pos = vec2( m_pos.x + 2, m_pos.y + 2 );
	const auto slider_size = vec2( ( m_size.x - 3 ) * clamp( ( m_var->get<float>() - m_min ) / static_cast< float >( m_max - m_min ), 0.f, 1.f ), m_string_size.y * .75f - 3 );

	/*
	 * Value slider
	 */
	d.add_rect_filled_linear_gradient( rect_legacy( slider_pos, slider_size ), gui_color::palette::main_pink( 255 ), gui_color::palette::main_pink( 150 ) );

	/*
	 * Value text
	 */
	const auto display_text = get_value_text();
	const auto display_text_pos = slider_pos.x + ( m_size.x - 3 ) / 2;

	//render::rect_filled( { slider_pos.x + slider_size.x / 2 - display_text_size.x / 2, slider_pos.y }, { display_text_size.x, slider_size.y }, gui_color::child_main( 100 ) );
	d.add_text( draw.get_font( FNV1A( "control" ) ), { display_text_pos, slider_pos.y + slider_size.y / 2 + 2 }, display_text, gui_color::text(), layer::align_center, layer::align_center );

	render_operators();
	/*
	* Render all sub controls registered
	*/
	render_sub_controls();

	/*
	* Reset position and adjust for controls after
	*/
	m_pos = pos;

	if ( !m_manual_padding )
		pos.y += m_size.y + g_menu->get_style( e_menu_style::control_padding ).y;
}

void slider::run_input()
{
	calc_string_size();

	auto multi = m_string_size_multiplier;
	_u( name, m_name );
	if ( name.empty() )
		multi = 1;

	if ( m_parent )
		m_size = { m_parent->get_size().x / 2, m_string_size.y * multi };
	else
		m_size.y = m_string_size.y * multi;


	if ( m_max >= m_min )
		m_var->set<float>( clamp( m_var->get<float>(), m_min, m_max ) );

	if ( !should_render() )
		return;

	m_hovered = is_hovered( m_pos, m_size );
	m_in_slider_area = g_input->mouse_in_bounds( m_pos + vec2( 0, name.empty() ? 0 : m_string_size.y ), { m_size.x, m_string_size.y * .75f } );

	/*
	 * Handle operator
	 */
	handle_operators();

	if ( !m_dragging && m_in_slider_area )
	{
		auto val = m_var->get<float>();

		if ( g_input->get_key( VK_LEFT ).pressed || g_input->get_key( VK_LEFT ).held )
			val -= m_step;
		if ( g_input->get_key( VK_RIGHT ).pressed || g_input->get_key( VK_RIGHT ).held )
			val += m_step;

		/*
		 * Clamp values so we don't exceed the min/max
		 */
		if ( m_max >= m_min )
			val = clamp( val, m_min, m_max );

		m_var->set( val );
	}

	/*
	 * Check if the slider is being toggled
	 */
	if ( !m_dragging && m_in_slider_area && g_input->get_mouse().state != e_mouse_state::idle )
	{
		m_dragging = true;

		if ( !( m_flags & slider_style::flags::no_focus ) )
			g_menu->set_focussed_control( this );
	}

	/*
	 * Don't continue further if the slider isn't active
	 */
	if ( !m_dragging )
		return;

	/*
	 * Mouse lost focus - reset state
	 */
	if ( g_input->get_mouse().state == e_mouse_state::idle ||
		( m_flags & slider_style::flags::no_focus && ( g_input->get_mouse().pos.y < m_pos.y || g_input->get_mouse().pos.y > m_pos.y + m_size.y ) ) )
	{
		m_dragging = false;

		if ( !( m_flags & slider_style::flags::no_focus ) )
			g_menu->set_focussed_control( nullptr );

		return;
	}

	/*
	 * Calculate fraction
	 */
	calc_fraction();

	/*
	 * Set value dependent on fraction
	 */
	m_var->set( static_cast< int >( m_min + ( m_max - m_min ) * m_fraction ) );
}

std::string slider::get_value_text() const
{
	_u( name, m_name );
	if ( m_var->get<float>() == 0.f && fnv1a_rt( name ) == FNV1A( "Damage accuracy" ) )
	{
		_( str, "Auto" );
		return std::string( str );
	}

	if ( m_var->get<float>() == 10000.f && fnv1a_rt( name ) == FNV1A( "Stattrak" ) )
	{
		_( str, "Match kills" );
		return std::string( str );
	}

	if ( m_var->get<float>() > 100.f && name.starts_with( xorstr_( "Minimum damage" ) ) )
	{
		char ret[ 64 ];
		_u( format, m_format );
		sprintf_s( ret, format.c_str(), std::min( m_var->get<float>(), 100.f ) );
		return std::string( ret ) += "+" + std::to_string( static_cast< int >( roundf( m_var->get< float >() ) ) - 100 );
	}

	char ret[ 64 ];

	// ReSharper disable once CppDeprecatedEntity
	if ( m_format.empty() )
	{
		sprintf_s( ret, m_format.c_str(), m_var->get<float>() );
	}
	else
	{
		_u( format, m_format );
		sprintf_s( ret, format.c_str(), m_var->get<float>() );
	}

	return std::string( ret );
}

void slider::calc_fraction()
{
	/*
	* Store mouse position to calculate fraction
	*/
	const auto mouse_pos = g_input->get_mouse().pos;

	/*
	* Get the delta between the mouse position and start
	* of the slider
	*/
	auto mouse_delta = mouse_pos.x - m_pos.x;

	/*
	* Clamp the delta and divide with the size to get
	* a value between 0 and 1
	*/
	mouse_delta = clamp( mouse_delta, 0.f, m_size.x ) / m_size.x;

	/*
	* Calculate the fraction by looking how far
	* away the mouse is from the right border
	*/
	m_fraction = mouse_delta;
}

void slider::render_operators() const
{
	if ( m_flags & slider_style::flags::no_operators )
		return;

	auto& d = draw.get_layer( layer::foreground );

	/*
	 * Left operator ( minus )
	 */
	d.add_line( { m_operator_pos.x - 2, m_operator_pos.y }, { m_operator_pos.x + 2, m_operator_pos.y }, gui_color::text() );

	/*
	 * Right operator ( plus )
	 */
	d.add_line( { m_operator_pos.x + 7, m_operator_pos.y }, { m_operator_pos.x + 12, m_operator_pos.y }, gui_color::text() );
	d.add_line( { m_operator_pos.x + 10, m_operator_pos.y - 3 }, { m_operator_pos.x + 10, m_operator_pos.y + 2 }, gui_color::text() );
}

void slider::handle_operators() const
{
	if ( m_flags & slider_style::flags::no_operators )
		return;

	/*
	 * No mouse input
	 */
	if ( g_input->get_mouse().state != e_mouse_state::pressed )
		return;

	const auto left_operator_pressed = g_input->mouse_in_bounds( { m_operator_pos.x - 4, m_operator_pos.y - 2 }, { 6, 6 } );
	const auto right_operator_pressed = g_input->mouse_in_bounds( { m_operator_pos.x + 6, m_operator_pos.y - 2 }, { 6, 6 } );

	auto val = m_var->get<float>();

	if ( left_operator_pressed )
		val -= m_step;
	else if ( right_operator_pressed )
		val += m_step;

	/*
	 * Clamp values so we don't exceed the min/max
	 */
	if ( m_max >= m_min )
		val = clamp( val, m_min, m_max );

	m_var->set( val );
}


