#include "../include_cheat.h"
#include <ren/adapter_dx9.h>

extern std::vector<std::function<void()>> queued_calls;

void __stdcall hook::frame_stage_notify( ClientFrameStage_t stage )
{
	visuals::game_curtime = interfaces::globals()->curtime;

	if ( globals::erase_init )
	{
		erase_fn( init::on_startup );
		globals::erase_init = false;
	}

	misc::custom_render_options( stage );

	chams::dark_mode( stage );

	freestanding::update_hotkeys( stage );

	misc::remove_smoke( stage );

	misc::remove_flash( stage );

	misc::preserve_deathnotices( stage );

	player_log::run_log( stage );

	misc::fix_prediction( stage );

	lua::api.callback( FNV1A( "on_frame_stage_notify" ), [&] ( lua::state& s ) -> int
	{
		s.push( static_cast< int >( stage ) );
		s.push( true );

		return 2;
	} );

	if ( stage == FRAME_RENDER_START )
	{
		if ( interfaces::engine()->IsConnected() && interfaces::engine()->IsInGame() )
		{
			/*for ( const auto player : interfaces::entity_list()->get_players() )
			{
				auto& var_mapping = player->get_var_mapping();
				auto& entries = var_mapping.m_Entries;

				for ( auto j = 0; j < entries.Count(); j++ )
				{
					const auto& entry = entries[ j ];
					entry.watcher->get_type_flags() |= 1 << 4;
				}
			}*/

			if ( const auto player = local_player; player && player->get_alive() && player->get_anim_state() )
			{
				const auto bak_sequence = player->get_sequence();
				player->get_sequence() = -1;
				player->get_anim_state()->player = nullptr;
				original( update_clientside_anim )( player, nullptr );
				player->get_anim_state()->player = player;
				player->get_sequence() = bak_sequence;
			}
		}
	}

	original( frame_stage_notify )( stage );

	resolver::approve_shots( stage );

	resolver::update_missed_shots( stage );

	chams::extend_drawing( stage );

	misc::ragdoll_slowpath( stage );

	skinchanger::run( stage );

	if ( stage == FRAME_RENDER_END )
	{
		if ( adapter && dx_adapter::mtx.try_lock() )
		{
			dx_adapter::clear_buffer();

			visuals::run();
			visuals::render();
			entity_rendering::paint();

			for ( auto& call : queued_calls )
				call();

			queued_calls.clear();

			lua::api.in_render = true;
			lua::api.callback( FNV1A( "on_paint" ) );
			lua::api.in_render = false;

			dx_adapter::ready = true;
			dx_adapter::mtx.unlock();
		}

		thirdperson::run();
	}

	lua::api.callback( FNV1A( "on_frame_stage_notify" ), [&] ( lua::state& s ) -> int
	{
		s.push( static_cast< int >( stage ) );
		s.push( false );

		return 2;
	} );

	if ( stage == FRAME_START )
		lua::api.run_timers();
}
