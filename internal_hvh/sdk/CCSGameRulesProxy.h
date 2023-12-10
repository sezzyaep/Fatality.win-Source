#pragma once

class C_CSGameRulesProxy
{
public:
		NETVAR( m_bFreezePeriod, bool, CCSGameRulesProxy_m_bFreezePeriod )
		NETVAR( m_bIsValveDS, bool, CCSGameRulesProxy_m_bIsValveDS )
		NETVAR( m_flSurvivalStartTime, float, CCSGameRulesProxy_m_flSurvivalStartTime )
		NETVAR( get_play_all_step_sounds_on_server, bool, CCSGameRulesProxy_m_bPlayAllStepSoundsOnServer )
		VFUNC( 28, ShouldCollide( int collisionGroup0, int collisionGroup1 ), bool( __thiscall* )( void*, int, int ) )( collisionGroup0, collisionGroup0 )
};