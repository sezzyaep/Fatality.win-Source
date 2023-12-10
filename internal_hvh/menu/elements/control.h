#pragma once
#include "../../misc/config.h"
#include <ren/misc.h>

struct control_dependency_t
{
	control_dependency_t( value* val, const double required, const bool flip = false ) :m_val( val ), m_required( required ), m_valid( true ), m_flip( flip ) {}
	control_dependency_t() : m_val( nullptr ), m_required( 0 ), m_valid( false ), m_flip( false ) {}

	value* m_val;
	double m_required;
	bool m_valid;
	bool m_flip;
};

class sub_control;

class control : public std::enable_shared_from_this<control>
{
public:
	control() = default;
	virtual ~control() = default;

	virtual void run_render( evo::ren::vec2& pos ) = 0;
	virtual void run_input() = 0;

	virtual void reset_state();
	virtual bool is_listbox()
	{
		return false;
	}

	virtual bool is_combo()
	{
		return false;
	}

	virtual bool can_add_subcontrols()
	{
		return false;
	}

	bool is_active() const
	{
		return !m_deactivated;
	}

	void deactivate()
	{
		m_deactivated = true;
	}

	void activate()
	{
		m_deactivated = false;
	}

	void run_hotkey_input();

	static bool is_hovered( const evo::ren::vec2& pos, const evo::ren::vec2& size )
	{
		const auto info = g_input->get_mouse();

		return info.pos.x > pos.x && info.pos.x < pos.x + size.x && info.pos.y > pos.y && info.pos.y < pos.y + size.y;
	}

	void init( const std::shared_ptr<child> child )
	{
		m_parent = child;
	}

	void calc_string_size()
	{
		if ( m_name.empty() )
		{
			if ( m_string_size.is_zero() )
				m_string_size = evo::ren::text_size( evo::ren::draw.get_font( FNV1A( "control" ) ), m_name ) + 3.f;
			return;
		}

		_u( name, m_name );
		if ( m_string_size.is_zero() )
			m_string_size = evo::ren::text_size( evo::ren::draw.get_font( FNV1A( "control" ) ), name ) + 3.f;
	}

	evo::ren::vec2 get_string_size() const
	{
		return m_string_size;
	}

	void handle_hover_progress()
	{
		VAL_SMOOTHER( m_hover_progress, 0.f, 1.f, m_hovered, 1.f / 0.25f );
	}

	void handle_active_progress( bool active )
	{
		VAL_SMOOTHER( m_active_progress, 0.f, 1.f, active, 1.f / 0.35f );
	}

	std::shared_ptr<child> get_parent() const
	{
		return m_parent;
	}

	std::vector<std::shared_ptr<sub_control>>& get_controls()
	{
		return m_controls;
	}

	std::shared_ptr<control> add_control( const std::shared_ptr<sub_control>& control_to_add )
	{
		m_controls.push_back( control_to_add );
		return shared_from_this();
	}

	std::shared_ptr<control> add_dependency( const control_dependency_t dependency )
	{
		m_dependencies.push_back( dependency );
		return shared_from_this();
	}

	std::shared_ptr<control> add_manual_padding()
	{
		m_manual_padding = true;
		return shared_from_this();
	}

	void set_size( const evo::ren::vec2& size )
	{
		m_size = size;
	}

	evo::ren::vec2 get_size() const
	{
		return m_size;
	}

	float get_string_size_multiplier() const
	{
		return m_string_size_multiplier;
	}

	bool should_render() const;

	void set_focussed_control( sub_control* sub )
	{
		m_focussed_control = sub;
	}

	sub_control* get_focussed_control() const
	{
		return m_focussed_control;
	}

	evo::ren::vec2 get_pos() const
	{
		return m_pos;
	}

	std::string& get_name()
	{
		return m_name;
	}

	value* get_var()
	{
		return m_var;
	}

	void set_visibility( bool visible = true )
	{
		m_force_hide = !visible;
	}

	void init_subs();

	void render_sub_controls();
protected:
	std::shared_ptr<child> m_parent;

	std::string m_name;

	bool m_hovered = false;
	bool m_did_init_subs = false;
	bool m_manual_padding = false;
	bool m_force_hide = false;

	bool m_deactivated = false;

	std::vector<control_dependency_t> m_dependencies;

	evo::ren::vec2 m_pos = evo::ren::vec2( 0, 0 );
	evo::ren::vec2 m_size = evo::ren::vec2( 0, 0 );
	evo::ren::vec2 m_string_size = evo::ren::vec2( 0, 0 );

	float m_active_progress = 0.f;
	float m_hover_progress = 0.f;
	float m_string_size_multiplier = 1.f;

	sub_control* m_focussed_control = nullptr;
	std::vector<std::shared_ptr<sub_control>> m_controls;

	value* m_var = nullptr;
};
