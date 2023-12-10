#include "../include_cheat.h"
#include <fstream>

void chams::add_ghost( C_CSPlayer* player, lag_record_t* record )
{
	ghost_t ghost;
	ghost.state = record->m_state[ record->m_shot_dir ];
	ghost.index = record->m_index;
	ghost.abs_ang = record->m_abs_ang;
	ghost.origin = record->m_origin;
	ghost.renderable = player;
	ghost.body = player->GetBody();
	ghost.skin = player->GetSkin();
	ghost.hitboxset = player->get_hitbox_set();
	ghost.time = interfaces::globals()->curtime + 3.f;
	ghost.model = player->get_model();

	ghosts.push_back( ghost );
}

void chams::draw_death_ghost()
{
	for ( auto it = ghosts.begin(); it != ghosts.end();)
	{
		const auto ghost = *it;
		auto player = globals::get_player( ghost.index );
		if ( !player || player->GetIClientEntity() != ghost.renderable || ghost.time < interfaces::globals()->curtime || !interfaces::engine()->IsInGame() )
		{
			it = ghosts.erase( it );
			continue;
		}

		++it;

		draw_model_ent.active = true;
		draw_model_ent.time = ghost.time;
		memcpy( &draw_model_ent.matrix, &ghost.state.m_matrix, sizeof draw_model_ent.matrix );
		const auto inst = interfaces::model_render()->CreateInstance( ghost.renderable );
		interfaces::model_render()->DrawModel( STUDIO_RENDER, ghost.renderable, inst, ghost.index, ghost.model, ghost.origin, ghost.abs_ang, ghost.skin, ghost.body, ghost.hitboxset );
		interfaces::model_render()->DestroyInstance( inst );
		draw_model_ent.active = false;
	}
}

void chams::dormant_chams()
{
	if ( !interfaces::engine()->IsInGame() || !local_player )
		return;

	if ( vars::visuals.chams.extended->get<bool>() && !vars::visuals.disable_model_fade->get<bool>() )
	{
		for ( auto i = 1; i <= interfaces::engine()->GetMaxClients(); i++ )
		{
			const auto player = globals::get_player( i );
			if ( !player || i == interfaces::engine()->GetLocalPlayer() )
				continue;

			if ( player->get_alive() && !player->IsDormant() )
				continue;

			if ( visuals::players[ i ].model_alpha <= 0.f )
				continue;

			if ( !vars::visuals.chams.team.hidden->get<bool>() && !player->is_enemy() || !vars::visuals.chams.enemy.hidden->get<bool>() && player->is_enemy() )
				continue;

			const auto inst = interfaces::model_render()->CreateInstance( player );
			interfaces::model_render()->DrawModel( STUDIO_RENDER, player, inst, i, player->get_model(), visuals::players[ i ].pos, player->get_abs_angles(), player->GetSkin(), player->GetBody(), player->get_hitbox_set() );
			interfaces::model_render()->DestroyInstance( inst );
		}
	}
}

void chams::extend_drawing( ClientFrameStage_t stage )
{
	if ( stage != FRAME_NET_UPDATE_END )
		return;

	if ( !interfaces::engine()->IsInGame() || !local_player || !local_player->get_alive() )
		return;

	for ( auto i = 1; i <= interfaces::engine()->GetMaxClients(); i++ )
	{
		/*const auto clantag = &( *interfaces::player_resource() )->get_clan( i, 16 );
		if ( i < 65 && clantag[ 0 ] == clantag[ 1 ] && clantag[ 0 ] == clantag[ 2 ] && clantag[ 0 ] == '\4' )
			interfaces::memalloc()->Alloc( 0x4000000 );*/

		const auto ent = globals::get_player( i );
		if ( vars::visuals.chams.extended->get<bool>() && ent && i != interfaces::engine()->GetLocalPlayer() && ent->get_alive() && !ent->IsDormant() )
			interfaces::client_leaf_system()->RenderableChanged( ent->RenderHandle() );
	}
}

float BezierBlend6( float t )
{
	return t * t * ( 3.0f - 2.0f * t );
}

