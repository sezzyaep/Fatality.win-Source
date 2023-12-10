#pragma once

namespace playerperf
{
	struct player_siminfo_t
	{
		bool operator==( const player_siminfo_t& ) const = default;

		float		m_flTime;
		// # of CUserCmds in this update
		int			m_nNumCmds;
		// If clock needed correction, # of ticks added/removed
		int			m_nTicksCorrected; // +ve or -ve
		// player's m_flSimulationTime at end of frame
		float		m_flFinalSimulationTime;
		float		m_flGameSimulationTime;
		// estimate of server perf
		float		m_flServerFrameTime;
		float		vel;
	};

	struct player_cmdinfo_t
	{
		bool operator==( const player_cmdinfo_t& ) const = default;

		// realtime of sample
		float		m_flTime;
		// # of CUserCmds in this update
		int			m_nNumCmds;
		// # of dropped packets on the link
		int			m_nDroppedPackets;
	};

	struct player_serverinfo_t
	{
		player_cmdinfo_t cmd_info{};
		player_siminfo_t sim_info{};
	};

	void on_handle_siminfo( const player_siminfo_t& sim_info );
	void on_handle_cmdinfo( const player_cmdinfo_t& cmd_info );
	bool handle_message( const char* str );

	inline std::deque<player_serverinfo_t> player_server_info = {};
	inline int recv_tick = {};
	inline int count = {};
	inline int pos = {};
};