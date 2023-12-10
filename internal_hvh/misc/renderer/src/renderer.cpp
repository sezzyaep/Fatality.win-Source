//
// Created by panzerfaust on 9/16/2020.
//

#include <ren/renderer.h>
#include <ren/command.h>

using namespace evo::ren;

size_t renderer::get_total_vertices()
{
	size_t count{};
	for ( auto& l : layers )
	{
		if ( l.second.get_draw_mode() == layer::dm_custom )
			continue;

		count += l.second.vertices.size();
	}

	return count;
}

size_t renderer::get_total_indices()
{
	size_t count{};
	for ( auto& l : layers )
	{
		if ( l.second.get_draw_mode() == layer::dm_custom )
			continue;

		count += l.second.indices.size();
	}

	return count;
}

void renderer::for_each_layer( const std::function<void( layer& )>& fn )
{
	std::lock_guard _lock( mtx );
	for ( auto& layer : layers )
	{
		if ( layer.second.get_draw_mode() == layer::dm_custom )
			continue;

		fn( layer.second );
	}
}

void renderer::for_each_font( const std::function<void( std::shared_ptr<font_base>& )>& fn )
{
	for ( auto& f : fonts )
		fn( f.second );
}

void renderer::for_each_texture( const std::function<void( std::shared_ptr<texture>& )>& fn )
{
	for ( auto& t : textures )
		fn( t.second );
}

void renderer::for_each_anim( const std::function<void( std::shared_ptr<anim_base>& )>& fn )
{
	for ( auto& a : anims )
		fn( a.second );
}
