#pragma once

#define FTYPEDESC_GLOBAL			0x0001		// This field is masked for global entity save/restore
#define FTYPEDESC_SAVE				0x0002		// This field is saved to disk
#define FTYPEDESC_KEY				0x0004		// This field can be requested and written to by string name at load time
#define FTYPEDESC_INPUT				0x0008		// This field can be written to by string name at run time, and a function called
#define FTYPEDESC_OUTPUT			0x0010		// This field propogates it's value to all targets whenever it changes
#define FTYPEDESC_FUNCTIONTABLE		0x0020		// This is a table entry for a member function pointer
#define FTYPEDESC_PTR				0x0040		// This field is a pointer, not an embedded object
#define FTYPEDESC_OVERRIDE			0x0080		// The field is an override for one in a base class (only used by prediction system for now)

// Flags used by other systems (e.g., prediction system)
#define FTYPEDESC_INSENDTABLE		0x0100		// This field is present in a network SendTable
#define FTYPEDESC_PRIVATE			0x0200		// The field is local to the client or server only (not referenced by prediction code and not replicated by networking)
#define FTYPEDESC_NOERRORCHECK		0x0400		// The field is part of the prediction typedescription, but doesn't get compared when checking for errors

#define FTYPEDESC_MODELINDEX		0x0800		// The field is a model index (used for debugging output)

#define FTYPEDESC_INDEX				0x1000		// The field is an index into file data, used for byteswapping. 

// These flags apply to C_BasePlayer derived objects only
#define FTYPEDESC_VIEW_OTHER_PLAYER		0x2000		// By default you can only view fields on the local player (yourself), 
													//   but if this is set, then we allow you to see fields on other players
#define FTYPEDESC_VIEW_OWN_TEAM			0x4000		// Only show this data if the player is on the same team as the local player
#define FTYPEDESC_VIEW_NEVER			0x8000		// Never show this field to anyone, even the local player (unusual)

struct inputdata_t;
typedef enum _fieldtypes : int
{
	FIELD_VOID = 0,			// No type or value
	FIELD_FLOAT,			// Any floating point value
	FIELD_STRING,			// A string ID (return from ALLOC_STRING)
	FIELD_VECTOR,			// Any vector, QAngle, or AngularImpulse
	FIELD_QUATERNION,		// A quaternion
	FIELD_INTEGER,			// Any integer or enum
	FIELD_BOOLEAN,			// boolean, implemented as an int, I may use this as a hint for compression
	FIELD_SHORT,			// 2 byte integer
	FIELD_CHARACTER,		// a byte
	FIELD_COLOR32,			// 8-bit per channel r,g,b,a (32bit color)
	FIELD_EMBEDDED,			// an embedded object with a datadesc, recursively traverse and embedded class/structure based on an additional typedescription
	FIELD_CUSTOM,			// special type that contains function pointers to it's read/write/parse functions

	FIELD_CLASSPTR,			// CBaseEntity *
	FIELD_EHANDLE,			// Entity handle
	FIELD_EDICT,			// edict_t *

	FIELD_POSITION_VECTOR,	// A world coordinate (these are fixed up across level transitions automagically)
	FIELD_TIME,				// a floating point time (these are fixed up automatically too!)
	FIELD_TICK,				// an integer tick count( fixed up similarly to time)
	FIELD_MODELNAME,		// Engine string that is a model name (needs precache)
	FIELD_SOUNDNAME,		// Engine string that is a sound name (needs precache)

	FIELD_INPUT,			// a list of inputed data fields (all derived from CMultiInputVar)
	FIELD_FUNCTION,			// A class function pointer (Think, Use, etc)

	FIELD_VMATRIX,			// a vmatrix (output coords are NOT worldspace)

							// NOTE: Use float arrays for local transformations that don't need to be fixed up.
							FIELD_VMATRIX_WORLDSPACE,// A VMatrix that maps some local space to world space (translation is fixed up on level transitions)
							FIELD_MATRIX3X4_WORLDSPACE,	// matrix3x4_t that maps some local space to world space (translation is fixed up on level transitions)

							FIELD_INTERVAL,			// a start and range floating point interval ( e.g., 3.2->3.6 == 3.2 and 0.4 )
							FIELD_MODELINDEX,		// a model index
							FIELD_MATERIALINDEX,	// a material index (using the material precache string table)

							FIELD_VECTOR2D,			// 2 floats

							FIELD_TYPECOUNT,		// MUST BE LAST
} fieldtype_t;

class ISaveRestoreOps;
class C_BaseEntity;
//
// Function prototype for all input handlers.
//
typedef void ( C_BaseEntity::* inputfunc_t )( inputdata_t& data );

struct datamap_t;

enum
{
	TD_OFFSET_NORMAL = 0,
	TD_OFFSET_PACKED = 1,

	// Must be last
	TD_OFFSET_COUNT,
};

