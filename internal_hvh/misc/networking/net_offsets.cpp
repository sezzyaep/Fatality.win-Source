#include <vector>
#include <shared_mutex>
#include <random>
#include "../../include_cheat.h"

#include "connector.h"
#include "../MinHook.h"
#include "../protection.h"
#include <nmmintrin.h>

namespace networking
{
	std::atomic_uint32_t call_counter = 0;

#pragma clang section text=".discard"
	void __declspec( noinline ) net_offsets::init()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		mutex.lock();
		offsets.clear();
		offsets.resize( static_cast< size_t >( offsets::as_enum::COUNT ), init_val );
		for ( auto i = 0; i < offsets.size(); i++ )
			offsets[ i ] ^= i * 9283;
		requested.resize( static_cast< size_t >( offsets::as_enum::COUNT ), false );
		received.resize( static_cast< size_t >( offsets::as_enum::COUNT ), false );
		mutex.unlock();

		VIRTUALIZER_DOLPHIN_WHITE_END;

		erase_end;
	}
#pragma clang section text=""

	__declspec( noinline ) bool check_and_request( const uint64_t offset_hashed, const bool notify = false )
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		const auto offset = static_cast< offsets::as_enum >( offset_hashed ^ 0x2917284928126482 );

		if ( offset <= offsets::as_enum::INVALID || offset >= offsets::as_enum::COUNT )
			return false;

		network_offsets->mutex.lock_shared();
		const auto initialized = network_offsets->offsets[ static_cast< size_t >( offset ) ] != ( net_offsets::init_val ^ static_cast< uint64_t >( offset ) * 9283 );
		network_offsets->mutex.unlock_shared();

		if ( initialized )
			return false;

		network_offsets->mutex.lock_shared();
		const auto was_requested = network_offsets->requested[ static_cast< size_t >( offset ) ];
		network_offsets->mutex.unlock_shared();
		if ( !was_requested )
		{
			network_offsets->mutex.lock();
			network_offsets->requested[ static_cast< size_t >( offset ) ] = true;
			network_offsets->mutex.unlock();

			const auto crc = /*++call_counter % 8 == 0 ? crc64::calculate( textstart, textend, rdatastart, rdataend ) : */crc64::calculate_fast( textstart, textend, rdatastart, rdataend );
			if ( crc != globals::secure_info.initial_checksum )
				util::send_report( msg::report_type::TYPE1, 0xCCCC000F );

			msg::offset_request msg;
			msg.crc = crc ^ globals::secure_info.values.threads_ran_cached ^ protection::id_ran_all;
			msg.offset = static_cast< size_t >( offset );

			msg::message m( msg::msg_id::msg_offset_request );
			m.set_data( msg );
			
			//conn->queue_message( m );

			if ( notify )
				network_offsets->queue_cond_.notify_all();
		}

		VIRTUALIZER_DOLPHIN_WHITE_END;

		return true;
	}

	__declspec( noinline ) void net_offsets::precache_init()
	{
		VIRTUALIZER_TIGER_WHITE_START;

		get_offset( ( uint64_t ) offsets::as_enum::interface_memalloc ^ 0x0827343994827389, false );
		get_offset( ( uint64_t ) offsets::as_enum::interface_key_values_system ^ 0x0827343994827389, false );
		get_offset( ( uint64_t ) offsets::as_enum::interface_material_system ^ 0x0827343994827389, false );
		get_offset( ( uint64_t ) offsets::as_enum::sig_keyvalues_init ^ 0x0827343994827389, false );
		get_offset( ( uint64_t ) offsets::as_enum::sig_load_from_buffer ^ 0x0827343994827389, false );


		VIRTUALIZER_TIGER_WHITE_END;
	}

	__declspec( noinline ) void net_offsets::precache_map_load()
	{
		VIRTUALIZER_TIGER_WHITE_START;

		check_and_request( ( uint64_t ) offsets::as_enum::interface_weapon_system ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_engine_trace ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_model_render ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_prediction ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_game_movement ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_model_info ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_render_view ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_view_render_beams ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_input ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_surface_props ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_debug_overlay ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_debug_overlay ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_glow_object_manager ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_game_rules_proxy ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_player_resource ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::interface_hardware_config ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_disable_post_processing ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_lookup_bone ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_modifier_table ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_load_named_sky ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_view_matrix ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_get_item_schema ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_econ_item_destructor ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_animstate_reset ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_empty_mapping ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_snapshot_front ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_snapshot_back ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_ik_context_construct ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_ik_context_destruct ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_ik_context_init ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_ik_context_update_targets ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_ik_context_solve_dependencies ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_ammodef_get ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_process_sockets ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_render_triangle ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_vertex_color_ignorez ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_collision_bounds_change_time ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::sig_collision_viewheight ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CBaseViewModel_m_hOwner ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CBaseViewModel_m_hWeapon ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CBaseWeaponWorldModel_m_hCombatWeaponParent ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CCSGameRulesProxy_m_bFreezePeriod ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CCSGameRulesProxy_m_bIsValveDS ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CCSPlayerResource_m_iPlayerC4 ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CCSPlayerResource_m_szClan ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CCSPlayer_m_iPrimaryAddon ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CCSPlayer_m_iSecondaryAddon ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CCSRagdoll_m_hPlayer ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CPlantedC4_m_hBombDefuser ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CPlantedC4_m_bBombDefused ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CPlantedC4_m_bBombTicking ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CPlantedC4_m_flC4Blow ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CPlantedC4_m_flDefuseCountDown ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CPlantedC4_m_flDefuseLength ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CWeaponCSBaseGun_m_zoomLevel ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CWeaponCSBase_m_fLastShotTime ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CWeaponCSBase_m_flPostponeFireReadyTime ^ 0x2917284928126482 );
		check_and_request( ( uint64_t ) offsets::as_enum::CSmokeGrenadeProjectile_m_nSmokeEffectTickBegin ^ 0x2917284928126482, true );

		VIRTUALIZER_TIGER_WHITE_END;
	}

	__forceinline void precache_batch( const offsets::as_enum offset )
	{
		// sigs
		if ( offset > offsets::as_enum::interface_string_tables && offset < offsets::as_enum::hook_start_sound_immediate )
			for ( auto i = static_cast< uint64_t >( offset ) - 5; i < static_cast< uint64_t >( offset ) + 5; i++ )
				if ( i != static_cast< uint64_t >( offset ) && static_cast< offsets::as_enum >( i ) > offsets::as_enum::interface_string_tables && static_cast< offsets::as_enum >( i ) < offsets::as_enum::hook_start_sound_immediate )
					check_and_request( i ^ 0x2917284928126482 );

		// netvars
		if ( offset >= offsets::as_enum::CAI_BaseNPC_m_bFadeCorpse && offset <= offsets::as_enum::SporeTrail_m_vecEndColor )
			for ( auto i = static_cast< uint64_t >( offset ) - 50; i < static_cast< uint64_t >( offset ) + 50; i++ )
				if ( i != static_cast< uint64_t >( offset ) && static_cast< offsets::as_enum >( i ) >= offsets::as_enum::CAI_BaseNPC_m_bFadeCorpse && static_cast< offsets::as_enum >( i ) <= offsets::as_enum::SporeTrail_m_vecEndColor )
					check_and_request( i ^ 0x2917284928126482 );

		// hooks
		if ( offset >= offsets::as_enum::hook_start_sound_immediate && offset < offsets::as_enum::CAI_BaseNPC_m_bFadeCorpse )
			for ( auto i = static_cast< uint64_t >( offset ) - 2; i < static_cast< uint64_t >( offset ) + 2; i++ )
				if ( i != static_cast< uint64_t >( offset ) && static_cast< offsets::as_enum >( i ) >= offsets::as_enum::hook_start_sound_immediate && static_cast< offsets::as_enum >( i ) < offsets::as_enum::CAI_BaseNPC_m_bFadeCorpse )
					check_and_request( i ^ 0x2917284928126482 );
	}

	using namespace vm;

	__declspec( noinline ) uint64_t net_offsets::get_offset( uint64_t offset_hashed, bool remove )
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		auto ret = invalid_val;

		const auto offset = static_cast< offsets::as_enum >( offset_hashed ^ 0x0827343994827389 );

		mutex.lock_shared();
		const auto initialized = offsets[ static_cast< size_t >( offset ) ] != ( init_val ^ static_cast< uint64_t >( offset ) * 9283 );
		mutex.unlock_shared();

		log_debug( "a4\n" );

		if ( !initialized )
			precache_batch( offset );

		++in_request;
		queue_cond_.notify_all();

		log_debug( "a5\n" );

		int iterations = 0;

		while ( true )
		{
			if ( ret != invalid_val )
				break;

			mutex.lock_shared();
			const auto val = offsets[ static_cast< size_t >( offset ) ];
			mutex.unlock_shared();

			if ( val != ( init_val ^ static_cast< uint64_t >( offset ) * 9283 ) && val != ( deleted_val ^ static_cast< uint64_t >( offset ) * 9283 ) )
			{
				received[ static_cast< size_t >( offset ) ] = true;
				requested[ static_cast< size_t >( offset ) ] = false;

				if ( remove )
				{
					mutex.lock();
					offsets[ static_cast< size_t >( offset ) ] = deleted_val ^ static_cast< uint64_t >( offset ) * 9283;
					mutex.unlock();
				}

				--in_request;

				ret = val ^ globals::secure_info.initial_checksum;
				break;
			}

			if ( val == ( deleted_val ^ static_cast< uint64_t >( offset ) * 9283 ) )
				break;

			mutex.lock_shared();
			const auto was_requested = requested[ static_cast< size_t >( offset ) ] || received[ static_cast< size_t >( offset ) ];
			mutex.unlock_shared();

			if ( !was_requested )
			{
				std::unique_lock lock( mutex );

				requested[ static_cast< size_t >( offset ) ] = true;

				const auto crc = ++call_counter % 8 == 0 ? crc64::calculate( textstart, textend, rdatastart, rdataend ) : crc64::calculate_fast( textstart, textend, rdatastart, rdataend );
				if ( crc != globals::secure_info.initial_checksum )
					util::send_report( msg::report_type::TYPE1, 0xCCCC000F );

				msg::offset_request msg;
				msg.crc = crc;
				msg.offset = static_cast< size_t >( offset );

				msg::message m( msg::msg_id::msg_offset_request );
				m.set_data( msg );

				//conn->queue_message( m );

				queue_cond_.notify_all();
			}

			if ( was_requested && iterations > 50000 )
			{
				ret = 0x2facf8f;
				break;
			}

			imp( "kernel32.dll", SleepEx )( 1, true );
			iterations++;
		}

		VIRTUALIZER_DOLPHIN_WHITE_END;

		return ret;
	}
}

