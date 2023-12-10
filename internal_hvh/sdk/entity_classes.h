#pragma once
#include "../misc/netvars.h"

class C_CSPlayer;
struct lag_record_t;
struct VarMapping_t;
class C_BaseEntity : public IClientEntity
{
public:
	VFUNC( 0, construct(), C_BaseEntity* ( __thiscall* )( void* ) )( )
		VFUNC( 17, get_pred_desc_map(), datamap_t* ( __thiscall* )( void* ) )( )
		NETVARA( get_ent_index, int, CBaseEntity_m_bIsAutoaimTarget, 0x4 )
		NETVAR( get_team, int, CBaseEntity_m_iTeamNum )
		NETVAR( get_origin, Vector, CBaseEntity_m_vecOrigin )
		NETVAR( get_simtime, float, CBaseEntity_m_flSimulationTime )
		NETVARA( get_oldsimtime, float, CBaseEntity_m_flSimulationTime, 4 )
		NETVAR( get_spotted, bool, CBaseEntity_m_bSpotted )
		NETVAR( get_model_idx, int, CBaseEntity_m_nModelIndex )
		NETVAR( get_owner_ent, CBaseHandle, CBaseEntity_m_hOwnerEntity )
		NETVAR( get_rotation, QAngle, CBaseEntity_m_angRotation )
		NETVAR( get_anim_time, float, CBaseEntity_m_flAnimTime )
		NETVAR( get_friction, float, CBasePlayer_m_flFriction )
		NETVAR( get_ducked, bool, CBasePlayer_m_bDucked )
		NETVAR( get_ducking, bool, CBasePlayer_m_bDucking )
		NETVAR( get_old_buttons, int, CBasePlayer_m_nOldButtons )
		NETVAR( get_last_duck_time, float, CBasePlayer_m_flLastDuckTime )
		OFFSET( get_simulation_tick, int, server_offset( sig_simulation_tick ) )
		OFFSET( get_var_mapping, VarMapping_t, server_offset( sig_var_mapping ) )
		DATAMAPVAR( get_move_type, int, "m_MoveType" )
		NETVAR( get_effects, int, CBaseEntity_m_fEffects )
		DATAMAPVAR( get_has_moved_since_last_jump, bool, "m_bHasWalkMovedSinceLastJump" )
		DATAMAPVAR( get_duck_amt, float, "m_flDuckAmount" )
		DATAMAPVAR( get_duck_speed, float, "m_flDuckSpeed" )
		DATAMAPVAR( get_eflags, int, "m_iEFlags" )
		DATAMAPVAR( get_abs_rotation, QAngle, "m_angAbsRotation" )
		DATAMAPVAR( get_data_rotation, QAngle, "m_angRotation" )
		DATAMAPVAR( get_abs_origin, Vector, "m_vecAbsOrigin" )
		DATAMAPVAR( get_data_origin, Vector, "m_vecOrigin" )
		DATAMAPVAR( get_surface_friction, float, "m_surfaceFriction" )

		VFUNC( 11, get_abs_angles(), QAngle& ( __thiscall* )( void* ) )( )
		VFUNC( 96, init_as_client_ent( const char* pszModelName, int renderGroup ), bool( __thiscall* )( void*, const char*, int ) )( pszModelName, renderGroup )
		VFUNC( 101, get_old_origin(), Vector* ( __thiscall* )( void* ) )( )
		VFUNC( 166, is_base_combat_weapon(), bool( __thiscall* )( void* ) )( )
		VFUNC( 166, get_creation_time(), bool( __thiscall* )( void* ) )( )
		MFUNC( set_abs_origin_game( Vector orig ), void( __thiscall* )( void*, const Vector& ), make_offset( "client.dll", sig_set_abs_origin ) )( orig )
		MFUNC( is_breakable_entity(), bool( __thiscall* )( void* ), make_offset( "client.dll", sig_is_breakable_entity ) )( )
		MFUNC( physics_run_think(), bool( __thiscall* )( void*, int ), make_offset( "client.dll", sig_physics_run_think ) )( 0 )
		VFUNC( 75, set_model_idx( int index ), void( __thiscall* )( void*, int ) )( index )
		OFFSET( get_predictable, bool, server_offset( sig_predictable ) + 0x4 )
		VFUNC( 173, should_collide( int collision_group, int contents_mask ), bool( __thiscall* )( void*, int, int ) )( collision_group, contents_mask )
		MFUNC( get_root_move_parent(), C_BaseEntity* ( __thiscall* )( void* ), make_offset( "client.dll", sig_get_root_move_parent ) )( )
		OFFSET( get_cycle_offset, float, server_offset( sig_cycle_offset ) )

