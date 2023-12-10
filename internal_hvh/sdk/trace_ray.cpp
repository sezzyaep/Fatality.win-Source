#include "../include_cheat.h"

bool StandardFilterRules( IHandleEntity* pHandleEntity, int fContentsMask )
{
	IClientUnknown* pUnk = ( IClientUnknown* ) pHandleEntity;
	C_BaseEntity* pCollide = pUnk->GetBaseEntity();

	// Static prop case...
	if ( !pCollide )
		return true;

	SolidType_t solid = ( SolidType_t ) pCollide->GetCollideable()->GetSolid();
	const model_t* pModel = pCollide->GetModel();

	if ( ( interfaces::model_info()->GetModelType( pModel ) != mod_brush ) || ( solid != SOLID_BSP && solid != SOLID_VPHYSICS ) )
	{
		if ( ( fContentsMask & CONTENTS_MONSTER ) == 0 )
			return false;
	}

	// This code is used to cull out tests against see-thru entities
	if ( !( fContentsMask & CONTENTS_WINDOW )/* && pCollide->GetCollideable()->IsTransparent()*/ )
		return false;

	// FIXME: this is to skip BSP models that are entities that can be 
	// potentially moved/deleted, similar to a monster but doors don't seem to 
	// be flagged as monsters
	// FIXME: the FL_WORLDBRUSH looked promising, but it needs to be set on 
	// everything that's actually a worldbrush and it currently isn't
	if ( !( fContentsMask & CONTENTS_MOVEABLE ) && ( pCollide->get_move_type() == MOVETYPE_PUSH ) )// !(touch->flags & FL_WORLDBRUSH) )
		return false;

	return true;
}

bool PassServerEntityFilter( const IHandleEntity* pTouch, const IHandleEntity* pPass )
{
	if ( !pPass )
		return true;

	if ( pTouch == pPass )
		return false;

	IClientUnknown* pUnk1 = ( IClientUnknown* ) pTouch;
	IClientUnknown* pUnk2 = ( IClientUnknown* ) pPass;
	C_BaseEntity* pEntTouch = pUnk1->GetBaseEntity();
	C_BaseEntity* pEntPass = pUnk2->GetBaseEntity();
	if ( !pEntTouch || !pEntPass )
		return true;

	// don't clip against own missiles
	if ( pEntTouch->get_owner_ent() == ( uint32_t ) pEntPass )
		return false;

	// don't clip against owner
	if ( pEntPass->get_owner_ent() == ( uint32_t ) pEntTouch )
		return false;

	return true;
}

bool CTraceFilterSimple::ShouldHitEntity( C_BaseEntity* pHandleEntity, int contentsMask )
{
	if ( !StandardFilterRules( pHandleEntity, contentsMask ) )
		return false;

	if ( m_pPassEnt )
	{
		if ( !PassServerEntityFilter( pHandleEntity, m_pPassEnt ) )
		{
			return false;
		}
	}

	if ( m_pPassEnt2 )
	{
		if ( !PassServerEntityFilter( pHandleEntity, m_pPassEnt2 ) )
		{
			return false;
		}
	}

	// Don't test if the game code tells us we should ignore this collision...
	IClientUnknown* pUnk = ( IClientUnknown* ) pHandleEntity;
	auto pEntity = pUnk->GetBaseEntity();
	if ( !pEntity )
		return false;
	if ( !pEntity->should_collide( m_collisionGroup, contentsMask ) )
		return false;
	if ( pEntity && !( *interfaces::game_rules_proxy() )->ShouldCollide( m_collisionGroup, pEntity->GetCollideable()->GetCollisionGroup() ) )
		return false;
	/*if ( m_pExtraShouldHitCheckFunction &&
		( !( m_pExtraShouldHitCheckFunction( pHandleEntity, contentsMask ) ) ) )
		return false;*/

	return true;
}

bool CTraceFilterSimpleNoPlayers::ShouldHitEntity( C_BaseEntity* pHandleEntity, int contentsMask )
{
	if ( !StandardFilterRules( pHandleEntity, contentsMask ) )
		return false;

	if ( m_pPassEnt )
	{
		if ( !PassServerEntityFilter( pHandleEntity, m_pPassEnt ) )
		{
			return false;
		}
	}

	// Don't test if the game code tells us we should ignore this collision...
	IClientUnknown* pUnk = ( IClientUnknown* ) pHandleEntity;
	auto pEntity = pUnk->GetBaseEntity();
	if ( !pEntity )
		return false;
	if ( !pEntity->should_collide( m_collisionGroup, contentsMask ) )
		return false;
	if ( pEntity && !( *interfaces::game_rules_proxy() )->ShouldCollide( m_collisionGroup, pEntity->GetCollideable()->GetCollisionGroup() ) )
		return false;
	/*if ( m_pExtraShouldHitCheckFunction &&
		( !( m_pExtraShouldHitCheckFunction( pHandleEntity, contentsMask ) ) ) )
		return false;*/

	if ( pEntity->GetClientClass()->m_ClassID == ClassId::CCSPlayer )
		return false;

	return true;
}

