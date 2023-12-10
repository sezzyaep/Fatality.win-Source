#include "../include_cheat.h"
#include "ren/adapter_dx9.h"

bool entity_rendering::screen_transform( const Vector& in, Vector& out )
{
	const auto& w2_s_matrix = world_to_projection;
	out.x = w2_s_matrix[ 0 ][ 0 ] * in[ 0 ] + w2_s_matrix[ 0 ][ 1 ] * in[ 1 ] + w2_s_matrix[ 0 ][ 2 ] * in[ 2 ] + w2_s_matrix[ 0 ][ 3 ];
	out.y = w2_s_matrix[ 1 ][ 0 ] * in[ 0 ] + w2_s_matrix[ 1 ][ 1 ] * in[ 1 ] + w2_s_matrix[ 1 ][ 2 ] * in[ 2 ] + w2_s_matrix[ 1 ][ 3 ];
	out.z = 0.0f;

	const auto w = w2_s_matrix[ 3 ][ 0 ] * in.x + w2_s_matrix[ 3 ][ 1 ] * in.y + w2_s_matrix[ 3 ][ 2 ] * in.z + w2_s_matrix[ 3 ][ 3 ];

	if ( w < 0.001f )
	{
		out.x *= 100000;
		out.y *= 100000;
		return false;
	}

	const auto invw = 1.0f / w;
	out.x *= invw;
	out.y *= invw;

	return true;
}

bool entity_rendering::world_to_screen( const Vector& in, Vector& out )
{
	const auto result = screen_transform( in, out );

	out.x = ( view_setup.width / 2.0f ) + ( out.x * view_setup.width ) / 2.0f;
	out.y = ( view_setup.height / 2.0f ) - ( out.y * view_setup.height ) / 2.0f;

	return result;
}

