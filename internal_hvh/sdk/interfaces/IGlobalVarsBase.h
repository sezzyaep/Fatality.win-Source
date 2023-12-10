#pragma once

class IGlobalVarsBase
{
public:
	float				realtime;
	int					framecount;
	float				absoluteframetime;
	float				absoluteframestarttimestddev;
	float				curtime;
	float				frametime;
	int					maxClients;
	int					tickcount;
	float				interval_per_tick;
	float				interpolation_at;
	int					simTicksThisFrame;
	int					network_protocol;
	void*				pSaveData;
	bool				m_bClient;
	bool				m_bRemoteClient;
	int					nTimestampNetworkingBase;
	int					nTimestampRandomizeWindow;

	float get_realtime() const { return realtime; }
	int get_framecount() const { return framecount; }
	float get_curtime() const { return curtime; }
	float get_frametime() const { if ( frametime <= 0.f ) return 0.0001f; return frametime; }
	int get_tickcount() const { return tickcount; }
	float get_ipt() const { return interval_per_tick; }
};