bool CTraceFilterSimpleNoTeammates::ShouldHitEntity( C_BaseEntity* pHandleEntity, int contentsMask )
{
	if ( !StandardFilterRules( pHandleEntity, contentsMask ) )
		return false;

	if ( m_pPassEnt )
	{
		if ( !PassServerEntityFilter( pHandleEntity, m_pPassEnt ) )
		{
			return false;
		}
	}

	// Don't test if the game code tells us we should ignore this collision...
	IClientUnknown* pUnk = ( IClientUnknown* ) pHandleEntity;
	auto pEntity = pUnk->GetBaseEntity();
	if ( !pEntity )
		return false;
	if ( !pEntity->should_collide( m_collisionGroup, contentsMask ) )
		return false;
	if ( pEntity && !( *interfaces::game_rules_proxy() )->ShouldCollide( m_collisionGroup, pEntity->GetCollideable()->GetCollisionGroup() ) )
		return false;
	/*if ( m_pExtraShouldHitCheckFunction &&
		( !( m_pExtraShouldHitCheckFunction( pHandleEntity, contentsMask ) ) ) )
		return false;*/

	var( mp_solid_teammates );
	if ( pEntity->GetClientClass()->m_ClassID == ClassId::CCSPlayer )
	{
		if ( mp_solid_teammates->get_int() == 2 && !( ( C_CSPlayer* ) pEntity )->is_enemy() )
			return ( ( C_CSPlayer* ) pEntity )->get_origin().z - local_player->get_origin().z >= 54;

		return mp_solid_teammates->get_int() == 1 || ( ( C_CSPlayer* ) pEntity )->is_enemy();
	}


	return false;
}


bool CGameTrace::DidHitWorld() const
{
	return m_pEnt == ( C_BaseEntity* ) interfaces::entity_list()->get_client_entity( 0 );
}




template <class T>
class CRangeValidatedArray
{
public:
	void Attach( int nCount, T* pData );
	void Detach();

	T& operator[]( int i );
	const T& operator[]( int i ) const;

	T* Base();

private:
	T* m_pArray;
};

template <class T>
inline T& CRangeValidatedArray<T>::operator[]( int i )
{
	Assert( ( i >= 0 ) && ( i < m_nCount ) );
	return m_pArray[ i ];
}

template <class T>
inline const T& CRangeValidatedArray<T>::operator[]( int i ) const
{
	Assert( ( i >= 0 ) && ( i < m_nCount ) );
	return m_pArray[ i ];
}

template <class T>
inline void CRangeValidatedArray<T>::Attach( int nCount, T* pData )
{
	m_pArray = pData;
}

template <class T>
inline void CRangeValidatedArray<T>::Detach()
{
	m_pArray = NULL;

}

template <class T>
inline T* CRangeValidatedArray<T>::Base()
{
	return m_pArray;
}


struct cnode_t
{
	cplane_t* plane;
	int			children[ 2 ];		// negative numbers are leafs
};

struct cbrushside_t
{
	cplane_t* plane;
	unsigned short	surfaceIndex;
	byte			bBevel;			// is the side a bevel plane?
	byte			bThin;			// is a thin brush side?
};

struct cboxbrush_t
{
	VectorAligned	mins;
	VectorAligned	maxs;

	unsigned short	surfaceIndex[ 6 ];
	unsigned short	thinMask;
	unsigned short	pad;
};

struct cleaf_t
{
	int			    contents;
	short			cluster;
	short			area : 9;
	short			flags : 7;
	unsigned short	firstleafbrush;
	unsigned short	numleafbrushes;
	unsigned short	dispListStart;
	unsigned short	dispCount;
};

struct vcollide_t
{
	unsigned short solidCount : 15;
	unsigned short isPacked : 1;
	unsigned short descSize;
	// VPhysicsSolids
	CPhysCollide** solids;
	char* pKeyValues;
	void* pUserData;
};

struct cmodel_t
{
	Vector		mins, maxs;
	Vector		origin;		// for sounds or lights
	int			headnode;

	vcollide_t	vcollisionData;
};

#define NUMSIDES_BOXBRUSH	0xFFFF

struct cbrush_t
{
	int				contents;
	unsigned short	numsides;
	unsigned short	firstbrushside;

