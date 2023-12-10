#include "../include_cheat.h"
#include "ren/adapter.h"

bool grenade_pred::data_t::draw() const
{
	if ( m_path.size() <= 1u || interfaces::globals()->curtime >= m_expire_time )
		return false;

	const auto distance = local_player->get_abs_origin().Dist( m_origin );
	if ( distance > 2000.f )
		return false;

	const auto box_col = Color( vars::misc.box_color->get<D3DCOLOR>() );
	const auto tracer_col = Color( vars::misc.tracer_color->get<D3DCOLOR>() );

	const float percent = std::min( 1.f - static_cast< float >( m_path.size() * 2 - 4 ) / m_tick, 1.f );
	auto damage = 0.f;

	if ( m_index == WEAPON_HEGRENADE )
		damage = 350.f - distance;

	if ( m_index == WEAPON_MOLOTOV || m_index == WEAPON_INCGRENADE )
		damage = 140.f - distance;

	auto back = std::get< Vector >( m_path.back() );
	if ( this == &m_data )
		switch ( m_index )
		{
			case WEAPON_MOLOTOV:
			case WEAPON_INCGRENADE:
				if ( !std::get< bool >( m_path.back() ) )
					break;
			case WEAPON_SMOKEGRENADE:
			case WEAPON_HEGRENADE:
				//IVDebugOverlay::Circle( back, QAngle( -90.f, 0.f, 0.f ), 140.f, color, true );
				//IVDebugOverlay::Circle( back, QAngle( 90.f, 0.f, 0.f ), 140.f, color, true );
				break;
			default:
				break;
		}

	auto prev = std::get< Vector >( m_path[ 0 ] );
	if ( this == &m_data || true )
		for ( auto i = 1u; i < m_path.size(); ++i )
		{
			auto current = std::get< Vector >( m_path[ i ] );
			Vector vec3_temp_orientation = ( current - prev );
			QAngle ang_gr_traj_angles{};
			math::vector_angles( vec3_temp_orientation, ang_gr_traj_angles );


			IVDebugOverlay::RenderBox( prev, Vector( 0.f, -0.5f, -0.5f ), Vector( vec3_temp_orientation.Length(), 0.5f, 0.5f ), ang_gr_traj_angles, tracer_col, false );
			prev = current;
		}

	for ( auto i = 1u; i < m_path.size(); ++i )
	{
		if ( std::get <bool>( m_path[ i ] ) )
			IVDebugOverlay::RenderBox( std::get< Vector >( m_path[ i ] ), Vector( -0.5f ), Vector( 0.5f ), {}, box_col, false );
	}

	if ( this == &m_data )
		return true;

	const auto back_distance = local_player->get_abs_origin().Dist( back );
	if ( back_distance > 1600.f )
		return false;

	m_current_esp_data.emplace_back( back, m_index, percent, damage, back_distance );

	return true;
}

void grenade_pred::simulate()
{
	m_outstanding_grenades.erase( std::ranges::remove_if( m_outstanding_grenades, [] ( const std::pair<int, float>& current ) { return std::get<float>( current ) + 15.f < interfaces::globals()->curtime || std::get<float>( current ) - 0.5f > interfaces::globals()->curtime; } ).begin(), m_outstanding_grenades.end() );
	for ( auto it = m_grenade_spawntimes.begin(); it != m_grenade_spawntimes.end(); )
	{
		if ( std::get<float>( *it ) + 15.f < interfaces::globals()->curtime || std::get<float>( *it ) - 0.5f > interfaces::globals()->curtime )
			it = m_grenade_spawntimes.erase( it );
		else
			++it;
	}

	m_data = {};

	if ( !local_player || !local_player->get_alive() )
		return;

	if ( !vars::misc.grenade_tracer->get<bool>() )
		return;

	const auto weapon = local_weapon;
	if ( !weapon )
		return;

	if ( !weapon->is_grenade() || weapon->get_throw_time() || !weapon->get_pin_pulled() )
		return;

	QAngle vangles{};
	interfaces::engine()->GetViewAngles( vangles );
	vangles.x -= ( 90.f - fabsf( vangles.x ) ) * 10.f / 90.f;

	Vector direction{};
	math::angle_vectors( vangles, &direction );

	const auto throw_strength = clamp< float >( weapon->get_throw_strength(), 0.f, 1.f );
	const auto eye_pos = local_player->get_eye_pos() + local_player->get_velocity() * 0.1f;
	const auto src = Vector( eye_pos.x, eye_pos.y, eye_pos.z + ( throw_strength * 12.f - 12.f ) );

	trace_t trace{};
	globals::util_trace_hull( src, src + direction * 22.f, { 2.f, 2.f, 2.f }, MASK_SOLID | CONTENTS_CURRENT_90, local_player, &trace );

	const auto local_velocity = local_player->get_velocity();
	m_data.m_owner = local_player;
	m_data.m_index = weapon->get_weapon_id();
	m_data.predict( trace.endpos - direction * 6.f,
		direction * ( clamp< float >( weapon->get_wpn_data()->flthrowvelocity * 0.9f, 15.f, 750.f ) * ( throw_strength * 0.7f + 0.3f ) ) + local_velocity * 1.25f,
		ticks_to_time( interfaces::client_state()->get_last_server_tick() ), interfaces::client_state()->get_last_server_tick() - local_player->get_tickbase() );
}

