#pragma once
#include <memory>
#include <vector>

class tab;
class child;

class window
{
public:
	window( std::string name, evo::ren::vec2 size, evo::ren::vec2 pos = evo::ren::vec2( 0, 0 ), window_style::flags flags = window_style::flags::none, std::vector<std::shared_ptr<tab>> tabs = {} );
	~window() = default;

	void reset_state() const;
	void run_render();
	void run_input();
	void run_hotkey_input();

	evo::ren::vec2 get_size() const;
	evo::ren::vec2 get_pos() const;

	int get_tab_amount() const;
	void set_active_tab( int id );
	int get_active_tab() const;

	evo::ren::vec2 get_control_area_start() const;
	evo::ren::vec2 get_control_area_size() const;

	std::vector<std::shared_ptr<tab>>& get_tabs();

	std::shared_ptr<child> config_child;

private:
	void border() const;
	void header() const;
	void glow() const;

	std::string m_name;

	int m_active_tab;
	int m_old_active_tab{};

	evo::ren::vec2 m_drag_offset{};

	evo::ren::vec2 m_pos{};
	evo::ren::vec2 m_size{};
	window_style::flags m_flags;

	std::vector<std::shared_ptr<tab>> m_tabs;
};
