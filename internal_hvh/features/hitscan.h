#pragma once


namespace hitscan
{
	inline lag_record_t local_record = {};

	bool get_best_hitpoint( aimbot::target_data_t* target, lag_record_t& record, aimbot::aimpoint_t& ret, lag_record_t* extra = nullptr , Vector eyepos = local_player->get_eye_pos() );
	aimbot::aimpoint_t hitscan_record( aimbot::target_data_t* target, lag_record_t& record, int safety = penetration::safety_none, lag_record_t* extra = nullptr, Vector eyepos = local_player->get_eye_pos() );
	void create_local_record( bool extrapolate );
	bool hitscan_self( C_CSPlayer* enemy, bool extrapolate, lag_record_t& record = local_record );
	bool check_hittable_by_enemy( C_CSPlayer* enemy, bool extrapolate );
	bool check_enemy_hittable( C_CSPlayer* enemy );
	void multipoint_self( lag_record_t& record, Vector& eyepos, std::vector<aimbot::aimpoint_t>& points );
	void multipoint_hitbox( circular_buffer<aimbot::aimpoint_t, 78>& points, int index, const studiohdr_t* studio_hdr, lag_record_t& record, const Vector& eyepos, float spread_radius, bool is_knife, bool is_zeus, const weaponconfig_t* config, int safety, lag_record_t* extra, CCSWeaponData* weapon_data );
	void filter_hitboxes();

	inline std::array<bool, 21> enabled_hitboxes = {};
	inline std::array<bool, 21> pointscale_hitboxes = {};

	inline static constexpr size_t capsule_vert_positions_size = 74;
	inline float capsule_vert_positions_[ capsule_vert_positions_size ][ 3 ] =
	{
		{ -0.01f, -0.01f, 1.0f }, { 0.51f, 0.0f, 0.86f }, { 0.44f, 0.25f, 0.86f }, { 0.25f, 0.44f, 0.86f }, { -0.01f, 0.51f, 0.86f }, { -0.26f, 0.44f, 0.86f }, { -0.45f, 0.25f, 0.86f }, { -0.51f, 0.0f, 0.86f }, { -0.45f, -0.26f, 0.86f },
		{ -0.26f, -0.45f, 0.86f }, { -0.01f, -0.51f, 0.86f }, { 0.25f, -0.45f, 0.86f }, { 0.44f, -0.26f, 0.86f }, { 0.86f, 0.0f, 0.51f }, { 0.75f, 0.43f, 0.51f }, { 0.43f, 0.75f, 0.51f }, { -0.01f, 0.86f, 0.51f }, { -0.44f, 0.75f, 0.51f },
		{ -0.76f, 0.43f, 0.51f }, { -0.87f, 0.0f, 0.51f }, { -0.76f, -0.44f, 0.51f }, { -0.44f, -0.76f, 0.51f }, { -0.01f, -0.87f, 0.51f }, { 0.43f, -0.76f, 0.51f }, { 0.75f, -0.44f, 0.51f }, { 1.0f, 0.0f, 0.01f }, { 0.86f, 0.5f, 0.01f },
		{ 0.49f, 0.86f, 0.01f }, { -0.01f, 1.0f, 0.01f }, { -0.51f, 0.86f, 0.01f }, { -0.87f, 0.5f, 0.01f }, { -1.0f, 0.0f, 0.01f }, { -0.87f, -0.5f, 0.01f }, { -0.51f, -0.87f, 0.01f }, { -0.01f, -1.0f, 0.01f }, { 0.49f, -0.87f, 0.01f },
		{ 0.86f, -0.51f, 0.01f }, { 1.0f, 0.0f, -0.02f }, { 0.86f, 0.5f, -0.02f }, { 0.49f, 0.86f, -0.02f }, { -0.01f, 1.0f, -0.02f }, { -0.51f, 0.86f, -0.02f }, { -0.87f, 0.5f, -0.02f }, { -1.0f, 0.0f, -0.02f }, { -0.87f, -0.5f, -0.02f },
		{ -0.51f, -0.87f, -0.02f }, { -0.01f, -1.0f, -0.02f }, { 0.49f, -0.87f, -0.02f }, { 0.86f, -0.51f, -0.02f }, { 0.86f, 0.0f, -0.51f }, { 0.75f, 0.43f, -0.51f }, { 0.43f, 0.75f, -0.51f }, { -0.01f, 0.86f, -0.51f }, { -0.44f, 0.75f, -0.51f },
		{ -0.76f, 0.43f, -0.51f }, { -0.87f, 0.0f, -0.51f }, { -0.76f, -0.44f, -0.51f }, { -0.44f, -0.76f, -0.51f }, { -0.01f, -0.87f, -0.51f }, { 0.43f, -0.76f, -0.51f }, { 0.75f, -0.44f, -0.51f }, { 0.51f, 0.0f, -0.87f }, { 0.44f, 0.25f, -0.87f },
		{ 0.25f, 0.44f, -0.87f }, { -0.01f, 0.51f, -0.87f }, { -0.26f, 0.44f, -0.87f }, { -0.45f, 0.25f, -0.87f }, { -0.51f, 0.0f, -0.87f }, { -0.45f, -0.26f, -0.87f }, { -0.26f, -0.45f, -0.87f }, { -0.01f, -0.51f, -0.87f }, { 0.25f, -0.45f, -0.87f },
		{ 0.44f, -0.26f, -0.87f }, { 0.0f, 0.0f, -1.0f },
	};
	inline static constexpr size_t capsule_line_indices_size = 117;
	inline int capsule_line_indices_[ capsule_line_indices_size ] =
	{
		-1,
		14, 0, 4, 16, 28, 40, 52, 64, 73, 70, 58, 46, 34, 22, 10, -1,
		14, 0, 1, 13, 25, 37, 49, 61, 73, 67, 55, 43, 31, 19, 7, -1,
		12, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, -1,
		12, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, -1,
		12, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, -1,
		12, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, -1,
		12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, -1,
		12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, -1
	};