		inline bool is_player()
	{
		return GetClientClass()->m_ClassID == ClassId::CCSPlayer;
	}

	inline void set_abs_origin( const Vector& origin, bool full = false )
	{
		get_abs_origin() = origin;
		if ( full && !( get_eflags() & EFL_DIRTY_SPATIAL_PARTITION ) )
		{
			reinterpret_cast< void( __stdcall* )( C_BaseEntity* ) >( make_offset( "client.dll", sig_add_to_dirty_kd_tree ) )( this );
			get_eflags() |= EFL_DIRTY_SPATIAL_PARTITION;
		}
		get_eflags() &= ~EFL_DIRTY_ABSTRANSFORM;
	}
	inline void set_abs_angles( const QAngle& angle )
	{
		get_abs_rotation() = angle;
		get_eflags() &= ~EFL_DIRTY_ABSTRANSFORM;
	}

	model_t*& get_model();
	IClientRenderable* get_renderable();
	bool get_var_bool( const char* var );
	int get_var_int( const char* var );
	float get_var_float( const char* var );
	short get_var_short( const char* var );
	unsigned long get_var_handle( const char* var );
	QAngle get_var_angle( const char* var );
	Vector get_var_vector( const char* var );
	int get_class_id();
	bool is_player_lua();
	bool is_dormant();
	int get_index();
	static C_BaseEntity* from_index( int index );
	static C_BaseEntity* from_handle( CBaseHandle h );
};

class C_EnvTonemapController : public C_BaseEntity
{
public:
	NETVAR( get_use_custom_bloom_scale, bool, CEnvTonemapController_m_bUseCustomBloomScale )
		NETVAR( get_custom_bloom_scale, float, CEnvTonemapController_m_flCustomBloomScale )
		NETVAR( get_use_custom_exposure_min, bool, CEnvTonemapController_m_bUseCustomAutoExposureMin )
		NETVAR( get_use_custom_exposure_max, bool, CEnvTonemapController_m_bUseCustomAutoExposureMax )
		NETVAR( get_custom_exposure_min, float, CEnvTonemapController_m_flCustomAutoExposureMin )
		NETVAR( get_custom_exposure_max, float, CEnvTonemapController_m_flCustomAutoExposureMax )
};

class C_FogController : public C_BaseEntity
{
public:
	NETVAR( get_primary, int, CFogController_m_fog_colorPrimary )
		NETVAR( get_secondary, int, CFogController_m_fog_colorSecondary )
		NETVAR( get_enable, bool, CFogController_m_fog_enable )
};

class C_BaseCombatCharacter : public C_BaseEntity
{
public:
	PNETVAR( get_weapons, CBaseHandle, CBaseCombatCharacter_m_hMyWeapons )
		PNETVAR( get_wearables, CBaseHandle, CBaseCombatCharacter_m_hMyWearables )
		NETVAR( get_active_weapon, CBaseHandle, CBaseCombatCharacter_m_hActiveWeapon )
		NETVAR( get_last_time_of_injury, float, CBaseCombatCharacter_m_flTimeOfLastInjury )
		NETVAR( get_relative_dir_of_injury, Vector, CBaseCombatCharacter_m_nRelativeDirectionOfLastInjury )
};

class C_EconItemView;
class IRefCounted;
class C_BaseAttributableItem : public C_BaseEntity
{
private:
	using str_32 = char[ 32 ];
public:
	NETVAR( get_account_id, int, CBaseAttributableItem_m_iAccountID )
		NETVAR( get_item_id, uint64_t, CBaseAttributableItem_m_iItemIDLow )
		NETVAR( get_item_id_high, uint32_t, CBaseAttributableItem_m_iItemIDHigh )
		NETVAR( get_item_id_low, uint32_t, CBaseAttributableItem_m_iItemIDLow )
		NETVAR( get_entity_quality, int, CBaseAttributableItem_m_iEntityQuality )
		NETVAR( get_custom_name, str_32, CBaseAttributableItem_m_szCustomName )
		NETVAR( get_fallback_paint_kit, unsigned, CBaseAttributableItem_m_nFallbackPaintKit )
		NETVAR( get_fallback_seed, unsigned, CBaseAttributableItem_m_nFallbackSeed )
		NETVAR( get_fallback_wear, float, CBaseAttributableItem_m_flFallbackWear )
		NETVAR( get_fallback_stat_trak, unsigned, CBaseAttributableItem_m_nFallbackStatTrak )
		NETVAR( get_item_definition_index, short, CBaseAttributableItem_m_iItemDefinitionIndex )
		NETVAR( get_xuid_low, int, CBaseAttributableItem_m_OriginalOwnerXuidLow )
		NETVAR( get_xuid_high, int, CBaseAttributableItem_m_OriginalOwnerXuidHigh )
		MFUNC( get_econ_item_view(), C_EconItemView* ( __thiscall* )( void* ), make_offset( "client.dll", sig_get_econ_item_view ) )( )
		const wchar_t* get_diplay_name();

