#include "../include_cheat.h"
#ifdef NO_NETOFFSETS
#include "../misc/hitsounds.h"
#endif
#include "ren/adapter.h"

using namespace evo::ren;

void hitmarker::listener( IGameEvent* game_event )
{
	if ( !vars::misc.hitsound_select->get<int>() )
		return;

	_( attacker_s, "attacker" );
	_( userid_s, "userid" );

	const auto attacker = interfaces::engine()->GetPlayerForUserID( game_event->GetInt( attacker_s.c_str() ) );
	const auto victim = interfaces::engine()->GetPlayerForUserID( game_event->GetInt( userid_s.c_str() ) );

	if ( attacker != interfaces::engine()->GetLocalPlayer() )
		return;

	if ( victim == interfaces::engine()->GetLocalPlayer() )
		return;

	const auto player = globals::get_player( victim );
	if ( !player || !player->is_enemy() )
		return;

	_u( file, soundfiles[ vars::misc.hitsound_select->get< int >() - 1 ] );
	interfaces::surface()->PlaySound( file.c_str() );
}

void hitmarker::draw_hits()
{
	for ( auto i = 0; i < hits.size(); i++ )
	{
		auto& hit = hits[ i ];

		if ( hit.time + 1.5f < interfaces::globals()->realtime || hit.time > interfaces::globals()->realtime + 1.5f )
		{
			hits.erase( hits.begin() + i );
			i--;
		}

		if ( vars::misc.hitmarker->get<int>() == 2 )
		{
			render_screen_hitmarker( hit );
		}
		else if ( vars::misc.hitmarker->get<int>() == 1 )
		{
			Vector screen_pos{};
			if ( math::world_to_screen( hit.pos, screen_pos ) )
			{
				render_hitmarker( hit, screen_pos );
			}
		}
	}
}

void hitmarker::add_hit( const hitmarker_t hit )
{
	if ( !vars::misc.hitmarker->get<int>() )
		return;

	hits.push_back( hit );
}

#ifdef NO_NETOFFSETS
void __forceinline extern hitmarker::save_files()
{
	_( bell, "csgo//sound//bell.wav" );
	util::create_file_from_memory( bell, ::bell );
	::bell.clear();
	auto vec = std::vector( ::bell );
	::bell.swap( vec );

	_( cod, "csgo//sound//cod.wav" );
	util::create_file_from_memory( cod, ::cod );
	::cod.clear();
	vec = std::vector( ::cod );
	::cod.swap( vec );

	_( fatality, "csgo//sound//fatality.wav" );
	util::create_file_from_memory( fatality, ::fatality );
	::fatality.clear();
	vec = std::vector( ::fatality );
	::fatality.swap( vec );

	_( phonk, "csgo//sound//phonk.wav" );
	util::create_file_from_memory( phonk, ::phonk );
	::phonk.clear();
	vec = std::vector( ::phonk );
	::phonk.swap( vec );

	_( bepis, "csgo//sound//bepis.wav" );
	util::create_file_from_memory( bepis, ::bepis );
	::bepis.clear();
	vec = std::vector( ::bepis );
	::bepis.swap( vec );

	_( sparkle, "csgo//sound//sparkle.wav" );
	util::create_file_from_memory( sparkle, ::sparkle );
	::sparkle.clear();
	vec = std::vector( ::sparkle );
	::sparkle.swap( vec );

	_( cock, "csgo//sound//cock.wav" );
	util::create_file_from_memory( cock, ::cock );
	::cock.clear();
	vec = std::vector( ::cock );
	::cock.swap( vec );
}
#endif

void hitmarker::render_hitmarker( hitmarker_t& hit, const Vector& screen_pos )
{
	constexpr auto line_size = 6;

	const auto step = 255.f / 1.0f * interfaces::globals()->frametime;
	const auto step_move = 30.f / 1.5f * interfaces::globals()->frametime;
	constexpr auto multiplicator = 0.3f;

	hit.moved -= step_move;

	if ( hit.time + 0.5f <= interfaces::globals()->realtime )
		hit.alpha -= step;

	const auto int_alpha = static_cast< int >( hit.alpha );

	auto& layer = dx_adapter::buf;

	if ( int_alpha > 0 )
	{
		auto col = color( 255, 255, 255, int_alpha );

		layer.add_line( vec2( screen_pos.x - line_size * multiplicator, screen_pos.y - line_size * multiplicator ), vec2( screen_pos.x - line_size, screen_pos.y - line_size ), col );
		layer.add_line( vec2( screen_pos.x - line_size * multiplicator, screen_pos.y + line_size * multiplicator ), vec2( screen_pos.x - line_size, screen_pos.y + line_size ), col );
		layer.add_line( vec2( screen_pos.x + line_size * multiplicator, screen_pos.y + line_size * multiplicator ), vec2( screen_pos.x + line_size, screen_pos.y + line_size ), col );
		layer.add_line( vec2( screen_pos.x + line_size * multiplicator, screen_pos.y - line_size * multiplicator ), vec2( screen_pos.x + line_size, screen_pos.y - line_size ), col );

		col = color( 255, 0, 0, int_alpha );
		_( s, "- " );
		layer.add_text( draw.get_font( FNV1A( "esp" ) ), vec2( screen_pos.x + 8, screen_pos.y - 12 + hit.moved ), s + std::to_string( hit.damage ), col );
	}
}

float BezierBlend5( float t )
{
	return t * t * ( 3.0f - 2.0f * t );
}

void hitmarker::render_screen_hitmarker( hitmarker_t& hit )
{
	const auto step = 255.f / 0.75f * interfaces::globals()->frametime;
	const auto step_move = 30.f / 1.5f * interfaces::globals()->frametime;

	hit.moved -= step_move;

	if ( hit.time <= interfaces::globals()->realtime )
		hit.alpha -= step;

	const auto int_alpha = static_cast< int >( hit.alpha );

	constexpr int line_size = 3;

	auto& layer = dx_adapter::buf;

	if ( int_alpha > 0 )
	{
		const auto col = color( 255.f, 255.f, 255.f, 50.f * BezierBlend5( hit.alpha / 255.f ), true );
		const auto col2 = color( 255.f, 255.f, 255.f, 255.f * BezierBlend5( hit.alpha / 255.f ), true );

		const auto screen_pos = ( adapter->display / 2.f ).round() + 1.f;

		layer.add_line_multicolor( vec2( screen_pos.x - line_size - 7, screen_pos.y - line_size - 7 ), vec2( screen_pos.x - line_size, screen_pos.y - line_size ), { col2, col } );
		layer.add_line_multicolor( vec2( screen_pos.x - line_size - 7, screen_pos.y + line_size + 7 ), vec2( screen_pos.x - line_size, screen_pos.y + line_size ), { col2, col } );
		layer.add_line_multicolor( vec2( screen_pos.x + line_size + 7, screen_pos.y + line_size + 7 ), vec2( screen_pos.x + line_size, screen_pos.y + line_size ), { col2, col } );
		layer.add_line_multicolor( vec2( screen_pos.x + line_size + 7, screen_pos.y - line_size - 7 ), vec2( screen_pos.x + line_size, screen_pos.y - line_size ), { col2, col } );
	}
}
