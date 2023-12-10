#include "../include_cheat.h"
#include <algorithm>

void IClientEntityList::for_each_z( std::function<void( C_BaseEntity* )> fn )
{
	const auto view_setup = interfaces::view_render()->GetViewSetup();

	if ( !view_setup )
		return;

	static std::vector<std::pair<C_BaseEntity*, float>> order;
	order.clear();

	for ( auto ent : *this )
		order.push_back( std::make_pair( ent, ( ent->get_abs_origin() - view_setup->origin ).Length2D() ) );

	std::sort( order.begin(), order.end(),
		[] ( const std::pair<C_BaseEntity*, float>& a, const std::pair<C_BaseEntity*, float>& b ) -> bool
	{
		return a.second > b.second;
	} );

	for ( auto const& ent : order )
		fn( ent.first );
}

void IClientEntityList::for_each_player_z( std::function<void( C_CSPlayer* )> fn )
{
	const auto view_setup = interfaces::view_render()->GetViewSetup();

	if ( !view_setup )
		return;

	static std::vector<std::pair<C_CSPlayer*, float>> order;
	order.clear();

	for ( auto ent : get_players() )
		order.push_back( std::make_pair( ent, ( ent->get_abs_origin() - view_setup->origin ).Length2D() ) );

	std::sort( order.begin(), order.end(),
		[] ( const std::pair<C_CSPlayer*, float>& a, const std::pair<C_CSPlayer*, float>& b ) -> bool
	{
		return a.second > b.second;
	} );

	for ( auto const& ent : order )
		fn( ent.first );
}

inline bool invalidChar( char c )
{
	return !( c >= 0 );
}
inline void stripUnicode( std::string& str )
{
	str.erase( std::ranges::remove_if( str, invalidChar ).begin(), str.end() );
}

model_t*& C_BaseEntity::get_model() { return *reinterpret_cast< model_t** > ( reinterpret_cast< uintptr_t >( this ) + 0x6C ); }

IClientRenderable* C_BaseEntity::get_renderable() { return  reinterpret_cast< IClientRenderable* > ( reinterpret_cast< uintptr_t >( this ) + 0x4 ); }

const wchar_t* C_BaseAttributableItem::get_diplay_name()
{
	const auto econ_item = this->get_econ_item_view();
	if ( !econ_item )
		return L"";

	const auto item_def = econ_item->get_static_data();
	if ( !item_def )
		return L"";

	return item_def->GetLocalizedName();
}

const wchar_t* CEconItemDefinition::GetLocalizedName()
{
	return interfaces::localize()->Find( this->get_item_base_name() );
};

bool C_BasePlayer::get_alive() { return this->get_lifestate() == LifeState::ALIVE && ( this == local_player || !( ( C_CSPlayer* ) this )->is_ghost() ); }
Vector C_BasePlayer::get_eye_pos( bool interpolated )
{
	Vector out{};
	if ( this->EntIndex() == interfaces::engine()->GetLocalPlayer() )
	{
		util::get_vfunc<void( __thiscall* )( void*, Vector& )>( this, 285 )( this, out );
		return out;
	}

	if ( !interpolated )
	{
		out = get_origin();

		const auto duck_view_offset = interfaces::game_movement()->GetPlayerViewOffset( true );
		const auto stand_view_offset = interfaces::game_movement()->GetPlayerViewOffset( false );

		out += math::lerp( stand_view_offset, duck_view_offset, get_duck_amt() );

		return out;
	}

	out = get_abs_origin();

	const auto duck_view_offset = interfaces::game_movement()->GetPlayerViewOffset( true );
	const auto stand_view_offset = interfaces::game_movement()->GetPlayerViewOffset( false );

	const auto delta = stand_view_offset.z - duck_view_offset.z;

	const auto duck_amount = get_duck_amt() * 0.9f;

	const auto should_deduct = get_flags() & FL_ONGROUND;

	out.z += stand_view_offset.z - delta * ( should_deduct ? duck_amount : 0.f );

	return out;
}
bool C_BasePlayer::is_enemy()
{
	if ( globals::is_dangerzone() )
	{
		// solo
		if ( local_player->get_survival_team() < 0 && this->EntIndex() != local_player->EntIndex() )
			return true;

		// squads
		if ( local_player->get_survival_team() != ( ( C_CSPlayer* ) this )->get_survival_team() )
			return true;

		return false;
	}

	var( mp_teammates_are_enemies );
	if ( mp_teammates_are_enemies->get_bool() && this->EntIndex() != local_player->EntIndex() )
		return true;

	return this->get_team() != local_player->get_team();
}

