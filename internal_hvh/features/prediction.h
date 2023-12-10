#pragma once
#include "animations.h"

namespace prediction
{
	struct pred_info_t
	{
		void reset()
		{
			*this = {};
		}
		int sequence = {};
		float m_curtime = {};
		int m_flags = {};
		int tick_base = {};
		int buttons = {};
		Vector origin = {};
		Vector velocity = {};
		bool has_anim_state = {};
		float throwtime = {};
		int movetype = {};
		float postpone_fire_ready_time = FLT_MAX;
		float viewoffset_z = {};
		float view_delta = {};
		float undpred_change_time = {};
		Vector maxs = {};
		CBaseHandle groundentity = {};
		CBaseHandle wpn = {};
		float last_shot = 0.f;
		CCSGOPlayerAnimState anim_state = {};
		animations::animation_copy animation = {};
		float collision_viewheight{};
		float collision_bounds_change_time{};
		int ev = {};

		struct tickbase_t
		{
			int limit = {};
			int sent_commands = {};
			int choked_commands = {};
			bool skip_fake_commands = {};
		} tickbase = {};
	};

	void start( int sequence_nr );
	void finish( bool sendpacket = false );
	void run_prediction( int command_number = interfaces::client_state()->lastoutgoingcommand + interfaces::client_state()->chokedcommands + 1 );
	pred_info_t& get_pred_info( int cmd );
	void clear_pred_info();
	void set_last_command_number( int num );
	void predict_post_pone_time( CUserCmd* cmd );
	void predict_can_fire( CUserCmd* cmd );

	void take_shot( bool shot = true );
	void take_secondary_shot( bool shot = true );
	bool has_shot();
	bool can_fire();
	void evaluate_shots();
	void reset_shot();
	void set_forced_shot();

	inline Vector2D unpred_move = {};
	inline bool computing = {};
	inline float unpred_throw_time = {};
	inline int orig_predicted = {};
	inline bool had_attack = {};
	inline bool had_secondary_attack = {};
	inline bool can_shoot = {};
	inline bool has_shot_this_cycle = {};
	inline float orig_curtime = {};
	inline float orig_frametime = {};
	inline bool attempted_jump = {};
	inline std::array<pred_info_t, 150> pred_info = {};
	inline int unpred_flags = {};
	inline Vector unpred_abs_origin = {};
	inline Vector unpred_vel = {};
	inline Vector initial_vel = {};
	inline int last_update_command_number = {};
	inline int prone_delay = {};

	inline static constexpr auto prone_time = 0.03448275862f;
	inline static constexpr auto post_delay = 0.2f;
};
