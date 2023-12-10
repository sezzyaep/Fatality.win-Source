#pragma once

struct model_t
{
	int pad;
	char        name[ 255 ];
};
typedef unsigned short ModelInstanceHandle_t;

enum modtype_t
{
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};

struct ModelRenderInfo_t
{
	Vector origin{};
	QAngle angles{};
	char pad[ 0x4 ]{};
	void* pRenderable{};
	const model_t* pModel{};
	const matrix3x4_t* pModelToWorld;
	const matrix3x4_t* pLightingOffset;
	const Vector* pLightingOrigin;
	int flags{};
	int entity_index{};
	int skin{};
	int body{};
	int hitboxset{};
	ModelInstanceHandle_t instance{};
	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

typedef void* StudioDecalHandle_t;

struct studiohwdata_t;
struct DrawModelState_t
{
	studiohdr_t* m_pStudioHdr;
	studiohwdata_t* m_pStudioHWData;
	IClientRenderable* m_pRenderable;
	const matrix3x4_t* m_pModelToWorld;
	StudioDecalHandle_t		m_decals;
	int						m_drawFlags;
	int						m_lod;
};

struct ColorMeshInfo_t
{
	// A given color mesh can own a unique Mesh, or it can use a shared Mesh
	// (in which case it uses a sub-range defined by m_nVertOffset and m_nNumVerts)
	void* m_pMesh;
	void* m_pPooledVBAllocator;
	int						m_nVertOffsetInBytes;
	int						m_nNumVerts;
};

enum LightType_t
{
	MATERIAL_LIGHT_DISABLE = 0,
	MATERIAL_LIGHT_POINT,
	MATERIAL_LIGHT_DIRECTIONAL,
	MATERIAL_LIGHT_SPOT,
};

struct LightDesc_t
{
	LightType_t m_Type;										//< MATERIAL_LIGHT_xxx
	Vector m_Color;											//< color+intensity 
	Vector m_Position;										//< light source center position
	Vector m_Direction;										//< for SPOT, direction it is pointing
	float  m_Range;											//< distance range for light.0=infinite
	float m_Falloff;										//< angular falloff exponent for spot lights
	float m_Attenuation0;									//< constant distance falloff term
	float m_Attenuation1;									//< linear term of falloff
	float m_Attenuation2;									//< quadatic term of falloff

	// NOTE: theta and phi are *half angles*
	float m_Theta;											//< inner cone angle. no angular falloff 
															//< within this cone
	float m_Phi;											//< outer cone angle

	// the values below are derived from the above settings for optimizations
	// These aren't used by DX8. . used for software lighting.

	// NOTE: These dots are cos( m_Theta ), cos( m_Phi )
	float m_ThetaDot;
	float m_PhiDot;
	float m_OneOverThetaDotMinusPhiDot;
	unsigned int m_Flags;
	float m_RangeSquared;
};

struct DrawModelInfo_t
{
	studiohdr_t* m_pStudioHdr;
	studiohwdata_t* m_pHardwareData;
	StudioDecalHandle_t m_Decals;
	int				m_Skin;
	int				m_Body;
	int				m_HitboxSet;
	void* m_pClientEntity;
	int				m_Lod;
	ColorMeshInfo_t* m_pColorMeshes;
	bool			m_bStaticLighting;
	Vector			m_vecAmbientCube[ 6 ];		// ambient, and lights that aren't in locallight[]
	int				m_nLocalLightCount;
	LightDesc_t		m_LocalLightDescs[ 4 ];
};

class IMatRenderContext;
class IVModelRender
{
public:
	virtual int	DrawModel( int flags,
						   IClientRenderable* pRenderable,
						   ModelInstanceHandle_t instance,
						   int entity_index,
						   const model_t* model,
						   Vector const& origin,
						   QAngle const& angles,
						   int skin,
						   int body,
						   int hitboxset,
						   const matrix3x4_t* modelToWorld = NULL,
						   const matrix3x4_t* pLightingOffset = NULL ) = 0;
	virtual void ForcedMaterialOverride( IMaterial* newMaterial = NULL, OverrideType_t nOverrideType = OVERRIDE_NORMAL, int pad = 0 ) = 0;
	virtual void SetViewTarget( const CStudioHdr* pStudioHdr, int nBodyIndex, const Vector& target ) = 0;

	virtual void pad1() = 0;
	// Creates, destroys instance data to be associated with the model
	virtual ModelInstanceHandle_t CreateInstance( IClientRenderable* pRenderable, void* pCache = NULL ) = 0;
	virtual void DestroyInstance( ModelInstanceHandle_t handle ) = 0;

	VFUNC( 13, DrawModelShadowSetup( IClientRenderable* pRenderable, int body, int skin, DrawModelInfo_t* pInfo, matrix3x4_t* pCustomBoneToWorld = nullptr ), matrix3x4_t* ( __thiscall* )( void*, IClientRenderable*, int, int, DrawModelInfo_t*, matrix3x4_t* ) )( pRenderable, body, skin, pInfo, pCustomBoneToWorld )
		VFUNC( 14, DrawModelShadow( IClientRenderable* pRenderable, const DrawModelInfo_t& info, matrix3x4_t* pCustomBoneToWorld = nullptr ), void( __thiscall* )( void*, IClientRenderable*, const DrawModelInfo_t&, matrix3x4_t* ) )( pRenderable, info, pCustomBoneToWorld )
		VFUNC( 21, DrawModelExecute( IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld = nullptr ), void( __thiscall* )( void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t* ) )( context, state, info, pCustomBoneToWorld )
		VFUNC( 24, SuppressEngineLighting( bool bSuppress ), void( __thiscall* )( void*, bool ) )( bSuppress )
};