int mbtowc( LPCSTR src, INT srclen, LPWSTR dst, INT dstlen )
{
	if ( srclen < 0 )
		srclen = strlen( src ) + 1;

	if ( !dstlen )
		return srclen;

	int i;
	for ( i = 0; i < srclen && i < dstlen; i++ )
		dst[ i ] = src[ i ];

	return i;
}

player_info_t C_BasePlayer::get_player_info() const { player_info_t pinfo; interfaces::engine()->GetPlayerInfo( this->EntIndex(), &pinfo ); return pinfo; }
std::string C_BasePlayer::get_name() const { return std::string( this->get_player_info().name ); }
std::wstring C_BasePlayer::get_name_unicode() const
{
	return util::utf8_decode( this->get_player_info().name );
}
bool C_BaseCombatWeapon::is_grenade() { return this->get_weapon_id() >= WEAPON_FLASHBANG && this->get_weapon_id() <= WEAPON_INCGRENADE; }
int C_BaseCombatWeapon::get_weapon_id() { return reinterpret_cast< C_BaseAttributableItem* >( this )->get_item_definition_index(); }
bool C_BaseCombatWeapon::is_knife() { return this->get_weapon_type() == WEAPONTYPE_KNIFE && this->get_weapon_id() != WEAPON_TASER; }
bool C_BaseCombatWeapon::is_secondary_attack_weapon() { return this->is_knife() || this->is_shield(); }
bool C_BaseCombatWeapon::is_shield() { return this->get_weapon_id() == WEAPON_SHIELD; }
bool C_BaseCombatWeapon::is_zeus() { return this->get_weapon_id() == WEAPON_TASER; }
bool C_BaseCombatWeapon::is_shootable()
{
	const auto id = this->get_weapon_id();
	return !this->is_grenade() && !this->is_knife() && !this->is_shield() && id != WEAPON_C4 && id != WEAPON_HEALTHSHOT;
}
bool C_BaseCombatWeapon::is_gun()
{
	const auto type = this->get_weapon_type();
	return type == WEAPONTYPE_MACHINEGUN || type == WEAPONTYPE_PISTOL || type == WEAPONTYPE_RIFLE || type == WEAPONTYPE_SNIPER_RIFLE || type == WEAPONTYPE_SHOTGUN || type == WEAPONTYPE_SUBMACHINEGUN;
}

CCSWeaponData* C_BaseCombatWeapon::get_wpn_data_safe()
{
	return interfaces::weapon_system()->GetWpnData( this->get_weapon_id() );
}

C_CSPlayer* C_BasePlayer::get_observer()
{
	C_CSPlayer* observer = nullptr;
	const auto spec_handle = this->get_observer_target();
	if ( spec_handle )
		observer = reinterpret_cast< C_CSPlayer* >( interfaces::entity_list()->get_client_entity_from_handle( spec_handle ) );

	return observer;
}

int C_BasePlayer::get_predicted_health()
{
	auto predicted_health = IsDormant() ? clamp( ( *interfaces::player_resource() )->get_health( EntIndex() ), 0, 100 ) : get_health();
	for ( const auto& shot : resolver::shots )
		if ( shot.enemy_index == this->EntIndex() )
			predicted_health -= shot.damage;

	return predicted_health <= 0 ? get_health() : predicted_health;
}

bool C_CSPlayer::setup_bones( lag_record_t& record, resolver_direction dir, bool extrapolated )
{
	const auto ret = bone_setup::handle_bone_setup( record, dir, false, extrapolated );
	if ( !ret )
	{
		memset( record.m_state[ dir ].m_matrix, 0, sizeof( record.m_state[ dir ].m_matrix ) );
		record.m_valid = false;
	}
	return ret;
}

bool C_BaseWeaponWorldModel::holds_player_animations()
{
	const auto studio_hdr = this->get_model_ptr();

	auto numseq = 0;

	if ( !studio_hdr->m_pVModel )
		numseq = studio_hdr->m_pStudioHdr->numlocalseq;
	else
		numseq = studio_hdr->m_pVModel->m_seq.Count();

	return numseq >= 3;
}

