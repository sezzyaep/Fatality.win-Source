#include <ren/command.h>
#include <ren/adapter.h>

using namespace evo::ren;

command::command( layer* l, bool set_texture ) : buffer( l )
{
	alpha_mod = buffer->get_alpha();

	// we don't need to
	if ( alpha_mod.value_or( 1.f ) == 0.f )
	{
		buffer = nullptr;
		return;
	}

	// pre-set everything
	no_dpi = buffer->get_no_dpi().value_or( false );
	clip = buffer->get_clip().value_or( rect().size( adapter->display ) );
	anti_alias = buffer->get_aa().value_or( false );

	if ( set_texture )
		texture = buffer->get_texture().value_or( nullptr );
}

void command::add_vertex( const vertex& vtx )
{
	if ( !buffer )
		return;

	vertices++;
	buffer->vertices.emplace_back( vertex(
		vtx.get_pos() * ( no_dpi ? 1.f : adapter->dpi ),
		vtx.get_color().mod_a( alpha_mod.value_or( vtx.get_color().value.w ) ),
		vtx.get_uv() )
	);
}

void command::add_index( const uint16_t idx )
{
	if ( !buffer )
		return;

	indices++;
	buffer->indices.emplace_back( idx );
}
