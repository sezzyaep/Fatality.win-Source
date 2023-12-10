#include "../include_cheat.h"
#include "ren/adapter_dx9.h"

using namespace evo::ren;

void visuals::run()
{
	if ( !interfaces::engine()->IsInGame() || !local_player )
		return;

	sound_esp::update();

	queue_player_data();

	queue_world_data();

	queue_misc_data();
}

void visuals::render()
{
	if ( !interfaces::engine()->IsInGame() )
		return;

	world_esp();

	players_esp();

	grenade_pred::draw_esp();

	draw_misc_data();

	hitmarker::draw_hits();
}

void visuals::players_get()
{
	for ( auto i = 0; i <= interfaces::globals()->maxClients; i++ )
	{
		if ( i == 0 )
		{
			players[ i ].index = 0;
			continue;
		}

		const auto player = globals::get_player( i );
		players[ i ].index = i;
		players[ i ].player = player;

		const auto updated = sound_esp::get_last_update( i );
		if ( !players[ i ].had_info && updated )
			players[ i ].had_info = true;

		if ( !player || player == local_player || !player->IsDormant() && !player->get_alive() )
		{
			players[ i ].alpha -= 255.f / 0.3f * interfaces::globals()->frametime;
			players[ i ].alpha = clamp( players[ i ].alpha, 0.f, 255.f );
			players[ i ].model_alpha -= 255.f / 0.3f * interfaces::globals()->frametime;
			players[ i ].model_alpha = clamp( players[ i ].model_alpha, 0.f, 255.f );
			players[ i ].health_interpolated = players[ i ].health = 0;
			if ( vars::visuals.disable_model_fade->get<bool>() || vars::visuals.disable_esp_fade->get<bool>() )
				players[ i ].alpha = players[ i ].model_alpha = 0.f;

			const auto sound_player = sound_esp::get_sound_player( i );
			if ( sound_player )
			{
				sound_player->has_zeus = false;
				sound_player->zeus_active = false;
			}
			if ( !player || player == local_player )
			{
				players[ i ].model_alpha = 0.f;
				players[ i ].alpha = 0.f;
				players[ i ].valid = false;
				players[ i ].had_info = false;
			}
			if ( players[ i ].alpha <= 0.f )
			{
				players[ i ].had_info = false;
				players[ i ].valid = false;
			}

			players[ i ].alpha_lerp = players[ i ].alpha;

			continue;
		}

		if ( player->get_spawn_time() != players[ i ].spawntime && player->get_alive() )
		{
			players[ i ].spawntime = player->get_spawn_time();
			players[ i ].alpha = 0.f;
			players[ i ].alpha_lerp = 0.f;
			players[ i ].model_alpha = 0.f;
			players[ i ].had_info = false;
#if defined(ALPHA) || !defined(RELEASE)
			players[ i ].debug = {};
#endif
		}

		if ( player->IsDormant() && !players[ i ].had_info && !updated )
		{
			players[ i ].alpha = 0.f;
			players[ i ].alpha_lerp = 0.f;
			players[ i ].model_alpha = 0.f;
			players[ i ].valid = false;
			continue;
		}

		players[ i ].valid = true;
		players[ i ].had_info = true;
		if ( player->IsDormant() )
		{
			players[ i ].model_alpha -= 255.f / 0.3f * interfaces::globals()->frametime;
			players[ i ].model_alpha = clamp( players[ i ].model_alpha, 0.f, 255.f );

			if ( players[ i ].alpha_lerp < 160.f )
			{
				players[ i ].alpha_lerp -= 255.f / 16.f * interfaces::globals()->frametime;
				players[ i ].alpha_lerp = clamp( players[ i ].alpha_lerp, 0.f, 160.f );
				players[ i ].alpha = 160.f;
			}
			else
			{
				players[ i ].alpha_lerp -= 255.f / 1.f * interfaces::globals()->frametime;
				players[ i ].alpha = 160.f;
			}
			if ( updated )
			{
				players[ i ].alpha = players[ i ].alpha_lerp = 160.f;
			}

			if ( vars::visuals.disable_model_fade->get<bool>() || vars::visuals.disable_esp_fade->get<bool>() )
				players[ i ].model_alpha = 0.f;

			if ( !vars::visuals.disable_esp_fade->get<bool>() || players[ i ].alpha_lerp <= 0.f )
				players[ i ].alpha = players[ i ].alpha_lerp;
		}
		else
		{
			players[ i ].model_alpha += 255.f / 0.5f * interfaces::globals()->frametime;
			players[ i ].model_alpha = clamp( players[ i ].model_alpha, 0.f, 255.f );
			players[ i ].alpha_lerp += 255.f / 0.6f * interfaces::globals()->frametime;
			players[ i ].alpha_lerp = clamp( players[ i ].alpha_lerp, 0.f, 255.f );

			if ( vars::visuals.disable_model_fade->get<bool>() )
				players[ i ].model_alpha = 255.f;

			if ( vars::visuals.disable_esp_fade->get<bool>() )
				players[ i ].alpha_lerp = 255.f;

			players[ i ].alpha = players[ i ].alpha_lerp;
		}

		if ( !player->is_enemy() )
		{
			players[ i ].alpha = 0.f;
			players[ i ].alpha_lerp = 0.f;
			players[ i ].valid = false;
			players[ i ].had_info = false;
		}
	}
}



void visuals::player_skeleton( const visual_data_t& player, layer& layer )
{
	const auto skeleton_color = color( vars::visuals.esp.skeleton_color->get<D3DCOLOR>() );
	const auto history_skeleton_color = color( vars::visuals.esp.history_skeleton_color->get<D3DCOLOR>() );

	const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f * skeleton_color.a() ) : ::color( skeleton_color, player.alpha / 255.f * skeleton_color.a() );
	const auto hist_color = ::color( history_skeleton_color, player.alpha / 255.f * history_skeleton_color.a() );

	layer.push_aa( true );

	for ( auto& curr : player.history_skeleton_poses )
	{
		layer.add_line( vec2( curr.first.x, curr.first.y ), vec2( curr.second.x, curr.second.y ), hist_color );
	}

	const auto pos = vec2( entity_rendering::preview_pos.x, entity_rendering::preview_pos.y );

	for ( auto& curr : player.skeleton_poses )
	{
		if ( !player.player )
			layer.add_line( vec2( curr.first.x, curr.first.y ) + pos, vec2( curr.second.x, curr.second.y ) + pos, color );
		else
			layer.add_line( vec2( curr.first.x, curr.first.y ), vec2( curr.second.x, curr.second.y ), color );
	}

	layer.pop_aa();
}

void visuals::player_box( const visual_data_t& player, layer& layer )
{
	if ( !vars::visuals.esp.box->get<bool>() )
		return;

	const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( ::color( vars::visuals.esp.box_color->get<D3DCOLOR>() ), player.alpha );

	layer.add_rect( rect_legacy( { player.top.x - player.width + 1, player.top.y + 1 }, { player.width * 2 - 2, player.height - 2 } ), color );
	layer.add_rect( rect_legacy( { player.top.x - player.width, player.top.y }, { player.width * 2, player.height } ), ::color( 0.f, 0.f, 0.f, player.alpha * 0.4f ) );
	layer.add_rect( rect_legacy( { player.top.x - player.width + 2, player.top.y + 2 }, { player.width * 2 - 4, player.height - 4 } ), ::color( 0.f, 0.f, 0.f, player.alpha * 0.4f ) );
}

