#include "../include_cheat.h"

bool penetration::is_cfuncbrush( IClientEntity* entity )
{
	if ( !entity || entity->EntIndex() == 0 )
		return false;


	const auto client_class = entity->GetClientClass();

	if ( !client_class )
		return false;

	_( CFuncBrush, "CFuncBrush" );
	return strstr( client_class->m_pNetworkName, CFuncBrush.c_str() ) != nullptr;
}

////////////////////////////////////// Misc Functions //////////////////////////////////////
bool penetration::is_breakable_entity( C_BaseEntity* entity )
{
	if ( !entity )
		return false;

	const auto clientclass = entity->GetClientClass();

	const auto result = entity->is_breakable_entity();

	if ( !result &&
		( clientclass->m_ClassID == ClassId::CBaseDoor ||
		clientclass->m_ClassID == ClassId::CBreakableSurface ||
		( clientclass->m_ClassID == ClassId::CBaseEntity && entity->GetCollideable()->GetSolid() == SOLID_BSP ) ) )
		return true;

	return result;
}

void penetration::scale_damage( int item_definition_index, int hit_group, float armor, bool heavy_armor, bool helment, float& currentDamage )
{
	const auto weapon_data = interfaces::weapon_system()->GetWpnData( item_definition_index );
	if ( !weapon_data )
		return;

	const auto is_armored = [&] () -> bool
	{
		if ( armor > 0.f )
		{
			switch ( hit_group )
			{
				case HITGROUP_GENERIC:
				case HITGROUP_CHEST:
				case HITGROUP_STOMACH:
				case HITGROUP_LEFTARM:
				case HITGROUP_RIGHTARM:
					return true;
				case HITGROUP_HEAD:
					return helment;
				default:
					break;
			}
		}

		return false;
	};

	switch ( hit_group )
	{
		case HITGROUP_HEAD:
			if ( heavy_armor )
				currentDamage = ( currentDamage * 4.f ) * .5f;
			else
				currentDamage *= 4.f;
			break;
		case HITGROUP_STOMACH:
			currentDamage *= 1.25f;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			currentDamage *= .75f;
			break;
		default:
			break;
	}

	if ( is_armored() )
	{
		auto modifier = 1.f, armor_bonus_ratio = .5f, armor_ratio = weapon_data->flarmorratio * .5f;

		if ( heavy_armor )
		{
			armor_bonus_ratio = 0.33f;
			armor_ratio = ( weapon_data->flarmorratio * 0.5f ) * 0.5f;
			modifier = 0.33f;
		}

		auto new_damage = currentDamage * armor_ratio;

		if ( heavy_armor )
			new_damage *= 0.85f;

		if ( ( currentDamage - currentDamage * armor_ratio ) * ( modifier * armor_bonus_ratio ) > armor )
			new_damage = currentDamage - armor / armor_bonus_ratio;

		currentDamage = new_damage;
	}
}

void penetration::scale_damage( C_CSPlayer* ent, int hit_group, CCSWeaponData* weapon_data, float& currentDamage, bool scale )
{
	const auto is_armored = [&] () -> bool
	{
		if ( ent->get_armor() > 0.f )
		{
			switch ( hit_group )
			{
				case HITGROUP_GENERIC:
				case HITGROUP_CHEST:
				case HITGROUP_STOMACH:
				case HITGROUP_LEFTARM:
				case HITGROUP_RIGHTARM:
					return true;
				case HITGROUP_HEAD:
					return ent->get_helmet();
				default:
					break;
			}
		}

		return false;
	};

	if ( scale )
	{
		switch ( hit_group )
		{
			case HITGROUP_HEAD:
				if ( ent->get_heavy_armor() )
					currentDamage = ( currentDamage * 4.f ) * .5f;
				else
					currentDamage *= 4.f;
				break;
			case HITGROUP_STOMACH:
				currentDamage *= 1.25f;
				break;
			case HITGROUP_LEFTLEG:
			case HITGROUP_RIGHTLEG:
				currentDamage *= .75f;
				break;
			default:
				break;
		}
	}

	if ( is_armored() )
	{
		auto modifier = 1.f, armor_bonus_ratio = .5f, armor_ratio = weapon_data->flarmorratio * .5f;

		if ( ent->get_heavy_armor() )
		{
			armor_bonus_ratio = 0.33f;
			armor_ratio = ( weapon_data->flarmorratio * 0.5f ) * 0.5f;
			modifier = 0.33f;
		}

		auto new_damage = currentDamage * armor_ratio;

		if ( ent->get_heavy_armor() )
			new_damage *= 0.85f;

		if ( ( currentDamage - currentDamage * armor_ratio ) * ( modifier * armor_bonus_ratio ) > ent->get_armor() )
			new_damage = currentDamage - ent->get_armor() / armor_bonus_ratio;

		currentDamage = new_damage;
	}
}

