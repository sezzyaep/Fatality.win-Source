#pragma once
class CMeshBuilder;
//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class IServerEntity;
class IClientEntity;
class IToolSystem;
class IClientRenderable;
class Vector;
class QAngle;


//-----------------------------------------------------------------------------
// Safe accessor to an entity
//-----------------------------------------------------------------------------
typedef unsigned int HTOOLHANDLE;
enum
{
	HTOOLHANDLE_INVALID = 0
};


//-----------------------------------------------------------------------------
// Flags for the creation method
//-----------------------------------------------------------------------------
enum ShadowFlags_t
{
	SHADOW_FLAGS_FLASHLIGHT = ( 1 << 0 ),
	SHADOW_FLAGS_SHADOW = ( 1 << 1 ),
	SHADOW_FLAGS_SIMPLE_PROJECTION = ( 1 << 2 ),

	// Update this if you add flags
	SHADOW_FLAGS_LAST_FLAG = SHADOW_FLAGS_SIMPLE_PROJECTION
};

//-----------------------------------------------------------------------------
// If you change this, change the flags in IClientShadowMgr.h also
//-----------------------------------------------------------------------------
enum ClientShadowFlags_t
{
	SHADOW_FLAGS_USE_RENDER_TO_TEXTURE = ( SHADOW_FLAGS_LAST_FLAG << 1 ),
	SHADOW_FLAGS_ANIMATING_SOURCE = ( SHADOW_FLAGS_LAST_FLAG << 2 ),
	SHADOW_FLAGS_USE_DEPTH_TEXTURE = ( SHADOW_FLAGS_LAST_FLAG << 3 ),
	SHADOW_FLAGS_CUSTOM_DRAW = ( SHADOW_FLAGS_LAST_FLAG << 4 ),
	// Update this if you add flags
	CLIENT_SHADOW_FLAGS_LAST_FLAG = SHADOW_FLAGS_CUSTOM_DRAW
};


//-----------------------------------------------------------------------------
// Opaque pointer returned from Find* methods, don't store this, you need to
// Attach it to a tool entity or discard after searching
//-----------------------------------------------------------------------------
typedef void* EntitySearchResult;
typedef void* ParticleSystemSearchResult;


//-----------------------------------------------------------------------------
// Purpose: Client side tool interace (right now just handles IClientRenderables).
//  In theory could support hooking into client side entities directly
//-----------------------------------------------------------------------------
class IClientTools
{
public:
	virtual ~IClientTools() = 0;
	// Allocates or returns the handle to an entity previously found using the Find* APIs below
	virtual HTOOLHANDLE		AttachToEntity( EntitySearchResult entityToAttach ) = 0;
	virtual void			DetachFromEntity( EntitySearchResult entityToDetach ) = 0;

	virtual EntitySearchResult	GetEntity( HTOOLHANDLE handle ) = 0;

	// Checks whether a handle is still valid.
	virtual bool			IsValidHandle( HTOOLHANDLE handle ) = 0;

	// Iterates the list of entities which have been associated with tools
	virtual int				GetNumRecordables() = 0;
	virtual HTOOLHANDLE		GetRecordable( int index ) = 0;

	// Iterates through ALL entities (separate list for client vs. server)
	virtual EntitySearchResult	NextEntity( EntitySearchResult currentEnt ) = 0;
	EntitySearchResult			FirstEntity() { return NextEntity( NULL ); }

	// Use this to turn on/off the presence of an underlying game entity
	virtual void			SetEnabled( HTOOLHANDLE handle, bool enabled ) = 0;
	// Use this to tell an entity to post "state" to all listening tools
	virtual void			SetRecording( HTOOLHANDLE handle, bool recording ) = 0;
	// Some entities are marked with ShouldRecordInTools false, such as ui entities, etc.
	virtual bool			ShouldRecord( HTOOLHANDLE handle ) = 0;

	virtual HTOOLHANDLE		GetToolHandleForEntityByIndex( int entindex ) = 0;

	virtual int				GetModelIndex( HTOOLHANDLE handle ) = 0;
	virtual const char* GetModelName( HTOOLHANDLE handle ) = 0;
	virtual const char* GetClassname( HTOOLHANDLE handle ) = 0;

	virtual void			AddClientRenderable( IClientRenderable* pRenderable, bool bDrawWithViewModels, RenderableTranslucencyType_t nType, int nModelType = 0 ) = 0;
	virtual void			RemoveClientRenderable( IClientRenderable* pRenderable ) = 0;
	virtual void			SetTranslucencyType( IClientRenderable* pRenderable, RenderableTranslucencyType_t nType ) = 0;
	virtual void			MarkClientRenderableDirty( IClientRenderable* pRenderable ) = 0;
	virtual void			UpdateProjectedTexture( ClientShadowHandle_t h, bool bForce ) = 0;


	virtual bool			DrawSprite( IClientRenderable* pRenderable, float scale, float frame, int rendermode, int renderfx, const Color& color, float flProxyRadius, int* pVisHandle ) = 0;
	virtual void			DrawSprite( const Vector& vecOrigin, float flWidth, float flHeight, color32 color ) = 0;

	virtual EntitySearchResult	GetLocalPlayer() = 0;
	virtual bool			GetLocalPlayerEyePosition( Vector& org, QAngle& ang, float& fov ) = 0;

	// See ClientShadowFlags_t above
	virtual ClientShadowHandle_t CreateShadow( CBaseHandle handle, int nFlags ) = 0;
	virtual void			DestroyShadow( ClientShadowHandle_t h ) = 0;

	virtual ClientShadowHandle_t CreateFlashlight( const FlashlightState_t& lightState ) = 0;
	virtual void			DestroyFlashlight( ClientShadowHandle_t h ) = 0;
	virtual void			UpdateFlashlightState( ClientShadowHandle_t h, const FlashlightState_t& lightState ) = 0;

	virtual void			AddToDirtyShadowList( ClientShadowHandle_t h, bool force = false ) = 0;
	virtual void			MarkRenderToTextureShadowDirty( ClientShadowHandle_t h ) = 0;

	// Global toggle for recording
	virtual void			EnableRecordingMode( bool bEnable ) = 0;
	virtual bool			IsInRecordingMode() const = 0;

	// Trigger a temp entity
	virtual void			TriggerTempEntity( KeyValues* pKeyValues ) = 0;

