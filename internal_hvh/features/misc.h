#pragma once

#define sv_maxusrcmdprocessticks (((*interfaces::game_rules_proxy()) && (*interfaces::game_rules_proxy())->m_bIsValveDS()) ? 7 : 16)
#define max_new_cmds (sv_maxusrcmdprocessticks - 2)

namespace misc
{
	void bhop( bool post_prediction = false );
	void remove_smoke( ClientFrameStage_t stage );
	void remove_flash( ClientFrameStage_t stage );
	void reveal_ranks();
	void clan_tag();
	void play_freezetime_music();
	void preserve_deathnotices( ClientFrameStage_t stage );
	void buybot();
	void write_tick( int cmdnum );
	void custom_render_options( ClientFrameStage_t stage );
	void peek_assist();
	void ragdoll_slowpath( ClientFrameStage_t stage );
	void fix_prediction( ClientFrameStage_t stage );
	float get_latency();

	inline bool retract_peek = {};
	inline bool cant_reach = {};
	inline bool has_position = {};
	inline bool has_released = {};
	inline Vector movement_position{};
	inline Vector peek_position{};
	inline int last_recv = {};

	inline void retract()
	{
		retract_peek = true;
		peek_position = local_player->get_origin();
		cant_reach = false;
	}
};
