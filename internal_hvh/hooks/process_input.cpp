#include "../include_cheat.h"
#include <intrin.h>

void __fastcall hook::lock_cursor( void* ecx, void* edx )
{
	if ( g_menu->is_open() )
		return interfaces::surface()->UnlockCursor();

	original( lock_cursor )( ecx, edx );
}

void __fastcall hook::draw_set_color( void* ecx, void* edx, int r, int g, int b, int a )
{
	const auto return_address = reinterpret_cast< uintptr_t >( _ReturnAddress() );

	if ( vars::misc.remove_scope->get<int>() == 2
		&& ( return_address == make_offset( "client.dll", sig_ret_to_scope_arc )
		|| return_address == make_offset( "client.dll", sig_ret_to_scope_lens ) ) )
	{
		// We don't want this to draw, so we set the alpha to 0
		return original( draw_set_color )( ecx, edx, r, g, b, 0 );
	}

	original( draw_set_color )( ecx, edx, r, g, b, a );
}

int __fastcall hook::hud_reticle_process_input( void* ecx, void* edx )
{
	QAngle viewangles;
	interfaces::engine()->GetViewAngles( viewangles );
	interfaces::prediction()->SetLocalViewAngles( viewangles );

	return original( hud_reticle_process_input )( ecx, edx );
}