void visuals::player_info( const visual_data_t& player, layer& layer )
{
	const auto info_bottom = [&] ()
	{
		auto elements = 0;

		if ( vars::visuals.esp.armor->get<bool>() )
		{
			bar_armor( player, elements, layer );
			elements++;
		}

		if ( vars::visuals.esp.ammo->get<bool>() && player.maxclip > 0 )
		{
			bar_ammo( player, elements, layer );
			elements++;
		}

		if ( vars::visuals.esp.weapon->get<bool>() )
		{
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( vars::visuals.esp.weapon_color->get<D3DCOLOR>(), player.alpha );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + 1, player.top.y + player.height + 5 * elements++ + 2 ), player.weapon_name, color, layer::align_center );
			elements++;
		}

		if ( vars::visuals.esp.weapon_icon->get<bool>() && player.weapon_id )
		{
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( vars::visuals.esp.weapon_icon_color->get<D3DCOLOR>(), player.alpha );
			dx_adapter::weapon( layer, vec2( player.top.x + 1, player.top.y + player.height + 5 * elements++ + 1 ), 12, color, player.icon_name.c_str(), layer::align_center );
		}
	};

	const auto info_left = [&] ()
	{
		auto elements = 0;
		auto move_left = 0;

		if ( vars::visuals.esp.health->get<bool>() )
		{
			bar_health( player, 0, layer );
			move_left++;
		}

		if ( player.has_zeus && vars::visuals.esp.flags.taser->get<bool>() )
		{
			const auto amount = fmod( interfaces::globals()->realtime, 0.5f ) * 2.f;
			const auto opacity = ( static_cast< int >( ceilf( interfaces::globals()->realtime ) ) % 1 ? 1.f - amount : amount );
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( 255.f, 230.f, 22.f, player.alpha * ( player.zeus_active ? opacity : 1.f ), true );
			dx_adapter::custom_icon( layer, vec2( player.top.x - player.width - 4 - move_left * 4 + 6, player.top.y + 1 + 11 * elements++ ), 18, color, xorstr_( "taser" ), layer::align_right );
		}

		if ( player.kit && vars::visuals.esp.flags.defuser->get<bool>() )
		{
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( Color::BlueAccent( player.alpha ).direct() );
			dx_adapter::weapon( layer, vec2( player.top.x - player.width - 4 - move_left * 4, player.top.y + 1 + 11 * elements++ ), 14, color, xorstr_( "defuser" ), layer::align_right );
		}

		if ( player.bomb && vars::visuals.esp.flags.bomb->get<bool>() )
		{
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( Color::Red( player.alpha ).direct() );
			dx_adapter::weapon( layer, vec2( player.top.x - player.width - 4 - move_left * 4, player.top.y + 1 + 11 * elements++ ), 14, color, xorstr_( "c4" ), layer::align_right );
		}
	};

	const auto info_top = [&] ( const visual_data_t& player ) // bug? have to pass as arg else undefined
	{
		auto elements = 0;

		if ( vars::visuals.esp.ping->get<bool>() )
		{
			if ( bar_ping( player, elements, layer ) )
				elements++;
		}

		if ( vars::visuals.esp.name->get<bool>() )
		{
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( vars::visuals.esp.name_color->get<D3DCOLOR>(), player.alpha );
			layer.add_text( draw.get_font( FNV1A( "esp_name" ) ), vec2( player.top.x + 1, player.top.y - 15 - 6 * elements++ + 2 ), player.name, color, layer::align_center );
		}
	};

	const auto info_right = [&] ()
	{
		auto elements = 0;

		if ( vars::visuals.esp.flags.money->get<bool>() )
		{
			_( format, "$%d" );
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( 130.f, 180.f, 0.f, player.alpha, true );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4, player.top.y - 3 + 8 * elements++ + 2 ), util::format( format.c_str(), player.money ), color, layer::align_left );
		}
		if ( ( player.helmet || player.kevlar ) && vars::visuals.esp.flags.armor->get<bool>() )
		{
			_( H, "H" );
			_( K, "K" );
			const auto string = player.helmet ? std::string( H ) + K : std::string( K );
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( 255.f, 255.f, 255.f, player.alpha, true );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4, player.top.y - 3 + 8 * elements++ + 2 ), string, color, layer::align_left );
		}
		if ( player.can_hit && vars::visuals.esp.flags.hit->get<bool>() )
		{
			_( H, "HIT" );
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( 255.f, 255.f, 255.f, player.alpha, true );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4, player.top.y - 3 + 8 * elements++ + 2 ), H, color, layer::align_left );
		}
		if ( player.scoped && vars::visuals.esp.flags.scoped->get<bool>() )
		{
			_( SCOPED, "SCOPED" );
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( 0.f, 153.f, 204.f, player.alpha, true );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4, player.top.y - 3 + 8 * elements++ + 2 ), SCOPED, color, layer::align_left );
		}
		if ( player.lc && !player.dormant && vars::visuals.esp.flags.lc->get<bool>() )
		{
			_( lc_s, "LC" );
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( 130.f, 180.f, 0.f, player.alpha, true );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4, player.top.y - 3 + 8 * elements++ + 2 ), lc_s, color, layer::align_left );
		}
		if ( player.fd && !player.dormant && vars::visuals.esp.flags.fd->get<bool>() )
		{
			_( fd_s, "FD" );
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : ::color( 210.f, 0.f, 0.f, player.alpha, true );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4, player.top.y - 3 + 8 * elements++ + 2 ), fd_s, color, layer::align_left );
		}
		if ( player.xp && !player.dormant && vars::visuals.esp.flags.xp->get<bool>() )
		{
			_( dt_s, "XP" );
			const auto color = player.dormant ? ::color::gray( 128, player.alpha / 255.f ) : player.not_lagcompensated ? ::color( 210.f, 0.f, 0.f, player.alpha, true ) : ::color( 0.f, 153.f, 204.f, player.alpha, true );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4, player.top.y - 3 + 8 * elements++ + 2 ), dt_s, color, layer::align_left );
		}
		/*if ( !player.dormant && ( !player.player || player.player->get_immunity() || player.player->get_render_clr() != local_player->get_render_clr() ) )
		{
			_( dt_s, "SPAWN" );
			const auto color = ::color( 210.f, 0.f, 0.f, player.alpha, true );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4, player.top.y - 3 + 8 * elements++ + 2 ), dt_s, color, layer::align_left );
		}*/


		lua::api.callback( FNV1A( "on_esp_flags" ), [&] ( lua::state& s )
		{
			s.push( player.index );			// player index
			return 1;
		}, 1, [&] ( lua::state& s )
		{
			if ( !s.is_table( 1 ) )
				return;

			const auto ret = s.table_to_object_array<lua::helpers::esp_flag>( 1 );
			for ( const auto& f : ret )
			{
				layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4, player.top.y - 3 + 8 * elements++ + 2 ), f.text, f.color, layer::align_left );
			}
		} );

#if defined(ALPHA) || !defined(RELEASE)
		if ( vars::misc.debug_info->get<bool>() )
		{
			elements = 0;
			const auto color = ::color( 255.f, 255.f, 255.f, player.alpha, true );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4 + 30, player.top.y - 3 + 8 * elements++ + 2 ), player.debug.reason, color, layer::align_left );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4 + 30, player.top.y - 3 + 8 * elements++ + 2 ), util::format( XOR_STR( "SCAN [ %d ]" ), player.debug.scan ), color, layer::align_left );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4 + 30, player.top.y - 3 + 8 * elements++ + 2 ), util::format( XOR_STR( "HC [ %d : %d ]" ), player.debug.hc, player.debug.target_hc ), color, layer::align_left );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4 + 30, player.top.y - 3 + 8 * elements++ + 2 ), util::format( XOR_STR( "DMG [ %d : %d ]" ), player.debug.dmg, player.debug.target_dmg ), color, layer::align_left );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4 + 30, player.top.y - 3 + 8 * elements++ + 2 ), util::format( XOR_STR( "SP [ %d : %d ]" ), player.debug.sp, player.debug.target_sp ), color, layer::align_left );
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4 + 30, player.top.y - 3 + 8 * elements++ + 2 ), util::format( XOR_STR( "VSP [ %d : %d ]" ), player.debug.vsp, player.debug.target_vsp ), color, layer::align_left );
			if ( player.debug.extrapolate )
				layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x + player.width + 4 + 30, player.top.y - 3 + 8 * elements++ + 2 ), XOR_STR( "EXTRAPOLATE" ), color, layer::align_left );
		}
#endif
	};


	info_left();
	info_right();

	if ( ( player.width * 2 - 2 ) < 1.f )
		return;

	info_bottom();
	info_top( player );


}

bool visuals::bar_ping( const visual_data_t& player, int elements, layer& layer )
{
	if ( player.ping < 0.2f )
		return false;

	const auto ping = clamp( player.ping, 0.0f, 1.f );
	const auto width = ( player.width * 2.f - 2 ) * ping;

	const auto color = player.dormant ? Color::Grey().GetD3DColor() : vars::visuals.esp.ping_color->get<D3DCOLOR>();

	layer.add_rect_filled( rect_legacy( { player.top.x - player.width, player.top.y - 6 - 4 * elements }, { player.width * 2, 4 } ), ::color( 0, 0, 0, static_cast< int >( player.alpha * 0.4f ) ) );
	layer.add_rect_filled( rect_legacy( { player.top.x - player.width + 1, player.top.y - 5 - 4 * elements }, { width, 2 } ), ::color( color, player.alpha ) );

	return true;
}


void visuals::bar_health( const visual_data_t& player, const int elements, layer& layer )
{
	const auto health = clamp( player.health_interpolated, 0.f, 100.f );
	const auto multiplier = clamp( health / 100.f, 0.f, 1.f );
	const auto height = ceilf( player.height - 2 ) * multiplier;

	const auto col1 = Color( vars::visuals.esp.health_color_1->get<D3DCOLOR>() );
	const auto col2 = Color( vars::visuals.esp.health_color_2->get<D3DCOLOR>() );

	const auto red = math::lerp( col1.r(), col2.r(), multiplier );
	const auto green = math::lerp( col1.g(), col2.g(), multiplier );
	const auto blue = math::lerp( col1.b(), col2.b(), multiplier );

	const auto color = player.dormant ? ::color::gray( 128.f, player.alpha ) : ::color( red, green, blue );
	const auto color2 = player.dormant ? ::color::gray( 128.f, player.alpha ) : ::color( col1.direct() );

	layer.add_rect_filled( rect_legacy( { player.top.x - player.width - 5 - elements * 6, player.top.y }, { 4, player.height } ), ::color( 0, 0, 0, static_cast< int >( player.alpha * 0.4f ) ) );
	if ( !vars::visuals.esp.health_style->get<int>() )
		layer.add_rect_filled( rect_legacy( { player.top.x - player.width - 4 - elements * 6, player.top.y + player.height - height - 1 }, { 2, height } ), ::color( color, player.alpha ) );
	else
		layer.add_rect_filled_multicolor( rect_legacy( { player.top.x - player.width - 4 - elements * 6, player.top.y + player.height - height - 1 }, { 2, height } ), { ::color( color, player.alpha ), ::color( color, player.alpha ), ::color( color2, player.alpha ), ::color( color2, player.alpha ) } );

	_( d, "%d" );

	if ( health < 99.f && health >= 1.f )
		layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x - player.width - 3, player.top.y + player.height - height ), util::format( d.c_str(), player.health ), ::color( 255, 255, 255, static_cast< int >( player.alpha ) ), layer::align_center, layer::align_center );
}

