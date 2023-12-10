#include "../include_cheat.h"

// meme to avoid capture since can't pass those to lua without static rtti
struct
{
	IVModelRender* render;
	uint32_t edx;
	IMatRenderContext* context;
	DrawModelState_t* state;
	ModelRenderInfo_t* info;
	matrix3x4_t* bone;
} cur_data;

void dme_lua_callback()
{
	original( hook::draw_model_execute )( cur_data.render, cur_data.edx, cur_data.context, *cur_data.state, *cur_data.info, cur_data.bone );
}

void __fastcall hook::draw_model_execute( IVModelRender* model_render, int edx, IMatRenderContext* context, DrawModelState_t& state, ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld )
{
	globals::interpolated_curtime = interfaces::globals()->curtime;

	if ( chams::in_glow )
		return original( hook::draw_model_execute )( model_render, edx, context, state, info, pCustomBoneToWorld );

	static auto tickcount = 0;
	if ( tickcount != interfaces::globals()->tickcount )
	{
		static std::array < std::pair<std::string, IMaterial*>, 5 > vistasmoke_mats =
		{
			std::pair{ _w( "particle/vistasmokev1/vistasmokev1_fire" ), nullptr },
			std::pair{ _w( "particle/vistasmokev1/vistasmokev1_smokegrenade" ), nullptr },
			std::pair{ _w( "particle/vistasmokev1/vistasmokev1_emods" ), nullptr },
			std::pair{ _w( "particle/vistasmokev1/vistasmokev1_emods_impactdust" ), nullptr },
			std::pair{ _w( "models/weapons/shared/scope/scope_lens_dirt" ), nullptr }
		};

		for ( auto& mat : vistasmoke_mats )
		{
			if ( !mat.second )
			{
				_u( tex, TEXTURE_GROUP_OTHER );
				_u( mat_name, mat.first );
				mat.second = interfaces::material_system()->FindMaterial( mat_name.c_str(), tex.c_str() );
			}

			if ( !mat.second )
				continue;

			const auto is_lens_dirt = &mat == &vistasmoke_mats[ 4 ];
			mat.second->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, vars::misc.remove_smoke->get<bool>() || is_lens_dirt );
		}
	}

	tickcount = interfaces::globals()->tickcount;

	constexpr static float b[ 3 ] = { 1.f, 1.f, 1.f };
	interfaces::render_view()->SetColorModulation( b );

	const Vector def( 1.f, 1.f, 1.f );
	const auto org = context->GetToneMappingScaleLinear();

	context->SetToneMappingScaleLinear( def );

	cur_data.render = model_render;
	cur_data.edx = edx;
	cur_data.context = context;
	cur_data.state = &state;
	cur_data.info = &info;
	cur_data.bone = pCustomBoneToWorld;

	const auto draw_cbk = [] ( lua_State* l ) -> int
	{
		lua::runtime_state s( l );

		dme_lua_callback();

		return 0;
	};


	lua::api.callback( FNV1A( "on_draw_model_execute" ), [&] ( lua::state& s )
	{
		s.push( draw_cbk );
		s.push( info.entity_index );
		s.push( info.pModel->name );

		return 3;
	} );

	if ( chams::run( context, state, info, pCustomBoneToWorld ) )
		original( hook::draw_model_execute )( model_render, edx, context, state, info, pCustomBoneToWorld );

	context->SetToneMappingScaleLinear( org );

	interfaces::model_render()->ForcedMaterialOverride();
}

