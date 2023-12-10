#include "../include_cheat.h"
#include "intrin.h"

void __fastcall hook::trace_ray( void* thisptr, void* edx, Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace )
{
	if ( reinterpret_cast< uintptr_t >( _ReturnAddress() ) == make_offset( "client.dll", sig_ret_to_trace_ray ) )
	{
		if ( fMask == 0x200400B )
		{
			CTraceFilterSimpleNoPlayers new_filter( local_player, 0 );
			if ( local_player && antiaim::in_fakeduck() )
				ray.m_Start.z = local_player->get_abs_origin().z + 64.f;

			return original( trace_ray )( thisptr, edx, ray, fMask, &new_filter, pTrace );
		}

		if ( pTraceFilter->pSkip == local_player )
		{
			CTraceFilterSimpleNoTeammates new_filter( local_player, 0 );
			return original( trace_ray )( thisptr, edx, ray, fMask, &new_filter, pTrace );
		}
	}

	return original( trace_ray )( thisptr, edx, ray, fMask, pTraceFilter, pTrace );
}

void __stdcall hook::override_view( CViewSetup* setup )
{
	if ( local_player && antiaim::in_fakeduck() && local_player->get_alive() && interfaces::input()->m_fCameraInThirdPerson )
		setup->origin.z = local_player->get_abs_origin().z + 64.f;

	original( override_view )( setup );
}