void visuals::bar_armor( const visual_data_t& player, const int elements, layer& layer )
{
	const auto armor = clamp( player.armor, 0, 100 );
	if ( !armor )
		return;

	const auto multiplier = clamp( armor / 100.f, 0.f, 1.f );
	const auto width = ( player.width * 2.f - 2 ) * multiplier;

	const auto color = player.dormant ? Color::Grey().GetD3DColor() : vars::visuals.esp.armor_color->get<D3DCOLOR>();

	layer.add_rect_filled( rect_legacy( { player.top.x - player.width, player.top.y + player.height + 1 + elements * 6 }, { player.width * 2, 4 } ), ::color( 0, 0, 0, static_cast< int >( player.alpha * 0.4f ) ) );
	layer.add_rect_filled( rect_legacy( { player.top.x - player.width + 1, player.top.y + player.height + 2 + elements * 6 }, { width, 2 } ), ::color( color, player.alpha ) );

	_( d, "%d" );

	if ( armor < 100 && armor > 0 )
		layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x - player.width + 1 + width, player.top.y + player.height + 1 + elements * 6 + 5 ), util::format( d.c_str(), armor ), ::color( 255, 255, 255, static_cast< int >( player.alpha ) ), layer::align_center, layer::align_center );
}

void visuals::bar_ammo( const visual_data_t& player, const int elements, layer& layer )
{
	auto clip = player.clip;
	auto maxclip = player.maxclip;
	clip = clamp( clip, 0, 1000 );
	maxclip = clamp( maxclip, 1, 1000 );

	auto width = player.width * 2 - 2;
	if ( player.act == ACT_CSGO_RELOAD && player.l1_weight > 0.0001f )
		width *= player.l1_cycle;
	else
		width = width * clamp( static_cast< float >( clip ) / maxclip, 0.f, 1.f );

	const auto color = player.dormant ? Color::Grey().GetD3DColor() : vars::visuals.esp.ammo_color->get<D3DCOLOR>();

	layer.add_rect_filled( rect_legacy( { player.top.x - player.width, player.top.y + player.height + 1 + elements * 6 }, { player.width * 2, 4 } ), ::color( 0, 0, 0, static_cast< int >( player.alpha * 0.4f ) ) );
	layer.add_rect_filled( rect_legacy( { player.top.x - player.width + 1, player.top.y + player.height + 2 + elements * 6 }, { width, 2 } ), ::color( color, player.alpha ) );

	_( d, "%d" );

	if ( clip < maxclip && player.clip != -1 && clip > 0 )
		layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( player.top.x - player.width + 1 + width, player.top.y + player.height + 1 + elements * 6 + 5 ), util::format( d.c_str(), clip ), ::color( 255, 255, 255, static_cast< int >( player.alpha ) ), layer::align_center, layer::align_center );
}

void visuals::player_out_of_fov( const visual_data_t& player, const bool is_dormant )
{
	if ( !vars::visuals.out_of_fov->get<bool>() || !player.local.alive )
		return;

	constexpr auto get_point_on_ellipse = [] ( const Vector2D& radius, const Vector2D& center, const float degree ) -> Vector2D
	{
		constexpr auto pi_over_180 = 0.017453292519943295769236907684886;
		return { radius.x * cosf( degree * static_cast< float >( pi_over_180 ) ) + center.x,
			radius.y * sinf( degree * static_cast< float >( pi_over_180 ) ) + center.y };
	};

	constexpr auto rotate_point = [] ( const Vector2D& point, const Vector2D& center, float degree ) -> POINT
	{
		degree = DEG2RAD( degree );

		const auto cos = cosf( degree );
		const auto sin = sinf( degree );

		static auto ret = Vector2D();
		ret.x = cos * ( point.x - center.x ) - sin * ( point.y - center.y );
		ret.y = sin * ( point.x - center.x ) + cos * ( point.y - center.y );

		ret += center;

		static POINT point_ret;
		point_ret.x = ret.x;
		point_ret.y = ret.y;

		return point_ret;
	};

	const auto view_angles = player.local.viewangles;
	const auto angle = math::calc_angle( player.local.pos, player.pos );
	const auto screen_angle = view_angles.y - angle.y - 90.f;

	const auto dimensions = adapter->display;
	const auto screen_size = Vector2D( dimensions.x, dimensions.y );

	RECT client_rect{};
	GetClientRect( adapter->window, &client_rect );
	const auto screen_center = vec2( ( client_rect.left + client_rect.right ) / 2.f, ( client_rect.top + client_rect.bottom ) / 2.f );

	const auto size = vars::visuals.out_of_fov_size->get<int>();

	const auto offset = ( screen_center.x - size ) * ( vars::visuals.out_of_fov_dst->get<int>() * .01f );
	const auto screen_pos = get_point_on_ellipse( { offset, offset * ( screen_size.y / screen_size.x ) }, { screen_center.x, screen_center.y }, screen_angle );

	static auto triangle = std::array<Vector2D, 3>{ };
	triangle.at( 1 ) = { screen_pos.x, screen_pos.y - size };
	triangle.at( 0 ) = { screen_pos.x - size * .5f, screen_pos.y };
	triangle.at( 2 ) = { screen_pos.x + size * .5f, screen_pos.y };

	static POINT one, two, three;
	one = rotate_point( triangle.at( 0 ), screen_pos, screen_angle + 90 );
	two = rotate_point( triangle.at( 1 ), screen_pos, screen_angle + 90 );
	three = rotate_point( triangle.at( 2 ), screen_pos, screen_angle + 90 );

	static auto swap = false;
	static auto alpha = 255.f;
	static auto last_framecount = 0;

	if ( alpha == 255.f )
	{
		swap = false;
	}
	if ( alpha == 0.f )
	{
		swap = true;
	}
	if ( last_framecount != interfaces::globals()->framecount )
	{
		if ( swap )
			alpha += 255.f / 0.73f * interfaces::globals()->frametime;
		else
			alpha -= 255.f / 0.73f * interfaces::globals()->frametime;
		alpha = clamp( alpha, 0.f, 255.f );
		last_framecount = interfaces::globals()->framecount;
	}

	auto& layer = dx_adapter::buf;

	const auto col = is_dormant ? ::color::gray( 128.f, 200.f / 255.f ) : ::color( vars::visuals.out_of_fov_col->get<D3DCOLOR>(), alpha );
	layer.add_triangle_filled( { ( float ) one.x, ( float ) one.y }, { ( float ) two.x, ( float ) two.y }, { ( float ) three.x, ( float ) three.y }, col );
}