bool penetration::trace_to_studio_csgo_hitgroups_priority( Ray_t* ray, CStudioHdr* pStudioHdr, mstudiohitboxset_t* set, matrix3x4_t** scan_matrix, int fContentsMask, Vector* vecOrigin, float flScale, trace_t* trace )
{
	const auto function = make_offset( "client.dll", sig_trace_to_studio_csgo_hitgroups_priority );
	auto rval = true;

	__asm
	{
		mov edx, ray
		push trace
		push flScale
		push vecOrigin
		push fContentsMask
		push scan_matrix
		push set
		push pStudioHdr
		mov eax, [ function ]
		call eax
		add esp, 0x1C
		mov rval, al
	}

	return rval;
}

////////////////////////////////////// Main Autowall Functions //////////////////////////////////////
bool penetration::trace_to_exit( CGameTrace& enter_trace, CGameTrace& exit_trace, const Vector start_position, const Vector direction )
{
	constexpr auto max_distance = 90.f;
	constexpr auto ray_extension = 4.f;
	float current_distance = 0;
	auto first_contents = 0;
	cbrush_t* prev_brush = nullptr;

	while ( current_distance <= max_distance )
	{
		current_distance += ray_extension;

		auto start = start_position + direction * current_distance;

		const auto point_contents = interfaces::engine_trace()->GetPointContents_WorldOnly( start, MASK_SHOT_HULL );

		if ( !first_contents )
			first_contents = point_contents;

		if ( !( point_contents & MASK_SHOT_HULL ) )
		{
			auto end = start - direction * ray_extension;

			globals::util_trace_line( start, end, MASK_SHOT_HULL, nullptr, &exit_trace );

			if ( exit_trace.startsolid && exit_trace.surface.flags & SURF_HITBOX )
			{
				globals::util_trace_line( start, start_position, MASK_SHOT_HULL, exit_trace.m_pEnt, &exit_trace );

				if ( exit_trace.DidHit() && !exit_trace.startsolid )
					return true;
			}
			else if ( exit_trace.DidHit() && !exit_trace.startsolid )
			{
				if ( exit_trace.surface.flags & SURF_NODRAW && is_breakable_entity( enter_trace.m_pEnt ) && is_breakable_entity( exit_trace.m_pEnt ) )
					return true;

				if ( ( !( exit_trace.surface.flags & SURF_NODRAW ) || enter_trace.surface.flags & SURF_NODRAW ) && exit_trace.plane.normal.Dot( direction ) <= 1.f )
					return true;
			}
			else if ( enter_trace.DidHitNonWorldEntity() && is_breakable_entity( enter_trace.m_pEnt ) )
			{
				exit_trace = enter_trace;
				exit_trace.endpos = start + direction;
				return true;
			}
		}
	}

	return false;
}