	// get owning weapon (for viewmodels)
	virtual int				GetOwningWeaponEntIndex( int entindex ) = 0;
	virtual int				GetEntIndex( EntitySearchResult entityToAttach ) = 0;

	virtual int				FindGlobalFlexcontroller( char const* name ) = 0;
	virtual char const* GetGlobalFlexControllerName( int idx ) = 0;

	// helper for traversing ownership hierarchy
	virtual EntitySearchResult	GetOwnerEntity( EntitySearchResult currentEnt ) = 0;

	// common and useful types to query for hierarchically
	virtual bool			IsPlayer( EntitySearchResult currentEnt ) = 0;
	virtual bool			IsCombatCharacter( EntitySearchResult currentEnt ) = 0;
	virtual bool			IsNPC( EntitySearchResult currentEnt ) = 0;
	virtual bool			IsRagdoll( EntitySearchResult currentEnt ) = 0;
	virtual bool			IsViewModel( EntitySearchResult currentEnt ) = 0;
	virtual bool			IsViewModelOrAttachment( EntitySearchResult currentEnt ) = 0;
	virtual bool			IsWeapon( EntitySearchResult currentEnt ) = 0;
	virtual bool			IsSprite( EntitySearchResult currentEnt ) = 0;
	virtual bool			IsProp( EntitySearchResult currentEnt ) = 0;
	virtual bool			IsBrush( EntitySearchResult currentEnt ) = 0;

	virtual Vector			GetAbsOrigin( HTOOLHANDLE handle ) = 0;
	virtual QAngle			GetAbsAngles( HTOOLHANDLE handle ) = 0;

	// This reloads a portion or all of a particle definition file.
	// It's up to the client to decide if it cares about this file
	// Use a UtlBuffer to crack the data
	virtual void			ReloadParticleDefintions( const char* pFileName, const void* pBufData, int nLen ) = 0;

	// ParticleSystem iteration, query, modification
	virtual ParticleSystemSearchResult	FirstParticleSystem() { return NextParticleSystem( NULL ); }
	virtual ParticleSystemSearchResult	NextParticleSystem( ParticleSystemSearchResult sr ) = 0;
	virtual void						SetRecording( ParticleSystemSearchResult sr, bool bRecord ) = 0;

	// Sends a mesage from the tool to the client
	virtual void			PostToolMessage( KeyValues* pKeyValues ) = 0;

	// Indicates whether the client should render particle systems
	virtual void			EnableParticleSystems( bool bEnable ) = 0;

	// Is the game rendering in 3rd person mode?
	virtual bool			IsRenderingThirdPerson() const = 0;
};

struct IndexDesc_t
{
	// Pointers to the index data
	unsigned short* m_pIndices;

	// The offset in bytes of the memory we're writing into
	// from the start of the D3D buffer (will be 0 for static meshes)
	unsigned int	m_nOffset;

	// The first index (used for buffered index buffers, or cards that don't support stream offset)
	unsigned int	m_nFirstIndex;

	// 1 if the device is active, 0 if the device isn't active.
	// Faster than doing if checks for null m_pIndices if someone is
	// trying to write the m_pIndices while the device is inactive.
	unsigned int	m_nIndexSize;
};

enum
{
	VERTEX_MAX_TEXTURE_COORDINATES = 8,
	BONE_MATRIX_INDEX_INVALID = 255
};

struct VertexDesc_t
{
	// These can be set to zero if there are pointers to dummy buffers, when the
	// actual buffer format doesn't contain the data but it needs to be safe to
	// use all the CMeshBuilder functions.
	int	m_VertexSize_Position;
	int m_VertexSize_BoneWeight;
	int m_VertexSize_BoneMatrixIndex;
	int	m_VertexSize_Normal;
	int	m_VertexSize_Color;
	int	m_VertexSize_Specular;
	int m_VertexSize_TexCoord[ VERTEX_MAX_TEXTURE_COORDINATES ];
	int m_VertexSize_TangentS;
	int m_VertexSize_TangentT;
	int m_VertexSize_Wrinkle;

	int m_VertexSize_UserData;

	int m_ActualVertexSize;	// Size of the vertices.. Some of the m_VertexSize_ elements above
							// are set to this value and some are set to zero depending on which
							// fields exist in a buffer's vertex format.

	// The type of compression applied to this vertex data
	VertexCompressionType_t m_CompressionType;

	// Number of bone weights per vertex...
	int m_NumBoneWeights;

	// Pointers to our current vertex data
	float* m_pPosition;

	float* m_pBoneWeight;

#ifndef NEW_SKINNING
	unsigned char* m_pBoneMatrixIndex;
#else
	float* m_pBoneMatrixIndex;
#endif

	float* m_pNormal;

	unsigned char* m_pColor;
	unsigned char* m_pSpecular;
	float* m_pTexCoord[ VERTEX_MAX_TEXTURE_COORDINATES ];

	// Tangent space *associated with one particular set of texcoords*
	float* m_pTangentS;
	float* m_pTangentT;

	float* m_pWrinkle;

	// user data
	float* m_pUserData;

	// The first vertex index (used for buffered vertex buffers, or cards that don't support stream offset)
	int	m_nFirstVertex;

	// The offset in bytes of the memory we're writing into
	// from the start of the D3D buffer (will be 0 for static meshes)
	unsigned int	m_nOffset;

#ifdef DEBUG_WRITE_COMBINE
	int m_nLastWrittenField;
	unsigned char* m_pLastWrittenAddress;
#endif
};

class IIndexBuffer
{
public:
	// NOTE: The following two methods are only valid for static index buffers
	// Returns the number of indices and the format of the index buffer
	virtual int IndexCount() const = 0;
	virtual MaterialIndexFormat_t IndexFormat() const = 0;

	// Is this index buffer dynamic?
	virtual bool IsDynamic() const = 0;

	// NOTE: For dynamic index buffers only!
	// Casts the memory of the dynamic index buffer to the appropriate type
	virtual void BeginCastBuffer( MaterialIndexFormat_t format ) = 0;
	virtual void EndCastBuffer() = 0;

	// Returns the number of indices that can still be written into the buffer
	virtual int GetRoomRemaining() const = 0;

	// Locks, unlocks the index buffer
	virtual bool Lock( int nMaxIndexCount, bool bAppend, IndexDesc_t& desc ) = 0;
	virtual void Unlock( int nWrittenIndexCount, IndexDesc_t& desc ) = 0;

