#include "../include_cheat.h"
#include "intrin.h"

IMaterial* __fastcall hook::find_material( void* thisptr, void* edx, const char* material_name, const char* texture_group, bool complain, const char* complain_prefix )
{
	if ( !vars::misc.remove_blur->get<bool>() )
		return original( find_material )( thisptr, edx, material_name, texture_group, complain, complain_prefix );

	if ( material_name[ 0 ] == 'd' && material_name[ 4 ] == 's' && material_name[ 10 ] == 'b' )
	{
		_( str, "dev/clearalpha" );
		static auto clear_alpha = original( find_material )( thisptr, edx, str.c_str(), nullptr, complain, complain_prefix );
		return clear_alpha;
	}

	return original( find_material )( thisptr, edx, material_name, texture_group, complain, complain_prefix );
}