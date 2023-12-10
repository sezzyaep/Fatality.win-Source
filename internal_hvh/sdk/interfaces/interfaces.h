#pragma once

#include "ILocalize.h"
#include "IBaseClientDll.h"
#include "IClientEntityList.h"
#include "IClientModeShared.h"
#include "IEngineClient.h"
#include "IMoveHelper.h"
#include "IPrediction.h"
#include "IGlobalVarsBase.h"
#include "IVModelInfo.h"
#include "IVModelRender.h"
#include "IMaterialSystem.h"
#include "IMatRenderContext.h"
#include "IVRenderView.h"
#include "ICVar.h"
#include "IViewRender.h"
#include "IViewRenderBeams.h"
#include "IInput.h"
#include "IGameEventManager.h"
#include "IPhysicsSurfaceProps.h"
#include "IEngineTrace.h"
#include "IClientState.h"
#include "ISurface.h"
#include "IVDebugOverlay.h"
#include "CGlowObjectManager.h"
#include "IVPanel.h"
#include "IWeaponSystem.h"
#include "CHud.h"
#include "CSPlayerResource.h"
#include "IVEffects.h"
#include "IStudioRender.h"
#include "IToolEntity.h"
#include "../networkstringtable.h"
#include "IBaseFileSystem.h"
#include "../panorama.h"

#ifndef INTERFACES_H
#define INTERFACES_H

#define make_interface( name, type, offset_name, module_name ) inline auto name = []() __attribute__((always_inline)) { return encrypted_ptr<type>(( type* ) ( make_offset_multi( module_name, offset_name ) )); }
#define make_interface_no_enc( name, type, offset_name, module_name ) inline auto name = []() __attribute__((always_inline)) { return( type* ) ( make_offset_multi( module_name, offset_name ) ); }
#define make_interface_alt( name, type, offset_name, module_name ) inline auto name = []() __attribute__((always_inline)) { return encrypted_ptr<type>(*( type** ) ( make_offset_multi( module_name, offset_name )) ); }
#define make_interface_alt_no_enc( name, type, offset_name, module_name ) inline auto name = []() __attribute__((always_inline)) { return *( type** ) ( make_offset_multi( module_name, offset_name ) ); }
#define make_interface_func( name, type, offset_name, module_name ) inline auto name = []() __attribute__((always_inline)) { return encrypted_ptr<type>(reinterpret_cast< uintptr_t( * )( ) >( make_offset_multi( module_name, offset_name ) )( ) ); };

namespace interfaces
{
	make_interface( client, IBaseClientDll, interface_client, "client.dll" );
	make_interface_no_enc( entity_list, IClientEntityList, interface_entity_list, "client.dll" );
	make_interface( engine, IEngineClient, interface_engine, "engine.dll" );
	make_interface( prediction, IPrediction, interface_prediction, "client.dll" );
	make_interface( move_helper, IMoveHelper, interface_move_helper, "client.dll" );
	make_interface( game_movement, CGameMovement, interface_game_movement, "client.dll" );
	make_interface_no_enc( globals, IGlobalVarsBase, interface_globals, "engine.dll" );
	make_interface( model_info, IVModelInfo, interface_model_info, "engine.dll" );
	make_interface( model_render, IVModelRender, interface_model_render, "engine.dll" );
	make_interface( material_system, IMaterialSystem, interface_material_system, "materialsystem.dll" );
	make_interface( render_view, IVRenderView, interface_render_view, "engine.dll" );
	make_interface( cvar, ICVar, interface_cvar, "vstdlib.dll" );
	make_interface( view_render, IViewRender, interface_view_render, "client.dll" );
	make_interface( view_render_beams, IViewRenderBeams, interface_view_render_beams, "client.dll" );
	make_interface( game_event_manager, IGameEventManager2, interface_game_event_manager, "engine.dll" );
	make_interface( input, IInput, interface_input, "client.dll" );
	make_interface( surface_props, IPhysicsSurfaceProps, interface_surface_props, "vphysics.dll" );
	make_interface( engine_trace, IEngineTrace, interface_engine_trace, "engine.dll" );
	make_interface_alt_no_enc( client_state, IClientState, interface_client_state, "engine.dll" );
	make_interface( surface, ISurface, interface_surface, "vguimatsurface.dll" );
	make_interface( memalloc, IMemAlloc, interface_memalloc, "tier0.dll" );
	make_interface( debug_overlay, IVDebugOverlay, interface_debug_overlay, "engine.dll" );
	make_interface( glow_object_manager, CGlowObjectManager, interface_glow_object_manager, "client.dll" );
	make_interface( panel, IVPanel, interface_panel, "vgui2.dll" );
	make_interface( engine_sound, IEngineSound, interface_engine_sound, "engine.dll" );
	make_interface( game_rules_proxy, C_CSGameRulesProxy*, interface_game_rules_proxy, "client.dll" );
	make_interface( weapon_system, IWeaponSystem, interface_weapon_system, "client.dll" );
	make_interface( localize, ILocalize, interface_localize, "localize.dll" );
	make_interface( hud, CHud, interface_hud, "client.dll" );
	make_interface( player_resource, C_CSPlayerResource*, interface_player_resource, "client.dll" );
	make_interface( client_leaf_system, IClientLeafSystem, interface_client_leaf_system, "client.dll" );
	make_interface( effects, IVEffects, interface_effects, "engine.dll" );
	make_interface( host_state, CHostState*, interface_host_state, "engine.dll" );
	make_interface( mdl_cache, IMDLCache, interface_mdl_cache, "datacache.dll" );
	make_interface( studio_render, IStudioRender, interface_studio_render, "studiorender.dll" );
	make_interface_alt( hardware_config, IMaterialSystemHardwareConfig, interface_hardware_config, "materialsystem.dll" );
	make_interface( client_tools, IClientTools, interface_client_tools, "client.dll" );
	make_interface( string_tables, CNetworkStringTableContainer, interface_string_tables, "engine.dll" );
	make_interface( file_system, IBaseFileSystem, interface_file_system, "filesystem_stdio.dll" );
	make_interface_func( key_values_system, KeyValuesSystem, interface_key_values_system, "vstdlib.dll" );
	make_interface( panoroama_ui_engine, panorama_ui_engine_t, interface_panoroama_ui_engine, "panorama.dll" );
}

#define var(name) static auto name = interfaces::cvar()->FindVar(_r(#name))

#define EMIT_TICK_SOUND() interfaces::surface()->PlaySound(XOR_STR("buttons\\lightswitch2.wav"));
#define EMIT_SUCCESS_SOUND() interfaces::surface()->PlaySound(XOR_STR("common\\stuck1.wav"));
#define EMIT_ERROR_SOUND() interfaces::surface()->PlaySound(XOR_STR("buttons\\weapon_cant_buy.wav"));

#endif