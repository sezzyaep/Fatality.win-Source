#pragma once

class CMoveData
{
public:
	bool    m_bFirstRunOfFunctions : 1;
	bool    m_bGameCodeMovedPlayer : 1;
	int     m_nPlayerHandle;        // edict index on server, client entity handle on client=
	int     m_nImpulseCommand;      // Impulse command issued.
	QAngle  m_vecViewAngles;        // Command view angles (local space)
	QAngle  m_vecAbsViewAngles;     // Command view angles (world space)
	int     m_nButtons;             // Attack buttons.
	int     m_nOldButtons;          // From host_client->oldbuttons;
	float   m_flForwardMove;
	float   m_flSideMove;
	float   m_flUpMove;
	float   m_flMaxSpeed;
	float   m_flClientMaxSpeed;
	Vector  m_vecVelocity;          // edict::velocity        // Current movement direction.
	QAngle  m_vecAngles;            // edict::angles
	QAngle  m_vecOldAngles;
	float   m_outStepHeight;        // how much you climbed this move
	Vector  m_outWishVel;           // This is where you tried 
	Vector  m_outJumpVel;           // This is your jump velocity
	Vector  m_vecConstraintCenter;
	float   m_flConstraintRadius;
	float   m_flConstraintWidth;
	float   m_flConstraintSpeedFactor;
	float   m_flUnknown[ 5 ];
	Vector  m_vecAbsOrigin;
};

class IGameMovement
{
public:
	virtual			~IGameMovement( void ) {}

	virtual void	          ProcessMovement( C_BasePlayer* pPlayer, CMoveData* pMove ) = 0;
	virtual void	          Reset( void ) = 0;
	virtual void	          StartTrackPredictionErrors( C_BasePlayer* pPlayer ) = 0;
	virtual void	          FinishTrackPredictionErrors( C_BasePlayer* pPlayer ) = 0;
	virtual void	          DiffPrint( char const* fmt, ... ) = 0;
	virtual Vector const& GetPlayerMins( bool ducked ) const = 0;
	virtual Vector const& GetPlayerMaxs( bool ducked ) const = 0;
	virtual Vector const& GetPlayerViewOffset( bool ducked ) const = 0;
	virtual bool		        IsMovingPlayerStuck( void ) const = 0;
	virtual C_BasePlayer* GetMovingPlayer( void ) const = 0;
	virtual void		        UnblockPusher( C_BasePlayer* pPlayer, C_BasePlayer* pPusher ) = 0;
	virtual void            SetupMovementBounds( CMoveData* pMove ) = 0;
};

struct process_move_changes_t
{
	void store( C_CSPlayer* player )
	{
		view_offset = player->get_view_offset();
		duckamt = player->get_duck_amt();
		duckspeed = player->get_duck_speed();
		last_duck_time = player->get_last_duck_time();
		has_moved_since_last_jump = player->get_has_moved_since_last_jump();
		stamina = player->get_stamina();
		velocity_modifier = player->get_velocity_modifier();
		ground_entity = player->get_ground_entity();
		flags = player->get_flags();
		local = player->get_local();
		ground_accel_linear_frac_last_time = player->get_ground_accel_linear_frac_last_time();
		move_type = player->get_move_type();
		obb_maxs = player->GetCollideable()->OBBMaxs();
		collision_bounds_change_time = player->get_collision_bounds_change_time();
		collision_viewheight = player->get_collision_viewheight();
	}

	void restore( C_CSPlayer* player ) const
	{
		player->get_view_offset() = view_offset;
		player->get_duck_amt() = duckamt;
		player->get_duck_speed() = duckspeed;
		player->get_last_duck_time() = last_duck_time;
		player->get_has_moved_since_last_jump() = has_moved_since_last_jump;
		player->get_stamina() = stamina;
		player->get_velocity_modifier() = velocity_modifier;
		player->get_ground_entity() = ground_entity;
		player->get_flags() = flags;
		player->get_local() = local;
		player->get_ground_accel_linear_frac_last_time() = ground_accel_linear_frac_last_time;
		player->get_move_type() = move_type;
		player->GetCollideable()->OBBMaxs() = obb_maxs;
		player->get_collision_bounds_change_time() = collision_bounds_change_time;
		player->get_collision_viewheight() = collision_viewheight;
	}

