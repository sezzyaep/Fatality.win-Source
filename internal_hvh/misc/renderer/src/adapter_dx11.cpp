#ifdef EVO_USE_DX11

#include <ren/adapter_dx11.h>
#include <ren/command.h>
#include <stdexcept>

using namespace evo::ren;

void adapter_dx11::create_objects()
{
	adapter_base::create_objects();

	if (objects_created)
		return;

	ID3D10Blob* vs_blob{};
	ID3D10Blob* ps_blob{};

	static const char* vs_src = "cbuffer vertex_buffer:register(b0){float4x4 projection_matrix;};struct VS_INPUT{float2 pos:POSITION;float4 col:COLOR0;float2 uv:TEXCOORD0;};struct PS_INPUT{float4 pos:SV_POSITION;float4 col:COLOR0;float2 uv:TEXCOORD0;};PS_INPUT main(VS_INPUT input){PS_INPUT output;output.pos=mul(projection_matrix,float4(input.pos.xy,0.f,1.f));output.col=input.col;output.uv=input.uv;return output;}";
	static const char* ps_src = "struct PS_INPUT{float4 pos:SV_POSITION;float4 col:COLOR0;float2 uv:TEXCOORD0;};sampler sampler0;Texture2D texture0;float4 main(PS_INPUT input):SV_Target{return input.col*texture0.Sample(sampler0,input.uv);}";

	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0x00, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0x08, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 0x10, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	if (FAILED(D3DCompile(vs_src, strlen(vs_src), nullptr, nullptr, nullptr, "main", "vs_4_0", 0, 0, &vs_blob, nullptr)))
		throw std::runtime_error("Failed to compile vertex shader");
	if (FAILED(D3DCompile(ps_src, strlen(ps_src), nullptr, nullptr, nullptr, "main", "ps_4_0", 0, 0, &ps_blob, nullptr)))
		throw std::runtime_error("Failed to compile pixel shader");
	if (FAILED(device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &vertex_shader)))
		throw std::runtime_error("Failed to create vertex shader");
	if (FAILED(device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &pixel_shader)))
		throw std::runtime_error("Failed to create pixel shader");
	if (FAILED(device->CreateInputLayout(layout, 3, vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &input_layout)))
		throw std::runtime_error("Failed to create input layout");

	vs_blob->Release();
	ps_blob->Release();

	create_const_buffer();
	create_blend_state();
	create_rasterizer_state();
	create_aa_rasterizer_state();
	create_depth_stencil_state();
	create_sampler_state();

	auto pixel = 0xFFFFFFFFu;
	texture_object = reinterpret_cast<ID3D11ShaderResourceView*>(create_texture(&pixel, 1, 1, 4));

	objects_created = true;
}

void adapter_dx11::create_const_buffer()
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(vertex_constant_buffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	if (FAILED(device->CreateBuffer(&desc, nullptr, &constant_buffer)))
		throw std::runtime_error("Failed to create vertex constant buffer");
}

void adapter_dx11::create_blend_state()
{
	D3D11_BLEND_DESC desc{};
	desc.AlphaToCoverageEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(device->CreateBlendState(&desc, &blend_state)))
		throw std::runtime_error("Failed to create blend state");
}

void adapter_dx11::create_rasterizer_state()
{
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	desc.ScissorEnable = true;
	desc.DepthClipEnable = true;

	if (FAILED(device->CreateRasterizerState(&desc, &rasterizer_state)))
		throw std::runtime_error("Failed to create rasterizer state");
}

void adapter_dx11::create_aa_rasterizer_state()
{
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	desc.ScissorEnable = true;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;
	desc.AntialiasedLineEnable = true;

	if (FAILED(device->CreateRasterizerState(&desc, &aa_rasterizer_state)))
		throw std::runtime_error("Failed to create anti-aliased rasterizer state");
}

void adapter_dx11::create_depth_stencil_state()
{
	D3D11_DEPTH_STENCIL_DESC desc{};
	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	desc.StencilEnable = false;
	desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.BackFace = desc.FrontFace;

	if (FAILED(device->CreateDepthStencilState(&desc, &depth_stencil)))
		throw std::runtime_error("Failed to create depth stencil state");
}

void adapter_dx11::create_sampler_state()
{
	D3D11_SAMPLER_DESC desc{};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0.f;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.MinLOD = 0.f;
	desc.MaxLOD = 0.f;

	if (FAILED(device->CreateSamplerState(&desc, &texture_sampler)))
		throw std::runtime_error("Failed to create sampler state");
}

void adapter_dx11::destroy_objects()
{
	adapter_base::destroy_objects();

	if (!objects_created)
		return;

	if (vertex_shader)
		vertex_shader->Release();
	if (pixel_shader)
		pixel_shader->Release();
	if (input_layout)
		input_layout->Release();
	if (depth_stencil)
		depth_stencil->Release();
	if (rasterizer_state)
		rasterizer_state->Release();
	if (aa_rasterizer_state)
		aa_rasterizer_state->Release();
	if (blend_state)
		blend_state->Release();
	if (constant_buffer)
		constant_buffer->Release();
	if (texture_sampler)
		texture_sampler->Release();
	if (texture_object)
		texture_object->Release();

	objects_created = false;
}

