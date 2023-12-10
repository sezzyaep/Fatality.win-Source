#pragma once

class CHud
{
public:
	MFUNC( FindElement( const char* name ), void* ( __thiscall* )( void*, const char* ), make_offset( "client.dll", sig_find_element ) )( name )
};
