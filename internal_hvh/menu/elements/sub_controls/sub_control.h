#pragma once

class sub_control : public std::enable_shared_from_this<sub_control>
{
public:
	virtual ~sub_control() = default;

	virtual void run_render( evo::ren::vec2 pos, evo::ren::vec2 size ) = 0;
	virtual void run_input() = 0;
	virtual void run_hotkey_input() {}
	virtual void reset_state() {}
	virtual bool is_colorpicker() { return false; }
	virtual bool is_keybind() { return false; }

	static bool is_hovered( const evo::ren::vec2& pos, const evo::ren::vec2& size )
	{
		const auto info = g_input->get_mouse();

		return info.pos.x > pos.x && info.pos.x < pos.x + size.x && info.pos.y > pos.y && info.pos.y < pos.y + size.y;
	}

	void handle_hover_progress()
	{
		VAL_SMOOTHER( m_hover_progress, 0.f, 1.f, m_hovered, 1.f / 0.25f );
	}

	void handle_active_progress( bool active )
	{
		VAL_SMOOTHER( m_active_progress, 0.f, 1.f, active, 1.f / 0.35f );
	}

	evo::ren::vec2 get_size() const
	{
		return m_size;
	}

	evo::ren::vec2 get_pos() const
	{
		return m_pos;
	}

	float get_max_size() const
	{
		return m_max_size;
	}

	void set_parent( std::shared_ptr<control> parent )
	{
		m_parent = parent;
	}

	bool should_render() const
	{
		if ( m_dependencies.empty() )
			return true;

		auto ret = false;

		for ( auto& dependency : m_dependencies )
		{
			if ( !dependency.m_val )
				return true;//!interfaces::engine()->IsInGame() || !( *interfaces::game_rules_proxy() )->m_bIsValveDS();

			if ( !dependency.m_flip && dependency.m_val->get<double>() == dependency.m_required )
				ret = true;

			if ( dependency.m_flip && dependency.m_val->get<double>() != dependency.m_required )
				ret = true;
		}

		return ret;
	}

	std::shared_ptr<sub_control> add_dependency( const control_dependency_t dependency )
	{
		m_dependencies.push_back( dependency );
		return shared_from_this();
	}

protected:
	bool m_hovered = false;

	evo::ren::vec2 m_pos = {};
	evo::ren::vec2 m_size = {};

	std::shared_ptr<control> m_parent;

	float m_active_progress = 0.f;
	float m_hover_progress = 0.f;
	float m_max_size = 30.f;

public:
	bool m_in_force_off = false;

protected:
	std::vector<control_dependency_t> m_dependencies;

	value* m_var = nullptr;
	int m_temp_value = 1;
	bool m_last_key_state = false;
};

