#pragma once

enum MDLCacheDataType_t
{
	// Callbacks to Get called when data is loaded or unloaded for these:
	MDLCACHE_STUDIOHDR = 0,
	MDLCACHE_STUDIOHWDATA,
	MDLCACHE_VCOLLIDE,

	// Callbacks NOT called when data is loaded or unloaded for these:
	MDLCACHE_ANIMBLOCK,
	MDLCACHE_VIRTUALMODEL,
	MDLCACHE_VERTEXES,
	MDLCACHE_DECODEDANIMBLOCK
};

enum MDLCacheFlush_t
{
	MDLCACHE_FLUSH_STUDIOHDR = 0x01,
	MDLCACHE_FLUSH_STUDIOHWDATA = 0x02,
	MDLCACHE_FLUSH_VCOLLIDE = 0x04,
	MDLCACHE_FLUSH_ANIMBLOCK = 0x08,
	MDLCACHE_FLUSH_VIRTUALMODEL = 0x10,
	MDLCACHE_FLUSH_AUTOPLAY = 0x20,
	MDLCACHE_FLUSH_VERTEXES = 0x40,

	MDLCACHE_FLUSH_IGNORELOCK = 0x80000000,
	MDLCACHE_FLUSH_ALL = 0xFFFFFFFF
};

typedef unsigned short MDLHandle_t;

class C_MDLCacheNotify
{
public:
	virtual void OnDataLoaded( MDLCacheDataType_t type, MDLHandle_t handle ) = 0;
	virtual void OnDataUnloaded( MDLCacheDataType_t type, MDLHandle_t handle ) = 0;
};

struct studiohdr_t;
struct studiohwdata_t;
struct vcollide_t;
struct virtualmodel_t;
struct vertexFileHeader_t;

class IMDLCache : public IAppSystem
{
public:
	virtual void                SetCacheNotify( C_MDLCacheNotify* pNotify ) = 0;
	virtual MDLHandle_t         FindMDL( const char* pMDLRelativePath ) = 0;
	virtual int                 AddRef( MDLHandle_t handle ) = 0;
	virtual int                 Release( MDLHandle_t handle ) = 0;
	virtual int                 GetRef( MDLHandle_t handle ) = 0;
	virtual studiohdr_t* GetStudioHdr( MDLHandle_t handle ) = 0;
	virtual studiohwdata_t* GetHardwareData( MDLHandle_t handle ) = 0;
	virtual vcollide_t* GetVCollide( MDLHandle_t handle ) = 0;
	virtual unsigned char* GetAnimBlock( MDLHandle_t handle, int nBlock ) = 0;
	virtual virtualmodel_t* GetVirtualModel( MDLHandle_t handle ) = 0;
	virtual int                 GetAutoplayList( MDLHandle_t handle, unsigned short** pOut ) = 0;
	virtual vertexFileHeader_t* GetVertexData( MDLHandle_t handle ) = 0;
	virtual void                TouchAllData( MDLHandle_t handle ) = 0;
	virtual void                SetUserData( MDLHandle_t handle, void* pData ) = 0;
	virtual void* GetUserData( MDLHandle_t handle ) = 0;
	virtual bool                IsErrorModel( MDLHandle_t handle ) = 0;
	virtual void                Flush( MDLCacheFlush_t nFlushFlags = MDLCACHE_FLUSH_ALL ) = 0;
	virtual void                Flush( MDLHandle_t handle, int nFlushFlags = MDLCACHE_FLUSH_ALL ) = 0;
	virtual const char* GetModelName( MDLHandle_t handle ) = 0;
	virtual virtualmodel_t* GetVirtualModelFast( const studiohdr_t* pStudioHdr, MDLHandle_t handle ) = 0;
	virtual void                BeginLock() = 0;
	virtual void                EndLock() = 0;
	virtual int* GetFrameUnlockCounterPtrOLD() = 0;
	virtual void                FinishPendingLoads() = 0;
	virtual vcollide_t* GetVCollideEx( MDLHandle_t handle, bool synchronousLoad = true ) = 0;
	virtual bool                GetVCollideSize( MDLHandle_t handle, int* pVCollideSize ) = 0;
	virtual bool                GetAsyncLoad( MDLCacheDataType_t type ) = 0;
	virtual bool                SetAsyncLoad( MDLCacheDataType_t type, bool bAsync ) = 0;
	virtual void                BeginMapLoad() = 0;
	virtual void                EndMapLoad() = 0;
	virtual void                MarkAsLoaded( MDLHandle_t handle ) = 0;
	virtual void                InitPreloadData( bool rebuild ) = 0;
	virtual void                ShutdownPreloadData() = 0;
	virtual bool                IsDataLoaded( MDLHandle_t handle, MDLCacheDataType_t type ) = 0;
	virtual int* GetFrameUnlockCounterPtr( MDLCacheDataType_t type ) = 0;
	virtual studiohdr_t* LockStudioHdr( MDLHandle_t handle ) = 0;
	virtual void                UnlockStudioHdr( MDLHandle_t handle ) = 0;
	virtual bool                PreloadModel( MDLHandle_t handle ) = 0;
	virtual void                ResetErrorModelStatus( MDLHandle_t handle ) = 0;
	virtual void                MarkFrame() = 0;
	virtual void                BeginCoarseLock() = 0;
	virtual void                EndCoarseLock() = 0;
	virtual void                ReloadVCollide( MDLHandle_t handle ) = 0;
};