	// FIXME: Remove this!! Here only for backward compat on IMesh
	// Locks, unlocks the index buffer for modify
	virtual void ModifyBegin( bool bReadOnly, int nFirstIndex, int nIndexCount, IndexDesc_t& desc ) = 0;
	virtual void ModifyEnd( IndexDesc_t& desc ) = 0;

	// Spews the mesh data
	virtual void Spew( int nIndexCount, const IndexDesc_t& desc ) = 0;

	// Ensures the data in the index buffer is valid
	virtual void ValidateData( int nIndexCount, const IndexDesc_t& desc ) = 0;

	// For backward compat to IMesh
	virtual IMesh* GetMesh() = 0;
};

//-----------------------------------------------------------------------------
// The Mesh memory descriptor
//-----------------------------------------------------------------------------
struct MeshDesc_t : public VertexDesc_t, public IndexDesc_t
{};

class CIndexBuilder : private IndexDesc_t
{
public:
	CIndexBuilder();
	~CIndexBuilder();

	// Resets the mesh builder so it points to the start of everything again
	void Reset();

	// Generates indices for a particular primitive type
	void GenerateIndices( MaterialPrimitiveType_t primitiveType, int nIndexCount );

	// FIXME: Remove! Backward compat so we can use this from a CMeshBuilder.
	void AttachBegin( IMesh* pMesh, int nMaxIndexCount, const MeshDesc_t& desc );
	void AttachEnd();
	void GenerateSequentialIndexBuffer( unsigned short* pIndices, int nIndexCount, int nFirstVertex );
	void GenerateQuadIndexBuffer( unsigned short* pIndices, int nIndexCount, int nFirstVertex );

	inline void Index( unsigned short nIndex )
	{
		Assert( m_pIndices );
		Assert( m_nCurrentIndex < m_nMaxIndexCount );
		m_pIndices[ m_nCurrentIndex ] = ( unsigned short ) ( m_nIndexOffset + nIndex );
	}

	void AdvanceIndices( int nIndices )
	{
		m_nCurrentIndex += nIndices * m_nIndexSize;
		if ( m_nCurrentIndex > m_nIndexCount )
		{
			m_nIndexCount = m_nCurrentIndex;
		}
	}

	inline void AdvanceIndex()
	{
		m_nCurrentIndex += m_nIndexSize;
		if ( m_nCurrentIndex > m_nIndexCount )
		{
			m_nIndexCount = m_nCurrentIndex;
		}
	}

	int IndexCount() const
	{
		return m_nIndexCount;
	}

private:
	// The mesh we're modifying
	IIndexBuffer* m_pIndexBuffer;

	// Max number of indices
	int				m_nMaxIndexCount;

	// Number of indices
	int				m_nIndexCount;

	// Offset to add to each index as it's written into the buffer
	int				m_nIndexOffset{};

	// The current index
	mutable int		m_nCurrentIndex;

	// Total number of indices appended
	int				m_nTotalIndexCount;

	// First index buffer offset + first index
	unsigned int	m_nBufferOffset;
	unsigned int	m_nBufferFirstIndex;

	// Used to make sure Begin/End calls and BeginModify/EndModify calls match.
	bool			m_bModify{};
};

//-----------------------------------------------------------------------------
// Used in lists of indexed primitives.
//-----------------------------------------------------------------------------
class CPrimList
{
public:
	CPrimList();
	CPrimList( int nFirstIndex, int nIndexCount );

	int			m_FirstIndex{};
	int			m_NumIndices{};
};

inline CPrimList::CPrimList()
{}

inline CPrimList::CPrimList( int nFirstIndex, int nIndexCount )
{
	m_FirstIndex = nFirstIndex;
	m_NumIndices = nIndexCount;
}

class IVertexBuffer
{
public:
	// NOTE: The following two methods are only valid for static vertex buffers
	// Returns the number of vertices and the format of the vertex buffer
	virtual int VertexCount() const = 0;
	virtual VertexFormat_t GetVertexFormat() const = 0;

	// Is this vertex buffer dynamic?
	virtual bool IsDynamic() const = 0;

	// NOTE: For dynamic vertex buffers only!
	// Casts the memory of the dynamic vertex buffer to the appropriate type
	virtual void BeginCastBuffer( VertexFormat_t format ) = 0;
	virtual void EndCastBuffer() = 0;

	// Returns the number of vertices that can still be written into the buffer
	virtual int GetRoomRemaining() const = 0;

	virtual bool Lock( int nVertexCount, bool bAppend, VertexDesc_t& desc ) = 0;
	virtual void Unlock( int nVertexCount, VertexDesc_t& desc ) = 0;

	// Spews the mesh data
	virtual void Spew( int nVertexCount, const VertexDesc_t& desc ) = 0;

	// Call this in debug mode to make sure our data is good.
	virtual void ValidateData( int nVertexCount, const VertexDesc_t& desc ) = 0;
};

class IMesh : public IVertexBuffer, public IIndexBuffer
{
public:
	// -----------------------------------

	// Sets/gets the primitive type
	virtual void SetPrimitiveType( MaterialPrimitiveType_t type ) = 0;

	// Draws the mesh
	virtual void Draw( int nFirstIndex = -1, int nIndexCount = 0 ) = 0;

	virtual void SetColorMesh( IMesh* pColorMesh, int nVertexOffset ) = 0;

	// Draw a list of (lists of) primitives. Batching your lists together that use
	// the same lightmap, material, vertex and index buffers with multipass shaders
	// can drastically reduce state-switching overhead.
	// NOTE: this only works with STATIC meshes.
	virtual void Draw( CPrimList* pLists, int nLists ) = 0;

	// Copy verts and/or indices to a mesh builder. This only works for temp meshes!
	virtual void CopyToMeshBuilder(
		int iStartVert,		// Which vertices to copy.
		int nVerts,
		int iStartIndex,	// Which indices to copy.
		int nIndices,
		int indexOffset,	// This is added to each index.
		CMeshBuilder& builder ) = 0;

	// Spews the mesh data
	virtual void Spew( int nVertexCount, int nIndexCount, const MeshDesc_t& desc ) = 0;

	// Call this in debug mode to make sure our data is good.
	virtual void ValidateData( int nVertexCount, int nIndexCount, const MeshDesc_t& desc ) = 0;

