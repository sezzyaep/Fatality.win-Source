#pragma once

struct anim_debug_info_t
{
	matrix3x4_t m_client_bones[ 128 ]{};
	matrix3x4_t m_server_bones[ 128 ]{};

	uint32_t m_has_data{};
	static auto constexpr SERVER = ( 1 << 0 );
	static auto constexpr CLIENT = ( 1 << 1 );

	void emplace_server( matrix3x4_t* bones )
	{
		if ( m_has_data & SERVER )
			return;

		memcpy( m_server_bones, bones, 128 * sizeof( matrix3x4_t ) );
		m_has_data |= SERVER;
	}

	void emplace_client( matrix3x4_t* bones )
	{
		if ( m_has_data & CLIENT )
			return;

		memcpy( m_client_bones, bones, 128 * sizeof( matrix3x4_t ) );
		m_has_data |= CLIENT;
	}
};

template<typename T, size_t size>
struct ring_buffer_t
{
	T& get_or_put( size_t const& key )
	{
		if ( memory[ key % size ].key != key )
		{
			memory[ key % size ].key = key;
			memory[ key % size ].value = {};
		}

		return memory[ key % size ].value;
	}

	struct
	{
		size_t key;
		T value;
	} memory[ size ];
};

namespace player_log
{
	struct shared_esp_data
	{
		uint16_t identifier;
		uint8_t user_id;
		uint8_t weapon_id;
		uint32_t server_tick;
		Vector pos;
	};

	void run_log( ClientFrameStage_t stage );
	void queue_esp_data( C_CSPlayer* player );
	void send_esp_data();
	void parse_esp_data( const int client, const voice_usable_fields& data );
	void run_player( C_CSPlayer* player );
	player_log_t& get_log( int index );
	void filter_records();

	inline player_log_t logs[ 65 ] = {};
	inline std::deque<std::pair<int, shared_esp_data>> queued_esp_data;

#ifdef _DEBUG
	inline ring_buffer_t<anim_debug_info_t, 150> anim_debug_info[ 65 ] = {};
#endif
};