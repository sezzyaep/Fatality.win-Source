#include "../include_cheat.h"
#include <intrin.h>

void __fastcall hook::estimate_abs_velocity( C_CSPlayer* player, int edx, Vector* out )
{
			// grab return address from stack frame.
	uintptr_t* _ebp;
	__asm mov _ebp, ebp;
	auto& ret_addr = _ebp[ 1 ];

	// game tried to setup velocity for something irrelevant.
	if ( ret_addr != make_offset( "client.dll", sig_ret_to_setup_velocity ) || !player->is_player() || vars::legit_enabled() && player->EntIndex() != interfaces::engine()->GetLocalPlayer() )
		return original( estimate_abs_velocity )( player, edx, out );

	// write 'real' velocity to output.
	Vector velocity{};
	if ( player->EntIndex() == interfaces::engine()->GetLocalPlayer() && false )
	{
		original( estimate_abs_velocity )( player, edx, out );
		velocity = *out;
	}
	else
	{
		velocity = player->get_velocity();
		*out = velocity;
	}

	// skip velocity clamping shit.
	ret_addr += 0x101;

	// emulate volatile registers between the two addresses.
	__asm
	{
		movss xmm1, velocity.z
		movss xmm3, velocity.y
		movss xmm4, velocity.x
	}
}