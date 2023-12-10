#include "input.h"
#include "init.h"
#include "../cthash.h"
#include "memory.h"

input* g_input = new input();

struct Bits
{
	WORD nRepeatCount : 16;
	BYTE nScanCode : 8;
	BYTE nExtended : 1;
	BYTE nReserved : 4;
	BYTE nContext : 1;
	BYTE nPrevious : 1;
	BYTE nTransition : 1;
};

#define GET_X_LPARAM(lp) (int(short(LOWORD(lp))))
#define GET_Y_LPARAM(lp) (int(short(HIWORD(lp))))

void input::run()
{
	POINT p;
	if ( !imp( "user32.dll", GetCursorPos )( &p ) )
		return;

	if ( !init::window )
	{
		_( Valve001, "Valve001" );
		init::window = imp( "user32.dll", FindWindowA )( Valve001.c_str(), nullptr );
	}

	imp( "user32.dll", ScreenToClient )( init::window, &p );

	m_mouse_info.pos = { ( float ) p.x, ( float ) p.y };
}

mouse_info_t& input::get_mouse()
{
	return m_mouse_info;
}

key_info_t input::get_key( const int key ) const
{
	return m_key_info.at( key );
}

wchar_t input::get_last_char() const
{
	return m_last_char;
}

void input::clear_char()
{
	m_last_char = '\0';
}

bool input::run_mouse( const UINT message )
{
	switch ( message )
	{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if ( m_mouse_info.state == idle )
				m_mouse_info.state = pressed;
			return true;
		case WM_LBUTTONUP:
			m_mouse_info.state = idle;
			return true;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
			if ( m_mouse_info.state_right == idle )
				m_mouse_info.state_right = pressed;
			return true;
		case WM_RBUTTONUP:
			m_mouse_info.state_right = idle;
			return true;
		case WM_KILLFOCUS:
			m_mouse_info.state = m_mouse_info.state_right = idle;
			return true;
		default:
			break;
	}

	if ( m_mouse_info.state == pressed )
		m_mouse_info.state = held;

	if ( m_mouse_info.state_right == pressed )
		m_mouse_info.state_right = held;

	return message == WM_MOUSEMOVE || message == WM_NCMOUSEMOVE;
}

bool input::run_keyboard( const UINT message, const WPARAM param )
{
	switch ( message )
	{
		case WM_KEYDOWN:
			if ( param >= 0 && param < 256 )
			{
				if ( m_key_info.at( param ).pressed )
					m_key_info.at( param ).held = true;
				m_key_info.at( param ).pressed = true;
			}
			return true;
		case WM_KEYUP:
			if ( param >= 0 && param < 256 )
			{
				m_key_info.at( param ).pressed = false;
				m_key_info.at( param ).held = false;
				m_key_info.at( param ).released = true;
			}
			return true;
		case WM_XBUTTONDOWN:
		case WM_XBUTTONDBLCLK:
			if ( GET_XBUTTON_WPARAM( param ) & XBUTTON1 )
				m_key_info.at( VK_XBUTTON1 ).pressed = true;
			else if ( GET_XBUTTON_WPARAM( param ) & XBUTTON2 )
				m_key_info.at( VK_XBUTTON2 ).pressed = true;
			return true;
		case WM_XBUTTONUP:
			if ( GET_XBUTTON_WPARAM( param ) & XBUTTON1 )
			{
				m_key_info.at( VK_XBUTTON1 ).pressed = false;
				m_key_info.at( VK_XBUTTON1 ).held = false;
				m_key_info.at( VK_XBUTTON1 ).released = true;
			}
			else if ( GET_XBUTTON_WPARAM( param ) & XBUTTON2 )
			{
				m_key_info.at( VK_XBUTTON2 ).pressed = false;
				m_key_info.at( VK_XBUTTON2 ).held = false;
				m_key_info.at( VK_XBUTTON2 ).released = true;
			}
			return true;
		case WM_SYSKEYDOWN:
			if ( param >= 0 && param < 256 )
				m_key_info.at( param ).pressed = true;
			return true;
		case WM_SYSKEYUP:
			if ( param >= 0 && param < 256 )
			{
				m_key_info.at( param ).pressed = false;
				m_key_info.at( param ).held = false;
				m_key_info.at( param ).released = true;
			}
			return true;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
			m_key_info.at( VK_MBUTTON ).pressed = true;
			return true;
		case WM_MBUTTONUP:
			m_key_info.at( VK_MBUTTON ).pressed = false;
			m_key_info.at( VK_MBUTTON ).held = false;
			m_key_info.at( VK_MBUTTON ).released = true;
			return true;
		case WM_LBUTTONDOWN:
			m_key_info.at( VK_LBUTTON ).pressed = true;
			return true;
		case WM_LBUTTONUP:
			m_key_info.at( VK_LBUTTON ).pressed = false;
			m_key_info.at( VK_LBUTTON ).held = false;
			m_key_info.at( VK_LBUTTON ).released = true;
			return true;
		case WM_RBUTTONDOWN:
			m_key_info.at( VK_RBUTTON ).pressed = true;
			return true;
		case WM_RBUTTONUP:
			m_key_info.at( VK_RBUTTON ).pressed = false;
			m_key_info.at( VK_RBUTTON ).held = false;
			m_key_info.at( VK_RBUTTON ).released = true;
			return true;
		case WM_MOUSEWHEEL:
			m_mouse_info.scroll = GET_WHEEL_DELTA_WPARAM( param );
			return true;
		case WM_KILLFOCUS:
			for ( auto i = 0; i < 256; i++ )
			{
				m_key_info.at( i ).pressed = false;
				m_key_info.at( i ).held = false;
				m_key_info.at( i ).released = true;
			}
			return true;
		default:;
	}

	return false;
}

