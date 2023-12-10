#include "../menu_include.h"

using namespace evo::ren;

listbox::listbox( value* var, std::vector<std::string>* items, const int items_to_display, const uint8_t flags ) :
	m_scroll_amount( 0 ),
	m_cached_items( false ), m_items( items ), m_items_alt( {} ), m_display_items_alt( {} ), m_last_itemsize( 0 ), m_old_search( 0 ), m_flags( flags )
{
	m_name = "";
	m_var = var;
	m_direct = true;
	m_display_items = &m_display_items_alt;

	/*
	 * If we have a search bar add more space and init the searchbar
	 */
	m_items_to_display = items_to_display;
	m_search_bar = std::make_shared<text_box>( "A", m_search, textbox_style::flags::search_bar );
	m_search_bar->run_input();
}

listbox::listbox( const std::string& name, value* var, std::vector<std::string>& items, const int items_to_display, const uint8_t flags ) :
	m_scroll_amount( 0 ),
	m_cached_items( false ), m_display_items_alt( {} ), m_last_itemsize( 0 ), m_old_search( 0 ), m_flags( flags )
{
	m_name = name;
	m_var = var;
	m_direct = false;

	m_items_alt = items;
	m_items = &m_items_alt;
	m_display_items = &m_display_items_alt;

	/*
	 * If we have a search bar add more space and init the searchbar
	 */
	m_items_to_display = items_to_display;
	m_search_bar = std::make_shared<text_box>( "A", m_search, textbox_style::flags::search_bar );
	m_search_bar->run_input();
}

listbox::listbox( const std::string& name, value* var, std::vector<listbox_item_t>* items, const int items_to_display, const uint8_t flags ) :
	m_scroll_amount( 0 ),
	m_cached_items( false ), m_display_items_alt( {} ), m_items_alt( {} ), m_last_itemsize( 0 ), m_old_search( 0 ), m_flags( flags )
{
	m_name = name;
	m_var = var;
	m_direct = false;

	m_items = nullptr;
	m_display_items = items;

	/*
	 * If we have a search bar add more space and init the searchbar
	 */
	m_items_to_display = items_to_display;
	m_search_bar = std::make_shared<text_box>( "A", m_search, textbox_style::flags::search_bar );
	m_search_bar->run_input();
}

