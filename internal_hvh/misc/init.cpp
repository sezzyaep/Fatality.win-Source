#include "../include_cheat.h"
#include "networking/connector.h"
#include "ipc.h"
#include <winnt.h>
#include "MinHook.h"
#include "protection.h"

#include <ren/adapter_dx9.h>
#ifdef NO_NETOFFSETS
#include "../smallest_pixel.h"
#endif

uint32_t runtime_basis = 2166136261u;
char server_address[ 255 ];
char token[ 255 ];

uint32_t rdatastart;
uint32_t rdataend;
uint32_t textstart;
uint32_t textend;

extern "C" NTSTATUS NTAPI LdrDisableThreadCalloutsForDll( PVOID BaseAddress );

namespace hook::proxy
{
	_declspec( noinline ) uintptr_t ntqueryvirtualmemory()
	{
		return memory::get_proc_address( memory::get_module_handle( FNV1A( "ntdll.dll" ) ), FNV1A( "NtQueryVirtualMemory" ) );
	}
}

namespace init
{
	_declspec( noinline ) void rest()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		skinchanger::parse_kits();
		erase_fn( skinchanger::parse_kits );

		skinchanger::parse_weapon_names();
		erase_fn( skinchanger::parse_weapon_names );

		inventorychanger::fill_weapon_list();
		erase_fn( inventorychanger::fill_weapon_list );

		log( "5\n" );

		listeners::setup_listeners();

		log( "6\n" );

		inventorychanger::load();

		log( "7\n" );

		init_pred_map();
		erase_fn( init_pred_map );

		log( "8\n" );

#ifndef RELEASE
		if ( interfaces::engine()->IsInGame() )
			interfaces::client_state()->force_full_update();
#endif

		globals::erase_init = true;

		VIRTUALIZER_DOLPHIN_WHITE_END;
	}
}

#pragma clang section text=".discard"
void init::on_startup()
{
	VIRTUALIZER_DOLPHIN_WHITE_START;

#ifdef _DEBUG
	AllocConsole();
	freopen( "CONOUT$", "w", stdout );

	std::cout << "initializing" << std::endl;

#endif
	log( "0\n" );

#ifdef NO_NETOFFSETS
	save_resources();
	erase_fn( save_resources );

	log( "0.1\n" );
#endif

#ifndef NO_NETOFFSETS
#ifndef RELEASE
	auto addr = "ctbtxgkkuxbedotjdvaw.fatality.win";
	auto tok = "xLXubaG9WkTR6Wag4apgb6OpIZX5Y9ufjKuDBG";
	memcpy_s( server_address, 255, addr, strlen( addr ) + 1 );
	memcpy_s( token, 255, tok, strlen( tok ) + 1 );
#endif
	import( "ntdll.dll", RtlAddVectoredExceptionHandler )( 1, reinterpret_cast<PVECTORED_EXCEPTION_HANDLER>( protection::AntiDebugHandler ) );

	log( "0.1\n" );

	// exceptions
	make_hook_direct( hook::nt_query_virtual_memory, hook::proxy::ntqueryvirtualmemory() );
	hook::hook_list[ reinterpret_cast< uintptr_t >( hook::nt_query_virtual_memory ) ].apply();

	log( "0.13\n" );

	networking::network_offsets = new networking::net_offsets();
	networking::network_offsets->init();
	erase_fn( networking::net_offsets::init );

	log( "0.15\n" );

#ifdef ALPHA
	networking::conn = new networking::connector( server_address, 65387, token );
#else
	networking::conn = new networking::connector( server_address, 65387, token );
#endif

	log( "0.18\n" );

#ifndef _DEBUG
	RtlSecureZeroMemory( server_address, 255 );
	RtlSecureZeroMemory( token, 255 );
#endif

	log( "0.2\n" );

	while ( !networking::conn->is_ready_.load() )
		imp( "kernel32.dll", SleepEx )( 200, false );

	networking::conn->send_status( 5 );
#endif

	log( "0.3\n" );

#ifdef RELEASE
	protection::unstoppable_thread( &protection::CheckHosts );

	protection::suspended_thread();

	//protection::unstoppable_thread( &protection::CheckInternetConnection );

	//protection::unstoppable_thread( &protection::AntiVirtualMachine );

	protection::unstoppable_thread( &protection::CheckDebugger );

	//protection::unstoppable_thread( &protection::HasTitanHide );

	protection::unstoppable_thread( &protection::CheckBlacklistedDrivers );

	protection::unstoppable_thread( &protection::CheckAntiDebugIntegrity );

	protection::unstoppable_thread( &protection::CheckCodeSection );
#else
	globals::secure_info.values.threads_ran = protection::id_ran_all;
	globals::secure_info.initial_checksum = crc64::calculate( textstart, textend, rdatastart, rdataend );
#endif

	log( "0.4\n" );

#ifdef RELEASE
	HMODULE client = 0;
	while ( !( client = GetModuleHandleA( xorstr_( "client.dll" ) ) ) )
		LdrDisableThreadCalloutsForDll( client );
#endif

	log( "0.5\n" );

	HANDLE hand = 0;
	while ( !GetModuleHandleA( xorstr_( "serverbrowser.dll" ) ) || globals::secure_info.values.threads_ran != protection::id_ran_base && globals::secure_info.values.threads_ran != protection::id_ran_all
#ifdef RELEASE
	|| !hand && !( hand = protection::unstoppable_thread( &protection::CheckThreads ) ) || globals::secure_info.values.threads_ran != protection::id_ran_all
#endif
	)
		imp( "kernel32.dll", SleepEx )( 200, false );

	log( "0.5\n" );

#ifdef RELEASE
	protection::replaceIsDbgPresent();
#endif

	* reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( &globals::secure_info.values.threads_ran ) + 0x4 ) = globals::secure_info.values.threads_ran;

