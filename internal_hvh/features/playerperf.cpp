#include "../include_cheat.h"
#include <deque>

void split_string( std::vector<std::string>& ret, std::string s )
{
	int start = 0;
	int end = s.find( '\n' );
	while ( end != -1 )
	{
		ret.emplace_back( s.substr( start, end - start ) );
		start = end + 1;
		end = s.find( '\n', start );
	}
	ret.emplace_back( s.substr( start, end - start ) );
}

void playerperf::on_handle_siminfo( const player_siminfo_t& sim_info )
{
	if ( recv_tick != interfaces::globals()->tickcount )
	{
		while ( player_server_info.size() > 150 )
			player_server_info.pop_back();
		count = 0;
		pos = 0;
		recv_tick = interfaces::globals()->tickcount;
	}

	if ( std::ranges::find_if( player_server_info, [&] ( const player_serverinfo_t& elem ) { return elem.sim_info == sim_info; } ) != player_server_info.end() )
		return;

	player_server_info.insert( player_server_info.begin() + count++, { player_cmdinfo_t{}, sim_info } );
}

void playerperf::on_handle_cmdinfo( const player_cmdinfo_t& cmd_info )
{
	if ( pos >= count )
		return;
	
	player_server_info.at( pos++ ).cmd_info = cmd_info;
}

bool playerperf::handle_message( const char* str )
{
	auto strings = std::vector<std::string>{};
	split_string( strings, std::string( str ) );
	if ( strings.empty() )
		return false;

	auto found = false;
	for ( const auto& elem : strings )
	{
		_( fmt1, "%f %d %d %f %f %f [vel %f]" );
		_( fmt2, "%f %d %d" );

		player_siminfo_t sim_info{};
		player_cmdinfo_t cmd_info{};

		if ( sscanf_s( elem.c_str(), fmt1.c_str(), &sim_info.m_flTime, &sim_info.m_nNumCmds, &sim_info.m_nTicksCorrected,
			&sim_info.m_flFinalSimulationTime, &sim_info.m_flGameSimulationTime, &sim_info.m_flServerFrameTime, &sim_info.vel ) == 7 )
		{
			on_handle_siminfo( sim_info );
			found = true;
		}
		else if ( sscanf_s( elem.c_str(), fmt2.c_str(), &cmd_info.m_flTime, &cmd_info.m_nNumCmds, &cmd_info.m_nDroppedPackets ) == 3 )
		{
			on_handle_cmdinfo( cmd_info );
			found = true;
		}
	}

	return found;
}