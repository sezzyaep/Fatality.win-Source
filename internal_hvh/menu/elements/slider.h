#pragma once
#include <string>

namespace slider_style
{
	enum flags
	{
		none = 1 << 0,
		no_operators = 1 << 1,
		no_focus = 1 << 2
	};
}

class slider : public control
{
public:
	slider( std::string name, value* var, float min, float max, uint8_t flags = slider_style::flags::none, std::string format = "%.f", float step = 1.f );

	void run_render( evo::ren::vec2& pos ) override;
	void run_input() override;
protected:
	std::string get_value_text() const;

	void calc_fraction();

	void render_operators() const;
	void handle_operators() const;
	evo::ren::vec2 m_operator_pos{};

	float m_fraction;
	float m_min;
	float m_max;
	float m_step;

	bool m_dragging;
	bool m_in_slider_area;

	std::string m_format;

	uint8_t m_flags;
};


