#pragma once

#include "tsl/robin_map.h"

class value
{
public:
	value() {};
	value( std::string name, value* dependency = nullptr, const double def = 0.0, value* key_mode = nullptr, value* key_temp = nullptr ) : m_value( def ), m_name( std::move( name ) ), m_default( def ), m_dependency( dependency ), m_key_mode( key_mode ), m_key_temp( key_temp ) {}

	template <typename T = float>
	T get()
	{
#ifndef _DEBUG
		if ( m_dependency && !m_dependency->get<bool>() )
			return 0.0;
#endif
		return static_cast< T >( m_value );
	}

	template <typename T = float>
	T get_real()
	{
		return static_cast< T >( m_value );
	}

	int get_int() const
	{
		return static_cast< int >( m_value );
	}

	bool get_bool() const
	{
		return !!get_int();
	}

	float get_float() const
	{
		return static_cast< float >( m_value );
	}

	color get_color() const
	{
		return color( static_cast< D3DCOLOR >( m_value ) );
	}

	std::string get_string() const
	{
		if ( m_string_val.empty() )
			return m_string_val;

		_u( ret, m_string_val );

		return ret;
	}

	std::string get_string_raw()
	{
		return m_string_val;
	}

	void set_int( int i )
	{
		m_value = i;
	}

	void set_bool( bool b )
	{
		m_value = b ? 1 : 0;
	}

	void set_float( float f )
	{
		m_value = f;
	}

	void set_color( const color& col )
	{
		m_value = col.d3d();
	}

	void set_string( const std::string& str )
	{
		if ( str.empty() )
			m_string_val = {};
		else
			m_string_val = _w2( str );
	}

	template <typename T = float>
	void set( T in ) { m_value = static_cast< double >( in ); }

	std::string get_name() const
	{
		return m_name;
	}

	void set_default()
	{
		m_value = m_default;
	}

	value* get_key_mode() const
	{
		return m_key_mode;
	}

	void set_key_mode( value* val )
	{
		m_key_mode = val;
	}

	value* get_key_temp() const
	{
		return m_key_temp;
	}

	void set_key_temp( value* val )
	{
		m_key_temp = val;
	}
private:
	double m_value = 0.0;
	std::string m_name = {};
	std::string m_string_val = {};
	double m_default = 0.0;
	value* m_dependency = nullptr;
	value* m_key_mode = nullptr;
	value* m_key_temp = nullptr;
};

namespace config_init
{
	inline std::string config_name = {};

	inline tsl::robin_map< uint32_t, value* >& get_cfg()
	{
		static tsl::robin_map< uint32_t, value* > m_cfg = {};
		return m_cfg;
	}

	inline value* add( uint32_t name, value* dependency = nullptr, double def = 0.0, value* key_mode = nullptr, value* key_temp = nullptr )
	{
		if ( get_cfg().find( name ) != get_cfg().end() )
		{
			const auto val = get_cfg().find( name )->second;
			if ( key_mode && !val->get_key_mode() )
				val->set_key_mode( key_mode );
			if ( key_temp && !val->get_key_temp() )
				val->set_key_temp( key_temp );

			return get_cfg().find( name )->second;
		}

		const auto val = new value( std::to_string( name ), dependency, def, key_mode, key_temp );

		get_cfg().insert( { name, val } );

		return val;
	}

	inline bool exists( uint32_t hash )
	{
		return get_cfg().find( hash ) != get_cfg().end();
	}

	inline value* get_item_direct( uint32_t hash )
	{
		if ( get_cfg().find( hash ) == get_cfg().end() )
		{
			const auto val = new value( std::to_string( hash ) );

			get_cfg().insert( { hash, val } );

			return val;
		}

		return get_cfg().at( hash );
	}
};

#define VAR(name, cfg_name) value* name = config_init::add( FNV1A( cfg_name ) )
#define VAR_DEF(name, cfg_name, def) value* name = config_init::add( FNV1A( cfg_name ), nullptr, def )
#define VARTYPE(var, name, cfg_name) var = config_init::add( name + FNV1A( cfg_name ) )
#define VARTYPE_DEF(var, name, cfg_name, def) var = config_init::add( name + FNV1A( cfg_name ), nullptr, def )
#define VARKEY(name, cfg_name) value* name = config_init::add( FNV1A( cfg_name ), nullptr, 0.0, config_init::add( FNV1A( cfg_name ) + FNV1A( ".key" ) ), config_init::add( FNV1A( cfg_name ) + FNV1A( ".key_temp" ) ) )


