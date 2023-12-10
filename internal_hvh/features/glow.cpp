#include "../include_cheat.h"

void glow::draw()
{
	if ( !vars::visuals.glow.enabled->get<bool>() )
		return;

	if ( interfaces::glow_object_manager()->m_glObjectDefinitions.Count() <= 0 )
		return;

	m_glObjectDefinitions.resize( interfaces::glow_object_manager()->m_glObjectDefinitions.Count() );
	for ( auto i = 0; i < interfaces::glow_object_manager()->m_glObjectDefinitions.Count(); i++ )
	{
		m_glObjectDefinitions[ i ].m_vGlowColor = interfaces::glow_object_manager()->m_glObjectDefinitions[ i ].m_vGlowColor;
		m_glObjectDefinitions[ i ].m_flGlowAlpha = interfaces::glow_object_manager()->m_glObjectDefinitions[ i ].m_flGlowAlpha;
		m_glObjectDefinitions[ i ].m_bRenderWhenOccluded = interfaces::glow_object_manager()->m_glObjectDefinitions[ i ].m_bRenderWhenOccluded;
		m_glObjectDefinitions[ i ].m_bRenderWhenUnoccluded = interfaces::glow_object_manager()->m_glObjectDefinitions[ i ].m_bRenderWhenUnoccluded;
		m_glObjectDefinitions[ i ].m_flGlowAlphaMax = interfaces::glow_object_manager()->m_glObjectDefinitions[ i ].m_flGlowAlphaMax;
	}


	interfaces::glow_object_manager()->RenderGlowEffects();
}

void glow::restore()
{
	if ( interfaces::glow_object_manager()->m_glObjectDefinitions.Count() != m_glObjectDefinitions.size() )
		return;
	
	for ( auto i = 0; i < interfaces::glow_object_manager()->m_glObjectDefinitions.Count(); i++ )
	{
		interfaces::glow_object_manager()->m_glObjectDefinitions[ i ].m_vGlowColor = m_glObjectDefinitions[ i ].m_vGlowColor;
		interfaces::glow_object_manager()->m_glObjectDefinitions[ i ].m_flGlowAlpha = m_glObjectDefinitions[ i ].m_flGlowAlpha;
		interfaces::glow_object_manager()->m_glObjectDefinitions[ i ].m_bRenderWhenOccluded = m_glObjectDefinitions[ i ].m_bRenderWhenOccluded;
		interfaces::glow_object_manager()->m_glObjectDefinitions[ i ].m_bRenderWhenUnoccluded = m_glObjectDefinitions[ i ].m_bRenderWhenUnoccluded;
		interfaces::glow_object_manager()->m_glObjectDefinitions[ i ].m_flGlowAlphaMax = m_glObjectDefinitions[ i ].m_flGlowAlphaMax;
	}
}
