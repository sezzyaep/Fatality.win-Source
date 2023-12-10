#pragma once

enum class resolver_direction;
struct lag_record_t;
class C_CSPlayer;
class QAngle;
class Vector;

class NoticeText_t
{
public:
	wchar_t m_nString[ 512 ]; //0x0000 
	char pad_0x0400[ 0xC ]; //0x0400
	float set; //0x040C
	float m_flStartTime; //0x0410 
	float m_flStartTime2; //0x0414 
	float m_flLifeTimeModifier; //0x0418 
	char pad_0x041C[ 0x4 ]; //0x041C
}; //Size=0x420

class SFHudDeathNoticeAndBotStatus
{
public:
	char pad_0x00[ 0x6C ];
	CUtlVector<NoticeText_t>m_nDeathNotices;
};

typedef unsigned short ClientRenderHandle_t;
class IClientLeafSystem
{
public:
	VFUNC( 0, CreateRenderableHandle( void* obj ), void( __thiscall* )( void*, int, int, char, int, int ) )( reinterpret_cast< uintptr_t >( obj ) + 0x4, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF )
		VFUNC( 1, RemoveRenderable( ClientRenderHandle_t handle ), void( __thiscall* )( void*, ClientRenderHandle_t ) )( handle )
		VFUNC( 15, RenderableChanged( ClientRenderHandle_t handle ), void( __thiscall* )( void*, ClientRenderHandle_t ) )( handle )
};

class CUserCmd
{
public:
	virtual ~CUserCmd() {};
	int		command_number;
	int		tick_count;
	QAngle	viewangles;
	Vector	aimdirection;
	float	forwardmove;
	float	sidemove;
	float	upmove;
	int		buttons;
	byte    impulse;
	int		weaponselect;
	int		weaponsubtype;
	int		random_seed;
	short	mousedx;
	short	mousedy;
	bool	hasbeenpredicted;

	// vac
	QAngle  headangles;
	int     buttons2;

	char	pad_0x4C[ 8 ];

	int get_tick_count() const { return tick_count; } void set_tick_count( int t ) { tick_count = t; }
	QAngle get_view_angles() const { return viewangles; } void set_view_angles( const QAngle& ang ) { viewangles = ang; }
	float get_forward_move() const { return forwardmove; } void set_forward_move( float f ) { forwardmove = f; }
	float get_side_move() const { return sidemove; } void set_side_move( float f ) { sidemove = f; }
	float get_up_move() const { return upmove; } void set_up_move( float f ) { upmove = f; }
};

typedef unsigned int CRC32_t;
class CVerifiedUserCmd
{
public:
	CUserCmd	m_cmd;
	CRC32_t		m_crc;
};


class CStrikeWeaponDefinition
{
public:
	char pad_0x0000[ 0x8 ]; //0x0000
	int m_nWeaponID; //0x0008
};

class CCSWeaponData
{
public:

	virtual ~CCSWeaponData() {};

