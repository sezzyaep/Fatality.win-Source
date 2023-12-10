#ifdef EVO_USE_DX11

#ifndef EV0LVE_RENDERER_ADAPTER_DX11_H
#define EV0LVE_RENDERER_ADAPTER_DX11_H

#include <ren/adapter.h>

#pragma warning (push)
#pragma warning (disable: 4005)

#include <D3D11.h>
#include <D3DX11.h>
#include <dinput.h>
#include <D3Dcompiler.h>

#pragma warning (pop)

namespace evo::ren
{
	class adapter_dx11 : public adapter_base
	{
	public:
		adapter_dx11(ID3D11Device* dev, ID3D11DeviceContext* c, HWND wnd) : adapter_base(wnd), device(dev), ctx(c)
		{
			name = "DirectX 11";
		}

		void create_objects() override;
		void destroy_objects() override;
		void render() override;

		void* create_texture(void* arr, uint32_t w, uint32_t h, uint32_t p) override;
		void destroy_texture(void* tex) override;

	private:
		void create_const_buffer();
		void create_blend_state();
		void create_rasterizer_state();
		void create_aa_rasterizer_state();
		void create_depth_stencil_state();
		void create_sampler_state();

		ID3D11Device* device{};
		ID3D11DeviceContext* ctx{};
		ID3D11VertexShader* vertex_shader{};
		ID3D11PixelShader* pixel_shader{};
		ID3D11InputLayout* input_layout{};
		ID3D11Buffer* vertex_buffer{};
		ID3D11Buffer* index_buffer{};
		ID3D11DepthStencilState* depth_stencil{};
		ID3D11RasterizerState* rasterizer_state{};
		ID3D11RasterizerState* aa_rasterizer_state{};
		ID3D11BlendState* blend_state{};
		ID3D11Buffer* constant_buffer{};
		ID3D11SamplerState* texture_sampler{};
		ID3D11ShaderResourceView* texture_object{};

		size_t vb_size{5000};
		size_t ib_size{10000};

		bool objects_created{};
	};
}

#endif //EV0LVE_RENDERER_ADAPTER_DX11_H
#endif