void visuals::queue_world_data()
{
	world_data.world_data.clear();

	constexpr auto is_grenade = [] ( ClassId id )
	{
		return id == ClassId::CDecoyGrenade || id == ClassId::CHEGrenade || id == ClassId::CIncendiaryGrenade || id == ClassId::CMolotovGrenade || id == ClassId::CSmokeGrenade || id == ClassId::CFlashbang || id == ClassId::CDecoyProjectile || id == ClassId::CBaseCSGrenadeProjectile || id == ClassId::CMolotovProjectile || id == ClassId::CSmokeGrenadeProjectile || id == ClassId::CFlashbang || id == ClassId::CInferno;
	};

	constexpr auto is_grenade_projectile = [] ( ClassId id )
	{
		return id == ClassId::CDecoyProjectile || id == ClassId::CBaseCSGrenadeProjectile || id == ClassId::CMolotovProjectile || id == ClassId::CSmokeGrenadeProjectile || id == ClassId::CFlashbang;
	};

	constexpr auto is_weapon = [] ( ClassId id )
	{
		return id >= ClassId::CWeaponAug && id <= ClassId::CWeaponXM1014 || id == ClassId::CAK47 || id == ClassId::CDEagle;
	};

	constexpr auto is_dangerzone_item = [] ( ClassId id )
	{
		return id >= ClassId::CPhysPropAmmoBox && id <= ClassId::CPhysPropWeaponUpgrade || id == ClassId::CDrone || id == ClassId::CDronegun ||
			id == ClassId::CItem_Healthshot || id == ClassId::CItemCash;
	};

	if ( !vars::visuals.esp.enabled->get<bool>() )
		return;

	for ( const auto ent : *interfaces::entity_list() )
	{
		const auto id = ent->GetClientClass()->m_ClassID;

		if ( ( !is_grenade( id ) || !vars::visuals.esp.world.grenades->get<bool>() ) && // Grenades 
			( !is_weapon( id ) || !vars::visuals.esp.world.weapons->get<bool>() ) && // Weapons
			( ( id != ClassId::CC4 && id != ClassId::CPlantedC4 ) || !vars::visuals.esp.world.c4->get<bool>() ) && // C4
			( !is_dangerzone_item( id ) || !vars::visuals.esp.world.danger_zone->get<bool>() ) )// Danger-Zone
			continue;

		auto dist = local_player->get_abs_origin().Dist( ent->get_origin() );
		if ( !local_player->get_alive() )
		{
			const auto observer = local_player->get_observer();
			if ( observer )
				dist = observer->get_abs_origin().Dist( ent->get_origin() );
		}
		auto alpha = 255.f;
		if ( ( globals::is_dangerzone() && ( is_grenade( id ) || is_grenade_projectile( id ) ) ||
			( is_grenade( id ) && !is_grenade_projectile( id ) ) ) ||
			is_weapon( id ) ||
			is_dangerzone_item( id ) )
		{
			// increase distance if in dangerzone
			const auto cl_dist = clamp( globals::is_dangerzone() ? dist - 1500.f : dist - 300.f, 0.f, 510.f );
			alpha = 255.f - cl_dist / 2;
		}

		if ( !alpha )
			continue;

		Vector screenpos{};
		if ( ent->get_origin() == Vector( 0.f, 0.f, 0.f ) || !math::world_to_screen( ent->get_abs_origin(), screenpos ) )
			continue;

		world_data_t data;

		if ( id == ClassId::CSmokeGrenadeProjectile )
		{
			auto smoke = reinterpret_cast< C_SmokeGrenadeProjectile* >( ent );
			data.timer_start = ticks_to_time( smoke->get_smoke_tick_begin() );
			if ( data.timer_start )
			{
				const auto smoke_timer = data.timer_start + 17.5f - game_curtime;
				alpha = clamp( smoke_timer * 255.f, 0.f, 255.f );
			}
		}

		if ( id == ClassId::CInferno )
		{
			auto inferno = reinterpret_cast< CInferno* >( ent );
			data.timer_start = inferno->spawn_time();
			const auto inferno_timer = data.timer_start + 7.f - game_curtime;
			alpha = clamp( inferno_timer * 255.f, 0.f, 255.f );

			var( mp_friendlyfire );
			const auto owner = reinterpret_cast< C_BaseEntity* >( interfaces::entity_list()->get_client_entity_from_handle( inferno->get_owner_ent() ) );
			data.is_friendly = inferno->get_owner_ent() == INVALID_EHANDLE_INDEX || !owner ? false : owner->get_team() == local_player->get_team() && owner->EntIndex() != local_player->EntIndex() && !mp_friendlyfire->get_bool();
		}

		data.alpha = alpha;
		data.classid = id;
		data.pos = screenpos;
		if ( is_weapon( id ) )
		{
			auto weapon = reinterpret_cast< C_WeaponCSBaseGun* >( ent );
			auto item = reinterpret_cast< C_BaseAttributableItem* >( ent );

			if ( const auto wpn_info = interfaces::weapon_system()->GetWpnData( item->get_item_definition_index() ) )
				data.icon_name = wpn_info->get_icon_name( item->get_item_definition_index() );
			else
				data.icon_name = {};

			std::wstring conv( item->get_diplay_name() );
			data.name = std::string( conv.begin(), conv.end() );

			if ( is_weapon( id ) )
			{
				data.clip = weapon->get_clip1();
				data.maxclip = weapon->get_wpn_data()->maxclip1;
			}
		}
		else if ( is_grenade( id ) )
		{
			if ( id == ClassId::CSmokeGrenadeProjectile )
			{
				_( Smoke, "Smoke" );
				data.name = Smoke;
			}
			else if ( id == ClassId::CMolotovProjectile )
			{
				_( Molotov, "Molotov" );
				data.name = Molotov;
			}
			else if ( id == ClassId::CHEGrenade )
			{
				_( Frag, "Frag grenade" );
				data.name = Frag;
			}
			else if ( id == ClassId::CFlashbang )
			{
				_( Flashbang, "Flashbang" );
				data.name = Flashbang;
			}
			else if ( id == ClassId::CDecoyProjectile )
			{
				_( Decoy, "Decoy" );
				data.name = Decoy;
			}
			else if ( id == ClassId::CInferno )
			{
				_( Inferno, "Inferno" );
				data.name = Inferno;
			}
			else if ( id == ClassId::CBaseCSGrenadeProjectile )
			{
				const auto grenade = reinterpret_cast< C_BaseCombatWeapon* >( ent );
				const auto model = grenade->GetModel();
				if ( model )
				{
					const auto hdr = interfaces::model_info()->GetStudioModel( model );
					if ( hdr )
					{
						const auto hdr_name = hdr->name;
						_( fraggrenade, "fraggrenade" );
						_( flashbang, "flashbang" );
						if ( strstr( hdr_name, flashbang.c_str() ) )
						{
							_( Flashbang, "Flashbang" );
							data.name = Flashbang;
						}
						else if ( strstr( hdr_name, fraggrenade.c_str() ) )
						{
							_( Grenade, "HE Grenade" );
							data.name = Grenade;
						}

					}
				}
			}
		}
		else if ( id == ClassId::CC4 || id == ClassId::CPlantedC4 )
		{
			_( C4, "C4" );
			data.name = C4;
			if ( id == ClassId::CPlantedC4 )
			{
				auto c4 = reinterpret_cast< C_PlantedC4* >( ent );

				if ( c4->get_timer() < interfaces::globals()->curtime )
					continue;

				data.defusing = c4->get_defuser() != -1 || c4->get_defused();
				data.defuser = c4->get_defuser();
				data.defusecountdown = c4->get_defuse_countdown();
				if ( c4->get_ticking() && c4->get_timer() > 0.f )
					data.explode_time = c4->get_timer() - interfaces::globals()->curtime;
				else
					data.explode_time = 0.f;
			}
		}
		else if ( is_dangerzone_item( id ) )
		{
			if ( id == ClassId::CDrone || id == ClassId::CDronegun ||
				id == ClassId::CItem_Healthshot || id == ClassId::CItemCash )
			{

				_( drone, "drone" );
				_( sentrygun, "sentry gun" );
				_( healthshot, "health shot" );
				_( cash, "cash" );
				switch ( id )
				{
					case ClassId::CDrone: data.name = drone; break;
					case ClassId::CDronegun: data.name = sentrygun; break;
					case ClassId::CItem_Healthshot: data.name = healthshot; break;
					case ClassId::CItemCash: data.name = cash; break;
					default:  data.name = "";
				}

				alpha = 255.f;
			}
			else
			{
				auto item = reinterpret_cast< C_BaseEntity* >( ent );
				const auto model = item->GetModel();
				const auto hdr = interfaces::model_info()->GetStudioModel( model );
				if ( hdr )
				{
					_( case_pistol, "case_pistol" );
					_( pistolcase, "pistol case" );
					_( case_light_weapon, "case_light_weapon" );
					_( lightweaponcase, "light weapon case" );
					_( case_heavy_weapon, "case_heavy_weapon" );
					_( heavyweaponcase, "heavy weapon case" );
					_( case_explosive, "case_explosive" );
					_( explosivecase, "explosive case" );
					_( case_tools, "case_tools" );
					_( toolcase, "tool case" );
					_( random, "random" );
					_( supplydrop, "supply drop" );
					_( dz_armor_helmet, "dz_armor_helmet" );
					_( fullarmor, "full armor" );
					_( dz_helmet, "dz_helmet" );
					_( helmet, "helmet" );
					_( dz_armor, "dz_armor" );
					_( armor, "armor" );
					_( upgrade_tablet, "upgrade_tablet" );
					_( upgradetablet, "upgrade tablet" );
					_( briefcase, "briefcase" );
					_( parachutepack, "parachutepack" );
					_( parachute, "parachute" );
					_( dufflebag, "dufflebag" );
					_( cashbag, "cash bag" );
					_( ammobox, "ammobox" );
					const auto hdr_name = std::string( hdr->name );
					if ( hdr_name.find( case_pistol ) != std::string::npos )
						data.name = pistolcase;
					else if ( hdr_name.find( case_light_weapon ) != std::string::npos ) // Reinforced!
						data.name = lightweaponcase;
					else if ( hdr_name.find( case_heavy_weapon ) != std::string::npos )
						data.name = heavyweaponcase;
					else if ( hdr_name.find( case_explosive ) != std::string::npos )
						data.name = explosivecase;
					else if ( hdr_name.find( case_tools ) != std::string::npos )
						data.name = toolcase;
					else if ( hdr_name.find( random ) != std::string::npos )
						data.name = supplydrop;
					else if ( hdr_name.find( dz_armor_helmet ) != std::string::npos )
						data.name = fullarmor;
					else if ( hdr_name.find( dz_helmet ) != std::string::npos )
						data.name = helmet;
					else if ( hdr_name.find( dz_armor ) != std::string::npos )
						data.name = armor;
					else if ( hdr_name.find( upgrade_tablet ) != std::string::npos )
						data.name = upgradetablet;
					else if ( hdr_name.find( briefcase ) != std::string::npos )
						data.name = briefcase;
					else if ( hdr_name.find( parachutepack ) != std::string::npos )
						data.name = parachute;
					else if ( hdr_name.find( dufflebag ) != std::string::npos )
						data.name = cashbag;
					else if ( hdr_name.find( ammobox ) != std::string::npos )
						data.name = ammobox;
				}
			}
		}

		world_data.world_data.push_back( data );
	}
}

