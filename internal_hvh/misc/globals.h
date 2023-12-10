#pragma once

#define local_player ( reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity( interfaces::engine()->GetLocalPlayer() ) ) )
#define local_weapon ( reinterpret_cast< C_WeaponCSBaseGun* >( interfaces::entity_list()->get_client_entity_from_handle( local_player->get_active_weapon() ) ) )

struct weaponconfig_t;
namespace globals
{
	inline HMODULE module_base;

	struct secure_info_t
	{
		int pad{};
		uint64_t initial_checksum = 0;
		union
		{
			unsigned int threads_ran;
			unsigned int threads_ran_cached;
		} values{};
		int useless_pad{};
	} extern secure_info;

	C_CSPlayer* get_player( int index );
	C_WeaponCSBaseGun* get_weapon( CBaseHandle handle );
	bool is_dangerzone();
	inline bool in_cm = {};

	inline bool* send_packet = {};
	inline CUserCmd* current_cmd = {};
	inline CUserCmd* first_cmd = {};
	inline std::vector<int> sent_commands = {};
	inline std::vector<int> outgoing_commands = {};

	struct
	{
		std::string name;
		std::string expiry;
		std::vector<uint8_t> img;
		bool alpha = false;
	} inline user_info;

	float random_float( float from, float to );
	int random_int( int from, int to );
	void random_seed( int seed );

	bool line_goes_through_smoke( const Vector& vec_start, const Vector& vec_end );
	void util_trace_line( const Vector& vec_start, const Vector& vec_end, const unsigned n_mask, const C_BaseEntity* ignore, trace_t* p_trace, int collision_group = 0 );
	void util_trace_hull( const Vector& vec_start, const Vector& vec_end, const Vector& extents, const unsigned n_mask, const C_BaseEntity* ignore, trace_t* p_trace, int collision_group = 0 );
	void util_clip_trace_to_player( const Vector& vec_start, const Vector& vec_end, const unsigned int n_mask, C_BaseEntity* target, trace_t* p_trace );

	inline bool nospread = {};
	inline bool restricted_unduck = {};
	inline int shot_command = {};
	inline int choked_shot_command = {};
	inline bool erase_init = {};
	inline float interpolated_curtime = {};
}