struct wconfig_type_t
{
	wconfig_type_t( std::string name_o )
	{
		_u( s_name, name_o );
		const auto name = fnv1a_rt( s_name );
		VARTYPE( hitchance, name, "hitchance" );
		VARTYPE_DEF( pointscale, name, "pointscale", 100.0 );
		VARTYPE( damage_accuracy, name, "damage_accuracy" );
		VARTYPE( mindmg, name, "mindmg" );
		VARTYPE( mindmg_override_enabled, name, "mindmg_override_enabled" );
		VARTYPE( mindmg_override, name, "mindmg_override" );
		VARTYPE( autostop, name, "autostop" );
		VARTYPE( autostop_on_key, name, "autostop_on_key" );
		VARTYPE( autostop_lethal, name, "autostop_lethal" );
		VARTYPE( autostop_inbetween, name, "autostop_inbetween" );
		VARTYPE( fallback_mode, name, "fallback_mode" );
		VARTYPE( autorevolver, name, "autorevolver" );
		VARTYPE( scout_jumpshot, name, "scout_jumpshot" );
		VARTYPE( autoscope, name, "autoscope" );
		VARTYPE( ignore_limbs, name, "ignore_limbs" );

		VARTYPE( predictive_autostop, name, "predictive_autostop" );
		VARTYPE( refine_shot, name, "refine_shot" );

		VARTYPE( force_safe.air, name, "extra_safety.air" );
		VARTYPE( force_safe.lethal, name, "extra_safety.lethal" );
		VARTYPE( force_safe.moving, name, "extra_safety.moving" );
		VARTYPE( force_safe.standing, name, "extra_safety.standing" );
		VARTYPE( force_safe.limbs, name, "extra_safety.limbs" );

		VARTYPE( extra_safety.air, name, "baim.air" );
		VARTYPE( extra_safety.lethal, name, "baim.lethal" );
		VARTYPE( extra_safety.moving, name, "fallback_when.moving" );
		VARTYPE( extra_safety.standing, name, "fallback_when.standing" );
		VARTYPE( extra_safety.limbs, name, "fallback_when.limbs" );

		VARTYPE( baim.air, name, "bodyaim.air" );
		VARTYPE( baim.lethal, name, "bodyaim.lethal" );
		VARTYPE( baim.moving, name, "bodyaim.moving" );
		VARTYPE( baim.standing, name, "bodyaim.standing" );
		VARTYPE( baim.high_risk, name, "bodyaim.high_risk" );

		VARTYPE_DEF( hitboxes.head, name, "hitboxes.head", 1.0 );
		VARTYPE_DEF( hitboxes.chest, name, "hitboxes.chest", 1.0 );
		VARTYPE_DEF( hitboxes.stomach, name, "hitboxes.stomach", 1.0 );
		VARTYPE( hitboxes.legs, name, "hitboxes.legs" );
		VARTYPE( hitboxes.feet, name, "hitboxes.feet" );
		VARTYPE( hitboxes.arms, name, "hitboxes.arms" );

		VARTYPE_DEF( pointscales.head, name, "pointscales.head", 1.0 );
		VARTYPE_DEF( pointscales.chest, name, "pointscales.chest", 1.0 );
		VARTYPE_DEF( pointscales.stomach, name, "pointscales.stomach", 1.0 );
		VARTYPE( pointscales.legs, name, "pointscales.legs" );
		VARTYPE( pointscales.feet, name, "pointscales.feet" );
		VARTYPE( pointscales.arms, name, "pointscales.arms" );
	}

	value* hitchance;
	value* pointscale;
	value* damage_accuracy;
	value* mindmg;
	value* mindmg_override_enabled;
	value* mindmg_override;
	value* autostop;
	value* autostop_on_key;
	value* autostop_lethal;
	value* autostop_inbetween;
	value* fallback_mode;
	value* autorevolver;
	value* scout_jumpshot;
	value* autoscope;
	value* ignore_limbs;
	value* predictive_autostop;
	value* refine_shot;
	struct
	{
		value* air;
		value* lethal;
		value* moving;
		value* standing;
		value* limbs;
	}extra_safety{};
	struct
	{
		value* air;
		value* lethal;
		value* moving;
		value* standing;
		value* limbs;
	}force_safe{};
	struct
	{
		value* air;
		value* lethal;
		value* moving;
		value* standing;
		value* high_risk;
	}baim{};
	struct
	{
		value* head;
		value* chest;
		value* stomach;
		value* legs;
		value* feet;
		value* arms;
	} hitboxes{};
	struct
	{
		value* head;
		value* neck;
		value* chest;
		value* stomach;
		value* legs;
		value* feet;
		value* arms;
	} pointscales{};
};

struct wconfig_legit_type_t
{
	wconfig_legit_type_t( std::string name_o )
	{
		_u( s_name, name_o );
		const auto name = fnv1a_rt( s_name );
		VARTYPE( magnet, name, "magnet" );
		VARTYPE( magnet_smooth, name, "magnet_smooth" );
		VARTYPE( magnet_fov, name, "magnet_fov" );
		VARTYPE( magnet_speed, name, "magnet_speed" );
		VARTYPE( magnet_hitboxes.head, name, "magnet_hitboxes.head" );
		VARTYPE( magnet_hitboxes.chest, name, "magnet_hitboxes.chest" );
		VARTYPE( magnet_hitboxes.body, name, "magnet_hitboxes.body" );
		VARTYPE( magnet_hitboxes.pelvis, name, "magnet_hitboxes.pelvis" );
		VARTYPE( override_factor, name, "override_factor" );
		VARTYPE( limit_factor, name, "limit_factor" );
		VARTYPE( target_delay, name, "target_delay" );
		VARTYPE( triggerbot, name, "triggerbot" );
		VARTYPE( reaction_time, name, "reaction_time" );
		VARTYPE( rcs, name, "rcs" );
		VARTYPE( rcs_speed, name, "rcs_speed" );
		VARTYPE( triggerbot_only_in_scope, name, "triggerbot_only_in_scope" );
		VARTYPE( magnet_only_in_scope, name, "magnet_only_in_scope" );
	}

	struct
	{
		value* head;
		value* chest;
		value* body;
		value* pelvis;
	} magnet_hitboxes{};

	value* magnet;
	value* magnet_smooth;
	value* magnet_fov;
	value* magnet_speed;
	value* override_factor;
	value* limit_factor;
	value* target_delay;
	value* triggerbot;
	value* reaction_time;
	value* rcs;
	value* rcs_speed;
	value* triggerbot_only_in_scope;
	value* magnet_only_in_scope;
};