	CUtlVector<IRefCounted*>& get_custom_materials()
	{
		const auto addr = server_offset( sig_custom_materials ) - 0xC;
		return *reinterpret_cast< CUtlVector<IRefCounted*>* >( reinterpret_cast< uintptr_t >( this ) + addr );
	}

	bool& get_custom_material_initialized()
	{
		return *reinterpret_cast< bool* >( reinterpret_cast< uint32_t >( this ) + server_offset( sig_custom_material_initialized ) );
	}
};

class CEconItemDefinition
{
public:
	virtual const char* get_definition_index() = 0;
	virtual const char* get_prefab_name() = 0;
	virtual const char* get_item_base_name() = 0;
	virtual const char* get_item_type_name() = 0;
	virtual const char* get_item_desc() = 0;
	virtual const char* get_inventory_image() = 0;
	int get_loadout_slot()
	{
		return *reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( this ) + 0x24C ); //55 8B EC 8B 45 08 8D 50 FF 83 FA 02
	}
	const wchar_t* GetLocalizedName();
};

class CEconItem;
class C_EconItemView
{
public:
	CEconItemDefinition* get_static_data()
	{
		return reinterpret_cast< CEconItemDefinition * ( __thiscall* )( void* ) >( make_offset( "client.dll", sig_get_static_data ) )( this );
	}

	CUtlVector<IRefCounted*>& get_visual_data_processors()
	{
		return *reinterpret_cast< CUtlVector<IRefCounted*>* >( reinterpret_cast< uintptr_t >( this ) + server_offset( sig_visual_data_processors ) );
	}

	CUtlVector<IRefCounted*>& get_custom_materials()
	{
		return *reinterpret_cast< CUtlVector<IRefCounted*>* >( reinterpret_cast< uintptr_t >( this ) + 0x14 );
	}
};

class C_AttributeManager
{
public:
	char pad_0000[ 64 ]; //0x0000
	C_EconItemView m_Item; //0x0040
};

struct surfacedata_t;
class C_BasePlayer : public C_BaseCombatCharacter
{
public:
	NETVAR( get_lifestate, LifeState, CBasePlayer_m_lifeState )
		NETVAR( get_health, int, CBasePlayer_m_iHealth )
		NETVAR( get_flags, int, CBasePlayer_m_fFlags )
		NETVAR( get_tickbase, int, CBasePlayer_m_nTickBase )
		NETVAR( get_velocity, Vector, CBasePlayer_m_vecVelocity_0_ )
		NETVAR( get_base_velocity, Vector, CBasePlayer_m_vecBaseVelocity )
		NETVAR( get_aim_punch, QAngle, CBasePlayer_m_aimPunchAngle )
		NETVAR( get_aim_punch_vel, Vector, CBasePlayer_m_aimPunchAngleVel )
		NETVAR( get_view_punch, QAngle, CBasePlayer_m_viewPunchAngle )
		NETVAR( get_view_offset, Vector, CBasePlayer_m_vecViewOffset_0_ )
		NETVAR( get_observer_mode, int, CBasePlayer_m_iObserverMode )
		NETVAR( get_observer_target, CBaseHandle, CBasePlayer_m_hObserverTarget )
		NETVAR( get_view_model, CBaseHandle, CBasePlayer_m_hViewModel_0_ )
		NETVAR( get_fov_time, float, CBasePlayer_m_flFOVTime )
		NETVAR( get_fov, int, CBasePlayer_m_iFOV )
		NETVAR( get_fov_start, int, CBasePlayer_m_iFOVStart )
		NETVAR( get_default_fov, int, CBasePlayer_m_iDefaultFOV )
		NETVAR( get_no_interp_parity, uint8_t, CBasePlayer_m_ubEFNoInterpParity )
		NETVAR( get_max_speed, float, CBasePlayer_m_flMaxspeed )
		NETVAR( get_local, CPlayerLocalData, CBasePlayer_m_Local )
		NETVAR( get_step_size, float, CBasePlayer_m_flStepSize )
		NETVARA( get_old_no_interp_parity, uint8_t, CBasePlayer_m_ubEFNoInterpParity, 1 )
		NETVARA( get_tp_angle, QAngle, CBasePlayer_deadflag, 0x4 )
		NETVARRS( get_ammo, int, CBasePlayer_m_iAmmo )
		DATAMAPVAR( get_next_attack, float, "m_flNextAttack" )
		DATAMAPVAR( get_water_level, char, "m_nWaterLevel" )
		DATAMAPVAR( get_ground_entity, CBaseHandle, "m_hGroundEntity" )
		OFFSET( get_spawn_time, float, server_offset( sig_spawn_time ) )
		OFFSET( get_surface_props, surfacedata_t*, server_offset( sig_surface_props ) )
		MFUNC( invalidate_physics_recursive( int physbit ), void( __thiscall* )( void*, int ), make_offset( "client.dll", sig_invalidate_physics_recursive ) )( physbit )
		MFUNC( emit_sound( const char* name ), void( __thiscall* )( void*, const char*, void* ), make_offset( "client.dll", sig_emit_sound ) )( name, this )

