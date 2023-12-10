#include "../include_cheat.h"

bool bone_setup::handle_bone_setup( lag_record_t& record, resolver_direction dir, const bool lean, const bool extrapolated )
{
	const auto player = globals::get_player( record.m_index );
	if ( !player )
		__debugbreak();

	const auto backup_layers = player->get_anim_layers();
	const auto backup_anim_state = *player->get_anim_state();
	const auto backup_poses = player->get_pose_params();
	const auto backup_abs_origin = player->get_abs_origin();

	player->get_anim_layers() = extrapolated ? record.m_state[ dir ].m_own_layers : record.m_layers;
	player->get_pose_params() = record.m_state[ dir ].m_poses;
	player->set_abs_origin( record.m_origin );

	const auto ret = perform_bone_setup( record, dir, lean );

	 // restore everything.
	player->get_anim_layers() = backup_layers;
	*player->get_anim_state() = backup_anim_state;
	player->get_pose_params() = backup_poses;
	player->set_abs_origin( backup_abs_origin );

	return ret;
}

bool bone_setup::perform_bone_setup( lag_record_t& record, const resolver_direction dir, const bool lean )
{
	const auto player = globals::get_player( record.m_index );
	if ( !player )
		return false;

	const auto hdr = player->get_model_ptr();
	if ( !hdr )
		return false;

	auto& log = player_log::get_log( record.m_index );

	const auto fresh = record.m_state[ dir ].m_setup_tick == -1;

	record.m_state[ dir ].m_setup_tick = interfaces::globals()->tickcount;

	auto& layers = player->get_anim_layers();

	// keep track of old values.
	const auto backup_effects = player->get_effects();
	const auto backup_flags = player->get_lod_data().flags;
	const auto backup_ik = player->get_ik_context();
	const auto backup_layer = layers[ 12 ].m_flWeight;
	const auto backup_abs_angles = player->get_abs_angles();
	const auto backup_eye_angle = *player->eye_angles_virtual();
	const auto backup_view_offset = player->get_view_offset();
	const auto backup_collision_viewheight = player->get_collision_viewheight();
	const auto backup_collision_bounds_change_time = player->get_collision_bounds_change_time();

	auto& out = record.m_state[ dir ].m_pristine_matrix;

	if ( fresh )
	{
		// stop interpolation, snapshots and other shit.
		player->get_effects() |= EF_NOINTERP;
		player->get_lod_data().flags = 0;
		player->get_snapshot_front().player = player->get_snapshot_back().player = nullptr;
		if ( !lean )
			layers[ 12 ].m_flWeight = 0.f;

		// setup temporary storage for inverse kinematics.
		ik_context ik;
		uint8_t computed[ 0x100 ]{};
		auto angle = record.m_state[ dir ].m_abs_ang;
		ik.init( hdr, &angle, &record.m_origin, record.m_simtime, interfaces::globals()->framecount, BONE_USED_BY_ANYTHING );

		// perform bone blending.
		alignas( 16 ) Vector pos[ 128 ] = {};
		alignas( 16 ) QuaternionAligned q[ 128 ] = {};
		player->get_ik_context() = &ik;
		player->standard_blending_rules( hdr, pos, q, record.m_simtime, BONE_USED_BY_ANYTHING );
		player->update_ik_locks( record.m_simtime );
		ik.update_targets( pos, q, out, computed );
		player->calculate_ik_locks( record.m_simtime );
		ik.solve_dependencies( pos, q, out, computed );

		// build chain.
		int32_t chain[ 128 ] = {};
		const auto chain_length = hdr->numbones();
		for ( auto i = 0; i < chain_length; i++ )
			chain[ chain_length - i - 1 ] = i;

		// build transformations.
		const auto rotation = math::angle_matrix( angle, record.m_origin );
		for ( auto j = chain_length - 1; j >= 0; j-- )
		{
			const auto i = chain[ j ];
			const auto parent = hdr->m_boneParent.Count() > i ? &hdr->m_boneParent[ i ] : nullptr;

			if ( !parent )
				continue;

			const auto qua = math::quaternion_matrix( q[ i ], pos[ i ] );

			if ( *parent == -1 )
				out[ i ] = math::concat_transforms( rotation, qua );
			else
				out[ i ] = math::concat_transforms( out[ *parent ], qua );
		}
	}

	// post transformation setup.
	if ( !player->get_predictable() )
	{
		if ( const auto eye_watcher = player->get_var_mapping().find( FNV1A( "C_CSPlayer::m_iv_angEyeAngles" ) ) )
			eye_watcher->RestoreToLastNetworked();
		if ( const auto viewoffset_watcher = player->get_var_mapping().find( FNV1A( "C_BasePlayer::m_iv_vecViewOffset" ) ) )
			viewoffset_watcher->RestoreToLastNetworked();
	}

	player->set_abs_angles( record.m_state[ dir ].m_abs_ang );

	memcpy( record.m_state[ dir ].m_matrix, out, sizeof( out ) );

	auto networked_eye = *player->eye_angles_virtual();
	networked_eye.x = clamp( math::normalize_float( networked_eye.x ), -90.f, 90.f );
	auto last_eye_angle = log.m_last_eye_angle;

	record.m_pitch_jitter = fabsf( last_eye_angle.x - networked_eye.x ) > 10.f && !record.m_shot; // intended shot check
	if ( record.m_pitch_jitter && player->EntIndex() != interfaces::engine()->GetLocalPlayer() )
	{
		const auto forward = ( record.m_origin - local_player->get_origin() ).Normalized();
		QAngle ang;
		math::vector_angles( forward, ang );
		networked_eye.y = math::normalize_float( ang.y + 130.f );
		last_eye_angle.y = math::normalize_float( ang.y - 130.f );
	}

	record.m_last_ang_differs = fabsf( math::normalize_float( last_eye_angle.y - math::normalize_float( networked_eye.y ) ) ) > 2.f;

	if ( dir == resolver_direction::resolver_min )
	{
		memcpy( record.m_state[ resolver_direction::resolver_min_min ].m_matrix, out, sizeof( out ) );
		player->eye_angles_virtual()->z = lag_record_t::get_resolver_roll( resolver_direction::resolver_min_min );
		player->post_build_transformations( record.m_state[ resolver_direction::resolver_min_min ].m_matrix, BONE_USED_BY_ANYTHING );

		memcpy( record.m_state[ resolver_direction::resolver_min_extra ].m_matrix, out, sizeof( out ) );
		player->eye_angles_virtual()->z = lag_record_t::get_resolver_roll( resolver_direction::resolver_min_extra );
		player->post_build_transformations( record.m_state[ resolver_direction::resolver_min_extra ].m_matrix, BONE_USED_BY_ANYTHING );

		if ( record.m_last_ang_differs )
		{
			memcpy( record.m_state[ resolver_direction::resolver_min_extra ].m_extra_matrix, out, sizeof( out ) );
			*player->eye_angles_virtual() = last_eye_angle;
			player->eye_angles_virtual()->z = lag_record_t::get_resolver_roll( resolver_direction::resolver_min_extra );
			player->post_build_transformations( record.m_state[ resolver_direction::resolver_min_extra ].m_extra_matrix, BONE_USED_BY_ANYTHING );

			memcpy( record.m_state[ resolver_direction::resolver_min_min ].m_extra_matrix, out, sizeof( out ) );
			*player->eye_angles_virtual() = last_eye_angle;
			player->eye_angles_virtual()->z = lag_record_t::get_resolver_roll( resolver_direction::resolver_min_min );
			player->post_build_transformations( record.m_state[ resolver_direction::resolver_min_min ].m_extra_matrix, BONE_USED_BY_ANYTHING );
		}
	}
	if ( dir == resolver_direction::resolver_max )
	{
		memcpy( record.m_state[ resolver_direction::resolver_max_max ].m_matrix, out, sizeof( out ) );
		player->eye_angles_virtual()->z = lag_record_t::get_resolver_roll( resolver_direction::resolver_max_max );
		player->post_build_transformations( record.m_state[ resolver_direction::resolver_max_max ].m_matrix, BONE_USED_BY_ANYTHING );

		memcpy( record.m_state[ resolver_direction::resolver_max_extra ].m_matrix, out, sizeof( out ) );
		player->eye_angles_virtual()->z = lag_record_t::get_resolver_roll( resolver_direction::resolver_max_extra );
		player->post_build_transformations( record.m_state[ resolver_direction::resolver_max_extra ].m_matrix, BONE_USED_BY_ANYTHING );

		if ( record.m_last_ang_differs )
		{
			memcpy( record.m_state[ resolver_direction::resolver_max_extra ].m_extra_matrix, out, sizeof( out ) );
			*player->eye_angles_virtual() = last_eye_angle;
			player->eye_angles_virtual()->z = lag_record_t::get_resolver_roll( resolver_direction::resolver_max_extra );
			player->post_build_transformations( record.m_state[ resolver_direction::resolver_max_extra ].m_extra_matrix, BONE_USED_BY_ANYTHING );

			memcpy( record.m_state[ resolver_direction::resolver_max_max ].m_extra_matrix, out, sizeof( out ) );
			*player->eye_angles_virtual() = last_eye_angle;
			player->eye_angles_virtual()->z = lag_record_t::get_resolver_roll( resolver_direction::resolver_max_max );
			player->post_build_transformations( record.m_state[ resolver_direction::resolver_max_max ].m_extra_matrix, BONE_USED_BY_ANYTHING );
		}
	}

	if ( player != local_player )
	{
		if ( record.m_last_ang_differs )
		{
			memcpy( record.m_state[ dir ].m_extra_matrix, out, sizeof( out ) );
			*player->eye_angles_virtual() = last_eye_angle;
			player->eye_angles_virtual()->z = lag_record_t::get_resolver_roll( dir );
			player->post_build_transformations( record.m_state[ dir ].m_extra_matrix, BONE_USED_BY_ANYTHING );
		}

		*player->eye_angles_virtual() = networked_eye;
		player->eye_angles_virtual()->z = lag_record_t::get_resolver_roll( dir );
	}

	player->post_build_transformations( record.m_state[ dir ].m_matrix, BONE_USED_BY_ANYTHING );


	// restore original values.
	player->set_abs_angles( backup_abs_angles );
	*player->eye_angles_virtual() = backup_eye_angle;
	player->get_effects() = backup_effects;
	player->get_lod_data().flags = backup_flags;
	player->get_ik_context() = backup_ik;
	player->get_view_offset() = backup_view_offset;
	player->get_collision_viewheight() = backup_collision_viewheight;
	player->get_collision_bounds_change_time() = backup_collision_bounds_change_time;
	layers[ 12 ].m_flWeight = backup_layer;

	return true;
}