void C_CSPlayer::set_pose_params( std::array<float, 24>& params )
{
	memcpy( this->get_pose_params().data(), params.data(), sizeof( params ) );
}

void C_CSPlayer::set_ragdoll_pos( std::array<float, 24>& ragposes )
{
	memcpy( this->get_ragdoll_pos().data(), ragposes.data(), sizeof( ragposes ) );
}

int C_BaseAnimating::get_sequence_act( int sequence )
{
	const auto model = this->GetModel();
	if ( !model )
		return -1;

	const auto hdr = interfaces::model_info()->GetStudioModel( model );
	if ( !hdr )
		return -1;

	return reinterpret_cast< int( __fastcall* )( void*, studiohdr_t*, int ) >( make_offset( "client.dll", sig_get_sequence_act ) )( this, hdr, sequence );
}

void C_BaseAnimating::set_cycle( int fcycle )
{
	auto& cycle = get_cycle();
	if ( cycle != fcycle )
	{
		cycle = fcycle;
		invalidate_physics_recursive( 8 );
	}
}

Vector C_CSPlayer::world_space_center()
{
	const auto collideable = GetCollideable();

	const auto max = collideable->OBBMaxs() + get_abs_origin();
	const auto min = collideable->OBBMins() + get_abs_origin();
	auto size = max - min;
	size /= 2;
	size += min;
	return size;
}

void CCSGOPlayerAnimState::update( const QAngle ang )
{
	static auto invalidate_bone_cache = reinterpret_cast< uint8_t* >( make_offset( "client.dll", sig_enable_invalidate_bone_cache ) );
	const auto backup_invalidate_bone_cache = *invalidate_bone_cache;
	_update( ang, false );
	*invalidate_bone_cache = backup_invalidate_bone_cache;
}


Vector C_CSPlayer::get_hitbox_pos( int hitbox )
{
	auto [min, max, radius] = get_hitbox_minmax( hitbox );
	return ( min + max ) * 0.5f;
}

std::tuple<Vector, Vector, float> C_CSPlayer::get_hitbox_minmax( int h )
{
	const auto accessor = get_bone_accessor();
	if ( !accessor )
		return {};

	const auto model = get_model();
	if ( !model )
		return {};

	const auto studio_hdr = interfaces::model_info()->GetStudioModel( model );
	if ( !studio_hdr )
		return {};

	const auto hitbox = studio_hdr->pHitbox( h, 0 );
	if ( !hitbox )
		return {};

	const auto matrix = accessor->get_bone_array_for_write();
	if ( !matrix )
		return{};

	const auto mat = matrix[ hitbox->bone ];

	Vector vmin{}, vmax{};
	math::vector_transform( hitbox->bbmax, mat, vmax );
	math::vector_transform( hitbox->bbmin, mat, vmin );

	return std::make_tuple( vmin, vmax, hitbox->radius );
}

bool C_CSPlayer::is_peeking() const
{
	auto& log = player_log::get_log( this->EntIndex() );
	if ( log.record.empty() )
		return false;

	const auto& first = log.record.back();

	if ( first.m_velocity.Length() > 50.f )
		return true;

	if ( log.record.size() < 2 )
		return false;

	const auto& second = log.record[ log.record.size() - 2 ];

	if ( ( first.m_velocity - second.m_velocity ).Length() > 10.f )
		return true;

	return false;
}

void C_CSPlayer::on_simulation_time_changing( float previous, float current )
{
	const auto fn = make_offset( "client.dll", sig_on_simulation_time_changing );
	__asm
	{
		mov ecx, this
		movss xmm1, previous
		movss xmm2, current
		call fn
	}
}

int32_t C_CSPlayer::get_ammo_count( uint32_t hash )
{
	const auto def = CAmmoDef::get();
	const auto ind = def->index( hash );

	if ( ind == -1 )
		return 0;

	if ( def->can_carry_infinite_ammo( ind ) )
		return 999;

	return get_ammo( ind );
}

