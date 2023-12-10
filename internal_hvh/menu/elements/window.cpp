#include "../menu_include.h"
#include "ren/adapter.h"

using namespace evo::ren;

window::window( const std::string name, const vec2 size, const vec2 pos, const window_style::flags flags, std::vector<std::shared_ptr<tab>> tabs )
{
	m_name = name;
	m_pos = pos;
	m_size = size;
	m_tabs = tabs;
	m_flags = flags;
	m_active_tab = 0;

	auto id = 0;

	for ( const auto& tab : m_tabs )
		tab->init( std::shared_ptr<window>( this ), id++ );
}

void window::reset_state() const
{
	for ( const auto& tab : m_tabs )
		tab->reset_state();

	if ( config_child )
		config_child->reset_state();

	g_menu->set_focussed_control( nullptr );
}

void window::glow() const
{
	auto& d = draw.get_layer( layer::foreground );

	d.add_rect_filled_radial_gradient( rect_legacy( { m_pos.x - 25, m_pos.y - 25 }, { m_size.x + 50, m_size.y + 50 } ), gui_color::palette::main_pink( 0 ), gui_color::palette::main_pink( 150 ) );
}

void window::run_render()
{
	inventorychanger::update_selected();
	auto search_term = std::string{};
	auto search = inventorychanger::paint_kit_listbox.lock()->get_search();
	if ( !search.empty() )
		search_term = std::string( search.begin(), search.end() );
	inventorychanger::fill_skin_list( search_term );

	//glow();
	/*
	* Border
	*/
	border();

	auto& d = draw.get_layer( layer::foreground );

	/*
	* Base background background texture
	*/
	d.add_rect_filled( rect_legacy( m_pos, m_size ), gui_color::custom( Color( 17, 15, 35 ) ) );

	/*
	 * Draw our header containing the name of the window
	 * and the close button
	 */
	header();

	/*
	 * Store padding and sizes for proper
	 * positioning
	 */
	static auto header_size = g_menu->get_style( e_menu_style::window_padding );
	static auto tab_size = g_menu->get_style( e_menu_style::tab_area_size );

	/*
	 * Check whether we need to render a
	 * sub tab area
	 */
	const auto has_sub_tabs = m_tabs.at( m_active_tab )->has_sub_tabs() && !m_tabs.at( m_active_tab )->m_noarea;

	/*
	* Draw the area where the childs will be drawn
	*/
	auto start = get_control_area_start();
	auto size = get_control_area_size();

	/*
	* if the tab has subtabs, draw an area
	* to make space and else increase the
	* total area to draw in
	*/
	if ( has_sub_tabs )
	{
		d.add_rect_filled_linear_gradient( rect_legacy( start - vec2( 0, header_size.y * 2 ), { size.x, header_size.y * 2 } ), gui_color::child_sub( 240 ), gui_color::child_sub( 210 ) );
		d.add_rect( rect_legacy( start - vec2( 0, header_size.y * 2 ), { size.x, header_size.y * 2 } ), gui_color::palette::border() );
	}
	/*
	 * No subtabs registered... Increase space
	 */
	else
	{
		start.y -= header_size.y * 2;
		size.y += header_size.y * 2;
	}

	d.add_rect_filled_linear_gradient( rect_legacy( start, size ), gui_color::child_sub( 130 ), gui_color::child_sub( 100 ) );
	d.add_rect( rect_legacy( start, size ), gui_color::palette::border() );

	/*
	 * Draw our tabs and adjust the position to
	 * ensure the right position for every tab
	 */
	auto pos = Vector2D( m_pos.x + m_size.x / 5, m_pos.y + header_size.y + tab_size.y / 2 );

	config_child->run_render();

	if ( m_old_active_tab != m_active_tab )
	{
		reset_state();

		m_old_active_tab = m_active_tab;
	}

	for ( const auto& tab : m_tabs )
		tab->run_render( pos );
}

void window::border() const
{
	auto& d = draw.get_layer( layer::foreground );

	if ( m_flags & window_style::flags::rainbow_border )
	{
		d.add_rect_filled_multicolor( rect_legacy( { m_pos.x - 2, m_pos.y - 2 }, { m_size.x + 4, m_size.y + 4 } ), { gui_color::palette::main_pink( 200 ), gui_color::palette::main_purple( 200 ), gui_color::palette::main_pink( 200 ), gui_color::palette::main_purple( 200 ) } );
	}

	d.add_rect( rect_legacy( { m_pos.x - 1, m_pos.y - 1 }, { m_size.x + 2, m_size.y + 2 } ), gui_color::palette::border() );

	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x - 4, m_pos.y - 4 }, { m_size.x * 0.6f, 3 } ), gui_color::palette::hot_pink( 255 ), gui_color::palette::hot_pink( 0 ), true );
	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x - 4, m_pos.y - 1 }, { 3, m_size.y * 0.6f } ), gui_color::palette::hot_pink( 255 ), gui_color::palette::hot_pink( 0 ), false );


	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + m_size.x + 4, m_pos.y + m_size.y + 4 }, { -m_size.x * 0.6f, -3 } ), gui_color::palette::hot_pink( 255 ), gui_color::palette::hot_pink( 0 ), true );
	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + m_size.x + 4, m_pos.y + m_size.y + 1 }, { -3, -m_size.y * 0.6f } ), gui_color::palette::hot_pink( 255 ), gui_color::palette::hot_pink( 0 ), false );
}

