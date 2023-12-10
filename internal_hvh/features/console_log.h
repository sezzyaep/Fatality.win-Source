#pragma once

namespace console_log
{

	void buy_event( IGameEvent* game_event );
	void hurt_event( IGameEvent* game_event, record_shot_info_t& shot_info );
	void listener( IGameEvent * game_event, record_shot_info_t& shot_info );

	inline bool m_in_print = false;
};