void entity_rendering::paint()
{
	if ( !preview_texture )
	{
		interfaces::material_system()->ForceBeginRenderTargetAllocation();

		preview_texture = interfaces::material_system()->CreateNamedRenderTargetTextureEx( std::to_string( RandomInt( 0, INT_MAX ) ).c_str(), 235, 370, RT_SIZE_NO_CHANGE, interfaces::material_system()->GetBackBufferFormat(), MATERIAL_RT_DEPTH_SHARED, TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT, CREATERENDERTARGETFLAGS_HDR );

		interfaces::material_system()->ForceEndRenderTargetAllocation();
		interfaces::material_system()->FinishRenderTargetAllocation();
	}

	if ( !cubemap_texture )
	{
		_u( cube_map, TEXTURE_GROUP_CUBE_MAP );
		cubemap_texture = interfaces::material_system()->FindTexture( _r( "editor/cubemap.hdr" ), cube_map.c_str() );
	}

	static auto previous_model_idx = vars::visuals.model_t->get<int>();
	static auto previous_model_idx_t = vars::visuals.model_t->get<int>();
	static auto previous_model_idx_ct = vars::visuals.model_ct->get<int>();
	static auto model_idx = vars::visuals.model_t->get<int>();
	if ( vars::visuals.model_t->get<int>() && vars::visuals.model_t->get<int>() != previous_model_idx_t )
		model_idx = vars::visuals.model_t->get<int>();
	else if ( vars::visuals.model_ct->get<int>() && vars::visuals.model_ct->get<int>() != previous_model_idx_ct )
		model_idx = vars::visuals.model_ct->get<int>();

	if ( interfaces::engine()->IsInGame() && local_player && local_player->get_alive() )
		model_idx = local_player->get_team() == 2 ? vars::visuals.model_t->get<int>() : vars::visuals.model_ct->get<int>();

	if ( !model_idx )
		model_idx = 46;

	if ( model_idx != previous_model_idx && preview_model )
	{
		interfaces::memalloc()->Free( preview_model );
		preview_model = nullptr;
	}

	previous_model_idx = model_idx;
	previous_model_idx_t = vars::visuals.model_t->get<int>();
	previous_model_idx_ct = vars::visuals.model_ct->get<int>();

	if ( g_menu->get_alpha() == 0.f )
		return;

	if ( !preview_model )
	{
		preview_model = static_cast< C_MergedMDL* >( interfaces::memalloc()->Alloc( 0x75C ) ); //(+0xCD) E8 ? ? ? ? 84 C0 75 28 8B 56 18
		preview_model->create();

		_u( model_name, skinchanger::player_model_list.at( model_idx ) );

		preview_model->SetMDL( _r( "models/player/custom_player/uiplayer/animset_uiplayer.mdl" ) );
		preview_model->SetMergedMDL( model_name.c_str() );
		preview_model->SetMergedMDL( _r( "models/weapons/w_snip_ssg08.mdl" ) );

		preview_model->SetSequence( 113, false ); // 12, 13, 16,    21
		preview_model->SetupBonesForAttachmentQueries();
		preview_model->RootMDL.flTime = 20.f;
	}

	preview_model->RootMDL.flTime += interfaces::globals()->frametime * 0.5f;

	view_setup.x = 0;
	view_setup.y = 0;
	view_setup.width = 235;
	view_setup.height = 370;
	view_setup.m_bOrtho = false;
	view_setup.fov = 12;
	view_setup.origin = Vector( -329, 0.f, 52 );
	view_setup.angles = QAngle( 2, 0.f, 0 );
	view_setup.zNear = 20.0f;
	view_setup.zFar = 1000.f;
	view_setup.m_bDoBloomAndToneMapping = true;

	VMatrix v1{}, v2{}, v3{};
	interfaces::render_view()->GetMatricesForView( view_setup, &v1, &v2, &world_to_projection, &v3 );

	CMatRenderContextPtr ctx( interfaces::material_system()( ) );

	ctx->PushRenderTargetAndViewport();
	ctx->SetRenderTarget( preview_texture );
	ctx->BindLocalCubemap( cubemap_texture );
	ctx->SetLightingOrigin( { -65.0f, 10.0f, 100.0f } );
	ctx->SetIntRenderingParameter( 10, 0 );

	Frustum dummyFrustum;
	interfaces::render_view()->Push3DView( ctx, view_setup, VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH | VIEW_CLEAR_STENCIL, preview_texture, dummyFrustum );

	auto type = 0;
	if ( vars::menu.preview_type->get<int>() == 0 )
		type = vars::visuals.chams.enemy.type->get<int>();
	else if ( vars::menu.preview_type->get<int>() == 2 )
		type = vars::visuals.chams.local.type->get<int>();
	else
		type = vars::visuals.chams.team.type->get<int>();

	if ( !vars::visuals.chams.enabled->get<bool>() )
		type = 0;

	ctx->ClearColor4ub( false, false, false, false );
	ctx->ClearBuffers( true, true, true );
	ctx->SetAmbientLightCube( type ? light_col2.data() : light_col1.data() );

	interfaces::studio_render()->SetAmbientLightColors2( type ? light_col2.data() : light_col1.data() );
	interfaces::studio_render()->SetLocalLights2( 0, nullptr );

	matrix3x4_t matPlayerView = {};
	math::angle_matrix( QAngle( 0, 180.f, 0 ), matPlayerView );

	interfaces::model_render()->SuppressEngineLighting( true );

	if ( type )
	{
		if ( type == MAT_RIM_GLOW || type == MAT_LEGACY )
		{
			auto col = Color( 36, 31, 66 );
			auto col2 = Color();
			chams::apply_material( MAT_FLAT, col, col2, 0.f, false );
			preview_model->Draw( matPlayerView );
		}

		if ( vars::visuals.chams.enemy.hidden->get<bool>() && !vars::menu.preview_type->get<int>() || vars::visuals.chams.team.hidden->get<bool>() && vars::menu.preview_type->get<int>() == 1 )
		{
			if ( chams::get_type( vars::menu.preview_type->get<int>() ) == material_index_t::MAT_RIM_GLOW )
			{
				chams::handle_preview( true, material_index_t::MAT_FLAT );
				preview_model->Draw( matPlayerView );
			}
			chams::handle_preview( true );
			preview_model->Draw( matPlayerView );
		}

		if ( chams::get_type( vars::menu.preview_type->get<int>() ) == material_index_t::MAT_RIM_GLOW )
		{
			chams::handle_preview( false, material_index_t::MAT_FLAT );
			preview_model->Draw( matPlayerView );
		}
		chams::handle_preview( false );
	}

	player_render = true;
	preview_model->Draw( matPlayerView );
	player_render = false;

	interfaces::model_render()->SuppressEngineLighting( false );

	interfaces::render_view()->PopView( ctx, dummyFrustum );
	ctx->BindLocalCubemap( nullptr );

	ctx->PopRenderTargetAndViewport();

	//visuals::queue_custom_player_esp();

	is_setup = true;
}