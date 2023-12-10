#include "../../menu_include.h"

Color copied_color = Color::Black();

using namespace evo::ren;

void copy_color()
{
	if ( !g_menu->get_focussed_control() || !g_menu->get_focussed_control()->get_focussed_control() )
		return;

	copied_color = reinterpret_cast< color_picker* >( g_menu->get_focussed_control()->get_focussed_control() )->get_var()->get<D3DCOLOR>();
}

void paste_color()
{
	if ( !g_menu->get_focussed_control() || !g_menu->get_focussed_control()->get_focussed_control() )
		return;

	const auto temp_col = reinterpret_cast< color_picker* >( g_menu->get_focussed_control()->get_focussed_control() )->get_flags() & colorpicker_style::no_alpha_bar ? Color( copied_color, 255 ) : copied_color;
	reinterpret_cast< color_picker* >( g_menu->get_focussed_control()->get_focussed_control() )->get_var()->set<D3DCOLOR>( temp_col.GetD3DColor() );
	reinterpret_cast< color_picker* >( g_menu->get_focussed_control()->get_focussed_control() )->force_color();
}

color_picker::color_picker( value* var, const colorpicker_style::flags flags )
	: m_popup_open( false ), m_needs_press( false ), m_holding_controls( false ), m_popup_size( { 240, 300 } ), m_flags( flags )
{
	m_var = var;
	m_max_size = 35.f;

	m_hue = new value( "", nullptr, 0.0 );
	m_alpha = new value( "", nullptr, 100.0 );

	m_sub_controls.push_back( std::make_shared<hue_slider>( _w( "" ), m_hue, 0.f, 100.f,
		slider_style::flags::no_operators | slider_style::flags::no_focus,
		"" )->add_manual_padding() );

	if ( !( m_flags & colorpicker_style::no_alpha_bar ) )
		m_sub_controls.push_back( std::make_shared<alpha_slider>( _w( "" ), m_alpha, m_var, 0.f, 100.f,
		slider_style::flags::no_operators | slider_style::flags::no_focus, "" )->add_manual_padding() );

	m_sub_controls.push_back( std::make_shared<button>( _w( "Copy" ), copy_color )->add_manual_padding() );
	m_sub_controls.push_back( std::make_shared<button>( _w( "Paste" ), paste_color )->add_manual_padding() );

/*
 * Mach ich wann anders
 */

//m_sub_controls.push_back( std::make_shared<checkbox>( "RAINBOW", m_rainbow )->add_manual_padding( ) );
//m_sub_controls.push_back( std::make_shared<slider>( "RAINBOW OFFSET", m_rainbow_offset, 0.f, 100.f, slider_style::flags::no_operators | slider_style::flags::no_focus, "" )->add_dependency( { m_rainbow, 1.0 } )->add_manual_padding( ) );
}