struct typedescription_t
{
	fieldtype_t fieldType;
	const char* fieldName;
	int fieldOffset;
	unsigned short fieldSize;
	short flags;
	char* externalName;
	ISaveRestoreOps* pSaveRestoreOps;
	uintptr_t* inputFunc;
	datamap_t* td;
	int fieldSizeInBytes;
	struct typedescription_t* override_field;
	int override_count;
	float fieldTolerance;
	int flatOffset[ TD_OFFSET_COUNT ];
	unsigned short flatGroup;
};

// A data run is a set of DEFINE_PRED_FIELD fields in a c++ object which are contiguous and can be processing
//  using a single memcpy operation
struct datarun_t
{
	datarun_t() : m_nStartFlatField( 0 ), m_nEndFlatField( 0 ), m_nLength( 0 )
	{
		for ( int i = 0; i < TD_OFFSET_COUNT; ++i )
		{
			m_nStartOffset[ i ] = 0;
#ifdef _GAMECONSOLE
			// These are the offsets of the next run, for priming the L1 cache
			m_nPrefetchOffset[ i ] = 0;
#endif
		}
	}

	// Indices of start/end fields in the flattened typedescription_t list
	int m_nStartFlatField;
	int m_nEndFlatField;

	// Offsets for run in the packed/unpacked data (I think the run starts need to be properly aligned)
	int m_nStartOffset[ TD_OFFSET_COUNT ]{};
#ifdef _GAMECONSOLE
	// These are the offsets of the next run, for priming the L1 cache
	int m_nPrefetchOffset[ TD_OFFSET_COUNT ];
#endif
	int m_nLength;
};

struct datacopyruns_t
{
public:
	CUtlVector< datarun_t > m_vecRuns;
};

   //-----------------------------------------------------------------------------
   // Purpose: stores the list of objects in the hierarchy
   //            used to iterate through an object's data descriptions
   //-----------------------------------------------------------------------------
struct optimized_datamap_t;
struct datamap_t
{
	typedescription_t* dataDesc;
	int					dataNumFields;
	char const* dataClassName;
	datamap_t* baseMap;

	int					m_nPackedSize;
	optimized_datamap_t* m_pOptimizedDataMap;
};

struct flattenedoffsets_t
{
	CUtlVector< typedescription_t >	m_Flattened;
	int								m_nPackedSize; // Contiguous memory to pack all of these together for TD_OFFSET_PACKED
	int								m_nPackedStartOffset;
};

enum
{
	PC_NON_NETWORKED_ONLY = 0,
	PC_NETWORKED_ONLY,

	PC_COPYTYPE_COUNT,
	PC_EVERYTHING = PC_COPYTYPE_COUNT,
};

struct datamapinfo_t
{
	// Flattened list, with FIELD_EMBEDDED, FTYPEDESC_PRIVATE, 
	//  and FTYPEDESC_OVERRIDE (overridden) fields removed
	flattenedoffsets_t	m_Flat;
	datacopyruns_t		m_CopyRuns;
};

struct optimized_datamap_t
{
	// Optimized info for PC_NON_NETWORKED and PC_NETWORKED data
	datamapinfo_t	m_Info[ PC_COPYTYPE_COUNT ];
};

__declspec( noinline ) static typedescription_t* find_in_data_map_hash( datamap_t* map, fnv_t hash )
{
	while ( map )
	{
		for ( auto i = 0; i < map->dataNumFields; i++ )
		{
			if ( map->dataDesc[ i ].fieldName == nullptr )
				continue;

			if ( hash == fnv1a_rt( map->dataDesc[ i ].fieldName ) )
				return &map->dataDesc[ i ];

			if ( map->dataDesc[ i ].fieldType == FIELD_EMBEDDED )
			{
				if ( map->dataDesc[ i ].td )
				{
					typedescription_t* ptr;

					if ( ( ptr = find_in_data_map_hash( map->dataDesc[ i ].td, hash ) ) )
						return ptr;
				}
			}
		}
		map = map->baseMap;
	}

	return nullptr;
}

//===============================================
#define DATAMAPVAR(funcname, type, datamapname) type& funcname() \
{ \
	static uint16_t _offset; \
	if ( !_offset ) _offset = find_in_data_map_hash( this->get_pred_desc_map(), FNV1A( datamapname ) )->fieldOffset; \
    return *reinterpret_cast< type* >( uintptr_t( this ) + _offset ); \
}
//===============================================
#define DATAMAPVARA(funcname, type, datamapname, add) type& funcname() \
{ \
	static uint16_t _offset; \
	if ( !_offset ) _offset = find_in_data_map_hash( this->get_pred_desc_map(), FNV1A( datamapname ) )->fieldOffset; \
    return *reinterpret_cast< type* >( uintptr_t( this ) + _offset + add ); \
}
//===============================================
#define DATAMAPVARRS(funcname, type, datamapname) type& funcname( int index ) \
{ \
	static uint16_t _offset; \
	if ( !_offset ) _offset = find_in_data_map_hash( this->get_pred_desc_map(), FNV1A( datamapname ) )->fieldOffset; \
    return *reinterpret_cast< type* >( uintptr_t( this ) + _offset + index * 4 ); \
}