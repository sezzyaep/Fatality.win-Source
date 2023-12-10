set(PROJECT_NAME internal_hvh)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "features/aimbot.cpp"
    "features/aimbot.h"
    "features/animations.cpp"
    "features/animations.h"
    "features/antiaim.cpp"
    "features/antiaim.h"
    "features/beams.cpp"
    "features/beams.h"
    "features/bone_setup.cpp"
    "features/bone_setup.h"
    "features/chams.cpp"
    "features/chams.h"
    "features/glow.cpp"
    "features/glow.h"
    "features/hitmarker.cpp"
    "features/hitmarker.h"
    "features/include_features.h"
    "features/lag_compensation.cpp"
    "features/lag_compensation.h"
    "features/player_log.cpp"
    "features/player_log.h"
    "features/prediction.cpp"
    "features/prediction.h"
    "features/resolver.cpp"
    "features/resolver.h"
    "features/structs.h"
    "features/thirdperson.cpp"
    "features/thirdperson.h"
    "features/wall_penetration.cpp"
    "features/wall_penetration.h"
    "hooks/create_move.cpp"
    "hooks/do_post_screen_space_effects.cpp"
    "hooks/draw_model_execute.cpp"
    "hooks/frame_stage_notify.cpp"
    "hooks/hooks.h"
    "hooks/is_box_visible.cpp"
    "hooks/render_view.cpp"
    "hooks/scene_end.cpp"
    "hooks/wnd_proc.cpp"
    "misc/init.cpp"
    "misc/init.h"
    "misc/listeners.cpp"
    "misc/listeners.h"
    "misc/math.cpp"
    "misc/math.h"
    "misc/netvars.cpp"
    "misc/netvars.h"
    "misc/offsets.h"
    "misc/util.cpp"
    "misc/util.h"
    "sdk/CBoneChache.h"
    "sdk/checksum_md5.cpp"
    "sdk/checksum_md5.h"
    "sdk/ClientClass.h"
    "sdk/ConVar.h"
    "sdk/datatypes/Color.h"
    "sdk/datatypes/CUtlVector.h"
    "sdk/datatypes/matrix3x4_t.h"
    "sdk/datatypes/QAngle.h"
    "sdk/datatypes/Vector.h"
    "sdk/datatypes/Vector2D.h"
    "sdk/datatypes/VMatrix.h"
    "sdk/defines.h"
    "sdk/entity_classes.cpp"
    "sdk/entity_classes.h"
    "sdk/IClientEntity.h"
    "sdk/IMaterial.h"
    "sdk/include_sdk.h"
    "sdk/INetChannelInfo.h"
    "sdk/interfaces/IBaseClientDll.h"
    "sdk/interfaces/IClientEntityList.h"
    "sdk/interfaces/IClientModeShared.h"
    "sdk/interfaces/IClientState.h"
    "sdk/interfaces/ICVar.h"
    "sdk/interfaces/IEngineClient.h"
    "sdk/interfaces/IEngineTrace.h"
    "sdk/interfaces/IGameEventManager.h"
    "sdk/interfaces/IGlobalVarsBase.h"
    "sdk/interfaces/IInput.h"
    "sdk/interfaces/IMaterialSystem.h"
    "sdk/interfaces/IMatRenderContext.h"
    "sdk/interfaces/IMemAlloc.h"
    "sdk/interfaces/IMoveHelper.h"
    "sdk/interfaces/interfaces.h"
    "sdk/interfaces/IPhysicsSurfaceProps.h"
    "sdk/interfaces/IPrediction.h"
    "sdk/interfaces/ISurface.h"
    "sdk/interfaces/IVDebugOverlay.h"
    "sdk/interfaces/IViewRender.h"
    "sdk/interfaces/IViewRenderBeams.h"
    "sdk/interfaces/IVModelInfo.h"
    "sdk/interfaces/IVModelRender.h"
    "sdk/interfaces/IVRenderView.h"
    "sdk/IRefCounted.h"
    "sdk/misc.h"
    "sdk/structs.h"
    "sdk/trace_ray.h"
)
source_group("" FILES ${no_group_source_files})

