#include "../menu_include.h"

using namespace evo::ren;

void alpha_slider::run_render( evo::ren::vec2& pos )
{
	if ( !should_render() )
		return;

	m_pos = pos;

	handle_hover_progress();

	/*
	* Name of the slider
	*/

	/*
	 * Store position where operators will be drawn
	 */
	m_operator_pos = { floorf( m_pos.x + m_size.x - g_menu->get_style( e_menu_style::window_padding ).x ), floorf( m_pos.y + m_string_size.y / 2 + 2 ) };

	/*
	* Start drawing the actual slider below the text
	*/
	_u( name, m_name );
	if ( !name.empty() )
	{
		m_pos.y += m_string_size.y;
	}


	auto& d = draw.get_layer( layer::foreground );

	/*
	* Inner
	*/
	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 1, m_pos.y + 1 }, { m_size.x - 1, m_string_size.y * .65f - 2 } ), gui_color::control_top(), gui_color::control_bot() );
	d.add_rect_filled( rect_legacy( { m_pos.x + 2, m_pos.y + 2 }, { m_size.x - 3, m_string_size.y * .65f - 2 } ), gui_color::custom( color( 242, 242, 242 ) )  );

	//upper
	for ( size_t i = 0; i < m_size.x - 12; i += 10 )
	{
		d.add_rect_filled( rect_legacy( { m_pos.x + 2 + i, m_pos.y + 2 }, { 5, 4 } ), color( 213, 213, 213 ) );
	}

	//lower
	for ( size_t i = 0; i < m_size.x - 12; i += 10 )
	{
		d.add_rect_filled( rect_legacy( { m_pos.x + 2 + 5 + i, m_pos.y + 2 + 4 }, { 5, 4 } ), color( 213, 213, 213 ) );
	}


	/*
	* Border
	*/
	d.add_rect( rect_legacy( { m_pos.x, m_pos.y }, { m_size.x + 1, m_string_size.y * .75f } ), gui_color::palette::border() );
	d.add_rect( rect_legacy( { m_pos.x, m_pos.y }, { m_size.x + 1, m_string_size.y * .75f } ), m_dragging ? gui_color::palette::main_pink() : gui_color::palette::main_pink( m_hover_progress * 255 ) );

	/*
	 * Slider bounds
	 */
	const auto slider_pos = Vector2D( m_pos.x + 2, m_pos.y + 3 );
	const auto slider_size = Vector2D( ( m_size.x - 3 ) * ( m_var->get<float>() - m_min ) / static_cast< float >( m_max - m_min ), m_string_size.y * .75f - 3 );

	/*
	 * Value slider
	 */

	d.add_rect_filled_linear_gradient( rect_legacy( { slider_pos.x, slider_pos.y - 1 }, { m_size.x - 3, slider_size.y - 1 } ),
											 gui_color::custom( Color( m_color->get<D3DCOLOR>(), 0 ) ), gui_color::custom( Color( m_color->get<D3DCOLOR>(), 255 ) ), true );

	d.add_rect_filled( rect_legacy( { slider_pos.x + slider_size.x - 2, slider_pos.y - 4 }, { 4, slider_size.y + 5 } ), gui_color::palette::main_pink() );

	/*
	* Reset position and adjust for controls after
	*/
	m_pos = pos;

	if ( !m_manual_padding )
		pos.y += m_size.y + g_menu->get_style( e_menu_style::control_padding ).y;
}