void window::header() const
{
	auto& d = draw.get_layer( layer::foreground );

	/*
	 * Draggable area with window name in it
	 */
	static auto header_size = g_menu->get_style( e_menu_style::window_padding ).y;

	d.add_rect_filled( rect_legacy( m_pos, { m_size.x, header_size } ), gui_color::palette::dark_blue() );

	//render::text( { m_pos.x + 6, m_pos.y + header_size / 2 }, m_name, gui_color::text(), fonts::title_font, c_font::font_flags::centered_y );

	/*
	* No need for a tab space of we don't have
	* more than 1 tab
	*/
	if ( m_tabs.size() > 1 )
	{
		static auto size = g_menu->get_style( e_menu_style::tab_area_size );
		static auto padding = g_menu->get_style( e_menu_style::window_padding );

		d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x, m_pos.y + padding.y + 1 }, { m_size.x, size.y } ), gui_color::custom( Color( 45, 32, 88, 160 ) ), gui_color::custom( Color( 33, 27, 70, 140 ) ) );
		d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x, m_pos.y + padding.y + 1 + size.y + 1 }, { m_size.x, 5 } ), gui_color::black( 140 ), gui_color::black( 0 ) );

		_u( name, m_name );

		glitch_text::run_render( name, { m_pos.x + padding.x, m_pos.y + padding.y * 2 - 2 }, FNV1A( "glitch_font" ) );
		/*
		 * White text
		 */
		d.add_text( draw.get_font( FNV1A( "glitch_font" ) ), { m_pos.x + padding.x, m_pos.y + padding.y * 2 - 2 }, name, gui_color::text_active() );
	}

	//render::line( { m_pos.x, m_pos.y + header_size }, { m_pos.x + m_size.x, m_pos.y + header_size }, gui_color::black( 255 ) );
	//render::rect_filled_linear_gradient( { m_pos.x, m_pos.y + header_size }, { m_size.x, 3 }, gui_color::black( 140 ), gui_color::black( 0 ) );

	///*
	// * Rainbow bar which will be drawn
	// * as the seperator between header
	// * and the tab area. The bar pulsates
	// * in brightness
	// */

	//static auto factor = 0.f;

	//static auto should_add = false;

	//VAL_SMOOTHER( factor, .4f, 1.f, should_add, .6f / 1.75f );

	///*
	// * Only half the alpha at max
	// * so it can still be visible
	// */
	//if ( factor <= .4f || factor >= 1.f )
	//	should_add = !should_add;

	////render::rect_filled_linear_gradient( { m_pos.x, m_pos.y + header_size }, { m_size.x, 2 }, gui_color::palette::main_purple( 255 * factor ), gui_color::palette::hot_pink( 255 * -factor ), true );
}

void window::run_input()
{
	static auto is_getting_dragged = false;

	const auto mouse_dragging = g_input->get_mouse().state == e_mouse_state::held;
	const auto mouse_pos = g_input->get_mouse().pos;

	/*
	* Check whether the mouse is dragging in the upper
	* window bounds and react accordingly.
	* Also check if there's no control focussed
	*/
	if ( !g_menu->get_focussed_control() && !is_getting_dragged && mouse_dragging &&
		( mouse_pos.x > m_pos.x && mouse_pos.y > m_pos.y ) &&
		( mouse_pos.x < m_pos.x + m_size.x && mouse_pos.y < m_pos.y + 18 ) )
	{
		m_drag_offset.x = m_pos.x - mouse_pos.x;
		m_drag_offset.y = m_pos.y - mouse_pos.y;
		is_getting_dragged = true;
	}

	if ( !mouse_dragging )
		is_getting_dragged = false;


	const auto dimensions = adapter->display;
	const auto small_res = dimensions.x - m_size.x < 0 || dimensions.y - m_size.y < 0;

	m_pos.x = clamp( m_pos.x, 0.f, small_res ? 0.f : dimensions.x - m_size.x );
	m_pos.y = clamp( m_pos.y, 0.f, small_res ? 0.f : dimensions.y - m_size.y );

	/*
	 * If the window isn't being dragged we
	 * can handle all tabs in this window
	 */
	if ( !is_getting_dragged )
	{
		/*
		* Handle input for every tab even if not selected
		* so we can still see whether it's hovered or w.e
		*/
		for ( const auto& tab : m_tabs )
			tab->run_input();

		config_child->run_input();

		return;
	}

	m_pos = mouse_pos + m_drag_offset;

	m_pos.x = clamp( m_pos.x, 0.f, small_res ? 0.f : dimensions.x - m_size.x );
	m_pos.y = clamp( m_pos.y, 0.f, small_res ? 0.f : dimensions.y - m_size.y );

	m_drag_offset = m_pos - mouse_pos;

}

void window::run_hotkey_input()
{
	for ( const auto& tab : m_tabs )
		tab->run_hotkey_input();
}


vec2 window::get_size() const
{
	return m_size;
}

int window::get_tab_amount() const
{
	return m_tabs.size();
}

vec2 window::get_pos() const
{
	return m_pos;
}

void window::set_active_tab( const int id )
{
	m_active_tab = id;
}

int window::get_active_tab() const
{
	return m_active_tab;
}

vec2 window::get_control_area_start() const
{
	static auto header_size = g_menu->get_style( e_menu_style::window_padding );
	static auto tab_size = g_menu->get_style( e_menu_style::tab_area_size );

	return { m_pos.x + header_size.x * 1.f, m_pos.y + header_size.y * 4 + tab_size.y };
}

vec2 window::get_control_area_size() const
{
	static auto header_size = g_menu->get_style( e_menu_style::window_padding );
	static auto tab_size = g_menu->get_style( e_menu_style::tab_area_size );

	return { m_size.x - header_size.x * 2.f, m_size.y - header_size.y * 5 - tab_size.y };
}

std::vector<std::shared_ptr<tab>>& window::get_tabs()
{
	return m_tabs;
}
