#pragma once
#include <memory>
#include <vector>

#include "../../sdk/datatypes/Vector2D.h"

class window;
class child;
class sub_tab;

class tab : public std::enable_shared_from_this<tab>
{
public:
	tab( std::string name, std::vector<std::shared_ptr<sub_tab>> sub_tabs = {}, bool noarea = false );

	void run_render( Vector2D& pos );
	void run_input();
	void reset_state();

	std::shared_ptr<window> get_parent() const;

	void init( std::shared_ptr<window> window, int id );

	sub_tab* add_subtab( const char* name );

	bool has_sub_tabs() const;
	int get_active_sub_tab() const;
	void set_active_sub_tab( int id );
	void run_hotkey_input();

	std::string& get_name() { return m_name; }

	std::vector<std::shared_ptr<sub_tab>>& get_subtabs();
	bool m_noarea = {};
private:
	void handle_line();
	void handle_hover();

	std::string m_name;

	float m_line_size = 1.f;
	float m_hover_progress = {};

	bool m_hovered = {};
	bool m_active = {};

	evo::ren::vec2 m_pos = {};
	evo::ren::vec2 m_size = {};
	evo::ren::vec2 m_string_size = {};

	int m_id = {};
	int m_active_sub_tab = 0;
	int m_last_active = {};

	std::shared_ptr<window> m_parent = {};
	std::vector<std::shared_ptr<sub_tab>> m_subtabs = {};
};
