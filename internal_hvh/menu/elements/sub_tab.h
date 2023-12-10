#pragma once
#include <string>
#include <vector>

#include "../../sdk/datatypes/Vector2D.h"

class child;
class control;
class tab;
class weapon_tab;

class sub_tab
{
public:
	sub_tab( std::string name, std::vector<std::shared_ptr<child>> childs = {} );
	sub_tab( std::string name, std::vector<std::shared_ptr<sub_tab>> sub_tabs );

	virtual ~sub_tab() = default;

	void add_child( std::shared_ptr<child> child );
	virtual void run_render( evo::ren::vec2& pos );
	virtual void run_input();
	void run_hotkey_input();
	void handle_line();
	void handle_hover();
	void reset_state();

	void init( std::shared_ptr<tab> tab, int id );
	void init( std::shared_ptr<sub_tab> sub_tab, int id );

	evo::ren::vec2 get_pos() const { return m_pos; }
	std::vector<std::shared_ptr<child>>& get_children();
	std::vector<std::shared_ptr<sub_tab>>& get_subtabs();

	std::shared_ptr<tab> get_parent() const;
	std::shared_ptr<sub_tab> get_parent_sub_tab() const;

	int get_active_sub_tab() const
	{
		return m_active_sub_tab;
	}

	void set_active_sub_tab( const int id )
	{
		m_active_sub_tab = id;
	}

	std::string& get_name() { return m_name; }
protected:
	std::string m_name;

	bool m_hovered = false;
	bool m_active = false;

	float m_hover_progress = 0.f;
	float m_line_size = 1.f;

	evo::ren::vec2 m_pos = {};
	evo::ren::vec2 m_size = {};
	evo::ren::vec2 m_string_size = {};

	int m_active_sub_tab = 0;

	int m_id = {};

	std::shared_ptr<tab> m_parent = {};
	std::shared_ptr<sub_tab> m_parent_sub_tab = {};
	std::vector<std::shared_ptr<child>> m_childs = {};
	std::vector<std::shared_ptr<sub_tab>> m_sub_tabs = {};
};
