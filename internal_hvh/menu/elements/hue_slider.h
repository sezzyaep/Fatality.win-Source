#pragma once

class hue_slider : public slider
{
public:
	hue_slider( std::string name, value* var, float min, float max, uint8_t flags = slider_style::flags::none, std::string format = "%.f", float step = 1.f ) : slider( name, var, min, max, flags, format, step ) {}

	void run_render( evo::ren::vec2& pos ) override;
};