set(headers
    "cthash.h"
    "globals.h"
    "include_cheat.h"
    "includes.h"
    "josefin_sans.h"
    "lua/exports/cvar.h"
    "lua/exports/lua_input.h"
    "lua/exports/lua_math.h"
    "md5.h"
    "porter.h"
    "singleton.h"
    "smallest_pixel.h"
    "space_texture.h"
    "threadsafe_deque.h"
    "visitor.h"
    "weaponicons.h"
)
source_group("headers" FILES ${headers})

set(headers__features
    "features/aimbot_helpers.h"
    "features/autostrafer.h"
    "features/console_log.h"
    "features/entity_rendering.h"
    "features/flashlight.h"
    "features/freestanding.h"
    "features/gamemovement.h"
    "features/grenade_pred.h"
    "features/hitscan.h"
    "features/inventorychanger.h"
    "features/knifebot.h"
    "features/legit.h"
    "features/misc.h"
    "features/mt.h"
    "features/playerperf.h"
    "features/skinchanger.h"
    "features/sound_esp.h"
    "features/tickbase.h"
    "features/visuals.h"
)
source_group("headers\\features" FILES ${headers__features})

set(headers__lua
    "lua/lua.h"
)
source_group("headers\\lua" FILES ${headers__lua})

set(headers__lua__dependencies__lua__include
    "lua/dependencies/lua/include/lauxlib.h"
    "lua/dependencies/lua/include/lua.h"
    "lua/dependencies/lua/include/lua.hpp"
    "lua/dependencies/lua/include/luaconf.h"
    "lua/dependencies/lua/include/lualib.h"
)
source_group("headers\\lua\\dependencies\\lua\\include" FILES ${headers__lua__dependencies__lua__include})

set(headers__lua__dependencies__lua_bridge__LuaBridge
    "lua/dependencies/lua_bridge/LuaBridge/List.h"
    "lua/dependencies/lua_bridge/LuaBridge/LuaBridge.h"
    "lua/dependencies/lua_bridge/LuaBridge/Map.h"
    "lua/dependencies/lua_bridge/LuaBridge/RefCountedObject.h"
    "lua/dependencies/lua_bridge/LuaBridge/RefCountedPtr.h"
    "lua/dependencies/lua_bridge/LuaBridge/Vector.h"
)
source_group("headers\\lua\\dependencies\\lua_bridge\\LuaBridge" FILES ${headers__lua__dependencies__lua_bridge__LuaBridge})

set(headers__lua__dependencies__lua_bridge__LuaBridge__detail
    "lua/dependencies/lua_bridge/LuaBridge/detail/CFunctions.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/ClassInfo.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/Constructor.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/dump.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/FuncTraits.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/Iterator.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/LuaException.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/LuaHelpers.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/LuaRef.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/Namespace.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/Security.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/Stack.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/TypeList.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/TypeTraits.h"
    "lua/dependencies/lua_bridge/LuaBridge/detail/Userdata.h"
)
source_group("headers\\lua\\dependencies\\lua_bridge\\LuaBridge\\detail" FILES ${headers__lua__dependencies__lua_bridge__LuaBridge__detail})

set(headers__lua__exports
    "lua/exports/callbacks.h"
    "lua/exports/csgo.h"
    "lua/exports/debug_overlay.h"
    "lua/exports/engine_client.h"
    "lua/exports/entity_list.h"
    "lua/exports/events.h"
    "lua/exports/global_vars.h"
    "lua/exports/lua_config.h"
    "lua/exports/lua_custom.h"
    "lua/exports/lua_render.h"
)
source_group("headers\\lua\\exports" FILES ${headers__lua__exports})

set(headers__menu
    "menu/menu.h"
    "menu/menu_include.h"
)
source_group("headers\\menu" FILES ${headers__menu})

set(headers__menu__elements
    "menu/elements/alpha_slider.h"
    "menu/elements/button.h"
    "menu/elements/checkbox.h"
    "menu/elements/child.h"
    "menu/elements/combobox.h"
    "menu/elements/control.h"
    "menu/elements/glitch_text.h"
    "menu/elements/hue_slider.h"
    "menu/elements/listbox.h"
    "menu/elements/pad.h"
    "menu/elements/slider.h"
    "menu/elements/sub_tab.h"
    "menu/elements/tab.h"
    "menu/elements/text_box.h"
    "menu/elements/weapon_tab.h"
    "menu/elements/window.h"
)
source_group("headers\\menu\\elements" FILES ${headers__menu__elements})