void listbox::run_render( vec2& pos )
{
	if ( !should_render() )
		return;

	m_pos = pos;

	auto& d = draw.get_layer( layer::foreground );

	if ( !is_active() )
		d.push_alpha( 0.5f );

	_u( name, m_name );
	d.add_text( draw.get_font( FNV1A( "control" ) ), m_pos + vec2{ 0, 5 }, name, gui_color::text() );

	/*
	* Start drawing the actual slider below the text
	*/
	if ( !name.empty() )
	{
		m_pos.y += m_string_size.y;
	}

	m_pos.y += g_menu->get_style( e_menu_style::control_padding ).y * 0.5f;

	handle_hover_progress();

	/*
	 * Inner of the listbox
	 */
	d.add_rect_filled_linear_gradient( rect_legacy( m_pos, m_size ), gui_color::control_top(), gui_color::control_bot( 150 ) );

	cache_items();

	/*
	 * Clip area
	 */
	d.push_clip( rect_legacy( m_pos, m_size ) );

	/*
	 * Render all items
	 */
	auto base_position = vec2( floorf( m_pos.x + 8 ), ( m_flags & listbox_style::flags::search_bar ) ? m_pos.y + m_string_size.y : m_pos.y );
	auto counter = 0;
	auto rendered_items = 0;
	const auto items_to_display = std::max( 1u, m_items_to_display == -1 ? m_display_items->size() : m_items_to_display );
	for ( const auto& item : *m_display_items )
	{
		if ( counter++ < m_scroll_amount )
			continue;

		if ( ++rendered_items > items_to_display )
			break;

		const auto hovered = is_active() && !g_menu->get_focussed_control() && is_hovered( { m_pos.x, base_position.y }, { m_size.x, m_string_size.y } );
		const auto active = m_var->get<int>() == item.m_index;

		_u( name, item.m_name );

		if ( ( hovered || active ) && item.m_color != gui_color::text_solid() )
		{
			d.add_rect_filled_linear_gradient( rect_legacy( { base_position.x - 4, base_position.y + 3 }, { m_size.x - 16, m_string_size.y - 3 } ), gui_color::palette::hot_pink( active ? 50 : 25 ), gui_color::palette::hot_pink( 0 ), true );
			//d.add_line( { base_position.x + 1, base_position.y + m_string_size.y + 1 }, { base_position.x + evo::ren::text_size( draw.get_font( FNV1A( "control_unicode" ) ), name ).x, base_position.y + m_string_size.y + 1 }, active ? gui_color::palette::hot_pink() : gui_color::palette::main_pink( 100 ), 1 );
		}

		d.add_text( draw.get_font( FNV1A( "control_unicode" ) ), base_position + vec2{ 0, 2 }, name, active && item.m_color == gui_color::text_solid() ? gui_color::palette::hot_pink() : hovered && item.m_color == gui_color::text_solid() ? gui_color::palette::main_pink( 150 ) : gui_color::custom( item.m_color ) );

		base_position.y += m_string_size.y;
	}


	render_scroll();

	/*
	 * Reset limit
	 */
	d.pop_clip();

	/*
	* If we have a search bar, render it
	*/
	if ( m_flags & listbox_style::flags::search_bar )
	{
		m_search_bar->set_size( { m_size.x, m_string_size.y } );

		m_search_bar->run_render( m_pos );
	}

	/*
	 * Outline of box
	 */
	d.add_rect( rect_legacy( m_pos, m_size ), gui_color::palette::border( 255 ) );
	//render::rect( m_pos, m_size, gui_color::palette::main_pink( 255 * m_hover_progress ) );

	if ( !is_active() )
		d.pop_alpha();

	if ( !m_manual_padding )
	{
		pos.y += m_size.y + g_menu->get_style( e_menu_style::control_padding ).y + g_menu->get_style( e_menu_style::control_padding ).y;
		if ( !name.empty() )
			pos.y += m_string_size.y;
	}
}

void listbox::run_input()
{
	calc_string_size();

	const auto items_to_display = std::max( 1u, m_items_to_display == -1 ? m_display_items->size() : m_items_to_display );
	const auto total_items = ( m_flags & listbox_style::flags::search_bar ) ? items_to_display + 1 : items_to_display;
	m_size = { m_parent->get_size().x - g_menu->get_style( e_menu_style::child_padding ).x * 2 - 1, total_items * m_string_size.y + 3 };

	if ( !should_render() )
		return;

	if ( !is_active() )
		return;

	m_hovered = is_hovered( { m_pos.x, m_pos.y + ( m_flags & listbox_style::flags::search_bar ? m_string_size.y : 0 ) }, { m_size.x, m_size.y - ( m_flags & listbox_style::flags::search_bar ? m_string_size.y : 0 ) } );

	m_search_bar->run_input();

	/*
	 * Not in listbox area
	 */
	if ( !m_hovered )
		return;

	handle_scroll();

	handle_items();

}

void listbox::handle_items() const
{
	/*
	* Go through every item and check whether its
	* hovered or being selected
	*/
	auto base_position = vec2( m_pos.x, ( m_flags & listbox_style::flags::search_bar ) ? m_pos.y + m_string_size.y : m_pos.y );

	auto counter = 0;
	auto handled_items = 0;
	const auto items_to_display = std::max( 1u, m_items_to_display == -1 ? m_display_items->size() : m_items_to_display );
	for ( const auto& item : *m_display_items )
	{
		if ( counter++ < m_scroll_amount )
			continue;

		if ( ++handled_items > items_to_display )
			break;

		const auto hovered = is_hovered( base_position, { m_size.x, m_string_size.y } );
		const auto clicked = g_input->get_mouse().state == e_mouse_state::pressed && hovered;

		if ( clicked )
			m_var->set( item.m_index );

		base_position.y += m_string_size.y;


		base_position.y = floorf( base_position.y );
	}
}

