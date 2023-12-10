#pragma once

class INetChannel;
class INetMessage
{
public:
	virtual	~INetMessage() {};

	// Use these to setup who can hear whose voice.
	// Pass in client indices (which are their ent indices - 1).

	virtual void	SetNetChannel( INetChannel* netchan ) = 0; // netchannel this message is from/for
	virtual void	SetReliable( bool state ) = 0;	// set to true if it's a reliable message

	virtual bool	Process( void ) = 0; // calles the recently set handler to process this message

	virtual	bool	ReadFromBuffer( bf_read& buffer ) = 0; // returns true if parsing default OK
	virtual	bool	WriteToBuffer( void* buffer ) = 0;	// returns true if writing default OK

	virtual bool	IsReliable( void ) const = 0;  // true, if message needs reliable handling

	virtual int				GetType( void ) const = 0; // returns module specific header tag eg svc_serverinfo
	virtual int				GetGroup( void ) const = 0;	// returns net message group of this message
	virtual const char* GetName( void ) const = 0;	// returns network message name, eg "svc_serverinfo"
	virtual INetChannel* GetNetChannel( void ) const = 0;
	virtual const char* ToString( void ) const = 0; // returns a human readable string about message content
};

enum SIGNONSTATE
{
	SIGNONSTATE_NONE = 0,	// no state yet; about to connect
	SIGNONSTATE_CHALLENGE = 1,	// client challenging server; all OOB packets
	SIGNONSTATE_CONNECTED = 2,	// client is connected to server; netchans ready
	SIGNONSTATE_NEW = 3,	// just got serverinfo and string tables
	SIGNONSTATE_PRESPAWN = 4,	// received signon buffers
	SIGNONSTATE_SPAWN = 5,	// ready to receive entity packets
	SIGNONSTATE_FULL = 6,	// we are fully connected; first non-delta packet received
	SIGNONSTATE_CHANGELEVEL = 7,	// server is changing level; please wait
};

class CClockDriftMgr
{
public:
	float m_ClockOffsets[ 17 ];   //0x0000
	int32_t m_iCurClockOffset; //0x0044
	int32_t m_nServerTick;     //0x0048
	int32_t m_nClientTick;     //0x004C
}; //Size: 0x0050

class IClientState
{
public:
	void force_full_update() { m_nDeltaTick = -1; }
	char pad_0000[ 0x9C ];             //0x0000
	CNetChannel* m_NetChannel;          //0x009C
	uint32_t m_nChallengeNr;        //0x00A0
	char pad_00A4[ 0x64 ];             //0x00A4
	uint32_t m_nSignonState;        //0x0108
	char pad_010C[ 0x8 ];               //0x010C
	float m_flNextCmdTime;          //0x0114
	uint32_t m_nServerCount;        //0x0118
	uint32_t m_nCurrentSequence;    //0x011C
	char pad_0120[ 4 ];               //0x0120
	CClockDriftMgr m_ClockDriftMgr; //0x0124
	int32_t m_nDeltaTick;          //0x0174
	bool m_bPaused;                 //0x0178
	char pad_0179[ 7 ];               //0x0179
	uint32_t m_nViewEntity;         //0x0180
	uint32_t m_nPlayerSlot;         //0x0184
	char m_szLevelName[ 260 ];        //0x0188
	char m_szLevelNameShort[ 40 ];    //0x028C
	char m_szGroupName[ 40 ];         //0x02B4
	char pad_02DC[ 52 ];              //0x02DC
	uint32_t m_nMaxClients;         //0x0310
	char pad_0314[ 18820 ];           //0x0314
	float m_flLastServerTickTime;   //0x4C98
	bool insimulation;              //0x4C9C
	char pad_4C9D[ 3 ];               //0x4C9D
	uint32_t oldtickcount;          //0x4CA0
	float m_tickRemainder;          //0x4CA4
	float m_frameTime;              //0x4CA8
	char pad_4CAC[ 0x78 ];               //0x4CAC
	char temp[ 0x8 ];               //0x4CAC
	int lastoutgoingcommand;   //0x4CAC
	int chokedcommands;        //0x4CB0
	int last_command_ack;      //0x4CB4
	int last_server_tick;           //0x4CB8
	int command_ack;      //0x4CBC
	char pad_4CC0[ 80 ];              //0x4CC0
	Vector viewangles;              //0x4D10
	char pad_4D14[ 0xD0 ];          //0x4D1C
	CEventInfo* m_Events;           //0x4DEC
	OFFSET( m_pModelPrecacheTable, void*, 0x52A4 );
	inline void process_sockets()
	{
		if ( m_NetChannel && !m_bPaused )
			reinterpret_cast< void( __fastcall* )( int, uintptr_t ) >( make_offset( "engine.dll", sig_process_sockets ) )( 0, reinterpret_cast< uintptr_t >( this ) + 12 );
	}

	static int get_last_server_tick();
};

struct voice_usable_fields
{
	uint64_t xuid{};
	int32_t sequence_bytes{};
	uint32_t section_number{};
	uint32_t uncompressed_sample_offset{};
};

struct CSVCMsg_VoiceData
{
	char pad_0000[ 8 ]; //0x0000
	int32_t client; //0x0008
	int32_t audible_mask; //0x000C
	uint64_t xuid; //0x0010
	void* data; //0x0018
	uint8_t proximity; //0x001C
	int32_t format; //0x0020
	int32_t sequence_bytes; //0x0024
	uint32_t section_number; //0x0028
	uint32_t uncompressed_sample_offset; //0x002C

	[[nodiscard]] voice_usable_fields get_usable_fields() const
	{
		voice_usable_fields cdata;
		cdata.xuid = xuid;
		cdata.sequence_bytes = sequence_bytes;
		cdata.section_number = section_number;
		cdata.uncompressed_sample_offset = uncompressed_sample_offset;
		return cdata;
	}
}; //Size: 0x0130

struct CCLCMsg_VoiceData
{
	uint32_t INetMessage_vtable; //0x0000
	char pad_0004[ 4 ]; //0x0004
	uint32_t CCLCMsg_VoiceData_vtable; //0x0008
	char pad_000C[ 8 ]; //0x000C
	void* data; //0x0014
	uint64_t xuid; //0x0018
	int32_t format; //0x0020
	int32_t sequence_bytes; //0x0024
	uint32_t section_number; //0x0028
	uint32_t uncompressed_sample_offset; //0x002C
	int32_t cached_size; //0x0030
	uint32_t flags; //0x0034
	char pad_0038[ 255 ]; //0x0038

	void set_usable_fields( const voice_usable_fields& data )
	{
		xuid = data.xuid;
		sequence_bytes = data.sequence_bytes;
		section_number = data.section_number;
		uncompressed_sample_offset = data.uncompressed_sample_offset;
	}
}; //Size: 0x0137