void DrawSprite( const Vector& vecOrigin, float flWidth, float flHeight, color32 color )
{
	unsigned char pColor[ 4 ] = { color.r, color.g, color.b, color.a };

	// Generate half-widths
	flWidth *= 0.5f;
	flHeight *= 0.5f;

	Vector right( 1, 0, 0 ), up( 0, 1, 0 );

	CMeshBuilder meshBuilder;
	Vector point{};
	CMatRenderContextPtr ctx( interfaces::material_system()( ) );
	IMesh* pMesh = ctx->GetDynamicMesh();

	meshBuilder.Begin( pMesh, MATERIAL_QUADS, 1 );

	meshBuilder.Color4ubv( pColor );
	meshBuilder.TexCoord2f( 0, 0, 1 );
	math::vector_ma( vecOrigin, -flHeight, up, point );
	math::vector_ma( point, -flWidth, right, point );
	meshBuilder.Position3fv( point.Base() );
	meshBuilder.AdvanceVertex();

	meshBuilder.Color4ubv( pColor );
	meshBuilder.TexCoord2f( 0, 0, 0 );
	math::vector_ma( vecOrigin, flHeight, up, point );
	math::vector_ma( point, -flWidth, right, point );
	meshBuilder.Position3fv( point.Base() );
	meshBuilder.AdvanceVertex();

	meshBuilder.Color4ubv( pColor );
	meshBuilder.TexCoord2f( 0, 1, 0 );
	math::vector_ma( vecOrigin, flHeight, up, point );
	math::vector_ma( point, flWidth, right, point );
	meshBuilder.Position3fv( point.Base() );
	meshBuilder.AdvanceVertex();

	meshBuilder.Color4ubv( pColor );
	meshBuilder.TexCoord2f( 0, 1, 1 );
	math::vector_ma( vecOrigin, -flHeight, up, point );
	math::vector_ma( point, flWidth, right, point );
	meshBuilder.Position3fv( point.Base() );
	meshBuilder.AdvanceVertex();

	meshBuilder.End();
	pMesh->Draw();
}

void DrawSpriteAdvanced( Vector min, Vector max, color32 color, float width )
{
	unsigned char pColor[ 4 ] = { color.r, color.g, color.b, color.a };

	if ( !interfaces::view_render()->GetViewSetup() )
		return;

	const auto cur_angles = math::calc_angle( ( min + max ) * 0.5f, interfaces::view_render()->GetViewSetup()->origin );

	Vector forward{};
	math::angle_vectors( cur_angles, &forward );

	auto vec_delta = ( max - min );
	math::fast_vec_normalize( vec_delta );

	QAngle hitbox_angle{};
	math::vector_angles( vec_delta, hitbox_angle );
	auto angle = hitbox_angle - cur_angles;
	angle.Clamp();

	auto right = forward.Cross( Vector( 0, 0, 1 ) ) * width * 0.5f;
	auto top = Vector( 0, 0, 1 ) * width * 0.5f;

	const auto is_horizontal = hitbox_angle.x < 20.f && hitbox_angle.x > -20.f;
	const auto is_flipped = angle.x < 0.f && !is_horizontal || is_horizontal && angle.y < 0.f;
	if ( is_flipped )
		std::swap( min, max );

	CMeshBuilder meshBuilder;
	Vector point{};
	CMatRenderContextPtr ctx( interfaces::material_system()( ) );
	IMesh* pMesh = ctx->GetDynamicMesh();

	meshBuilder.Begin( pMesh, MATERIAL_QUADS, 1 );

	//bot left
	meshBuilder.Color4ubv( pColor );
	meshBuilder.TexCoord2f( 0, 0, 1 );
	point = ( is_horizontal ? min : max ) - top + right;
	meshBuilder.Position3fv( point.Base() );
	meshBuilder.AdvanceVertex();

	//top left
	meshBuilder.Color4ubv( pColor );
	meshBuilder.TexCoord2f( 0, 0, 0 );
	point = min + top + right;
	meshBuilder.Position3fv( point.Base() );
	meshBuilder.AdvanceVertex();

	//top right
	meshBuilder.Color4ubv( pColor );
	meshBuilder.TexCoord2f( 0, 1, 0 );
	point = ( is_horizontal ? max : min ) + top - right;
	meshBuilder.Position3fv( point.Base() );
	meshBuilder.AdvanceVertex();

	//bot right
	meshBuilder.Color4ubv( pColor );
	meshBuilder.TexCoord2f( 0, 1, 1 );
	point = max - top - right;
	meshBuilder.Position3fv( point.Base() );
	meshBuilder.AdvanceVertex();

	meshBuilder.End();
	pMesh->Draw();
}


