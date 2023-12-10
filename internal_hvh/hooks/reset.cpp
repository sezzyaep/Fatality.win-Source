#include "../include_cheat.h"
#include <ren/adapter.h>

using namespace evo::ren;

long __stdcall hook::reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp )
{
	if ( !adapter )
		return original( reset )( device, pp );


	adapter->destroy_objects();

	const auto hr = original( reset )( device, pp );

	adapter->create_objects();

	return hr;
}