	inline int GetBox() const { return firstbrushside; }
	inline void SetBox( int boxID )
	{
		numsides = NUMSIDES_BOXBRUSH;
		firstbrushside = boxID;
	}
	inline bool IsBox() const { return numsides == NUMSIDES_BOXBRUSH ? true : false; }
};

struct dvis_t
{
	int			numclusters;
	int			bitofs[ 8 ][ 2 ];	// bitofs[numclusters][2]
};

class CByteswap
{
	unsigned int m_bSwapBytes : 1;
	unsigned int m_bBigEndian : 1;
};

class CUtlBuffer
{
public:
	CUtlMemory<unsigned char> m_Memory;
	int m_Get;
	int m_Put;

	unsigned char m_Error;
	unsigned char m_Flags;
	unsigned char m_Reserved;

	int m_nTab;
	int m_nMaxPut;
	int m_nOffset;

	void* m_GetOverflowFunc;
	void* m_PutOverflowFunc;

	CByteswap	m_Byteswap;
};

template <class T>
class CDiscardableArray
{
public:
	int			m_nCount;
	CUtlBuffer	m_buf;
	char		m_pFilename[ 256 ];
	int			m_nOffset;
};

struct carea_t
{
	int		numareaportals;
	int		firstareaportal;
	int		floodnum;							// if two areas have equal floodnums, they are connected
	int		floodvalid;
};


struct dareaportal_t
{
	static datamap_t m_DataMap;
	unsigned short	m_PortalKey;		// Entities have a key called portalnumber (and in vbsp a variable
									// called areaportalnum) which is used
									// to bind them to the area portals by comparing with this value.

	unsigned short	otherarea;		// The area this portal looks into.

	unsigned short	m_FirstClipPortalVert;	// Portal geometry.
	unsigned short	m_nClipPortalVerts;

	int				planenum;
};
class CCollisionBSPData
{
public:
	// This is sort of a hack, but it was a little too painful to do this any other way
	// The goal of this dude is to allow us to override the tree with some
	// other tree (or a subtree)
	cnode_t* map_rootnode;

	char						mapPathName[ 96 ];
	static csurface_t			nullsurface;

	int									numbrushsides;
	CRangeValidatedArray<cbrushside_t>	map_brushsides;
	int									numboxbrushes;
	CRangeValidatedArray<cboxbrush_t>	map_boxbrushes;
	int									numplanes;
	CRangeValidatedArray<cplane_t>		map_planes;
	int									numnodes;
	CRangeValidatedArray<cnode_t>		map_nodes;
	int									numleafs;				// allow leaf funcs to be called without a map
	CRangeValidatedArray<cleaf_t>		map_leafs;
	int									emptyleaf, solidleaf;
	int									numleafbrushes;
	CRangeValidatedArray<unsigned short> map_leafbrushes;
	int									numcmodels;
	CRangeValidatedArray<cmodel_t>		map_cmodels;
	int									numbrushes;
	CRangeValidatedArray<cbrush_t>		map_brushes;
	int									numdisplist;
	CRangeValidatedArray<unsigned short> map_dispList;

	// this points to the whole block of memory for vis data, but it is used to
	// reference the header at the top of the block.
	int									numvisibility;
	dvis_t* map_vis;

	int									numentitychars;
	CDiscardableArray<char>				map_entitystring;

	int									numareas;
	CRangeValidatedArray<carea_t>		map_areas;
	int									numareaportals;
	CRangeValidatedArray<dareaportal_t>	map_areaportals;
	int									numclusters;
	char* map_nullname;
	int									numtextures;
	char* map_texturenames;
	CRangeValidatedArray<csurface_t>	map_surfaces;
	int									floodvalid;
	int									numportalopen;
	CRangeValidatedArray<bool>			portalopen;
	int									allcontents;

	csurface_t* GetSurfaceAtIndex( unsigned short surfaceIndex );
};

int cm_point_leafnum_r( cnode_t* map_rootnode, const Vector& p, int num )
{
	float d;

	while ( num >= 0 )
	{
		const auto node = map_rootnode + num;
		const auto plane = node->plane;

		if ( plane->type < 3 )
			d = p[ plane->type ] - plane->dist;
		else
			d = plane->normal.Dot( p ) - plane->dist;
		if ( d < 0 )
			num = node->children[ 1 ];
		else
			num = node->children[ 0 ];
	}

	return -1 - num;
}