#ifndef NO_NETOFFSETS
	get_modules();
	erase_fn( get_modules );
#endif

	log( "1\n" );

#ifndef NO_NETOFFSETS
	networking::conn->send_status( 6 );
#endif

#ifdef NO_NETOFFSETS
	load_fonts();
	erase_fn( load_fonts );
#endif

	log( "2\n" );

#ifndef RELEASE
	auto p = **reinterpret_cast< ConVar*** >( reinterpret_cast< uintptr_t >( interfaces::cvar()( ) ) + 0x34 );
	for ( auto c = p->next; c != nullptr; c = c->next )
	{
		c->flags &= ~( ( 1 << 1 ) | ( 1 << 4 ) | ( 1 << 14 ) );
		c->orig_flags = c->flags | 1;
	}

	auto spam = sig( "server.dll", "2B CA 51 68 ? ? ? ?" ) + 2;
	if ( spam != 2 )
	{
		DWORD old{};
		VirtualProtect( reinterpret_cast< void* >( spam ), 4096, PAGE_EXECUTE_READWRITE, &old );
		for ( size_t i = 0; i < 15; i++ )
			*reinterpret_cast< BYTE* >( spam + i ) = 0x90;
		VirtualProtect( reinterpret_cast< void* >( spam ), 4096, old, &old );
	}
#endif

#ifndef NO_NETOFFSETS
	while ( networking::init_status != 1 )
		imp( "kernel32.dll", SleepEx )( 200, false );
#endif

	log( "2.5\n" );

	detail::callqueue.init();
	erase_fn( detail::call_queue::init );

	log( "3\n" );

	aimbot_helpers::build_seed_table();
	erase_fn( aimbot_helpers::build_seed_table );

#ifndef NO_NETOFFSETS
	networking::network_offsets->precache_init();
	erase_fn( networking::net_offsets::precache_init );

	networking::conn->send_status( 7 );

	log( "3.1\n" );

	while ( networking::init_status != 2 )
		imp( "kernel32.dll", SleepEx )( 200, false );