namespace vars
{
	struct
	{
		VAR( enabled, "legit.enabled" );
		VAR_DEF( backtrack, "legit.backtrackamt", 400.f );
		VAR( quick_stop, "legit.quick_stop" );
		VAR( disable_smoke, "legit.disable_smoke" );
		VAR( disable_flash, "legit.disable_flash" );

		struct
		{
			wconfig_legit_type_t pistol = wconfig_legit_type_t( _w( "config.legit.pistol" ) ), rifle = wconfig_legit_type_t( _w( "config.legit.rifle" ) ), sniper = wconfig_legit_type_t( _w( "config.legit.sniper" ) ), smg = wconfig_legit_type_t( _w( "config.legit.smg" ) ), shotgun = wconfig_legit_type_t( _w( "config.legit.shotgun" ) ), lmg = wconfig_legit_type_t( _w( "config.legit.lmg" ) );
		} weapon_config{};
	} inline legit{};

	struct
	{
		VAR( enabled, "aim.enabled" );
		VAR( autofire, "aim.autofire" );
		VAR( anti_exploit, "aim.anti_exploit" );
		VAR( fallback_on_key, "aim.force_baim" );
		VAR( baim_on_key, "aim.force_bodyaim" );
		VAR( hsmode, "aim.hsmode" );
		VAR( ping_spike, "aim.ping_spike" );
		VAR( ping_spike_amt, "aim.ping_spike_amt" );
		VAR( ping_spike_base, "aim.ping_spike_base" );
		VAR( ping_spike_base_amt, "aim.ping_spike_base_amt" );
		VAR( resolver, "aim.resolver" );
		VAR( resolve_team, "aim.resolve_team" );
		VAR( override, "aim.override" );
		VAR( zeus_hc, "aim.zeus_hc" );
		VAR( knife_bot, "aim.knife_bot" );
		VAR( zeus_bot, "aim.zeus_bot" );
		VAR( low_perf_targets, "aim.low_perf_targets" );
		VAR( low_perf_scan_accuracy, "aim.low_scan_accuracy" );
		VAR( remove_spread, "aim.remove_spread" );
		VAR( delay_shot, "aim.delay_shot" );
		VAR( can_ping_spike, "aim.can_ping_spike" );
		VAR( slide, "aim.slide" );
		VAR( fake_duck, "aim.fake_duck" );
		VAR( punch, "aim.punch" );
		VAR( doubletap, "aim.doubletap" );
		VAR( visualize_tickbase, "aim.visualize_tickbase" );
		VAR( visualize_aimbot, "aim.visualize_aimbot" );
		VAR_DEF( visualize_color, "aim.visualize_color", D3DCOLOR_RGBA( 150, 30, 30, 255 ) );
		VAR_DEF( visualize_color_safe, "aim.visualize_color_safe", D3DCOLOR_RGBA( 0, 75, 150, 255 ) );
		VAR( extend_peek, "aim.extend_peek" );
		VAR( silent, "aim.silent" );
		VAR( immediate_recharge, "aim.immediate_recharge" );
		VAR( dormant, "aim.dormant" );
		VAR( hitchance_mode, "aim.hitchance_mode" );
		VAR( hitchance_mode_maxacc, "aim.hitchance_mode_maxacc" );
		VAR( resolver_mode, "aim.resolver_mode" );
		VAR_DEF( fov, "aim.fov", 180.0 );
		bool headshot_only()
		{
			var( mp_damage_headshot_only );
			return hsmode->get<bool>() || mp_damage_headshot_only->get_bool();
		}
	} inline aim{};

	struct
	{
		wconfig_type_t autosniper = wconfig_type_t( _w( "config.autosniper" ) ), scout = wconfig_type_t( _w( "config.scout" ) ), awp = wconfig_type_t( _w( "config.awp" ) ), heavy_pistol = wconfig_type_t( _w( "config.heavy_pistol" ) ), pistol = wconfig_type_t( _w( "config.pistol" ) ), other = wconfig_type_t( _w( "config.other" ) );
	} inline weapon_config{};

	struct
	{
		VAR( manual, "aa.manual" );
		VAR( manual_left, "aa.manual_left" );
		VAR( manual_right, "aa.manual_right" );
		VAR( manual_back, "aa.manual_back" );
		VAR( lc_indicator, "aa.lc_indicator" );
		VAR( lby_indicator, "aa.lby_indicator" );
		VAR_DEF( dt_indicator, "aim.dt_indicator", 1 );
		VAR_DEF( st_indicator, "aim.st_indicator", 1 );
		VAR_DEF( do_indicator, "aim.do_indicator", 1 );
		VAR_DEF( safe_indicator, "aim.safe_indicator", 1 );
		VAR_DEF( baim_indicator, "aim.baim_indicator", 1 );
		VAR_DEF( hsonly_indicator, "aim.hsonly_indicator", 1 );
		VAR_DEF( ping_indicator, "aim.ping_indicator", 1 );
		VAR_DEF( tp_indicator, "aim.tp_indicator", 1 );
		VAR_DEF( fs_indicator, "aim.fs_indicator", 1 );
		VAR( indicator, "aa.indicator" );
		VAR( shot, "aa.shot" );