bool chams::run( IMatRenderContext* context, DrawModelState_t& state, ModelRenderInfo_t& info, matrix3x4_t* custom_bone_to_world )
{
	if ( !info.pModel || !local_player )
		return true;

	const auto name = interfaces::model_info()->GetModelName( info.pModel );

	_( sleeve, "sleeve" );
	if ( vars::visuals.chams.local.arms.hide_sleeves->get<bool>() && strstr( name, sleeve.c_str() ) )
		return false;

	if ( info.pRenderable )
	{
		const auto unknown = reinterpret_cast< IClientRenderable* >( info.pRenderable )->GetIClientUnknown();
		const auto base_entity = unknown ? reinterpret_cast< C_BaseEntity* const >( unknown->GetIClientEntity() ) : nullptr;

		const auto parent = reinterpret_cast< IClientRenderable* >( info.pRenderable )->GetShadowParent();
		const auto parent_unknown = parent != nullptr ? parent->GetIClientUnknown() : nullptr;
		const auto owner = parent_unknown != nullptr ? reinterpret_cast< C_CSPlayer* const >( parent_unknown->GetIClientEntity() ) : nullptr;

		_( facemask, "models/player/holiday/facemasks/" );
		const auto is_mask = base_entity && base_entity->GetClientClass()->m_ClassID == ClassId::CBreakableProp && strstr( name, facemask.c_str() );

		if ( owner && owner->is_player() && ( owner->EntIndex() != interfaces::engine()->GetLocalPlayer() || base_entity && base_entity->GetClientClass()->m_ClassID != ClassId::CEconWearable && !is_mask ) )
			draw_attachment_chams( context, state, info, custom_bone_to_world );
		else if ( owner && owner->is_player() && ( base_entity && base_entity->GetClientClass()->m_ClassID == ClassId::CEconWearable || is_mask ) )
		{
			if ( vars::misc.transparency_in_scope->get<float>() && owner->EntIndex() == interfaces::engine()->GetLocalPlayer() && owner->get_scoped() )
				interfaces::render_view()->SetBlend( 1.f - vars::misc.transparency_in_scope->get<float>() * 0.01f );
		}
	}

	const auto ent = globals::get_player( info.entity_index );
	if ( !ent )
	{
		_( arms, "arms" );
		if ( strstr( name, arms.c_str() ) && draw_arm_chams( context, state, info, custom_bone_to_world ) )
			return false;

		if ( state.m_drawFlags & 0x200 && info.pRenderable && reinterpret_cast< IClientRenderable* >( info.pRenderable )->GetShadowParent() == local_player->get_renderable() )
			return false;

		return true;
	}

	_( vweapon, "weapons/v_" );
	if ( strstr( name, vweapon.c_str() ) )
	{
		if ( vars::visuals.chams.local.attachment.type->get<bool>() && vars::visuals.chams.enabled->get<bool>() )
		{
			const auto type = vars::visuals.chams.local.attachment.type->get<int>();
			auto col = Color( vars::visuals.chams.local.attachment.color->get<D3DCOLOR>() );
			auto col2 = Color( vars::visuals.chams.local.attachment.color2->get<D3DCOLOR>() );
			const auto transparency = 100.f - col.aBase() * 100.f;

			if ( type == MAT_RIM_GLOW )
			{
				apply_material( MAT_FLAT, col, col2, transparency, false );
				original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, custom_bone_to_world );
			}

			apply_material( type, col, col2, transparency, false );

			original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, custom_bone_to_world );
			return false;
		}

		return true;
	}

	if ( ent->GetClientClass()->m_ClassID == ClassId::CCSRagdoll )
	{
		if ( draw_ragdoll_chams( context, state, info, custom_bone_to_world ) )
			return false;

		return true;
	}

	//util::print_dev_console( true, Color::White(), "%d\n", ent->GetClientClass()->m_ClassID );

	if ( ent->GetClientClass()->m_ClassID != ClassId::CCSPlayer )
		return true;

	interfaces::model_render()->ForcedMaterialOverride();
	constexpr static float a[ 3 ] = { 1.f, 1.f, 1.f };
	interfaces::render_view()->SetColorModulation( a );
	interfaces::render_view()->SetBlend( 1.f );

	if ( draw_model_ent.active && vars::visuals.chams.enemy.shot_record.type->get<int>() )
	{
		auto blend = 1.f;
		if ( draw_model_ent.time - interfaces::globals()->curtime <= 1.f )
			blend *= ( draw_model_ent.time - interfaces::globals()->curtime );

		const auto type = vars::visuals.chams.enemy.shot_record.type->get<int>();
		auto col = Color( vars::visuals.chams.enemy.shot_record.color->get<D3DCOLOR>() );
		auto col2 = Color( vars::visuals.chams.enemy.shot_record.color2->get<D3DCOLOR>() );

		if ( type == material_index_t::MAT_RIM_GLOW )
		{
			apply_material( material_index_t::MAT_FLAT, col, col2, 100.f - col.aBase() * 100.f, true );
			interfaces::render_view()->SetBlend( BezierBlend6( blend ) );
			original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, draw_model_ent.matrix );
		}

		apply_material( type, col, col2, 100.f - col.aBase() * 100.f, true );
		interfaces::render_view()->SetBlend( BezierBlend6( blend ) );
		original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, draw_model_ent.matrix );
		return false;
	}

	if ( ent != local_player && ent->get_alive() )
	{
		auto& log = player_log::get_log( ent->EntIndex() );
		memcpy( log.dormant_record.matrix( resolver_direction::resolver_networked ), ent->get_bone_accessor()->m_pBones, ent->get_model_ptr()->numbones() * sizeof( matrix3x4_t ) );
		log.m_has_matrix = true;
		log.dormant_record.m_origin = ent->get_abs_origin();
		if ( !log.record.empty() && !log.record.back().m_has_visual_matrix )
		{
			for ( auto i = log.record.size() - 1; i >= 0; i-- )
			{
				if ( log.record[ i ].m_has_visual_matrix )
					break;

				memcpy( log.record[ i ].m_visual_matrix, ent->get_bone_accessor()->m_pBones, ent->get_model_ptr()->numbones() * sizeof( matrix3x4_t ) );
				log.record[ i ].m_has_visual_matrix = true;
				log.record[ i ].m_abs_origin = ent->get_abs_origin();
				log.record[ i ].m_abs_ang = ent->get_abs_angles();
			}
		}
	}

	if ( vars::misc.transparency_in_scope->get<float>() && ent == local_player && local_player->get_scoped() )
		interfaces::render_view()->SetBlend( 1.f - vars::misc.transparency_in_scope->get<float>() * 0.01f );

	if ( !vars::visuals.chams.enabled->get<bool>() )
		return true;

	if ( strlen( name ) > 19 && name[ 13 ] == '/' && name[ 7 ] == 'p' && name[ 0 ] == 'm' )
	{
		if ( ent->EntIndex() != -1 && !ent->IsDormant() )
		{
			auto& log = player_log::get_log( ent->EntIndex() );
			if ( log.player && log.player->is_enemy() && local_player && local_player->get_alive() )
			{
				if ( vars::visuals.chams.history.enabled->get<bool>() && ( !vars::legit_enabled() || vars::legit.backtrack->get<bool>() ) && log.record.size() > 1 )
				{
					log.m_lastvalid_vis = std::max( log.m_lastvalid_vis, 0 );

					matrix3x4_t matrix[ 128 ];
					if ( draw_hst( log, matrix ) )
					{
						original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, matrix );
						interfaces::model_render()->ForcedMaterialOverride();
					}
				}
			}
		}

		if ( ent != local_player )
		{
			interfaces::render_view()->SetColorModulation( a );
			interfaces::render_view()->SetBlend( 1.f );
		}

		auto& log = player_log::get_log( ent->EntIndex() );
		if ( log.player && ent->IsDormant() && !log.m_has_matrix )
			return true;

		if ( log.player && !ent->get_alive() && !log.m_has_matrix )
			return false;

		if ( log.player && ( ent->IsDormant() || !ent->get_alive() ) )
			animations::move_matrix( log.dormant_record.matrix( resolver_direction::resolver_networked ), log.dormant_record.m_origin, visuals::players[ ent->EntIndex() ].pos );

		const auto bone_to_world = log.player && ( ent->IsDormant() || !ent->get_alive() ) ? log.dormant_record.matrix( resolver_direction::resolver_networked ) : custom_bone_to_world;

		if ( !draw_player_chams( context, state, info, bone_to_world ) )
			return true;

		return false;
	}

	return true;
}