void C_CSPlayer::try_initiate_animation( size_t layer, int32_t activity, std::vector<uint16_t>& modifiers, int seed )
{
	const auto empty_mapping = reinterpret_cast< activity_mapping* >( make_offset( "client.dll", sig_empty_mapping ) );

	const auto hdr = this->get_model_ptr();

	if ( !hdr || !hdr->sequences_available() || hdr->get_numseq() == 1 || modifiers.empty() )
		return;

	if ( hdr->get_activity_list_version() < 1 )
		hdr->index_model_sequences();

	auto& mapping = hdr->get_activity_mapping();

	if ( !mapping )
		mapping = hdr->find_mapping();

	if ( mapping != empty_mapping && ( mapping->studio_hdr != hdr->m_pStudioHdr || mapping->v_model != hdr->m_pVModel ) )
		mapping->reinitialize( hdr );

	if ( !mapping->sequence_tuples )
		return;

	hash_value_type dummy{ activity, 0, 0, 0 };
	const uint32_t handle = mapping->act_to_sequence_hash.find_hash( &dummy );
	const int32_t data_index = handle & 0xFFFF;
	const int32_t bucket_index = ( handle >> 16 ) & 0xFFFF;

	if ( bucket_index >= mapping->act_to_sequence_hash.buckets.Count()
		|| data_index >= mapping->act_to_sequence_hash.buckets[ bucket_index ].Count() )
		return;

	const auto data = &mapping->act_to_sequence_hash.buckets[ bucket_index ][ data_index ];

	int32_t top_score = -1;
	std::vector<int32_t> top_scores;
	top_scores.reserve( data->count );

	for ( int i = 0; i < data->count; i++ )
	{
		const auto info = &mapping->sequence_tuples[ data->starting_index + i ];

		auto score = 0;

		for ( auto k = 0; k < info->count_activity_modifiers; k++ )
		{
			auto matching_mods = false;

			for ( auto m = 0; m < modifiers.size(); m++ )
				if ( info->activity_modifiers[ k ] == modifiers[ m ] )
				{
					matching_mods = true;
					break;
				}

			auto done = false;

			switch ( info->calculation_modes[ k ] )
			{
				case 0:
					if ( matching_mods )
						score += 2;
					else
						score--;
					break;
				case 1:
					if ( matching_mods )
						score += 2;
					break;
				case 2:
					score += ( matching_mods ? 2 : -2 );
					break;
				case 3:
					if ( matching_mods )
						done = true;
					break;
				case 4:
					if ( !matching_mods )
						done = true;
					break;
			}

			if ( done )
				break;
		}

		if ( score > top_score )
			top_scores.clear();

		if ( score >= top_score )
		{
			for ( auto i = 0; i < info->weight; i++ )
				top_scores.push_back( info->seqnum );

			top_score = score;
		}
	}

	if ( top_scores.empty() )
		return;

	globals::random_seed( seed );
	const auto winner = top_scores[ globals::random_int( 0, static_cast< int >( top_scores.size() ) - 1 ) ];

	if ( winner < 2 )
		return;

	auto& l = this->get_anim_layers()[ layer ];
	l.m_flPlaybackRate = this->get_layer_sequence_cycle_rate( &l, winner );
	l.m_nSequence = winner;
	l.m_flCycle = l.m_flWeight = 0.f;
	this->get_anim_state()->update_layer_order_preset( layer, winner );
}

template<class T>
T get_entity_var( C_BaseEntity* ent, const char* var )
{
	return *reinterpret_cast< T* >( uintptr_t( ent ) + 0 );
}

bool C_BaseEntity::get_var_bool( const char* var )
{
	return get_entity_var<bool>( this, var );
}

int C_BaseEntity::get_var_int( const char* var )
{
	return get_entity_var<int>( this, var );
}

float C_BaseEntity::get_var_float( const char* var )
{
	return get_entity_var<float>( this, var );
}

short C_BaseEntity::get_var_short( const char* var )
{
	return get_entity_var<short>( this, var );
}

unsigned long C_BaseEntity::get_var_handle( const char* var )
{
	return get_entity_var<unsigned long>( this, var );
}

QAngle C_BaseEntity::get_var_angle( const char* var )
{
	return get_entity_var<QAngle>( this, var );
}

Vector C_BaseEntity::get_var_vector( const char* var )
{
	return get_entity_var<Vector>( this, var );
}

int C_BaseEntity::get_class_id()
{
	return ( int ) GetClientClass()->m_ClassID;
}

bool C_BaseEntity::is_player_lua()
{
	return GetClientClass()->m_ClassID == ClassId::CCSPlayer;
}

bool C_BaseEntity::is_dormant()
{
	return IsDormant();
}

int C_BaseEntity::get_index()
{
	return EntIndex();
}