void adapter_dx11::render()
{
	adapter_base::render();

	create_objects();
	if (!objects_created)
		return;

	D3D11_VIEWPORT vp{};
	vp.Width = display.x;
	vp.Height = display.y;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = vp.TopLeftY = 0.f;

	D3D11_MAPPED_SUBRESOURCE cb{};
	if (FAILED(ctx->Map(constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &cb)))
		throw std::runtime_error("Failed to map constant buffer");

	const float mvp[4][4] = {
			{2.f / display.x, 0.f, 0.f, 0.f},
			{0.f, 2.f / -display.y, 0.f, 0.f},
			{0.f, 0.f, 0.5f, 0.f},
			{-1.f, 1.f, 0.5f, 1.f},
	};

	memcpy(&reinterpret_cast<vertex_constant_buffer*>(cb.pData)->mvp, &mvp, sizeof(float) * 16);
	ctx->Unmap(constant_buffer, 0);

	ctx->IASetInputLayout(input_layout);
	ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ctx->VSSetShader(vertex_shader, nullptr, 0);
	ctx->VSSetConstantBuffers(0, 1, &constant_buffer);
	ctx->PSSetShader(pixel_shader, nullptr, 0);
	ctx->PSSetSamplers(0, 1, &texture_sampler);
	ctx->OMSetDepthStencilState(depth_stencil, 0);
	ctx->OMSetBlendState(blend_state, new float[4]{ 0.f, 0.f, 0.f, 0.f }, 0xFFFFFFFF);
	ctx->RSSetState(rasterizer_state);
	ctx->RSSetViewports(1, &vp);

	if (auto actual_vb_size = draw.get_total_vertices() * sizeof(vertex); !vertex_buffer || vb_size < actual_vb_size)
	{
		if (vertex_buffer)
			vertex_buffer->Release();
		if (vb_size < actual_vb_size)
			vb_size = actual_vb_size + 5000;

		D3D11_BUFFER_DESC desc{};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(vertex) * vb_size;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (FAILED(device->CreateBuffer(&desc, nullptr, &vertex_buffer)))
			throw std::runtime_error("Failed to create vertex buffer");

		const auto stride = (uint32_t)sizeof(vertex);
		const auto offset = 0u;

		ctx->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
	}

	if (auto actual_ib_size = draw.get_total_indices() * sizeof(uint16_t); !index_buffer || ib_size < actual_ib_size)
	{
		if (index_buffer)
			index_buffer->Release();
		if (ib_size < actual_ib_size)
			ib_size = actual_ib_size + 5000;

		D3D11_BUFFER_DESC desc{};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(uint16_t) * ib_size;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (FAILED(device->CreateBuffer(&desc, nullptr, &index_buffer)))
			throw std::runtime_error("Failed to create index buffer");

		ctx->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R16_UINT, 0);
	}

	D3D11_MAPPED_SUBRESOURCE v_data{};
	if (FAILED(ctx->Map(vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &v_data)))
		throw std::runtime_error("Failed to map vertex buffer");

	D3D11_MAPPED_SUBRESOURCE i_data{};
	if (FAILED(ctx->Map(index_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &i_data)))
		throw std::runtime_error("Failed to map index buffer");

	size_t v_offset{}, i_offset{};
	draw.for_each_layer([&](layer& l) {
		const auto v_size = l.vertices.size() * sizeof(vertex);
		const auto i_size = l.indices.size() * sizeof(uint16_t);

		memcpy((char*)v_data.pData + v_offset, l.vertices.data(), v_size);
		memcpy((char*)i_data.pData + i_offset, l.indices.data(), i_size);

		v_offset += v_size;
		i_offset += i_size;
	});

	ctx->Unmap(vertex_buffer, 0);
	ctx->Unmap(index_buffer, 0);

	v_offset = i_offset = 0;
	draw.for_each_layer([&](layer& l) {
		for (auto& c : l.commands)
		{
			const D3D11_RECT clip = {
					(LONG)c.clip.mins.x,
					(LONG)c.clip.mins.y,
					(LONG)c.clip.maxs.x,
					(LONG)c.clip.maxs.y
			};

			const auto texture = c.texture ? reinterpret_cast<ID3D11ShaderResourceView*>(c.texture) : texture_object;

			ctx->RSSetState(c.anti_alias ? aa_rasterizer_state : rasterizer_state);
			ctx->RSSetScissorRects(1, &clip);
			ctx->PSSetShaderResources(0, 1, &texture);
			ctx->DrawIndexed(c.indices, i_offset, v_offset);

			v_offset += c.vertices;
			i_offset += c.indices;
		}
	});
}

void* adapter_dx11::create_texture(void* arr, uint32_t w, uint32_t h, uint32_t p)
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = w;
	desc.Height = h;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = arr;
	sd.SysMemPitch = p;
	sd.SysMemSlicePitch = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC r{};
	r.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	r.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	r.Texture2D.MipLevels = 1;
	r.Texture2D.MostDetailedMip = 0;

	ID3D11Texture2D* texture{};
	if (FAILED(device->CreateTexture2D(&desc, &sd, &texture)))
		throw std::runtime_error("Coudln't create 2D texture");

	ID3D11ShaderResourceView* _tex{};
	if (FAILED(device->CreateShaderResourceView(texture, &r, &_tex)))
		throw std::runtime_error("Couldn't create texture");

	return _tex;
}

void adapter_dx11::destroy_texture(void* tex)
{
	reinterpret_cast<ID3D11ShaderResourceView*>(tex)->Release();
}

#endif