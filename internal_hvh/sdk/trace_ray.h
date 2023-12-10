#pragma once

class C_BaseEntity;
class IHandleEntity;
class VectorAligned : public Vector
{
public:
	VectorAligned() : Vector() {}

	VectorAligned( const Vector &vec ) : Vector()
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}

	float w{};
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

struct Ray_t
{
	Ray_t() {}

	VectorAligned		m_Start;
	VectorAligned		m_Delta;
	VectorAligned		m_StartOffset;
	VectorAligned		m_Extents;
	const matrix3x4_t	*m_pWorldAxisTransform{};
	bool				m_IsRay{};
	bool				m_IsSwept{};

	void Init( Vector vecStart, Vector vecEnd )
	{
		m_Delta = VectorAligned( vecEnd - vecStart );
		m_IsSwept = ( m_Delta.LengthSqr() != 0 );
		m_Extents.Zero();
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;
		m_StartOffset.Zero();
		m_Start = vecStart;
	}
	void Init( Vector vecStart, Vector vecEnd, const Vector & extents )
	{
		m_Delta = VectorAligned( vecEnd - vecStart );
		m_IsSwept = ( m_Delta.LengthSqr() != 0 );
		m_Extents = extents;
		m_pWorldAxisTransform = NULL;
		m_IsRay = false;
		m_StartOffset.Zero();
		m_Start = vecStart;
	}
	void Init( Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs )
	{
		m_Delta = end - start;

		m_pWorldAxisTransform = NULL;
		m_IsSwept = ( m_Delta.LengthSqr() != 0 );

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = ( m_Extents.LengthSqr() < 1e-6 );

		// Offset m_Start to be in the center of the box...
		m_StartOffset = maxs + mins;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}

	Vector InvDelta() const
	{
		Vector vecInvDelta;
		for ( int iAxis = 0; iAxis < 3; ++iAxis )
		{
			if ( m_Delta[ iAxis ] != 0.0f )
			{
				vecInvDelta[ iAxis ] = 1.0f / m_Delta[ iAxis ];
			}
			else
			{
				vecInvDelta[ iAxis ] = FLT_MAX;
			}
		}
		return vecInvDelta;
	}
};

struct cplane_t
{
	Vector	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	pad[ 2 ];
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity( C_BaseEntity* pEntityHandle, int contentsMask )
	{
		return !( pEntityHandle == pSkip );
	}
	virtual TraceType_t	GetTraceType()
	{
		return TRACE_EVERYTHING;
	}
	void* pSkip = nullptr;
};

class CTraceFilter : public ITraceFilter
{
public:
	CTraceFilter() = default;
	explicit CTraceFilter( C_BaseEntity* entity, TraceType_t tracetype = TRACE_EVERYTHING )
	{
		pSkip = entity;
		type = tracetype;
	}
	explicit CTraceFilter( C_BaseEntity* entity, int contentsMask )
	{
		pSkip = entity;
	}
	virtual bool ShouldHitEntity( C_BaseEntity* pEntityHandle, int contentsMask ) override
	{
		return !( pEntityHandle == pSkip );
	}

	virtual TraceType_t	GetTraceType() override
	{
		return type;
	}
	void* pSkip = nullptr;
	TraceType_t type = TRACE_EVERYTHING;
};

class CTraceFilterSkipTwoEntities : public ITraceFilter
{
public:
	CTraceFilterSkipTwoEntities() {}
	CTraceFilterSkipTwoEntities( void *pPassEnt1, void *pPassEnt2 )
	{
		pPassEntity1 = pPassEnt1;
		pPassEntity2 = pPassEnt2;
	}

	virtual bool ShouldHitEntity( C_BaseEntity *pEntityHandle, int contentsMask ) override
	{
		return !( pEntityHandle == pPassEntity1 || pEntityHandle == pPassEntity2 );
	}

	virtual TraceType_t GetTraceType() override
	{
		return TRACE_EVERYTHING;
	}

	void *pPassEntity1{};
	void *pPassEntity2{};
};

class CBaseTrace
{
public:
	Vector			startpos{};
	Vector			endpos{};
	cplane_t		plane{};

	float			fraction{};

	int				contents{};
	unsigned short	dispFlags{};

	bool			allsolid{};
	bool			startsolid{};

	CBaseTrace() {}
};

struct csurface_t
{
	const char*		name;
	short			surfaceProps;
	unsigned short	flags;
};

class CTraceFilterWorldOnly : public ITraceFilter
{
public:
	virtual bool ShouldHitEntity( C_BaseEntity *pServerEntity, int contentsMask ) override
	{
		return false;
	}
	virtual TraceType_t	GetTraceType() override
	{
		return TRACE_WORLD_ONLY;
	}
};