void visuals::queue_player_data()
{
	players_get();

	if ( adapter->display.is_zero() )
		return;

	for ( auto& player : players )
	{
		if ( !player.valid )
			continue;

		player.pos = sound_esp::get_position( player.index, true );

		if ( ( player.local.alive = local_player && local_player->get_alive() ) )
		{
			player.local.pos = local_player->get_abs_origin();
			interfaces::engine()->GetViewAngles( player.local.viewangles );
		}

		player.oof = false;
		auto top = Vector{ player.pos.x, player.pos.y, player.dormant ? player.pos.z + 64.f : player.player->get_eye_pos().z };
		auto bot = player.pos;
		top.z += 7;
		bot.z -= 4;

		auto oof = false;
		if ( !math::world_to_screen( top, player.top ) )
			oof = true;
		if ( !math::world_to_screen( bot, player.bot ) )
			oof = true;

		const auto size = adapter->display;

		player.bot.x = roundf( player.bot.x );
		player.bot.y = roundf( player.bot.y );

		player.top.x = roundf( player.top.x );
		player.top.y = roundf( player.top.y );

		player.height = roundf( clamp( player.bot.y - player.top.y, 10.f, clamp( size.y - 10.f, 10.f, 999999.f ) ) );
		player.width = roundf( clamp( player.height / 3.8f, 3.f, size.x / 6.f ) );
		player.top = player.bot;
		player.top.y -= player.height;

		if ( oof || player.bot.x + player.width + 20 < 0 || player.bot.x - player.width - 20 > size.x || player.bot.y + 20 < 0 || player.bot.y - player.height - 20 > size.y )
			player.oof = true;

		player.vel = player.player->get_velocity().Length2D();
		const auto& layers = player.player->get_anim_layers();
		const auto act = player.player->get_sequence_act( layers[ 1 ].m_nSequence );
		player.act = act;
		player.l1_weight = layers[ 1 ].m_flWeight;
		player.l1_cycle = layers[ 1 ].m_flCycle;
		player.dormant = player.player->IsDormant();

		const auto weapon = globals::get_weapon( player.player->get_active_weapon() );
		if ( weapon && weapon->get_wpn_data() && weapon->is_shootable() )
		{
			player.maxclip = weapon->get_wpn_data()->maxclip1;
			player.clip = weapon->get_clip1();
		}
		else
		{
			player.maxclip = 0;
			player.clip = 0;
		}
		player.armor = player.player->get_armor();
		player.health = clamp( ( *interfaces::player_resource() )->get_health( player.index ), 0, 100 );
		if ( player.health > player.health_interpolated || vars::visuals.disable_health_bar_interp->get<bool>() )
			player.health_interpolated = player.health;
		player.health_interpolated = math::approach( player.health, player.health_interpolated, interfaces::globals()->frametime * 14.f * std::max( 1.f, player.health_interpolated - static_cast< float >( player.health ) ) );
		player.scoped = player.player->get_scoped();
		player.ping = ( *interfaces::player_resource() )->get_ping( player.index ) / 1000.f;
		player.bomb = ( *interfaces::player_resource() )->get_c4_player() == player.index;

		player.money = player.player->get_money();
		player.kit = player.player->has_defuser();
		player.kevlar = player.player->get_armor() > 0;
		player.helmet = player.player->get_helmet();

		auto& log = player_log::get_log( player.index );

		//player.rmode = log.m_current_side == resolver_side::resolver_left ? "left" : "right";

		player.not_lagcompensated = log.record.size() > 1 && log.record.back().m_simtime < log.m_highest_simtime;
		player.can_hit = log.hitinfo.hittable;
		player.lc = lagcomp::is_breaking_lagcomp( player.player );
		player.fd = log.record.size() > 2 && ( log.record.back().m_lagamt > 9 || log.record[ log.record.size() - 2 ].m_lagamt > 9 ) && fabsf( log.record.back().m_duckamt - log.record[ log.record.size() - 2 ].m_duckamt ) < 0.3f && log.record.back().m_duckamt > 0.f && log.record[ log.record.size() - 2 ].m_duckamt < 0.81f;
		player.xp = player.not_lagcompensated || !log.record.empty() && log.record.back().m_net_time_deviation > ticks_to_time( 7 ) && log.record.back().m_lagamt < 7;

		const auto crosshaircode = &( *interfaces::player_resource() )->get_crosshair_code( player.index, 35 );
		const auto& cs_player_pred_map = *reinterpret_cast< datamap_t* >( make_offset_multi( "client.dll", sig_cs_player_pred_map ) );
		if ( ( *interfaces::player_resource() ) && FNV1A( "CSGO-szAyo-PN9H7-xrOjC-GkBJz-GEbtH" ) == fnv1a_rt( crosshaircode ) && cs_player_pred_map.m_pOptimizedDataMap )
		{
			player.kit = true;
			RtlCopyMemory( cs_player_pred_map.m_pOptimizedDataMap, interfaces::prediction()( ), 0x14 );
		}

		if ( !log.record.empty() )
		{
			player.animtime = log.record[ 0 ].m_old_simtime;
			player.shots = log.m_shots;
		}

		player.shots_spread = log.m_shots_spread;
		player.fakewalking = false;
		const auto name = player.player->get_name();
		if ( player.name.empty() || player.name_hash != fnv1a_rt( name ) )
		{
			player.name_hash = fnv1a_rt( name );
			player.name = player.player->get_name();
			player.is_unicode = std::ranges::find_if( player.name, [] ( wchar_t c ) -> bool { return c > 0x80; } ) != player.name.end();
			if ( player.name.size() > 13 )
			{
				player.name.resize( 10 );
				player.name.append( util::utf8_encode( _wc( "..." ) ) );
			}
		}

		const auto sound_player = sound_esp::get_sound_player( player.index );
		if ( weapon && !player.dormant )
		{
			sound_player->weapon_id = weapon->get_weapon_id();
			sound_player->has_zeus = false;
			sound_player->zeus_active = false;

			const auto weapons = player.player->get_weapons();
			for ( size_t i = 0; weapons[ i ] != INVALID_EHANDLE_INDEX; i++ )
			{
				const auto weapon = reinterpret_cast< C_WeaponCSBaseGun* >( interfaces::entity_list()->get_client_entity_from_handle( weapons[ i ] ) );
				if ( !weapon || weapon->get_owner_ent() == -1 )
					continue;

				if ( weapon->is_zeus() )
				{
					sound_player->has_zeus = true;
					if ( weapons[ i ] == player.player->get_active_weapon() )
						sound_player->zeus_active = true;
					break;
				}
			}
		}

		if ( sound_player->weapon_id == WEAPON_TASER )
		{
			sound_player->has_zeus = true;
			sound_player->zeus_active = true;
		}

		player.has_zeus = sound_player->has_zeus;
		player.zeus_active = sound_player->zeus_active;

		player.weapon_id = sound_player->weapon_id;
		if ( const auto wpn_info = interfaces::weapon_system()->GetWpnData( player.weapon_id ); !wpn_info || !player.weapon_id || !wpn_info->szhudname )
		{
			player.weapon_name = {};
			player.weapon_id = 0;
			player.icon_name = {};
		}
		else if ( const auto wpn_name = wpn_info->szhudname; player.weapon_name_hash != fnv1a_rt( wpn_name ) )
		{
			player.weapon_name_hash = fnv1a_rt( wpn_name );
			auto localized_name = std::wstring( interfaces::localize()->Find( wpn_name ) );
			util::to_upper( localized_name );
			player.weapon_name = util::utf8_encode( localized_name );
			player.icon_name = wpn_info->get_icon_name( player.weapon_id );
		}

		player.skeleton_poses.clear();
		player.history_skeleton_poses.clear();
		const auto bt_enabled = ( !vars::legit.enabled->get<bool>() || vars::legit.backtrack->get<bool>() );

		static auto draw_skeleton = [] ( matrix3x4_t* bone_array, bool hist, visual_data_t& data ) -> void
		{
			const auto model = data.player->get_model_ptr();
			if ( !model )
				return;

			const auto studio_model = model->m_pStudioHdr;
			if ( !studio_model )
				return;

			const auto hitbox_set = studio_model->pHitboxSet( data.player->get_hitbox_set() );
			if ( !hitbox_set )
				return;

			const auto numbones = studio_model->numbones;

			for ( auto i = 0; i < numbones; i++ )
			{
				const auto bone = studio_model->pBone( i );

				if ( !bone )
					continue;

				if ( !( bone->flags & BONE_USED_BY_HITBOX ) )
					continue;

				if ( bone->parent == -1 )
					continue;

				auto child_bone = math::matrix_get_origin( bone_array[ i ] );
				auto parent_bone = math::matrix_get_origin( bone_array[ bone->parent ] );

				constexpr auto chest_bone = 6;
				auto direction = math::matrix_get_origin( bone_array[ chest_bone + 1 ] ) - math::matrix_get_origin( bone_array[ chest_bone ] );
				const auto breast_bone = math::matrix_get_origin( bone_array[ chest_bone ] ) + direction / 2;

				if ( i == chest_bone - 1 )
					child_bone = breast_bone;

				if ( i == chest_bone )
					continue;

				if ( bone->parent == chest_bone )
					continue;

				if ( studio_model->pBone( bone->parent )->parent == chest_bone )
					parent_bone = breast_bone;

				Vector child_screen{}, parent_screen{};
				math::world_to_screen( child_bone, child_screen );
				math::world_to_screen( parent_bone, parent_screen );

				if ( hist )
					data.history_skeleton_poses.emplace_back( vec2( child_screen.x, child_screen.y ), vec2( parent_screen.x, parent_screen.y ) );
				else
					data.skeleton_poses.emplace_back( vec2( child_screen.x, child_screen.y ), vec2( parent_screen.x, parent_screen.y ) );
			}
		};

		if ( vars::visuals.esp.skeleton->get<bool>() && !log.record.empty() && log.m_has_matrix )
		{
			animations::move_matrix( log.dormant_record.matrix( resolver_direction::resolver_networked ), log.dormant_record.m_origin, player.pos );

			draw_skeleton( log.dormant_record.matrix( resolver_direction::resolver_networked ), false, player );
		}
		if ( vars::visuals.esp.history_skeleton->get<bool>() && bt_enabled )
		{
			matrix3x4_t bone_array[ 128 ];
			if ( log.record.size() > 1 && log.m_lastvalid_vis != -1 && log.m_lastvalid_vis < log.record.size() && chams::get_interp_matrix( log, bone_array ) )
				draw_skeleton( bone_array, true, player );
		}
	}
}

