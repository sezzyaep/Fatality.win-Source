#include "../menu_include.h"

using namespace evo::ren;

weapon_tab::weapon_tab( const std::string name, std::vector<std::shared_ptr<child>> childs ) : sub_tab( name, childs )
{

}

void weapon_tab::run_render( vec2& pos )
{
	m_pos = pos;
	/*
	* If this tab is selected in parent
	* tab toggle state
	*/
	m_active = m_parent ? m_parent->get_active_sub_tab() == m_id : m_parent_sub_tab->get_active_sub_tab() == m_id;

	/*
	* Handle the line size under each tab &
	* the hover progress
	*/
	handle_line();
	handle_hover();

	/*
	 * Calculate the line size below now to adjust
	 * the text for it
	 */
	const auto line_size = ( m_size.y / 10 ) * m_line_size;

	auto& d = draw.get_layer( layer::foreground );
	/*
	 * Draw base text
	 */
	_u( name, m_name );

	dx_adapter::weapon( d, { m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2 - line_size }, 20, m_active ? gui_color::tab_active() : gui_color::tab( 255 * ( 1.f - m_hover_progress ) ), name.c_str(), layer::align_center, layer::align_center );

	/*
	 * If the subtab is in hovering progress and
	 * not active fade the text to pink and draw
	 * a transparent pink rectangle over the text
	 */
	if ( m_hover_progress != 0.f )
	{
		/*
		 * Pink text will only be visible when the tab
		 * is not active
		 */
		if ( !m_active )
			dx_adapter::weapon( d, { m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2 - line_size }, 20, gui_color::palette::hot_pink( 255 * m_hover_progress ), name.c_str(), layer::align_center, layer::align_center );

		/*
		 * Pink overlay
		 */
		d.add_rect_filled( rect_legacy( m_pos, m_size ), gui_color::palette::hot_pink( 15 * m_hover_progress ) );
	}

	/*
	 * If the tab is or default active draw a small
	 * pink line under it
	 */
	if ( m_line_size != 0.f )
	{
		d.add_rect_filled( rect_legacy( { m_pos.x, m_pos.y + m_size.y - line_size }, { m_size.x, line_size } ), gui_color::palette::hot_pink() );
	}

	pos.x += m_size.x;

	/*
	* Reser previously established limit to be able to
	* render controls in their area
	*/
	d.pop_clip();

	/*
	* Tab is not active, so no need
	* to render the childs
	*/
	if ( !m_active )
		return;

	/*
	* Tab is active so we can render all childs stored
	* in it
	*/
	if ( !m_sub_tabs.empty() )
	{
		auto start_pos = vec2( m_parent->get_parent()->get_control_area_start() ) + vec2( 0, g_menu->get_style( e_menu_style::tab_padding ).y );

		for ( const auto& sub_tab : m_sub_tabs )
		{
			d.push_clip( rect_legacy( { m_parent->get_parent()->get_control_area_start() + vec2( 0, g_menu->get_style( e_menu_style::tab_padding ).y ) }, { m_parent->get_parent()->get_control_area_size().x - 1, g_menu->get_style( e_menu_style::tab_padding ).y - 1 } ) );
			sub_tab->run_render( start_pos );
		}

	}

	for ( const auto& child : m_childs )
		child->run_render();

}

void weapon_tab::run_input()
{
	static auto padding = g_menu->get_style( e_menu_style::window_padding ) + vec2( -2.22, 0 );

	_u( name, m_name );
	if ( m_string_size.is_zero() )
		m_string_size = { dx_adapter::get_weapon_width( 20, name.c_str() ), 20 };

	m_size = { padding.x * 2 + m_string_size.x, padding.y * 2 };

	/*
	* Grab mouse info for checking whether
	* the tab is hovered or clicked
	*/
	const auto info = g_input->get_mouse();

	/*
	* Check if mouse is in the bounds
	*/
	m_hovered = !g_menu->get_focussed_control() && info.pos.x > m_pos.x && info.pos.x < m_pos.x + m_size.x &&
		info.pos.y > m_pos.y && info.pos.y < m_pos.y + m_size.y;

	/*
	* If the mouse is in bound and LMB is clicked
	* set this tab to active one
	*/
	if ( m_hovered && info.state == e_mouse_state::pressed )
	{
		m_hovered = false;

		m_parent ? m_parent->set_active_sub_tab( m_id ) : m_parent_sub_tab->set_active_sub_tab( m_id );
	}

	/*
	* Dont handle childs if the tab isnt active
	*/
	if ( m_parent ? m_parent->get_active_sub_tab() != m_id : m_parent_sub_tab->get_active_sub_tab() != m_id )
		return;

	/*
	* Go through all childs registered on this tab
	* and handle the input.
	*/
	for ( const auto& sub_tab : m_sub_tabs )
		sub_tab->run_input();
	for ( const auto& child : m_childs )
		child->run_input();
}