void grenade_pred::paint()
{
	m_current_esp_data.clear();

	m_data.draw();

	if ( !vars::misc.grenade_helper->get<bool>() )
	{
		m_list.clear();
		return;
	}

	static auto last_server_tick = interfaces::client_state()->get_last_server_tick();
	if ( last_server_tick != interfaces::client_state()->get_last_server_tick() )
	{
		m_list.clear();

		last_server_tick = interfaces::client_state()->get_last_server_tick();
	}

	for ( const auto entity : *interfaces::entity_list() )
	{
		const auto client_class = entity->GetClientClass();
		if ( !client_class
			|| client_class->m_ClassID != ClassId::CMolotovProjectile && client_class->m_ClassID != ClassId::CBaseCSGrenadeProjectile )
			continue;

		if ( entity->IsDormant() )
			continue;

		if ( client_class->m_ClassID == ClassId::CBaseCSGrenadeProjectile )
		{
			const auto model = entity->GetModel();
			if ( !model )
				continue;

			_( fraggrenade, "fraggrenade" );

			const auto studio_model = interfaces::model_info()->GetStudioModel( model );
			if ( !studio_model || std::string_view( studio_model->name ).find( fraggrenade ) == std::string::npos )
				continue;
		}

		auto handle = entity->GetRefEHandle();

		if ( reinterpret_cast< C_BaseCSGrenadeProjectile* >( entity )->get_explode_tick_begin() )
		{
			m_list.erase( handle );

			continue;
		}

		const auto thrower = reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity_from_handle( reinterpret_cast< C_BaseCombatWeapon* >( entity )->get_thrower() ) );
		if ( thrower )
		{
			if ( m_grenade_spawntimes.find( handle ) == m_grenade_spawntimes.end() )
			{
				auto res = m_outstanding_grenades.end();
				for ( auto it = m_outstanding_grenades.begin(); it != m_outstanding_grenades.end(); ++it )
				{
					if ( std::get<int>( *it ) != thrower->EntIndex() )
						continue;

					if ( res == m_outstanding_grenades.end() || std::get<float>( *it ) <= reinterpret_cast< C_BaseCSGrenadeProjectile* >( entity )->get_spawn_time() && std::get<float>( *it ) > std::get<float>( *res ) )
						res = it;
				}
				if ( res != m_outstanding_grenades.end() )
				{
					m_grenade_spawntimes.emplace( handle, entity->get_simtime() < std::get<float>( *res ) ? entity->get_simtime() : std::get<float>( *res ) );
					m_outstanding_grenades.erase( res );
				}
			}
		}

		const auto matched_spawntime = m_grenade_spawntimes.find( handle );
		if ( m_list.find( handle ) == m_list.end() && matched_spawntime != m_grenade_spawntimes.end() )
		{
			m_list.emplace( handle,
				data_t( thrower,
				client_class->m_ClassID == ClassId::CMolotovProjectile ? WEAPON_MOLOTOV : WEAPON_HEGRENADE,
				entity->get_origin(), reinterpret_cast< C_BasePlayer* >( entity )->get_velocity(),
				std::get<float>( *matched_spawntime ), interfaces::client_state()->get_last_server_tick() - time_to_ticks( std::get<float>( *matched_spawntime ) ) )
			);
		}

		if ( m_list.find( handle ) == m_list.end() )
			continue;

		if ( m_list.at( handle ).draw() )
			continue;

		m_list.erase( handle );
	}
}

