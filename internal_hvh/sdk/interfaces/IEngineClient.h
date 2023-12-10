#pragma once

enum
{
	SNDLEVEL_NONE = 0,			/**< None */
	SNDLEVEL_RUSTLE = 20,		/**< Rustling leaves */
	SNDLEVEL_WHISPER = 25,		/**< Whispering */
	SNDLEVEL_LIBRARY = 30,		/**< In a library */
	SNDLEVEL_FRIDGE = 45,		/**< Refrigerator */
	SNDLEVEL_HOME = 50,			/**< Average home (3.9 attn) */
	SNDLEVEL_CONVO = 60,		/**< Normal conversation (2.0 attn) */
	SNDLEVEL_DRYER = 60,		/**< Clothes dryer */
	SNDLEVEL_DISHWASHER = 65,	/**< Dishwasher/washing machine (1.5 attn) */
	SNDLEVEL_CAR = 70,			/**< Car or vacuum cleaner (1.0 attn) */
	SNDLEVEL_NORMAL = 75,		/**< Normal sound level */
	SNDLEVEL_TRAFFIC = 75,		/**< Busy traffic (0.8 attn) */
	SNDLEVEL_MINIBIKE = 80,		/**< Mini-bike, alarm clock (0.7 attn) */
	SNDLEVEL_SCREAMING = 90,	/**< Screaming child (0.5 attn) */
	SNDLEVEL_TRAIN = 100,		/**< Subway train, pneumatic drill (0.4 attn) */
	SNDLEVEL_HELICOPTER = 105,	/**< Helicopter */
	SNDLEVEL_SNOWMOBILE = 110,	/**< Snow mobile */
	SNDLEVEL_AIRCRAFT = 120,	/**< Auto horn, aircraft */
	SNDLEVEL_RAIDSIREN = 130,	/**< Air raid siren */
	SNDLEVEL_GUNFIRE = 140,		/**< Gunshot, jet engine (0.27 attn) */
	SNDLEVEL_ROCKET = 180,		/**< Rocket launching (0.2 attn) */
};

#define SNDVOL_NORMAL		1.0		/**< Normal volume */
#define SNDPITCH_NORMAL		100		/**< Normal pitch */
#define SNDPITCH_LOW		95		/**< A low pitch */
#define SNDPITCH_HIGH		120		/**< A high pitch */
#define SNDATTN_NONE		0.0		/**< No attenuation */
#define SNDATTN_NORMAL		0.8		/**< Normal attenuation */
#define SNDATTN_STATIC		1.25	/**< Static attenuation? */
#define SNDATTN_RICOCHET	1.5		/**< Ricochet effect */
#define SNDATTN_IDLE		2.0		/**< Idle attenuation? */

#define CCHANVOLUMES	12

struct gain_t
{
	float		ob_gain;		// gain drop if sound source obscured from listener
	float		ob_gain_target;	// target gain while crossfading between ob_gain & ob_gain_target
	float		ob_gain_inc;	// crossfade increment
};

struct hrtf_info_t
{
	Vector		vec;	// Sound source relative to the listener, updated every frame for channels using HRTF.
	float		lerp;	// 1.0 = use phonon fully, 0.0 = don't use phonon at all.
	bool		follow_entity;   // If true, we update the position of the entity every frame, otherwise we use the position of the sound.
	bool		bilinear_filtering;  // If true, we use more expensive bilinear filtering for this sound.
	bool		debug_lock_position;   // If true, the vec will not be modified after the sound starts.
};

// Max 2 player splitscreen in portal (don't merge this back), saves a bunch of memory [8/31/2010 tom]
#define MAX_SPLITSCREEN_CLIENT_BITS 1
// this should == MAX_JOYSTICKS in InputEnums.h
#define MAX_SPLITSCREEN_CLIENTS	( 1 << MAX_SPLITSCREEN_CLIENT_BITS ) // 2

typedef unsigned int HSOUNDSCRIPTHASH;
typedef int SoundSource;
typedef void* FileNameHandle_t;

