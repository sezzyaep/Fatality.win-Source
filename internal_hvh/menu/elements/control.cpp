#include "../menu_include.h"

void control::reset_state()
{
	if ( g_menu->get_focussed_control() == this )
	{
		g_menu->set_focussed_control( nullptr );
		set_focussed_control( nullptr );
	}

	m_hover_progress = 0.f;
	m_hovered = false;

	for ( auto& control : m_controls )
		control->reset_state();
}

void control::run_hotkey_input()
{
	for ( auto& control : m_controls )
	{
		control->run_hotkey_input();
	}
}

void control::init_subs()
{
	if ( m_did_init_subs )
		return;

	for ( const auto& control : m_controls )
		control->set_parent( shared_from_this() );

	m_did_init_subs = true;
}

void control::render_sub_controls()
{
	if ( m_controls.empty() )
		return;

	const auto amount = m_controls.size();

	/*
	 * The maximum size the sub controls can be drawn in
	 */
	const auto max_size = Vector2D(
		m_parent->get_size().x - m_size.x - g_menu->get_style( e_menu_style::control_padding ).x * 2, m_string_size.y - 4 );
	const auto max_size_per_control = floorf( max_size.x / amount - g_menu->get_style( e_menu_style::sub_control_padding ).x * ( amount -
		1 ) - g_menu->get_style( e_menu_style::child_padding ).x );

	/*
	 * We will start at the back
	 */
	auto base_position = m_parent->get_pos().x + m_parent->get_size().x - g_menu->get_style( e_menu_style::child_padding ).x;

	auto initial_padding = false;

	for ( const auto& control : m_controls )
	{
		if ( !control->should_render() )
			continue;

		auto size = max_size_per_control;

		/*
		 * We don't want to go over the max width of the sub control
		 */
		if ( max_size_per_control > control->get_max_size() )
			size = control->get_max_size();

		/*
		 * Adjust our position once
		 */
		if ( !initial_padding )
		{
			base_position -= size;
			initial_padding = true;
		}

		if ( m_focussed_control == *( sub_control** ) &control )
			control->m_in_force_off = true;

		control->run_render( { base_position, m_pos.y + 2 }, { size, max_size.y } );

		/*
		 * Set up next position ( if there is another control )
		 */
		base_position -= g_menu->get_style( e_menu_style::sub_control_padding ).x;
		base_position -= size;
	}

	if ( m_focussed_control && m_focussed_control->should_render() )
	{
		m_focussed_control->m_in_force_off = false;
		m_focussed_control->run_render( m_focussed_control->get_pos(), m_focussed_control->get_size() );
	}
}

bool control::should_render() const
{
	if ( m_force_hide )
		return false;

	if ( m_dependencies.empty() )
		return true;

	auto ret = false;

	auto has_flip = false;
	auto temp_ret = true;

	for ( auto& dependency : m_dependencies )
	{
		if ( !dependency.m_val )
			return true;//!interfaces::engine()->IsInGame() || !( *interfaces::game_rules_proxy() )->m_bIsValveDS();

		if ( !dependency.m_flip && dependency.m_val->get<double>() == dependency.m_required )
			ret = true;

		if ( dependency.m_flip )
			has_flip = true;

		if ( dependency.m_flip && dependency.m_val->get<double>() == dependency.m_required )
			temp_ret = false;
	}

	return ret || has_flip && temp_ret;
}