	Vector view_offset;
	float duckamt;
	float duckspeed;
	float last_duck_time;
	bool has_moved_since_last_jump;
	float stamina;
	float velocity_modifier;
	CBaseHandle ground_entity;
	int flags;
	CPlayerLocalData local;
	float ground_accel_linear_frac_last_time;
	int move_type;
	Vector obb_maxs;
	float collision_bounds_change_time;
	float collision_viewheight;
};

class CGameMovement
	: public IGameMovement
{
public:
	virtual ~CGameMovement( void ) {}

	inline static bool computing = false;

	inline process_move_changes_t process_movement( C_CSPlayer* player, CMoveData* move )
	{
		process_move_changes_t backup;
		backup.store( player );

		computing = true;
		ProcessMovement( player, move );
		computing = false;

		process_move_changes_t ret;
		ret.store( player );

		backup.restore( player );

		return ret;
	}

	inline CMoveData setup_move( C_CSPlayer* const player, CUserCmd* cmd )
	{
		CMoveData data{};

		data.m_bFirstRunOfFunctions = false;
		data.m_bGameCodeMovedPlayer = false;
		data.m_nPlayerHandle = player->GetRefEHandle();

		data.m_vecVelocity = player->get_velocity();
		data.m_vecAbsOrigin = player->get_abs_origin();
		data.m_flClientMaxSpeed = player->get_max_speed();

		data.m_vecAbsViewAngles = data.m_vecAngles = data.m_vecViewAngles = cmd->viewangles;
		data.m_nImpulseCommand = cmd->impulse;
		data.m_nButtons = cmd->buttons;

		data.m_flForwardMove = cmd->forwardmove;
		data.m_flSideMove = cmd->sidemove;
		data.m_flUpMove = cmd->upmove;

		data.m_vecConstraintCenter = {};
		data.m_flConstraintRadius = 0.f;
		data.m_flConstraintSpeedFactor = 0.f;

		this->SetupMovementBounds( &data );

		return data;
	}

	static void air_accelerate( Vector const& wishdir, float wishspeed, CMoveData* mv, float friction );
	static void accelerate( const Vector& wishdir, float wishspeed, float accel, CMoveData* mv, C_CSPlayer* ent );
	static void friction( CMoveData* mv, C_CSPlayer* player );
	static void check_parameters( CMoveData* mv, C_CSPlayer* player );
	static void air_move( CMoveData* mv, C_CSPlayer* ent );
	static void walk_move( CMoveData* mv, C_CSPlayer* ent );

};

#define	MAX_FORWARD	6
#define	ON_EPSILON 0.1	
#define COLLISION_GROUP_PLAYER_MOVEMENT 8