	char* consolename; // 0x0004
	char pad_0008[ 8 ]; // 0x0008
	void* m_pweapondef; //0x0010
	int maxclip1; // 0x0014
	int imaxclip2; // 0x0018
	int idefaultclip1; // 0x001c
	int idefaultclip2; // 0x0020
	char pad_0024[ 8 ]; // 0x0024
	char* szworldmodel; // 0x002c
	char* szviewmodel; // 0x0030
	char* szdroppedmodel; // 0x0034
	char pad_0038[ 4 ]; // 0x0038
	char* n0000023e; // 0x003c
	char pad_0040[ 56 ]; // 0x0040
	char* szemptysound; // 0x0078
	char pad_007c[ 4 ]; // 0x007c
	char* szbullettype; // 0x0080
	char pad_0084[ 4 ]; // 0x0084
	char* szhudname; // 0x0088
	char* szweaponname; // 0x008c
	char pad_0090[ 60 ]; // 0x0090
	int weapontype; // 0x00c8
	int iweaponprice; // 0x00cc
	int ikillaward; // 0x00d0
	char* szanimationprefix; // 0x00d4
	float cycle_time; // 0x00d8
	float cycle_time_alt; // 0x00dc
	float fltimetoidle; // 0x00e0
	float flidleinterval; // 0x00e4
	bool bfullauto; // 0x00e8
	char pad_0x00e5[ 3 ]; // 0x00e9
	int idamage; // 0x00ec
	float padding;
	float flarmorratio; // 0x00f0
	int ibullets; // 0x00f4
	float flpenetration; // 0x00f8
	float flflinchvelocitymodifierlarge; // 0x00fc
	float flflinchvelocitymodifiersmall; // 0x0100
	float range; // 0x0104
	float flrangemodifier; // 0x0108
	float flthrowvelocity; // 0x010c
	char pad_0x010c[ 16 ]; // 0x0110
	bool bhassilencer; // 0x011c
	char pad_0x0119[ 3 ]; // 0x011d
	char* psilencermodel; // 0x0120
	int icrosshairmindistance; // 0x0124
	float flmaxplayerspeed; // 0x0128
	float flmaxplayerspeedalt; // 0x012c
	char pad_0x0130[ 4 ]; // 0x0130
	float flspread; // 0x0134
	float flspreadalt; // 0x0138
	float flinaccuracycrouch; // 0x013c
	float flinaccuracycrouchalt; // 0x0140
	float flinaccuracystand; // 0x0144
	float flinaccuracystandalt; // 0x0148
	float flinaccuracyjumpinitial; // 0x014c
	float flinaccuracyjump; // 0x0150
	float flinaccuracyjumpalt; // 0x0154
	float flinaccuracyland; // 0x0158
	float flinaccuracylandalt; // 0x015c
	float flinaccuracyladder; // 0x0160
	float flinaccuracyladderalt; // 0x0164
	float flinaccuracyfire; // 0x0168
	float flinaccuracyfirealt; // 0x016c
	float flinaccuracymove; // 0x0170
	float flinaccuracymovealt; // 0x0174
	uint8_t pad_0x0180[ 0x4C ];
	int32_t zoom_levels;
	int32_t zoom_fov1;
	int32_t zoom_fov2;
	uint8_t pad_0x01D4[ 0xC ];

	[[nodiscard]] std::string get_icon_name( int def_idx = -1 ) const
	{
		if ( def_idx == 64 )
			return xorstr_( "revolver" );

		if ( def_idx == 63 )
			return xorstr_( "cz75a" );

		auto name = this->szweaponname;
		if ( const auto underscore = strchr( name, '_' ) )
			name = underscore + 1;

		return name;
	}
};

class CViewSetup
{
public:
	int			x, x_old;
	int			y, y_old;
	int			width, width_old;
	int			height, height_old;
	bool		m_bOrtho;
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;
	bool		m_bCustomViewMatrix;
	matrix3x4_t	m_matCustomViewMatrix;
	char		pad_0x68[ 0x48 ];
	float		fov;
	float		fovViewmodel;
	Vector		origin;
	QAngle		angles;
	float		zNear;
	float		zFar;
	float		zNearViewmodel;
	float		zFarViewmodel;
	float		m_flAspectRatio;
	float		m_flNearBlurDepth;
	float		m_flNearFocusDepth;
	float		m_flFarFocusDepth;
	float		m_flFarBlurDepth;
	float		m_flNearBlurRadius;
	float		m_flFarBlurRadius;
	int			m_nDoFQuality;
	int			m_nMotionBlurMode;
	float		m_flShutterTime;
	Vector		m_vShutterOpenPosition;
	QAngle		m_shutterOpenAngles;
	Vector		m_vShutterClosePosition;
	QAngle		m_shutterCloseAngles;
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;
	bool		m_bOffCenter : 1;

	// set to true if this is to draw into a subrect of the larger screen
	// this really is a hack, but no more than the rest of the way this class is used
	bool		m_bRenderToSubrectOfLargerScreen : 1;

	// Controls that the SFM needs to tell the engine when to do certain post-processing steps
	bool		m_bDoBloomAndToneMapping : 1;
	bool		m_bDoDepthOfField : 1;
	bool		m_bHDRTarget : 1;
	bool		m_bDrawWorldNormal : 1;
	bool		m_bCullFrontFaces : 1;

	// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
	bool		m_bCacheFullSceneState : 1;

	// True if this is a CSM depth view. The CSM world->view matrix doesn't have an XY translation (that's moved into the CSM ortho view->projection 
	// matrix to address continuity issues), so the usual assumptions made about camera/view space do not necessarily apply.
	bool		m_bCSMView : 1;
};

class IInterpolatedVar
{
public:
	VFUNC( 8, RestoreToLastNetworked(), void( __thiscall* )( void* ) )( );
	VFUNC( 10, GetDebugName(), const char* ( __thiscall* )( void* ) )( );
	VFUNC( 2, SetInterpolationAmount( const float seconds ), void( __thiscall* )( void*, float ) )( seconds );