set(headers__menu__elements__sub_controls
    "menu/elements/sub_controls/color_picker.h"
    "menu/elements/sub_controls/key_bind.h"
    "menu/elements/sub_controls/sub_control.h"
)
source_group("headers\\menu\\elements\\sub_controls" FILES ${headers__menu__elements__sub_controls})

set(headers__misc
    "misc/config.h"
    "misc/d3dfont.h"
    "misc/datamap.h"
    "misc/defs.h"
    "misc/hitsounds.h"
    "misc/hybrid_hook.h"
    "misc/input.h"
    "misc/intersection.h"
    "misc/intrinsics.h"
    "misc/ipc.h"
    "misc/memory.h"
    "misc/recv_prop_hook.h"
    "misc/render.h"
    "misc/StackTracer.h"
    "misc/tinyformat.h"
)
source_group("headers\\misc" FILES ${headers__misc})

set(headers__misc__tsl
    "misc/tsl/robin_growth_policy.h"
    "misc/tsl/robin_hash.h"
    "misc/tsl/robin_map.h"
    "misc/tsl/robin_set.h"
)
source_group("headers\\misc\\tsl" FILES ${headers__misc__tsl})

set(headers__misc__win64
    "misc/win64/wow64ext.h"
)
source_group("headers\\misc\\win64" FILES ${headers__misc__win64})

set(headers__sdk
    "sdk/CCSGameRulesProxy.h"
    "sdk/CCSGOPlayerAnimState.h"
    "sdk/CNetChannel.h"
    "sdk/IMaterialVar.h"
    "sdk/KeyValues.h"
    "sdk/networkstringtable.h"
)
source_group("headers\\sdk" FILES ${headers__sdk})

set(headers__sdk__datatypes
    "sdk/datatypes/include_datatypes.h"
    "sdk/datatypes/matrix3x3_t.h"
    "sdk/datatypes/Vector4D.h"
)
source_group("headers\\sdk\\datatypes" FILES ${headers__sdk__datatypes})

set(headers__sdk__interfaces
    "sdk/interfaces/CGlowObjectManager.h"
    "sdk/interfaces/CHud.h"
    "sdk/interfaces/CSPlayerResource.h"
    "sdk/interfaces/ILocalize.h"
    "sdk/interfaces/IStudioRender.h"
    "sdk/interfaces/IToolEntity.h"
    "sdk/interfaces/IVEffects.h"
    "sdk/interfaces/IVPanel.h"
    "sdk/interfaces/IWeaponSystem.h"
)
source_group("headers\\sdk\\interfaces" FILES ${headers__sdk__interfaces})

set(source
    "DLLMain.cpp"
    "globals.cpp"
    "md5.cpp"
)
source_group("source" FILES ${source})

set(source__features
    "features/aimbot_helpers.cpp"
    "features/autostrafer.cpp"
    "features/console_log.cpp"
    "features/entity_rendering.cpp"
    "features/flashlight.cpp"
    "features/freestanding.cpp"
    "features/gamemovement.cpp"
    "features/grenade_pred.cpp"
    "features/hitscan.cpp"
    "features/inventorychanger.cpp"
    "features/knifebot.cpp"
    "features/legit.cpp"
    "features/misc.cpp"
    "features/mt.cpp"
    "features/playerperf.cpp"
    "features/skinchanger.cpp"
    "features/sound_esp.cpp"
    "features/tickbase.cpp"
    "features/visuals.cpp"
)
source_group("source\\features" FILES ${source__features})

set(source__hooks
    "hooks/con_vars.cpp"
    "hooks/emit_sound.cpp"
    "hooks/estimate_abs_velocity.cpp"
    "hooks/find_material.cpp"
    "hooks/is_connected.cpp"
    "hooks/level_init_post_entity.cpp"
    "hooks/level_shutdown.cpp"
    "hooks/net_props.cpp"
    "hooks/override_view.cpp"
    "hooks/paint_traverse.cpp"
    "hooks/physics_simulate.cpp"
    "hooks/player.cpp"
    "hooks/present.cpp"
    "hooks/process_input.cpp"
    "hooks/reset.cpp"
    "hooks/run_command.cpp"
    "hooks/packet_start.cpp"
    "hooks/sequence.cpp"
    "hooks/setup_bones_calls.cpp"
    "hooks/test_hitboxes.cpp"
)
source_group("source\\hooks" FILES ${source__hooks})