enum Hitboxes
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_BODY,
	HITBOX_THORAX,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX
};

inline Hitboxes& operator++( Hitboxes& s, int )
{
	s = static_cast< Hitboxes >( static_cast< int >( s ) + 1 );

	return s;
}

struct mstudioactivitymodifier_t
{
	int					sznameindex;
	inline char* get_name() { return ( sznameindex ) ? ( char* ) ( ( ( uint8_t* ) this ) + sznameindex ) : NULL; }
};

struct mstudioseqdesc_t
{
	int					baseptr{};
	int					szlabelindex{};
	int					szactivitynameindex{};
	int					flags{};
	int					activity{};
	int					actweight{};
	int					numevents{};
	int					eventindex{};
	Vector				bbmin{};
	Vector				bbmax{};
	int					numblends{};
	int					animindexindex{};
	int					movementindex{};	// [blend] float array for blended movement
	int					groupsize[ 2 ]{};
	int					paramindex[ 2 ]{};	// X, Y, Z, XR, YR, ZR
	float				paramstart[ 2 ]{};	// local (0..1) starting value
	float				paramend[ 2 ]{};	// local (0..1) ending value
	int					paramparent{};
	float				fadeintime{};		// ideal cross fate in time (0.2 default)
	float				fadeouttime{};	// ideal cross fade out time (0.2 default)
	int					localentrynode{};		// transition node at entry
	int					localexitnode{};		// transition node at exit
	int					nodeflags{};		// transition rules
	float				entryphase{};		// used to match entry gait
	float				exitphase{};		// used to match exit gait
	float				lastframe{};		// frame that should generation EndOfSequence
	int					nextseq{};		// auto advancing sequences
	int					pose{};			// index of delta animation between end and nextseq
	int					numikrules{};
	int					numautolayers{};	//
	int					autolayerindex{};
	int					weightlistindex{};
	inline float* pBoneweight( int i ) const { return ( ( float* ) ( ( ( uint8_t* ) this ) + weightlistindex ) + i ); };
	inline float		weight( int i ) const { return *( pBoneweight( i ) ); };
	int					posekeyindex{};
	float* pPoseKey( int iParam, int iAnim ) const { return ( float* ) ( ( ( uint8_t* ) this ) + posekeyindex ) + iParam * groupsize[ 0 ] + iAnim; }
	float				poseKey( int iParam, int iAnim ) const { return *( pPoseKey( iParam, iAnim ) ); }
	int					numiklocks{};
	int					iklockindex{};
	int					keyvalueindex{};
	int					keyvaluesize{};
	inline const char* KeyValueText( void ) const { return keyvaluesize != 0 ? ( ( char* ) this ) + keyvalueindex : NULL; }
	int					cycleposeindex{};		// index of pose parameter to use as cycle index
	int					activitymodifierindex{};
	int					numactivitymodifiers{};
	inline mstudioactivitymodifier_t* get_activity_modifier( int i ) const { return activitymodifierindex != 0 ? ( mstudioactivitymodifier_t* ) ( ( ( uint8_t* ) this ) + activitymodifierindex ) + i : NULL; };
	int					animtagindex{};
	int					numanimtags{};
	int					rootDriverIndex{};
	int					unused[ 2 ]{};		// remove/add as appropriate (grow back to 8 ints on version change!)