class CAudioSource;
class CSfxTable
{
public:
	CSfxTable();

	// gets sound name, possible decorated with prefixes
	virtual const char* getname( char* pBuf, size_t bufLen );
	// gets the filename, the part after the optional prefixes
	const char* GetFileName( char* pBuf, size_t bufLen );
	FileNameHandle_t	GetFileNameHandle();

	void				SetNamePoolIndex( int index );
	bool				IsPrecachedSound();
	void				OnNameChanged( const char* pName );

	int					m_namePoolIndex;
	CAudioSource* pSource;

	bool				m_bUseErrorFilename : 1;
	bool				m_bIsUISound : 1;
	bool				m_bIsLateLoad : 1;
	bool				m_bMixGroupsCached : 1;
	bool				m_bIsMusic : 1;
	bool				m_bIsCreatedByQueuedLoader : 1;

	byte				m_mixGroupCount;
	// UNDONE: Use a fixed bit vec here?
	byte				m_mixGroupList[ 8 ];

private:
	// Only set in debug mode so you can see the name.
	const char* m_pDebugName;
};


//-----------------------------------------------------------------------------
// Purpose: Each currently playing wave is stored in a channel
//-----------------------------------------------------------------------------
// NOTE: 128bytes.  These are memset to zero at some points.  Do not add virtuals without changing that pattern.
// UNDONE: now 300 bytes...
class channel_t
{
public:
	int32_t guid; //0x0000
	char pad_0004[ 240 ]; //0x0004
	int32_t soundsource; //0x00F4
	int32_t entchannel; //0x00F8
	int32_t speakerentity; //0x00FC
	int16_t master_vol; //0x0100
	int16_t basePitch; //0x0102
	char pad_0104[ 40 ]; //0x0104
	float last_vol; //0x012C
	Vector origin; //0x0130
	Vector direction; //0x013C
	char pad_0148[ 28 ]; //0x0148
	float radius; //0x0164
	char pad_0168[ 44 ]; //0x0168
	struct
	{
		bool bUpdatePositions; //0x0194
		bool isSentence; //0x0195
		bool bdry; //0x0196
		bool bSpeaker; //0x0197
		bool bstereowav; //0x0198
		bool delayed_start; //0x0199
		bool fromserver; //0x019A
	} flags;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define	MAX_CHANNELS			128
#define	MAX_DYNAMIC_CHANNELS	32

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class CChannelList
{
public:
	int		Count() const;
	int		GetChannelIndex( int listIndex );
	channel_t* GetChannel( int listIndex );
	void	RemoveChannelFromList( int listIndex );
	bool	IsQuashed( int listIndex );

	int		m_count;
	short	m_list[ MAX_CHANNELS ];
	bool	m_quashed[ MAX_CHANNELS ]; // if true, the channel should be advanced, but not mixed, because it's been heuristically suppressed
	bool	m_hasSpeakerChannels : 1;
	bool	m_hasDryChannels : 1;
	bool	m_has11kChannels : 1;
	bool	m_has22kChannels : 1;
	bool	m_has44kChannels : 1;
};

inline int CChannelList::Count() const
{
	return m_count;
}

inline int CChannelList::GetChannelIndex( int listIndex )
{
	return m_list[ listIndex ];
}
inline channel_t* CChannelList::GetChannel( int listIndex )
{
	const auto channels = reinterpret_cast< channel_t* >( make_offset( "engine.dll", sig_sound_channels ) );
	return &channels[ GetChannelIndex( listIndex ) ];
}

struct sfx_table
{
	VFUNC( 0, get_name( char* buf, size_t len ), const char* ( __thiscall* )( void*, char*, size_t ) )( buf, len )
};

struct start_sound_params
{
	int32_t user_data;
	int32_t sound_source;
	int32_t ent_channel;
	sfx_table* sfx;
	Vector origin;
	Vector direction;
	float volume;
	int32_t sound_level;
	int32_t flags;
	int				pitch;
	float			delay;
	int				speakerentity;
	int				initialStreamPosition;
	int				skipInitialSamples;
	int				m_nQueuedGUID;
	HSOUNDSCRIPTHASH m_nSoundScriptHash;
	const char* m_pSoundEntryName;
	KeyValues* m_pOperatorsKV;
	float			opStackElapsedTime;
	float			opStackElapsedStopTime;

