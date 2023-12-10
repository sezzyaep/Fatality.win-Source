#include "../include_cheat.h"

void __fastcall hook::render_view( void* thisptr, void* edx, CViewSetup& setup, CViewSetup& hudViewSetup, unsigned int nClearFlags, uint8_t whatToDraw )
{
	aimbot_helpers::no_visual_recoil( setup );

	beams::draw_impacts();

	var( r_aspectratio );
	r_aspectratio->set_value( vars::misc.aspect_ratio->get<float>() / 100.f );

	if ( vars::misc.vm_fov_enabled->get<bool>() && local_player && ( !local_player->get_scoped() || local_weapon && local_weapon->get_weapon_type() == WEAPONTYPE_SNIPER_RIFLE ) )
		setup.fovViewmodel = vars::misc.vm_fov->get<float>();

	if ( vars::misc.remove_scope->get<bool>() )
		reinterpret_cast< float* >( make_offset( "client.dll", sig_post_process_params ) )[ 5 ] = 0.f;

	var( mat_software_aa_strength );
	const auto backup_aa_strength = mat_software_aa_strength->get_float();
	mat_software_aa_strength->set_float( 0.f );

	*reinterpret_cast< bool* >( make_offset( "client.dll", sig_disable_post_processing ) ) = vars::visuals.disable_postprocessing->get<bool>();
	original( render_view )( thisptr, edx, setup, hudViewSetup, nClearFlags, whatToDraw );

	mat_software_aa_strength->set_float( backup_aa_strength );
}
