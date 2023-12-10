#pragma once

struct weaponconfig_legit_t;

namespace legit
{
	Vector closest_hitbox_pos( const player_log_t& log, lag_record_t* record, float& bestdist );
	void run_backtrack();
	void run_triggerbot();
	void run_rcs( float* mouse_x, float* mouse_y, C_WeaponCSBaseGun* weapon );
	void run_aim( float* mouse_x, float* mouse_y );

	float get_curve( const float t );
	void smooth( QAngle& dest_ang, const QAngle& cur_ang, const float speed );
	Vector2D angle_to_pixels( const QAngle& angle );
	QAngle pixels_to_angle( const Vector2D& pixel );

	inline lag_record_t* shot_record = {};
	inline int shot_index = -1;

	struct legit_target_t
	{
		int hitbox{};
		int index{};
		float fov{};
	};

	inline float visible_time[ 65 ]{};
	inline legit_target_t target = {};
	inline float target_time = {};
	inline float trigger_time = {};
	inline QAngle last_punch = {};
};