		VAR( enabled, "aa.enabled" );
		VAR( spin, "aa.spin" );
		VAR( spin_speed, "aa.spin_speed" );
		VAR( spin_range, "aa.spin_range" );
		VAR( jitter, "aa.jitter" );
		VAR( jitter_range, "aa.jitter_range" );
		VAR( jitter_random, "aa.jitter_random" );
		VAR( at_target, "aa.at_target" );
		VAR( edge, "aa.edge" );
		VAR( pitch, "aa.pitch" );
		VAR( freestand, "aa.freestand" );
		VAR( freestand_fake, "aa.freestand_fake" );
		VAR( yaw, "aa.yaw" );
		VAR( add, "aa.add" );
		VAR( add_enabled, "aa.add_enabled" );
		VAR_DEF( choke_limit, "aa.choke_limit", 14.0 );
		VAR( choke_mode, "aa.choke_mode" );
		VAR( fake, "aa.fake" );
		VAR( fake_amount, "aa.fake_amount" );
		VAR_DEF( compensate_angle, "aa.compensate_angle", 100.0 );
		VAR( lean_type, "aa.lean_type" );
		VAR( lean_amount, "aa.lean_amount" );
		VAR( ensure_lean, "aa.ensure_lean" );
		VAR( strafing, "aa.strafing" );
		VAR( flip_fake, "aa.flip_fake" );
		VAR( flip_lean, "aa.flip_lean" );
	} inline aa{};

	struct
	{
		VAR( compensate_grenade, "misc.compensate_grenade" );
		VAR( peek_assist, "misc.peek_assist" );
		VAR( peek_assist_retreat_on_release, "misc.peek_retreat_on_release" );
		VAR_DEF( peek_assist_col, "misc.peek_assist_col", D3DCOLOR_RGBA( 0, 153, 204, 255 ) );
		VAR_DEF( peek_assist_col2, "misc.peek_assist_col2", D3DCOLOR_RGBA( 160, 255, 0, 255 ) );
		VAR( birthday_mode, "misc.birthday_mode" );
		VAR( instant_unduck, "misc.instant_unduck" );
		VAR( bhop, "misc.bhop" );
		VAR( edge_jump, "misc.edge_jump" );
		VAR( fov_enabled, "misc.fov_enabled" );
		VAR( fov_scope, "misc.fov_scope" );
		VAR( fov, "misc.fov" );
		VAR( vm_fov_enabled, "misc.vm_fov_enabled" );
		VAR( vm_fov, "misc.vm_fov" );
		VAR( aspect_ratio, "misc.aspect_ratio" );
		VAR( goblin_mode, "misc.goblin_mode" );
		VAR( hitmarker, "misc.hitmarker" );
		VAR( hitsound_select, "misc.hitsound_select" );
		VAR( hitsound_volume, "misc.hitsound_volume" );
		VAR( radar, "misc.radar" );
		VAR( remove_flash, "misc.remove_flash" );
		VAR( remove_scope, "misc.remove_scope" );
		VAR( scope_line_size, "misc.scope_line_size" );
		VAR( scope_line_invert, "misc.scope_line_invert" );
		VAR_DEF( scope_line_color, "misc.scope_line_color", D3DCOLOR_RGBA( 0, 0, 0, 255 ) );
		VAR( remove_smoke, "misc.remove_smoke" );
		VAR( remove_blur, "misc.remove_blur" );
		VAR_DEF( autostrafe_smoothing, "misc.smoothing", 70.0 );
		VAR( autostrafe_viewangles, "misc.autostrafe_viewangles" );
		VAR( autostrafe_movement_keys, "misc.autostrafe_movement_keys" );
		VAR( autostrafe_easy_strafe, "misc.autostrafe_easy_strafe" );
		VAR( autostrafe_jump_release, "misc.autostrafe_jump_release" );
		VAR( avoid_walls, "misc.avoid_walls" );
		VAR( circlestrafe, "misc.circlestrafe" );
		VAR( thirdperson, "misc.thirdperson" );
		VAR( thirdperson_dead, "misc.thirdperson_dead" );
		VAR( thirdperson_grenade, "misc.thirdperson_grenade" );
		VAR_DEF( thirdperson_dist, "misc.thirdperson_dist", 30.0 );
		VAR( transparency_in_scope, "misc.transparency_in_scope" );
		VAR( force_crosshair, "misc.force_crosshair" );
		VAR( autoaccept, "misc.autoaccept" );
		VAR( reveal_ranks, "misc.reveal_ranks" );
		VAR( hurtlog, "misc.hurtlog" );
		VAR( hurtlog_info, "misc.hurtlog_info" );
		VAR( buylog, "misc.buylog" );
		VAR( clantag, "misc.clantag" );
		VAR( preservedn, "misc.preservedn" );
		VAR( impacts, "misc.impacts" );
		VAR_DEF( impacts_color, "misc.impacts_color", D3DCOLOR_RGBA( 255, 0, 0, 255 ) );
		VAR_DEF( impacts_color2, "misc.impacts_color2", D3DCOLOR_RGBA( 0, 0, 155, 255 ) );
		VAR( grenade_tracer, "misc.grenade_tracer" );
		VAR( grenade_helper, "misc.grenade_helper" );
		VAR( sv_pure, "misc.sv_pure" );
		VAR( shared_esp_with_enemy, "misc.shared_esp_with_enemy" );
		VAR( shared_esp, "misc.shared_esp" );
		VAR_DEF( box_color, "misc.box_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		VAR_DEF( tracer_color, "misc.tracer_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		struct
		{
			VAR( primary, "misc.buybot.primary" );
			VAR( pistol, "misc.buybot.pistol" );
			VAR( kevlar_helmet, "misc.buybot.kevlar_helmet" );
			VAR( defuser, "misc.buybot.defuser" );
			VAR( taser, "misc.buybot.taser" );
			VAR( grenades, "misc.buybot.grenades" );
		}buybot;
		bool in_peek_assist() const
		{
			return peek_assist->get<bool>() && !misc::retract_peek && misc::has_position;
		}
		VAR( debug_info, "misc.debug_info" );
	} inline misc{};