	// New version
	// Locks/unlocks the mesh, providing space for nVertexCount and nIndexCount.
	// nIndexCount of -1 means don't lock the index buffer...
	virtual void LockMesh( int nVertexCount, int nIndexCount, MeshDesc_t& desc, void* pSettings = 0 ) = 0;
	virtual void ModifyBegin( int nFirstVertex, int nVertexCount, int nFirstIndex, int nIndexCount, MeshDesc_t& desc ) = 0;
	virtual void ModifyEnd( MeshDesc_t& desc ) = 0;
	virtual void UnlockMesh( int nVertexCount, int nIndexCount, MeshDesc_t& desc ) = 0;

	virtual void ModifyBeginEx( bool bReadOnly, int nFirstVertex, int nVertexCount, int nFirstIndex, int nIndexCount, MeshDesc_t& desc ) = 0;

	virtual void SetFlexMesh( IMesh* pMesh, int nVertexOffset ) = 0;

	virtual void DisableFlexMesh() = 0;

	virtual void MarkAsDrawn() = 0;

	// NOTE: I chose to create this method strictly because it's 2 days to code lock
	// and I could use the DrawInstances technique without a larger code change
	// Draws the mesh w/ modulation.
	virtual void DrawModulated( const Vector4D& diffuseModulation, int nFirstIndex = -1, int nIndexCount = 0 ) = 0;

	virtual unsigned int ComputeMemoryUsed() = 0;

	virtual void* AccessRawHardwareDataStream( uint8_t nRawStreamIndex, uint32 numBytes, uint32 uiFlags, void* pvContext ) = 0;

	virtual void* GetCachedPerFrameMeshData() = 0;
	virtual void ReconstructFromCachedPerFrameMeshData( void* pData ) = 0;
};


#define INVALID_BUFFER_OFFSET 0xFFFFFFFFUL

// flags for advancevertex optimization
#define VTX_HAVEPOS 1
#define VTX_HAVENORMAL 2
#define VTX_HAVECOLOR 4
#define VTX_HAVEALL ( VTX_HAVEPOS | VTX_HAVENORMAL | VTX_HAVECOLOR )


//-----------------------------------------------------------------------------
//
// Helper class used to define vertex buffers
//
//-----------------------------------------------------------------------------
class CVertexBuilder : private VertexDesc_t
{
public:
	CVertexBuilder();
	~CVertexBuilder();

	// returns the number of vertices
	int VertexCount() const;

	template<int nFlags, int nNumTexCoords> void AdvanceVertexF();

	// Resets the mesh builder so it points to the start of everything again
	void Reset();

	void AdvanceVertex();

	void Position3f( float x, float y, float z );
	void Position3fv( const float* v );
	void Position3fv( int nVertexOffset, const float* v );
	void Normal3f( float nx, float ny, float nz );
	void UserData( const float* pData );

	void Color4ubv( unsigned char const* rgb );
	void Color4ub( unsigned char r, unsigned char g, unsigned char b, unsigned char a );

	void TexCoord2f( int stage, float s, float t );
	void TexCoord2f( int nVertexOffset, int stage, float s, float t );

	// FIXME: Remove! Backward compat so we can use this from a CMeshBuilder.
	void AttachBegin( IMesh* pMesh, int nMaxVertexCount, const MeshDesc_t& desc );
	void AttachEnd();

private:
	// The vertex buffer we're modifying
	IVertexBuffer* m_pVertexBuffer;

	// Used to make sure Begin/End calls and BeginModify/EndModify calls match.
	bool m_bModify{};

	// Max number of indices and vertices
	int m_nMaxVertexCount;

	// Number of indices and vertices
	int m_nVertexCount;

	// The current vertex and index
	mutable int m_nCurrentVertex;

	// Optimization: Pointer to the current pos, norm, texcoord, and color
	mutable float* m_pCurrPosition{};
	mutable float* m_pCurrNormal{};
	mutable unsigned char* m_pCurrColor{};
	mutable float* m_pCurrTexCoord[ VERTEX_MAX_TEXTURE_COORDINATES ]{};

	// Total number of vertices appended
	int m_nTotalVertexCount;

	// First vertex buffer offset + index
	unsigned int m_nBufferOffset;
	unsigned int m_nBufferFirstVertex;

#if ( COMPRESSED_NORMALS_TYPE == COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4 )
	// Debug checks to make sure we write userdata4/tangents AFTER normals
	bool m_bWrittenNormal : 1{};
	bool m_bWrittenUserData : 1{};
#endif

	friend class CMeshBuilder;
};


//-----------------------------------------------------------------------------
//
// Inline methods of CVertexBuilder
//
//-----------------------------------------------------------------------------
inline CVertexBuilder::CVertexBuilder() : VertexDesc_t()
{
	m_pVertexBuffer = NULL;
	m_nBufferOffset = INVALID_BUFFER_OFFSET;
	m_nBufferFirstVertex = 0;
	m_nVertexCount = 0;
	m_nCurrentVertex = 0;
	m_nMaxVertexCount = 0;
	m_nTotalVertexCount = 0;
	m_CompressionType = VERTEX_COMPRESSION_INVALID;

#ifdef _DEBUG
	m_pCurrPosition = NULL;
	m_pCurrNormal = NULL;
	m_pCurrColor = NULL;
	memset( m_pCurrTexCoord, 0, sizeof( m_pCurrTexCoord ) );
	m_bModify = false;
#endif
}

inline CVertexBuilder::~CVertexBuilder()
{
	if ( m_pVertexBuffer && m_pVertexBuffer->IsDynamic() )
	{
		m_pVertexBuffer->EndCastBuffer();
	}
}

//-----------------------------------------------------------------------------
// FIXME: Remove! Backward compat so we can use this from a CMeshBuilder.
//-----------------------------------------------------------------------------
inline void CVertexBuilder::AttachBegin( IMesh* pMesh, int nMaxVertexCount, const MeshDesc_t& desc )
{
	m_pVertexBuffer = pMesh;
	memcpy( static_cast< VertexDesc_t* >( this ), static_cast< const VertexDesc_t* >( &desc ), sizeof( VertexDesc_t ) );
	m_nMaxVertexCount = nMaxVertexCount;
	m_NumBoneWeights = m_NumBoneWeights == 0 ? 0 : 2;	// Two weights if any
	m_nVertexCount = 0;
	m_bModify = false;

	if ( m_nBufferOffset == INVALID_BUFFER_OFFSET )
	{
		m_nTotalVertexCount = 0;
		m_nBufferOffset = static_cast< const VertexDesc_t* >( &desc )->m_nOffset;
		m_nBufferFirstVertex = desc.m_nFirstVertex;
	}
}