	mstudioseqdesc_t() {}
private:
	// No copy constructors allowed
	mstudioseqdesc_t( const mstudioseqdesc_t& vOther );
};

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class IMaterial;
struct vcollide_t;
struct model_t;
class Vector;
class QAngle;
class CGameTrace;
struct cplane_t;
typedef CGameTrace trace_t;
struct studiohdr_t;
struct virtualmodel_t;
typedef unsigned char byte;
struct virtualterrainparams_t;
class CPhysCollide;
typedef unsigned short MDLHandle_t;
class CUtlBuffer;
class IClientRenderable;
class Quaternion;
struct mstudioanimdesc_t;
struct mstudiobodyparts_t;
struct mstudiotexture_t;

class RadianEuler
{
public:
	inline RadianEuler( void ) {}
	inline RadianEuler( float X, float Y, float Z ) { x = X; y = Y; z = Z; }
	inline RadianEuler( Quaternion const& q );	// evil auto type promotion!!!
	inline RadianEuler( QAngle const& angles );	// evil auto type promotion!!!

												// Initialization
	inline void Init( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f ) { x = ix; y = iy; z = iz; }

	//	conversion to qangle
	QAngle ToQAngle( void ) const;
	bool IsValid() const;
	void Invalidate();

	inline float* Base() { return &x; }
	inline const float* Base() const { return &x; }

	// array access...
	float operator[]( int i ) const;
	float& operator[]( int i );

	float x{}, y{}, z{};
};


struct mstudiohitboxset_t
{
	int sznameindex;

	inline char* const pszName( void ) const
	{
		return ( ( char* ) this ) + sznameindex;
	}

	int numhitboxes;
	int hitboxindex;

	inline mstudiobbox_t* pHitbox( int i ) const
	{
		return ( mstudiobbox_t* ) ( ( ( byte* ) this ) + hitboxindex ) + i;
	}
};

struct mstudiobone_t
{
	int					sznameindex;
	inline char* const pszName( void ) const { return ( ( char* ) this ) + sznameindex; }
	int		 			parent;		// parent bone
	int					bonecontroller[ 6 ];	// bone controller index, -1 == none

												// default values
	Vector				pos;
	Quaternion			quat;
	RadianEuler			rot;
	// compression scale
	Vector				posscale;
	Vector				rotscale;

	matrix3x4_t			poseToBone;
	Quaternion			qAlignment;
	int					flags;
	int					proctype;
	int					procindex;		// procedural rule
	mutable int			physicsbone;	// index into physically simulated bone
	inline void* pProcedure() const { if ( procindex == 0 ) return NULL; else return  ( void* ) ( ( ( byte* ) this ) + procindex ); };
	int					surfacepropidx;	// index into string tablefor property name
	inline char* const pszSurfaceProp( void ) const { return ( ( char* ) this ) + surfacepropidx; }
	inline int			GetSurfaceProp( void ) const { return surfacepropLookup; }

	int					contents;		// See BSPFlags.h for the contents flags
	int					surfacepropLookup;	// this index must be cached by the loader, not saved in the file
	int					unused[ 7 ];		// remove as appropriate
};

struct mstudiobonecontroller_t
{
	int					bone;	// -1 == 0
	int					type;	// X, Y, Z, XR, YR, ZR, M
	float				start;
	float				end;
	int					rest;	// byte index value at rest
	int					inputfield;	// 0-3 user set controller, 4 mouth
	int					unused[ 8 ];
};

// attachment
struct mstudioattachment_t
{
	int					sznameindex{};
	inline char* const pszName( void ) const { return ( ( char* ) this ) + sznameindex; }
	unsigned int		flags{};
	int					localbone{};
	matrix3x4_t			local; // attachment point
	int					unused[ 8 ]{};
};

