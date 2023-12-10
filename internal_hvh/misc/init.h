#pragma once

#include <vector>

namespace init
{
	void on_startup();
	void get_modules();
	void load_fonts();
	void remove_fonts();
	void get_materials();
	void patches();
	void save_resources();
	void init_pred_map();
#ifndef RELEASE
	void unload_cheat();
#endif

	inline std::vector<HANDLE> mem_font_hadles = {};
	inline HWND window = {};
}