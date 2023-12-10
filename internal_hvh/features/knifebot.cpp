#include "../include_cheat.h"

std::mutex knife_trace_mutex;

bool knifebot::trace_knife( const Vector spot, lag_record_t* record, bool& retstab )
{
	std::lock_guard l( knife_trace_mutex );

	// Attack range of knife attack
	const auto stab = rules_knife( record );
	const auto range = stab ? 32.0f : 48.0f;

	const auto player = globals::get_player( record->m_index );

	const auto collideable = player->GetCollideable();

	auto backup_origin = player->get_origin();
	auto backup_absorigin = player->get_abs_origin();
	auto backup_mins = collideable->OBBMins();
	auto backup_maxs = collideable->OBBMaxs();

	player->set_abs_origin( record->m_origin, true );
	player->get_origin() = record->m_origin;

	collideable->OBBMins() = record->m_obb_mins;
	collideable->OBBMaxs() = record->m_obb_maxs;

	Ray_t ray;
	trace_t tr;

	const auto eyepos = local_player->get_eye_pos();

	// Calculte end spot
	Vector vec_aim{};
	const auto angle = math::calc_angle( eyepos, spot );
	math::angle_vectors( angle, &vec_aim );
	const auto end = eyepos + vec_aim * range;

	// Trace attack
	ray.Init( eyepos, end );
	CTraceFilterSimple filter( local_player );
	interfaces::engine_trace()->TraceRay( ray, MASK_SOLID, &filter, &tr );

	// If that failed, try a hull trace
	if ( tr.fraction >= 1.0f )
	{
		static const Vector hull[] = { Vector( -16.0f, -16.0f, -18.0f ), Vector( 16.0f, 16.0f, 18.0f ) };
		ray.Init( eyepos, end, hull[ 0 ], hull[ 1 ] );
		CTraceFilterSimple filter( local_player );
		interfaces::engine_trace()->TraceRay( ray, MASK_SOLID, &filter, &tr );
	}

	player->get_origin() = backup_origin;
	player->set_abs_origin( backup_absorigin, true );
	player->get_eflags() |= EFL_DIRTY_ABSTRANSFORM;

	collideable->OBBMins() = backup_mins;
	collideable->OBBMaxs() = backup_maxs;

	if ( reinterpret_cast< C_CSPlayer* >( tr.m_pEnt ) != player )
		return false;

	retstab = stab;
	return true;
}

bool knifebot::rules_knife( lag_record_t* record )
{
	const auto player = globals::get_player( record->m_index );
	C_BaseCombatWeapon* weapon = local_weapon;

	//------------------------------------------------
	// Swing and stab damage

	struct table_t
	{
		// [first][armor][back]
		unsigned char swing[ 2 ][ 2 ][ 2 ];
		// [armor][back]
		unsigned char stab[ 2 ][ 2 ];
	};
	static const table_t table = { { { { 25, 90 }, { 21, 76 } }, { { 40, 90 }, { 34, 76 } } }, { { 65, 180 }, { 55, 153 } } };

	const auto armor = ( player->get_armor() > 0 );
	const auto first = weapon->get_next_primary_attack() + 0.4f < interfaces::globals()->curtime;
	const auto back = is_behind( record );

	const auto stab_dmg = table.stab[ armor ][ back ];
	const auto slash_dmg = table.swing[ false ][ armor ][ back ];
	const auto swing_dmg = table.swing[ first ][ armor ][ back ];

	//------------------------------------------------
	// Select desired attack

	const auto health = player->get_predicted_health();
	bool stab;

	// IF health lower than swing_dmg, do a swing
	if ( health <= swing_dmg ) stab = false;
	// IF health lower than stab_dmg, do a stab
	else if ( health <= stab_dmg ) stab = true;
	// IF health more than swing+swing+stab, do a stab
	else if ( health > ( swing_dmg + slash_dmg + stab_dmg ) ) stab = true;
	// ELSE swing (initiate swing+swing+stab)
	else stab = false;

	//------------------------------------------------
	// Range and attack mode

	return stab;
}

bool knifebot::is_behind( const lag_record_t* record )
{
	auto vec_los = record->m_origin - local_player->get_origin();
	vec_los.z = 0.0f;
	vec_los.NormalizeInPlace();

	Vector forward{};
	math::angle_vectors( record->m_abs_ang, &forward );
	forward.z = 0.0f;

	return vec_los.Dot( forward ) > 0.475;
}

float knifebot::get_damage( const lag_record_t* record, const bool stab )
{
	const auto player = globals::get_player( record->m_index );
	C_BaseCombatWeapon* weapon = local_weapon;

	//------------------------------------------------
	// Swing and stab damage

	struct table_t
	{
		// [first][armor][back]
		unsigned char swing[ 2 ][ 2 ][ 2 ];
		// [armor][back]
		unsigned char stab[ 2 ][ 2 ];
	};
	static constexpr table_t table = { { { { 25, 90 }, { 21, 76 } }, { { 40, 90 }, { 34, 76 } } }, { { 65, 180 }, { 55, 153 } } };

	const auto armor = ( player->get_armor() > 0 );
	const auto first = weapon->get_next_primary_attack() + 0.4f < interfaces::globals()->curtime;
	const auto back = is_behind( record );

	const auto stab_dmg = table.stab[ armor ][ back ];
	const auto swing_dmg = table.swing[ first ][ armor ][ back ];

	return stab ? stab_dmg : swing_dmg;
}