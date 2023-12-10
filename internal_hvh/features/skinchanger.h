#pragma once

namespace skinchanger
{
	void on_level_shudown();
	void spawn_glove( ClientFrameStage_t stage );
	void apply_glove();
	void update_hud();
	void run( ClientFrameStage_t stage );
	void model_changer();
	void parse_kits();
	void parse_weapon_names();
	void on_addon_bits( C_CSPlayer* player, int32_t& bits );

	struct paint_kit_data_s
	{
		auto operator ==( const paint_kit_data_s& rhs ) const
		{
			return this->paint_kit == rhs.paint_kit;
		}

		int32_t paint_kit = -1;
		int32_t rarity{};
		std::string name{};
	};

	struct weapon_skin_info
	{
		int32_t index = 0;
		std::string name{};
		std::string item_name{};
		std::deque<paint_kit_data_s> paint_kits{};
	};

	struct model_info
	{
		std::string definition_name{};
		std::string base_display_model{};
		std::string world_display_model{};
	};

	paint_kit_data_s find_paintkit( int32_t paint_kit );

	inline std::vector<int32_t> glove_def_indices{};
	inline std::map<int32_t, model_info> models{};
	inline std::map<int32_t, weapon_skin_info> weapon_infos{};

	//inline std::vector<Kit_t> k_skins = {};
	//inline std::vector<std::string> k_pure_skin_names = {};
	//inline std::vector<size_t> k_skin_name_to_kit = {};
	//inline std::vector<std::string> k_pure_weapon_names = {};
	//inline std::vector<size_t> k_idx_to_defidex = {};
	inline std::unordered_map<int, int> original_activity = {};
	inline uintptr_t glove_handle = {};
	inline int server_model_idx = -1;

