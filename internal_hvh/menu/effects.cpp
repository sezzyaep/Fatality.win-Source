#include "menu_include.h"

using namespace evo::ren;

void particle_drawer::spawn_particles()
{
	for ( auto i = 0; i < max_particles; i++ )
	{
		particles.emplace_back( Vector{ globals::random_float( 0.f, screen_width ),
	   globals::random_float( 0.f, screen_height ),
	   globals::random_float( 100.f, 800.f ) },
		   Vector{ globals::random_float( -0.2f, 0.2f ), globals::random_float( 2.f, 2.1f ), 0.f }, false );
	}
}

void particle_drawer::handle_respawn( particle& particle ) const
{
	if ( particle.pos.x - 20 > screen_width || particle.pos.x + 20 < 0 )
		particle.velocity.x = -particle.velocity.x;

	if ( particle.pos.y - 20 > screen_height )
	{
		particle.pos.y = -20;
		particle.pos.x = globals::random_float( 0.f, screen_width );
		particle.golden = globals::random_int( 0, 1000 ) == 133;
	}

}

void particle_drawer::update( particle& particle ) const
{
	handle_respawn( particle );

	particle.pos += particle.velocity * ( 1 / ( particle.pos.z / 100.f ) ) * interfaces::globals()->frametime * 50.f;
}

void particle_drawer::parallax( particle& particle, const vec2& delta )
{
	particle.pos.x += ( 1 / ( particle.pos.z / 100.f ) ) * delta.x * interfaces::globals()->frametime * 20.f;
	particle.pos.y += ( 1 / ( particle.pos.z / 100.f ) ) * delta.y * interfaces::globals()->frametime * 20.f;
}

void particle_drawer::draw_particle( particle& particle, const int alpha, const bool foreground ) const
{
	if ( ( !particle.golden || particle.pos.z > foreground_plane ) && foreground )
		return;

	if ( particle.golden && particle.pos.z <= foreground_plane && !foreground )
		return;

	//render::circle_filled_radial_gradient( { particle.pos.x, particle.pos.y }, 10 * scaling, Color( 255, 255, 255, alpha / 4 ), Color( 200, 200, 200, alpha / 2 ) );
	//render::circle( { particle.pos.x, particle.pos.y }, 10 * scaling, Color( 255, 255, 255, alpha / 2 ) );

	auto& d = ::draw.get_layer( layer::foreground );

	const auto scaling = ( 1 / ( particle.pos.z / 40.f ) ) * 1.4f;

	d.push_aa( true );
	d.add_rect_filled( rect_legacy( { particle.pos.x, particle.pos.y + 5 * scaling }, { 6 * scaling, 7 * scaling } ), color( 0, 0, 0, alpha ) );
	d.add_rect_filled( rect_legacy( { particle.pos.x, particle.pos.y + 16 * scaling }, { 6 * scaling, 13 * scaling } ), color( 0, 0, 0, alpha ) );
	d.add_rect_filled( rect_legacy( { particle.pos.x, particle.pos.y }, { 19 * scaling, 6 * scaling } ), color( 0, 0, 0, alpha ) );
	d.add_rect_filled( rect_legacy( { particle.pos.x, particle.pos.y + 11 * scaling }, { 16 * scaling, 6 * scaling } ), color( 0, 0, 0, alpha ) );
	d.add_rect_filled_linear_gradient( rect_legacy( { particle.pos.x, particle.pos.y + 5 * scaling }, { 5 * scaling, 6 * scaling } ), particle.golden ? color( 211, 164, 33, alpha ) : color( 250, 4, 79, alpha ), particle.golden ? color( 225, 188, 38, alpha ) : color( 228, 13, 103, alpha ), true );
	d.add_rect_filled_linear_gradient( rect_legacy( { particle.pos.x, particle.pos.y + 16 * scaling }, { 5 * scaling, 12 * scaling } ), particle.golden ? color( 211, 164, 33, alpha ) : color( 250, 4, 79, alpha ), particle.golden ? color( 225, 188, 38, alpha ) : color( 228, 13, 103, alpha ), true );
	d.add_rect_filled_linear_gradient( rect_legacy( { particle.pos.x, particle.pos.y }, { 18 * scaling, 5 * scaling } ), particle.golden ? color( 211, 164, 33, alpha ) : color( 250, 4, 79, alpha ), particle.golden ? color( 249, 227, 46, alpha ) : color( 182, 42, 148, alpha ), true );
	d.add_rect_filled_linear_gradient( rect_legacy( { particle.pos.x, particle.pos.y + 11 * scaling }, { 15 * scaling, 5 * scaling } ), particle.golden ? color( 211, 164, 33, alpha ) : color( 250, 4, 79, alpha ), particle.golden ? color( 249, 227, 46, alpha ) : color( 196, 26, 139, alpha ), true );
	d.pop_aa();
}

void particle_drawer::draw( const int alpha )
{
	const auto delta = focus_point - g_input->get_mouse().pos;
	focus_point = g_input->get_mouse().pos;

	for ( auto& particle : particles )
	{
		parallax( particle, delta );
		update( particle );
		draw_particle( particle, alpha, false );
	}
}

void particle_drawer::draw_foreground( const int alpha )
{
	for ( auto& particle : particles )
		draw_particle( particle, alpha, true );
}