#else
	get_materials();

	patches();
#endif

	log( "3.2\n" );

	hook::init();
	erase_fn( hook::init );

	log( "4\n" );

	VIRTUALIZER_DOLPHIN_WHITE_END;

#ifndef RELEASE
	if ( interfaces::engine()->IsInGame() )
		hook::level_init_pre_entity( nullptr, nullptr, nullptr );
#endif

	rest();

	erase_end;
}

__declspec( noinline ) void init::get_modules()
{
	std::vector<uint32_t> modules
	{
		memory::get_module_handle( FNV1A( "client.dll" ) ),
		memory::get_module_handle( FNV1A( "engine.dll" ) ),
		memory::get_module_handle( FNV1A( "materialsystem.dll" ) ),
		memory::get_module_handle( FNV1A( "vstdlib.dll" ) ),
		memory::get_module_handle( FNV1A( "vphysics.dll" ) ),
		memory::get_module_handle( FNV1A( "vguimatsurface.dll" ) ),
		memory::get_module_handle( FNV1A( "tier0.dll" ) ),
		memory::get_module_handle( FNV1A( "vgui2.dll" ) ),
		memory::get_module_handle( FNV1A( "localize.dll" ) ),
		memory::get_module_handle( FNV1A( "datacache.dll" ) ),
		memory::get_module_handle( FNV1A( "studiorender.dll" ) ),
		memory::get_module_handle( FNV1A( "filesystem_stdio.dll" ) ),
		memory::get_module_handle( FNV1A( "panorama.dll" ) ),
		memory::get_module_handle( FNV1A( "gameoverlayrenderer.dll" ) ),
		memory::get_module_handle( FNV1A( "server.dll" ) ),
		memory::get_module_handle( FNV1A( "v8.dll" ) ),
		memory::get_module_handle( FNV1A( "inputsystem.dll" ) ),
		memory::get_module_handle( FNV1A( "shaderapidx9.dll" ) ),
	};

	//networking::conn->send_modules( modules );

	erase_end;
}

#ifdef NO_NETOFFSETS
__declspec( noinline ) void init::load_fonts()
{
	std::vector<unsigned char> v;

	DWORD n_fonts;
	mem_font_hadles.push_back( imp( "gdi32.dll", AddFontMemResourceEx )( smallest_pixel.data(), smallest_pixel.size(), nullptr, &n_fonts ) );
	smallest_pixel.clear();
	auto vec = std::vector<unsigned char>( smallest_pixel );
	smallest_pixel.swap( vec );

	//load_font_from_resource( reinterpret_cast< char* >( josefin_sans.data() ), josefin_sans.size() );
	//josefin_sans.clear();
	//vec = std::vector<unsigned char>( josefin_sans );
	//josefin_sans.swap( vec );

	//load_font_from_resource( reinterpret_cast< char* >( jost.data() ), jost.size() );
	//jost.clear();
	//vec = std::vector<unsigned char>( jost );
	//jost.swap( vec );

	erase_end;
}
#endif

void init::remove_fonts()
{
	for ( auto& handle : mem_font_hadles )
		imp( "gdi32.dll", RemoveFontMemResourceEx )( handle );
}