	inline const tsl::robin_map<size_t, std::string> player_model_list =
	{
		// leet
		{ 1, _w( "models/player/custom_player/legacy/tm_leet_variantf.mdl" ) },
	{ 2, _w( "models/player/custom_player/legacy/tm_leet_varianti.mdl" ) },
	{ 3, _w( "models/player/custom_player/legacy/tm_leet_varianth.mdl" ) },
	{ 4, _w( "models/player/custom_player/legacy/tm_leet_variantg.mdl" ) },
	{ 5, _w( "models/player/custom_player/legacy/tm_leet_variantj.mdl" ) }, // Mr. Muhlik | Elite Crew

		// fbi
	{ 6, _w( "models/player/custom_player/legacy/ctm_fbi_variantb.mdl" ) },
	{ 7, _w( "models/player/custom_player/legacy/ctm_fbi_varianth.mdl" ) },
	{ 8, _w( "models/player/custom_player/legacy/ctm_fbi_variantg.mdl" ) },
	{ 9, _w( "models/player/custom_player/legacy/ctm_fbi_variantf.mdl" ) },

		// st6
	{ 10, _w( "models/player/custom_player/legacy/ctm_st6_variante.mdl" ) },
	{ 11, _w( "models/player/custom_player/legacy/ctm_st6_variantm.mdl" ) },
	{ 12, _w( "models/player/custom_player/legacy/ctm_st6_variantg.mdl" ) },
	{ 13, _w( "models/player/custom_player/legacy/ctm_st6_variantk.mdl" ) },
	{ 14, _w( "models/player/custom_player/legacy/ctm_st6_varianti.mdl" ) },
	{ 15, _w( "models/player/custom_player/legacy/ctm_st6_variantj.mdl" ) },
	{ 16, _w( "models/player/custom_player/legacy/ctm_st6_variantl.mdl" ) },
	{ 17, _w( "models/player/custom_player/legacy/ctm_st6_variantn.mdl" ) },

		// swat
	{ 18, _w( "models/player/custom_player/legacy/ctm_swat_variante.mdl" ) },
	{ 19, _w( "models/player/custom_player/legacy/ctm_swat_variantf.mdl" ) },
	{ 20, _w( "models/player/custom_player/legacy/ctm_swat_variantg.mdl" ) },
	{ 21, _w( "models/player/custom_player/legacy/ctm_swat_varianth.mdl" ) },
	{ 22, _w( "models/player/custom_player/legacy/ctm_swat_varianti.mdl" ) },
	{ 23, _w( "models/player/custom_player/legacy/ctm_swat_variantj.mdl" ) },
	{ 24, _w( "models/player/custom_player/legacy/ctm_swat_variantk.mdl" ) },

		// balkan
	{ 25, _w( "models/player/custom_player/legacy/tm_balkan_varianti.mdl" ) },
	{ 26, _w( "models/player/custom_player/legacy/tm_balkan_variantf.mdl" ) },
	{ 27, _w( "models/player/custom_player/legacy/tm_balkan_varianth.mdl" ) },
	{ 28, _w( "models/player/custom_player/legacy/tm_balkan_variantg.mdl" ) },
	{ 29, _w( "models/player/custom_player/legacy/tm_balkan_variantj.mdl" ) },
	{ 30, _w( "models/player/custom_player/legacy/tm_balkan_variantk.mdl" ) },
	{ 31, _w( "models/player/custom_player/legacy/tm_balkan_variantl.mdl" ) },

		// sas
	{ 32, _w( "models/player/custom_player/legacy/ctm_sas_variantg.mdl" ) },
	{ 33, _w( "models/player/custom_player/legacy/ctm_sas_variantf.mdl" ) },

		// phoenix
	{ 34, _w( "models/player/custom_player/legacy/tm_phoenix_varianth.mdl" ) },
	{ 35, _w( "models/player/custom_player/legacy/tm_phoenix_variantf.mdl" ) },
	{ 36, _w( "models/player/custom_player/legacy/tm_phoenix_variantg.mdl" ) },
	{ 37, _w( "models/player/custom_player/legacy/tm_phoenix_varianti.mdl" ) },

		// professional
	{ 38, _w( "models/player/custom_player/legacy/tm_professional_varf.mdl" ) },
	{ 39, _w( "models/player/custom_player/legacy/tm_professional_varf1.mdl" ) },
	{ 40, _w( "models/player/custom_player/legacy/tm_professional_varf2.mdl" ) },
	{ 41, _w( "models/player/custom_player/legacy/tm_professional_varf3.mdl" ) },
	{ 42, _w( "models/player/custom_player/legacy/tm_professional_varf4.mdl" ) },
	{ 43, _w( "models/player/custom_player/legacy/tm_professional_varf5.mdl" ) }, // Bloody Darryl The Strapped | The Professionals
	{ 44, _w( "models/player/custom_player/legacy/tm_professional_varg.mdl" ) },
	{ 45, _w( "models/player/custom_player/legacy/tm_professional_varh.mdl" ) },
	{ 46, _w( "models/player/custom_player/legacy/tm_professional_vari.mdl" ) },
	{ 47, _w( "models/player/custom_player/legacy/tm_professional_varj.mdl" ) },

		// jungle raider
	{ 48, _w( "models/player/custom_player/legacy/tm_jungle_raider_variantf2.mdl" ) }, // Trapper | Guerrilla Warfare
	{ 49, _w( "models/player/custom_player/legacy/tm_jungle_raider_variantf.mdl" ) }, // Trapper Aggressor | Guerrilla Warfare
	{ 50, _w( "models/player/custom_player/legacy/tm_jungle_raider_variante.mdl" ) }, // Vypa Sista of the Revolution | Guerrilla Warfare
	{ 51, _w( "models/player/custom_player/legacy/tm_jungle_raider_variantd.mdl" ) }, // Col. Mangos Dabisi | Guerrilla Warfare
	{ 52, _w( "models/player/custom_player/legacy/tm_jungle_raider_variantc.mdl" ) }, // Arno The Overgrown | Guerrilla Warfare
	{ 53, _w( "models/player/custom_player/legacy/tm_jungle_raider_variantb2.mdl" ) }, // 'Medium Rare' Crasswater | Guerrilla Warfare
	{ 54, _w( "models/player/custom_player/legacy/tm_jungle_raider_variantb.mdl" ) }, // Crasswater The Forgotten | Guerrilla Warfare
	{ 55, _w( "models/player/custom_player/legacy/tm_jungle_raider_varianta.mdl" ) }, // Elite Trapper Solman | Guerrilla Warfare

		// diver
	{ 56, _w( "models/player/custom_player/legacy/ctm_diver_varianta.mdl" ) },
	{ 57, _w( "models/player/custom_player/legacy/ctm_diver_variantb.mdl" ) },
	{ 58, _w( "models/player/custom_player/legacy/ctm_diver_variantc.mdl" ) },

		// gendarmerie
	{ 59, _w( "models/player/custom_player/legacy/ctm_gendarmerie_varianta.mdl" ) },
	{ 60, _w( "models/player/custom_player/legacy/ctm_gendarmerie_variantb.mdl" ) },
	{ 61, _w( "models/player/custom_player/legacy/ctm_gendarmerie_variantc.mdl" ) },
	{ 62, _w( "models/player/custom_player/legacy/ctm_gendarmerie_variantd.mdl" ) },
	{ 63, _w( "models/player/custom_player/legacy/ctm_gendarmerie_variante.mdl" ) },

	{ 64, _w( "models/player/custom_player/legacy/tm_jumpsuit_varianta.mdl" ) },
	{ 65, _w( "models/player/custom_player/legacy/tm_jumpsuit_variantb.mdl" ) },
	{ 66, _w( "models/player/custom_player/legacy/tm_jumpsuit_variantc.mdl" ) },


	{ 67, _w( "models/player/custom_player/legacy/tm_pirate.mdl" ) },
	{ 68, _w( "models/player/custom_player/legacy/tm_pirate_varianta.mdl" ) },
	{ 69, _w( "models/player/custom_player/legacy/tm_pirate_variantb.mdl" ) },
	{ 70, _w( "models/player/custom_player/legacy/tm_pirate_variantc.mdl" ) },
	{ 71, _w( "models/player/custom_player/legacy/tm_pirate_variantd.mdl" ) },
	{ 72, _w( "models/player/custom_player/legacy/tm_separatist.mdl" ) },
	{ 73, _w( "models/player/custom_player/legacy/tm_separatist_varianta.mdl" ) },
	{ 74, _w( "models/player/custom_player/legacy/tm_separatist_variantb.mdl" ) },
	{ 75, _w( "models/player/custom_player/legacy/tm_separatist_variantc.mdl" ) },
	{ 76, _w( "models/player/custom_player/legacy/tm_separatist_variantd.mdl" ) },
	{ 77, _w( "models/player/custom_player/legacy/tm_phoenix_heavy.mdl" ) },
	{ 78, _w( "models/player/custom_player/legacy/tm_phoenix_varianta.mdl" ) },
	{ 79, _w( "models/player/custom_player/legacy/tm_phoenix_variantb.mdl" ) },
	{ 80, _w( "models/player/custom_player/legacy/tm_phoenix_variantc.mdl" ) },
	{ 81, _w( "models/player/custom_player/legacy/tm_phoenix_variantd.mdl" ) },
	{ 82, _w( "models/player/custom_player/legacy/tm_leet_varianta.mdl" ) },
	{ 83, _w( "models/player/custom_player/legacy/tm_leet_variantb.mdl" ) },
	{ 84, _w( "models/player/custom_player/legacy/tm_leet_variantc.mdl" ) },
	{ 85, _w( "models/player/custom_player/legacy/tm_leet_variantd.mdl" ) },
	{ 86, _w( "models/player/custom_player/legacy/tm_leet_variante.mdl" ) },
	{ 87, _w( "models/player/custom_player/legacy/tm_professional.mdl" ) },
	{ 88, _w( "models/player/custom_player/legacy/tm_professional_var1.mdl" ) },
	{ 89, _w( "models/player/custom_player/legacy/tm_professional_var2.mdl" ) },
	{ 90, _w( "models/player/custom_player/legacy/tm_professional_var3.mdl" ) },
	{ 91, _w( "models/player/custom_player/legacy/tm_professional_var4.mdl" ) },

	};