void draw_player_hitbox( aimbot::target_data_t& target )
{
	/*for ( auto i = 0; i < target.targeted_hitgroups.size(); i++ )
	{
		if ( target.targeted_hitgroups[ i ] == aimbot::target_data_t::target_hitbox_none )
			continue;

		auto hitbox = -1;
		switch ( 1 + i )
		{
			case HITGROUP_HEAD:
				hitbox = HITBOX_HEAD;
				break;
			case HITGROUP_CHEST:
				hitbox = HITBOX_UPPER_CHEST;
				break;
			case HITGROUP_STOMACH:
				hitbox = HITBOX_BODY;
				break;
			case HITGROUP_LEFTARM:
				hitbox = HITBOX_LEFT_UPPER_ARM;
				break;
			case HITGROUP_RIGHTARM:
				hitbox = HITBOX_RIGHT_UPPER_ARM;
				break;
			case HITGROUP_LEFTLEG:
				hitbox = HITBOX_LEFT_CALF;
				break;
			case HITGROUP_RIGHTLEG:
				hitbox = HITBOX_RIGHT_CALF;
				break;
			case HITGROUP_NECK:
				hitbox = HITBOX_NECK;
				break;
			default:
				break;
		}

		if ( hitbox == -1 )
			continue;

		auto [min, max] = player->get_hitbox_minmax( hitbox );
		if ( min.IsZero() && max.IsZero() )
			continue;

		const auto r = target.targeted_hitgroups[ i ] == aimbot::target_data_t::target_hitbox_scanned;
		const auto g = target.targeted_hitgroups[ i ] == aimbot::target_data_t::target_hitbox_chosen || target.targeted_hitgroups[ i ] == aimbot::target_data_t::target_hitbox_chosen_safe_point;
		const auto b = target.targeted_hitgroups[ i ] == aimbot::target_data_t::target_hitbox_safe_point || target.targeted_hitgroups[ i ] == aimbot::target_data_t::target_hitbox_chosen_safe_point;

		_( color, "$color" );
		chams::materials[ MAT_SPRITE ]->FindVar( color.c_str(), nullptr )->set_vec_val( r, g, b );
		CMatRenderContextPtr render_context = interfaces::material_system()->GetRenderContext();
		render_context->Bind( chams::materials[ MAT_SPRITE ] );

		DrawSpriteAdvanced( min, max, { 255, 255, 255, 255 }, 15.f );

		render_context.SafeRelease();
	}*/
}

