#pragma once
#include <string>
#include <vector>

#include "../../sdk/datatypes/Vector2D.h"

class tab;
class control;
class sub_tab;

class child : public std::enable_shared_from_this<child>
{
public:
	child( std::string name, evo::ren::vec2 offset, evo::ren::vec2 size, std::vector<std::shared_ptr<control>> controls = {}, std::shared_ptr<window> window_parent = nullptr );

	void run_render();
	void run_input();
	void reset_state();

	void init( std::shared_ptr<sub_tab> tab );

	evo::ren::vec2 get_size() const;
	evo::ren::vec2 get_pos() const;

	std::string& get_name();

	std::vector<std::shared_ptr<control>>& get_controls();

	std::shared_ptr<sub_tab> get_parent() const;
	void run_hotkey_input();
	std::shared_ptr<control> add_control( std::shared_ptr<control> control );
	void remove_control( std::shared_ptr<control> control );
private:
	std::string m_name;

	void handle_scroll();
	void render_scroll() const;

	evo::ren::vec2 m_pos{};
	evo::ren::vec2 m_offset{};
	evo::ren::vec2 m_size = {};

	int m_scroll_amount {};
	float m_max_scroll_size {};

	evo::ren::vec2 m_string_size;

	std::shared_ptr<window>m_window_parent;
	std::shared_ptr<sub_tab> m_parent;
	std::vector<std::shared_ptr<control>> m_controls;
};