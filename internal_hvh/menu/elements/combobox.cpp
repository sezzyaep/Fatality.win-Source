#include <utility>
#include "../menu_include.h"
#include <ren/adapter.h>

using namespace evo::ren;

combobox::combobox( const std::string& name, value* var, const combobox_style::flags flags, std::vector<combo_item_t> items ) : m_open( false ), m_cached_text( false ),
m_scroll_amount(), m_flags( flags ), m_items( std::move( items ) )
{
	m_name = name;
	m_var = var;
}

void combobox::run_render( vec2& pos )
{
	if ( !should_render() )
		return;

	m_pos = pos;

	handle_hover_progress();

	/*
	 * If this is just aa popup dont do any
	 * of the other stuff
	 */
	if ( m_flags & combobox_style::flags::pop_up )
	{
		/*
		 * It's open all the time
		 */
		m_open = true;

		/*
		 * For input handling
		 */
		m_popup_pos = pos;
		m_popup_size = m_size;

		render_popup( pos, m_size );

		return;
	}

	auto& d = draw.get_layer( layer::foreground );

	/*
	 * Name of the combobox
	 */
	if ( !m_name.empty() )
	{
		_u( name, m_name );
		d.add_text( draw.get_font( FNV1A( "control" ) ), m_pos + vec2{ 0, 5 }, name, gui_color::text() );

		/*
	 * Start drawing the actual combobox below the text
	 */
		m_pos.y += m_string_size.y;
	}

	/*
	 * Inner
	 */
	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 1, m_pos.y + 1 }, { m_size.x - 1, m_size.y * .5f - 1 } ), gui_color::control_top(), gui_color::control_bot() );

	/*
	 * Border
	 */
	d.add_rect( rect_legacy( { m_pos.x, m_pos.y }, { m_size.x, m_size.y * .5f + 1 } ), gui_color::palette::border() );
	d.add_rect( rect_legacy( { m_pos.x, m_pos.y }, { m_size.x, m_size.y * .5f + 1 } ), m_open ? gui_color::palette::main_pink() : gui_color::palette::main_pink( m_hover_progress * 255 ) );

	d.push_clip( rect_legacy( m_pos, { m_size.x - 18, m_size.y } ) );

	/*
	 * Current item
	 */
	const auto display_text = get_display_name();
	d.add_text( draw.get_font( FNV1A( "control" ) ), { m_pos.x + g_menu->get_style( e_menu_style::popup_padding ).x, floorf( m_pos.y + m_size.y * .25f + 3 ) }, display_text, gui_color::text(), 0, layer::align_center );

	d.pop_clip();

	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + m_size.x - g_menu->get_style( e_menu_style::control_padding ).x * 2.5f - 24, m_pos.y + 1 }, { g_menu->get_style( e_menu_style::control_padding ).x * 5.f - 9, m_size.y * .5f - 2 } ), gui_color::control_top( 0 ), gui_color::control_top( 255 ), true );
	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + m_size.x - 18, m_pos.y + 2 }, { 17, m_size.y * .5f - 3 } ), gui_color::palette::main_pink( 255 ), gui_color::palette::main_pink( 150 ) );

	/*
	 * Selection stripes
	 */
	auto stripe_pos = vec2( m_pos.x + m_size.x - g_menu->get_style( e_menu_style::control_padding ).x * 2.5f, m_pos.y + m_size.y * .25f - 3 );
	for ( auto i = 0; i < 4; i++ )
	{
		d.add_line( stripe_pos + vec2{ 1, 0 }, stripe_pos + vec2( g_menu->get_style( e_menu_style::control_padding ).x * 1.5f + 1, 0.f ), gui_color::palette::border() );
		d.add_line( stripe_pos + vec2{ 1, 0 }, stripe_pos + vec2( g_menu->get_style( e_menu_style::control_padding ).x * 1.5f + 1, 0.f ), m_open ? gui_color::text() : gui_color::text( m_hover_progress * 255 ) );

		stripe_pos.y += 2;
	}

	/*
	 * Just in case it gets opened
	 */
	const auto max_shown_items = std::min( m_items.size(), 20u );
	m_popup_pos = vec2( m_pos.x, pos.y + m_size.y + g_menu->get_style( e_menu_style::popup_padding ).y * 2 );
	m_popup_size = vec2( m_size.x, g_menu->get_style( e_menu_style::popup_padding ).y * 2 + max_shown_items * m_string_size.y );
	if ( m_name.empty() )
		m_popup_pos.y -= m_size.y * 0.5f;


	/*
	 * ghetto width for model changer
	 */
	if ( m_items.size() > 40 )
	{
		m_popup_size.x += floorf( m_string_size.x * 4.f );
		m_popup_pos.x = std::max( g_menu->get_style( e_menu_style::popup_padding ).y, m_popup_pos.x - ( m_string_size.x * 3.52f + g_menu->get_style( e_menu_style::control_padding ).x * 3.f + 2.f ) );
	}
	/*
	 * ghetto width for skybox changer
	 */
	if ( m_var == vars::visuals.skybox )
	{
		m_popup_size.x += floorf( m_string_size.x * 2.f );
	}

		/*
	 * account for lengh exceeding screen size
	 */
	const auto diff = m_popup_pos.y + m_popup_size.y + g_menu->get_style( e_menu_style::popup_padding ).y - adapter->display.y;
	if ( diff > 0 )
		m_popup_pos.y -= diff;

	/*
	 * Render popup only if active
	 */
	if ( m_open )
		render_popup( m_popup_pos, m_popup_size );

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