void color_picker::run_render( const evo::ren::vec2 pos, const evo::ren::vec2 size )
{
	if ( !should_render() )
		return;

	auto& d = draw.get_layer( layer::foreground );

	if ( m_in_force_off || m_parent->get_focussed_control() != this )
	{
		handle_hover_progress();

		m_pos = pos;
		m_size = size;

		d.push_clip( rect_legacy( m_pos, m_size ) );

		/*
		 * Render preview first
		 */

		/*
		 * Border of preview
		 */
		d.add_rect( rect_legacy( { m_pos.x, m_pos.y }, { m_size.x, m_size.y } ), gui_color::palette::border() );
		d.add_rect( rect_legacy( { m_pos.x, m_pos.y }, { m_size.x, m_size.y } ), m_popup_open ? gui_color::palette::main_pink() : gui_color::palette::main_pink( m_hover_progress * 255 ) );


		d.push_clip( rect_legacy( m_pos + 1.f, m_size - 2.f ) );
		/*
		 * Inner
		 */
		d.add_rect_filled( rect_legacy( { m_pos.x + 1.f, m_pos.y + 1.f }, { m_size.x - 2.f, m_size.y - 2.f } ), gui_color::custom( Color( 242, 242, 242 ) ) );


		//upper
		d.add_rect_filled( rect_legacy( { m_pos.x + 2, m_pos.y + 2 }, { 5, 5 } ), gui_color::custom( Color( 213, 213, 213 ) ) );
		d.add_rect_filled( rect_legacy( { m_pos.x + 2 + 10, m_pos.y + 2 }, { 5, 5 } ), gui_color::custom( Color( 213, 213, 213 ) ) );
		d.add_rect_filled( rect_legacy( { m_pos.x + 2 + 20, m_pos.y + 2 }, { 5, 5 } ), gui_color::custom( Color( 213, 213, 213 ) ) );

		//lower
		d.add_rect_filled( rect_legacy( { m_pos.x + 2 + 5, m_pos.y + 2 + 5 }, { 5, 5 } ), gui_color::custom( Color( 213, 213, 213 ) ) );
		d.add_rect_filled( rect_legacy( { m_pos.x + 2 + 15, m_pos.y + 2 + 5 }, { 5, 5 } ), gui_color::custom( Color( 213, 213, 213 ) ) );

		d.pop_clip();

		/*
		 * Color filling
		 */

		m_color = Color::FromHSB( m_hue->get<float>() * 0.01f, 1.f, 1.f ).direct();

		const auto col = get_color();

		d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 1, m_pos.y + 1 }, { m_size.x - 2, m_size.y - 2 } ), gui_color::custom( col ), gui_color::custom( Color( col.r(), col.g(), col.b(), static_cast< int >( col.a() * 0.8f ) ) ) );


		d.pop_clip();
	}
	/*
	 * Now render the popup of needed
	 */
	if ( !m_popup_open || m_in_force_off )
		return;

	d.push_clip( rect_legacy( m_popup_pos, m_popup_size ), false );
	/*
	* Setup close button position
	*/
	m_close_pos = { m_popup_pos.x + 4, m_popup_pos.y + 4 };

	/*
	* Inner
	*/
	d.add_rect_filled_linear_gradient( rect_legacy( m_popup_pos, m_popup_size ), gui_color::control_top(), gui_color::control_bot() );

	/*
	* Border
	*/
	d.add_rect( rect_legacy( m_popup_pos, m_popup_size ), gui_color::palette::border() );

	static auto factor = 0.f;

	static auto should_add = false;

	VAL_SMOOTHER( factor, .4f, 1.f, should_add, .6f / 1.75f );

	/*
	 * Only half the alpha at max
	 * so it can still be visible
	 */
	if ( factor <= .4f || factor >= 1.f )
		should_add = !should_add;

	d.add_rect_filled( rect_legacy( { m_popup_pos.x + 1, m_popup_pos.y + 1 }, { m_popup_size.x - 2, 2 } ), gui_color::palette::main_pink() );

	render_color_picker();

	//render_color_mouse_preview();

	const auto control_pos = m_popup_pos + vec2( g_menu->get_style( e_menu_style::child_padding ).x + 1, g_menu->get_style( e_menu_style::child_padding ).y + 1 );

	const auto pos1 = control_pos + m_cursor - 3.f;
	auto color = Color( get_color(), 255 );
	d.add_rect( rect_legacy( { pos1.x, pos1.y }, { 7.f, 7.f } ), Color( 255 - color.brightness() * 255, 255 - color.brightness() * 255, 255 - color.brightness() * 255 ).direct() );

	d.pop_clip();
}

