#include "../../menu_include.h"

using namespace evo::ren;

key_bind::key_bind( value* var, value* key ) : m_waiting( false ), m_popup_open( false ), m_key( key )
{
	m_var = var;

	m_max_size = 35.f;
	if ( key->get_key_mode() )
		m_mode = key->get_key_mode();
	else
		m_mode = new value( "" );

	if ( key->get_key_temp() )
		m_tempval = key->get_key_temp();
	else
		m_tempval = new value( "" );

	m_temp_value = m_tempval->get_int();
	m_popup = std::make_shared<combobox>( "A", m_mode, combobox_style::flags::pop_up, std::vector<combo_item_t>{ { _w( "ALWAYS ON" ), 0 }, { _w( "HOLD" ), 1 }, { _w( "TOGGLE" ), 2 }, { _w( "FORCE OFF" ), 3 } } );
	m_popup->set_size( { 120, 69 + g_menu->get_style( e_menu_style::popup_padding ).y * 2 } );
}

key_bind::key_bind( value* key ) : m_waiting( false ), m_popup_open( false )
{
	m_var = nullptr;
	m_key = key;
	m_tempval = nullptr;
	m_max_size = 35.f;
	if ( key->get_key_mode() )
		m_mode = key->get_key_mode();
	else
		m_mode = new value( "" );

	if ( key->get_key_temp() )
		m_tempval = key->get_key_temp();
	else
		m_tempval = new value( "" );

	m_temp_value = m_tempval->get_int();

	m_popup = std::make_shared<combobox>( "A", m_mode, combobox_style::flags::pop_up, std::vector<combo_item_t>{ { _w( "ALWAYS ON" ), 0 }, { _w( "HOLD" ), 1 }, { _w( "TOGGLE" ), 2 }, { _w( "FORCE OFF" ), 3 } } );
	m_popup->set_size( { 120, 69 + g_menu->get_style( e_menu_style::popup_padding ).y * 2 } );
}

void key_bind::run_render( const evo::ren::vec2 pos, const evo::ren::vec2 size )
{
	if ( !should_render() )
		return;

	handle_hover_progress();

	m_pos = pos;
	m_size = size;

	auto& d = draw.get_layer( layer::foreground );

	/*
	 * Border
	 */
	d.add_rect( rect_legacy( m_pos, m_size ), gui_color::palette::border() );

	/*
	 * Pink border
	 */
	d.add_rect( rect_legacy( m_pos, m_size ), m_waiting || m_popup_open ? gui_color::palette::main_pink() : gui_color::palette::main_pink( 150 * m_hover_progress ) );

	/*
	 * Inner
	 */
	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 1, m_pos.y + 1 }, { m_size.x - 2, m_size.y - 2 } ), gui_color::control_top( 200 + 50 * m_hover_progress ), gui_color::control_bot( 170 + 60 * m_hover_progress ) );

	/*
	 * If we are waiting for key input - display 3 dots
	 */
	const auto key_text = input::get_key_string( m_key->get<int>() );

	/*
	 * Limit the viewport just in case...
	 */
	d.push_clip( rect_legacy( m_pos, m_size ) );

	/*
	 * Render key string
	 */
	_u( render_key_text, key_text );
	d.add_text( draw.get_font( FNV1A( "title_font" ) ), { m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2 + 2 }, render_key_text, gui_color::text(), layer::align_center, layer::align_center );

	d.pop_clip();

	/*
	 * If the popup is open - render it
	 */
	if ( m_popup_open )
	{
		/*
		 * We have to calc string size here to prevent
		 * it being done too late
		 */
		m_popup->calc_string_size();

		m_popup->run_render( m_popup_pos );
	}
}