bool penetration::handle_bullet_penetration( CCSWeaponData* weapon_data, CGameTrace& enter_trace, Vector& eye_position, const Vector direction, int& possible_hits_remaining, float& current_damage, const float penetration_power )
{
	CGameTrace exit_trace;
	const auto enter_surface_data = interfaces::surface_props()->GetSurfaceData( enter_trace.surface.surfaceProps );
	const int enter_material = enter_surface_data->game.material;

	const auto enter_surf_penetration_modifier = enter_surface_data->game.penetrationmodifier;
	float combined_penetration_modifier;
	const bool is_solid_surf = enter_trace.contents >> 3 & CONTENTS_SOLID;
	const bool is_light_surf = enter_trace.surface.flags >> 7 & SURF_LIGHT;
	const  bool hit_grate = ( enter_trace.contents & CONTENTS_GRATE ) != 0;

	if ( possible_hits_remaining <= 0
		 || weapon_data->flpenetration <= 0.f
		 || !trace_to_exit( enter_trace, exit_trace, enter_trace.endpos, direction )
		 && !( interfaces::engine_trace()->GetPointContents_WorldOnly( enter_trace.endpos, MASK_SHOT_HULL ) & MASK_SHOT_HULL ) )
		return false;

	const auto exit_surface_data = interfaces::surface_props()->GetSurfaceData( exit_trace.surface.surfaceProps );
	const auto exit_material = exit_surface_data->game.material;
	const auto exit_surf_penetration_modifier = exit_surface_data->game.penetrationmodifier;
	const auto is_nodraw = !!( enter_trace.surface.flags & ( SURF_NODRAW ) );
	auto final_damage_modifier = 0.16f;

	var( ff_damage_reduction_bullets );
	if ( hit_grate || is_nodraw || enter_material == CHAR_TEX_GLASS || enter_material == CHAR_TEX_GRATE )
	{
		// If we're a concrete grate (TOOLS/TOOLSINVISIBLE texture) allow more penetrating power.
		if ( enter_material == CHAR_TEX_GLASS || enter_material == CHAR_TEX_GRATE )
		{
			combined_penetration_modifier = 3.0f;
			final_damage_modifier = 0.05f;
		}
		else
			combined_penetration_modifier = 1.0f;
	}
	else if ( is_solid_surf || is_light_surf )
	{
		combined_penetration_modifier = 1.f;
	}
	else if ( enter_material == CHAR_TEX_FLESH )
	{

		combined_penetration_modifier = ff_damage_reduction_bullets->get_float();
		if ( combined_penetration_modifier == 0.f )
			return false;
	}
	else
	{
		combined_penetration_modifier = ( enter_surf_penetration_modifier + exit_surf_penetration_modifier ) / 2.f;
	}

	if ( enter_material == exit_material )
	{
		if ( exit_material == CHAR_TEX_CARDBOARD || exit_material == CHAR_TEX_WOOD )
			combined_penetration_modifier = 3.f;
		else if ( exit_material == CHAR_TEX_PLASTIC )
			combined_penetration_modifier = 2.f;
	}

	auto thickness = ( exit_trace.endpos - enter_trace.endpos ).Length();
	thickness *= thickness;
	thickness *= fmaxf( 0.f, 1.0f / combined_penetration_modifier );
	thickness /= 24.0f;

	const auto v35 = ( current_damage * final_damage_modifier ) + fmaxf( 0.f, 1.0f / combined_penetration_modifier ) * 3.0f * fmaxf( 0.0f, ( 3.0f / penetration_power ) * 1.25f );

	const auto lost_damage = fmaxf( 0.0f, v35 + thickness );

	if ( lost_damage > current_damage )
		return false;

	if ( lost_damage > 0.f )
		current_damage -= lost_damage;

	if ( current_damage < 1.f )
		return false;

	eye_position = exit_trace.endpos;
	--possible_hits_remaining;

	return true;
}

inline auto distance_to_ray = [] ( const Vector& pos, const Vector& start, const Vector& end ) -> float
{
	const auto to = pos - start;
	auto dir = end - start;
	const auto length = dir.Length();
	dir.NormalizeInPlace();
	const auto range_along = dir.Dot( to );

	float range;

	if ( range_along < 0.0f )
		range = -( pos - start ).Length();
	else if ( range_along > length )
		range = -( pos - end ).Length();
	else
		range = ( pos - ( start + ( dir * range_along ) ) ).Length();

	return range;
};