//void __fastcall hook::render_glow_boxes( CGlowObjectManager* manager, void* edx, int pass, CMatRenderContextPtr& render_ctx )
//{
//	if ( !render_ctx )
//		return original( render_glow_boxes )( manager, edx, pass, render_ctx );
//
//	static auto AddGlowBox = reinterpret_cast< int( __thiscall* )( void*, Vector, Vector, Vector, Vector, Color, float ) >( sig(
//		"client.dll", "55 8B EC 53 56 8D" ) );
//
//	if ( !manager->m_GlowBoxDefinitions.Count() )
//		AddGlowBox( manager, Vector( 9999999.f ), Vector( 0.f ), Vector( 0.f ), Vector( 0.f ), Color( 255, 255, 255, 100 ), 99999 );
//
//	const Vector def( 1.f, 1.f, 1.f );
//	const Vector dark( 0.5f, 0.5f, 0.5f );
//	const auto org = render_ctx->GetToneMappingScaleLinear();
//	render_ctx->SetToneMappingScaleLinear( dark );
//
//	if ( misc::has_position )
//	{
//		auto col = Color( vars::misc.peek_assist_col->get<D3DCOLOR>() ).fade_to( Color( vars::misc.peek_assist_col2->get<D3DCOLOR>() ), misc::retract_peek );
//		if ( misc::cant_reach )
//			col = Color::RedAccent();
//
//		CMatRenderContextPtr render_context = interfaces::material_system()->GetRenderContext();
//		render_context->Bind( chams::materials[ MAT_SPRITE ] );
//
//		IVDebugOverlay::Circle( misc::movement_position + Vector( 0, 0, 0 ), QAngle( 90, 0, 0 ), 15, Color( col, 75 ), true );
//		IVDebugOverlay::Circle( misc::movement_position + Vector( 0, 0, 0 ), QAngle( -90, 0, 0 ), 15, Color( col, 75 ), true );
//
//		render_context.SafeRelease();
//	}
//
//	for ( const auto& target : aimbot::targets )
//	{
//		const auto player = globals::get_player( target.index );
//		if ( !player )
//			continue;
//
//		for ( const auto& hitbox : target.hitboxes )
//		{
//			const auto col = hitbox.second ? Color( 0, 75, 150, 255 ) : Color( 150, 30, 30, 255 );
//
//			_( color, "$color" );
//			chams::materials[ MAT_SPRITE ]->FindVar( color.c_str(), nullptr )->set_vec_val( col.r(), col.g(), col.b() );
//			chams::materials[ MAT_SPRITE ]->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
//
//			CMatRenderContextPtr render_context = interfaces::material_system()->GetRenderContext();
//			render_context->Bind( chams::materials[ MAT_SPRITE ] );
//
//			auto [min, max, radius] = player->get_hitbox_minmax( hitbox.first );
//			DrawSpriteAdvanced( min, max, { 255, 255, 255, 75 }, radius );
//
//			render_context.SafeRelease();
//		}
//	}
//
//	render_ctx->SetToneMappingScaleLinear( def );
//
//	original( render_glow_boxes )( manager, edx, pass, render_ctx );
//
//	render_ctx->SetToneMappingScaleLinear( org );
//}

void __fastcall hook::draw_3d_debug_overlays( void* ecx, void* edx )
{
	CMatRenderContextPtr ctx( interfaces::material_system()( )->GetRenderContext() );
	const Vector def( 1.f, 1.f, 1.f );
	const auto org = ctx->GetToneMappingScaleLinear();
	ctx->SetToneMappingScaleLinear( def );

	if ( misc::has_position )
	{
		auto col = Color( vars::misc.peek_assist_col->get<D3DCOLOR>() ).fade_to( Color( vars::misc.peek_assist_col2->get<D3DCOLOR>() ), misc::retract_peek );
		if ( misc::cant_reach )
			col = Color::RedAccent();

		_( color, "$color" );
		chams::materials[ MAT_SPRITE ]->FindVar( color.c_str(), nullptr )->set_vec_val( col.rBase() * 4, col.gBase() * 4, col.bBase() * 4 );
		chams::materials[ MAT_SPRITE ]->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );

		CMatRenderContextPtr ctx2( interfaces::material_system()( ) );
		ctx2->Bind( chams::materials[ MAT_SPRITE ] );

		DrawSprite( misc::movement_position + Vector( 0, 0, 0 ), 50, 50, { 255, 255, 255, 255 } );
	}

	if ( vars::aim.visualize_aimbot->get<bool>() )
		for ( const auto& target : aimbot::targets )
		{
			const auto player = globals::get_player( target.index );
			if ( !player )
				continue;

			for ( const auto& hitbox : target.hitboxes )
			{
				const auto col = hitbox.second ? Color( vars::aim.visualize_color_safe->get<D3DCOLOR>() ) : Color( vars::aim.visualize_color->get<D3DCOLOR>() );
				_( color, "$color" );
				chams::materials[ MAT_SPRITE ]->FindVar( color.c_str(), nullptr )->set_vec_val( col.rBase() * 50.f * col.aBase(), col.gBase() * 50.f * col.aBase(), col.bBase() * 50.f * col.aBase() );
				chams::materials[ MAT_SPRITE ]->FindVar( "$nocull", nullptr )->set_int_value( 1 );
				chams::materials[ MAT_SPRITE ]->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );

				CMatRenderContextPtr render_context( interfaces::material_system()( ) );
				render_context->Bind( chams::materials[ MAT_SPRITE ] );

				auto [min, max, radius] = player->get_hitbox_minmax( hitbox.first );
				DrawSpriteAdvanced( min, max, { 255, 255, 255, 255 }, radius * 2.f );

				render_context.SafeRelease();
			}
		}

	grenade_pred::simulate();
	grenade_pred::paint();