inline void CVertexBuilder::AttachEnd()
{
	// Make sure they called Begin()
	Assert( !m_bModify );

	m_nMaxVertexCount = 0;
	m_pVertexBuffer = NULL;

	m_CompressionType = VERTEX_COMPRESSION_INVALID;

#ifdef _DEBUG
	// Null out our pointers...
	m_pCurrPosition = NULL;
	m_pCurrNormal = NULL;
	m_pCurrColor = NULL;
	memset( m_pCurrTexCoord, 0, sizeof( m_pCurrTexCoord ) );
	memset( static_cast< VertexDesc_t* >( this ), 0, sizeof( VertexDesc_t ) );
#endif
}

// number of elements in an array of static size
#define NELEMS(x) ((sizeof(x))/sizeof(x[0]))

//-----------------------------------------------------------------------------
// Resets the vertex buffer builder so it points to the start of everything again
//-----------------------------------------------------------------------------
inline void CVertexBuilder::Reset()
{
	m_nCurrentVertex = 0;

	m_pCurrPosition = m_pPosition;
	m_pCurrNormal = m_pNormal;
	for ( int i = 0; i < NELEMS( m_pCurrTexCoord ); i++ )
	{
		m_pCurrTexCoord[ i ] = m_pTexCoord[ i ];
	}
	m_pCurrColor = m_pColor;
}


//-----------------------------------------------------------------------------
// returns the number of vertices
//-----------------------------------------------------------------------------
inline int CVertexBuilder::VertexCount() const
{
	return m_nVertexCount;
}

inline void IncrementFloatPointer( float*& pBufferPointer, int vertexSize )
{
	pBufferPointer = reinterpret_cast< float* >( reinterpret_cast< unsigned char* >( pBufferPointer ) + vertexSize );
}

//-----------------------------------------------------------------------------
// Advances vertex after you're done writing to it.
//-----------------------------------------------------------------------------

template<int nFlags, int nNumTexCoords> FORCEINLINE void CVertexBuilder::AdvanceVertexF()
{
	if ( ++m_nCurrentVertex > m_nVertexCount )
	{
		m_nVertexCount = m_nCurrentVertex;
	}

	if ( nFlags & VTX_HAVEPOS )
		IncrementFloatPointer( m_pCurrPosition, m_VertexSize_Position );
	if ( nFlags & VTX_HAVENORMAL )
		IncrementFloatPointer( m_pCurrNormal, m_VertexSize_Normal );
	if ( nFlags & VTX_HAVECOLOR )
		m_pCurrColor += m_VertexSize_Color;

	if ( nNumTexCoords > 0 )
		IncrementFloatPointer( m_pCurrTexCoord[ 0 ], m_VertexSize_TexCoord[ 0 ] );
	if ( nNumTexCoords > 1 )
		IncrementFloatPointer( m_pCurrTexCoord[ 1 ], m_VertexSize_TexCoord[ 1 ] );
	if ( nNumTexCoords > 2 )
		IncrementFloatPointer( m_pCurrTexCoord[ 2 ], m_VertexSize_TexCoord[ 2 ] );
	if ( nNumTexCoords > 3 )
		IncrementFloatPointer( m_pCurrTexCoord[ 3 ], m_VertexSize_TexCoord[ 3 ] );
	if ( nNumTexCoords > 4 )
		IncrementFloatPointer( m_pCurrTexCoord[ 4 ], m_VertexSize_TexCoord[ 4 ] );
	if ( nNumTexCoords > 5 )
		IncrementFloatPointer( m_pCurrTexCoord[ 5 ], m_VertexSize_TexCoord[ 5 ] );
	if ( nNumTexCoords > 6 )
		IncrementFloatPointer( m_pCurrTexCoord[ 6 ], m_VertexSize_TexCoord[ 6 ] );
	if ( nNumTexCoords > 7 )
		IncrementFloatPointer( m_pCurrTexCoord[ 7 ], m_VertexSize_TexCoord[ 7 ] );
}

inline void CVertexBuilder::AdvanceVertex()
{
	AdvanceVertexF<VTX_HAVEALL, 8>();
}

//-----------------------------------------------------------------------------
// Utility methods for buffer builders
//-----------------------------------------------------------------------------
inline float* OffsetFloatPointer( float* pBufferPointer, int nVertexCount, int vertexSize )
{
	return reinterpret_cast< float* >(
		reinterpret_cast< unsigned char* >( pBufferPointer ) +
		nVertexCount * vertexSize );
}

inline void	CVertexBuilder::Position3f( float x, float y, float z )
{
	Assert( m_pPosition && m_pCurrPosition );
	Assert( IsFinite( x ) && IsFinite( y ) && IsFinite( z ) );
	float* pDst = m_pCurrPosition;
	*pDst++ = x;
	*pDst++ = y;
	*pDst = z;
}

inline void	CVertexBuilder::Position3fv( const float* v )
{
	Assert( v );
	Assert( m_pPosition && m_pCurrPosition );

	float* pDst = m_pCurrPosition;
	*pDst++ = *v++;
	*pDst++ = *v++;
	*pDst = *v;
}

inline void	CVertexBuilder::Position3fv( int nVertexOffset, const float* v )
{
	Assert( v );
	Assert( m_pPosition && m_pCurrPosition );
	float* pDst = OffsetFloatPointer( m_pCurrPosition, nVertexOffset, m_VertexSize_Position );
	*pDst++ = *v++;
	*pDst++ = *v++;
	*pDst = *v;
}

inline void	CVertexBuilder::Normal3f( float nx, float ny, float nz )
{
	float* pDst = m_pCurrNormal;
	*pDst++ = nx;
	*pDst++ = ny;
	*pDst = nz;
}

inline void CVertexBuilder::UserData( const float* pData )
{
	Assert( m_CompressionType == VERTEX_COMPRESSION_NONE ); // Use the templatized version if you want to support compression
	Assert( pData );

	int userDataSize = 4; // garymcthack
	float* pUserData = OffsetFloatPointer( m_pUserData, m_nCurrentVertex, m_VertexSize_UserData );
	memcpy( pUserData, pData, sizeof( float ) * userDataSize );
}