void listbox::render_scroll() const
{
	/*
	 * Check if we need to render scroll arrows
	 */
	const auto items_to_display = std::max( 1u, m_items_to_display == -1 ? m_display_items->size() : m_items_to_display );
	if ( m_display_items->size() <= items_to_display )
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
	const auto show_lower_arrow = m_scroll_amount < m_display_items->size() - items_to_display;

	if ( show_upper_arrow )
	{
		d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 1, m_pos.y + ( m_flags & listbox_style::flags::search_bar ? m_string_size.y + 1 : 0 ) + 1 }, { m_size.x - 1, g_menu->get_style( e_menu_style::child_padding ).y + 4 } ), gui_color::child_sub( 255 ), gui_color::child_sub( 0 ) );
		auto base_position = Vector2D( m_pos.x + m_size.x - g_menu->get_style( e_menu_style::child_padding ).x, m_pos.y + g_menu->get_style( e_menu_style::child_padding ).y );
		if ( m_flags & listbox_style::flags::search_bar )
			base_position.y += m_string_size.y;

		arrow( base_position, false );
	}

	if ( show_lower_arrow )
	{
		d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 1, m_pos.y + m_size.y - g_menu->get_style( e_menu_style::child_padding ).y - 3 }, { m_size.x - 1, g_menu->get_style( e_menu_style::child_padding ).y + 4 } ), gui_color::child_sub( 0 ), gui_color::child_sub( 255 ) );
		const auto base_position = Vector2D( m_pos.x + m_size.x - g_menu->get_style( e_menu_style::child_padding ).x, m_pos.y + m_size.y - g_menu->get_style( e_menu_style::child_padding ).y );
		arrow( base_position, true );
	}
}

void listbox::handle_scroll()
{
	/*
	 * Check if we need a scrollbar
	 */
	const auto items_to_display = std::max( 1u, m_items_to_display == -1 ? m_display_items->size() : m_items_to_display );
	if ( m_display_items->size() <= items_to_display )
	{
		m_scroll_amount = 0;
		return;
	}

	///*
	// * No scroll input received
	// */
	//if ( g_input->get_mouse().scroll == 0 )
	//	return;

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

	m_scroll_amount = clamp( m_scroll_amount, 0, static_cast< int >( m_display_items->size() - items_to_display ) );

	g_input->get_mouse().reset_scroll();
}

void listbox::cache_items()
{
	// managed externally
	if ( !m_items )
		return;

	/*
	 * No items
	 */
	if ( m_items->empty() )
	{
		m_display_items->clear();
		m_last_itemsize = 0;
		return;
	}

	/*
	 * Original item size changed, re-cache it
	 */
	if ( m_items->size() != m_last_itemsize )
	{
		m_cached_items = false;
		m_display_items->clear();
		m_last_itemsize = m_items->size();
	}

	/*
	 * Search input changed
	 */
	if ( m_search.size() != m_old_search && ( m_flags & listbox_style::flags::search_bar ) )
	{
		m_cached_items = false;
		m_display_items->clear();
		m_old_search = m_search.size();
		m_scroll_amount = 0;
	}

	if ( m_cached_items )
	{
		auto should_recache = false;
		for ( auto i = 0; i < m_display_items->size(); i++ )
		{
			auto& string = m_items->at( m_display_items->at( i ).m_index );
			_u( item, string );
			if ( fnv1a_rt( item ) != m_display_items->at( i ).hash )
				should_recache = true;
		}

		if ( should_recache )
		{
			m_cached_items = false;
			m_display_items->clear();
			m_old_search = m_search.size();
		}
	}

	/*
	 * No need to do all calculations every
	 * time
	 */
	if ( m_cached_items )
		return;

	for ( auto i = 0; i < m_items->size(); i++ )
	{
		m_display_items->emplace_back( m_items->at( i ), i );
	}

	/*
	 * If we have a searchbar make sure to only display
	 * items that match the text input
	 */
	if ( m_flags & listbox_style::flags::search_bar && !m_search.empty() )
	{
		auto filtered_items = std::vector<listbox_item_t>{};
		auto filter_text = m_search;
		util::to_lower( filter_text );

		for ( const auto& item : *m_display_items )
		{
			_u( current_text, item.m_name );

			if ( !util::contains( filter_text, util::utf8_decode( current_text ) ) )
				continue;

			filtered_items.push_back( item );
		}

		*m_display_items = filtered_items;
	}

	m_cached_items = true;
}
