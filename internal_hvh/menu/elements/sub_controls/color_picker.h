#pragma once

namespace colorpicker_style
{
	enum flags : int
	{
		none = 1,
		no_rainbow = 2,
		no_alpha_bar = 4,
		open_left = 8
	};

	inline flags operator|( const flags a, const flags b )
	{
		return static_cast< flags >( static_cast< int >( a ) | static_cast< int >( b ) );
	}
}

class color_picker : public sub_control
{
public:
	color_picker( value* var, colorpicker_style::flags flags = colorpicker_style::flags::none );

	void run_render( evo::ren::vec2 pos, evo::ren::vec2 size ) override;
	void run_input() override;
	bool is_colorpicker() override { return true; }
	int get_flags() const { return m_flags; }
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
		m_holding_controls = false;
	}

	value* get_var() const { return m_var; }

	void update_color() const
	{
		const auto size = Vector2D( m_popup_size.x - g_menu->get_style( e_menu_style::child_padding ).x * 2 - 2, 200 - 2 );
		const auto hue = m_hue->get<float>() * 0.01f;
		const auto saturation = clamp( m_cursor.x / size.x, 0.f, 1.f );
		const auto brightness = clamp( 1.f - m_cursor.y / size.y, 0.f, 1.f );

		m_var->set( Color( Color::FromHSB( hue, saturation, brightness ), m_alpha->get<float>() / 100.f * 255.f ).GetD3DColor() );
	}
	void force_color()
	{
		const auto size = Vector2D( m_popup_size.x - g_menu->get_style( e_menu_style::child_padding ).x * 2 - 2, 200 - 2 );
		const auto col = Color( m_var->get<D3DCOLOR>(), 255 );
		m_cursor = { col.saturation() * size.x, ( 1.f - col.brightness() ) * size.y };
		m_alpha->set<float>( Color( m_var->get<D3DCOLOR>() ).aBase() * 100.f );
		m_hue->set<float>( col.hue() * 100.f );
	}
private:
	void render_close_button() const;
	void render_color_mouse_preview() const;
	void render_color_picker();

	void calc_popup_size();

	Color get_color();

	evo::ren::color m_color;
	evo::ren::color m_hovering_color;

	bool m_popup_open;
	bool m_needs_press;
	bool m_holding_controls;

	evo::ren::vec2 m_close_pos{};
	evo::ren::vec2 m_popup_pos{};
	evo::ren::vec2 m_popup_size;
	evo::ren::vec2 m_control_size{};

	evo::ren::vec2 m_cursor{};

	colorpicker_style::flags m_flags;

	value* m_hue;
	value* m_alpha;

	std::vector<std::shared_ptr<control>> m_sub_controls;

	static LPDIRECT3DTEXTURE9 m_picker_texture;
	static LPD3DXSPRITE ColorPickerSprite;
};
