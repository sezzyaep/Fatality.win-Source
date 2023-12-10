#pragma once

namespace penetration
{
	struct fire_bullet_data
	{
		Vector						src;
		trace_t						enter_trace;
		Vector						direction;
		ITraceFilter				filter;
		float						trace_length;
		float						trace_length_remaining;
		float						current_damage;
		int							penetrate_count;
	};

	enum
	{
		safety_none = 0,
		safety_no_roll = 2,
		safety_full = 6
	};

	struct pen_data
	{
		pen_data( lag_record_t* record = nullptr, resolver_direction dir = resolver_direction::resolver_networked, int safety = safety_none, lag_record_t* extra = nullptr, CCSWeaponData* custom_data = nullptr )
		{
			this->record = record;
			this->dir = dir;
			this->safety = safety;
			this->extra = extra;
			this->custom_data = custom_data;
			this->weapon = nullptr;
		}

		lag_record_t* extra = nullptr;
		lag_record_t* record = nullptr;
		CCSWeaponData* custom_data = nullptr;
		CGameTrace* out_trace = nullptr;
		C_WeaponCSBaseGun* weapon = nullptr;
		resolver_direction dir = resolver_direction::resolver_networked;
		bool one_wall = false;
		int safety = 0;

		// only set if hb test before awall
		bool custom_test = false;
		trace_t tr = {};
	};

	struct awall_trace_t
	{
		int index;
		Vector start;
		Vector point;
	};

	bool is_cfuncbrush( IClientEntity* entity );
	bool is_breakable_entity( C_BaseEntity* entity );
	void scale_damage( int item_definition_index, int hit_group, float armor, bool heavy_armor, bool helment, float& currentDamage );
	void scale_damage( C_CSPlayer* ent, int hit_group, CCSWeaponData* weapon_data, float& currentDamage, bool scale = true );
	bool trace_to_studio_csgo_hitgroups_priority( Ray_t* ray, CStudioHdr* pStudioHdr, mstudiohitboxset_t* set, matrix3x4_t** scan_matrix, int fContentsMask, Vector* vecOrigin, float flScale, trace_t* trace );
	bool trace_to_exit( CGameTrace& enter_trace, CGameTrace& exit_trace, Vector start_position, Vector direction );
	bool handle_bullet_penetration( CCSWeaponData* weapon_data, CGameTrace& enter_trace, Vector& eye_position, Vector direction, int& possible_hits_remaining, float& current_damage, float penetration_power );
	bool can_hit( C_CSPlayer* shooter, pen_data pen_data, const Vector pos, aimbot::aimpoint_t* point, int& damage, bool hitchance = false );
	bool can_hit( int item_definition_index, const Vector pos, const Vector end, int& damage, trace_t& trace );
	CCSWeaponData* get_normal_weapon();
	CCSWeaponData* get_full_weapon();
	bool fire_bullet( C_WeaponCSBaseGun* weapon, pen_data& pen_data, const Vector pos, aimbot::aimpoint_t* point, int& damage, bool hitchance = false );
	int can_wallbang();
	bool check_hittable( awall_trace_t trace );
	bool test_hitboxes( trace_t& tr, Ray_t& ray, const pen_data& pen_data, int force_hitgorup = -1 );

	constexpr auto ray_extension = 40.f;
};
