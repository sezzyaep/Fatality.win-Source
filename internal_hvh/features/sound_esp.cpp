#include "../include_cheat.h"

void sound_esp::update()
{
	for ( auto i = 1; i <= interfaces::globals()->maxClients; i++ )
	{
		const auto player = globals::get_player( i );
		auto& cur = players[ i ];
		cur.index = i;
		cur.player = player;
		cur.valid = false;

		if ( !player || i == interfaces::engine()->GetLocalPlayer() )
			continue;

		cur.valid = true;
		cur.valid_origin = false;

		if ( !player->IsDormant() )
		{
			cur.lerp_start = 0.f;
			cur.lerped_origin = cur.origin = player->get_abs_origin();
			cur.valid_origin = true;
			cur.last_update_tick = interfaces::client_state()->get_last_server_tick();
		}
	}
}

void sound_esp::update_origin( sound_data_t& player, const Vector& origin )
{
	if ( origin.Length2DSqr() < 1.f )
		return;

	if ( player.valid_origin )
		return;

	if ( player.observerindex )
	{
		const auto observer = globals::get_player( player.observerindex );
		if ( observer && observer->GetClientClass()->m_ClassID == ClassId::CCSPlayer && observer->get_observer() == player.player && ( observer->get_observer_mode() == OBS_MODE_IN_EYE || observer->get_observer_mode() == OBS_MODE_CHASE ) )
			return;

		player.observerindex = 0;
	}

	auto new_origin = origin;

	trace_t tr;
	Ray_t ray;
	ray.Init( origin + Vector( 0.f, 0.f, 10.f ), origin - Vector( 0.f, 0.f, 74.04f ) );
	CTraceFilterWorldOnly filter{};
	interfaces::engine_trace()->TraceRay( ray, MASK_PLAYERSOLID, &filter, &tr );
	if ( tr.fraction < 1.f )
		new_origin = tr.endpos;

	if ( vars::visuals.esp.enabled->get<bool>() && vars::visuals.esp.sound_esp->get<bool>() && player.player && player.player->is_enemy() )
		beams::add_beacon( new_origin );

	if ( player.last_update_tick == interfaces::client_state()->get_last_server_tick() )
		return;

	if ( player.valid_origin )
		return;

	if ( player.last_update_tick < interfaces::client_state()->get_last_server_tick() - 3 && vars::visuals.disable_dormancy_interp->get<bool>() )
	{
		player.lerp_start = 0.f;
		player.lerped_origin = new_origin;
	}

	player.last_update_tick = interfaces::client_state()->get_last_server_tick();

	if ( player.origin.Dist( new_origin ) > 0.2f )
		player.lerp_start = interfaces::globals()->realtime;

	player.origin = new_origin;

	if ( player.lerped_origin.IsZero() )
		player.lerped_origin = new_origin;

	player.updated = true;

	player_log::get_log( player.index ).last_server_tick = player.last_update_tick;
}

Vector sound_esp::get_position( const int index, bool lerp )
{
	if ( !INRANGE( index, 1, 64 ) )
		return {};

	if ( lerp )
	{
		if ( players[ index ].origin.Dist( players[ index ].lerped_origin ) > 800.f )
			players[ index ].lerped_origin = players[ index ].origin;

		players[ index ].lerped_origin = math::lerp( players[ index ].lerped_origin, players[ index ].origin, clamp( ( interfaces::globals()->realtime - players[ index ].lerp_start ) * 1.8f, 0.f, 1.f ) );

		return players[ index ].lerped_origin;
	}

	return players[ index ].origin;
}

bool sound_esp::get_last_update( const int index )
{
	const auto updated = players[ index ].updated;
	players[ index ].updated = false;

	return updated;
}

sound_data_t* sound_esp::get_sound_player( const int index )
{
	if ( !INRANGE( index, 1, 64 ) )
		return nullptr;

	return &players[ index ];
}

void sound_esp::on_start_sound( const start_sound_params* params, char* name )
{
	if ( !interfaces::engine()->IsInGame() )
		return;

	const auto ent = globals::get_player( params->sound_source );

	auto player = ent;
	if ( player && !player->is_player() )
	{
		const auto parent = player->GetShadowParent();
		const auto unknown = parent != nullptr ? parent->GetIClientUnknown() : nullptr;
		const auto owner = unknown != nullptr ? reinterpret_cast< C_CSPlayer* const >( unknown->GetIClientEntity() ) : nullptr;

		if ( owner && owner->is_player() )
			player = owner;
	}

	if ( !player || !player->is_player() )
		return;

	_( weapons, "weapons" );
	_( auto_semiauto_switch, "auto_semiauto_switch" );
	_( player_footsteps, "player\\footsteps" );
	_( player_land, "\\land" );

	const auto wpn = strstr( name, weapons.c_str() );
	auto origin = params->origin;

	const auto land = strstr( name, player_land.c_str() );
	if ( land )
		origin.z -= 32.f;

	if ( wpn || strstr( name, player_footsteps.c_str() ) || land )
	{
		const auto sound_player = get_sound_player( player->EntIndex() );
		if ( player != sound_player->player )
			return;

		if ( player->IsDormant() )
		{
			update_origin( *sound_player, origin );
			if ( wpn )
				player_log::get_log( player->EntIndex() ).m_dormant_misses = 0;
		}
		else if ( vars::visuals.esp.sound_esp->get<bool>() && player->is_enemy() )
		{
			auto new_origin = params->origin;

			trace_t tr;
			Ray_t ray;
			ray.Init( new_origin + Vector( 0.f, 0.f, 10.f ), new_origin - Vector( 0.f, 0.f, 74.04f ) );
			CTraceFilterWorldOnly filter{};
			interfaces::engine_trace()->TraceRay( ray, MASK_PLAYERSOLID, &filter, &tr );
			if ( tr.fraction < 1.f )
				new_origin = tr.endpos;

			beams::add_beacon( new_origin );
		}
	}
}

void sound_esp::on_item_equip( IGameEvent* game_event )
{
	_( userid, "userid" );
	_( defindex, "defindex" );
	const auto player_idx = interfaces::engine()->GetPlayerForUserID( game_event->GetInt( userid.c_str() ) );
	const auto weapon = game_event->GetInt( defindex.c_str() );

	const auto sound_player = get_sound_player( player_idx );
	if ( !sound_player )
		return;

	sound_player->weapon_id = weapon;
}