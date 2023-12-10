#include "../include_cheat.h"
#include <intrin.h>

int __fastcall hook::convar_get_int_engine( ConVar* convar, void* edx )
{
	if ( reinterpret_cast< uintptr_t >( _ReturnAddress() ) == make_offset( "engine.dll", sig_ret_to_draw_notify ) || reinterpret_cast< uintptr_t >( _ReturnAddress() ) == make_offset( "engine.dll", sig_ret_to_add_notify ) ) // engine hook
		return true;

	if ( reinterpret_cast< uintptr_t >( _ReturnAddress() ) == make_offset( "engine.dll", sig_ret_to_console_print ) && console_log::m_in_print ) // engine hook
		return true;

	if ( reinterpret_cast< uintptr_t >( _ReturnAddress() ) == make_offset( "client.dll", sig_ret_to_cam_think ) ) // engine hook
		return true;

	return original_vfunc( convar, convar_get_int_engine )( convar, edx );
}

int __fastcall hook::convar_get_int_client( ConVar* convar, void* edx )
{
	if ( reinterpret_cast< uintptr_t >( _ReturnAddress() ) == make_offset( "client.dll", sig_ret_to_draw_crosshair ) && !local_player->get_scoped() && local_player->get_alive() && vars::misc.force_crosshair->get<bool>() ) // client hook
		return 3;

	if ( vars::misc.birthday_mode->get<bool>() && fnv1a_rt( convar->name ) == FNV1A( "sv_party_mode" ) )
		return true;

	return original_vfunc( convar, convar_get_int_client )( convar, edx );
}

float __fastcall hook::convar_get_float_engine( ConVar* convar, void* edx )
{
	if ( *reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( _ReturnAddress() ) + 0x8F ) == 0x77
	&& *reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( _ReturnAddress() ) + 0x90 ) == 0x2E
	&& *reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( _ReturnAddress() ) + 0x91 ) == 0x80 )
	{
		const auto darkness = vars::visuals.dark->get<bool>() ? vars::visuals.darkness->get<float>() : 0.f;
		return clamp( .5f - ( darkness * .01f ) * .475f, 0.01f, 1.f );
	}

	return original_vfunc( convar, convar_get_float_engine )( convar, edx );
}

void __fastcall hook::server_cmd_key_values( void* ecx, void* edx, KeyValues* key_values )
{
	if ( fnv1a_rt( key_values->get_name() ) == FNV1A( "CrosshairCode" ) )
		key_values->set_string( xorstr_( "code" ), xorstr_( "CSGO-9whau-zVNhk-799MD-jBNq3-HsnkP" ) );

	return original( server_cmd_key_values )( ecx, edx, key_values );
}

void __cdecl hook::convar_network_change_callback( ConVar* var, const char* old1, float old2 )
{
	const auto var2 = reinterpret_cast< ConVar* >( reinterpret_cast< uintptr_t >( var ) - 0x18 );

	if ( var->flags & ( 1 << 9 ) )
	{
		if ( fnv1a_rt( var->get_name() ) == FNV1A( "cl_lagcompensation" ) )
			ConVar::cl_lagcompensation = fnv1a_rt( var2->value.string ) != FNV1A( "0" );
		else if ( fnv1a_rt( var->get_name() ) == FNV1A( "cl_predict" ) )
			ConVar::cl_predict = fnv1a_rt( var2->value.string ) != FNV1A( "0" );
	}

	original( convar_network_change_callback )( var, old1, old2 );
}
