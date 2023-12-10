//
// Created by panzerfaust on 9/19/2020.
//

#include "../include_cheat.h"

#include <ren/texture.h>
#include <ren/adapter.h>

#include <deps/stb/stb_image.h>

using namespace evo::ren;

texture::texture( void* tex, size_t sz ) : adp( adapter ), way( c_mem )
{
	creation_data.raw = malloc( sz );
	creation_data.a = sz;

	memcpy( creation_data.raw, tex, sz );

	create();
}

texture::texture( const std::string& f ) : adp( adapter ), way( c_file )
{
	creation_data.str = f;

	create();
}

texture::texture( void* tex, uint32_t w, uint32_t h, uint32_t p ) : adp( adapter ), way( c_rgba )
{
	creation_data.raw = malloc( h * p );
	creation_data.a = w;
	creation_data.b = h;
	creation_data.c = p;

	memcpy( creation_data.raw, tex, h * p );

	create();
}

void texture::destroy()
{
	if ( is_valid() && way != c_none )
		adp->destroy_texture( object );
	object = nullptr;
}

void texture::create()
{
	if ( is_valid() || way == c_none )
		return;

	uint8_t* data{}; int w{}, h{}, channels{}; uint32_t stride{};
	if ( way == c_mem )
		data = stbi_load_from_memory( reinterpret_cast< uint8_t* >( creation_data.raw ), creation_data.a, &w, &h, &channels, 4 );
	if ( way == c_file )
		data = stbi_load( creation_data.str.c_str(), &w, &h, &channels, 4 );
	if ( way == c_rgba )
	{
		data = reinterpret_cast< uint8_t* >( creation_data.raw );
		w = creation_data.a;
		h = creation_data.b;
		stride = creation_data.c;
		channels = 4;
	}
	else
		stride = w * 4;

	// we don't want null or non-rgba images
	if ( !data || !channels )
	{
		if ( way != c_rgba && data )
			stbi_image_free( data );

		return;
	}

	object = adp->create_texture( data, w, h, stride );
	if ( way != c_rgba )
	{
		stbi_image_free( data );

		creation_data.b = w;
		creation_data.c = h;
	}
}
