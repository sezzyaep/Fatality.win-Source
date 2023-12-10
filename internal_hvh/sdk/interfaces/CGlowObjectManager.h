#pragma once

class CGlowObjectManager
{
public:
	class GlowObjectDefinition_t
	{
	public:
		bool ShouldDraw() const { return m_pEntity && ( m_bRenderWhenOccluded || m_bRenderWhenUnoccluded ); }
		bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

		int m_nNextFreeSlot;
		C_BaseEntity* m_pEntity;
		Vector m_vGlowColor;
		float m_flGlowAlpha;
		bool m_bGlowAlphaCappedByRenderAlpha;
		float m_flGlowAlphaFunctionOfMaxVelocity;
		float m_flGlowAlphaMax;
		float m_flGlowPulseOverdrive;
		bool m_bRenderWhenOccluded;
		bool m_bRenderWhenUnoccluded;
		bool m_bFullBloomRender;
		int m_nFullBloomStencilTestValue;
		int m_nRenderStyle;
		int m_nSplitScreenSlot;
		// Special values for GlowObjectDefinition_t::m_nNextFreeSlot 
		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	};

	void RenderGlowEffects();

	CUtlVector< GlowObjectDefinition_t > m_glObjectDefinitions;
	int m_nFirstFreeSlot;
	struct GlowBoxDefinition_t
	{
		Vector m_vPosition;
		QAngle m_angOrientation;
		Vector m_vMins;
		Vector m_vMaxs;
		float m_flBirthTimeIndex;
		float m_flTerminationTimeIndex; //when to die
		Color m_colColor;
	};

	CUtlVector< GlowBoxDefinition_t > m_GlowBoxDefinitions;
};

struct ShaderStencilState_t
{
	bool m_bEnable;
	ShaderStencilOp_t m_FailOp;
	ShaderStencilOp_t m_ZFailOp;
	ShaderStencilOp_t m_PassOp;
	ShaderStencilFunc_t m_CompareFunc;
	int m_nReferenceValue;
	uint32 m_nTestMask;
	uint32 m_nWriteMask;

	ShaderStencilState_t()
	{
		m_bEnable = false;
		m_PassOp = m_FailOp = m_ZFailOp = SHADER_STENCILOP_KEEP;
		m_CompareFunc = SHADER_STENCILFUNC_ALWAYS;
		m_nReferenceValue = 0;
		m_nTestMask = m_nWriteMask = 0xFFFFFFFF;
	}
};
