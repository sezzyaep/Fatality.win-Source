#include "../include_cheat.h"

#ifdef DEBUG

void flashlight::run()
{
	if ( !interfaces::engine()->IsInGame() || !interfaces::engine()->IsConnected() || !local_player || !local_player->get_alive() || !vars::visuals.flashlight->get<bool>() )
	{
		if ( m_flashlight )
			destroy();

		return;
	}

	//_( flashlight001, "effects/flashlight001" );
	create( local_player->EntIndex(), nullptr, 35, 1000, 1000 );
	if ( !m_flashlight )
		return;

	Vector f{}, r{}, u{};
	QAngle viewAngles{};

	interfaces::engine()->GetViewAngles( viewAngles );
	math::angle_vectors( viewAngles, &f, &r, &u );

	m_flashlight->m_bIsOn = true;
	m_flashlight->m_bCastsShadows = true;
	m_flashlight->m_flCurrentPullBackDist = 1.f;
	m_flashlight->m_flFov = vars::visuals.flashlight_radius->get<float>();
	update( local_player->get_eye_pos(), f, r, u );
}

void flashlight::create( int ent_index, const char* name, float fov, float far_z, float lin_atten )
{
	// Already created ?
	if ( m_flashlight )
		return;

	// 1. xref -> "effects/flashlight_freezecam"
	static auto func = reinterpret_cast< CFlashLightEffect * ( __vectorcall* )( void*, void*, float, float, float, float, int, const char*, float, float ) >( sig( "client.dll", "55 8B EC F3 0F 10 45 ? B8" ) );

	auto new_flashlight = reinterpret_cast< CFlashLightEffect* >( interfaces::memalloc()->Alloc( sizeof( CFlashLightEffect ) ) );
	if ( !new_flashlight )
		return;

	func( new_flashlight, nullptr, 0.f, 0.f, 0.f, fov, ent_index, name, far_z, lin_atten );

	m_flashlight = new_flashlight;
}

void flashlight::destroy()
{
	// Already destroyed ?
	if ( !m_flashlight )
		return;

	// 1 - 4. xref -> "FlashlightState"
	// 2 - 1. xref func
	static auto func = reinterpret_cast< void( __thiscall* )( void*, void* ) >( sig( "client.dll", "56 8B F1 E8 ? ? ? ? 8B 4E 28" ) );
	;
	m_flashlight->m_bIsOn = false;
	func( m_flashlight, m_flashlight );
	m_flashlight = nullptr;
}

void flashlight::update( const Vector& pos, const Vector& forward, const Vector& right, const Vector& up )
{
	if ( !m_flashlight )
		return;

	typedef void( __thiscall* update_light_t )( void*, int, const Vector&, const Vector&, const Vector&, const Vector&, float, float, float, bool, const char* );
	static update_light_t orig = reinterpret_cast< update_light_t >( sig_rel( "client.dll", "E8 ? ? ? ? 8B 06 F3 0F 10 46" ) );

	orig( m_flashlight, m_flashlight->m_nEntIndex, pos, forward, right, up, m_flashlight->m_flFov, m_flashlight->m_flFarZ, m_flashlight->m_flLinearAtten, m_flashlight->m_bCastsShadows, m_flashlight->m_szTextureName );
}

#endif