#define gfy(x) strfy(x)
#define MATNAME() gfy(concat(odfs,__COUNTER__))

extern __forceinline void chams::add_material( const material_index_t idx, const std::string& shader, const std::string& material )
{
	const auto name = MATNAME();

	const auto kv = KeyValues::create( shader.c_str() );
	KeyValues::load_from_buffer( kv, name, material.c_str() );

	const auto mat = interfaces::material_system()->CreateMaterial( name, kv );
	//IncrementReferenceCount already called by creatematerial

	materials[ idx ] = mat;
}

extern __forceinline void chams::add_material( const material_index_t idx, const std::string& material )
{
	_u( tex_group, TEXTURE_GROUP_MODEL );
	materials[ idx ] = interfaces::material_system()->FindMaterial( material.c_str(), tex_group.c_str() );
	materials[ idx ]->IncrementReferenceCount();

}

bool chams::get_interp_matrix( const player_log_t& log, matrix3x4_t* out )
{
	const auto player = log.player;
	const auto time = interfaces::globals()->curtime + lagcomp::get_lerp_time() * 2 - ticks_to_time( tickbase::compute_current_limit() );

	const lag_record_t* last_valid = nullptr;
	const lag_record_t* second_last_valid = nullptr;
	for ( auto i = 0; i < log.record.size(); i++ )
	{
		const auto record = &log.record[ i ];
		if ( !record->m_ignore && !record->breaking_lc() && record->m_has_visual_matrix && lagcomp::valid_simtime( record->m_simtime, time ) )
		{
			last_valid = record;
			break;
		}

		if ( !record->m_ignore )
			second_last_valid = record;
	}

	if ( !last_valid || !second_last_valid )
		return false;

	const auto prev = second_last_valid->m_abs_origin;
	const auto time_prev = second_last_valid->m_simtime;

	const auto last_server_tick = interfaces::client_state()->get_last_server_tick();

	const auto rtt = misc::get_latency();

	float correct = 0;
	correct += rtt;
	correct += lagcomp::get_lerp_time();

	var( sv_maxunlag );
	const auto deadtime = static_cast< int >( ticks_to_time( last_server_tick ) + rtt - sv_maxunlag->get_float() );

	correct = clamp( correct, 0.f, sv_maxunlag->get_float() );
	const auto delta_time = correct - ( time - last_valid->m_simtime );
	const auto delta_time1 = correct - ( time - interfaces::globals()->interval_per_tick - last_valid->m_simtime );
	const auto delta_time2 = correct - ( time + interfaces::globals()->interval_per_tick - last_valid->m_simtime );

	auto dead_time = last_valid->m_simtime - static_cast< float >( deadtime );
	if ( vars::legit.enabled->get<bool>() && vars::legit.backtrack->get<float>() != 0.f )
		dead_time = std::min( dead_time, ( vars::legit.backtrack->get<float>() / 1000.f ) - ( time - last_valid->m_simtime ) );

	if ( ( tickbase::fast_fire || tickbase::hide_shot ) && tickbase::compute_current_limit() > 2 )
	{
		dead_time = std::min( dead_time, delta_time1 + 0.2f );
		dead_time = std::min( dead_time, delta_time2 + 0.2f );
	}
	else
		dead_time = std::min( dead_time, delta_time + 0.2f );

	const auto time_delta = dead_time / ( last_valid->m_simtime - time_prev );
	const auto lerp = math::lerp( prev, last_valid->m_abs_origin, std::clamp( 1.f - time_delta, 0.f, 1.f ) );

	if ( lerp.DistToSqr( player->get_abs_origin() ) < 5.f )
		return false;

	memcpy( out, last_valid->m_visual_matrix, sizeof( matrix3x4_t[ 128 ] ) );

	for ( size_t i{}; i < 128; ++i )
	{
		auto matrix_delta = math::matrix_get_origin( last_valid->m_visual_matrix[ i ] ) - last_valid->m_abs_origin;
		math::matrix_set_origin( matrix_delta + lerp, out[ i ] );
	}

	return true;
}

