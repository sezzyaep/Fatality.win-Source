#ifdef EVO_USE_DX9

#ifndef EV0LVE_RENDERER_ADAPTER_DX9_H
#define EV0LVE_RENDERER_ADAPTER_DX9_H

#include <ren/adapter.h>
#include <ren/command.h>

#include <d3d9.h>
#include <d3dx9.h>

namespace evo::ren
{
	class backup_dx9
	{
	public:
		void backup( IDirect3DDevice9* device )
		{
			device->GetTransform( D3DTS_WORLD, &world );
			device->GetTransform( D3DTS_VIEW, &view );
			device->GetTransform( D3DTS_PROJECTION, &projection );

			device->GetViewport( &viewport );
			device->GetPixelShader( &pixel_shader );
			device->GetVertexShader( &vertex_shader );

			device->GetRenderState( D3DRS_CULLMODE, &rs_cull_mode );
			device->GetRenderState( D3DRS_LIGHTING, &rs_lighting );
			device->GetRenderState( D3DRS_ZENABLE, &rs_z_enable );
			device->GetRenderState( D3DRS_ALPHABLENDENABLE, &rs_alpha_blend_enable );
			device->GetRenderState( D3DRS_ALPHATESTENABLE, &rs_alpha_test_enable );
			device->GetRenderState( D3DRS_BLENDOP, &rs_blend_op );
			device->GetRenderState( D3DRS_SRCBLEND, &rs_src_blend );
			device->GetRenderState( D3DRS_DESTBLEND, &rs_dest_blend );
			device->GetRenderState( D3DRS_SCISSORTESTENABLE, &rs_scissor_test_enable );
			device->GetRenderState( D3DRS_SHADEMODE, &rs_shade_mode );
			device->GetRenderState( D3DRS_FOGENABLE, &rs_fog_enable );
			device->GetTextureStageState( 0, D3DTSS_COLOROP, &tss_color_op );
			device->GetTextureStageState( 0, D3DTSS_COLORARG1, &tss_color_arg_1 );
			device->GetTextureStageState( 0, D3DTSS_COLORARG2, &tss_color_arg_2 );
			device->GetTextureStageState( 0, D3DTSS_ALPHAOP, &tss_alpha_op );
			device->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &tss_alpha_arg_1 );
			device->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &tss_alpha_arg_2 );
			device->GetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, &tss_ttff );
			device->GetSamplerState( 0, D3DSAMP_MINFILTER, &samp_min_filter );
			device->GetSamplerState( 0, D3DSAMP_MAGFILTER, &samp_mag_filter );

			device->GetFVF( &fvf );
			device->GetStreamSource( 0, &vertex_buffer, &offset_in_bytes, &stride );
			device->GetIndices( &index_buffer );

			device->GetRenderState( D3DRS_SRGBWRITEENABLE, &srgbwrite );
			device->GetRenderState( D3DRS_SRGBWRITEENABLE, &colorwrite );

			device->GetTexture( 0, &texture );
			device->GetRenderTarget( 0, &target );
			device->GetVertexDeclaration( &vertex_declaration );

		}

		void restore( IDirect3DDevice9* device )
		{
			device->SetTransform( D3DTS_WORLD, &world );
			device->SetTransform( D3DTS_VIEW, &view );
			device->SetTransform( D3DTS_PROJECTION, &projection );

			device->SetViewport( &viewport );
			device->SetPixelShader( pixel_shader );
			device->SetVertexShader( vertex_shader );

			device->SetRenderState( D3DRS_CULLMODE, rs_cull_mode );
			device->SetRenderState( D3DRS_LIGHTING, rs_lighting );
			device->SetRenderState( D3DRS_ZENABLE, rs_z_enable );
			device->SetRenderState( D3DRS_ALPHABLENDENABLE, rs_alpha_blend_enable );
			device->SetRenderState( D3DRS_ALPHATESTENABLE, rs_alpha_test_enable );
			device->SetRenderState( D3DRS_BLENDOP, rs_blend_op );
			device->SetRenderState( D3DRS_SRCBLEND, rs_src_blend );
			device->SetRenderState( D3DRS_DESTBLEND, rs_dest_blend );
			device->SetRenderState( D3DRS_SCISSORTESTENABLE, rs_scissor_test_enable );
			device->SetRenderState( D3DRS_SHADEMODE, rs_shade_mode );
			device->SetRenderState( D3DRS_FOGENABLE, rs_fog_enable );
			device->SetTextureStageState( 0, D3DTSS_COLOROP, tss_color_op );
			device->SetTextureStageState( 0, D3DTSS_COLORARG1, tss_color_arg_1 );
			device->SetTextureStageState( 0, D3DTSS_COLORARG2, tss_color_arg_2 );
			device->SetTextureStageState( 0, D3DTSS_ALPHAOP, tss_alpha_op );
			device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, tss_alpha_arg_1 );
			device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, tss_alpha_arg_2 );
			device->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, tss_ttff );
			device->SetSamplerState( 0, D3DSAMP_MINFILTER, samp_min_filter );
			device->SetSamplerState( 0, D3DSAMP_MAGFILTER, samp_mag_filter );

			device->SetFVF( fvf );
			device->SetStreamSource( 0, vertex_buffer, offset_in_bytes, stride );
			device->SetIndices( index_buffer );

			device->SetRenderState( D3DRS_SRGBWRITEENABLE, srgbwrite );
			device->SetRenderState( D3DRS_SRGBWRITEENABLE, colorwrite );

			device->SetTexture( 0, texture );
			device->SetRenderTarget( 0, target );
			device->SetVertexDeclaration( vertex_declaration );

		}

	private:
		D3DMATRIX world{}, view{}, projection{};
		D3DVIEWPORT9 viewport{};
		IDirect3DPixelShader9* pixel_shader{};
		IDirect3DVertexShader9* vertex_shader{};
		DWORD rs_cull_mode{}, rs_lighting{}, rs_z_enable{}, rs_alpha_blend_enable{}, rs_alpha_test_enable{};
		DWORD rs_blend_op{}, rs_src_blend{}, rs_dest_blend{}, rs_scissor_test_enable{}, rs_shade_mode{}, rs_fog_enable{};
		DWORD tss_color_op{}, tss_color_arg_1{}, tss_color_arg_2{}, tss_alpha_op{}, tss_alpha_arg_1{}, tss_alpha_arg_2{};
		DWORD tss_ttff{};
		DWORD samp_min_filter{}, samp_mag_filter{};
		DWORD fvf{};
		IDirect3DVertexBuffer9* vertex_buffer{};
		UINT offset_in_bytes{};
		UINT stride{};
		IDirect3DIndexBuffer9* index_buffer{};


		DWORD srgbwrite{};
		DWORD colorwrite{};
		IDirect3DBaseTexture9* texture = nullptr;
		IDirect3DSurface9* target = nullptr;
		IDirect3DVertexDeclaration9* vertex_declaration = nullptr;

	};

	class vertex_dx9
	{
	public:
		vertex_dx9( const vertex& vtx )
		{
			const auto p = vtx.get_pos() - 0.5f;
			const auto u = vtx.get_uv();
			const auto c = vtx.get_color();

			pos[ 0 ] = p.x;
			pos[ 1 ] = p.y;
			uv[ 0 ] = u.x;
			uv[ 1 ] = u.y;
			col = D3DCOLOR_ARGB( c.a(), c.r(), c.g(), c.b() );
		}

	private:
		float pos[ 3 ]{};
		uint32_t col{};
		float uv[ 2 ]{};
	};

	class adapter_dx9 : public adapter_base
	{
	public:
		adapter_dx9( IDirect3DDevice9* dev, HWND wnd ) : adapter_base( wnd ), device( dev )
		{
			name = "DirectX 9";
		}

		void create_objects() override;
		void destroy_objects() override;
		void render() override;

		void* create_texture( void* arr, uint32_t w, uint32_t h, uint32_t p ) override;
		void destroy_texture( void* tex ) override;

	private:
		IDirect3DDevice9* device{};
		IDirect3DVertexBuffer9* vertex_buffer{};
		IDirect3DIndexBuffer9* index_buffer{};
		IDirect3DTexture9* texture_object{};

		size_t vb_size{ 5000 };
		size_t ib_size{ 10000 };

		backup_dx9 backup{};
	};
}

#endif //EV0LVE_RENDERER_ADAPTER_DX9_H
#endif