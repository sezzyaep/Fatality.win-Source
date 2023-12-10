#include "../include_cheat.h"

int __fastcall hook::emit_sound( void* thisptr, int edx, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk )
{
	if ( CGameMovement::computing )
		return 0;

	if ( !interfaces::engine()->IsInGame() && vars::misc.autoaccept->get<bool>() && fnv1a_rt( pSoundEntry ) == FNV1A( "UIPanorama.popup_accept_match_beep" ) )
	{
		reinterpret_cast< char( __stdcall* )( const char* ) >( make_offset( "client.dll", sig_is_ready ) )( " " );
		interfaces::surface()->PlaySound( xorstr_( "buttons/light_power_on_switch_01.wav" ) );

		FLASHWINFO flash;
		flash.cbSize = sizeof( FLASHWINFO );
		flash.hwnd = init::window;
		flash.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
		flash.uCount = 0;
		flash.dwTimeout = 0;
		imp( "user32.dll", FlashWindowEx )( &flash );

		return original( emit_sound )( thisptr, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, 0.f, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk );
	}

	_( weapon, "weapon" );
	_( draw, "draw" );
	_( deploy, "deploy" );
	if ( strstr( pSample, weapon.c_str() ) && ( strstr( pSample, draw.c_str() ) || strstr( pSample, deploy.c_str() ) ) )
	{
		static uint32_t prev_hash = 0;
		const uint32_t hash = fnv1a_rt( pSample );

		if ( prev_hash == hash )
			return 0;

		prev_hash = hash;
	}

	return original( emit_sound )( thisptr, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk );
}

int __fastcall hook::start_sound_immediate( start_sound_params* params, int edx )
{
	if ( CGameMovement::computing || !params || !params->sfx )
		return 0;

	char name[ MAX_PATH ];
	params->sfx->get_name( name, MAX_PATH );

	if ( params->fromserver && params->sound_source == interfaces::engine()->GetLocalPlayer() )
	{
		_( player_footsteps, "player\\footsteps" );
		_( zoom, "zoom" );
		if ( strstr( name, player_footsteps.c_str() ) || strstr( name, zoom.c_str() ) )
			return 0;
	}

	sound_esp::on_start_sound( params, name );

	return original( start_sound_immediate )( params, edx );
}

void __fastcall hook::start_sound( IMoveHelper* move_helper, int edx, const Vector& origin, const char* soundname )
{
	if ( CGameMovement::computing )
		return;

	original( start_sound )( move_helper, edx, origin, soundname );
}