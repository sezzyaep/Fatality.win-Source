#pragma once

struct SndInfo_t;
namespace sound_esp
{
	void update();
	void update_origin( sound_data_t& player, const Vector& origin );
	Vector get_position( int index, bool lerp = false );
	bool get_last_update( int index );
	sound_data_t* get_sound_player( int index );
	void on_start_sound( const start_sound_params* params, char* name );
	void on_item_equip( IGameEvent* game_event );

	inline std::array<sound_data_t, 65> players = {};
};