void init::get_materials()
{
	_( UnlitGeneric, "UnlitGeneric" );
	_( VertexLitGeneric, "VertexLitGeneric" );

	_( cstm_flat, R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#" );

	_( cstm_normal, R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#" );


	_( cstm_rim_glow, R"#("VertexLitGeneric"
{
	"$basetexture" "vgui/white"
	"$additive" "1"
	"$envmap" "models/effects/cube_white"
	"$envmaptint" "[1 1 1]"
	"$envmapfresnel" "1"
	"$envmapfresnelminmaxexp" "[0 1 2]"
	"$alpha" "0.8"
})#" );

	_( sniperdot, R"#("UnlitGeneric"
{
		"$additive" "1"
		"$basetexture" "sprites/glow06"

})#" );

	_( cstm_contour, R"#("VertexLitGeneric"
{
	$basetexture "vgui/white"
	$bumpmap "vgui/white"
	$selfillum "1"
	$selfIllumFresnel "1"
	$selfIllumFresnelMinMaxExp "[0 0.18 0.1]"
	$selfillumtint "[0.3 0.001 0.1]"
})#" );

	_( cstm_flow, R"#("UnlitGeneric"
{
	"$basetexture" "dev/zone_warning"
	"proxies"
	{
		"TextureScroll"
		{
			"texturescrollvar" "$basetexturetransform"
			"texturescrollrate" "0.1"
			"texturescrollangle" "90"
		}
	}
})#" );


	_( cstm_acrylic, R"#("VertexLitGeneric"
{
	"$basetexture" "VGUI/white"
	"$bumpmap" "de_nuke/hr_nuke/pool_water_normals_002"
	"$bumptransform" "center 0.5 0.5 scale 0.25 0.25 rotate 0.0 translate 0.0 0.0"
	"$nofog" "1"
	"$model" "1"
	"$color2" "[1.0 1.0 1.0]"
	"$halflambert" "1"
	"$envmap" "env_cubemap"
	"$envmaptint" "[0.1 0.1 0.1]"
	"$envmapfresnel" "1.0"
	"$envmapfresnelminmaxexp" "[1.0 1.0 1.0]"
	"$phong" "1"
	"$phongexponent" "1024"
	"$phongboost" "4.0"
	"$phongfresnelranges" "[1.0 1.0 1.0]"
	"$rimlight" "1"
	"$rimlightexponent" "4.0"
	"$rimlightboost" "2.0"
})#" );


	chams::add_material( material_index_t::MAT_FLAT, UnlitGeneric, cstm_flat );
	chams::add_material( material_index_t::MAT_NORMAL, VertexLitGeneric, cstm_normal );
	chams::add_material( material_index_t::MAT_RIM_GLOW, VertexLitGeneric, cstm_rim_glow );
	chams::add_material( material_index_t::MAT_SPRITE, UnlitGeneric, sniperdot );
	chams::add_material( material_index_t::MAT_CONTOUR, VertexLitGeneric, cstm_contour );
	chams::add_material( material_index_t::MAT_FLOW, UnlitGeneric, cstm_flow );
	chams::add_material( material_index_t::MAT_ACRYLIC, VertexLitGeneric, cstm_acrylic );

	erase_end;
}

void init::patches()
{
	// remove vac cvar check
	DWORD old{};
	auto addr = reinterpret_cast< void* >( make_offset_once( "vstdlib.dll", sig_cvar_vac_check ) );
	memory::protect_mem( addr, 3, PAGE_EXECUTE_READWRITE, old );
	*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) ) = 0xC2;
	*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) + 0x1 ) = 0x04;
	*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) + 0x2 ) = 0x00;
	memory::protect_mem( addr, 3, old, old );

	// change ents in leaves limit so things dont render in front of local player
	addr = reinterpret_cast< void* >( make_offset_once( "client.dll", sig_somelimit ) + 0x1 );
	memory::protect_mem( addr, 4, PAGE_EXECUTE_READWRITE, old );
	*reinterpret_cast< uint32_t* >( reinterpret_cast< uintptr_t >( addr ) ) = 4000;
	memory::protect_mem( addr, 4, old, old );

	// remove cvar check by valve for untrusted prevention
	addr = reinterpret_cast< void* >( make_offset_once( "client.dll", sig_cvar_check ) );
	memory::protect_mem( addr, 2, PAGE_EXECUTE_READWRITE, old );
	*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) ) = 0x90;
	*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) + 0x1 ) = 0xE9;
	memory::protect_mem( addr, 2, old, old );

	// remove move msg cmd limit
	addr = reinterpret_cast< void* >( make_offset_once( "engine.dll", sig_cl_send_move_limit ) + 0x1 );
	memory::protect_mem( addr, 1, PAGE_EXECUTE_READWRITE, old );
	*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) ) = 62;
	memory::protect_mem( addr, 1, old, old );

	// remove skinchanger console spam
	addr = reinterpret_cast< void* >( make_offset_once( "client.dll", sig_skinchanger_spam_patch ) + 0x1 );
	memory::protect_mem( addr, 1, PAGE_EXECUTE_READWRITE, old );
	*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) ) = 0x15;
	memory::protect_mem( addr, 1, old, old );

	//// remove stupid performance robbing retaddr check
	//addr = reinterpret_cast< void* >( sig_rel( "client.dll", "E8 ? ? ? ? 84 C0 75 15 FF 75 10" ) );
	//memory::protect_mem( addr, 3, PAGE_EXECUTE_READWRITE, old );
	//*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) ) = 0xC2;
	//*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) + 1 ) = 0x04;
	//*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) + 2 ) = 0x00;
	//memory::protect_mem( addr, 3, old, old );

	//addr = reinterpret_cast< void* >( sig_rel( "engine.dll", "E8 ? ? ? ? 84 C0 75 15 FF 75 10" ) );
	//memory::protect_mem( addr, 3, PAGE_EXECUTE_READWRITE, old );
	//*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) ) = 0xC2;
	//*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) + 1 ) = 0x04;
	//*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) + 2 ) = 0x00;
	//memory::protect_mem( addr, 3, old, old );

	// remove game velocity calc
	/*addr = reinterpret_cast< void* >( make_offset_once( "client.dll", sig_velocity_patch ) );
	memory::protect_mem( addr, 4, PAGE_EXECUTE_READWRITE, old );
	*reinterpret_cast< uint32_t* >( reinterpret_cast< uintptr_t >( addr ) ) = 0xD0E9;
	memory::protect_mem( addr, 4, old, old );*/

	erase_end;
}