struct studiohdr2_t;
struct studiohdr_t
{
	// Look up hitbox set by index
	mstudiohitboxset_t* pHitboxSet( int i ) const
	{
		Assert( i >= 0 && i < numhitboxsets );
		return ( mstudiohitboxset_t* ) ( ( ( byte* ) this ) + hitboxsetindex ) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t* pHitbox( int i, int set ) const
	{
		mstudiohitboxset_t const* s = pHitboxSet( set );
		if ( !s )
			return NULL;

		return s->pHitbox( i );
	};

	// Calls through to set to get hitbox count for set
	inline int			iHitboxCount( int set ) const
	{
		mstudiohitboxset_t const* s = pHitboxSet( set );
		if ( !s )
			return 0;

		return s->numhitboxes;
	};
	inline mstudiobone_t* pBone( int i ) const { Assert( i >= 0 && i < numbones ); return ( mstudiobone_t* ) ( ( ( byte* ) this ) + boneindex ) + i; };
	inline mstudioattachment_t* pLocalAttachment( int i ) const { Assert( i >= 0 && i < numlocalattachments ); return ( mstudioattachment_t* ) ( ( ( byte* ) this ) + localattachmentindex ) + i; };
	studiohdr2_t* pStudioHdr2() const { return ( studiohdr2_t* ) ( ( ( byte* ) this ) + studiohdr2index ); }

	mstudioseqdesc_t* get_local_seqdesc( int i ) const
	{
		const auto hdr = reinterpret_cast< unsigned char* >( const_cast< studiohdr_t* >( this ) );
		if ( i < 0 || i >= numlocalseq ) i = 0;
		return ( mstudioseqdesc_t* ) ( static_cast< unsigned char* >( hdr + localseqindex ) + i );
	}

	int id{};
	int version{};
	int checksum{};
	char name[ 64 ]{};
	int length{};

	Vector eyeposition{};
	Vector illumposition{};
	Vector hull_min{};
	Vector hull_max{};
	Vector view_bbmin{};
	Vector view_bbmax{};

	int flags{};
	int numbones{};
	int boneindex{};

	int numbonecontrollers{};
	int bonecontrollerindex{};
	int numhitboxsets{};
	int hitboxsetindex{};

	int numlocalanim{};
	int localanimindex{};
	int numlocalseq{};
	int localseqindex{};

	mutable int activitylistversion{};
	mutable int eventsindexed{};

	int numtextures{};
	int textureindex{};
	int numcdtextures{};
	int cdtextureindex{};

	int numskinref{};
	int numskinfamilies{};
	int skinindex{};

	int numbodyparts{};
	int bodypartindex{};
	int numlocalattachments{};
	int localattachmentindex{};

	int numlocalnodes{};
	int localnodeindex{};
	int localnodenameindex{};

	int numflexdesc{};
	int flexdescindex{};
	int numflexcontrollers{};
	int flexcontrollerindex{};
	int numflexrules{};
	int flexruleindex{};
	int numikchains{};
	int ikchainindex{};
	int nummouths{};
	int mouthindex{};
	int numlocalposeparameters{};
	int localposeparamindex{};
	int surfacepropindex{};

	int keyvalueindex{};
	int keyvaluesize{};

	int numlocalikautoplaylocks{};
	int localikautoplaylockindex{};

	float mass{};
	int contents{};
	int numincludemodels{};
	int includemodelindex{};

	mutable void* virtualModel{};
	int szanimblocknameindex{};

	int numanimblocks{};
	int animblockindex{};
	mutable void* animblockModel{};
	int bonetablebynameindex{};

	void* pVertexBase{};
	void* pIndexBase{};
	unsigned char constdirectionallightdot{};
	unsigned char rootLOD{};
	unsigned char numAllowedRootLODs{};
	unsigned char unused[ 1 ]{};
	int unused4{};
	int numflexcontrollerui{};
	int flexcontrolleruiindex{};
	int unused3[ 2 ]{};
	int studiohdr2index{};
	int unused2[ 1 ]{};

	// ReSharper disable once CppPossiblyUninitializedMember
	studiohdr_t() {}
private:
	studiohdr_t( const studiohdr_t& vOther );

	friend struct virtualmodel_t;
};

class virtualgroup_t
{
public:
	void* cache;
	const studiohdr_t* GetStudioHdr( void ) const
	{
		return ( studiohdr_t* ) cache;
	}

