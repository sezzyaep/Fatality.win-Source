#include "../include_cheat.h"

void beams::draw_impacts()
{
	if ( !interfaces::engine()->IsInGame() || !local_player )
	{
		impacts.clear();
		return;
	}

	if ( impacts.size() > 30 )
		impacts.pop_back();

	_( modelname, "sprites/physbeam.vmt" );

	for ( auto& impact : impacts )
	{
		auto player = globals::get_player( impact.index );
		if ( !player || player->IsDormant() )
			continue;

		const auto width = vars::visuals.beams.width->get<float>();

		BeamInfo_t beam_info;
		beam_info.m_vecStart = impact.start;
		if ( player == local_player )
			beam_info.m_vecStart.z -= 2.f;
		beam_info.m_vecEnd = impact.pos;
		beam_info.m_nType = TE_BEAMPOINTS;
		beam_info.m_pszModelName = modelname.c_str();
		beam_info.m_nModelIndex = -1;
		beam_info.m_flHaloScale = 0.0f;
		beam_info.m_flLife = 2.f;
		beam_info.m_flWidth = width;
		beam_info.m_flEndWidth = width;
		beam_info.m_flFadeLength = 0.0f;
		beam_info.m_flAmplitude = 2.0f;
		beam_info.m_flBrightness = static_cast< float >( impact.color.a() );
		beam_info.m_flSpeed = 0.2f;
		beam_info.m_nStartFrame = 0;
		beam_info.m_flFrameRate = 0.f;
		beam_info.m_flRed = static_cast< float >( impact.color.r() );
		beam_info.m_flGreen = static_cast< float >( impact.color.g() );
		beam_info.m_flBlue = static_cast< float >( impact.color.b() );
		beam_info.m_nSegments = 2;
		beam_info.m_bRenderable = true;
		beam_info.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

		const auto beam = interfaces::view_render_beams()->CreateBeamPoints( beam_info );
		if ( beam )
			interfaces::view_render_beams()->DrawBeam( beam );
	}

	for ( auto& beacon : beacons )
	{
		BeamInfo_t beam_info;

		beam_info.m_nType = TE_BEAMRINGPOINT;
		beam_info.m_pszModelName = modelname.c_str();
		beam_info.m_nModelIndex = interfaces::model_info()->GetModelIndex( beam_info.m_pszModelName );
		beam_info.m_pszHaloName = modelname.c_str();
		beam_info.m_nHaloIndex = interfaces::model_info()->GetModelIndex( beam_info.m_pszHaloName );

		auto color = Color( vars::visuals.esp.sound_esp_color->get<D3DCOLOR>() );

		beam_info.m_flHaloScale = 5.f;
		beam_info.m_flLife = 1.f;
		beam_info.m_flWidth = 5.f;
		beam_info.m_flFadeLength = 1.0f;
		beam_info.m_flAmplitude = 0.f;
		beam_info.m_flBrightness = static_cast< float >( color.a() );
		beam_info.m_flSpeed = 0;
		beam_info.m_nStartFrame = 0;
		beam_info.m_flFrameRate = 1;
		beam_info.m_flRed = static_cast< float >( color.r() );
		beam_info.m_flGreen = static_cast< float >( color.g() );
		beam_info.m_flBlue = static_cast< float >( color.b() );
		beam_info.m_nSegments = 1;
		beam_info.m_bRenderable = true;
		beam_info.m_nFlags = 0;
		beam_info.m_vecCenter = beacon + Vector( 0.f, 0.f, beam_info.m_flHaloScale );
		beam_info.m_flStartRadius = 1.f;
		beam_info.m_flEndRadius = 250.f;

		const auto beam = interfaces::view_render_beams()->CreateBeamRingPoint( beam_info );
		if ( beam )
			interfaces::view_render_beams()->DrawBeam( beam );
	}

	beacons.clear();
	impacts.clear();
}

void beams::listener( IGameEvent* game_event )
{
	_( uid, "userid" );

	const auto userid = interfaces::engine()->GetPlayerForUserID( game_event->GetInt( uid.c_str() ) );

	static float last_curtime;

	const auto player = globals::get_player( userid );
	if ( !player )
		return;

	if ( player->IsDormant() )
		return;

	if ( player == local_player )
		return;

	if ( !vars::visuals.beams.enabled->get<bool>() )
		return;

	if ( !vars::visuals.beams.enemy.enabled->get<bool>() && player->is_enemy() )
		return;

	if ( !vars::visuals.beams.team.enabled->get<bool>() && !player->is_enemy() && player != local_player )
		return;

	_( x, "x" );
	_( y, "y" );
	_( z, "z" );

	const Vector pos( game_event->GetFloat( x.c_str() ), game_event->GetFloat( y.c_str() ), game_event->GetFloat( z.c_str() ) );
	Color col;

	if ( player->is_enemy() )
		col = Color( vars::visuals.beams.enemy.color->get<D3DCOLOR>() );
	else if ( !player->is_enemy() )
		col = Color( vars::visuals.beams.team.color->get<D3DCOLOR>() );

	const impact_info_t impact( interfaces::globals()->curtime, player->get_eye_pos(), pos, player->EntIndex(), col );

	if ( last_curtime == interfaces::globals()->curtime )
	{
		if ( !impacts.empty() && impact.length() < ( impacts.end() - 1 )->length() || impacts.empty() )
			return;

		impacts.pop_back();
	}

	last_curtime = interfaces::globals()->curtime;

	impacts.push_back( impact );
}

void beams::add_local_beam( const impact_info_t info )
{
	if ( !vars::visuals.beams.local.enabled->get<bool>() )
		return;

	impacts.push_back( info );
}

bool beams::beam_exists( C_CSPlayer* player, const float curtime )
{
	return std::ranges::any_of( impacts, [&] ( const impact_info_t& impact ) { return impact.time == curtime && player == globals::get_player( impact.index ); } );
}

void beams::add_beacon( const Vector& origin )
{
	beacons.push_back( origin );
}