	bool			staticsound : 1;
	bool			bUpdatePositions : 1;
	bool			fromserver : 1;
	bool			bToolSound : 1;
	bool			m_bIsScriptHandle : 1;
	bool			m_bDelayedStart : 1;
	bool			m_bInEyeSound : 1;
	bool			m_bHRTFFollowEntity : 1;
	bool			m_bHRTFBilinear : 1;
	bool			m_bHRTFLock : 1;
};

class IEngineSound
{
public:
public:
// Precache a particular sample
	virtual bool PrecacheSound( const char* pSample, bool bPreload = false, bool bIsUISound = false ) = 0;
	virtual bool IsSoundPrecached( const char* pSample ) = 0;
	virtual void PrefetchSound( const char* pSample ) = 0;
	virtual bool IsLoopingSound( const char* pSample ) = 0;

	// Just loads the file header and checks for duration (not hooked up for .mp3's yet)
	// Is accessible to server and client though
	virtual float GetSoundDuration( const char* pSample ) = 0;

	// Pitch of 100 is no pitch shift.  Pitch > 100 up to 255 is a higher pitch, pitch < 100
	// down to 1 is a lower pitch.   150 to 70 is the realistic range.
	// EmitSound with pitch != 100 should be used sparingly, as it's not quite as
	// fast (the pitchshift mixer is not native coded).

	// NOTE: setting iEntIndex to -1 will cause the sound to be emitted from the local
	// player (client-side only)
	virtual int EmitSound( void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
		float flVolume, float flAttenuation, int nSeed, int iFlags = 0, int iPitch = 0,
		const Vector* pOrigin = NULL, const Vector* pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1 ) = 0;

	virtual int EmitSound( void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
		float flVolume, int iSoundlevel, int nSeed, int iFlags = 0, int iPitch = 0,
		const Vector* pOrigin = NULL, const Vector* pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1 ) = 0;

	virtual void EmitSentenceByIndex( void* filter, int iEntIndex, int iChannel, int iSentenceIndex,
		float flVolume, int iSoundlevel, int nSeed, int iFlags = 0, int iPitch = 0,
		const Vector* pOrigin = NULL, const Vector* pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1 ) = 0;