	CUtlVector< int > boneMap;				// maps global bone to local bone
	CUtlVector< int > masterBone;			// maps local bone to global bone
	CUtlVector< int > masterSeq;			// maps local sequence to master sequence
	CUtlVector< int > masterAnim;			// maps local animation to master animation
	CUtlVector< int > masterAttachment;	// maps local attachment to global
	CUtlVector< int > masterPose;			// maps local pose parameter to global
	CUtlVector< int > masterNode;			// maps local transition nodes to global
};

struct virtualsequence_t
{
	int	flags;
	int activity;
	int group;
	int index;
};

struct virtualgeneric_t
{
	int group;
	int index;
};

struct virtualmodel_t
{
	char pad_mutex[ 0x8 ];
	CUtlVector< virtualsequence_t > m_seq;
	CUtlVector< virtualgeneric_t > m_anim;
	CUtlVector< virtualgeneric_t > m_attachment;
	CUtlVector< virtualgeneric_t > m_pose;
	CUtlVector< virtualgroup_t > m_group;
	CUtlVector< virtualgeneric_t > m_node;
	CUtlVector< virtualgeneric_t > m_iklock;
	CUtlVector< unsigned short > m_autoplaySequences;
};


struct mstudiolinearbone_t
{
	int numbones{};

	int flagsindex{};
	inline int flags( int i ) const { Assert( i >= 0 && i < numbones ); return *( ( int* ) ( ( ( byte* ) this ) + flagsindex ) + i ); };
	inline int* pflags( int i ) { Assert( i >= 0 && i < numbones ); return ( ( int* ) ( ( ( byte* ) this ) + flagsindex ) + i ); };

	int	parentindex{};
	inline int parent( int i ) const { Assert( i >= 0 && i < numbones ); return *( ( int* ) ( ( ( byte* ) this ) + parentindex ) + i ); };

	int	posindex{};
	inline const Vector& pos( int i ) const { Assert( i >= 0 && i < numbones ); return *( ( Vector* ) ( ( ( byte* ) this ) + posindex ) + i ); };

	int quatindex{};
	inline const Quaternion& quat( int i ) const { Assert( i >= 0 && i < numbones ); return *( ( Quaternion* ) ( ( ( byte* ) this ) + quatindex ) + i ); };

	int rotindex{};
	inline const RadianEuler& rot( int i ) const { Assert( i >= 0 && i < numbones ); return *( ( RadianEuler* ) ( ( ( byte* ) this ) + rotindex ) + i ); };

	int posetoboneindex{};
	inline const matrix3x4_t& poseToBone( int i ) const { Assert( i >= 0 && i < numbones ); return *( ( matrix3x4_t* ) ( ( ( byte* ) this ) + posetoboneindex ) + i ); };

	int	posscaleindex{};
	inline const Vector& posscale( int i ) const { Assert( i >= 0 && i < numbones ); return *( ( Vector* ) ( ( ( byte* ) this ) + posscaleindex ) + i ); };

	int	rotscaleindex{};
	inline const Vector& rotscale( int i ) const { Assert( i >= 0 && i < numbones ); return *( ( Vector* ) ( ( ( byte* ) this ) + rotscaleindex ) + i ); };

	int	qalignmentindex{};
	inline const Quaternion& qalignment( int i ) const { Assert( i >= 0 && i < numbones ); return *( ( Quaternion* ) ( ( ( byte* ) this ) + qalignmentindex ) + i ); };

	int unused[ 6 ]{};

	mstudiolinearbone_t() {}
private:
	// No copy constructors allowed
	mstudiolinearbone_t( const mstudiolinearbone_t& vOther );
};

struct studiohdr2_t
{
	// NOTE: For forward compat, make sure any methods in this struct
	// are also available in studiohdr_t so no leaf code ever directly references
	// a studiohdr2_t structure
	int numsrcbonetransform;
	int srcbonetransformindex;

	int	illumpositionattachmentindex;
	inline int			IllumPositionAttachmentIndex() const { return illumpositionattachmentindex; }

	float flMaxEyeDeflection;
	inline float		MaxEyeDeflection() const { return flMaxEyeDeflection != 0.0f ? flMaxEyeDeflection : 0.866f; } // default to cos(30) if not set

