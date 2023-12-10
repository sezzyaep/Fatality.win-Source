#pragma once

namespace grenade_pred
{
	void paint();
	void simulate();
	void draw_esp();
	void register_grenade( IGameEvent* game_event );

	struct data_t
	{
		__forceinline data_t() = default;
		__forceinline data_t( C_BasePlayer* owner, int index, const Vector& origin, const Vector& velocity, float throw_time, int offset ) : data_t()
		{
			m_owner = owner;
			m_index = index;

			predict( origin, velocity, throw_time, offset );
		}

		__forceinline bool physics_simulate()
		{
			if ( m_detonated )
				return true;


			var( sv_gravity );
			const auto new_velocity_z = m_velocity.z - ( sv_gravity->get_float() * 0.4f ) * interfaces::globals()->interval_per_tick;

			const auto move = Vector(
				m_velocity.x * interfaces::globals()->interval_per_tick,
				m_velocity.y * interfaces::globals()->interval_per_tick,
				( m_velocity.z + new_velocity_z ) / 2.f * interfaces::globals()->interval_per_tick
			);

			m_velocity.z = new_velocity_z;

			auto trace = trace_t();

			physics_push_entity( move, trace );

			if ( m_detonated )
				return true;

			if ( trace.fraction != 1.f )
			{
				update_path< true >();

				perform_fly_collision_resolution( trace );
			}

			return false;
		}

		__forceinline void physics_trace_entity( const Vector& src, const Vector& dst, std::uint32_t mask, trace_t& trace )
		{
			globals::util_trace_hull( src, dst, { 2.f, 2.f, 2.f }, mask, m_owner, &trace, m_collision_group );

			if ( trace.startsolid && ( trace.contents & CONTENTS_CURRENT_90 ) )
				globals::util_trace_hull( src, dst, { 2.f, 2.f, 2.f }, mask & ~CONTENTS_CURRENT_90, m_owner, &trace, m_collision_group );

			if ( !trace.DidHit()
				|| !trace.m_pEnt
				|| !trace.m_pEnt->is_player() )
				return;

			globals::util_trace_line( src, dst, mask, m_owner, &trace, m_collision_group );
		}

		__forceinline void physics_push_entity( const Vector& push, trace_t& trace )
		{
			physics_trace_entity( m_origin, m_origin + push, m_collision_group == COLLISION_GROUP_DEBRIS ? ( MASK_SOLID | CONTENTS_CURRENT_90 ) & ~CONTENTS_MONSTER : MASK_SOLID | CONTENTS_OPAQUE | CONTENTS_IGNORE_NODRAW_OPAQUE | CONTENTS_CURRENT_90 | CONTENTS_HITBOX, trace );

			if ( trace.startsolid )
			{
				m_collision_group = COLLISION_GROUP_INTERACTIVE_DEBRIS;
				globals::util_trace_line( m_origin - push, m_origin + push, ( MASK_SOLID | CONTENTS_CURRENT_90 ) & ~CONTENTS_MONSTER, m_owner, &trace, m_collision_group );
			}

			if ( trace.fraction )
			{
				m_origin = trace.endpos;
			}

			if ( !trace.m_pEnt )
				return;

			if ( trace.m_pEnt->is_player() || m_index != WEAPON_TAGRENADE && m_index != WEAPON_MOLOTOV && m_index != WEAPON_INCGRENADE )
				return;

			var( weapon_molotov_maxdetonateslope );

			if ( m_index != WEAPON_TAGRENADE
				&& trace.plane.normal.z < std::cos( DEG2RAD( weapon_molotov_maxdetonateslope->get_float() ) ) )
				return;

			detonate< true >();
		}

		__forceinline void perform_fly_collision_resolution( trace_t& trace )
		{
			auto surface_elasticity = 1.f;

			if ( trace.m_pEnt )
			{
				if ( trace.m_pEnt->is_breakable_entity() )
				{
					m_velocity *= 0.4f;
					return;
				}

				const auto is_player = trace.m_pEnt->is_player();
				if ( is_player )
				{
					surface_elasticity = 0.3f;
				}

				if ( trace.m_pEnt->EntIndex() )
				{
					if ( is_player
						&& m_last_hit_entity == trace.m_pEnt )
					{
						m_collision_group = COLLISION_GROUP_DEBRIS;

						return;
					}

					m_last_hit_entity = trace.m_pEnt;
				}
			}

			auto velocity = Vector();

			const auto back_off = m_velocity.Dot( trace.plane.normal ) * 2.f;

			for ( auto i = 0u; i < 3u; i++ )
			{
				const auto change = trace.plane.normal[ i ] * back_off;

				velocity[ i ] = m_velocity[ i ] - change;

				if ( std::fabs( velocity[ i ] ) >= 1.f )
					continue;

				velocity[ i ] = 0.f;
			}

			velocity *= clamp< float >( surface_elasticity * 0.45f, 0.f, 0.9f );

			if ( trace.plane.normal.z > 0.7f )
			{
				const auto speed_sqr = velocity.LengthSqr();
				if ( speed_sqr > 96000.f )
				{
					const auto l = velocity.Normalized().Dot( trace.plane.normal );
					if ( l > 0.5f )
					{
						velocity *= 1.f - l + 0.5f;
					}
				}

				if ( speed_sqr < 400.f )
				{
					m_velocity = Vector( 0, 0, 0 );
				}
				else
				{
					m_velocity = velocity;

					physics_push_entity( velocity * ( ( 1.f - trace.fraction ) * interfaces::globals()->interval_per_tick ), trace );
				}
			}
			else
			{
				m_velocity = velocity;

				physics_push_entity( velocity * ( ( 1.f - trace.fraction ) * interfaces::globals()->interval_per_tick ), trace );
			}

			if ( m_bounces_count > 20 )
				return detonate< false >();

			++m_bounces_count;
		}

