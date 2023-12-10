#include "../menu_include.h"

using namespace evo::ren;

tab::tab( const std::string name, std::vector<std::shared_ptr<sub_tab>> sub_tabs, bool noarea ) : m_noarea( noarea ), m_string_size( vec2( 0, 0 ) )
{
	m_name = name;
	m_subtabs = sub_tabs;
	m_line_size = 0.f;
	m_active = false;
	auto id = 0;

	for ( const auto& sub_tab : m_subtabs )
		sub_tab->init( std::shared_ptr<tab>( this ), id++ );
}

void tab::run_render( Vector2D& pos )
{
	if ( m_last_active != m_active_sub_tab )
	{
		for ( const auto& sub_tab : m_subtabs )
			sub_tab->reset_state();

		g_menu->set_focussed_control( nullptr );

		m_last_active = m_active_sub_tab;
	}

	/*
	 * The pos parameter is centered
	 * so we have to un-center it
	 */
	m_pos = { pos.x, pos.y - m_size.y / 2 };

	/*
	 * If this tab is selected in parent
	 * window toggle state
	 */
	m_active = m_parent->get_active_tab() == m_id;

	/*
	 * Handle the line size under each tab &
	 * the hover progress
	 */
	handle_line();
	handle_hover();

	/*
	* If the tab is in hovering progress
	* add glitch effect
	*/
	_u( name, m_name );
	if ( m_hovered )
		glitch_text::run_render( name, m_pos + vec2{ 0, 5 }, FNV1A( "tab_font" ) );


	auto& d = draw.get_layer( layer::foreground );
	/*
	 * Draw the base text
	 */
	d.add_text( draw.get_font( FNV1A( "tab_font" ) ), m_pos + vec2{ 0, 5 }, name, gui_color::tab() );

	/*
	 * Draw the hovered text or active
	 */
	if ( m_active )
		d.add_text( draw.get_font( FNV1A( "tab_font" ) ), m_pos + vec2{ 0, 5 }, name, gui_color::tab_active() );
	else
		d.add_text( draw.get_font( FNV1A( "tab_font" ) ), m_pos + vec2{ 0, 5 }, name, gui_color::tab_hovered( 255 * m_hover_progress ) );

	/*
	* Draw the line under the tab text
	*/
	const auto size_without_padding = m_size.x - g_menu->get_style( e_menu_style::tab_padding ).x;
	const auto size = size_without_padding * m_line_size;
	/*
	 * Start the line in the middle and
	 * then slowly go to left
	 */
	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + size_without_padding / 2 - size / 2 - 1, m_pos.y + m_size.y },
		{ size, 2 } ), gui_color::palette::hot_pink(), gui_color::palette::hot_pink( 150 ) );

/*
 * Adjust the size so the next
 * tab starts at the right position
 */
	pos.x += m_size.x;

	/*
	 * Tab is not active, so no need
	 * to render the childs
	 */
	if ( !m_active )
		return;

	/*
	* Tab is active so we can render all subtabs stored
	* in it
	*/
	const auto header_size = g_menu->get_style( e_menu_style::window_padding );

	d.push_clip( rect_legacy( { m_pos.x - 50, m_pos.y + header_size.y + 8 }, { m_size.x + 100, header_size.y + g_menu->get_style( e_menu_style::tab_area_size ).y } ) );
	d.add_rect_filled_radial_gradient( rect_legacy( { m_pos.x + size_without_padding / 2 - size / 2 - 1, m_pos.y + 7 }, { size, 14 } ), gui_color::palette::main_pink( 0 ), gui_color::palette::main_pink( 200 * m_line_size ) );
	d.pop_clip();

	auto subtab_pos = vec2( m_parent->get_control_area_start() ) - vec2( 0, g_menu->get_style( e_menu_style::window_padding ).y * ( m_subtabs.size() == 1 ? 4 : 2 ) );
	if ( m_noarea )
	{
		subtab_pos = vec2( m_parent->get_control_area_start() ) + vec2( 16, -g_menu->get_style( e_menu_style::window_padding ).y * ( m_subtabs.size() == 1 ? 4 : 2 ) + 8 );
	}

	for ( const auto& sub_tab : m_subtabs )
	{
		/*
		* Limit subtab region
		*/
		if ( m_noarea )
		{
			d.push_clip( rect_legacy( m_parent->get_control_area_start() + vec2( 17, -( header_size.y * 2 ) + 9 ), { m_parent->get_control_area_size().x - 2, g_menu->get_style( e_menu_style::tab_padding ).y * 4 - 1 } ) );
		}
		else
			d.push_clip( rect_legacy( m_parent->get_control_area_start() + vec2( 1, -( header_size.y * 2 ) + 1 ), { m_parent->get_control_area_size().x - 1, header_size.y * 2 - 1 } ) );

		sub_tab->run_render( subtab_pos );
	}
}

void tab::run_input()
{
	/*
	* Calculate size based on string size and
	* add a padding so secure a safe padding
	* between each padding
	*/
	const auto padding = g_menu->get_style( e_menu_style::tab_padding );

	_u( name, m_name );
	if ( m_string_size.is_zero() )
		m_string_size = evo::ren::text_size( evo::ren::draw.get_font( FNV1A( "tab_font" ) ), name ) + 3.f;

	m_size = { m_string_size.x + padding.x, m_string_size.y };

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
	if ( m_hovered && info.state == e_mouse_state::pressed && !g_menu->get_focussed_control() )
	{
		m_hovered = false;

		m_parent->set_active_tab( m_id );
	}

	/*
	* Dont handle childs if the tab isnt active
	*/
	if ( m_parent->get_active_tab() != m_id )
		return;

	/*
	* Go through all subtabs registered on this tab
	* and handle the input.
	*/
	for ( const auto& sub_tab : m_subtabs )
		sub_tab->run_input();
}

void tab::reset_state()
{
	for ( auto& subtab : m_subtabs )
		subtab->reset_state();
}


void tab::run_hotkey_input()
{
	for ( const auto& sub_tab : m_subtabs )
		sub_tab->run_hotkey_input();
}

std::shared_ptr<window> tab::get_parent() const
{
	return m_parent;
}

void tab::init( const std::shared_ptr<window> window, const int id )
{
	m_parent = window;

	/*
	* Basically number of tabs created. Used for checking
	* whether a tab is selected and if the controls should
	* be rendered
	*/
	m_id = id;
}

sub_tab* tab::add_subtab( const char* name )
{
	auto sub = std::make_shared<sub_tab>( name );
	sub->init( shared_from_this(), m_subtabs.size() );
	m_subtabs.push_back( sub );

	return sub.get();
}

bool tab::has_sub_tabs() const
{
	/*
	 * If there is more than 1 subtab registered,
	 * draw a selection
	 */
	return m_subtabs.size() > 1;
}

int tab::get_active_sub_tab() const
{
	return m_active_sub_tab;
}

void tab::set_active_sub_tab( const int id )
{
	m_active_sub_tab = id;
}

std::vector<std::shared_ptr<sub_tab>>& tab::get_subtabs()
{
	return m_subtabs;
}

void tab::handle_line()
{
	VAL_SMOOTHER( m_line_size, 0.f, 1.f, m_active, 1.f / 0.2f );
}

void tab::handle_hover()
{
	VAL_SMOOTHER( m_hover_progress, 0.f, 1.f, m_hovered, 1.f / 0.2f );
}
