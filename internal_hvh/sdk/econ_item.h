#pragma once

class utl_string
{
public:
	const char* Get()
	{
		return reinterpret_cast< const char* >( m_Storage.Base() );
	}

private:
	CUtlMemory<uint8_t> m_Storage;
	int m_nActualLength;
};

template <typename T>
class CDefLess
{
public:
	CDefLess() {}
	CDefLess( int i ) {}
	inline bool operator()( const T& lhs, const T& rhs ) const { return ( lhs < rhs ); }
	inline bool operator!() const { return false; }
};

template < class I >
struct UtlRBTreeLinks_t
{
	I  m_Left;
	I  m_Right;
	I  m_Parent;
	I  m_Tag;
};

template < class T, class I >
struct UtlRBTreeNode_t : public UtlRBTreeLinks_t< I >
{
	T  m_Data;
};

template < class T, class I = uint16_t, typename L = bool ( * )( const T&, const T& ), class M = CUtlMemory< UtlRBTreeNode_t< T, I >, I > >
class utl_rb_tree
{
	typedef L LessFunc_t;
	typedef UtlRBTreeNode_t< T, I > Node_t;

public:
	I parent( I i ) const;

	I left_child( I i ) const;

	I right_child( I i ) const;

	bool is_valid_index( I i ) const;

	static I invalid_index();

	I max_element() const;

	T& element( I i );
	T const& element( I i ) const;
	T& operator[]( I i );
	T const& operator[]( I i ) const;

public:
	LessFunc_t m_LessFunc;
	M m_Elements;
	I m_Root;
	I m_NumElements;
	I m_FirstFree;
	typename M::Iterator_t m_LastAlloc; // the last index allocated
	Node_t* m_pElements;
};

template < class T, class I, typename L, class M >
inline bool utl_rb_tree<T, I, L, M>::is_valid_index( I i ) const
{
	if ( !m_Elements.IsIdxValid( i ) )
		return false;

	if ( m_Elements.IsIdxAfter( i, m_LastAlloc ) )
		return false; // don't read values that have been allocated, but not constructed

	return left_child( i ) != i;
}

template < class T, class I, typename L, class M >
inline I utl_rb_tree<T, I, L, M>::parent( I i ) const
{
	return i != invalid_index() ? m_Elements[ i ].m_Parent : invalid_index();
}

template < class T, class I, typename L, class M >
inline I utl_rb_tree<T, I, L, M>::left_child( I i ) const
{
	return i != invalid_index() ? m_Elements[ i ].m_Left : invalid_index();
}

template < class T, class I, typename L, class M >
inline I utl_rb_tree<T, I, L, M>::right_child( I i ) const
{
	return i != invalid_index() ? m_Elements[ i ].m_Right : invalid_index();
}

template < class T, class I, typename L, class M >
inline I utl_rb_tree<T, I, L, M>::invalid_index()
{
	return ( I ) M::InvalidIndex();
}

template < class T, class I, typename L, class M >
inline	I  utl_rb_tree<T, I, L, M>::max_element() const
{
	return ( I ) m_Elements.NumAllocated();
}

template < class T, class I, typename L, class M >
inline T& utl_rb_tree<T, I, L, M>::element( I i )
{
	return m_Elements[ i ].m_Data;
}

template < class T, class I, typename L, class M >
inline T const& utl_rb_tree<T, I, L, M>::element( I i ) const
{
	return m_Elements[ i ].m_Data;
}

template < class T, class I, typename L, class M >
inline T& utl_rb_tree<T, I, L, M>::operator[]( I i )
{
	return element( i );
}

template < class T, class I, typename L, class M >
inline T const& utl_rb_tree<T, I, L, M>::operator[]( I i ) const
{
	return element( i );
}

template <typename K, typename T, typename I = uint16_t, typename LessFunc_t = bool ( * )( const K&, const K& ) >
class utl_map
{
	typedef K KeyType_t;
	typedef T ElemType_t;
	typedef I IndexType_t;

	struct Node_t
	{
		Node_t()
		{}

		Node_t( const Node_t& from )
			: key( from.key ),
			elem( from.elem )
		{}

		KeyType_t	key;
		ElemType_t	elem;
	};

	class CKeyLess
	{
	public:
		CKeyLess( const LessFunc_t& lessFunc ) : m_LessFunc( lessFunc ) {}

		bool operator!() const
		{
			return !m_LessFunc;
		}

		bool operator()( const Node_t& left, const Node_t& right ) const
		{
			return m_LessFunc( left.key, right.key );
		}

		LessFunc_t m_LessFunc;
	};

public:
	bool is_valid_index( IndexType_t i ) const { return m_Tree.is_valid_index( i ); }

