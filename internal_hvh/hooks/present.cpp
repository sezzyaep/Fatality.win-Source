#include "../include_cheat.h"
#include <ren/adapter_dx9.h>
#include <ren/bitfont.h>
#include "../josefin_sans.h"

using namespace evo::ren;

long __stdcall hook::present( IDirect3DDevice9* device, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion )
{
	g_input->run();

	if ( !adapter )
	{
		adapter = std::make_shared<adapter_dx9>( device, init::window );

		char win_dir_arr[ 256 ]{};
		GetSystemWindowsDirectoryA( win_dir_arr, 256 );

		std::string win_dir{ win_dir_arr };
		draw.manage( FNV1A( "esp" ), std::make_shared<::gui::bitfont>( xorstr_( "Smallest Pixel-7" ), 11.f, 400, ff_antialias | ff_outline, 0, 0x45F ) );
		draw.manage( FNV1A( "esp_name" ), std::make_shared<font>( win_dir + xorstr_( "/fonts/segoeuib.ttf" ), 13.f, ff_shadow, 0, 0x45F ) );
		draw.manage( FNV1A( "title_font" ), std::make_shared<font>( josefin_sans.data(), josefin_sans.size(), 11.f, 0, 0, 0x80 ) );
		draw.manage( FNV1A( "control" ), std::make_shared<font>( josefin_sans.data(), josefin_sans.size(), 11.57f, 0, 0, 0x80 ) );
		draw.manage( FNV1A( "child_title" ), std::make_shared<font>( josefin_sans.data(), josefin_sans.size(), 11.57f, 0, 0, 0x80 ) ); // 600
		draw.manage( FNV1A( "tab_font" ), std::make_shared<font>( josefin_sans.data(), josefin_sans.size(), 15.5f, 0, 0, 0x80 ) ); // 600
		draw.manage( FNV1A( "glitch_font" ), std::make_shared<font>( win_dir + xorstr_( "/fonts/verdanab.ttf" ), 25.f, 0, 0, 0x80 ) );
		draw.manage( FNV1A( "lby" ), std::make_shared<font>( win_dir + xorstr_( "/fonts/verdanab.ttf" ), 27.f, ff_shadow, 0, 0x80 ) );
		draw.manage( FNV1A( "control_unicode" ), std::make_shared<font>( win_dir + xorstr_( "/fonts/seguisb.ttf" ), 14.6f, 0, 0, 0x45F ) );

		adapter->create_objects();
	}

	draw.set_frame_time( interfaces::globals()->frametime );
	adapter->refresh();

	dx_adapter::mtx.lock();
	if ( dx_adapter::ready )
	{
		adapter->flush();
		dx_adapter::swap_buffers();
		dx_adapter::ready = false;
	}
	dx_adapter::mtx.unlock();

	g_menu->run();

	adapter->render();
	adapter->flush();

	return original( present )( device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );
}

