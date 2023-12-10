#include "../include_cheat.h"
#include <intrin.h>

bool __fastcall hook::is_connected( void* ecx, void* edx )
{
	if ( reinterpret_cast< uintptr_t >( _ReturnAddress() ) == make_offset( "client.dll", sig_ret_to_enable_inventory ) ) // enable inventory
		return false;

	return original( is_connected )( ecx, edx );
}
