#include "../..\include_cheat.h"

bool is_grenade( ClassId id )
{
	return id == ClassId::CDecoyGrenade || id == ClassId::CHEGrenade || id == ClassId::CIncendiaryGrenade || id == ClassId::CMolotovGrenade || id == ClassId::CSmokeGrenade || id == ClassId::CFlashbang || id == ClassId::CDecoyProjectile || id == ClassId::CBaseCSGrenadeProjectile || id == ClassId::CMolotovProjectile || id == ClassId::CSmokeGrenadeProjectile || id == ClassId::CFlashbang;
}

bool is_weapon( ClassId id )
{
	return id >= ClassId::CWeaponAug && id <= ClassId::CWeaponXM1014 || id == ClassId::CAK47 || id == ClassId::CDEagle;
}

void CGlowObjectManager::RenderGlowEffects()
{
	const auto player_glow = [&] ( const int idx )
	{
		const auto classid = m_glObjectDefinitions[ idx ].m_pEntity->GetClientClass()->m_ClassID;
		if ( classid != ClassId::CCSPlayer )
			return;

		auto player = reinterpret_cast< C_BasePlayer* >( m_glObjectDefinitions[ idx ].m_pEntity );
		
		if ( !vars::visuals.glow.local.enabled->get<bool>() && player == local_player )
			return;
		if ( !vars::visuals.glow.enemy.enabled->get<bool>() && player->is_enemy() )
			return;
		if ( !vars::visuals.glow.team.enabled->get<bool>() && !player->is_enemy() && player != local_player )
			return;

		Color col;

		if ( player == local_player )
			col = Color( vars::visuals.glow.local.color->get<D3DCOLOR>() );
		else if ( player->is_enemy() )
			col = Color( vars::visuals.glow.enemy.color->get<D3DCOLOR>() );
		else
			col = Color( vars::visuals.glow.team.color->get<D3DCOLOR>() );

		const Vector color( col.rBase(), col.gBase(), col.bBase() );

		const auto player_alpha = player->EntIndex() == -1 ? 1.f : visuals::players[ player->EntIndex() ].model_alpha / 255.f;

		const auto alpha = ( !player->is_enemy() || player == local_player ) ? col.aBase() : player_alpha * col.aBase();

		m_glObjectDefinitions[ idx ].m_vGlowColor = color;
		m_glObjectDefinitions[ idx ].m_flGlowAlpha = alpha;
		m_glObjectDefinitions[ idx ].m_bRenderWhenOccluded = true;
		m_glObjectDefinitions[ idx ].m_bRenderWhenUnoccluded = false;
		m_glObjectDefinitions[ idx ].m_flGlowAlphaMax = 1.0f;
	};

	const auto weapon_glow = [&] ( const int idx )
	{
		const auto classid = m_glObjectDefinitions[ idx ].m_pEntity->GetClientClass()->m_ClassID;
		if ( !is_weapon( classid ) )
			return;

		if ( m_glObjectDefinitions[ idx ].m_pEntity->get_owner_ent() != -1 )
			return;

		if ( !vars::visuals.glow.world.weapons->get<bool>() )
			return;

		auto col = Color( vars::visuals.glow.world.color->get<D3DCOLOR>() );
		const Vector color( col.rBase(), col.gBase(), col.bBase() );

		m_glObjectDefinitions[ idx ].m_vGlowColor = color;
		m_glObjectDefinitions[ idx ].m_flGlowAlpha = col.aBase();
		m_glObjectDefinitions[ idx ].m_bRenderWhenOccluded = true;
		m_glObjectDefinitions[ idx ].m_bRenderWhenUnoccluded = false;
		m_glObjectDefinitions[ idx ].m_flGlowAlphaMax = 1.0f;
	};

	const auto grenade_glow = [&] ( const int idx )
	{
		const auto classid = m_glObjectDefinitions[ idx ].m_pEntity->GetClientClass()->m_ClassID;
		if ( !is_grenade( classid ) )
			return;

		if ( !vars::visuals.glow.world.grenades->get<bool>() )
			return;

		auto col = Color( vars::visuals.glow.world.color->get<D3DCOLOR>() );
		const Vector color( col.rBase(), col.gBase(), col.bBase() );

		m_glObjectDefinitions[ idx ].m_vGlowColor = color;
		m_glObjectDefinitions[ idx ].m_flGlowAlpha = col.aBase();
		m_glObjectDefinitions[ idx ].m_bRenderWhenOccluded = true;
		m_glObjectDefinitions[ idx ].m_bRenderWhenUnoccluded = false;
		m_glObjectDefinitions[ idx ].m_flGlowAlphaMax = 1.0f;
	};

	const auto c4_glow = [&] ( const int idx )
	{
		const auto classid = m_glObjectDefinitions[ idx ].m_pEntity->GetClientClass()->m_ClassID;
		if ( classid != ClassId::CC4 && classid != ClassId::CPlantedC4 )
			return;

		if ( !vars::visuals.glow.world.c4->get<bool>() )
			return;

		auto c4 = reinterpret_cast< C_PlantedC4* >( m_glObjectDefinitions[ idx ].m_pEntity );
		const auto defusing = c4->get_defuser() != -1 || c4->get_defused();

		auto col = Color( vars::visuals.glow.world.color->get<D3DCOLOR>() );
		if ( classid == ClassId::CPlantedC4 )col = Color::Red();
		if ( defusing ) col = Color::LightBlue();
		const Vector color( col.rBase(), col.gBase(), col.bBase() );

		m_glObjectDefinitions[ idx ].m_vGlowColor = color;
		m_glObjectDefinitions[ idx ].m_flGlowAlpha = col.aBase();
		m_glObjectDefinitions[ idx ].m_bRenderWhenOccluded = true;
		m_glObjectDefinitions[ idx ].m_bRenderWhenUnoccluded = false;
		m_glObjectDefinitions[ idx ].m_flGlowAlphaMax = 1.0f;
	};

	for ( auto i = 0; i < m_glObjectDefinitions.Count(); i++ )
	{
		if ( m_glObjectDefinitions[ i ].IsUnused() || !m_glObjectDefinitions[ i ].m_pEntity )
			continue;

		//if ( chams::in_glow && m_glObjectDefinitions[ i ].m_pEntity != entity_rendering::player )
			//continue;

		//if ( !chams::in_glow && m_glObjectDefinitions[ i ].m_pEntity == entity_rendering::player )
			//continue;

		player_glow( i );

		weapon_glow( i );

		grenade_glow( i );

		c4_glow( i );
	}
}