#undef local_player
C_CSPlayer* C_CSPlayer::local_player()
{
	return globals::get_player( interfaces::engine()->GetLocalPlayer() );
}

C_CSPlayer* C_CSPlayer::from_userid( const int id )
{
	return reinterpret_cast< C_CSPlayer* >( globals::get_player( interfaces::engine()->GetPlayerForUserID( id ) ) );
}

C_CSPlayer* C_CSPlayer::player_from_index( const int i )
{
	return reinterpret_cast< C_CSPlayer* >( globals::get_player( i ) );
}

C_BaseEntity* C_BaseEntity::from_index( int index )
{
	return reinterpret_cast< C_BaseEntity* >( globals::get_player( index ) );
}
C_BaseEntity* C_BaseEntity::from_handle( CBaseHandle h )
{
	return reinterpret_cast< C_BaseEntity* >( interfaces::entity_list()->get_client_entity_from_handle( h ) );
}

void C_AnimationLayer::set_layer_weight_rate( const float delta, const float previous )
{
	if ( delta == 0.f )
		return;

	m_flWeightDeltaRate = ( m_flWeight - previous ) / delta;
}

static float smoothstep_bounds( float edge0, float edge1, float x )
{
	x = clamp( ( x - edge0 ) / ( edge1 - edge0 ), 0.f, 1.f );
	return x * x * ( 3 - 2 * x );
}

void C_AnimationLayer::set_layer_ideal_weight_from_sequence_cycle( CStudioHdr* hdr )
{
	const auto seqdesc = hdr->get_seq_desc( m_nSequence );

	if ( !seqdesc )
	{
		m_flWeight = 0.f;
		return;
	}

	auto ideal = 1.f;
	auto cur_cycle = m_flCycle;
	if ( cur_cycle >= .999f )
		cur_cycle = 1.f;

	if ( seqdesc->fadeintime > 0.f && cur_cycle < seqdesc->fadeintime )
		ideal = smoothstep_bounds( 0.f, seqdesc->fadeintime, cur_cycle );
	else if ( seqdesc->fadeouttime < 1.f && cur_cycle > seqdesc->fadeouttime )
		ideal = smoothstep_bounds( 1.f, seqdesc->fadeouttime, cur_cycle );

	if ( ideal < .0015f )
		ideal = 0.f;

	m_flWeight = clamp( ideal, 0.f, 1.f );
}

void C_AnimationLayer::increment_layer_cycle( const float delta, const bool loop )
{
	if ( fabs( m_flPlaybackRate ) <= 0.f )
		return;

	auto cur_cycle = m_flCycle;
	cur_cycle += delta * m_flPlaybackRate;

	if ( !loop && cur_cycle >= 1.f )
		cur_cycle = .999f;

	cur_cycle -= int( cur_cycle );

	if ( cur_cycle < 0.f )
		cur_cycle += 1.f;
	else if ( cur_cycle > 1.f )
		cur_cycle -= 1.f;

	m_flCycle = cur_cycle;
}

void C_AnimationLayer::increment_layer_cycle_weight_rate_generic( const float delta, CStudioHdr* hdr )
{
	const auto old_weight = m_flWeight;
	increment_layer_cycle( delta, false );
	set_layer_ideal_weight_from_sequence_cycle( hdr );
	set_layer_weight_rate( delta, old_weight );
}

bool C_AnimationLayer::has_animation_completed( const float delta ) const
{
	return m_flCycle + m_flPlaybackRate * delta >= 0.999f;
}

bool C_AnimationLayer::has_sequence_completed( const float delta ) const
{
	return m_flCycle + m_flPlaybackRate * delta >= 1.f;
}

void ConVar::enforce_sent_value( bool& current, bool val )
{
	if ( current == val )
		return;

	char new_str[] = { val ? '1' : '0', '\0' };
	const auto bak = value.string;
	orig_value.string = value.string = new_str;
	hook::convar_network_change_callback( reinterpret_cast< ConVar* >( reinterpret_cast< uintptr_t >( this ) + 0x18 ), nullptr, NAN );
	orig_value.string = value.string = bak;
	current = val;
}

int IClientState::get_last_server_tick()
{
	return time_to_ticks( interfaces::engine()->GetLastTimeStamp() );
}