void combobox::run_input()
{
	init_subs();

	calc_string_size();

	/*
	 * This is usually NOT a nullptr, only occurs when having a combobox as a popup
	 */
	if ( m_parent )
		m_size = { m_parent->get_size().x / 2, m_string_size.y * 2 };

	if ( !should_render() )
		return;

	if ( m_focussed_control )
	{
		m_focussed_control->run_input();
		return;
	}

	m_hovered = is_hovered( m_pos, m_size );


	popup_input();
	/*
	 * No sub controls registered -> no work to do
	 */
	if ( !m_controls.empty() )
	{
		/*
		* Handle input for all sub controls registered
		* If there's on in focus - only handle that one
		*/
		for ( const auto& sub_control : m_controls )
		{
			sub_control->run_input();

			/*
			 * If a control has been focussed by now, break
			 */
			if ( m_focussed_control )
				break;
		}
	}
}

void combobox::popup_input()
{

	/*
	 * Combobox is not open yet
	 */
	if ( !m_open && !( m_flags & combobox_style::flags::pop_up ) )
	{
		/*
		 * Toggle popup
		 */
		if ( m_hovered && g_input->get_mouse().state == e_mouse_state::pressed )
		{
			m_open = true;

			g_menu->set_focussed_control( this );
		}

		return;
	}

	/*
	 * Combobox is open
	 */

	const auto mouse_in_bounds = g_input->mouse_in_bounds( m_popup_pos, m_popup_size );

	if ( mouse_in_bounds )
		handle_scroll();

	/*
	 * If mouse not pressed nothing to check
	 */
	if ( g_input->get_mouse().state != e_mouse_state::pressed )
		return;

	/*
	 * Mouse default pressed somewhere else, close it
	 */
	if ( !mouse_in_bounds && !( m_flags & combobox_style::flags::pop_up ) )
	{
		m_open = false;

		g_menu->set_focussed_control( nullptr );
		g_input->reset_mouse();

		return;
	}

	/*
	 * Mouse default pressed in combobox
	 */
	if ( mouse_in_bounds )
	{
		/*
		 * Now do the same as when rendering just without the rendering
		 */
		auto base_position = vec2( m_popup_pos.x + g_menu->get_style( e_menu_style::popup_padding ).x, m_popup_pos.y - 2 + g_menu->get_style( e_menu_style::popup_padding ).y );
		auto counter = 0;
		auto rendered_items = 0;
		for ( const auto& item : m_items )
		{
			if ( counter++ < m_scroll_amount )
				continue;

			if ( ++rendered_items > 20u )
				break;

			const auto was_selected = g_input->mouse_in_bounds( base_position, { m_popup_size.x, m_string_size.y } );

			base_position.y += m_string_size.y;

			if ( !was_selected )
				continue;

			/*
			 * Mutliselect items just need a toggle
			 */
			if ( m_flags & combobox_style::flags::multi_select )
				item.m_value->set( !item.m_value->get<bool>() );
			else
				m_var->set( item.m_index );
		}

		/*
		 * If a new item default selected and its not a multi select
		 * box close the pop up
		 */
		if ( m_flags & combobox_style::flags::multi_select || m_flags & combobox_style::flags::pop_up )
			return;

		m_open = false;
		g_input->reset_mouse();
		g_menu->set_focussed_control( nullptr );
	}
}

