#pragma once

class Color;
class C_CSPlayerResource : C_BaseEntity
{
public:
	DATAMAPVARRS( get_ping, int, "m_iPing" )
		NETVAR( get_c4_player, int, CCSPlayerResource_m_iPlayerC4 )
		DATAMAPVARRS( get_kills, int, "m_iKills" )
		DATAMAPVARRS( get_health, int, "m_iHealth" )
		NETVARRS( get_clan, const char, CCSPlayerResource_m_szClan )
		NETVARRS( get_crosshair_code, const char, CCSPlayerResource_m_szCrosshairCodes )
};