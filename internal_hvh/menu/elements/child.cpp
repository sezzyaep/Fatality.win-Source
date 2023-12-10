#include "../menu_include.h"

using namespace evo::ren;

child::child( const std::string name, const vec2 offset, const vec2 size, std::vector<std::shared_ptr<control>> controls, std::shared_ptr<window> window_parent ) : m_string_size( vec2( 0, 0 ) )
{
	m_name = name;
	m_offset = offset;
	m_size = size;
	m_controls = controls;
	m_window_parent = window_parent;

	/*for ( auto& item : g_lua_items )
	{
		_u( tname, m_parent->get_parent()->get_name() );
		if ( std::get<0>( item ) != tname )
			continue;

		_u( sname, m_parent->get_name() );
		if ( std::get<1>( item ) != sname )
			continue;

		_u( cname, m_name );
		if ( std::get<2>( item ) != cname )
			continue;
	}*/

	for ( const auto& control : m_controls )
		control->init( std::shared_ptr<child>( this ) );
}

void child::run_render()
{
	/*
	* "Calculate" position
	*/
	if ( !m_window_parent )
	{
		const auto add = g_menu->get_style( e_menu_style::window_padding ).y * 2.f;

		if ( m_parent->get_parent() )
		{
			m_pos = { m_parent->get_parent()->get_parent()->get_control_area_start().x + m_offset.x,
				m_parent->get_pos().y + add + m_offset.y };
		}
		else
		{
			m_pos = { m_parent->get_parent_sub_tab()->get_parent()->get_parent()->get_control_area_start().x + m_offset.x,
				m_parent->get_pos().y + add + m_offset.y };
		}

	}
	else
	{
		m_pos = { m_window_parent->get_control_area_start().x + m_offset.x,
			m_window_parent->get_control_area_start().y + m_offset.y };
	}

	/*
	* Main area
	*/
	auto& d = draw.get_layer( layer::foreground );

	d.add_rect_filled_linear_gradient( rect_legacy( m_pos, m_size ), gui_color::child_sub(), gui_color::child_sub( 140 ) );

	/*
	 * Title
	 */
	d.add_rect_filled( rect_legacy( m_pos, { m_size.x, m_string_size.y } ), gui_color::child_sub() );

	/*
	 * Border
	 */
	d.add_line( m_pos, { m_pos.x, m_pos.y + m_size.y }, gui_color::palette::border() );
	d.add_line( { m_pos.x, m_pos.y + m_size.y }, { m_pos.x + m_size.x, m_pos.y + m_size.y }, gui_color::palette::border() );
	d.add_line( { m_pos.x + m_size.x, m_pos.y }, { m_pos.x + m_size.x, m_pos.y + m_size.y }, gui_color::palette::border() );
	d.add_line( m_pos, m_pos + vec2( g_menu->get_style( e_menu_style::control_padding ).x, 0 ), gui_color::palette::border() );
	d.add_line( m_pos + vec2( g_menu->get_style( e_menu_style::control_padding ).x * 3 + m_string_size.x, 0 ), m_pos + vec2( m_size.x, 0 ), gui_color::palette::border() );

	d.push_clip( rect_legacy( { m_pos.x, floorf( m_pos.y + m_string_size.y / 2.f ) }, { m_size.x, floorf( m_size.y - m_string_size.y / 2.f ) } ) );
	/*
	 * Draw every control and if there's a focussed
	 * control draw it after everyone else
	 */
	auto base_position = m_pos + g_menu->get_style( e_menu_style::child_padding );

	if ( m_max_scroll_size <= 0.f )
	{
		m_scroll_amount = 0;
	}

	_u( name, m_name );

	if ( m_max_scroll_size > 0.f )
		base_position.y -= clamp( m_scroll_amount * m_string_size.y, 0.f, m_max_scroll_size );

	auto is_preview = fnv1a_rt( name ) == FNV1A( "Preview" );

	if ( is_preview )
		base_position += vec2( 49.f, 3.f );

	for ( const auto& control : m_controls )
	{
		base_position.x = floorf( base_position.x );
		base_position.y = floorf( base_position.y );

		if ( base_position.y > m_pos.y + m_size.y )
			break;
		/*
		 * Save position of focussed control for later
		 */
		if ( control.get() == g_menu->get_focussed_control() )
		{
			g_menu->set_focused_control_pos( base_position );
			g_menu->set_focused_scissor( d.get_clip().value() );

			/*
			* Manually add to position
			*/
			base_position.y += control->get_size().y + g_menu->get_style( e_menu_style::control_padding ).y;

			continue;
		}

		control->run_render( base_position );
	}

	if ( is_preview && entity_rendering::is_setup )
	{
		d.push_aa( true );
		d.push_texture_raw( entity_rendering::preview_texture->pTextureHandles[ 0 ]->lpRawTexture );
		d.add_rect_filled( rect_legacy( m_pos + vec2( 0, 8 ), { ( float ) entity_rendering::preview_texture->GetActualWidth(), ( float ) entity_rendering::preview_texture->GetActualHeight() } ), gui_color::custom( color( 255, 255, 255 ) ) );
		d.pop_texture();
		d.pop_aa();

		entity_rendering::preview_pos = Vector2D( m_pos + vec2( 0, 8 ) );

		if ( !vars::menu.preview_type->get<int>() )
		{
			visuals::queue_custom_player_esp();
			visuals::render_custom_player_esp();
		}

	}

	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x, m_pos.y + m_size.y - g_menu->get_style( e_menu_style::child_padding ).y }, { m_size.x - 1, g_menu->get_style( e_menu_style::child_padding ).y - 1 } ), gui_color::child_sub( 0 ), gui_color::child_sub( 255 ) );
	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x, m_pos.y + m_string_size.y / 2.f - 2 }, { m_size.x - 1, g_menu->get_style( e_menu_style::child_padding ).y - 4 } ), gui_color::child_sub( 255 ), gui_color::child_sub( 0 ) );

	render_scroll();

	d.pop_clip();

	d.add_text( draw.get_font( FNV1A( "child_title" ) ), m_pos + vec2( g_menu->get_style( e_menu_style::control_padding ).x * 2, +2 ), name, gui_color::tab_active(), 0, layer::align_center );
}