#ifdef _DEBUG
	/*for ( const auto ent : interfaces::entity_list()->get_players() )
	{
		auto res = &player_log::anim_debug_info[ ent->EntIndex() ].memory[ 0 ];
		for ( auto i = 1u; i < 150u; ++i )
		{
			const auto cur = &player_log::anim_debug_info[ ent->EntIndex() ].memory[ i ];
			if ( cur->key > res->key && ( cur->value.m_has_data & ( anim_debug_info_t::CLIENT | anim_debug_info_t::SERVER ) ) == ( anim_debug_info_t::CLIENT | anim_debug_info_t::SERVER ) )
				res = cur;
		}

		if ( res->key && ( res->value.m_has_data & ( anim_debug_info_t::CLIENT | anim_debug_info_t::SERVER ) ) == ( anim_debug_info_t::CLIENT | anim_debug_info_t::SERVER ) )
		{
			aimbot_helpers::draw_debug_hitboxes( ent, res->value.m_server_bones, -1, -1.f, Color( 0, 0, 255, 255 ) );
			aimbot_helpers::draw_debug_hitboxes( ent, res->value.m_client_bones, -1, -1.f, Color( 255, 0, 0, 255 ) );
		}
	}*/
#endif

	original( draw_3d_debug_overlays )( ecx, edx );

	ctx->SetToneMappingScaleLinear( org );
	ctx.SafeRelease();
}

void __fastcall hook::shader_api_draw_mesh( void* ecx, void* edx, uintptr_t pMesh, int nCount, MeshInstanceData_t* pInstances, int nCompressionType, uintptr_t pCompiledState, uintptr_t pInfo )
{
	_( World, "World" );
	_( vgui_white, "vgui_white" );
	_( SkyBox, "SkyBox" );

	const auto material = *reinterpret_cast< IMaterial** >( reinterpret_cast< uintptr_t >( ecx ) + 0x2FA4 );
	if ( material && ( material->GetTextureGroupName()[ 6 ] == 'P' || material->GetName()[ 7 ] == 'p' && material->GetName()[ 8 ] == 'r' && material->GetName()[ 9 ] == 'o' && material->GetName()[ 17 ] == 'c' && material->GetName()[ 18 ] == 'h' && material->GetName()[ 19 ] == 'i' ) )
	{
		const auto alpha = 100.f - vars::visuals.prop_transparency->get<float>();
		for ( auto i = 0; i < nCount; i++ )
		{
			auto col = Color( vars::visuals.prop_color->get<D3DCOLOR>() );
			pInstances[ i ].m_DiffuseModulation.w = alpha * 0.01f;
			if ( vars::visuals.dark->get<bool>() )
			{
				pInstances[ i ].m_DiffuseModulation.x = col.rBase();
				pInstances[ i ].m_DiffuseModulation.y = col.gBase();
				pInstances[ i ].m_DiffuseModulation.z = col.bBase();
			}
		}
	}
	else if ( material && vars::visuals.dark->get<bool>() && strstr( material->GetTextureGroupName(), World.c_str() ) )
	{
		for ( auto i = 0; i < nCount; i++ )
		{
			auto col = Color( vars::visuals.dark_color->get<D3DCOLOR>() );
			pInstances[ i ].m_DiffuseModulation.x = col.rBase();
			pInstances[ i ].m_DiffuseModulation.y = col.gBase();
			pInstances[ i ].m_DiffuseModulation.z = col.bBase();
		}

	}
	/*else if ( material && vars::visuals.dark->get<bool>() && strstr( material->GetName(), vgui_white.c_str() ) )
	{
		for ( auto i = 0; i < nCount; i++ )
		{
			pInstances[ i ].m_DiffuseModulation.x = 60.f / 100.f;
			pInstances[ i ].m_DiffuseModulation.y = 60.f / 100.f;
			pInstances[ i ].m_DiffuseModulation.z = 65.f / 100.f;
		}
	}*/
	else if ( material && vars::visuals.skybox->get<int>() && strstr( material->GetTextureGroupName(), SkyBox.c_str() ) )
	{
		const auto col = Color( vars::visuals.skybox_color->get<D3DCOLOR>() );
		for ( auto i = 0; i < nCount; i++ )
		{
			pInstances[ i ].m_DiffuseModulation.x = col.rBase() * 10.f;
			pInstances[ i ].m_DiffuseModulation.y = col.gBase() * 10.f;
			pInstances[ i ].m_DiffuseModulation.z = col.bBase() * 10.f;
		}
	}

	original( shader_api_draw_mesh )( ecx, edx, pMesh, nCount, pInstances, nCompressionType, pCompiledState, pInfo );
}

