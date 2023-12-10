#include "../menu_include.h"

using namespace evo::ren;

void glitch_text::run_render( std::string text, const vec2& pos, uint32_t font )
{
	static auto last_frame = interfaces::globals()->framecount;

	const auto calculate_glitch = [] ( float& factor ) -> void
	{
		/*
		 * Don't do our calculations multiple times per frame!
		 */
		if ( last_frame == interfaces::globals()->framecount )
			return;

		/*
		* Value will reach 1 within 200ms
		*/
		static auto should_add = false;

		VAL_SMOOTHER( factor, -1.f, 1.f, should_add, 1.f / 0.2f );

		if ( factor <= -1.f || factor >= 1.f )
			should_add = !should_add;

		last_frame = interfaces::globals()->framecount;
	};

	static auto factor = -1.f;

	calculate_glitch( factor );

	auto& d = draw.get_layer( layer::foreground );
	/*
	 * Top text ( blue )
	 */
	d.add_text( draw.get_font( font ), { pos.x + factor * 2, pos.y - 1 * std::abs( factor ) }, text, gui_color::glitch_blue() );

	/*
	 * Bottom text ( red )
	 */
	d.add_text( draw.get_font( font ), { pos.x + -factor * 2, pos.y + 1 * std::abs( factor ) }, text, gui_color::glitch_red() );
}
