#pragma once
#include "sub_control.h"

class sub_control;
class value;

namespace keybind_style
{
	enum mode : int
	{
		always,
		hold,
		toggle
	};
}

class key_bind : public sub_control
{
public:
	key_bind( value* var, value* key );
	key_bind( value* key );

	void run_render( evo::ren::vec2 pos, evo::ren::vec2 size ) override;
	void run_input() override;
	bool is_keybind() override
	{
		return true;
	}
	void run_hotkey_input() override
	{
		handle_mode();
	}
	void reset_state() override
	{
		if ( m_parent && m_parent->get_focussed_control() == this )
		{
			m_parent->set_focussed_control( nullptr );

			if ( g_menu->get_focussed_control() )
				g_menu->set_focussed_control( nullptr );
		}

		m_hover_progress = 0.f;
		m_hovered = false;
		m_popup_open = false;
		m_waiting = false;
		m_held = false;
	}

	int get_key() const
	{
		return m_key->get<int>();
	}

	int get_mode() const
	{
		return m_mode->get<int>();
	}
private:
	void handle_mode();

	bool m_waiting;
	bool m_held;
	bool m_popup_open;

	evo::ren::vec2 m_popup_pos{};

	std::shared_ptr<combobox> m_popup;

	value* m_key;
	value* m_mode;
	value* m_tempval;
};