void grenade_pred::draw_esp()
{
	const auto tracer_col = color( vars::misc.tracer_color->get<D3DCOLOR>() );

	auto& layer = dx_adapter::buf;

	for ( const auto& data : m_current_esp_data )
	{
		Vector screenpos{};
		auto oof = !math::world_to_screen( data.pos, screenpos );
		const auto dimensions = adapter->display;
		if ( screenpos.x - 23.f < 0 || screenpos.x - 23.f > 0 + dimensions.x || screenpos.y - 23.f < 0 || screenpos.y - 23.f > 0 + dimensions.y )
			oof = true;

		if ( oof )
		{
			constexpr auto get_point_on_ellipse = [] ( const vec2& radius, const vec2& center, const float degree ) -> vec2
			{
				constexpr auto pi_over_180 = 0.017453292519943295769236907684886;
				return { radius.x * cosf( degree * static_cast< float >( pi_over_180 ) ) + center.x,
					radius.y * sinf( degree * static_cast< float >( pi_over_180 ) ) + center.y };
			};

			constexpr auto rotate_point = [] ( const vec2& point, const vec2& center, float degree ) -> vec2
			{
				degree = DEG2RAD( degree );

				const auto cos = cosf( degree );
				const auto sin = sinf( degree );

				static auto ret = vec2();
				ret.x = cos * ( point.x - center.x ) - sin * ( point.y - center.y );
				ret.y = sin * ( point.x - center.x ) + cos * ( point.y - center.y );

				ret += center;

				return ret;
			};

			QAngle view_angles{};
			interfaces::engine()->GetViewAngles( view_angles );
			const auto angle = math::calc_angle( local_player->get_abs_origin(), data.pos );
			const auto delta = math::normalize_float( view_angles.y - angle.y );
			const auto screen_angle = delta - 90.f;
			const auto screen_size = Vector2D( dimensions.x, dimensions.y );
			const auto screen_center = screen_size / 2.f;

			auto offset = ( screen_center.x ) * 0.7f;
			auto screen_pos = get_point_on_ellipse( { offset, offset * ( screen_size.y / screen_size.x ) }, { screen_center.x, screen_center.y }, screen_angle );

			static auto triangle = std::array<vec2, 3>{ };
			triangle.at( 1 ) = { screen_pos.x, screen_pos.y - 23.f };
			triangle.at( 0 ) = { screen_pos.x - 23.f * .5f, screen_pos.y };
			triangle.at( 2 ) = { screen_pos.x + 23.f * .5f, screen_pos.y };

			static vec2 one, two, three;
			one = rotate_point( triangle.at( 0 ), screen_pos, screen_angle + 90 );
			two = rotate_point( triangle.at( 1 ), screen_pos, screen_angle + 90 );
			three = rotate_point( triangle.at( 2 ), screen_pos, screen_angle + 90 );

			layer.add_triangle_filled( one, two, three, data.damage > 0.f ? color( Color::RedAccent( 100 ).direct() ) : color( Color::DarkGrey( 100 ).direct() ) );
			layer.add_triangle( one, two, three, color( tracer_col, 100.f ) );

			screen_pos = rotate_point( { screen_pos.x, screen_pos.y + 35.f }, screen_pos, screen_angle + 90 );
			screenpos.x = screen_pos.x;
			screenpos.y = screen_pos.y;
		}

		layer.push_aa( true );
		layer.add_circle_filled( { floorf( screenpos.x ), floorf( screenpos.y ) }, 23.f, data.damage > 0.f ? color( Color::RedAccent( 100 ).direct() ) : color( Color::DarkGrey( 100 ).direct() ) );
		layer.add_circle( { floorf( screenpos.x ), floorf( screenpos.y ) }, 23.f, color( tracer_col, 200.f ), 2.f, 64, data.explode_perc );
		//render::circle( { floorf( screenpos.x ), floorf( screenpos.y ) }, 23.f, Color( tracer_col, 100 ) );
		layer.pop_aa();
		dx_adapter::weapon(layer, { floorf( screenpos.x ), floorf( screenpos.y ) - 5 }, 20, color::white(), data.icon_string.c_str(), layer::align_center, layer::align_center );
		layer.add_text( draw.get_font( FNV1A( "esp" ) ), { floorf( screenpos.x ), floorf( screenpos.y ) + 13 }, util::format( "%.0f m", data.distance * 0.01905f ), color::white(), layer::align_center, layer::align_center );
	}
}

void grenade_pred::register_grenade( IGameEvent* game_event )
{
	_( uid, "userid" );
	const auto userid = interfaces::engine()->GetPlayerForUserID( game_event->GetInt( uid.c_str() ) );
	m_outstanding_grenades.emplace_back( userid, interfaces::globals()->curtime );
}