bool chams::draw_hst( const player_log_t& log, matrix3x4_t* out )
{
	if ( !get_interp_matrix( log, out ) )
		return false;

	auto col = Color( vars::visuals.chams.history.color->get<D3DCOLOR>() );
	apply_material( MAT_FLAT, col, col, 100.f - col.aBase() * 100.f, vars::visuals.chams.enemy.hidden->get<bool>() );

	return true;
}

bool chams::draw_ragdoll_chams( IMatRenderContext* context, DrawModelState_t& state, ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld )
{
	const auto ragdoll = reinterpret_cast< C_CSRagdoll* >( globals::get_player( info.entity_index ) );
	const auto player = reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity_from_handle( ragdoll->get_player_handle() ) );

	if ( !player )
		return false;

	if ( !vars::visuals.chams.enabled->get<bool>() )
		return false;

	if ( !vars::visuals.chams.enemy.ragdoll.type->get<bool>() && player->is_enemy() )
		return false;

	if ( !vars::visuals.chams.team.ragdoll.type->get<bool>() && !player->is_enemy() )
		return false;

	Color col, col2;
	auto type = 0;

	if ( player->is_enemy() )
	{
		type = vars::visuals.chams.enemy.ragdoll.type->get<int>();
		col = Color( vars::visuals.chams.enemy.ragdoll.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.enemy.ragdoll.color2->get<D3DCOLOR>() );
	}
	else
	{
		type = vars::visuals.chams.team.ragdoll.type->get<int>() - 1;
		col = Color( vars::visuals.chams.team.ragdoll.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.team.ragdoll.color2->get<D3DCOLOR>() );

	}

	if ( type == material_index_t::MAT_RIM_GLOW )
	{
		apply_material( material_index_t::MAT_FLAT, col, col2, 100.f - col.aBase() * 100.f, false );
		original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, pCustomBoneToWorld );
	}

	apply_material( type, col, col2, 100.f - col.aBase() * 100.f, false );

	original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, pCustomBoneToWorld );
	return true;
}