	//inline static constexpr size_t capsule_vert_positions_size = 26;
	//inline float capsule_vert_positions_[ capsule_vert_positions_size ][ 3 ] =
	//{
	//	{ -0.01f, -0.01f, 1.0f } /*0*/, { 0.0f, 0.0f, -1.0f } /*73*/,
	//{ 0.51f, 0.0f, 0.86f } /*1*/, { 0.86f, 0.0f, 0.51f } /*13*/, { 1.0f, 0.0f, 0.01f } /*25*/, { 1.0f, 0.0f, -0.02f } /*37*/, { 0.86f, 0.0f, -0.51f } /*49*/, { 0.51f, 0.0f, -0.87f } /*64*/,
	//{ -0.01f, 0.51f, 0.86f } /*4*/, { -0.01f, 0.86f, 0.51f } /*16*/, { -0.01f, 1.0f, 0.01f } /*28*/, { -0.01f, 1.0f, -0.02f } /*40*/, { -0.01f, 0.86f, -0.51f } /*52*/, { -0.01f, 0.51f, -0.87f } /*64*/,
	//{ -0.51f, 0.0f, 0.86f } /*7*/, { -0.87f, 0.0f, 0.51f } /*19*/, { -1.0f, 0.0f, 0.01f } /*31*/, { -1.0f, 0.0f, -0.02f } /*43*/, { -0.87f, 0.0f, -0.51f } /*55*/, { -0.51f, 0.0f, -0.87f } /*67*/,
	//{ -0.01f, -0.51f, 0.86f } /*10*/, { -0.01f, -0.87f, 0.51f } /*22*/, { -0.01f, -1.0f, 0.01f } /*34*/, { -0.01f, -1.0f, -0.02f } /*46*/, { -0.01f, -0.87f, -0.51f } /*58*/, { -0.01f, -0.51f, -0.87f } /*70*/,
	//};
	//inline static constexpr size_t capsule_line_indices_size = 8;
	//inline int capsule_line_indices_[ capsule_line_indices_size ] =
	//{
	//	4, 5, 10, 11,
	//	16, 17, 22, 23,
	//};
};