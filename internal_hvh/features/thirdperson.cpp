#include "../include_cheat.h"

float BezierBlend2( float t )
{
	return t * t * ( 3.0f - 2.0f * t );
}

[[clang::optnone]] void thirdperson::run()
{
#ifndef NO_NETOFFSETS

	VIRTUALIZER_LION_WHITE_START

	if ( !interfaces::engine()->IsInGame() || !local_player )
		return;

	if ( util::get_current_tick() > util::last_heartbeat_tick.load() + 60000u )
	{
		log( "connection failure\n" );
#ifdef ALPHA
		util::print_log( std::to_string( util::get_current_tick() - util::last_heartbeat_tick ).c_str() );
		log( "\n" );
#endif
		util::crash();
}

	VIRTUALIZER_LION_WHITE_END
#else
	if ( !interfaces::engine()->IsInGame() || !local_player )
		return;
#endif

	const auto weapon = local_weapon;

	static auto progress = 0.f;
	if ( local_player->get_alive() && vars::misc.thirdperson->get<bool>() && ( !vars::misc.thirdperson_grenade->get<bool>() || !weapon || !weapon->is_grenade() ) )
	{
		interfaces::input()->m_fCameraInThirdPerson = true;

		progress = clamp( progress + interfaces::globals()->frametime * 6.f, 40.f / vars::misc.thirdperson_dist->get<float>(), 1.f );

		var( cam_idealdist );
		var( cam_collision );
		var( cam_snapto );

		cam_idealdist->set_value( vars::misc.thirdperson_dist->get<float>() * ( vars::visuals.disable_thirdperson_interp->get<bool>() ? 1.f : BezierBlend2( progress ) ) );
		cam_collision->set_value( 1 );
		cam_snapto->set_value( 1 );
	}
	else if ( interfaces::input()->m_fCameraInThirdPerson )
	{
		progress = vars::misc.thirdperson->get<bool>() ? 1.f : 0.f;
		interfaces::input()->m_fCameraInThirdPerson = false;
	}

	static auto b_once = false;

	if ( local_player->get_alive() || !vars::misc.thirdperson_dead->get<bool>() )
	{
		b_once = false;
		return;
	}

	if ( b_once )
		local_player->get_observer_mode() = 5;

	if ( local_player->get_observer_mode() == 4 )
		b_once = true;
}
