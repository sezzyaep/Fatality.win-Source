#include "../include_cheat.h"

LRESULT __stdcall hook::wnd_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	lua::api.callback( FNV1A( "on_input" ), [&] ( lua::state& s ) -> int
	{
		s.push( static_cast< int >( uMsg ) );
		s.push( static_cast< int >( wParam ) );
		s.push( static_cast< int >( lParam ) );

		return 3;
	} );


	auto handled_mouse = false;
	auto handled_keyboard = false;
	auto handled_char = false;

	if ( uMsg == WM_KEYUP && wParam == VK_INSERT )
		g_menu->toggle();

	if ( hWnd != GetActiveWindow() )
	{
		for ( auto i = 0; i < 256; i++ )
		{
			g_input->m_key_info.at( i ).pressed = false;
			g_input->m_key_info.at( i ).held = false;
			g_input->m_key_info.at( i ).released = true;
		}

		g_menu->run_hotkey_input();
	}
	else
	{
		handled_mouse = g_input->run_mouse( uMsg );
		handled_keyboard = g_input->run_keyboard( uMsg, wParam );

		if ( uMsg == WM_CHAR )
		{
			if ( wParam > 0 && wParam < 0x10000 )
				g_input->m_last_char = static_cast< wchar_t >( wParam );

			handled_char = true;
		}
	}

	//if ( uMsg == WM_KEYUP && wParam == VK_END ) // debug
		//g_menu->reset();

	if ( handled_keyboard )
		g_menu->run_hotkey_input();

	if ( g_menu->is_open() && ( handled_mouse || handled_keyboard || handled_char ) )
	{
		g_menu->run_input();

		const auto handled_scroll = g_input->get_mouse().scroll == 0;

		g_input->get_mouse().reset_scroll();

		g_input->clear_char();

		if ( handled_mouse || handled_scroll && uMsg == WM_MOUSEWHEEL || wParam == VK_ESCAPE || g_menu->get_keyboard_control() )
			return false;
	}

	for ( auto i = 0; i < 256; i++ )
		g_input->m_key_info.at( i ).released = false;

	const auto ret = original( wnd_proc )( hWnd, uMsg, wParam, lParam );

	return ret && !handled_keyboard && !handled_mouse;
}