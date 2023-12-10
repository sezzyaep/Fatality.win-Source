#pragma once

class IWorldRenderList : public IRefCounted
{};

struct VisibleFogVolumeInfo_t
{
	int		m_nVisibleFogVolume;
	int		m_nVisibleFogVolumeLeaf;
	bool	m_bEyeInFogVolume;
	float	m_flDistanceToWater;
	float	m_flWaterHeight;
	IMaterial* m_pFogVolumeMaterial;
};

typedef unsigned short LeafIndex_t;
enum
{
	INVALID_LEAF_INDEX = ( LeafIndex_t ) ~0
};

struct WorldListLeafData_t
{
	LeafIndex_t	leafIndex;	// 16 bits
	int16	waterData;
	uint16 	firstTranslucentSurface;	// engine-internal list index
	uint16	translucentSurfaceCount;	// count of translucent surfaces+disps
};

struct WorldListInfo_t
{
	int		m_ViewFogVolume;
	int		m_LeafCount;
	bool	m_bHasWater;
	WorldListLeafData_t* m_pLeafDataList;
};

struct VisOverrideData_t
{
	Vector		m_vecVisOrigin;					// The point to to use as the viewpoint for area portal backface cull checks.
	float		m_fDistToAreaPortalTolerance;	// The distance from an area portal before using the full screen as the viewable portion.
};

struct BrushVertex_t
{
	Vector		m_Pos;
	Vector		m_Normal;
	Vector		m_TangentS;
	Vector		m_TangentT;
	Vector2D	m_TexCoord;
	Vector2D	m_LightmapCoord;

private:
	BrushVertex_t( const BrushVertex_t& src );
};

class IBrushSurface
{
public:
	// Computes texture coordinates + lightmap coordinates given a world position
	virtual void ComputeTextureCoordinate( Vector const& worldPos, Vector2D& texCoord ) = 0;
	virtual void ComputeLightmapCoordinate( Vector const& worldPos, Vector2D& lightmapCoord ) = 0;

	// Gets the vertex data for this surface
	virtual int  GetVertexCount() const = 0;
	virtual void GetVertexData( BrushVertex_t* pVerts ) = 0;

	// Gets at the material properties for this surface
	virtual IMaterial* GetMaterial() = 0;
};

class IBrushRenderer
{
public:
	// Draws the surface; returns true if decals should be rendered on this surface
	virtual bool RenderBrushModelSurface( C_BaseEntity* pBaseEntity, IBrushSurface* pBrushSurface ) = 0;
};

class VPlane
{
public:
	Vector m_Normal;
	float m_Dist;
};

enum
{
	FRUSTUM_RIGHT = 0,
	FRUSTUM_LEFT = 1,
	FRUSTUM_TOP = 2,
	FRUSTUM_BOTTOM = 3,
	FRUSTUM_NEARZ = 4,
	FRUSTUM_FARZ = 5,
	FRUSTUM_NUMPLANES = 6
};

enum ClearFlags_t
{
	VIEW_CLEAR_COLOR = 0x1,
	VIEW_CLEAR_DEPTH = 0x2,
	VIEW_CLEAR_FULL_TARGET = 0x4,
	VIEW_NO_DRAW = 0x8,
	VIEW_CLEAR_OBEY_STENCIL = 0x10, // Draws a quad allowing stencil test to clear through portals
	VIEW_CLEAR_STENCIL = 0x20,
};

typedef VPlane Frustum[ FRUSTUM_NUMPLANES ];


