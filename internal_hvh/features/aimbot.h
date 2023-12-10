#pragma once

namespace aimbot
{
	struct aimpoint_t
	{
		aimpoint_t() = default;
		aimpoint_t( const Vector point, const Vector center, const int hitbox, const int hitgroup, const float hitchance = -1.f )
		{
			this->point = point;
			this->center = center;
			this->hitgroup = hitgroup;
			this->hitbox = hitbox;
			this->hitchance = hitchance;
			this->damage = 0.f;
			this->no_pen_damage = 0.f;
			this->highest_damage = 0.f;
			this->player_health = 0.f;
			this->lethal_player = false;
			this->safety_size = 0.f;
			this->has_safe_point = false;
		}
		std::vector<Vector> penetration_points = {};
		Vector point = {};
		Vector center = {};
		lag_record_t* record = {};
		float center_dist = {};
		int hitbox = {};
		int hitgroup = {};

		float hitchance = -1.f;
		int damage = {};
		int no_pen_damage = {};
		int highest_damage = {};
		int player_health = {};
		bool lethal_player = {};
		int safety = {};
		float safety_size = {};
		bool has_safe_point = {};
		float safe_point_damage = {};
		std::string extra_info {};
	};

	struct final_target_t
	{
		final_target_t() = default;
		final_target_t( C_CSPlayer* player, lag_record_t* record, aimpoint_t point, bool predictive = false )
		{
			this->point = point;
			this->player = player;
			this->record = record;
			this->predictive = predictive;
		}

		C_CSPlayer* player = nullptr;
		lag_record_t* record = nullptr;
		aimpoint_t point = { { 0, 0, 0 }, { 0, 0, 0 }, -1, -1, 0.f };
		bool predictive = false;
	};

	struct target_data_t
	{
		target_data_t( int index )
		{
			this->index = index;
			this->player = nullptr;
		}
		void set( C_CSPlayer* player )
		{
			this->player = player;
		}
		int index;
		C_CSPlayer* player;
		std::unordered_map<int, bool> hitboxes;
	};

	struct
	{
		C_CSPlayer* last_shootable_target = nullptr;
		std::vector<C_CSPlayer*> last_targets{};
		std::vector<target_data_t*> tick_targets{};
	} inline cycle_data = {};

	enum class stop_type
	{
		stop_none = 0,
		stop_slide = 1,
		stop_full = 2
	};

	void run();
	void manual_shot();
	void fix_shot( CUserCmd* const cmd );
	void get_scan_records( C_CSPlayer* player, std::vector<lag_record_t*>& ret, bool ignore_extra = false );
	void calculate_hittable_enemies();
	void select_targets();
	bool has_dormant_target();
	void scan_dormant_targets( final_target_t& final_target );
	stop_type scan_targets( CUserCmd* const cmd );
	void get_best_target( final_target_t& final_target );

	inline CUserCmd original_shot_command {};
	inline C_WeaponCSBaseGun* weapon = nullptr;
	inline stop_type stop = {};
	inline stop_type keep_stop = {};
	inline int last_target = -1;
	inline bool has_shot = false;
	inline std::vector<target_data_t> targets = {};
	inline lag_record_t shot_record{};
	inline C_CSPlayer* last_target_ptr = nullptr;
	inline aimpoint_t final_aimpoint = {};
};