bool penetration::fire_bullet( C_WeaponCSBaseGun* weapon, pen_data& pen_data, const Vector pos, aimbot::aimpoint_t* point, int& damage, bool hitchance )
{
	auto eye_position = pos;
	const auto player = pen_data.record ? globals::get_player( pen_data.record->m_index ) : nullptr;
	const auto shooter = !weapon ? local_player : reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity_from_handle( weapon->get_owner_ent() ) );

	auto direction = point->point - eye_position;
	math::fast_vec_normalize( direction );

	const auto dist = point->point.Dist( eye_position );
	const auto dist_origin = pen_data.record ? pen_data.record->m_origin.Dist( eye_position ) : 0.f;

	CGameTrace enter_trace;
	const auto weapon_data = pen_data.custom_data ? pen_data.custom_data : weapon->get_wpn_data();
	auto current_distance = 0.f;
	const auto is_knife = weapon && weapon->is_knife();

	const auto penetration_power = weapon_data->flpenetration;
	auto max_range = weapon_data->range;

	float current_damage = weapon_data->idamage;

	auto hits_remaining = 4;
	while ( hits_remaining >= 0 && current_damage >= 1.f )
	{
		max_range -= current_distance;

		Ray_t ray;
		ray.Init( eye_position, eye_position + direction * max_range );
		if ( player )
		{
			CTraceFilterSimple filter( player, shooter );
			interfaces::engine_trace()->TraceRay( ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &enter_trace );
		}
		else
		{
			CTraceFilterSimpleNoPlayers filter( nullptr, 0 );
			interfaces::engine_trace()->TraceRay( ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &enter_trace );
		}

		if ( !player && enter_trace.startsolid )
			break;

		if ( pen_data.one_wall )
		{
			if ( pos.Dist( enter_trace.endpos ) >= dist )
			{
				damage = current_damage;
				return true;
			}

			if ( hits_remaining < 4 )
			{
				if ( enter_trace.startpos.Dist( enter_trace.endpos ) >= 4.f )
				{
					damage = current_damage;
					return true;
				}
			}
		}

		if ( !player && pos.Dist( enter_trace.endpos ) >= dist )
		{
			current_distance += enter_trace.startpos.Dist( point->point );
			current_damage *= pow( weapon_data->flrangemodifier, ( current_distance / 500.f ) );
			damage = current_damage;

			if ( pen_data.out_trace )
				*pen_data.out_trace = enter_trace;
			return true;
		}

		if ( player && ( enter_trace.fraction == 1.f || pos.Dist( enter_trace.endpos ) >= dist || pos.Dist( enter_trace.endpos ) >= dist_origin ) )
		{
			if ( !pen_data.custom_data && weapon->is_knife() )
				return true;

			auto ray_end = eye_position + direction * ( max_range + ray_extension );
			ray.Init( eye_position, ray_end );
			if ( pen_data.custom_test )
			{
				pen_data.tr.fraction = ray.m_Start.Dist( pen_data.tr.endpos ) / ray.m_Start.Dist( ray_end );
				enter_trace = pen_data.tr;
			}
			else
			{
				if ( !test_hitboxes( enter_trace, ray, pen_data ) || enter_trace.hitgroup != point->hitgroup && !hitchance && player != local_player )
				{
					point->penetration_points.push_back( enter_trace.endpos );
					break;
				}
			}

			auto smallest_fraction = eye_position.Dist( enter_trace.endpos ) / max_range;
			const auto range = distance_to_ray( pen_data.record->m_origin + Vector{ 0.f, 0.f, .5f * player->GetCollideable()->OBBMaxs().z }, ray.m_Start, ray.m_Start + ray.m_Delta );
			if ( range >= 0.0f && range <= 60.f && enter_trace.fraction < smallest_fraction )
				smallest_fraction = enter_trace.fraction;

			current_distance += smallest_fraction * ( max_range + ray_extension );

			if ( current_distance > max_range )
				break;

			current_damage *= pow( weapon_data->flrangemodifier, ( current_distance / 500.f ) );

			point->penetration_points.push_back( enter_trace.endpos );

			scale_damage( player, enter_trace.hitgroup, weapon_data, current_damage, pen_data.custom_data || !weapon->is_zeus() );
			damage = current_damage;

			point->point = enter_trace.endpos;
			if ( hitchance )
				point->hitbox = enter_trace.hitbox;
			point->hitgroup = enter_trace.hitgroup;

			if ( pen_data.out_trace )
				*pen_data.out_trace = enter_trace;
			return true;
		}

		const auto enter_surface_data = interfaces::surface_props()->GetSurfaceData( enter_trace.surface.surfaceProps );
		const auto enter_surf_penetration_modifier = enter_surface_data->game.penetrationmodifier;

		current_distance += enter_trace.fraction * max_range;
		current_damage *= pow( weapon_data->flrangemodifier, ( current_distance / 500.f ) );

		if ( enter_trace.fraction == 1.f )
			break;

		if ( current_distance > max_range && weapon_data->flpenetration > 0.f || enter_surf_penetration_modifier < 0.1f )
			break;

		if ( enter_trace.fraction == 0.f )
			break;

		if ( is_knife )
			break;

		point->penetration_points.push_back( enter_trace.endpos );

		if ( !handle_bullet_penetration( weapon_data, enter_trace, eye_position, direction, hits_remaining, current_damage, penetration_power ) )
			break;
	}

	if ( pen_data.out_trace )
		*pen_data.out_trace = enter_trace;

	damage = 0;
	return false;
}

