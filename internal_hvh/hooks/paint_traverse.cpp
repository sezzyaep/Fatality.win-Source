#include "../include_cheat.h"

void __fastcall hook::paint_traverse( void* pPanels, int edx, unsigned int panel, bool forceRepaint, bool allowForce )
{
	static unsigned int scope_panel;
	if ( !scope_panel )
	{
		const auto panelname = interfaces::panel()->GetName( panel );

		_( HudZoom, "HudZoom" );
		if ( !strcmp( panelname, HudZoom.c_str() ) )
			scope_panel = panel;
	}

	if ( panel == scope_panel && ( vars::misc.remove_scope->get<int>() == 1 || vars::misc.remove_scope->get<int>() == 3 || vars::misc.remove_scope->get<int>() == 4 ) )
		return;

	original( paint_traverse )( pPanels, edx, panel, forceRepaint, allowForce );

	static unsigned int draw_panel;
	if ( !draw_panel )
	{
		const auto panelname = interfaces::panel()->GetName( panel );

		_( FocusOverlayPanel, "FocusOverlayPanel" );
		if ( !strcmp( panelname, FocusOverlayPanel.c_str() ) )
			draw_panel = panel;
	}

	if ( panel != draw_panel )
		return;

	lua::api.callback( FNV1A( "on_paint_traverse" ) );
}