#include "../include_cheat.h"
#include <intrin.h>

int	__stdcall hook::is_box_visible( const Vector& mins, const Vector& maxs )
{
	if ( reinterpret_cast< uintptr_t >( _ReturnAddress() ) == make_offset( "client.dll", sig_ret_is_box_visible ) )
		return 1;

	return original( is_box_visible )( mins, maxs );
}