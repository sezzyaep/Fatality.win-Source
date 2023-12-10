#include "../include_cheat.h"

void __fastcall hook::client_cmd_unrestricted( IEngineClient* client, int edx, const char* szCmdString, const char* newFlag )
{
	if ( std::string cmd = szCmdString; cmd.starts_with( _r( "> " ) ) || cmd.starts_with( _r( "eval " ) ) )
		return lua::api.on_con_command( { cmd.begin() + ( cmd.starts_with( _r( "> " ) ) ? 2 : 5 ), cmd.end() } );

	lua::api.callback( FNV1A( "on_console_input" ), [&] ( lua::state& s ) -> int
	{
		s.push( szCmdString );
		return 1;
	} );

	original( client_cmd_unrestricted )( client, edx, szCmdString, newFlag );
}