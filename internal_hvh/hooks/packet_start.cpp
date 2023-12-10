#include "../include_cheat.h"

bool __fastcall hook::send_net_msg( void* netchan, void* edx, INetMessage& msg, bool reliable, bool voice )
{
	if ( vars::misc.sv_pure->get<bool>() && msg.GetType() == 14 )
		return true;

	if ( msg.GetType() == 18 && fnv1a_rt( msg.GetName() ) == FNV1A( "CCLCMsg_CmdKeyValues" ) )
	{
		const auto keyvalues = KeyValues::from_netsmg( reinterpret_cast< INetMessage* >( reinterpret_cast< uintptr_t >( &msg ) + 0x4 ) );
		const auto orig_name = keyvalues->get_name();
		const auto name = fnv1a_rt( keyvalues->get_name() );
		const auto cmp = name == FNV1A( "UserExtraData" ) || name == FNV1A( "GameInfo" );
		keyvalues->~KeyValues();
		if ( cmp )
			return true;
	}

	if ( msg.GetGroup() == 9 )
		voice = false;

	return original( send_net_msg )( netchan, edx, msg, reliable, voice );
}

void __fastcall hook::packet_start( IClientState* state, uint32_t edx, int incoming_sequence, int outgoing_acknowledged )
{
	// erase commands that are out of range by a huge margin.
	globals::sent_commands.erase( std::ranges::remove_if( globals::sent_commands, [&] ( const uint32_t& cmd ) { return abs( static_cast< int32_t >( outgoing_acknowledged - cmd ) ) >= 150; } ).begin(), globals::sent_commands.end() );

	// rollback the ack count to what we aimed for.
	auto target_acknowledged = outgoing_acknowledged;
	for ( const auto cmd : globals::sent_commands )
		if ( outgoing_acknowledged >= cmd )
			target_acknowledged = cmd;

	original( packet_start )( state, edx, incoming_sequence, target_acknowledged );
}

bool __fastcall hook::SVCMsg_VoiceData( void* ecx, void* edx, CSVCMsg_VoiceData* msg )
{
	if ( shared_data::on_voice_data( msg ) )
		return original( SVCMsg_VoiceData )( ecx, edx, msg );

	return true;
}