class IPrediction
{
public:
	VFUNC( 12, GetLocalViewAngles( QAngle& ang ), void( __thiscall* )( void*, QAngle& ) )( ang )
		VFUNC( 13, SetLocalViewAngles( QAngle& ang ), void( __thiscall* )( void*, QAngle& ) )( ang )
		VFUNC( 14, InPrediction(), bool( __thiscall* )( void* ) )( )
		VFUNC( 19, RunCommand( C_BasePlayer* player, CUserCmd* cmd, IMoveHelper* movehelper ), void( __thiscall* )( void*, C_BasePlayer*, CUserCmd*, IMoveHelper* ) )( player, cmd, movehelper )
		VFUNC( 20, SetupMove( C_BasePlayer* player, CUserCmd* cmd, IMoveHelper* movehelper, void* movedata ), void( __thiscall* )( void*, C_BasePlayer*, CUserCmd*, IMoveHelper*, void* ) )( player, cmd, movehelper, movedata )
		VFUNC( 21, FinishMove( C_BasePlayer* player, CUserCmd* cmd, void* movedata ), void( __thiscall* )( void*, C_BasePlayer*, CUserCmd*, void* ) )( player, cmd, movedata )
		VFUNC( 3, Update( int startframe, bool validframe, int incoming_acknowledged, int outgoing_command ), void( __thiscall* )( void*, int, bool, int, int ) )( startframe, validframe, incoming_acknowledged, outgoing_command )
		VFUNC( 24, _Update( bool received_new_world_update, bool validframe, int incoming_acknowledged, int outgoing_command ), void( __thiscall* )( void*, int, bool, bool, int, int ) )( 0, received_new_world_update, validframe, incoming_acknowledged, outgoing_command )
		OFFSET( get_predicted_commands, int, 0x1C )
public:
	char pad[ 0x14 ];
	float			m_flLastServerWorldTimeStamp{};
};

class CPredictionCopy
{
public:
	typedef enum
	{
		TRANSFERDATA_COPYONLY = 0,  // Data copying only (uses runs)
		TRANSFERDATA_ERRORCHECK_NOSPEW, // Checks for errors, returns after first error found
		TRANSFERDATA_ERRORCHECK_SPEW,   // checks for errors, reports all errors to console
		TRANSFERDATA_ERRORCHECK_DESCRIBE, // used by hud_pdump, dumps values, etc, for all fields
	} optype_t;

	typedef enum
	{
		DIFFERS = 0,
		IDENTICAL,
		WITHINTOLERANCE,
	} difftype_t;

	CPredictionCopy( int type, byte* dest, bool dest_packed, byte* src, bool src_packed, optype_t opType, void* func = NULL )
	{
		m_OpType = opType;
		m_nType = type;
		m_pDest = dest;
		m_pSrc = src;
		m_nDestOffsetIndex = dest_packed ? TD_OFFSET_PACKED : TD_OFFSET_NORMAL;
		m_nSrcOffsetIndex = src_packed ? TD_OFFSET_PACKED : TD_OFFSET_NORMAL;

		m_nErrorCount = 0;
		m_nEntIndex = -1;

		m_pWatchField = NULL;
		m_FieldCompareFunc = func;
	}

	FORCEINLINE int ComputeTypeMask( int nType )
	{
		return nType + 1;
	}

#define RESTRICT				__restrict

	void CopyFlatFieldsUsingRuns( const datamap_t* pCurrentMap, int nPredictionCopyType )
	{
		int				fieldOffsetSrc;
		int				fieldOffsetDest;
		byte* RESTRICT pOutputData;
		const byte* RESTRICT pInputData;

		const datacopyruns_t& runs = pCurrentMap->m_pOptimizedDataMap->m_Info[ nPredictionCopyType ].m_CopyRuns;

		byte* RESTRICT pDest = ( byte * RESTRICT )m_pDest;
		const byte* RESTRICT pSrc = ( const byte * RESTRICT )m_pSrc;

		int c = runs.m_vecRuns.Count();
		for ( int i = 0; i < c; ++i )
		{
			const datarun_t* RESTRICT run = &runs.m_vecRuns[ i ];
			fieldOffsetDest = run->m_nStartOffset[ m_nDestOffsetIndex ];
			fieldOffsetSrc = run->m_nStartOffset[ m_nSrcOffsetIndex ];
			pOutputData = pDest + fieldOffsetDest;
			pInputData = pSrc + fieldOffsetSrc;

			memcpy( pOutputData, pInputData, run->m_nLength );
		}
	}

