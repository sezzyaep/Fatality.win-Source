#include "../include_cheat.h"

void listeners::event_listeners::FireGameEvent( IGameEvent* game_event )
{
	record_shot_info_t shot_info = {};
	const auto event_name = fnv1a_rt( game_event->GetName() );

	if ( event_name == FNV1A( "player_hurt" ) )
	{
		resolver::hurt_listener( game_event, shot_info );
		hitmarker::listener( game_event );
	}
	else if ( event_name == FNV1A( "bullet_impact" ) )
	{
		resolver::listener( game_event );
		beams::listener( game_event );
	}
	else if ( event_name == FNV1A( "weapon_fire" ) )
	{
		resolver::listener( game_event );
	}
	else if ( event_name == FNV1A( "game_newmap" ) )
	{
		chams::m_rerun_nightmode = true;
	}
	else if ( event_name == FNV1A( "grenade_thrown" ) )
	{
		grenade_pred::register_grenade( game_event );
	}
	else if ( event_name == FNV1A( "item_equip" ) )
	{
		sound_esp::on_item_equip( game_event );
	}

	console_log::listener( game_event, shot_info );

	const std::string name = XOR_STR( "on_" ) + std::string( game_event->GetName() );
	lua::api.create_callback( name.c_str() );

	const auto cbk = [game_event]( lua::state& s ) -> int {
		s.create_user_object_ptr( XOR_STR( "csgo.event" ), game_event );
		return 1;
	};

	lua::api.callback( FNV1A( "on_game_event" ), cbk );
	lua::api.callback( fnv1a_rt( name.c_str() ), cbk );
}