		bool get_alive();
	Vector get_eye_pos( bool interpolated = true );
	bool is_enemy();
	player_info_t get_player_info() const;
	std::string get_name() const;
	std::wstring get_name_unicode() const;
	C_CSPlayer* get_observer();
	int get_predicted_health();
};

class alignas( 16 ) ik_context
{
	MFUNC( construct(), void* ( __thiscall* )( void* ), make_offset( "client.dll", sig_ik_context_construct ) )( )
		MFUNC( destruct(), void* ( __thiscall* )( void* ), make_offset( "client.dll", sig_ik_context_destruct ) )( )

public:
	ik_context() { construct(); }
	~ik_context() { destruct(); }
	MFUNC( init( CStudioHdr* const hdr, QAngle* const ang, Vector* const origin, float time, int32_t frame_counter, int32_t bone_mask ),
		void( __thiscall* )( void*, CStudioHdr*, QAngle*, Vector*, float, int32_t, int32_t ), make_offset( "client.dll", sig_ik_context_init ) )( hdr, ang, origin, time, frame_counter, bone_mask )
		MFUNC( update_targets( Vector* pos, QuaternionAligned* q, matrix3x4_t* mat, uint8_t* computed ),
			void( __thiscall* )( void*, Vector*, QuaternionAligned*, matrix3x4_t*, uint8_t* ), make_offset( "client.dll", sig_ik_context_update_targets ) )( pos, q, mat, computed )
		MFUNC( solve_dependencies( Vector* pos, QuaternionAligned* q, matrix3x4_t* mat, uint8_t* computed ),
			void( __thiscall* )( void*, Vector*, QuaternionAligned*, matrix3x4_t*, uint8_t* ), make_offset( "client.dll", sig_ik_context_solve_dependencies ) )( pos, q, mat, computed )

private:
	uint32_t gap[ 0x41A ]{};

public:
	int32_t mask{};
};

struct alignas( 16 ) bone_snapshot
{
	C_BaseEntity* player{};
	float weight{};
	matrix3x4_t cache[ 128 ];
	const char* weight_list_name{};
	float weight_list[ 128 ]{};
	bool weight_list_init{};
	bool enabled{};
	bool capture_pending{};
	float ideal_duration{};
};

class CBoneAccessor;
class CBoneCache;
class CStudioHdr;
class CThreadFastMutex;
class C_CSRagdoll;
class C_BaseAnimating : public C_BasePlayer
{
public:

	NETVAR( get_hitbox_set, int, CBaseAnimating_m_nHitboxSet )
		ANETVAR( get_pose_params, float, 24, CBaseAnimating_m_flPoseParameter )
		NETVAR( get_model_scale, float, CBaseAnimating_m_flModelScale )
		NETVAR( get_body, int, CBaseAnimating_m_nBody )
		NETVAR( get_reset_events_parity, int, CBaseAnimating_m_nResetEventsParity )
		NETVARA( get_prev_reset_events_parity, int, CBaseAnimating_m_nResetEventsParity, 0x8 )
		NETVARA( get_attachments, CUtlVector<CAttachmentData>, CBaseAnimating_m_hLightingOrigin, -0x4 )
		OFFSET( get_model_ptr, CStudioHdr*, server_offset( sig_studio_hdr ) )
		POFFSET( get_bone_accessor, CBoneAccessor, server_offset( sig_bone_accessor ) )
		NETVAR( get_sequence, int, CBaseAnimating_m_nSequence )
		NETVAR( get_playback_rate, float, CBaseAnimating_m_flPlaybackRate )
		NETVAR( get_cycle, float, CBaseAnimating_m_flCycle )
		NETVAR( get_new_sequence_parity, int, CBaseAnimating_m_nNewSequenceParity )
		NETVAR( get_clientside_animation, bool, CBaseAnimating_m_bClientSideAnimation )
		NETVARA( get_prev_new_sequence_parity, int, CBaseAnimating_m_nNewSequenceParity, 0x8 )
		ANETVAR( get_ragdoll_pos, float, 24, CRagdollProp_m_ragPos_0_ )
		MFUNC( allocate_and_merge_bones( int32_t mask ), void( __thiscall* )( void*, int32_t ), make_offset( "client.dll", sig_allocate_and_merge_bones ) )( mask )
		OFFSET( get_ragdoll, C_CSRagdoll*, server_offset( sig_ragdoll ) )
		OFFSET( get_lod_data, lod_data, server_offset( sig_lod_data ) )
		NETVARA( get_use_fastpath, bool, CBaseAnimating_m_flFrozen, 0x4 )
		VFUNC( 192, update_ik_locks( float time ), void( __thiscall* )( void*, float ) )( time )
		VFUNC( 193, calculate_ik_locks( float time ), void( __thiscall* )( void*, float ) )( time )
		OFFSET( get_ik_context, ik_context*, server_offset( sig_ik_context ) + 0x4 )
		OFFSET( get_snapshot_front, bone_snapshot, server_offset( sig_snapshot_front ) )
		OFFSET( get_snapshot_back, bone_snapshot, server_offset( sig_snapshot_back ) )
		OFFSET( get_blend_tick, int, server_offset( sig_blend_tick ) )
		VFUNC( 112, reset_latched(), void( __thiscall* )( void* ) )( )
		VFUNC( 220, set_sequence( int seq ), void( __thiscall* )( void*, int ) )( seq )
		VFUNC( 221, frame_advance( float amt ), void( __thiscall* )( void*, float ) )( amt )
		VFUNC( 222, get_sequence_cycle_rate( CStudioHdr* hdr, int32_t seq ), float( __thiscall* )( void*, CStudioHdr*, int32_t ) )( hdr, seq )
		MFUNC( lookup_sequence( const char* name ), int32_t( __thiscall* )( void*, const char* ), make_offset( "client.dll", sig_lookup_sequence ) )( name )
		MFUNC( follow_entity( C_BaseEntity* ent, bool bBoneMerge ), void( __thiscall* )( void*, C_BaseEntity*, bool ), make_offset( "client.dll", sig_get_follow_entity ) )( ent, bBoneMerge )
		int get_sequence_act( int sequence );
	void set_cycle( int fcycle );

};

class C_BaseCombatWeapon : public C_BaseAnimating
{
public:
	NETVAR( get_next_primary_attack, float, CBaseCombatWeapon_m_flNextPrimaryAttack )
		NETVAR( get_next_secondary_attack, float, CBaseCombatWeapon_m_flNextSecondaryAttack )
		NETVAR( get_postpone_fire_ready_time, float, CWeaponCSBase_m_flPostponeFireReadyTime )
		NETVAR( get_clip1, int, CBaseCombatWeapon_m_iClip1 )
		NETVAR( get_max_clip, int, CBaseCombatWeapon_m_iPrimaryReserveAmmoCount )
		NETVARA( in_reload, bool, CBaseCombatWeapon_m_flNextPrimaryAttack, 0x6D )
		NETVAR( get_throw_time, float, CBaseCSGrenade_m_fThrowTime )
		NETVAR( get_throw_strength, float, CBaseCSGrenade_m_flThrowStrength )
		NETVAR( get_pin_pulled, bool, CBaseCSGrenade_m_bPinPulled )
		NETVAR( get_thrower, CBaseHandle, CBaseGrenade_m_hThrower )
		NETVAR( get_world_model_handle, CBaseHandle, CBaseCombatWeapon_m_hWeaponWorldModel )
		NETVAR( get_world_model_index, int, CBaseCombatWeapon_m_iWorldModelIndex )
		NETVAR( get_view_model_index, int, CBaseCombatWeapon_m_iViewModelIndex )
		NETVAR( get_world_dropped_model_index, int, CBaseCombatWeapon_m_iWorldDroppedModelIndex )
		NETVAR( get_time_weapon_idle, float, CBaseCombatWeapon_m_flTimeWeaponIdle )
		OFFSET( get_last_client_fire_bullet_time, float, server_offset( sig_last_client_fire_bullet_time ) )
		OFFSET( get_iron_sight_controller, uintptr_t, server_offset( sig_get_iron_sight_controller ) )
		VFUNC( 483, get_inaccuracy(), float( __thiscall* )( void* ) )( )
		VFUNC( 453, get_spread(), float( __thiscall* )( void* ) )( )
		VFUNC( 484, update_accuracy(), void( __thiscall* )( void* ) )( )
		VFUNC( 461, get_wpn_data(), CCSWeaponData* ( __thiscall* )( void* ) )( )
		VFUNC( 442, get_max_speed(), float( __thiscall* )( void* ) )( )
		VFUNC( 455, get_weapon_type(), int( __thiscall* )( void* ) )( )