	inline uint8_t& get_type_flags() { return *reinterpret_cast< uint8_t* >( uintptr_t( this ) + 0x1C ); }
	inline float& get_interp_amt() { return *reinterpret_cast< float* >( uintptr_t( this ) + 0x24 ); }
};

class VarMapEntry_t
{


public:
	unsigned short		type;
	unsigned short		m_bNeedsToInterpolate;	// Set to false when this var doesn't
												// need Interpolate() called on it anymore.
	void* data;
	IInterpolatedVar* watcher;
};

struct VarMapping_t
{
	VarMapping_t()
	{
		m_nInterpolatedEntries = 0;
	}

	IInterpolatedVar* find( uint32_t hash )
	{
		for ( const auto& item : m_Entries )
		{
			if ( fnv1a_rt( item.watcher->GetDebugName() ) == hash )
				return item.watcher;
		}

		return nullptr;
	}

	CUtlVector< VarMapEntry_t >	m_Entries;
	int							m_nInterpolatedEntries;
	float						m_lastInterpolationTime{};
};

class CStudioHdr;
struct C_AnimationLayer
{
public:
	char  pad_0000[ 20 ];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float m_flPrevCycle; //0x001C
	float m_flWeight; //0x0020
	float m_flWeightDeltaRate; //0x0024
	float m_flPlaybackRate; //0x0028
	float m_flCycle; //0x002C
	C_CSPlayer* m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[ 4 ]; //0x0034
	bool operator==( const C_AnimationLayer& other ) const
	{
		return m_nSequence == other.m_nSequence && m_flWeight == other.m_flWeight &&
			m_flWeightDeltaRate == other.m_flWeightDeltaRate &&
			m_flPlaybackRate == other.m_flPlaybackRate && m_flCycle == other.m_flCycle
			&& m_flPrevCycle == other.m_flPrevCycle && m_nOrder == other.m_nOrder;
	}
	C_AnimationLayer& operator=( const C_AnimationLayer& rhs )
	{
		if ( this == &rhs )
			return *this;

		this->m_nSequence = rhs.m_nSequence;
		this->m_flCycle = rhs.m_flCycle;
		this->m_flPlaybackRate = rhs.m_flPlaybackRate;
		this->m_flPrevCycle = rhs.m_flPrevCycle;
		this->m_flWeight = rhs.m_flWeight;
		this->m_flWeightDeltaRate = rhs.m_flWeightDeltaRate;
		this->m_nOrder = rhs.m_nOrder;

		return *this;
	}
	void set_layer_weight_rate( const float delta, const float previous );
	void set_layer_ideal_weight_from_sequence_cycle( CStudioHdr* hdr );
	void increment_layer_cycle( const float delta, const bool loop );
	void increment_layer_cycle_weight_rate_generic( const float delta, CStudioHdr* hdr );
	bool has_animation_completed( const float delta = 0.f ) const;
	bool has_sequence_completed( const float delta = 0.f ) const;
};

class ClientClass;
class CEventInfo
{
public:
	uint16_t classID; //0x0000 0 implies not in use
	char pad_0002[ 2 ]; //0x0002 
	float fire_delay; //0x0004 If non-zero, the delay time when the event should be fired ( fixed up on the client )
	char pad_0008[ 4 ]; //0x0008
	ClientClass* pClientClass; //0x000C
	void* pData; //0x0010 Raw event data
	char pad_0014[ 48 ]; //0x0014
}; //Size: 0x0044

class C_TEFireBullets
{
public:
	char pad[ 12 ];
	int		m_iPlayer; //12
	int _m_iItemDefinitionIndex;
	Vector	_m_vecOrigin;
	QAngle	m_vecAngles;
	int		_m_iWeaponID;
	int		m_iMode;
	int		m_iSeed;
	float	m_flSpread;
};

struct Item_t
{
	Item_t( std::string model, std::string icon = "" ) :
		model( model ),
		icon( icon )
	{}

	std::string model;
	std::string icon;
};

struct WeaponName_t
{
	WeaponName_t( int definition_index, std::string name ) :
		definition_index( definition_index ),
		name( name )
	{}

	int definition_index = 0;
	std::string name = {};
};

struct QualityName_t
{
	QualityName_t( int index, std::string name ) :
		index( index ),
		name( name )
	{}

	int index = 0;
	std::string name;
};

struct trace_player_t
{
	matrix3x4_t* matrix1[ 128 ];
	matrix3x4_t* matrix2[ 128 ];
	matrix3x4_t* matrix3[ 128 ];
	matrix3x4_t* matrix4[ 128 ];
	matrix3x4_t* matrix5[ 128 ];
	matrix3x4_t* matrix6[ 128 ];