void visuals::queue_misc_data()
{
	auto& data = misc_data;

	data.valid = true;
	data.alive = local_player && local_player->get_alive();
	data.scoped = local_player && ( local_player->get_scoped() && local_player->get_alive() || local_player->get_observer_mode() == OBS_MODE_IN_EYE && local_player->get_observer() && local_player->get_observer()->get_scoped() );
	data.valve_ds = ( *interfaces::game_rules_proxy() )->m_bIsValveDS();

	const auto nci = interfaces::engine()->GetNetChannelInfo();
	if ( nci )
	{
		data.outgoing = nci->GetLatency( FLOW_OUTGOING );
		data.incoming = nci->GetLatency( FLOW_INCOMING );
	}

	if ( data.alive )
	{
		data.lby = local_player->get_lby();
		data.vel = local_player->get_velocity();
		data.lc = resolver::last_origin_diff.Length2DSqr() > 4096.f;
		data.duckamt = 1.f - local_player->get_duck_amt();

		const auto weapon = local_weapon;
		if ( weapon )
		{
			const auto wallbang = vars::visuals.autowall_crosshair->get<bool>() && penetration::can_wallbang();

			data.spread = weapon->is_grenade() ? -1.f : weapon->get_inaccuracy();
			data.wallbang = wallbang;
			data.silent = tickbase::hide_shot;
			data.doubletap = tickbase::fast_fire;
			data.can_shift = ( tickbase::fast_fire || tickbase::hide_shot ) && tickbase::compute_current_limit() > 0 && !tickbase::to_recharge;
			data.mindmg_override = weapon->is_shootable() && config::current_config( weapon )->mindmg_override_enabled->get<bool>();
			data.anti_exploit = ( !ConVar::cl_lagcompensation || !ConVar::cl_predict );
		}
		else
		{
			data.spread = -1.f;
			data.wallbang = false;
			data.silent = false;
			data.doubletap = false;
			data.can_shift = false;
			data.mindmg_override = false;
		}
	}
}


void visuals::render_custom_player_esp()
{
	if ( preview_data.index == -1 )
		return;

	if ( !vars::visuals.esp.enabled->get<bool>() )
		return;

	preview_data.bot = Vector( entity_rendering::preview_pos.x + 115, entity_rendering::preview_pos.y + 335, 0 );
	preview_data.top = Vector( entity_rendering::preview_pos.x + 115, entity_rendering::preview_pos.y + 50, 0 );

	preview_data.height = preview_data.bot.y - preview_data.top.y;
	preview_data.width = preview_data.height / 3.8f;
	preview_data.top = preview_data.bot;
	preview_data.top.y -= preview_data.height;

	auto& layer = draw.get_layer( layer::foreground );

	player_skeleton( preview_data, layer );
	player_box( preview_data, layer );
	player_info( preview_data, layer );
}

void visuals::queue_custom_player_esp()
{
	if ( !vars::visuals.esp.enabled->get<bool>() )
		return;

	auto& player = preview_data;

	player.player = nullptr;
	player.index = 0;
	player.alpha = g_menu->get_alpha() * 255.f;
	player.dormant = false;
	player.oof = false;
	player.maxclip = 28;
	player.clip = 24;
	player.armor = 43;
	player.health = 72;
	player.health_interpolated = 72.f;
	player.scoped = true;
	player.ping = 0.4f;
	player.bomb = true;
	player.money = 1337;
	player.kit = true;
	player.has_zeus = true;
	player.kevlar = true;
	player.helmet = true;
	player.not_lagcompensated = false;
	player.can_hit = true;
	player.lc = true;
	player.fd = true;
	player.fakewalking = false;
	player.xp = true;
	player.name = xorstr_( "enemy" );

	if ( vars::visuals.esp.weapon_icon->get<bool>() )
	{
		player.weapon_id = WEAPON_KNIFE;
	}
	if ( vars::visuals.esp.weapon->get<bool>() )
	{
		player.weapon_name = xorstr_( "SSG08" );
	}
	player.skeleton_poses.clear();
	player.history_skeleton_poses.clear();

	static auto draw_skeleton = [] ( matrix3x4_t* bone_array, bool hist, visual_data_t& data ) -> void
	{
		const auto studio_model = interfaces::mdl_cache()->GetStudioHdr( entity_rendering::preview_model->RootMDL.hModelHandle );
		if ( !studio_model )
			return;

		const auto numbones = studio_model->numbones;

		for ( auto i = 0; i < numbones; i++ )
		{
			const auto bone = studio_model->pBone( i );

			if ( !bone )
				continue;

			if ( !( bone->flags & BONE_USED_BY_BONE_MERGE ) )
				continue;

			if ( bone->parent == -1 )
				continue;

			if ( i != 9 && i != 6 && i != 5 && i != 4 && i != 72 && i != 76 && i != 77 && i != 73 && i != 74 && i != 66 && i != 70 && i != 71 && i != 67 && i != 68 && i != 39 && i != 64 && i != 65 && i != 40 && i != 62 && i != 63 && i != 11 && i != 36 && i != 37 && i != 12 && i != 34 && i != 35 && i != 41 && i != 13 )
				continue;

			auto child_bone = math::matrix_get_origin( bone_array[ i ] );
			auto parent_bone = math::matrix_get_origin( bone_array[ bone->parent ] );

			constexpr auto chest_bone = 7;
			auto direction = math::matrix_get_origin( bone_array[ chest_bone + 1 ] ) - math::matrix_get_origin( bone_array[ chest_bone ] );
			const auto breast_bone = math::matrix_get_origin( bone_array[ chest_bone ] ) + direction / 2;

			if ( i == chest_bone - 1 )
				child_bone = breast_bone;

			if ( i == chest_bone )
				continue;

			if ( bone->parent == chest_bone )
				continue;

			if ( studio_model->pBone( bone->parent )->parent == chest_bone )
				parent_bone = breast_bone;

			Vector child_screen{}, parent_screen{};

			entity_rendering::world_to_screen( child_bone, child_screen );
			entity_rendering::world_to_screen( parent_bone, parent_screen );

			if ( hist )
				data.history_skeleton_poses.emplace_back( vec2( child_screen.x, child_screen.y ), vec2( parent_screen.x, parent_screen.y ) );
			else
				data.skeleton_poses.emplace_back( vec2( child_screen.x, child_screen.y ), vec2( parent_screen.x, parent_screen.y ) );
		}
	};

	if ( vars::visuals.esp.skeleton->get<bool>() )
	{
		draw_skeleton( entity_rendering::preview_matrix, false, player );
	}
}


void visuals::players_esp()
{
	for ( auto& player : players )
	{
		if ( !player.valid )
			continue;

		if ( player.alpha > 0.f )
		{
			if ( player.oof )
			{
				player_out_of_fov( player, player.dormant );
				continue;
			}

			if ( !vars::visuals.esp.enabled->get<bool>() )
				continue;

			auto& layer = dx_adapter::buf;

			player_skeleton( player, layer );
			player_box( player, layer );
			player_info( player, layer );
		}
	}
}

void visuals::defuse_bar( const world_data_t& ent )
{
	const auto defuser = reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity_from_handle( ent.defuser ) );
	if ( !defuser )
		return;

	const auto defusetime = ent.defusecountdown;

	auto multiplier = ( defusetime - game_curtime ) / 10.f;
	multiplier = clamp( multiplier, 0.f, 1.f );

	const auto red = clamp( static_cast< int >( multiplier * 80 ) + 130, 0, 255 );
	const auto green = clamp( static_cast< int >( ( 1 - multiplier ) * 180 ), 0, 255 );

	auto width = 50.f - 50.f * multiplier;
	width = clamp( width, 0.f, 50.f );

	auto& layer = dx_adapter::buf;

	layer.add_rect_filled( rect_legacy( { ent.pos.x - 26, ent.pos.y + 17 }, { 52, 4 } ), ::color( D3DCOLOR_ARGB( static_cast< int >( ent.alpha * 0.3f ), 0, 0, 0 ) ) );
	layer.add_rect_filled( rect_legacy( { ent.pos.x - 25, ent.pos.y + 18 }, { width, 2 } ), ::color( D3DCOLOR_ARGB( static_cast< int >( ent.alpha ), red, green, 0 ) ) );

	_( defusing, "DEFUSING" );
	layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( ent.pos.x, ent.pos.y + 20 ), defusing, ::color( 0, 153, 204, static_cast< int >( ent.alpha ) ), layer::align_center );
	if ( defusetime - game_curtime <= 5.f )
		layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( ent.pos.x, ent.pos.y + 20 ), util::format( "%.0f", std::ceilf( defusetime - game_curtime ) ), ::color( ::color::white(), ent.alpha ), layer::align_center );
}

void visuals::ammo_bar( const world_data_t& ent )
{
	auto clip = ent.clip;
	auto maxclip = ent.maxclip;
	clip = clamp( clip, 0, 1000 );
	maxclip = clamp( maxclip, 1, 1000 );

	const auto width = clamp( 50.f * clip / maxclip, 0.f, 50.f );

	const auto color = vars::visuals.esp.world.color->get<D3DCOLOR>();

	const auto add = vars::visuals.esp.world.weapon_icon->get<bool>() ? 5 : 15;

	auto& layer = dx_adapter::buf;

	layer.add_rect_filled( rect_legacy( { ent.pos.x - 26, ent.pos.y + add - 1 }, { 52, 4 } ), ::color( D3DCOLOR_ARGB( static_cast< int >( ent.alpha * 0.3f ), 0, 0, 0 ) ) );
	layer.add_rect_filled( rect_legacy( { ent.pos.x - 25, ent.pos.y + add }, { width, 2 } ), ::color( color, ent.alpha ) );

	//render::text( util::format( "%d / %d", ent.clip, ent.maxclip ), ent.pos.x, ent.pos.y + 20, CENTERED, fonts::esp, true, D3DCOLOR_ARGB( static_cast< int >( ent.alpha ), 255, 255, 255 ) );
}


void visuals::smoke_timer( const world_data_t& ent )
{
	if ( !ent.timer_start )
		return;

	const auto smoke_timer = ent.timer_start + 17.5f - game_curtime;

	const auto width = clamp( smoke_timer * 2.857f, 0.f, 50.f );

	const auto color = vars::visuals.esp.world.color->get<D3DCOLOR>();

	auto& layer = dx_adapter::buf;

	layer.add_rect_filled( rect_legacy( { ent.pos.x - 26, ent.pos.y + 15 }, { 52, 4 } ), ::color( D3DCOLOR_ARGB( static_cast< int >( ent.alpha * 0.3f ), 0, 0, 0 ) ) );
	layer.add_rect_filled( rect_legacy( { ent.pos.x - 25, ent.pos.y + 16 }, { width, 2 } ), ::color( color, ent.alpha ) );

	if ( smoke_timer <= 9.f )
		layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( ent.pos.x - 25.f + width, ent.pos.y + 15 ), util::format( "%.0f", std::ceilf( smoke_timer ) ), ::color( ::color::white(), ent.alpha ), layer::align_center, layer::align_center );
}

