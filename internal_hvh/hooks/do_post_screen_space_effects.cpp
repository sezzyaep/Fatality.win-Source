#include "../include_cheat.h"

bool _fastcall hook::do_post_screen_space_effects( void* ecx, void* edx, CViewSetup* setup )
{
	glow::draw();

	lua::api.callback( FNV1A( "on_do_post_screen_space_effects" ) );

	chams::in_glow = true;
	const auto ret = original( do_post_screen_space_effects )( ecx, edx, setup );
	chams::in_glow = false;

	glow::restore();

	return ret;
}