void color_picker::run_input()
{
	if ( m_in_force_off )
		return;

	if ( !should_render() )
		return;

	m_var->set<D3DCOLOR>( static_cast< unsigned int >( m_var->get<double>() ) );

	m_hovered = is_hovered( m_pos, m_size );

	/*
	 * Toggle popup
	 */
	if ( m_hovered && g_input->get_mouse().state == e_mouse_state::pressed )
	{
		m_popup_pos = g_input->get_mouse().pos;
		m_popup_open = true;
		m_needs_press = true;

		m_parent->set_focussed_control( this );
		g_menu->set_focussed_control( m_parent.get() );
	}

	/*
	 * No need to call functions below if popup is closed
	 *
	 */
	if ( !m_popup_open )
	{
		force_color();
		return;
	}

		/*
	 * Calc size every input tick because it may be resized
	 * when checking the rainbow checkbox etc.
	 */
	calc_popup_size();

	if ( m_hovered && g_input->get_mouse().state == e_mouse_state::pressed && m_flags & colorpicker_style::open_left )
		m_popup_pos.x -= m_popup_size.x;

	/*
	 * Check if mouse is in popup area
	 */
	const auto in_popup = g_input->mouse_in_bounds( m_popup_pos, m_popup_size );

	/*
	 * Was clicked outside of popup - close it
	 */
	if ( !in_popup && !m_needs_press && g_input->get_mouse().state == e_mouse_state::pressed )
	{
		m_popup_open = false;

		if ( this == m_parent->get_focussed_control() )
			m_parent->set_focussed_control( nullptr );
		if ( m_parent.get() == g_menu->get_focussed_control() )
			g_menu->set_focussed_control( nullptr );

		return;
	}

	/*
	* Positions of controls ( brighness etc. )
	*/
	const auto control_pos = m_popup_pos + vec2( g_menu->get_style( e_menu_style::child_padding ).x + 1, g_menu->get_style( e_menu_style::child_padding ).y + 1 );
	const auto control_size = vec2( m_popup_size.x - g_menu->get_style( e_menu_style::child_padding ).x * 2 - 2, 200 - 2 );
	const auto in_control_area = is_hovered( control_pos, control_size );

	/*
	* Received press, continue
	*/
	if ( m_needs_press && g_input->get_mouse().state == e_mouse_state::pressed )
		m_needs_press = false;

	/*
	 * If we still need input stop here
	 */
	if ( m_needs_press )
		return;

	/*
	 * Allow input again
	 */
	if ( m_holding_controls && g_input->get_mouse().state == e_mouse_state::idle )
		m_holding_controls = false;

	/*
	 * Mouse started to control controls - toggle state
	 */
	if ( in_control_area && g_input->get_mouse().state != e_mouse_state::idle )
		m_holding_controls = true;

	if ( g_input->get_mouse().state != e_mouse_state::idle && m_holding_controls )
	{
		const auto size = vec2( m_popup_size.x - g_menu->get_style( e_menu_style::child_padding ).x * 2 - 2, 200 - 2 );
		m_cursor.x = clamp( g_input->get_mouse().pos.x - control_pos.x, 0.f, size.x );
		m_cursor.y = clamp( g_input->get_mouse().pos.y - control_pos.y, 0.f, size.y );
		update_color();
	}

	/*
	 * New color selected
	 */
	if ( ( g_input->get_mouse().state != e_mouse_state::idle || g_input->get_mouse().state_right != e_mouse_state::idle )
		 && !in_control_area )
		update_color();
}

void color_picker::render_close_button() const
{
	auto& d = draw.get_layer( layer::foreground );
	/*
	 * Top left - Down right
	 */
	for ( auto i = 0; i < 3; i++ )
		d.add_rect_filled( rect_legacy( { m_close_pos.x + i, m_close_pos.y + i }, { 1, 1 } ), gui_color::text_active() );

	/*
	* Down left - Top right
	*/
	for ( auto i = 0; i < 3; i++ )
		d.add_rect_filled( rect_legacy( { m_close_pos.x + i, m_close_pos.y + 2 - i }, { 1, 1 } ), gui_color::text_active() );
}

