#pragma once
#include <Windows.h>
#include <memory>
#include <vector>

//#include "../misc/Vector2D.h"
//#include "../misc/Color.h"

enum e_menu_style
{
	window_padding,
	tab_area_size,
	control_inner_padding,
	control_padding,
	child_padding,
	tab_padding,
	sub_control_padding,
	popup_padding
};

namespace window_style
{
	enum mouse_mode
	{
		regular,
		color,
		color_hint
	};

	enum flags
	{
		none,
		rainbow_border,
	};
}

class control;
class window;
class tab;

class menu
{
public:
	menu();
	~menu() = default;

	void run();
	void run_input();
	void run_hotkey_input();
	void run_alpha();

	void toggle();
	bool is_open() const;

	float get_alpha() const;
	evo::ren::color get_main_color() const;

	void set_mouse_mode( window_style::mouse_mode mouse_mode );
	void set_mouse_box_color( evo::ren::color col );
	void set_focused_control_pos( const evo::ren::vec2& pos );

	//tab* add_window( const char* name, const Vector2D& pos, const Vector2D& size );

	evo::ren::vec2 get_style( e_menu_style style ) const;

	control* get_focussed_control() const;
	void set_focussed_control( control* control );
	void reset_focussed_control();

	void set_keyboard_control( control* access );
	control* get_keyboard_control();

	void reset();
	void set_focused_scissor( evo::ren::rect focused_scissor_record );
	std::vector<std::shared_ptr<tab>>& get_tabs();
private:
	bool m_init;
	void init();

	evo::ren::color m_color_box_mouse;
	window_style::mouse_mode m_mouse_mode;
	control* m_keyboard_control;
	void render_mouse() const;


	void render_background() const;

	control* m_focus_control;
	evo::ren::vec2 m_focused_control_pos{};
	evo::ren::rect m_focused_scissor_record;

	evo::ren::color m_main_color = evo::ren::color( 150, 55, 220 );

	float m_alpha;
	bool m_open;

public:
	std::vector<std::shared_ptr<window>> m_windows;
	std::shared_ptr<window> m_loading_window;
	std::mutex draw_mutex;
};

extern menu* g_menu;

class gui_color
{
public:
	static evo::ren::color custom( const Color col ) { return { col.r(), col.g(), col.b(), static_cast< int >( g_menu->get_alpha() * col.a() ) }; }
	static evo::ren::color custom( const evo::ren::color col ) { return { col.r(), col.g(), col.b(), static_cast< int >( g_menu->get_alpha() * col.a() ) }; }

	struct palette
	{
		static evo::ren::color main_pink( const int alpha = 255 ) { return { 235, 5, 90, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
		static evo::ren::color main_purple( const int alpha = 255 ) { return { 70, 50, 240, static_cast< int >( g_menu->get_alpha() * alpha ) }; }

		static evo::ren::color border( const int alpha = 255 ) { return { 60, 53, 93, static_cast< int >( g_menu->get_alpha() * alpha ) }; }

		static evo::ren::color black( const int alpha = 255 ) { return { 32, 34, 37, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
		static evo::ren::color dark_blue( const int alpha = 255 ) { return { 33, 27, 70, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
		static evo::ren::color matte_pink( const int alpha = 255 ) { return { 159, 39, 181, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
		static evo::ren::color hot_pink( const int alpha = 255 ) { return { 255, 0, 72, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
		static evo::ren::color bright_blue( const int alpha = 255 ) { return { 50, 51, 255, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
	};

	static evo::ren::color glitch_red( const int alpha = 255 ) { return { 255, 0, 72, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
	static evo::ren::color glitch_blue( const int alpha = 255 ) { return { 50, 52, 255, static_cast< int >( g_menu->get_alpha() * alpha ) }; }

	static evo::ren::color child_main( const int alpha = 255 ) { return { 35, 29, 72, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
	static evo::ren::color child_sub( const int alpha = 255 ) { return { 33, 27, 70, static_cast< int >( g_menu->get_alpha() * alpha ) }; }

	static evo::ren::color control_top( const int alpha = 255 ) { return { 30, 24, 67, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
	static evo::ren::color control_bot( const int alpha = 255 ) { return { 24, 16, 61, static_cast< int >( g_menu->get_alpha() * alpha ) }; }

	static evo::ren::color tab_active( const int alpha = 255 ) { return { 245, 245, 255, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
	static evo::ren::color tab_hovered( const int alpha = 255 ) { return { 166, 165, 200, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
	static evo::ren::color tab( const int alpha = 255 ) { return { 110, 113, 152, static_cast< int >( g_menu->get_alpha() * alpha ) }; }

	static evo::ren::color text_active( const int alpha = 255 ) { return { 245, 245, 245, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
	static evo::ren::color text( const int alpha = 255 ) { return { 166, 165, 200, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
	static evo::ren::color text_solid( const int alpha = 255 ) { return { 166, 165, 200, alpha }; }

	static evo::ren::color white( const int alpha = 255 ) { return { 255, 255, 255, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
	static evo::ren::color light_grey( const int alpha = 255 ) { return { 65, 65, 65, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
	static evo::ren::color dark_grey( const int alpha = 255 ) { return { 40, 40, 40, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
	static evo::ren::color light_black( const int alpha = 255 ) { return { 10, 10, 10, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
	static evo::ren::color black( const int alpha = 255 ) { return { 0, 0, 0, static_cast< int >( g_menu->get_alpha() * alpha ) }; }
};
