#pragma once

namespace animations
{
	struct anim_state_info_t
	{
		CBaseHandle	handle;
		float spawn_time;
		CCSGOPlayerAnimState* animstate;
		float animvel;
		std::array<float, 24> poses;
	};

	float get_yaw_modifier( C_CSPlayer* player, const Vector& velocity, float duckamt, const CCSGOPlayerAnimState* state );
	void calculate_sim_ticks( lag_record_t* record, C_CSPlayer* player, lag_record_t* previous );
	void update_player_animations( lag_record_t* record, C_CSPlayer* player, lag_record_t* previous );
	bool local_bone_setup( lag_record_t& record, bool ignore_check = false );
	void move_matrix( matrix3x4_t* matrix, Vector& orig_pos, Vector new_pos );
	Vector get_headpos();
	void build_fake_animation();
	void play_additional_animations( CUserCmd* cmd, const CCSGOPlayerAnimState& pred_state, lag_record_t* record, int seed = time_to_ticks( interfaces::globals()->curtime ) );
	std::pair<CCSGOPlayerAnimState, std::array<C_AnimationLayer, 13>> predict_animation_state( C_CSPlayer* player );
	void predict_animation_state( lag_record_t& record, C_CSPlayer* player, int times, bool force_angle = false );
	void update_animations( lag_record_t* record, CUserCmd* cmd = nullptr, bool tickbase_drift = false );
	void restore_animation( int32_t sequence );
	void merge_local_animation( CUserCmd* cmd, C_CSPlayer* player );
	void update_addon_bits( C_CSPlayer* player );
	void replace_server_model( bool start );

	inline lag_record_t fake_record = {};
	inline lag_record_t local_record = {};
	inline lag_record_t hittable_record = {};
	inline std::pair<int32_t, int32_t> lag = {};
	inline std::pair<int32_t, int32_t> most_recent = {};

	struct animation_copy
	{
		animation_copy() = default;

		animation_copy( int32_t seq )
		{
			if ( local_player->get_anim_state()->last_update <= 0.f )
				return;

			sequence = seq;
			checksum = interfaces::input()->m_pVerifiedCommands[ sequence % 150 ].m_crc;
			state = *local_player->get_anim_state();
			addon = local_record.addon;
			client_layers = layers = local_player->get_anim_layers();
			poses = local_player->get_pose_params();
			lower_body_yaw_target = local_player->get_lby();
			is_strafing = local_player->get_strafing();
			eye_angles = *local_player->eye_angles_virtual();
		}

		void restore( C_CSPlayer* player ) const
		{
			const auto bak = *player->get_anim_state();
			*player->get_anim_state() = state;
			bak.copy_meta( player->get_anim_state() );
			local_record.addon = addon;
			player->get_anim_layers() = layers;
			player->get_pose_params() = poses;
			player->get_lby() = lower_body_yaw_target;
		}

		int32_t sequence = INT_MAX;
		int32_t checksum = INT_MAX;
		CCSGOPlayerAnimState state{};
		addons_t addon{};
		std::array<C_AnimationLayer, 13> layers{};
		std::array<C_AnimationLayer, 13> client_layers{};
		std::array<float, 24> poses{};
		float lower_body_yaw_target{};
		int32_t remaining_choke{};
		bool is_strafing{};
		QAngle eye_angles{};
	};
};