	int linearboneindex;
	inline mstudiolinearbone_t* pLinearBones() const { return ( linearboneindex ) ? ( mstudiolinearbone_t* ) ( ( ( byte* ) this ) + linearboneindex ) : NULL; }

	int sznameindex;
	inline char* pszName() { return ( sznameindex ) ? ( char* ) ( ( ( byte* ) this ) + sznameindex ) : NULL; }

	int m_nBoneFlexDriverCount;
	int m_nBoneFlexDriverIndex;

	int reserved[ 56 ];
};

struct hash_value_type
{
	int activityIdx;
	int starting_index;
	int count;
	int totalWeight;
};

struct sequence_tuple
{
	short seqnum;
	short weight;
	uint16_t* activity_modifiers;
	int count_activity_modifiers;
	int* calculation_modes;
};
struct sequence_hash
{
	MFUNC( find_hash( hash_value_type* value ), uint32_t( __thiscall* )( void*, hash_value_type* ), make_offset( "client.dll", sig_sequence_hash_find_hash ) )( value )

		CUtlVector<CUtlVector<hash_value_type>> buckets;
	uintptr_t cmp_fn, key_fn;
};

class CStudioHdr;
struct activity_mapping
{
	MFUNC( reinitialize( CStudioHdr* hdr ), void( __thiscall* )( void*, CStudioHdr* ), make_offset( "client.dll", sig_activity_mapping_reinitialize ) )( hdr )

		sequence_tuple* sequence_tuples{};
	uint32_t sequence_tuples_count{};
	sequence_hash act_to_sequence_hash;
	const void* studio_hdr{};
	const void* v_model{};
};

class CStudioHdr
{
public:
	MFUNC( find_mapping(), activity_mapping* ( __thiscall* )( void* ), make_offset( "client.dll", sig_find_mapping ) )( )
		MFUNC( index_model_sequences(), void( __thiscall* )( void* ), make_offset( "client.dll", sig_index_model_sequences ) )( )
		OFFSET( get_activity_mapping, activity_mapping*, *reinterpret_cast< byte* >( make_offset( "client.dll", sig_activity_mapping ) ) )

		mutable studiohdr_t* m_pStudioHdr; //0x0000 
	mutable virtualmodel_t* m_pVModel; //0x0004 
	char				pad_unknown[ 0x4 ]; //0x0008 
	mutable CUtlVector< const studiohdr_t* > m_pStudioHdrCache; //0x000C 
	mutable int			m_nFrameUnlockCounter; //0x0020 
	int* m_pFrameUnlockCounter; //0x0024 
	char				pad_mutex[ 0x8 ]; //0x0028 
	CUtlVector< int >	m_boneFlags; //0x0030
	CUtlVector< int >	m_boneParent; //0x0050
	inline int			numbones( void ) const { return m_pStudioHdr->numbones; };
	inline const byte* GetBoneTableSortedByName() const { return ( byte* ) m_pStudioHdr + m_pStudioHdr->bonetablebynameindex; }
	inline mstudiobone_t* pBone( int i ) const { return m_pStudioHdr->pBone( i ); };
	inline mstudiolinearbone_t* pLinearBones() const { return m_pStudioHdr->studiohdr2index ? m_pStudioHdr->pStudioHdr2()->pLinearBones() : NULL; }
	inline virtualmodel_t* GetVirtualModel( void ) const { return m_pVModel; };
	__forceinline int	GetNumAttachments( void ) const
	{
		if ( m_pStudioHdr->numincludemodels == 0 )
		{
			return m_pStudioHdr->numlocalattachments;
		}

		virtualmodel_t* pVModel = GetVirtualModel();
		Assert( pVModel );

		return pVModel->m_attachment.Count();
	}
	__forceinline const studiohdr_t* GroupStudioHdr( int i )
	{
		const studiohdr_t* pStudioHdr = m_pStudioHdrCache[ i ];

		if ( !pStudioHdr )
		{
			virtualgroup_t* pGroup = &m_pVModel->m_group[ i ];
			pStudioHdr = pGroup->GetStudioHdr();
		}

		Assert( pStudioHdr );
		return pStudioHdr;
	}
	__forceinline const mstudioattachment_t& pAttachment( int i )
	{
		if ( m_pVModel == NULL )
		{
			return *m_pStudioHdr->pLocalAttachment( i );
		}

		Assert( m_pVModel );

		const studiohdr_t* pStudioHdr = GroupStudioHdr( m_pVModel->m_attachment[ i ].group );

		return *pStudioHdr->pLocalAttachment( m_pVModel->m_attachment[ i ].index );
	}
	__forceinline int	GetAttachmentBone( int i )
	{
		const mstudioattachment_t& attachment = pAttachment( i );

		// remap bone
		virtualmodel_t* pVModel = GetVirtualModel();
		if ( pVModel )
		{
			virtualgroup_t* pGroup = &pVModel->m_group[ pVModel->m_attachment[ i ].group ];
			int iBone = pGroup->masterBone[ attachment.localbone ];
			if ( iBone == -1 )
				return 0;
			return iBone;
		}
		return attachment.localbone;
	}

