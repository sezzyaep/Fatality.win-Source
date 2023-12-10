#pragma once

typedef enum
{
	NA_NULL = 0,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
} netadrtype_t;

struct netadr_t
{
	netadrtype_t	type;
	unsigned char	ip[ 4 ];
	unsigned short	port;
};

class CBitBuffer
{
public:
	char const * m_pDebugName;
	bool m_bOverflow;
	int m_nDataBits;
	size_t m_nDataBytes;
};

class bf_read : public CBitBuffer
{
public:

	
	uint32 m_nInBufWord;
	int m_nBitsAvail;
	uint32 const *m_pDataIn;
	uint32 const *m_pBufferEnd;
	uint32 const *m_pData;
};

typedef struct netpacket_s netpacket_t;
enum NetworkSystemAddressType_t
{
	NSAT_NETADR,
	NSAT_P2P,
	NSAT_PROXIED_GAMESERVER,	// Client proxied through Steam Datagram Transport
	NSAT_PROXIED_CLIENT,		// Client proxied through Steam Datagram Transport
};

enum EUniverse
{
	k_EUniverseInvalid = 0,
	k_EUniversePublic = 1,
	k_EUniverseBeta = 2,
	k_EUniverseInternal = 3,
	k_EUniverseDev = 4,
	// k_EUniverseRC = 5,				// no such universe anymore
	k_EUniverseMax
};

class CSteamID
{
public:
	// 64 bits total
	union SteamID_t
	{
		struct SteamIDComponent_t
		{
			uint32				m_unAccountID : 32;			// unique account identifier
			unsigned int		m_unAccountInstance : 20;	// dynamic instance ID
			unsigned int		m_EAccountType : 4;			// type of account - can't show as EAccountType, due to signed / unsigned difference
			EUniverse			m_EUniverse : 8;	// universe this account belongs to
		} m_comp;

		uint64 m_unAll64Bits;
	} m_steamid;
};

enum PeerToPeerAddressType_t
{
	P2P_STEAMID,
};

class CPeerToPeerAddress
{
public:
	CSteamID m_steamID;
	int m_steamChannel; // SteamID channel (like a port number to disambiguate multiple connections)

	PeerToPeerAddressType_t m_AddrType;
};

struct ns_address
{
	netadr_t m_adr; // ip:port and network type (NULL/IP/BROADCAST/etc).
	CPeerToPeerAddress m_steamID; // SteamID destination
	NetworkSystemAddressType_t m_AddrType;
};

typedef struct netpacket_s
{
	char		from[32];		// sender address
	int				source;		// received source 
	double			received;	// received time
	unsigned char	*data;		// pointer to raw packet data
	bf_read			message;	// easy bitbuf data access
	int				size;		// size in bytes
	int				wiresize;   // size in bytes before decompression
	bool			stream;		// was send as stream
	struct netpacket_s *pNext;	// for internal use, should be NULL in public
} netpacket_t;

class CNetChannel
{
public:
	__int32 vtable; //0x0000 
	void* msgbinder1; //0x0004 
	void* msgbinder2;
	void* msgbinder3;
	void* msgbinder4;
	unsigned char m_bProcessingMessages;
	unsigned char m_bShouldDelete;
	char pad_0x0016[ 0x2 ];
	__int32 m_nOutSequenceNr;
	__int32 m_nInSequenceNr;
	__int32 m_nOutSequenceNrAck;
	__int32 m_nOutReliableState;
	__int32 m_nInReliableState;
	__int32 m_nChokedPackets;
	char pad_0030[ 112 ]; //0x0030
	int m_Socket; //0x009C
	int m_StreamSocket; //0x00A0
	int m_MaxReliablePayloadSize; //0x00A4
	char remote_address[ 32 ]; //0x00A8
	char m_szRemoteAddressName[ 64 ]; //0x00A8
	float last_received; //0x010C
	float connect_time; //0x0110
	char pad_0114[ 4 ]; //0x0114
	int m_Rate; //0x0118
	char pad_011C[ 4 ]; //0x011C
	float m_fClearTime; //0x0120
	char pad_0124[ 16688 ]; //0x0124
	char m_Name[ 32 ]; //0x4254
	unsigned int m_ChallengeNr; //0x4274
	float m_flTimeout; //0x4278
	char pad_427C[ 32 ]; //0x427C
	float m_flInterpolationAmount; //0x429C
	float m_flRemoteFrameTime; //0x42A0
	float m_flRemoteFrameTimeStdDeviation; //0x42A4
	int m_nMaxRoutablePayloadSize; //0x42A8
	int m_nSplitPacketSequence; //0x42AC
	char pad_42B0[ 40 ]; //0x42B0
	bool m_bIsValveDS; //0x42D8
	char pad_42D9[ 65 ]; //0x42D9

	VFUNC( 40, SendNetMsg( void* msg, bool bForceReliable = false, bool bVoice = false ), bool( __thiscall* )( void*, void*, bool, bool ) )( msg, bForceReliable, bVoice )
		VFUNC( 46, SendDatagram(), int( __thiscall* )( CNetChannel*, uint32_t* ) )( nullptr )
		VFUNC( 47, Transmit( bool onlyreliable = false ), bool( __thiscall* )( void*, bool ) )( onlyreliable )
		VFUNC( 59, has_pending_reliable_data(), bool( __thiscall* )( class CNetChannel* ) )( )
};