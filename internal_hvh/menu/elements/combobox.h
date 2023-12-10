#pragma once
#include <string>
#include <utility>

namespace combobox_style
{
	enum flags
	{
		none,
		pop_up,
		multi_select
	};
}

struct combo_item_t
{
	combo_item_t() = default;
	combo_item_t( std::string name, const int index ) : m_value( nullptr ), m_index( index ), m_name( std::move( name ) ) {}
	combo_item_t( std::string name, value* val ) : m_value( val ), m_index( -1 ), m_name( std::move( name ) ) {}
	combo_item_t( std::string name, std::string string ) : m_value( nullptr ), m_index( -1 ), m_name( std::move( name ) ), m_string( std::move( string ) ) {}

	value* m_value{};
	int m_index{};
	std::string m_name;
	std::string m_string;
};

class combobox : public control
{
public:
	combobox( const std::string& name, value* var, combobox_style::flags flags = combobox_style::flags::none, std::vector<combo_item_t> items = {} );

	void run_render( evo::ren::vec2& pos ) override;
	void run_input() override;
	void popup_input();
	bool is_combo() override
	{
		return true;
	}

	bool can_add_subcontrols() override
	{
		return true;
	}

	combobox* add_item( const std::string& name, value* var );

	void reset_state() override
	{
		if ( g_menu->get_focussed_control() && g_menu->get_focussed_control() == this )
			g_menu->set_focussed_control( nullptr );

		m_hover_progress = 0.f;
		m_hovered = false;
		m_open = false;

		for ( auto& control : m_controls )
			control->reset_state();
	}

	std::vector<combo_item_t>& get_items()
	{
		return m_items;
	}

	void set_items( const std::vector<combo_item_t>& new_items )
	{
		m_items = new_items;
	}

private:
	std::string get_display_name();
	void render_popup( const evo::ren::vec2& pos, const evo::ren::vec2& size );
	void render_scroll() const;

	void handle_scroll();

	evo::ren::vec2 m_popup_pos{};
	evo::ren::vec2 m_popup_size{};

	bool m_open{};
	bool m_cached_text{};

	int m_scroll_amount{};

	std::string m_display_text{};

	combobox_style::flags m_flags{};

	std::vector<combo_item_t> m_items{};
};