void CGameMovement::air_accelerate( Vector const& wishdir, float wishspeed, CMoveData* mv, float friction )
{
	const auto wishspd = std::min( wishspeed, 30.f );

	auto const currentspeed = mv->m_vecVelocity.Dot( wishdir );
	auto const addspeed = wishspd - currentspeed;

	if ( addspeed <= 0 )
		return;

	var( sv_airaccelerate );
	auto accelspeed = sv_airaccelerate->get_float() * wishspeed * interfaces::globals()->interval_per_tick * friction;

	if ( accelspeed > addspeed )
		accelspeed = addspeed;

	mv->m_vecVelocity += wishdir * accelspeed;
}

void CGameMovement::air_move( CMoveData* mv, C_CSPlayer* ent )
{
	Vector forward, right, up;
	math::angle_vectors( mv->m_vecViewAngles, &forward, &right, &up ); // Determine movement angles

	// Copy movement amounts
	auto const fmove = mv->m_flForwardMove;
	auto const smove = mv->m_flSideMove;

	forward.z = 0;
	right.z = 0;
	forward.NormalizeInPlace();
	right.NormalizeInPlace();

	Vector wishvel( forward.x * fmove + right.x * smove, forward.y * fmove + right.y * smove, 0.f );

	auto wishdir = wishvel;
	auto wishspeed = VectorNormalize( wishdir );

	// clamp to server defined max speed
	if ( wishspeed != 0 && ( wishspeed > mv->m_flMaxSpeed ) )
	{
		wishvel = wishvel * ( mv->m_flMaxSpeed / wishspeed );
		wishspeed = mv->m_flMaxSpeed;
	}

	air_accelerate( wishdir, wishspeed, mv, ent->get_surface_friction() );
}

void CGameMovement::walk_move( CMoveData* mv, C_CSPlayer* ent )
{
	Vector forward, right, up;
	math::angle_vectors( mv->m_vecViewAngles, &forward, &right, &up ); // Determine movement angles

	// Copy movement amounts
	auto const fmove = mv->m_flForwardMove;
	auto const smove = mv->m_flSideMove;

	forward.z = right.z = 0.f;

	VectorNormalize( forward );
	VectorNormalize( right );

	Vector wishvel( forward.x * fmove + right.x * smove, forward.y * fmove + right.y * smove, 0.f );

	auto wishdir = wishvel;
	auto wishspeed = VectorNormalize( wishdir );

	// clamp to server defined max speed
	if ( wishspeed != 0 && ( wishspeed > mv->m_flMaxSpeed ) )
	{
		wishvel = wishvel * ( mv->m_flMaxSpeed / wishspeed );
		wishspeed = mv->m_flMaxSpeed;
	}

	var( sv_accelerate );

	// Set pmove velocity
	mv->m_vecVelocity[ 2 ] = 0;
	accelerate( wishdir, wishspeed, sv_accelerate->get_float(), mv, ent );
	mv->m_vecVelocity[ 2 ] = 0;

	// Additional max speed clamp to keep us from going faster than allowed while turning
	if ( mv->m_vecVelocity.LengthSqr() > mv->m_flMaxSpeed * mv->m_flMaxSpeed )
	{
		float fRatio = mv->m_flMaxSpeed / mv->m_vecVelocity.Length();
		mv->m_vecVelocity *= fRatio;
	}

	// If we made it all the way, then copy trace end as new player position.
	mv->m_outWishVel += wishdir * wishspeed;
}