combobox* combobox::add_item( const std::string& name, value* var )
{
	if ( m_flags & combobox_style::flags::multi_select )
		m_items.emplace_back( _w2( name ), var );
	else
		m_items.emplace_back( _w2( name ), m_items.size() );

	return this;
}

std::string combobox::get_display_name()
{
	/*
	 * Only one item can be selected
	 */
	if ( !( m_flags & combobox_style::flags::multi_select ) )
	{
		for ( const auto& item : m_items )
		{
			if ( m_var->get<int>() != item.m_index )
				continue;

			_u( name, item.m_name );
			m_display_text = name;
			break;
		}

		return m_display_text;
	}

	//const auto max_size = m_size.x - g_menu->get_style( e_menu_style::control_padding ).x * 3.f - g_menu->get_style( e_menu_style::popup_padding ).x - render::get_text_size( "...", fonts::control ).x;

	auto ret = std::string( "" );
	for ( const auto& item : m_items )
	{
		/*
		 * Item not selected anyway
		 */
		if ( !item.m_value->get<bool>() )
			continue;

		_u( name, item.m_name );

		/*
		 * If it's the first item being added,
		 * dont add extra comma
		 */
		ret += ret.empty() ? std::string( name ) : ", " + std::string( name );
	}

	/*
	 * If no item default active, set the text to "-"
	 */
	if ( m_items.empty() || ret.empty() )
		ret = "-";

	m_display_text = ret;

	m_cached_text = true;
	return m_display_text;
}

void combobox::render_popup( const vec2& pos, const vec2& size )
{
	auto& d = draw.get_layer( layer::foreground );
	/*
	* Limit viewport so we stay in popup area
	*/
	d.push_clip( rect_legacy( pos, { size.x + 1, size.y + 1 } ), false );
	/*
	* Inner
	*/
	d.add_rect_filled_linear_gradient( rect_legacy( pos, size ), gui_color::control_bot( 255 ), gui_color::control_bot( 230 ) );

	/*
	* All items
	*/
	auto base_position = vec2( pos.x + g_menu->get_style( e_menu_style::popup_padding ).x, pos.y - 2 + g_menu->get_style( e_menu_style::popup_padding ).y );
	auto counter = 0;
	auto rendered_items = 0;
	for ( const auto& item : m_items )
	{
		if ( counter++ < m_scroll_amount )
			continue;

		if ( ++rendered_items > 20u )
			break;

		auto col = color();
		const auto is_hovered = g_input->mouse_in_bounds( base_position, { size.x, m_string_size.y } );

		/*
		* If we have multiselect, multiple items can be selected
		*/
		if ( m_flags & combobox_style::flags::multi_select )
			col = item.m_value->get<bool>() ? gui_color::palette::main_pink() : is_hovered ? gui_color::text_active() : gui_color::text();
		else
			col = m_var->get<int>() == item.m_index ? gui_color::palette::main_pink() : is_hovered ? gui_color::text_active() : gui_color::text();

		_u( name, item.m_name );
		d.add_text( draw.get_font( FNV1A( "control" ) ), { base_position.x, base_position.y + m_string_size.y / 2 + 4 }, name, col, 0, layer::align_center );

		base_position.y += m_string_size.y;
	}

	render_scroll();

	d.add_rect( rect_legacy( pos, size ), gui_color::palette::border( 255 ) );

	d.pop_clip();
}