		bool is_grenade();
	int get_weapon_id();
	bool is_knife();
	bool is_shield();
	bool is_secondary_attack_weapon();
	bool is_zeus();
	bool is_shootable();
	bool is_gun();
	CCSWeaponData* get_wpn_data_safe();
	C_EconItemView* cmbat_get_econ_item_view()
	{
		return reinterpret_cast< C_EconItemView * ( __thiscall* )( void* ) >( make_offset( "client.dll", sig_cmbat_get_econ_item_view ) )( this );
	}
};

class C_WeaponCSBase : public C_BaseCombatWeapon
{
public:
	NETVAR( get_last_shot_time, float, CWeaponCSBase_m_fLastShotTime )
		NETVAR( get_accuracy_penalty, float, CWeaponCSBase_m_fAccuracyPenalty )
		NETVAR( get_recoil_index, float, CWeaponCSBase_m_flRecoilIndex )
};

class C_WeaponCSBaseGun : public C_WeaponCSBase
{
public:
	NETVAR( get_zoom_level, int, CWeaponCSBaseGun_m_zoomLevel )
		VFUNC( 445, get_zoom_in_sound(), const char* ( __thiscall* )( void* ) )( )
		VFUNC( 446, get_zoom_out_sound(), const char* ( __thiscall* )( void* ) )( )
};

class C_BaseWeaponWorldModel : public C_BaseAnimating
{
public:
	bool holds_player_animations();
	NETVAR( get_parent, CBaseHandle, CBaseWeaponWorldModel_m_hCombatWeaponParent )
		NETVAR( get_base_weapon_world_model_index, int, CBaseWeaponWorldModel_m_nModelIndex )
};

struct mstudiobone_t;
class C_CSRagdoll : public C_BaseAnimating
{
public:
	NETVAR( get_player_handle, CBaseHandle, CCSRagdoll_m_hPlayer )
		VFUNC( 1, bone( C_BaseEntity* const ent, mstudiobone_t* const bones, int32_t count, bool* const simulated, CBoneAccessor* const accessor ),
				void( __thiscall* )( void*, C_BaseEntity*, mstudiobone_t*, int32_t, bool*, CBoneAccessor* ) )( ent, bones, count, simulated, accessor )
};

struct command_context
{
	bool needs_processing;
	CUserCmd cmd;
	int32_t    cmd_number;
};

struct ammo
{
	char* name;
	int32_t dmg_type;
	int32_t tracer_type;
	int32_t min_splash;
	int32_t max_splash;
	int32_t flags;
	int32_t plr_dmg;
	int32_t npc_dmg;
	int32_t max_carry;
	int32_t force_impulse;
	ConVar* cplr_dmg;
	ConVar* cnpc_dmg;
	ConVar* cmax_carry;
	ConVar* cforce_impulse;
};

class CAmmoDef
{
	int32_t pad;
	int32_t ind;
	ammo type[ 32 ];

public:
	static CAmmoDef* get()
	{
		return reinterpret_cast< CAmmoDef * ( __stdcall* )( ) >( make_offset( "client.dll", sig_ammodef_get ) )( );
	}

	int index( uint32_t search ) const
	{
		for ( auto i = 1; i < ind; i++ )
			if ( fnv1a_rt( type[ i ].name ) == search )
				return i;

		return -1;
	}

	bool can_carry_infinite_ammo( uint32_t index ) const
	{
		if ( index < 1 || index >= ind )
			return false;

		auto max_carry = type[ index ].max_carry;
		if ( max_carry == -1 && type[ index ].cmax_carry )
			max_carry = type[ index ].cmax_carry->get_int();

		return max_carry == -2;
	}
};