void CGameMovement::accelerate( const Vector& wishdir, float wishspeed, float accel, CMoveData* mv, C_CSPlayer* ent )
{
	auto stored_accel = accel;
	auto currentspeed = mv->m_vecVelocity.Dot( wishdir );
	const float addspeed = wishspeed - currentspeed;

	if ( addspeed <= 0 )
		return;

	if ( currentspeed < 0 )
		currentspeed = 0;

	const auto is_ducking = mv->m_nButtons & IN_DUCK || ent->get_ducking() || ent->get_flags() & FL_DUCKING;
	const auto is_walking = ( mv->m_nButtons & IN_SPEED ) != 0 && !is_ducking;

	constexpr auto max_speed = 250.0f;
	auto acceleration_scale = std::max( max_speed, wishspeed );
	auto goal_speed = acceleration_scale;

	auto is_slow_sniper_scoped = false;

	var( sv_accelerate_use_weapon_speed );

	const auto wpn = globals::get_weapon( ent->get_active_weapon() );
	if ( sv_accelerate_use_weapon_speed->get_bool() && wpn )
	{
		is_slow_sniper_scoped = ( wpn->get_zoom_level() > 0 && wpn->get_wpn_data()->zoom_levels > 1 && ( wpn->get_max_speed() * 0.52f ) < 110.0 );

		goal_speed *= std::min( 1.0f, ( wpn->get_max_speed() / max_speed ) );

		if ( ( !is_ducking && !is_walking ) || ( ( is_walking || is_ducking ) && is_slow_sniper_scoped ) )
			acceleration_scale *= std::min( 1.0f, ( wpn->get_max_speed() / max_speed ) );
	}

	if ( is_ducking )
	{
		if ( !is_slow_sniper_scoped )
			acceleration_scale *= 0.34f;

		goal_speed *= 0.34f;
	}

	if ( is_walking )
	{
		if ( !is_slow_sniper_scoped )
			acceleration_scale *= 0.52f;

		goal_speed *= 0.52f;
	}

	if ( is_walking && currentspeed > ( goal_speed - 5 ) )
		stored_accel *= clamp( 1.0f - ( std::max( 0.0f, currentspeed - ( goal_speed - 5 ) ) / std::max( 0.0f, goal_speed - ( goal_speed - 5 ) ) ), 0.0f, 1.0f );

	auto accelspeed = stored_accel * interfaces::globals()->interval_per_tick * acceleration_scale * ent->get_surface_friction();

	if ( accelspeed > addspeed )
		accelspeed = addspeed;

	mv->m_vecVelocity += ( wishdir * accelspeed );
}

void CGameMovement::friction( CMoveData* mv, C_CSPlayer* player )
{
	// Calculate speed
	const auto speed = mv->m_vecVelocity.Length();

	// If too slow, return
	if ( speed < 0.1f )
		return;

	auto drop = 0.f;

	// apply ground friction
	if ( player->get_ground_entity() != 0 )  // On an entity that is the ground
	{
		var( sv_friction );
		const auto friction = sv_friction->get_float() * player->get_surface_friction();

		// Bleed off some speed, but if we have less than the bleed
		//  threshold, bleed the threshold amount.
		var( sv_stopspeed );
		const auto control = ( speed < sv_stopspeed->get_float() ) ? sv_stopspeed->get_float() : speed;

		// Add the amount to the drop amount.
		drop += control * friction * interfaces::globals()->interval_per_tick;
	}

	// scale the velocity
	auto newspeed = speed - drop;
	if ( newspeed < 0 )
		newspeed = 0;

	if ( newspeed != speed )
	{
		// Determine proportion of old speed we are using.
		newspeed /= speed;
		// Adjust velocity according to proportion.
		mv->m_vecVelocity *= newspeed;
	}

	mv->m_outWishVel -= mv->m_vecVelocity * ( 1.f - newspeed );
}

void CGameMovement::check_parameters( CMoveData* mv, C_CSPlayer* player )
{
	const auto speed_squared = ( mv->m_flForwardMove * mv->m_flForwardMove ) +
		( mv->m_flSideMove * mv->m_flSideMove ) +
		( mv->m_flUpMove * mv->m_flUpMove );


	// Slow down by the speed factor
	auto speed_factor = 1.0f;
	if ( player->get_surface_props() )
		speed_factor = player->get_surface_props()->game.maxspeedfactor;

	const auto weapon = globals::get_weapon( player->get_active_weapon() );
	mv->m_flMaxSpeed = weapon ? weapon->get_max_speed() : mv->m_flClientMaxSpeed;

	mv->m_flMaxSpeed *= speed_factor;

	// stamina slowing factor
	if ( player->get_stamina() > 0 )
	{
		auto speed_scale = clamp( 1.0f - player->get_stamina() / 100.f, 0.f, 1.f );
		speed_scale *= speed_scale;	// square the scale factor so that it correlates more closely with the jump penalty (i.e. a 50% stamina jumps .25 * normal height)
		mv->m_flMaxSpeed *= speed_scale;
	}

	// Same thing but only do the sqrt if we have to.
	if ( ( speed_squared != 0.0 ) && ( speed_squared > mv->m_flMaxSpeed * mv->m_flMaxSpeed ) )
	{
		const auto ratio = mv->m_flMaxSpeed / sqrt( speed_squared );
		mv->m_flForwardMove *= ratio;
		mv->m_flSideMove *= ratio;
		mv->m_flUpMove *= ratio;
	}
}