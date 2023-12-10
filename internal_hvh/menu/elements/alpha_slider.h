#pragma once


class alpha_slider : public slider
{
public:
	alpha_slider( std::string name, value* var, value* color, float min, float max, uint8_t flags = slider_style::flags::none, std::string format = "%.f", float step = 1.f ) : slider( name, var, min, max, flags, format, step ), m_color( color ) {}

	void run_render( evo::ren::vec2& pos ) override;
private:
	value* m_color;
};