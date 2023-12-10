#include "../include_cheat.h"

class CModelLoader
{
private:
	char _pad0x00[ 0x170 ];

public:
	// local base name of current loading model
	// generally used for debugging spew where only the name is desired, not the disk path
	char				m_szBaseName[ 64 ];

	bool				m_bMapRenderInfoLoaded;
	bool				m_bMapHasHDRLighting;
};


void __fastcall hook::level_init_pre_entity( void* ecx, void* edx, char* map_name )
{
	skinchanger::on_level_shudown();

	if ( ecx )
		original( level_init_pre_entity )( ecx, edx, map_name );

	animations::fake_record = {};
	animations::local_record = {};
	aimbot::shot_record = {};
	player_log::queued_esp_data.clear();

	prediction::clear_pred_info();
	tickbase::reset();
	animations::lag = {};
	animations::most_recent = {};
	skinchanger::server_model_idx = -1;

	auto addr = reinterpret_cast< float* > ( make_offset( "client.dll", sig_interp_clamp ) );
	if ( *addr != 0.5f )
	{
		DWORD old;
		memory::protect_mem( addr, 4, PAGE_EXECUTE_READWRITE, old );
		*addr = 0.5f;
		memory::protect_mem( addr, 4, old, old );
	}

#ifndef NO_NETOFFSETS
	std::thread th( networking::net_offsets::precache_map_load );
	th.detach();
#endif

	var( cl_interp );
	var( cl_interp_ratio );
	cl_interp->set_value( 0.015625f );
	cl_interp_ratio->set_value( 2.f );

	var( engine_no_focus_sleep );
	engine_no_focus_sleep->set_int( 0 );

	var( net_earliertempents );
	net_earliertempents->set_int( 1 );

	var( cl_pred_doresetlatch );
	cl_pred_doresetlatch->set_int( 0 );

	var( r_player_visibility_mode );
	r_player_visibility_mode->unarchive();
	r_player_visibility_mode->set_int( 0 );

	var( dsp_slow_cpu );
	dsp_slow_cpu->unlock();
	dsp_slow_cpu->set_int( 1 );

	var( dsp_enhance_stereo );
	dsp_enhance_stereo->unarchive();
	dsp_enhance_stereo->set_int( 0 );

	var( rate );
	if ( rate->get_int() != 786432 )
	{
		rate->unarchive();
		rate->set_int( 786432 );
	}

	var( cl_lagcompensation );
	var( cl_predict );
	ConVar::cl_lagcompensation = fnv1a_rt( cl_lagcompensation->value.string ) != FNV1A( "0" );
	ConVar::cl_predict = fnv1a_rt( cl_predict->value.string ) != FNV1A( "0" );
	cl_lagcompensation->enforce_sent_value( ConVar::cl_lagcompensation, ConVar::cl_lagcompensation );
	cl_predict->enforce_sent_value( ConVar::cl_predict, ConVar::cl_predict );
}

void __fastcall hook::level_init_post_entity( void* ecx, void* edx )
{
	original( level_init_post_entity )( ecx, edx );

	interfaces::engine()->ClientCmd( xorstr_( "exec thirdperson" ) );

	lua::api.callback( FNV1A( "on_level_init" ) );

	//static auto modelloader = *reinterpret_cast< CModelLoader** >( sig( "engine.dll", "B9 ? ? ? ? 50 E8 ? ? ? ? 85 C0 74 0F" ) + 0x1 );
	interfaces::hardware_config()->SetHDREnabled( true );
}

void __fastcall hook::ent_info_list_link_before( ent_info_list_t* list, uint32_t edx, ent_info_t* before, ent_info_t* element )
{
	before = interfaces::entity_list()->get_next_info( element );
	if ( !before || list != interfaces::entity_list()->get_active_list() )
		return original( ent_info_list_link_before )( list, edx, before, element );

	if ( element->prev != element )
	{
		if ( element->prev )
			element->prev->next = element->next;
		else
			list->head = element->next;

		if ( element->next )
			element->next->prev = element->prev;
		else
			list->tail = element->prev;

		element->prev = element->next = element;
	}

	element->next = before;
	element->prev = before->prev;
	before->prev = element;

	if ( element->prev )
		element->prev->next = element;
	else
		list->head = element;
}

int __stdcall hook::extract_occluded_renderables( int32_t view_id, int32_t count, RenderableInfo_t** renderables, uintptr_t rl_info )
{
	if ( !vars::visuals.chams.extended->get<bool>() )
		return original( extract_occluded_renderables )( view_id, count, renderables, rl_info );

	for ( auto i = 0; i < count; i++ )
	{
		auto el = renderables[ i ];
		if ( reinterpret_cast< size_t >( el ) & 1 )
			continue;

		const auto ent = el && el->m_pRenderable && el->m_pRenderable->GetIClientUnknown() ?
			reinterpret_cast< C_CSPlayer* >( el->m_pRenderable->GetIClientUnknown()->GetBaseEntity() ) : nullptr;

		if ( ent && ent->is_player() )
			*reinterpret_cast< bool* >( rl_info + 0x14 + 0x1C * i + 7 ) = false;
	}

	return original( extract_occluded_renderables )( view_id, count, renderables, rl_info );
}