	matrix3x4_t* matrix_extra1[ 128 ];
	matrix3x4_t* matrix_extra2[ 128 ];
	matrix3x4_t* matrix_extra3[ 128 ];
	resolver_direction resolve_mode = {};
	bool safe_point = false;
	bool scan_extra = false;
	C_CSPlayer* player = nullptr;
	Vector origin = {};
	Vector extra_origin = {};
	lag_record_t* record = nullptr;
};

struct draw_model_ent_t
{
	bool active = false;
	matrix3x4_t matrix[ 128 ];
	float time = 0.f;
};

class CAttachmentData
{
public:
	matrix3x4_t	m_AttachmentToWorld;
	QAngle	m_angRotation;
	Vector	m_vOriginVelocity;
	int		m_nLastFramecount : 31;
	int		m_bAnglesComputed : 1;
};

struct BuildRenderListInfo_t
{
	Vector	m_vecMins;
	Vector	m_vecMaxs;
	short	m_nArea;
	uint8_t	m_nAlpha;
	bool	m_bPerformOcclusionTest : 1;
	bool	m_bIgnoreZBuffer : 1;
};

class IClientRenderable;
struct RenderableInfo_t
{
	IClientRenderable* m_pRenderable;
	void* m_pAlphaProperty;
	int					m_EnumCount;				// Have I been added to a particular shadow yet?
	int					m_nRenderFrame;
	unsigned short		m_FirstShadow;				// The first shadow caster that cast on it
	unsigned short		m_LeafList;					// What leafs is it in?
	short				m_Area;						// -1 if the renderable spans multiple areas.
	uint16				m_Flags;					// rendering flags
	uint16				m_bRenderInFastReflection : 1;	// Should we render in the "fast" reflection?
	uint16				m_bDisableShadowDepthRendering : 1;	// Should we not render into the shadow depth map?
	uint16				m_bDisableCSMRendering : 1;			// Should we not render into the CSM?
	uint16				m_bDisableShadowDepthCaching : 1;	// Should we not be cached in the shadow depth map?
	uint16				m_nSplitscreenEnabled : 2;	// splitscreen rendering flags
	uint16				m_nTranslucencyType : 2;	// RenderableTranslucencyType_t
	uint16				m_nModelType : 8;			// RenderableModelType_t
	Vector				m_vecBloatedAbsMins;		// Use this for tree insertion
	Vector				m_vecBloatedAbsMaxs;
	Vector				m_vecAbsMins;			// NOTE: These members are not threadsafe!!
	Vector				m_vecAbsMaxs;			// They can be updated from any viewpoint (based on RENDER_FLAGS_BOUNDS_VALID)
	int				    pad;
};

struct ScreenSizeComputeInfo_t
{
	VMatrix m_matViewProj;
	Vector m_vecViewUp;
	int m_nViewportHeight;
};

struct SetupRenderInfo_t
{
	mutable void* m_pWorldListInfo;
	mutable void* m_pRenderList;
	Vector m_vecRenderOrigin{};
	Vector m_vecRenderForward{};
	int m_nRenderFrame{};
	int m_nDetailBuildFrame{};	// The "render frame" for detail objects

	float m_flRenderDistSq{};
	int m_nViewID{};
	int m_nBuildViewID;
	int m_nOcclustionViewID;
	mutable const void* m_pCSMVolumeCuller;
	mutable const void* m_pFrustum;
	mutable void** m_ppFrustumList;
	ScreenSizeComputeInfo_t m_screenSizeInfo{};

	bool m_bDrawDetailObjects : 1;
	bool m_bDrawTranslucentObjects : 1;
	bool m_bFastEntityRendering : 1;
	bool m_bDrawDepthViewNonCachedObjectsOnly : 1;
	bool m_bCSMView : 1;

	SetupRenderInfo_t()
	{
		m_pWorldListInfo = NULL;
		m_pRenderList = NULL;
		m_pCSMVolumeCuller = NULL;
		m_pFrustum = NULL;
		m_ppFrustumList = NULL;
		m_nBuildViewID = -1;
		m_nOcclustionViewID = -1;

		m_bDrawDetailObjects = true;
		m_bDrawTranslucentObjects = true;
		m_bFastEntityRendering = false;
		m_bDrawDepthViewNonCachedObjectsOnly = false;
		m_bCSMView = false;
	}
};

struct lod_data
{
	uint32_t bone_mask;
	uint32_t flags, old_flags;
	uint32_t frame;
	float distance;
};