	void TransferDataCopyOnly( const datamap_t* dmap )
	{
		int types = ComputeTypeMask( m_nType );
		for ( int i = 0; i < PC_COPYTYPE_COUNT; ++i )
		{
			if ( types & ( 1 << i ) )
			{
				CopyFlatFieldsUsingRuns( dmap, i );
			}
		}
	}

	template< class T >
	static __forceinline difftype_t CompareField( const typedescription_t* pField, const T* outvalue, const T* invalue, int count )
	{
		return *outvalue == *invalue ? IDENTICAL : DIFFERS;;
	}

	template< class T >
	__forceinline void ProcessField_Compare_NoSpew( const datamap_t* pCurrentMap, const typedescription_t* pField, const T* pOutputData, const T* pInputData, int fieldSize )
	{
		difftype_t difftype = CompareField( pField, pOutputData, pInputData, fieldSize );
		if ( difftype == DIFFERS )
		{
			++m_nErrorCount;
		}
	}

	void ErrorCheckFlatFields_NoSpew( const datamap_t* pCurrentMap, int nPredictionCopyType )
	{
		int				i;
		int				flags;
		int				fieldOffsetSrc;
		int				fieldOffsetDest;
		int				fieldSize;

		const flattenedoffsets_t& flat = pCurrentMap->m_pOptimizedDataMap->m_Info[ nPredictionCopyType ].m_Flat;
		const typedescription_t* pBase = &flat.m_Flattened[ 0 ];
		int fieldCount = flat.m_Flattened.Count();

		const byte* RESTRICT pDest = ( const byte * RESTRICT )m_pDest;
		const byte* RESTRICT pSrc = ( const byte * RESTRICT )m_pSrc;

		const byte* pOutputData;
		const byte* pInputData;

		for ( i = 0; i < fieldCount && !m_nErrorCount; ++i )
		{
			const typedescription_t* RESTRICT pField = &pBase[ i ];

			flags = pField->flags;
			if ( flags & 0x0400 )
				continue;

			fieldOffsetDest = pField->flatOffset[ m_nDestOffsetIndex ];
			fieldOffsetSrc = pField->flatOffset[ m_nSrcOffsetIndex ];

			pOutputData = pDest + fieldOffsetDest;
			pInputData = pSrc + fieldOffsetSrc;

			fieldSize = pField->fieldSizeInBytes;

			if ( fnv1a_rt( pField->fieldName ) != FNV1A( "m_nTickBase" ) )
				continue;

			ProcessField_Compare_NoSpew( pCurrentMap, pField, ( int* ) pOutputData, ( int* ) pInputData, fieldSize );
		}
	}

	// Stop at first error
	void TransferDataErrorCheckNoSpew( char const* pchOperation, const datamap_t* dmap )
	{
		int types = ComputeTypeMask( m_nType );
		for ( int i = 0; i < PC_COPYTYPE_COUNT && !m_nErrorCount; ++i )
		{
			if ( types & ( 1 << i ) )
			{
				ErrorCheckFlatFields_NoSpew( dmap, i );
			}
		}
	}

	int TransferData( const char* operation, int entindex, datamap_t* dmap )
	{
		m_nEntIndex = entindex;

		switch ( m_OpType )
		{
			case TRANSFERDATA_COPYONLY:  // Data copying only (uses runs)
			{
				TransferDataCopyOnly( dmap );
				break;
			}
			case TRANSFERDATA_ERRORCHECK_NOSPEW: // Checks for errors, returns after first error found
			{
				TransferDataErrorCheckNoSpew( operation, dmap );
			}
			break;
			default:
				break;
		}

		return m_nErrorCount;
	}


	optype_t		m_OpType;
	int				m_nType;
	byte* m_pDest;
	byte* m_pSrc;
	int				m_nDestOffsetIndex;
	int				m_nSrcOffsetIndex;
	int				m_nErrorCount;
	int				m_nEntIndex;

	void* m_FieldCompareFunc;

	const typedescription_t* m_pWatchField;
	char const* m_pOperation{};
};