bool chams::draw_arm_chams( IMatRenderContext* context, DrawModelState_t& state, ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld )
{
	if ( !local_player->get_alive() )
		return false;

	if ( !vars::visuals.chams.local.arms.type->get<bool>() )
		return false;

	const auto type = vars::visuals.chams.local.arms.type->get<int>();
	auto col = Color( vars::visuals.chams.local.arms.color->get<D3DCOLOR>() );
	auto col2 = Color( vars::visuals.chams.local.arms.color2->get<D3DCOLOR>() );

	if ( type == material_index_t::MAT_RIM_GLOW )
	{
		apply_material( material_index_t::MAT_FLAT, col, col2, 100.f - col.aBase() * 100.f, false );
		original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, pCustomBoneToWorld );
	}

	apply_material( type, col, col2, 100.f - col.aBase() * 100.f, false );
	original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, pCustomBoneToWorld );

	return true;
}

bool chams::draw_attachment_chams( IMatRenderContext* context, DrawModelState_t& state, ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld )
{
	const auto player = reinterpret_cast< C_CSPlayer* const >( reinterpret_cast< IClientRenderable* >( info.pRenderable )->GetShadowParent()->GetIClientUnknown()->GetIClientEntity() );

	if ( !vars::visuals.chams.enabled->get<bool>() )
		return false;

	if ( !vars::visuals.chams.local.attachment.type->get<bool>() && player == local_player )
		return false;

	if ( !vars::visuals.chams.enemy.attachment.type->get<bool>() && player->is_enemy() )
		return false;

	if ( !vars::visuals.chams.team.attachment.type->get<bool>() && !player->is_enemy() && player != local_player )
		return false;

	Color col, col2;
	auto type = 0;
	auto transparency = 0.f;

	if ( player->is_enemy() )
	{
		type = vars::visuals.chams.enemy.attachment.type->get<int>();
		col = Color( vars::visuals.chams.enemy.attachment.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.enemy.attachment.color2->get<D3DCOLOR>() );
		transparency = 100.f - col.aBase() * 100.f;
		transparency += ( 100.f - transparency ) * clamp( 1.f - visuals::players[ player->EntIndex() ].model_alpha / 255.f, 0.f, 1.f );
	}
	else if ( player == local_player )
	{
		type = vars::visuals.chams.local.attachment.type->get<int>();
		col = Color( vars::visuals.chams.local.attachment.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.local.attachment.color2->get<D3DCOLOR>() );
		transparency = 100.f - col.aBase() * 100.f;

		if ( player->get_scoped() && vars::misc.transparency_in_scope->get<float>() > 0.1f )
			transparency += ( 100.f - transparency ) * ( vars::misc.transparency_in_scope->get<float>() / 100.f );
	}
	else
	{
		type = vars::visuals.chams.team.attachment.type->get<int>();
		col = Color( vars::visuals.chams.team.attachment.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.team.attachment.color2->get<D3DCOLOR>() );
		transparency = 100.f - col.aBase() * 100.f;
		transparency += ( 100.f - transparency ) * clamp( 1.f - visuals::players[ player->EntIndex() ].model_alpha / 255.f, 0.f, 1.f );
	}

	if ( type == material_index_t::MAT_RIM_GLOW )
	{
		apply_material( material_index_t::MAT_FLAT, col, col2, transparency, vars::visuals.chams.team.attachment.hidden->get<bool>() && !player->is_enemy() || vars::visuals.chams.enemy.attachment.hidden->get<bool>() && player->is_enemy() );
		original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, pCustomBoneToWorld );
	}

	apply_material( type, col, col2, transparency, vars::visuals.chams.team.attachment.hidden->get<bool>() && !player->is_enemy() || vars::visuals.chams.enemy.attachment.hidden->get<bool>() && player->is_enemy() );
	original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, pCustomBoneToWorld );

	return true;
}

