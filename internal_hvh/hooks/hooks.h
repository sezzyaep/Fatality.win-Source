#pragma once

namespace hook
{
	struct hook_t
	{
		void apply()
		{
			if ( !applied_ )
			{
				if ( hook_->is_detour() )
					original_ = hook_->apply( new_func_ );
				else
					original_ = hook_->apply( index_, new_func_ );
				applied_ = true;
			}
		}

		template<typename fn>
		fn get_original() volatile
		{
			return reinterpret_cast< fn >( original_ );
		}

		hook_t() = default;
		hook_t( const uintptr_t new_func, const uintptr_t original ) : original_( original ), new_func_( new_func ), index_( 0 ), applied_( false )
		{
			hook_ = std::make_shared<c_detour>( original );
		}

		hook_t( const uintptr_t new_func, const uint32_t index, const uintptr_t ent ) : new_func_( new_func ), index_( index ), applied_( false )
		{
			hook_ = std::make_shared<c_vtable_hook>( ent );
		}

		std::shared_ptr<c_hook> hook_;
		uintptr_t original_{};
		uintptr_t new_func_;
		uint32_t index_;
		bool applied_;
	};

	inline std::unordered_map<uintptr_t, hook_t> hook_list;

	// functions
	int __fastcall start_sound_immediate( start_sound_params* params, int edx );
	int __fastcall set_bodygroup( void* ecx, void* edx, int group, int value );
	void __fastcall modify_eye_pos( void* ecx, void* edx, Vector* pos );
	LRESULT __stdcall wnd_proc( HWND, UINT, WPARAM, LPARAM );
	void __stdcall create_move( int sequence_number, float input_sample_frametime, bool active );
	void cl_move_naked();
	void __stdcall frame_stage_notify( ClientFrameStage_t );
	void __fastcall on_latch_interp_var( C_BaseEntity* ent, void* edx, int flags );
	void __fastcall draw_model_execute( IVModelRender*, int edx, IMatRenderContext*, DrawModelState_t&, ModelRenderInfo_t&, matrix3x4_t* );
	void __fastcall draw_model_studio( IStudioRender* studio_render, int edx, void* results, DrawModelInfo_t* info, matrix3x4_t* bone_to_world, float* flex_weights, float* flex_delayed_weights, Vector& model_origin, int flags );
	void __fastcall render_view( void*, void*, CViewSetup&, CViewSetup&, unsigned int, uint8_t );
	void __fastcall level_init_post_entity( void*, void* );
	void __fastcall level_init_pre_entity( void* ecx, void* edx, char* map_name );
	void __fastcall draw_3d_debug_overlays( void* ecx, void* edx );
	void __fastcall render_glow_boxes( CGlowObjectManager* manager, void* edx, int pass, CMatRenderContextPtr& render_ctx );
	void __fastcall shader_api_draw_mesh( void* ecx, void* edx, uintptr_t pMesh, int nCount, MeshInstanceData_t* pInstances, int nCompressionType, uintptr_t pCompiledState, uintptr_t pInfo );
	int	__stdcall is_box_visible( const Vector&, const Vector& );
	void __fastcall scene_end( void*, void* );
	void _stdcall calc_renderable_world_space_aabb_bloated( RenderableInfo_t* info, Vector& absMins, Vector& absMaxs );
	void __stdcall add_renderables_to_render_lists( const SetupRenderInfo_t& info, int nCount, RenderableInfo_t** ppRenderables, BuildRenderListInfo_t* pRLInfo, int nDetailCount, void* pDetailInfo );
	bool _fastcall do_post_screen_space_effects( void*, void*, CViewSetup* );
	long __stdcall present( IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA* );
	long __stdcall reset( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
	bool  __fastcall send_net_msg( void* netchan, void* edx, INetMessage& msg, bool reliable, bool voice );
	void __stdcall override_view( CViewSetup* setup );
	void __fastcall paint_traverse( void* pPanels, int edx, unsigned int panel, bool forceRepaint, bool allowForce );
	IMaterial* __fastcall find_material( void* thisptr, void* edx, const char* material_name, const char* texture_group, bool complain, const char* complain_prefix );
	int __fastcall emit_sound( void* thisptr, int edx, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk );
	bool __fastcall is_connected( void* ecx, void* edx );
	void __fastcall lock_cursor( void* ecx, void* edx );
	void __fastcall draw_set_color( void* ecx, void* edx, int r, int g, int b, int a );
	bool __fastcall setup_bones( void* ecx, void* edx, matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime );
	bool __fastcall animating_setup_bones( void* ecx, void* edx, matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime );
	void __fastcall reevauluate_anim_lod( C_CSPlayer* player, uint32_t edx, uint32_t bone_mask );
	void __fastcall post_data_update( void* ecx, void* edx, int type );
	void __fastcall maintain_sequence_transitions( C_BaseAnimating* ent, uintptr_t edx, uintptr_t bone_setup, Vector* pos, Quaternion* q );
	void __fastcall trace_ray( void*, void*, Ray_t&, unsigned int, ITraceFilter*, trace_t* );
	void __fastcall run_command( void* ecx, void* edx, C_BasePlayer* player, CUserCmd* cmd, IMoveHelper* move_helper );
	void __fastcall process_movement( void* ecx, void* edx, C_CSPlayer* player, CMoveData* data );
	int __fastcall transfer_data( CPredictionCopy* copy, void* edx, char* operation, int entindex, datamap_t* dmap );
	bool __fastcall perform_prediction( uintptr_t prediction, uint32_t edx, int32_t slot, C_CSPlayer* player, bool recv_update, int32_t ack, int32_t out );
	bool __fastcall MsgFunc_TextMsg( void* ecx, void* edx, void* msg );
	void __fastcall packet_start( IClientState* state, uint32_t, int incoming_sequence, int outgoing_acknowledged );
	void __fastcall level_shutdown( void* ecx, void* edx );
	void __fastcall physics_simulate( C_CSPlayer* const player, const uint32_t edx );
	void __fastcall estimate_abs_velocity( C_CSPlayer* player, int edx, Vector* out );
	void __fastcall send_weapon_anim( C_WeaponCSBaseGun* weapon, uint32_t edx, uint32_t act );
	void __fastcall fire_event( C_CSPlayer* player, uint32_t edx, Vector* origin, Vector* angles, int event, const char* options );
	void __fastcall do_animation_events( C_CSPlayer* player, uint32_t edx, CStudioHdr* hdr );
	void __fastcall do_animation_events_animating( C_BaseAnimating* animating, uint32_t edx, CStudioHdr* hdr );
	bool __fastcall deploy( C_WeaponCSBaseGun* wpn, uint32_t edx );
	double __fastcall get_fov( C_CSPlayer* player, uintptr_t edx );
	int __fastcall get_default_fov( C_CSPlayer* player, uintptr_t edx );
	void __fastcall obb_change_callback( C_CSPlayer* player, uint32_t edx, Vector* old_mins, Vector* mins, Vector* old_maxs, Vector* maxs );
	void __fastcall server_cmd_key_values( void* ecx, void* edx, KeyValues* key_values );
	void __fastcall set_abs_angles( void* ecx, void* edx, QAngle& angles );
	void __fastcall calc_view( C_CSPlayer* player, uint32_t edx, Vector* eye, QAngle* ang, float* znear, float* zfar, float* fov );
	void __stdcall scale_mouse( int slot, float* mouse_x, float* mouse_y );
	void __fastcall start_sound( IMoveHelper* move_helper, int edx, const Vector& origin, const char* soundname );
	void __cdecl convar_network_change_callback( ConVar* var, const char* old1, float old2 );
	void __fastcall update_clientside_anim( C_CSPlayer* player, void* edx );
	void __fastcall ent_info_list_link_before( ent_info_list_t* list, uint32_t edx, ent_info_t* before, ent_info_t* element );
	int __stdcall extract_occluded_renderables( int32_t view_id, int32_t count, RenderableInfo_t** renderables, uintptr_t rl_info );
	bool __fastcall SVCMsg_VoiceData( void* ecx, void* edx, CSVCMsg_VoiceData* msg );
	int __fastcall hud_reticle_process_input( void* ecx, void* edx );
	void __fastcall update_interp_vars( C_BaseAnimating* animating, void* edx );
	int __fastcall particle_draw_model( void* ecx, void* edx, int flags, const RenderableInstance_t& instance );

	// netprops
	void __cdecl effects( CRecvProxyData* data, void* entity, void* output );
	void __cdecl sequence( CRecvProxyData* data, void* entity, void* output );
	void __cdecl weapon_handle( CRecvProxyData* data, void* entity, void* output );
	void __cdecl simulation_time( CRecvProxyData* pData, void* entity, void* output );
	void __cdecl general_float( CRecvProxyData* pData, void* entity, void* output );
	void __cdecl general_vec( CRecvProxyData* pData, void* entity, void* output );
	void __cdecl general_int( CRecvProxyData* pData, void* entity, void* output );
	void __cdecl general_bool( CRecvProxyData* pData, void* entity, void* output );
	void __cdecl layer_sequence( CRecvProxyData* pData, void* entity, void* output );
	void __cdecl layer_cycle( CRecvProxyData* pData, void* entity, void* output );
	void __cdecl layer_playback_rate( CRecvProxyData* pData, void* entity, void* output );
	void __cdecl layer_weight( CRecvProxyData* pData, void* entity, void* output );
	void __cdecl layer_weight_delta_rate( CRecvProxyData* pData, void* entity, void* output );
	void __cdecl layer_order( CRecvProxyData* pData, void* entity, void* output );

	// convars
	int __fastcall convar_get_int_client( ConVar*, void* );
	int __fastcall convar_get_int_engine( ConVar*, void* );
	float __fastcall convar_get_float_engine( ConVar*, void* );

	// vac
	int32_t __stdcall get_module_handle_ex_a( uint32_t flags, const char* str, uint32_t* mod );

	// lua
	void __fastcall client_cmd_unrestricted( IEngineClient* client, int edx, const char* szCmdString, const char* newFlag = 0 );

	// exceptions
	NTSTATUS NTAPI nt_query_virtual_memory( HANDLE process_handle, PVOID base_address, int memory_information_class, PMEMORY_BASIC_INFORMATION mem_information, ULONG length, PULONG result_length );

#ifdef _DEBUG
	// debug
	void __fastcall run_command_server( void* ecx, void* edx, C_BasePlayer* player, CUserCmd* cmd, IMoveHelper* move_helper );
#endif


	void init();
	void unhook();
}

#ifndef RELEASE
#define make_hook_dbg( new_func, target ) hook::hook_list.insert( std::make_pair<>( ( uintptr_t ) ( new_func ), hook::hook_t( ( uintptr_t ) ( new_func ), target ) ) )
#endif
#define make_hook_direct( new_func, target ) hook::hook_list.insert( std::make_pair<>( ( uintptr_t ) ( new_func ), hook::hook_t( ( uintptr_t ) ( new_func ), ( uintptr_t ) ( target ) ) ) )
#define make_hook( new_func, target_module, target ) hook::hook_list.insert( std::make_pair<>( ( uintptr_t ) ( new_func ), hook::hook_t( ( uintptr_t ) ( new_func ), make_offset_once( target_module, target ) ) ) )
#define make_hook_virt( new_func, index, target ) hook::hook_list.insert( std::make_pair<>( ( uintptr_t ) ( target ), hook::hook_t( ( uintptr_t ) ( new_func ), index, ( uintptr_t ) ( target ) ) ) )
#define make_hook_multi( new_func, target_module, target ) hook::hook_list.insert( std::make_pair<>( ( uintptr_t ) ( new_func ), hook::hook_t( ( uintptr_t ) ( new_func ), make_offset_multi( target_module, target ) ) ) )
#define original( func ) []() { static volatile hook::hook_t* ret = &hook::hook_list[ ( uintptr_t ) ( func ) ]; return ret->get_original<decltype( &( func ) )>(); }()
#define original_vfunc( inst, func ) [inst]() { static volatile hook::hook_t* ret = &hook::hook_list[ ( uintptr_t ) ( inst ) ]; return ret->get_original<decltype( &( func ) )>(); }()