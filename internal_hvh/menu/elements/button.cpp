#include "../menu_include.h"

using namespace evo::ren;

std::vector<std::function<void()>> queued_calls = {};

button::button( std::string name, std::function<void()> func, uint8_t flags ) : m_flags( flags )
{
	m_name = name;
	m_func = func;
}

void button::run_render( vec2& pos )
{
	/*
	 * If we require a dependency dont render unless
	 * it's fulfilled - same for input
	 */
	if ( !should_render() )
		return;

	m_pos = pos;

	/*
	 * Those two have to be called every frame rather than
	 * only when receiving input
	 */
	handle_active_progress( m_held );
	handle_hover_progress();

	/*
	 * Y Size of text = checkbox width
	 */

	auto& d = draw.get_layer( layer::foreground );

	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 2, m_pos.y + 2 }, { m_size.x - 4, m_size.y - 4 } ), gui_color::palette::main_pink( 255 - m_active_progress * 255 ), gui_color::palette::main_pink( 150 - m_active_progress * 150 ) );
	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 2, m_pos.y + 2 }, { m_size.x - 4, m_size.y - 4 } ), gui_color::palette::main_pink( m_active_progress * 150 ), gui_color::palette::main_pink( m_active_progress * 255 ) );

	d.add_rect( rect_legacy( { m_pos.x, m_pos.y }, { m_size.x, m_size.y } ), gui_color::palette::border() );
	d.add_rect( rect_legacy( { m_pos.x, m_pos.y }, { m_size.x, m_size.y } ), m_held ? gui_color::palette::main_pink() : gui_color::palette::main_pink( m_hover_progress * 255 ) );

	_u( name, m_name );
	d.add_text( draw.get_font( FNV1A( "control" ) ), { floorf( m_pos.x + m_size.x / 2 ), m_pos.y + 3 }, name, gui_color::text( 255 ), layer::align_center );

	/*
	 * Render all sub controls registered
	 */
	render_sub_controls();

	if ( !m_manual_padding )
		pos.y += m_size.y + g_menu->get_style( e_menu_style::control_padding ).y;
}

void button::run_input()
{
	init_subs();

	calc_string_size();

	m_size = { m_parent ? roundf( m_parent->get_size().x / 2 ) : 70, m_string_size.y };

	if ( !should_render() )
		return;

	if ( m_focussed_control )
	{
		m_focussed_control->run_input();
		return;
	}

	m_hovered = is_hovered( m_pos, m_size );

	/*
	 * Checkbox default pressed
	 */
	if ( m_hovered && g_input->get_mouse().state != e_mouse_state::idle )
	{
		m_held = true;
		m_running = false;
	}
	else
	{
		/*
		 * toggle held state / run func if released
		 */
		if ( m_held && m_hovered && !m_cancel_callback )
		{
			if ( !m_running /*&& !m_cancel_callback*/ )
			{
				m_running = true;
				/*m_cancel_callback = true;
				m_func_thread = std::thread( [this]
				{
					m_func();
					m_cancel_callback = false;
				} );
				m_func_thread.detach();*/
				queued_calls.emplace_back( m_func );
			}
		}

		m_held = false;
	}

	/*
	 * No sub controls registered -> no work to do
	 */
	if ( m_controls.empty() )
		return;

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