void color_picker::render_color_mouse_preview() const
{
	/*
	* Check if mouse is in popup area, if not dont render
	*/
	const auto in_popup = g_input->mouse_in_bounds( m_popup_pos, m_popup_size );

	/*
	* Positions of controls ( brighness etc. )
	*/
	const auto control_pos = m_popup_pos + vec2( 0, g_menu->get_style( e_menu_style::child_padding ).y + 204 );
	const auto control_size = vec2( m_popup_size.x, m_control_size.y );
	const auto in_control_area = is_hovered( control_pos, control_size );

	/*
	 * Dont render if we're in control area
	 */
	if ( in_control_area || m_holding_controls )
		return;

	/*
	 * Render preview box of defined color near mouse
	 */
	g_menu->set_mouse_mode( !in_popup ? window_style::mouse_mode::color_hint : window_style::mouse_mode::color );

	/*
	 * Set color
	 */
	g_menu->set_mouse_box_color( m_hovering_color );
}

void color_picker::render_color_picker()
{
	auto& d = draw.get_layer( layer::foreground );

	const auto pos = m_popup_pos + g_menu->get_style( e_menu_style::child_padding );
	const auto size = vec2( m_popup_size.x - g_menu->get_style( e_menu_style::child_padding ).x * 2, 200 );

	d.add_rect( rect_legacy( pos, size ), gui_color::palette::border() );

	d.add_rect_filled_linear_gradient( rect_legacy( pos + 1, size - 2 ), Color::White().direct(), m_color, true );
	d.add_rect_filled_linear_gradient( rect_legacy( pos + 1, size - 2 ), Color( Color::Black(), 0 ).direct(), Color::Black().direct() );

	/*
	 * Additional controls
	 */
	auto start_pos = vec2( { pos.x, pos.y + size.y + g_menu->get_style( e_menu_style::child_padding ).x } );

	auto count = 0;
	for ( const auto& control : m_sub_controls )
	{
		if ( count < floor( m_sub_controls.size() / 2.f ) )
		{
			control->set_size( { size.x, floorf( control->get_string_size().y * control->get_string_size_multiplier() ) } );
			control->run_render( start_pos );

			/*
			 * We need to do the padding manually
			 */
			start_pos.y += std::roundf( control->get_string_size().y + g_menu->get_style( e_menu_style::control_padding ).y );
		}
		else
		{
			control->set_size( { 70, floorf( control->get_string_size().y + 2 ) } );
			auto newpos = start_pos + vec2( ( count - floor( m_sub_controls.size() / 2.f ) ) * 75, 0 );
			control->run_render( newpos );
		}

		count++;
	}

	char hexcol[ 17 ];
	const auto col = Color( m_var->get<D3DCOLOR>() );
	_( format, "#%02X%02X%02X" );
	snprintf( hexcol, sizeof hexcol, format.c_str(), col.r(), col.g(), col.b() );
	auto newpos = start_pos + vec2( 11 + 2 * 75, -1 );
	d.add_text( draw.get_font( FNV1A( "control" ) ), newpos + vec2{ 0, 4 }, hexcol, gui_color::text() );
}

void color_picker::calc_popup_size()
{
	/*
	 * Picker size + padding
	 */
	const auto base_size = 200.f + g_menu->get_style( e_menu_style::child_padding ).x * 2;

	/*
	 * Go through every control and add to size if needed
	 */
	auto item_size = g_menu->get_style( e_menu_style::control_padding ).y;
	auto count = 0;
	for ( const auto& control : m_sub_controls )
	{
		/*
		 * Run input first to get up-to-date data
		 */
		control->run_input();

		/*
		 * Only add size if the control is actually getting rendered
		 */
		if ( control->should_render() && count < floor( m_sub_controls.size() / 2.f ) )
			item_size += control->get_string_size().y + g_menu->get_style( e_menu_style::control_padding ).y;

		count++;
	}

	const auto size = base_size + item_size + 22;

	m_control_size.y = item_size;
	m_popup_size.y = size;
}

Color color_picker::get_color()
{
	return m_var->get<D3DCOLOR>();
}