	__forceinline int32_t get_numseq() const
	{
		auto numseq = 0u;

		if ( !m_pVModel )
			numseq = m_pStudioHdr->numlocalseq;
		else
			numseq = m_pVModel->m_seq.Count();

		return numseq;
	}

	__forceinline bool holds_animation() const
	{
		return get_numseq() >= 3;
	}

	__forceinline mstudioseqdesc_t* get_seq_desc( const int index )
	{
		if ( m_pStudioHdr->numincludemodels == 0 || !m_pVModel )
			return m_pStudioHdr->get_local_seqdesc( index );

		return reinterpret_cast< mstudioseqdesc_t * ( __thiscall* )( void*, int32_t ) >( make_offset( "server.dll", sig_get_seq_desc ) )( this, index );
	}

	bool sequences_available() const
	{
		if ( m_pStudioHdr->numincludemodels == 0 )
			return true;

		return m_pVModel != nullptr;
	}

	__forceinline int get_activity_list_version()
	{
		if ( m_pVModel == nullptr )
			return m_pStudioHdr->activitylistversion;

		auto version = m_pStudioHdr->activitylistversion;

		for ( auto i = 1; i < m_pVModel->m_group.Count(); i++ )
		{
			const auto g_hdr = GroupStudioHdr( i );
			version = std::min( version, g_hdr->activitylistversion );
		}

		return version;
	}

}; //Size=0x0054


   //-----------------------------------------------------------------------------
   // Indicates the type of translucency of an unmodulated renderable
   //-----------------------------------------------------------------------------
enum RenderableTranslucencyType_t
{
	RENDERABLE_IS_OPAQUE = 0,
	RENDERABLE_IS_TRANSLUCENT,
	RENDERABLE_IS_TWO_PASS,	// has both translucent and opaque sub-partsa
};


//-----------------------------------------------------------------------------
// Model info interface
//-----------------------------------------------------------------------------

class IVModelInfo
{
public:
	virtual							~IVModelInfo( void ) {}
	virtual const model_t* GetModel( int modelindex ) const = 0;
	// Returns index of model by name
	virtual int						GetModelIndex( const char* name ) const = 0;
	// Returns name of model
	virtual const char* GetModelName( const model_t* model ) const = 0;
	virtual vcollide_t* GetVCollide( const model_t* model ) const = 0;
	virtual vcollide_t* GetVCollide( int modelindex ) const = 0;
	virtual void					_pad01() = 0;
	virtual void					_pad02() = 0;
	virtual void					GetModelBounds( const model_t* model, Vector& mins, Vector& maxs ) const = 0;
	virtual	void					GetModelRenderBounds( const model_t* model, Vector& mins, Vector& maxs ) const = 0;
	virtual int						GetModelFrameCount( const model_t* model ) const = 0;
	virtual int						GetModelType( const model_t* model ) const = 0;
	virtual void* GetModelExtraData( const model_t* model ) = 0;
	virtual bool					ModelHasMaterialProxy( const model_t* model ) const = 0;
	virtual bool					IsTranslucent( model_t const* model ) const = 0;
	virtual bool					IsTranslucentTwoPass( const model_t* model ) const = 0;
	virtual void					_pad03() {};
	virtual RenderableTranslucencyType_t ComputeTranslucencyType( const model_t* model, int nSkin, int nBody ) = 0;
	virtual int						GetModelMaterialCount( const model_t* model ) const = 0;
	virtual void					GetModelMaterials( const model_t* model, int count, IMaterial** ppMaterials ) = 0;
	virtual bool					IsModelVertexLit( const model_t* model ) const = 0;
	virtual const char* GetModelKeyValueText( const model_t* model ) = 0;
	virtual bool					GetModelKeyValue( const model_t* model, CUtlBuffer& buf ) = 0; // supports keyvalue blocks in submodels
	virtual float					GetModelRadius( const model_t* model ) = 0;