inline void CVertexBuilder::Color4ubv( unsigned char const* rgba )
{
	Assert( rgba );
	Assert( m_pColor && m_pCurrColor );
#ifdef OPENGL_SWAP_COLORS
	int col = rgba[ 0 ] | ( rgba[ 1 ] << 8 ) | ( rgba[ 2 ] << 16 ) | ( rgba[ 3 ] << 24 );	// r, g, b, a in memory
#elif defined( CELL_GCM_SWAP_COLORS )
	int col = ( rgba[ 0 ] << 24 ) | ( rgba[ 1 ] << 16 ) | ( rgba[ 2 ] << 8 ) | rgba[ 3 ];
#else
	int col = rgba[ 2 ] | ( rgba[ 1 ] << 8 ) | ( rgba[ 0 ] << 16 ) | ( rgba[ 3 ] << 24 );
#endif
	* ( int* ) m_pCurrColor = col;
}

inline void CVertexBuilder::Color4ub( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	Assert( m_pColor && m_pCurrColor );
#ifdef OPENGL_SWAP_COLORS
	int col = r | ( g << 8 ) | ( b << 16 ) | ( a << 24 );	// r, g, b, a in memory
#elif defined( CELL_GCM_SWAP_COLORS )
	int col = ( r << 24 ) | ( g << 16 ) | ( b << 8 ) | a;
#else
	int col = b | ( g << 8 ) | ( r << 16 ) | ( a << 24 );
#endif

	* ( int* ) m_pCurrColor = col;
}

inline void	CVertexBuilder::TexCoord2f( int nStage, float s, float t )
{
	Assert( m_pTexCoord[ nStage ] && m_pCurrTexCoord[ nStage ] );
	Assert( IsFinite( s ) && IsFinite( t ) );

	float* pDst = m_pCurrTexCoord[ nStage ];
	*pDst++ = s;
	*pDst = t;
}

inline void	CVertexBuilder::TexCoord2f( int nVertexOffset, int nStage, float s, float t )
{
	Assert( m_pTexCoord[ nStage ] && m_pCurrTexCoord[ nStage ] );
	Assert( IsFinite( s ) && IsFinite( t ) );

	float* pDst = OffsetFloatPointer( m_pCurrTexCoord[ nStage ], nVertexOffset, m_VertexSize_TexCoord[ nStage ] );
	*pDst++ = s;
	*pDst = t;
}

//-----------------------------------------------------------------------------
//
// Inline methods related to CIndexBuilder
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
inline CIndexBuilder::CIndexBuilder() : IndexDesc_t(), m_pIndexBuffer( 0 ), m_nIndexCount( 0 ),
m_nCurrentIndex( 0 ), m_nMaxIndexCount( 0 )
{
	m_nTotalIndexCount = 0;
	m_nBufferOffset = INVALID_BUFFER_OFFSET;
	m_nBufferFirstIndex = 0;
#ifdef _DEBUG
	m_bModify = false;
#endif
}

inline CIndexBuilder::~CIndexBuilder()
{
	if ( m_pIndexBuffer && m_pIndexBuffer->IsDynamic() )
	{
		m_pIndexBuffer->EndCastBuffer();
	}
}

//-----------------------------------------------------------------------------
// FIXME: Remove! Backward compat so we can use this from a CMeshBuilder.
//-----------------------------------------------------------------------------
inline void CIndexBuilder::AttachBegin( IMesh* pMesh, int nMaxIndexCount, const MeshDesc_t& desc )
{
	m_pIndexBuffer = pMesh;
	m_nIndexCount = 0;
	m_nMaxIndexCount = nMaxIndexCount;

	m_bModify = false;

	// Copy relevant data from the mesh desc
	m_nIndexOffset = desc.m_nFirstVertex;
	m_pIndices = desc.m_pIndices;
	m_nIndexSize = desc.m_nIndexSize;

	// Point to the start of the buffers..
	Reset();
}

inline void CIndexBuilder::AttachEnd()
{
	Assert( m_pIndexBuffer );
	Assert( !m_bModify );	// Make sure they called AttachBegin.

	m_pIndexBuffer = 0;
	m_nMaxIndexCount = 0;

#ifdef _DEBUG
	// Null out our data...
	memset( ( IndexDesc_t* ) this, 0, sizeof( IndexDesc_t ) );
#endif
}

//-----------------------------------------------------------------------------
// Helper methods to create various standard index buffer types
//-----------------------------------------------------------------------------
inline void CIndexBuilder::GenerateSequentialIndexBuffer( unsigned short* pIndices, int nIndexCount, int nFirstVertex )
{
	if ( !pIndices )
		return;

	// Format the sequential buffer
	for ( int i = 0; i < nIndexCount; ++i )
	{
		pIndices[ i ] = ( unsigned short ) ( i + nFirstVertex );
	}
}

inline void CIndexBuilder::GenerateQuadIndexBuffer( unsigned short* pIndices, int nIndexCount, int nFirstVertex )
{
	if ( !pIndices )
		return;

	// Format the quad buffer
	int i;
	int numQuads = nIndexCount / 6;
	unsigned short baseVertex = ( unsigned short ) nFirstVertex;

	if ( ( ( size_t ) pIndices & 0x3 ) == 0 )
	{
		// Fast version, requires aligned indices
		int* pWrite = ( int* ) pIndices;
		int nWrite = ( baseVertex << 16 ) | baseVertex;
		for ( i = 0; i < numQuads; ++i )
		{
			pWrite[ 0 ] = nWrite + ( 1 << 16 );
			pWrite[ 1 ] = nWrite + 2;
			pWrite[ 2 ] = nWrite + ( 3 << 16 ) + 2;
			pWrite += 3;
			nWrite += ( 4 << 16 ) | 4;
		}
	}
	else
	{
		for ( i = 0; i < numQuads; ++i )
		{
			pIndices[ 0 ] = baseVertex;
			pIndices[ 1 ] = baseVertex + 1;
			pIndices[ 2 ] = baseVertex + 2;

			// Triangle 2
			pIndices[ 3 ] = baseVertex;
			pIndices[ 4 ] = baseVertex + 2;
			pIndices[ 5 ] = baseVertex + 3;

			baseVertex += 4;
			pIndices += 6;
		}
	}
}