enum SolidType_t
{
	SOLID_NONE = 0,	// no solid model
	SOLID_BSP = 1,	// a BSP tree
	SOLID_BBOX = 2,	// an AABB
	SOLID_OBB = 3,	// an OBB (not implemented yet)
	SOLID_OBB_YAW = 4,	// an OBB, constrained so that it can only yaw
	SOLID_CUSTOM = 5,	// Always call into the entity for tests
	SOLID_VPHYSICS = 6,	// solid vphysics object, get vcollide from the model and collide with that
	SOLID_LAST,
};


typedef bool( *ShouldHitFunc_t )( IHandleEntity *pHandleEntity, int contentsMask );

class CTraceFilterSimple : public CTraceFilter
{
public:
	CTraceFilterSimple( const C_BaseEntity *passentity, const C_BaseEntity* passentity2 = nullptr, int collisionGroup = 0, ShouldHitFunc_t pExtraShouldHitCheckFn = NULL )
	{
		m_pPassEnt = passentity;
		m_pPassEnt2 = passentity2;
		m_collisionGroup = collisionGroup;
		m_pExtraShouldHitCheckFunction = pExtraShouldHitCheckFn;
	}
	virtual bool ShouldHitEntity( C_BaseEntity *pHandleEntity, int contentsMask ) override;

	const C_BaseEntity *GetPassEntity( void ) { return m_pPassEnt; }

public:
	const C_BaseEntity *m_pPassEnt;
	const C_BaseEntity *m_pPassEnt2;
	int m_collisionGroup;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;
};

class CTraceFilterSimpleNoPlayers : public CTraceFilter
{
public:
	CTraceFilterSimpleNoPlayers( const C_BaseEntity *passentity, int collisionGroup, ShouldHitFunc_t pExtraShouldHitCheckFn = NULL )
	{
		m_pPassEnt = passentity;
		m_collisionGroup = collisionGroup;
		m_pExtraShouldHitCheckFunction = pExtraShouldHitCheckFn;
	}
	virtual bool ShouldHitEntity( C_BaseEntity *pHandleEntity, int contentsMask ) override;

	const C_BaseEntity *GetPassEntity( void ) { return m_pPassEnt; }

private:
	const C_BaseEntity *m_pPassEnt;
	int m_collisionGroup;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;

};

class CTraceFilterSimpleNoTeammates : public CTraceFilter
{
public:
	CTraceFilterSimpleNoTeammates( const C_BaseEntity *passentity, int collisionGroup, ShouldHitFunc_t pExtraShouldHitCheckFn = NULL )
	{
		m_pPassEnt = passentity;
		m_collisionGroup = collisionGroup;
		m_pExtraShouldHitCheckFunction = pExtraShouldHitCheckFn;
	}
	virtual bool ShouldHitEntity( C_BaseEntity *pHandleEntity, int contentsMask ) override;

	const C_BaseEntity *GetPassEntity( void ) { return m_pPassEnt; }

private:
	const C_BaseEntity *m_pPassEnt;
	int m_collisionGroup;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;

};

class IClientUnknown;
class IClientEntity;
class CTraceFilterNoPlayers : public CTraceFilter
{
public:
	CTraceFilterNoPlayers() {}
	virtual bool ShouldHitEntity( C_BaseEntity* pEntityHandle, int contentsMask ) override
	{
		return false;
	}
};

class CGameTrace : public CBaseTrace
{
public:
	bool                    DidHitWorld() const;
	bool                    DidHitNonWorldEntity() const
	{
		return m_pEnt != NULL && !DidHitWorld();
	}
	int                     GetEntityIndex() const;
	bool                    DidHit() const { return fraction < 1 || allsolid || startsolid; };
	bool					BoneIsVisible() const;

public:

	float                   fractionleftsolid{};
	csurface_t              surface{};
	int                     hitgroup{};
	short                   physicsbone{};
	unsigned short          worldSurfaceIndex{};
	C_BaseEntity*           m_pEnt{};
	int                     hitbox{};

	CGameTrace() {}

public:
	CGameTrace( const CGameTrace& vOther )
	{
		fractionleftsolid = vOther.fractionleftsolid;
		surface = vOther.surface;
		hitgroup = vOther.hitgroup;
		physicsbone = vOther.physicsbone;
		worldSurfaceIndex = vOther.worldSurfaceIndex;
		m_pEnt = vOther.m_pEnt;
		hitbox = vOther.hitbox;
		startpos = vOther.startpos;
		endpos = vOther.endpos;
		plane = vOther.plane;
		fraction = vOther.fraction;
		contents = vOther.contents;
		dispFlags = vOther.dispFlags;
		allsolid = vOther.allsolid;
		startsolid = vOther.startsolid;
	}
};

typedef CGameTrace trace_t;