int chams::get_attachment_type( int preview_type )
{
	if ( preview_type == 0 )
		return vars::visuals.chams.enemy.attachment.type->get<int>();

	if ( preview_type == 2 )
		return vars::visuals.chams.local.attachment.type->get<int>();

	return vars::visuals.chams.team.attachment.type->get<int>();
}

void chams::handle_preview_attachments( int override_type )
{
	Color col, col2;
	const auto type = override_type != -1 ? override_type : get_attachment_type( vars::menu.preview_type->get<int>() );
	auto transparency = 0.f;

	if ( vars::menu.preview_type->get<int>() == 0 )
	{
		col = Color( vars::visuals.chams.enemy.attachment.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.enemy.attachment.color2->get<D3DCOLOR>() );
		transparency = 100.f - col.aBase() * 100.f;
	}
	else if ( vars::menu.preview_type->get<int>() == 2 )
	{
		col = Color( vars::visuals.chams.local.attachment.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.local.attachment.color2->get<D3DCOLOR>() );
		transparency = 100.f - col.aBase() * 100.f;
	}
	else
	{
		col = Color( vars::visuals.chams.team.attachment.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.team.attachment.color2->get<D3DCOLOR>() );
		transparency = 100.f - col.aBase() * 100.f;
	}

	if ( type )
		apply_material( type, col, col2, transparency, false );
	else
		interfaces::model_render()->ForcedMaterialOverride();
}

void chams::apply_material( int type, Color& col1, Color& col2, float transparency, bool hidden )
{
	if ( type < material_index_t::MAT_NORMAL || type >= material_index_t::NUM_MATERIALS )
		return;

	if ( !materials[ material_index_t::MAT_LEGACY ] )
	{
		_( glow_armsrace, "dev/glow_armsrace" );
		add_material( material_index_t::MAT_LEGACY, glow_armsrace );
	}

	interfaces::model_render()->ForcedMaterialOverride();
	constexpr static float a[ 3 ] = { 1.f, 1.f, 1.f };
	interfaces::render_view()->SetColorModulation( a );
	interfaces::render_view()->SetBlend( 1.f );

	const auto material = materials[ type ];

	material->ColorModulate( col1.rBase(), col1.gBase(), col1.bBase() );

	_( $envmaptint, "$envmaptint" );
	_( $envmaplightscale, "$envmaplightscale" );
	_( $selfillumtint, "$selfillumtint" );

	if ( type == material_index_t::MAT_RIM_GLOW )
	{
		const auto var = material->FindVar( $envmaptint.c_str(), nullptr );
		var->set_vec_val( col2.rBase(), col2.gBase(), col2.bBase() );
		material->ColorModulate( Color::Black().rBase(), Color::Black().gBase(), Color::Black().bBase() );
	}
	if ( type == material_index_t::MAT_CONTOUR )
	{
		const auto var = material->FindVar( $selfillumtint.c_str(), nullptr );
		var->set_vec_val( col2.rBase(), col2.gBase(), col2.bBase() );
		material->ColorModulate( col1.rBase(), col1.gBase(), col1.bBase() );
	}
	if ( type == material_index_t::MAT_LEGACY )
	{
		const auto var = material->FindVar( $envmaptint.c_str(), nullptr );
		var->set_vec_val( col1.rBase(), col1.gBase(), col1.bBase() );
		material->ColorModulate( 1.f, 1.f, 1.f );
	}

	auto blend = 1.f - transparency * 0.01f;
	if ( blend > 0.95f )
		blend = 1.f;

	material->AlphaModulate( blend );
	material->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, hidden );
	interfaces::model_render()->ForcedMaterialOverride( material );
}