		__forceinline void think()
		{
			switch ( m_index )
			{
				case WEAPON_SMOKEGRENADE:
					if ( m_velocity.LengthSqr() <= 0.01f )
					{
						detonate< false >();
					}

					break;
				case WEAPON_DECOY:
					if ( m_velocity.LengthSqr() <= 0.04f )
					{
						detonate< false >();
					}

					break;
				case WEAPON_FLASHBANG:
				case WEAPON_HEGRENADE:
				case WEAPON_MOLOTOV:
				case WEAPON_INCGRENADE:
					if ( ticks_to_time( m_tick ) > m_detonate_time )
					{
						detonate< false >();
					}

					break;
			}

			m_next_think_tick = m_tick + time_to_ticks( 0.2f );
		}

		template < bool _bounced >
		__forceinline void detonate()
		{
			m_detonated = true;

			update_path< _bounced >();
		}

		template < bool _bounced >
		__forceinline void update_path()
		{
			m_last_update_tick = m_tick;

			m_path.emplace_back( m_origin, _bounced );
		}

		__forceinline void predict( const Vector& origin, const Vector& velocity, float throw_time, int offset )
		{
			m_origin = origin;
			m_velocity = velocity;
			m_collision_group = COLLISION_GROUP_PROJECTILE;

			const auto tick = time_to_ticks( 1.f / 30.f );

			m_last_update_tick = -tick;

			switch ( m_index )
			{
				case WEAPON_SMOKEGRENADE: m_next_think_tick = time_to_ticks( 1.5f ); break;
				case WEAPON_DECOY: m_next_think_tick = time_to_ticks( 2.f ); break;
				case WEAPON_FLASHBANG:
				case WEAPON_HEGRENADE:
					m_detonate_time = 1.5f;
					m_next_think_tick = time_to_ticks( 0.02f );

					break;
				case WEAPON_MOLOTOV:
				case WEAPON_INCGRENADE:
					var( molotov_throw_detonate_time );

					m_detonate_time = molotov_throw_detonate_time->get_float();
					m_next_think_tick = time_to_ticks( 0.02f );

					break;
			}

			for ( ; m_tick < time_to_ticks( 60.f ); ++m_tick )
			{
				if ( m_next_think_tick <= m_tick )
				{
					think();
				}

				if ( m_tick < offset )
					continue;

				if ( physics_simulate() )
					break;

				if ( m_last_update_tick + tick > m_tick )
					continue;

				update_path< false >();
			}

			if ( m_last_update_tick + tick <= m_tick )
			{
				update_path< false >();
			}

			if ( !m_path.empty() && ( m_index == WEAPON_MOLOTOV || m_index == WEAPON_INCGRENADE ) )
			{
				trace_t tr{};
				const auto start = std::get<Vector>( m_path.back() );
				const auto end = Vector{ start.x, start.y, start.z - 115.f };

				globals::util_trace_hull( start, end, Vector( 2.0f, 2.0f, 2.0f ), 0x200400B, m_owner, &tr, m_collision_group );
				if ( tr.fraction < 1.f )
				{
					m_origin = tr.endpos;
					update_path< true >();
				}
			}

			m_expire_time = throw_time + ticks_to_time( m_tick );
		}

		bool draw() const;

		bool											m_detonated{};
		C_BasePlayer* m_owner{};
		Vector											m_origin{}, m_velocity{};
		IClientEntity* m_last_hit_entity{};
		Collision_Group_t								m_collision_group{};
		float											m_detonate_time{}, m_expire_time{};
		int												m_index{}, m_tick{}, m_next_think_tick{}, m_last_update_tick{}, m_bounces_count{};
		std::vector< std::pair< Vector, bool > >		m_path{};
	} inline m_data{};

	inline tsl::robin_map< CBaseHandle, data_t > m_list{};
	inline tsl::robin_map< CBaseHandle, float > m_grenade_spawntimes{};
	inline std::vector<std::pair< int, float >> m_outstanding_grenades{};

	struct grenade_esp_data_t
	{
		grenade_esp_data_t( Vector pos, int index, float explode_perc, float damage, float distance ) : pos( pos ), index( index ), explode_perc( explode_perc ), damage( damage ), distance( distance )
		{
			if ( const auto wpn_info = interfaces::weapon_system()->GetWpnData( index ) )
				icon_string = wpn_info->get_icon_name();
			else
				icon_string = {};
		}

		Vector pos;
		int index;
		std::string icon_string;
		float explode_perc;
		float damage;
		float distance;
	};

	inline std::vector<grenade_esp_data_t> m_current_esp_data = {};
};