	virtual const studiohdr_t* FindModel( const studiohdr_t* pStudioHdr, void** cache, const char* modelname ) const = 0;
	virtual const studiohdr_t* FindModel( void* cache ) const = 0;
	virtual	virtualmodel_t* GetVirtualModel( const studiohdr_t* pStudioHdr ) const = 0;
	virtual byte* GetAnimBlock( const studiohdr_t* pStudioHdr, int iBlock ) const = 0;
	virtual bool					HasAnimBlockBeenPreloaded( studiohdr_t const*, int ) const = 0;

	// Available on Client only!!!
	virtual void					GetModelMaterialColorAndLighting( const model_t* model, Vector const& origin,
																	  QAngle const& angles, trace_t* pTrace,
																	  Vector& lighting, Vector& matColor ) = 0;
	virtual void					GetIlluminationPoint( const model_t* model, IClientRenderable* pRenderable, Vector const& origin,
														  QAngle const& angles, Vector* pLightingCenter ) = 0;

	virtual int						GetModelContents( int modelIndex ) const = 0;
	virtual studiohdr_t* GetStudioModel( const model_t* mod ) = 0;
	virtual int						GetModelSpriteWidth( const model_t* model ) const = 0;
	virtual int						GetModelSpriteHeight( const model_t* model ) const = 0;

	// Sets/gets a map-specified fade range (Client only)
	virtual void					SetLevelScreenFadeRange( float flMinSize, float flMaxSize ) = 0;
	virtual void					GetLevelScreenFadeRange( float* pMinArea, float* pMaxArea ) const = 0;

	// Sets/gets a map-specified per-view fade range (Client only)
	virtual void					SetViewScreenFadeRange( float flMinSize, float flMaxSize ) = 0;

	// Computes fade alpha based on distance fade + screen fade (Client only)
	virtual unsigned char			ComputeLevelScreenFade( const Vector& vecAbsOrigin, float flRadius, float flFadeScale ) const = 0;
	virtual unsigned char			ComputeViewScreenFade( const Vector& vecAbsOrigin, float flRadius, float flFadeScale ) const = 0;

	// both Client and server
	virtual int						GetAutoplayList( const studiohdr_t* pStudioHdr, unsigned short** pAutoplayList ) const = 0;

	// Gets a virtual terrain collision model (creates if necessary)
	// NOTE: This may return NULL if the terrain model cannot be virtualized
	virtual CPhysCollide* GetCollideForVirtualTerrain( int index ) = 0;
	virtual bool					IsUsingFBTexture( const model_t* model, int nSkin, int nBody, void /*IClientRenderable*/* pClientRenderable ) const = 0;
	virtual const model_t* FindOrLoadModel( const char* name ) const = 0;
	virtual MDLHandle_t				GetCacheHandle( const model_t* model ) const = 0;
	// Returns planes of non-nodraw brush model surfaces
	virtual int						GetBrushModelPlaneCount( const model_t* model ) const = 0;
	virtual void					GetBrushModelPlane( const model_t* model, int nIndex, cplane_t& plane, Vector* pOrigin ) const = 0;
	virtual int						GetSurfacepropsForVirtualTerrain( int index ) = 0;
	virtual bool					UsesEnvCubemap( const model_t* model ) const = 0;
	virtual bool					UsesStaticLighting( const model_t* model ) const = 0;
};


class IVModelInfoClient : public IVModelInfo
{
public:
};


struct virtualterrainparams_t
{
	// UNDONE: Add grouping here, specified in BSP file? (test grouping to see if this is necessary)
	int index;
};

struct viewmodel_arm_config
{
	const char* player_model_sub_string;
	const char* skintone_index;
	const char* glove_model;
	const char* sleeve_model;
	const char* sleeve_model_econ_override;
};