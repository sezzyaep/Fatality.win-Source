#pragma once

class IAppSystem2
{
public:
	virtual void func0() = 0;
	virtual void func1() = 0;
	virtual void func2() = 0;
	virtual void func3() = 0;
	virtual void func4() = 0;
	virtual void func5() = 0;
	virtual void func6() = 0;
	virtual void func7() = 0;
	virtual void func8() = 0;
	virtual void func9() = 0;
};

struct CVarDLLIdentifier_t;

class ConVar;
class ICVar : public IAppSystem2
{
public:
	virtual void			func10() = 0;
	virtual void			RegisterConCommand( ConVar *pCommandBase ) = 0;
	virtual void			UnregisterConCommand( ConVar *pCommandBase ) = 0;
	virtual void			func13() = 0;
	virtual ConVar			*FindVar( const char *var_name ) = 0;

	template <typename... Values>
	void ConsoleColorPrintf( const Color& MsgColor, const char* szMsgFormat, Values... Parameters )
	{
		typedef void( *oConsoleColorPrintf )( void*, const Color&, const char*, ... );
		return util::get_vfunc<oConsoleColorPrintf>( this, 25 )( this, MsgColor, szMsgFormat, Parameters... );
	}
};

class ConCommand
{
public:
    enum
    {
        COMMAND_MAX_ARGC = 64,
        COMMAND_MAX_LENGTH = 512,
    };

    int        m_nArgc;
    int        m_nArgv0Size;
    char    m_pArgSBuffer[ COMMAND_MAX_LENGTH ]{};
    char    m_pArgvBuffer[ COMMAND_MAX_LENGTH ]{};
    const char* m_ppArgv[ COMMAND_MAX_ARGC ]{};

    explicit ConCommand( const char* command )
    {
        strcpy_s( m_pArgSBuffer, command );
        strcpy_s( m_pArgvBuffer, command );

        m_nArgc = 1;
        m_ppArgv[ 0 ] = m_pArgvBuffer;

        for ( size_t i = 0; i < strlen( m_pArgSBuffer ); i++ )
        {
            if ( m_pArgvBuffer[ i ] == ' ' )
            {
                m_pArgvBuffer[ i ] = 0;
                m_ppArgv[ m_nArgc ] = &m_pArgvBuffer[ i + 1 ]; //dont ever call that with a space at the end.
                m_nArgc++;
            }
        }
        m_nArgv0Size = strlen( m_pArgvBuffer );
    }
};