	static IndexType_t invalid_index() { return CTree::invalid_index(); }

	IndexType_t  max_element() const { return m_Tree.max_element(); }

	ElemType_t& element( IndexType_t i ) { return m_Tree.element( i ).elem; }
	const ElemType_t& element( IndexType_t i ) const { return m_Tree.element( i ).elem; }
	ElemType_t& operator[]( IndexType_t i ) { return m_Tree.element( i ).elem; }
	const ElemType_t& operator[]( IndexType_t i ) const { return m_Tree.element( i ).elem; }
	KeyType_t& key( IndexType_t i ) { return m_Tree.element( i ).key; }
	const KeyType_t& key( IndexType_t i ) const { return m_Tree.element( i ).key; }

public:
	typedef utl_rb_tree<Node_t, I, CKeyLess> CTree;

	CTree m_Tree;
};

struct sticker_kit
{
	int id;
	int rarity;
	utl_string name;
	utl_string description;
	utl_string item_name;
	utl_string material_path;
	utl_string material_path_no_drips;
	utl_string inventory_image;
	int event_id;
	int event_team_id;
	int player_id;
	bool is_material_path_absolute;
	float rotate_start;
	float rotate_end;
	float scale_min;
	float scale_max;
	float wear_min;
	float wear_max;
	utl_string icon_url_small;
	utl_string icon_url_large;
	KeyValues* kv_item;
};

struct paint_kit
{
	int id;
	utl_string name;
	utl_string description_string;
	utl_string description_tag;
	utl_string same_name_family_aggregate;
	utl_string pattern;
	utl_string normal;
	utl_string logo_material;
	bool base_diffuse_override;
	int rarity;
	int style;
	uint8_t rgba_color[ 4 ][ 4 ];
	uint8_t rgba_logo_color[ 4 ][ 4 ];
	float wear_default;
	float wear_remap_min;
	float wear_remap_max;
	uint8_t fixed_seed;
	uint8_t phong_exponent;
	uint8_t phong_albedo_boost;
	uint8_t phong_intensity;
	float pattern_scale;
	float pattern_offset_x_start;
	float pattern_offset_x_end;
	float pattern_offset_y_start;
	float pattern_offset_y_end;
	float pattern_rotate_start;
	float pattern_rotate_end;
	float logo_scale;
	float logo_offset_x;
	float logo_offset_y;
	float logo_rotation;
	bool ignore_weapon_size_scale;
	int viewmodel_exponent_override_size;
	bool only_first_material;
	bool use_normal;
	float pearlescent;
	utl_string vmt_path;
	KeyValues* vmt_overrides;
};

struct item_list_entry_t
{
	int item_def;
	int paint_kit;
	int paint_kit_seed;
	float paint_kit_wear;
	uint32_t sticker_kit;
	uint32_t music_kit;
	int pad18;
};

struct econ_item_set_definition
{
	int vtable;
	const char* name;
	const char* localized_name;
	const char* unlocalized_name;
	const char* localized_description;
	CUtlVector<item_list_entry_t> item_entries;
	int bundle_item_def;
	bool is_collection;
	bool is_hidden_set;
	ItemDefinitionIndex craft_reward;

	struct itemset_attrib_t
	{
		int	attrib_def_index;
		uint32_t value;
	};

	CUtlVector<itemset_attrib_t> attributes;
};

struct alternate_icon_data_t
{
	utl_string simple_name;
	utl_string large_simple_name;
	utl_string icon_url_small;
	utl_string icon_url_large;
	utl_string pad58;
	utl_string pad68;
};

union attribute_data_union_t
{
	float as_float;
	uint32_t as_uint32;
	uint8_t* as_blobpointer;
};

struct static_attrib_t
{
	uint16_t def_index;
	attribute_data_union_t value;
	bool force_gc_to_generate;
};

struct sticker_data_t
{
	char	sticker_model_path[ 128 ];
	char	sticker_material_path[ 128 ];
	Vector	world_model_projection_start;
	Vector	world_model_projection_end;
	char	sticker_bone_parent_name[ 32 ];
};

struct weapon_paintable_material_t
{
	char name[ 128 ];
	char originalmaterial_name[ 128 ];
	char folder_name[ 128 ];
	int view_model_size;						// texture size
	int world_model_size;						// texture size
	float weapon_length;
	float uv_scale;
	bool base_texture_override;
	bool mirror_pattern;
};