	inline const tsl::robin_map<size_t, std::string> player_mask_list_raw =
	{
		{ 1, "models/player/holiday/facemasks/facemask_hoxton.mdl" },
	{ 2, "models/player/holiday/facemasks/porcelain_doll.mdl" },
	{ 3, "models/player/holiday/facemasks/facemask_skull.mdl" },
	{ 4, "models/player/holiday/facemasks/facemask_samurai.mdl" },
	{ 5, "models/player/holiday/facemasks/evil_clown.mdl" },
	{ 6, "models/player/holiday/facemasks/facemask_wolf.mdl" },
	{ 7, "models/player/holiday/facemasks/facemask_sheep_model.mdl" },
	{ 8, "models/player/holiday/facemasks/facemask_bunny_gold.mdl" },
	{ 9, "models/player/holiday/facemasks/facemask_anaglyph.mdl" },
	{ 10, "models/player/holiday/facemasks/facemask_porcelain_doll_kabuki.mdl" },
	{ 11, "models/player/holiday/facemasks/facemask_dallas.mdl" },
	{ 12, "models/player/holiday/facemasks/facemask_pumpkin.mdl" },
	{ 13, "models/player/holiday/facemasks/facemask_sheep_bloody.mdl" },
	{ 14, "models/player/holiday/facemasks/facemask_devil_plastic.mdl" },
	{ 15, "models/player/holiday/facemasks/facemask_boar.mdl" },
	{ 16, "models/player/holiday/facemasks/facemask_chains.mdl" },
	{ 17, "models/player/holiday/facemasks/facemask_tiki.mdl" },
	{ 18, "models/player/holiday/facemasks/facemask_bunny.mdl" },
	{ 19, "models/player/holiday/facemasks/facemask_sheep_gold.mdl" },
	{ 20, "models/player/holiday/facemasks/facemask_zombie_fortune_plastic.mdl" },
	{ 21, "models/player/holiday/facemasks/facemask_chicken.mdl" },
	{ 22, "models/player/holiday/facemasks/facemask_skull_gold.mdl" },
	{ 23, "models/player/holiday/facemasks/facemask_tf2_demo_model.mdl" },
	{ 24, "models/player/holiday/facemasks/facemask_tf2_engi_model.mdl" },
	{ 25, "models/player/holiday/facemasks/facemask_tf2_heavy_model.mdl" },
	{ 26, "models/player/holiday/facemasks/facemask_tf2_medic_model.mdl" },
	{ 27, "models/player/holiday/facemasks/facemask_tf2_pyro_model.mdl" },
	{ 28, "models/player/holiday/facemasks/facemask_tf2_scout_model.mdl" },
	{ 29, "models/player/holiday/facemasks/facemask_tf2_sniper_model.mdl" },
	{ 30, "models/player/holiday/facemasks/facemask_tf2_soldier_model.mdl" },
	{ 31, "models/player/holiday/facemasks/facemask_tf2_spy_model.mdl" },
	};
};