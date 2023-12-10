#include "../menu_include.h"

using namespace evo::ren;

checkbox::checkbox( std::string name, value* var )
{
	m_name = name;
	m_var = var;
}

void checkbox::run_render( vec2& pos )
{
	/*
	 * If we require a dependency dont render unless
	 * it's fulfilled - same for input
	 */
	if ( !should_render() )
		return;


	_u( name, m_name );
	_( los, "Load with config" );
	if ( std::string( name ) == los )
	{
		const auto selected = lua::api.selected_unload->get<int>();

		if ( selected >= 0 && selected < lua::api.loaded_scripts.size() )
		{
			_u( name, lua::api.loaded_scripts[ selected ] );
			m_var = config_init::get_item_direct( fnv1a_rt( xorstr_( "lua_" ) + name ) );
		}
		else
		{
			m_var = nullptr;
		}
	}

	if ( !m_var )
		return;

	m_pos = pos;

	/*
	 * Those two have to be called every frame rather than
	 * only when receiving input
	 */
	handle_active_progress( m_var->get<bool>() );
	handle_hover_progress();

	/*
	 * Y Size of text = checkbox width
	 */
	auto& d = draw.get_layer( layer::foreground );

	d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 1, m_pos.y + 3 }, { m_size.y - 6, m_size.y - 6 } ), gui_color::control_top(), gui_color::control_bot() );

	if ( m_active_progress != 0.f )
		d.add_rect_filled_linear_gradient( rect_legacy( { m_pos.x + 2, m_pos.y + 4 }, { m_size.y - 9, m_size.y - 9 } ), gui_color::palette::main_pink( 255 * m_active_progress ), gui_color::palette::main_pink( 150 * m_active_progress ) );

	d.add_rect( rect_legacy( { m_pos.x, m_pos.y + 2 }, { m_size.y - 5, m_size.y - 5 } ), gui_color::palette::border() );
	d.add_rect( rect_legacy( { m_pos.x, m_pos.y + 2 }, { m_size.y - 5, m_size.y - 5 } ), m_var->get<bool>() ? gui_color::palette::main_pink() : gui_color::palette::main_pink( m_hover_progress * 255 ) );

	d.add_text( draw.get_font( FNV1A( "control" ) ), { m_pos.x + m_size.y + g_menu->get_style( e_menu_style::control_inner_padding ).x, m_pos.y + 3 }, name, gui_color::text() );

	/*
	 * Render all sub controls registered
	 */
	render_sub_controls();

	if ( !m_manual_padding )
		pos.y += m_size.y + g_menu->get_style( e_menu_style::control_padding ).y;
}

void checkbox::run_input()
{
	init_subs();

	calc_string_size();

	m_size = { floorf( m_string_size.x + g_menu->get_style( e_menu_style::control_inner_padding ).x + m_string_size.y * m_string_size_multiplier ), floorf( m_string_size.y * m_string_size_multiplier ) };

	if ( !should_render() )
		return;

	if ( !m_var )
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
	if ( m_hovered && g_input->get_mouse().state == e_mouse_state::pressed )
	{
		/*
		 * Grab old value and set it to the
		 * opposite
		 */
		const auto old = m_var->get<bool>();

		m_var->set( !old );
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

