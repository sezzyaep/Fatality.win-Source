#include "../include_cheat.h"

void shared_data::send_data_msg( const voice_usable_fields& data )
{
	CCLCMsg_VoiceData msg;
	memset( &msg, 0, sizeof( CCLCMsg_VoiceData ) );

	reinterpret_cast< uint32_t( __fastcall* )( CCLCMsg_VoiceData*, void* ) >( make_offset( "engine.dll", sig_msg_voicedata_constructor ) )( &msg, nullptr );

	msg.set_usable_fields( data );

	struct
	{
		char data[ 16 ]{};
		uint32_t current_len = 0;
		uint32_t max_len = 15;
	} voice_data{};
	msg.data = &voice_data;
	msg.format = 0;
	msg.flags = 63;

	original( hook::send_net_msg )( interfaces::client_state()->m_NetChannel, nullptr, *reinterpret_cast< INetMessage* >( &msg ), false, true );
}


bool shared_data::on_voice_data( const CSVCMsg_VoiceData* msg )
{
	if ( !vars::misc.shared_esp->get<bool>() )
		return true;

	if ( msg->format != 0 )
		return true;

	const auto data = msg->get_usable_fields();
	if ( !data.sequence_bytes && !data.section_number && !data.uncompressed_sample_offset ) // check for empty message due to bHearsPlayer
		return false;

	if ( msg->client + 1 == interfaces::engine()->GetLocalPlayer() )
		return false;

	player_log::parse_esp_data( msg->client + 1, data );

	return false;
}