void combobox::render_scroll() const
{
	/*
	 * Check if we need to render scroll arrows
	 */
	if ( m_items.size() <= 20u )
		return;

	auto& d = draw.get_layer( layer::foreground );

	const auto arrow = [&] ( const Vector2D& base, const bool reverse, const bool active = false ) -> void
	{
		const auto col = gui_color::palette::border( 255 );

		if ( reverse )
		{
			d.add_rect_filled( rect_legacy( { base.x, base.y }, { 1, 1 } ), col );
			d.add_rect_filled( rect_legacy( { base.x - 1, base.y - 1 }, { 3, 1 } ), col );
			d.add_rect_filled( rect_legacy( { base.x - 2, base.y - 2 }, { 5, 1 } ), col );
			d.add_rect_filled( rect_legacy( { base.x - 3, base.y - 3 }, { 7, 1 } ), col );
		}
		else
		{
			d.add_rect_filled( rect_legacy( { base.x, base.y }, { 1, 1 } ), col );
			d.add_rect_filled( rect_legacy( { base.x - 1, base.y + 1 }, { 3, 1 } ), col );
			d.add_rect_filled( rect_legacy( { base.x - 2, base.y + 2 }, { 5, 1 } ), col );
			d.add_rect_filled( rect_legacy( { base.x - 3, base.y + 3 }, { 7, 1 } ), col );
		}
	};

	const auto show_upper_arrow = m_scroll_amount > 0;
	const auto show_lower_arrow = m_scroll_amount < m_items.size() - 30u;

	if ( show_upper_arrow )
	{
		d.add_rect_filled_linear_gradient( rect_legacy( { m_popup_pos.x + 1, m_popup_pos.y + 1 }, { m_popup_size.x - 1, g_menu->get_style( e_menu_style::child_padding ).y + 4 } ), gui_color::child_sub( 255 ), gui_color::child_sub( 0 ) );
		const auto base_position = Vector2D( m_popup_pos.x + m_popup_size.x - g_menu->get_style( e_menu_style::child_padding ).x, m_popup_pos.y + g_menu->get_style( e_menu_style::child_padding ).y );
		arrow( base_position, false );
	}

	if ( show_lower_arrow )
	{
		d.add_rect_filled_linear_gradient( rect_legacy( { m_popup_pos.x + 1, m_popup_pos.y + m_popup_size.y - g_menu->get_style( e_menu_style::child_padding ).y - 3 }, { m_popup_size.x - 1, g_menu->get_style( e_menu_style::child_padding ).y + 4 } ), gui_color::child_sub( 0 ), gui_color::child_sub( 255 ) );
		const auto base_position = Vector2D( m_popup_pos.x + m_popup_size.x - g_menu->get_style( e_menu_style::child_padding ).x, m_popup_pos.y + m_popup_size.y - g_menu->get_style( e_menu_style::child_padding ).y );
		arrow( base_position, true );
	}
}

void combobox::handle_scroll()
{
	/*
	 * Handle scrollbar max once per frame
	 */
	static auto last_framecount = interfaces::globals()->framecount;
	if ( last_framecount == interfaces::globals()->framecount )
		return;

	/*
	 * Check if we need a scrollbar
	 */
	if ( m_items.size() <= 20u )
	{
		m_scroll_amount = 0;
		return;
	}

	///*
	// * No scroll input received
	// */
	//if ( g_input->get_mouse().scroll == 0 )
	//	return;

	last_framecount = interfaces::globals()->framecount;

	/*
	 * Scroll down
	 */
	if ( g_input->get_mouse().scroll < 0 )
		m_scroll_amount++;

	if ( g_input->get_key( VK_DOWN ).pressed || g_input->get_key( VK_DOWN ).held )
		m_scroll_amount++;
	if ( g_input->get_key( VK_UP ).pressed || g_input->get_key( VK_UP ).held )
		m_scroll_amount--;
	/*
	 * Scroll up
	 */
	else if ( g_input->get_mouse().scroll > 0 )
		m_scroll_amount--;

	m_scroll_amount = clamp( m_scroll_amount, 0, static_cast< int >( m_items.size() - 30u ) );
}