void key_bind::run_input()
{
	if ( !g_menu->is_open() )
		return;

	if ( !should_render() )
		return;

	m_hovered = is_hovered( m_pos, m_size );

	/*
	 * Toggle key state
	 */
	if ( !m_waiting && !m_popup_open && m_hovered && g_input->get_mouse().state == e_mouse_state::pressed )
	{
		m_waiting = true;
		m_held = true;
		m_parent->set_focussed_control( this );
		g_menu->set_focussed_control( m_parent.get() );
		g_menu->set_keyboard_control( m_parent.get() );
		return;
	}

	/*
	 * Keybind default right-clicked - open popup menu
	 * Don't open if we have no control to change
	 * the values of and just a key
	 */
	if ( !m_waiting && m_hovered && g_input->get_mouse().state_right == e_mouse_state::pressed && m_var )
	{
		m_popup_pos = g_input->get_mouse().pos;
		m_popup_open = true;

		m_parent->set_focussed_control( this );
		g_menu->set_focussed_control( m_parent.get() );

		return;
	}

	/*
	* Handle popup input
	*/
	if ( m_popup_open )
		m_popup->run_input();

	/*
	 * If popup is open and the user clicks anywhere - close it
	 */
	if ( m_popup_open &&
		 g_input->get_mouse().state == e_mouse_state::pressed || g_input->get_mouse().state_right == e_mouse_state::pressed )
	{
		m_popup_open = false;

		m_parent->set_focussed_control( nullptr );
		g_menu->set_focussed_control( nullptr );

		return;
	}

	if ( m_held && m_waiting && g_input->get_mouse().state == e_mouse_state::idle )
		m_held = false;
	/*
	 * If this control is not active, no need tp
	 * go throguh functions below
	 */
	if ( !m_waiting || m_held )
		return;

	/*
	 * If Escape is pressed the key will be reset
	 */
	if ( g_input->get_key( VK_ESCAPE ).held || g_input->get_key( VK_ESCAPE ).released )
	{
		if ( g_input->get_key( VK_ESCAPE ).held )
			m_key->set( 0 );
		m_waiting = false;
		m_held = false;

		m_parent->set_focussed_control( nullptr );
		g_menu->set_focussed_control( nullptr );
		g_menu->set_keyboard_control( nullptr );
		return;
	}

	/*
	 * Go through every key and check which one is pressed
	 */
	for ( auto i = 1; i < 256; i++ )
	{
		if ( i == VK_ESCAPE )
			continue;

		if ( g_input->get_key( i ).pressed )
		{
			m_key->set( i );
			m_waiting = false;
			m_held = false;

			m_parent->set_focussed_control( nullptr );
			g_menu->set_focussed_control( nullptr );
			g_menu->set_keyboard_control( nullptr );
			break;
		}
	}
}

struct CCSGO_HudChat
{
private:
	char pad_0x0000[ 0x60 ];
public:
	bool m_isOpen;
};


void key_bind::handle_mode()
{
	if ( m_waiting || !m_var || m_key->get<int>() <= 0 || g_menu->get_keyboard_control() )
		return;

	if ( m_last_key_state == g_input->get_key( m_key->get<int>() ).pressed || g_input->get_key( m_key->get<int>() ).held )
		return;

	m_last_key_state = g_input->get_key( m_key->get<int>() ).pressed || g_input->get_key( m_key->get<int>() ).held;

	if ( m_tempval )
		m_temp_value = m_tempval->get_int();

	_( chat_elem, "CCSGO_HudChat" );
	const auto chat_element = reinterpret_cast< CCSGO_HudChat* >( interfaces::hud()->FindElement( chat_elem.c_str() ) );

	if ( ( interfaces::surface()->NeedKBInput() || chat_element->m_isOpen ) && !g_menu->is_open() )
		return;

	if ( m_var->get<int>() && m_temp_value != m_var->get<int>() )
		m_temp_value = m_var->get<int>();

	if ( !m_var->get<int>() && !m_temp_value )
		m_temp_value = 1;

	switch ( m_mode->get<int>() )
	{
		case 0: // Always on, don't modify value
			break;
		case 2: // Toggle
			if ( m_last_key_state )
			{
				if ( m_var->get<int>() )
				{
					m_temp_value = m_var->get<int>();
					m_var->set( 0 );
				}
				else if ( m_temp_value )
				{
					m_var->set( m_temp_value );
				}
			}
			break;
		case 1: // Hold
			if ( m_last_key_state )
			{
				if ( m_temp_value )
					m_var->set( m_temp_value );
			}
			else
			{
				if ( m_var->get<int>() )
					m_temp_value = m_var->get<int>();
				m_var->set( 0 );
			}
			break;
		case 3: // Force off
			if ( m_last_key_state )
			{
				if ( m_var->get<int>() )
					m_temp_value = m_var->get<int>();

				m_var->set( 0 );
			}
			else
			{
				if ( m_temp_value )
					m_var->set( m_temp_value );
			}
		default:;
	}

	if ( m_tempval )
		m_tempval->set_int( m_temp_value );
}