set(source__lua
    "lua/lua.cpp"
)
source_group("source\\lua" FILES ${source__lua})

set(source__lua__exports
    "lua/exports/callbacks.cpp"
    "lua/exports/csgo.cpp"
    "lua/exports/cvar.cpp"
    "lua/exports/debug_overlay.cpp"
    "lua/exports/engine_client.cpp"
    "lua/exports/entity_list.cpp"
    "lua/exports/events.cpp"
    "lua/exports/global_vars.cpp"
    "lua/exports/lua_config.cpp"
    "lua/exports/lua_custom.cpp"
    "lua/exports/lua_input.cpp"
    "lua/exports/lua_math.cpp"
    "lua/exports/lua_render.cpp"
)
source_group("source\\lua\\exports" FILES ${source__lua__exports})

set(source__menu
    "menu/menu.cpp"
)
source_group("source\\menu" FILES ${source__menu})

set(source__menu__elements
    "menu/elements/alpha_slider.cpp"
    "menu/elements/button.cpp"
    "menu/elements/checkbox.cpp"
    "menu/elements/child.cpp"
    "menu/elements/combobox.cpp"
    "menu/elements/control.cpp"
    "menu/elements/glitch_text.cpp"
    "menu/elements/hue_slider.cpp"
    "menu/elements/listbox.cpp"
    "menu/elements/slider.cpp"
    "menu/elements/sub_tab.cpp"
    "menu/elements/tab.cpp"
    "menu/elements/text_box.cpp"
    "menu/elements/weapon_tab.cpp"
    "menu/elements/window.cpp"
)
source_group("source\\menu\\elements" FILES ${source__menu__elements})

set(source__menu__elements__sub_controls
    "menu/elements/sub_controls/color_picker.cpp"
    "menu/elements/sub_controls/key_bind.cpp"
)
source_group("source\\menu\\elements\\sub_controls" FILES ${source__menu__elements__sub_controls})

set(source__misc
    "misc/config.cpp"
    "misc/d3dfont.cpp"
    "misc/input.cpp"
    "misc/render.cpp"
    "misc/StackTracer.cpp"
)
source_group("source\\misc" FILES ${source__misc})

set(source__sdk
    "sdk/KeyValues.cpp"
    "sdk/trace_ray.cpp"
)
source_group("source\\sdk" FILES ${source__sdk})

set(source__sdk__datatypes
    "sdk/datatypes/Vector.cpp"
)
source_group("source\\sdk\\datatypes" FILES ${source__sdk__datatypes})

set(source__sdk__interfaces
    "sdk/interfaces/CGlowObjectManager.cpp"
)
source_group("source\\sdk\\interfaces" FILES ${source__sdk__interfaces})

set(ALL_FILES
    ${no_group_source_files}
    ${headers}
    ${headers__features}
    ${headers__lua}
    ${headers__lua__dependencies__lua__include}
    ${headers__lua__dependencies__lua_bridge__LuaBridge}
    ${headers__lua__dependencies__lua_bridge__LuaBridge__detail}
    ${headers__lua__exports}
    ${headers__menu}
    ${headers__menu__elements}
    ${headers__menu__elements__sub_controls}
    ${headers__misc}
    ${headers__misc__tsl}
    ${headers__misc__win64}
    ${headers__sdk}
    ${headers__sdk__datatypes}
    ${headers__sdk__interfaces}
    ${source}
    ${source__features}
    ${source__hooks}
    ${source__lua}
    ${source__lua__exports}
    ${source__menu}
    ${source__menu__elements}
    ${source__menu__elements__sub_controls}
    ${source__misc}
    ${source__sdk}
    ${source__sdk__datatypes}
    ${source__sdk__interfaces}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

set(ROOT_NAMESPACE internalhvh)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME_RELWITHDEBINFO "release_symbols"
    OUTPUT_NAME_RELEASE        "fatality"
)