void child::run_input()
{
	/*
	 * Get string size for proper size of title bar
	 */
	_u( name, m_name );

	if ( m_string_size.is_zero() )
		m_string_size = evo::ren::text_size( evo::ren::draw.get_font( FNV1A( "child_title" ) ), name ) + 3.f;

	/*
	* Handle all controls.
	* if there's a focussed control only
	* handle that one
	*/
	for ( const auto& control : m_controls )
	{
		if ( control->get_size().is_zero() )
		{
			control->run_input();
		}
		/*
		 * Check whether the control is out of focus
		 */
		if ( !control->get_pos().is_zero() && ( control->get_pos().y + 1 > m_pos.y + m_size.y - g_menu->get_style( e_menu_style::child_padding ).y / 2.f
			|| control->get_pos().y + control->get_size().y < m_pos.y + m_string_size.y / 2.f + g_menu->get_style( e_menu_style::child_padding ).y / 2.f ) )
		{
			/*
			 * If the control is active, reset it
			 */
			control->reset_state();
			continue;
		}

		if ( control.get() == g_menu->get_focussed_control() )
		{
			control->run_input();
			break;
		}

		if ( g_menu->get_focussed_control() )
			continue;

		control->run_input();
	}

	if ( !g_menu->get_focussed_control() && g_input->mouse_in_bounds( m_pos, m_size ) )
		handle_scroll();
}

void child::reset_state()
{
	for ( const auto& control : m_controls )
	{
		control->reset_state();
	}
}

void child::run_hotkey_input()
{
	for ( const auto& control : m_controls )
	{
		control->run_hotkey_input();
	}
}

std::shared_ptr<control> child::add_control( std::shared_ptr<control> control )
{
	control->init( this->shared_from_this() );
	m_controls.emplace_back( control );
	return control;
}

void child::remove_control( std::shared_ptr<control> control )
{
	for ( auto it = m_controls.begin(); it != m_controls.end(); )
	{
		if ( *it == control )
		{
			it = m_controls.erase( it );
			break;
		}

		++it;
	}
}

void child::init( std::shared_ptr<sub_tab> tab )
{
	m_parent = tab;
}

vec2 child::get_size() const
{
	return m_size;
}

vec2 child::get_pos() const
{
	return m_pos;
}

std::string& child::get_name()
{
	return m_name;
}

std::vector<std::shared_ptr<control>>& child::get_controls()
{
	return m_controls;
}

std::shared_ptr<sub_tab> child::get_parent() const
{
	return m_parent;
}

void child::render_scroll() const
{
	const auto arrow = [&] ( const Vector2D& base, const bool reverse ) -> void
	{
		const auto col = gui_color::palette::border( 255 );

		auto& d = draw.get_layer( layer::foreground );

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
	const auto show_lower_arrow = m_scroll_amount * m_string_size.y < m_max_scroll_size;

	if ( show_upper_arrow )
	{
		const auto base_position = Vector2D( m_pos.x + m_size.x - g_menu->get_style( e_menu_style::child_padding ).x, m_pos.y + g_menu->get_style( e_menu_style::child_padding ).y - 2 );
		arrow( base_position, false );
	}

	if ( show_lower_arrow )
	{
		const auto base_position = Vector2D( m_pos.x + m_size.x - g_menu->get_style( e_menu_style::child_padding ).x, m_pos.y + m_size.y - g_menu->get_style( e_menu_style::child_padding ).y ) + 2;
		arrow( base_position, true );
	}
}

void child::handle_scroll()
{
	/*
	* Check if control size overrides chile size
	* control draw it after everyone else
	*/
	auto temp_position = g_menu->get_style( e_menu_style::child_padding ) + g_menu->get_style( e_menu_style::control_padding ).y;
	for ( const auto& control : m_controls )
	{
		if ( !control->should_render() )
			continue;

		if ( control->is_listbox() )
		{
			_u( name, control->get_name() );
			if ( !name.empty() )
				temp_position.y += control->get_string_size().y + g_menu->get_style( e_menu_style::control_padding ).y;
		}

		temp_position.y += control->get_size().y + g_menu->get_style( e_menu_style::control_padding ).y;
	}

	/*
	 * Total size of all controls.
	 * If above the child size a scrollbar
	 * is needed to still display all controls
	 */
	m_max_scroll_size = temp_position.y - m_size.y;
	if ( m_max_scroll_size <= 0.f )
	{
		m_max_scroll_size = 0.f;
		return;
	}

	/*
	* Handle scrolling ( add to base postition )
	*/
	if ( g_input->get_mouse().scroll < 0 )
		m_scroll_amount++;

	if ( g_input->get_key( VK_DOWN ).pressed || g_input->get_key( VK_DOWN ).held )
		m_scroll_amount++;
	if ( g_input->get_key( VK_UP ).pressed || g_input->get_key( VK_UP ).held )
		m_scroll_amount--;

	/*
	 * Scroll down
	 */

	/*
	 * Scroll up
	 */
	else if ( g_input->get_mouse().scroll > 0 )
		m_scroll_amount--;

	g_input->get_mouse().reset_scroll();

	m_scroll_amount = clamp( m_scroll_amount, 0, ( int ) std::ceil( m_max_scroll_size / m_string_size.y ) );
}
