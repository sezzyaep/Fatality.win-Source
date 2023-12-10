#pragma once

namespace freestanding
{
	bool override( float & yaw );
	bool get_rotated_damage( C_CSPlayer* shooter, CCSWeaponData* weapon, Vector shooter_position, Vector local_position, QAngle angle, float* dmg_left, float* dmg_right, float dist, bool first_run, float rot = 999.f );
	bool get_real( float& yaw );
	void get_targets();
	bool freestand( float& yaw );
	void update_hotkeys( ClientFrameStage_t stage );
	void do_jitter( float & yaw );

	inline float last_fs_time = {};
	inline int direction = -1;
	inline std::vector<C_CSPlayer*> players = {};
};