bool penetration::test_hitboxes( trace_t& tr, Ray_t& ray, const pen_data& pen_data, int force_hitgorup )
{
	const auto player = globals::get_player( pen_data.record->m_index );

	const auto studio_model = interfaces::model_info()->GetStudioModel( player->get_model() );
	if ( !studio_model )
		return false;

	const auto hitbox_set = studio_model->pHitboxSet( player->get_hitbox_set() );
	if ( !hitbox_set )
		return false;

	/*if ( player != local_player )
		interfaces::debug_overlay()->AddLineOverlay( ray.m_Start + ray.m_StartOffset, ray.m_Start + ray.m_Delta, 0, 255, 0, true, 0.2f );*/

	matrix3x4_t* hitboxbones[ 128 ];
	for ( auto i = 0; i < 128; i++ )
		hitboxbones[ i ] = &pen_data.record->matrix( pen_data.dir )[ i ];
	if ( trace_to_studio_csgo_hitgroups_priority( &ray, player->get_model_ptr(), hitbox_set, hitboxbones, MASK_SHOT_HULL | CONTENTS_HITBOX, &pen_data.record->m_origin, player->get_model_scale(), &tr ) && ( force_hitgorup == -1 || tr.hitgroup == force_hitgorup ) )
	{
		const auto pbox = hitbox_set->pHitbox( tr.hitbox );
		const auto pbone = studio_model->pBone( pbox->bone );
		tr.surface.flags = SURF_HITBOX;
		tr.surface.name = "**studio**";
		tr.surface.surfaceProps = interfaces::surface_props()->GetSurfaceIndex( pbone->pszSurfaceProp() );

		if ( pen_data.safety && force_hitgorup != -1 )
		{
			auto run_test = [&] ( resolver_direction dir )
			{
				auto tr_copy = tr;

				for ( auto i = 0; i < 128; i++ )
					hitboxbones[ i ] = &pen_data.record->matrix( dir )[ i ];

				return trace_to_studio_csgo_hitgroups_priority( &ray, player->get_model_ptr(), hitbox_set, hitboxbones, MASK_SHOT_HULL | CONTENTS_HITBOX, &pen_data.record->m_origin, player->get_model_scale(), &tr_copy ) && tr_copy.hitgroup == force_hitgorup;
			};

			if ( pen_data.safety == safety_no_roll )
			{
				if ( !run_test( resolver_direction::resolver_min )
				|| !run_test( resolver_direction::resolver_max ) )
					return false;
			}
			else
			{
				if ( !run_test( resolver_direction::resolver_min_min )
				|| !run_test( resolver_direction::resolver_max_max )
				|| !run_test( resolver_direction::resolver_min_extra )
				|| !run_test( resolver_direction::resolver_max_extra ) )
					return true;
			}

		}

		return true;
	}

	return false;
}

int penetration::can_wallbang()
{
	QAngle angles{};
	Vector direction = {};
	interfaces::engine()->GetViewAngles( angles );

	math::angle_vectors( angles, &direction );
	direction.NormalizeInPlace();

	const auto weapon = local_weapon;
	const auto pos = local_player->get_eye_pos();

	if ( !weapon->is_gun() )
		return -1;

	pen_data pen_data = {};
	pen_data.custom_data = interfaces::weapon_system()->GetWpnData( weapon->get_weapon_id() );
	pen_data.one_wall = true;

	aimbot::aimpoint_t point;
	point.point = pos + direction * pen_data.custom_data->range;

	if ( !point.point.IsValid() || !pos.IsValid() )
		return false;

	if ( !pen_data.custom_data )
		return false;

	auto damage = 0;
	const auto ret = fire_bullet( pen_data.weapon, pen_data, pos, &point, damage, false );
	if ( !ret )
		damage = 0;

	auto current_damage = static_cast< float >( damage );
	scale_damage( weapon->get_weapon_id(), HITGROUP_HEAD, false, false, false, current_damage );

	return current_damage >= config::current_config( weapon )->mindmg->get<int>();
}

