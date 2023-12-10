#include "../menu_include.h"

using namespace evo::ren;

void hue_slider::run_render( vec2& pos )
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
	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 1, m_pos.y + 1 }, { m_size.x - 2, m_string_size.y * .65f - 2 } ), gui_color::control_top(), gui_color::control_bot() );

	/*
	* Border
	*/
	d.add_rect( rect_legacy( { m_pos.x, m_pos.y }, { m_size.x, m_string_size.y * .75f } ), gui_color::palette::border() );
	d.add_rect( rect_legacy( { m_pos.x, m_pos.y }, { m_size.x, m_string_size.y * .75f } ), m_dragging ? gui_color::palette::main_pink() : gui_color::palette::main_pink( m_hover_progress * 255 ) );

	/*
	 * Slider bounds
	 */
	const auto slider_pos = Vector2D( m_pos.x + 2, m_pos.y + 3 );
	const auto slider_size = Vector2D( ( m_size.x - 3 ) * ( m_var->get<float>() - m_min ) / static_cast< float >( m_max - m_min ), m_string_size.y * .75f - 3 );

	/*
	 * Value slider
	 */
	// split bar into 6 segments
	const auto segment_size = ( m_size.x - 3 ) / 6.f;

	for ( auto i = 0u; i < 6u; i++ )
	{
		const auto cur_color = Color::FromHSB( ( 1.f / 6.f ) * i, 1.f, 1.f );
		const auto next_color = Color::FromHSB( ( 1.f / 6.f ) * ( i + 1 ), 1.f, 1.f );

		const auto cur_segment = segment_size * i;

		d.add_rect_filled_linear_gradient( rect_legacy( { slider_pos.x + cur_segment, slider_pos.y - 1 }, { i == 5 ? floor( segment_size ) - 1 : ceil( segment_size ), slider_size.y - 1 } ),
											 cur_color.direct(), next_color.direct(), true );
	}

	d.add_rect_filled( rect_legacy( { slider_pos.x + slider_size.x - 2, slider_pos.y - 4 }, { 4, slider_size.y + 5 } ), gui_color::palette::main_pink() );

	/*
	* Reset position and adjust for controls after
	*/
	m_pos = pos;

	if ( !m_manual_padding )
		pos.y += m_size.y + g_menu->get_style( e_menu_style::control_padding ).y;
}