void __fastcall hook::draw_model_studio( IStudioRender* studio_render, int edx, void* results, DrawModelInfo_t* info, matrix3x4_t* bone_to_world, float* flex_weights, float* flex_delayed_weights, Vector& model_origin, int flags )
{
	if ( entity_rendering::player_render && info->m_pStudioHdr && entity_rendering::preview_model && interfaces::mdl_cache()->GetStudioHdr( entity_rendering::preview_model->RootMDL.hModelHandle ) == info->m_pStudioHdr && bone_to_world )
		memcpy( entity_rendering::preview_matrix, bone_to_world, sizeof( matrix3x4_t ) * info->m_pStudioHdr->numbones );

	else if ( info->m_pStudioHdr && fnv1a_rt( info->m_pStudioHdr->name ) == FNV1A( "weapons\\w_snip_ssg08.mdl" ) && entity_rendering::player_render )
	{
		auto type = 0;
		if ( vars::visuals.chams.enabled->get<bool>() )
		{
			if ( vars::menu.preview_type->get<int>() == 0 )
				type = vars::visuals.chams.enemy.attachment.type->get<int>();
			else if ( vars::menu.preview_type->get<int>() == 2 )
				type = vars::visuals.chams.local.attachment.type->get<int>();
			else
				type = vars::visuals.chams.team.attachment.type->get<int>();

			if ( !vars::visuals.chams.enabled->get<bool>() )
				type = 0;

			if ( type == MAT_RIM_GLOW || type == MAT_LEGACY )
			{
				auto col = Color( 36, 31, 66 );
				auto col2 = Color();
				chams::apply_material( MAT_FLAT, col, col2, 0.f, false );
				original( draw_model_studio )( studio_render, edx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags );
			}

			if ( type == MAT_RIM_GLOW )
			{
				chams::handle_preview_attachments( MAT_FLAT );
				original( draw_model_studio )( studio_render, edx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags );
			}

			chams::handle_preview_attachments();
		}

		interfaces::studio_render()->SetAmbientLightColors2( type ? entity_rendering::light_col2.data() : entity_rendering::light_col1.data() );

		original( draw_model_studio )( studio_render, edx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags );

		interfaces::model_render()->ForcedMaterialOverride();

		return;
	}


	original( draw_model_studio )( studio_render, edx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags );
}