//-----------------------------------------------------------------------------
// Generates indices for a particular primitive type
//-----------------------------------------------------------------------------
inline void CIndexBuilder::GenerateIndices( MaterialPrimitiveType_t primitiveType, int nIndexCount )
{
	// FIXME: How to make this work with short vs int sized indices?
	// Don't generate indices if we've got an empty buffer
	if ( m_nIndexSize == 0 )
		return;

	int nMaxIndices = m_nMaxIndexCount - m_nCurrentIndex;
	nIndexCount = std::min( nMaxIndices, nIndexCount );
	if ( nIndexCount == 0 )
		return;

	unsigned short* pIndices = &m_pIndices[ m_nCurrentIndex ];

	switch ( primitiveType )
	{
		//case MATERIAL_INSTANCED_QUADS:
		//	Assert( 0 ); // Shouldn't get here (this primtype is unindexed)
		//	break;
		case MATERIAL_QUADS:
			GenerateQuadIndexBuffer( pIndices, nIndexCount, m_nIndexOffset );
			break;
		//case MATERIAL_POLYGON:
		//	GeneratePolygonIndexBuffer( pIndices, nIndexCount, m_nIndexOffset );
		//	break;
		//case MATERIAL_LINE_STRIP:
		//	GenerateLineStripIndexBuffer( pIndices, nIndexCount, m_nIndexOffset );
		//	break;
		//case MATERIAL_LINE_LOOP:
		//	GenerateLineLoopIndexBuffer( pIndices, nIndexCount, m_nIndexOffset );
		//	break;
		//case MATERIAL_POINTS:
		//	Assert( 0 ); // Shouldn't get here (this primtype is unindexed)
		//	break;
		//case MATERIAL_SUBD_QUADS_EXTRA:
		//case MATERIAL_SUBD_QUADS_REG:
		default:
			GenerateSequentialIndexBuffer( pIndices, nIndexCount, m_nIndexOffset );
			break;
	}

	AdvanceIndices( nIndexCount );
}

//-----------------------------------------------------------------------------
// Resets the index buffer builder so it points to the start of everything again
//-----------------------------------------------------------------------------
inline void CIndexBuilder::Reset()
{
	m_nCurrentIndex = 0;
}

//-----------------------------------------------------------------------------
//
// Helper class used to define meshes
//
//-----------------------------------------------------------------------------
//class CMeshBuilder : private MeshDesc_t
// hack fixme
class CMeshBuilder : public MeshDesc_t
{
public:
	CMeshBuilder();
	~CMeshBuilder() { Assert( !m_pMesh ); }		// if this fires you did a Begin() without an End()

	operator CIndexBuilder& ( ) { return m_IndexBuilder; }

	// Locks the vertex buffer
	// (*cannot* use the Index() call below)
	void Begin( IMesh* pMesh, MaterialPrimitiveType_t type, int numPrimitives );
	void Begin( IMesh* pMesh, MaterialPrimitiveType_t type, int nVertexCount, int nIndexCount );

	// Use this when you're done writing
	// Set bDraw to true to call m_pMesh->Draw automatically.
	void End( bool bSpewData = false, bool bDraw = false );

	// Resets the mesh builder so it points to the start of everything again
	void Reset();
	// position setting
	void Position3fv( const float* v );
	void Position3fv( int nVertexOffset, const float* v );
	void Position3f( float x, float y, float z );
	void Normal3f( float nx, float ny, float nz );

	void TexCoord2f( int stage, float s, float t );
	void TexCoord2f( int nVertexOffset, int stage, float s, float t );

	// color setting
	void Color4ubv( unsigned char const* rgba );
	void Color4ub( unsigned char r, unsigned char g, unsigned char b, unsigned char a );
	void UserData( const float* pData );
	void Index( unsigned short idx );
	void AdvanceIndex();

	void AdvanceVertex();

private:
	// Computes number of verts and indices
	void ComputeNumVertsAndIndices( int* pMaxVertices, int* pMaxIndices,
		MaterialPrimitiveType_t type, int nPrimitiveCount );
	int IndicesFromVertices( MaterialPrimitiveType_t type, int nVertexCount );

	// The mesh we're modifying
	IMesh* m_pMesh;

	MaterialPrimitiveType_t m_Type;

	// Generate indices?
	bool m_bGenerateIndices;

	CIndexBuilder	m_IndexBuilder;
	CVertexBuilder	m_VertexBuilder;
};

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
inline CMeshBuilder::CMeshBuilder() : m_pMesh( 0 ), m_bGenerateIndices( false )
{
	VertexDesc_t::m_nOffset = 0;
	VertexDesc_t::m_CompressionType = VERTEX_COMPRESSION_NONE;
}


//-----------------------------------------------------------------------------
// Computes the number of verts and indices based on primitive type and count
//-----------------------------------------------------------------------------
inline void CMeshBuilder::ComputeNumVertsAndIndices( int* pMaxVertices, int* pMaxIndices,
													MaterialPrimitiveType_t type, int nPrimitiveCount )
{
	switch ( type )
	{
		case MATERIAL_POINTS:
			*pMaxVertices = *pMaxIndices = nPrimitiveCount;
			break;

		case MATERIAL_LINES:
			*pMaxVertices = *pMaxIndices = nPrimitiveCount * 2;
			break;

		case MATERIAL_LINE_STRIP:
			*pMaxVertices = nPrimitiveCount + 1;
			*pMaxIndices = nPrimitiveCount * 2;
			break;

		case MATERIAL_LINE_LOOP:
			*pMaxVertices = nPrimitiveCount;
			*pMaxIndices = nPrimitiveCount * 2;
			break;

		case MATERIAL_TRIANGLES:
			*pMaxVertices = *pMaxIndices = nPrimitiveCount * 3;
			break;

		case MATERIAL_TRIANGLE_STRIP:
			*pMaxVertices = *pMaxIndices = nPrimitiveCount + 2;
			break;

		case MATERIAL_QUADS:
			*pMaxVertices = nPrimitiveCount * 4;
			*pMaxIndices = nPrimitiveCount * 6;
			break;

		case MATERIAL_INSTANCED_QUADS:
			*pMaxVertices = nPrimitiveCount;
			*pMaxIndices = 0; // This primtype is unindexed
			break;

		case MATERIAL_POLYGON:
			*pMaxVertices = nPrimitiveCount;
			*pMaxIndices = ( nPrimitiveCount - 2 ) * 3;
			break;

		default:
			Assert( 0 );
	}

	// FIXME: need to get this from meshdx8.cpp, or move it to somewhere common
	Assert( *pMaxVertices <= 32768 );
	Assert( *pMaxIndices <= 32768 );
}


