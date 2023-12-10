#pragma once

enum ENetworkStringtableFlags
{
	NSF_NONE = 0,
	NSF_DICTIONARY_ENABLED = ( 1 << 0 ), // Uses pre-calculated per map dictionaries to reduce bandwidth
};

const unsigned short INVALID_STRING_INDEX = ( unsigned short ) -1;

typedef int TABLEID;

class INetworkStringTable
{
public:

	virtual					~INetworkStringTable( void ) {};

	// Table Info
	virtual const char		*GetTableName( void ) const = 0;
	virtual TABLEID			GetTableId( void ) const = 0;
	virtual int				GetNumStrings( void ) const = 0;
	virtual int				GetMaxStrings( void ) const = 0;
	virtual int				GetEntryBits( void ) const = 0;

	// Networking
	virtual void			SetTick( int tick ) = 0;
	virtual bool			ChangedSinceTick( int tick ) const = 0;

	// Accessors (length -1 means don't change user data if string already exits)
	virtual int				AddString( bool bIsServer, const char *value, int length = -1, const void *userdata = 0 ) = 0;

	virtual const char		*GetString( int stringNumber ) const = 0;
	virtual void			SetStringUserData( int stringNumber, int length, const void *userdata ) = 0;
	virtual const void		*GetStringUserData( int stringNumber, int *length ) const = 0;
	virtual int				FindStringIndex( char const *string ) = 0; // returns INVALID_STRING_INDEX if not found

	// Callbacks
	virtual void			SetStringChangedCallback( void *object, void* changeFunc ) = 0;
};


class CNetworkStringTableContainer
{
public:
	virtual					~CNetworkStringTableContainer( void ) {};
	// Implement INetworkStringTableContainer
	virtual INetworkStringTable	*CreateStringTable( const char *tableName, int maxentries, int userdatafixedsize = 0, int userdatanetworkbits = 0, int flags = NSF_NONE ) const = 0;
	virtual void				RemoveAllTables( void ) const = 0;

	// table infos
	virtual INetworkStringTable	*FindTable( const char *tableName ) const = 0;
	virtual INetworkStringTable	*GetTable( TABLEID stringTable ) const = 0;
	virtual int					GetNumTables( void ) const = 0;

	bool		m_bAllowCreation;	// creat guard Guard
	int			m_nTickCount;		// current tick
	bool		m_bLocked;			// currently locked?
	bool		m_bEnableRollback;	// enables rollback feature

	CUtlVector < void* > m_Tables;
};