class econ_item_definition
{
public:
	VFUNC( 2, get_base_name(), const char* ( __thiscall* )( void* ) )( );
	void* vtable;
	KeyValues* kv_item;
	ItemDefinitionIndex def_index;
	CUtlVector< ItemDefinitionIndex > associated_items_def_indexes;
	bool enabled;
	char* prefab;
	uint8_t min_item_level;
	uint8_t max_item_level;
	uint8_t item_rarity;
	uint8_t item_quality;
	uint8_t forced_item_quality;
	uint8_t default_drop_item_quality;
	uint8_t default_drop_quantity;
	CUtlVector<static_attrib_t> static_attributes;
	uint8_t popularity_seed;
	void* portraits_kv;
	const char* item_base_name;
	bool is_proper_name;
	const char* item_type_name;
	uint32_t item_type_id;
	const char* item_description;
	int32_t expiration;
	int32_t def_creation;
	const char* inventory_model;
	const char* inventory_image;
	CUtlVector<const char*>	inventory_overlay_images;
	int32_t inventory_image_position[ 2 ];
	int32_t inventory_image_size[ 2 ];
	const char* base_display_model; // NOTE: contains the glove arms model if item is a glove
	bool load_on_demand;
	bool has_been_loaded;
	bool hide_body_groups_deployed_only;
	const char* world_display_model;
	const char* world_dropped_model_ptr;
	const char* world_holstered_model;
	const char* world_extra_wearable_model;
	char world_dropped_model[ 80 ];
	CUtlVector<sticker_data_t> sticker_models;
	const char* icon_default_image;
	bool attach_to_hands;
	bool attach_to_hands_vm_only;
	bool flip_view_model;
	bool act_as_wearable;
	CUtlVector< int > item_sets;
	void* asset_info;
	uint32_t item_type;
	const char* brass_model_override;
	const char* zoom_in_sound;
	const char* zoom_out_sound;
	void* tool;
	void* bundle_info;
	uint32_t capabilities;
	uint32_t num_concrete_items;
	int sound_material_id;
	bool disable_style_selection;
	utl_string icon_url_small;
	utl_string icon_url_large;
	char pad178[ 32 ];
	const char* particle_file;
	const char* particle_snapshot_file;
	const char* loot_list_name;
	CUtlVector<int> character_paint_kit_indices;
	CUtlVector<weapon_paintable_material_t> paint_data;
	void* inventory_image_data;
	const char* item_classname;
	const char* item_log_classname;
	const char* item_icon_classname;
	const char* definition_name;
	bool is_hidden;
	bool should_show_in_armory;
	bool base_item;
	bool default_slot_item;
	bool is_imported;
	bool one_per_account_cdkey;
	econ_item_definition* owning_pack_bundle;
	const char* armory_description;
	int armory_remap;
	int store_remap;
	const char* armory_remap_ptr;
	const char* store_remap_ptr;
	const char* class_token;
	const char* slot_token;
	int drop_type;
	const char* holiday_restriction;
	int sub_type;
	bool allowed_in_this_match;
	void* proxy_criteria;
	uint32_t equip_region_mask;
	uint32_t equip_region_conflict_mask;
	char pad_224[ 64 ];
	bool developer;
	bool ignore_in_collection_view;
	int default_loadout_slot;
	int item_gear_slot;
	int item_gear_slot_position;
	int animation_slot;
	const char* player_display_model[ 4 ];
	char class_usability[ 4 ];
	int loadout_slots[ 4 ];
	bool is_supply_crate : 1;
	bool item_shares_equip_slot : 1;
	char pad_29e[ 2 ];
	const char* vo_prefix;
	const char* default_cheer;
}; // size = 0x2A8

class econ_item_schema
{
public:
	char pad04[ 100 ];

	utl_map<int, void*, int, CDefLess<int>> rarities;
	utl_map<int, void*, int, CDefLess<int>> qualities;
	char items[ 72 ];
	utl_map<int, econ_item_definition*, int, CDefLess<int>> items_sorted;
	econ_item_definition* default_item_definition;
	CUtlVector<void*> attributes_container;
	utl_map<int, void*, int, CDefLess<int>> recipes;
	utl_map<int, econ_item_set_definition, int> item_sets;

	char pad178[ 164 ];

	utl_map<uint64_t, alternate_icon_data_t, int, CDefLess<uint64_t>> alternate_icons;

	char pad240[ 72 ];

	utl_map<int, paint_kit*, int, CDefLess<int>> paint_kits;
	utl_map<int, sticker_kit*, int, CDefLess<int>> sticker_kits;

	char pad2D0[ 132 ];

	utl_map<const char*, KeyValues*, int> definition_prefabs;

	char pad39C[ 116 ];

	utl_map<int, void*, int, CDefLess<int>> music_defs;

	char pad410[ 824 ];

	utl_map<int, void*, int> character_defs;

	char pad748[ 120 ];
};

class econ_item_system
{
public:
	VFUNC( 0, get_item_schema_interface(), econ_item_schema* ( __thiscall* )( void* ) )( );
};