bool input::mouse_in_bounds( const evo::ren::vec2& pos, const evo::ren::vec2& size ) const
{
	const auto info = m_mouse_info;

	return info.pos.x > pos.x && info.pos.x < pos.x + size.x && info.pos.y > pos.y && info.pos.y < pos.y + size.y;
}

std::string keys[] = { _w( "" ), _w( "M1" ), _w( "M2" ), _w( "BRK" ), _w( "M3" ), _w( "M4" ), _w( "M5" ),
_w( "UNK" ), _w( "BSPC" ), _w( "TAB" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "ENTER" ), _w( "UNK" ), _w( "UNK" ), _w( "SHFT" ),
_w( "CTRL" ), _w( "ALT" ), _w( "PAUSE" ), _w( "CAPS" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ),
_w( "ESC" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "SPACE" ), _w( "PGUP" ), _w( "PGDOWN" ), _w( "END" ), _w( "HOME" ), _w( "LFT" ),
_w( "UP" ), _w( "RGHT" ), _w( "DWN" ), _w( "UNK" ), _w( "PRNT" ), _w( "UNK" ), _w( "PRTSCR" ), _w( "INS" ), _w( "DEL" ), _w( "UNK" ), _w( "0" ), _w( "1" ),
_w( "2" ), _w( "3" ), _w( "4" ), _w( "5" ), _w( "6" ), _w( "7" ), _w( "8" ), _w( "9" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ),
_w( "UNK" ), _w( "A" ), _w( "B" ), _w( "C" ), _w( "D" ), _w( "E" ), _w( "F" ), _w( "G" ), _w( "H" ), _w( "I" ), _w( "J" ), _w( "K" ), _w( "L" ), _w( "M" ), _w( "N" ), _w( "O" ), _w( "P" ), _w( "Q" ), _w( "R" ), _w( "S" ), _w( "T" ), _w( "U" ),
_w( "V" ), _w( "W" ), _w( "X" ), _w( "Y" ), _w( "Z" ), _w( "LFTWIN" ), _w( "RGHTWIN" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "NUM0" ), _w( "NUM1" ),
_w( "NUM2" ), _w( "NUM3" ), _w( "NUM4" ), _w( "NUM5" ), _w( "NUM6" ), _w( "NUM7" ), _w( "NUM8" ), _w( "NUM9" ), _w( "*" ), _w( "+" ), _w( "UNK" ), _w( "-" ), _w( "." ), _w( "/" ), _w( "F1" ), _w( "F2" ), _w( "F3" ),
_w( "F4" ), _w( "F5" ), _w( "F6" ), _w( "F7" ), _w( "F8" ), _w( "F9" ), _w( "F10" ), _w( "F11" ), _w( "F12" ), _w( "F13" ), _w( "F14" ), _w( "F15" ), _w( "F16" ), _w( "F17" ), _w( "F18" ), _w( "F19" ), _w( "F20" ), _w( "F21" ),
_w( "F22" ), _w( "F23" ), _w( "F24" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ),
_w( "NUMLCK" ), _w( "SCRLLCK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ),
_w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "LSHFT" ), _w( "RSHFT" ), _w( "LCTRL" ),
_w( "RCTRL" ), _w( "LMENU" ), _w( "RMENU" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ),
_w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "NTRK" ), _w( "PTRK" ), _w( "STOP" ), _w( "PLAY" ), _w( "UNK" ), _w( "UNK" ),
_w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( ";" ), _w( "+" ), _w( ")," ), _w( "-" ), _w( "." ), _w( "?" ), _w( "~" ), _w( "UNK" ), _w( "UNK" ),
_w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ),
_w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ),
_w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ),
_w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ),
_w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ),
_w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ), _w( "UNK" ),
_w( "UNK" ), _w( "UNK" ) };

std::string input::get_key_string( const int i )
{
	_( dot, "..." );

	if ( i < 0 || i > 256 )
		return dot;

	return keys[ i ];
}

void input::reset_mouse()
{
	m_mouse_info.state = idle;
}
