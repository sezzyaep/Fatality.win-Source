#pragma once

class ClientClass;
class IBaseClientDll
{
public:
	VFUNC( 8, GetAllClasses(), ClientClass*( __thiscall* )( void* ) )( )
	VFUNC( 20, IN_KeyEvent( int eventcode, int keynum, const char *pszCurrentBinding ), void( __thiscall* )( void*, int, int, const char* ) )( eventcode, keynum, pszCurrentBinding )
		VFUNC( 21, CreateMove( int sequence_number, float frametime, bool active ), void( __thiscall* )( void*, int, float, bool ) )( sequence_number, frametime, active )
		VFUNC( 24, WriteUsercmdDeltaToBuffer( int nSlot, void* buf, int from, int to, bool isNewCmd ), bool( __thiscall* )( void*, int, void*, int, int, bool ) )( nSlot, buf, from, to, isNewCmd )
};