class IVRenderView
{
public:
	virtual void      DrawBrushModel( void* pBaseEntity, void* pModel, const Vector& vecOrigin, const Vector& angView, bool bSort ) = 0;
	virtual void      DrawIdentityBrushModel( void* pList, void* pModel ) = 0;
	virtual void      TouchLight( struct dlight_t* dLight ) = 0;
	virtual void      Draw3DDebugOverlays() = 0;
	virtual void      SetBlend( float flBlend ) = 0;
	virtual float      GetBlend() = 0;
	virtual void      SetColorModulation( float const* flBlend ) = 0;
	virtual void      GetColorModulation( float* flBlend ) = 0;
	virtual void      SceneBegin() = 0;
	virtual void      SceneEnd() = 0;
	virtual void      GetVisibleFogVolume( const Vector& vecEyePoint, void* pInfo ) = 0;
	virtual void* CreateWorldList() = 0;
	virtual void      BuildWorldLists_Epilogue( void* pList, void* pInfo, bool bShadowDepth ) = 0;
	virtual void      BuildWorldLists( void* pList, void* pInfo, int iForceFViewLeaf, const void* pVisData = nullptr, bool bShadowDepth = false, float* pReflectionWaterHeight = nullptr ) = 0;
	virtual void      DrawWorldLists( void* pList, unsigned long flags, float flWaterZAdjust ) = 0;
	virtual void      GetWorldListIndicesInfo( void* pIndicesInfoOut, void* pList, unsigned long nFlags ) = 0;
	virtual void      DrawTopView( bool bEnable ) = 0;
	virtual void      TopViewNoBackfaceCulling( bool bDisable ) = 0;
	virtual void      TopViewNoVisCheck( bool bDisable ) = 0;
	virtual void      Unused0() = 0;// TopViewBounds(Vector2D const& vecMins, Vector2D const& vecMaxs) = 0;
	virtual void      SetTopViewVolumeCuller( const void* pVolumeCuller ) = 0;
	virtual void      DrawLights() = 0;
	virtual void      DrawMaskEntities() = 0;
	virtual void      DrawTranslucentSurfaces( void* pList, int* pSortList, int iSortCount, unsigned long fFlags ) = 0;
	virtual void      DrawLineFile() = 0;
	virtual void      DrawLightmaps( void* pList, int iPageID ) = 0;
	virtual void      ViewSetupVis( bool bNoVis, int nOrigins, const Vector vecOrigin[] ) = 0;
	virtual bool      AreAnyLeavesVisible( int* pLeafList, int nLeaves ) = 0;
	virtual  void      VguiPaint() = 0;
	virtual void      ViewDrawFade( uint8_t* pColor, IMaterial* pMaterial ) = 0;
	virtual void      OLD_SetProjectionMatrix( float flFov, float zNear, float zFar ) = 0;
	virtual void      Unused1() = 0;// ColorVec_t    GetLightAtPoint(Vector& vecPosition) = 0;
	virtual int        GetViewEntity() = 0;
	virtual bool      IsViewEntity( int nEntityIndex ) = 0;
	virtual float      GetFieldOfView() = 0;
	virtual unsigned char** GetAreaBits() = 0;
	virtual void      SetFogVolumeState( int nVisibleFogVolume, bool bUseHeightFog ) = 0;
	virtual void      InstallBrushSurfaceRenderer( void* pBrushRenderer ) = 0;
	virtual void      DrawBrushModelShadow( void* pRenderable ) = 0;
	virtual  bool      LeafContainsTranslucentSurfaces( void* pList, int nSortIndex, unsigned long fFlags ) = 0;
	virtual bool      DoesBoxIntersectWaterVolume( const Vector& vecMins, const Vector& vecMaxs, int nLeafWaterDataID ) = 0;
	virtual void      SetAreaState( unsigned char uAreaBits[ 32 ], unsigned char uAreaPortalBits[ 24 ] ) = 0;
	virtual void      VGui_Paint( int nMode ) = 0;
	virtual void      Push3DView( void* pRenderContext, const CViewSetup& view, int nFlags, ITexture* pRenderTarget, Frustum frustumPlanes ) = 0;
	virtual void      Push2DView( void* pRenderContext, const CViewSetup& view, int nFlags, ITexture* pRenderTarget, Frustum frustumPlanes ) = 0;
	virtual void      PopView( void* pRenderContext, Frustum frustumPlanes ) = 0;
	virtual void      SetMainView( const Vector& vecOrigin, const Vector& angView ) = 0;
	virtual void      ViewSetupVisEx( bool bNoVis, int nOrigins, const Vector arrOrigin[], unsigned int& uReturnFlags ) = 0;
	virtual void      OverrideViewFrustum( Frustum custom ) = 0;
	virtual void      DrawBrushModelShadowDepth( void* pEntity, void* pModel, const Vector& vecOrigin, const Vector& angView, int nDepthMode ) = 0;
	virtual void      UpdateBrushModelLightmap( void* pModel, void* pRenderable ) = 0;
	virtual void      BeginUpdateLightmaps() = 0;
	virtual void      EndUpdateLightmaps() = 0;
	virtual void      OLD_SetOffCenterProjectionMatrix( float flFOV, float flNearZ, float flFarZ, float flAspectRatio, float flBottom, float flTop, float flLeft, float flRight ) = 0;
	virtual void      OLD_SetProjectionMatrixOrtho( float flLeft, float flTop, float flRight, float flBottom, float flNearZ, float flFarZ ) = 0;
	virtual void      Push3DView( void* pRenderContext, const CViewSetup& view, int nFlags, ITexture* pRenderTarget, Frustum frustumPlanes, ITexture* pDepthTexture ) = 0;
	virtual void      GetMatricesForView( const CViewSetup& view, VMatrix* pWorldToView, VMatrix* pViewToProjection, VMatrix* pWorldToProjection, VMatrix* pWorldToPixels ) = 0;
	virtual void      DrawBrushModelEx( void* pEntity, void* pModel, const Vector& vecOrigin, const Vector& angView, int nMode ) = 0;
	virtual bool      DoesBrushModelNeedPowerOf2Framebuffer( const void* pModel ) = 0;
	virtual void      DrawBrushModelArray( void* pContext, int nCount, const void* pInstanceData, int nModelTypeFlags ) = 0;
};