	struct
	{
		struct
		{
			VAR( enabled, "visuals.esp.enabled" );
			VAR( ammo, "visuals.esp.ammo" );
			VAR_DEF( ammo_color, "visuals.esp.ammo_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			VAR( armor, "visuals.esp.armor" );
			VAR_DEF( armor_color, "visuals.esp.armor_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			VAR( ping, "visuals.esp.ping" );
			VAR_DEF( ping_color, "visuals.esp.ping_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			VAR( health, "visuals.esp.health" );
			VAR( health_style, "visuals.esp.health_style" );
			VAR_DEF( health_color_1, "visuals.esp.health_color_1", D3DCOLOR_RGBA( 130, 0, 0, 255 ) );
			VAR_DEF( health_color_2, "visuals.esp.health_color_2", D3DCOLOR_RGBA( 130, 180, 0, 255 ) );
			VAR( name, "visuals.esp.name" );
			VAR_DEF( name_color, "visuals.esp.name_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			VAR( weapon, "visuals.esp.weapon" );
			VAR_DEF( weapon_color, "visuals.esp.weapon_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			VAR_DEF( weapon_icon_color, "visuals.esp.weapon_icon_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			VAR( weapon_icon, "visuals.esp.weapon_icon" );
			VAR( box, "visuals.esp.box" );
			VAR_DEF( box_color, "visuals.esp.box_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			VAR( skeleton, "visuals.esp.skeleton" );
			VAR_DEF( skeleton_color, "visuals.esp.skeleton_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			VAR( history_skeleton, "visuals.esp.history_skeleton" );
			VAR_DEF( history_skeleton_color, "visuals.esp.history_skeleton_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			VAR( sound_esp, "visuals.esp.sound_esp" );
			VAR_DEF( sound_esp_color, "visuals.esp.sound_esp_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );

			struct
			{
				VAR( enabled, "visuals.esp.world.enabled" );
				VAR( weapons, "visuals.esp.world.weapons" );
				VAR( weapon_icon, "visuals.esp.world.weapon_icon" );
				VAR( grenades, "visuals.esp.world.grenades" );
				VAR( danger_zone, "visuals.esp.world.danger_zone" );
				VAR( c4, "visuals.esp.world.c4" );
				VAR_DEF( color, "visuals.esp.world.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			} world{};

			struct
			{
				VAR( scoped, "visuals.esp.scoped" );
				VAR( bomb, "visuals.esp.flags.bomb" );
				VAR( lc, "visuals.esp.flags.lc" );
				VAR( fd, "visuals.esp.flags.fd" );
				VAR( xp, "visuals.esp.flags.xp" );
				VAR( armor, "visuals.esp.flags.armor" );
				VAR( hit, "visuals.esp.flags.hit" );
				VAR( money, "visuals.esp.flags.money" );
				VAR( defuser, "visuals.esp.flags.defuser" );
				VAR( taser, "visuals.esp.flags.taser" );
			} flags{};
		} esp{};

		struct
		{
			VAR( enabled, "visuals.beams.enabled" );
			VAR_DEF( width, "visuals.beams.width", 8.0 );
			struct
			{
				VAR( enabled, "visuals.beams.enemy.enabled" );
				VAR_DEF( color, "visuals.beams.enemy.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			} enemy{};

			struct
			{
				VAR( enabled, "visuals.beams.team.enabled" );
				VAR_DEF( color, "visuals.beams.team.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			} team{};

			struct
			{
				VAR( enabled, "visuals.beams.local.enabled" );
				VAR_DEF( color, "visuals.beams.local.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			} local{};

		} beams{};
		struct
		{
			struct
			{
				VAR( hidden, "visuals.chams.enemy.hidden" );
				VAR( enabled, "visuals.chams.enemy.enabled" );
				VAR( type, "visuals.chams.enemy.type" );
				VAR_DEF( color, "visuals.chams.enemy.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				VAR_DEF( hidden_color, "visuals.chams.enemy.hidden_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				VAR_DEF( color2, "visuals.chams.enemy.color2", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );

				struct
				{
					VAR( type, "visuals.chams.enemy.ragdoll.type" );
					VAR_DEF( color, "visuals.chams.enemy.ragdoll.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
					VAR_DEF( color2, "visuals.chams.enemy.ragdoll.color2", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				} ragdoll{};

				struct
				{
					VAR( type, "visuals.chams.enemy.shot_record.type" );
					VAR_DEF( color, "visuals.chams.enemy.shot_record.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
					VAR_DEF( color2, "visuals.chams.enemy.shot_record.color2", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				} shot_record{};

				struct
				{
					VAR( type, "visuals.chams.enemy.weapon.type" );
					VAR( hidden, "visuals.chams.enemy.weapon.hidden" );
					VAR_DEF( color, "visuals.chams.enemy.weapon.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
					VAR_DEF( color2, "visuals.chams.enemy.weapon.color2", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				} attachment{};
			} enemy{};

			struct
			{
				VAR( hidden, "visuals.chams.team.hidden" );
				VAR( enabled, "visuals.chams.team.enabled" );
				VAR( type, "visuals.chams.team.type" );
				VAR_DEF( color, "visuals.chams.team.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				VAR_DEF( hidden_color, "visuals.chams.team.hidden_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				VAR_DEF( color2, "visuals.chams.team.color2", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );

				struct
				{
					VAR( type, "visuals.chams.team.ragdoll.type" );
					VAR_DEF( color, "visuals.chams.team.ragdoll.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
					VAR_DEF( color2, "visuals.chams.team.ragdoll.color2", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				} ragdoll{};

				struct
				{
					VAR( type, "visuals.chams.team.weapon.type" );
					VAR( hidden, "visuals.chams.team.weapon.hidden" );
					VAR_DEF( color, "visuals.chams.team.weapon.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
					VAR_DEF( color2, "visuals.chams.team.weapon.color2", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				} attachment{};
			} team{};

			struct
			{
				VAR( enabled, "visuals.chams.local.enabled" );
				VAR( type, "visuals.chams.local.type" );
				VAR_DEF( color, "visuals.chams.local.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				VAR_DEF( color2, "visuals.chams.local.color2", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );

				struct
				{
					VAR( enabled, "visuals.chams.local.fake.enabled" );
					VAR( type, "visuals.chams.local.fake.type" );
					VAR( interpolate, "visuals.chams.local.fake.interpolate" );
					VAR( pulsate, "visuals.chams.local.fake.pulsate" );
					VAR_DEF( color, "visuals.chams.local.fake.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
					VAR_DEF( color2, "visuals.chams.local.fake.color2", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				} fake{};

				struct
				{
					VAR( type, "visuals.chams.local.arms.type" );
					VAR( hide_sleeves, "visuals.chams.local.arms.hide_sleeves" );
					VAR_DEF( color, "visuals.chams.local.arms.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
					VAR_DEF( color2, "visuals.chams.local.arms.color2", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				} arms{};

				struct
				{
					VAR( type, "visuals.chams.local.weapon.type" );
					VAR_DEF( color, "visuals.chams.local.weapon.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
					VAR_DEF( color2, "visuals.chams.local.weapon.color2", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
				} attachment{};
			} local{};

			struct
			{
				VAR( enabled, "visuals.chams.history.enabled" );
				VAR_DEF( color, "visuals.chams.history.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			} history{};

			VAR( enabled, "visuals.chams.enabled" );
			VAR( extended, "visuals.chams.extended" );
			VAR( custom_bloom, "visuals.chams.custom_bloom" );
			VAR( custom_bloom_scale, "visuals.chams.custom_bloom_scale" );
			VAR( custom_exposure, "visuals.chams.custom_exposure" );
			VAR( custom_exposure_amt, "visuals.chams.custom_exposure_amt" );
			VAR( model_brightness, "visuals.chams.model_brightness" );
			VAR( model_brightness_amt, "visuals.chams.model_brightness_amt" );
		} chams{};

		struct
		{
			VAR( enabled, "visuals.glow.enabled" );
			struct
			{
				VAR( enabled, "visuals.glow.enemy.enabled" );
				VAR_DEF( color, "visuals.glow.enemy.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );

			} enemy;

			struct
			{
				VAR( enabled, "visuals.glow.team.enabled" );
				VAR_DEF( color, "visuals.glow.team.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			} team;

			struct
			{
				VAR( enabled, "visuals.glow.local.enabled" );
				VAR_DEF( color, "visuals.glow.local.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			} local;

			struct
			{
				VAR( weapons, "visuals.glow.world.weapons" );
				VAR( grenades, "visuals.glow.world.grenades" );
				VAR( c4, "visuals.glow.world.c4" );
				VAR_DEF( color, "visuals.glow.world.color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
			} world{};
		} glow{};

		VAR_DEF( disable_postprocessing, "visuals.disable_postprocessing", 1.0 );
		VAR( dark, "visuals.dark" );
		VAR( darkness, "visuals.darkness" );
		VAR( skybox, "visuals.skybox" );
		VAR_DEF( skybox_color, "visuals.skybox_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		VAR_DEF( dark_color, "visuals.dark_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		VAR_DEF( prop_color, "visuals.prop_color", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		VAR( prop_transparency, "visuals.prop_transparency" );
		VAR( spread_circle, "visuals.spread_circle" );
		VAR_DEF( spread_circle_color1, "visuals.spread_circle_color1", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		VAR_DEF( spread_circle_color2, "visuals.spread_circle_color2", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		VAR( spread_circle_type, "visuals.spread_circle_type" );
		VAR( out_of_fov, "visuals.out_of_fov" );
		VAR_DEF( out_of_fov_col, "visuals.out_of_fov_col", D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		VAR_DEF( out_of_fov_dst, "visuals.out_of_fov_dst", 50.0 );
		VAR_DEF( out_of_fov_size, "visuals.out_of_fov_size", 24.0 );
		VAR( autowall_crosshair, "visuals.autowall_crosshair" );
		VAR_DEF( ragdoll_thrust, "visuals.ragdoll_thrust", 100.0 );
		VAR( flashlight, "visuals.flashlight" );
		VAR_DEF( flashlight_radius, "visuals.flashlight_radius", 50.0 );
		VAR( model_t, "visuals.model_t" );
		VAR( model_ct, "visuals.model_ct" );
		VAR( mask_t, "visuals.mask_t" );
		VAR( mask_ct, "visuals.mask_ct" );
		VAR( disable_model_fade, "visuals.disable_model_fade" );
		VAR( disable_dormancy_interp, "misc.disable_dormancy_interp" );
		VAR( disable_health_bar_interp, "misc.disable_health_bar_interp" );
		VAR( disable_thirdperson_interp, "misc.disable_thirdperson_interp" );
		VAR( disable_esp_fade, "misc.disable_esp_fade" );
	} inline visuals{};

	struct
	{
		VAR( inventory_list_idx, "menu.inventory_list_idx" );
		VAR_DEF( slot, "menu.slot", 1.0 );
		VAR( preview_type, "menu.preview_type" );
	} inline menu{};

	struct
	{
		VAR( definition_index, "inventory.definition_index" );
		VAR( paint_kit, "inventory.paint_kit" );
		VAR( wear, "inventory.wear" );
		VAR( seed, "inventory.seed" );
		VAR_DEF( stat_trak, "inventory.stat_trak", -1.0 );
		VAR_DEF( enabled_ct, "inventory.enabled_ct", 1.0 );
		VAR_DEF( enabled_t, "inventory.enabled_t", 1.0 );
		VAR_DEF( autoselect_weapon, "inventory.autoselect_weapon", 1.0 );
		VAR( show_all_skins, "inventory.show_all_skins" );
	} inline inventory{};

	struct
	{
		VARKEY( aimbot, "aimbot" );
		VARKEY( hsmode, "hsmode" );
		VARKEY( doubletap, "doubletap" );
		VARKEY( silent, "silent" );
		VARKEY( dormant, "dormant" );
		VARKEY( mindmg_override_enabled_auto, "mindmg_override_enabled_auto" );
		VARKEY( mindmg_override_enabled_scout, "mindmg_override_enabled_scout" );
		VARKEY( mindmg_override_enabled_awp, "mindmg_override_enabled_awp" );
		VARKEY( mindmg_override_enabled_pistol, "mindmg_override_enabled_pistol" );
		VARKEY( mindmg_override_enabled_heavy_pistol, "mindmg_override_enabled_heavy_pistol" );
		VARKEY( mindmg_override_enabled_other, "mindmg_override_enabled_other" );
		VARKEY( antiaim, "antiaim" );
		VARKEY( autostop, "autostop" );
		VARKEY( fallback, "baim" );
		VARKEY( baim, "bodyaim" );
		VARKEY( esp, "esp" );
		VARKEY( chams, "chams" );
		VARKEY( thirdperson, "thirdperson" );
		VARKEY( aa_back, "aa_back" );
		VARKEY( aa_left, "aa_left" );
		VARKEY( aa_right, "aa_right" );
		VARKEY( slide, "slide" );
		VARKEY( edge_jump, "edge_jump" );
		VARKEY( fake_duck, "fake_duck" );
		VARKEY( peek_assist, "peek_assist" );
		VARKEY( freestanding, "freestanding" );
		VARKEY( ensure_lean, "ensure_lean" );
		VARKEY( magnet, "magnet" );
		VARKEY( triggerbot, "triggerbot" );
		VARKEY( flashlight, "flashlight" );
	} inline key{};

	struct
	{
		VAR( allow_insecure, "lua.allow_insecure" );
	} inline lua{};

	inline bool legit_enabled()
	{
		return legit.enabled->get<bool>() || !aim.enabled->get<bool>();
	}
};

struct weaponconfig_t
{
	weaponconfig_t( const int current )
	{
		index = current;

		const wconfig_type_t* config = nullptr;

		switch ( current )
		{
			case 0:
			{
				config = &vars::weapon_config.autosniper;
				break;
			}
			case 1:
			{
				config = &vars::weapon_config.scout;
				break;
			}
			case 2:
			{
				config = &vars::weapon_config.awp;
				break;
			}
			case 3:
			{
				config = &vars::weapon_config.heavy_pistol;
				break;
			}
			case 4:
			{
				config = &vars::weapon_config.pistol;
				break;
			}
			case 5:
			{
				config = &vars::weapon_config.other;
				break;
			}
		}
		hitchance = config->hitchance;
		pointscale = config->pointscale;;
		damage_accuracy = config->damage_accuracy;
		mindmg = config->mindmg;
		mindmg_override_enabled = config->mindmg_override_enabled;
		mindmg_override = config->mindmg_override;
		autostop = config->autostop;
		autostop_key = config->autostop_on_key;
		autostop_lethal = config->autostop_lethal;
		autostop_inbetween = config->autostop_inbetween;
		fallback_mode = config->fallback_mode;
		scout_jumpshot = config->scout_jumpshot;
		autorevolver = config->autorevolver;
		autoscope = config->autoscope;
		ignore_limbs = config->ignore_limbs;
		predictive_autostop = config->predictive_autostop;
		refine_shot = config->refine_shot;
		force_safe.lethal = config->force_safe.lethal;
		force_safe.air = config->force_safe.air;
		force_safe.moving = config->force_safe.moving;
		force_safe.standing = config->force_safe.standing;
		force_safe.limbs = config->force_safe.limbs;
		extra_safety.lethal = config->extra_safety.lethal;
		extra_safety.air = config->extra_safety.air;
		extra_safety.moving = config->extra_safety.moving;
		extra_safety.standing = config->extra_safety.standing;
		extra_safety.limbs = config->extra_safety.limbs;
		baim.lethal = config->baim.lethal;
		baim.air = config->baim.air;
		baim.moving = config->baim.moving;
		baim.standing = config->baim.standing;
		baim.high_risk = config->baim.high_risk;
		hitboxes.head = config->hitboxes.head;
		hitboxes.chest = config->hitboxes.chest;
		hitboxes.stomach = config->hitboxes.stomach;
		hitboxes.legs = config->hitboxes.legs;
		hitboxes.feet = config->hitboxes.feet;
		hitboxes.arms = config->hitboxes.arms;
		pointscales.head = config->pointscales.head;
		pointscales.chest = config->pointscales.chest;
		pointscales.stomach = config->pointscales.stomach;
		pointscales.legs = config->pointscales.legs;
		pointscales.feet = config->pointscales.feet;
		pointscales.arms = config->pointscales.arms;
	}
	inline bool is_sniper() const
	{
		return index >= 0 && index <= 2;
	}
	inline int get_mindmg( C_CSPlayer* player = nullptr, int highest_damage = 0 ) const
	{
		const auto minimum_dmg = static_cast< int >( mindmg_override_enabled->get< bool >() ? mindmg_override->get< float >() : mindmg->get< float >() );
		if ( !player )
			return std::min( 100, minimum_dmg );

		const auto additional_damage = std::max( 0, minimum_dmg - 100 );
		const auto adjusted_mindmg = std::min( 100, std::min( std::min( 100, minimum_dmg ), std::min( highest_damage + additional_damage, player->get_predicted_health() + additional_damage ) ) );
		return adjusted_mindmg;
	}
	value* hitchance;
	value* pointscale;
	value* damage_accuracy;
	value* mindmg;
	value* mindmg_override_enabled;
	value* mindmg_override;
	value* autostop;
	value* autostop_key;
	value* autostop_lethal;
	value* autostop_inbetween;
	value* fallback_mode;
	value* scout_jumpshot;
	value* autorevolver;
	value* autoscope;
	value* ignore_limbs;
	value* predictive_autostop;
	value* refine_shot;
	struct
	{
		value* lethal;
		value* air;
		value* moving;
		value* standing;
		value* limbs;
	}extra_safety{};
	struct
	{
		value* lethal;
		value* air;
		value* moving;
		value* standing;
		value* limbs;
	}force_safe{};
	struct
	{
		value* lethal;
		value* air;
		value* moving;
		value* standing;
		value* high_risk;
	}baim{};
	struct
	{
		value* head;
		value* chest;
		value* stomach;
		value* legs;
		value* feet;
		value* arms;
	} hitboxes{};
	struct
	{
		value* head;
		value* chest;
		value* stomach;
		value* legs;
		value* feet;
		value* arms;
	} pointscales{};
	int index;
};

struct weaponconfig_legit_t
{
	weaponconfig_legit_t( const int current ) : index( current )
	{
		const wconfig_legit_type_t* config = nullptr;

		switch ( current )
		{
			case 0:
			{
				config = &vars::legit.weapon_config.pistol;
				break;
			}
			case 1:
			{
				config = &vars::legit.weapon_config.rifle;
				break;
			}
			case 2:
			{
				config = &vars::legit.weapon_config.sniper;
				break;
			}
			case 3:
			{
				config = &vars::legit.weapon_config.smg;
				break;
			}
			case 4:
			{
				config = &vars::legit.weapon_config.shotgun;
				break;
			}
			case 5:
			{
				config = &vars::legit.weapon_config.lmg;
				break;
			}
		}
		magnet = config->magnet;
		magnet_smooth = config->magnet_smooth;
		magnet_fov = config->magnet_fov;
		magnet_speed = config->magnet_speed;
		magnet_hitboxes.head = config->magnet_hitboxes.head;
		magnet_hitboxes.chest = config->magnet_hitboxes.chest;
		magnet_hitboxes.body = config->magnet_hitboxes.body;
		magnet_hitboxes.pelvis = config->magnet_hitboxes.pelvis;
		override_factor = config->override_factor;
		limit_factor = config->limit_factor;
		target_delay = config->target_delay;
		triggerbot = config->triggerbot;
		reaction_time = config->reaction_time;
		rcs = config->rcs;
		rcs_speed = config->rcs_speed;
		triggerbot_only_in_scope = config->triggerbot_only_in_scope;
		magnet_only_in_scope = config->magnet_only_in_scope;
	}

	struct
	{
		value* head;
		value* chest;
		value* body;
		value* pelvis;
	} magnet_hitboxes{};
	value* magnet;
	value* magnet_smooth;
	value* magnet_fov;
	value* magnet_speed;
	value* override_factor;
	value* limit_factor;
	value* target_delay;
	value* triggerbot;
	value* reaction_time;
	value* rcs;
	value* rcs_speed;
	value* triggerbot_only_in_scope;
	value* magnet_only_in_scope;
	int index;
};

namespace config
{
	bool load( int slot );
	bool save( int slot );
	weaponconfig_t* current_config( C_BaseCombatWeapon* weapon );
	weaponconfig_legit_t* current_legit_config( C_BaseCombatWeapon* weapon );

	inline std::array weapon_config =
	{
		weaponconfig_t( 0 ),
		weaponconfig_t( 1 ),
		weaponconfig_t( 2 ),
		weaponconfig_t( 3 ),
		weaponconfig_t( 4 ),
		weaponconfig_t( 5 ),
	};
	inline std::array weapon_config_legit =
	{
		weaponconfig_legit_t( 0 ),
		weaponconfig_legit_t( 1 ),
		weaponconfig_legit_t( 2 ),
		weaponconfig_legit_t( 3 ),
		weaponconfig_legit_t( 4 ),
		weaponconfig_legit_t( 5 ),
	};
	inline std::string config_name = _w( "slot" );
};