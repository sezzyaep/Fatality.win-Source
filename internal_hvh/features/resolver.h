#pragma once

namespace resolver
{
	struct shot_t
	{
		shot_t() {};
		shot_t( const Vector& shotpos, const int cmdnum, const int tick, const bool secondary, const std::vector<Vector>& penetration_points, const int enemy_index = -1, const lag_record_t* record = nullptr, const int hitgroup = -1, const Vector pos = {}, const int damage = -1, const float inaccuracy = -1, const float hitchance = -1.f, const QAngle aimpunch = {}, int safety = 0 )
		{
			this->shotpos = this->original_shotpos = shotpos;
			this->enemy_index = enemy_index;
			this->tick = tick;
			this->cmdnum = cmdnum;
			this->aimpunch = aimpunch;
			if ( record )
				this->record = *record;
			else
				this->record.m_index = -1;

			this->hit = false;
			this->hit_originally = false;
			this->hit_extrapolation = false;
			this->hurt = false;
			this->hitgroup = hitgroup;
			this->hitpos = this->target_hitpos = pos;
			this->penetration_points = penetration_points;
			this->damage = damage;
			this->inaccuracy = inaccuracy;
			this->hitchance = hitchance;
			this->safety = safety;
			this->weapon_data = *local_weapon->get_wpn_data();
			this->secondary = secondary;
		}
		Vector shotpos{};
		Vector original_shotpos{};
		Vector hitpos{};
		Vector target_hitpos{};
		QAngle aimpunch{};
		CCSWeaponData weapon_data{};
		std::vector<Vector> hitposes;
		std::vector<Vector> penetration_points;
		bool hit{};
		bool hit_originally{};
		bool hit_extrapolation{};
		bool hurt{};
		struct
		{
			int victim = -1;
			int damage = -1;
			int hitgroup = -1;
		}hitinfo;
		int tick{};
		int cmdnum{};
		int enemy_index{};
		int hitgroup{};
		int damage{};
		bool secondary{};
		float inaccuracy{};
		float hitchance{};
		int safety{};
		lag_record_t record;
	};

	bool extrapolate_record( int ticks, lag_record_t& outrecord, bool simple = false );
	void pitch_resolve( lag_record_t* record );
	float get_resolver_angle( const lag_record_t& record, resolver_direction direction, float eye_angle );
	void yaw_resolve( const lag_record_t* record, const lag_record_t* previous );
	void post_animate( C_CSPlayer* player, lag_record_t* record );
	void wall_detect( lag_record_t* record );
	void resolve( C_CSPlayer* player, lag_record_t* record, lag_record_t* previous );
	void update_missed_shots( const ClientFrameStage_t& stage );
	void add_shot( shot_t& shot );
	void on_createmove();
	void calc_missed_shots( shot_t* shot );
	void hurt_listener( IGameEvent* game_event, record_shot_info_t& shot_info );
	shot_t* closest_shot( int tickcount );
	bool record_shot( IGameEvent* game_event );
	void listener( IGameEvent* game_event );
	Vector get_closest_hitpos( const shot_t& shot, const Vector& pos );
	Vector get_closest_penetrationpos( const shot_t& shot, const Vector& pos );
	void approve_shots( const ClientFrameStage_t& stage );
	void get_brute_angle( shot_t* shot );
	void set_local_info();

	inline std::deque<shot_t> shots = {};
	inline std::deque<shot_t> current_shots = {};
	inline std::deque<shot_t> unapproved_shots = {};
	inline std::vector<Vector> current_hitposes = {};
	inline Vector last_eye = {};
	inline Vector last_origin_diff = {};
	inline Vector last_origin = {};
	inline Vector current_eye = {};
};