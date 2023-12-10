#include "../../include_cheat.h"

bool Vector::to_screen()
{
	static auto out = Vector();
	const auto ret = math::world_to_screen( *this, out );
	*this = out;
	return ret;
}
