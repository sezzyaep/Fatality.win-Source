#pragma once

struct StudioRenderConfig_t
{
	float fEyeShiftX;	// eye X position
	float fEyeShiftY;	// eye Y position
	float fEyeShiftZ;	// eye Z position
	float fEyeSize;		// adjustment to iris textures
	float fEyeGlintPixelWidthLODThreshold;

	int maxDecalsPerModel;
	int drawEntities;
	int skin;
	int fullbright;

	bool bEyeMove : 1;		// look around
	bool bSoftwareSkin : 1;
	bool bNoHardware : 1;
	bool bNoSoftware : 1;
	bool bTeeth : 1;
	bool bEyes : 1;
	bool bFlex : 1;
	bool bWireframe : 1;
	bool bDrawNormals : 1;
	bool bDrawTangentFrame : 1;
	bool bDrawZBufferedWireframe : 1;
	bool bSoftwareLighting : 1;
	bool bShowEnvCubemapOnly : 1;
	bool bWireframeDecals : 1;

	// Reserved for future use
	int m_nReserved[ 4 ];
};

class IStudioRender
{
private:
	template <typename T, typename ... args_t>
	constexpr T CallVFunc( void* thisptr, std::size_t nIndex, args_t... argList )
	{
		using VirtualFn = T( __thiscall* )( void*, decltype( argList )... );
		return ( *static_cast< VirtualFn** >( thisptr ) )[ nIndex ]( thisptr, argList... );
	}
private:
	virtual void _pad1() = 0;
	virtual void _pad2() = 0;
	virtual void _pad3() = 0;
	virtual void _pad4() = 0;
	virtual void _pad5() = 0;
	virtual void _pad6() = 0;
	virtual void _pad7() = 0;
	virtual void _pad8() = 0;
	virtual void _pad9() = 0;
public:

	virtual void BeginFrame( void ) = 0;
	virtual void EndFrame( void ) = 0;

	// Used for the mat_stub console command.
	virtual void Mat_Stub( IMaterialSystem *pMatSys ) = 0;

	// Updates the rendering configuration 
	virtual void UpdateConfig( const StudioRenderConfig_t& config ) = 0;
	virtual void GetCurrentConfig( StudioRenderConfig_t& config ) = 0;

	// Load, unload model data
	virtual bool LoadModel( studiohdr_t *pStudioHdr, void *pVtxData, studiohwdata_t	*pHardwareData ) = 0;
	virtual void UnloadModel( studiohwdata_t *pHardwareData ) = 0;

	// Refresh the studiohdr since it was lost...
	virtual void RefreshStudioHdr( studiohdr_t* pStudioHdr, studiohwdata_t* pHardwareData ) = 0;

	// This is needed to do eyeglint and calculate the correct texcoords for the eyes.
	virtual void SetEyeViewTarget( const studiohdr_t *pStudioHdr, int nBodyIndex, const Vector& worldPosition ) = 0;

	// Methods related to lighting state
	// NOTE: SetAmbientLightColors assumes that the arraysize is the same as 
	// returned from GetNumAmbientLightSamples
	virtual int GetNumAmbientLightSamples() = 0;
	virtual const Vector *GetAmbientLightDirections() = 0;
	virtual void SetAmbientLightColors( const Vector4D *pAmbientOnlyColors ) = 0;
	virtual void SetAmbientLightColors( const Vector *pAmbientOnlyColors ) = 0;
	virtual void SetLocalLights( int numLights, const LightDesc_t *pLights ) = 0;

	void SetAmbientLightColors2( const Vector4D* pAmbientOnlyColors )
	{
		CallVFunc<void>( this, 20, pAmbientOnlyColors );
	}
	void SetAmbientLightColors2( const Vector* pAmbientOnlyColors )
	{
		CallVFunc<void>( this, 21, pAmbientOnlyColors );
	}
	void SetLocalLights2( int nLights, const LightDesc_t* pLights )
	{
		CallVFunc<void>( this, 22, nLights, pLights );
	}

	// Sets information about the camera location + orientation
	virtual void SetViewState( const Vector& viewOrigin, const Vector& viewRight,
		const Vector& viewUp, const Vector& viewPlaneNormal ) = 0;

	// LOD stuff
	virtual int GetNumLODs( const studiohwdata_t &hardwareData ) const = 0;
	virtual float GetLODSwitchValue( const studiohwdata_t &hardwareData, int lod ) const = 0;
	virtual void SetLODSwitchValue( studiohwdata_t &hardwareData, int lod, float switchValue ) = 0;

	// Sets the color/alpha modulation
	virtual void SetColorModulation( float const* pColor ) = 0;
	virtual void SetAlphaModulation( float flAlpha ) = 0;

	// Draws the model
	virtual void DrawModel( void *pResults, const DrawModelInfo_t& info,
		matrix3x4_t *pBoneToWorld, float *pFlexWeights, float *pFlexDelayedWeights, const Vector &modelOrigin, int flags = 0 ) = 0;

	// Methods related to static prop rendering
	virtual void DrawModelStaticProp( const DrawModelInfo_t& drawInfo, const matrix3x4_t &modelToWorld, int flags = 0 ) = 0;
	virtual void DrawStaticPropDecals( const DrawModelInfo_t &drawInfo, const matrix3x4_t &modelToWorld ) = 0;
	virtual void DrawStaticPropShadows( const DrawModelInfo_t &drawInfo, const matrix3x4_t &modelToWorld, int flags ) = 0;

