#pragma once

namespace glow
{
	void draw();
	void restore();

	inline std::vector< CGlowObjectManager::GlowObjectDefinition_t > m_glObjectDefinitions = {};
};