bool IsPointInBox( const Vector& pt, const Vector& boxMin, const Vector& boxMax )
{
	if ( ( pt[ 0 ] > boxMax[ 0 ] ) || ( pt[ 0 ] < boxMin[ 0 ] ) )
		return false;
	if ( ( pt[ 1 ] > boxMax[ 1 ] ) || ( pt[ 1 ] < boxMin[ 1 ] ) )
		return false;
	if ( ( pt[ 2 ] > boxMax[ 2 ] ) || ( pt[ 2 ] < boxMin[ 2 ] ) )
		return false;

	return true;
}

int	IEngineTrace::GetPointContents_WorldOnly( cbrush_t*& prev_brush, const Vector& p, int contentsMask )
{
	return GetPointContents_WorldOnly( p, contentsMask );

	static auto reference = 0;//*reinterpret_cast< uint32_t* >( sig( "engine.dll", "83 3D ? ? ? ? ? 56 57 8B F1" ) + 0x2 );

	const auto numnodes = *reinterpret_cast< int* >( reference );
	if ( !numnodes )
		return 0;

	const auto allcontents = *reinterpret_cast< int* >( reference + 0x1d8 );
	if ( !( allcontents & contentsMask ) )
		return 0;

	const auto map_rootnode = *reinterpret_cast< cnode_t** >( reference - 0x88 );
	auto l = cm_point_leafnum_r( map_rootnode, p, 0 );

	// iterate the leaf brushes and check for intersection with each one
	const auto map_leafs = *reinterpret_cast< cleaf_t** >( reference + 0x10 );
	cleaf_t& leaf = map_leafs[ l ];
	if ( leaf.cluster < 0 )
		return leaf.contents;

	int nContents = 0;

	const auto map_boxbrushes = *reinterpret_cast< cboxbrush_t** >( reference + 0x10 );
	const auto map_brushsides = *reinterpret_cast< cbrushside_t** >( reference - 0x20 );
	const auto map_leafbrushes = *reinterpret_cast< unsigned short** >( reference + 0x24 );
	const auto map_brushes = *reinterpret_cast< cbrush_t** >( reference + 0x3c );

	//check if still in last brush
	if ( prev_brush )
	{
		const auto pBrush = prev_brush;
		if ( pBrush->IsBox() )
		{
			// special case for box brush
			const auto pBox = &map_boxbrushes[ pBrush->GetBox() ];
			if ( IsPointInBox( p, pBox->mins, pBox->maxs ) )
			{
				nContents |= pBrush->contents;
			}
		}
		else
		{
			// must be on the back of each brush side to be inside, skip bevels because they aren't necessary for testing points
			cbrushside_t* RESTRICT pSide = &map_brushsides[ pBrush->firstbrushside ];
			bool bInside = true;
			for ( const cbrushside_t* const pSideLimit = pSide + pBrush->numsides; pSide < pSideLimit; pSide++ )
			{
				if ( pSide->bBevel )
					continue;
				float flDist = pSide->plane->normal.Dot( p ) - pSide->plane->dist;
				// outside plane, no intersection
				if ( flDist > 0.0f )
				{
					bInside = false;
					break;
				}
			}

			if ( bInside )
			{
				nContents |= pBrush->contents;
			}
		}

		if ( nContents & contentsMask )
			return nContents;

		prev_brush = nullptr;
	}

	const unsigned short* pBrushList = &map_leafbrushes[ leaf.firstleafbrush ];

	for ( int i = 0; i < leaf.numleafbrushes; i++ )
	{
		cbrush_t* pBrush = &map_brushes[ pBrushList[ i ] ];
		// only consider brushes that have contents
		if ( !pBrush->contents )
			continue;

		if ( pBrush->IsBox() )
		{
			// special case for box brush
			cboxbrush_t* pBox = &map_boxbrushes[ pBrush->GetBox() ];
			if ( IsPointInBox( p, pBox->mins, pBox->maxs ) )
			{
				nContents |= pBrush->contents;
			}
		}
		else
		{
			// must be on the back of each brush side to be inside, skip bevels because they aren't necessary for testing points
			cbrushside_t* RESTRICT pSide = &map_brushsides[ pBrush->firstbrushside ];
			bool bInside = true;
			for ( const cbrushside_t* const pSideLimit = pSide + pBrush->numsides; pSide < pSideLimit; pSide++ )
			{
				if ( pSide->bBevel )
					continue;
				float flDist = pSide->plane->normal.Dot( p ) - pSide->plane->dist;
				// outside plane, no intersection
				if ( flDist > 0.0f )
				{
					bInside = false;
					break;
				}
			}

			if ( bInside )
			{
				nContents |= pBrush->contents;
			}
		}

		if ( nContents & contentsMask )
		{
			prev_brush = pBrush;
			return nContents;
		}

	}
	// point wasn't inside any brushes so return empty
	return nContents;
}