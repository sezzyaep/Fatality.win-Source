#include "../include_cheat.h"

void __fastcall hook::scene_end( void* ecx, void* edx )
{
	original( scene_end )( ecx, edx );

	chams::dormant_chams();

	chams::draw_death_ghost();
}

void __stdcall hook::calc_renderable_world_space_aabb_bloated( RenderableInfo_t* info, Vector& absMins, Vector& absMaxs )
{
	if ( !info || !info->m_pRenderable )
		return original( calc_renderable_world_space_aabb_bloated )( info, absMins, absMaxs );

	const auto unknown = info->m_pRenderable->GetIClientUnknown();
	if ( !unknown )
		return original( calc_renderable_world_space_aabb_bloated )( info, absMins, absMaxs );

	const auto player = reinterpret_cast< C_BasePlayer* >( unknown->GetBaseEntity() );
	if ( !player || !player->is_player() && player->GetClientClass()->m_ClassID != ClassId::CCSRagdoll )
		return original( calc_renderable_world_space_aabb_bloated )( info, absMins, absMaxs );

	if ( !vars::visuals.chams.extended->get<bool>() || !local_player || player == local_player || !local_player->get_alive() )
		return original( calc_renderable_world_space_aabb_bloated )( info, absMins, absMaxs );

	const auto backup_renderable = info->m_pRenderable;
	info->m_pRenderable = local_player->get_renderable();
	original( calc_renderable_world_space_aabb_bloated )( info, absMins, absMaxs );
	info->m_pRenderable = backup_renderable;
}

#define IsLeafMarker( p ) (bool)((reinterpret_cast<size_t>(p)) & 1)


void __stdcall hook::add_renderables_to_render_lists( const SetupRenderInfo_t& info, int nCount, RenderableInfo_t** ppRenderables, BuildRenderListInfo_t* pRLInfo, int nDetailCount, void* pDetailInfo )
{
	for ( auto i = 0; i < nCount; ++i )
	{
		const auto renderable_info = ppRenderables[ i ];
		if ( IsLeafMarker( renderable_info ) )
			continue;

		if ( renderable_info && renderable_info->m_nModelType == 2 && !renderable_info->m_bDisableCSMRendering )
			renderable_info->m_nTranslucencyType = vars::visuals.prop_transparency->get<float>() > 0.f ? 1 : 2;

		if ( renderable_info && renderable_info->m_nModelType == 1 && renderable_info->m_pRenderable )
		{
			const auto unkn = renderable_info->m_pRenderable->GetIClientUnknown();
			const auto ent = unkn ? unkn->GetBaseEntity() : nullptr;

			if ( ent && ent->is_player() )
				renderable_info->m_nTranslucencyType = 1;

			/*const auto parent = reinterpret_cast< IClientRenderable* >( renderable_info->m_pRenderable )->GetShadowParent();
			const auto unknown = parent != nullptr ? parent->GetIClientUnknown() : nullptr;
			const auto owner = unknown != nullptr ? reinterpret_cast< C_CSPlayer* const >( unknown->GetIClientEntity() ) : nullptr;
			if ( owner && owner->is_player() )
				renderable_info->m_nTranslucencyType = 1;*/
		}

	}
	original( add_renderables_to_render_lists )( info, nCount, ppRenderables, pRLInfo, nDetailCount, pDetailInfo );
}