################################################################################
# Config properties
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO "TRUE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE        "TRUE"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_EXTRAINFO"
    ">"
    "$<$<CONFIG:RelWithDebInfo>:"
        "_WIN32;"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "_WIN32;"
        "NDEBUG;"
        "NO_LUA_EX;"
        "RELEASE"
        "_HAS_STATIC_RTTI=0"
    ">"
    "USE_LUA;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile options
################################################################################
# target_compile_options (${PROJECT_NAME} PUBLIC -m32
#   -fdiagnostics-absolute-paths
#   -fno-rtti
#   # -fno-threadsafe-statics
#   -fno-caret-diagnostics
#   -ffast-math
#   -Wno-pch-date-time
#   -Wno-switch
#   -Wno-parentheses
#   -Wno-microsoft-cast
#   -Wno-microsoft-unqualified-friend
#   -Wno-c++98-compat
#   -Wno-old-style-cast
#   -Wno-cast-align
#   -Wno-non-virtual-dtor
#   -Wno-gnu-anonymous-struct
#   -Wno-nested-anon-types
#   -Wno-comment
#   -Wno-new-returns-null
#   -Wno-unused-parameter
#   -Wno-language-extension-token
#   -Wno-microsoft-goto
#   -Wno-c++98-compat-pedantic
#   -Wno-implicit-exception-spec-mismatch
#   -Wno-invalid-noreturn
#   -Wno-inconsistent-dllimport
#   -Wno-format-security
#   -Wno-nonportable-include-path
#   -Wno-macro-redefined
#   -Wno-c99-extensions)
#
# target_compile_definitions (${PROJECT_NAME} PUBLIC _HAS_ITERATOR_DEBUGGING=0
#   _SECURE_SCL=0
#   _HAS_STATIC_RTTI=0
#   _CRT_SECURE_NO_WARNINGS
#   _X86_
#   _WINDOWS
#   WIN32)

target_compile_options (${PROJECT_NAME} PUBLIC -fno-stack-protector -ffunction-sections -O3 -flto -ffast-math -fno-rtti)

#target_compile_options(${PROJECT_NAME} PUBLIC /GS- /W1 /Gy /O2 /fp:fast /WX- /GR- /Gd /Oi /MD)
#target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:/GS /analyze- /ZI /Gm- /Od /Zc:inline /fp:fast /WX- /Zc:forScope /Gd /Oy- /MDd /FC /EHsc")

target_link_options(${PROJECT_NAME} PUBLIC -fno-stack-protector -ffunction-sections -O3 -flto -ffast-math -fno-rtti)

#target_link_options(${PROJECT_NAME} PUBLIC /NXCOMPAT /SAFESEH:NO /INCREMENTAL:NO)
#target_link_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:/NXCOMPAT /PDB:internal_hvh.pdb /DEBUG /INCREMENTAL")

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include")
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC "lua\\dependencies\\lua_bridge")
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC "lua\\dependencies\\lua\\include")

################################################################################
# Library options
################################################################################
# if (CMAKE_BUILD_TYPE MATCHES Release)
# target_compile_options (${PROJECT_NAME} PUBLIC -flto)
#   target_link_options (${PROJECT_NAME} PUBLIC -Wl,/incremental:no -flto)
# endif (CMAKE_BUILD_TYPE MATCHES Release)

################################################################################
# Library directories
################################################################################
target_link_directories(${PROJECT_NAME} SYSTEM PUBLIC "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86")
target_link_directories(${PROJECT_NAME} SYSTEM PUBLIC "lua\\dependencies\\lua")
target_link_directories(${PROJECT_NAME} SYSTEM PUBLIC "wow64ext\\src\\Release")

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "ntdll"
    "wow64ext"
    "lua53"
)

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

################################################################################
# std version
################################################################################
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

get_target_property(MAIN_CFLAGS internal_hvh COMPILE_OPTIONS)
# also see: COMPILE_DEFINITIONS INCLUDE_DIRECTORIES
message("-- Target compiler flags are: ${MAIN_CFLAGS}")

add_custom_command(TARGET internal_hvh POST_BUILD
COMMAND echo built with the flags: ${MAIN_CFLAGS})
