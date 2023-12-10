#include "../include_cheat.h"
#include <unordered_map>

namespace globals
{
	float random_float( float from, float to ) { return reinterpret_cast< float( * ) ( float, float ) >( make_offset( "vstdlib.dll", sig_random_float ) )( from, to ); }
	int random_int( int from, int to ) { return reinterpret_cast< int( * ) ( int, int ) >( make_offset( "vstdlib.dll", sig_random_int ) )( from, to ); }
	void random_seed( int seed ) { reinterpret_cast< void( * ) ( int ) >( make_offset( "vstdlib.dll", sig_random_seed ) )( seed ); }

	C_CSPlayer* get_player( const int index ) { return reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity( index ) ); }
	C_WeaponCSBaseGun* get_weapon( const CBaseHandle handle ) { if ( !handle ) { return nullptr; } return reinterpret_cast< C_WeaponCSBaseGun* >( interfaces::entity_list()->get_client_entity_from_handle( handle ) ); }

	secure_info_t secure_info = {};

	bool is_dangerzone()
	{
		var( game_mode );
		var( game_type );
		return game_type->get_int() == 6 && game_mode->get_int() == 0;
	}

	void util_trace_line( const Vector& vec_start, const Vector& vec_end, const unsigned n_mask, const C_BaseEntity* ignore, trace_t* p_trace, int collision_group )
	{
		Ray_t ray;
		ray.Init( vec_start, vec_end );

		CTraceFilterSimple filter( ignore, nullptr, collision_group );
		interfaces::engine_trace()->TraceRay( ray, n_mask, &filter, p_trace );
	}

	bool line_goes_through_smoke( const Vector& vec_start, const Vector& vec_end )
	{
		return reinterpret_cast< bool( __cdecl* ) ( Vector, Vector, uint16_t ) >( make_offset( "client.dll", sig_line_goes_through_smoke ) )( vec_start, vec_end, 1 );
	}

	void util_trace_hull( const Vector& vec_start, const Vector& vec_end, const Vector& extents, const unsigned n_mask, const C_BaseEntity* ignore, trace_t* p_trace, int collision_group )
	{
		Ray_t ray;
		ray.Init( vec_start, vec_end, extents * -1.f, extents );

		CTraceFilterSimple filter( ignore, nullptr, collision_group );
		interfaces::engine_trace()->TraceRay( ray, n_mask, &filter, p_trace );
	}

	void util_clip_trace_to_player( const Vector& vec_start, const Vector& vec_end, const unsigned int n_mask, C_BaseEntity* target, trace_t* p_trace )
	{
		if ( !target )
			return;

		trace_t player_trace;
		Ray_t ray;
		ray.Init( vec_start, vec_end );

		const auto smallest_fraction = p_trace->fraction;
		interfaces::engine_trace()->ClipRayToEntity( ray, n_mask, target, &player_trace );

		if ( player_trace.fraction < smallest_fraction )
			*p_trace = player_trace;
	}
}