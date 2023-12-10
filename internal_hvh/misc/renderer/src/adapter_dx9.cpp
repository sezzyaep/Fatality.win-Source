#ifdef EVO_USE_DX9

#include <stdexcept>
#include <ren/adapter.h>
#include <ren/adapter_dx9.h>

using namespace evo::ren;

void adapter_dx9::create_objects()
{
	adapter_base::create_objects();

	if ( !texture_object )
	{
		auto pixel = 0xFFFFFFFFu;
		texture_object = reinterpret_cast< IDirect3DTexture9* >( create_texture( &pixel, 1, 1, 4 ) );
	}
}

void adapter_dx9::destroy_objects()
{
	adapter_base::destroy_objects();

	if ( texture_object )
	{
		texture_object->Release();
		texture_object = nullptr;
	}
}

void adapter_dx9::render()
{
	adapter_base::render();

	if ( !run )
		return;

	create_objects();
	backup.backup( device );

	IDirect3DStateBlock9* state{};
	if ( FAILED( device->CreateStateBlock( D3DSBT_PIXELSTATE, &state ) ) )
		return;

	state->Capture();

	D3DVIEWPORT9 vp{};
	vp.Width = display.x;
	vp.Height = display.y;
	vp.MaxZ = 1.f;

	device->SetViewport( &vp );
	device->SetPixelShader( nullptr );
	device->SetVertexShader( nullptr );
	device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	device->SetRenderState( D3DRS_LIGHTING, false );
	device->SetRenderState( D3DRS_ZENABLE, false );
	device->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	device->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	device->SetRenderState( D3DRS_SCISSORTESTENABLE, true );
	device->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	device->SetRenderState( D3DRS_FOGENABLE, false );
	device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	device->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	static D3DMATRIX mat_identity = { { {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		} } };
	D3DMATRIX mat_projection = { { {
			2.f / display.x, 0.0f, 0.0f, 0.0f,
			0.0f, 2.f / -display.y, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			-1.f, 1.f, 0.5f, 1.f
		} } };

	device->SetTransform( D3DTS_WORLD, &mat_identity );
	device->SetTransform( D3DTS_VIEW, &mat_identity );
	device->SetTransform( D3DTS_PROJECTION, &mat_projection );
	device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );

	if ( auto actual_vb_size = draw.get_total_vertices() * sizeof( vertex_dx9 ); !vertex_buffer || vb_size < actual_vb_size )
	{
		if ( vertex_buffer )
			vertex_buffer->Release();
		if ( vb_size < actual_vb_size )
			vb_size = actual_vb_size + 5000;

		if ( FAILED( device->CreateVertexBuffer(
			vb_size,
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
			D3DPOOL_DEFAULT,
			&vertex_buffer,
			nullptr
			) ) )
			return;
	}

	if ( auto actual_ib_size = draw.get_total_indices() * sizeof( uint16_t ); !index_buffer || ib_size < actual_ib_size )
	{
		if ( index_buffer )
			index_buffer->Release();
		if ( ib_size < actual_ib_size )
			ib_size = actual_ib_size + 5000;

		if ( FAILED( device->CreateIndexBuffer(
			ib_size,
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&index_buffer,
			nullptr
			) ) )
			return;
	}

	char* vertices{};
	char* indices{};

	if ( FAILED( vertex_buffer->Lock( 0, vb_size, ( void** ) &vertices, D3DLOCK_DISCARD ) ) )
		return;
	if ( FAILED( index_buffer->Lock( 0, ib_size, ( void** ) &indices, D3DLOCK_DISCARD ) ) )
		return;

	size_t i_offset{};
	draw.for_each_layer( [&] ( layer& l )
	{
		for ( auto& v : l.vertices )
		{
			*( vertex_dx9* ) vertices = vertex_dx9( v );
			vertices += sizeof( vertex_dx9 );
		}

		const auto i_size = l.indices.size() * sizeof( uint16_t );
		memcpy( indices + i_offset, l.indices.data(), i_size );
		i_offset += i_size;
	} );

	vertex_buffer->Unlock();
	index_buffer->Unlock();

	device->SetStreamSource( 0, vertex_buffer, 0, sizeof( vertex_dx9 ) );
	device->SetIndices( index_buffer );

	size_t v_offset{}; i_offset = 0;
	draw.for_each_layer( [&] ( layer& l )
	{
		for ( auto& c : l.commands )
		{
			const RECT clip = {
				( LONG ) c.clip.mins.x,
				( LONG ) c.clip.mins.y,
				( LONG ) c.clip.maxs.x,
				( LONG ) c.clip.maxs.y
			};

			device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, c.anti_alias );
			device->SetTexture( 0, c.texture && *c.texture ? c.texture->as<IDirect3DTexture9>() : texture_object );
			device->SetScissorRect( &clip );
			device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, v_offset, 0, c.vertices, i_offset, c.indices / 3 );

			v_offset += c.vertices;
			i_offset += c.indices;
		}
	} );

	state->Apply();
	state->Release();

	backup.restore( device );
}

void* adapter_dx9::create_texture( void* arr, uint32_t w, uint32_t h, uint32_t p )
{
	IDirect3DTexture9* _tex{};
	if ( FAILED( device->CreateTexture( w, h, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_tex, nullptr ) ) )
		return nullptr;

	D3DLOCKED_RECT r{};
	if ( FAILED( _tex->LockRect( 0, &r, nullptr, 0 ) ) )
		return nullptr;

	// swap colors to correct rendering
	for ( auto y = 0; y < h; y++ )
	{
		for ( auto x = 0; x < w; x++ )
		{
			auto c = reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( arr ) + ( x + y * w ) * 4 );
			c[ 0 ] ^= c[ 2 ];
			c[ 2 ] ^= c[ 0 ];
			c[ 0 ] ^= c[ 2 ];
		}
	}

	if ( r.Pitch == p )
		memcpy( r.pBits, arr, p * h );
	else
	{
		for ( auto y = 0; y < h; y++ )
			memcpy( ( char* ) r.pBits + r.Pitch * y, ( char* ) arr + p * y, p );
	}

	_tex->UnlockRect( 0 );
	return _tex;
}

void adapter_dx9::destroy_texture( void* tex )
{
	reinterpret_cast< IDirect3DTexture9* >( tex )->Release();
}

#endif