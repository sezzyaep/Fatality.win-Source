#pragma once

#include "../sdk/datatypes/Vector2D.h"
#include <Windows.h>
#include <array>
#include <string>

#include "ren/types.h"


enum e_mouse_state
{
	idle,
	pressed,
	held
};

struct mouse_info_t
{
	void reset_scroll()
	{
		scroll = 0;
	}

	e_mouse_state state;
	e_mouse_state state_right;
	int scroll;

	evo::ren::vec2 pos;
};

struct key_info_t
{
	bool pressed;
	bool held;
	bool released;
};

class input
{
public:
	bool run_mouse( const UINT message );
	bool run_keyboard( const UINT message, const WPARAM param );

	static std::string get_key_string( const int i );

	void reset_mouse();
	bool mouse_in_bounds( const evo::ren::vec2& pos, const evo::ren::vec2& size ) const;

	void run();
	mouse_info_t& get_mouse();
	key_info_t get_key( int key ) const;

	wchar_t get_last_char() const;
	void clear_char();
private:
	mouse_info_t m_mouse_info;
public:
	std::array<key_info_t, 256> m_key_info;
public:
	wchar_t m_last_char;
};

extern input* g_input;
