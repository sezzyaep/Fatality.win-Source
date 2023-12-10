#pragma once

struct aimmatrix_transition
{
	float has_been_valid;
	float has_been_invalid;
	float blend_in;
	float blend_out;
	float value;
};

struct pose_param_cache
{
	bool init;
	int32_t index;
	const char* name;
};

struct procedural_foot
{
	Vector anim;
	Vector anim_last;
	Vector plant;
	Vector plant_vel;
	float lock_amount;
	float plant_time;
};

class CCSGOPlayerAnimState
{

private:
	MFUNC( _update( const QAngle ang, bool force ), void( __vectorcall* )( void*, void*, float, float, float, bool ), make_offset( "client.dll", sig_update_anim_state ) )( nullptr, 0, ang.y, ang.x, force );
public:
	MFUNC( reset(), void( __thiscall* )( void* ), make_offset( "client.dll", sig_animstate_reset ) )( );
	MFUNC( get_active_weapon_prefix() const, const char* ( __thiscall* )( const void* ), make_offset( "client.dll", sig_get_active_weapon_prefix ) )( );
	MFUNC( get_layer_ideal_weight_from_sequence_cycle( int32_t layer ), float( __thiscall* )( void*, int32_t ), make_offset( "client.dll", sig_get_layer_ideal_weight_from_sequence_cycle ) )( layer );
	MFUNC( update_layer_order_preset( int32_t index, int32_t sequence ), void( __thiscall* )( void*, int32_t, int32_t ), make_offset_multi( "client.dll", sig_update_layer_order_preset ) )( index, sequence );

	void update( const QAngle ang );

	inline void copy_meta( CCSGOPlayerAnimState* const to ) const
	{
		to->first_run = this->first_run;
		to->first_foot_plant = this->first_foot_plant;
		to->last_frame = this->last_frame;
		to->smooth_eye_position = this->smooth_eye_position;
		to->strafe_change_weight_smooth_falloff = this->strafe_change_weight_smooth_falloff;
		to->cached_model = this->cached_model;
		to->step_height_left = this->step_height_left;
		to->step_height_right = this->step_height_right;
		to->old_weapon = this->old_weapon;
		to->player = this->player;
		memcpy( to->poses, this->poses, sizeof( poses ) );
		to->foot_left = this->foot_left;
		to->foot_right = this->foot_right;
		to->camera_smooth_height = this->camera_smooth_height;
		to->smooth_height_valid = this->smooth_height_valid;
		to->last_time_velocity_over_ten = this->last_time_velocity_over_ten;
		to->head_interpolation_height = this->head_interpolation_height;
		to->animset_version = this->animset_version;
	}

	uintptr_t layer_order_preset{};
	bool first_run{};
	bool first_foot_plant{};
	int32_t last_frame{};
	float smooth_eye_position{};
	float strafe_change_weight_smooth_falloff{};
	aimmatrix_transition stand_walk_aim{};
	aimmatrix_transition stand_run_aim{};
	aimmatrix_transition crouch_walk_aim{};
	int32_t cached_model{};
	float step_height_left{};
	float step_height_right{};
	C_WeaponCSBaseGun* old_weapon{};
	C_CSPlayer* player{};
	C_WeaponCSBaseGun* weapon{};
	C_WeaponCSBaseGun* last_weapon{};
	float last_update{};
	int32_t last_framecount{};
	float last_increment{};
	float eye_yaw{};
	float eye_pitch{};
	float foot_yaw{};
	float foot_yaw_last{};
	float move_yaw{};
	float move_yaw_ideal{};
	float move_yaw_current_to_ideal{};
	float time_to_align_lower_body{};
	float feet_cycle{};
	float feet_weight{};
	float feet_weight_smoothed{};
	float duck_amount{};
	float duck_additional{};
	float recrouch_weight{};
	Vector position_current{};
	Vector position_last{};
	Vector velocity{};
	Vector movement_direction{};
	Vector last_accel_speed{};
	float speed{};
	float speed_z{};
	float walk_speed{};
	float running_speed{};
	float ducking_speed{};
	float moving_time{};
	float standing_time{};
	bool on_ground{};
	bool in_hit_ground{};
	float jump_to_fall{};
	float air_time{};
	float left_ground_height{};
	float land_anim_mult{};
	float ground_fraction{};
	bool landed_this_frame{};
	bool left_this_frame{};
	float in_air_smooth{};
	bool on_ladder{};
	float ladder_weight{};
	float ladder_speed{};
	bool ground_fraction_state{};
	bool defuse_started{};
	bool plant_started{};
	bool twitch_started{};
	bool adjust_started{};
	struct
	{
		uint16_t* m_pMemory{};
		int m_nAllocationCount{};
		int m_nGrowSize{};
		int m_Size{};
		uint16_t* m_pElements{};
	} modifiers;
	float next_twicth{};
	float last_injury{};
	float last_velocity_test{};
	Vector velocity_last{};
	Vector target_accel{};
	Vector accel{};
	float acceleration_weight{};
	float aim_matrix_transition{};
	float aim_matrix_transition_delay{};
	bool flashed{};
	float strafe_change_weight{};
	float strafe_change_target_weight{};
	float strafe_change_cycle{};
	int32_t strafe_sequence{};
	bool strafe_changing{};
	float strafe_duration{};
	float foop_lerp{};
	bool feet_crossed{};
	bool is_accelerating{};
	pose_param_cache poses[ 20 ]{};
	float move_weight_too_high{};
	float static_approach_speed{};
	int32_t previous_move_state{};
	float stutter_step{};
	float action_weight_bias_remainder{};
	procedural_foot foot_left{};
	procedural_foot foot_right{};
	float camera_smooth_height{};
	bool smooth_height_valid{};
	float last_time_velocity_over_ten{};
	float head_interpolation_height{};
	float aim_yaw_min{};
	float aim_yaw_max{};
	float aim_pitch_min{};
	float aim_pitch_max{};
	int32_t animset_version{};
};