#include "../menu_include.h"

using namespace evo::ren;

text_box::text_box( std::string name, std::wstring& in, const uint8_t flags ) : m_focus( false ), m_text( &in ), m_flags( flags ), m_value( nullptr )
{
	m_name = std::move( name );
	m_string_size_multiplier = 1.75f;
}

text_box::text_box( std::string name, value* val, const uint8_t flags ) : m_focus( false ), m_text( nullptr ), m_flags( flags ), m_value( val )
{
	m_name = std::move( name );
	m_string_size_multiplier = 1.75f;
}

void text_box::run_render( vec2& pos )
{
	if ( !should_render() )
		return;

	m_pos = pos;

	handle_hover_progress();

	auto& d = draw.get_layer( layer::foreground );

	_u( name, m_name );
	d.add_text( draw.get_font( FNV1A( "control" ) ), m_pos + vec2{ 0, 5 }, name, gui_color::text() );

	/*
	* Start drawing the actual slider below the text
	*/
	if ( !name.empty() )
	{
		m_pos.y += m_string_size.y;
	}


	/*
	 * Inner
	 */
	d.add_rect_filled( rect_legacy( m_pos, { m_size.x, m_string_size.y } ), gui_color::control_bot() );

	/*
	 * Outline
	 */
	d.add_rect( rect_legacy( m_pos, { m_size.x, m_string_size.y } ), gui_color::palette::border( 255 - 255 * m_hover_progress ) );
	d.add_rect( rect_legacy( m_pos, { m_size.x, m_string_size.y } ), m_focus ? gui_color::palette::main_pink() : gui_color::palette::main_pink( 255 * m_hover_progress ) );

	d.push_clip( rect_legacy( m_pos + 1.f, { m_size.x - 7, m_string_size.y - 2 } ) );

	/*
	 * Search bar
	 */
	if ( m_flags & textbox_style::flags::search_bar )
	{
		/*
		 * No search input, draw "Search..." text
		 */
		_( search, "Search..." );

		if ( m_text->empty() && !m_focus )
			d.add_text( draw.get_font( FNV1A( "control" ) ), { m_pos.x + g_menu->get_style( e_menu_style::control_inner_padding ).x, m_pos.y + m_string_size.y / 2 + 3 }, search, gui_color::palette::border( 150 ), 0, layer::align_center );
		else
		{
			auto text = util::utf8_encode( *m_text );
			/*if ( m_focus && static_cast< int >( interfaces::globals()->realtime ) % 2 )
				text += L" |";*/

			const auto sz = evo::ren::text_size( evo::ren::draw.get_font( FNV1A( "control" ) ), text );
			const auto move_by = std::max( 0, int( sz.x - m_size.x ) + 10 );

			d.add_text( draw.get_font( FNV1A( "control_unicode" ) ), { m_pos.x + g_menu->get_style( e_menu_style::control_inner_padding ).x + move_by, m_pos.y + m_string_size.y / 2 }, text, gui_color::text(), 0, layer::align_center );

			if ( move_by )
				d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 1, m_pos.y + 1 }, { g_menu->get_style( e_menu_style::control_padding ).x * 5.f - 9, m_string_size.y - 2 } ), gui_color::control_top( 255 ), gui_color::control_top( 0 ), true );
		}

	}
	else
	{
		auto text = m_value->get_string();
		/*if ( m_focus && static_cast< int >( interfaces::globals()->realtime ) % 2 )
			text += xorstr_( " |" );*/

		const auto sz = evo::ren::text_size( evo::ren::draw.get_font( FNV1A( "control" ) ), text );
		const auto move_by = std::max( 0, int( sz.x - m_size.x ) + 10 );

		d.add_text( draw.get_font( FNV1A( "control" ) ), { m_pos.x + g_menu->get_style( e_menu_style::control_inner_padding ).x - move_by, m_pos.y + m_string_size.y / 2 + 3 }, text, gui_color::text(), 0, layer::align_center );

		/*
		* Render all sub controls registered
		*/
		render_sub_controls();


		if ( move_by )
			d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 1, m_pos.y + 1 }, { g_menu->get_style( e_menu_style::control_padding ).x * 5.f - 9, m_string_size.y - 2 } ), gui_color::control_top( 255 ), gui_color::control_top( 0 ), true );
		/*
		* Reset position and adjust for controls after
		*/
		m_pos = pos;

		if ( !m_manual_padding )
			pos.y += m_size.y + g_menu->get_style( e_menu_style::control_padding ).y;
	}

	d.pop_clip();

}

void text_box::run_input()
{
	calc_string_size();

	auto multi = m_string_size_multiplier;
	_u( name, m_name );
	if ( name.empty() )
		multi = 1;

	if ( m_parent )
		m_size = { m_parent->get_size().x - g_menu->get_style( e_menu_style::child_padding ).x * 2, m_string_size.y * multi };

	if ( !should_render() )
		return;

	m_hovered = is_hovered( m_pos, m_size );

	if ( m_hovered && g_input->get_mouse().state == e_mouse_state::pressed )
	{
		m_focus = true;
		g_menu->set_keyboard_control( this );
	}

	if ( !m_focus )
		return;

	if ( g_input->get_key( VK_ESCAPE ).pressed || g_input->get_key( VK_INSERT ).pressed ||
		 g_input->get_mouse().state == e_mouse_state::pressed && !m_hovered )
	{
		m_focus = false;
		g_menu->set_keyboard_control( nullptr );
		return;
	}

	/*
	 * Key input received
	 */
	const auto input_char = g_input->get_last_char();
	if ( input_char == '\0' )
		return;

	if ( m_text )
	{
		switch ( input_char )
		{
			case '\n':

				break;
			case '\b':
				if ( !m_text->empty() )
					m_text->resize( m_text->size() - 1 );
				break;
			default:
				if ( input_char >= 0x20 )
					*m_text += input_char;
		}
	}
	else
	{
		auto str = m_value->get_string();

		switch ( input_char )
		{
			case '\n':

				break;
			case '\b':
				if ( !str.empty() )
					str.resize( str.size() - 1 );
				break;
			default:
				if ( input_char >= 0x20 && input_char <= 0x80 )
					str += input_char;
		}

		m_value->set_string( str );
	}


	g_input->clear_char();
}