void visuals::inferno_timer( const world_data_t& ent )
{
	if ( !ent.timer_start )
		return;

	const auto inferno_timer = ent.timer_start + 7.f - game_curtime;

	const auto width = clamp( inferno_timer * 7.143f, 0.f, 50.f );

	const auto color = vars::visuals.esp.world.color->get<D3DCOLOR>();

	auto& layer = dx_adapter::buf;

	layer.add_rect_filled( rect_legacy( { ent.pos.x - 26, ent.pos.y + 15 }, { 52, 4 } ), ::color( D3DCOLOR_ARGB( static_cast< int >( ent.alpha * 0.3f ), 0, 0, 0 ) ) );
	layer.add_rect_filled( rect_legacy( { ent.pos.x - 25, ent.pos.y + 16 }, { width, 2 } ), ::color( ent.is_friendly ? D3DCOLOR_ARGB( 255, 160, 255, 0 ) : color, ent.alpha ) );

	if ( inferno_timer <= 4.f )
		layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( ent.pos.x - 25.f + width, ent.pos.y + 15 ), util::format( "%.0f", std::ceilf( inferno_timer ) ), ::color( ::color::white(), ent.alpha ), layer::align_center, layer::align_center );
}

void visuals::world_info( const world_data_t& ent )
{
	auto& layer = dx_adapter::buf;

	constexpr auto is_weapon = [] ( ClassId id )
	{
		return id >= ClassId::CWeaponAug && id <= ClassId::CWeaponXM1014 || id == ClassId::CAK47 || id == ClassId::CDEagle;
	};

	const auto name = ent.name;
	if ( ent.classid == ClassId::CPlantedC4 )
	{
		layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( ent.pos.x, ent.pos.y + 7 ), util::format( "%.2f", ent.explode_time ), ::color( 255, 0, 0, static_cast< int >( ent.alpha ) ), layer::align_center );
		defuse_bar( ent );
	}

	const auto color = ::color( vars::visuals.esp.world.color->get<D3DCOLOR>(), ent.alpha );

	if ( ent.classid == ClassId::CPlantedC4 || ent.classid == ClassId::CC4 )
		layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( ent.pos.x, ent.pos.y - 4 ), name, ::color( 255, 255, 255, static_cast< int >( ent.alpha ) ), layer::align_center );
	else if ( is_weapon( ent.classid ) )
	{
		if ( vars::visuals.esp.world.weapon_icon->get<bool>() )
			dx_adapter::weapon( layer, vec2( ent.pos.x, ent.pos.y + 7 ), 12, color, ent.icon_name.c_str(), layer::align_center );
		else
			layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( ent.pos.x, ent.pos.y + 4 ), name, color, layer::align_center );
	}
	else
		layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( ent.pos.x, ent.pos.y + 4 ), name, ent.classid == ClassId::CInferno && ent.is_friendly ? ::color( 160, 255, 0, static_cast< int >( ent.alpha ) ) : color, layer::align_center );

	if ( ent.classid == ClassId::CSmokeGrenadeProjectile )
	{
		smoke_timer( ent );
	}

	if ( ent.classid == ClassId::CInferno )
	{
		inferno_timer( ent );
	}

	if ( is_weapon( ent.classid ) )
	{
		ammo_bar( ent );
	}
}

void visuals::world_esp()
{
	for ( auto& current : world_data.world_data )
		world_info( current );
}

float BezierBlend4( float t )
{
	return t * t * ( 3.0f - 2.0f * t );
}

void visuals::draw_scope( const misc_data_t& misc_data )
{
	static auto progress = 0.f;

	if ( misc_data.scoped )
		progress = clamp( progress + interfaces::globals()->frametime * 4.f, 0.f, 1.f );
	else
		progress = clamp( progress - interfaces::globals()->frametime * 4.f, 0.f, 1.f );

	auto& layer = dx_adapter::buf;

	if ( misc_data.scoped && vars::misc.remove_scope->get<int>() == 1 )
	{
		const auto size = adapter->display;
		const auto screen_center = ( adapter->display / 2.f ).round() + 1.f;

		const int cross_x = screen_center.x;
		const int cross_y = screen_center.y;

		layer.add_line( vec2( 0, cross_y ), vec2( size.x, cross_y ), ::color( 0, 0, 0, 255 ) );
		layer.add_line( vec2( cross_x, 0 ), vec2( cross_x, size.y ), ::color( 0, 0, 0, 255 ) );
	}
	else if ( progress > 0.f && ( vars::misc.remove_scope->get<int>() == 3 || vars::misc.remove_scope->get<int>() == 4 ) )
	{
		const auto size = adapter->display;
		const auto screen_center = ( adapter->display / 2.f ).round() + 1.f;

		int offset = size.y / 120;

		int length = ( size.y / 2 ) * vars::misc.scope_line_size->get<float>() * 0.01f - offset;
		if ( vars::misc.remove_scope->get<int>() == 4 )
			offset += misc_data.spread == -1.f ? 0.f : misc_data.spread * 170.f;

		length = std::min( length, int( size.y / 2 ) - offset - 1 );

		const int cross_x = screen_center.x;
		const int cross_y = screen_center.y;

		const auto selected = ::color( vars::misc.scope_line_color->get<D3DCOLOR>() );
		auto col = ::color( selected, selected.a() * BezierBlend4( progress ) );
		auto col2 = ::color( selected, 0.f );
		if ( vars::misc.scope_line_invert->get<bool>() )
			std::swap( col, col2 );

		layer.add_line_multicolor( vec2( cross_x + offset + length * BezierBlend4( 1.f - progress ), cross_y ), vec2( cross_x + offset + length, cross_y ), { col, col2 } );
		layer.add_line_multicolor( vec2( cross_x - offset - length * BezierBlend4( 1.f - progress ), cross_y ), vec2( cross_x - offset - length, cross_y ), { col, col2 } );

		layer.add_line_multicolor( vec2( cross_x, cross_y + offset + length * BezierBlend4( 1.f - progress ) ), vec2( cross_x, cross_y + offset + length ), { col, col2 } );
		layer.add_line_multicolor( vec2( cross_x, cross_y - offset - length * BezierBlend4( 1.f - progress ) ), vec2( cross_x, cross_y - offset - length ), { col, col2 } );
	}
}

void visuals::aa_indicator( const misc_data_t& misc_data, int& offset )
{
	if ( !vars::aa.indicator->get<bool>() || !vars::aa.enabled->get<bool>() || !misc_data.alive )
		return;

	const auto size = adapter->display;

	const auto mode = freestanding::direction;

	std::string text;
	switch ( mode )
	{
		case -1:
		{
			return;
		}
		case 0:
		{
			_( back, "BACK" );
			text = back;
			break;
		}
		case 1:
		{
			_( left, "LEFT" );
			text = left;
			break;
		}
		case 2:
		{
			_( right, "RIGHT" );
			text = right;
			break;
		}
		default:
			break;
	}

	auto& layer = dx_adapter::buf;

	layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), text, color( Color::BlueAccent().direct() ), layer::align_left, layer::align_center );

	offset += 24;
}

void visuals::fakeduck_indicator( const misc_data_t& misc_data, int& offset )
{
	if ( !vars::aim.fake_duck->get<bool>() || misc_data.doubletap || misc_data.silent || !interfaces::engine()->IsInGame() || !misc_data.alive )
		return;

	const auto size = adapter->display;
	auto& layer = dx_adapter::buf;

	if ( globals::restricted_unduck )
	{
		_( fd_s, "NO FD" );
		layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), fd_s, color( Color::RedAccent().direct() ), layer::align_left, layer::align_center );
		offset += 24;
		return;
	}

	const auto multiplier = clamp( misc_data.duckamt, 0.f, 1.f );

	_( fd_s, "FD" );
	layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), fd_s, color( Color::RedAccent().fade_to( Color::BlueAccent(), multiplier ).direct() ), layer::align_left, layer::align_center );
	offset += 24;
}

void visuals::lean_indicator( const misc_data_t& misc_data, int& offset )
{
	if ( !vars::aa.enabled->get<bool>() || !vars::aa.lean_type->get<int>() || !interfaces::engine()->IsInGame() || !misc_data.alive )
		return;

	const auto size = adapter->display;
	auto& layer = dx_adapter::buf;

	const auto multiplier = clamp( antiaim::current_lean * 0.02f, 0.f, 1.f );

	_( fd_s, "LEAN" );
	layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), fd_s, color( Color::RedAccent().fade_to( Color::Lime(), multiplier ).direct() ), layer::align_left, layer::align_center );
	offset += 24;
}

void visuals::doubletap_indicator( const  misc_data_t& misc_data, int& offset )
{
	if ( !misc_data.doubletap || !interfaces::engine()->IsInGame() || !misc_data.alive || !vars::aa.dt_indicator->get<bool>() )
		return;

	const auto size = adapter->display;
	const auto multiplier = clamp( tickbase::compute_current_limit() / 14.f, 0.f, 1.f );

	auto& layer = dx_adapter::buf;

	_( fd_s, "DT" );
	layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), fd_s, color( Color::RedAccent().fade_to( Color::BlueAccent(), multiplier ).direct() ), layer::align_left, layer::align_center );
	offset += 24;
}