#ifdef NO_NETOFFSETS
__declspec( noinline ) void init::save_resources()
{
	hitmarker::save_files();

	erase_end;
}
#endif

#ifndef RELEASE
struct backup_map_t
{
	typedescription_t* old_data_desc = nullptr;
	datamap_t* datamap = nullptr;
	int old_num_fields = 0;
};

std::vector<backup_map_t> backup_map{};

#endif

__declspec( noinline ) void init::init_pred_map()
{
	{
		auto& cs_player_pred_map = *reinterpret_cast< datamap_t* >( make_offset_multi( "client.dll", sig_cs_player_pred_map ) );

#ifndef RELEASE
		backup_map_t backup;
		backup.old_data_desc = cs_player_pred_map.dataDesc;
		backup.old_num_fields = cs_player_pred_map.dataNumFields;
		backup.datamap = &cs_player_pred_map;
		backup_map.push_back( backup );
#endif

		auto new_typedescs = static_cast< typedescription_t* >( interfaces::memalloc()->Alloc( sizeof( typedescription_t ) * ( cs_player_pred_map.dataNumFields + 5 ) ) );
		memcpy( new_typedescs, cs_player_pred_map.dataDesc, cs_player_pred_map.dataNumFields * sizeof( typedescription_t ) );
		new_typedescs[ cs_player_pred_map.dataNumFields++ ] = { FIELD_BOOLEAN, reinterpret_cast< char* >( make_offset_once( "client.dll", string_CCSPlayer_m_bWaitForNoAttack ) ), static_cast< int >( server_offset_multi( CCSPlayer_m_bWaitForNoAttack ) ), 1, FTYPEDESC_INSENDTABLE, nullptr, nullptr, nullptr, nullptr, 1 /*size in bytes*/, nullptr, 0, 0 };
		new_typedescs[ cs_player_pred_map.dataNumFields++ ] = { FIELD_INTEGER, reinterpret_cast< char* >( make_offset_once( "client.dll", string_CCSPlayer_m_iMoveState ) ), static_cast< int >( server_offset_multi( CCSPlayer_m_iMoveState ) ), 1, FTYPEDESC_INSENDTABLE, nullptr, nullptr, nullptr, nullptr, 4 /*size in bytes*/, nullptr, 0, 0 };
		new_typedescs[ cs_player_pred_map.dataNumFields++ ] = { FIELD_BOOLEAN, reinterpret_cast< char* >( make_offset_once( "client.dll", string_CCSPlayer_m_bStrafing ) ), static_cast< int >( server_offset_multi( CCSPlayer_m_bStrafing ) ), 1, FTYPEDESC_INSENDTABLE, nullptr, nullptr, nullptr, nullptr, 1 /*size in bytes*/, nullptr, 0, 0 };
		new_typedescs[ cs_player_pred_map.dataNumFields++ ] = { FIELD_FLOAT, reinterpret_cast< char* >( make_offset_once( "client.dll", string_CCSPlayer_m_flVelocityModifier ) ), static_cast< int >( server_offset_multi( CCSPlayer_m_flVelocityModifier ) ), 1, FTYPEDESC_INSENDTABLE, nullptr, nullptr, nullptr, nullptr, 4 /*size in bytes*/, nullptr, 0, interfaces::globals()->interval_per_tick * 1.f / 2.5f };
		new_typedescs[ cs_player_pred_map.dataNumFields++ ] = { FIELD_FLOAT, reinterpret_cast< char* >( make_offset_once( "client.dll", string_CCSPlayer_m_flThirdpersonRecoil ) ), static_cast< int >( server_offset_multi( CCSPlayer_m_flThirdpersonRecoil ) ), 1, FTYPEDESC_INSENDTABLE, nullptr, nullptr, nullptr, nullptr, 4 /*size in bytes*/, nullptr, 0, 0.01f };

		cs_player_pred_map.dataDesc = new_typedescs;
		cs_player_pred_map.m_pOptimizedDataMap = nullptr;
	}


	auto& base_cs_grenade_pred_map = *reinterpret_cast< datamap_t* >( make_offset_once( "client.dll", sig_base_cs_grenade_pred_map ) );

#ifndef RELEASE
	backup_map_t backup;
	backup.old_data_desc = base_cs_grenade_pred_map.dataDesc;
	backup.old_num_fields = base_cs_grenade_pred_map.dataNumFields;
	backup.datamap = &base_cs_grenade_pred_map;
	backup_map.push_back( backup );
#endif

	auto new_typedescs = static_cast< typedescription_t* >( interfaces::memalloc()->Alloc( sizeof( typedescription_t ) * ( base_cs_grenade_pred_map.dataNumFields + 2 ) ) );
	memcpy( new_typedescs, base_cs_grenade_pred_map.dataDesc, base_cs_grenade_pred_map.dataNumFields * sizeof( typedescription_t ) );
	new_typedescs[ base_cs_grenade_pred_map.dataNumFields++ ] = { FIELD_FLOAT, reinterpret_cast< char* >( make_offset_once( "client.dll", string_CBaseCSGrenade_m_fThrowTime ) ), static_cast< int >( server_offset_multi( CBaseCSGrenade_m_fThrowTime ) ), 1, FTYPEDESC_INSENDTABLE, nullptr, nullptr, nullptr, nullptr, 4 /*size in bytes*/, nullptr, 0, 0.01f };
	new_typedescs[ base_cs_grenade_pred_map.dataNumFields++ ] = { FIELD_BOOLEAN, reinterpret_cast< char* >( make_offset_once( "client.dll", string_CBaseCSGrenade_m_bIsHeldByPlayer ) ), static_cast< int >( server_offset_multi( CBaseCSGrenade_m_bIsHeldByPlayer ) ), 1, FTYPEDESC_INSENDTABLE, nullptr, nullptr, nullptr, nullptr, 1 /*size in bytes*/, nullptr, 0, 0 };

	base_cs_grenade_pred_map.dataDesc = new_typedescs;
	reinterpret_cast< datamap_t* >( make_offset_once( "client.dll", sig_decoy_map ) )->m_pOptimizedDataMap = nullptr;
	reinterpret_cast< datamap_t* >( make_offset_once( "client.dll", sig_he_map ) )->m_pOptimizedDataMap = nullptr;
	reinterpret_cast< datamap_t* >( make_offset_once( "client.dll", sig_molotov_map ) )->m_pOptimizedDataMap = nullptr;
	reinterpret_cast< datamap_t* >( make_offset_once( "client.dll", sig_sensor_map ) )->m_pOptimizedDataMap = nullptr;
	reinterpret_cast< datamap_t* >( make_offset_once( "client.dll", sig_smoke_map ) )->m_pOptimizedDataMap = nullptr;
	reinterpret_cast< datamap_t* >( make_offset_once( "client.dll", sig_flash_map ) )->m_pOptimizedDataMap = nullptr;

	erase_end;
}

