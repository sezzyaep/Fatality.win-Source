#pragma once

namespace antiaim
{
	void run();
	void run_lag();
	void run_on_send();
	void fake_duck();
	void fix_movement( CUserCmd* cmd = globals::current_cmd );
	bool get_antiaim_state();
	void do_jitter( float& yaw );
	bool in_fakeduck();
	bool started_peek_fakelag(); //DANGER!! changes return value for next call;
	bool check_hittable();
	bool check_server_hittable();
	std::pair<bool, float> check_knife_round();
	bool check_in_defuse_range( Vector* pos = nullptr );
	void fake_lag();
	bool on_peek_fakelag( bool& notarget );
	void do_spin( float& yaw );
	void do_add( float& yaw );
	float get_lean( float target_direction, float target_angle );
	float get_wish_speed( CUserCmd* const cmd );

	bool do_edge( float& yaw );
	void do_at_target( float& yaw );

	inline int to_choke = {};
	inline float anim_vel = {};
	inline float lby_update = {};
	inline float next_lby_update = {};
	inline bool in_flip = {};
	inline float fake_direction = {};
	inline float real_direction = {};
	inline QAngle target_angle = {};

	inline int antiaim_called = {};
	inline float fakeduck_called = {};
	inline bool started_fakelag = {};
	inline bool fakelag_next = {};
	inline bool in_fakelag = {};
	inline bool in_onpeek = {};
	inline bool in_skip_animation = {};
	inline QAngle wish_angles[ 150 ] = {};
	inline bool has_animstate = {};

	//lua
	inline bool run_antiaim = {};
	inline float current_lean = 0.f;
	inline int to_lag = {};

	inline void reset()
	{
		antiaim_called = 0.f;
		has_animstate = false;
		in_fakelag = false;
		started_fakelag = false;
		fakeduck_called = 0.f;
		fakelag_next = false;
		in_onpeek = false;
		in_skip_animation = false;
	}
};