class CCSGOPlayerAnimState;
class QuaternionAligned;
class Quaternion;
struct C_AnimationLayer;
class CIKContext;
struct studiohdr_t;
class C_CSPlayer : public C_BaseAnimating
{
public:
	NETVAR( get_lby, float, CCSPlayer_m_flLowerBodyYawTarget )
		NETVAR( get_helmet, bool, CCSPlayer_m_bHasHelmet )
		NETVAR( get_armor, int, CCSPlayer_m_ArmorValue )
		NETVAR( get_heavy_armor, bool, CCSPlayer_m_bHasHeavyArmor )
		NETVAR( get_eye_angles, QAngle, CCSPlayer_m_angEyeAngles )
		NETVAR( get_flash_alpha, float, CCSPlayer_m_flFlashMaxAlpha )
		NETVAR( get_flash_duration, float, CCSPlayer_m_flFlashDuration )
		NETVARA( get_flash_amount, float, CCSPlayer_m_flFlashMaxAlpha, -0x8 )
		NETVAR( get_scoped, bool, CCSPlayer_m_bIsScoped )
		NETVAR( get_immunity, bool, CCSPlayer_m_bGunGameImmunity )
		NETVAR( get_render_clr, Color, CBaseEntity_m_clrRender )
		NETVAR( has_defuser, bool, CCSPlayer_m_bHasDefuser )
		NETVAR( is_ghost, bool, CCSPlayer_m_bIsPlayerGhost )
		NETVAR( get_money, int, CCSPlayer_m_iAccount )
		NETVAR( get_survival_team, short, CCSPlayer_m_nSurvivalTeam )
		NETVAR( get_velocity_modifier, float, CCSPlayer_m_flVelocityModifier )
		NETVAR( get_tp_recoil, float, CCSPlayer_m_flThirdpersonRecoil )
		NETVAR( get_wait_for_no_attack, bool, CCSPlayer_m_bWaitForNoAttack )
		NETVAR( get_move_state, int, CCSPlayer_m_iMoveState )
		NETVAR( get_strafing, bool, CCSPlayer_m_bStrafing )
		NETVAR( get_walking, bool, CCSPlayer_m_bIsWalking )
		NETVAR( get_addon_bits, int, CCSPlayer_m_iAddonBits )
		NETVAR( get_primary_addon, int, CCSPlayer_m_iPrimaryAddon )
		NETVAR( get_secondary_addon, int, CCSPlayer_m_iSecondaryAddon )
		NETVAR( is_looking_at_weapon, bool, CCSPlayer_m_bIsLookingAtWeapon )
		NETVAR( is_holding_looking_at_weapon, bool, CCSPlayer_m_bIsHoldingLookAtWeapon )
		NETVAR( get_stamina, float, CCSPlayer_m_flStamina )
		NETVAR( get_ground_accel_linear_frac_last_time, float, CCSPlayer_m_flGroundAccelLinearFracLastTime )
		OFFSET( get_final_predicted_tick, int, server_offset( sig_final_predicted_tick ) )
		OFFSET( get_occlusion_mask, int, server_offset( sig_occlusion_mask ) )
		APOFFSET( get_anim_layers, C_AnimationLayer, 13, server_offset_multi( sig_anim_layers ) )
		OFFSET( get_anim_layer_count, int, server_offset_multi( sig_anim_layers ) + 0x10 )
		VFUNC( 170, eye_angles_virtual(), QAngle* ( __thiscall* )( void* ) )( )
		OFFSET( get_anim_state, CCSGOPlayerAnimState*, server_offset( sig_anim_state ) )
		OFFSET( is_ragdoll, bool, server_offset( sig_is_ragdoll ) )
		MFUNC( post_build_transformations( matrix3x4_t* out, uint32_t mask ), void( __thiscall* )( void*, matrix3x4_t*, uint32_t ), make_offset( "client.dll", sig_post_build_transformations ) )( out, mask )
		FUNC( standard_blending_rules( CStudioHdr* hdr, Vector* pos, QuaternionAligned* q, float curTime, int boneMask ), void( __thiscall* )( void*, CStudioHdr*, Vector*, QuaternionAligned*, float, int ), make_offset( "client.dll", sig_standard_blending_rules ) )( this, hdr, pos, q, curTime, boneMask )
		FUNC( build_transformations( CStudioHdr* hdr, Vector* pos, QuaternionAligned* q, const matrix3x4_t& cameraTransform, int boneMask, byte* boneComputed ), void( __thiscall* )( void*, CStudioHdr*, Vector*, QuaternionAligned*, matrix3x4_t const&, int, byte* ), make_offset( "client.dll", sig_build_transformations ) )( this, hdr, pos, q, cameraTransform, boneMask, boneComputed )
		VFUNC( 223, get_layer_sequence_cycle_rate( C_AnimationLayer* layer, int sequence ), float( __thiscall* )( void*, C_AnimationLayer*, int ) )( layer, sequence )
		MFUNC( get_any_sequence_anim_tag( int32_t sequence, int32_t tag ), int32_t( __thiscall* )( void*, int32_t, int32_t ), make_offset( "client.dll", sig_get_any_sequence_anim_tag ) )( sequence, tag )
		MFUNC( accumulate_interleaved_dispatch_layers( C_BaseEntity* world_model, void** setup, Vector* pos, Quaternion* q, float time, bool is_local ), void( __thiscall* )( void*, C_BaseEntity*, void**, Vector*, Quaternion*, float, bool ), make_offset( "client.dll", sig_accumulate_interleaved_dispatch_layers ) )( world_model, setup, pos, q, time, is_local )
		MFUNC( update_addon_models( const bool force ), int( __thiscall* )( void*, bool ), make_offset( "client.dll", sig_update_addon_models ) )( force )
		OFFSET( get_command_context, command_context, server_offset( sig_command_context ) )
		VFUNC( 263, owns_this_type( const char* type ), C_WeaponCSBaseGun* ( __thiscall* )( void*, const char*, int32_t ) )( type, 0 )
		VFUNC( 264, get_slot( int32_t slot ), C_WeaponCSBaseGun* ( __thiscall* )( void*, int32_t ) )( slot )
		OFFSET( get_collision_viewheight, float, server_offset( sig_collision_viewheight ) )
		OFFSET( get_collision_bounds_change_time, float, server_offset( sig_collision_bounds_change_time ) )
		OFFSET( get_most_recent_model_bone_counter, uint32_t, server_offset( sig_most_recent_model_bone_counter ) )
		OFFSET( get_accumulated_bone_mask, uint32_t, server_offset( sig_accumulated_bone_mask ) )
		OFFSET( get_assassination_target_addon, CBaseHandle, server_offset( sig_assassination_target_addon ) )