inline int CMeshBuilder::IndicesFromVertices( MaterialPrimitiveType_t type, int nVertexCount )
{
	switch ( type )
	{
		case MATERIAL_QUADS:
			Assert( ( nVertexCount & 0x3 ) == 0 );
			return ( nVertexCount * 6 ) / 4;

		case MATERIAL_INSTANCED_QUADS:
			// This primtype is unindexed
			return 0;

		case MATERIAL_POLYGON:
			Assert( nVertexCount >= 3 );
			return ( nVertexCount - 2 ) * 3;

		case MATERIAL_LINE_STRIP:
			Assert( nVertexCount >= 2 );
			return ( nVertexCount - 1 ) * 2;

		case MATERIAL_LINE_LOOP:
			Assert( nVertexCount >= 3 );
			return nVertexCount * 2;

		default:
			return nVertexCount;
	}
}

//-----------------------------------------------------------------------------
// Begins modifying the mesh
//-----------------------------------------------------------------------------
inline void CMeshBuilder::Begin( IMesh* pMesh, MaterialPrimitiveType_t type, int numPrimitives )
{
	Assert( pMesh && ( !m_pMesh ) );
	Assert( type != MATERIAL_HETEROGENOUS );

	m_pMesh = pMesh;
	m_bGenerateIndices = true;
	m_Type = type;

	int nMaxVertexCount = 0, nMaxIndexCount = 0;
	ComputeNumVertsAndIndices( &nMaxVertexCount, &nMaxIndexCount, type, numPrimitives );

	switch ( type )
	{
		case MATERIAL_INSTANCED_QUADS:
			m_pMesh->SetPrimitiveType( MATERIAL_INSTANCED_QUADS );
			break;

		case MATERIAL_QUADS:
		case MATERIAL_POLYGON:
			m_pMesh->SetPrimitiveType( MATERIAL_TRIANGLES );
			break;

		case MATERIAL_LINE_STRIP:
		case MATERIAL_LINE_LOOP:
			m_pMesh->SetPrimitiveType( MATERIAL_LINES );
			break;

		default:
			m_pMesh->SetPrimitiveType( type );
	}

	// Lock the mesh
	m_pMesh->LockMesh( nMaxVertexCount, nMaxIndexCount, *this );

	m_IndexBuilder.AttachBegin( pMesh, nMaxIndexCount, *this );
	m_VertexBuilder.AttachBegin( pMesh, nMaxVertexCount, *this );

	// Point to the start of the index and vertex buffers
	Reset();
}


inline void CMeshBuilder::Begin( IMesh* pMesh, MaterialPrimitiveType_t type, int nVertexCount, int nIndexCount )
{
	m_pMesh = pMesh;
	m_bGenerateIndices = false;
	m_Type = type;

	// Set the primitive type
	m_pMesh->SetPrimitiveType( type );

	// Lock the vertex and index buffer
	m_pMesh->LockMesh( nVertexCount, nIndexCount, *this );

	m_IndexBuilder.AttachBegin( pMesh, nIndexCount, *this );
	m_VertexBuilder.AttachBegin( pMesh, nVertexCount, *this );

	// Point to the start of the buffers..
	Reset();
}
//-----------------------------------------------------------------------------
// Use this when you're done modifying the mesh
//-----------------------------------------------------------------------------
inline void CMeshBuilder::End( bool bSpewData, bool bDraw )
{
	if ( m_bGenerateIndices )
	{
		int nIndexCount = IndicesFromVertices( m_Type, m_VertexBuilder.VertexCount() );
		m_IndexBuilder.GenerateIndices( m_Type, nIndexCount );
	}

	if ( bSpewData )
	{
		m_pMesh->Spew( m_VertexBuilder.VertexCount(), m_IndexBuilder.IndexCount(), *this );
	}

#ifdef _DEBUG
	m_pMesh->ValidateData( m_VertexBuilder.VertexCount(), m_IndexBuilder.IndexCount(), *this );
#endif

	// Unlock our buffers
	m_pMesh->UnlockMesh( m_VertexBuilder.VertexCount(), m_IndexBuilder.IndexCount(), *this );

	m_IndexBuilder.AttachEnd();
	m_VertexBuilder.AttachEnd();

	if ( bDraw )
	{
		m_pMesh->Draw();
	}

	m_pMesh = 0;

#ifdef _DEBUG
	memset( ( MeshDesc_t* ) this, 0, sizeof( MeshDesc_t ) );
#endif
}

//-----------------------------------------------------------------------------
// Resets the mesh builder so it points to the start of everything again
//-----------------------------------------------------------------------------
inline void CMeshBuilder::Reset()
{
	m_IndexBuilder.Reset();
	m_VertexBuilder.Reset();
}

FORCEINLINE void CMeshBuilder::AdvanceVertex()
{
	m_VertexBuilder.AdvanceVertex();
}

FORCEINLINE void CMeshBuilder::Position3fv( const float* v )
{
	m_VertexBuilder.Position3fv( v );
}

FORCEINLINE void CMeshBuilder::Position3fv( int nVertexOffset, const float* v )
{
	m_VertexBuilder.Position3fv( nVertexOffset, v );
}

FORCEINLINE void CMeshBuilder::Position3f( float x, float y, float z )
{
	m_VertexBuilder.Position3f( x, y, z );
}

FORCEINLINE void CMeshBuilder::Normal3f( float nx, float ny, float nz )
{
	m_VertexBuilder.Normal3f( nx, ny, nz );
}
FORCEINLINE void CMeshBuilder::TexCoord2f( int nStage, float s, float t )
{
	m_VertexBuilder.TexCoord2f( nStage, s, t );
}

FORCEINLINE void CMeshBuilder::TexCoord2f( int nVertexOffset, int nStage, float s, float t )
{
	m_VertexBuilder.TexCoord2f( nVertexOffset, nStage, s, t );
}

FORCEINLINE void CMeshBuilder::Color4ubv( unsigned char const* rgba )
{
	m_VertexBuilder.Color4ubv( rgba );
}

FORCEINLINE void CMeshBuilder::Color4ub( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	m_VertexBuilder.Color4ub( r, g, b, a );
}

FORCEINLINE void CMeshBuilder::UserData( const float* pData )
{
	m_VertexBuilder.UserData( pData );
}

FORCEINLINE void CMeshBuilder::Index( unsigned short idx )
{
	m_IndexBuilder.Index( idx );
}

FORCEINLINE void CMeshBuilder::AdvanceIndex()
{
	m_IndexBuilder.AdvanceIndex();
}