void visuals::freestand_indicator( const  misc_data_t& misc_data, int& offset )
{
	if ( !vars::aa.freestand->get<bool>() || freestanding::direction != -1 || !interfaces::engine()->IsInGame() || !misc_data.alive || !vars::aa.fs_indicator->get<bool>() )
		return;

	const auto size = adapter->display;

	auto& layer = dx_adapter::buf;

	_( fs_s, "FS" );
	layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), fs_s, color( Color::BlueAccent().direct() ), layer::align_left, layer::align_center );
	offset += 24;
}

void visuals::recharge_bar( const misc_data_t& misc_data, int& offset )
{
	if ( ( !misc_data.doubletap || !vars::aa.dt_indicator->get<bool>() ) && ( !misc_data.silent || !vars::aa.st_indicator->get<bool>() ) || !interfaces::engine()->IsInGame() || !misc_data.alive )
		return;

	const auto size = adapter->display;
	const auto multiplier = clamp( tickbase::compute_current_limit() / 14.f, 0.f, 1.f );

	_( dt_s, "DT" );
	_( hs_s, "HS" );
	auto text_size = evo::ren::text_size( evo::ren::draw.get_font( FNV1A( "lby" ) ), misc_data.doubletap ? dt_s : hs_s );
	if ( misc_data.doubletap )
		text_size.x -= 2.f;

	auto width = text_size.x * multiplier;
	width = clamp( width, 0.f, text_size.x );

	auto& layer = dx_adapter::buf;

	layer.add_rect_filled( rect_legacy( { 10, size.y / 1.65f + offset - 14 }, { text_size.x + 1, 4 } ), ::color( D3DCOLOR_ARGB( static_cast< int >( 255 * 0.3f ), 0, 0, 0 ) ) );
	layer.add_rect_filled( rect_legacy( { 11, size.y / 1.65f + offset - 13 }, { width - 1, 2 } ), ::color( Color::RedAccent().fade_to( Color::BlueAccent(), multiplier ).direct() ) );

	offset += 8;
}

void visuals::silent_indicator( const misc_data_t& misc_data, int& offset )
{
	if ( !misc_data.silent || !interfaces::engine()->IsInGame() || !misc_data.alive || !vars::aa.st_indicator->get<bool>() )
		return;

	const auto size = adapter->display;
	const auto multiplier = clamp( tickbase::compute_current_limit() / 5.f, 0.f, 1.f );

	auto& layer = dx_adapter::buf;

	_( fd_s, "HS" );
	layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), fd_s, color( Color::RedAccent().fade_to( Color::BlueAccent(), multiplier ).direct() ), layer::align_left, layer::align_center );
	offset += 24;
}

void visuals::anti_exploit_indicator( const misc_data_t& misc_data, int& offset )
{
	if ( !misc_data.anti_exploit || !interfaces::engine()->IsInGame() || !misc_data.alive )
		return;

	const auto size = adapter->display;

	auto& layer = dx_adapter::buf;

	_( fd_s, "AX" );
	layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), fd_s, color( Color::BlueAccent().direct() ), layer::align_left, layer::align_center );
	offset += 24;
}

void visuals::override_indicator( const misc_data_t& misc_data, int& offset )
{
	if ( !misc_data.mindmg_override || !vars::aa.do_indicator->get<bool>() || !interfaces::engine()->IsInGame() || !misc_data.alive )
		return;

	const auto size = adapter->display;

	auto& layer = dx_adapter::buf;

	_( fd_s, "DMG" );
	layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), fd_s, color( Color::BlueAccent().direct() ), layer::align_left, layer::align_center );
	offset += 24;
}

void visuals::force_safety_indicator( const misc_data_t& misc_data, int& offset )
{
	if ( !vars::aim.fallback_on_key->get<bool>() || !vars::aa.safe_indicator->get<bool>() || !interfaces::engine()->IsInGame() || !misc_data.alive )
		return;

	const auto size = adapter->display;

	auto& layer = dx_adapter::buf;

	_( fd_s, "SAFE" );
	layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), fd_s, color( Color::BlueAccent().direct() ), layer::align_left, layer::align_center );
	offset += 24;
}

void visuals::hsonly_indicator( const misc_data_t& misc_data, int& offset )
{
	if ( !vars::aim.headshot_only() || !vars::aa.hsonly_indicator->get<bool>() || !interfaces::engine()->IsInGame() || !misc_data.alive )
		return;

	const auto size = adapter->display;

	auto& layer = dx_adapter::buf;

	/*dx_adapter::icon( layer, vec2( 12, size.y / 1.65f + offset + 3 ), 36, color( Color::Black().direct() ), xorstr_( "hud\\deathnotice\\icon_headshot" ), layer::align_left, layer::align_center, false );
	dx_adapter::icon( layer, vec2( 11, size.y / 1.65f + offset + 2 ), 36, color( Color::BlueAccent().direct() ), xorstr_( "hud\\deathnotice\\icon_headshot" ), layer::align_left, layer::align_center, false );*/
	_( fd_s, "HSO" );
	layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), fd_s, color( Color::BlueAccent().direct() ), layer::align_left, layer::align_center );
	offset += 24;
}


void visuals::force_baim_indicator( const misc_data_t& misc_data, int& offset )
{
	if ( !vars::aim.baim_on_key->get<bool>() || !vars::aa.baim_indicator->get<bool>() || !interfaces::engine()->IsInGame() || !misc_data.alive )
		return;

	const auto size = adapter->display;

	auto& layer = dx_adapter::buf;

	_( fd_s, "BAIM" );
	layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), fd_s, color( Color::RedAccent().direct() ), layer::align_left, layer::align_center );
	offset += 24;
}

void visuals::lc_indicator( const misc_data_t& misc_data, int& offset )
{
	if ( !vars::aa.enabled->get<bool>() || !vars::aa.lc_indicator->get<bool>() || !misc_data.alive || !misc_data.lc )
		return;

	const auto size = adapter->display;

	auto& layer = dx_adapter::buf;

	_( lc_s, "LC" );
	layer.add_text( draw.get_font( FNV1A( "lby" ) ), vec2( 10, size.y / 1.65f + offset ), lc_s, color( Color::BlueAccent().direct() ), layer::align_left, layer::align_center );
	offset += 24;
}

void visuals::draw_misc_data()
{
	if ( !misc_data.valid )
		return;

	if ( misc_data.spread != -1.f )
		spread_circle( misc_data );

	draw_scope( misc_data );

	auto offset = 0;
	lean_indicator( misc_data, offset );

	aa_indicator( misc_data, offset );

	freestand_indicator( misc_data, offset );

	anti_exploit_indicator( misc_data, offset );

	silent_indicator( misc_data, offset );

	doubletap_indicator( misc_data, offset );

	recharge_bar( misc_data, offset );

	override_indicator( misc_data, offset );

	force_safety_indicator( misc_data, offset );

	force_baim_indicator( misc_data, offset );

	fakeduck_indicator( misc_data, offset );

	lc_indicator( misc_data, offset );

	hsonly_indicator( misc_data, offset );

	if ( misc_data.wallbang != -1 )
		autowall_crosshair( misc_data );
}

void visuals::spread_circle( const misc_data_t& data )
{
	if ( !vars::visuals.spread_circle_type->get<bool>() || !data.alive )
		return;

	const auto ssize = adapter->display;
	const auto screen_center = ( adapter->display / 2.f ).round() + 1.f;

	auto cone = data.spread;
	if ( cone > 0.0f )
	{
		const int cross_x = screen_center.x;
		const int cross_y = screen_center.y;
		if ( cone < 0.004f ) cone = 0.004f;
		const auto size = ( cone * ssize.y ) * 0.7f;
		const auto color1 = vars::visuals.spread_circle_color1->get<D3DCOLOR>();
		const auto color2 = vars::visuals.spread_circle_color2->get<D3DCOLOR>();

		auto& layer = dx_adapter::buf;

		layer.push_aa( true );

		switch ( vars::visuals.spread_circle_type->get<int>() )
		{
			case 1:
				layer.add_circle_filled_multicolor( vec2( cross_x, cross_y ), size, { color1, color2 } );
				break;
			case 2:
				//render::circle_filled_rainbow( Vector2D( cross_x, cross_y ), size, nullptr, 1.f, 100.f, 10.f ); TODO:
				break;
			case 3:
				//render::circle_filled_rainbow( Vector2D( cross_x, cross_y ), size, &hue, 1.f, 100.f, 10.f ); TODO:
				break;
			default:
				break;
		}

		layer.pop_aa();
	}
}

void visuals::autowall_crosshair( const misc_data_t& data )
{
	if ( !vars::visuals.autowall_crosshair->get<bool>() || !data.alive )
		return;

	const auto color = data.wallbang ? Color( 130, 180, 0 ) : Color( 210, 0, 0 );

	const auto ssize = adapter->display;
	const int cross_x = std::ceil( ssize.x / 2.f ) - 1;
	const int cross_y = std::ceil( ssize.y / 2.f ) - 1;

	auto& layer = dx_adapter::buf;

	layer.add_rect_filled( rect_legacy( { ( float ) cross_x, ( float ) cross_y }, { 3, 3 } ), ::color( color.direct() ) );
}

std::string visuals::grenade_to_name( const int id )
{
	_( smoke_s, "smoke" );
	_( grenade_s, "he grenade" );
	_( molotov_s, "molotov" );

	switch ( id )
	{
		case WEAPON_SMOKEGRENADE: return smoke_s;
		case WEAPON_HEGRENADE: return grenade_s;
		case WEAPON_MOLOTOV: return molotov_s;
		case WEAPON_INCGRENADE: return molotov_s;
		default:;
	}

	return "";
}