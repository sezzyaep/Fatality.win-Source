#include "../include_cheat.h"

std::string hitgroup_to_name( const int hitgroup )
{
	switch ( hitgroup )
	{
		case HITGROUP_HEAD:
			return _w( "head" );
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			return _w( "leg" );
		case HITGROUP_STOMACH:
			return _w( "stomach" );
		case HITGROUP_CHEST:
			return _w( "chest" );
		default:
			return _w( "body" );;
	}
}

void console_log::buy_event( IGameEvent* game_event )
{
	_( team, "team" );
	_( userid, "userid" );
	_( weapon_s, "weapon" );
	_( weapon_, "weapon_" );
	_( item_, "item_" );
	_( fatality, "[fatality] " );
	_( s, "%s " );
	_( bought, "bought " );
	_( sn, "%s\n" );

	if ( !local_player || local_player->get_team() == game_event->GetInt( team.c_str() ) )
		return;

	const auto index = interfaces::engine()->GetPlayerForUserID( game_event->GetInt( userid.c_str() ) );

	player_info_t pinfo;
	interfaces::engine()->GetPlayerInfo( index, &pinfo );

	std::string weapon( game_event->GetString( weapon_s.c_str() ) );
	if ( weapon.find( weapon_ ) != std::string::npos )
		weapon.replace( 0, 7, "" );
	else if ( weapon.find( item_ ) != std::string::npos )
		weapon.replace( 0, 5, "" );

	if ( fnv1a_rt( weapon ) == FNV1A( "unknown" ) )
		return;

	interfaces::cvar()->ConsoleColorPrintf( Color( 235, 5, 90, 255 ), fatality.c_str() );
	util::print_dev_console( true, Color( 173, 216, 230, 255 ), s.c_str(), pinfo.name );
	util::print_dev_console( true, Color( 255, 255, 255, 255 ), bought.c_str() );
	util::print_dev_console( true, Color( 173, 216, 230, 255 ), sn.c_str(), weapon.c_str() );
}

void console_log::hurt_event( IGameEvent* game_event, record_shot_info_t& shot_info )
{
	_( attacker_s, "attacker" );
	_( userid, "userid" );
	_( dmg_health, "dmg_health" );

	const auto attacker = interfaces::engine()->GetPlayerForUserID( game_event->GetInt( attacker_s.c_str() ) );
	const auto victim = interfaces::engine()->GetPlayerForUserID( game_event->GetInt( userid.c_str() ) );
	if ( attacker == interfaces::engine()->GetLocalPlayer() )
	{
		auto damage = game_event->GetInt( dmg_health.c_str() );
		damage = clamp( damage, 0, 100 );
		const int green = 255 - damage * 2.55;
		const int red = damage * 2.55;

		player_info_t pinfo;
		interfaces::engine()->GetPlayerInfo( victim, &pinfo );

		_( fatality, "[fatality] " );
		_( d, "%d " );
		_( f, "%.0f " );
		_( in, "in " );
		_( hp, "hp " );
		_( s, "%s" );
		_( hitgroup, "hitgroup" );
		_( for_s, " for " );
		_( sn, "%s" );
		_( hit, "hit " );
		_u( spot, hitgroup_to_name( game_event->GetInt( hitgroup.c_str() ) ) );

		interfaces::cvar()->ConsoleColorPrintf( Color( 235, 5, 90, 255 ), fatality.c_str() );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), hit.c_str() );
		util::print_dev_console( true, Color( 173, 216, 230, 255 ), sn.c_str(), pinfo.name );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), for_s.c_str() );
		util::print_dev_console( true, Color( red, green, 0, 255 ), d.c_str(), game_event->GetInt( dmg_health.c_str() ) );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), hp.c_str() );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), in.c_str() );
		util::print_dev_console( true, Color( 173, 216, 230, 255 ), s.c_str(), spot.c_str() );


		if ( vars::misc.hurtlog_info->get<bool>() && shot_info.has_info )
		{
			_( one, " - " );
			_( hc, "hitchance " );
			_( dmg, "target dmg " );
			_( bt, "backtrack ticks " );

			interfaces::cvar()->ConsoleColorPrintf( Color( 235, 5, 90, 255 ), one.c_str() );
			interfaces::cvar()->ConsoleColorPrintf( Color( 173, 216, 230, 255 ), hc.c_str() );
			interfaces::cvar()->ConsoleColorPrintf( Color( 255, 255, 255, 255 ), f.c_str(), shot_info.hitchance );
			interfaces::cvar()->ConsoleColorPrintf( Color( 173, 216, 230, 255 ), dmg.c_str() );
			interfaces::cvar()->ConsoleColorPrintf( Color( 255, 255, 255, 255 ), f.c_str(), shot_info.target_dmg );
			interfaces::cvar()->ConsoleColorPrintf( Color( 173, 216, 230, 255 ), bt.c_str() );
			interfaces::cvar()->ConsoleColorPrintf( Color( 255, 255, 255, 255 ), d.c_str(), shot_info.backtrack_ticks );

#ifdef ALPHA
			if ( !shot_info.extra_info.empty() )
				interfaces::cvar()->ConsoleColorPrintf( Color( 180, 180, 180, 255 ), shot_info.extra_info.c_str() );
#endif
		}

		_( nl, "\n" );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), nl.c_str() );
	}
	else if ( victim == interfaces::engine()->GetLocalPlayer() )
	{
		auto damage = game_event->GetInt( dmg_health.c_str() );
		damage = clamp( damage, 0, 100 );
		const int green = 255 - damage * 2.55;
		const int red = damage * 2.55;

		player_info_t pinfo;
		interfaces::engine()->GetPlayerInfo( attacker, &pinfo );

		_( fatality, "[fatality] " );
		_( d, "%d " );
		_( in, "in " );
		_( s, "%s " );
		_( hitgroup, "hitgroup" );
		_( by, "by " );
		_( sn, "%s\n" );
		_( hp, "hp " );
		_( hurt, "hurt for " );
		_u( spot, hitgroup_to_name( game_event->GetInt( hitgroup.c_str() ) ) );

		interfaces::cvar()->ConsoleColorPrintf( Color( 235, 5, 90, 255 ), fatality.c_str() );
		interfaces::cvar()->ConsoleColorPrintf( Color( 255, 255, 255, 255 ), hurt.c_str() );
		interfaces::cvar()->ConsoleColorPrintf( Color( red, green, 0, 255 ), d.c_str(), game_event->GetInt( dmg_health.c_str() ) );
		interfaces::cvar()->ConsoleColorPrintf( Color( 255, 255, 255, 255 ), hp.c_str() );
		interfaces::cvar()->ConsoleColorPrintf( Color( 255, 255, 255, 255 ), in.c_str() );
		interfaces::cvar()->ConsoleColorPrintf( Color( 173, 216, 230, 255 ), s.c_str(), spot.c_str() );
		interfaces::cvar()->ConsoleColorPrintf( Color( 255, 255, 255, 255 ), by.c_str() );
		interfaces::cvar()->ConsoleColorPrintf( Color( 173, 216, 230, 255 ), sn.c_str(), pinfo.name );
	}

}

void console_log::listener( IGameEvent* game_event, record_shot_info_t& shot_info )
{
	const auto event_name = fnv1a_rt( game_event->GetName() );

	if ( vars::misc.hurtlog->get<bool>() && event_name == FNV1A( "player_hurt" ) )
		hurt_event( game_event, shot_info );
	else if ( vars::misc.buylog->get<bool>() && event_name == FNV1A( "item_purchase" ) )
		buy_event( game_event );
}