	// Causes a material to be used instead of the materials the model was compiled with
	virtual void ForcedMaterialOverride( IMaterial *newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL, int nMaterialIndex = -1 ) = 0;
	virtual bool IsForcedMaterialOverride() = 0;

	// Create, destroy list of decals for a particular model
	virtual StudioDecalHandle_t CreateDecalList( studiohwdata_t *pHardwareData ) = 0;
	virtual void DestroyDecalList( StudioDecalHandle_t handle ) = 0;

	// Add decals to a decal list by doing a planar projection along the ray
	// The BoneToWorld matrices must be set before this is called
	virtual void AddDecal( StudioDecalHandle_t handle, studiohdr_t *pStudioHdr, matrix3x4_t *pBoneToWorld,
		const Ray_t & ray, const Vector& decalUp, IMaterial* pDecalMaterial, float radius, int body, bool noPokethru = false, int maxLODToDecal = 0, void *pvProxyUserData = NULL, int nAdditionalDecalFlags = 0 ) = 0;

	// Compute the lighting at a point and normal
	virtual void ComputeLighting( const Vector* pAmbient, int lightCount,
		LightDesc_t* pLights, const Vector& pt, const Vector& normal, Vector& lighting ) = 0;

	// Compute the lighting at a point, constant directional component is passed
	// as flDirectionalAmount
	virtual void ComputeLightingConstDirectional( const Vector* pAmbient, int lightCount,
		LightDesc_t* pLights, const Vector& pt, const Vector& normal, Vector& lighting, float flDirectionalAmount ) = 0;

	// Shadow state (affects the models as they are rendered)
	virtual void AddShadow( IMaterial* pMaterial, void* pProxyData, FlashlightState_t *m_pFlashlightState = NULL, VMatrix *pWorldToTexture = NULL, ITexture *pFlashlightDepthTexture = NULL ) = 0;
	virtual void ClearAllShadows() = 0;

	// Gets the model LOD; pass in the screen size in pixels of a sphere 
	// of radius 1 that has the same origin as the model to get the LOD out...
	virtual int ComputeModelLod( studiohwdata_t* pHardwareData, float unitSphereSize, float *pMetric = NULL ) = 0;
};


#define DEFCONFIGMETHOD( ret_type, method, xbox_return_value )		\
FORCEINLINE ret_type method const 									\
{																	\
	return xbox_return_value;										\
}

enum VertexCompressionType_t
{
	// This indicates an uninitialized VertexCompressionType_t value
	VERTEX_COMPRESSION_INVALID = 0xFFFFFFFF,

	// 'VERTEX_COMPRESSION_NONE' means that no elements of a vertex are compressed
	VERTEX_COMPRESSION_NONE = 0,

	// Currently (more stuff may be added as needed), 'VERTEX_COMPRESSION_ON' means:
	//  - if a vertex contains VERTEX_ELEMENT_NORMAL, this is compressed
	//    (see CVertexBuilder::CompressedNormal3f)
	//  - if a vertex contains VERTEX_ELEMENT_USERDATA4 (and a normal - together defining a tangent
	//    frame, with the binormal reconstructed in the vertex shader), this is compressed
	//    (see CVertexBuilder::CompressedUserData)
	//  - if a vertex contains VERTEX_ELEMENT_BONEWEIGHTSx, this is compressed
	//    (see CVertexBuilder::CompressedBoneWeight3fv)
	VERTEX_COMPRESSION_ON = 1
};

enum HDRType_t
{
	HDR_TYPE_NONE,
	HDR_TYPE_INTEGER,
	HDR_TYPE_FLOAT,
};

enum ShadowFilterMode_t
{
	SHADOWFILTERMODE_DEFAULT = 0,

	NVIDIA_PCF = 0,
	ATI_NO_PCF_FETCH4 = 1,
	NVIDIA_PCF_CHEAP = 2,
	ATI_NOPCF = 3,

	// Game consoles use a different set of combo indices to control shadow filtering.
	GAMECONSOLE_NINE_TAP_PCF = 0,
	GAMECONSOLE_SINGLE_TAP_PCF = 1,

	// All modes >= SHADOWFILTERMODE_FIRST_CHEAP_MODE are considered the "cheap" modes.

	SHADOWFILTERMODE_FIRST_CHEAP_MODE = NVIDIA_PCF_CHEAP,
};

enum CSMQualityMode_t
{
	CSMQUALITY_VERY_LOW,
	CSMQUALITY_LOW,
	CSMQUALITY_MEDIUM,
	CSMQUALITY_HIGH,

	CSMQUALITY_TOTAL_MODES
};

enum CSMShaderMode_t
{
	CSMSHADERMODE_LOW_OR_VERY_LOW = 0,
	CSMSHADERMODE_MEDIUM = 1,
	CSMSHADERMODE_HIGH = 2,
	CSMSHADERMODE_ATIFETCH4 = 3,

	CSMSHADERMODE_TOTAL_MODES
};

class IMaterialSystemHardwareConfig
{
public:
	VFUNC( 50, SetHDREnabled( bool bEnable ), void( __thiscall* )( void*, bool ) )( bEnable )
};