#pragma once

class listbox;
struct listbox_item_t;

namespace inventorychanger
{
	struct inventory_item_t
	{
		auto operator<=>( const inventory_item_t& ) const = default;

		bool enabled() const
		{
			return local_player && ( local_player->get_team() == 2 && enabled_t || local_player->get_team() == 3 && enabled_ct );
		}

		int definition_index{};
		int paint_kit{};
		int seed{};
		float wear = 0.f;
		int stat_trak{};
		bool update = false;
		bool fullupdate = false;
		bool enabled_ct = false;
		bool enabled_t = false;
	};

	void update_menu_items();
	void update_selected( bool force = false );
	void add();
	void set( int index, bool force = false );
	void load();
	void save( bool force = false );
	void remove();

	void fill_weapon_list();
	void fill_skin_list( std::string& search );

	bool is_knife( int idx );
	bool is_glove( int idx );
	bool same_item( int idx, int idx2 );

	inline std::vector<inventory_item_t> items = {};

	inline std::vector< skinchanger::paint_kit_data_s> menu_items_data = {};

	inline std::vector<int32_t> weapon_menu_items_data{};

	inline std::weak_ptr<listbox> paint_kit_listbox{};
	inline std::weak_ptr<listbox> weapons_listbox{};

	inline uint64_t highest_id = {};
};