		DATAMAPVAR( get_cycle, float, "m_flCycle" )

		static bool setup_bones( lag_record_t& record, resolver_direction dir, bool extrapolated = false );
	void set_pose_params( std::array<float, 24>& params );
	void set_ragdoll_pos( std::array<float, 24>& ragposes );
	Vector world_space_center();
	Vector get_hitbox_pos( int hitbox );
	std::tuple<Vector, Vector, float> get_hitbox_minmax( int hitbox );
	bool is_peeking() const;
	void on_simulation_time_changing( float previous, float current );
	int32_t get_ammo_count( uint32_t hash );
	void try_initiate_animation( size_t layer, int32_t activity, std::vector<uint16_t>& modifiers, int seed );
	inline bool is_valid( bool not_dormant = false )
	{
		return get_alive() && !is_ragdoll() && ( !is_dormant() || not_dormant )
			&& EntIndex() >= 1 && EntIndex() <= 65;
	}

	static C_CSPlayer* local_player();
	static C_CSPlayer* from_userid( int id );
	static C_CSPlayer* player_from_index( int i );
};

class C_PlantedC4 : public C_BaseEntity
{
public:
	NETVAR( get_defuser, int32, CPlantedC4_m_hBombDefuser )
		NETVAR( get_defused, bool, CPlantedC4_m_bBombDefused )
		NETVAR( get_ticking, bool, CPlantedC4_m_bBombTicking )
		NETVAR( get_timer, float, CPlantedC4_m_flC4Blow )
		NETVAR( get_defuse_countdown, float, CPlantedC4_m_flDefuseCountDown )
		NETVAR( get_defuse_length, float, CPlantedC4_m_flDefuseLength )
};

class C_SmokeGrenadeProjectile : public C_BaseEntity
{
public:
	NETVAR( get_smoke_tick_begin, int32, CSmokeGrenadeProjectile_m_nSmokeEffectTickBegin )
};

class C_BaseCSGrenadeProjectile : public C_BaseEntity
{
public:
	NETVAR( get_explode_tick_begin, int32, CBaseCSGrenadeProjectile_m_nExplodeEffectTickBegin )
		NETVARA( get_spawn_time, float, sig_projectile_throwtime, 0x4 )
};

class C_BaseViewModel : public C_BaseAnimating
{
public:
	NETVAR( get_owner, int32, CBaseViewModel_m_hOwner )
		NETVAR( get_weapon, CBaseHandle, CBaseViewModel_m_hWeapon )
		DATAMAPVAR( get_cycle, float, "m_flCycle" )
		DATAMAPVAR( get_sequence, int, "m_nSequence" )
		DATAMAPVAR( get_animation_parity, int, "m_nAnimationParity" )
		MFUNC( send_matching_sequence( int32_t sequence ), void( __thiscall* )( void*, int32_t ), make_offset( "client.dll", sig_send_matching_sequence ) )( sequence )
};

class CInferno : public C_BaseEntity
{
public:
	OFFSET( spawn_time, float, 0x20 );
};

class C_DangerZoneController
{
public:
	NETVAR( is_enabled, bool, CDangerZoneController_m_bDangerZoneControllerEnabled )
};