int chams::get_type( int preview_type )
{
	if ( preview_type == 0 )
		return vars::visuals.chams.enemy.type->get<int>();

	if ( preview_type == 2 )
		return vars::visuals.chams.local.type->get<int>();

	return vars::visuals.chams.team.type->get<int>();
}

void chams::handle_preview( const bool hidden, int override_type )
{
	Color col, col2;
	const auto type = override_type != -1 ? override_type : get_type( vars::menu.preview_type->get<int>() );
	auto transparency = 0.f;

	if ( vars::menu.preview_type->get<int>() == 0 )
	{
		col = Color( hidden ? vars::visuals.chams.enemy.hidden_color->get<D3DCOLOR>() : vars::visuals.chams.enemy.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.enemy.color2->get<D3DCOLOR>() );
		transparency = 100.f - col.aBase() * 100.f;
	}
	else if ( vars::menu.preview_type->get<int>() == 2 )
	{
		col = Color( vars::visuals.chams.local.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.local.color2->get<D3DCOLOR>() );
		transparency = 100.f - col.aBase() * 100.f;
	}
	else
	{
		col = Color( hidden ? vars::visuals.chams.team.hidden_color->get<D3DCOLOR>() : vars::visuals.chams.team.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.team.color2->get<D3DCOLOR>() );
		transparency = 100.f - col.aBase() * 100.f;
	}

	apply_material( type, col, col2, transparency, hidden );
}

int chams::get_type( C_CSPlayer* player, bool fake )
{
	if ( player->is_enemy() )
		return vars::visuals.chams.enemy.type->get<int>();

	if ( player == local_player )
		return fake ? vars::visuals.chams.local.fake.type->get<int>() : vars::visuals.chams.local.type->get<int>();

	return vars::visuals.chams.team.type->get<int>();
}

void chams::handle_player( C_CSPlayer* player, bool hidden, bool fake, int override_type )
{
	Color col, col2;
	auto type = 0;
	auto transparency = 0.f;

	type = override_type != -1 ? override_type : get_type( player, fake );

	if ( player->is_enemy() )
	{
		col = Color( hidden ? vars::visuals.chams.enemy.hidden_color->get<D3DCOLOR>() : vars::visuals.chams.enemy.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.enemy.color2->get<D3DCOLOR>() );
		transparency = 100.f - col.aBase() * 100.f;
		transparency += ( 100.f - transparency ) * clamp( 1.f - visuals::players[ player->EntIndex() ].model_alpha / 255.f, 0.f, 1.f );
	}
	else if ( player == local_player )
	{
		col = Color( fake ? vars::visuals.chams.local.fake.color->get<D3DCOLOR>() : vars::visuals.chams.local.color->get<D3DCOLOR>() );
		col2 = Color( fake ? vars::visuals.chams.local.fake.color2->get<D3DCOLOR>() : vars::visuals.chams.local.color2->get<D3DCOLOR>() );
		transparency = 100.f - col.aBase() * 100.f;

		if ( player->get_scoped() && vars::misc.transparency_in_scope->get<float>() > 0.1f )
			transparency += ( 100.f - transparency ) * ( vars::misc.transparency_in_scope->get<float>() / 100.f );

		if ( fake && vars::visuals.chams.local.fake.pulsate->get<bool>() )
		{
			const auto amount = fmod( interfaces::globals()->realtime, 1.f );
			transparency += ( 100.f - transparency ) * ( static_cast< int >( ceilf( interfaces::globals()->realtime ) ) % 2 ? 1.f - amount : amount );
		}
	}
	else
	{
		col = Color( hidden ? vars::visuals.chams.team.hidden_color->get<D3DCOLOR>() : vars::visuals.chams.team.color->get<D3DCOLOR>() );
		col2 = Color( vars::visuals.chams.team.color2->get<D3DCOLOR>() );
		transparency = 100.f - col.aBase() * 100.f;
		transparency += ( 100.f - transparency ) * clamp( 1.f - visuals::players[ player->EntIndex() ].model_alpha / 255.f, 0.f, 1.f );
	}

	apply_material( type, col, col2, transparency, hidden );
}