	virtual void    StopSound( int iEntIndex, int iChannel, const char* pSample, unsigned int nSoundEntryHash ) = 0;
	virtual void    StopAllSounds( bool bClearBuffers ) = 0;
	virtual void    SetRoomType( void* filter, int roomType ) = 0;
	virtual void    SetPlayerDSP( void* filter, int dspType, bool fastReset ) = 0;
	virtual int     EmitAmbientSound( const char* pSample, float flVolume = SNDVOL_NORMAL, int iPitch = SNDPITCH_NORMAL, int flags = 0, float soundtime = 0.0f ) = 0;
	virtual float   GetDistGainFromSoundLevel( int soundlevel, float dist ) = 0;
	virtual int		GetGuidForLastSoundEmitted() = 0;
	virtual bool	IsSoundStillPlaying( int guid ) = 0;
	virtual void	StopSoundByGuid( int guid, bool bForceSync ) = 0;
	virtual void	SetVolumeByGuid( int guid, float fvol ) = 0;
	virtual void	pad1() = 0;
	virtual void	GetActiveSounds( CUtlVector<SndInfo_t>& sndlist ) = 0;
	virtual void	PrecacheSentenceGroup( const char* pGroupName ) = 0;
	virtual void	NotifyBeginMoviePlayback() = 0;
	virtual void	NotifyEndMoviePlayback() = 0;
	virtual bool	GetSoundChannelVolume( const char* sound, float& flVolumeLeft, float& flVolumeRight ) = 0;
	virtual float	GetElapsedTimeByGuid( int guid ) = 0;
};

typedef struct InputContextHandle_t__* InputContextHandle_t;
struct client_textmessage_t;
struct model_t;
class SurfInfo;
class IMaterial;
class CSentence;
class CAudioSource;
class AudioState_t;
class ISpatialQuery;
class IMaterialSystem;
class CPhysCollide;
class IAchievementMgr;
class INetChannelInfo;
class ISPSharedMemory;
class CGamestatsData;
class CSteamAPIContext;
struct Frustum_t;
class pfnDemoCustomDataCallback;

class IEngineClient
{
public:
	virtual int                   GetIntersectingSurfaces( const model_t* model, const Vector& vCenter, const float radius, const bool bOnlyVisibleSurfaces, SurfInfo* pInfos, const int nMaxInfos ) = 0;
	virtual Vector                GetLightForPoint( const Vector& pos, bool bClamp ) = 0;
	virtual IMaterial* TraceLineMaterialAndLighting( const Vector& start, const Vector& end, Vector& diffuseLightColor, Vector& baseColor ) = 0;
	virtual const char* ParseFile( const char* data, char* token, int maxlen ) = 0;
	virtual bool                  CopyFile( const char* source, const char* destination ) = 0;
	virtual void                  GetScreenSize( int& width, int& height ) = 0;
	virtual void                  ServerCmd( const char* szCmdString, bool bReliable = true ) = 0;
	virtual void                  ClientCmd( const char* szCmdString ) = 0;
	virtual bool                  GetPlayerInfo( int ent_num, player_info_t* pinfo ) = 0;
	virtual int                   GetPlayerForUserID( int userID ) = 0;
	virtual client_textmessage_t* TextMessageGet( const char* pName ) = 0; // 10
	virtual bool                  Con_BoneIsVisible( void ) = 0;
	virtual int                   GetLocalPlayer( void ) = 0;
	virtual const model_t* LoadModel( const char* pName, bool bProp = false ) = 0;
	virtual float                 GetLastTimeStamp( void ) = 0;
	virtual CSentence* GetSentence( CAudioSource* pAudioSource ) = 0; // 15
	virtual float                 GetSentenceLength( CAudioSource* pAudioSource ) = 0;
	virtual bool                  IsStreaming( CAudioSource* pAudioSource ) const = 0;
	virtual void                  GetViewAngles( QAngle& va ) = 0;
	virtual void                  SetViewAngles( QAngle& va ) = 0;
	virtual int                   GetMaxClients( void ) = 0; // 20
	virtual   const char* k_LookupBinding( const char* pBinding ) = 0;
	virtual const char* k_BindingForKey( int& code ) = 0;
	virtual void                  k_SetBinding( int, char const* ) = 0;
	virtual void                  StartKeyTrapMode( void ) = 0;
	virtual bool                  CheckDoneKeyTrapping( int& code ) = 0;
	virtual bool                  IsInGame( void ) = 0;
	virtual bool                  IsConnected( void ) = 0;
	virtual bool                  IsDrawingLoadingImage( void ) = 0;
	virtual void                  HideLoadingPlaque( void ) = 0;
	virtual void                  Con_NPrintf( int pos, const char* fmt, ... ) = 0; // 30
	virtual void                  Con_NXPrintf( const struct con_nprint_s* info, const char* fmt, ... ) = 0;
	virtual int                   IsBoxVisible( const Vector& mins, const Vector& maxs ) = 0;
	virtual int                   IsBoxInViewCluster( const Vector& mins, const Vector& maxs ) = 0;
	virtual bool                  CullBox( const Vector& mins, const Vector& maxs ) = 0;
	virtual void                  Sound_ExtraUpdate( void ) = 0;
	virtual const char* GetGameDirectory( void ) = 0;
	virtual const VMatrix& WorldToScreenMatrix() = 0;
	virtual const VMatrix& WorldToViewMatrix() = 0;
	virtual int                   GameLumpVersion( int lumpId ) const = 0;
	virtual int                   GameLumpSize( int lumpId ) const = 0; // 40
	virtual bool                  LoadGameLump( int lumpId, void* pBuffer, int size ) = 0;
	virtual int                   LevelLeafCount() const = 0;
	virtual ISpatialQuery* GetBSPTreeQuery() = 0;
	virtual void                  LinearToGamma( float* linear, float* gamma ) = 0;
	virtual float                 LightStyleValue( int style ) = 0; // 45
	virtual void                  ComputeDynamicLighting( const Vector& pt, const Vector* pNormal, Vector& color ) = 0;
	virtual void                  GetAmbientLightColor( Vector& color ) = 0;
	virtual int                   GetDXSupportLevel() = 0;
	virtual bool                  SupportsHDR() = 0;
	virtual void                  Mat_Stub( IMaterialSystem* pMatSys ) = 0; // 50
	virtual void                  GetChapterName( char* pchBuff, int iMaxLength ) = 0;
	virtual char const* GetLevelName( void ) = 0;
	virtual char const* GetLevelNameShort( void ) = 0;
	virtual char const* GetMapGroupName( void ) = 0;
	virtual struct IVoiceTweak_s* GetVoiceTweakAPI( void ) = 0;
	virtual void                  SetVoiceCasterID( unsigned int someint ) = 0; // 56
	virtual void                  EngineStats_BeginFrame( void ) = 0;
	virtual void                  EngineStats_EndFrame( void ) = 0;
	virtual void                  FireEvents() = 0;
	virtual int                   GetLeavesArea( unsigned short* pLeaves, int nLeaves ) = 0;
	virtual bool                  DoesBoxTouchAreaFrustum( const Vector& mins, const Vector& maxs, int iArea ) = 0; // 60
	virtual int                   GetFrustumList( Frustum_t** pList, int listMax ) = 0;
	virtual bool                  ShouldUseAreaFrustum( int i ) = 0;
	virtual void                  SetAudioState( const AudioState_t& state ) = 0;
	virtual int                   SentenceGroupPick( int groupIndex, char* name, int nameBufLen ) = 0;
	virtual int                   SentenceGroupPickSequential( int groupIndex, char* name, int nameBufLen, int sentenceIndex, int reset ) = 0;
	virtual int                   SentenceIndexFromName( const char* pSentenceName ) = 0;
	virtual const char* SentenceNameFromIndex( int sentenceIndex ) = 0;
	virtual int                   SentenceGroupIndexFromName( const char* pGroupName ) = 0;
	virtual const char* SentenceGroupNameFromIndex( int groupIndex ) = 0;
	virtual float                 SentenceLength( int sentenceIndex ) = 0;
	virtual void                  ComputeLighting( const Vector& pt, const Vector* pNormal, bool bClamp, Vector& color, Vector* pBoxColors = nullptr ) = 0;
	virtual void                  ActivateOccluder( int nOccluderIndex, bool bActive ) = 0;
	virtual bool                  IsOccluded( const Vector& vecAbsMins, const Vector& vecAbsMaxs ) = 0; // 74
	virtual int                   GetOcclusionViewId( void ) = 0;
	virtual void* SaveAllocMemory( size_t num, size_t size ) = 0;
	virtual void                  SaveFreeMemory( void* pSaveMem ) = 0;
	virtual INetChannelInfo* GetNetChannelInfo( void ) = 0;
	virtual void                  DebugDrawPhysCollide( const CPhysCollide* pCollide, IMaterial* pMaterial, const matrix3x4_t& transform, const Color& color ) = 0; //79
	virtual void                  CheckPoint( const char* pName ) = 0; // 80
	virtual void                  DrawPortals() = 0;
	virtual bool                  IsPlayingDemo( void ) = 0;
	virtual bool                  IsRecordingDemo( void ) = 0;
	virtual bool                  IsPlayingTimeDemo( void ) = 0;
	virtual int                   GetDemoRecordingTick( void ) = 0;
	virtual int                   GetDemoPlaybackTick( void ) = 0;
	virtual int                   GetDemoPlaybackStartTick( void ) = 0;
	virtual float                 GetDemoPlaybackTimeScale( void ) = 0;
	virtual int                   GetDemoPlaybackTotalTicks( void ) = 0;
	virtual bool                  IsPaused( void ) = 0; // 90
	virtual float                 GetTimescale( void ) const = 0;
	virtual bool                  IsTakingScreenshot( void ) = 0;
	virtual bool                  IsHLTV( void ) = 0;
	virtual bool                  IsLevelMainMenuBackground( void ) = 0;
	virtual void                  GetMainMenuBackgroundName( char* dest, int destlen ) = 0;
	virtual void                  SetOcclusionParameters( const int /*OcclusionParams_t*/& params ) = 0; // 96
	virtual void                  GetUILanguage( char* dest, int destlen ) = 0;
	virtual int                   IsSkyboxVisibleFromPoint( const Vector& vecPoint ) = 0;
	virtual const char* GetMapEntitiesString() = 0;
	virtual bool                  IsInEditMode( void ) = 0; // 100
	virtual float                 GetScreenAspectRatio( int viewportWidth, int viewportHeight ) = 0;
	virtual bool                  REMOVED_SteamRefreshLogin( const char* password, bool isSecure ) = 0; // 100
	virtual bool                  REMOVED_SteamProcessCall( bool& finished ) = 0;
	virtual unsigned int          GetEngineBuildNumber() = 0; // engines build
	virtual const char* GetProductVersionString() = 0; // mods version number (steam.inf)
	virtual void                  GrabPreColorCorrectedFrame( int x, int y, int width, int height ) = 0;
	virtual bool                  IsHammerRunning() const = 0;
	virtual void                  ExecuteClientCmd( const char* szCmdString ) = 0; //108
	virtual bool                  MapHasHDRLighting( void ) = 0;
	virtual bool                  MapHasLightMapAlphaData( void ) = 0;
	virtual int                   GetAppID() = 0;
	virtual Vector                GetLightForPointFast( const Vector& pos, bool bClamp ) = 0;
	virtual void                  ClientCmd_Unrestricted1( char  const*, int, bool );
	virtual void                  ClientCmd_Unrestricted( const char* szCmdString, const char* newFlag = 0 ) = 0;
	virtual void                  SetRestrictServerCommands( bool bRestrict ) = 0;
	virtual void                  SetRestrictClientCommands( bool bRestrict ) = 0;
	virtual void                  SetOverlayBindProxy( int iOverlayID, void* pBindProxy ) = 0;
	virtual bool                  CopyFrameBufferToMaterial( const char* pMaterialName ) = 0;
	virtual void                  ReadConfiguration( const int iController, const bool readDefault ) = 0;
	virtual void                  SetAchievementMgr( IAchievementMgr* pAchievementMgr ) = 0;
	virtual IAchievementMgr* GetAchievementMgr() = 0;
	virtual bool                  MapLoadFailed( void ) = 0;
	virtual void                  SetMapLoadFailed( bool bState ) = 0;
	virtual bool                  IsLowViolence() = 0;
	virtual const char* GetMostRecentSaveGame( void ) = 0;
	virtual void                  SetMostRecentSaveGame( const char* lpszFilename ) = 0;
	virtual void                  StartXboxExitingProcess() = 0;
	virtual bool                  IsSaveInProgress() = 0;
	virtual bool                  IsAutoSaveDangerousInProgress( void ) = 0;
	virtual unsigned int          OnStorageDeviceAttached( int iController ) = 0;
	virtual void                  OnStorageDeviceDetached( int iController ) = 0;
	virtual char* const           GetSaveDirName( void ) = 0;
	virtual void                  WriteScreenshot( const char* pFilename ) = 0;
	virtual void                  ResetDemoInterpolation( void ) = 0;
	virtual int                   GetActiveSplitScreenPlayerSlot() = 0;
	virtual int                   SetActiveSplitScreenPlayerSlot( int slot ) = 0;
	virtual bool                  SetLocalPlayerIsResolvable( char const* pchContext, int nLine, bool bResolvable ) = 0;
	virtual bool                  IsLocalPlayerResolvable() = 0;
	virtual int                   GetSplitScreenPlayer( int nSlot ) = 0;
	virtual bool                  IsSplitScreenActive() = 0;
	virtual bool                  IsValidSplitScreenSlot( int nSlot ) = 0;
	virtual int                   FirstValidSplitScreenSlot() = 0; // -1 == invalid
	virtual int                   NextValidSplitScreenSlot( int nPreviousSlot ) = 0; // -1 == invalid
	virtual ISPSharedMemory* GetSinglePlayerSharedMemorySpace( const char* szName, int ent_num = ( 1 << 11 ) ) = 0;
	virtual void                  ComputeLightingCube( const Vector& pt, bool bClamp, Vector* pBoxColors ) = 0;
	virtual void                  RegisterDemoCustomDataCallback( const char* szCallbackSaveID, pfnDemoCustomDataCallback pCallback ) = 0;
	virtual void                  RecordDemoCustomData( pfnDemoCustomDataCallback pCallback, const void* pData, size_t iDataLength ) = 0;
	virtual void                  SetPitchScale( float flPitchScale ) = 0;
	virtual float                 GetPitchScale( void ) = 0;
	virtual bool                  LoadFilmmaker() = 0;
	virtual void                  UnloadFilmmaker() = 0;
	virtual void                  SetLeafFlag( int nLeafIndex, int nFlagBits ) = 0;
	virtual void                  RecalculateBSPLeafFlags( void ) = 0;
	virtual bool                  DSPGetCurrentDASRoomNew( void ) = 0;
	virtual bool                  DSPGetCurrentDASRoomChanged( void ) = 0;
	virtual bool                  DSPGetCurrentDASRoomSkyAbove( void ) = 0;
	virtual float                 DSPGetCurrentDASRoomSkyPercent( void ) = 0;
	virtual void                  SetMixGroupOfCurrentMixer( const char* szgroupname, const char* szparam, float val, int setMixerType ) = 0;
	virtual int                   GetMixLayerIndex( const char* szmixlayername ) = 0;
	virtual void                  SetMixLayerLevel( int index, float level ) = 0;
	virtual int                   GetMixGroupIndex( char  const* groupname ) = 0;
	virtual void                  SetMixLayerTriggerFactor( int i1, int i2, float fl ) = 0;
	virtual void                  SetMixLayerTriggerFactor( char  const* char1, char  const* char2, float fl ) = 0;
	virtual bool                  IsCreatingReslist() = 0;
	virtual bool                  IsCreatingXboxReslist() = 0;
	virtual void                  SetTimescale( float flTimescale ) = 0;
	virtual void                  SetGamestatsData( CGamestatsData* pGamestatsData ) = 0;
	virtual CGamestatsData* GetGamestatsData() = 0;
	virtual void                  GetMouseDelta( int& dx, int& dy, bool b ) = 0; // unknown
	virtual   const char* k_LookupBindingEx( const char* pBinding, int iUserId = -1, int iStartCount = 0, int iAllowJoystick = -1 ) = 0;
	virtual int                   k_CodeForBinding( char  const*, int, int, int ) = 0;
	virtual void                  UpdateDAndELights( void ) = 0;
	virtual int                   GetBugSubmissionCount() const = 0;
	virtual void                  ClearBugSubmissionCount() = 0;
	virtual bool                  DoesLevelContainWater() const = 0;
	virtual float                 GetServerSimulationFrameTime() const = 0;
	virtual void                  SolidMoved( class IClientEntity* pSolidEnt, class ICollideable* pSolidCollide, const Vector* pPrevAbsOrigin, bool accurateBboxTriggerChecks ) = 0;
	virtual void                  TriggerMoved( class IClientEntity* pTriggerEnt, bool accurateBboxTriggerChecks ) = 0;
	virtual void                  ComputeLeavesConnected( const Vector& vecOrigin, int nCount, const int* pLeafIndices, bool* pIsConnected ) = 0;
	virtual bool                  IsInCommentaryMode( void ) = 0;
	virtual void                  SetBlurFade( float amount ) = 0;
	virtual bool                  IsTransitioningToLoad() = 0;
	virtual void                  SearchPathsChangedAfterInstall() = 0;
	virtual void                  ConfigureSystemLevel( int nCPULevel, int nGPULevel ) = 0;
	virtual void                  SetConnectionPassword( char const* pchCurrentPW ) = 0;
	virtual CSteamAPIContext* GetSteamAPIContext() = 0;
	virtual void                  SubmitStatRecord( char const* szMapName, unsigned int uiBlobVersion, unsigned int uiBlobSize, const void* pvBlob ) = 0;
	virtual void                  ServerCmdKeyValues( void* pKeyValues ) = 0; // 203
	virtual void                  SpherePaintSurface( const model_t* model, const Vector& location, unsigned char chr, float fl1, float fl2 ) = 0;
	virtual bool                  HasPaintmap( void ) = 0;
	virtual void                  EnablePaintmapRender() = 0;
	//virtual void                TracePaintSurface( const model_t *model, const Vector& position, float radius, CUtlVector<Color>& surfColors ) = 0;
	virtual void                  SphereTracePaintSurface( const model_t* model, const Vector& position, const Vector& vec2, float radius, /*CUtlVector<unsigned char, CUtlMemory<unsigned char, int>>*/ int& utilVecShit ) = 0;
	virtual void                  RemoveAllPaint() = 0;
	virtual void                  PaintAllSurfaces( unsigned char uchr ) = 0;
	virtual void                  RemovePaint( const model_t* model ) = 0;
	virtual bool                  IsActiveApp() = 0;
	virtual bool                  IsClientLocalToActiveServer() = 0;
	virtual void                  TickProgressBar() = 0;
	virtual InputContextHandle_t  GetInputContext( int /*EngineInputContextId_t*/ id ) = 0;
	virtual void                  GetStartupImage( char* filename, int size ) = 0;
	virtual bool                  IsUsingLocalNetworkBackdoor( void ) = 0;
	virtual void                  SaveGame( const char*, bool, char*, int, char*, int ) = 0;
	virtual void                  GetGenericMemoryStats(/* GenericMemoryStat_t */ void** ) = 0;
	virtual bool                  GameHasShutdownAndFlushedMemory( void ) = 0;
	virtual int                   GetLastAcknowledgedCommand( void ) = 0;
	virtual void                  FinishContainerWrites( int i ) = 0;
	virtual void                  FinishAsyncSave( void ) = 0;
	virtual int                   GetServerTick( void ) = 0;
	virtual const char* GetModDirectory( void ) = 0;
	virtual bool                  AudioLanguageChanged( void ) = 0;
	virtual bool                  IsAutoSaveInProgress( void ) = 0;
	virtual void                  StartLoadingScreenForCommand( const char* command ) = 0;
	virtual void                  StartLoadingScreenForKeyValues( void* values ) = 0;
	virtual void                  SOSSetOpvarFloat( const char*, float ) = 0;
	virtual void                  SOSGetOpvarFloat( const char*, float& ) = 0;
	virtual bool                  IsSubscribedMap( const char*, bool ) = 0;
	virtual bool                  IsFeaturedMap( const char*, bool ) = 0;
	virtual void                  GetDemoPlaybackParameters( void ) = 0;
	virtual int                   GetClientVersion( void ) = 0;
	virtual bool                  IsDemoSkipping( void ) = 0;
	virtual void                  SetDemoImportantEventData( const void* values ) = 0;
	virtual void                  ClearEvents( void ) = 0;
	virtual int                   GetSafeZoneXMin( void ) = 0;
	virtual bool                  IsVoiceRecording( void ) = 0;
	virtual void                  ForceVoiceRecordOn( void ) = 0;
	virtual bool                  IsReplay( void ) = 0;
};
