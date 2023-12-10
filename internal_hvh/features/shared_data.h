#pragma once

namespace shared_data
{
	void send_data_msg( const voice_usable_fields& data );
	bool on_voice_data( const CSVCMsg_VoiceData* msg );
}