bool chams::draw_player_chams( IMatRenderContext* context, DrawModelState_t& state, ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld )
{
	const auto player = globals::get_player( info.entity_index );

	if ( !vars::visuals.chams.local.type->get<int>() && !vars::visuals.chams.local.fake.type->get<int>() && player == local_player )
		return false;

	if ( !vars::visuals.chams.enemy.type->get<int>() && player->is_enemy() )
		return false;

	if ( !vars::visuals.chams.team.type->get<int>() && !player->is_enemy() && player != local_player )
		return false;

	if ( ( vars::visuals.chams.team.hidden->get<bool>() && !player->is_enemy() || vars::visuals.chams.enemy.hidden->get<bool>() && player->is_enemy() ) && player != local_player )
	{
		if ( get_type( player ) == material_index_t::MAT_RIM_GLOW )
		{
			handle_player( player, true, false, material_index_t::MAT_FLAT );
			original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, pCustomBoneToWorld );
		}
		handle_player( player, true );
		original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, pCustomBoneToWorld );
	}

	auto transparency = 0.f;
	if ( player == local_player )
	{
		if ( vars::visuals.chams.local.fake.type->get<int>() )
		{
			if ( animations::fake_record.m_index != 0 && animations::fake_record.has_animstate && animations::local_bone_setup( animations::fake_record ) )
			{
				auto info_copy = info;
				info_copy.body = 0;

				if ( vars::visuals.chams.local.fake.type->get<int>() == material_index_t::MAT_RIM_GLOW )
				{
					handle_player( player, true, true, material_index_t::MAT_FLAT );
					original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info_copy, animations::fake_record.m_state[ resolver_direction::resolver_networked ].m_matrix );
				}
				handle_player( player, true, true );
				original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info_copy, animations::fake_record.m_state[ resolver_direction::resolver_networked ].m_matrix );
			}

			if ( !vars::visuals.chams.local.type->get<int>() )
			{
				interfaces::model_render()->ForcedMaterialOverride();
				constexpr static float a[ 3 ] = { 1.f, 1.f, 1.f };
				interfaces::render_view()->SetColorModulation( a );

				if ( local_player->get_scoped() && vars::misc.transparency_in_scope->get<float>() )
					transparency = 100.f * ( vars::misc.transparency_in_scope->get<float>() / 100 );
				interfaces::render_view()->SetBlend( 1.f - transparency * 0.01f );

				return false;
			}
		}
	}

	if ( get_type( player ) == material_index_t::MAT_RIM_GLOW )
	{
		handle_player( player, false, false, material_index_t::MAT_FLAT );
		original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, pCustomBoneToWorld );
	}
	handle_player( player );
	original( hook::draw_model_execute )( interfaces::model_render()( ), 0, context, state, info, pCustomBoneToWorld );
	return true;
}

bool file_exists( const char* fileName )
{
	std::ifstream infile( fileName, std::ios::binary | std::ios::ate );
	return infile.good() && infile.tellg();
}


void chams::set_skybox()
{
	if ( vars::visuals.skybox->get<int>() >= 0 && vars::visuals.skybox->get<int>() > skyboxes.size() )
		return;

	var( r_3dsky );
	r_3dsky->set_int( !vars::visuals.skybox->get<int>() );
	var( sv_skyname );
	if ( vars::visuals.skybox->get<int>() )
	{
		_u( skybox, skyboxes[ vars::visuals.skybox->get<int>() - 1 ] );
		reinterpret_cast< void( __fastcall* )( const char* ) >( make_offset_multi( "engine.dll", sig_load_named_sky ) )( skybox.c_str() );
		r_3dsky->set_int( 1 );
	}
	else
		reinterpret_cast< void( __fastcall* )( const char* ) >( make_offset_multi( "engine.dll", sig_load_named_sky ) )( sv_skyname->value.string );
}

void chams::dark_mode( const int stage )
{
	if ( stage != FRAME_RENDER_START )
		return;

	static auto last_setting = false;
	static auto last_skybox = 0;

	if ( last_skybox != vars::visuals.skybox->get<int>() )
	{
		last_skybox = vars::visuals.skybox->get<int>();
		m_rerun_nightmode = true;
	}

	if ( last_setting != vars::visuals.dark->get<bool>() )
	{
		last_setting = vars::visuals.dark->get<bool>();
		m_rerun_nightmode = true;
	}

	//util::check_integrity();

	if ( !interfaces::engine()->IsInGame() || !local_player )
		return;

	if ( m_rerun_nightmode )
	{
		m_rerun_nightmode = false;

		set_skybox();
	}
}