#pragma clang section text=""

#ifndef RELEASE
void init::unload_cheat()
{
	constexpr auto detach_func = [] ()
	{
		config::load( -1 );

		for ( auto& item : lua::api.loaded_scripts )
		{
			_u( name, item );
			const auto hash = fnv1a_rt( xorstr_( "lua_" ) + name );
			if ( !config_init::exists( hash ) || !config_init::get_item_direct( hash )->get<bool>() )
			{
				_( LUA, "[lua] " );
				_( unload, "unloading script %s\n" );

				interfaces::cvar()->ConsoleColorPrintf( Color( 171, 34, 210, 255 ), LUA.c_str() );
				util::print_dev_console( true, Color( 255, 255, 255, 255 ), unload.c_str(), name.c_str() );

				lua::script_file file{
					lua::st_script,
					name
				};

				lua::api.stop_script( file );
			}
		}

		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

		listeners::remove_listeners();

		hook::unhook();

		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

		for ( const auto& map : backup_map )
		{
			// get rid of our map.
			interfaces::memalloc()->Free( map.datamap->dataDesc );
			map.datamap->dataDesc = map.old_data_desc;
			map.datamap->dataNumFields = map.old_num_fields;
			map.datamap->m_pOptimizedDataMap = nullptr;
		}

#ifdef NO_NETOFFSETS
		// its fine to use make_offset here since it is only used in debug where the server connection isnt running
		reinterpret_cast< datamap_t* >( make_offset( "client.dll", sig_decoy_map ) )->m_pOptimizedDataMap = nullptr;
		reinterpret_cast< datamap_t* >( make_offset( "client.dll", sig_he_map ) )->m_pOptimizedDataMap = nullptr;
		reinterpret_cast< datamap_t* >( make_offset( "client.dll", sig_molotov_map ) )->m_pOptimizedDataMap = nullptr;
		reinterpret_cast< datamap_t* >( make_offset( "client.dll", sig_sensor_map ) )->m_pOptimizedDataMap = nullptr;
		reinterpret_cast< datamap_t* >( make_offset( "client.dll", sig_smoke_map ) )->m_pOptimizedDataMap = nullptr;
		reinterpret_cast< datamap_t* >( make_offset( "client.dll", sig_flash_map ) )->m_pOptimizedDataMap = nullptr;
#endif

		detail::callqueue.decomission();

		if ( adapter )
			adapter->destroy_objects();

		remove_fonts();

		interfaces::client_state()->force_full_update();

		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

		FreeLibrary( globals::module_base );
		FreeLibraryAndExitThread( globals::module_base, 0 );
	};

	static bool has_run = false;
	if ( has_run )
		return;

	has_run = true;

	std::thread thread( detach_func );
	thread.detach();
}
#endif