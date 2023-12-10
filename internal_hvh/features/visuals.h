#pragma once
#include <map>

namespace evo
{
	namespace ren
	{
		class layer;
	}
}

namespace visuals
{
	struct misc_data_t
	{
		misc_data_t() = default;
		bool valid = false;
		float spread = -1.f;
		int wallbang = -1;
		bool scoped = false;
		bool alive{};
		bool doubletap{};
		bool silent{};
		bool anti_exploit{};
		bool mindmg_override{};
		float outgoing{};
		float incoming{};
		float lby{};
		Vector vel{};
		float anim_vel{};
		float z_vel{};
		float duckamt{};
		bool lc{};
		bool can_shift{};
		bool valve_ds = false;
	};

	void run();
	void render();
	void queue_custom_player_esp();
	void render_custom_player_esp();
	void players_get();
	void player_skeleton( const visual_data_t& player, evo::ren::layer& layer );
	void player_box( const visual_data_t& player, evo::ren::layer& layer );
	void player_info( const visual_data_t& player, evo::ren::layer& layer );
	bool bar_ping( const visual_data_t& player, int elements, evo::ren::layer& layer );
	void bar_health( const visual_data_t& player, int elements, evo::ren::layer& layer );
	void bar_armor( const visual_data_t& player, int elements, evo::ren::layer& layer );
	void bar_ammo( const visual_data_t& player, int elements, evo::ren::layer& layer );
	void player_out_of_fov( const visual_data_t& player, bool dormant = false );
	void queue_world_data();
	void queue_player_data();
	void queue_misc_data();
	void players_esp();
	void defuse_bar( const world_data_t& ent );
	void ammo_bar( const world_data_t& ent );
	void smoke_timer( const world_data_t& ent );
	void inferno_timer( const world_data_t& ent );
	void world_info( const world_data_t& ent );
	void world_esp();
	void draw_scope( const misc_data_t& misc_data );
	void aa_indicator( const misc_data_t& misc_data, int& offset );
	void fakeduck_indicator( const  misc_data_t& misc_data, int& offset );
	void lean_indicator( const  misc_data_t& misc_data, int& offset );
	void doubletap_indicator( const misc_data_t& misc_data, int& offset );
	void freestand_indicator( const  misc_data_t& misc_data, int& offset );
	void recharge_bar( const misc_data_t& misc_data, int& offset );
	void silent_indicator( const misc_data_t& misc_data, int& offset );
	void anti_exploit_indicator( const misc_data_t& misc_data, int& offset );
	void override_indicator( const misc_data_t& misc_data, int& offset );
	void force_safety_indicator( const misc_data_t& misc_data, int& offset );
	void force_baim_indicator( const misc_data_t& misc_data, int& offset );
	void hsonly_indicator( const misc_data_t& misc_data, int& offset );
	void lc_indicator( const misc_data_t& misc_data, int& offset );
	void draw_misc_data();
	void spread_circle( const misc_data_t& data );
	void autowall_crosshair( const misc_data_t& data );
	std::string grenade_to_name( int id );


	inline std::array<visual_data_t, 65> players = {};
	inline visual_world_data_t world_data = {};
	inline misc_data_t misc_data = {};
	inline visual_data_t preview_data = {};
	inline float game_curtime = {};
	inline bool setup_the_bones = false;
};