bool penetration::check_hittable( awall_trace_t trace )
{
	const auto dist = trace.start.Dist( trace.point );
	auto direction = trace.point - trace.start;
	math::fast_vec_normalize( direction );

	Ray_t ray;
	CTraceFilterNoPlayers filter{};
	cbrush_t* prev_brush = nullptr;

	constexpr auto ray_extension = 12.f;

	auto current_distance = 0.f;
	auto hits_remaining = 4;
	auto wall_thickness = 0.f;

	while ( current_distance <= dist )
	{
		if ( hits_remaining < 0 )
			return false;

		current_distance += ray_extension;

		auto start = trace.start + direction * current_distance;

		trace_t tr;
		ray.Init( start, trace.point );
		interfaces::engine_trace()->TraceRay( ray, MASK_SHOT_HULL, &filter, &tr );

		if ( tr.fraction == 1.f )
			return true;

		hits_remaining--;
		wall_thickness += ray_extension;

		current_distance += start.Dist( trace.point ) * tr.fraction + ray_extension;

		while ( current_distance <= dist )
		{
			start = trace.start + direction * current_distance;
			auto hit_wall = false;
			//if ( GetAsyncKeyState( 'C' ) )
			hit_wall = !!( interfaces::engine_trace()->GetPointContents_WorldOnly( start, MASK_SHOT_HULL ) & MASK_SHOT_HULL );
			//else
				//hit_wall = !!( interfaces::engine_trace()->GetPointContents_WorldOnly( prev_brush, start, MASK_SHOT_HULL ) & MASK_SHOT_HULL );

			if ( hit_wall )
				wall_thickness += ray_extension;
			else
			{
				wall_thickness = 0.f;
				break;
			}

			if ( wall_thickness >= 90.f )
				return false;

			current_distance += ray_extension;
		}
	}

	return true;
}

bool penetration::can_hit( C_CSPlayer* shooter, pen_data pen_data, const Vector pos, aimbot::aimpoint_t* point, int& damage, bool hitchance )
{
	if ( !point->point.IsValid() || !pos.IsValid() )
		return false;

	if ( !pen_data.custom_data )
	{
		pen_data.weapon = globals::get_weapon( shooter->get_active_weapon() );
		if ( !pen_data.weapon )
			return false;

		if ( pen_data.weapon->is_knife() && pen_data.record )
		{
			auto stab = false;
			if ( !knifebot::trace_knife( point->point, pen_data.record, stab ) )
				point->damage = 0;
			else
				point->damage = knifebot::get_damage( pen_data.record, stab );

			return point->damage > 0;
		}

		if ( !pen_data.weapon->is_zeus() )
		{
			auto max_range = pen_data.custom_data ? pen_data.custom_data->range : pen_data.weapon->get_wpn_data()->range;
			if ( pos.Dist( point->point ) > max_range + 5.f )
				return false;
		}
	}

	if ( const auto player = pen_data.record ? globals::get_player( pen_data.record->m_index ) : nullptr; player && point->hitbox != -2 )
	{
		auto direction = point->point - pos;
		math::fast_vec_normalize( direction );

		auto max_range = pen_data.custom_data ? pen_data.custom_data->range : pen_data.weapon->get_wpn_data()->range;

		CGameTrace enter_trace;
		Ray_t ray;
		ray.Init( pos, pos + direction * ( max_range + ray_extension ) );
		if ( !test_hitboxes( enter_trace, ray, pen_data, hitchance || player == local_player ? -1 : point->hitgroup ) )
		{
			damage = 0;
			point->safety = safety_none;
			return false;
		}

		pen_data.custom_test = true;
		pen_data.tr = enter_trace;
	}

	const auto ret = fire_bullet( pen_data.weapon, pen_data, pos, point, damage, hitchance );
	if ( !ret )
		damage = 0;

	return ret;
}

bool penetration::can_hit( int item_definition_index, const Vector pos, const Vector end, int& damage, trace_t& trace )
{
	aimbot::aimpoint_t point;
	point.point = end;

	if ( !point.point.IsValid() || !pos.IsValid() )
		return false;

	pen_data pen_data = {};
	pen_data.custom_data = interfaces::weapon_system()->GetWpnData( item_definition_index );
	pen_data.out_trace = &trace;

	if ( !pen_data.custom_data )
		return false;

	if ( pos.Dist( point.point ) > pen_data.custom_data->range + 5.f )
		return false;

	const auto ret = fire_bullet( pen_data.weapon, pen_data, pos, &point, damage, false );
	if ( !ret )
		damage = 0;

	return ret;
}

CCSWeaponData* penetration::get_normal_weapon()
{
	static CCSWeaponData normal_weapon{};
	normal_weapon.idamage = 200;
	normal_weapon.flrangemodifier = 1.0f;
	normal_weapon.flpenetration = 6.0f;
	normal_weapon.flarmorratio = 2.0f;
	normal_weapon.range = 8192.f;
	return &normal_weapon;
}

CCSWeaponData* penetration::get_full_weapon()
{
	static CCSWeaponData full_weapon{};
	full_weapon.idamage = 20000;
	full_weapon.flrangemodifier = 1.0f;
	full_weapon.flpenetration = 3.0f;
	full_weapon.flarmorratio = 1.0f;
	full_weapon.range = 8192.f;
	return &full_weapon;
}