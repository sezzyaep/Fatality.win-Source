#include "../include_cheat.h"


void __fastcall hook::level_shutdown( void* ecx, void* edx )
{
	skinchanger::on_level_shudown();
	
	return original( level_shutdown )( ecx, edx );
}
