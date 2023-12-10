#pragma once

namespace lagcomp
{
	bool run_extrapolation( const C_CSPlayer* player, const bool simple = false );
	int fix_tickcount( const float& simtime );
	bool valid_simtime( const float& simtime, const float time = ticks_to_time( local_player->get_tickbase() - 1 ) );
	bool is_breaking_lagcomp( const C_CSPlayer* player );
	bool is_breaking_lagcomp( const C_CSPlayer* player, const lag_record_t* current );
	int get_real_lag( const C_CSPlayer* player, const lag_record_t* current );
	float get_lerp_time();
	void extrapolate( C_CSPlayer* player, Vector& origin, Vector& velocity, Vector base_velocity, int& flags, bool wasonground );
};
