#pragma once

namespace offsets
{
	constexpr uint64_t interface_client = 0x5f950a0a05605672;
	constexpr uint64_t interface_entity_list = 0x6c385bd2d88876ed;
	constexpr uint64_t interface_engine = 0x6440607e874b5eb3;
	constexpr uint64_t interface_prediction = 0x26a97ec0330c9d1a;
	constexpr uint64_t interface_move_helper = 0x8f4a73c10c6d7685;
	constexpr uint64_t interface_game_movement = 0xd75d1bef22efe5a0;
	constexpr uint64_t interface_globals = 0xf77913ecbf0799f7;
	constexpr uint64_t interface_model_info = 0xe6cac7b268d12498;
	constexpr uint64_t interface_model_render = 0xe3e936ce84c3f4ce;
	constexpr uint64_t interface_material_system = 0x3ad18acae725dcd7;
	constexpr uint64_t interface_render_view = 0x790f9909e89adcc9;
	constexpr uint64_t interface_cvar = 0x8f55ff2c8ef0f083;
	constexpr uint64_t interface_view_render = 0xabbccb3b2577a2f0;
	constexpr uint64_t interface_view_render_beams = 0xed2603870adca1b2;
	constexpr uint64_t interface_game_event_manager = 0x6f1b5aa7eac0ac1e;
	constexpr uint64_t interface_input = 0xde6f3f4e7cbab091;
	constexpr uint64_t interface_surface_props = 0x8c0d532171b8ac9a;
	constexpr uint64_t interface_engine_trace = 0x87f54b60cdea5ada;
	constexpr uint64_t interface_client_state = 0x5fbe0848b92abfcb;
	constexpr uint64_t interface_surface = 0x19ba827c7ff8863a;
	constexpr uint64_t interface_memalloc = 0xc6f89eb3ec46f297;
	constexpr uint64_t interface_debug_overlay = 0x9c0d656b67373e7c;
	constexpr uint64_t interface_glow_object_manager = 0xffded58a3e710bfe;
	constexpr uint64_t interface_panel = 0x52b684df3ee66e63;
	constexpr uint64_t interface_engine_sound = 0x45aa27a005c9606d;
	constexpr uint64_t interface_game_rules_proxy = 0xfbd1b1bb57956a2d;
	constexpr uint64_t interface_weapon_system = 0x2b0da6691ef42455;
	constexpr uint64_t interface_inventory_manager = 0xe8f710a554722fe;
	constexpr uint64_t interface_localize = 0x83f31d18385fe68c;
	constexpr uint64_t interface_hud = 0x8de9a3e3a6a7fe56;
	constexpr uint64_t interface_player_resource = 0x836ed98ce95653db;
	constexpr uint64_t interface_client_leaf_system = 0xfaad2edafb3c210c;
	constexpr uint64_t interface_effects = 0x20750c86c25e7925;
	constexpr uint64_t interface_host_state = 0x7599ce1a802f7235;
	constexpr uint64_t interface_mdl_cache = 0x666b408ce30f4ba0;
	constexpr uint64_t interface_studio_render = 0xcf8a4047a0511531;
	constexpr uint64_t interface_hardware_config = 0xc52b5fcd0241f48e;
	constexpr uint64_t interface_client_tools = 0x56771e6c7138dac6;
	constexpr uint64_t interface_string_tables = 0x5c84e591d81ed499;
	constexpr uint64_t interface_file_system = 0x2a8ad007a599b0e3;
	constexpr uint64_t interface_key_values_system = 0x3262ce91de15d0a3;
	constexpr uint64_t interface_panoroama_ui_engine = 0x78e66385c86c032d;
	constexpr uint64_t sig_set_abs_angles = 0xc3a1d32bd7bcd31f;
	constexpr uint64_t sig_set_abs_origin = 0xf5fed60f6dae2aec;
	constexpr uint64_t sig_update_anim_state = 0x36e25bd40f5cc89;
	constexpr uint64_t sig_ret_to_cam_think = 0x8e46e062503f4150;
	constexpr uint64_t sig_ret_to_draw_notify = 0xc42fd79f8a3030ed;
	constexpr uint64_t sig_ret_to_console_print = 0xff1351fa85fcf389;
	constexpr uint64_t sig_ret_to_add_notify = 0x3e5770e2b6a9782d;
	constexpr uint64_t sig_ret_to_draw_crosshair = 0xf4d2f297ceea5645;
	constexpr uint64_t sig_standard_blending_rules = 0xd84a911417ddf082;
	constexpr uint64_t sig_build_transformations = 0x46ff1c838276703c;
	constexpr uint64_t sig_smoke_count = 0x6f2142ec8090c6bc;
	constexpr uint64_t sig_get_econ_item_view = 0xde09f047f2c08903;
	constexpr uint64_t sig_get_sequence_act = 0x7407bc9d7fb2b749;
	constexpr uint64_t sig_invalidate_physics_recursive = 0xe8d207d9e93c6229;
	constexpr uint64_t sig_is_breakable_entity = 0x3928934de0aeeccc;
	constexpr uint64_t sig_print_dev_console = 0xbe3564b890efde15;
	constexpr uint64_t sig_find_element = 0xd157c3edd38cac19;
	constexpr uint64_t sig_physics_run_think = 0xbe65f44bb17d2246;
	constexpr uint64_t sig_cmbat_get_econ_item_view = 0xa41fb05fec6fa981;
	constexpr uint64_t sig_get_static_data = 0xffd223aaf2c76be8;
	constexpr uint64_t sig_disable_post_processing = 0x7db4e75e9fb366d8;
	constexpr uint64_t sig_gc_client_system = 0xecaa3a90ab162975;
	constexpr uint64_t sig_lookup_bone = 0xccce944e870ea5f7;
	constexpr uint64_t sig_modifier_table = 0x24abb75efb6b28f2;
	constexpr uint64_t sig_load_named_sky = 0x7cc3509ae525c10a;
	constexpr uint64_t sig_reveal_ranks = 0x490096b3c2ac4eb2;
	constexpr uint64_t sig_set_clantag = 0x64ede7812291d28c;
	constexpr uint64_t sig_deathnotice_check = 0x94dec95c061d601b;
	constexpr uint64_t sig_clear_deathnotices = 0x2dbdf37b105e3df6;
	constexpr uint64_t sig_get_checksum = 0x2b235adcef40e0c2;
	constexpr uint64_t sig_equip_glove = 0x1f34daea43589159;
	constexpr uint64_t sig_item_system = 0x37a20d6d0b3bb13e;
	constexpr uint64_t sig_parse_kits_addr = 0x22fe44c2f5f98b74;
	constexpr uint64_t sig_util_traceline = 0x2f246636ddcd739c;
	constexpr uint64_t sig_cl_send_move_limit = 0xc1d50290b8929622;
	constexpr uint64_t sig_is_ready = 0x844414dc8356dbe6;
	constexpr uint64_t sig_ret_to_setup_velocity = 0xc278b4f361a86eea;
	constexpr uint64_t sig_ret_to_post_data_update = 0x5cd9aa960a71db6d;
	constexpr uint64_t sig_ret_to_enable_inventory = 0xf57ee058e58ce8a6;
	constexpr uint64_t sig_interp_clamp = 0xfb2fdd26bb7a0599;
	constexpr uint64_t sig_ret_to_trace_ray = 0xc42fea95e1cafb67;
	constexpr uint64_t sig_ret_to_scope_arc = 0xde35d7f551febc49;
	constexpr uint64_t sig_ret_to_scope_lens = 0x80bc033e976bf790;
	constexpr uint64_t sig_post_process_params = 0x8a472a02d2785629;
	constexpr uint64_t sig_ret_to_post_network_data_received = 0x2d63be60ef1dbfd8;
	constexpr uint64_t sig_ret_to_add_viewmodel_stattrak = 0x6a6162f1877aa97a;
	constexpr uint64_t sig_trace_to_studio_csgo_hitgroups_priority = 0x2a02701bea9460f4;
	constexpr uint64_t sig_should_skip_anim_frame_check = 0xaaf12770a9c49139;
	constexpr uint64_t sig_somelimit = 0x553941c275e2f497;
	constexpr uint64_t sig_cvar_check = 0xedbda6235256e02d;
	constexpr uint64_t sig_cs_player_pred_map = 0x9adff1132c3c9db0;
	constexpr uint64_t sig_base_cs_grenade_pred_map = 0x4f4704cac49e49fa;
	constexpr uint64_t sig_decoy_map = 0xa9d819157ae00df2;
	constexpr uint64_t sig_he_map = 0x23c2b09661bc59d6;
	constexpr uint64_t sig_molotov_map = 0xd7807f9e1e2ae1cf;
	constexpr uint64_t sig_sensor_map = 0x706ab858037750a5;
	constexpr uint64_t sig_smoke_map = 0x4bdd967cd08c757b;
	constexpr uint64_t sig_flash_map = 0x3fd82477752d6151;
	constexpr uint64_t sig_view_matrix = 0x7378065c760caf52;
	constexpr uint64_t sig_get_item_schema = 0xfb3531a9eb707b56;
	constexpr uint64_t sig_econ_item_destructor = 0x82fb50c1b49a36b8;
	constexpr uint64_t sig_create_econ_item = 0x764724ad86314692;
	constexpr uint64_t sig_animstate_reset = 0x7a080929481d48b2;
	constexpr uint64_t sig_update_layer_order_preset = 0xd3928a0a81559fff;
	constexpr uint64_t sig_update_equipped_state = 0xf9fdcadcfb1a1065;
	constexpr uint64_t sig_set_custom_name = 0x57b82724f130663b;
	constexpr uint64_t sig_set_custom_desc = 0xac82ecfda07eb6af;
	constexpr uint64_t sig_set_dynamic_attribute_value = 0x52778119d3c2de0d;
	constexpr uint64_t sig_empty_mapping = 0xd6dbb55482a2550f;
	constexpr uint64_t sig_simulation_tick = 0x8af4a3de0e05db60;
	constexpr uint64_t sig_predictable = 0x1e735fa66949f69c;
	constexpr uint64_t sig_custom_materials = 0x82ed461e41517960;
	constexpr uint64_t sig_custom_material_initialized = 0xd5797cf7bbf4e6b9;
	constexpr uint64_t sig_visual_data_processors = 0x365df4bdd74f8f9e;
	constexpr uint64_t sig_spawn_time = 0x36d78d396680029c;
	constexpr uint64_t sig_ragdoll = 0x28e7810d44232948;
	constexpr uint64_t sig_lod_data = 0xacbf4f14183d0c92;
	constexpr uint64_t sig_ik_context = 0x1aaa2f9406552861;
	constexpr uint64_t sig_snapshot_front = 0x97e44daeccdc4637;
	constexpr uint64_t sig_snapshot_back = 0x4a0399fdee6b65d7;
	constexpr uint64_t sig_blend_tick = 0xda223f6932869407;
	constexpr uint64_t sig_get_follow_entity = 0xba7e6a572cb1fd93;
	constexpr uint64_t sig_last_client_fire_bullet_time = 0x8fef906073f3f319;
	constexpr uint64_t sig_final_predicted_tick = 0xc293e4b7c6f0b202;
	constexpr uint64_t sig_occlusion_mask = 0x9ed793f6cc4430ee;
	constexpr uint64_t sig_anim_state = 0xe5eaa2bd622a212b;
	constexpr uint64_t sig_command_context = 0xa50337dc518ac966;
	constexpr uint64_t sig_send_matching_sequence = 0x2ed0be876534ef26;
	constexpr uint64_t sig_sound_channels = 0xaed9917e2d919803;
	constexpr uint64_t sig_activity_mapping = 0x7fb7c0f4aa8ec982;
	constexpr uint64_t sig_load_from_buffer = 0x13947a2831c3428a;
	constexpr uint64_t sig_modifier_table_find = 0x9bffcda77c60049a;
	constexpr uint64_t sig_modifier_table_add_string = 0x8bfc485169d638b;
	constexpr uint64_t sig_allocate_thread_id = 0xebbe2b099bac1649;
	constexpr uint64_t sig_game_tonemap_settings = 0xa258d7a07d4b2677;
	constexpr uint64_t sig_length_disasm_game = 0xfc01c1815b9deec4;
	constexpr uint64_t sig_get_active_weapon_prefix = 0x3c6f262f8862f0e0;
	constexpr uint64_t sig_get_layer_ideal_weight_from_sequence_cycle = 0x88f12f1cb5bcfe2e;
	constexpr uint64_t sig_on_simulation_time_changing = 0x25b320b6906c8cee;
	constexpr uint64_t sig_get_root_move_parent = 0x20da1a15cd67abd0;
	constexpr uint64_t sig_ik_context_construct = 0xe1285122e4d3d237;
	constexpr uint64_t sig_ik_context_destruct = 0x11534a46a76649d5;
	constexpr uint64_t sig_ik_context_init = 0xb2c61463d7f3631f;
	constexpr uint64_t sig_ik_context_update_targets = 0x4243e94ec7d1ec98;
	constexpr uint64_t sig_ik_context_solve_dependencies = 0x55c1dbb9af7ccbf9;
	constexpr uint64_t sig_allocate_and_merge_bones = 0xb21b578e0aaea1fb;
	constexpr uint64_t sig_lookup_sequence = 0x1c4a557c1afabd12;
	constexpr uint64_t sig_ammodef_get = 0xefc47e99011799ab;
	constexpr uint64_t sig_get_any_sequence_anim_tag = 0xe382250da638268c;
	constexpr uint64_t sig_accumulate_interleaved_dispatch_layers = 0x9e0ab026c9d786a7;
	constexpr uint64_t sig_process_sockets = 0x3a655f43371e1442;
	constexpr uint64_t sig_sequence_hash_find_hash = 0xfd495a1c9234ee38;
	constexpr uint64_t sig_activity_mapping_reinitialize = 0x6a268ae97cfc97d9;
	constexpr uint64_t sig_find_mapping = 0x79ff80b381877f3e;
	constexpr uint64_t sig_index_model_sequences = 0x5702b8e6ad351ce1;
	constexpr uint64_t sig_get_seq_desc = 0x37c02d1d677c9817;
	constexpr uint64_t sig_keyvalues_init = 0xdcb56c67431ecb63;
	constexpr uint64_t sig_ret_is_box_visible = 0x20cb2176c901f932;
	constexpr uint64_t sig_random_int = 0x6485da387fa70f9a;
	constexpr uint64_t sig_random_float = 0x9f28ebb1c1f009d1;
	constexpr uint64_t sig_random_seed = 0xa9d0ca2aa104428b;
	constexpr uint64_t sig_var_mapping = 0x23f8ce10532a2ecd;
	constexpr uint64_t sig_free_thread_id = 0x368a7026fcdf541f;
	constexpr uint64_t sig_tls_slots = 0xb7ff556d7edf6ef8;
	constexpr uint64_t sig_projectile_throwtime = 0x271b6c0e2c60e5e6;
	constexpr uint64_t sig_init_materials = 0x4a5eccce6ab81846;
	constexpr uint64_t sig_render_box = 0xe5e5361cbbc15f6e;
	constexpr uint64_t sig_render_triangle = 0x4084b1f52e5a03b1;
	constexpr uint64_t sig_vertex_color_ignorez = 0x421ad52fa6ad1dbe;
	constexpr uint64_t sig_cycle_offset = 0x99e1e9f68a387c13;
	constexpr uint64_t sig_studio_hdr = 0x563bb319e81e2b5e;
	constexpr uint64_t sig_anim_layers = 0x9665292c350da9cf;
	constexpr uint64_t sig_is_ragdoll = 0x70a6503e73895f47;
	constexpr uint64_t sig_bone_accessor = 0xeb3755e63bc588cf;
	constexpr uint64_t sig_cl_move_rest = 0xf41f9195e5a4a5f5;
	constexpr uint64_t sig_post_build_transformations = 0xa5d317be05c6ffa8;
	constexpr uint64_t sig_ret_to_post_build_transformations = 0xf7e51cb5d3a301c9;
	constexpr uint64_t sig_collision_viewheight = 0xae612a2ac926698e;
	constexpr uint64_t sig_collision_bounds_change_time = 0x6c4d1697ebe311bc;
	constexpr uint64_t sig_velocity_patch = 0xcf3a383100b892c4;
	constexpr uint64_t sig_kv_find_key = 0x4f9d628d378bc9dc;
	constexpr uint64_t sig_kv_set_string = 0xa4557c28e1922c4e;
	constexpr uint64_t sig_surface_props = 0xffab6bcca7022fc0;
	constexpr uint64_t sig_animstate_update_setabsagles = 0x54fadacfd08513d6;
	constexpr uint64_t sig_mdl_create = 0x9789a04a47edca72;
	constexpr uint64_t sig_set_merged_mdl = 0x46f1e57a0555996a;
	constexpr uint64_t sig_setup_bones_attachment_queries = 0x2ef716a05cc4568;
	constexpr uint64_t sig_disable_render_target_allocation = 0xad19d20f5dc47300;
	constexpr uint64_t sig_get_sequence_flags = 0x5a8a09ed34f57832;
	constexpr uint64_t sig_get_iron_sight_controller = 0x7ebc5344bac80a96;
	constexpr uint64_t sig_enable_invalidate_bone_cache = 0x85f4cad426f78e8b;
	constexpr uint64_t sig_line_goes_through_smoke = 0xab4ea08f292ebd66;
	constexpr uint64_t sig_keyvalues_from_netsmg = 0x4d6c9d4053598786;
	constexpr uint64_t sig_model_bone_counter = 0x14e6506f7f9f7a44;
	constexpr uint64_t sig_most_recent_model_bone_counter = 0x1cdbca2db4cad8d1;
	constexpr uint64_t sig_accumulated_bone_mask = 0x7a06f807dd237ebc;
	constexpr uint64_t sig_ret_hitbox_to_world_transforms = 0xd944c1fb78f29a94;
	constexpr uint64_t sig_emit_sound = 0xdf27340985e27904;
	constexpr uint64_t sig_add_to_dirty_kd_tree = 0xa3ed8cb40064fbf5;
	constexpr uint64_t sig_get_player_viewmodel_arm_config_for_player_model = 0xeaf555c797db1741;
	constexpr uint64_t sig_client_side_addons_mask = 0xff4c0d4b3f95fd55;
	constexpr uint64_t sig_assassination_target_addon = 0x42893065b2bd69ca;
	constexpr uint64_t sig_update_addon_models = 0xd9be6e58a853cf94;
	constexpr uint64_t sig_msg_voicedata_constructor = 0xda3234911b157b58;
	constexpr uint64_t sig_parse_svg = 0x704eef6d50899593;
	constexpr uint64_t sig_construct_image = 0x70530363b711fce4;
	constexpr uint64_t sig_load_text_file = 0xe26232176602031c;
	constexpr uint64_t sig_econ_item_system = 0xa57ad8774ca7da34;
	constexpr uint64_t sig_clear_hud_weapon_icon = 0x907c4b8bf3a67e77;
	constexpr uint64_t sig_skinchanger_spam_patch = 0xaee4871f0b158567;
	constexpr uint64_t sig_cvar_vac_check = 0x5aedb571a8048798;
	constexpr uint64_t sig_v8_try_catch_ctor = 0x56d1d749879ead0e;
	constexpr uint64_t sig_v8_try_catch_dtor = 0x1b384287178dbf15;
	constexpr uint64_t sig_v8_handle_scope_ctor = 0x6c0ada1422807960;
	constexpr uint64_t sig_v8_handle_scope_dtor = 0x5bdb2735b13d980c;
	constexpr uint64_t sig_v8_handle_scope_create_handle = 0xddf663355d4b45ca;
	constexpr uint64_t sig_v8_context_enter = 0x9b878b80cf7325d3;
	constexpr uint64_t sig_v8_context_exit = 0xdcefdd2c64f6e320;
	constexpr uint64_t sig_v8_isolate_enter = 0x45f80fe26b4fa648;
	constexpr uint64_t sig_v8_isolate_exit = 0x6d19cc3d93a89d6c;
	constexpr uint64_t sig_v8_array_get = 0xd2f263411cd78c2f;
	constexpr uint64_t sig_v8_array_length = 0xa703567883cd3ecf;
	constexpr uint64_t sig_v8_object_get = 0x7fc0da130bf67f24;
	constexpr uint64_t sig_v8_object_get_property_names = 0xe90245bd2236e62a;
	constexpr uint64_t sig_v8_value_is_boolean = 0xba1a8d48ca5d3d88;
	constexpr uint64_t sig_v8_value_is_boolean_object = 0xd5572bd1d425607e;
	constexpr uint64_t sig_v8_value_is_number = 0xc177fa825ede9b2d;
	constexpr uint64_t sig_v8_value_is_number_object = 0xc696d67e88b00b34;
	constexpr uint64_t sig_v8_value_is_string = 0xfb7c81faecee59bf;
	constexpr uint64_t sig_v8_value_is_string_object = 0xa766d1be9c3ebd58;
	constexpr uint64_t sig_v8_value_is_object = 0xdb10af33fb398a98;
	constexpr uint64_t sig_v8_value_is_array = 0xae8c9cb172173fb2;
	constexpr uint64_t sig_v8_value_is_function = 0x320bdf6913d3c0f6;
	constexpr uint64_t sig_v8_value_boolean_value = 0x7e415ee1e064c334;
	constexpr uint64_t sig_v8_value_number_value = 0xdc4039f8f61692dd;
	constexpr uint64_t sig_v8_value_to_object = 0x8276c837503ea52;
	constexpr uint64_t sig_v8_string_utf8_ctor = 0xfad7d6ef77bcdc5a;
	constexpr uint64_t sig_v8_string_utf8_dtor = 0x15cc74626ce6fc7;
	constexpr uint64_t sig_ui_engine_get_panel_context = 0x4c29beb51b80c9e2;
	constexpr uint64_t sig_ui_engine_compile = 0x22ba178e6144e703;
	constexpr uint64_t sig_ui_engine_run_compiled_script = 0xd5dbbeedd9fc0bc3;
	constexpr uint64_t hook_start_sound_immediate = 0x990dab38871d6d3f;
	constexpr uint64_t hook_set_bodygroup = 0xcfc09f11031886c2;
	constexpr uint64_t hook_modify_eye_pos = 0x491badaeb6fa276c;
	constexpr uint64_t hook_wnd_proc = 0x4ef9c358b8662f99;
	constexpr uint64_t hook_create_move = 0xcc6f95df263f6def;
	constexpr uint64_t hook_cl_move = 0xc63d7273239ee02a;
	constexpr uint64_t hook_frame_stage_notify = 0x6a7b1934448cabbb;
	constexpr uint64_t hook_on_latch_interp_var = 0xa90f00725fbce11c;
	constexpr uint64_t hook_draw_model_execute = 0x9a3bc7b9ec1b8856;
	constexpr uint64_t hook_render_view = 0x25f1228d5d099012;
	constexpr uint64_t hook_level_init_post_entity = 0x280c26b8b858c663;
	constexpr uint64_t hook_level_init_pre_entity = 0x729295c7b647f072;
	constexpr uint64_t hook_get_tonemap_settings_from_env_tonemap_controller = 0x3e932b8428405342;
	constexpr uint64_t hook_draw_3d_debug_overlays = 0x24070b2882df21fb;
	constexpr uint64_t hook_shader_api_draw_mesh = 0x77ee3a2f0f64a5e0;
	constexpr uint64_t hook_is_box_visible = 0xec50c1689d8d515f;
	constexpr uint64_t hook_scene_end = 0x8ae10242ce954707;
	constexpr uint64_t hook_calc_renderable_world_space_aabb_bloated = 0xab3a239a6964b3bc;
	constexpr uint64_t hook_add_renderables_to_render_lists = 0x90b230ca3ba0c504;
	constexpr uint64_t hook_do_post_screen_space_effects = 0x259621db724598ac;
	constexpr uint64_t hook_present = 0x32811d6fe36918ad;
	constexpr uint64_t hook_reset = 0x870f7d4cd2ffb961;
	constexpr uint64_t hook_send_net_msg = 0xf6b85466cef64227;
	constexpr uint64_t hook_override_view = 0xd3812977c9dcea47;
	constexpr uint64_t hook_paint_traverse = 0xf041529edd84bab9;
	constexpr uint64_t hook_find_material = 0xafce1a76c4010c5f;
	constexpr uint64_t hook_emit_sound = 0x4218a1cef7b742a;
	constexpr uint64_t hook_is_connected = 0xc79ed9aa13428e13;
	constexpr uint64_t hook_lock_cursor = 0xfdbb17cf17e7313f;
	constexpr uint64_t hook_draw_set_color = 0xf375acda5d06ee03;
	constexpr uint64_t hook_test_hitboxes = 0xc9a81a4d67e259f1;
	constexpr uint64_t hook_setup_bones = 0x3586d0b0540b5fbf;
	constexpr uint64_t hook_animating_setup_bones = 0x3d79f162c6eb757c;
	constexpr uint64_t hook_post_data_update = 0x4a0670480f7fcc68;
	constexpr uint64_t hook_maintain_sequence_transitions = 0x6141e72d20f1e4b9;
	constexpr uint64_t hook_trace_ray = 0x46bb51893b974a62;
	constexpr uint64_t hook_run_command = 0x7145f23f1a38aae7;
	constexpr uint64_t hook_process_movement = 0xfefadab0f4330109;
	constexpr uint64_t hook_transfer_data = 0xde8067686f5a233;
	constexpr uint64_t hook_perform_prediction = 0x6bf91d34bae9f988;
	constexpr uint64_t hook_packet_start = 0xdbdac78dc810872f;
	constexpr uint64_t hook_level_shutdown = 0x77d6660c6c54fd2c;
	constexpr uint64_t hook_physics_simulate = 0x3ece973186afa88b;
	constexpr uint64_t hook_estimate_abs_velocity = 0xf89f58d4881a22f6;
	constexpr uint64_t hook_interpolate = 0x4ecc6f77dfd05f11;
	constexpr uint64_t hook_reset_latched = 0x8e0961461e25e9c6;
	constexpr uint64_t hook_send_weapon_anim = 0x61b4addfa3196cae;
	constexpr uint64_t hook_fire_event = 0xbbbd323eaddaa3f0;
	constexpr uint64_t hook_do_animation_events = 0xfc32d9f36c9d377d;
	constexpr uint64_t hook_deploy = 0x5614288d125ad210;
	constexpr uint64_t hook_get_fov = 0x8c6c10faf8c91090;
	constexpr uint64_t hook_effects = 0x26c2c48f27d2cd90;
	constexpr uint64_t hook_sequence = 0xda9e4d6e1fc28954;
	constexpr uint64_t hook_weapon_handle = 0x6a01b6121dbde2fd;
	constexpr uint64_t hook_simulation_time = 0x3097c47ad9af786f;
	constexpr uint64_t hook_general_float = 0xe8b3d47dd75224a9;
	constexpr uint64_t hook_general_vec = 0x9ae68733dfabe5a6;
	constexpr uint64_t hook_general_int = 0x1cb662d97f59b198;
	constexpr uint64_t hook_general_bool = 0x18fcd8ded7e63666;
	constexpr uint64_t hook_layer_sequence = 0x66b9b8cf128c8e5e;
	constexpr uint64_t hook_layer_cycle = 0x8e333820c84c3d8;
	constexpr uint64_t hook_layer_playback_rate = 0x6bb565641643b936;
	constexpr uint64_t hook_layer_weight = 0xacfc09ad85e23e98;
	constexpr uint64_t hook_layer_weight_delta_rate = 0xa39a870d89226fae;
	constexpr uint64_t hook_layer_order = 0xdddec39131fd74bb;
	constexpr uint64_t hook_convar_get_float_engine = 0x850b66607b67a641;
	constexpr uint64_t hook_convar_get_int_engine = 0x1129dabf0a427987;
	constexpr uint64_t hook_convar_get_int_client = 0x900f0dc432483f00;
	constexpr uint64_t hook_eye_position_and_vectors = 0x10542e161baf4879;
	constexpr uint64_t hook_obb_change_callback = 0x8a574e498758617e;
	constexpr uint64_t hook_server_cmd_key_values = 0x87e429ad2aa05d75;
	constexpr uint64_t hook_calc_view = 0xe19616d26bfb14c2;
	constexpr uint64_t hook_scale_mouse = 0x1f6a2852462ae86;
	constexpr uint64_t hook_convar_network_change_callback = 0x8a9ce871e18d3289;
	constexpr uint64_t hook_reevauluate_anim_lod = 0xfbf6d511deeb04fb;
	constexpr uint64_t hook_start_sound = 0xce6828b31c055059;
	constexpr uint64_t hook_update_clientside_anim = 0x9c162a2f63ea9d5b;
	constexpr uint64_t hook_do_animation_events_animating = 0x90c4ca2c347e9537;
	constexpr uint64_t hook_get_default_fov = 0xeae8a53ae8082fc9;
	constexpr uint64_t hook_ent_info_list_link_before = 0xc0d164cea3000504;
	constexpr uint64_t hook_extract_occluded_renderables = 0x2658ec380eae78d8;
	constexpr uint64_t hook_sv_msg_voicedata = 0x44212512945b841d;
	constexpr uint64_t hook_hud_reticle_process_input = 0x5af1ebd655400d66;
	constexpr uint64_t hook_update_interp_vars = 0x52ffa0489ad61978;
	constexpr uint64_t hook_particle_draw_model = 0x3bb4ced5ce0ba3da;
	constexpr uint64_t CAI_BaseNPC_m_bFadeCorpse = 0xf77050cc22597adb;
	constexpr uint64_t CAI_BaseNPC_m_bImportanRagdoll = 0x8723379f9af3b0bc;
	constexpr uint64_t CAI_BaseNPC_m_bIsMoving = 0xb4f7c16c49332c8f;
	constexpr uint64_t CAI_BaseNPC_m_bPerformAvoidance = 0x389bc95f48bcf83d;
	constexpr uint64_t CAI_BaseNPC_m_bSpeedModActive = 0x896e88bfd4c9f096;
	constexpr uint64_t CAI_BaseNPC_m_flTimePingEffect = 0xbc88e6ee75943a01;
	constexpr uint64_t CAI_BaseNPC_m_iDeathFrame = 0x555c32e02ff68f5f;
	constexpr uint64_t CAI_BaseNPC_m_iDeathPose = 0x2fc2503104913151;
	constexpr uint64_t CAI_BaseNPC_m_iSpeedModRadius = 0xc53914361ae95037;
	constexpr uint64_t CAI_BaseNPC_m_iSpeedModSpeed = 0xdc59a1005fd2d270;
	constexpr uint64_t CAI_BaseNPC_m_lifeState = 0x5582ffc5a4807c71;
	constexpr uint64_t CBRC4Target_m_bBrokenOpen = 0xae5afe6442d0b9a1;
	constexpr uint64_t CBRC4Target_m_flRadius = 0x69bbb9091de8e1;
	constexpr uint64_t CBaseAnimating_m_ScaleType = 0x23f650aa6ff146ba;
	constexpr uint64_t CBaseAnimating_m_bClientSideAnimation = 0x175b5fb4a10068cf;
	constexpr uint64_t CBaseAnimating_m_bClientSideFrameReset = 0x4f595a929a6c5570;
	constexpr uint64_t CBaseAnimating_m_bClientSideRagdoll = 0xa81056fb23d64fe6;
	constexpr uint64_t CBaseAnimating_m_bSuppressAnimSounds = 0x946459939cfd1622;
	constexpr uint64_t CBaseAnimating_m_flCycle = 0x3a83aaf41a652ee6;
	constexpr uint64_t CBaseAnimating_m_flEncodedController = 0x450158f6592d1764;
	constexpr uint64_t CBaseAnimating_m_flFrozen = 0x49143d8c7fc065a8;
	constexpr uint64_t CBaseAnimating_m_flModelScale = 0xbf3fca6e1a79823c;
	constexpr uint64_t CBaseAnimating_m_flPlaybackRate = 0x8bf67f1599d93b83;
	constexpr uint64_t CBaseAnimating_m_flPoseParameter = 0x133b5ef32c7b012e;
	constexpr uint64_t CBaseAnimating_m_hLightingOrigin = 0xcb0a6b82e3a8ce84;
	constexpr uint64_t CBaseAnimating_m_nBody = 0x334ac86dc2bef231;
	constexpr uint64_t CBaseAnimating_m_nForceBone = 0xd01c30af1eeb2810;
	constexpr uint64_t CBaseAnimating_m_nHighlightColorB = 0x7d5211a62617cea7;
	constexpr uint64_t CBaseAnimating_m_nHighlightColorG = 0x423e4159555f76c7;
	constexpr uint64_t CBaseAnimating_m_nHighlightColorR = 0x766c5beefabdbea6;
	constexpr uint64_t CBaseAnimating_m_nHitboxSet = 0xfc77f9c5975f3c64;
	constexpr uint64_t CBaseAnimating_m_nMuzzleFlashParity = 0xdfcfc0643c7ffb2;
	constexpr uint64_t CBaseAnimating_m_nNewSequenceParity = 0xc61ed00e4cc1a66a;
	constexpr uint64_t CBaseAnimating_m_nResetEventsParity = 0x1282769551c9a726;
	constexpr uint64_t CBaseAnimating_m_nSequence = 0x63ba532880f73ad3;
	constexpr uint64_t CBaseAnimating_m_nSkin = 0x49dc7f22d03338ee;
	constexpr uint64_t CBaseAnimating_m_vecForce = 0x79f0706dec8a5cc7;
	constexpr uint64_t CBaseAttributableItem_m_AttributeManager = 0x9757c05bf103582f;
	constexpr uint64_t CBaseAttributableItem_m_Attributes = 0x911574dc2061f592;
	constexpr uint64_t CBaseAttributableItem_m_Item = 0x35775e41511575a6;
	constexpr uint64_t CBaseAttributableItem_m_NetworkedDynamicAttributesForDemos = 0x4f358ac7bd801e6a;
	constexpr uint64_t CBaseAttributableItem_m_OriginalOwnerXuidHigh = 0xcf1a277cb16934d;
	constexpr uint64_t CBaseAttributableItem_m_OriginalOwnerXuidLow = 0x50a46704bb9312ca;
	constexpr uint64_t CBaseAttributableItem_m_ProviderType = 0xd1084cfb9e3280d5;
	constexpr uint64_t CBaseAttributableItem_m_bInitialized = 0x293554f8a77a8047;
	constexpr uint64_t CBaseAttributableItem_m_flFallbackWear = 0x759b8dfd1099bae2;
	constexpr uint64_t CBaseAttributableItem_m_hOuter = 0x401c72afec3e4a48;
	constexpr uint64_t CBaseAttributableItem_m_iAccountID = 0xd1375ef0933acb02;
	constexpr uint64_t CBaseAttributableItem_m_iEntityLevel = 0x9ed280991a972537;
	constexpr uint64_t CBaseAttributableItem_m_iEntityQuality = 0xb3ef86873fca72af;
	constexpr uint64_t CBaseAttributableItem_m_iItemDefinitionIndex = 0x39737ba98d734884;
	constexpr uint64_t CBaseAttributableItem_m_iItemIDHigh = 0xb6a368e13a35aa63;
	constexpr uint64_t CBaseAttributableItem_m_iItemIDLow = 0x36c3f0b218931ac7;
	constexpr uint64_t CBaseAttributableItem_m_iReapplyProvisionParity = 0x64a3b77e58d4fe7e;
	constexpr uint64_t CBaseAttributableItem_m_nFallbackPaintKit = 0xd3127189eba77bc;
	constexpr uint64_t CBaseAttributableItem_m_nFallbackSeed = 0x56893511566cfcf8;
	constexpr uint64_t CBaseAttributableItem_m_nFallbackStatTrak = 0xce33f719228446af;
	constexpr uint64_t CBaseAttributableItem_m_szCustomName = 0xca7383d36da25e28;
	constexpr uint64_t CBaseButton_m_usable = 0x69fcb81bf108067;
	constexpr uint64_t CBaseCSGrenadeProjectile_m_nBounces = 0x6166ae5fdc5dc3c6;
	constexpr uint64_t CBaseCSGrenadeProjectile_m_nExplodeEffectIndex = 0xa936a644ff336170;
	constexpr uint64_t CBaseCSGrenadeProjectile_m_nExplodeEffectTickBegin = 0x251e6c1a0d10becc;
	constexpr uint64_t CBaseCSGrenadeProjectile_m_vInitialVelocity = 0x1bf6a9c531f48898;
	constexpr uint64_t CBaseCSGrenadeProjectile_m_vecExplodeEffectOrigin = 0xa416885b6af4235;
	constexpr uint64_t CBaseCSGrenade_m_bIsHeldByPlayer = 0x7b1d62473771355a;
	constexpr uint64_t CBaseCSGrenade_m_bLoopingSoundPlaying = 0x1d381a49844fba19;
	constexpr uint64_t CBaseCSGrenade_m_bPinPulled = 0x2e9f53bf1cf1bb9;
	constexpr uint64_t CBaseCSGrenade_m_bRedraw = 0x59e65437f76983d2;
	constexpr uint64_t CBaseCSGrenade_m_fDropTime = 0x3f6e05956cec0f9f;
	constexpr uint64_t CBaseCSGrenade_m_fThrowTime = 0xf3930d81d148c92f;
	constexpr uint64_t CBaseCSGrenade_m_flThrowStrength = 0x746b5d27a2f7db8e;
	constexpr uint64_t CBaseCombatCharacter_m_LastHitGroup = 0xd3874c4bbe5047d1;
	constexpr uint64_t CBaseCombatCharacter_m_flNextAttack = 0x8fc26f04ab6b9f03;
	constexpr uint64_t CBaseCombatCharacter_m_flTimeOfLastInjury = 0x83fed75786ee20f6;
	constexpr uint64_t CBaseCombatCharacter_m_hActiveWeapon = 0x3cb50a2348aca2b7;
	constexpr uint64_t CBaseCombatCharacter_m_hMyWeapons = 0xf837a95b3596faeb;
	constexpr uint64_t CBaseCombatCharacter_m_hMyWearables = 0x37e2232a39fe0739;
	constexpr uint64_t CBaseCombatCharacter_m_nRelativeDirectionOfLastInjury = 0xf0844333a76efdb3;
	constexpr uint64_t CBaseCombatWeapon_m_bFlipViewModel = 0xb6a32b9d5ee018;
	constexpr uint64_t CBaseCombatWeapon_m_flNextPrimaryAttack = 0xbb68e3157f64642a;
	constexpr uint64_t CBaseCombatWeapon_m_flNextSecondaryAttack = 0x195b08de99f6bcf0;
	constexpr uint64_t CBaseCombatWeapon_m_flTimeWeaponIdle = 0x7a0fd609687e1036;
	constexpr uint64_t CBaseCombatWeapon_m_hOwner = 0x96c360b46f7a0242;
	constexpr uint64_t CBaseCombatWeapon_m_hWeaponWorldModel = 0xdac67002457056d4;
	constexpr uint64_t CBaseCombatWeapon_m_iClip1 = 0x9f98fb7c32fc8415;
	constexpr uint64_t CBaseCombatWeapon_m_iClip2 = 0xe4764fd67492458b;
	constexpr uint64_t CBaseCombatWeapon_m_iNumEmptyAttacks = 0x4b7569d8186fba98;
	constexpr uint64_t CBaseCombatWeapon_m_iPrimaryAmmoType = 0x2c2fcf5dbee1aa2b;
	constexpr uint64_t CBaseCombatWeapon_m_iPrimaryReserveAmmoCount = 0x743ce973aad67990;
	constexpr uint64_t CBaseCombatWeapon_m_iSecondaryAmmoType = 0xc0cd0412927b609;
	constexpr uint64_t CBaseCombatWeapon_m_iSecondaryReserveAmmoCount = 0x9d5351a39582c7b;
	constexpr uint64_t CBaseCombatWeapon_m_iState = 0x6d677b0f091c0ea8;
	constexpr uint64_t CBaseCombatWeapon_m_iViewModelIndex = 0x59621973776c8614;
	constexpr uint64_t CBaseCombatWeapon_m_iWeaponModule = 0x78faf97b3c989ff9;
	constexpr uint64_t CBaseCombatWeapon_m_iWeaponOrigin = 0x25b9c56bde9a613b;
	constexpr uint64_t CBaseCombatWeapon_m_iWorldDroppedModelIndex = 0x7c9d2b637cf40461;
	constexpr uint64_t CBaseCombatWeapon_m_iWorldModelIndex = 0xee9fd920fa103bc;
	constexpr uint64_t CBaseCombatWeapon_m_nNextThinkTick = 0x37025358fe6dc751;
	constexpr uint64_t CBaseCombatWeapon_m_nViewModelIndex = 0x8d39ba1a02b65c13;
	constexpr uint64_t CBaseDoor_m_flWaveHeight = 0x1a9422c5c0d6ff4b;
	constexpr uint64_t CBaseEntity_m_Collision = 0xa3ba2cb11ad4139b;
	constexpr uint64_t CBaseEntity_m_CollisionGroup = 0xa57f8d34a4954598;
	constexpr uint64_t CBaseEntity_m_angRotation = 0xe253a0d0bb0ab80b;
	constexpr uint64_t CBaseEntity_m_bAlternateSorting = 0xae3e66a74bb8eb7d;
	constexpr uint64_t CBaseEntity_m_bAnimatedEveryTick = 0xa034cba5fbcf854e;
	constexpr uint64_t CBaseEntity_m_bEligibleForScreenHighlight = 0x648a9c7cf1b72f89;
	constexpr uint64_t CBaseEntity_m_bIsAutoaimTarget = 0xc0bbe0f22d05cea8;
	constexpr uint64_t CBaseEntity_m_bSimulatedEveryTick = 0x773b08647e85daf3;
	constexpr uint64_t CBaseEntity_m_bSpotted = 0x7f7602ea3180b43a;
	constexpr uint64_t CBaseEntity_m_bSpottedBy = 0x98dc70d80f04b2c7;
	constexpr uint64_t CBaseEntity_m_bSpottedByMask = 0x1fe759e5f9b7faf8;
	constexpr uint64_t CBaseEntity_m_cellX = 0xd4edf0a05735050e;
	constexpr uint64_t CBaseEntity_m_cellY = 0x6784a99de9a90c25;
	constexpr uint64_t CBaseEntity_m_cellZ = 0x922b7d41ac6f451b;
	constexpr uint64_t CBaseEntity_m_cellbits = 0x265e731ca6477fa2;
	constexpr uint64_t CBaseEntity_m_clrRender = 0x6774ae4ffb51a712;
	constexpr uint64_t CBaseEntity_m_fEffects = 0x85c87e1f88db5f98;
	constexpr uint64_t CBaseEntity_m_fadeMaxDist = 0xc6432b4a02163ef6;
	constexpr uint64_t CBaseEntity_m_fadeMinDist = 0xa5394c4b8e5b8be6;
	constexpr uint64_t CBaseEntity_m_flAnimTime = 0x8f4d441d2459c62;
	constexpr uint64_t CBaseEntity_m_flElasticity = 0xbd9feac22602db97;
	constexpr uint64_t CBaseEntity_m_flFadeScale = 0x3f5162c3af39a36a;
	constexpr uint64_t CBaseEntity_m_flLastMadeNoiseTime = 0xda62d7721ad17b28;
	constexpr uint64_t CBaseEntity_m_flMaxFallVelocity = 0xaf84dcd4b6a6b917;
	constexpr uint64_t CBaseEntity_m_flShadowCastDistance = 0x6fb6a4c907fba176;
	constexpr uint64_t CBaseEntity_m_flSimulationTime = 0xf940869839415c33;
	constexpr uint64_t CBaseEntity_m_flUseLookAtAngle = 0xdc5ab6e5103779f4;
	constexpr uint64_t CBaseEntity_m_hEffectEntity = 0xd31a3d493585df34;
	constexpr uint64_t CBaseEntity_m_hOwnerEntity = 0x2ebc9579b82a944f;
	constexpr uint64_t CBaseEntity_m_iName = 0x228139a8f53bca78;
	constexpr uint64_t CBaseEntity_m_iParentAttachment = 0xdc3525a563bcac1e;
	constexpr uint64_t CBaseEntity_m_iPendingTeamNum = 0x1feb258af6193879;
	constexpr uint64_t CBaseEntity_m_iTeamNum = 0xece7b094ce050eb1;
	constexpr uint64_t CBaseEntity_m_iTextureFrameIndex = 0xa3b8178d1756cc5a;
	constexpr uint64_t CBaseEntity_m_nMaxCPULevel = 0x7170fcee5276d4a7;
	constexpr uint64_t CBaseEntity_m_nMaxGPULevel = 0x683f2ee95e7d2830;
	constexpr uint64_t CBaseEntity_m_nMinCPULevel = 0x4c64f82d2752e38b;
	constexpr uint64_t CBaseEntity_m_nMinGPULevel = 0x57369d651f55679f;
	constexpr uint64_t CBaseEntity_m_nModelIndex = 0x54325bd6b6ceea09;
	constexpr uint64_t CBaseEntity_m_nRenderFX = 0x45cf59c71e630a02;
	constexpr uint64_t CBaseEntity_m_nRenderMode = 0xf1ec8321d4da277c;
	constexpr uint64_t CBaseEntity_m_nSolidType = 0xe25dd039832a1abb;
	constexpr uint64_t CBaseEntity_m_nSurroundType = 0x32d510d5de588992;
	constexpr uint64_t CBaseEntity_m_triggerBloat = 0xc6f1e81c7ec5f9c5;
	constexpr uint64_t CBaseEntity_m_usSolidFlags = 0xdcf8fba1c81f0ffb;
	constexpr uint64_t CBaseEntity_m_vecMaxs = 0xfe547442a58e697b;
	constexpr uint64_t CBaseEntity_m_vecMins = 0x407607d9422ff73a;
	constexpr uint64_t CBaseEntity_m_vecOrigin = 0x6ab3bfcc36e14902;
	constexpr uint64_t CBaseEntity_m_vecSpecifiedSurroundingMaxs = 0xa3027aeaa5d5bcdc;
	constexpr uint64_t CBaseEntity_m_vecSpecifiedSurroundingMins = 0xee4c431de8ae0c2c;
	constexpr uint64_t CBaseEntity_moveparent = 0x4da303548a28dde5;
	constexpr uint64_t CBaseFlex_m_blinktoggle = 0xa3697585bf371b43;
	constexpr uint64_t CBaseFlex_m_flexWeight = 0x27f57bde63fdc1a2;
	constexpr uint64_t CBaseFlex_m_viewtarget = 0x3826a92fc946f618;
	constexpr uint64_t CBaseGrenade_m_DmgRadius = 0xa8c5d4637bde0fef;
	constexpr uint64_t CBaseGrenade_m_bIsLive = 0x14adf020a3b8db29;
	constexpr uint64_t CBaseGrenade_m_fFlags = 0xb81cf00cf4921ca6;
	constexpr uint64_t CBaseGrenade_m_flDamage = 0x6826ff760133974e;
	constexpr uint64_t CBaseGrenade_m_hThrower = 0x6cf4330516f291fd;
	constexpr uint64_t CBaseGrenade_m_vecVelocity = 0x7d4d7191387d9ea4;
	constexpr uint64_t CBasePlayer_deadflag = 0xfa77f70fe1b6b4ed;
	constexpr uint64_t CBasePlayer_m_Local = 0xebd83efde553acb5;
	constexpr uint64_t CBasePlayer_m_PlayerFog_m_hCtrl = 0x765c3e9b63aa1923;
	constexpr uint64_t CBasePlayer_m_afPhysicsFlags = 0xf07e3040b0115436;
	constexpr uint64_t CBasePlayer_m_aimPunchAngle = 0x653b5ddec05f596;
	constexpr uint64_t CBasePlayer_m_aimPunchAngleVel = 0x16898817ec79b20a;
	constexpr uint64_t CBasePlayer_m_audio_entIndex = 0x44f4d15e39a4f10f;
	constexpr uint64_t CBasePlayer_m_audio_localBits = 0x99aa31d9885d2a0f;
	constexpr uint64_t CBasePlayer_m_audio_localSound_0_ = 0xdd802484b045a8f2;
	constexpr uint64_t CBasePlayer_m_audio_localSound_1_ = 0x974303107d3def68;
	constexpr uint64_t CBasePlayer_m_audio_localSound_2_ = 0x1730cd653a8a4ae9;
	constexpr uint64_t CBasePlayer_m_audio_localSound_3_ = 0x9d958c766f2eab36;
	constexpr uint64_t CBasePlayer_m_audio_localSound_4_ = 0xe7fdb66e72ccf5e7;
	constexpr uint64_t CBasePlayer_m_audio_localSound_5_ = 0x6f2c3c105c9387b;
	constexpr uint64_t CBasePlayer_m_audio_localSound_6_ = 0x238243bb7a71637c;
	constexpr uint64_t CBasePlayer_m_audio_localSound_7_ = 0xcce30978651f7eb6;
	constexpr uint64_t CBasePlayer_m_audio_soundscapeIndex = 0xa4215754ecad66c9;
	constexpr uint64_t CBasePlayer_m_bActiveCameraMan = 0xf19e3a1adf7e68f0;
	constexpr uint64_t CBasePlayer_m_bAllowAutoMovement = 0x79e0ec34b807c161;
	constexpr uint64_t CBasePlayer_m_bCameraManOverview = 0xdc57a72e58351b1d;
	constexpr uint64_t CBasePlayer_m_bCameraManScoreBoard = 0xad6a03d8d259d789;
	constexpr uint64_t CBasePlayer_m_bCameraManXRay = 0x998306a31209eb5c;
	constexpr uint64_t CBasePlayer_m_bConstraintPastRadius = 0x9e7f5f886e2667ce;
	constexpr uint64_t CBasePlayer_m_bDrawViewmodel = 0x25659f6d5963169b;
	constexpr uint64_t CBasePlayer_m_bDucked = 0xe5e9268e2481d36a;
	constexpr uint64_t CBasePlayer_m_bDucking = 0xa84266fd26f648a6;
	constexpr uint64_t CBasePlayer_m_bInDuckJump = 0xd8058a0c598d9210;
	constexpr uint64_t CBasePlayer_m_bPoisoned = 0x80ce162655b78940;
	constexpr uint64_t CBasePlayer_m_bShouldDrawPlayerWhileUsingViewEntity = 0xe137a49a683ce047;
	constexpr uint64_t CBasePlayer_m_bWearingSuit = 0xe8d18744dcacc4e8;
	constexpr uint64_t CBasePlayer_m_chAreaBits = 0xedffeea234dd2506;
	constexpr uint64_t CBasePlayer_m_chAreaPortalBits = 0xd581eaa6de4d67fc;
	constexpr uint64_t CBasePlayer_m_fFlags = 0xee7d3aa6ec204106;
	constexpr uint64_t CBasePlayer_m_fForceTeam = 0x7f2b38a5f8764137;
	constexpr uint64_t CBasePlayer_m_fOnTarget = 0xeb3c22c564601a3f;
	constexpr uint64_t CBasePlayer_m_flConstraintRadius = 0xbabba77477e95596;
	constexpr uint64_t CBasePlayer_m_flConstraintSpeedFactor = 0x767a501e60bcfd7b;
	constexpr uint64_t CBasePlayer_m_flConstraintWidth = 0x828c0839c94b3034;
	constexpr uint64_t CBasePlayer_m_flDeathTime = 0x1824ccb76d23646f;
	constexpr uint64_t CBasePlayer_m_flDuckAmount = 0x5bf0ee44c8367fd7;
	constexpr uint64_t CBasePlayer_m_flDuckSpeed = 0xce81b3dfbf39271c;
	constexpr uint64_t CBasePlayer_m_flFOVRate = 0x277a3f0f3362cbbb;
	constexpr uint64_t CBasePlayer_m_flFOVTime = 0xeddc9d81f40328c4;
	constexpr uint64_t CBasePlayer_m_flFallVelocity = 0xffc914884523bf9f;
	constexpr uint64_t CBasePlayer_m_flFriction = 0x4d573cf8ab89f049;
	constexpr uint64_t CBasePlayer_m_flLaggedMovementValue = 0x70c34fa007b94361;
	constexpr uint64_t CBasePlayer_m_flLastDuckTime = 0xc3b3fdc51b699049;
	constexpr uint64_t CBasePlayer_m_flMaxspeed = 0xdb4a410e183e9fd6;
	constexpr uint64_t CBasePlayer_m_flNextDecalTime = 0x5fa59b2f7802202c;
	constexpr uint64_t CBasePlayer_m_flStepSize = 0x2066aa9ac64c336a;
	constexpr uint64_t CBasePlayer_m_hColorCorrectionCtrl = 0xf230dea56ebbc0ea;
	constexpr uint64_t CBasePlayer_m_hConstraintEntity = 0xe93eeff3d79cb076;
	constexpr uint64_t CBasePlayer_m_hGroundEntity = 0x33742923332a4537;
	constexpr uint64_t CBasePlayer_m_hLastWeapon = 0x7b3c97fcbf09c518;
	constexpr uint64_t CBasePlayer_m_hObserverTarget = 0x9119fc9e18c69ca;
	constexpr uint64_t CBasePlayer_m_hPostProcessCtrl = 0x51299fa4da4dd2e3;
	constexpr uint64_t CBasePlayer_m_hTonemapController = 0x8b5484a92cb05085;
	constexpr uint64_t CBasePlayer_m_hUseEntity = 0x6c846e68edc0d3ae;
	constexpr uint64_t CBasePlayer_m_hVehicle = 0x96cb5772ca4e96db;
	constexpr uint64_t CBasePlayer_m_hViewEntity = 0x6d61e6cd0d41e8fa;
	constexpr uint64_t CBasePlayer_m_hViewModel_0_ = 0xcc0167e852572452;
	constexpr uint64_t CBasePlayer_m_hZoomOwner = 0x7a64c50f24d25784;
	constexpr uint64_t CBasePlayer_m_iAmmo = 0x5d06d8be80ef2b52;
	constexpr uint64_t CBasePlayer_m_iBonusChallenge = 0x723f90331a0f5150;
	constexpr uint64_t CBasePlayer_m_iBonusProgress = 0x9ecb9677d2bede89;
	constexpr uint64_t CBasePlayer_m_iCoachingTeam = 0x632f200a7c745162;
	constexpr uint64_t CBasePlayer_m_iDeathPostEffect = 0xa6c0d949e89c96ba;
	constexpr uint64_t CBasePlayer_m_iDefaultFOV = 0xdc5be8560aca257c;
	constexpr uint64_t CBasePlayer_m_iFOV = 0xf297e45d7f873f21;
	constexpr uint64_t CBasePlayer_m_iFOVStart = 0x5809dc8f733b588a;
	constexpr uint64_t CBasePlayer_m_iHealth = 0xf84e0f8afd2418d9;
	constexpr uint64_t CBasePlayer_m_iHideHUD = 0x55274be20129ca2d;
	constexpr uint64_t CBasePlayer_m_iObserverMode = 0xc3ae2f98b97ff1a4;
	constexpr uint64_t CBasePlayer_m_ladderSurfaceProps = 0x707649ebf34707bc;
	constexpr uint64_t CBasePlayer_m_lifeState = 0xb15af6a722eca60e;
	constexpr uint64_t CBasePlayer_m_nDuckJumpTimeMsecs = 0xb0cd24590d8b3c97;
	constexpr uint64_t CBasePlayer_m_nDuckTimeMsecs = 0x4b202c410153e5f3;
	constexpr uint64_t CBasePlayer_m_nJumpTimeMsecs = 0xd877d47ff8ce618a;
	constexpr uint64_t CBasePlayer_m_nNextThinkTick = 0x1eb02831e37c154c;
	constexpr uint64_t CBasePlayer_m_nOldButtons = 0x60a0dbc071f2adb;
	constexpr uint64_t CBasePlayer_m_nTickBase = 0x4b766eb702bced61;
	constexpr uint64_t CBasePlayer_m_nWaterLevel = 0x82c464371b6a9a87;
	constexpr uint64_t CBasePlayer_m_skybox3d_area = 0x23fa34aaf9a14fdc;
	constexpr uint64_t CBasePlayer_m_skybox3d_fog_HDRColorScale = 0x86c21e6a5b9bfa6d;
	constexpr uint64_t CBasePlayer_m_skybox3d_fog_blend = 0x33204ef4864c4d94;
	constexpr uint64_t CBasePlayer_m_skybox3d_fog_colorPrimary = 0x330833a6e57c9104;
	constexpr uint64_t CBasePlayer_m_skybox3d_fog_colorSecondary = 0x78114ad4cc458762;
	constexpr uint64_t CBasePlayer_m_skybox3d_fog_dirPrimary = 0x3da82741e24491e9;
	constexpr uint64_t CBasePlayer_m_skybox3d_fog_enable = 0x4b191cdca8104a31;
	constexpr uint64_t CBasePlayer_m_skybox3d_fog_end = 0x1b1ffade119445d1;
	constexpr uint64_t CBasePlayer_m_skybox3d_fog_maxdensity = 0x2f4c7356e40d8e0b;
	constexpr uint64_t CBasePlayer_m_skybox3d_fog_start = 0x503619cbc2d8a830;
	constexpr uint64_t CBasePlayer_m_skybox3d_origin = 0x2f573be6be55735c;
	constexpr uint64_t CBasePlayer_m_skybox3d_scale = 0xb47f5a923d56e5a8;
	constexpr uint64_t CBasePlayer_m_szLastPlaceName = 0x9737fe79cf0a941f;
	constexpr uint64_t CBasePlayer_m_uCameraManGraphs = 0x2c3ced0e439516c8;
	constexpr uint64_t CBasePlayer_m_ubEFNoInterpParity = 0x15f553d0f877213a;
	constexpr uint64_t CBasePlayer_m_vecBaseVelocity = 0x331713a65c7789b8;
	constexpr uint64_t CBasePlayer_m_vecConstraintCenter = 0xde9009c36922793d;
	constexpr uint64_t CBasePlayer_m_vecLadderNormal = 0xb842a0bf9d5e3c40;
	constexpr uint64_t CBasePlayer_m_vecVelocity_0_ = 0xcca7009dc80ff9c3;
	constexpr uint64_t CBasePlayer_m_vecVelocity_1_ = 0x84e1896eb07adb7c;
	constexpr uint64_t CBasePlayer_m_vecVelocity_2_ = 0x553b21f40e428956;
	constexpr uint64_t CBasePlayer_m_vecViewOffset_0_ = 0xe454c0521bc4b4e6;
	constexpr uint64_t CBasePlayer_m_vecViewOffset_1_ = 0x4e166988c46fc684;
	constexpr uint64_t CBasePlayer_m_vecViewOffset_2_ = 0xbc73739b4bcbb982;
	constexpr uint64_t CBasePlayer_m_viewPunchAngle = 0x8d388a134daca7c8;
	constexpr uint64_t CBasePlayer_m_vphysicsCollisionState = 0x956b7c54713e425;
	constexpr uint64_t CBasePlayer_pl = 0xf5011799631ed4d4;
	constexpr uint64_t CBaseTeamObjectiveResource_m_bBlocked = 0xaebfce3fd6d09c14;
	constexpr uint64_t CBaseTeamObjectiveResource_m_bCPIsVisible = 0x86ff2bb91d48ebdc;
	constexpr uint64_t CBaseTeamObjectiveResource_m_bControlPointsReset = 0xcc3ab0d9c6abb5bc;
	constexpr uint64_t CBaseTeamObjectiveResource_m_bInMiniRound = 0x982da19bb2442469;
	constexpr uint64_t CBaseTeamObjectiveResource_m_bPlayingMiniRounds = 0x4f24422909ed4e6c;
	constexpr uint64_t CBaseTeamObjectiveResource_m_bTeamCanCap = 0xc763d171fc9a3acf;
	constexpr uint64_t CBaseTeamObjectiveResource_m_flLazyCapPerc = 0x8384c4a7d3e46ca8;
	constexpr uint64_t CBaseTeamObjectiveResource_m_flPathDistance = 0x98bc4c5afe990ab3;
	constexpr uint64_t CBaseTeamObjectiveResource_m_flTeamCapTime = 0xf373e412eaa48617;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iBaseControlPoints = 0xbb895bc20f0b7b21;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iCappingTeam = 0xf64b5aff81f60722;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iNumControlPoints = 0x5bffcfe22ec9c9b5;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iNumTeamMembers = 0xd6525fa3adcf8ff4;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iOwner = 0xf09eed5b793852d3;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iPreviousPoints = 0x83d284d7c035a7be;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iStopWatchTimer = 0x32d0222a910bf512;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iTeamBaseIcons = 0x15388710d9f44b93;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iTeamIcons = 0x376c75698babf9bc;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iTeamInZone = 0x679f6b113a6ff6f4;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iTeamOverlays = 0xf1658980d6e1a146;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iTeamReqCappers = 0xca6ad221166c8213;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iTimerToShowInHUD = 0x707eddb09ce7c068;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iUpdateCapHudParity = 0x13b8fb834855d98e;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iWarnOnCap = 0x3db56303fbe0cfbb;
	constexpr uint64_t CBaseTeamObjectiveResource_m_iszWarnSound_0_ = 0xb9103d57764b5954;
	constexpr uint64_t CBaseTeamObjectiveResource_m_pszCapLayoutInHUD = 0xc03edd5d5b7e7861;
	constexpr uint64_t CBaseTeamObjectiveResource_m_vCPPositions_0_ = 0xe54f911bc7eb531;
	constexpr uint64_t CBaseToggle_m_flMoveTargetTime = 0x859964a0b57fb895;
	constexpr uint64_t CBaseToggle_m_movementType = 0x9ca04727b6887cee;
	constexpr uint64_t CBaseToggle_m_vecFinalDest = 0xb8462a8cad9caac9;
	constexpr uint64_t CBaseTrigger_m_bClientSidePredicted = 0x51081206b846f085;
	constexpr uint64_t CBaseTrigger_m_spawnflags = 0x70bf313c322afaf2;
	constexpr uint64_t CBaseViewModel_m_bShouldIgnoreOffsetAndAccuracy = 0xb3e518234ec718ea;
	constexpr uint64_t CBaseViewModel_m_fEffects = 0x108959363a5892ac;
	constexpr uint64_t CBaseViewModel_m_flPlaybackRate = 0x61d93fab08e25d5;
	constexpr uint64_t CBaseViewModel_m_hOwner = 0x12b20f86ccde0120;
	constexpr uint64_t CBaseViewModel_m_hWeapon = 0xc03d30b8b0994b93;
	constexpr uint64_t CBaseViewModel_m_nAnimationParity = 0xcdd3f2ddc9e9a610;
	constexpr uint64_t CBaseViewModel_m_nBody = 0x7b8bc9cc789cb3bc;
	constexpr uint64_t CBaseViewModel_m_nModelIndex = 0x506f7c7e21b20d11;
	constexpr uint64_t CBaseViewModel_m_nMuzzleFlashParity = 0xa24dd3e5c3e60c69;
	constexpr uint64_t CBaseViewModel_m_nNewSequenceParity = 0x7784b41b6eb8bf27;
	constexpr uint64_t CBaseViewModel_m_nResetEventsParity = 0x8c6e4ba878df2777;
	constexpr uint64_t CBaseViewModel_m_nSequence = 0x1eef3618d6b4f8f0;
	constexpr uint64_t CBaseViewModel_m_nSkin = 0x7e5ab4e6c96e7fe5;
	constexpr uint64_t CBaseViewModel_m_nViewModelIndex = 0x23bacf6d6f807b45;
	constexpr uint64_t CBaseWeaponWorldModel_m_fEffects = 0x2e31173182333332;
	constexpr uint64_t CBaseWeaponWorldModel_m_hCombatWeaponParent = 0x6767c4bd06ead7d7;
	constexpr uint64_t CBaseWeaponWorldModel_m_nBody = 0x6551e8a232d93bc6;
	constexpr uint64_t CBaseWeaponWorldModel_m_nModelIndex = 0x565e41812bea090b;
	constexpr uint64_t CBaseWeaponWorldModel_moveparent = 0x8079e9961ca8c66a;
	constexpr uint64_t CBeamSpotlight_m_bHasDynamicLight = 0x436eaf17bf53e04d;
	constexpr uint64_t CBeamSpotlight_m_bSpotlightOn = 0x2dccf1d1acd20caf;
	constexpr uint64_t CBeamSpotlight_m_flHDRColorScale = 0x31a02268d68a660;
	constexpr uint64_t CBeamSpotlight_m_flRotationSpeed = 0xdb333b7946bdbb83;
	constexpr uint64_t CBeamSpotlight_m_flSpotlightGoalWidth = 0xe0f24f9a812adbf9;
	constexpr uint64_t CBeamSpotlight_m_flSpotlightMaxLength = 0xbeccd2288fede55e;
	constexpr uint64_t CBeamSpotlight_m_nHaloIndex = 0x22a76bdfbc9294b1;
	constexpr uint64_t CBeamSpotlight_m_nRotationAxis = 0x5c1677318ec9d8ea;
	constexpr uint64_t CBeam_m_clrRender = 0x27506f290e45568c;
	constexpr uint64_t CBeam_m_fAmplitude = 0xd141a33121b23671;
	constexpr uint64_t CBeam_m_fEndWidth = 0x239e6963db877a3b;
	constexpr uint64_t CBeam_m_fFadeLength = 0xda5bbc660b7d3d28;
	constexpr uint64_t CBeam_m_fHaloScale = 0xa76ebbb72072761b;
	constexpr uint64_t CBeam_m_fSpeed = 0x22335bc7fd674230;
	constexpr uint64_t CBeam_m_fStartFrame = 0x9212d96cb55903ed;
	constexpr uint64_t CBeam_m_fWidth = 0x371e7609d8d255e2;
	constexpr uint64_t CBeam_m_flFrame = 0xa723de2d37e0c8c3;
	constexpr uint64_t CBeam_m_flFrameRate = 0x6e626d28d3ce973a;
	constexpr uint64_t CBeam_m_flHDRColorScale = 0xf4c0fc8779dbdb1b;
	constexpr uint64_t CBeam_m_hAttachEntity = 0x50cd902cf04609c4;
	constexpr uint64_t CBeam_m_nAttachIndex = 0xaf2ebfeb25724ee6;
	constexpr uint64_t CBeam_m_nBeamFlags = 0x8c19f5ddddc8217a;
	constexpr uint64_t CBeam_m_nBeamType = 0x1b454b1688593366;
	constexpr uint64_t CBeam_m_nClipStyle = 0x4b7bc01de8cac586;
	constexpr uint64_t CBeam_m_nHaloIndex = 0x7037d4b892edd17a;
	constexpr uint64_t CBeam_m_nModelIndex = 0xf0dee0d3d334db35;
	constexpr uint64_t CBeam_m_nNumBeamEnts = 0x8ddd40601a15bd83;
	constexpr uint64_t CBeam_m_nRenderFX = 0xd84e9e1b2a29b4d8;
	constexpr uint64_t CBeam_m_nRenderMode = 0x58398eeabd5bfddd;
	constexpr uint64_t CBeam_m_vecEndPos = 0x2d172fb2d88090ff;
	constexpr uint64_t CBeam_m_vecOrigin = 0xb8a7eb8c8382f9d7;
	constexpr uint64_t CBeam_moveparent = 0xa9ed5c94bdcd3e38;
	constexpr uint64_t CBoneFollower_m_modelIndex = 0x9e56511ecc94fd73;
	constexpr uint64_t CBoneFollower_m_solidIndex = 0x32cee5e5767438d6;
	constexpr uint64_t CBreachChargeProjectile_m_bShouldExplode = 0xde339be7d79e5f7a;
	constexpr uint64_t CBreachChargeProjectile_m_nParentBoneIndex = 0xe1eb648442e52dea;
	constexpr uint64_t CBreachChargeProjectile_m_vecParentBonePos = 0x7bda86fae211fd5d;
	constexpr uint64_t CBreachChargeProjectile_m_weaponThatThrewMe = 0xc71078faf7c7a73c;
	constexpr uint64_t CBreakableProp_m_bClientPhysics = 0x3f8c97b7a5d73504;
	constexpr uint64_t CBreakableProp_m_qPreferredPlayerCarryAngles = 0xe858e35023f641a8;
	constexpr uint64_t CBreakableSurface_m_RawPanelBitVec = 0x2685b51f8b779f44;
	constexpr uint64_t CBreakableSurface_m_bIsBroken = 0xca9c550292634926;
	constexpr uint64_t CBreakableSurface_m_flPanelHeight = 0xb1e7c9249c0fcf57;
	constexpr uint64_t CBreakableSurface_m_flPanelWidth = 0xeccde3b8b2a85540;
	constexpr uint64_t CBreakableSurface_m_nNumHigh = 0x6a6a54d8eeb338b9;
	constexpr uint64_t CBreakableSurface_m_nNumWide = 0x473e8df881b474e7;
	constexpr uint64_t CBreakableSurface_m_nSurfaceType = 0x45cfdc7e8b4b60b;
	constexpr uint64_t CBreakableSurface_m_vCorner = 0xf04ff70c2e08ebe6;
	constexpr uint64_t CBreakableSurface_m_vNormal = 0xe6e9de602b017a89;
	constexpr uint64_t CBumpMineProjectile_m_bArmed = 0x7d317eb330b6f29;
	constexpr uint64_t CBumpMineProjectile_m_nParentBoneIndex = 0x2a78adebbf2132fb;
	constexpr uint64_t CBumpMineProjectile_m_vecParentBonePos = 0x778fbc6b50bb3e26;
	constexpr uint64_t CC4_m_bBombPlacedAnimation = 0xe715f1f4cac5b375;
	constexpr uint64_t CC4_m_bIsPlantingViaUse = 0x7f6f8f3db2b00d8;
	constexpr uint64_t CC4_m_bShowC4LED = 0x6cf5203090ef03b2;
	constexpr uint64_t CC4_m_bStartedArming = 0xcda5a9caf361815;
	constexpr uint64_t CC4_m_fArmedTime = 0x108d390371756e03;
	constexpr uint64_t CCSGameRulesProxy_m_GGProgressiveWeaponKillUpgradeOrderCT = 0x923bb9031277d22;
	constexpr uint64_t CCSGameRulesProxy_m_GGProgressiveWeaponKillUpgradeOrderT = 0xb403dd1b61f4079;
	constexpr uint64_t CCSGameRulesProxy_m_GGProgressiveWeaponOrderCT = 0xd40bc9610121b6de;
	constexpr uint64_t CCSGameRulesProxy_m_GGProgressiveWeaponOrderT = 0xac4b0334029611e4;
	constexpr uint64_t CCSGameRulesProxy_m_MatchDevice = 0x2acf8feae03c3701;
	constexpr uint64_t CCSGameRulesProxy_m_RetakeRules = 0x77e5d4a7b7b63c59;
	constexpr uint64_t CCSGameRulesProxy_m_SpawnTileState = 0xdd3100aa592afead;
	constexpr uint64_t CCSGameRulesProxy_m_SurvivalGameRuleDecisionTypes = 0xf7a865c6937581b2;
	constexpr uint64_t CCSGameRulesProxy_m_SurvivalGameRuleDecisionValues = 0x5c627d3c4fca608e;
	constexpr uint64_t CCSGameRulesProxy_m_SurvivalRules = 0xa0d05ee8cfba1386;
	constexpr uint64_t CCSGameRulesProxy_m_TeamRespawnWaveTimes = 0x1162921f2b6b2f;
	constexpr uint64_t CCSGameRulesProxy_m_arrFeaturedGiftersAccounts = 0xe5f0a512a22e5499;
	constexpr uint64_t CCSGameRulesProxy_m_arrFeaturedGiftersGifts = 0x263c061e9e39ba82;
	constexpr uint64_t CCSGameRulesProxy_m_arrProhibitedItemIndices = 0xf84f641c411bbba9;
	constexpr uint64_t CCSGameRulesProxy_m_arrTournamentActiveCasterAccounts = 0x758ccefdb8f75cf4;
	constexpr uint64_t CCSGameRulesProxy_m_bAnyHostageReached = 0x7a904898f2014dd2;
	constexpr uint64_t CCSGameRulesProxy_m_bBlockersPresent = 0x44779b57cb959c22;
	constexpr uint64_t CCSGameRulesProxy_m_bBombDropped = 0x554364a475023322;
	constexpr uint64_t CCSGameRulesProxy_m_bBombPlanted = 0x54e2dd9f6d4b8053;
	constexpr uint64_t CCSGameRulesProxy_m_bCTCantBuy = 0x865b4f205e71c5db;
	constexpr uint64_t CCSGameRulesProxy_m_bCTTimeOutActive = 0x893da444f4b1b16d;
	constexpr uint64_t CCSGameRulesProxy_m_bDMBonusActive = 0x6d5f8c6bde1ea9fe;
	constexpr uint64_t CCSGameRulesProxy_m_bFreezePeriod = 0x91f022d6ce8d1ed7;
	constexpr uint64_t CCSGameRulesProxy_m_bGameRestart = 0x255ec0d0ef51dd36;
	constexpr uint64_t CCSGameRulesProxy_m_bHasMatchStarted = 0x6ef8cc403f867603;
	constexpr uint64_t CCSGameRulesProxy_m_bIsDroppingItems = 0x9cb02707a6197304;
	constexpr uint64_t CCSGameRulesProxy_m_bIsQuestEligible = 0xe715333b62fe9c50;
	constexpr uint64_t CCSGameRulesProxy_m_bIsQueuedMatchmaking = 0x4e38a55fabb536e0;
	constexpr uint64_t CCSGameRulesProxy_m_bIsValveDS = 0x1ec937aba021202f;
	constexpr uint64_t CCSGameRulesProxy_m_bLogoMap = 0x730c31c0456d517c;
	constexpr uint64_t CCSGameRulesProxy_m_bMapHasBombTarget = 0x1f1c829ee1e37b65;
	constexpr uint64_t CCSGameRulesProxy_m_bMapHasBuyZone = 0x5c645761d9c63f11;
	constexpr uint64_t CCSGameRulesProxy_m_bMapHasRescueZone = 0x698fd3e944d7fbe7;
	constexpr uint64_t CCSGameRulesProxy_m_bMatchWaitingForResume = 0xe39826cd5495e950;
	constexpr uint64_t CCSGameRulesProxy_m_bPlayAllStepSoundsOnServer = 0xebe3080b81e8f4e4;
	constexpr uint64_t CCSGameRulesProxy_m_bRoundInProgress = 0xb8ad83a108785bbe;
	constexpr uint64_t CCSGameRulesProxy_m_bTCantBuy = 0x80c70aabf81d9f5d;
	constexpr uint64_t CCSGameRulesProxy_m_bTechnicalTimeOut = 0xc37e121c6c367ddf;
	constexpr uint64_t CCSGameRulesProxy_m_bTerroristTimeOutActive = 0x8a06c1406dcdf82e;
	constexpr uint64_t CCSGameRulesProxy_m_bWarmupPeriod = 0x7416c15f44526b9f;
	constexpr uint64_t CCSGameRulesProxy_m_eRoundWinReason = 0x9b6e1c322974245e;
	constexpr uint64_t CCSGameRulesProxy_m_fMatchStartTime = 0xa22d75a116d42fa9;
	constexpr uint64_t CCSGameRulesProxy_m_fRoundStartTime = 0x30d89799ce9f402a;
	constexpr uint64_t CCSGameRulesProxy_m_fWarmupPeriodEnd = 0xd42caf44a7f00fc2;
	constexpr uint64_t CCSGameRulesProxy_m_fWarmupPeriodStart = 0xf293b0edb69839d2;
	constexpr uint64_t CCSGameRulesProxy_m_flCMMItemDropRevealEndTime = 0xefc6d89421295a58;
	constexpr uint64_t CCSGameRulesProxy_m_flCMMItemDropRevealStartTime = 0x4c56976af4d449a8;
	constexpr uint64_t CCSGameRulesProxy_m_flCTTimeOutRemaining = 0x5fe6bb3412c64ab;
	constexpr uint64_t CCSGameRulesProxy_m_flDMBonusStartTime = 0x5f2896883507ffaf;
	constexpr uint64_t CCSGameRulesProxy_m_flDMBonusTimeLength = 0x1079fa5be426480a;
	constexpr uint64_t CCSGameRulesProxy_m_flGameStartTime = 0x8cd2d2f2213dcc20;
	constexpr uint64_t CCSGameRulesProxy_m_flGuardianBuyUntilTime = 0xd505e37a261f53de;
	constexpr uint64_t CCSGameRulesProxy_m_flNextRespawnWave = 0x8dc2804fd2789150;
	constexpr uint64_t CCSGameRulesProxy_m_flRestartRoundTime = 0x2181a0a83f7d592b;
	constexpr uint64_t CCSGameRulesProxy_m_flSpawnSelectionTimeEnd = 0x25e13214ad7fce68;
	constexpr uint64_t CCSGameRulesProxy_m_flSpawnSelectionTimeLoadout = 0x53e9219aca0a4bc;
	constexpr uint64_t CCSGameRulesProxy_m_flSpawnSelectionTimeStart = 0x371342558154bbd5;
	constexpr uint64_t CCSGameRulesProxy_m_flSurvivalStartTime = 0xe160bf51d6f1fbac;
	constexpr uint64_t CCSGameRulesProxy_m_flTabletHexOriginX = 0xf3b6edbd71b744cf;
	constexpr uint64_t CCSGameRulesProxy_m_flTabletHexOriginY = 0x7ac94bc99753df56;
	constexpr uint64_t CCSGameRulesProxy_m_flTabletHexSize = 0x9e16f9d51751345;
	constexpr uint64_t CCSGameRulesProxy_m_flTerroristTimeOutRemaining = 0xb0913458a66f7f31;
	constexpr uint64_t CCSGameRulesProxy_m_gamePhase = 0xa291ba7a923788d1;
	constexpr uint64_t CCSGameRulesProxy_m_iActiveAssassinationTargetMissionID = 0x83dcfce96ade5a48;
	constexpr uint64_t CCSGameRulesProxy_m_iBombSite = 0x88ee51dac42bbad1;
	constexpr uint64_t CCSGameRulesProxy_m_iFirstSecondHalfRound = 0xbf8ad427864cc8ea;
	constexpr uint64_t CCSGameRulesProxy_m_iHostagesRemaining = 0x3cd2f6536a825b0d;
	constexpr uint64_t CCSGameRulesProxy_m_iMatchStats_PlayersAlive_CT = 0x30fe67b5fc284277;
	constexpr uint64_t CCSGameRulesProxy_m_iMatchStats_PlayersAlive_T = 0xfdb5899bd5d36b67;
	constexpr uint64_t CCSGameRulesProxy_m_iMatchStats_RoundResults = 0x46c3c5a505e9dc14;
	constexpr uint64_t CCSGameRulesProxy_m_iNumConsecutiveCTLoses = 0xafafcb6e23d46699;
	constexpr uint64_t CCSGameRulesProxy_m_iNumConsecutiveTerroristLoses = 0xca080bc646bab965;
	constexpr uint64_t CCSGameRulesProxy_m_iNumGunGameProgressiveWeaponsCT = 0x1fcb56a45dabeea;
	constexpr uint64_t CCSGameRulesProxy_m_iNumGunGameProgressiveWeaponsT = 0x75f73693089722ba;
	constexpr uint64_t CCSGameRulesProxy_m_iPlayerSpawnHexIndices = 0xc81004c11704df2b;
	constexpr uint64_t CCSGameRulesProxy_m_iRoundTime = 0xccc89c9326591123;
	constexpr uint64_t CCSGameRulesProxy_m_iRoundWinStatus = 0xf1c6403a4877fa65;
	constexpr uint64_t CCSGameRulesProxy_m_iSpectatorSlotCount = 0xdb2ca60965343576;
	constexpr uint64_t CCSGameRulesProxy_m_nCTTimeOuts = 0xe5bad59e9721da37;
	constexpr uint64_t CCSGameRulesProxy_m_nEndMatchMapGroupVoteOptions = 0x412c019233632696;
	constexpr uint64_t CCSGameRulesProxy_m_nEndMatchMapGroupVoteTypes = 0xcc89f167f4293392;
	constexpr uint64_t CCSGameRulesProxy_m_nEndMatchMapVoteWinner = 0x34b2550fddc88170;
	constexpr uint64_t CCSGameRulesProxy_m_nGuardianModeSpecialKillsRemaining = 0xd988378fa2670f95;
	constexpr uint64_t CCSGameRulesProxy_m_nGuardianModeSpecialWeaponNeeded = 0xcdb4a8be0d21da82;
	constexpr uint64_t CCSGameRulesProxy_m_nGuardianModeWaveNumber = 0xe46d4c00ee5df295;
	constexpr uint64_t CCSGameRulesProxy_m_nHalloweenMaskListSeed = 0xf7b4cae404e21437;
	constexpr uint64_t CCSGameRulesProxy_m_nMatchSeed = 0xef3687701da47f64;
	constexpr uint64_t CCSGameRulesProxy_m_nNextMapInMapgroup = 0x69481f5b4eb22441;
	constexpr uint64_t CCSGameRulesProxy_m_nOvertimePlaying = 0xc75fcbc69ed0c74b;
	constexpr uint64_t CCSGameRulesProxy_m_nQueuedMatchmakingMode = 0xebcd0c25444ddbf3;
	constexpr uint64_t CCSGameRulesProxy_m_nTerroristTimeOuts = 0x769e67fd0bc11e94;
	constexpr uint64_t CCSGameRulesProxy_m_nTournamentPredictionsPct = 0x5f5245ffd5266028;
	constexpr uint64_t CCSGameRulesProxy_m_numBestOfMaps = 0x3f44fdfd0117a988;
	constexpr uint64_t CCSGameRulesProxy_m_numGlobalGifters = 0x8cbcc92b9377afc5;
	constexpr uint64_t CCSGameRulesProxy_m_numGlobalGiftsGiven = 0xd8325cc5531f3c81;
	constexpr uint64_t CCSGameRulesProxy_m_numGlobalGiftsPeriodSeconds = 0x4086d94ca6ed365c;
	constexpr uint64_t CCSGameRulesProxy_m_roundData_playerPositions = 0xbf09ea9a79898dc0;
	constexpr uint64_t CCSGameRulesProxy_m_roundData_playerTeams = 0xde645fbd15179a0b;
	constexpr uint64_t CCSGameRulesProxy_m_roundData_playerXuids = 0x551d72f0fcc45712;
	constexpr uint64_t CCSGameRulesProxy_m_spawnStage = 0x92ff3aa5ffb16c86;
	constexpr uint64_t CCSGameRulesProxy_m_szMatchStatTxt = 0x335b13274492f5f6;
	constexpr uint64_t CCSGameRulesProxy_m_szTournamentEventName = 0x2a08b54bbc404d8b;
	constexpr uint64_t CCSGameRulesProxy_m_szTournamentEventStage = 0xb7715ee78c7d1514;
	constexpr uint64_t CCSGameRulesProxy_m_szTournamentPredictionsTxt = 0x10a8b4c9fa112b19;
	constexpr uint64_t CCSGameRulesProxy_m_timeUntilNextPhaseStarts = 0xef1add771bb82ee5;
	constexpr uint64_t CCSGameRulesProxy_m_totalRoundsPlayed = 0xea1fe48faa560cb4;
	constexpr uint64_t CCSGameRulesProxy_m_unDMBonusWeaponLoadoutSlot = 0x7e62e44665efa1cb;
	constexpr uint64_t CCSGameRulesProxy_m_vecPlayAreaMaxs = 0x72c2e4c33e20ef39;
	constexpr uint64_t CCSGameRulesProxy_m_vecPlayAreaMins = 0x7cb274f52479ba51;
	constexpr uint64_t CCSPlayerResource_m_bControllingBot = 0xe18c8ec0ad4c68ac;
	constexpr uint64_t CCSPlayerResource_m_bEndMatchNextMapAllVoted = 0xbeddd1ca30a76a7c;
	constexpr uint64_t CCSPlayerResource_m_bHasCommunicationAbuseMute = 0xc7c245aad5a06174;
	constexpr uint64_t CCSPlayerResource_m_bHasDefuser = 0x14b077d60e78be90;
	constexpr uint64_t CCSPlayerResource_m_bHasHelmet = 0x86b6c8370389cb34;
	constexpr uint64_t CCSPlayerResource_m_bHostageAlive = 0x2011e1b6eb391a76;
	constexpr uint64_t CCSPlayerResource_m_bombsiteCenterA = 0xbeb8c9525a14af1c;
	constexpr uint64_t CCSPlayerResource_m_bombsiteCenterB = 0x873ad92ee5b803aa;
	constexpr uint64_t CCSPlayerResource_m_hostageRescueX = 0x5e65b8062320c6e1;
	constexpr uint64_t CCSPlayerResource_m_hostageRescueY = 0x35a3730969ff5d83;
	constexpr uint64_t CCSPlayerResource_m_hostageRescueZ = 0x73351bcbe9b4e36;
	constexpr uint64_t CCSPlayerResource_m_iArmor = 0x238fa83913454699;
	constexpr uint64_t CCSPlayerResource_m_iBotDifficulty = 0x15125725e64c3d5c;
	constexpr uint64_t CCSPlayerResource_m_iCashSpentThisRound = 0xe17e8023ac4c2f22;
	constexpr uint64_t CCSPlayerResource_m_iCompTeammateColor = 0xe9d0b99bdfbed58c;
	constexpr uint64_t CCSPlayerResource_m_iCompetitiveRankType = 0xf71c8e647e1096d9;
	constexpr uint64_t CCSPlayerResource_m_iCompetitiveRanking = 0x638f9b3144c8d975;
	constexpr uint64_t CCSPlayerResource_m_iCompetitiveWins = 0x2bb87a6d33930bca;
	constexpr uint64_t CCSPlayerResource_m_iControlledByPlayer = 0x5a9feddcf645e2cf;
	constexpr uint64_t CCSPlayerResource_m_iControlledPlayer = 0xba76ddc1acd22cd;
	constexpr uint64_t CCSPlayerResource_m_iGunGameLevel = 0xbb07b10de80522be;
	constexpr uint64_t CCSPlayerResource_m_iHostageEntityIDs = 0x8d75ea7caf56ab5;
	constexpr uint64_t CCSPlayerResource_m_iLifetimeEnd = 0x7c576451e7b1a02a;
	constexpr uint64_t CCSPlayerResource_m_iLifetimeStart = 0x829f2cb9ff6f874;
	constexpr uint64_t CCSPlayerResource_m_iMVPs = 0xea0012f57d705248;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_3k_Total = 0x3bb20222977388fd;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_4k_Total = 0x3e2c1962a9efce0b;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_5k_Total = 0xa5e4082a14041ee9;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_Assists_Total = 0x59498634920ba031;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_CashEarned_Total = 0x1316182f40b17ef0;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_Damage_Total = 0x3ad45d857d293259;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_Deaths_Total = 0x5f651c374efee12e;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_EnemiesFlashed_Total = 0x842e7218e689af71;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_EquipmentValue_Total = 0x4e18ee0e3fed2a9d;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_HeadShotKills_Total = 0x2f33d518faf390be;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_KillReward_Total = 0xe8fdf0428a6bcdc8;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_Kills_Total = 0x6ae24d74bf7f7ff2;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_LiveTime_Total = 0xdeeff90e3fd29616;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_Objective_Total = 0xe00258988124d67c;
	constexpr uint64_t CCSPlayerResource_m_iMatchStats_UtilityDamage_Total = 0x6baf27349f504ea4;
	constexpr uint64_t CCSPlayerResource_m_iPlayerC4 = 0x1ddcb1a50fc6fd0c;
	constexpr uint64_t CCSPlayerResource_m_iPlayerVIP = 0x1d70694cab758613;
	constexpr uint64_t CCSPlayerResource_m_iScore = 0x88fa70926043128b;
	constexpr uint64_t CCSPlayerResource_m_iTotalCashSpent = 0x4cfacf4f59ba4166;
	constexpr uint64_t CCSPlayerResource_m_isHostageFollowingSomeone = 0x3c1db4c30a935e04;
	constexpr uint64_t CCSPlayerResource_m_nActiveCoinRank = 0x72df80f8ade94187;
	constexpr uint64_t CCSPlayerResource_m_nCharacterDefIndex = 0x4a76894c0add77ef;
	constexpr uint64_t CCSPlayerResource_m_nEndMatchNextMapVotes = 0x1c4c9cec3b0f4ce8;
	constexpr uint64_t CCSPlayerResource_m_nMusicID = 0x6dd2bc1082fd7841;
	constexpr uint64_t CCSPlayerResource_m_nPersonaDataPublicCommendsFriendly = 0xada45e900715cfde;
	constexpr uint64_t CCSPlayerResource_m_nPersonaDataPublicCommendsLeader = 0xc41e8bf3705d196b;
	constexpr uint64_t CCSPlayerResource_m_nPersonaDataPublicCommendsTeacher = 0x43b21169264974a3;
	constexpr uint64_t CCSPlayerResource_m_nPersonaDataPublicLevel = 0x383430937dcfdab2;
	constexpr uint64_t CCSPlayerResource_m_szClan = 0xdaf903af0abb0594;
	constexpr uint64_t CCSPlayerResource_m_szCrosshairCodes = 0x11bf80d0d2006ce2;
	constexpr uint64_t CCSPlayer_m_ArmorValue = 0xfe6bebbceda3cbb3;
	constexpr uint64_t CCSPlayer_m_EquippedLoadoutItemDefIndices = 0x233b64f2d0e63d10;
	constexpr uint64_t CCSPlayer_m_RetakesMVPBoostExtraUtility = 0x14bd8d142884db2;
	constexpr uint64_t CCSPlayer_m_angEyeAngles = 0xdbbdfd1b64b39502;
	constexpr uint64_t CCSPlayer_m_angEyeAngles_0_ = 0xb56d5f3acd8fafbe;
	constexpr uint64_t CCSPlayer_m_angEyeAngles_1_ = 0x95ede5bbbd93ba6a;
	constexpr uint64_t CCSPlayer_m_bCanControlObservedBot = 0x201f70bc74392a0;
	constexpr uint64_t CCSPlayer_m_bCanMoveDuringFreezePeriod = 0x67c4a2676c36e406;
	constexpr uint64_t CCSPlayer_m_bDuckOverride = 0x796810d82f123ad1;
	constexpr uint64_t CCSPlayer_m_bGunGameImmunity = 0x207381e078d3636c;
	constexpr uint64_t CCSPlayer_m_bHasControlledBotThisRound = 0x1f8fa8d6ea3b5fde;
	constexpr uint64_t CCSPlayer_m_bHasDefuser = 0x2f6bdf8912c17440;
	constexpr uint64_t CCSPlayer_m_bHasHeavyArmor = 0xd95ee6ed9c52b515;
	constexpr uint64_t CCSPlayer_m_bHasHelmet = 0xf41c154f47a74cde;
	constexpr uint64_t CCSPlayer_m_bHasMovedSinceSpawn = 0xfe28afeeccb28f26;
	constexpr uint64_t CCSPlayer_m_bHasNightVision = 0x1ec79e4853a8cd5f;
	constexpr uint64_t CCSPlayer_m_bHasParachute = 0x6978a81d800fd0b9;
	constexpr uint64_t CCSPlayer_m_bHideTargetID = 0xc9f3e40a5ac739b4;
	constexpr uint64_t CCSPlayer_m_bHud_MiniScoreHidden = 0xf6b722acb91e61df;
	constexpr uint64_t CCSPlayer_m_bHud_RadarHidden = 0xae8ec0a72af4c0a6;
	constexpr uint64_t CCSPlayer_m_bInBombZone = 0xe13b4341d762eb62;
	constexpr uint64_t CCSPlayer_m_bInBuyZone = 0xd27c172a0f330a97;
	constexpr uint64_t CCSPlayer_m_bInHostageRescueZone = 0x77392a3fb31feaae;
	constexpr uint64_t CCSPlayer_m_bInNoDefuseArea = 0x86e3369b8e2367dc;
	constexpr uint64_t CCSPlayer_m_bIsAssassinationTarget = 0xba6c5af1746e2c3;
	constexpr uint64_t CCSPlayer_m_bIsControllingBot = 0xd5687a521f8a279b;
	constexpr uint64_t CCSPlayer_m_bIsDefusing = 0x659f6d009fa3f459;
	constexpr uint64_t CCSPlayer_m_bIsGrabbingHostage = 0x85c1919e5e2f2879;
	constexpr uint64_t CCSPlayer_m_bIsHoldingLookAtWeapon = 0xee2c0362ec495646;
	constexpr uint64_t CCSPlayer_m_bIsLookingAtWeapon = 0x4df5c17d4758a6b0;
	constexpr uint64_t CCSPlayer_m_bIsPlayerGhost = 0xefde25cbf33d06aa;
	constexpr uint64_t CCSPlayer_m_bIsRescuing = 0x547ad23080c7755e;
	constexpr uint64_t CCSPlayer_m_bIsRespawningForDMBonus = 0x28fdd055e26eae52;
	constexpr uint64_t CCSPlayer_m_bIsScoped = 0x47c50812d01d6e96;
	constexpr uint64_t CCSPlayer_m_bIsSpawnRappelling = 0x1d9c9e0c446c6c2c;
	constexpr uint64_t CCSPlayer_m_bIsWalking = 0xa99302c43a012b5;
	constexpr uint64_t CCSPlayer_m_bKilledByTaser = 0x18c5946c0f3fd310;
	constexpr uint64_t CCSPlayer_m_bMadeFinalGunGameProgressiveKill = 0x6ddfdb2d8370e2cc;
	constexpr uint64_t CCSPlayer_m_bNightVisionOn = 0x98c97bf04fc1e878;
	constexpr uint64_t CCSPlayer_m_bPlayerDominated = 0x75a9d3c1b8521135;
	constexpr uint64_t CCSPlayer_m_bPlayerDominatingMe = 0xe958a18cd200e7a5;
	constexpr uint64_t CCSPlayer_m_bResumeZoom = 0xa88e7d06203bd741;
	constexpr uint64_t CCSPlayer_m_bRetakesHasDefuseKit = 0x97916087a89c9545;
	constexpr uint64_t CCSPlayer_m_bRetakesMVPLastRound = 0x530f5d4b882c9313;
	constexpr uint64_t CCSPlayer_m_bStrafing = 0x3fed8a4a3d2a10d0;
	constexpr uint64_t CCSPlayer_m_bWaitForNoAttack = 0x70edbda12d2b631;
	constexpr uint64_t CCSPlayer_m_cycleLatch = 0x1524435d924676de;
	constexpr uint64_t CCSPlayer_m_fImmuneToGunGameDamageTime = 0xfc45aa219d755815;
	constexpr uint64_t CCSPlayer_m_fMolotovDamageTime = 0x26c8ac3bd749784d;
	constexpr uint64_t CCSPlayer_m_fMolotovUseTime = 0xd6deccbadc277945;
	constexpr uint64_t CCSPlayer_m_flAutoMoveStartTime = 0xa6b14666f1ec6eab;
	constexpr uint64_t CCSPlayer_m_flAutoMoveTargetTime = 0xc441ef9b5add5932;
	constexpr uint64_t CCSPlayer_m_flDetectedByEnemySensorTime = 0x2478b5b0a86903a2;
	constexpr uint64_t CCSPlayer_m_flFlashDuration = 0x82e37eab0fc3f0fc;
	constexpr uint64_t CCSPlayer_m_flFlashMaxAlpha = 0x331ac96a90104002;
	constexpr uint64_t CCSPlayer_m_flGroundAccelLinearFracLastTime = 0xae1906d340f122aa;
	constexpr uint64_t CCSPlayer_m_flGuardianTooFarDistFrac = 0xafd3999de22e49e9;
	constexpr uint64_t CCSPlayer_m_flHealthShotBoostExpirationTime = 0xb4ac66fe425588fd;
	constexpr uint64_t CCSPlayer_m_flLastExoJumpTime = 0x26cd6400c89c136d;
	constexpr uint64_t CCSPlayer_m_flLowerBodyYawTarget = 0x30b741a187422adf;
	constexpr uint64_t CCSPlayer_m_flProgressBarStartTime = 0x97700b5c19d765fa;
	constexpr uint64_t CCSPlayer_m_flStamina = 0x2e26fe4c706ed0e0;
	constexpr uint64_t CCSPlayer_m_flThirdpersonRecoil = 0x6dc2bb74cfe9b07;
	constexpr uint64_t CCSPlayer_m_flVelocityModifier = 0x3e65f235915b750f;
	constexpr uint64_t CCSPlayer_m_hCarriedHostage = 0xea189eea4924;
	constexpr uint64_t CCSPlayer_m_hCarriedHostageProp = 0xec68ee586e1c3cdb;
	constexpr uint64_t CCSPlayer_m_hPlayerPing = 0x1b4f60137b48a228;
	constexpr uint64_t CCSPlayer_m_hRagdoll = 0xf5c51e49a9decc1;
	constexpr uint64_t CCSPlayer_m_hSurvivalAssassinationTarget = 0x8a9f781cf6607f6e;
	constexpr uint64_t CCSPlayer_m_iAccount = 0x4dd38de97b052785;
	constexpr uint64_t CCSPlayer_m_iAddonBits = 0x5041a2aac087a3da;
	constexpr uint64_t CCSPlayer_m_iBlockingUseActionInProgress = 0xf40d7b79fb23f749;
	constexpr uint64_t CCSPlayer_m_iClass = 0x2fa25b500656c517;
	constexpr uint64_t CCSPlayer_m_iControlledBotEntIndex = 0x8ff2d9e6e7611318;
	constexpr uint64_t CCSPlayer_m_iDirection = 0x8fab24791d8cf025;
	constexpr uint64_t CCSPlayer_m_iGunGameProgressiveWeaponIndex = 0x8cb1e8bc0d6572f4;
	constexpr uint64_t CCSPlayer_m_iMatchStats_Assists = 0xdc6a93d3cb68be8f;
	constexpr uint64_t CCSPlayer_m_iMatchStats_CashEarned = 0x18b0eb7a1a467a2;
	constexpr uint64_t CCSPlayer_m_iMatchStats_Damage = 0xcbcd6c9bde529d28;
	constexpr uint64_t CCSPlayer_m_iMatchStats_Deaths = 0xe497d5a5c8abbcab;
	constexpr uint64_t CCSPlayer_m_iMatchStats_EnemiesFlashed = 0x72b63de0b0a73200;
	constexpr uint64_t CCSPlayer_m_iMatchStats_EquipmentValue = 0xc3a262a7db92dca5;
	constexpr uint64_t CCSPlayer_m_iMatchStats_HeadShotKills = 0x543aaea16f87175d;
	constexpr uint64_t CCSPlayer_m_iMatchStats_KillReward = 0xdaa6c813b88bf38;
	constexpr uint64_t CCSPlayer_m_iMatchStats_Kills = 0x5a320a9edb4fc5f6;
	constexpr uint64_t CCSPlayer_m_iMatchStats_LiveTime = 0x6b6c2742114aae42;
	constexpr uint64_t CCSPlayer_m_iMatchStats_MoneySaved = 0xb110c0a6bf45b433;
	constexpr uint64_t CCSPlayer_m_iMatchStats_Objective = 0x51bbbf8ff260591;
	constexpr uint64_t CCSPlayer_m_iMatchStats_UtilityDamage = 0x6b61e93f907e4925;
	constexpr uint64_t CCSPlayer_m_iMoveState = 0x4b0f6ee9cb537df0;
	constexpr uint64_t CCSPlayer_m_iNumGunGameKillsWithCurrentWeapon = 0xaa6c48767ca5d5dd;
	constexpr uint64_t CCSPlayer_m_iNumGunGameTRKillPoints = 0xb975143aba0187ae;
	constexpr uint64_t CCSPlayer_m_iNumRoundKills = 0xe53c7b7cacd9fe56;
	constexpr uint64_t CCSPlayer_m_iNumRoundKillsHeadshots = 0x7e558d9f6766c4ff;
	constexpr uint64_t CCSPlayer_m_iPlayerState = 0x8b57c8e941e84160;
	constexpr uint64_t CCSPlayer_m_iPrimaryAddon = 0xa83c8078f4449921;
	constexpr uint64_t CCSPlayer_m_iProgressBarDuration = 0x84db5ea187d9cca5;
	constexpr uint64_t CCSPlayer_m_iRetakesMVPBoostItem = 0xc4ae84c68dcb83fc;
	constexpr uint64_t CCSPlayer_m_iRetakesOffering = 0xc4a9152b672ae199;
	constexpr uint64_t CCSPlayer_m_iRetakesOfferingCard = 0xea7b12c06af6946a;
	constexpr uint64_t CCSPlayer_m_iSecondaryAddon = 0xa27cebcbd5ec23f9;
	constexpr uint64_t CCSPlayer_m_iShotsFired = 0x86b0584bcde8cb73;
	constexpr uint64_t CCSPlayer_m_iStartAccount = 0xedb1aa1c8d7cd88a;
	constexpr uint64_t CCSPlayer_m_iThrowGrenadeCounter = 0x8cfadbab6c9777c3;
	constexpr uint64_t CCSPlayer_m_iWeaponPurchasesThisMatch = 0xb8df04b2c14d3e94;
	constexpr uint64_t CCSPlayer_m_iWeaponPurchasesThisRound = 0xee2ebfbee0b37b6;
	constexpr uint64_t CCSPlayer_m_isCurrentGunGameLeader = 0x9d0e26da33b7adc5;
	constexpr uint64_t CCSPlayer_m_isCurrentGunGameTeamLeader = 0xf815875fd6af53f4;
	constexpr uint64_t CCSPlayer_m_nDeathCamMusic = 0x74359ae69639c6aa;
	constexpr uint64_t CCSPlayer_m_nHeavyAssaultSuitCooldownRemaining = 0xfd12c0bac3dcd082;
	constexpr uint64_t CCSPlayer_m_nIsAutoMounting = 0xee0fbae951d66d04;
	constexpr uint64_t CCSPlayer_m_nLastConcurrentKilled = 0x8ef29d539ca2ff67;
	constexpr uint64_t CCSPlayer_m_nLastKillerIndex = 0xabd6b7afd929ef3b;
	constexpr uint64_t CCSPlayer_m_nNumFastDucks = 0xc3ad45b0c689f8c5;
	constexpr uint64_t CCSPlayer_m_nQuestProgressReason = 0x5ffb2c0c1decc0fe;
	constexpr uint64_t CCSPlayer_m_nSurvivalTeam = 0x713d554d3c4551b3;
	constexpr uint64_t CCSPlayer_m_passiveItems = 0x987dde0faf6da654;
	constexpr uint64_t CCSPlayer_m_rank = 0xcb8960b0275e5c6;
	constexpr uint64_t CCSPlayer_m_szArmsModel = 0x9bc7301cce2f7424;
	constexpr uint64_t CCSPlayer_m_totalHitsOnServer = 0xe036208d2cdfb384;
	constexpr uint64_t CCSPlayer_m_unActiveQuestId = 0x8e9a2e3038728e3f;
	constexpr uint64_t CCSPlayer_m_unCurrentEquipmentValue = 0x2fba76cca01d97e9;
	constexpr uint64_t CCSPlayer_m_unFreezetimeEndEquipmentValue = 0xac289cea38a7b623;
	constexpr uint64_t CCSPlayer_m_unMusicID = 0xab6a0ca8057c9d8b;
	constexpr uint64_t CCSPlayer_m_unPlayerTvControlFlags = 0x3266a9b1a523fcdb;
	constexpr uint64_t CCSPlayer_m_unRoundStartEquipmentValue = 0xdd7da3d4bd0cf36d;
	constexpr uint64_t CCSPlayer_m_unTotalRoundDamageDealt = 0x34fea20a9996a68;
	constexpr uint64_t CCSPlayer_m_vecAutomoveTargetEnd = 0x9df742145ee93d76;
	constexpr uint64_t CCSPlayer_m_vecOrigin = 0x103177dd78fc6578;
	constexpr uint64_t CCSPlayer_m_vecOrigin_2_ = 0xb99206f7605a0e23;
	constexpr uint64_t CCSPlayer_m_vecPlayerPatchEconIndices = 0xad956766668dbdc3;
	constexpr uint64_t CCSPlayer_m_vecSpawnRappellingRopeOrigin = 0x51742a1af6734f4c;
	constexpr uint64_t CCSRagdoll_m_bClientSideAnimation = 0x606344aa3e12f0;
	constexpr uint64_t CCSRagdoll_m_bDiedAirborne = 0xa25f0bcec99f11eb;
	constexpr uint64_t CCSRagdoll_m_flAbsYaw = 0x20f53d389735447d;
	constexpr uint64_t CCSRagdoll_m_flDeathYaw = 0xa14801fad4b76028;
	constexpr uint64_t CCSRagdoll_m_hPlayer = 0x2f6d5b065ee18589;
	constexpr uint64_t CCSRagdoll_m_iDeathFrame = 0x6191160ef87fe70f;
	constexpr uint64_t CCSRagdoll_m_iDeathPose = 0x9df475fa731876ca;
	constexpr uint64_t CCSRagdoll_m_iTeamNum = 0x81746369b799624b;
	constexpr uint64_t CCSRagdoll_m_nForceBone = 0x2d2e26f76d2c22b5;
	constexpr uint64_t CCSRagdoll_m_nModelIndex = 0x30a3cf18318b8dc1;
	constexpr uint64_t CCSRagdoll_m_vecForce = 0x52e75f07e25a898a;
	constexpr uint64_t CCSRagdoll_m_vecOrigin = 0xb8eb8990d706fd80;
	constexpr uint64_t CCSRagdoll_m_vecRagdollOrigin = 0x22c9b42119c4a871;
	constexpr uint64_t CCSRagdoll_m_vecRagdollVelocity = 0xb983c167897e2748;
	constexpr uint64_t CCascadeLight_m_LightColor = 0xfc51850169d8a7b3;
	constexpr uint64_t CCascadeLight_m_LightColorScale = 0x9534a515858b8826;
	constexpr uint64_t CCascadeLight_m_bEnabled = 0x2c6541cd95fb942d;
	constexpr uint64_t CCascadeLight_m_bUseLightEnvAngles = 0xd66084c335f1e6b2;
	constexpr uint64_t CCascadeLight_m_envLightShadowDirection = 0x7f4cba061e7efe0d;
	constexpr uint64_t CCascadeLight_m_flMaxShadowDist = 0x3a259472c25e6d52;
	constexpr uint64_t CCascadeLight_m_shadowDirection = 0x8d16992d1fba8a19;
	constexpr uint64_t CChicken_m_jumpedThisFrame = 0x13f47ffa6deee6ed;
	constexpr uint64_t CChicken_m_leader = 0xda1824385b15b689;
	constexpr uint64_t CColorCorrectionVolume_m_FadeDuration = 0x10552899ff5cfe23;
	constexpr uint64_t CColorCorrectionVolume_m_MaxWeight = 0x56dc2c78561e6aeb;
	constexpr uint64_t CColorCorrectionVolume_m_Weight = 0xf4871c58dfc4e115;
	constexpr uint64_t CColorCorrectionVolume_m_bEnabled = 0xeb393f3d91b1239;
	constexpr uint64_t CColorCorrectionVolume_m_lookupFilename = 0xfa38a2b35676e59e;
	constexpr uint64_t CColorCorrection_m_bClientSide = 0x79d2a9b94cb2d6b3;
	constexpr uint64_t CColorCorrection_m_bEnabled = 0xdb09effb2826bf5a;
	constexpr uint64_t CColorCorrection_m_bExclusive = 0x61401adce63e171f;
	constexpr uint64_t CColorCorrection_m_bMaster = 0x6e13c82758e5bd2c;
	constexpr uint64_t CColorCorrection_m_flCurWeight = 0x28636d7174bb3792;
	constexpr uint64_t CColorCorrection_m_flFadeInDuration = 0x80364c6e08b98fe1;
	constexpr uint64_t CColorCorrection_m_flFadeOutDuration = 0xb31554dc16a02a42;
	constexpr uint64_t CColorCorrection_m_flMaxWeight = 0xfea4916eef35245;
	constexpr uint64_t CColorCorrection_m_maxFalloff = 0x7c747a7096516197;
	constexpr uint64_t CColorCorrection_m_minFalloff = 0x7060159ddd0df8ec;
	constexpr uint64_t CColorCorrection_m_netLookupFilename = 0xe644da6462f16e4e;
	constexpr uint64_t CColorCorrection_m_vecOrigin = 0x4abe96a031926d87;
	constexpr uint64_t CDangerZoneController_m_DangerZones = 0x3972f85d5c86c278;
	constexpr uint64_t CDangerZoneController_m_bDangerZoneControllerEnabled = 0x9b07403f251821ce;
	constexpr uint64_t CDangerZoneController_m_bMissionControlledExplosions = 0x36ae38a47e16405;
	constexpr uint64_t CDangerZoneController_m_flFinalExpansionTime = 0xeb90288e4f24b0d;
	constexpr uint64_t CDangerZoneController_m_flStartTime = 0x9526ab21d5b6d07;
	constexpr uint64_t CDangerZoneController_m_flWaveEndTimes = 0x200fdf1298714736;
	constexpr uint64_t CDangerZoneController_m_hTheFinalZone = 0x750379c69665c8df;
	constexpr uint64_t CDangerZoneController_m_vecEndGameCircleEnd = 0xb2144e84dd71e9c4;
	constexpr uint64_t CDangerZoneController_m_vecEndGameCircleStart = 0xa2b7954ddf3d3ae;
	constexpr uint64_t CDangerZone_m_flBombLaunchTime = 0x4b9d795d9d1ac89d;
	constexpr uint64_t CDangerZone_m_flExtraRadius = 0x6ef267ab19ac7113;
	constexpr uint64_t CDangerZone_m_flExtraRadiusStartTime = 0xb02db0d2045ff92d;
	constexpr uint64_t CDangerZone_m_flExtraRadiusTotalLerpTime = 0x8cce496aeb5c13ba;
	constexpr uint64_t CDangerZone_m_iWave = 0xdbc6627cc5cae494;
	constexpr uint64_t CDangerZone_m_nDropOrder = 0xaa84421ee8a8a588;
	constexpr uint64_t CDangerZone_m_vecDangerZoneOriginStartedAt = 0xb2f4dbff7cac4d49;
	constexpr uint64_t CDrone_m_bPilotTakeoverAllowed = 0x78f3b077903a3c1f;
	constexpr uint64_t CDrone_m_hCurrentPilot = 0x3267c5d1aab180d1;
	constexpr uint64_t CDrone_m_hDeliveryCargo = 0xec8093ca39628a3b;
	constexpr uint64_t CDrone_m_hMoveToThisEntity = 0x9310fd5d74d4925;
	constexpr uint64_t CDrone_m_hPotentialCargo = 0x55f8d2cf9a244562;
	constexpr uint64_t CDrone_m_vecTagIncrements_0_ = 0xf767a77137e1483e;
	constexpr uint64_t CDrone_m_vecTagPositions_0_ = 0x4addcd9e566d1614;
	constexpr uint64_t CDronegun_m_bHasTarget = 0x8fc328fef60e10;
	constexpr uint64_t CDronegun_m_iHealth = 0x6a032ebaaed4011c;
	constexpr uint64_t CDronegun_m_vecAttentionTarget = 0x2c4cb51e560fa7a9;
	constexpr uint64_t CDronegun_m_vecTargetOffset = 0x6fd714a7391ab04b;
	constexpr uint64_t CDynamicLight_m_Exponent = 0x1beb20a94d41ad58;
	constexpr uint64_t CDynamicLight_m_Flags = 0x879686e8ae4684a6;
	constexpr uint64_t CDynamicLight_m_InnerAngle = 0x5cccc21a4fdbfdfd;
	constexpr uint64_t CDynamicLight_m_LightStyle = 0x68637f133d8c85fa;
	constexpr uint64_t CDynamicLight_m_OuterAngle = 0x57b7571bf78e16e5;
	constexpr uint64_t CDynamicLight_m_Radius = 0x827ac22812ec1838;
	constexpr uint64_t CDynamicLight_m_SpotRadius = 0x7ad5aea439b5ce70;
	constexpr uint64_t CDynamicProp_m_bShouldGlow = 0x2815feb0a57b6113;
	constexpr uint64_t CDynamicProp_m_bUseHitboxesForRenderBox = 0x9cc83ad2ba59baa6;
	constexpr uint64_t CDynamicProp_m_clrGlow = 0x9dba3a4244fbe6;
	constexpr uint64_t CDynamicProp_m_flGlowMaxDist = 0x35283387b9c88eb2;
	constexpr uint64_t CDynamicProp_m_nGlowStyle = 0xf362d9f167d126f3;
	constexpr uint64_t CEconEntity_m_AttributeManager = 0x562d736f26e9ff34;
	constexpr uint64_t CEconEntity_m_Attributes = 0xf8b208129a0b1bf5;
	constexpr uint64_t CEconEntity_m_Item = 0xc811d1867dc29b8a;
	constexpr uint64_t CEconEntity_m_NetworkedDynamicAttributesForDemos = 0xdd028f4892d3dd8f;
	constexpr uint64_t CEconEntity_m_OriginalOwnerXuidHigh = 0x37f51160bd14487d;
	constexpr uint64_t CEconEntity_m_OriginalOwnerXuidLow = 0xee33c2c7440d841c;
	constexpr uint64_t CEconEntity_m_ProviderType = 0xc96fca27ba592e5b;
	constexpr uint64_t CEconEntity_m_bInitialized = 0x536fd12f1329531f;
	constexpr uint64_t CEconEntity_m_flFallbackWear = 0xb03ce676a01b1bca;
	constexpr uint64_t CEconEntity_m_hOuter = 0x76332d791012c4a2;
	constexpr uint64_t CEconEntity_m_iAccountID = 0xeedb0d813b6952e4;
	constexpr uint64_t CEconEntity_m_iEntityLevel = 0xfdc0e54774085196;
	constexpr uint64_t CEconEntity_m_iEntityQuality = 0x1f858d98ec7fab3;
	constexpr uint64_t CEconEntity_m_iItemDefinitionIndex = 0x64662ee0fbcb9d6a;
	constexpr uint64_t CEconEntity_m_iItemIDHigh = 0x5fb02e9fef9392e1;
	constexpr uint64_t CEconEntity_m_iItemIDLow = 0x6f233a0f09da2ddd;
	constexpr uint64_t CEconEntity_m_iReapplyProvisionParity = 0x297c7b0d992cdce5;
	constexpr uint64_t CEconEntity_m_nFallbackPaintKit = 0xc8fdc5c51d13bf89;
	constexpr uint64_t CEconEntity_m_nFallbackSeed = 0x69709c5217a88bdc;
	constexpr uint64_t CEconEntity_m_nFallbackStatTrak = 0xb838efe0714ab6ff;
	constexpr uint64_t CEconEntity_m_szCustomName = 0x13fb3b8495657917;
	constexpr uint64_t CEmbers_m_bEmit = 0x1b0f9edc2689e7a5;
	constexpr uint64_t CEmbers_m_nDensity = 0xa4a0ee6878b9d24f;
	constexpr uint64_t CEmbers_m_nLifetime = 0xb7d9d633b954a49a;
	constexpr uint64_t CEmbers_m_nSpeed = 0x34ec2a8c8e2aa3b5;
	constexpr uint64_t CEntityDissolve_m_flFadeInLength = 0x1a05f60e647a2ba;
	constexpr uint64_t CEntityDissolve_m_flFadeInStart = 0xa050b83b959d52a6;
	constexpr uint64_t CEntityDissolve_m_flFadeOutLength = 0x457e629d79b006a4;
	constexpr uint64_t CEntityDissolve_m_flFadeOutModelLength = 0x43d0311923b96e2c;
	constexpr uint64_t CEntityDissolve_m_flFadeOutModelStart = 0x886a3b2678c79dcf;
	constexpr uint64_t CEntityDissolve_m_flFadeOutStart = 0x64da96dda440406;
	constexpr uint64_t CEntityDissolve_m_flStartTime = 0xd264d63b943f3e97;
	constexpr uint64_t CEntityDissolve_m_nDissolveType = 0xd323976bff5d68c4;
	constexpr uint64_t CEntityDissolve_m_nMagnitude = 0x1e12363f863aa769;
	constexpr uint64_t CEntityDissolve_m_vDissolverOrigin = 0x3dcf76266804336c;
	constexpr uint64_t CEntityFlame_m_bCheapEffect = 0x1d63083afd4a3b09;
	constexpr uint64_t CEntityFlame_m_hEntAttached = 0xb0ec0a557aa4c9cb;
	constexpr uint64_t CEntityFreezing_m_bFinishFreezing = 0xea3b115a6a57110d;
	constexpr uint64_t CEntityFreezing_m_flFrozen = 0x396012bc88391a0f;
	constexpr uint64_t CEntityFreezing_m_flFrozenPerHitbox = 0xa1e6a215c2461559;
	constexpr uint64_t CEntityFreezing_m_vFreezingOrigin = 0x1179f885b7c80f1e;
	constexpr uint64_t CEntityParticleTrail_m_Info = 0x479fc717f9885cba;
	constexpr uint64_t CEntityParticleTrail_m_flEndSize = 0xb452049a4aefae6;
	constexpr uint64_t CEntityParticleTrail_m_flLifetime = 0xf4963cda3c44dbe1;
	constexpr uint64_t CEntityParticleTrail_m_flStartSize = 0xe767e5da0de86f49;
	constexpr uint64_t CEntityParticleTrail_m_hConstraintEntity = 0xab9814baa003a3ef;
	constexpr uint64_t CEntityParticleTrail_m_iMaterialName = 0x8a8f52253c2e2be8;
	constexpr uint64_t CEnvAmbientLight_m_vecColor = 0x709b9b73cb6c28ac;
	constexpr uint64_t CEnvDOFController_m_bDOFEnabled = 0xd605ee12adc89b96;
	constexpr uint64_t CEnvDOFController_m_flFarBlurDepth = 0x53dd9675ac57126d;
	constexpr uint64_t CEnvDOFController_m_flFarBlurRadius = 0xdc50b161930401c8;
	constexpr uint64_t CEnvDOFController_m_flFarFocusDepth = 0x8e923b17db6159df;
	constexpr uint64_t CEnvDOFController_m_flNearBlurDepth = 0xf0a2508cfd0803f9;
	constexpr uint64_t CEnvDOFController_m_flNearBlurRadius = 0xfff688d709b776ee;
	constexpr uint64_t CEnvDOFController_m_flNearFocusDepth = 0xf6266739e843d23b;
	constexpr uint64_t CEnvDetailController_m_flFadeEndDist = 0xbd644940455dbb3c;
	constexpr uint64_t CEnvDetailController_m_flFadeStartDist = 0x5997aa39d6f37f02;
	constexpr uint64_t CEnvGasCanister_m_bDoImpactEffects = 0x54ba4722087cd811;
	constexpr uint64_t CEnvGasCanister_m_bInSkybox = 0x20fa5429f3584b9;
	constexpr uint64_t CEnvGasCanister_m_bLanded = 0xf22f00125da58e8a;
	constexpr uint64_t CEnvGasCanister_m_bLaunchedFromWithinWorld = 0x109c8838d5c17163;
	constexpr uint64_t CEnvGasCanister_m_flFlightSpeed = 0xced38d31f51260c9;
	constexpr uint64_t CEnvGasCanister_m_flFlightTime = 0xdf3997a9c4b61cb3;
	constexpr uint64_t CEnvGasCanister_m_flHorizSpeed = 0x636326001aec07c7;
	constexpr uint64_t CEnvGasCanister_m_flInitialZSpeed = 0xa294f40570e3d166;
	constexpr uint64_t CEnvGasCanister_m_flLaunchTime = 0x3e49133ef8ac7633;
	constexpr uint64_t CEnvGasCanister_m_flSkyboxScale = 0xe6faeb30c6203bff;
	constexpr uint64_t CEnvGasCanister_m_flWorldEnterTime = 0xb6c285692cc21ae1;
	constexpr uint64_t CEnvGasCanister_m_flZAcceleration = 0xf159f2ddb20a31;
	constexpr uint64_t CEnvGasCanister_m_hSkyboxCopy = 0x30fb65eabb701bbc;
	constexpr uint64_t CEnvGasCanister_m_nMyZoneIndex = 0xd6c2f96d8b2c9344;
	constexpr uint64_t CEnvGasCanister_m_vecDirection = 0x1c58e8c6996eafc2;
	constexpr uint64_t CEnvGasCanister_m_vecEnterWorldPosition = 0x6c894ad9543d5ff3;
	constexpr uint64_t CEnvGasCanister_m_vecImpactPosition = 0xd97c7d3729a3e7c6;
	constexpr uint64_t CEnvGasCanister_m_vecOrigin = 0xf399034ea4a5ec2d;
	constexpr uint64_t CEnvGasCanister_m_vecOrigin_2_ = 0x439c9e493a16fd96;
	constexpr uint64_t CEnvGasCanister_m_vecParabolaDirection = 0xb1bae97ccad8a118;
	constexpr uint64_t CEnvGasCanister_m_vecSkyboxOrigin = 0x94795e54671a7e2d;
	constexpr uint64_t CEnvGasCanister_m_vecStartAngles = 0x8688bee27da0bdd;
	constexpr uint64_t CEnvGasCanister_m_vecStartPosition = 0x15f1a7f81b38c63d;
	constexpr uint64_t CEnvParticleScript_m_flSequenceScale = 0x29c0676eded8ce93;
	constexpr uint64_t CEnvProjectedTexture_m_LightColor = 0xbe476ce6941af0c0;
	constexpr uint64_t CEnvProjectedTexture_m_SpotlightTextureName = 0xe8c063c013392892;
	constexpr uint64_t CEnvProjectedTexture_m_bAlwaysUpdate = 0xe778854259ee55a2;
	constexpr uint64_t CEnvProjectedTexture_m_bCameraSpace = 0xf184c23c8bd9ad68;
	constexpr uint64_t CEnvProjectedTexture_m_bEnableShadows = 0x166fe40266757f3b;
	constexpr uint64_t CEnvProjectedTexture_m_bLightOnlyTarget = 0x3c963ff3c382f2f9;
	constexpr uint64_t CEnvProjectedTexture_m_bLightWorld = 0xd47c9b5f5fc22578;
	constexpr uint64_t CEnvProjectedTexture_m_bSimpleProjection = 0x26d8c009e9b74115;
	constexpr uint64_t CEnvProjectedTexture_m_bState = 0x2e471ecb13ba12c2;
	constexpr uint64_t CEnvProjectedTexture_m_flAmbient = 0xfc1084d23a67b66b;
	constexpr uint64_t CEnvProjectedTexture_m_flBrightnessScale = 0x917afb7c3ef74583;
	constexpr uint64_t CEnvProjectedTexture_m_flColorTransitionTime = 0x448b8845ae7e22db;
	constexpr uint64_t CEnvProjectedTexture_m_flFarZ = 0x7ab47e91886c9f38;
	constexpr uint64_t CEnvProjectedTexture_m_flLightFOV = 0x755d1a005dde3841;
	constexpr uint64_t CEnvProjectedTexture_m_flNearZ = 0x416ffe9869d4d717;
	constexpr uint64_t CEnvProjectedTexture_m_flProjectionSize = 0xc4cbb210d4f8dc94;
	constexpr uint64_t CEnvProjectedTexture_m_flRotation = 0x53ec849b6aec55ec;
	constexpr uint64_t CEnvProjectedTexture_m_hTargetEntity = 0xaa19c58ee456169e;
	constexpr uint64_t CEnvProjectedTexture_m_iStyle = 0x8c8e499205bb259b;
	constexpr uint64_t CEnvProjectedTexture_m_nShadowQuality = 0x6ab17803531e5159;
	constexpr uint64_t CEnvProjectedTexture_m_nSpotlightTextureFrame = 0xc73d39f0c303379f;
	constexpr uint64_t CEnvQuadraticBeam_m_controlPosition = 0x25930b16130eacf7;
	constexpr uint64_t CEnvQuadraticBeam_m_flWidth = 0x4d742eccbc81af25;
	constexpr uint64_t CEnvQuadraticBeam_m_scrollRate = 0x1aac7328324a68c;
	constexpr uint64_t CEnvQuadraticBeam_m_targetPosition = 0xf79b3cf26c713705;
	constexpr uint64_t CEnvScreenEffect_m_flDuration = 0x56fd2d6787f43396;
	constexpr uint64_t CEnvScreenEffect_m_nType = 0x1daa23c7bbcc0d98;
	constexpr uint64_t CEnvScreenOverlay_m_bIsActive = 0x3d2492567e7b9264;
	constexpr uint64_t CEnvScreenOverlay_m_flOverlayTimes_0_ = 0xcc1ae2217ca21ec6;
	constexpr uint64_t CEnvScreenOverlay_m_flStartTime = 0xe6a47e1d81f2d7ba;
	constexpr uint64_t CEnvScreenOverlay_m_iDesiredOverlay = 0x123cd8d81cc208b7;
	constexpr uint64_t CEnvScreenOverlay_m_iszOverlayNames_0_ = 0x6f5ac55550a2d21f;
	constexpr uint64_t CEnvTonemapController_m_bUseCustomAutoExposureMax = 0x79c05c3fa7a441d8;
	constexpr uint64_t CEnvTonemapController_m_bUseCustomAutoExposureMin = 0x2b8c699138932347;
	constexpr uint64_t CEnvTonemapController_m_bUseCustomBloomScale = 0xc92c383c933de87;
	constexpr uint64_t CEnvTonemapController_m_flBloomExponent = 0xa146c3850be453c4;
	constexpr uint64_t CEnvTonemapController_m_flBloomSaturation = 0x43cb3c527f4d119;
	constexpr uint64_t CEnvTonemapController_m_flCustomAutoExposureMax = 0xcb93ca009b63f940;
	constexpr uint64_t CEnvTonemapController_m_flCustomAutoExposureMin = 0xab87ce33e48805fb;
	constexpr uint64_t CEnvTonemapController_m_flCustomBloomScale = 0x4993839982dc8db1;
	constexpr uint64_t CEnvTonemapController_m_flCustomBloomScaleMinimum = 0xc2f015caf7b1ff60;
	constexpr uint64_t CEnvTonemapController_m_flTonemapMinAvgLum = 0xf40a78cd11ac4787;
	constexpr uint64_t CEnvTonemapController_m_flTonemapPercentBrightPixels = 0x33c3ab28d53ad934;
	constexpr uint64_t CEnvTonemapController_m_flTonemapPercentTarget = 0xf0a0d4752154a318;
	constexpr uint64_t CEnvTonemapController_m_flTonemapRate = 0xfc21baa7b461953f;
	constexpr uint64_t CEnvWind_m_EnvWindShared = 0xbc1693a87f0447ff;
	constexpr uint64_t CEnvWind_m_flGustDuration = 0x8a0d5ed0d1c3745d;
	constexpr uint64_t CEnvWind_m_flInitialWindSpeed = 0x5939775fb18e9e9e;
	constexpr uint64_t CEnvWind_m_flMaxGustDelay = 0xdff181103e988269;
	constexpr uint64_t CEnvWind_m_flMinGustDelay = 0xcffc49ca13874f8f;
	constexpr uint64_t CEnvWind_m_flStartTime = 0xe8da7d8cbf793f89;
	constexpr uint64_t CEnvWind_m_iGustDirChange = 0x5f4d1f439f30eecc;
	constexpr uint64_t CEnvWind_m_iInitialWindDir = 0x884e4dc6d7ceebfd;
	constexpr uint64_t CEnvWind_m_iMaxGust = 0x48048c9c8801e1c5;
	constexpr uint64_t CEnvWind_m_iMaxWind = 0x67913e2525d5e411;
	constexpr uint64_t CEnvWind_m_iMinGust = 0x81ed26f562e52d7c;
	constexpr uint64_t CEnvWind_m_iMinWind = 0x367a53a32522b346;
	constexpr uint64_t CEnvWind_m_iWindSeed = 0x9de66c33a1ea5e3f;
	constexpr uint64_t CFEPlayerDecal_m_flCreationTime = 0x12447019f10fe1ef;
	constexpr uint64_t CFEPlayerDecal_m_nEntity = 0xea7f7959f0cd4ab4;
	constexpr uint64_t CFEPlayerDecal_m_nHitbox = 0x5c0eeac4ab72a8ea;
	constexpr uint64_t CFEPlayerDecal_m_nPlayer = 0x58089ce60f894135;
	constexpr uint64_t CFEPlayerDecal_m_nTintID = 0x7f7aed1eafd29df8;
	constexpr uint64_t CFEPlayerDecal_m_nUniqueID = 0xc0807cd8dc4e929c;
	constexpr uint64_t CFEPlayerDecal_m_nVersion = 0xd41fac5ec382f7ba;
	constexpr uint64_t CFEPlayerDecal_m_rtGcTime = 0x7c819a3052db94e9;
	constexpr uint64_t CFEPlayerDecal_m_ubSignature = 0xf60dfd288aff6308;
	constexpr uint64_t CFEPlayerDecal_m_unAccountID = 0x986c5e28dc83ae70;
	constexpr uint64_t CFEPlayerDecal_m_unTraceID = 0x66a15004075b8e99;
	constexpr uint64_t CFEPlayerDecal_m_vecEndPos = 0xcb818e5751453c7e;
	constexpr uint64_t CFEPlayerDecal_m_vecNormal = 0xabdb46ed51e2cbdb;
	constexpr uint64_t CFEPlayerDecal_m_vecRight = 0x3d034adb4e7c7fef;
	constexpr uint64_t CFEPlayerDecal_m_vecStart = 0x1c0e94acfa8666ec;
	constexpr uint64_t CFireSmoke_m_flScale = 0xd8f099482fdf8ce;
	constexpr uint64_t CFireSmoke_m_flScaleTime = 0xff707af45c31053e;
	constexpr uint64_t CFireSmoke_m_flStartScale = 0x8c69030f70b4a5b7;
	constexpr uint64_t CFireSmoke_m_nFlags = 0x56aa80ad39e52a58;
	constexpr uint64_t CFireSmoke_m_nFlameFromAboveModelIndex = 0xede57c511e653d8f;
	constexpr uint64_t CFireSmoke_m_nFlameModelIndex = 0xb179c3909799f06a;
	constexpr uint64_t CFireTrail_m_flLifetime = 0x987a90853cfb987b;
	constexpr uint64_t CFireTrail_m_nAttachment = 0x6881a66a591533c1;
	constexpr uint64_t CFish_m_angle = 0xd1f873432459b3fb;
	constexpr uint64_t CFish_m_lifeState = 0xa94cf7bd3e64e32b;
	constexpr uint64_t CFish_m_nModelIndex = 0x1c18b32c172c0da1;
	constexpr uint64_t CFish_m_poolOrigin = 0xe8210ca6a23a3d47;
	constexpr uint64_t CFish_m_waterLevel = 0x391ff21a02675e2;
	constexpr uint64_t CFish_m_x = 0xbe0dae58dff6d09c;
	constexpr uint64_t CFish_m_y = 0x366c12dc59f32b05;
	constexpr uint64_t CFish_m_z = 0x1db701a2ae54b110;
	constexpr uint64_t CFists_m_bPlayingUninterruptableAct = 0x887e9bcf087242b9;
	constexpr uint64_t CFogController_m_fog_HDRColorScale = 0x703faf8bbdaf0d7f;
	constexpr uint64_t CFogController_m_fog_ZoomFogScale = 0x6f834a18d82e207a;
	constexpr uint64_t CFogController_m_fog_blend = 0x3bcd00b3bfc8acc0;
	constexpr uint64_t CFogController_m_fog_colorPrimary = 0x29fda88c9090e38b;
	constexpr uint64_t CFogController_m_fog_colorPrimaryLerpTo = 0xe192ff763578e841;
	constexpr uint64_t CFogController_m_fog_colorSecondary = 0x59b8b7230f83b625;
	constexpr uint64_t CFogController_m_fog_colorSecondaryLerpTo = 0xf00c8075194c038a;
	constexpr uint64_t CFogController_m_fog_dirPrimary = 0x59f752d53ee6b2db;
	constexpr uint64_t CFogController_m_fog_duration = 0x21746703ec425a5d;
	constexpr uint64_t CFogController_m_fog_enable = 0x92361fb92241d9d9;
	constexpr uint64_t CFogController_m_fog_end = 0x27c4d80d833a2ee5;
	constexpr uint64_t CFogController_m_fog_endLerpTo = 0xc1f0a50cc05ffe6b;
	constexpr uint64_t CFogController_m_fog_farz = 0xc600c489faebc21d;
	constexpr uint64_t CFogController_m_fog_lerptime = 0xb498d71c27dc971;
	constexpr uint64_t CFogController_m_fog_maxdensity = 0xb8201d6e59861e25;
	constexpr uint64_t CFogController_m_fog_maxdensityLerpTo = 0xa295fdc619deff0;
	constexpr uint64_t CFogController_m_fog_start = 0x2d94ea95b1c7ff76;
	constexpr uint64_t CFogController_m_fog_startLerpTo = 0xe6a64bc559e93677;
	constexpr uint64_t CFootstepControl_m_destination = 0x2c5209c882410e33;
	constexpr uint64_t CFootstepControl_m_source = 0x9ad1c7e66bf831c1;
	constexpr uint64_t CFuncAreaPortalWindow_m_flFadeDist = 0xdf036180902eabdf;
	constexpr uint64_t CFuncAreaPortalWindow_m_flFadeStartDist = 0xbccc9116a87a9570;
	constexpr uint64_t CFuncAreaPortalWindow_m_flTranslucencyLimit = 0x2ec738d0dd63f530;
	constexpr uint64_t CFuncAreaPortalWindow_m_iBackgroundModelIndex = 0x680eabb1dfc6f7b7;
	constexpr uint64_t CFuncConveyor_m_flConveyorSpeed = 0x5b47eb9bc8369639;
	constexpr uint64_t CFuncLadder_m_bFakeLadder = 0x1d2966f485908388;
	constexpr uint64_t CFuncLadder_m_vecLadderDir = 0xa3be732176800431;
	constexpr uint64_t CFuncLadder_m_vecPlayerMountPositionBottom = 0x2b1e2adf23032b8b;
	constexpr uint64_t CFuncLadder_m_vecPlayerMountPositionTop = 0xcc3215963407ca32;
	constexpr uint64_t CFuncMoveLinear_m_fFlags = 0x36bf5f817d845d50;
	constexpr uint64_t CFuncMoveLinear_m_vecVelocity = 0xba394315840511c;
	constexpr uint64_t CFuncOccluder_m_bActive = 0xdcd2f3db4dc241a4;
	constexpr uint64_t CFuncOccluder_m_nOccluderIndex = 0x705b0ba8cce66641;
	constexpr uint64_t CFuncRotating_m_angRotation_0_ = 0x6f0c13c050be8d57;
	constexpr uint64_t CFuncRotating_m_angRotation_1_ = 0x4106f652e3433e33;
	constexpr uint64_t CFuncRotating_m_angRotation_2_ = 0x3cb9b30399bee41c;
	constexpr uint64_t CFuncRotating_m_flSimulationTime = 0xb6d05c00d1bc0911;
	constexpr uint64_t CFuncRotating_m_vecOrigin = 0x883f2eea53adf67f;
	constexpr uint64_t CFuncSmokeVolume_m_Collision = 0x675219f79769b501;
	constexpr uint64_t CFuncSmokeVolume_m_Color1 = 0x4451bb4c660b6ef1;
	constexpr uint64_t CFuncSmokeVolume_m_Color2 = 0x56d2e012f578a8f9;
	constexpr uint64_t CFuncSmokeVolume_m_Density = 0x19b9abca4674dedf;
	constexpr uint64_t CFuncSmokeVolume_m_DensityRampSpeed = 0xc14f76b9328a719d;
	constexpr uint64_t CFuncSmokeVolume_m_MaterialName = 0xcd867fa65b3e1314;
	constexpr uint64_t CFuncSmokeVolume_m_MovementSpeed = 0x27625aecc7f7bcc7;
	constexpr uint64_t CFuncSmokeVolume_m_ParticleDrawWidth = 0x7b4cfb92d176d179;
	constexpr uint64_t CFuncSmokeVolume_m_ParticleSpacingDistance = 0x3b2248d4510d8c5a;
	constexpr uint64_t CFuncSmokeVolume_m_RotationSpeed = 0x5643640686df1247;
	constexpr uint64_t CFuncSmokeVolume_m_maxDrawDistance = 0xd20cf4528bda24d3;
	constexpr uint64_t CFuncSmokeVolume_m_nSolidType = 0xb242e4d61e9e169b;
	constexpr uint64_t CFuncSmokeVolume_m_nSurroundType = 0xb3153560a6a46645;
	constexpr uint64_t CFuncSmokeVolume_m_spawnflags = 0x60e78ee99f905683;
	constexpr uint64_t CFuncSmokeVolume_m_triggerBloat = 0xc703020bc6c3cd2d;
	constexpr uint64_t CFuncSmokeVolume_m_usSolidFlags = 0x61a0064477781e5a;
	constexpr uint64_t CFuncSmokeVolume_m_vecMaxs = 0x4e3a220feac34c3;
	constexpr uint64_t CFuncSmokeVolume_m_vecMins = 0x9ddb2bec98d9eaae;
	constexpr uint64_t CFuncSmokeVolume_m_vecSpecifiedSurroundingMaxs = 0x7059937920fd0ceb;
	constexpr uint64_t CFuncSmokeVolume_m_vecSpecifiedSurroundingMins = 0xf7cb5cc4f1c9ae3a;
	constexpr uint64_t CFunc_Dust_m_Collision = 0xe1882e6cc380cee8;
	constexpr uint64_t CFunc_Dust_m_Color = 0x126ba8fb50c28deb;
	constexpr uint64_t CFunc_Dust_m_DistMax = 0xe5a61a76af1e859d;
	constexpr uint64_t CFunc_Dust_m_DustFlags = 0xcb12ae02ec42262c;
	constexpr uint64_t CFunc_Dust_m_FallSpeed = 0xd421d09b1310faec;
	constexpr uint64_t CFunc_Dust_m_LifetimeMax = 0x93fa79a621360a85;
	constexpr uint64_t CFunc_Dust_m_LifetimeMin = 0x983a19bdc82139f1;
	constexpr uint64_t CFunc_Dust_m_SpawnRate = 0x84e6fe943e6a0e4;
	constexpr uint64_t CFunc_Dust_m_SpeedMax = 0xaacbce0c0fd232c9;
	constexpr uint64_t CFunc_Dust_m_bAffectedByWind = 0xd46dcf2bec212c81;
	constexpr uint64_t CFunc_Dust_m_flSizeMax = 0x6c067de6caba053b;
	constexpr uint64_t CFunc_Dust_m_flSizeMin = 0xd608a56d7e7cdb0d;
	constexpr uint64_t CFunc_Dust_m_nModelIndex = 0x68943cf02a6a2e7c;
	constexpr uint64_t CFunc_Dust_m_nSolidType = 0x89d14f8871fdda75;
	constexpr uint64_t CFunc_Dust_m_nSurroundType = 0xd8a9fa88e9c59791;
	constexpr uint64_t CFunc_Dust_m_triggerBloat = 0xbdc5055177c6c1c0;
	constexpr uint64_t CFunc_Dust_m_usSolidFlags = 0xf6fe532f9a1eab14;
	constexpr uint64_t CFunc_Dust_m_vecMaxs = 0xe1051c0654e5ed6a;
	constexpr uint64_t CFunc_Dust_m_vecMins = 0x7058a191c658dbf2;
	constexpr uint64_t CFunc_Dust_m_vecSpecifiedSurroundingMaxs = 0x38b74f2c64758a98;
	constexpr uint64_t CFunc_Dust_m_vecSpecifiedSurroundingMins = 0x9cf795df92b08cac;
	constexpr uint64_t CFunc_LOD_m_nDisappearMaxDist = 0x5e861c6f50c14ab1;
	constexpr uint64_t CFunc_LOD_m_nDisappearMinDist = 0x3ea2e96e8ca367ee;
	constexpr uint64_t CGrassBurn_m_flGrassBurnClearTime = 0xa33b8a643d9355ab;
	constexpr uint64_t CHandleTest_m_Handle = 0xe207302943631634;
	constexpr uint64_t CHandleTest_m_bSendHandle = 0xf561f6399edf480e;
	constexpr uint64_t CHostage_m_fFlags = 0xa7480e85963e0b8c;
	constexpr uint64_t CHostage_m_flDropStartTime = 0xd4f9756a8098c42a;
	constexpr uint64_t CHostage_m_flGrabSuccessTime = 0x939a0d05fb9ce7ef;
	constexpr uint64_t CHostage_m_flRescueStartTime = 0x484458d3b4bf33ab;
	constexpr uint64_t CHostage_m_iHealth = 0x2145101448bba2fa;
	constexpr uint64_t CHostage_m_iMaxHealth = 0xaf2f9ebc6b701da6;
	constexpr uint64_t CHostage_m_isRescued = 0x1bd8a30013e1e0be;
	constexpr uint64_t CHostage_m_jumpedThisFrame = 0x82d1b2d1cd1dd3c0;
	constexpr uint64_t CHostage_m_leader = 0xc9b7e68169253ea3;
	constexpr uint64_t CHostage_m_lifeState = 0x2fd16c320a8decd0;
	constexpr uint64_t CHostage_m_nHostageState = 0xa99974335b38c6a7;
	constexpr uint64_t CHostage_m_vel = 0xc45b4bee9e5baf87;
	constexpr uint64_t CInferno_m_bFireIsBurning = 0x12a6e8be7eaba2bf;
	constexpr uint64_t CInferno_m_fireCount = 0x2a80b2ece4ac96a0;
	constexpr uint64_t CInferno_m_fireXDelta = 0xfba3ba2ae92b49ff;
	constexpr uint64_t CInferno_m_fireYDelta = 0xd9e750d2f02fc1e3;
	constexpr uint64_t CInferno_m_fireZDelta = 0x7f98e5034afa50b4;
	constexpr uint64_t CInferno_m_nFireEffectTickBegin = 0xf9c30ed0d96d841b;
	constexpr uint64_t CInfoMapRegion_m_flRadius = 0x94d4036eb591b037;
	constexpr uint64_t CInfoMapRegion_m_szLocToken = 0xcb686dc8c52b7b16;
	constexpr uint64_t CInfoOverlayAccessor_m_iOverlayID = 0x4594dc29a2941e19;
	constexpr uint64_t CInfoOverlayAccessor_m_iTextureFrameIndex = 0xaa8e7abfe519f918;
	constexpr uint64_t CItemAssaultSuitUseable_m_bIsHeavyAssaultSuit = 0x18a1a84e53ed1834;
	constexpr uint64_t CItemAssaultSuitUseable_m_nArmorValue = 0x55293a744feee756;
	constexpr uint64_t CItemDogtags_m_KillingPlayer = 0x5f2fad99f1a37663;
	constexpr uint64_t CItemDogtags_m_OwningPlayer = 0xa6a1daa531508c09;
	constexpr uint64_t CItem_m_bShouldGlow = 0x9807f03890992fcd;
	constexpr uint64_t CLightGlow_m_angRotation = 0x42898648ceb03b87;
	constexpr uint64_t CLightGlow_m_clrRender = 0x13bda46b49050602;
	constexpr uint64_t CLightGlow_m_flGlowProxySize = 0x98abaf18ab6dc7c1;
	constexpr uint64_t CLightGlow_m_nHorizontalSize = 0xcc221962373b24ab;
	constexpr uint64_t CLightGlow_m_nMaxDist = 0x5e72a2a11d05e1e5;
	constexpr uint64_t CLightGlow_m_nMinDist = 0xa695ef7e217a29cf;
	constexpr uint64_t CLightGlow_m_nOuterMaxDist = 0x93bb190793691461;
	constexpr uint64_t CLightGlow_m_nVerticalSize = 0x784ecf323dba2fa2;
	constexpr uint64_t CLightGlow_m_spawnflags = 0x8ec9182612325a85;
	constexpr uint64_t CLightGlow_m_vecOrigin = 0x12687195fab937bc;
	constexpr uint64_t CLightGlow_moveparent = 0xf4b3b7b797bb9ac6;
	constexpr uint64_t CMapVetoPickController_m_nAccountIDs = 0x11c4098590e183b0;
	constexpr uint64_t CMapVetoPickController_m_nCurrentPhase = 0x897ed7cb0123da96;
	constexpr uint64_t CMapVetoPickController_m_nDraftType = 0x36b2e0909359f62a;
	constexpr uint64_t CMapVetoPickController_m_nMapId0 = 0x8176db41b490d830;
	constexpr uint64_t CMapVetoPickController_m_nMapId1 = 0xc759fd1c225521b;
	constexpr uint64_t CMapVetoPickController_m_nMapId2 = 0xa29d2210ca309519;
	constexpr uint64_t CMapVetoPickController_m_nMapId3 = 0xf18498ad5f4445fe;
	constexpr uint64_t CMapVetoPickController_m_nMapId4 = 0x9d28dd0934592f6b;
	constexpr uint64_t CMapVetoPickController_m_nMapId5 = 0x1ea38ca0efec3c85;
	constexpr uint64_t CMapVetoPickController_m_nPhaseDurationTicks = 0x699e46fb63e13d30;
	constexpr uint64_t CMapVetoPickController_m_nPhaseStartTick = 0xfd87f83288f9e43f;
	constexpr uint64_t CMapVetoPickController_m_nStartingSide0 = 0x410bcd41909a3f53;
	constexpr uint64_t CMapVetoPickController_m_nTeamWinningCoinToss = 0x2140b2f13f477d24;
	constexpr uint64_t CMapVetoPickController_m_nTeamWithFirstChoice = 0x709a2ed0fd2f1ab7;
	constexpr uint64_t CMapVetoPickController_m_nVoteMapIdsList = 0x7f7231357a15215f;
	constexpr uint64_t CMaterialModifyControl_m_bFloatLerpWrap = 0xe8a394322363a413;
	constexpr uint64_t CMaterialModifyControl_m_bNewAnimCommandsSemaphore = 0xc96eeb209c7ef5b6;
	constexpr uint64_t CMaterialModifyControl_m_bWrap = 0xc487d2a3b042f99e;
	constexpr uint64_t CMaterialModifyControl_m_flFloatLerpEndValue = 0x5094a51f58dd7790;
	constexpr uint64_t CMaterialModifyControl_m_flFloatLerpStartValue = 0x87be91a51d5268ca;
	constexpr uint64_t CMaterialModifyControl_m_flFloatLerpTransitionTime = 0x293cba2b9dda2d5f;
	constexpr uint64_t CMaterialModifyControl_m_flFramerate = 0x73265f96ee43614f;
	constexpr uint64_t CMaterialModifyControl_m_iFrameEnd = 0x874cdb78d1902e3f;
	constexpr uint64_t CMaterialModifyControl_m_iFrameStart = 0x72c6e59d34a9c824;
	constexpr uint64_t CMaterialModifyControl_m_nModifyMode = 0x70d65420468c1fc;
	constexpr uint64_t CMaterialModifyControl_m_szMaterialName = 0xf2d530cfd71127d9;
	constexpr uint64_t CMaterialModifyControl_m_szMaterialVar = 0x311c320964b78db3;
	constexpr uint64_t CMaterialModifyControl_m_szMaterialVarValue = 0x23ad7c21ed70e94;
	constexpr uint64_t CMelee_m_flThrowAt = 0x8de28266371e24ad;
	constexpr uint64_t CMolotovProjectile_m_bIsIncGrenade = 0xf57f0922b56f9e1d;
	constexpr uint64_t CMovieDisplay_m_bEnabled = 0xd1d412082c1179de;
	constexpr uint64_t CMovieDisplay_m_bForcedSlave = 0x9b2c3eb7b1d3a1b3;
	constexpr uint64_t CMovieDisplay_m_bLooping = 0xc8c18865bebc03d8;
	constexpr uint64_t CMovieDisplay_m_bStretchToFill = 0x2faa9967459a303d;
	constexpr uint64_t CMovieDisplay_m_bUseCustomUVs = 0x7652fdc47bbb975b;
	constexpr uint64_t CMovieDisplay_m_flUMax = 0x2dbc6c92d2b70de6;
	constexpr uint64_t CMovieDisplay_m_flUMin = 0x2f6b3e1f2f435076;
	constexpr uint64_t CMovieDisplay_m_flVMax = 0xd185a0216c09d311;
	constexpr uint64_t CMovieDisplay_m_flVMin = 0x3a7bf8394e41d441;
	constexpr uint64_t CMovieDisplay_m_szGroupName = 0x6e6142fc20719f5b;
	constexpr uint64_t CMovieDisplay_m_szMovieFilename = 0xa35e5385cdf62d1f;
	constexpr uint64_t CParadropChopper_m_hCallingPlayer = 0x13cfe58697d6a842;
	constexpr uint64_t CParadropChopper_m_vecOrigin = 0x882c0a59d9d1e89b;
	constexpr uint64_t CParadropChopper_m_vecOrigin_2_ = 0xf7f8e844bf3f7a81;
	constexpr uint64_t CParticleFire_m_vDirection = 0xb68f4a607a8b33ff;
	constexpr uint64_t CParticleFire_m_vOrigin = 0x88985d9c57f0f0e5;
	constexpr uint64_t CParticlePerformanceMonitor_m_bDisplayPerf = 0x392318d2c4022948;
	constexpr uint64_t CParticlePerformanceMonitor_m_bMeasurePerf = 0x1ed4811fd21410d2;
	constexpr uint64_t CParticleSystem_m_angRotation = 0x36f087332e9aafad;
	constexpr uint64_t CParticleSystem_m_bActive = 0xc3743586e7f7462f;
	constexpr uint64_t CParticleSystem_m_fEffects = 0x11a914f32cf8bdc1;
	constexpr uint64_t CParticleSystem_m_flStartTime = 0x944abd9f46689469;
	constexpr uint64_t CParticleSystem_m_hControlPointEnts = 0x135c6eb427331a5c;
	constexpr uint64_t CParticleSystem_m_hOwnerEntity = 0xcb7c1f9d975e7afc;
	constexpr uint64_t CParticleSystem_m_iControlPointParents = 0xb6d958f38c01233c;
	constexpr uint64_t CParticleSystem_m_iEffectIndex = 0xa7fb895a15d45c3e;
	constexpr uint64_t CParticleSystem_m_iParentAttachment = 0xf697e36a978123a2;
	constexpr uint64_t CParticleSystem_m_iServerControlPointAssignments = 0x54df283e4611d7d2;
	constexpr uint64_t CParticleSystem_m_nStopType = 0x353c8d8370dead8c;
	constexpr uint64_t CParticleSystem_m_szSnapshotFileName = 0x257a34508e3cbccd;
	constexpr uint64_t CParticleSystem_m_vServerControlPoints = 0xd28105a2e0876ec3;
	constexpr uint64_t CParticleSystem_m_vecOrigin = 0x8eab67a7267f7090;
	constexpr uint64_t CParticleSystem_moveparent = 0xe074d2a8cd464435;
	constexpr uint64_t CPhysBoxMultiplayer_m_fMass = 0xed434e02bcadc52e;
	constexpr uint64_t CPhysBoxMultiplayer_m_iPhysicsMode = 0x3f94bf23e0a27c46;
	constexpr uint64_t CPhysBox_m_mass = 0xc3c00c817f35511;
	constexpr uint64_t CPhysPropLootCrate_m_bRenderInPSPM = 0xa338d6ea9595866e;
	constexpr uint64_t CPhysPropLootCrate_m_bRenderInTablet = 0xb47a1e0d31f51666;
	constexpr uint64_t CPhysPropLootCrate_m_iHealth = 0x3312d6bd82090c2d;
	constexpr uint64_t CPhysPropLootCrate_m_iMaxHealth = 0xa393df0b6c0d464b;
	constexpr uint64_t CPhysicsPropMultiplayer_m_collisionMaxs = 0x385dfe2763d436ae;
	constexpr uint64_t CPhysicsPropMultiplayer_m_collisionMins = 0xde00c2f913c689ca;
	constexpr uint64_t CPhysicsPropMultiplayer_m_fMass = 0xd82dafd97095e614;
	constexpr uint64_t CPhysicsPropMultiplayer_m_iPhysicsMode = 0x7c74aa3d2a9514d0;
	constexpr uint64_t CPhysicsProp_m_bAwake = 0xb7069e697460a145;
	constexpr uint64_t CPhysicsProp_m_spawnflags = 0xaf0eba6ac673d3b2;
	constexpr uint64_t CPlantedC4_m_bBombDefused = 0xb9df21babce1ec0d;
	constexpr uint64_t CPlantedC4_m_bBombTicking = 0x83d9240d9c54f5e3;
	constexpr uint64_t CPlantedC4_m_flC4Blow = 0x444082d3206fdf51;
	constexpr uint64_t CPlantedC4_m_flDefuseCountDown = 0x78083747fe12f69e;
	constexpr uint64_t CPlantedC4_m_flDefuseLength = 0xfb582689c386b77e;
	constexpr uint64_t CPlantedC4_m_flTimerLength = 0xe6a6f00176586474;
	constexpr uint64_t CPlantedC4_m_hBombDefuser = 0xf75c503ac051b91c;
	constexpr uint64_t CPlantedC4_m_nBombSite = 0xffa5be47ed82bd05;
	constexpr uint64_t CPlasma_m_flScale = 0xd0db4b58467d6c5f;
	constexpr uint64_t CPlasma_m_flScaleTime = 0xfcb19e31b1cf0d0f;
	constexpr uint64_t CPlasma_m_flStartScale = 0x35eac3bc5242feda;
	constexpr uint64_t CPlasma_m_nFlags = 0x78618c8cb7880841;
	constexpr uint64_t CPlasma_m_nGlowModelIndex = 0x5b9bf8188b4dc6b9;
	constexpr uint64_t CPlasma_m_nPlasmaModelIndex = 0x9a528d98da763c9;
	constexpr uint64_t CPlasma_m_nPlasmaModelIndex2 = 0x127183a7527211bf;
	constexpr uint64_t CPlayerPing_m_bUrgent = 0xe2c3e6558a096b12;
	constexpr uint64_t CPlayerPing_m_hPingedEntity = 0xed9f3c378fa798f0;
	constexpr uint64_t CPlayerPing_m_hPlayer = 0x1a05c1f114b26f5d;
	constexpr uint64_t CPlayerPing_m_iType = 0x21e0c2db15e6c312;
	constexpr uint64_t CPlayerPing_m_szPlaceName = 0xbf8aa0e5b6ee469c;
	constexpr uint64_t CPlayerResource_m_bAlive = 0x77798aab5d75eb86;
	constexpr uint64_t CPlayerResource_m_bConnected = 0x9ed808214da6ca85;
	constexpr uint64_t CPlayerResource_m_iAssists = 0x45bcf821a0a3c83b;
	constexpr uint64_t CPlayerResource_m_iCoachingTeam = 0xd135f72040f5ca9f;
	constexpr uint64_t CPlayerResource_m_iDeaths = 0x3eefca39dc39e947;
	constexpr uint64_t CPlayerResource_m_iHealth = 0xc3ac89bc97dc2bfe;
	constexpr uint64_t CPlayerResource_m_iKills = 0xe8ab41f58ca25e1;
	constexpr uint64_t CPlayerResource_m_iPendingTeam = 0xc09fece4846d2f7a;
	constexpr uint64_t CPlayerResource_m_iPing = 0x6f4bc696d20b9414;
	constexpr uint64_t CPlayerResource_m_iTeam = 0x506b72a6a9422f0f;
	constexpr uint64_t CPointCamera_m_FOV = 0x57fa778acfc4a43d;
	constexpr uint64_t CPointCamera_m_FogColor = 0xe0d46a004ecd60a3;
	constexpr uint64_t CPointCamera_m_Resolution = 0xd9354ffe4bc245b3;
	constexpr uint64_t CPointCamera_m_bActive = 0xd4506190e24184d2;
	constexpr uint64_t CPointCamera_m_bFogEnable = 0xe4596a944e4e89c7;
	constexpr uint64_t CPointCamera_m_bUseScreenAspectRatio = 0xe12f9e22fd2ee6db;
	constexpr uint64_t CPointCamera_m_flFogEnd = 0x77242aa6714f4686;
	constexpr uint64_t CPointCamera_m_flFogMaxDensity = 0x67402ebb8b6af488;
	constexpr uint64_t CPointCamera_m_flFogStart = 0x8e196a0d895f5f59;
	constexpr uint64_t CPointCommentaryNode_m_bActive = 0x78ffe6fa446129e8;
	constexpr uint64_t CPointCommentaryNode_m_flStartTime = 0x119c811b40714d7c;
	constexpr uint64_t CPointCommentaryNode_m_hViewPosition = 0x85131d3a864b40b8;
	constexpr uint64_t CPointCommentaryNode_m_iNodeNumber = 0x8746f3102cb9e4c2;
	constexpr uint64_t CPointCommentaryNode_m_iNodeNumberMax = 0x7cf7af85d2b5630d;
	constexpr uint64_t CPointCommentaryNode_m_iszCommentaryFile = 0x20c64fcddcc662f3;
	constexpr uint64_t CPointCommentaryNode_m_iszCommentaryFileNoHDR = 0xec821cfbfd139a1;
	constexpr uint64_t CPointCommentaryNode_m_iszSpeakers = 0x867bf9e8bd62b63e;
	constexpr uint64_t CPointWorldText_m_flTextSize = 0x67ba6b60faa140c;
	constexpr uint64_t CPointWorldText_m_szText = 0x5b88a2b850a51a8a;
	constexpr uint64_t CPointWorldText_m_textColor = 0x7d3b87ee67459586;
	constexpr uint64_t CPoseController_m_bInterpolationWrap = 0xf26739cf03be763;
	constexpr uint64_t CPoseController_m_bPoseValueParity = 0xaee835f657f321c1;
	constexpr uint64_t CPoseController_m_chPoseIndex = 0x8b8e1db1de1a71c4;
	constexpr uint64_t CPoseController_m_fCycleFrequency = 0x3c1d4ffc6b0d6478;
	constexpr uint64_t CPoseController_m_fFModAmplitude = 0x84b0893205995d48;
	constexpr uint64_t CPoseController_m_fFModRate = 0x38157367cd01c781;
	constexpr uint64_t CPoseController_m_fFModTimeOffset = 0x32b48367a6d56879;
	constexpr uint64_t CPoseController_m_fInterpolationTime = 0xd40e3b3596355459;
	constexpr uint64_t CPoseController_m_fPoseValue = 0x1c314b51e0f7da0e;
	constexpr uint64_t CPoseController_m_hProps = 0xa668e5efdda53abe;
	constexpr uint64_t CPoseController_m_nFModType = 0x831d022d7727b3f5;
	constexpr uint64_t CPostProcessController_m_bMaster = 0x1e9e8efbb1aff941;
	constexpr uint64_t CPostProcessController_m_flPostProcessParameters = 0x68a0c2b069863669;
	constexpr uint64_t CPrecipitation_m_nPrecipType = 0x8cc6c9ff5791b1a0;
	constexpr uint64_t CPropCounter_m_flDisplayValue = 0xb553cce17fe2fb2d;
	constexpr uint64_t CPropJeep_m_bHeadlightIsOn = 0x34cb48f7da055b03;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_bEnterAnimOn = 0xeb440dec44e7995f;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_bExitAnimOn = 0xeeffba6fb59d66da;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_bForceEyesToAttachment = 0x722115576d47a284;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_hPlayer = 0x500c255d4ee74976;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_vecEyeExitEndpoint = 0x180cc4f9679fb50e;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_vehicleView_bClampEyeAngles = 0xaed55690b0e3ba51;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_vehicleView_flFOV = 0x47ab3ff3874788c5;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_vehicleView_flPitchCurveLinear = 0x7fd4f97cc10216e2;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_vehicleView_flPitchCurveZero = 0x8996080f9228102e;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_vehicleView_flPitchMax = 0x7805a22409761643;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_vehicleView_flPitchMin = 0x6b2bc1e8144f2524;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_vehicleView_flRollCurveLinear = 0x18122c22f8e7ef86;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_vehicleView_flRollCurveZero = 0x85b3648c31e62aca;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_vehicleView_flYawMax = 0x394099a975549410;
	constexpr uint64_t CPropVehicleChoreoGeneric_m_vehicleView_flYawMin = 0xd853df216bc3fb53;
	constexpr uint64_t CPropVehicleDriveable_m_bEnterAnimOn = 0x33ed9cc3a50e062;
	constexpr uint64_t CPropVehicleDriveable_m_bExitAnimOn = 0x417096f12d40d82f;
	constexpr uint64_t CPropVehicleDriveable_m_bHasGun = 0x9fae514287a41168;
	constexpr uint64_t CPropVehicleDriveable_m_bUnableToFire = 0x96b48d84360d305c;
	constexpr uint64_t CPropVehicleDriveable_m_flThrottle = 0x8926fe78c23e8e3e;
	constexpr uint64_t CPropVehicleDriveable_m_hPlayer = 0x7414334f59036678;
	constexpr uint64_t CPropVehicleDriveable_m_nBoostTimeLeft = 0xf8f6f265953f9478;
	constexpr uint64_t CPropVehicleDriveable_m_nHasBoost = 0xaaf9d8af88aca162;
	constexpr uint64_t CPropVehicleDriveable_m_nRPM = 0x6b17892628e2efad;
	constexpr uint64_t CPropVehicleDriveable_m_nScannerDisabledVehicle = 0xd09507adb7f5afa3;
	constexpr uint64_t CPropVehicleDriveable_m_nScannerDisabledWeapons = 0xc11cec608c8dde45;
	constexpr uint64_t CPropVehicleDriveable_m_nSpeed = 0x4d23d804b4e82910;
	constexpr uint64_t CPropVehicleDriveable_m_vecEyeExitEndpoint = 0x64727b900b4e0deb;
	constexpr uint64_t CPropVehicleDriveable_m_vecGunCrosshair = 0x36b9716248b1cdcc;
	constexpr uint64_t CProp_Hallucination_m_bEnabled = 0x29d84e435f2ea4ef;
	constexpr uint64_t CProp_Hallucination_m_fRechargeTime = 0x522c87f5e9710a19;
	constexpr uint64_t CProp_Hallucination_m_fVisibleTime = 0x83f02405b86306b7;
	constexpr uint64_t CRagdollManager_m_iCurrentMaxRagdollCount = 0x8485a92c5e9067e3;
	constexpr uint64_t CRagdollPropAttached_m_attachmentPointBoneSpace = 0x6ff1cc9521ed005b;
	constexpr uint64_t CRagdollPropAttached_m_attachmentPointRagdollSpace = 0x2e020f5bfc1f1cd0;
	constexpr uint64_t CRagdollPropAttached_m_boneIndexAttached = 0x4b88fd8ada2a8878;
	constexpr uint64_t CRagdollPropAttached_m_ragdollAttachedObjectIndex = 0xba3b86915241560;
	constexpr uint64_t CRagdollProp_m_flBlendWeight = 0xb6f49b1969daf19b;
	constexpr uint64_t CRagdollProp_m_hUnragdoll = 0xc9636feae3039812;
	constexpr uint64_t CRagdollProp_m_nOverlaySequence = 0x8afd7f8a6a07573;
	constexpr uint64_t CRagdollProp_m_ragAngles_0_ = 0xdf06b42a4f08dcef;
	constexpr uint64_t CRagdollProp_m_ragPos_0_ = 0x75bec7ff547fbed4;
	constexpr uint64_t CRopeKeyframe_m_RopeFlags = 0x806d448d0a052bae;
	constexpr uint64_t CRopeKeyframe_m_RopeLength = 0xb1b5a987b9b8e4d7;
	constexpr uint64_t CRopeKeyframe_m_Slack = 0xb34cbaf05ce864b7;
	constexpr uint64_t CRopeKeyframe_m_Subdiv = 0x48a4bbd559e317dd;
	constexpr uint64_t CRopeKeyframe_m_TextureScale = 0xdba83b1a06083da4;
	constexpr uint64_t CRopeKeyframe_m_Width = 0x7f06a3f0dce6ec9a;
	constexpr uint64_t CRopeKeyframe_m_bConstrainBetweenEndpoints = 0x2b9f802181906d01;
	constexpr uint64_t CRopeKeyframe_m_fLockedPoints = 0xbc5c13f870221d0e;
	constexpr uint64_t CRopeKeyframe_m_flScrollSpeed = 0xc3b481c36f510885;
	constexpr uint64_t CRopeKeyframe_m_hEndPoint = 0x12cfde64dcc5a7a1;
	constexpr uint64_t CRopeKeyframe_m_hStartPoint = 0x99b09db7e71a2dd4;
	constexpr uint64_t CRopeKeyframe_m_iDefaultRopeMaterialModelIndex = 0xd2f94fd51c962786;
	constexpr uint64_t CRopeKeyframe_m_iEndAttachment = 0x32670689cb6c5cfe;
	constexpr uint64_t CRopeKeyframe_m_iParentAttachment = 0xf7b20e08bc3b1f39;
	constexpr uint64_t CRopeKeyframe_m_iRopeMaterialModelIndex = 0x2e4141f542cc9f4c;
	constexpr uint64_t CRopeKeyframe_m_iStartAttachment = 0x825a64a42bde8e3c;
	constexpr uint64_t CRopeKeyframe_m_nChangeCount = 0x2ab101cf5d047e83;
	constexpr uint64_t CRopeKeyframe_m_nMaxCPULevel = 0xc8e448260b2788a9;
	constexpr uint64_t CRopeKeyframe_m_nMaxGPULevel = 0xa56ad50e0b65911e;
	constexpr uint64_t CRopeKeyframe_m_nMinCPULevel = 0x355b7e7f2a26f62f;
	constexpr uint64_t CRopeKeyframe_m_nMinGPULevel = 0xa32eb304495c7c78;
	constexpr uint64_t CRopeKeyframe_m_nSegments = 0x1e09ad1002f8087a;
	constexpr uint64_t CRopeKeyframe_m_vecOrigin = 0xd97c4eda6d27680d;
	constexpr uint64_t CRopeKeyframe_moveparent = 0x5b6c7337b1d007af;
	constexpr uint64_t CSceneEntity_m_bIsPlayingBack = 0x12892609a52e44e0;
	constexpr uint64_t CSceneEntity_m_bMultiplayer = 0xcf10c47777e886e1;
	constexpr uint64_t CSceneEntity_m_bPaused = 0x2927b9f22992512e;
	constexpr uint64_t CSceneEntity_m_flForceClientTime = 0xfff7a4d241afc6c1;
	constexpr uint64_t CSceneEntity_m_nSceneStringIndex = 0x1b64802d7c395c7f;
	constexpr uint64_t CShadowControl_m_bDisableShadows = 0x1b1d846854c6f45;
	constexpr uint64_t CShadowControl_m_bEnableLocalLightShadows = 0x595c082a6b1c4fea;
	constexpr uint64_t CShadowControl_m_flShadowMaxDist = 0xb024f69de96e4667;
	constexpr uint64_t CShadowControl_m_shadowColor = 0xebcbc13217bebcd5;
	constexpr uint64_t CShadowControl_m_shadowDirection = 0xc07269b48aed9f35;
	constexpr uint64_t CSlideshowDisplay_m_bEnabled = 0xa3db179365e8b11b;
	constexpr uint64_t CSlideshowDisplay_m_bNoListRepeats = 0xed494613e7be3bba;
	constexpr uint64_t CSlideshowDisplay_m_chCurrentSlideLists = 0x499e1dfa0f2ee77c;
	constexpr uint64_t CSlideshowDisplay_m_fMaxSlideTime = 0xbcae45cbc1cc6026;
	constexpr uint64_t CSlideshowDisplay_m_fMinSlideTime = 0xa091f4587f9f1d8d;
	constexpr uint64_t CSlideshowDisplay_m_iCycleType = 0xda00d919ba7fad62;
	constexpr uint64_t CSlideshowDisplay_m_szDisplayText = 0xdb256aad22136648;
	constexpr uint64_t CSlideshowDisplay_m_szSlideshowDirectory = 0x7dbfbeae531cf17;
	constexpr uint64_t CSmokeGrenadeProjectile_m_bDidSmokeEffect = 0x176d487b176c82af;
	constexpr uint64_t CSmokeGrenadeProjectile_m_nSmokeEffectTickBegin = 0x7e3859f722ebb5fe;
	constexpr uint64_t CSmokeStack_m_AmbientLight_m_flIntensity = 0x9b035112eba1587b;
	constexpr uint64_t CSmokeStack_m_AmbientLight_m_vColor = 0xf4d703b27fffc2a6;
	constexpr uint64_t CSmokeStack_m_AmbientLight_m_vPos = 0x9a396fc4fec1196b;
	constexpr uint64_t CSmokeStack_m_DirLight_m_flIntensity = 0xa20123e081a992b7;
	constexpr uint64_t CSmokeStack_m_DirLight_m_vColor = 0x4f51f3758025372;
	constexpr uint64_t CSmokeStack_m_DirLight_m_vPos = 0x259ba63a81599878;
	constexpr uint64_t CSmokeStack_m_EndSize = 0x90cd19f41a3d2900;
	constexpr uint64_t CSmokeStack_m_JetLength = 0x7c54f89a7b59dc05;
	constexpr uint64_t CSmokeStack_m_Rate = 0x75ed8941b4f34a51;
	constexpr uint64_t CSmokeStack_m_Speed = 0x553b65e844acf15f;
	constexpr uint64_t CSmokeStack_m_SpreadSpeed = 0xd7b8c2a615c98960;
	constexpr uint64_t CSmokeStack_m_StartSize = 0xf36305aa15625c3a;
	constexpr uint64_t CSmokeStack_m_bEmit = 0x98717459f8dc911d;
	constexpr uint64_t CSmokeStack_m_flBaseSpread = 0x2c61277730f3ca0f;
	constexpr uint64_t CSmokeStack_m_flRollSpeed = 0x9b321e6c73aef7ec;
	constexpr uint64_t CSmokeStack_m_flTwist = 0x303b18bc00a4d32f;
	constexpr uint64_t CSmokeStack_m_iMaterialModel = 0x87625bd331ee3375;
	constexpr uint64_t CSmokeStack_m_vWind = 0xbd9b23122a854133;
	constexpr uint64_t CSpatialEntity_m_bEnabled = 0xf2f1ee6ec5c27ed5;
	constexpr uint64_t CSpatialEntity_m_flCurWeight = 0x7f4f9c183cf7c0b;
	constexpr uint64_t CSpatialEntity_m_maxFalloff = 0x588b7dca8ff97c83;
	constexpr uint64_t CSpatialEntity_m_minFalloff = 0x8439d027ff4e254c;
	constexpr uint64_t CSpatialEntity_m_vecOrigin = 0xab385a05d31f1a6f;
	constexpr uint64_t CSpotlightEnd_m_Radius = 0x4422c86d5d578e8a;
	constexpr uint64_t CSpotlightEnd_m_flLightScale = 0x50ef21f05e61464c;
	constexpr uint64_t CSpriteTrail_m_flEndWidth = 0xa1bc7858c7c162cf;
	constexpr uint64_t CSpriteTrail_m_flLifeTime = 0xf419304ee3c9976c;
	constexpr uint64_t CSpriteTrail_m_flMinFadeLength = 0x510f0e27cd4c3b8d;
	constexpr uint64_t CSpriteTrail_m_flSkyboxScale = 0x193d7a91fe3551b4;
	constexpr uint64_t CSpriteTrail_m_flStartWidth = 0x65964ba5de96afa5;
	constexpr uint64_t CSpriteTrail_m_flStartWidthVariance = 0x20a4d537edd5919b;
	constexpr uint64_t CSpriteTrail_m_flTextureRes = 0x79d4577c2cb09be5;
	constexpr uint64_t CSpriteTrail_m_vecSkyboxOrigin = 0x64f367a73dca311d;
	constexpr uint64_t CSprite_m_bWorldSpaceScale = 0xd551154bba4a29a0;
	constexpr uint64_t CSprite_m_flBrightnessTime = 0x81bf5471ef52371;
	constexpr uint64_t CSprite_m_flFrame = 0x4a595be4cc0b8c03;
	constexpr uint64_t CSprite_m_flGlowProxySize = 0xba1a9f3a71fa01df;
	constexpr uint64_t CSprite_m_flHDRColorScale = 0x3bf8906cd4998170;
	constexpr uint64_t CSprite_m_flScaleTime = 0x14d51a405135b796;
	constexpr uint64_t CSprite_m_flSpriteFramerate = 0x3b9682deb0da53ac;
	constexpr uint64_t CSprite_m_flSpriteScale = 0x178c8372c4ed38ca;
	constexpr uint64_t CSprite_m_hAttachedToEntity = 0x315cf0143aff132e;
	constexpr uint64_t CSprite_m_nAttachment = 0xb5bd9f52bb57d457;
	constexpr uint64_t CSprite_m_nBrightness = 0xa744471593877f28;
	constexpr uint64_t CStatueProp_m_bShatter = 0xfb7c7a60c93bcea0;
	constexpr uint64_t CStatueProp_m_hInitBaseAnimating = 0xf4047c51748fe204;
	constexpr uint64_t CStatueProp_m_nShatterFlags = 0xa7e4963510476fd3;
	constexpr uint64_t CStatueProp_m_vShatterForce = 0xeafc6c6329fe5525;
	constexpr uint64_t CStatueProp_m_vShatterPosition = 0x92bc28b233ee3c41;
	constexpr uint64_t CSteamJet_m_EndSize = 0x3f25466f317d7f08;
	constexpr uint64_t CSteamJet_m_JetLength = 0x3804dd9db28bbcaa;
	constexpr uint64_t CSteamJet_m_Rate = 0x220523bfacd2fe1f;
	constexpr uint64_t CSteamJet_m_Speed = 0x546c752df7bff40e;
	constexpr uint64_t CSteamJet_m_SpreadSpeed = 0xc63d2ddf47700be0;
	constexpr uint64_t CSteamJet_m_StartSize = 0x2bd7b4f6abb41a26;
	constexpr uint64_t CSteamJet_m_bEmit = 0x70a4a972f846c03e;
	constexpr uint64_t CSteamJet_m_bFaceLeft = 0x6100e9f2bae51b72;
	constexpr uint64_t CSteamJet_m_flRollSpeed = 0x17a5c376b631869c;
	constexpr uint64_t CSteamJet_m_nType = 0x1a96148551105d90;
	constexpr uint64_t CSteamJet_m_spawnflags = 0x953cf5149d22f300;
	constexpr uint64_t CSun_m_bOn = 0xb63a59b191cd7fac;
	constexpr uint64_t CSun_m_clrOverlay = 0xb95bcf2b0e3e08ed;
	constexpr uint64_t CSun_m_clrRender = 0xba0fb02dc92627ba;
	constexpr uint64_t CSun_m_nMaterial = 0x5a7babd2c242d9d0;
	constexpr uint64_t CSun_m_nOverlayMaterial = 0xd449292908540844;
	constexpr uint64_t CSun_m_nOverlaySize = 0x985f2193786ff70a;
	constexpr uint64_t CSun_m_nSize = 0x5737c074fcec773f;
	constexpr uint64_t CSun_m_vDirection = 0xab5686d78b39444f;
	constexpr uint64_t CSunlightShadowControl_m_LightColor = 0x5f89bea64cb0be18;
	constexpr uint64_t CSunlightShadowControl_m_TextureName = 0x9773a588cf80a39d;
	constexpr uint64_t CSunlightShadowControl_m_bEnableShadows = 0xefb23ea6edab4775;
	constexpr uint64_t CSunlightShadowControl_m_bEnabled = 0xa5a11d01a9c1aba6;
	constexpr uint64_t CSunlightShadowControl_m_flColorTransitionTime = 0xf3482ebf4ab3d16e;
	constexpr uint64_t CSunlightShadowControl_m_flFOV = 0xf6db4757d298bc61;
	constexpr uint64_t CSunlightShadowControl_m_flNearZ = 0x5429c9aae2b8ebf1;
	constexpr uint64_t CSunlightShadowControl_m_flNorthOffset = 0xc179a31f803c567d;
	constexpr uint64_t CSunlightShadowControl_m_flSunDistance = 0x230e0562ac588fd;
	constexpr uint64_t CSunlightShadowControl_m_shadowDirection = 0x9be848c453b62213;
	constexpr uint64_t CSurvivalSpawnChopper_m_vecOrigin = 0x265a169b25784b87;
	constexpr uint64_t CSurvivalSpawnChopper_m_vecOrigin_2_ = 0x5c0a901836151c1;
	constexpr uint64_t CTEBSPDecal_m_nEntity = 0xe75ac1f08faeca98;
	constexpr uint64_t CTEBSPDecal_m_nIndex = 0xb1d42511e9352f4d;
	constexpr uint64_t CTEBSPDecal_m_vecOrigin = 0x2e3fdba98f38f43e;
	constexpr uint64_t CTEBaseBeam_a = 0x803b117ffa44a358;
	constexpr uint64_t CTEBaseBeam_b = 0xe82eb88c22ba03f5;
	constexpr uint64_t CTEBaseBeam_g = 0xa25a30ff3806d267;
	constexpr uint64_t CTEBaseBeam_m_fAmplitude = 0x3ea01faef8e78134;
	constexpr uint64_t CTEBaseBeam_m_fEndWidth = 0x3c4b29d42b61a361;
	constexpr uint64_t CTEBaseBeam_m_fLife = 0xbd00a2246e4a59b2;
	constexpr uint64_t CTEBaseBeam_m_fWidth = 0x7297a8e920642cb4;
	constexpr uint64_t CTEBaseBeam_m_nFadeLength = 0x257ce002dbc9b86d;
	constexpr uint64_t CTEBaseBeam_m_nFlags = 0x57c3409059814592;
	constexpr uint64_t CTEBaseBeam_m_nFrameRate = 0x1e8aa9cfd608a2ab;
	constexpr uint64_t CTEBaseBeam_m_nHaloIndex = 0xb9ae585644f9a9de;
	constexpr uint64_t CTEBaseBeam_m_nModelIndex = 0x7109ea1bd2b4726;
	constexpr uint64_t CTEBaseBeam_m_nSpeed = 0x95620cf3fdd9ef2d;
	constexpr uint64_t CTEBaseBeam_m_nStartFrame = 0x85e15e5a0afe7b13;
	constexpr uint64_t CTEBaseBeam_r = 0xed2f917ee0ffde71;
	constexpr uint64_t CTEBeamEntPoint_m_nEndEntity = 0x78ee1532d1d887aa;
	constexpr uint64_t CTEBeamEntPoint_m_nStartEntity = 0x4a54c1bc20527c2c;
	constexpr uint64_t CTEBeamEntPoint_m_vecEndPoint = 0xcfc159a3af504b07;
	constexpr uint64_t CTEBeamEntPoint_m_vecStartPoint = 0x52bc4b6d07d56f69;
	constexpr uint64_t CTEBeamEnts_m_nEndEntity = 0xb0032b34afefbc10;
	constexpr uint64_t CTEBeamEnts_m_nStartEntity = 0x31956b16df8a88a9;
	constexpr uint64_t CTEBeamFollow_m_iEntIndex = 0x438076526166c469;
	constexpr uint64_t CTEBeamLaser_m_nEndEntity = 0x8f1e784ba8bcb326;
	constexpr uint64_t CTEBeamLaser_m_nStartEntity = 0x13899a75293a56a;
	constexpr uint64_t CTEBeamPoints_m_vecEndPoint = 0xef3f4467fc60493b;
	constexpr uint64_t CTEBeamPoints_m_vecStartPoint = 0xce1c5e22c813be73;
	constexpr uint64_t CTEBeamRingPoint_m_flEndRadius = 0x191548a9549a51d8;
	constexpr uint64_t CTEBeamRingPoint_m_flStartRadius = 0xc3396e76f58bfd41;
	constexpr uint64_t CTEBeamRingPoint_m_vecCenter = 0xd74788738aef463e;
	constexpr uint64_t CTEBeamRing_m_nEndEntity = 0xbf2dffa77750733c;
	constexpr uint64_t CTEBeamRing_m_nStartEntity = 0xb86427f815291042;
	constexpr uint64_t CTEBeamSpline_m_nPoints = 0x72620b59daf6aac0;
	constexpr uint64_t CTEBeamSpline_m_vecPoints_0_ = 0x94de1a93aede3fc6;
	constexpr uint64_t CTEBloodSprite_a = 0x6f93df58c5c38627;
	constexpr uint64_t CTEBloodSprite_b = 0x4da3a57271404e94;
	constexpr uint64_t CTEBloodSprite_g = 0x2c38de7ca5bc9234;
	constexpr uint64_t CTEBloodSprite_m_nDropModel = 0xc87f205c4c0f73f0;
	constexpr uint64_t CTEBloodSprite_m_nSize = 0xfc632b09ca8cfd0c;
	constexpr uint64_t CTEBloodSprite_m_nSprayModel = 0x2c01338de8312a5b;
	constexpr uint64_t CTEBloodSprite_m_vecDirection = 0x17b125085f756d2a;
	constexpr uint64_t CTEBloodSprite_m_vecOrigin = 0x20494727f4214bac;
	constexpr uint64_t CTEBloodSprite_r = 0x8830d3fffd2d085a;
	constexpr uint64_t CTEBloodStream_a = 0xa3feda611875cccd;
	constexpr uint64_t CTEBloodStream_b = 0xc838fb8750fb22d1;
	constexpr uint64_t CTEBloodStream_g = 0x59e48859407a1a2;
	constexpr uint64_t CTEBloodStream_m_nAmount = 0xa52599042ec826a0;
	constexpr uint64_t CTEBloodStream_m_vecDirection = 0xec0fc385976524c7;
	constexpr uint64_t CTEBloodStream_r = 0xa4f778638d645b63;
	constexpr uint64_t CTEBreakModel_m_angRotation_0_ = 0x3a8b6572a646e12b;
	constexpr uint64_t CTEBreakModel_m_angRotation_1_ = 0xd932f42604e5332e;
	constexpr uint64_t CTEBreakModel_m_angRotation_2_ = 0x4e67452cecf67be0;
	constexpr uint64_t CTEBreakModel_m_fTime = 0xdb6c09e59a04b8f1;
	constexpr uint64_t CTEBreakModel_m_nCount = 0xfc5c9c1b5ddd972c;
	constexpr uint64_t CTEBreakModel_m_nFlags = 0x8b04f957e2d561c4;
	constexpr uint64_t CTEBreakModel_m_nModelIndex = 0x2139e645bbd2142b;
	constexpr uint64_t CTEBreakModel_m_nRandomization = 0xb73f0aebf4cb057b;
	constexpr uint64_t CTEBreakModel_m_vecOrigin = 0x18dcba677639efb7;
	constexpr uint64_t CTEBreakModel_m_vecSize = 0xcf9efc8b7f615ae9;
	constexpr uint64_t CTEBreakModel_m_vecVelocity = 0xbf62aa9fde9c937;
	constexpr uint64_t CTEBubbleTrail_m_fSpeed = 0xf6dfbcd67e1c0691;
	constexpr uint64_t CTEBubbleTrail_m_flWaterZ = 0xecdf962725e1b9a8;
	constexpr uint64_t CTEBubbleTrail_m_nCount = 0xbb83c24240b6f34f;
	constexpr uint64_t CTEBubbleTrail_m_nModelIndex = 0xf2d924738f61444e;
	constexpr uint64_t CTEBubbleTrail_m_vecMaxs = 0x249b4224e4ecd72d;
	constexpr uint64_t CTEBubbleTrail_m_vecMins = 0x29453a1f0d409063;
	constexpr uint64_t CTEBubbles_m_fHeight = 0xd1f8e74dd37fafba;
	constexpr uint64_t CTEBubbles_m_fSpeed = 0x64d9f1c0c8468ad4;
	constexpr uint64_t CTEBubbles_m_nCount = 0xbac6a886a539bca7;
	constexpr uint64_t CTEBubbles_m_nModelIndex = 0x28e6fc8af2da37eb;
	constexpr uint64_t CTEBubbles_m_vecMaxs = 0x30f0bce43f3dd298;
	constexpr uint64_t CTEBubbles_m_vecMins = 0x9d0eeb21192b4cbd;
	constexpr uint64_t CTEClientProjectile_m_hOwner = 0xf9a7d8f6d03d13b6;
	constexpr uint64_t CTEClientProjectile_m_nLifeTime = 0xed22dbb7199caca2;
	constexpr uint64_t CTEClientProjectile_m_nModelIndex = 0x2a071755c543d96c;
	constexpr uint64_t CTEClientProjectile_m_vecOrigin = 0x3149913fdf53c1c0;
	constexpr uint64_t CTEClientProjectile_m_vecVelocity = 0x93c80c786b2f8290;
	constexpr uint64_t CTEDecal_m_nEntity = 0x883db723cb066d9f;
	constexpr uint64_t CTEDecal_m_nHitbox = 0x8fd2bc7cb6eb57b8;
	constexpr uint64_t CTEDecal_m_nIndex = 0xc6629c4997770861;
	constexpr uint64_t CTEDecal_m_vecOrigin = 0x8763036fc2a511ad;
	constexpr uint64_t CTEDecal_m_vecStart = 0xcb88ba91dba7bfce;
	constexpr uint64_t CTEDust_m_flSize = 0x725ae4d5ceba0fdf;
	constexpr uint64_t CTEDust_m_flSpeed = 0xf2303062a7d00fec;
	constexpr uint64_t CTEDust_m_vecDirection = 0xe1478341e00d4635;
	constexpr uint64_t CTEDynamicLight_b = 0x58557ce83373fee5;
	constexpr uint64_t CTEDynamicLight_exponent = 0xb2a4e5deebb34b5f;
	constexpr uint64_t CTEDynamicLight_g = 0x6315b8f4e30eab3f;
	constexpr uint64_t CTEDynamicLight_m_fDecay = 0x96edd688a74783e2;
	constexpr uint64_t CTEDynamicLight_m_fRadius = 0xe15073258423d713;
	constexpr uint64_t CTEDynamicLight_m_fTime = 0x1b3805f3e997352b;
	constexpr uint64_t CTEDynamicLight_m_vecOrigin = 0x42a80de5fd6412fb;
	constexpr uint64_t CTEDynamicLight_r = 0xd99b048edcbad4a9;
	constexpr uint64_t CTEEffectDispatch_entindex = 0x792b93e51b7394bf;
	constexpr uint64_t CTEEffectDispatch_m_EffectData = 0x1245cc79d35a8fa6;
	constexpr uint64_t CTEEffectDispatch_m_bPositionsAreRelativeToEntity = 0x9b27cff6803d6aa9;
	constexpr uint64_t CTEEffectDispatch_m_fFlags = 0x3a7828a93c72457;
	constexpr uint64_t CTEEffectDispatch_m_flMagnitude = 0xdda4694cc3ca679c;
	constexpr uint64_t CTEEffectDispatch_m_flRadius = 0xe235331ffc9f83f1;
	constexpr uint64_t CTEEffectDispatch_m_flScale = 0x656776e67f712789;
	constexpr uint64_t CTEEffectDispatch_m_iEffectName = 0x9428ffa1d9cd76fa;
	constexpr uint64_t CTEEffectDispatch_m_nAttachmentIndex = 0xa666852c7fd70783;
	constexpr uint64_t CTEEffectDispatch_m_nColor = 0x2bcd0c16006c5079;
	constexpr uint64_t CTEEffectDispatch_m_nDamageType = 0x1e7a3d2e7a563103;
	constexpr uint64_t CTEEffectDispatch_m_nHitBox = 0xb5309a53c5d94677;
	constexpr uint64_t CTEEffectDispatch_m_nMaterial = 0xade4d3c3f1bd9650;
	constexpr uint64_t CTEEffectDispatch_m_nOtherEntIndex = 0x4277434ea304ae1e;
	constexpr uint64_t CTEEffectDispatch_m_nSurfaceProp = 0xbebee4f7265b0dcc;
	constexpr uint64_t CTEEffectDispatch_m_vAngles = 0x8b5158f42cc6f113;
	constexpr uint64_t CTEEffectDispatch_m_vNormal = 0x398311442adc22b5;
	constexpr uint64_t CTEEffectDispatch_m_vOrigin_x = 0xeb96fd9bdadc5d10;
	constexpr uint64_t CTEEffectDispatch_m_vOrigin_y = 0x65eaafeef7edcb9;
	constexpr uint64_t CTEEffectDispatch_m_vOrigin_z = 0x24a218c725473a0d;
	constexpr uint64_t CTEEffectDispatch_m_vStart_x = 0xb62b63938f71f4ea;
	constexpr uint64_t CTEEffectDispatch_m_vStart_y = 0xb27984db9b5659cc;
	constexpr uint64_t CTEEffectDispatch_m_vStart_z = 0xeee3d23a71b9eeeb;
	constexpr uint64_t CTEEnergySplash_m_bExplosive = 0x69e470aee4479c55;
	constexpr uint64_t CTEEnergySplash_m_vecDir = 0x4f48de4ec7a18caa;
	constexpr uint64_t CTEEnergySplash_m_vecPos = 0xc8b6e54ff214388f;
	constexpr uint64_t CTEExplosion_m_chMaterialType = 0x22170e46af71f6b9;
	constexpr uint64_t CTEExplosion_m_fScale = 0x7b98bc95de82c8d7;
	constexpr uint64_t CTEExplosion_m_nFlags = 0xdd9a9f87a015b9ee;
	constexpr uint64_t CTEExplosion_m_nFrameRate = 0x6d06f6b870417fed;
	constexpr uint64_t CTEExplosion_m_nMagnitude = 0xdf5f360e5e86f254;
	constexpr uint64_t CTEExplosion_m_nModelIndex = 0xecf2d92ef0c2add6;
	constexpr uint64_t CTEExplosion_m_nRadius = 0x68bfee7a45712ab7;
	constexpr uint64_t CTEExplosion_m_vecNormal = 0xefb56f7e0de2164d;
	constexpr uint64_t CTEFireBullets_m_fInaccuracy = 0x5cf59013d0a3fead;
	constexpr uint64_t CTEFireBullets_m_fSpread = 0x5fa19c2d730630c2;
	constexpr uint64_t CTEFireBullets_m_flRecoilIndex = 0x400e1f48134b3a96;
	constexpr uint64_t CTEFireBullets_m_iMode = 0x57f4afde4524cc57;
	constexpr uint64_t CTEFireBullets_m_iPlayer = 0xca03f21a610e532e;
	constexpr uint64_t CTEFireBullets_m_iSeed = 0x577e2cd09f40983c;
	constexpr uint64_t CTEFireBullets_m_iSoundType = 0x30ca744ea2a0c309;
	constexpr uint64_t CTEFireBullets_m_iWeaponID = 0x6c7eb862a7f6294a;
	constexpr uint64_t CTEFireBullets_m_nItemDefIndex = 0xfb15b3344f5c3ab3;
	constexpr uint64_t CTEFireBullets_m_vecAngles_0_ = 0x13dfd73bb96fcd27;
	constexpr uint64_t CTEFireBullets_m_vecAngles_1_ = 0x6e39b234a0b357fc;
	constexpr uint64_t CTEFireBullets_m_vecOrigin = 0x991c59d84ae522c1;
	constexpr uint64_t CTEFireBullets_m_weapon = 0x830059f7aedd8caa;
	constexpr uint64_t CTEFizz_m_nCurrent = 0x173d8ba50630cdb7;
	constexpr uint64_t CTEFizz_m_nDensity = 0xa95cdcaa0ad48c1a;
	constexpr uint64_t CTEFizz_m_nEntity = 0xa431ffe3e703fe9;
	constexpr uint64_t CTEFizz_m_nModelIndex = 0xbfebb7da6ef9e14f;
	constexpr uint64_t CTEFootprintDecal_m_chMaterialType = 0xb1679c7ba2a755ed;
	constexpr uint64_t CTEFootprintDecal_m_nEntity = 0xf246dda691d41ac2;
	constexpr uint64_t CTEFootprintDecal_m_nIndex = 0x3c2b48696d439bb4;
	constexpr uint64_t CTEFootprintDecal_m_vecDirection = 0x8ccf55820cb665d9;
	constexpr uint64_t CTEFootprintDecal_m_vecOrigin = 0xfbfb3125e83e6298;
	constexpr uint64_t CTEFoundryHelpers_m_iEntity = 0xadb5296e7f0254e0;
	constexpr uint64_t CTEGaussExplosion_m_nType = 0x3c9d079963d5e553;
	constexpr uint64_t CTEGaussExplosion_m_vecDirection = 0x645d1014ade9dde3;
	constexpr uint64_t CTEGlowSprite_m_fLife = 0x3d777d54d26bf66;
	constexpr uint64_t CTEGlowSprite_m_fScale = 0x6926256ae1305cd1;
	constexpr uint64_t CTEGlowSprite_m_nBrightness = 0x1e31973e7702359a;
	constexpr uint64_t CTEGlowSprite_m_nModelIndex = 0x4671c6b4e5afdf1b;
	constexpr uint64_t CTEGlowSprite_m_vecOrigin = 0x6989879b38b2a561;
	constexpr uint64_t CTEImpact_m_iType = 0x9cba74b168be09ae;
	constexpr uint64_t CTEImpact_m_ucFlags = 0x2da6d65438a4556f;
	constexpr uint64_t CTEImpact_m_vecNormal = 0x19df801bfbf2282e;
	constexpr uint64_t CTEImpact_m_vecOrigin = 0x9952ef19d2bbaab7;
	constexpr uint64_t CTEKillPlayerAttachments_m_nPlayer = 0x6a20120ce2c6f719;
	constexpr uint64_t CTELargeFunnel_m_nModelIndex = 0x9a5f52cedd22c8b5;
	constexpr uint64_t CTELargeFunnel_m_nReversed = 0xac67614a2393e12;
	constexpr uint64_t CTEMetalSparks_m_vecDir = 0x1398483634056709;
	constexpr uint64_t CTEMetalSparks_m_vecPos = 0xc8f7ee75edea4ea3;
	constexpr uint64_t CTEMuzzleFlash_m_flScale = 0x4eee1042ca2df3c4;
	constexpr uint64_t CTEMuzzleFlash_m_nType = 0x1b771205309939a7;
	constexpr uint64_t CTEMuzzleFlash_m_vecAngles = 0x361ab9044929489;
	constexpr uint64_t CTEMuzzleFlash_m_vecOrigin = 0x5362cb619e490644;
	constexpr uint64_t CTEParticleSystem_m_vecOrigin_0_ = 0xc8e3db890998d9b7;
	constexpr uint64_t CTEParticleSystem_m_vecOrigin_1_ = 0x486dbc68e8201451;
	constexpr uint64_t CTEParticleSystem_m_vecOrigin_2_ = 0xe7e8752ba9b4d45f;
	constexpr uint64_t CTEPhysicsProp_m_angRotation_0_ = 0x7cbf019444122714;
	constexpr uint64_t CTEPhysicsProp_m_angRotation_1_ = 0xdfda73a3ffdb2d1d;
	constexpr uint64_t CTEPhysicsProp_m_angRotation_2_ = 0x23048c6598213d00;
	constexpr uint64_t CTEPhysicsProp_m_clrRender = 0xb6fc7c2171ee5c42;
	constexpr uint64_t CTEPhysicsProp_m_nEffects = 0x1389db53a0f4cd7a;
	constexpr uint64_t CTEPhysicsProp_m_nFlags = 0xccab4b3f2de81a67;
	constexpr uint64_t CTEPhysicsProp_m_nModelIndex = 0xb2ee0db344224137;
	constexpr uint64_t CTEPhysicsProp_m_nSkin = 0x3efe6c91248816e6;
	constexpr uint64_t CTEPhysicsProp_m_vecOrigin = 0x863586113cdf5ed6;
	constexpr uint64_t CTEPhysicsProp_m_vecVelocity = 0xc757609dde5f7626;
	constexpr uint64_t CTEPlantBomb_m_iPlayer = 0xb0eb1ed649c0776c;
	constexpr uint64_t CTEPlantBomb_m_option = 0x9a4dbe1a630bc54e;
	constexpr uint64_t CTEPlantBomb_m_vecOrigin = 0x55a14e1b82a3afc4;
	constexpr uint64_t CTEPlayerAnimEvent_m_hPlayer = 0xfa930786b6c3341b;
	constexpr uint64_t CTEPlayerAnimEvent_m_iEvent = 0x51bc2750d622692;
	constexpr uint64_t CTEPlayerAnimEvent_m_nData = 0x9bd152e8f2d49e97;
	constexpr uint64_t CTEPlayerDecal_m_nEntity = 0x22dbbf1584f6c7e9;
	constexpr uint64_t CTEPlayerDecal_m_nHitbox = 0xbf9485b1ff69f4e2;
	constexpr uint64_t CTEPlayerDecal_m_nPlayer = 0x524504372b202f2;
	constexpr uint64_t CTEPlayerDecal_m_vecOrigin = 0x5431b0401f6d094c;
	constexpr uint64_t CTEPlayerDecal_m_vecRight = 0x18e72e70dd65421;
	constexpr uint64_t CTEPlayerDecal_m_vecStart = 0x70fe39239799e289;
	constexpr uint64_t CTEProjectedDecal_m_angRotation = 0x4f50b93807aa692d;
	constexpr uint64_t CTEProjectedDecal_m_flDistance = 0xaf18bc20df77ed35;
	constexpr uint64_t CTEProjectedDecal_m_nIndex = 0xa3f3d62bf3bb036a;
	constexpr uint64_t CTEProjectedDecal_m_vecOrigin = 0x69cebbcd9f1b81f2;
	constexpr uint64_t CTERadioIcon_m_iAttachToClient = 0x13d90f5e7d339b74;
	constexpr uint64_t CTEShatterSurface_m_flHeight = 0x2f774aa67d50a329;
	constexpr uint64_t CTEShatterSurface_m_flShardSize = 0x69a0124ae4729ee9;
	constexpr uint64_t CTEShatterSurface_m_flWidth = 0xb22ebbd2d71d6bee;
	constexpr uint64_t CTEShatterSurface_m_nSurfaceType = 0x9b6a4d3c45f6795c;
	constexpr uint64_t CTEShatterSurface_m_uchBackColor_0_ = 0x3d32ec2c3365e94;
	constexpr uint64_t CTEShatterSurface_m_uchBackColor_1_ = 0xb1206d2c5b34c5e9;
	constexpr uint64_t CTEShatterSurface_m_uchBackColor_2_ = 0x174b7611056039e3;
	constexpr uint64_t CTEShatterSurface_m_uchFrontColor_0_ = 0xc08f45ee76508c91;
	constexpr uint64_t CTEShatterSurface_m_uchFrontColor_1_ = 0xbe3cdaaf59ec7e4c;
	constexpr uint64_t CTEShatterSurface_m_uchFrontColor_2_ = 0x59484a2fe0ac3af3;
	constexpr uint64_t CTEShatterSurface_m_vecAngles = 0x24f083185196cea5;
	constexpr uint64_t CTEShatterSurface_m_vecForce = 0xe1827e470ac74fb9;
	constexpr uint64_t CTEShatterSurface_m_vecForcePos = 0x5607ac0db1fccef6;
	constexpr uint64_t CTEShatterSurface_m_vecOrigin = 0x3416604c2b6727f;
	constexpr uint64_t CTEShowLine_m_vecEnd = 0x85a732dc0863c2f8;
	constexpr uint64_t CTESmoke_m_fScale = 0xaecf31875b915fff;
	constexpr uint64_t CTESmoke_m_nFrameRate = 0x967d9da35ee1a255;
	constexpr uint64_t CTESmoke_m_nModelIndex = 0xeded1412e8e37f8f;
	constexpr uint64_t CTESmoke_m_vecOrigin = 0x62b70e1820172157;
	constexpr uint64_t CTESparks_m_nMagnitude = 0x194b1e2bed4bef4f;
	constexpr uint64_t CTESparks_m_nTrailLength = 0x3589c67d51b24885;
	constexpr uint64_t CTESparks_m_vecDir = 0x98bb0425e9a7d2e;
	constexpr uint64_t CTESpriteSpray_m_fNoise = 0x487faefd6df87d2d;
	constexpr uint64_t CTESpriteSpray_m_nCount = 0x9e47a58772837a0;
	constexpr uint64_t CTESpriteSpray_m_nModelIndex = 0x393f94d86fa64278;
	constexpr uint64_t CTESpriteSpray_m_nSpeed = 0x41d8b73e1eef5bd2;
	constexpr uint64_t CTESpriteSpray_m_vecDirection = 0xd2327d34546152cd;
	constexpr uint64_t CTESpriteSpray_m_vecOrigin = 0x850665f9fe55120d;
	constexpr uint64_t CTESprite_m_fScale = 0x2cd04089278a90c4;
	constexpr uint64_t CTESprite_m_nBrightness = 0xa11b1f3e2d04e162;
	constexpr uint64_t CTESprite_m_nModelIndex = 0x2487d1e5c648f0f4;
	constexpr uint64_t CTESprite_m_vecOrigin = 0xeb51726ef59be563;
	constexpr uint64_t CTEWorldDecal_m_nIndex = 0x4a78095bc5cccb4a;
	constexpr uint64_t CTEWorldDecal_m_vecOrigin = 0xf658bd98fd3f4785;
	constexpr uint64_t CTablet_m_bTabletReceptionIsBlocked = 0x2461808ebc1ffd43;
	constexpr uint64_t CTablet_m_flBootTime = 0xb1badc9cd5268a08;
	constexpr uint64_t CTablet_m_flScanProgress = 0x69b068e5c1bb32c5;
	constexpr uint64_t CTablet_m_flShowMapTime = 0x3452fa0cd2ba354c;
	constexpr uint64_t CTablet_m_flUpgradeExpirationTime_0_ = 0x7bfad48996441fd7;
	constexpr uint64_t CTablet_m_nContractKillGridHighResIndex = 0x35635fd98143d68d;
	constexpr uint64_t CTablet_m_nContractKillGridIndex = 0xb95cafca165f89f8;
	constexpr uint64_t CTablet_m_nLastPurchaseIndex = 0x33ce730796a47f3c;
	constexpr uint64_t CTablet_m_vecLocalHexFlags_0_ = 0x6c56b65446d9f39e;
	constexpr uint64_t CTablet_m_vecNearestMetalCratePos = 0x59a1ecddcbde2db3;
	constexpr uint64_t CTablet_m_vecNotificationIds_0_ = 0x6905288726cf9f0e;
	constexpr uint64_t CTablet_m_vecNotificationTimestamps_0_ = 0xbcaef5eb840fabff;
	constexpr uint64_t CTablet_m_vecPlayerPositionHistory_0_ = 0x18857281c5175c29;
	constexpr uint64_t CTeam_m_bSurrendered = 0x361088b59eff92ba;
	constexpr uint64_t CTeam_m_iClanID = 0x20cefd0fe399f747;
	constexpr uint64_t CTeam_m_iTeamNum = 0x598f2fa98873c1fd;
	constexpr uint64_t CTeam_m_nGGLeaderEntIndex_CT = 0xed47eff5feb6161d;
	constexpr uint64_t CTeam_m_nGGLeaderEntIndex_T = 0xe5de7362d76a5a8c;
	constexpr uint64_t CTeam_m_numMapVictories = 0x2db841fd71671b80;
	constexpr uint64_t CTeam_m_scoreFirstHalf = 0x8e6bf4cdfd155fbb;
	constexpr uint64_t CTeam_m_scoreOvertime = 0xfe19fe8c45a50b1b;
	constexpr uint64_t CTeam_m_scoreSecondHalf = 0xfa8c2a20ee91600b;
	constexpr uint64_t CTeam_m_scoreTotal = 0x34113faf7f1891e;
	constexpr uint64_t CTeam_m_szClanTeamname = 0xd902b180cf3ed8ed;
	constexpr uint64_t CTeam_m_szTeamFlagImage = 0x24ab93631137f02d;
	constexpr uint64_t CTeam_m_szTeamLogoImage = 0xf22b1bf540ed67b2;
	constexpr uint64_t CTeam_m_szTeamMatchStat = 0x83c9797bd7662d09;
	constexpr uint64_t CTeam_m_szTeamname = 0x1b5ac04a17d666b8;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_TeamRespawnWaveTimes = 0xfe2f0a256b7f403;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_bAwaitingReadyRestart = 0xd8e3914284b93668;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_bInOvertime = 0x6606622c1346298c;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_bInSetup = 0x401a31b9cba72b9;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_bInWaitingForPlayers = 0x695162621300694;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_bStopWatch = 0x1f889cbe935f1294;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_bSwitchedTeamsThisRound = 0x2bb964f8b0c30a70;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_bTeamReady = 0xb518e9855a9d8214;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_flMapResetTime = 0x74c54b674f13c5b8;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_flNextRespawnWave = 0x6e0558a60500f883;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_flRestartRoundTime = 0x3039ab0576173e53;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_iRoundState = 0x8257aec35875ba58;
	constexpr uint64_t CTeamplayRoundBasedRulesProxy_m_iWinningTeam = 0x315e0245148099fb;
	constexpr uint64_t CTesla_m_SoundName = 0xa5b9cc765149249;
	constexpr uint64_t CTesla_m_iszSpriteName = 0x2ada6c0aad621e86;
	constexpr uint64_t CTestTraceline_m_angRotation_0_ = 0x776117d7e7c76dee;
	constexpr uint64_t CTestTraceline_m_angRotation_1_ = 0x43f146dc7b61ac0e;
	constexpr uint64_t CTestTraceline_m_angRotation_2_ = 0x5b12862500c45667;
	constexpr uint64_t CTestTraceline_m_clrRender = 0xefe8ecc61954095c;
	constexpr uint64_t CTestTraceline_m_vecOrigin = 0x5d232f10d84e1200;
	constexpr uint64_t CTestTraceline_moveparent = 0x657d331b4663c179;
	constexpr uint64_t CTest_ProxyToggle_Networkable_m_WithProxy = 0xaec4a34b9753bdbb;
	constexpr uint64_t CTriggerSoundOperator_m_nSoundOperator = 0xfe9527383dd5a05b;
	constexpr uint64_t CVGuiScreen_m_fScreenFlags = 0x55f09eac4927e8a5;
	constexpr uint64_t CVGuiScreen_m_flHeight = 0x4d52189404055fd6;
	constexpr uint64_t CVGuiScreen_m_flWidth = 0xc854a47acc4fc2b7;
	constexpr uint64_t CVGuiScreen_m_hPlayerOwner = 0xa47c44d38aca67e8;
	constexpr uint64_t CVGuiScreen_m_nAttachmentIndex = 0xac1c1cdcc62b851e;
	constexpr uint64_t CVGuiScreen_m_nOverlayMaterial = 0x3db61004e2c204e3;
	constexpr uint64_t CVGuiScreen_m_nPanelName = 0x5300947ee304234;
	constexpr uint64_t CVoteController_m_bIsYesNoVote = 0x2a9ab7ec4ab0be7b;
	constexpr uint64_t CVoteController_m_iActiveIssueIndex = 0xb35a6ca2b84b94bb;
	constexpr uint64_t CVoteController_m_iOnlyTeamToVote = 0x7305d8a395aa43dc;
	constexpr uint64_t CVoteController_m_nPotentialVotes = 0xff808f4732e403d4;
	constexpr uint64_t CVoteController_m_nVoteOptionCount = 0xc14db084b915b3d5;
	constexpr uint64_t CWaterLODControl_m_flCheapWaterEndDistance = 0xf941f58080deec47;
	constexpr uint64_t CWaterLODControl_m_flCheapWaterStartDistance = 0x627b1fe14f98d811;
	constexpr uint64_t CWeaponBaseItem_m_bRedraw = 0x300dca9dfb4c48ef;
	constexpr uint64_t CWeaponCSBaseGun_m_iBurstShotsRemaining = 0x334ba8aef98ad181;
	constexpr uint64_t CWeaponCSBaseGun_m_zoomLevel = 0xa5a361f838bd387;
	constexpr uint64_t CWeaponCSBase_m_bBurstMode = 0x8869271aa12c3b8b;
	constexpr uint64_t CWeaponCSBase_m_bReloadVisuallyComplete = 0xc2ca19380c61909d;
	constexpr uint64_t CWeaponCSBase_m_bSilencerOn = 0x2c2470647662e8ae;
	constexpr uint64_t CWeaponCSBase_m_fAccuracyPenalty = 0xe59404b77ee1c895;
	constexpr uint64_t CWeaponCSBase_m_fLastShotTime = 0x5b7cf4b7eeb718a4;
	constexpr uint64_t CWeaponCSBase_m_flDoneSwitchingSilencer = 0xbd750a9ab1fb3b0d;
	constexpr uint64_t CWeaponCSBase_m_flPostponeFireReadyTime = 0xccba599fb9af41a5;
	constexpr uint64_t CWeaponCSBase_m_flRecoilIndex = 0x198cc4352019f21e;
	constexpr uint64_t CWeaponCSBase_m_hPrevOwner = 0xeac4691e9387b4c3;
	constexpr uint64_t CWeaponCSBase_m_iIronSightMode = 0x6458f8405e9dafb6;
	constexpr uint64_t CWeaponCSBase_m_iOriginalTeamNumber = 0x164ccadd9a07f895;
	constexpr uint64_t CWeaponCSBase_m_iRecoilIndex = 0x19de3187a1f803a8;
	constexpr uint64_t CWeaponCSBase_m_weaponMode = 0x15993d7a92281c4a;
	constexpr uint64_t CWeaponM3_m_reloadState = 0x8744ea8070081398;
	constexpr uint64_t CWeaponNOVA_m_reloadState = 0xeae6b71ac8215e1f;
	constexpr uint64_t CWeaponSawedoff_m_reloadState = 0x5d099ff124a866f9;
	constexpr uint64_t CWeaponShield_m_flDisplayHealth = 0x32fc032022a15cff;
	constexpr uint64_t CWeaponTaser_m_fFireTime = 0x934d1d1d58030c06;
	constexpr uint64_t CWeaponXM1014_m_reloadState = 0xa88e1a4e22d177b8;
	constexpr uint64_t CWorldVguiText_m_bEnabled = 0x44e31155f3a48de8;
	constexpr uint64_t CWorldVguiText_m_clrText = 0xb59d47568ab2b50b;
	constexpr uint64_t CWorldVguiText_m_iTextPanelWidth = 0xbb1411950b745dd6;
	constexpr uint64_t CWorldVguiText_m_szDisplayText = 0x13b4a8a9588afc37;
	constexpr uint64_t CWorldVguiText_m_szDisplayTextOption = 0xe64fb6056f2b0316;
	constexpr uint64_t CWorldVguiText_m_szFont = 0xd0b7563dc443cffa;
	constexpr uint64_t CWorld_m_WorldMaxs = 0xe60477fb98de91da;
	constexpr uint64_t CWorld_m_WorldMins = 0xf405bdf34c58c270;
	constexpr uint64_t CWorld_m_bColdWorld = 0xf5ff9d81bf60db82;
	constexpr uint64_t CWorld_m_bStartDark = 0x516276f62d10227a;
	constexpr uint64_t CWorld_m_flMaxOccludeeArea = 0xc57aaa15040bb4a8;
	constexpr uint64_t CWorld_m_flMaxPropScreenSpaceWidth = 0x5c30910a2550b237;
	constexpr uint64_t CWorld_m_flMinOccluderArea = 0x1a0d43c5cb600e46;
	constexpr uint64_t CWorld_m_flMinPropScreenSpaceWidth = 0xec6f8f0428db5fcc;
	constexpr uint64_t CWorld_m_flWaveHeight = 0x3b91dc90c3f0af28;
	constexpr uint64_t CWorld_m_iTimeOfDay = 0x30b48946bb92af;
	constexpr uint64_t CWorld_m_iszDetailSpriteMaterial = 0x126e512e33d935;
	constexpr uint64_t DustTrail_m_Color = 0x9dc662f413b95ec9;
	constexpr uint64_t DustTrail_m_EndSize = 0x4d1be2125177e290;
	constexpr uint64_t DustTrail_m_MaxDirectedSpeed = 0xad58d91f83a27034;
	constexpr uint64_t DustTrail_m_MaxSpeed = 0x822cd9aa0e1fec76;
	constexpr uint64_t DustTrail_m_MinDirectedSpeed = 0x49021b300cf11ed3;
	constexpr uint64_t DustTrail_m_MinSpeed = 0x17664c1503088052;
	constexpr uint64_t DustTrail_m_Opacity = 0xcd691890bc1acc08;
	constexpr uint64_t DustTrail_m_ParticleLifetime = 0x252bff6160545af8;
	constexpr uint64_t DustTrail_m_SpawnRadius = 0xea2b160bfb3891d2;
	constexpr uint64_t DustTrail_m_SpawnRate = 0xce795cd00323f6da;
	constexpr uint64_t DustTrail_m_StartSize = 0xce44ebb5b8baae4;
	constexpr uint64_t DustTrail_m_StopEmitTime = 0xb5cf9d539c1690ef;
	constexpr uint64_t DustTrail_m_bEmit = 0x207facf06b8b20ec;
	constexpr uint64_t ParticleSmokeGrenade_m_CurrentStage = 0xd42b1abbb518ed9a;
	constexpr uint64_t ParticleSmokeGrenade_m_FadeEndTime = 0x1c31dcb5b66b939a;
	constexpr uint64_t ParticleSmokeGrenade_m_FadeStartTime = 0x7bd100ca18e3e539;
	constexpr uint64_t ParticleSmokeGrenade_m_MaxColor = 0x3ca9123ade7cabd3;
	constexpr uint64_t ParticleSmokeGrenade_m_MinColor = 0xa56503e0ba19f7ab;
	constexpr uint64_t ParticleSmokeGrenade_m_flSpawnTime = 0x354fd912ad2a737d;
	constexpr uint64_t RocketTrail_m_EndColor = 0xbef9a0e76ec5d463;
	constexpr uint64_t RocketTrail_m_EndSize = 0x63cd46259662f768;
	constexpr uint64_t RocketTrail_m_MaxSpeed = 0xdc1efda9748e9e60;
	constexpr uint64_t RocketTrail_m_MinSpeed = 0x1facae03bb02c20e;
	constexpr uint64_t RocketTrail_m_Opacity = 0xe69769b010346970;
	constexpr uint64_t RocketTrail_m_ParticleLifetime = 0xea648e4d63ed3a6;
	constexpr uint64_t RocketTrail_m_SpawnRadius = 0x76ac530febe447f;
	constexpr uint64_t RocketTrail_m_SpawnRate = 0x9668b057a90ebc65;
	constexpr uint64_t RocketTrail_m_StartColor = 0xbdb3f99e3a9b6b37;
	constexpr uint64_t RocketTrail_m_StartSize = 0xfb3d503e07e9bcb3;
	constexpr uint64_t RocketTrail_m_StopEmitTime = 0x1af383be5912b2e0;
	constexpr uint64_t RocketTrail_m_bDamaged = 0xe3454e0ecc57da0f;
	constexpr uint64_t RocketTrail_m_bEmit = 0x408d97a9177659c1;
	constexpr uint64_t RocketTrail_m_flFlareScale = 0xb25d13ab40a1eefa;
	constexpr uint64_t RocketTrail_m_nAttachment = 0x1982481684853bbb;
	constexpr uint64_t SmokeTrail_m_EndColor = 0x73f22472780e9bde;
	constexpr uint64_t SmokeTrail_m_EndSize = 0x1146422c186c6db9;
	constexpr uint64_t SmokeTrail_m_MaxDirectedSpeed = 0x1a2fda9af45a31f3;
	constexpr uint64_t SmokeTrail_m_MaxSpeed = 0xa278d95ec8d10aa5;
	constexpr uint64_t SmokeTrail_m_MinDirectedSpeed = 0x189e183ccf03c56c;
	constexpr uint64_t SmokeTrail_m_MinSpeed = 0xc49d968153a67dd7;
	constexpr uint64_t SmokeTrail_m_Opacity = 0xdbd5889fe80c43f7;
	constexpr uint64_t SmokeTrail_m_ParticleLifetime = 0x4084819e48778497;
	constexpr uint64_t SmokeTrail_m_SpawnRadius = 0x218630731832e52d;
	constexpr uint64_t SmokeTrail_m_SpawnRate = 0xc05a9c60122b013f;
	constexpr uint64_t SmokeTrail_m_StartColor = 0xcf0b8144984ace95;
	constexpr uint64_t SmokeTrail_m_StartSize = 0x7bfbf11fe7a86056;
	constexpr uint64_t SmokeTrail_m_StopEmitTime = 0xc1860a08b012456;
	constexpr uint64_t SmokeTrail_m_bEmit = 0x57fe6818ecef215e;
	constexpr uint64_t SmokeTrail_m_nAttachment = 0x7f50c6d8658dea5b;
	constexpr uint64_t SporeExplosion_m_bDontRemove = 0x19d8aa53a0ce5241;
	constexpr uint64_t SporeExplosion_m_bEmit = 0x58e82619b407d4f3;
	constexpr uint64_t SporeExplosion_m_flEndSize = 0xaa3d930fad5dfe13;
	constexpr uint64_t SporeExplosion_m_flParticleLifetime = 0xe0ff7f502f732f11;
	constexpr uint64_t SporeExplosion_m_flSpawnRadius = 0x46d0c3b07753525f;
	constexpr uint64_t SporeExplosion_m_flSpawnRate = 0x3e3f47f9bb221c47;
	constexpr uint64_t SporeExplosion_m_flStartSize = 0xf6b3333a1256bf61;
	constexpr uint64_t SporeTrail_m_bEmit = 0x1794221dbd2ceba2;
	constexpr uint64_t SporeTrail_m_flEndSize = 0x640a43a39f892bee;
	constexpr uint64_t SporeTrail_m_flParticleLifetime = 0xed4df61520b49891;
	constexpr uint64_t SporeTrail_m_flSpawnRadius = 0xec58d7c0b8086076;
	constexpr uint64_t SporeTrail_m_flSpawnRate = 0xd2dc6af7689a62bc;
	constexpr uint64_t SporeTrail_m_flStartSize = 0xe37e4ee12a358763;
	constexpr uint64_t SporeTrail_m_vecEndColor = 0x4482e3d5b1226d76;
	constexpr uint64_t string_CCSPlayer_m_bWaitForNoAttack = 0xda8116162cded828;
	constexpr uint64_t string_CCSPlayer_m_iMoveState = 0x180ea4418b56add3;
	constexpr uint64_t string_CCSPlayer_m_bStrafing = 0xee1abc1f24181c24;
	constexpr uint64_t string_CCSPlayer_m_flVelocityModifier = 0x2d42d42c3dd915d2;
	constexpr uint64_t string_CCSPlayer_m_flThirdpersonRecoil = 0xc9cad95330e73ec8;
	constexpr uint64_t string_CBaseCSGrenade_m_fThrowTime = 0x3edefe45037cc076;
	constexpr uint64_t string_CBaseCSGrenade_m_bIsHeldByPlayer = 0xcdf1ebdb459e659e;

	enum class as_enum
	{
		INVALID,
		interface_client,
		interface_entity_list,
		interface_engine,
		interface_prediction,
		interface_move_helper,
		interface_game_movement,
		interface_globals,
		interface_model_info,
		interface_model_render,
		interface_material_system,
		interface_render_view,
		interface_cvar,
		interface_view_render,
		interface_view_render_beams,
		interface_game_event_manager,
		interface_input,
		interface_surface_props,
		interface_engine_trace,
		interface_client_state,
		interface_surface,
		interface_memalloc,
		interface_debug_overlay,
		interface_glow_object_manager,
		interface_panel,
		interface_engine_sound,
		interface_game_rules_proxy,
		interface_weapon_system,
		interface_inventory_manager,
		interface_localize,
		interface_hud,
		interface_player_resource,
		interface_client_leaf_system,
		interface_effects,
		interface_host_state,
		interface_mdl_cache,
		interface_studio_render,
		interface_hardware_config,
		interface_client_tools,
		interface_string_tables,
		interface_file_system,
		interface_key_values_system,
		interface_panoroama_ui_engine,
		sig_set_abs_angles,
		sig_set_abs_origin,
		sig_update_anim_state,
		sig_ret_to_cam_think,
		sig_ret_to_draw_notify,
		sig_ret_to_console_print,
		sig_ret_to_add_notify,
		sig_ret_to_draw_crosshair,
		sig_standard_blending_rules,
		sig_build_transformations,
		sig_smoke_count,
		sig_get_econ_item_view,
		sig_get_sequence_act,
		sig_invalidate_physics_recursive,
		sig_is_breakable_entity,
		sig_print_dev_console,
		sig_find_element,
		sig_physics_run_think,
		sig_cmbat_get_econ_item_view,
		sig_get_static_data,
		sig_disable_post_processing,
		sig_gc_client_system,
		sig_lookup_bone,
		sig_modifier_table,
		sig_load_named_sky,
		sig_reveal_ranks,
		sig_set_clantag,
		sig_deathnotice_check,
		sig_clear_deathnotices,
		sig_get_checksum,
		sig_equip_glove,
		sig_item_system,
		sig_parse_kits_addr,
		sig_util_traceline,
		sig_cl_send_move_limit,
		sig_is_ready,
		sig_ret_to_setup_velocity,
		sig_ret_to_post_data_update,
		sig_ret_to_enable_inventory,
		sig_interp_clamp,
		sig_ret_to_trace_ray,
		sig_ret_to_scope_arc,
		sig_ret_to_scope_lens,
		sig_post_process_params,
		sig_ret_to_post_network_data_received,
		sig_ret_to_add_viewmodel_stattrak,
		sig_trace_to_studio_csgo_hitgroups_priority,
		sig_should_skip_anim_frame_check,
		sig_somelimit,
		sig_cvar_check,
		sig_cs_player_pred_map,
		sig_base_cs_grenade_pred_map,
		sig_decoy_map,
		sig_he_map,
		sig_molotov_map,
		sig_sensor_map,
		sig_smoke_map,
		sig_flash_map,
		sig_view_matrix,
		sig_get_item_schema,
		sig_econ_item_destructor,
		sig_create_econ_item,
		sig_animstate_reset,
		sig_update_layer_order_preset,
		sig_update_equipped_state,
		sig_set_custom_name,
		sig_set_custom_desc,
		sig_set_dynamic_attribute_value,
		sig_empty_mapping,
		sig_simulation_tick,
		sig_predictable,
		sig_custom_materials,
		sig_custom_material_initialized,
		sig_visual_data_processors,
		sig_spawn_time,
		sig_ragdoll,
		sig_lod_data,
		sig_ik_context,
		sig_snapshot_front,
		sig_snapshot_back,
		sig_blend_tick,
		sig_get_follow_entity,
		sig_last_client_fire_bullet_time,
		sig_final_predicted_tick,
		sig_occlusion_mask,
		sig_anim_state,
		sig_command_context,
		sig_send_matching_sequence,
		sig_sound_channels,
		sig_activity_mapping,
		sig_load_from_buffer,
		sig_modifier_table_find,
		sig_modifier_table_add_string,
		sig_allocate_thread_id,
		sig_game_tonemap_settings,
		sig_length_disasm_game,
		sig_get_active_weapon_prefix,
		sig_get_layer_ideal_weight_from_sequence_cycle,
		sig_on_simulation_time_changing,
		sig_get_root_move_parent,
		sig_ik_context_construct,
		sig_ik_context_destruct,
		sig_ik_context_init,
		sig_ik_context_update_targets,
		sig_ik_context_solve_dependencies,
		sig_allocate_and_merge_bones,
		sig_lookup_sequence,
		sig_ammodef_get,
		sig_get_any_sequence_anim_tag,
		sig_accumulate_interleaved_dispatch_layers,
		sig_process_sockets,
		sig_sequence_hash_find_hash,
		sig_activity_mapping_reinitialize,
		sig_find_mapping,
		sig_index_model_sequences,
		sig_get_seq_desc,
		sig_keyvalues_init,
		sig_ret_is_box_visible,
		sig_random_int,
		sig_random_float,
		sig_random_seed,
		sig_var_mapping,
		sig_free_thread_id,
		sig_tls_slots,
		sig_projectile_throwtime,
		sig_init_materials,
		sig_render_box,
		sig_render_triangle,
		sig_vertex_color_ignorez,
		sig_cycle_offset,
		sig_studio_hdr,
		sig_anim_layers,
		sig_is_ragdoll,
		sig_bone_accessor,
		sig_cl_move_rest,
		sig_post_build_transformations,
		sig_ret_to_post_build_transformations,
		sig_collision_viewheight,
		sig_collision_bounds_change_time,
		sig_velocity_patch,
		sig_kv_find_key,
		sig_kv_set_string,
		sig_surface_props,
		sig_animstate_update_setabsagles,
		sig_mdl_create,
		sig_set_merged_mdl,
		sig_setup_bones_attachment_queries,
		sig_disable_render_target_allocation,
		sig_get_sequence_flags,
		sig_get_iron_sight_controller,
		sig_enable_invalidate_bone_cache,
		sig_line_goes_through_smoke,
		sig_keyvalues_from_netsmg,
		sig_model_bone_counter,
		sig_most_recent_model_bone_counter,
		sig_accumulated_bone_mask,
		sig_ret_hitbox_to_world_transforms,
		sig_emit_sound,
		sig_add_to_dirty_kd_tree,
		sig_get_player_viewmodel_arm_config_for_player_model,
		sig_client_side_addons_mask,
		sig_assassination_target_addon,
		sig_update_addon_models,
		sig_msg_voicedata_constructor,
		sig_parse_svg,
		sig_construct_image,
		sig_load_text_file,
		sig_econ_item_system,
		sig_clear_hud_weapon_icon,
		sig_skinchanger_spam_patch,
		sig_cvar_vac_check,
		sig_v8_try_catch_ctor,
		sig_v8_try_catch_dtor,
		sig_v8_handle_scope_ctor,
		sig_v8_handle_scope_dtor,
		sig_v8_handle_scope_create_handle,
		sig_v8_context_enter,
		sig_v8_context_exit,
		sig_v8_isolate_enter,
		sig_v8_isolate_exit,
		sig_v8_array_get,
		sig_v8_array_length,
		sig_v8_object_get,
		sig_v8_object_get_property_names,
		sig_v8_value_is_boolean,
		sig_v8_value_is_boolean_object,
		sig_v8_value_is_number,
		sig_v8_value_is_number_object,
		sig_v8_value_is_string,
		sig_v8_value_is_string_object,
		sig_v8_value_is_object,
		sig_v8_value_is_array,
		sig_v8_value_is_function,
		sig_v8_value_boolean_value,
		sig_v8_value_number_value,
		sig_v8_value_to_object,
		sig_v8_string_utf8_ctor,
		sig_v8_string_utf8_dtor,
		sig_ui_engine_get_panel_context,
		sig_ui_engine_compile,
		sig_ui_engine_run_compiled_script,
		hook_start_sound_immediate,
		hook_set_bodygroup,
		hook_modify_eye_pos,
		hook_wnd_proc,
		hook_create_move,
		hook_cl_move,
		hook_frame_stage_notify,
		hook_on_latch_interp_var,
		hook_draw_model_execute,
		hook_render_view,
		hook_level_init_post_entity,
		hook_level_init_pre_entity,
		hook_get_tonemap_settings_from_env_tonemap_controller,
		hook_draw_3d_debug_overlays,
		hook_shader_api_draw_mesh,
		hook_is_box_visible,
		hook_scene_end,
		hook_calc_renderable_world_space_aabb_bloated,
		hook_add_renderables_to_render_lists,
		hook_do_post_screen_space_effects,
		hook_present,
		hook_reset,
		hook_send_net_msg,
		hook_override_view,
		hook_paint_traverse,
		hook_find_material,
		hook_emit_sound,
		hook_is_connected,
		hook_lock_cursor,
		hook_draw_set_color,
		hook_test_hitboxes,
		hook_setup_bones,
		hook_animating_setup_bones,
		hook_post_data_update,
		hook_maintain_sequence_transitions,
		hook_trace_ray,
		hook_run_command,
		hook_process_movement,
		hook_transfer_data,
		hook_perform_prediction,
		hook_packet_start,
		hook_level_shutdown,
		hook_physics_simulate,
		hook_estimate_abs_velocity,
		hook_interpolate,
		hook_reset_latched,
		hook_send_weapon_anim,
		hook_fire_event,
		hook_do_animation_events,
		hook_deploy,
		hook_get_fov,
		hook_effects,
		hook_sequence,
		hook_weapon_handle,
		hook_simulation_time,
		hook_general_float,
		hook_general_vec,
		hook_general_int,
		hook_general_bool,
		hook_layer_sequence,
		hook_layer_cycle,
		hook_layer_playback_rate,
		hook_layer_weight,
		hook_layer_weight_delta_rate,
		hook_layer_order,
		hook_convar_get_float_engine,
		hook_convar_get_int_engine,
		hook_convar_get_int_client,
		hook_eye_position_and_vectors,
		hook_obb_change_callback,
		hook_server_cmd_key_values,
		hook_calc_view,
		hook_scale_mouse,
		hook_convar_network_change_callback,
		hook_reevauluate_anim_lod,
		hook_start_sound,
		hook_update_clientside_anim,
		hook_do_animation_events_animating,
		hook_get_default_fov,
		hook_ent_info_list_link_before,
		hook_extract_occluded_renderables,
		hook_sv_msg_voicedata,
		hook_hud_reticle_process_input,
		hook_update_interp_vars,
		hook_particle_draw_model,
		CAI_BaseNPC_m_bFadeCorpse,
		CAI_BaseNPC_m_bImportanRagdoll,
		CAI_BaseNPC_m_bIsMoving,
		CAI_BaseNPC_m_bPerformAvoidance,
		CAI_BaseNPC_m_bSpeedModActive,
		CAI_BaseNPC_m_flTimePingEffect,
		CAI_BaseNPC_m_iDeathFrame,
		CAI_BaseNPC_m_iDeathPose,
		CAI_BaseNPC_m_iSpeedModRadius,
		CAI_BaseNPC_m_iSpeedModSpeed,
		CAI_BaseNPC_m_lifeState,
		CBRC4Target_m_bBrokenOpen,
		CBRC4Target_m_flRadius,
		CBaseAnimating_m_ScaleType,
		CBaseAnimating_m_bClientSideAnimation,
		CBaseAnimating_m_bClientSideFrameReset,
		CBaseAnimating_m_bClientSideRagdoll,
		CBaseAnimating_m_bSuppressAnimSounds,
		CBaseAnimating_m_flCycle,
		CBaseAnimating_m_flEncodedController,
		CBaseAnimating_m_flFrozen,
		CBaseAnimating_m_flModelScale,
		CBaseAnimating_m_flPlaybackRate,
		CBaseAnimating_m_flPoseParameter,
		CBaseAnimating_m_hLightingOrigin,
		CBaseAnimating_m_nBody,
		CBaseAnimating_m_nForceBone,
		CBaseAnimating_m_nHighlightColorB,
		CBaseAnimating_m_nHighlightColorG,
		CBaseAnimating_m_nHighlightColorR,
		CBaseAnimating_m_nHitboxSet,
		CBaseAnimating_m_nMuzzleFlashParity,
		CBaseAnimating_m_nNewSequenceParity,
		CBaseAnimating_m_nResetEventsParity,
		CBaseAnimating_m_nSequence,
		CBaseAnimating_m_nSkin,
		CBaseAnimating_m_vecForce,
		CBaseAttributableItem_m_AttributeManager,
		CBaseAttributableItem_m_Attributes,
		CBaseAttributableItem_m_Item,
		CBaseAttributableItem_m_NetworkedDynamicAttributesForDemos,
		CBaseAttributableItem_m_OriginalOwnerXuidHigh,
		CBaseAttributableItem_m_OriginalOwnerXuidLow,
		CBaseAttributableItem_m_ProviderType,
		CBaseAttributableItem_m_bInitialized,
		CBaseAttributableItem_m_flFallbackWear,
		CBaseAttributableItem_m_hOuter,
		CBaseAttributableItem_m_iAccountID,
		CBaseAttributableItem_m_iEntityLevel,
		CBaseAttributableItem_m_iEntityQuality,
		CBaseAttributableItem_m_iItemDefinitionIndex,
		CBaseAttributableItem_m_iItemIDHigh,
		CBaseAttributableItem_m_iItemIDLow,
		CBaseAttributableItem_m_iReapplyProvisionParity,
		CBaseAttributableItem_m_nFallbackPaintKit,
		CBaseAttributableItem_m_nFallbackSeed,
		CBaseAttributableItem_m_nFallbackStatTrak,
		CBaseAttributableItem_m_szCustomName,
		CBaseButton_m_usable,
		CBaseCSGrenadeProjectile_m_nBounces,
		CBaseCSGrenadeProjectile_m_nExplodeEffectIndex,
		CBaseCSGrenadeProjectile_m_nExplodeEffectTickBegin,
		CBaseCSGrenadeProjectile_m_vInitialVelocity,
		CBaseCSGrenadeProjectile_m_vecExplodeEffectOrigin,
		CBaseCSGrenade_m_bIsHeldByPlayer,
		CBaseCSGrenade_m_bLoopingSoundPlaying,
		CBaseCSGrenade_m_bPinPulled,
		CBaseCSGrenade_m_bRedraw,
		CBaseCSGrenade_m_fDropTime,
		CBaseCSGrenade_m_fThrowTime,
		CBaseCSGrenade_m_flThrowStrength,
		CBaseCombatCharacter_m_LastHitGroup,
		CBaseCombatCharacter_m_flNextAttack,
		CBaseCombatCharacter_m_flTimeOfLastInjury,
		CBaseCombatCharacter_m_hActiveWeapon,
		CBaseCombatCharacter_m_hMyWeapons,
		CBaseCombatCharacter_m_hMyWearables,
		CBaseCombatCharacter_m_nRelativeDirectionOfLastInjury,
		CBaseCombatWeapon_m_bFlipViewModel,
		CBaseCombatWeapon_m_flNextPrimaryAttack,
		CBaseCombatWeapon_m_flNextSecondaryAttack,
		CBaseCombatWeapon_m_flTimeWeaponIdle,
		CBaseCombatWeapon_m_hOwner,
		CBaseCombatWeapon_m_hWeaponWorldModel,
		CBaseCombatWeapon_m_iClip1,
		CBaseCombatWeapon_m_iClip2,
		CBaseCombatWeapon_m_iNumEmptyAttacks,
		CBaseCombatWeapon_m_iPrimaryAmmoType,
		CBaseCombatWeapon_m_iPrimaryReserveAmmoCount,
		CBaseCombatWeapon_m_iSecondaryAmmoType,
		CBaseCombatWeapon_m_iSecondaryReserveAmmoCount,
		CBaseCombatWeapon_m_iState,
		CBaseCombatWeapon_m_iViewModelIndex,
		CBaseCombatWeapon_m_iWeaponModule,
		CBaseCombatWeapon_m_iWeaponOrigin,
		CBaseCombatWeapon_m_iWorldDroppedModelIndex,
		CBaseCombatWeapon_m_iWorldModelIndex,
		CBaseCombatWeapon_m_nNextThinkTick,
		CBaseCombatWeapon_m_nViewModelIndex,
		CBaseDoor_m_flWaveHeight,
		CBaseEntity_m_Collision,
		CBaseEntity_m_CollisionGroup,
		CBaseEntity_m_angRotation,
		CBaseEntity_m_bAlternateSorting,
		CBaseEntity_m_bAnimatedEveryTick,
		CBaseEntity_m_bEligibleForScreenHighlight,
		CBaseEntity_m_bIsAutoaimTarget,
		CBaseEntity_m_bSimulatedEveryTick,
		CBaseEntity_m_bSpotted,
		CBaseEntity_m_bSpottedBy,
		CBaseEntity_m_bSpottedByMask,
		CBaseEntity_m_cellX,
		CBaseEntity_m_cellY,
		CBaseEntity_m_cellZ,
		CBaseEntity_m_cellbits,
		CBaseEntity_m_clrRender,
		CBaseEntity_m_fEffects,
		CBaseEntity_m_fadeMaxDist,
		CBaseEntity_m_fadeMinDist,
		CBaseEntity_m_flAnimTime,
		CBaseEntity_m_flElasticity,
		CBaseEntity_m_flFadeScale,
		CBaseEntity_m_flLastMadeNoiseTime,
		CBaseEntity_m_flMaxFallVelocity,
		CBaseEntity_m_flShadowCastDistance,
		CBaseEntity_m_flSimulationTime,
		CBaseEntity_m_flUseLookAtAngle,
		CBaseEntity_m_hEffectEntity,
		CBaseEntity_m_hOwnerEntity,
		CBaseEntity_m_iName,
		CBaseEntity_m_iParentAttachment,
		CBaseEntity_m_iPendingTeamNum,
		CBaseEntity_m_iTeamNum,
		CBaseEntity_m_iTextureFrameIndex,
		CBaseEntity_m_nMaxCPULevel,
		CBaseEntity_m_nMaxGPULevel,
		CBaseEntity_m_nMinCPULevel,
		CBaseEntity_m_nMinGPULevel,
		CBaseEntity_m_nModelIndex,
		CBaseEntity_m_nRenderFX,
		CBaseEntity_m_nRenderMode,
		CBaseEntity_m_nSolidType,
		CBaseEntity_m_nSurroundType,
		CBaseEntity_m_triggerBloat,
		CBaseEntity_m_usSolidFlags,
		CBaseEntity_m_vecMaxs,
		CBaseEntity_m_vecMins,
		CBaseEntity_m_vecOrigin,
		CBaseEntity_m_vecSpecifiedSurroundingMaxs,
		CBaseEntity_m_vecSpecifiedSurroundingMins,
		CBaseEntity_moveparent,
		CBaseFlex_m_blinktoggle,
		CBaseFlex_m_flexWeight,
		CBaseFlex_m_viewtarget,
		CBaseGrenade_m_DmgRadius,
		CBaseGrenade_m_bIsLive,
		CBaseGrenade_m_fFlags,
		CBaseGrenade_m_flDamage,
		CBaseGrenade_m_hThrower,
		CBaseGrenade_m_vecVelocity,
		CBasePlayer_deadflag,
		CBasePlayer_m_Local,
		CBasePlayer_m_PlayerFog_m_hCtrl,
		CBasePlayer_m_afPhysicsFlags,
		CBasePlayer_m_aimPunchAngle,
		CBasePlayer_m_aimPunchAngleVel,
		CBasePlayer_m_audio_entIndex,
		CBasePlayer_m_audio_localBits,
		CBasePlayer_m_audio_localSound_0_,
		CBasePlayer_m_audio_localSound_1_,
		CBasePlayer_m_audio_localSound_2_,
		CBasePlayer_m_audio_localSound_3_,
		CBasePlayer_m_audio_localSound_4_,
		CBasePlayer_m_audio_localSound_5_,
		CBasePlayer_m_audio_localSound_6_,
		CBasePlayer_m_audio_localSound_7_,
		CBasePlayer_m_audio_soundscapeIndex,
		CBasePlayer_m_bActiveCameraMan,
		CBasePlayer_m_bAllowAutoMovement,
		CBasePlayer_m_bCameraManOverview,
		CBasePlayer_m_bCameraManScoreBoard,
		CBasePlayer_m_bCameraManXRay,
		CBasePlayer_m_bConstraintPastRadius,
		CBasePlayer_m_bDrawViewmodel,
		CBasePlayer_m_bDucked,
		CBasePlayer_m_bDucking,
		CBasePlayer_m_bInDuckJump,
		CBasePlayer_m_bPoisoned,
		CBasePlayer_m_bShouldDrawPlayerWhileUsingViewEntity,
		CBasePlayer_m_bWearingSuit,
		CBasePlayer_m_chAreaBits,
		CBasePlayer_m_chAreaPortalBits,
		CBasePlayer_m_fFlags,
		CBasePlayer_m_fForceTeam,
		CBasePlayer_m_fOnTarget,
		CBasePlayer_m_flConstraintRadius,
		CBasePlayer_m_flConstraintSpeedFactor,
		CBasePlayer_m_flConstraintWidth,
		CBasePlayer_m_flDeathTime,
		CBasePlayer_m_flDuckAmount,
		CBasePlayer_m_flDuckSpeed,
		CBasePlayer_m_flFOVRate,
		CBasePlayer_m_flFOVTime,
		CBasePlayer_m_flFallVelocity,
		CBasePlayer_m_flFriction,
		CBasePlayer_m_flLaggedMovementValue,
		CBasePlayer_m_flLastDuckTime,
		CBasePlayer_m_flMaxspeed,
		CBasePlayer_m_flNextDecalTime,
		CBasePlayer_m_flStepSize,
		CBasePlayer_m_hColorCorrectionCtrl,
		CBasePlayer_m_hConstraintEntity,
		CBasePlayer_m_hGroundEntity,
		CBasePlayer_m_hLastWeapon,
		CBasePlayer_m_hObserverTarget,
		CBasePlayer_m_hPostProcessCtrl,
		CBasePlayer_m_hTonemapController,
		CBasePlayer_m_hUseEntity,
		CBasePlayer_m_hVehicle,
		CBasePlayer_m_hViewEntity,
		CBasePlayer_m_hViewModel_0_,
		CBasePlayer_m_hZoomOwner,
		CBasePlayer_m_iAmmo,
		CBasePlayer_m_iBonusChallenge,
		CBasePlayer_m_iBonusProgress,
		CBasePlayer_m_iCoachingTeam,
		CBasePlayer_m_iDeathPostEffect,
		CBasePlayer_m_iDefaultFOV,
		CBasePlayer_m_iFOV,
		CBasePlayer_m_iFOVStart,
		CBasePlayer_m_iHealth,
		CBasePlayer_m_iHideHUD,
		CBasePlayer_m_iObserverMode,
		CBasePlayer_m_ladderSurfaceProps,
		CBasePlayer_m_lifeState,
		CBasePlayer_m_nDuckJumpTimeMsecs,
		CBasePlayer_m_nDuckTimeMsecs,
		CBasePlayer_m_nJumpTimeMsecs,
		CBasePlayer_m_nNextThinkTick,
		CBasePlayer_m_nOldButtons,
		CBasePlayer_m_nTickBase,
		CBasePlayer_m_nWaterLevel,
		CBasePlayer_m_skybox3d_area,
		CBasePlayer_m_skybox3d_fog_HDRColorScale,
		CBasePlayer_m_skybox3d_fog_blend,
		CBasePlayer_m_skybox3d_fog_colorPrimary,
		CBasePlayer_m_skybox3d_fog_colorSecondary,
		CBasePlayer_m_skybox3d_fog_dirPrimary,
		CBasePlayer_m_skybox3d_fog_enable,
		CBasePlayer_m_skybox3d_fog_end,
		CBasePlayer_m_skybox3d_fog_maxdensity,
		CBasePlayer_m_skybox3d_fog_start,
		CBasePlayer_m_skybox3d_origin,
		CBasePlayer_m_skybox3d_scale,
		CBasePlayer_m_szLastPlaceName,
		CBasePlayer_m_uCameraManGraphs,
		CBasePlayer_m_ubEFNoInterpParity,
		CBasePlayer_m_vecBaseVelocity,
		CBasePlayer_m_vecConstraintCenter,
		CBasePlayer_m_vecLadderNormal,
		CBasePlayer_m_vecVelocity_0_,
		CBasePlayer_m_vecVelocity_1_,
		CBasePlayer_m_vecVelocity_2_,
		CBasePlayer_m_vecViewOffset_0_,
		CBasePlayer_m_vecViewOffset_1_,
		CBasePlayer_m_vecViewOffset_2_,
		CBasePlayer_m_viewPunchAngle,
		CBasePlayer_m_vphysicsCollisionState,
		CBasePlayer_pl,
		CBaseTeamObjectiveResource_m_bBlocked,
		CBaseTeamObjectiveResource_m_bCPIsVisible,
		CBaseTeamObjectiveResource_m_bControlPointsReset,
		CBaseTeamObjectiveResource_m_bInMiniRound,
		CBaseTeamObjectiveResource_m_bPlayingMiniRounds,
		CBaseTeamObjectiveResource_m_bTeamCanCap,
		CBaseTeamObjectiveResource_m_flLazyCapPerc,
		CBaseTeamObjectiveResource_m_flPathDistance,
		CBaseTeamObjectiveResource_m_flTeamCapTime,
		CBaseTeamObjectiveResource_m_iBaseControlPoints,
		CBaseTeamObjectiveResource_m_iCappingTeam,
		CBaseTeamObjectiveResource_m_iNumControlPoints,
		CBaseTeamObjectiveResource_m_iNumTeamMembers,
		CBaseTeamObjectiveResource_m_iOwner,
		CBaseTeamObjectiveResource_m_iPreviousPoints,
		CBaseTeamObjectiveResource_m_iStopWatchTimer,
		CBaseTeamObjectiveResource_m_iTeamBaseIcons,
		CBaseTeamObjectiveResource_m_iTeamIcons,
		CBaseTeamObjectiveResource_m_iTeamInZone,
		CBaseTeamObjectiveResource_m_iTeamOverlays,
		CBaseTeamObjectiveResource_m_iTeamReqCappers,
		CBaseTeamObjectiveResource_m_iTimerToShowInHUD,
		CBaseTeamObjectiveResource_m_iUpdateCapHudParity,
		CBaseTeamObjectiveResource_m_iWarnOnCap,
		CBaseTeamObjectiveResource_m_iszWarnSound_0_,
		CBaseTeamObjectiveResource_m_pszCapLayoutInHUD,
		CBaseTeamObjectiveResource_m_vCPPositions_0_,
		CBaseToggle_m_flMoveTargetTime,
		CBaseToggle_m_movementType,
		CBaseToggle_m_vecFinalDest,
		CBaseTrigger_m_bClientSidePredicted,
		CBaseTrigger_m_spawnflags,
		CBaseViewModel_m_bShouldIgnoreOffsetAndAccuracy,
		CBaseViewModel_m_fEffects,
		CBaseViewModel_m_flPlaybackRate,
		CBaseViewModel_m_hOwner,
		CBaseViewModel_m_hWeapon,
		CBaseViewModel_m_nAnimationParity,
		CBaseViewModel_m_nBody,
		CBaseViewModel_m_nModelIndex,
		CBaseViewModel_m_nMuzzleFlashParity,
		CBaseViewModel_m_nNewSequenceParity,
		CBaseViewModel_m_nResetEventsParity,
		CBaseViewModel_m_nSequence,
		CBaseViewModel_m_nSkin,
		CBaseViewModel_m_nViewModelIndex,
		CBaseWeaponWorldModel_m_fEffects,
		CBaseWeaponWorldModel_m_hCombatWeaponParent,
		CBaseWeaponWorldModel_m_nBody,
		CBaseWeaponWorldModel_m_nModelIndex,
		CBaseWeaponWorldModel_moveparent,
		CBeamSpotlight_m_bHasDynamicLight,
		CBeamSpotlight_m_bSpotlightOn,
		CBeamSpotlight_m_flHDRColorScale,
		CBeamSpotlight_m_flRotationSpeed,
		CBeamSpotlight_m_flSpotlightGoalWidth,
		CBeamSpotlight_m_flSpotlightMaxLength,
		CBeamSpotlight_m_nHaloIndex,
		CBeamSpotlight_m_nRotationAxis,
		CBeam_m_clrRender,
		CBeam_m_fAmplitude,
		CBeam_m_fEndWidth,
		CBeam_m_fFadeLength,
		CBeam_m_fHaloScale,
		CBeam_m_fSpeed,
		CBeam_m_fStartFrame,
		CBeam_m_fWidth,
		CBeam_m_flFrame,
		CBeam_m_flFrameRate,
		CBeam_m_flHDRColorScale,
		CBeam_m_hAttachEntity,
		CBeam_m_nAttachIndex,
		CBeam_m_nBeamFlags,
		CBeam_m_nBeamType,
		CBeam_m_nClipStyle,
		CBeam_m_nHaloIndex,
		CBeam_m_nModelIndex,
		CBeam_m_nNumBeamEnts,
		CBeam_m_nRenderFX,
		CBeam_m_nRenderMode,
		CBeam_m_vecEndPos,
		CBeam_m_vecOrigin,
		CBeam_moveparent,
		CBoneFollower_m_modelIndex,
		CBoneFollower_m_solidIndex,
		CBreachChargeProjectile_m_bShouldExplode,
		CBreachChargeProjectile_m_nParentBoneIndex,
		CBreachChargeProjectile_m_vecParentBonePos,
		CBreachChargeProjectile_m_weaponThatThrewMe,
		CBreakableProp_m_bClientPhysics,
		CBreakableProp_m_qPreferredPlayerCarryAngles,
		CBreakableSurface_m_RawPanelBitVec,
		CBreakableSurface_m_bIsBroken,
		CBreakableSurface_m_flPanelHeight,
		CBreakableSurface_m_flPanelWidth,
		CBreakableSurface_m_nNumHigh,
		CBreakableSurface_m_nNumWide,
		CBreakableSurface_m_nSurfaceType,
		CBreakableSurface_m_vCorner,
		CBreakableSurface_m_vNormal,
		CBumpMineProjectile_m_bArmed,
		CBumpMineProjectile_m_nParentBoneIndex,
		CBumpMineProjectile_m_vecParentBonePos,
		CC4_m_bBombPlacedAnimation,
		CC4_m_bIsPlantingViaUse,
		CC4_m_bShowC4LED,
		CC4_m_bStartedArming,
		CC4_m_fArmedTime,
		CCSGameRulesProxy_m_GGProgressiveWeaponKillUpgradeOrderCT,
		CCSGameRulesProxy_m_GGProgressiveWeaponKillUpgradeOrderT,
		CCSGameRulesProxy_m_GGProgressiveWeaponOrderCT,
		CCSGameRulesProxy_m_GGProgressiveWeaponOrderT,
		CCSGameRulesProxy_m_MatchDevice,
		CCSGameRulesProxy_m_RetakeRules,
		CCSGameRulesProxy_m_SpawnTileState,
		CCSGameRulesProxy_m_SurvivalGameRuleDecisionTypes,
		CCSGameRulesProxy_m_SurvivalGameRuleDecisionValues,
		CCSGameRulesProxy_m_SurvivalRules,
		CCSGameRulesProxy_m_TeamRespawnWaveTimes,
		CCSGameRulesProxy_m_arrFeaturedGiftersAccounts,
		CCSGameRulesProxy_m_arrFeaturedGiftersGifts,
		CCSGameRulesProxy_m_arrProhibitedItemIndices,
		CCSGameRulesProxy_m_arrTournamentActiveCasterAccounts,
		CCSGameRulesProxy_m_bAnyHostageReached,
		CCSGameRulesProxy_m_bBlockersPresent,
		CCSGameRulesProxy_m_bBombDropped,
		CCSGameRulesProxy_m_bBombPlanted,
		CCSGameRulesProxy_m_bCTCantBuy,
		CCSGameRulesProxy_m_bCTTimeOutActive,
		CCSGameRulesProxy_m_bDMBonusActive,
		CCSGameRulesProxy_m_bFreezePeriod,
		CCSGameRulesProxy_m_bGameRestart,
		CCSGameRulesProxy_m_bHasMatchStarted,
		CCSGameRulesProxy_m_bIsDroppingItems,
		CCSGameRulesProxy_m_bIsQuestEligible,
		CCSGameRulesProxy_m_bIsQueuedMatchmaking,
		CCSGameRulesProxy_m_bIsValveDS,
		CCSGameRulesProxy_m_bLogoMap,
		CCSGameRulesProxy_m_bMapHasBombTarget,
		CCSGameRulesProxy_m_bMapHasBuyZone,
		CCSGameRulesProxy_m_bMapHasRescueZone,
		CCSGameRulesProxy_m_bMatchWaitingForResume,
		CCSGameRulesProxy_m_bPlayAllStepSoundsOnServer,
		CCSGameRulesProxy_m_bRoundInProgress,
		CCSGameRulesProxy_m_bTCantBuy,
		CCSGameRulesProxy_m_bTechnicalTimeOut,
		CCSGameRulesProxy_m_bTerroristTimeOutActive,
		CCSGameRulesProxy_m_bWarmupPeriod,
		CCSGameRulesProxy_m_eRoundWinReason,
		CCSGameRulesProxy_m_fMatchStartTime,
		CCSGameRulesProxy_m_fRoundStartTime,
		CCSGameRulesProxy_m_fWarmupPeriodEnd,
		CCSGameRulesProxy_m_fWarmupPeriodStart,
		CCSGameRulesProxy_m_flCMMItemDropRevealEndTime,
		CCSGameRulesProxy_m_flCMMItemDropRevealStartTime,
		CCSGameRulesProxy_m_flCTTimeOutRemaining,
		CCSGameRulesProxy_m_flDMBonusStartTime,
		CCSGameRulesProxy_m_flDMBonusTimeLength,
		CCSGameRulesProxy_m_flGameStartTime,
		CCSGameRulesProxy_m_flGuardianBuyUntilTime,
		CCSGameRulesProxy_m_flNextRespawnWave,
		CCSGameRulesProxy_m_flRestartRoundTime,
		CCSGameRulesProxy_m_flSpawnSelectionTimeEnd,
		CCSGameRulesProxy_m_flSpawnSelectionTimeLoadout,
		CCSGameRulesProxy_m_flSpawnSelectionTimeStart,
		CCSGameRulesProxy_m_flSurvivalStartTime,
		CCSGameRulesProxy_m_flTabletHexOriginX,
		CCSGameRulesProxy_m_flTabletHexOriginY,
		CCSGameRulesProxy_m_flTabletHexSize,
		CCSGameRulesProxy_m_flTerroristTimeOutRemaining,
		CCSGameRulesProxy_m_gamePhase,
		CCSGameRulesProxy_m_iActiveAssassinationTargetMissionID,
		CCSGameRulesProxy_m_iBombSite,
		CCSGameRulesProxy_m_iFirstSecondHalfRound,
		CCSGameRulesProxy_m_iHostagesRemaining,
		CCSGameRulesProxy_m_iMatchStats_PlayersAlive_CT,
		CCSGameRulesProxy_m_iMatchStats_PlayersAlive_T,
		CCSGameRulesProxy_m_iMatchStats_RoundResults,
		CCSGameRulesProxy_m_iNumConsecutiveCTLoses,
		CCSGameRulesProxy_m_iNumConsecutiveTerroristLoses,
		CCSGameRulesProxy_m_iNumGunGameProgressiveWeaponsCT,
		CCSGameRulesProxy_m_iNumGunGameProgressiveWeaponsT,
		CCSGameRulesProxy_m_iPlayerSpawnHexIndices,
		CCSGameRulesProxy_m_iRoundTime,
		CCSGameRulesProxy_m_iRoundWinStatus,
		CCSGameRulesProxy_m_iSpectatorSlotCount,
		CCSGameRulesProxy_m_nCTTimeOuts,
		CCSGameRulesProxy_m_nEndMatchMapGroupVoteOptions,
		CCSGameRulesProxy_m_nEndMatchMapGroupVoteTypes,
		CCSGameRulesProxy_m_nEndMatchMapVoteWinner,
		CCSGameRulesProxy_m_nGuardianModeSpecialKillsRemaining,
		CCSGameRulesProxy_m_nGuardianModeSpecialWeaponNeeded,
		CCSGameRulesProxy_m_nGuardianModeWaveNumber,
		CCSGameRulesProxy_m_nHalloweenMaskListSeed,
		CCSGameRulesProxy_m_nMatchSeed,
		CCSGameRulesProxy_m_nNextMapInMapgroup,
		CCSGameRulesProxy_m_nOvertimePlaying,
		CCSGameRulesProxy_m_nQueuedMatchmakingMode,
		CCSGameRulesProxy_m_nTerroristTimeOuts,
		CCSGameRulesProxy_m_nTournamentPredictionsPct,
		CCSGameRulesProxy_m_numBestOfMaps,
		CCSGameRulesProxy_m_numGlobalGifters,
		CCSGameRulesProxy_m_numGlobalGiftsGiven,
		CCSGameRulesProxy_m_numGlobalGiftsPeriodSeconds,
		CCSGameRulesProxy_m_roundData_playerPositions,
		CCSGameRulesProxy_m_roundData_playerTeams,
		CCSGameRulesProxy_m_roundData_playerXuids,
		CCSGameRulesProxy_m_spawnStage,
		CCSGameRulesProxy_m_szMatchStatTxt,
		CCSGameRulesProxy_m_szTournamentEventName,
		CCSGameRulesProxy_m_szTournamentEventStage,
		CCSGameRulesProxy_m_szTournamentPredictionsTxt,
		CCSGameRulesProxy_m_timeUntilNextPhaseStarts,
		CCSGameRulesProxy_m_totalRoundsPlayed,
		CCSGameRulesProxy_m_unDMBonusWeaponLoadoutSlot,
		CCSGameRulesProxy_m_vecPlayAreaMaxs,
		CCSGameRulesProxy_m_vecPlayAreaMins,
		CCSPlayerResource_m_bControllingBot,
		CCSPlayerResource_m_bEndMatchNextMapAllVoted,
		CCSPlayerResource_m_bHasCommunicationAbuseMute,
		CCSPlayerResource_m_bHasDefuser,
		CCSPlayerResource_m_bHasHelmet,
		CCSPlayerResource_m_bHostageAlive,
		CCSPlayerResource_m_bombsiteCenterA,
		CCSPlayerResource_m_bombsiteCenterB,
		CCSPlayerResource_m_hostageRescueX,
		CCSPlayerResource_m_hostageRescueY,
		CCSPlayerResource_m_hostageRescueZ,
		CCSPlayerResource_m_iArmor,
		CCSPlayerResource_m_iBotDifficulty,
		CCSPlayerResource_m_iCashSpentThisRound,
		CCSPlayerResource_m_iCompTeammateColor,
		CCSPlayerResource_m_iCompetitiveRankType,
		CCSPlayerResource_m_iCompetitiveRanking,
		CCSPlayerResource_m_iCompetitiveWins,
		CCSPlayerResource_m_iControlledByPlayer,
		CCSPlayerResource_m_iControlledPlayer,
		CCSPlayerResource_m_iGunGameLevel,
		CCSPlayerResource_m_iHostageEntityIDs,
		CCSPlayerResource_m_iLifetimeEnd,
		CCSPlayerResource_m_iLifetimeStart,
		CCSPlayerResource_m_iMVPs,
		CCSPlayerResource_m_iMatchStats_3k_Total,
		CCSPlayerResource_m_iMatchStats_4k_Total,
		CCSPlayerResource_m_iMatchStats_5k_Total,
		CCSPlayerResource_m_iMatchStats_Assists_Total,
		CCSPlayerResource_m_iMatchStats_CashEarned_Total,
		CCSPlayerResource_m_iMatchStats_Damage_Total,
		CCSPlayerResource_m_iMatchStats_Deaths_Total,
		CCSPlayerResource_m_iMatchStats_EnemiesFlashed_Total,
		CCSPlayerResource_m_iMatchStats_EquipmentValue_Total,
		CCSPlayerResource_m_iMatchStats_HeadShotKills_Total,
		CCSPlayerResource_m_iMatchStats_KillReward_Total,
		CCSPlayerResource_m_iMatchStats_Kills_Total,
		CCSPlayerResource_m_iMatchStats_LiveTime_Total,
		CCSPlayerResource_m_iMatchStats_Objective_Total,
		CCSPlayerResource_m_iMatchStats_UtilityDamage_Total,
		CCSPlayerResource_m_iPlayerC4,
		CCSPlayerResource_m_iPlayerVIP,
		CCSPlayerResource_m_iScore,
		CCSPlayerResource_m_iTotalCashSpent,
		CCSPlayerResource_m_isHostageFollowingSomeone,
		CCSPlayerResource_m_nActiveCoinRank,
		CCSPlayerResource_m_nCharacterDefIndex,
		CCSPlayerResource_m_nEndMatchNextMapVotes,
		CCSPlayerResource_m_nMusicID,
		CCSPlayerResource_m_nPersonaDataPublicCommendsFriendly,
		CCSPlayerResource_m_nPersonaDataPublicCommendsLeader,
		CCSPlayerResource_m_nPersonaDataPublicCommendsTeacher,
		CCSPlayerResource_m_nPersonaDataPublicLevel,
		CCSPlayerResource_m_szClan,
		CCSPlayerResource_m_szCrosshairCodes,
		CCSPlayer_m_ArmorValue,
		CCSPlayer_m_EquippedLoadoutItemDefIndices,
		CCSPlayer_m_RetakesMVPBoostExtraUtility,
		CCSPlayer_m_angEyeAngles,
		CCSPlayer_m_angEyeAngles_0_,
		CCSPlayer_m_angEyeAngles_1_,
		CCSPlayer_m_bCanControlObservedBot,
		CCSPlayer_m_bCanMoveDuringFreezePeriod,
		CCSPlayer_m_bDuckOverride,
		CCSPlayer_m_bGunGameImmunity,
		CCSPlayer_m_bHasControlledBotThisRound,
		CCSPlayer_m_bHasDefuser,
		CCSPlayer_m_bHasHeavyArmor,
		CCSPlayer_m_bHasHelmet,
		CCSPlayer_m_bHasMovedSinceSpawn,
		CCSPlayer_m_bHasNightVision,
		CCSPlayer_m_bHasParachute,
		CCSPlayer_m_bHideTargetID,
		CCSPlayer_m_bHud_MiniScoreHidden,
		CCSPlayer_m_bHud_RadarHidden,
		CCSPlayer_m_bInBombZone,
		CCSPlayer_m_bInBuyZone,
		CCSPlayer_m_bInHostageRescueZone,
		CCSPlayer_m_bInNoDefuseArea,
		CCSPlayer_m_bIsAssassinationTarget,
		CCSPlayer_m_bIsControllingBot,
		CCSPlayer_m_bIsDefusing,
		CCSPlayer_m_bIsGrabbingHostage,
		CCSPlayer_m_bIsHoldingLookAtWeapon,
		CCSPlayer_m_bIsLookingAtWeapon,
		CCSPlayer_m_bIsPlayerGhost,
		CCSPlayer_m_bIsRescuing,
		CCSPlayer_m_bIsRespawningForDMBonus,
		CCSPlayer_m_bIsScoped,
		CCSPlayer_m_bIsSpawnRappelling,
		CCSPlayer_m_bIsWalking,
		CCSPlayer_m_bKilledByTaser,
		CCSPlayer_m_bMadeFinalGunGameProgressiveKill,
		CCSPlayer_m_bNightVisionOn,
		CCSPlayer_m_bPlayerDominated,
		CCSPlayer_m_bPlayerDominatingMe,
		CCSPlayer_m_bResumeZoom,
		CCSPlayer_m_bRetakesHasDefuseKit,
		CCSPlayer_m_bRetakesMVPLastRound,
		CCSPlayer_m_bStrafing,
		CCSPlayer_m_bWaitForNoAttack,
		CCSPlayer_m_cycleLatch,
		CCSPlayer_m_fImmuneToGunGameDamageTime,
		CCSPlayer_m_fMolotovDamageTime,
		CCSPlayer_m_fMolotovUseTime,
		CCSPlayer_m_flAutoMoveStartTime,
		CCSPlayer_m_flAutoMoveTargetTime,
		CCSPlayer_m_flDetectedByEnemySensorTime,
		CCSPlayer_m_flFlashDuration,
		CCSPlayer_m_flFlashMaxAlpha,
		CCSPlayer_m_flGroundAccelLinearFracLastTime,
		CCSPlayer_m_flGuardianTooFarDistFrac,
		CCSPlayer_m_flHealthShotBoostExpirationTime,
		CCSPlayer_m_flLastExoJumpTime,
		CCSPlayer_m_flLowerBodyYawTarget,
		CCSPlayer_m_flProgressBarStartTime,
		CCSPlayer_m_flStamina,
		CCSPlayer_m_flThirdpersonRecoil,
		CCSPlayer_m_flVelocityModifier,
		CCSPlayer_m_hCarriedHostage,
		CCSPlayer_m_hCarriedHostageProp,
		CCSPlayer_m_hPlayerPing,
		CCSPlayer_m_hRagdoll,
		CCSPlayer_m_hSurvivalAssassinationTarget,
		CCSPlayer_m_iAccount,
		CCSPlayer_m_iAddonBits,
		CCSPlayer_m_iBlockingUseActionInProgress,
		CCSPlayer_m_iClass,
		CCSPlayer_m_iControlledBotEntIndex,
		CCSPlayer_m_iDirection,
		CCSPlayer_m_iGunGameProgressiveWeaponIndex,
		CCSPlayer_m_iMatchStats_Assists,
		CCSPlayer_m_iMatchStats_CashEarned,
		CCSPlayer_m_iMatchStats_Damage,
		CCSPlayer_m_iMatchStats_Deaths,
		CCSPlayer_m_iMatchStats_EnemiesFlashed,
		CCSPlayer_m_iMatchStats_EquipmentValue,
		CCSPlayer_m_iMatchStats_HeadShotKills,
		CCSPlayer_m_iMatchStats_KillReward,
		CCSPlayer_m_iMatchStats_Kills,
		CCSPlayer_m_iMatchStats_LiveTime,
		CCSPlayer_m_iMatchStats_MoneySaved,
		CCSPlayer_m_iMatchStats_Objective,
		CCSPlayer_m_iMatchStats_UtilityDamage,
		CCSPlayer_m_iMoveState,
		CCSPlayer_m_iNumGunGameKillsWithCurrentWeapon,
		CCSPlayer_m_iNumGunGameTRKillPoints,
		CCSPlayer_m_iNumRoundKills,
		CCSPlayer_m_iNumRoundKillsHeadshots,
		CCSPlayer_m_iPlayerState,
		CCSPlayer_m_iPrimaryAddon,
		CCSPlayer_m_iProgressBarDuration,
		CCSPlayer_m_iRetakesMVPBoostItem,
		CCSPlayer_m_iRetakesOffering,
		CCSPlayer_m_iRetakesOfferingCard,
		CCSPlayer_m_iSecondaryAddon,
		CCSPlayer_m_iShotsFired,
		CCSPlayer_m_iStartAccount,
		CCSPlayer_m_iThrowGrenadeCounter,
		CCSPlayer_m_iWeaponPurchasesThisMatch,
		CCSPlayer_m_iWeaponPurchasesThisRound,
		CCSPlayer_m_isCurrentGunGameLeader,
		CCSPlayer_m_isCurrentGunGameTeamLeader,
		CCSPlayer_m_nDeathCamMusic,
		CCSPlayer_m_nHeavyAssaultSuitCooldownRemaining,
		CCSPlayer_m_nIsAutoMounting,
		CCSPlayer_m_nLastConcurrentKilled,
		CCSPlayer_m_nLastKillerIndex,
		CCSPlayer_m_nNumFastDucks,
		CCSPlayer_m_nQuestProgressReason,
		CCSPlayer_m_nSurvivalTeam,
		CCSPlayer_m_passiveItems,
		CCSPlayer_m_rank,
		CCSPlayer_m_szArmsModel,
		CCSPlayer_m_totalHitsOnServer,
		CCSPlayer_m_unActiveQuestId,
		CCSPlayer_m_unCurrentEquipmentValue,
		CCSPlayer_m_unFreezetimeEndEquipmentValue,
		CCSPlayer_m_unMusicID,
		CCSPlayer_m_unPlayerTvControlFlags,
		CCSPlayer_m_unRoundStartEquipmentValue,
		CCSPlayer_m_unTotalRoundDamageDealt,
		CCSPlayer_m_vecAutomoveTargetEnd,
		CCSPlayer_m_vecOrigin,
		CCSPlayer_m_vecOrigin_2_,
		CCSPlayer_m_vecPlayerPatchEconIndices,
		CCSPlayer_m_vecSpawnRappellingRopeOrigin,
		CCSRagdoll_m_bClientSideAnimation,
		CCSRagdoll_m_bDiedAirborne,
		CCSRagdoll_m_flAbsYaw,
		CCSRagdoll_m_flDeathYaw,
		CCSRagdoll_m_hPlayer,
		CCSRagdoll_m_iDeathFrame,
		CCSRagdoll_m_iDeathPose,
		CCSRagdoll_m_iTeamNum,
		CCSRagdoll_m_nForceBone,
		CCSRagdoll_m_nModelIndex,
		CCSRagdoll_m_vecForce,
		CCSRagdoll_m_vecOrigin,
		CCSRagdoll_m_vecRagdollOrigin,
		CCSRagdoll_m_vecRagdollVelocity,
		CCascadeLight_m_LightColor,
		CCascadeLight_m_LightColorScale,
		CCascadeLight_m_bEnabled,
		CCascadeLight_m_bUseLightEnvAngles,
		CCascadeLight_m_envLightShadowDirection,
		CCascadeLight_m_flMaxShadowDist,
		CCascadeLight_m_shadowDirection,
		CChicken_m_jumpedThisFrame,
		CChicken_m_leader,
		CColorCorrectionVolume_m_FadeDuration,
		CColorCorrectionVolume_m_MaxWeight,
		CColorCorrectionVolume_m_Weight,
		CColorCorrectionVolume_m_bEnabled,
		CColorCorrectionVolume_m_lookupFilename,
		CColorCorrection_m_bClientSide,
		CColorCorrection_m_bEnabled,
		CColorCorrection_m_bExclusive,
		CColorCorrection_m_bMaster,
		CColorCorrection_m_flCurWeight,
		CColorCorrection_m_flFadeInDuration,
		CColorCorrection_m_flFadeOutDuration,
		CColorCorrection_m_flMaxWeight,
		CColorCorrection_m_maxFalloff,
		CColorCorrection_m_minFalloff,
		CColorCorrection_m_netLookupFilename,
		CColorCorrection_m_vecOrigin,
		CDangerZoneController_m_DangerZones,
		CDangerZoneController_m_bDangerZoneControllerEnabled,
		CDangerZoneController_m_bMissionControlledExplosions,
		CDangerZoneController_m_flFinalExpansionTime,
		CDangerZoneController_m_flStartTime,
		CDangerZoneController_m_flWaveEndTimes,
		CDangerZoneController_m_hTheFinalZone,
		CDangerZoneController_m_vecEndGameCircleEnd,
		CDangerZoneController_m_vecEndGameCircleStart,
		CDangerZone_m_flBombLaunchTime,
		CDangerZone_m_flExtraRadius,
		CDangerZone_m_flExtraRadiusStartTime,
		CDangerZone_m_flExtraRadiusTotalLerpTime,
		CDangerZone_m_iWave,
		CDangerZone_m_nDropOrder,
		CDangerZone_m_vecDangerZoneOriginStartedAt,
		CDrone_m_bPilotTakeoverAllowed,
		CDrone_m_hCurrentPilot,
		CDrone_m_hDeliveryCargo,
		CDrone_m_hMoveToThisEntity,
		CDrone_m_hPotentialCargo,
		CDrone_m_vecTagIncrements_0_,
		CDrone_m_vecTagPositions_0_,
		CDronegun_m_bHasTarget,
		CDronegun_m_iHealth,
		CDronegun_m_vecAttentionTarget,
		CDronegun_m_vecTargetOffset,
		CDynamicLight_m_Exponent,
		CDynamicLight_m_Flags,
		CDynamicLight_m_InnerAngle,
		CDynamicLight_m_LightStyle,
		CDynamicLight_m_OuterAngle,
		CDynamicLight_m_Radius,
		CDynamicLight_m_SpotRadius,
		CDynamicProp_m_bShouldGlow,
		CDynamicProp_m_bUseHitboxesForRenderBox,
		CDynamicProp_m_clrGlow,
		CDynamicProp_m_flGlowMaxDist,
		CDynamicProp_m_nGlowStyle,
		CEconEntity_m_AttributeManager,
		CEconEntity_m_Attributes,
		CEconEntity_m_Item,
		CEconEntity_m_NetworkedDynamicAttributesForDemos,
		CEconEntity_m_OriginalOwnerXuidHigh,
		CEconEntity_m_OriginalOwnerXuidLow,
		CEconEntity_m_ProviderType,
		CEconEntity_m_bInitialized,
		CEconEntity_m_flFallbackWear,
		CEconEntity_m_hOuter,
		CEconEntity_m_iAccountID,
		CEconEntity_m_iEntityLevel,
		CEconEntity_m_iEntityQuality,
		CEconEntity_m_iItemDefinitionIndex,
		CEconEntity_m_iItemIDHigh,
		CEconEntity_m_iItemIDLow,
		CEconEntity_m_iReapplyProvisionParity,
		CEconEntity_m_nFallbackPaintKit,
		CEconEntity_m_nFallbackSeed,
		CEconEntity_m_nFallbackStatTrak,
		CEconEntity_m_szCustomName,
		CEmbers_m_bEmit,
		CEmbers_m_nDensity,
		CEmbers_m_nLifetime,
		CEmbers_m_nSpeed,
		CEntityDissolve_m_flFadeInLength,
		CEntityDissolve_m_flFadeInStart,
		CEntityDissolve_m_flFadeOutLength,
		CEntityDissolve_m_flFadeOutModelLength,
		CEntityDissolve_m_flFadeOutModelStart,
		CEntityDissolve_m_flFadeOutStart,
		CEntityDissolve_m_flStartTime,
		CEntityDissolve_m_nDissolveType,
		CEntityDissolve_m_nMagnitude,
		CEntityDissolve_m_vDissolverOrigin,
		CEntityFlame_m_bCheapEffect,
		CEntityFlame_m_hEntAttached,
		CEntityFreezing_m_bFinishFreezing,
		CEntityFreezing_m_flFrozen,
		CEntityFreezing_m_flFrozenPerHitbox,
		CEntityFreezing_m_vFreezingOrigin,
		CEntityParticleTrail_m_Info,
		CEntityParticleTrail_m_flEndSize,
		CEntityParticleTrail_m_flLifetime,
		CEntityParticleTrail_m_flStartSize,
		CEntityParticleTrail_m_hConstraintEntity,
		CEntityParticleTrail_m_iMaterialName,
		CEnvAmbientLight_m_vecColor,
		CEnvDOFController_m_bDOFEnabled,
		CEnvDOFController_m_flFarBlurDepth,
		CEnvDOFController_m_flFarBlurRadius,
		CEnvDOFController_m_flFarFocusDepth,
		CEnvDOFController_m_flNearBlurDepth,
		CEnvDOFController_m_flNearBlurRadius,
		CEnvDOFController_m_flNearFocusDepth,
		CEnvDetailController_m_flFadeEndDist,
		CEnvDetailController_m_flFadeStartDist,
		CEnvGasCanister_m_bDoImpactEffects,
		CEnvGasCanister_m_bInSkybox,
		CEnvGasCanister_m_bLanded,
		CEnvGasCanister_m_bLaunchedFromWithinWorld,
		CEnvGasCanister_m_flFlightSpeed,
		CEnvGasCanister_m_flFlightTime,
		CEnvGasCanister_m_flHorizSpeed,
		CEnvGasCanister_m_flInitialZSpeed,
		CEnvGasCanister_m_flLaunchTime,
		CEnvGasCanister_m_flSkyboxScale,
		CEnvGasCanister_m_flWorldEnterTime,
		CEnvGasCanister_m_flZAcceleration,
		CEnvGasCanister_m_hSkyboxCopy,
		CEnvGasCanister_m_nMyZoneIndex,
		CEnvGasCanister_m_vecDirection,
		CEnvGasCanister_m_vecEnterWorldPosition,
		CEnvGasCanister_m_vecImpactPosition,
		CEnvGasCanister_m_vecOrigin,
		CEnvGasCanister_m_vecOrigin_2_,
		CEnvGasCanister_m_vecParabolaDirection,
		CEnvGasCanister_m_vecSkyboxOrigin,
		CEnvGasCanister_m_vecStartAngles,
		CEnvGasCanister_m_vecStartPosition,
		CEnvParticleScript_m_flSequenceScale,
		CEnvProjectedTexture_m_LightColor,
		CEnvProjectedTexture_m_SpotlightTextureName,
		CEnvProjectedTexture_m_bAlwaysUpdate,
		CEnvProjectedTexture_m_bCameraSpace,
		CEnvProjectedTexture_m_bEnableShadows,
		CEnvProjectedTexture_m_bLightOnlyTarget,
		CEnvProjectedTexture_m_bLightWorld,
		CEnvProjectedTexture_m_bSimpleProjection,
		CEnvProjectedTexture_m_bState,
		CEnvProjectedTexture_m_flAmbient,
		CEnvProjectedTexture_m_flBrightnessScale,
		CEnvProjectedTexture_m_flColorTransitionTime,
		CEnvProjectedTexture_m_flFarZ,
		CEnvProjectedTexture_m_flLightFOV,
		CEnvProjectedTexture_m_flNearZ,
		CEnvProjectedTexture_m_flProjectionSize,
		CEnvProjectedTexture_m_flRotation,
		CEnvProjectedTexture_m_hTargetEntity,
		CEnvProjectedTexture_m_iStyle,
		CEnvProjectedTexture_m_nShadowQuality,
		CEnvProjectedTexture_m_nSpotlightTextureFrame,
		CEnvQuadraticBeam_m_controlPosition,
		CEnvQuadraticBeam_m_flWidth,
		CEnvQuadraticBeam_m_scrollRate,
		CEnvQuadraticBeam_m_targetPosition,
		CEnvScreenEffect_m_flDuration,
		CEnvScreenEffect_m_nType,
		CEnvScreenOverlay_m_bIsActive,
		CEnvScreenOverlay_m_flOverlayTimes_0_,
		CEnvScreenOverlay_m_flStartTime,
		CEnvScreenOverlay_m_iDesiredOverlay,
		CEnvScreenOverlay_m_iszOverlayNames_0_,
		CEnvTonemapController_m_bUseCustomAutoExposureMax,
		CEnvTonemapController_m_bUseCustomAutoExposureMin,
		CEnvTonemapController_m_bUseCustomBloomScale,
		CEnvTonemapController_m_flBloomExponent,
		CEnvTonemapController_m_flBloomSaturation,
		CEnvTonemapController_m_flCustomAutoExposureMax,
		CEnvTonemapController_m_flCustomAutoExposureMin,
		CEnvTonemapController_m_flCustomBloomScale,
		CEnvTonemapController_m_flCustomBloomScaleMinimum,
		CEnvTonemapController_m_flTonemapMinAvgLum,
		CEnvTonemapController_m_flTonemapPercentBrightPixels,
		CEnvTonemapController_m_flTonemapPercentTarget,
		CEnvTonemapController_m_flTonemapRate,
		CEnvWind_m_EnvWindShared,
		CEnvWind_m_flGustDuration,
		CEnvWind_m_flInitialWindSpeed,
		CEnvWind_m_flMaxGustDelay,
		CEnvWind_m_flMinGustDelay,
		CEnvWind_m_flStartTime,
		CEnvWind_m_iGustDirChange,
		CEnvWind_m_iInitialWindDir,
		CEnvWind_m_iMaxGust,
		CEnvWind_m_iMaxWind,
		CEnvWind_m_iMinGust,
		CEnvWind_m_iMinWind,
		CEnvWind_m_iWindSeed,
		CFEPlayerDecal_m_flCreationTime,
		CFEPlayerDecal_m_nEntity,
		CFEPlayerDecal_m_nHitbox,
		CFEPlayerDecal_m_nPlayer,
		CFEPlayerDecal_m_nTintID,
		CFEPlayerDecal_m_nUniqueID,
		CFEPlayerDecal_m_nVersion,
		CFEPlayerDecal_m_rtGcTime,
		CFEPlayerDecal_m_ubSignature,
		CFEPlayerDecal_m_unAccountID,
		CFEPlayerDecal_m_unTraceID,
		CFEPlayerDecal_m_vecEndPos,
		CFEPlayerDecal_m_vecNormal,
		CFEPlayerDecal_m_vecRight,
		CFEPlayerDecal_m_vecStart,
		CFireSmoke_m_flScale,
		CFireSmoke_m_flScaleTime,
		CFireSmoke_m_flStartScale,
		CFireSmoke_m_nFlags,
		CFireSmoke_m_nFlameFromAboveModelIndex,
		CFireSmoke_m_nFlameModelIndex,
		CFireTrail_m_flLifetime,
		CFireTrail_m_nAttachment,
		CFish_m_angle,
		CFish_m_lifeState,
		CFish_m_nModelIndex,
		CFish_m_poolOrigin,
		CFish_m_waterLevel,
		CFish_m_x,
		CFish_m_y,
		CFish_m_z,
		CFists_m_bPlayingUninterruptableAct,
		CFogController_m_fog_HDRColorScale,
		CFogController_m_fog_ZoomFogScale,
		CFogController_m_fog_blend,
		CFogController_m_fog_colorPrimary,
		CFogController_m_fog_colorPrimaryLerpTo,
		CFogController_m_fog_colorSecondary,
		CFogController_m_fog_colorSecondaryLerpTo,
		CFogController_m_fog_dirPrimary,
		CFogController_m_fog_duration,
		CFogController_m_fog_enable,
		CFogController_m_fog_end,
		CFogController_m_fog_endLerpTo,
		CFogController_m_fog_farz,
		CFogController_m_fog_lerptime,
		CFogController_m_fog_maxdensity,
		CFogController_m_fog_maxdensityLerpTo,
		CFogController_m_fog_start,
		CFogController_m_fog_startLerpTo,
		CFootstepControl_m_destination,
		CFootstepControl_m_source,
		CFuncAreaPortalWindow_m_flFadeDist,
		CFuncAreaPortalWindow_m_flFadeStartDist,
		CFuncAreaPortalWindow_m_flTranslucencyLimit,
		CFuncAreaPortalWindow_m_iBackgroundModelIndex,
		CFuncConveyor_m_flConveyorSpeed,
		CFuncLadder_m_bFakeLadder,
		CFuncLadder_m_vecLadderDir,
		CFuncLadder_m_vecPlayerMountPositionBottom,
		CFuncLadder_m_vecPlayerMountPositionTop,
		CFuncMoveLinear_m_fFlags,
		CFuncMoveLinear_m_vecVelocity,
		CFuncOccluder_m_bActive,
		CFuncOccluder_m_nOccluderIndex,
		CFuncRotating_m_angRotation_0_,
		CFuncRotating_m_angRotation_1_,
		CFuncRotating_m_angRotation_2_,
		CFuncRotating_m_flSimulationTime,
		CFuncRotating_m_vecOrigin,
		CFuncSmokeVolume_m_Collision,
		CFuncSmokeVolume_m_Color1,
		CFuncSmokeVolume_m_Color2,
		CFuncSmokeVolume_m_Density,
		CFuncSmokeVolume_m_DensityRampSpeed,
		CFuncSmokeVolume_m_MaterialName,
		CFuncSmokeVolume_m_MovementSpeed,
		CFuncSmokeVolume_m_ParticleDrawWidth,
		CFuncSmokeVolume_m_ParticleSpacingDistance,
		CFuncSmokeVolume_m_RotationSpeed,
		CFuncSmokeVolume_m_maxDrawDistance,
		CFuncSmokeVolume_m_nSolidType,
		CFuncSmokeVolume_m_nSurroundType,
		CFuncSmokeVolume_m_spawnflags,
		CFuncSmokeVolume_m_triggerBloat,
		CFuncSmokeVolume_m_usSolidFlags,
		CFuncSmokeVolume_m_vecMaxs,
		CFuncSmokeVolume_m_vecMins,
		CFuncSmokeVolume_m_vecSpecifiedSurroundingMaxs,
		CFuncSmokeVolume_m_vecSpecifiedSurroundingMins,
		CFunc_Dust_m_Collision,
		CFunc_Dust_m_Color,
		CFunc_Dust_m_DistMax,
		CFunc_Dust_m_DustFlags,
		CFunc_Dust_m_FallSpeed,
		CFunc_Dust_m_LifetimeMax,
		CFunc_Dust_m_LifetimeMin,
		CFunc_Dust_m_SpawnRate,
		CFunc_Dust_m_SpeedMax,
		CFunc_Dust_m_bAffectedByWind,
		CFunc_Dust_m_flSizeMax,
		CFunc_Dust_m_flSizeMin,
		CFunc_Dust_m_nModelIndex,
		CFunc_Dust_m_nSolidType,
		CFunc_Dust_m_nSurroundType,
		CFunc_Dust_m_triggerBloat,
		CFunc_Dust_m_usSolidFlags,
		CFunc_Dust_m_vecMaxs,
		CFunc_Dust_m_vecMins,
		CFunc_Dust_m_vecSpecifiedSurroundingMaxs,
		CFunc_Dust_m_vecSpecifiedSurroundingMins,
		CFunc_LOD_m_nDisappearMaxDist,
		CFunc_LOD_m_nDisappearMinDist,
		CGrassBurn_m_flGrassBurnClearTime,
		CHandleTest_m_Handle,
		CHandleTest_m_bSendHandle,
		CHostage_m_fFlags,
		CHostage_m_flDropStartTime,
		CHostage_m_flGrabSuccessTime,
		CHostage_m_flRescueStartTime,
		CHostage_m_iHealth,
		CHostage_m_iMaxHealth,
		CHostage_m_isRescued,
		CHostage_m_jumpedThisFrame,
		CHostage_m_leader,
		CHostage_m_lifeState,
		CHostage_m_nHostageState,
		CHostage_m_vel,
		CInferno_m_bFireIsBurning,
		CInferno_m_fireCount,
		CInferno_m_fireXDelta,
		CInferno_m_fireYDelta,
		CInferno_m_fireZDelta,
		CInferno_m_nFireEffectTickBegin,
		CInfoMapRegion_m_flRadius,
		CInfoMapRegion_m_szLocToken,
		CInfoOverlayAccessor_m_iOverlayID,
		CInfoOverlayAccessor_m_iTextureFrameIndex,
		CItemAssaultSuitUseable_m_bIsHeavyAssaultSuit,
		CItemAssaultSuitUseable_m_nArmorValue,
		CItemDogtags_m_KillingPlayer,
		CItemDogtags_m_OwningPlayer,
		CItem_m_bShouldGlow,
		CLightGlow_m_angRotation,
		CLightGlow_m_clrRender,
		CLightGlow_m_flGlowProxySize,
		CLightGlow_m_nHorizontalSize,
		CLightGlow_m_nMaxDist,
		CLightGlow_m_nMinDist,
		CLightGlow_m_nOuterMaxDist,
		CLightGlow_m_nVerticalSize,
		CLightGlow_m_spawnflags,
		CLightGlow_m_vecOrigin,
		CLightGlow_moveparent,
		CMapVetoPickController_m_nAccountIDs,
		CMapVetoPickController_m_nCurrentPhase,
		CMapVetoPickController_m_nDraftType,
		CMapVetoPickController_m_nMapId0,
		CMapVetoPickController_m_nMapId1,
		CMapVetoPickController_m_nMapId2,
		CMapVetoPickController_m_nMapId3,
		CMapVetoPickController_m_nMapId4,
		CMapVetoPickController_m_nMapId5,
		CMapVetoPickController_m_nPhaseDurationTicks,
		CMapVetoPickController_m_nPhaseStartTick,
		CMapVetoPickController_m_nStartingSide0,
		CMapVetoPickController_m_nTeamWinningCoinToss,
		CMapVetoPickController_m_nTeamWithFirstChoice,
		CMapVetoPickController_m_nVoteMapIdsList,
		CMaterialModifyControl_m_bFloatLerpWrap,
		CMaterialModifyControl_m_bNewAnimCommandsSemaphore,
		CMaterialModifyControl_m_bWrap,
		CMaterialModifyControl_m_flFloatLerpEndValue,
		CMaterialModifyControl_m_flFloatLerpStartValue,
		CMaterialModifyControl_m_flFloatLerpTransitionTime,
		CMaterialModifyControl_m_flFramerate,
		CMaterialModifyControl_m_iFrameEnd,
		CMaterialModifyControl_m_iFrameStart,
		CMaterialModifyControl_m_nModifyMode,
		CMaterialModifyControl_m_szMaterialName,
		CMaterialModifyControl_m_szMaterialVar,
		CMaterialModifyControl_m_szMaterialVarValue,
		CMelee_m_flThrowAt,
		CMolotovProjectile_m_bIsIncGrenade,
		CMovieDisplay_m_bEnabled,
		CMovieDisplay_m_bForcedSlave,
		CMovieDisplay_m_bLooping,
		CMovieDisplay_m_bStretchToFill,
		CMovieDisplay_m_bUseCustomUVs,
		CMovieDisplay_m_flUMax,
		CMovieDisplay_m_flUMin,
		CMovieDisplay_m_flVMax,
		CMovieDisplay_m_flVMin,
		CMovieDisplay_m_szGroupName,
		CMovieDisplay_m_szMovieFilename,
		CParadropChopper_m_hCallingPlayer,
		CParadropChopper_m_vecOrigin,
		CParadropChopper_m_vecOrigin_2_,
		CParticleFire_m_vDirection,
		CParticleFire_m_vOrigin,
		CParticlePerformanceMonitor_m_bDisplayPerf,
		CParticlePerformanceMonitor_m_bMeasurePerf,
		CParticleSystem_m_angRotation,
		CParticleSystem_m_bActive,
		CParticleSystem_m_fEffects,
		CParticleSystem_m_flStartTime,
		CParticleSystem_m_hControlPointEnts,
		CParticleSystem_m_hOwnerEntity,
		CParticleSystem_m_iControlPointParents,
		CParticleSystem_m_iEffectIndex,
		CParticleSystem_m_iParentAttachment,
		CParticleSystem_m_iServerControlPointAssignments,
		CParticleSystem_m_nStopType,
		CParticleSystem_m_szSnapshotFileName,
		CParticleSystem_m_vServerControlPoints,
		CParticleSystem_m_vecOrigin,
		CParticleSystem_moveparent,
		CPhysBoxMultiplayer_m_fMass,
		CPhysBoxMultiplayer_m_iPhysicsMode,
		CPhysBox_m_mass,
		CPhysPropLootCrate_m_bRenderInPSPM,
		CPhysPropLootCrate_m_bRenderInTablet,
		CPhysPropLootCrate_m_iHealth,
		CPhysPropLootCrate_m_iMaxHealth,
		CPhysicsPropMultiplayer_m_collisionMaxs,
		CPhysicsPropMultiplayer_m_collisionMins,
		CPhysicsPropMultiplayer_m_fMass,
		CPhysicsPropMultiplayer_m_iPhysicsMode,
		CPhysicsProp_m_bAwake,
		CPhysicsProp_m_spawnflags,
		CPlantedC4_m_bBombDefused,
		CPlantedC4_m_bBombTicking,
		CPlantedC4_m_flC4Blow,
		CPlantedC4_m_flDefuseCountDown,
		CPlantedC4_m_flDefuseLength,
		CPlantedC4_m_flTimerLength,
		CPlantedC4_m_hBombDefuser,
		CPlantedC4_m_nBombSite,
		CPlasma_m_flScale,
		CPlasma_m_flScaleTime,
		CPlasma_m_flStartScale,
		CPlasma_m_nFlags,
		CPlasma_m_nGlowModelIndex,
		CPlasma_m_nPlasmaModelIndex,
		CPlasma_m_nPlasmaModelIndex2,
		CPlayerPing_m_bUrgent,
		CPlayerPing_m_hPingedEntity,
		CPlayerPing_m_hPlayer,
		CPlayerPing_m_iType,
		CPlayerPing_m_szPlaceName,
		CPlayerResource_m_bAlive,
		CPlayerResource_m_bConnected,
		CPlayerResource_m_iAssists,
		CPlayerResource_m_iCoachingTeam,
		CPlayerResource_m_iDeaths,
		CPlayerResource_m_iHealth,
		CPlayerResource_m_iKills,
		CPlayerResource_m_iPendingTeam,
		CPlayerResource_m_iPing,
		CPlayerResource_m_iTeam,
		CPointCamera_m_FOV,
		CPointCamera_m_FogColor,
		CPointCamera_m_Resolution,
		CPointCamera_m_bActive,
		CPointCamera_m_bFogEnable,
		CPointCamera_m_bUseScreenAspectRatio,
		CPointCamera_m_flFogEnd,
		CPointCamera_m_flFogMaxDensity,
		CPointCamera_m_flFogStart,
		CPointCommentaryNode_m_bActive,
		CPointCommentaryNode_m_flStartTime,
		CPointCommentaryNode_m_hViewPosition,
		CPointCommentaryNode_m_iNodeNumber,
		CPointCommentaryNode_m_iNodeNumberMax,
		CPointCommentaryNode_m_iszCommentaryFile,
		CPointCommentaryNode_m_iszCommentaryFileNoHDR,
		CPointCommentaryNode_m_iszSpeakers,
		CPointWorldText_m_flTextSize,
		CPointWorldText_m_szText,
		CPointWorldText_m_textColor,
		CPoseController_m_bInterpolationWrap,
		CPoseController_m_bPoseValueParity,
		CPoseController_m_chPoseIndex,
		CPoseController_m_fCycleFrequency,
		CPoseController_m_fFModAmplitude,
		CPoseController_m_fFModRate,
		CPoseController_m_fFModTimeOffset,
		CPoseController_m_fInterpolationTime,
		CPoseController_m_fPoseValue,
		CPoseController_m_hProps,
		CPoseController_m_nFModType,
		CPostProcessController_m_bMaster,
		CPostProcessController_m_flPostProcessParameters,
		CPrecipitation_m_nPrecipType,
		CPropCounter_m_flDisplayValue,
		CPropJeep_m_bHeadlightIsOn,
		CPropVehicleChoreoGeneric_m_bEnterAnimOn,
		CPropVehicleChoreoGeneric_m_bExitAnimOn,
		CPropVehicleChoreoGeneric_m_bForceEyesToAttachment,
		CPropVehicleChoreoGeneric_m_hPlayer,
		CPropVehicleChoreoGeneric_m_vecEyeExitEndpoint,
		CPropVehicleChoreoGeneric_m_vehicleView_bClampEyeAngles,
		CPropVehicleChoreoGeneric_m_vehicleView_flFOV,
		CPropVehicleChoreoGeneric_m_vehicleView_flPitchCurveLinear,
		CPropVehicleChoreoGeneric_m_vehicleView_flPitchCurveZero,
		CPropVehicleChoreoGeneric_m_vehicleView_flPitchMax,
		CPropVehicleChoreoGeneric_m_vehicleView_flPitchMin,
		CPropVehicleChoreoGeneric_m_vehicleView_flRollCurveLinear,
		CPropVehicleChoreoGeneric_m_vehicleView_flRollCurveZero,
		CPropVehicleChoreoGeneric_m_vehicleView_flYawMax,
		CPropVehicleChoreoGeneric_m_vehicleView_flYawMin,
		CPropVehicleDriveable_m_bEnterAnimOn,
		CPropVehicleDriveable_m_bExitAnimOn,
		CPropVehicleDriveable_m_bHasGun,
		CPropVehicleDriveable_m_bUnableToFire,
		CPropVehicleDriveable_m_flThrottle,
		CPropVehicleDriveable_m_hPlayer,
		CPropVehicleDriveable_m_nBoostTimeLeft,
		CPropVehicleDriveable_m_nHasBoost,
		CPropVehicleDriveable_m_nRPM,
		CPropVehicleDriveable_m_nScannerDisabledVehicle,
		CPropVehicleDriveable_m_nScannerDisabledWeapons,
		CPropVehicleDriveable_m_nSpeed,
		CPropVehicleDriveable_m_vecEyeExitEndpoint,
		CPropVehicleDriveable_m_vecGunCrosshair,
		CProp_Hallucination_m_bEnabled,
		CProp_Hallucination_m_fRechargeTime,
		CProp_Hallucination_m_fVisibleTime,
		CRagdollManager_m_iCurrentMaxRagdollCount,
		CRagdollPropAttached_m_attachmentPointBoneSpace,
		CRagdollPropAttached_m_attachmentPointRagdollSpace,
		CRagdollPropAttached_m_boneIndexAttached,
		CRagdollPropAttached_m_ragdollAttachedObjectIndex,
		CRagdollProp_m_flBlendWeight,
		CRagdollProp_m_hUnragdoll,
		CRagdollProp_m_nOverlaySequence,
		CRagdollProp_m_ragAngles_0_,
		CRagdollProp_m_ragPos_0_,
		CRopeKeyframe_m_RopeFlags,
		CRopeKeyframe_m_RopeLength,
		CRopeKeyframe_m_Slack,
		CRopeKeyframe_m_Subdiv,
		CRopeKeyframe_m_TextureScale,
		CRopeKeyframe_m_Width,
		CRopeKeyframe_m_bConstrainBetweenEndpoints,
		CRopeKeyframe_m_fLockedPoints,
		CRopeKeyframe_m_flScrollSpeed,
		CRopeKeyframe_m_hEndPoint,
		CRopeKeyframe_m_hStartPoint,
		CRopeKeyframe_m_iDefaultRopeMaterialModelIndex,
		CRopeKeyframe_m_iEndAttachment,
		CRopeKeyframe_m_iParentAttachment,
		CRopeKeyframe_m_iRopeMaterialModelIndex,
		CRopeKeyframe_m_iStartAttachment,
		CRopeKeyframe_m_nChangeCount,
		CRopeKeyframe_m_nMaxCPULevel,
		CRopeKeyframe_m_nMaxGPULevel,
		CRopeKeyframe_m_nMinCPULevel,
		CRopeKeyframe_m_nMinGPULevel,
		CRopeKeyframe_m_nSegments,
		CRopeKeyframe_m_vecOrigin,
		CRopeKeyframe_moveparent,
		CSceneEntity_m_bIsPlayingBack,
		CSceneEntity_m_bMultiplayer,
		CSceneEntity_m_bPaused,
		CSceneEntity_m_flForceClientTime,
		CSceneEntity_m_nSceneStringIndex,
		CShadowControl_m_bDisableShadows,
		CShadowControl_m_bEnableLocalLightShadows,
		CShadowControl_m_flShadowMaxDist,
		CShadowControl_m_shadowColor,
		CShadowControl_m_shadowDirection,
		CSlideshowDisplay_m_bEnabled,
		CSlideshowDisplay_m_bNoListRepeats,
		CSlideshowDisplay_m_chCurrentSlideLists,
		CSlideshowDisplay_m_fMaxSlideTime,
		CSlideshowDisplay_m_fMinSlideTime,
		CSlideshowDisplay_m_iCycleType,
		CSlideshowDisplay_m_szDisplayText,
		CSlideshowDisplay_m_szSlideshowDirectory,
		CSmokeGrenadeProjectile_m_bDidSmokeEffect,
		CSmokeGrenadeProjectile_m_nSmokeEffectTickBegin,
		CSmokeStack_m_AmbientLight_m_flIntensity,
		CSmokeStack_m_AmbientLight_m_vColor,
		CSmokeStack_m_AmbientLight_m_vPos,
		CSmokeStack_m_DirLight_m_flIntensity,
		CSmokeStack_m_DirLight_m_vColor,
		CSmokeStack_m_DirLight_m_vPos,
		CSmokeStack_m_EndSize,
		CSmokeStack_m_JetLength,
		CSmokeStack_m_Rate,
		CSmokeStack_m_Speed,
		CSmokeStack_m_SpreadSpeed,
		CSmokeStack_m_StartSize,
		CSmokeStack_m_bEmit,
		CSmokeStack_m_flBaseSpread,
		CSmokeStack_m_flRollSpeed,
		CSmokeStack_m_flTwist,
		CSmokeStack_m_iMaterialModel,
		CSmokeStack_m_vWind,
		CSpatialEntity_m_bEnabled,
		CSpatialEntity_m_flCurWeight,
		CSpatialEntity_m_maxFalloff,
		CSpatialEntity_m_minFalloff,
		CSpatialEntity_m_vecOrigin,
		CSpotlightEnd_m_Radius,
		CSpotlightEnd_m_flLightScale,
		CSpriteTrail_m_flEndWidth,
		CSpriteTrail_m_flLifeTime,
		CSpriteTrail_m_flMinFadeLength,
		CSpriteTrail_m_flSkyboxScale,
		CSpriteTrail_m_flStartWidth,
		CSpriteTrail_m_flStartWidthVariance,
		CSpriteTrail_m_flTextureRes,
		CSpriteTrail_m_vecSkyboxOrigin,
		CSprite_m_bWorldSpaceScale,
		CSprite_m_flBrightnessTime,
		CSprite_m_flFrame,
		CSprite_m_flGlowProxySize,
		CSprite_m_flHDRColorScale,
		CSprite_m_flScaleTime,
		CSprite_m_flSpriteFramerate,
		CSprite_m_flSpriteScale,
		CSprite_m_hAttachedToEntity,
		CSprite_m_nAttachment,
		CSprite_m_nBrightness,
		CStatueProp_m_bShatter,
		CStatueProp_m_hInitBaseAnimating,
		CStatueProp_m_nShatterFlags,
		CStatueProp_m_vShatterForce,
		CStatueProp_m_vShatterPosition,
		CSteamJet_m_EndSize,
		CSteamJet_m_JetLength,
		CSteamJet_m_Rate,
		CSteamJet_m_Speed,
		CSteamJet_m_SpreadSpeed,
		CSteamJet_m_StartSize,
		CSteamJet_m_bEmit,
		CSteamJet_m_bFaceLeft,
		CSteamJet_m_flRollSpeed,
		CSteamJet_m_nType,
		CSteamJet_m_spawnflags,
		CSun_m_bOn,
		CSun_m_clrOverlay,
		CSun_m_clrRender,
		CSun_m_nMaterial,
		CSun_m_nOverlayMaterial,
		CSun_m_nOverlaySize,
		CSun_m_nSize,
		CSun_m_vDirection,
		CSunlightShadowControl_m_LightColor,
		CSunlightShadowControl_m_TextureName,
		CSunlightShadowControl_m_bEnableShadows,
		CSunlightShadowControl_m_bEnabled,
		CSunlightShadowControl_m_flColorTransitionTime,
		CSunlightShadowControl_m_flFOV,
		CSunlightShadowControl_m_flNearZ,
		CSunlightShadowControl_m_flNorthOffset,
		CSunlightShadowControl_m_flSunDistance,
		CSunlightShadowControl_m_shadowDirection,
		CSurvivalSpawnChopper_m_vecOrigin,
		CSurvivalSpawnChopper_m_vecOrigin_2_,
		CTEBSPDecal_m_nEntity,
		CTEBSPDecal_m_nIndex,
		CTEBSPDecal_m_vecOrigin,
		CTEBaseBeam_a,
		CTEBaseBeam_b,
		CTEBaseBeam_g,
		CTEBaseBeam_m_fAmplitude,
		CTEBaseBeam_m_fEndWidth,
		CTEBaseBeam_m_fLife,
		CTEBaseBeam_m_fWidth,
		CTEBaseBeam_m_nFadeLength,
		CTEBaseBeam_m_nFlags,
		CTEBaseBeam_m_nFrameRate,
		CTEBaseBeam_m_nHaloIndex,
		CTEBaseBeam_m_nModelIndex,
		CTEBaseBeam_m_nSpeed,
		CTEBaseBeam_m_nStartFrame,
		CTEBaseBeam_r,
		CTEBeamEntPoint_m_nEndEntity,
		CTEBeamEntPoint_m_nStartEntity,
		CTEBeamEntPoint_m_vecEndPoint,
		CTEBeamEntPoint_m_vecStartPoint,
		CTEBeamEnts_m_nEndEntity,
		CTEBeamEnts_m_nStartEntity,
		CTEBeamFollow_m_iEntIndex,
		CTEBeamLaser_m_nEndEntity,
		CTEBeamLaser_m_nStartEntity,
		CTEBeamPoints_m_vecEndPoint,
		CTEBeamPoints_m_vecStartPoint,
		CTEBeamRingPoint_m_flEndRadius,
		CTEBeamRingPoint_m_flStartRadius,
		CTEBeamRingPoint_m_vecCenter,
		CTEBeamRing_m_nEndEntity,
		CTEBeamRing_m_nStartEntity,
		CTEBeamSpline_m_nPoints,
		CTEBeamSpline_m_vecPoints_0_,
		CTEBloodSprite_a,
		CTEBloodSprite_b,
		CTEBloodSprite_g,
		CTEBloodSprite_m_nDropModel,
		CTEBloodSprite_m_nSize,
		CTEBloodSprite_m_nSprayModel,
		CTEBloodSprite_m_vecDirection,
		CTEBloodSprite_m_vecOrigin,
		CTEBloodSprite_r,
		CTEBloodStream_a,
		CTEBloodStream_b,
		CTEBloodStream_g,
		CTEBloodStream_m_nAmount,
		CTEBloodStream_m_vecDirection,
		CTEBloodStream_r,
		CTEBreakModel_m_angRotation_0_,
		CTEBreakModel_m_angRotation_1_,
		CTEBreakModel_m_angRotation_2_,
		CTEBreakModel_m_fTime,
		CTEBreakModel_m_nCount,
		CTEBreakModel_m_nFlags,
		CTEBreakModel_m_nModelIndex,
		CTEBreakModel_m_nRandomization,
		CTEBreakModel_m_vecOrigin,
		CTEBreakModel_m_vecSize,
		CTEBreakModel_m_vecVelocity,
		CTEBubbleTrail_m_fSpeed,
		CTEBubbleTrail_m_flWaterZ,
		CTEBubbleTrail_m_nCount,
		CTEBubbleTrail_m_nModelIndex,
		CTEBubbleTrail_m_vecMaxs,
		CTEBubbleTrail_m_vecMins,
		CTEBubbles_m_fHeight,
		CTEBubbles_m_fSpeed,
		CTEBubbles_m_nCount,
		CTEBubbles_m_nModelIndex,
		CTEBubbles_m_vecMaxs,
		CTEBubbles_m_vecMins,
		CTEClientProjectile_m_hOwner,
		CTEClientProjectile_m_nLifeTime,
		CTEClientProjectile_m_nModelIndex,
		CTEClientProjectile_m_vecOrigin,
		CTEClientProjectile_m_vecVelocity,
		CTEDecal_m_nEntity,
		CTEDecal_m_nHitbox,
		CTEDecal_m_nIndex,
		CTEDecal_m_vecOrigin,
		CTEDecal_m_vecStart,
		CTEDust_m_flSize,
		CTEDust_m_flSpeed,
		CTEDust_m_vecDirection,
		CTEDynamicLight_b,
		CTEDynamicLight_exponent,
		CTEDynamicLight_g,
		CTEDynamicLight_m_fDecay,
		CTEDynamicLight_m_fRadius,
		CTEDynamicLight_m_fTime,
		CTEDynamicLight_m_vecOrigin,
		CTEDynamicLight_r,
		CTEEffectDispatch_entindex,
		CTEEffectDispatch_m_EffectData,
		CTEEffectDispatch_m_bPositionsAreRelativeToEntity,
		CTEEffectDispatch_m_fFlags,
		CTEEffectDispatch_m_flMagnitude,
		CTEEffectDispatch_m_flRadius,
		CTEEffectDispatch_m_flScale,
		CTEEffectDispatch_m_iEffectName,
		CTEEffectDispatch_m_nAttachmentIndex,
		CTEEffectDispatch_m_nColor,
		CTEEffectDispatch_m_nDamageType,
		CTEEffectDispatch_m_nHitBox,
		CTEEffectDispatch_m_nMaterial,
		CTEEffectDispatch_m_nOtherEntIndex,
		CTEEffectDispatch_m_nSurfaceProp,
		CTEEffectDispatch_m_vAngles,
		CTEEffectDispatch_m_vNormal,
		CTEEffectDispatch_m_vOrigin_x,
		CTEEffectDispatch_m_vOrigin_y,
		CTEEffectDispatch_m_vOrigin_z,
		CTEEffectDispatch_m_vStart_x,
		CTEEffectDispatch_m_vStart_y,
		CTEEffectDispatch_m_vStart_z,
		CTEEnergySplash_m_bExplosive,
		CTEEnergySplash_m_vecDir,
		CTEEnergySplash_m_vecPos,
		CTEExplosion_m_chMaterialType,
		CTEExplosion_m_fScale,
		CTEExplosion_m_nFlags,
		CTEExplosion_m_nFrameRate,
		CTEExplosion_m_nMagnitude,
		CTEExplosion_m_nModelIndex,
		CTEExplosion_m_nRadius,
		CTEExplosion_m_vecNormal,
		CTEFireBullets_m_fInaccuracy,
		CTEFireBullets_m_fSpread,
		CTEFireBullets_m_flRecoilIndex,
		CTEFireBullets_m_iMode,
		CTEFireBullets_m_iPlayer,
		CTEFireBullets_m_iSeed,
		CTEFireBullets_m_iSoundType,
		CTEFireBullets_m_iWeaponID,
		CTEFireBullets_m_nItemDefIndex,
		CTEFireBullets_m_vecAngles_0_,
		CTEFireBullets_m_vecAngles_1_,
		CTEFireBullets_m_vecOrigin,
		CTEFireBullets_m_weapon,
		CTEFizz_m_nCurrent,
		CTEFizz_m_nDensity,
		CTEFizz_m_nEntity,
		CTEFizz_m_nModelIndex,
		CTEFootprintDecal_m_chMaterialType,
		CTEFootprintDecal_m_nEntity,
		CTEFootprintDecal_m_nIndex,
		CTEFootprintDecal_m_vecDirection,
		CTEFootprintDecal_m_vecOrigin,
		CTEFoundryHelpers_m_iEntity,
		CTEGaussExplosion_m_nType,
		CTEGaussExplosion_m_vecDirection,
		CTEGlowSprite_m_fLife,
		CTEGlowSprite_m_fScale,
		CTEGlowSprite_m_nBrightness,
		CTEGlowSprite_m_nModelIndex,
		CTEGlowSprite_m_vecOrigin,
		CTEImpact_m_iType,
		CTEImpact_m_ucFlags,
		CTEImpact_m_vecNormal,
		CTEImpact_m_vecOrigin,
		CTEKillPlayerAttachments_m_nPlayer,
		CTELargeFunnel_m_nModelIndex,
		CTELargeFunnel_m_nReversed,
		CTEMetalSparks_m_vecDir,
		CTEMetalSparks_m_vecPos,
		CTEMuzzleFlash_m_flScale,
		CTEMuzzleFlash_m_nType,
		CTEMuzzleFlash_m_vecAngles,
		CTEMuzzleFlash_m_vecOrigin,
		CTEParticleSystem_m_vecOrigin_0_,
		CTEParticleSystem_m_vecOrigin_1_,
		CTEParticleSystem_m_vecOrigin_2_,
		CTEPhysicsProp_m_angRotation_0_,
		CTEPhysicsProp_m_angRotation_1_,
		CTEPhysicsProp_m_angRotation_2_,
		CTEPhysicsProp_m_clrRender,
		CTEPhysicsProp_m_nEffects,
		CTEPhysicsProp_m_nFlags,
		CTEPhysicsProp_m_nModelIndex,
		CTEPhysicsProp_m_nSkin,
		CTEPhysicsProp_m_vecOrigin,
		CTEPhysicsProp_m_vecVelocity,
		CTEPlantBomb_m_iPlayer,
		CTEPlantBomb_m_option,
		CTEPlantBomb_m_vecOrigin,
		CTEPlayerAnimEvent_m_hPlayer,
		CTEPlayerAnimEvent_m_iEvent,
		CTEPlayerAnimEvent_m_nData,
		CTEPlayerDecal_m_nEntity,
		CTEPlayerDecal_m_nHitbox,
		CTEPlayerDecal_m_nPlayer,
		CTEPlayerDecal_m_vecOrigin,
		CTEPlayerDecal_m_vecRight,
		CTEPlayerDecal_m_vecStart,
		CTEProjectedDecal_m_angRotation,
		CTEProjectedDecal_m_flDistance,
		CTEProjectedDecal_m_nIndex,
		CTEProjectedDecal_m_vecOrigin,
		CTERadioIcon_m_iAttachToClient,
		CTEShatterSurface_m_flHeight,
		CTEShatterSurface_m_flShardSize,
		CTEShatterSurface_m_flWidth,
		CTEShatterSurface_m_nSurfaceType,
		CTEShatterSurface_m_uchBackColor_0_,
		CTEShatterSurface_m_uchBackColor_1_,
		CTEShatterSurface_m_uchBackColor_2_,
		CTEShatterSurface_m_uchFrontColor_0_,
		CTEShatterSurface_m_uchFrontColor_1_,
		CTEShatterSurface_m_uchFrontColor_2_,
		CTEShatterSurface_m_vecAngles,
		CTEShatterSurface_m_vecForce,
		CTEShatterSurface_m_vecForcePos,
		CTEShatterSurface_m_vecOrigin,
		CTEShowLine_m_vecEnd,
		CTESmoke_m_fScale,
		CTESmoke_m_nFrameRate,
		CTESmoke_m_nModelIndex,
		CTESmoke_m_vecOrigin,
		CTESparks_m_nMagnitude,
		CTESparks_m_nTrailLength,
		CTESparks_m_vecDir,
		CTESpriteSpray_m_fNoise,
		CTESpriteSpray_m_nCount,
		CTESpriteSpray_m_nModelIndex,
		CTESpriteSpray_m_nSpeed,
		CTESpriteSpray_m_vecDirection,
		CTESpriteSpray_m_vecOrigin,
		CTESprite_m_fScale,
		CTESprite_m_nBrightness,
		CTESprite_m_nModelIndex,
		CTESprite_m_vecOrigin,
		CTEWorldDecal_m_nIndex,
		CTEWorldDecal_m_vecOrigin,
		CTablet_m_bTabletReceptionIsBlocked,
		CTablet_m_flBootTime,
		CTablet_m_flScanProgress,
		CTablet_m_flShowMapTime,
		CTablet_m_flUpgradeExpirationTime_0_,
		CTablet_m_nContractKillGridHighResIndex,
		CTablet_m_nContractKillGridIndex,
		CTablet_m_nLastPurchaseIndex,
		CTablet_m_vecLocalHexFlags_0_,
		CTablet_m_vecNearestMetalCratePos,
		CTablet_m_vecNotificationIds_0_,
		CTablet_m_vecNotificationTimestamps_0_,
		CTablet_m_vecPlayerPositionHistory_0_,
		CTeam_m_bSurrendered,
		CTeam_m_iClanID,
		CTeam_m_iTeamNum,
		CTeam_m_nGGLeaderEntIndex_CT,
		CTeam_m_nGGLeaderEntIndex_T,
		CTeam_m_numMapVictories,
		CTeam_m_scoreFirstHalf,
		CTeam_m_scoreOvertime,
		CTeam_m_scoreSecondHalf,
		CTeam_m_scoreTotal,
		CTeam_m_szClanTeamname,
		CTeam_m_szTeamFlagImage,
		CTeam_m_szTeamLogoImage,
		CTeam_m_szTeamMatchStat,
		CTeam_m_szTeamname,
		CTeamplayRoundBasedRulesProxy_m_TeamRespawnWaveTimes,
		CTeamplayRoundBasedRulesProxy_m_bAwaitingReadyRestart,
		CTeamplayRoundBasedRulesProxy_m_bInOvertime,
		CTeamplayRoundBasedRulesProxy_m_bInSetup,
		CTeamplayRoundBasedRulesProxy_m_bInWaitingForPlayers,
		CTeamplayRoundBasedRulesProxy_m_bStopWatch,
		CTeamplayRoundBasedRulesProxy_m_bSwitchedTeamsThisRound,
		CTeamplayRoundBasedRulesProxy_m_bTeamReady,
		CTeamplayRoundBasedRulesProxy_m_flMapResetTime,
		CTeamplayRoundBasedRulesProxy_m_flNextRespawnWave,
		CTeamplayRoundBasedRulesProxy_m_flRestartRoundTime,
		CTeamplayRoundBasedRulesProxy_m_iRoundState,
		CTeamplayRoundBasedRulesProxy_m_iWinningTeam,
		CTesla_m_SoundName,
		CTesla_m_iszSpriteName,
		CTestTraceline_m_angRotation_0_,
		CTestTraceline_m_angRotation_1_,
		CTestTraceline_m_angRotation_2_,
		CTestTraceline_m_clrRender,
		CTestTraceline_m_vecOrigin,
		CTestTraceline_moveparent,
		CTest_ProxyToggle_Networkable_m_WithProxy,
		CTriggerSoundOperator_m_nSoundOperator,
		CVGuiScreen_m_fScreenFlags,
		CVGuiScreen_m_flHeight,
		CVGuiScreen_m_flWidth,
		CVGuiScreen_m_hPlayerOwner,
		CVGuiScreen_m_nAttachmentIndex,
		CVGuiScreen_m_nOverlayMaterial,
		CVGuiScreen_m_nPanelName,
		CVoteController_m_bIsYesNoVote,
		CVoteController_m_iActiveIssueIndex,
		CVoteController_m_iOnlyTeamToVote,
		CVoteController_m_nPotentialVotes,
		CVoteController_m_nVoteOptionCount,
		CWaterLODControl_m_flCheapWaterEndDistance,
		CWaterLODControl_m_flCheapWaterStartDistance,
		CWeaponBaseItem_m_bRedraw,
		CWeaponCSBaseGun_m_iBurstShotsRemaining,
		CWeaponCSBaseGun_m_zoomLevel,
		CWeaponCSBase_m_bBurstMode,
		CWeaponCSBase_m_bReloadVisuallyComplete,
		CWeaponCSBase_m_bSilencerOn,
		CWeaponCSBase_m_fAccuracyPenalty,
		CWeaponCSBase_m_fLastShotTime,
		CWeaponCSBase_m_flDoneSwitchingSilencer,
		CWeaponCSBase_m_flPostponeFireReadyTime,
		CWeaponCSBase_m_flRecoilIndex,
		CWeaponCSBase_m_hPrevOwner,
		CWeaponCSBase_m_iIronSightMode,
		CWeaponCSBase_m_iOriginalTeamNumber,
		CWeaponCSBase_m_iRecoilIndex,
		CWeaponCSBase_m_weaponMode,
		CWeaponM3_m_reloadState,
		CWeaponNOVA_m_reloadState,
		CWeaponSawedoff_m_reloadState,
		CWeaponShield_m_flDisplayHealth,
		CWeaponTaser_m_fFireTime,
		CWeaponXM1014_m_reloadState,
		CWorldVguiText_m_bEnabled,
		CWorldVguiText_m_clrText,
		CWorldVguiText_m_iTextPanelWidth,
		CWorldVguiText_m_szDisplayText,
		CWorldVguiText_m_szDisplayTextOption,
		CWorldVguiText_m_szFont,
		CWorld_m_WorldMaxs,
		CWorld_m_WorldMins,
		CWorld_m_bColdWorld,
		CWorld_m_bStartDark,
		CWorld_m_flMaxOccludeeArea,
		CWorld_m_flMaxPropScreenSpaceWidth,
		CWorld_m_flMinOccluderArea,
		CWorld_m_flMinPropScreenSpaceWidth,
		CWorld_m_flWaveHeight,
		CWorld_m_iTimeOfDay,
		CWorld_m_iszDetailSpriteMaterial,
		DustTrail_m_Color,
		DustTrail_m_EndSize,
		DustTrail_m_MaxDirectedSpeed,
		DustTrail_m_MaxSpeed,
		DustTrail_m_MinDirectedSpeed,
		DustTrail_m_MinSpeed,
		DustTrail_m_Opacity,
		DustTrail_m_ParticleLifetime,
		DustTrail_m_SpawnRadius,
		DustTrail_m_SpawnRate,
		DustTrail_m_StartSize,
		DustTrail_m_StopEmitTime,
		DustTrail_m_bEmit,
		ParticleSmokeGrenade_m_CurrentStage,
		ParticleSmokeGrenade_m_FadeEndTime,
		ParticleSmokeGrenade_m_FadeStartTime,
		ParticleSmokeGrenade_m_MaxColor,
		ParticleSmokeGrenade_m_MinColor,
		ParticleSmokeGrenade_m_flSpawnTime,
		RocketTrail_m_EndColor,
		RocketTrail_m_EndSize,
		RocketTrail_m_MaxSpeed,
		RocketTrail_m_MinSpeed,
		RocketTrail_m_Opacity,
		RocketTrail_m_ParticleLifetime,
		RocketTrail_m_SpawnRadius,
		RocketTrail_m_SpawnRate,
		RocketTrail_m_StartColor,
		RocketTrail_m_StartSize,
		RocketTrail_m_StopEmitTime,
		RocketTrail_m_bDamaged,
		RocketTrail_m_bEmit,
		RocketTrail_m_flFlareScale,
		RocketTrail_m_nAttachment,
		SmokeTrail_m_EndColor,
		SmokeTrail_m_EndSize,
		SmokeTrail_m_MaxDirectedSpeed,
		SmokeTrail_m_MaxSpeed,
		SmokeTrail_m_MinDirectedSpeed,
		SmokeTrail_m_MinSpeed,
		SmokeTrail_m_Opacity,
		SmokeTrail_m_ParticleLifetime,
		SmokeTrail_m_SpawnRadius,
		SmokeTrail_m_SpawnRate,
		SmokeTrail_m_StartColor,
		SmokeTrail_m_StartSize,
		SmokeTrail_m_StopEmitTime,
		SmokeTrail_m_bEmit,
		SmokeTrail_m_nAttachment,
		SporeExplosion_m_bDontRemove,
		SporeExplosion_m_bEmit,
		SporeExplosion_m_flEndSize,
		SporeExplosion_m_flParticleLifetime,
		SporeExplosion_m_flSpawnRadius,
		SporeExplosion_m_flSpawnRate,
		SporeExplosion_m_flStartSize,
		SporeTrail_m_bEmit,
		SporeTrail_m_flEndSize,
		SporeTrail_m_flParticleLifetime,
		SporeTrail_m_flSpawnRadius,
		SporeTrail_m_flSpawnRate,
		SporeTrail_m_flStartSize,
		SporeTrail_m_vecEndColor,
		string_CCSPlayer_m_bWaitForNoAttack,
		string_CCSPlayer_m_iMoveState,
		string_CCSPlayer_m_bStrafing,
		string_CCSPlayer_m_flVelocityModifier,
		string_CCSPlayer_m_flThirdpersonRecoil,
		string_CBaseCSGrenade_m_fThrowTime,
		string_CBaseCSGrenade_m_bIsHeldByPlayer,
		COUNT
	};

	enum class randomized_hash : uint64_t
	{
		INVALID,
		interface_client = 0x5f950a0a80a3602f,
		interface_entity_list = 0x6c385bd25d7515dc,
		interface_engine = 0x6440607e060e109e,
		interface_prediction = 0x26a97ec0b735deb7,
		interface_move_helper = 0x8f4a73c18daee058,
		interface_game_movement = 0xd75d1befa6d6d735,
		interface_globals = 0xf77913ec3e42ea52,
		interface_model_info = 0xe6cac7b2e9976c3d,
		interface_model_render = 0xe3e936ce05a72e2b,
		interface_material_system = 0x3ad18aca66376bda,
		interface_render_view = 0x790f990969dc5c18,
		interface_cvar = 0x8f55ff2c0fefbe56,
		interface_view_render = 0xabbccb3ba14ee0d5,
		interface_view_render_beams = 0xed2603878b1f74ff,
		interface_game_event_manager = 0x6f1b5aa76b5df513,
		interface_input = 0xde6f3f4ef883f99c,
		interface_surface_props = 0x8c0d5321f0b55127,
		interface_engine_trace = 0x87f54b604cacb31b,
		interface_client_state = 0x5fbe0848386fd392,
		interface_surface = 0x19ba827cfeeb5c2f,
		interface_memalloc = 0xc6f89eb36d5f7ba2,
		interface_debug_overlay = 0x9c0d656be6725d09,
		interface_glow_object_manager = 0xffded58aba583ff3,
		interface_panel = 0x52b684dfbffcfb62,
		interface_engine_sound = 0x45aa27a084871bf4,
		interface_game_rules_proxy = 0xfbd1b1bbd3bb0344,
		interface_weapon_system = 0x2b0da6699acd5c34,
		interface_inventory_manager = 0xe8f710ad17d022b,
		interface_localize = 0x83f31d18b940f139,
		interface_hud = 0x8de9a3e322980a23,
		interface_player_resource = 0x836ed98c6b680ebe,
		interface_client_leaf_system = 0xfaad2eda7ec3bc89,
		interface_effects = 0x20750c864318f814,
		interface_host_state = 0x7599ce1a016975ac,
		interface_mdl_cache = 0x666b408c6215997d,
		interface_studio_render = 0xcf8a4047210c766c,
		interface_hardware_config = 0xc52b5fcd8352bca3,
		interface_client_tools = 0x56771e6cf50e30d7,
		interface_string_tables = 0x5c84e5915958a7cc,
		interface_file_system = 0x2a8ad0072483d28a,
		interface_key_values_system = 0x3262ce915f099d86,
		interface_panoroama_ui_engine = 0x78e6638549535e70,
		sig_set_abs_angles = 0xc3a1d32b56be3c0a,
		sig_set_abs_origin = 0xf5fed60fecacc639,
		sig_update_anim_state = 0x36e25bdc1aafb9c,
		sig_ret_to_cam_think = 0x8e46e062d10cca1a,
		sig_ret_to_draw_notify = 0xc42fd79f0b3ccc21,
		sig_ret_to_console_print = 0xff1351fa04f03551,
		sig_ret_to_add_notify = 0x3e5770e237a5bc6d,
		sig_ret_to_draw_crosshair = 0xf4d2f2974f9ccf48,
		sig_standard_blending_rules = 0xd84a911496dd8db7,
		sig_build_transformations = 0x46ff1c8303762609,
		sig_smoke_count = 0x6f2142ec04a92cbd,
		sig_get_econ_item_view = 0xde09f04773c692f6,
		sig_get_sequence_act = 0x7407bc9dfeb3749c,
		sig_invalidate_physics_recursive = 0xe8d207d9683a4a6c,
		sig_is_breakable_entity = 0x3928934d6183bc59,
		sig_print_dev_console = 0xbe3564b811e31920,
		sig_find_element = 0xd157c3ed52bcca0c,
		sig_physics_run_think = 0xbe65f44b305283c3,
		sig_cmbat_get_econ_item_view = 0xa41fb05f6d69b274,
		sig_get_static_data = 0xffd223aa73aacd6d,
		sig_disable_post_processing = 0x7db4e75e1b9c0305,
		sig_gc_client_system = 0xecaa3a902f2c0ca8,
		sig_lookup_bone = 0xccce944e060efa52,
		sig_modifier_table = 0x24abb75e7aa89a9f,
		sig_load_named_sky = 0x7cc3509a642a2b4f,
		sig_reveal_ranks = 0x490096b3438f7447,
		sig_set_clantag = 0x64ede781a38595c9,
		sig_deathnotice_check = 0x94dec95c82240968,
		sig_clear_deathnotices = 0x2dbdf37b9113c2a3,
		sig_get_checksum = 0x2b235adc6e6c4eb7,
		sig_equip_glove = 0x1f34daeac2361f6c,
		sig_item_system = 0x37a20d6d8a56250b,
		sig_parse_kits_addr = 0x22fe44c274a525b2,
		sig_util_traceline = 0x2f2466365ccb3d99,
		sig_cl_send_move_limit = 0xc1d502903983da83,
		sig_is_ready = 0x844414dc022a48b3,
		sig_ret_to_setup_velocity = 0xc278b4f3e0f7065e,
		sig_ret_to_post_data_update = 0x5cd9aa968b4d3fb0,
		sig_ret_to_enable_inventory = 0xf57ee05864d091f0,
		sig_interp_clamp = 0xfb2fdd263a27cfcf,
		sig_ret_to_trace_ray = 0xc42fea9560ccb04f,
		sig_ret_to_scope_arc = 0xde35d7f5d0a98680,
		sig_ret_to_scope_lens = 0x80bc033e163cc897,
		sig_post_process_params = 0x8a472a0253bb9bcc,
		sig_ret_to_post_network_data_received = 0x2d63be606e1fa146,
		sig_ret_to_add_viewmodel_stattrak = 0x6a6162f106471169,
		sig_trace_to_studio_csgo_hitgroups_priority = 0x2a02701b6bf52941,
		sig_should_skip_anim_frame_check = 0xaaf1277028c4f501,
		sig_somelimit = 0x553941c2f4d8b3bb,
		sig_cvar_check = 0xedbda623d3778a9b,
		sig_cs_player_pred_map = 0x9adff113adfb11a9,
		sig_base_cs_grenade_pred_map = 0x4f4704ca45599e97,
		sig_decoy_map = 0xa9d81915fb27c10f,
		sig_he_map = 0x23c2b096e07b958b,
		sig_molotov_map = 0xd7807f9e9fed2f0a,
		sig_sensor_map = 0x706ab85882b09958,
		sig_smoke_map = 0x4bdd967c514bbc3e,
		sig_flash_map = 0x3fd82477f4eaad14,
		sig_view_matrix = 0x7378065cf3cf3ee3,
		sig_get_item_schema = 0xfb3531a96a1def63,
		sig_econ_item_destructor = 0x82fb50c135ebdb7d,
		sig_create_econ_item = 0x764724ad07435df7,
		sig_animstate_reset = 0x7a080929c9427337,
		sig_update_layer_order_preset = 0xd3928a0a000aee7a,
		sig_update_equipped_state = 0xf9fdcadc7a6bfa90,
		sig_set_custom_name = 0x57b8272470418e1e,
		sig_set_custom_desc = 0xac82ecfd210f52da,
		sig_set_dynamic_attribute_value = 0x5277811952b3c6a8,
		sig_empty_mapping = 0xd6dbb554069ac8be,
		sig_simulation_tick = 0x8af4a3de8f194409,
		sig_predictable = 0x1e735fa6e85569b3,
		sig_custom_materials = 0x82ed461ec04ded4d,
		sig_custom_material_initialized = 0xd5797cf73ae8480c,
		sig_visual_data_processors = 0x365df4bd5653106b,
		sig_spawn_time = 0x36d78d39e79d9c99,
		sig_ragdoll = 0x28e7810dc53fbd79,
		sig_lod_data = 0xacbf4f1499219b73,
		sig_ik_context = 0x1aaa2f94874993c8,
		sig_snapshot_front = 0x97e44dae4dc040d2,
		sig_snapshot_back = 0x4a0399fd6f773762,
		sig_blend_tick = 0xda223f69b39a03aa,
		sig_get_follow_entity = 0xba7e6a57adb7a936,
		sig_last_client_fire_bullet_time = 0x8fef9060f2ef5d10,
		sig_final_predicted_tick = 0xc293e4b747ec1b83,
		sig_occlusion_mask = 0x9ed793f64d58a70b,
		sig_anim_state = 0xe5eaa2bde336258e,
		sig_command_context = 0xa50337dcd09661af,
		sig_send_matching_sequence = 0x2ed0be87e4333543,
		sig_sound_channels = 0xaed9917eace9b3ae,
		sig_activity_mapping = 0x7fb7c0f42b8bccd9,
		sig_load_from_buffer = 0x13947a28b0465e6f,
		sig_modifier_table_find = 0x9bffcda7fde569af,
		sig_modifier_table_add_string = 0x8bfc48597180fee,
		sig_allocate_thread_id = 0xebbe2b091ab1a29c,
		sig_game_tonemap_settings = 0xa258d7a0fc7583f2,
		sig_length_disasm_game = 0xfc01c181da864751,
		sig_get_active_weapon_prefix = 0x3c6f262f093a6bc5,
		sig_get_layer_ideal_weight_from_sequence_cycle = 0x88f12f1c34e38bdb,
		sig_on_simulation_time_changing = 0x25b320b6116e414b,
		sig_get_root_move_parent = 0x20da1a154c6181a5,
		sig_ik_context_construct = 0xe128512265b13fc2,
		sig_ik_context_destruct = 0x11534a4626666c20,
		sig_ik_context_init = 0xb2c6146356918fda,
		sig_ik_context_update_targets = 0x4243e94e46b30e6d,
		sig_ik_context_solve_dependencies = 0x55c1dbb92e1ec3bc,
		sig_allocate_and_merge_bones = 0xb21b578e8baef74e,
		sig_lookup_sequence = 0x1c4a557c9bfb7a77,
		sig_ammodef_get = 0xefc47e99804bf3ae,
		sig_get_any_sequence_anim_tag = 0xe382250d2739e109,
		sig_accumulate_interleaved_dispatch_layers = 0x9e0ab02648d6dec2,
		sig_process_sockets = 0x3a655f43b624ad97,
		sig_sequence_hash_find_hash = 0xfd495a1c1331db3d,
		sig_activity_mapping_reinitialize = 0x6a268ae9fdd681dc,
		sig_find_mapping = 0x79ff80b300ad6edb,
		sig_index_model_sequences = 0x5702b8e62c301614,
		sig_get_seq_desc = 0x37c02d1de6599d32,
		sig_keyvalues_init = 0xdcb56c67c29b0bc6,
		sig_ret_is_box_visible = 0x20cb21764825de8d,
		sig_random_int = 0x6485da38febb6def,
		sig_random_float = 0x9f28ebb140ec6b54,
		sig_random_seed = 0xa9d0ca2a2018206e,
		sig_var_mapping = 0x23f8ce10d236b32c,
		sig_free_thread_id = 0x368a70267dc2e07a,
		sig_tls_slots = 0xb7ff556dffc6bd0d,
		sig_projectile_throwtime = 0x271b6c0ead7c5223,
		sig_init_materials = 0x4a5eccceeb9450d3,
		sig_render_box = 0xe5e5361c3aed2bab,
		sig_render_triangle = 0x4084b1f5af766864,
		sig_vertex_color_ignorez = 0x421ad52f24202e67,
		sig_cycle_offset = 0x99e1e9f60b24c822,
		sig_studio_hdr = 0x563bb31969029fcb,
		sig_anim_layers = 0x9665292cb4111d9a,
		sig_is_ragdoll = 0x70a6503ef295cb76,
		sig_bone_accessor = 0xeb3755e6bad933a2,
		sig_cl_move_rest = 0xf41f919564b5ec2c,
		sig_post_build_transformations = 0xa5d317be84988d7d,
		sig_ret_to_post_build_transformations = 0xf7e51cb552fd6c89,
		sig_collision_viewheight = 0xae612a2a483a6d6b,
		sig_collision_bounds_change_time = 0x6c4d16976aff155d,
		sig_velocity_patch = 0xcf3a383181847d10,
		sig_kv_find_key = 0x4f9d628db60e3ee9,
		sig_kv_set_string = 0xa4557c286017c4bb,
		sig_surface_props = 0xffab6bcc261e87d9,
		sig_animstate_update_setabsagles = 0x54fadacf51da2153,
		sig_mdl_create = 0x9789a04ac66bf9e7,
		sig_set_merged_mdl = 0x46f1e57a84d3bd9f,
		sig_setup_bones_attachment_queries = 0x2ef716a844a6b2d,
		sig_disable_render_target_allocation = 0xad19d20fdcd8e729,
		sig_get_sequence_flags = 0x5a8a09edb5f04c07,
		sig_get_iron_sight_controller = 0x7ebc53443bd4a483,
		sig_enable_invalidate_bone_cache = 0x85f4cad4a73163ae,
		sig_line_goes_through_smoke = 0xab4ea08fa80ef9d3,
		sig_keyvalues_from_netsmg = 0x4d6c9d40d263e493,
		sig_model_bone_counter = 0x14e6506ffbb13745,
		sig_most_recent_model_bone_counter = 0x1cdbca2d35d66384,
		sig_accumulated_bone_mask = 0x7a06f8075c3fc5d9,
		sig_ret_hitbox_to_world_transforms = 0xd944c1fbf9f30816,
		sig_emit_sound = 0xdf27340904c8e291,
		sig_add_to_dirty_kd_tree = 0xa3ed8cb48150e850,
		sig_get_player_viewmodel_arm_config_for_player_model = 0xeaf555c71684af44,
		sig_client_side_addons_mask = 0xff4c0d4bbe52728c,
		sig_assassination_target_addon = 0x4289306533a1de03,
		sig_update_addon_models = 0xd9be6e5829728741,
		sig_msg_voicedata_constructor = 0xda3234919a1013cd,
		sig_parse_svg = 0x704eef6dd19fbea6,
		sig_construct_image = 0x705303633604c8a3,
		sig_load_text_file = 0xe2623217e738ec69,
		sig_econ_item_system = 0xa57ad877c88f3161,
		sig_clear_hud_weapon_icon = 0x907c4b8b72ef8bb2,
		sig_skinchanger_spam_patch = 0xaee4871f8a147d5d,
		sig_cvar_vac_check = 0x5aedb57129182c8d,
		sig_v8_try_catch_ctor = 0x56d1d7490683de7a,
		sig_v8_try_catch_dtor = 0x1b384287969370cb,
		sig_v8_handle_scope_ctor = 0x6c0ada14a39d025f,
		sig_v8_handle_scope_dtor = 0x5bdb2735302116ec,
		sig_v8_handle_scope_create_handle = 0xddf66335dc5781cf,
		sig_v8_context_enter = 0x9b878b804e6fcf0d,
		sig_v8_context_exit = 0xdcefdd2ce5eb9aca,
		sig_v8_isolate_enter = 0x45f80fe2ea51825c,
		sig_v8_isolate_exit = 0x6d19cc3d12b67d2f,
		sig_v8_array_get = 0xd2f263419dcacbb8,
		sig_v8_array_length = 0xa703567802d35bd7,
		sig_v8_object_get = 0x7fc0da138aeb5e5a,
		sig_v8_object_get_property_names = 0xe90245bda328b2a6,
		sig_v8_value_is_boolean = 0xba1a8d484b43d3b9,
		sig_v8_value_is_boolean_object = 0xd5572bd15538c247,
		sig_v8_value_is_number = 0xc177fa82dfc331b7,
		sig_v8_value_is_number_object = 0xc696d67e09ae4515,
		sig_v8_value_is_string = 0xfb7c81fa6df31504,
		sig_v8_value_is_string_object = 0xa766d1be1d2070c4,
		sig_v8_value_is_object = 0xdb10af337a2747c7,
		sig_v8_value_is_array = 0xae8c9cb1f30ad02c,
		sig_v8_value_is_function = 0x320bdf6992cd2ec7,
		sig_v8_value_boolean_value = 0x7e415ee1617a26d6,
		sig_v8_value_number_value = 0xdc4039f8770af745,
		sig_v8_value_to_object = 0x8276c83f41ec807,
		sig_v8_string_utf8_ctor = 0xfad7d6eff6a35a69,
		sig_v8_string_utf8_dtor = 0x15cc746a7d3ef18,
		sig_ui_engine_get_panel_context = 0x4c29beb59a98e3b7,
		sig_ui_engine_compile = 0x22ba178ee05ccf46,
		sig_ui_engine_run_compiled_script = 0xd5dbbeed58e45686,
		hook_start_sound_immediate = 0x990dab3806027c7a,
		hook_set_bodygroup = 0xcfc09f1182194d97,
		hook_modify_eye_pos = 0x491badae37a51539,
		hook_wnd_proc = 0x4ef9c358397a889c,
		hook_create_move = 0xcc6f95dfa70625ea,
		hook_cl_move = 0xc63d7273a28faf9f,
		hook_frame_stage_notify = 0x6a7b1934c5b5c21e,
		hook_on_latch_interp_var = 0xa90f0072debe2a89,
		hook_draw_model_execute = 0x9a3bc7b96d14f333,
		hook_render_view = 0x25f1228ddc2f1e07,
		hook_level_init_post_entity = 0x280c26b83961ba26,
		hook_level_init_pre_entity = 0x729295c7377eb207,
		hook_get_tonemap_settings_from_env_tonemap_controller = 0x3e932b84a97ef6c7,
		hook_draw_3d_debug_overlays = 0x24070b2803d431fe,
		hook_shader_api_draw_mesh = 0x77ee3a2f8e795525,
		hook_is_box_visible = 0xec50c1681c9aa2ea,
		hook_scene_end = 0x8ae102424f9e5582,
		hook_calc_renderable_world_space_aabb_bloated = 0xab3a239ae85e91a9,
		hook_add_renderables_to_render_lists = 0x90b230caba9aaa11,
		hook_do_post_screen_space_effects = 0x259621dbf366a0e9,
		hook_present = 0x32811d6f6273efc8,
		hook_reset = 0x870f7d4c53e54974,
		hook_send_net_msg = 0xf6b854664fcfb742,
		hook_override_view = 0xd381297748e74142,
		hook_paint_traverse = 0xf041529e5c99b25c,
		hook_find_material = 0xafce1a76451c3eaa,
		hook_emit_sound = 0x4218a1c6e65d4bf,
		hook_is_connected = 0xc79ed9aa925c7346,
		hook_lock_cursor = 0xfdbb17cf96fa92fa,
		hook_draw_set_color = 0xf375acdadc1aab56,
		hook_test_hitboxes = 0xc9a81a4de6e38d34,
		hook_setup_bones = 0x3586d0b0d52987ba,
		hook_animating_setup_bones = 0x3d79f16247eb1209,
		hook_post_data_update = 0x4a0670488e5d711d,
		hook_maintain_sequence_transitions = 0x6141e72da1f1a21c,
		hook_trace_ray = 0x46bb5189baab0927,
		hook_run_command = 0x7145f23f9b17caa2,
		hook_process_movement = 0xfefadab07504ee3c,
		hook_transfer_data = 0xde8067607dd7df6,
		hook_perform_prediction = 0x6bf91d343bc1684d,
		hook_packet_start = 0xdbdac78d4924460a,
		hook_level_shutdown = 0x77d6660ced6d82d9,
		hook_physics_simulate = 0x3ece973107939ebe,
		hook_estimate_abs_velocity = 0xf89f58d409182b53,
		hook_interpolate = 0x4ecc6f775ed1edc4,
		hook_reset_latched = 0x8e0961469f245ad3,
		hook_send_weapon_anim = 0x61b4addf221f947b,
		hook_fire_event = 0xbbbd323e2cf85ff5,
		hook_do_animation_events = 0xfc32d9f3ed9c6198,
		hook_deploy = 0x5614288d932fb445,
		hook_get_fov = 0x8c6c10fa79ebe735,
		hook_effects = 0x26c2c48fa6d46f85,
		hook_sequence = 0xda9e4d6e9ec54131,
		hook_weapon_handle = 0x6a01b6129cba2c58,
		hook_simulation_time = 0x3097c47a58ade16a,
		hook_general_float = 0xe8b3d47d5664939c,
		hook_general_vec = 0x9ae687335e964443,
		hook_general_int = 0x1cb662d9fe6f076d,
		hook_general_bool = 0x18fcd8de56d08083,
		hook_layer_sequence = 0x66b9b8cf938da06b,
		hook_layer_cycle = 0x8e333828d85eadd,
		hook_layer_playback_rate = 0x6bb56564974291c3,
		hook_layer_weight = 0xacfc09ad04e3176d,
		hook_layer_weight_delta_rate = 0xa39a870d082346cb,
		hook_layer_order = 0xdddec391b0fc5c2e,
		hook_convar_get_float_engine = 0x850b6660fa7f09a4,
		hook_convar_get_int_engine = 0x1129dabf8b5c28b2,
		hook_convar_get_int_client = 0x900f0dc4b34deb75,
		hook_eye_position_and_vectors = 0x10542e169aa9272c,
		hook_obb_change_callback = 0x8a574e4906061acb,
		hook_server_cmd_key_values = 0x87e429adabb746e0,
		hook_calc_view = 0xe19616d2eada59e7,
		hook_scale_mouse = 0x1f6a285a54e7df3,
		hook_convar_network_change_callback = 0x8a9ce871608f797c,
		hook_reevauluate_anim_lod = 0xfbf6d5115fc9dbce,
		hook_start_sound = 0xce6828b39d2870dc,
		hook_update_clientside_anim = 0x9c162a2fe2c83b5e,
		hook_do_animation_events_animating = 0x90c4ca2cb57f11b2,
		hook_get_default_fov = 0xeae8a53a690f971c,
		hook_ent_info_list_link_before = 0xc0d164ce2236a0f1,
		hook_extract_occluded_renderables = 0x2658ec388f9414dd,
		hook_sv_msg_voicedata = 0x44212512156f7ba8,
		hook_hud_reticle_process_input = 0x5af1ebd6d40f92d3,
		hook_update_interp_vars = 0x52ffa0481bd63e1d,
		hook_particle_draw_model = 0x3bb4ced54f25aebf,
		CAI_BaseNPC_m_bFadeCorpse = 0xf77050cca345c828,
		CAI_BaseNPC_m_bImportanRagdoll = 0x8723379f1bef0241,
		CAI_BaseNPC_m_bIsMoving = 0xb4f7c16cc82f9e7f,
		CAI_BaseNPC_m_bPerformAvoidance = 0x389bc95fc9a04acc,
		CAI_BaseNPC_m_bSpeedModActive = 0x896e88bf55d54264,
		CAI_BaseNPC_m_flTimePingEffect = 0xbc88e6eef48888e4,
		CAI_BaseNPC_m_iDeathFrame = 0x555c32e0aeea3db2,
		CAI_BaseNPC_m_iDeathPose = 0x2fc25031858d83b0,
		CAI_BaseNPC_m_iSpeedModRadius = 0xc53914369bf5e2de,
		CAI_BaseNPC_m_iSpeedModSpeed = 0xdc59a100dece6085,
		CAI_BaseNPC_m_lifeState = 0x5582ffc5259ce3eb,
		CBRC4Target_m_bBrokenOpen = 0xae5afe64c3cc0df0,
		CBRC4Target_m_flRadius = 0x69bbb988015cbc,
		CBaseAnimating_m_ScaleType = 0x23f650aaeeedfc2f,
		CBaseAnimating_m_bClientSideAnimation = 0x175b5fb4201cddaa,
		CBaseAnimating_m_bClientSideFrameReset = 0x4f595a921b70ee71,
		CBaseAnimating_m_bClientSideRagdoll = 0xa81056fba2cad05a,
		CBaseAnimating_m_bSuppressAnimSounds = 0x946459931de1a2a9,
		CBaseAnimating_m_flCycle = 0x3a83aaf49b79b937,
		CBaseAnimating_m_flEncodedController = 0x450158f6d83180f5,
		CBaseAnimating_m_flFrozen = 0x49143d8cfedcde91,
		CBaseAnimating_m_flModelScale = 0xbf3fca6e9b6538b5,
		CBaseAnimating_m_flPlaybackRate = 0x8bf67f1518c5ac5e,
		CBaseAnimating_m_flPoseParameter = 0x133b5ef3ad67bb93,
		CBaseAnimating_m_hLightingOrigin = 0xcb0a6b8262b47a09,
		CBaseAnimating_m_nBody = 0x334ac86d43a265d4,
		CBaseAnimating_m_nForceBone = 0xd01c30af9ff79359,
		CBaseAnimating_m_nHighlightColorB = 0x7d5211a6a70b5922,
		CBaseAnimating_m_nHighlightColorG = 0x423e4159d443e13e,
		CBaseAnimating_m_nHighlightColorR = 0x766c5bee7ba1295b,
		CBaseAnimating_m_nHitboxSet = 0xfc77f9c51643a85d,
		CBaseAnimating_m_nMuzzleFlashParity = 0xdfcfc06c2db6813,
		CBaseAnimating_m_nNewSequenceParity = 0xc61ed00ecddd31eb,
		CBaseAnimating_m_nResetEventsParity = 0x12827695d0d530ab,
		CBaseAnimating_m_nSequence = 0x63ba532801eb8fd6,
		CBaseAnimating_m_nSkin = 0x49dc7f22512faf37,
		CBaseAnimating_m_vecForce = 0x79f0706d6d96e782,
		CBaseAttributableItem_m_AttributeManager = 0x9757c05b701fe87a,
		CBaseAttributableItem_m_Attributes = 0x911574dca17d5867,
		CBaseAttributableItem_m_Item = 0x35775e41d009c5b3,
		CBaseAttributableItem_m_NetworkedDynamicAttributesForDemos = 0x4f358ac73c9cb39f,
		CBaseAttributableItem_m_OriginalOwnerXuidHigh = 0xcf1a2774a0a3f5c,
		CBaseAttributableItem_m_OriginalOwnerXuidLow = 0x50a467043a8fbedf,
		CBaseAttributableItem_m_ProviderType = 0xd1084cfb1f2e30a4,
		CBaseAttributableItem_m_bInitialized = 0x293554f826663266,
		CBaseAttributableItem_m_flFallbackWear = 0x759b8dfd918516c7,
		CBaseAttributableItem_m_hOuter = 0x401c72af6d22fa21,
		CBaseAttributableItem_m_iAccountID = 0xd1375ef01226791f,
		CBaseAttributableItem_m_iEntityLevel = 0x9ed280999b8b9732,
		CBaseAttributableItem_m_iEntityQuality = 0xb3ef8687bed6c0d6,
		CBaseAttributableItem_m_iItemDefinitionIndex = 0x39737ba90c6ffafb,
		CBaseAttributableItem_m_iItemIDHigh = 0xb6a368e1bb291876,
		CBaseAttributableItem_m_iItemIDLow = 0x36c3f0b2998fa8d6,
		CBaseAttributableItem_m_iReapplyProvisionParity = 0x64a3b77ed9c84e13,
		CBaseAttributableItem_m_nFallbackPaintKit = 0xd3127181fa6dba1,
		CBaseAttributableItem_m_nFallbackSeed = 0x56893511d77050e1,
		CBaseAttributableItem_m_nFallbackStatTrak = 0xce33f719a398ea8e,
		CBaseAttributableItem_m_szCustomName = 0xca7383d3ecbef3a1,
		CBaseButton_m_usable = 0x69fcb813e0c17aa,
		CBaseCSGrenadeProjectile_m_nBounces = 0x6166ae5f5d4177ef,
		CBaseCSGrenadeProjectile_m_nExplodeEffectIndex = 0xa936a6447e2fd545,
		CBaseCSGrenadeProjectile_m_nExplodeEffectTickBegin = 0x251e6c1a8c0c0afd,
		CBaseCSGrenadeProjectile_m_vInitialVelocity = 0x1bf6a9c5b0e83cbd,
		CBaseCSGrenadeProjectile_m_vecExplodeEffectOrigin = 0xa41688537b3f608,
		CBaseCSGrenade_m_bIsHeldByPlayer = 0x7b1d6247b66d9b7e,
		CBaseCSGrenade_m_bLoopingSoundPlaying = 0x1d381a4905531434,
		CBaseCSGrenade_m_bPinPulled = 0x2e9f53b70d3b59e,
		CBaseCSGrenade_m_bRedraw = 0x59e6543776752df7,
		CBaseCSGrenade_m_fDropTime = 0x3f6e0595edf0a1aa,
		CBaseCSGrenade_m_fThrowTime = 0xf3930d815054670e,
		CBaseCSGrenade_m_flThrowStrength = 0x746b5d2723eb75a7,
		CBaseCombatCharacter_m_LastHitGroup = 0xd3874c4b3f4cf790,
		CBaseCombatCharacter_m_flNextAttack = 0x8fc26f042a772f46,
		CBaseCombatCharacter_m_flTimeOfLastInjury = 0x83fed75707f2923f,
		CBaseCombatCharacter_m_hActiveWeapon = 0x3cb50a23c9b0107a,
		CBaseCombatCharacter_m_hMyWeapons = 0xf837a95bb48a4926,
		CBaseCombatCharacter_m_hMyWearables = 0x37e2232ab8e2b5e8,
		CBaseCombatCharacter_m_nRelativeDirectionOfLastInjury = 0xf084433326724f66,
		CBaseCombatWeapon_m_bFlipViewModel = 0xb6a32b1c424f09,
		CBaseCombatWeapon_m_flNextPrimaryAttack = 0xbb68e315fe78cba7,
		CBaseCombatWeapon_m_flNextSecondaryAttack = 0x195b08de18ea1379,
		CBaseCombatWeapon_m_flTimeWeaponIdle = 0x7a0fd609e962bf77,
		CBaseCombatWeapon_m_hOwner = 0x96c360b4ee66adc7,
		CBaseCombatWeapon_m_hWeaponWorldModel = 0xdac67002c46cf975,
		CBaseCombatWeapon_m_iClip1 = 0x9f98fb7cb3e02ba4,
		CBaseCombatWeapon_m_iClip2 = 0xe4764fd6f58eea36,
		CBaseCombatWeapon_m_iNumEmptyAttacks = 0x4b7569d89973153d,
		CBaseCombatWeapon_m_iPrimaryAmmoType = 0x2c2fcf5d3ffd0582,
		CBaseCombatWeapon_m_iPrimaryReserveAmmoCount = 0x743ce9732bcad629,
		CBaseCombatWeapon_m_iSecondaryAmmoType = 0xc0cd041a83b19bc,
		CBaseCombatWeapon_m_iSecondaryReserveAmmoCount = 0x9d5351ab844833e,
		CBaseCombatWeapon_m_iState = 0x6d677b0f8800a105,
		CBaseCombatWeapon_m_iViewModelIndex = 0x59621973f6702981,
		CBaseCombatWeapon_m_iWeaponModule = 0x78faf97bbd843060,
		CBaseCombatWeapon_m_iWeaponOrigin = 0x25b9c56b5f86ce26,
		CBaseCombatWeapon_m_iWorldDroppedModelIndex = 0x7c9d2b63fde8abfc,
		CBaseCombatWeapon_m_iWorldModelIndex = 0xee9fd928ebdac2d,
		CBaseCombatWeapon_m_nNextThinkTick = 0x370253587f715a68,
		CBaseCombatWeapon_m_nViewModelIndex = 0x8d39ba1a83aaf392,
		CBaseDoor_m_flWaveHeight = 0x1a9422c541ca6886,
		CBaseEntity_m_Collision = 0xa3ba2cb19bc88d7e,
		CBaseEntity_m_CollisionGroup = 0xa57f8d342589dc29,
		CBaseEntity_m_angRotation = 0xe253a0d03a1624e2,
		CBaseEntity_m_bAlternateSorting = 0xae3e66a7caa47f84,
		CBaseEntity_m_bAnimatedEveryTick = 0xa034cba57ad311b0,
		CBaseEntity_m_bEligibleForScreenHighlight = 0x648a9c7c70abbbf5,
		CBaseEntity_m_bIsAutoaimTarget = 0xc0bbe0f2ac19530d,
		CBaseEntity_m_bSimulatedEveryTick = 0x773b0864ff994e0c,
		CBaseEntity_m_bSpotted = 0x7f7602eab09c20c2,
		CBaseEntity_m_bSpottedBy = 0x98dc70d88e18263c,
		CBaseEntity_m_bSpottedByMask = 0x1fe759e578ab6ebd,
		CBaseEntity_m_cellX = 0xd4edf0a0d62998b7,
		CBaseEntity_m_cellY = 0x6784a99d68b59160,
		CBaseEntity_m_cellZ = 0x922b7d412d73d85a,
		CBaseEntity_m_cellbits = 0x265e731c275be213,
		CBaseEntity_m_clrRender = 0x6774ae4f7a4d3aa7,
		CBaseEntity_m_fEffects = 0x85c87e1f09c7c2ad,
		CBaseEntity_m_fadeMaxDist = 0xc6432b4a830aa1cb,
		CBaseEntity_m_fadeMinDist = 0xa5394c4b0f4714d7,
		CBaseEntity_m_flAnimTime = 0x8f4d441535903c7,
		CBaseEntity_m_flElasticity = 0xbd9feac2a71e4552,
		CBaseEntity_m_flFadeScale = 0x3f5162c32e253c53,
		CBaseEntity_m_flLastMadeNoiseTime = 0xda62d7729bcde6cd,
		CBaseEntity_m_flMaxFallVelocity = 0xaf84dcd437ba240e,
		CBaseEntity_m_flShadowCastDistance = 0x6fb6a4c986e73f13,
		CBaseEntity_m_flSimulationTime = 0xf9408698b85dc39e,
		CBaseEntity_m_flUseLookAtAngle = 0xdc5ab6e5912be6fd,
		CBaseEntity_m_hEffectEntity = 0xd31a3d49b4994b69,
		CBaseEntity_m_hOwnerEntity = 0x2ebc9579393608c6,
		CBaseEntity_m_iName = 0x228139a8742756e9,
		CBaseEntity_m_iParentAttachment = 0xdc3525a5e2a03337,
		CBaseEntity_m_iPendingTeamNum = 0x1feb258a7705a544,
		CBaseEntity_m_iTeamNum = 0xece7b0944f199380,
		CBaseEntity_m_iTextureFrameIndex = 0xa3b8178d964a5813,
		CBaseEntity_m_nMaxCPULevel = 0x7170fceed36a40eb,
		CBaseEntity_m_nMaxGPULevel = 0x683f2ee9df61bc7e,
		CBaseEntity_m_nMinCPULevel = 0x4c64f82da64e77c6,
		CBaseEntity_m_nMinGPULevel = 0x57369d659e49f3d0,
		CBaseEntity_m_nModelIndex = 0x54325bd637d27594,
		CBaseEntity_m_nRenderFX = 0x45cf59c79f7f959d,
		CBaseEntity_m_nRenderMode = 0xf1ec832155c6b8e2,
		CBaseEntity_m_nSolidType = 0xe25dd0390236843c,
		CBaseEntity_m_nSurroundType = 0x32d510d55f44171d,
		CBaseEntity_m_triggerBloat = 0xc6f1e81cffd96743,
		CBaseEntity_m_usSolidFlags = 0xdcf8fba14903917e,
		CBaseEntity_m_vecMaxs = 0xfe5474422492f78a,
		CBaseEntity_m_vecMins = 0x407607d9c33369d7,
		CBaseEntity_m_vecOrigin = 0x6ab3bfccb7fdd5ff,
		CBaseEntity_m_vecSpecifiedSurroundingMaxs = 0xa3027aea24c92241,
		CBaseEntity_m_vecSpecifiedSurroundingMins = 0xee4c431d69b292a5,
		CBaseEntity_moveparent = 0x4da303540b344168,
		CBaseFlex_m_blinktoggle = 0xa36975853e2bad62,
		CBaseFlex_m_flexWeight = 0x27f57bdee2e1765f,
		CBaseFlex_m_viewtarget = 0x3826a92f485a41dd,
		CBaseGrenade_m_DmgRadius = 0xa8c5d463fac2bbbe,
		CBaseGrenade_m_bIsLive = 0x14adf02022a46f7d,
		CBaseGrenade_m_fFlags = 0xb81cf00c758e8067,
		CBaseGrenade_m_flDamage = 0x6826ff76802f2323,
		CBaseGrenade_m_hThrower = 0x6cf4330597ee2588,
		CBaseGrenade_m_vecVelocity = 0x7d4d7191b9610275,
		CBasePlayer_deadflag = 0xfa77f70f60aa18cc,
		CBasePlayer_m_Local = 0xebd83efd644f1ebc,
		CBasePlayer_m_PlayerFog_m_hCtrl = 0x765c3e9be2b6b322,
		CBasePlayer_m_afPhysicsFlags = 0xf07e3040310dfb0f,
		CBasePlayer_m_aimPunchAngle = 0x653b5dd6d19586f,
		CBasePlayer_m_aimPunchAngleVel = 0x168988176d651f87,
		CBasePlayer_m_audio_entIndex = 0x44f4d15eb8b85d1a,
		CBasePlayer_m_audio_localBits = 0x99aa31d909418606,
		CBasePlayer_m_audio_localSound_0_ = 0xdd8024843159045f,
		CBasePlayer_m_audio_localSound_1_ = 0x97430310fc2143d9,
		CBasePlayer_m_audio_localSound_2_ = 0x1730cd65bb96e6ac,
		CBasePlayer_m_audio_localSound_3_ = 0x9d958c76ee32077f,
		CBasePlayer_m_audio_localSound_4_ = 0xe7fdb66ef3d059ba,
		CBasePlayer_m_audio_localSound_5_ = 0x6f2c3c184d5941a,
		CBasePlayer_m_audio_localSound_6_ = 0x238243bbfb6dcf09,
		CBasePlayer_m_audio_localSound_7_ = 0xcce30978e403d2cf,
		CBasePlayer_m_audio_soundscapeIndex = 0xa42157546db1cac4,
		CBasePlayer_m_bActiveCameraMan = 0xf19e3a1a5e62c6b9,
		CBasePlayer_m_bAllowAutoMovement = 0x79e0ec34391b6cc4,
		CBasePlayer_m_bCameraManOverview = 0xdc57a72ed929b556,
		CBasePlayer_m_bCameraManScoreBoard = 0xad6a03d8534579c3,
		CBasePlayer_m_bCameraManXRay = 0x998306a393154514,
		CBasePlayer_m_bConstraintPastRadius = 0x9e7f5f88ef3ac97b,
		CBasePlayer_m_bDrawViewmodel = 0x25659f6dd87fbb03,
		CBasePlayer_m_bDucked = 0xe5e9268ea59d7efb,
		CBasePlayer_m_bDucking = 0xa84266fda7eae536,
		CBasePlayer_m_bInDuckJump = 0xd8058a0cd8913f89,
		CBasePlayer_m_bPoisoned = 0x80ce1626d4ab24da,
		CBasePlayer_m_bShouldDrawPlayerWhileUsingViewEntity = 0xe137a49ae9204ed2,
		CBasePlayer_m_bWearingSuit = 0xe8d187445db06973,
		CBasePlayer_m_chAreaBits = 0xedffeea2b5c19713,
		CBasePlayer_m_chAreaPortalBits = 0xd581eaa65f51d5c9,
		CBasePlayer_m_fFlags = 0xee7d3aa66d3cddc7,
		CBasePlayer_m_fForceTeam = 0x7f2b38a5796aef2e,
		CBasePlayer_m_fOnTarget = 0xeb3c22c5e57cb4ce,
		CBasePlayer_m_flConstraintRadius = 0xbabba774f6f5fb37,
		CBasePlayer_m_flConstraintSpeedFactor = 0x767a501ee1a053d2,
		CBasePlayer_m_flConstraintWidth = 0x828c083948579e99,
		CBasePlayer_m_flDeathTime = 0x1824ccb7ec3fca7e,
		CBasePlayer_m_flDuckAmount = 0x5bf0ee44492acdae,
		CBasePlayer_m_flDuckSpeed = 0xce81b3df3e259519,
		CBasePlayer_m_flFOVRate = 0x277a3f0fb27e666e,
		CBasePlayer_m_flFOVTime = 0xeddc9d81751f8719,
		CBasePlayer_m_flFallVelocity = 0xffc91488c43f127e,
		CBasePlayer_m_flFriction = 0x4d573cf82a956cc8,
		CBasePlayer_m_flLaggedMovementValue = 0x70c34fa086a5eb00,
		CBasePlayer_m_flLastDuckTime = 0xc3b3fdc59a753dd4,
		CBasePlayer_m_flMaxspeed = 0xdb4a410e9922304b,
		CBasePlayer_m_flNextDecalTime = 0x5fa59b2ff91e8e31,
		CBasePlayer_m_flStepSize = 0x2066aa9a47509e83,
		CBasePlayer_m_hColorCorrectionCtrl = 0xf230dea5efa76a93,
		CBasePlayer_m_hConstraintEntity = 0xe93eeff356801ee7,
		CBasePlayer_m_hGroundEntity = 0x33742923b236d9a2,
		CBasePlayer_m_hLastWeapon = 0x7b3c97fc3e156bd9,
		CBasePlayer_m_hObserverTarget = 0x9119fc96090c793,
		CBasePlayer_m_hPostProcessCtrl = 0x51299fa45b51789e,
		CBasePlayer_m_hTonemapController = 0x8b5484a9adacfc9c,
		CBasePlayer_m_hUseEntity = 0x6c846e686cdc7d53,
		CBasePlayer_m_hVehicle = 0x96cb57724b52381e,
		CBasePlayer_m_hViewEntity = 0x6d61e6cd8c5d4673,
		CBasePlayer_m_hViewModel_0_ = 0xcc0167e8d34b8a9f,
		CBasePlayer_m_hZoomOwner = 0x7a64c50fa5cef81d,
		CBasePlayer_m_iAmmo = 0x5d06d8be01f39b1f,
		CBasePlayer_m_iBonusChallenge = 0x723f90339b13fec1,
		CBasePlayer_m_iBonusProgress = 0x9ecb967753a2711c,
		CBasePlayer_m_iCoachingTeam = 0x632f200afd68e3c7,
		CBasePlayer_m_iDeathPostEffect = 0xa6c0d949698038fb,
		CBasePlayer_m_iDefaultFOV = 0xdc5be8568bd68b85,
		CBasePlayer_m_iFOV = 0xf297e45dfe9b9310,
		CBasePlayer_m_iFOVStart = 0x5809dc8ff227f4b7,
		CBasePlayer_m_iHealth = 0xf84e0f8a7c38841c,
		CBasePlayer_m_iHideHUD = 0x55274be2803567fc,
		CBasePlayer_m_iObserverMode = 0xc3ae2f9838635fe9,
		CBasePlayer_m_ladderSurfaceProps = 0x707649eb725ba869,
		CBasePlayer_m_lifeState = 0xb15af6a7a3f03994,
		CBasePlayer_m_nDuckJumpTimeMsecs = 0xb0cd24598c97914e,
		CBasePlayer_m_nDuckTimeMsecs = 0x4b202c41804f482e,
		CBasePlayer_m_nJumpTimeMsecs = 0xd877d47f79d2cc6f,
		CBasePlayer_m_nNextThinkTick = 0x1eb0283162608875,
		CBasePlayer_m_nOldButtons = 0x60a0dbc86038712,
		CBasePlayer_m_nTickBase = 0x4b766eb783a044e4,
		CBasePlayer_m_nWaterLevel = 0x82c464379a76051c,
		CBasePlayer_m_skybox3d_area = 0x23fa34aa78bde309,
		CBasePlayer_m_skybox3d_fog_HDRColorScale = 0x86c21e6ada8756c8,
		CBasePlayer_m_skybox3d_fog_blend = 0x33204ef40750e108,
		CBasePlayer_m_skybox3d_fog_colorPrimary = 0x330833a664603de5,
		CBasePlayer_m_skybox3d_fog_colorSecondary = 0x78114ad44d592b8f,
		CBasePlayer_m_skybox3d_fog_dirPrimary = 0x3da8274163583d34,
		CBasePlayer_m_skybox3d_fog_enable = 0x4b191cdc290ce6ac,
		CBasePlayer_m_skybox3d_fog_end = 0x1b1ffade9088e92c,
		CBasePlayer_m_skybox3d_fog_maxdensity = 0x2f4c73566511228e,
		CBasePlayer_m_skybox3d_fog_start = 0x503619cb43c404c1,
		CBasePlayer_m_skybox3d_origin = 0x2f573be63f49df9d,
		CBasePlayer_m_skybox3d_scale = 0xb47f5a92bc4a496d,
		CBasePlayer_m_szLastPlaceName = 0x9737fe794e163c1e,
		CBasePlayer_m_uCameraManGraphs = 0x2c3ced0ec289b89d,
		CBasePlayer_m_ubEFNoInterpParity = 0x15f553d0796b8917,
		CBasePlayer_m_vecBaseVelocity = 0x331713a6dd6b155d,
		CBasePlayer_m_vecConstraintCenter = 0xde9009c3e83ed7a0,
		CBasePlayer_m_vecLadderNormal = 0xb842a0bf1c4293c5,
		CBasePlayer_m_vecVelocity_0_ = 0xcca7009d49136512,
		CBasePlayer_m_vecVelocity_1_ = 0x84e1896e316647a1,
		CBasePlayer_m_vecVelocity_2_ = 0x553b21f48f5e158f,
		CBasePlayer_m_vecViewOffset_0_ = 0xe454c0529ad8282b,
		CBasePlayer_m_vecViewOffset_1_ = 0x4e16698845735a4d,
		CBasePlayer_m_vecViewOffset_2_ = 0xbc73739bcad72557,
		CBasePlayer_m_viewPunchAngle = 0x8d388a13ccb00a3d,
		CBasePlayer_m_vphysicsCollisionState = 0x956b7c5c60f4b8c,
		CBasePlayer_pl = 0xf5011799e20278f1,
		CBaseTeamObjectiveResource_m_bBlocked = 0xaebfce3f57cc1d8d,
		CBaseTeamObjectiveResource_m_bCPIsVisible = 0x86ff2bb99c547c4d,
		CBaseTeamObjectiveResource_m_bControlPointsReset = 0xcc3ab0d947b72190,
		CBaseTeamObjectiveResource_m_bInMiniRound = 0x982da19b3358ab70,
		CBaseTeamObjectiveResource_m_bPlayingMiniRounds = 0x4f24422988f1da41,
		CBaseTeamObjectiveResource_m_bTeamCanCap = 0xc763d1717d86b696,
		CBaseTeamObjectiveResource_m_flLazyCapPerc = 0x8384c4a752f8fb31,
		CBaseTeamObjectiveResource_m_flPathDistance = 0x98bc4c5a7f858d8a,
		CBaseTeamObjectiveResource_m_flTeamCapTime = 0xf373e4126bb8164e,
		CBaseTeamObjectiveResource_m_iBaseControlPoints = 0xbb895bc28e17f4b8,
		CBaseTeamObjectiveResource_m_iCappingTeam = 0xf64b5aff00ea86fb,
		CBaseTeamObjectiveResource_m_iNumControlPoints = 0x5bffcfe2afd55d90,
		CBaseTeamObjectiveResource_m_iNumTeamMembers = 0xd6525fa32cd3092d,
		CBaseTeamObjectiveResource_m_iOwner = 0xf09eed5bf824d372,
		CBaseTeamObjectiveResource_m_iPreviousPoints = 0x83d284d7412934e7,
		CBaseTeamObjectiveResource_m_iStopWatchTimer = 0x32d0222a1017610b,
		CBaseTeamObjectiveResource_m_iTeamBaseIcons = 0x1538871058e8c78a,
		CBaseTeamObjectiveResource_m_iTeamIcons = 0x376c75690ab76ee5,
		CBaseTeamObjectiveResource_m_iTeamInZone = 0x679f6b11bb73770d,
		CBaseTeamObjectiveResource_m_iTeamOverlays = 0xf165898057fd371f,
		CBaseTeamObjectiveResource_m_iTeamReqCappers = 0xca6ad2219770134a,
		CBaseTeamObjectiveResource_m_iTimerToShowInHUD = 0x707eddb01dfb5475,
		CBaseTeamObjectiveResource_m_iUpdateCapHudParity = 0x13b8fb83c9494da7,
		CBaseTeamObjectiveResource_m_iWarnOnCap = 0x3db563037afc409a,
		CBaseTeamObjectiveResource_m_iszWarnSound_0_ = 0xb9103d57f757d795,
		CBaseTeamObjectiveResource_m_pszCapLayoutInHUD = 0xc03edd5dda62f948,
		CBaseTeamObjectiveResource_m_vCPPositions_0_ = 0xe54f9113d622100,
		CBaseToggle_m_flMoveTargetTime = 0x859964a034632cac,
		CBaseToggle_m_movementType = 0x9ca047273794e8d3,
		CBaseToggle_m_vecFinalDest = 0xb8462a8c2c803ee0,
		CBaseTrigger_m_bClientSidePredicted = 0x51081206395a6748,
		CBaseTrigger_m_spawnflags = 0x70bf313cb33665ff,
		CBaseViewModel_m_bShouldIgnoreOffsetAndAccuracy = 0xb3e51823cfdbac8f,
		CBaseViewModel_m_fEffects = 0x10895936bb440f99,
		CBaseViewModel_m_flPlaybackRate = 0x61d93fa3192b208,
		CBaseViewModel_m_hOwner = 0x12b20f864dc2b539,
		CBaseViewModel_m_hWeapon = 0xc03d30b83185ff8e,
		CBaseViewModel_m_nAnimationParity = 0xcdd3f2dd48f51201,
		CBaseViewModel_m_nBody = 0x7b8bc9ccf9802459,
		CBaseViewModel_m_nModelIndex = 0x506f7c7ea0ae928c,
		CBaseViewModel_m_nMuzzleFlashParity = 0xa24dd3e542fa9bc8,
		CBaseViewModel_m_nNewSequenceParity = 0x7784b41befa428a6,
		CBaseViewModel_m_nResetEventsParity = 0x8c6e4ba8f9c3b0fa,
		CBaseViewModel_m_nSequence = 0x1eef361857a84df5,
		CBaseViewModel_m_nSkin = 0x7e5ab4e64872e83c,
		CBaseViewModel_m_nViewModelIndex = 0x23bacf6dee9ccf50,
		CBaseWeaponWorldModel_m_fEffects = 0x2e311731032fae07,
		CBaseWeaponWorldModel_m_hCombatWeaponParent = 0x6767c4bd87f66012,
		CBaseWeaponWorldModel_m_nBody = 0x6551e8a2b3c5ac23,
		CBaseWeaponWorldModel_m_nModelIndex = 0x565e4181aaf69696,
		CBaseWeaponWorldModel_moveparent = 0x8079e9969db45ae7,
		CBeamSpotlight_m_bHasDynamicLight = 0x436eaf173e4f746d,
		CBeamSpotlight_m_bSpotlightOn = 0x2dccf1d12dce988e,
		CBeamSpotlight_m_flHDRColorScale = 0x31a02260c743255,
		CBeamSpotlight_m_flRotationSpeed = 0xdb333b79c7a12fa6,
		CBeamSpotlight_m_flSpotlightGoalWidth = 0xe0f24f9a00364fd0,
		CBeamSpotlight_m_flSpotlightMaxLength = 0xbeccd2280ef17173,
		CBeamSpotlight_m_nHaloIndex = 0x22a76bdf3d8e00ac,
		CBeamSpotlight_m_nRotationAxis = 0x5c1677310fd54cf3,
		CBeam_m_clrRender = 0x27506f298f59cb39,
		CBeam_m_fAmplitude = 0xd141a331a0aea1e8,
		CBeam_m_fEndWidth = 0x239e69635a9bedae,
		CBeam_m_fFadeLength = 0xda5bbc668a61aab9,
		CBeam_m_fHaloScale = 0xa76ebbb7a16ee186,
		CBeam_m_fSpeed = 0x22335bc77c7bd591,
		CBeam_m_fStartFrame = 0x9212d96c34459448,
		CBeam_m_fWidth = 0x371e760959cec26b,
		CBeam_m_flFrame = 0xa723de2db6fc5f6e,
		CBeam_m_flFrameRate = 0x6e626d2852d20327,
		CBeam_m_flHDRColorScale = 0xf4c0fc87f8c74f02,
		CBeam_m_hAttachEntity = 0x50cd902c715a9dfd,
		CBeam_m_nAttachIndex = 0xaf2ebfeba46ed907,
		CBeam_m_nBeamFlags = 0x8c19f5dd5cd4b547,
		CBeam_m_nBeamType = 0x1b454b160945a757,
		CBeam_m_nClipStyle = 0x4b7bc01d69d6522f,
		CBeam_m_nHaloIndex = 0x7037d4b813f1454f,
		CBeam_m_nModelIndex = 0xf0dee0d3522844a8,
		CBeam_m_nNumBeamEnts = 0x8ddd40609b0929ae,
		CBeam_m_nRenderFX = 0xd84e9e1bab352b47,
		CBeam_m_nRenderMode = 0x58398eea3c476243,
		CBeam_m_vecEndPos = 0x2d172fb2599c074a,
		CBeam_m_vecOrigin = 0xb8a7eb8c029e652a,
		CBeam_moveparent = 0xa9ed5c943cd1a2b5,
		CBoneFollower_m_modelIndex = 0x9e56511e4d88696e,
		CBoneFollower_m_solidIndex = 0x32cee5e5f768accf,
		CBreachChargeProjectile_m_bShouldExplode = 0xde339be75682eb5f,
		CBreachChargeProjectile_m_nParentBoneIndex = 0xe1eb6484c3f999c7,
		CBreachChargeProjectile_m_vecParentBonePos = 0x7bda86fa630d4974,
		CBreachChargeProjectile_m_weaponThatThrewMe = 0xc71078fa76db131d,
		CBreakableProp_m_bClientPhysics = 0x3f8c97b724cb8161,
		CBreakableProp_m_qPreferredPlayerCarryAngles = 0xe858e350a2eaf5f9,
		CBreakableSurface_m_RawPanelBitVec = 0x2685b51f0a6b08ad,
		CBreakableSurface_m_bIsBroken = 0xca9c5502137fdee7,
		CBreakableSurface_m_flPanelHeight = 0xb1e7c9241d135b7a,
		CBreakableSurface_m_flPanelWidth = 0xeccde3b833b4c161,
		CBreakableSurface_m_nNumHigh = 0x6a6a54d86fafac9c,
		CBreakableSurface_m_nNumWide = 0x473e8df800a8e0fe,
		CBreakableSurface_m_nSurfaceType = 0x45cfdc769a821c6,
		CBreakableSurface_m_vCorner = 0xf04ff70caf147fdb,
		CBreakableSurface_m_vNormal = 0xe6e9de60aa1deea0,
		CBumpMineProjectile_m_bArmed = 0x7d317ebb217db18,
		CBumpMineProjectile_m_nParentBoneIndex = 0x2a78adeb3e3d86da,
		CBumpMineProjectile_m_vecParentBonePos = 0x778fbc6bd1a78a0b,
		CC4_m_bBombPlacedAnimation = 0xe715f1f44bd91ab8,
		CC4_m_bIsPlantingViaUse = 0x7f6f8f35a37a917,
		CC4_m_bShowC4LED = 0x6cf5203011f3aa7e,
		CC4_m_bStartedArming = 0xcda5a9c2e2ab1d0,
		CC4_m_fArmedTime = 0x108d3903f069c7c2,
		CCSGameRulesProxy_m_GGProgressiveWeaponKillUpgradeOrderCT = 0x923bb90b03be28b,
		CCSGameRulesProxy_m_GGProgressiveWeaponKillUpgradeOrderT = 0xb403dd13703dee0,
		CCSGameRulesProxy_m_GGProgressiveWeaponOrderCT = 0xd40bc961803d2b97,
		CCSGameRulesProxy_m_GGProgressiveWeaponOrderT = 0xac4b0334838a8d5d,
		CCSGameRulesProxy_m_MatchDevice = 0x2acf8fea6120ae88,
		CCSGameRulesProxy_m_RetakeRules = 0x77e5d4a736aab25c,
		CCSGameRulesProxy_m_SpawnTileState = 0xdd3100aad8366d70,
		CCSGameRulesProxy_m_SurvivalGameRuleDecisionTypes = 0xf7a865c612690f5f,
		CCSGameRulesProxy_m_SurvivalGameRuleDecisionValues = 0x5c627d3cced6ee23,
		CCSGameRulesProxy_m_SurvivalRules = 0xa0d05ee84ea68343,
		CCSGameRulesProxy_m_TeamRespawnWaveTimes = 0x1162929e37fdca,
		CCSGameRulesProxy_m_arrFeaturedGiftersAccounts = 0xe5f0a5122332c1f8,
		CCSGameRulesProxy_m_arrFeaturedGiftersGifts = 0x263c061e1f252ff3,
		CCSGameRulesProxy_m_arrProhibitedItemIndices = 0xf84f641cc0072ea8,
		CCSGameRulesProxy_m_arrTournamentActiveCasterAccounts = 0x758ccefd39ebc8bd,
		CCSGameRulesProxy_m_bAnyHostageReached = 0x7a904898731dd067,
		CCSGameRulesProxy_m_bBlockersPresent = 0x44779b574a89155b,
		CCSGameRulesProxy_m_bBombDropped = 0x554364a4f41ea743,
		CCSGameRulesProxy_m_bBombPlanted = 0x54e2dd9fec571433,
		CCSGameRulesProxy_m_bCTCantBuy = 0x865b4f20df6d51af,
		CCSGameRulesProxy_m_bCTTimeOutActive = 0x893da44475ad2c85,
		CCSGameRulesProxy_m_bDMBonusActive = 0x6d5f8c6b5f023065,
		CCSGameRulesProxy_m_bFreezePeriod = 0x91f022d64f918332,
		CCSGameRulesProxy_m_bGameRestart = 0x255ec0d06e4d40a7,
		CCSGameRulesProxy_m_bHasMatchStarted = 0x6ef8cc40be9aef96,
		CCSGameRulesProxy_m_bIsDroppingItems = 0x9cb027072705e641,
		CCSGameRulesProxy_m_bIsQuestEligible = 0xe715333be3e20914,
		CCSGameRulesProxy_m_bIsQueuedMatchmaking = 0x4e38a55f2aa9ab51,
		CCSGameRulesProxy_m_bIsValveDS = 0x1ec937ab213dbd96,
		CCSGameRulesProxy_m_bLogoMap = 0x730c31c0c471ccc4,
		CCSGameRulesProxy_m_bMapHasBombTarget = 0x1f1c829e60ffe6d1,
		CCSGameRulesProxy_m_bMapHasBuyZone = 0x5c64576158daa2a7,
		CCSGameRulesProxy_m_bMapHasRescueZone = 0x698fd3e9c5cb6650,
		CCSGameRulesProxy_m_bMatchWaitingForResume = 0xe39826cdd58974d4,
		CCSGameRulesProxy_m_bPlayAllStepSoundsOnServer = 0xebe3080b00f4695f,
		CCSGameRulesProxy_m_bRoundInProgress = 0xb8ad83a18964d2c6,
		CCSGameRulesProxy_m_bTCantBuy = 0x80c70aab79010b28,
		CCSGameRulesProxy_m_bTechnicalTimeOut = 0xc37e121ced2ae05a,
		CCSGameRulesProxy_m_bTerroristTimeOutActive = 0x8a06c140ecd165c7,
		CCSGameRulesProxy_m_bWarmupPeriod = 0x7416c15fc54ef67b,
		CCSGameRulesProxy_m_eRoundWinReason = 0x9b6e1c32a868b037,
		CCSGameRulesProxy_m_fMatchStartTime = 0xa22d75a197c8b224,
		CCSGameRulesProxy_m_fRoundStartTime = 0x30d897994f83dda3,
		CCSGameRulesProxy_m_fWarmupPeriodEnd = 0xd42caf4426ec9223,
		CCSGameRulesProxy_m_fWarmupPeriodStart = 0xf293b0ed3784a43f,
		CCSGameRulesProxy_m_flCMMItemDropRevealEndTime = 0xefc6d894a035cfe1,
		CCSGameRulesProxy_m_flCMMItemDropRevealStartTime = 0x4c56976a75c8dc15,
		CCSGameRulesProxy_m_flCTTimeOutRemaining = 0x5fe6bb3c030f95a,
		CCSGameRulesProxy_m_flDMBonusStartTime = 0x5f289688b41b663e,
		CCSGameRulesProxy_m_flDMBonusTimeLength = 0x1079fa5b653ad197,
		CCSGameRulesProxy_m_flGameStartTime = 0x8cd2d2f2a02151bd,
		CCSGameRulesProxy_m_flGuardianBuyUntilTime = 0xd505e37aa703c7af,
		CCSGameRulesProxy_m_flNextRespawnWave = 0x8dc2804f53640735,
		CCSGameRulesProxy_m_flRestartRoundTime = 0x2181a0a8be61c4be,
		CCSGameRulesProxy_m_flSpawnSelectionTimeEnd = 0x25e132142c635d51,
		CCSGameRulesProxy_m_flSpawnSelectionTimeLoadout = 0x53e92192dbc3679,
		CCSGameRulesProxy_m_flSpawnSelectionTimeStart = 0x37134255004828e8,
		CCSGameRulesProxy_m_flSurvivalStartTime = 0xe160bf5157ed75c1,
		CCSGameRulesProxy_m_flTabletHexOriginX = 0xf3b6edbdf0abd602,
		CCSGameRulesProxy_m_flTabletHexOriginY = 0x7ac94bc9164f4d9f,
		CCSGameRulesProxy_m_flTabletHexSize = 0x9e16f9dd0698190,
		CCSGameRulesProxy_m_flTerroristTimeOutRemaining = 0xb09134582773e2c4,
		CCSGameRulesProxy_m_gamePhase = 0xa291ba7a132b1574,
		CCSGameRulesProxy_m_iActiveAssassinationTargetMissionID = 0x83dcfce9ebc2cbad,
		CCSGameRulesProxy_m_iBombSite = 0x88ee51da453733d0,
		CCSGameRulesProxy_m_iFirstSecondHalfRound = 0xbf8ad427075041ef,
		CCSGameRulesProxy_m_iHostagesRemaining = 0x3cd2f653eb9ec6a4,
		CCSGameRulesProxy_m_iMatchStats_PlayersAlive_CT = 0x30fe67b57d34d582,
		CCSGameRulesProxy_m_iMatchStats_PlayersAlive_T = 0xfdb5899b54cffc0a,
		CCSGameRulesProxy_m_iMatchStats_RoundResults = 0x46c3c5a584f54869,
		CCSGameRulesProxy_m_iNumConsecutiveCTLoses = 0xafafcb6ea2c8f720,
		CCSGameRulesProxy_m_iNumConsecutiveTerroristLoses = 0xca080bc6c7a62820,
		CCSGameRulesProxy_m_iNumGunGameProgressiveWeaponsCT = 0x1fcb56ac4c623af,
		CCSGameRulesProxy_m_iNumGunGameProgressiveWeaponsT = 0x75f73693898bbffb,
		CCSGameRulesProxy_m_iPlayerSpawnHexIndices = 0xc81004c196184ff6,
		CCSGameRulesProxy_m_iRoundTime = 0xccc89c93a7458ca2,
		CCSGameRulesProxy_m_iRoundWinStatus = 0xf1c6403ac96b6e08,
		CCSGameRulesProxy_m_iSpectatorSlotCount = 0xdb2ca609e428a83b,
		CCSGameRulesProxy_m_nCTTimeOuts = 0xe5bad59e163d47ce,
		CCSGameRulesProxy_m_nEndMatchMapGroupVoteOptions = 0x412c0192b27fb703,
		CCSGameRulesProxy_m_nEndMatchMapGroupVoteTypes = 0xcc89f1677535a27f,
		CCSGameRulesProxy_m_nEndMatchMapVoteWinner = 0x34b2550f5cd410cd,
		CCSGameRulesProxy_m_nGuardianModeSpecialKillsRemaining = 0xd988378f237b9ad8,
		CCSGameRulesProxy_m_nGuardianModeSpecialWeaponNeeded = 0xcdb4a8be8c3d4fcb,
		CCSGameRulesProxy_m_nGuardianModeWaveNumber = 0xe46d4c006f4167d4,
		CCSGameRulesProxy_m_nHalloweenMaskListSeed = 0xf7b4cae485fe8052,
		CCSGameRulesProxy_m_nMatchSeed = 0xef3687709cb8f619,
		CCSGameRulesProxy_m_nNextMapInMapgroup = 0x69481f5bcfaebde4,
		CCSGameRulesProxy_m_nOvertimePlaying = 0xc75fcbc61fcc5ae6,
		CCSGameRulesProxy_m_nQueuedMatchmakingMode = 0xebcd0c25c551464e,
		CCSGameRulesProxy_m_nTerroristTimeOuts = 0x769e67fd8add8369,
		CCSGameRulesProxy_m_nTournamentPredictionsPct = 0x5f5245ff543af599,
		CCSGameRulesProxy_m_numBestOfMaps = 0x3f44fdfd800b3dd1,
		CCSGameRulesProxy_m_numGlobalGifters = 0x8cbcc92b126b3a9c,
		CCSGameRulesProxy_m_numGlobalGiftsGiven = 0xd8325cc5d203a9dc,
		CCSGameRulesProxy_m_numGlobalGiftsPeriodSeconds = 0x4086d94c27f1a339,
		CCSGameRulesProxy_m_roundData_playerPositions = 0xbf09ea9af8950125,
		CCSGameRulesProxy_m_roundData_playerTeams = 0xde645fbd940b15ea,
		CCSGameRulesProxy_m_roundData_playerXuids = 0x551d72f07dd8c5cf,
		CCSGameRulesProxy_m_spawnStage = 0x92ff3aa57eadfe47,
		CCSGameRulesProxy_m_szMatchStatTxt = 0x335b1327c58e6e5f,
		CCSGameRulesProxy_m_szTournamentEventName = 0x2a08b54b3d5cd42a,
		CCSGameRulesProxy_m_szTournamentEventStage = 0xb7715ee70d618db9,
		CCSGameRulesProxy_m_szTournamentPredictionsTxt = 0x10a8b4c97b0db1ac,
		CCSGameRulesProxy_m_timeUntilNextPhaseStarts = 0xef1add779aa4b37c,
		CCSGameRulesProxy_m_totalRoundsPlayed = 0xea1fe48f2b4a9115,
		CCSGameRulesProxy_m_unDMBonusWeaponLoadoutSlot = 0x7e62e446e4f33852,
		CCSGameRulesProxy_m_vecPlayAreaMaxs = 0x72c2e4c3bf3c7ff0,
		CCSGameRulesProxy_m_vecPlayAreaMins = 0x7cb274f5a5652a94,
		CCSPlayerResource_m_bControllingBot = 0xe18c8ec02c50eab5,
		CCSPlayerResource_m_bEndMatchNextMapAllVoted = 0xbeddd1cab1bb9c35,
		CCSPlayerResource_m_bHasCommunicationAbuseMute = 0xc7c245aa54bcad55,
		CCSPlayerResource_m_bHasDefuser = 0x14b077d68f6434ad,
		CCSPlayerResource_m_bHasHelmet = 0x86b6c83782954ec8,
		CCSPlayerResource_m_bHostageAlive = 0x2011e1b66a2591cf,
		CCSPlayerResource_m_bombsiteCenterA = 0xbeb8c952db0824bd,
		CCSPlayerResource_m_bombsiteCenterB = 0x873ad92e64a4881f,
		CCSPlayerResource_m_hostageRescueX = 0x5e65b806a23c4de0,
		CCSPlayerResource_m_hostageRescueY = 0x35a37309e8e3d692,
		CCSPlayerResource_m_hostageRescueZ = 0x73351bc3f87c517,
		CCSPlayerResource_m_iArmor = 0x238fa8399259c320,
		CCSPlayerResource_m_iBotDifficulty = 0x151257256750e231,
		CCSPlayerResource_m_iCashSpentThisRound = 0xe17e80232d50f82f,
		CCSPlayerResource_m_iCompTeammateColor = 0xe9d0b99b5ea25499,
		CCSPlayerResource_m_iCompetitiveRankType = 0xf71c8e64ff0c1790,
		CCSPlayerResource_m_iCompetitiveRanking = 0x638f9b31c5d45e34,
		CCSPlayerResource_m_iCompetitiveWins = 0x2bb87a6db28f8d87,
		CCSPlayerResource_m_iControlledByPlayer = 0x5a9feddc77595e2e,
		CCSPlayerResource_m_iControlledPlayer = 0xba76ddc9bd19f28,
		CCSPlayerResource_m_iGunGameLevel = 0xbb07b10d6919f6bf,
		CCSPlayerResource_m_iHostageEntityIDs = 0x8d75ea74be9e1e4,
		CCSPlayerResource_m_iLifetimeEnd = 0x7c57645166ad2337,
		CCSPlayerResource_m_iLifetimeStart = 0x829f2cb1eea7865,
		CCSPlayerResource_m_iMVPs = 0xea0012f5fc6cd979,
		CCSPlayerResource_m_iMatchStats_3k_Total = 0x3bb20222166f4b2c,
		CCSPlayerResource_m_iMatchStats_4k_Total = 0x3e2c196228f30cd6,
		CCSPlayerResource_m_iMatchStats_5k_Total = 0xa5e4082a9518e330,
		CCSPlayerResource_m_iMatchStats_Assists_Total = 0x59498634131761f8,
		CCSPlayerResource_m_iMatchStats_CashEarned_Total = 0x1316182fc1ad840d,
		CCSPlayerResource_m_iMatchStats_Damage_Total = 0x3ad45d85fc35cebc,
		CCSPlayerResource_m_iMatchStats_Deaths_Total = 0x5f651c37cfe221fb,
		CCSPlayerResource_m_iMatchStats_EnemiesFlashed_Total = 0x842e721867955bf4,
		CCSPlayerResource_m_iMatchStats_EquipmentValue_Total = 0x4e18ee0ebef1d57c,
		CCSPlayerResource_m_iMatchStats_HeadShotKills_Total = 0x2f33d5187bef684b,
		CCSPlayerResource_m_iMatchStats_KillReward_Total = 0xe8fdf0420b773325,
		CCSPlayerResource_m_iMatchStats_Kills_Total = 0x6ae24d743e63b93f,
		CCSPlayerResource_m_iMatchStats_LiveTime_Total = 0xdeeff90ebece6fff,
		CCSPlayerResource_m_iMatchStats_Objective_Total = 0xe002589800382d8d,
		CCSPlayerResource_m_iMatchStats_UtilityDamage_Total = 0x6baf27341e4cbb5d,
		CCSPlayerResource_m_iPlayerC4 = 0x1ddcb1a58eda7695,
		CCSPlayerResource_m_iPlayerVIP = 0x1d70694c2a690db6,
		CCSPlayerResource_m_iScore = 0x88fa7092e15f96ce,
		CCSPlayerResource_m_iTotalCashSpent = 0x4cfacf4fd8a69463,
		CCSPlayerResource_m_isHostageFollowingSomeone = 0x3c1db4c38b8fd549,
		CCSPlayerResource_m_nActiveCoinRank = 0x72df80f82cf5978e,
		CCSPlayerResource_m_nCharacterDefIndex = 0x4a76894c8bc1ad96,
		CCSPlayerResource_m_nEndMatchNextMapVotes = 0x1c4c9cecba13bba5,
		CCSPlayerResource_m_nMusicID = 0x6dd2bc1003e1a954,
		CCSPlayerResource_m_nPersonaDataPublicCommendsFriendly = 0xada45e90860902fb,
		CCSPlayerResource_m_nPersonaDataPublicCommendsLeader = 0xc41e8bf3f141ca76,
		CCSPlayerResource_m_nPersonaDataPublicCommendsTeacher = 0x43b21169a755a6ba,
		CCSPlayerResource_m_nPersonaDataPublicLevel = 0x38343093fcd30aa3,
		CCSPlayerResource_m_szClan = 0xdaf903af8ba7dbfd,
		CCSPlayerResource_m_szCrosshairCodes = 0x11bf80d0531ca302,
		CCSPlayer_m_ArmorValue = 0xfe6bebbc6cbe41ba,
		CCSPlayer_m_EquippedLoadoutItemDefIndices = 0x233b64f251fbb671,
		CCSPlayer_m_RetakesMVPBoostExtraUtility = 0x14bd8d1c395cd73,
		CCSPlayer_m_angEyeAngles = 0xdbbdfd1be5ae1f17,
		CCSPlayer_m_angEyeAngles_0_ = 0xb56d5f3a4c9225ab,
		CCSPlayer_m_angEyeAngles_1_ = 0x95ede5bb3c8e307b,
		CCSPlayer_m_bCanControlObservedBot = 0x201f70b465e152b,
		CCSPlayer_m_bCanMoveDuringFreezePeriod = 0x67c4a267ed2ae017,
		CCSPlayer_m_bDuckOverride = 0x796810d8ae0fa4fc,
		CCSPlayer_m_bGunGameImmunity = 0x207381e0f9cf6739,
		CCSPlayer_m_bHasControlledBotThisRound = 0x1f8fa8d66b26d847,
		CCSPlayer_m_bHasDefuser = 0x2f6bdf8993dcfe59,
		CCSPlayer_m_bHasHeavyArmor = 0xd95ee6ed1d4f3f11,
		CCSPlayer_m_bHasHelmet = 0xf41c154fc6bac6db,
		CCSPlayer_m_bHasMovedSinceSpawn = 0xfe28afee4dae8b72,
		CCSPlayer_m_bHasNightVision = 0x1ec79e48d2b55370,
		CCSPlayer_m_bHasParachute = 0x6978a81d01125ad8,
		CCSPlayer_m_bHideTargetID = 0xc9f3e40adbdabd81,
		CCSPlayer_m_bHud_MiniScoreHidden = 0xf6b722ac3803ebe4,
		CCSPlayer_m_bHud_RadarHidden = 0xae8ec0a7abe94a9c,
		CCSPlayer_m_bInBombZone = 0xe13b4341567eef13,
		CCSPlayer_m_bInBuyZone = 0xd27c172a8e2f0ee7,
		CCSPlayer_m_bInHostageRescueZone = 0x77392a3f320260b6,
		CCSPlayer_m_bInNoDefuseArea = 0x86e3369b0f3f63af,
		CCSPlayer_m_bIsAssassinationTarget = 0xba6c5af965b654a,
		CCSPlayer_m_bIsControllingBot = 0xd5687a529e97a013,
		CCSPlayer_m_bIsDefusing = 0x659f6d001ebff0e0,
		CCSPlayer_m_bIsGrabbingHostage = 0x85c1919edf332cc1,
		CCSPlayer_m_bIsHoldingLookAtWeapon = 0xee2c03626d54d2f6,
		CCSPlayer_m_bIsLookingAtWeapon = 0x4df5c17dc6452201,
		CCSPlayer_m_bIsPlayerGhost = 0xefde25cb7221018e,
		CCSPlayer_m_bIsRescuing = 0x547ad23001db711f,
		CCSPlayer_m_bIsRespawningForDMBonus = 0x28fdd0556372aa2a,
		CCSPlayer_m_bIsScoped = 0x47c5081251016a27,
		CCSPlayer_m_bIsSpawnRappelling = 0x1d9c9e0cc571f214,
		CCSPlayer_m_bIsWalking = 0xa99302cc2bc1605,
		CCSPlayer_m_bKilledByTaser = 0x18c5946c8e23d718,
		CCSPlayer_m_bMadeFinalGunGameProgressiveKill = 0x6ddfdb2d026ce69b,
		CCSPlayer_m_bNightVisionOn = 0x98c97bf0cedc7654,
		CCSPlayer_m_bPlayerDominated = 0x75a9d3c1394f9418,
		CCSPlayer_m_bPlayerDominatingMe = 0xe958a18c531d6349,
		CCSPlayer_m_bResumeZoom = 0xa88e7d06a127d3f2,
		CCSPlayer_m_bRetakesHasDefuseKit = 0x979160872981147c,
		CCSPlayer_m_bRetakesMVPLastRound = 0x530f5d4b0931122b,
		CCSPlayer_m_bStrafing = 0x3fed8a4abc3617f5,
		CCSPlayer_m_bWaitForNoAttack = 0x70edbda93ceb248,
		CCSPlayer_m_cycleLatch = 0x1524435d135bf3fb,
		CCSPlayer_m_fImmuneToGunGameDamageTime = 0xfc45aa211c695c58,
		CCSPlayer_m_fMolotovDamageTime = 0x26c8ac3b56557c38,
		CCSPlayer_m_fMolotovUseTime = 0xd6deccba5d3b7d2c,
		CCSPlayer_m_flAutoMoveStartTime = 0xa6b1466670f0686a,
		CCSPlayer_m_flAutoMoveTargetTime = 0xc441ef9bdbc15fff,
		CCSPlayer_m_flDetectedByEnemySensorTime = 0x2478b5b0297507a3,
		CCSPlayer_m_flFlashDuration = 0x82e37eab8ede6949,
		CCSPlayer_m_flFlashMaxAlpha = 0x331ac96a110dd9ab,
		CCSPlayer_m_flGroundAccelLinearFracLastTime = 0xae1906d3c1ecbc9f,
		CCSPlayer_m_flGuardianTooFarDistFrac = 0xafd3999d63324dec,
		CCSPlayer_m_flHealthShotBoostExpirationTime = 0xb4ac66fec3481120,
		CCSPlayer_m_flLastExoJumpTime = 0x26cd6400498015a4,
		CCSPlayer_m_flLowerBodyYawTarget = 0x30b741a1065e2dc6,
		CCSPlayer_m_flProgressBarStartTime = 0x97700b5c98cafbeb,
		CCSPlayer_m_flStamina = 0x2e26fe4cf1734efd,
		CCSPlayer_m_flThirdpersonRecoil = 0x6dc2bb7cde31f2e,
		CCSPlayer_m_flVelocityModifier = 0x3e65f2351046eb26,
		CCSPlayer_m_hCarriedHostage = 0xea181ff7d0a9,
		CCSPlayer_m_hCarriedHostageProp = 0xec68ee58ef01a552,
		CCSPlayer_m_hPlayerPing = 0x1b4f6013fa553bad,
		CCSPlayer_m_hRagdoll = 0xf5c51e41b807538,
		CCSPlayer_m_hSurvivalAssassinationTarget = 0x8a9f781c777de6bf,
		CCSPlayer_m_iAccount = 0x4dd38de9fa18adf8,
		CCSPlayer_m_iAddonBits = 0x5041a2aa419a3ddb,
		CCSPlayer_m_iBlockingUseActionInProgress = 0xf40d7b797a3ff30c,
		CCSPlayer_m_iClass = 0x2fa25b50874b4f1a,
		CCSPlayer_m_iControlledBotEntIndex = 0x8ff2d9e6667c948d,
		CCSPlayer_m_iDirection = 0x8fab24799c916e3c,
		CCSPlayer_m_iGunGameProgressiveWeaponIndex = 0x8cb1e8bc8c7976a5,
		CCSPlayer_m_iMatchStats_Assists = 0xdc6a93d34a7524b6,
		CCSPlayer_m_iMatchStats_CashEarned = 0x18b0eb720b9f303,
		CCSPlayer_m_iMatchStats_Damage = 0xcbcd6c9b5f4f05c1,
		CCSPlayer_m_iMatchStats_Deaths = 0xe497d5a549b626ea,
		CCSPlayer_m_iMatchStats_EnemiesFlashed = 0x72b63de031baa591,
		CCSPlayer_m_iMatchStats_EquipmentValue = 0xc3a262a75a8f44c4,
		CCSPlayer_m_iMatchStats_HeadShotKills = 0x543aaea1ee9a82ec,
		CCSPlayer_m_iMatchStats_KillReward = 0xdaa6c81ba952469,
		CCSPlayer_m_iMatchStats_Kills = 0x5a320a9e5a525c87,
		CCSPlayer_m_iMatchStats_LiveTime = 0x6b6c27429057348b,
		CCSPlayer_m_iMatchStats_MoneySaved = 0xb110c0a63e582fea,
		CCSPlayer_m_iMatchStats_Objective = 0x51bbbf87e3b90b8,
		CCSPlayer_m_iMatchStats_UtilityDamage = 0x6b61e93f1163dd3c,
		CCSPlayer_m_iMoveState = 0x4b0f6ee94a4f79e5,
		CCSPlayer_m_iNumGunGameKillsWithCurrentWeapon = 0xaa6c4876fdb9d184,
		CCSPlayer_m_iNumGunGameTRKillPoints = 0xb975143a3b1d83f3,
		CCSPlayer_m_iNumRoundKills = 0xe53c7b7c2dc5fa33,
		CCSPlayer_m_iNumRoundKillsHeadshots = 0x7e558d9fe67ac09e,
		CCSPlayer_m_iPlayerState = 0x8b57c8e9c0f445dd,
		CCSPlayer_m_iPrimaryAddon = 0xa83c80787559072c,
		CCSPlayer_m_iProgressBarDuration = 0x84db5ea106c452b0,
		CCSPlayer_m_iRetakesMVPBoostItem = 0xc4ae84c60cd60339,
		CCSPlayer_m_iRetakesOffering = 0xc4a9152be63760a8,
		CCSPlayer_m_iRetakesOfferingCard = 0xea7b12c0ebeb1557,
		CCSPlayer_m_iSecondaryAddon = 0xa27cebcb54f1bdf0,
		CCSPlayer_m_iShotsFired = 0x86b0584b4cf55556,
		CCSPlayer_m_iStartAccount = 0xedb1aa1c0c6146bb,
		CCSPlayer_m_iThrowGrenadeCounter = 0x8cfadbabed8b73be,
		CCSPlayer_m_iWeaponPurchasesThisMatch = 0xb8df04b24050a99d,
		CCSPlayer_m_iWeaponPurchasesThisRound = 0xee2ebfb6f16b8eb,
		CCSPlayer_m_isCurrentGunGameLeader = 0x9d0e26dab2aba9d5,
		CCSPlayer_m_isCurrentGunGameTeamLeader = 0xf815875f57b357e7,
		CCSPlayer_m_nDeathCamMusic = 0x74359ae617244367,
		CCSPlayer_m_nHeavyAssaultSuitCooldownRemaining = 0xfd12c0ba42c15a83,
		CCSPlayer_m_nIsAutoMounting = 0xee0fbae9d0ca6a25,
		CCSPlayer_m_nLastConcurrentKilled = 0x8ef29d531dbf7aa6,
		CCSPlayer_m_nLastKillerIndex = 0xabd6b7af58346afe,
		CCSPlayer_m_nNumFastDucks = 0xc3ad45b0479466e4,
		CCSPlayer_m_nQuestProgressReason = 0x5ffb2c0c9cf14a47,
		CCSPlayer_m_nSurvivalTeam = 0x713d554dbd58c866,
		CCSPlayer_m_passiveItems = 0x987dde0f2e702c35,
		CCSPlayer_m_rank = 0xcb8960b83686f8f,
		CCSPlayer_m_szArmsModel = 0x9bc7301c4f337036,
		CCSPlayer_m_totalHitsOnServer = 0xe036208dadc22db9,
		CCSPlayer_m_unActiveQuestId = 0x8e9a2e30b96f0482,
		CCSPlayer_m_unCurrentEquipmentValue = 0x2fba76cc21001da8,
		CCSPlayer_m_unFreezetimeEndEquipmentValue = 0xac289ceab9ba3c6e,
		CCSPlayer_m_unMusicID = 0xab6a0ca8846117e6,
		CCSPlayer_m_unPlayerTvControlFlags = 0x3266a9b1243e769e,
		CCSPlayer_m_unRoundStartEquipmentValue = 0xdd7da3d43c11792e,
		CCSPlayer_m_unTotalRoundDamageDealt = 0x34fea2028856e05,
		CCSPlayer_m_vecAutomoveTargetEnd = 0x9df74214dff53a47,
		CCSPlayer_m_vecOrigin = 0x103177ddf9e0f985,
		CCSPlayer_m_vecOrigin_2_ = 0xb99206f7e14692a6,
		CCSPlayer_m_vecPlayerPatchEconIndices = 0xad956766e79039c2,
		CCSPlayer_m_vecSpawnRappellingRopeOrigin = 0x51742a1a776ed689,
		CCSRagdoll_m_bClientSideAnimation = 0x6063442b22a795,
		CCSRagdoll_m_bDiedAirborne = 0xa25f0bce4883a616,
		CCSRagdoll_m_flAbsYaw = 0x20f53d381629f38c,
		CCSRagdoll_m_flDeathYaw = 0xa14801fa55abd7dd,
		CCSRagdoll_m_hPlayer = 0x2f6d5b06dffd324c,
		CCSRagdoll_m_iDeathFrame = 0x6191160e796350e6,
		CCSRagdoll_m_iDeathPose = 0x9df475faf204c127,
		CCSRagdoll_m_iTeamNum = 0x817463693685ff7a,
		CCSRagdoll_m_nForceBone = 0x2d2e26f7ec3099fc,
		CCSRagdoll_m_nModelIndex = 0x30a3cf18b097125c,
		CCSRagdoll_m_vecForce = 0x52e75f07634632cf,
		CCSRagdoll_m_vecOrigin = 0xb8eb8990561a617d,
		CCSRagdoll_m_vecRagdollOrigin = 0x22c9b42198d81fa8,
		CCSRagdoll_m_vecRagdollVelocity = 0xb983c1670862909d,
		CCascadeLight_m_LightColor = 0xfc518501e8c43384,
		CCascadeLight_m_LightColorScale = 0x9534a51504971c1b,
		CCascadeLight_m_bEnabled = 0x2c6541cd14e70018,
		CCascadeLight_m_bUseLightEnvAngles = 0xd66084c3b4ed7286,
		CCascadeLight_m_envLightShadowDirection = 0x7f4cba069f626a2c,
		CCascadeLight_m_flMaxShadowDist = 0x3a2594724342f96b,
		CCascadeLight_m_shadowDirection = 0x8d16992d9ea61e04,
		CChicken_m_jumpedThisFrame = 0x13f47ffaecf252d0,
		CChicken_m_leader = 0xda182438da0902b0,
		CColorCorrectionVolume_m_FadeDuration = 0x105528997e4069ce,
		CColorCorrectionVolume_m_MaxWeight = 0x56dc2c78d702fd0a,
		CColorCorrectionVolume_m_Weight = 0xf4871c585ed876fc,
		CColorCorrectionVolume_m_bEnabled = 0xeb393f3580785dc,
		CColorCorrectionVolume_m_lookupFilename = 0xfa38a2b3d76a726b,
		CColorCorrection_m_bClientSide = 0x79d2a9b9cdae4074,
		CColorCorrection_m_bEnabled = 0xdb09effba93a299f,
		CColorCorrection_m_bExclusive = 0x61401adc672281d9,
		CColorCorrection_m_bMaster = 0x6e13c827d9f92be8,
		CColorCorrection_m_flCurWeight = 0x28636d71f5a7a3af,
		CColorCorrection_m_flFadeInDuration = 0x80364c6e89a51bc8,
		CColorCorrection_m_flFadeOutDuration = 0xb31554dc97bcbe77,
		CColorCorrection_m_flMaxWeight = 0xfea49166fefc674,
		CColorCorrection_m_maxFalloff = 0x7c747a70174df5ba,
		CColorCorrection_m_minFalloff = 0x7060159d5c116ccd,
		CColorCorrection_m_netLookupFilename = 0xe644da64e3edfa77,
		CColorCorrection_m_vecOrigin = 0x4abe96a0b08ef99a,
		CDangerZoneController_m_DangerZones = 0x3972f85ddd9a5641,
		CDangerZoneController_m_bDangerZoneControllerEnabled = 0x9b07403fa404b5d3,
		CDangerZoneController_m_bMissionControlledExplosions = 0x36ae38ac6fdf019,
		CDangerZoneController_m_flFinalExpansionTime = 0xeb9028865eedf30,
		CDangerZoneController_m_flStartTime = 0x9526ab29c47f936,
		CDangerZoneController_m_flWaveEndTimes = 0x200fdf12196dd057,
		CDangerZoneController_m_hTheFinalZone = 0x750379c617795fa2,
		CDangerZoneController_m_vecEndGameCircleEnd = 0xb2144e845c6d7de9,
		CDangerZoneController_m_vecEndGameCircleStart = 0xa2b79545cef47b7,
		CDangerZone_m_flBombLaunchTime = 0x4b9d795d1c065cbc,
		CDangerZone_m_flExtraRadius = 0x6ef267ab98b0e53e,
		CDangerZone_m_flExtraRadiusStartTime = 0xb02db0d285436d04,
		CDangerZone_m_flExtraRadiusTotalLerpTime = 0x8cce496a6a40878f,
		CDangerZone_m_iWave = 0xdbc6627c44d670a9,
		CDangerZone_m_nDropOrder = 0xaa84421e69b431b9,
		CDangerZone_m_vecDangerZoneOriginStartedAt = 0xb2f4dbfffdb0d954,
		CDrone_m_bPilotTakeoverAllowed = 0x78f3b0771126882a,
		CDrone_m_hCurrentPilot = 0x3267c5d12bad34ec,
		CDrone_m_hDeliveryCargo = 0xec8093cab87e3e12,
		CDrone_m_hMoveToThisEntity = 0x9310fd55651fd08,
		CDrone_m_hPotentialCargo = 0x55f8d2cf1b38f153,
		CDrone_m_vecTagIncrements_0_ = 0xf767a771b6fdfee7,
		CDrone_m_vecTagPositions_0_ = 0x4addcd9ed771a22d,
		CDronegun_m_bHasTarget = 0x8fc3287feaba7d,
		CDronegun_m_iHealth = 0x6a032eba2fc89dd9,
		CDronegun_m_vecAttentionTarget = 0x2c4cb51ed71313fc,
		CDronegun_m_vecTargetOffset = 0x6fd714a7b8060412,
		CDynamicLight_m_Exponent = 0x1beb20a9cc5d397d,
		CDynamicLight_m_Flags = 0x879686e82f5a10bb,
		CDynamicLight_m_InnerAngle = 0x5cccc21acec769dc,
		CDynamicLight_m_LightStyle = 0x68637f13bc9011e6,
		CDynamicLight_m_OuterAngle = 0x57b7571b769282c8,
		CDynamicLight_m_Radius = 0x827ac22893f08c21,
		CDynamicLight_m_SpotRadius = 0x7ad5aea4b8a95a59,
		CDynamicProp_m_bShouldGlow = 0x2815feb02467d50e,
		CDynamicProp_m_bUseHitboxesForRenderBox = 0x9cc83ad23b450ed7,
		CDynamicProp_m_clrGlow = 0x9dba3ac3584ffa,
		CDynamicProp_m_flGlowMaxDist = 0x3528338738d43aa3,
		CDynamicProp_m_nGlowStyle = 0xf362d9f1e6cd92d6,
		CEconEntity_m_AttributeManager = 0x562d736fa7f54f61,
		CEconEntity_m_Attributes = 0xf8b208121b17b600,
		CEconEntity_m_Item = 0xc811d186fcde2b9f,
		CEconEntity_m_NetworkedDynamicAttributesForDemos = 0xdd028f4813cf707a,
		CEconEntity_m_OriginalOwnerXuidHigh = 0x37f511603c08e46c,
		CEconEntity_m_OriginalOwnerXuidLow = 0xee33c2c7c5112809,
		CEconEntity_m_ProviderType = 0xc96fca273b459e2a,
		CEconEntity_m_bInitialized = 0x536fd12f9235e13e,
		CEconEntity_m_flFallbackWear = 0xb03ce6762107b7ef,
		CEconEntity_m_hOuter = 0x76332d79910e74cb,
		CEconEntity_m_iAccountID = 0xeedb0d81ba75e0f9,
		CEconEntity_m_iEntityLevel = 0xfdc0e547f514e393,
		CEconEntity_m_iEntityQuality = 0x1f858d90fdb48ca,
		CEconEntity_m_iItemDefinitionIndex = 0x64662ee07ad72f15,
		CEconEntity_m_iItemIDHigh = 0x5fb02e9f6e8f20f4,
		CEconEntity_m_iItemIDLow = 0x6f233a0f88c69fcc,
		CEconEntity_m_iReapplyProvisionParity = 0x297c7b0d18306c88,
		CEconEntity_m_nFallbackPaintKit = 0xc8fdc5c59c0f1394,
		CEconEntity_m_nFallbackSeed = 0x69709c5296b427c5,
		CEconEntity_m_nFallbackStatTrak = 0xb838efe0f0561ade,
		CEconEntity_m_szCustomName = 0x13fb3b841479d49e,
		CEmbers_m_bEmit = 0x1b0f9edca7957384,
		CEmbers_m_nDensity = 0xa4a0ee68f9a54652,
		CEmbers_m_nLifetime = 0xb7d9d63338483083,
		CEmbers_m_nSpeed = 0x34ec2a8c0f363790,
		CEntityDissolve_m_flFadeInLength = 0x1a05f60675b368b,
		CEntityDissolve_m_flFadeInStart = 0xa050b83b1481c693,
		CEntityDissolve_m_flFadeOutLength = 0x457e629df8ac9285,
		CEntityDissolve_m_flFadeOutModelLength = 0x43d03119a2a5fa05,
		CEntityDissolve_m_flFadeOutModelStart = 0x886a3b26f9db09e2,
		CEntityDissolve_m_flFadeOutStart = 0x64da96d5b589023,
		CEntityDissolve_m_flStartTime = 0xd264d63b1523aa8e,
		CEntityDissolve_m_nDissolveType = 0xd323976b7e41fcf9,
		CEntityDissolve_m_nMagnitude = 0x1e12363f072630a0,
		CEntityDissolve_m_vDissolverOrigin = 0x3dcf7626e918a4a9,
		CEntityFlame_m_bCheapEffect = 0x1d63083a7c56af20,
		CEntityFlame_m_hEntAttached = 0xb0ec0a55fbb85dd6,
		CEntityFreezing_m_bFinishFreezing = 0xea3b115aeb4b8678,
		CEntityFreezing_m_flFrozen = 0x396012bc09258d66,
		CEntityFreezing_m_flFrozenPerHitbox = 0xa1e6a215435a8178,
		CEntityFreezing_m_vFreezingOrigin = 0x1179f88536d49b03,
		CEntityParticleTrail_m_Info = 0x479fc7177894cbbb,
		CEntityParticleTrail_m_flEndSize = 0xb45204925b26df7,
		CEntityParticleTrail_m_flLifetime = 0xf4963cdabd584ce8,
		CEntityParticleTrail_m_flStartSize = 0xe767e5da8cf4f85c,
		CEntityParticleTrail_m_hConstraintEntity = 0xab9814ba211f34f2,
		CEntityParticleTrail_m_iMaterialName = 0x8a8f5225bd32bced,
		CEnvAmbientLight_m_vecColor = 0x709b9b734a70be69,
		CEnvDOFController_m_bDOFEnabled = 0xd605ee122cd40f8b,
		CEnvDOFController_m_flFarBlurDepth = 0x53dd96752d4b8640,
		CEnvDOFController_m_flFarBlurRadius = 0xdc50b161121895fd,
		CEnvDOFController_m_flFarFocusDepth = 0x8e923b175a7dcdfe,
		CEnvDOFController_m_flNearBlurDepth = 0xf0a2508c7c1497e0,
		CEnvDOFController_m_flNearBlurRadius = 0xfff688d788abe2c7,
		CEnvDOFController_m_flNearFocusDepth = 0xf6266739695f461e,
		CEnvDetailController_m_flFadeEndDist = 0xbd644940c4412f25,
		CEnvDetailController_m_flFadeStartDist = 0x5997aa3957efeb1f,
		CEnvGasCanister_m_bDoImpactEffects = 0x54ba472289606fe9,
		CEnvGasCanister_m_bInSkybox = 0x20fa5421e293340,
		CEnvGasCanister_m_bLanded = 0xf22f0012dcb93adf,
		CEnvGasCanister_m_bLaunchedFromWithinWorld = 0x109c883854ddc6be,
		CEnvGasCanister_m_flFlightSpeed = 0xced38d31740ed708,
		CEnvGasCanister_m_flFlightTime = 0xdf3997a945aaab76,
		CEnvGasCanister_m_flHorizSpeed = 0x636326009bf0b016,
		CEnvGasCanister_m_flInitialZSpeed = 0xa294f405f1ff66af,
		CEnvGasCanister_m_flLaunchTime = 0x3e49133e79b0c1fe,
		CEnvGasCanister_m_flSkyboxScale = 0xe6faeb30473c8c02,
		CEnvGasCanister_m_flWorldEnterTime = 0xb6c28569addead0c,
		CEnvGasCanister_m_flZAcceleration = 0xf159f25caebde4,
		CEnvGasCanister_m_hSkyboxCopy = 0x30fb65ea3a6cafc1,
		CEnvGasCanister_m_nMyZoneIndex = 0xd6c2f96d0a3024c1,
		CEnvGasCanister_m_vecDirection = 0x1c58e8c618721bef,
		CEnvGasCanister_m_vecEnterWorldPosition = 0x6c894ad9d521ebea,
		CEnvGasCanister_m_vecImpactPosition = 0xd97c7d37a8bf53c7,
		CEnvGasCanister_m_vecOrigin = 0xf399034e25b970d0,
		CEnvGasCanister_m_vecOrigin_2_ = 0x439c9e49bb0a6113,
		CEnvGasCanister_m_vecParabolaDirection = 0xb1bae97c4bc416c1,
		CEnvGasCanister_m_vecSkyboxOrigin = 0x94795e54e606c9c4,
		CEnvGasCanister_m_vecStartAngles = 0x8688beea6c6bfec,
		CEnvGasCanister_m_vecStartPosition = 0x15f1a7f89a247228,
		CEnvParticleScript_m_flSequenceScale = 0x29c0676e5fc47922,
		CEnvProjectedTexture_m_LightColor = 0xbe476ce6150664f1,
		CEnvProjectedTexture_m_SpotlightTextureName = 0xe8c063c09225bf4f,
		CEnvProjectedTexture_m_bAlwaysUpdate = 0xe7788542d8f2c186,
		CEnvProjectedTexture_m_bCameraSpace = 0xf184c23c0ac53941,
		CEnvProjectedTexture_m_bEnableShadows = 0x166fe402e769eb16,
		CEnvProjectedTexture_m_bLightOnlyTarget = 0x3c963ff3429e66d6,
		CEnvProjectedTexture_m_bLightWorld = 0xd47c9b5fdedeb156,
		CEnvProjectedTexture_m_bSimpleProjection = 0x26d8c00968abd539,
		CEnvProjectedTexture_m_bState = 0x2e471ecb92a686e7,
		CEnvProjectedTexture_m_flAmbient = 0xfc1084d2bb7b21a2,
		CEnvProjectedTexture_m_flBrightnessScale = 0x917afb7cbfebd1b6,
		CEnvProjectedTexture_m_flColorTransitionTime = 0x448b88452f62b516,
		CEnvProjectedTexture_m_flFarZ = 0x7ab47e91097008e9,
		CEnvProjectedTexture_m_flLightFOV = 0x755d1a00dcc2ac60,
		CEnvProjectedTexture_m_flNearZ = 0x416ffe98e8c840c2,
		CEnvProjectedTexture_m_flProjectionSize = 0xc4cbb21055e44a69,
		CEnvProjectedTexture_m_flRotation = 0x53ec849bebf0c315,
		CEnvProjectedTexture_m_hTargetEntity = 0xaa19c58e654a8287,
		CEnvProjectedTexture_m_iStyle = 0x8c8e499284a7b372,
		CEnvProjectedTexture_m_nShadowQuality = 0x6ab17803d202c7b4,
		CEnvProjectedTexture_m_nSpotlightTextureFrame = 0xc73d39f0421fa17e,
		CEnvQuadraticBeam_m_controlPosition = 0x25930b16921238d6,
		CEnvQuadraticBeam_m_flWidth = 0x4d742ecc3d9d3b14,
		CEnvQuadraticBeam_m_scrollRate = 0x1aac732023832b9,
		CEnvQuadraticBeam_m_targetPosition = 0xf79b3cf2ed6da318,
		CEnvScreenEffect_m_flDuration = 0x56fd2d6706e8a78b,
		CEnvScreenEffect_m_nType = 0x1daa23c73ad09981,
		CEnvScreenOverlay_m_bIsActive = 0x3d249256ff671ba1,
		CEnvScreenOverlay_m_flOverlayTimes_0_ = 0xcc1ae221fdbe90d3,
		CEnvScreenOverlay_m_flStartTime = 0xe6a47e1d00ee5987,
		CEnvScreenOverlay_m_iDesiredOverlay = 0x123cd8d89dde868e,
		CEnvScreenOverlay_m_iszOverlayNames_0_ = 0x6f5ac555d1be4602,
		CEnvTonemapController_m_bUseCustomAutoExposureMax = 0x79c05c3f26b8d5c4,
		CEnvTonemapController_m_bUseCustomAutoExposureMin = 0x2b8c6991b98fb75a,
		CEnvTonemapController_m_bUseCustomBloomScale = 0xc92c383482f4a98,
		CEnvTonemapController_m_flBloomExponent = 0xa146c3858af8c7ed,
		CEnvTonemapController_m_flBloomSaturation = 0x43cb3c5a6e8452c,
		CEnvTonemapController_m_flCustomAutoExposureMax = 0xcb93ca001a7f6d65,
		CEnvTonemapController_m_flCustomAutoExposureMin = 0xab87ce33659491e2,
		CEnvTonemapController_m_flCustomBloomScale = 0x4993839903c01990,
		CEnvTonemapController_m_flCustomBloomScaleMinimum = 0xc2f015ca76ad6b4d,
		CEnvTonemapController_m_flTonemapMinAvgLum = 0xf40a78cd90b0d3be,
		CEnvTonemapController_m_flTonemapPercentBrightPixels = 0x33c3ab2854264d09,
		CEnvTonemapController_m_flTonemapPercentTarget = 0xf0a0d475a0483729,
		CEnvTonemapController_m_flTonemapRate = 0xfc21baa7357d02fa,
		CEnvWind_m_EnvWindShared = 0xbc1693a8fe18d3e2,
		CEnvWind_m_flGustDuration = 0x8a0d5ed050dfe398,
		CEnvWind_m_flInitialWindSpeed = 0x5939775f30920913,
		CEnvWind_m_flMaxGustDelay = 0xdff18110bf841650,
		CEnvWind_m_flMinGustDelay = 0xcffc49ca929bdbb2,
		CEnvWind_m_flStartTime = 0xe8da7d8c3e65ab90,
		CEnvWind_m_iGustDirChange = 0x5f4d1f431e2c790d,
		CEnvWind_m_iInitialWindDir = 0x884e4dc656d27c7c,
		CEnvWind_m_iMaxGust = 0x48048c9c091d75f4,
		CEnvWind_m_iMaxWind = 0x67913e25a4c9703c,
		CEnvWind_m_iMinGust = 0x81ed26f5e3f9b949,
		CEnvWind_m_iMinWind = 0x367a53a3a43e2767,
		CEnvWind_m_iWindSeed = 0x9de66c3320f6ca1a,
		CFEPlayerDecal_m_flCreationTime = 0x1244701970137602,
		CFEPlayerDecal_m_nEntity = 0xea7f795971d1dd6d,
		CFEPlayerDecal_m_nHitbox = 0x5c0eeac42a6e3f0f,
		CFEPlayerDecal_m_nPlayer = 0x58089ce68e95d6e8,
		CFEPlayerDecal_m_nTintID = 0x7f7aed1e2ece0a19,
		CFEPlayerDecal_m_nUniqueID = 0xc0807cd85d520681,
		CFEPlayerDecal_m_nVersion = 0xd41fac5e429e6053,
		CFEPlayerDecal_m_rtGcTime = 0x7c819a30d3c700c8,
		CFEPlayerDecal_m_ubSignature = 0xf60dfd280be3f4e0,
		CFEPlayerDecal_m_unAccountID = 0x986c5e285d9f3a69,
		CFEPlayerDecal_m_unTraceID = 0x66a1500486471abc,
		CFEPlayerDecal_m_vecEndPos = 0xcb818e57d059a853,
		CFEPlayerDecal_m_vecNormal = 0xabdb46edd0fe5c12,
		CFEPlayerDecal_m_vecRight = 0x3d034adbcf60e82a,
		CFEPlayerDecal_m_vecStart = 0x1c0e94ac7b9af2dd,
		CFireSmoke_m_flScale = 0xd8f099403e16cd7,
		CFireSmoke_m_flScaleTime = 0xff707af4dd2d911b,
		CFireSmoke_m_flStartScale = 0x8c69030ff1a831aa,
		CFireSmoke_m_nFlags = 0x56aa80adb8f9be79,
		CFireSmoke_m_nFlameFromAboveModelIndex = 0xede57c519f79a9a6,
		CFireSmoke_m_nFlameModelIndex = 0xb179c39016856447,
		CFireTrail_m_flLifetime = 0x987a9085bde70f76,
		CFireTrail_m_nAttachment = 0x6881a66ad809a4c0,
		CFish_m_angle = 0xd1f87343a54507d6,
		CFish_m_lifeState = 0xa94cf7bdbf787cb1,
		CFish_m_nModelIndex = 0x1c18b32c9630923c,
		CFish_m_poolOrigin = 0xe8210ca623268972,
		CFish_m_waterLevel = 0x391ff21213ac1db,
		CFish_m_x = 0xbe0dae585eea6481,
		CFish_m_y = 0x366c12dcd8ef9f1c,
		CFish_m_z = 0x1db701a22f480535,
		CFists_m_bPlayingUninterruptableAct = 0x887e9bcf896eec9c,
		CFogController_m_fog_HDRColorScale = 0x703faf8b3cb39a9e,
		CFogController_m_fog_ZoomFogScale = 0x6f834a185932b79f,
		CFogController_m_fog_blend = 0x3bcd00b33ed43b18,
		CFogController_m_fog_colorPrimary = 0x29fda88c118c77a6,
		CFogController_m_fog_colorPrimaryLerpTo = 0xe192ff76b4647c74,
		CFogController_m_fog_colorSecondary = 0x59b8b7238e9f220c,
		CFogController_m_fog_colorSecondaryLerpTo = 0xf00c8075985097bb,
		CFogController_m_fog_dirPrimary = 0x59f752d5bffa26c2,
		CFogController_m_fog_duration = 0x217467036d5ecd80,
		CFogController_m_fog_enable = 0x92361fb9a35d4e00,
		CFogController_m_fog_end = 0x27c4d80d0226badc,
		CFogController_m_fog_endLerpTo = 0xc1f0a50c414369a2,
		CFogController_m_fog_farz = 0xc600c4897bf755d8,
		CFogController_m_fog_lerptime = 0xb498d7143615ea0,
		CFogController_m_fog_maxdensity = 0xb8201d6ed89a89e4,
		CFogController_m_fog_maxdensityLerpTo = 0xa295fdce0817825,
		CFogController_m_fog_start = 0x2d94ea9530db6b4b,
		CFogController_m_fog_startLerpTo = 0xe6a64bc5d8f5a1ba,
		CFootstepControl_m_destination = 0x2c5209c8035d99d6,
		CFootstepControl_m_source = 0x9ad1c7e6eae4a614,
		CFuncAreaPortalWindow_m_flFadeDist = 0xdf03618011323fc6,
		CFuncAreaPortalWindow_m_flFadeStartDist = 0xbccc91162966016d,
		CFuncAreaPortalWindow_m_flTranslucencyLimit = 0x2ec738d05c7f6115,
		CFuncAreaPortalWindow_m_iBackgroundModelIndex = 0x680eabb15eda6396,
		CFuncConveyor_m_flConveyorSpeed = 0x5b47eb9b492a0224,
		CFuncLadder_m_bFakeLadder = 0x1d2966f4048c145c,
		CFuncLadder_m_vecLadderDir = 0xa3be7321f79c902c,
		CFuncLadder_m_vecPlayerMountPositionBottom = 0x2b1e2adfa21fbc4a,
		CFuncLadder_m_vecPlayerMountPositionTop = 0xcc321596b51b5e0f,
		CFuncMoveLinear_m_fFlags = 0x36bf5f81fc98c191,
		CFuncMoveLinear_m_vecVelocity = 0xba39431d95ccdcd,
		CFuncOccluder_m_bActive = 0xdcd2f3dbccded5bd,
		CFuncOccluder_m_nOccluderIndex = 0x705b0ba84dfaf25c,
		CFuncRotating_m_angRotation_0_ = 0x6f0c13c0d1a211be,
		CFuncRotating_m_angRotation_1_ = 0x4106f652625fa2c6,
		CFuncRotating_m_angRotation_2_ = 0x3cb9b30318a278ed,
		CFuncRotating_m_flSimulationTime = 0xb6d05c0050a096bc,
		CFuncRotating_m_vecOrigin = 0x883f2eead2b16a82,
		CFuncSmokeVolume_m_Collision = 0x675219f716752be4,
		CFuncSmokeVolume_m_Color1 = 0x4451bb4ce717f9f0,
		CFuncSmokeVolume_m_Color2 = 0x56d2e01274643ff4,
		CFuncSmokeVolume_m_Density = 0x19b9abcac76848fa,
		CFuncSmokeVolume_m_DensityRampSpeed = 0xc14f76b9b396e78c,
		CFuncSmokeVolume_m_MaterialName = 0xcd867fa6da22841d,
		CFuncSmokeVolume_m_MovementSpeed = 0x27625aec46eb2ade,
		CFuncSmokeVolume_m_ParticleDrawWidth = 0x7b4cfb92506a4770,
		CFuncSmokeVolume_m_ParticleSpacingDistance = 0x3b2248d4d0111a4f,
		CFuncSmokeVolume_m_RotationSpeed = 0x5643640607c3845a,
		CFuncSmokeVolume_m_maxDrawDistance = 0xd20cf4520ac6b2f2,
		CFuncSmokeVolume_m_nSolidType = 0xb242e4d69f82881c,
		CFuncSmokeVolume_m_nSurroundType = 0xb315356027b8f8ca,
		CFuncSmokeVolume_m_spawnflags = 0x60e78ee91e8cc0ae,
		CFuncSmokeVolume_m_triggerBloat = 0xc703020b47df53ab,
		CFuncSmokeVolume_m_usSolidFlags = 0x61a00644f66480df,
		CFuncSmokeVolume_m_vecMaxs = 0x4e3a2207fb0aa32,
		CFuncSmokeVolume_m_vecMins = 0x9ddb2bec19c57443,
		CFuncSmokeVolume_m_vecSpecifiedSurroundingMaxs = 0x70599379a1e19276,
		CFuncSmokeVolume_m_vecSpecifiedSurroundingMins = 0xf7cb5cc470d530b3,
		CFunc_Dust_m_Collision = 0xe1882e6c429c500d,
		CFunc_Dust_m_Color = 0x126ba8fbd1de19f6,
		CFunc_Dust_m_DistMax = 0xe5a61a762e0211ac,
		CFunc_Dust_m_DustFlags = 0xcb12ae026d5eb1e9,
		CFunc_Dust_m_FallSpeed = 0xd421d09b920c6ed1,
		CFunc_Dust_m_LifetimeMax = 0x93fa79a6a02a9eb0,
		CFunc_Dust_m_LifetimeMin = 0x983a19bd493dadd8,
		CFunc_Dust_m_SpawnRate = 0x84e6fe9c2fa34fd,
		CFunc_Dust_m_SpeedMax = 0xaacbce0c8ecea6e4,
		CFunc_Dust_m_bAffectedByWind = 0xd46dcf2b6d3db8b8,
		CFunc_Dust_m_flSizeMax = 0x6c067de64ba6911a,
		CFunc_Dust_m_flSizeMin = 0xd608a56dff604f28,
		CFunc_Dust_m_nModelIndex = 0x68943cf0ab76b1e1,
		CFunc_Dust_m_nSolidType = 0x89d14f88f0e144f2,
		CFunc_Dust_m_nSurroundType = 0xd8a9fa8868d9091e,
		CFunc_Dust_m_triggerBloat = 0xbdc50551f6da5f46,
		CFunc_Dust_m_usSolidFlags = 0xf6fe532f1b023591,
		CFunc_Dust_m_vecMaxs = 0xe1051c06d5f9739b,
		CFunc_Dust_m_vecMins = 0x7058a1914744451f,
		CFunc_Dust_m_vecSpecifiedSurroundingMaxs = 0x38b74f2ce5691405,
		CFunc_Dust_m_vecSpecifiedSurroundingMins = 0x9cf795df13ac1225,
		CFunc_LOD_m_nDisappearMaxDist = 0x5e861c6fd1dddea8,
		CFunc_LOD_m_nDisappearMinDist = 0x3ea2e96e0dbff3f3,
		CGrassBurn_m_flGrassBurnClearTime = 0xa33b8a64bc8fc1b6,
		CHandleTest_m_Handle = 0xe2073029c27f8229,
		CHandleTest_m_bSendHandle = 0xf561f6391fc3dc17,
		CHostage_m_fFlags = 0xa7480e851722974d,
		CHostage_m_flDropStartTime = 0xd4f9756a018476bf,
		CHostage_m_flGrabSuccessTime = 0x939a0d057a805566,
		CHostage_m_flRescueStartTime = 0x484458d335a38126,
		CHostage_m_iHealth = 0x21451014c9a73e3f,
		CHostage_m_iMaxHealth = 0xaf2f9ebcea6caf4b,
		CHostage_m_isRescued = 0x1bd8a30092fd523b,
		CHostage_m_jumpedThisFrame = 0x82d1b2d14c016144,
		CHostage_m_leader = 0xc9b7e681e8398c56,
		CHostage_m_lifeState = 0x2fd16c328b91734a,
		CHostage_m_nHostageState = 0xa9997433da247426,
		CHostage_m_vel = 0xc45b4bee1f471d76,
		CInferno_m_bFireIsBurning = 0x12a6e8beffb731ee,
		CInferno_m_fireCount = 0x2a80b2ec65b018cd,
		CInferno_m_fireXDelta = 0xfba3ba2a6837ddde,
		CInferno_m_fireYDelta = 0xd9e750d271335752,
		CInferno_m_fireZDelta = 0x7f98e503cbe6c075,
		CInferno_m_nFireEffectTickBegin = 0xf9c30ed058710a6a,
		CInfoMapRegion_m_flRadius = 0x94d4036e348d242a,
		CInfoMapRegion_m_szLocToken = 0xcb686dc84437ef0f,
		CInfoOverlayAccessor_m_iOverlayID = 0x4594dc2923888a04,
		CInfoOverlayAccessor_m_iTextureFrameIndex = 0xaa8e7abf64056d51,
		CItemAssaultSuitUseable_m_bIsHeavyAssaultSuit = 0x18a1a84ed2f1b1a5,
		CItemAssaultSuitUseable_m_nArmorValue = 0x55293a74cef24ec3,
		CItemDogtags_m_KillingPlayer = 0x5f2fad9970bfdff2,
		CItemDogtags_m_OwningPlayer = 0xa6a1daa5b04c259c,
		CItem_m_bShouldGlow = 0x9807f03811858048,
		CLightGlow_m_angRotation = 0x428986484faca76e,
		CLightGlow_m_clrRender = 0x13bda46bc8199bb7,
		CLightGlow_m_flGlowProxySize = 0x98abaf182a7150c0,
		CLightGlow_m_nHorizontalSize = 0xcc221962b627b0b6,
		CLightGlow_m_nMaxDist = 0x5e72a2a19c1975c4,
		CLightGlow_m_nMinDist = 0xa695ef7ea066bdea,
		CLightGlow_m_nOuterMaxDist = 0x93bb19071275804c,
		CLightGlow_m_nVerticalSize = 0x784ecf32bca6bbbb,
		CLightGlow_m_spawnflags = 0x8ec91826932eceac,
		CLightGlow_m_vecOrigin = 0x126871957ba5ab41,
		CLightGlow_moveparent = 0xf4b3b7b716a7064b,
		CMapVetoPickController_m_nAccountIDs = 0x11c4098511fd1579,
		CMapVetoPickController_m_nCurrentPhase = 0x897ed7cb803f545f,
		CMapVetoPickController_m_nDraftType = 0x36b2e09012456207,
		CMapVetoPickController_m_nMapId0 = 0x8176db41358c49f9,
		CMapVetoPickController_m_nMapId1 = 0xc759fd14339c2d2,
		CMapVetoPickController_m_nMapId2 = 0xa29d22104b2c06d0,
		CMapVetoPickController_m_nMapId3 = 0xf18498adde58d737,
		CMapVetoPickController_m_nMapId4 = 0x9d28dd09b545a2a2,
		CMapVetoPickController_m_nMapId5 = 0x1ea38ca06ef0b04c,
		CMapVetoPickController_m_nPhaseDurationTicks = 0x699e46fbe2fdb3e1,
		CMapVetoPickController_m_nPhaseStartTick = 0xfd87f83209e56aea,
		CMapVetoPickController_m_nStartingSide0 = 0x410bcd411186b09a,
		CMapVetoPickController_m_nTeamWinningCoinToss = 0x2140b2f1be5be90d,
		CMapVetoPickController_m_nTeamWithFirstChoice = 0x709a2ed07c338e82,
		CMapVetoPickController_m_nVoteMapIdsList = 0x7f723135fb09b66a,
		CMaterialModifyControl_m_bFloatLerpWrap = 0xe8a39432a27f34d6,
		CMaterialModifyControl_m_bNewAnimCommandsSemaphore = 0xc96eeb201d626483,
		CMaterialModifyControl_m_bWrap = 0xc487d2a3315e68b3,
		CMaterialModifyControl_m_flFloatLerpEndValue = 0x5094a51fd9c1e6ad,
		CMaterialModifyControl_m_flFloatLerpStartValue = 0x87be91a59c4ef9fb,
		CMaterialModifyControl_m_flFloatLerpTransitionTime = 0x293cba2b1cc6bc66,
		CMaterialModifyControl_m_flFramerate = 0x73265f966f5ff066,
		CMaterialModifyControl_m_iFrameEnd = 0x874cdb78508cbf1e,
		CMaterialModifyControl_m_iFrameStart = 0x72c6e59db5b55901,
		CMaterialModifyControl_m_nModifyMode = 0x70d654285745131,
		CMaterialModifyControl_m_szMaterialName = 0xf2d530cf560db3c4,
		CMaterialModifyControl_m_szMaterialVar = 0x311c3209e5ab1aa1,
		CMaterialModifyControl_m_szMaterialVarValue = 0x23ad7c29fcb9887,
		CMelee_m_flThrowAt = 0x8de28266b6028a88,
		CMolotovProjectile_m_bIsIncGrenade = 0xf57f0922347329f8,
		CMovieDisplay_m_bEnabled = 0xd1d41208ad0dedc3,
		CMovieDisplay_m_bForcedSlave = 0x9b2c3eb730cf36ad,
		CMovieDisplay_m_bLooping = 0xc8c188653fa097c4,
		CMovieDisplay_m_bStretchToFill = 0x2faa9967c486a722,
		CMovieDisplay_m_bUseCustomUVs = 0x7652fdc4faa70042,
		CMovieDisplay_m_flUMax = 0x2dbc6c9253ab9ac7,
		CMovieDisplay_m_flUMin = 0x2f6b3e1fae5fc753,
		CMovieDisplay_m_flVMax = 0xd185a021ed154438,
		CMovieDisplay_m_flVMin = 0x3a7bf839cf5d436c,
		CMovieDisplay_m_szGroupName = 0x6e6142fca16d08c4,
		CMovieDisplay_m_szMovieFilename = 0xa35e53854ceab900,
		CParadropChopper_m_hCallingPlayer = 0x13cfe58616ca1c27,
		CParadropChopper_m_vecOrigin = 0x882c0a5958cd7466,
		CParadropChopper_m_vecOrigin_2_ = 0xf7f8e8443e23e604,
		CParticleFire_m_vDirection = 0xb68f4a60fb97a4e2,
		CParticleFire_m_vOrigin = 0x88985d9cd6ec67ec,
		CParticlePerformanceMonitor_m_bDisplayPerf = 0x392318d2451ebd55,
		CParticlePerformanceMonitor_m_bMeasurePerf = 0x1ed4811f530884ce,
		CParticleSystem_m_angRotation = 0x36f08733af863344,
		CParticleSystem_m_bActive = 0xc374358666ebd20a,
		CParticleSystem_m_fEffects = 0x11a914f3ade420f4,
		CParticleSystem_m_flStartTime = 0x944abd9fc7740048,
		CParticleSystem_m_hControlPointEnts = 0x135c6eb4a62f8ca9,
		CParticleSystem_m_hOwnerEntity = 0xcb7c1f9d1642e675,
		CParticleSystem_m_iControlPointParents = 0xb6d958f30d1db2d5,
		CParticleSystem_m_iEffectIndex = 0xa7fb895a94c8c823,
		CParticleSystem_m_iParentAttachment = 0xf697e36a169dbc8b,
		CParticleSystem_m_iServerControlPointAssignments = 0x54df283ec70d410b,
		CParticleSystem_m_nStopType = 0x353c8d83f1c23995,
		CParticleSystem_m_szSnapshotFileName = 0x257a34500f2028e0,
		CParticleSystem_m_vServerControlPoints = 0xd28105a2619bf9ea,
		CParticleSystem_m_vecOrigin = 0x8eab67a7a763ec6d,
		CParticleSystem_moveparent = 0xe074d2a84c5ad8b8,
		CPhysBoxMultiplayer_m_fMass = 0xed434e023db15103,
		CPhysBoxMultiplayer_m_iPhysicsMode = 0x3f94bf2361bee867,
		CPhysBox_m_mass = 0xc3c00c896efc10c,
		CPhysPropLootCrate_m_bRenderInPSPM = 0xa338d6ea148931af,
		CPhysPropLootCrate_m_bRenderInTablet = 0xb47a1e0db0e9a1a6,
		CPhysPropLootCrate_m_iHealth = 0x3312d6bd031590e8,
		CPhysPropLootCrate_m_iMaxHealth = 0xa393df0bed11f186,
		CPhysicsPropMultiplayer_m_collisionMaxs = 0x385dfe27e2c88283,
		CPhysicsPropMultiplayer_m_collisionMins = 0xde00c2f992da3dd3,
		CPhysicsPropMultiplayer_m_fMass = 0xd82dafd9f1895209,
		CPhysicsPropMultiplayer_m_iPhysicsMode = 0x7c74aa3dab89a0c1,
		CPhysicsProp_m_bAwake = 0xb7069e69f57c1530,
		CPhysicsProp_m_spawnflags = 0xaf0eba6a476f4cbf,
		CPlantedC4_m_bBombDefused = 0xb9df21ba3dfd5808,
		CPlantedC4_m_bBombTicking = 0x83d9240d1d4841b6,
		CPlantedC4_m_flC4Blow = 0x444082d3a1736b34,
		CPlantedC4_m_flDefuseCountDown = 0x780837477f0e42e7,
		CPlantedC4_m_flDefuseLength = 0xfb582689429a0303,
		CPlantedC4_m_flTimerLength = 0xe6a6f001f744d015,
		CPlantedC4_m_hBombDefuser = 0xf75c503a414d0d1d,
		CPlantedC4_m_nBombSite = 0xffa5be476c9e0954,
		CPlasma_m_flScale = 0xd0db4b58c761f846,
		CPlasma_m_flScaleTime = 0xfcb19e3130d3992a,
		CPlasma_m_flStartScale = 0x35eac3bcd35e6ac7,
		CPlasma_m_nFlags = 0x78618c8c36949c60,
		CPlasma_m_nGlowModelIndex = 0x5b9bf8180a51528c,
		CPlasma_m_nPlasmaModelIndex = 0x9a528d90cbbf7e4,
		CPlasma_m_nPlasmaModelIndex2 = 0x127183a7d36e8596,
		CPlayerPing_m_bUrgent = 0xe2c3e6550b15ff23,
		CPlayerPing_m_hPingedEntity = 0xed9f3c370ebb0cdd,
		CPlayerPing_m_hPlayer = 0x1a05c1f195aefb7c,
		CPlayerPing_m_iType = 0x21e0c2db94fa573b,
		CPlayerPing_m_szPlaceName = 0xbf8aa0e537f2d2ac,
		CPlayerResource_m_bAlive = 0x77798aabdc696703,
		CPlayerResource_m_bConnected = 0x9ed80821ccba5da4,
		CPlayerResource_m_iAssists = 0x45bcf82121bf58ce,
		CPlayerResource_m_iCoachingTeam = 0xd135f720c1e945d2,
		CPlayerResource_m_iDeaths = 0x3eefca395d257ab6,
		CPlayerResource_m_iHealth = 0xc3ac89bc16c0a7bf,
		CPlayerResource_m_iKills = 0xe8ab41fd9d6b408,
		CPlayerResource_m_iPendingTeam = 0xc09fece40571a283,
		CPlayerResource_m_iPing = 0x6f4bc696531702f9,
		CPlayerResource_m_iTeam = 0x506b72a6285ebdf2,
		CPointCamera_m_FOV = 0x57fa778a4ed83020,
		CPointCamera_m_FogColor = 0xe0d46a00cfd1f487,
		CPointCamera_m_Resolution = 0xd9354ffecaded1aa,
		CPointCamera_m_bActive = 0xd4506190635d10e3,
		CPointCamera_m_bFogEnable = 0xe4596a94cf521de2,
		CPointCamera_m_bUseScreenAspectRatio = 0xe12f9e227c3272eb,
		CPointCamera_m_flFogEnd = 0x77242aa6f053d2af,
		CPointCamera_m_flFogMaxDensity = 0x67402ebb0a7660bd,
		CPointCamera_m_flFogStart = 0x8e196a0d0843cb74,
		CPointCommentaryNode_m_bActive = 0x78ffe6fac57d9dbd,
		CPointCommentaryNode_m_flStartTime = 0x119c811bc16df92d,
		CPointCommentaryNode_m_hViewPosition = 0x85131d3a0757f1d1,
		CPointCommentaryNode_m_iNodeNumber = 0x8746f310ada555a7,
		CPointCommentaryNode_m_iNodeNumberMax = 0x7cf7af8553a9d26c,
		CPointCommentaryNode_m_iszCommentaryFile = 0x20c64fcd5ddad6ae,
		CPointCommentaryNode_m_iszCommentaryFileNoHDR = 0xec821cf3ecd8ef8,
		CPointCommentaryNode_m_iszSpeakers = 0x867bf9e83c7e005b,
		CPointWorldText_m_flTextSize = 0x67ba6b68eb682c5,
		CPointWorldText_m_szText = 0x5b88a2b8d1b98d47,
		CPointWorldText_m_textColor = 0x7d3b87eee6590353,
		CPoseController_m_bInterpolationWrap = 0xf26739c7127735e,
		CPoseController_m_bPoseValueParity = 0xaee835f6d6efb5e8,
		CPoseController_m_chPoseIndex = 0x8b8e1db15f06e5e9,
		CPoseController_m_fCycleFrequency = 0x3c1d4ffcea11f041,
		CPoseController_m_fFModAmplitude = 0x84b089328485ca81,
		CPoseController_m_fFModRate = 0x381573674c1d504c,
		CPoseController_m_fFModTimeOffset = 0x32b4836727c9ffb8,
		CPoseController_m_fInterpolationTime = 0xd40e3b351729c068,
		CPoseController_m_fPoseValue = 0x1c314b5161eb4e3b,
		CPoseController_m_hProps = 0xa668e5ef5cb9aea3,
		CPoseController_m_nFModType = 0x831d022df63b2430,
		CPostProcessController_m_bMaster = 0x1e9e8efb30b36e80,
		CPostProcessController_m_flPostProcessParameters = 0x68a0c2b0e89aa274,
		CPrecipitation_m_nPrecipType = 0x8cc6c9ffd68d2665,
		CPropCounter_m_flDisplayValue = 0xb553cce1fefe4f78,
		CPropJeep_m_bHeadlightIsOn = 0x34cb48f75b19ec1a,
		CPropVehicleChoreoGeneric_m_bEnterAnimOn = 0xeb440decc5fb2e96,
		CPropVehicleChoreoGeneric_m_bExitAnimOn = 0xeeffba6f3481d112,
		CPropVehicleChoreoGeneric_m_bForceEyesToAttachment = 0x72211557ec5b155d,
		CPropVehicleChoreoGeneric_m_hPlayer = 0x500c255dcffbfeb7,
		CPropVehicleChoreoGeneric_m_vecEyeExitEndpoint = 0x180cc4f9e68302db,
		CPropVehicleChoreoGeneric_m_vehicleView_bClampEyeAngles = 0xaed5569031ff0d34,
		CPropVehicleChoreoGeneric_m_vehicleView_flFOV = 0x47ab3ff3065b3fb4,
		CPropVehicleChoreoGeneric_m_vehicleView_flPitchCurveLinear = 0x7fd4f97c401ea18f,
		CPropVehicleChoreoGeneric_m_vehicleView_flPitchCurveZero = 0x8996080f1334a74f,
		CPropVehicleChoreoGeneric_m_vehicleView_flPitchMax = 0x7805a224886aa142,
		CPropVehicleChoreoGeneric_m_vehicleView_flPitchMin = 0x6b2bc1e895539221,
		CPropVehicleChoreoGeneric_m_vehicleView_flRollCurveLinear = 0x18122c2279fb58f3,
		CPropVehicleChoreoGeneric_m_vehicleView_flRollCurveZero = 0x85b3648cb0fa9da3,
		CPropVehicleChoreoGeneric_m_vehicleView_flYawMax = 0x394099a9f4482369,
		CPropVehicleChoreoGeneric_m_vehicleView_flYawMin = 0xd853df21eadf4c2e,
		CPropVehicleDriveable_m_bEnterAnimOn = 0x33ed9ccbb4c5477,
		CPropVehicleDriveable_m_bExitAnimOn = 0x417096f1ac5c6c3b,
		CPropVehicleDriveable_m_bHasGun = 0x9fae514206b8a6b1,
		CPropVehicleDriveable_m_bUnableToFire = 0x96b48d84b7118784,
		CPropVehicleDriveable_m_flThrottle = 0x8926fe7843223a5b,
		CPropVehicleDriveable_m_hPlayer = 0x7414334fd81fd229,
		CPropVehicleDriveable_m_nBoostTimeLeft = 0xf8f6f26514232019,
		CPropVehicleDriveable_m_nHasBoost = 0xaaf9d8af09b0150f,
		CPropVehicleDriveable_m_nRPM = 0x6b178926a9fe5bf4,
		CPropVehicleDriveable_m_nScannerDisabledVehicle = 0xd09507ad36e91bd6,
		CPropVehicleDriveable_m_nScannerDisabledWeapons = 0xc11cec600d916a2c,
		CPropVehicleDriveable_m_nSpeed = 0x4d23d80435f49d4d,
		CPropVehicleDriveable_m_vecEyeExitEndpoint = 0x64727b908a52ba3e,
		CPropVehicleDriveable_m_vecGunCrosshair = 0x36b97162c9ad79d1,
		CProp_Hallucination_m_bEnabled = 0x29d84e43de321083,
		CProp_Hallucination_m_fRechargeTime = 0x522c87f5686dbe6c,
		CProp_Hallucination_m_fVisibleTime = 0x83f02405397fb2de,
		CRagdollManager_m_iCurrentMaxRagdollCount = 0x8485a92cdf8cf3fe,
		CRagdollPropAttached_m_attachmentPointBoneSpace = 0x6ff1cc95a0f1b13e,
		CRagdollPropAttached_m_attachmentPointRagdollSpace = 0x2e020f5b7d03adad,
		CRagdollPropAttached_m_boneIndexAttached = 0x4b88fd8a5b363975,
		CRagdollPropAttached_m_ragdollAttachedObjectIndex = 0xba3b8699438a461,
		CRagdollProp_m_flBlendWeight = 0xb6f49b19e8c640ce,
		CRagdollProp_m_hUnragdoll = 0xc9636fea621f295b,
		CRagdollProp_m_nOverlaySequence = 0x8afd7f827bcc42e,
		CRagdollProp_m_ragAngles_0_ = 0xdf06b42ace146b9a,
		CRagdollProp_m_ragPos_0_ = 0x75bec7ffd5630a81,
		CRopeKeyframe_m_RopeFlags = 0x806d448d8b19bc7b,
		CRopeKeyframe_m_RopeLength = 0xb1b5a98738a475ae,
		CRopeKeyframe_m_Slack = 0xb34cbaf0ddf4f5b2,
		CRopeKeyframe_m_Subdiv = 0x48a4bbd5d8ff86a0,
		CRopeKeyframe_m_TextureScale = 0xdba83b1a8714aca5,
		CRopeKeyframe_m_Width = 0x7f06a3f05dfa7d8f,
		CRopeKeyframe_m_bConstrainBetweenEndpoints = 0x2b9f8021008cfd94,
		CRopeKeyframe_m_fLockedPoints = 0xbc5c13f8f13e8c03,
		CRopeKeyframe_m_flScrollSpeed = 0xc3b481c3ee4d9f4c,
		CRopeKeyframe_m_hEndPoint = 0x12cfde645dd936d4,
		CRopeKeyframe_m_hStartPoint = 0x99b09db76606bcbd,
		CRopeKeyframe_m_iDefaultRopeMaterialModelIndex = 0xd2f94fd59d8ab05b,
		CRopeKeyframe_m_iEndAttachment = 0x326706894a70cd8d,
		CRopeKeyframe_m_iParentAttachment = 0xf7b20e083d278010,
		CRopeKeyframe_m_iRopeMaterialModelIndex = 0x2e4141f5c3d0089d,
		CRopeKeyframe_m_iStartAttachment = 0x825a64a4aac21f4d,
		CRopeKeyframe_m_nChangeCount = 0x2ab101cfdc18ef8a,
		CRopeKeyframe_m_nMaxCPULevel = 0xc8e448268a3b1ce5,
		CRopeKeyframe_m_nMaxGPULevel = 0xa56ad50e8a790550,
		CRopeKeyframe_m_nMinCPULevel = 0x355b7e7fab3a6262,
		CRopeKeyframe_m_nMinGPULevel = 0xa32eb304c840e837,
		CRopeKeyframe_m_nSegments = 0x1e09ad1083e49917,
		CRopeKeyframe_m_vecOrigin = 0xd97c4edaec3bf4f0,
		CRopeKeyframe_moveparent = 0x5b6c733730cc9b22,
		CSceneEntity_m_bIsPlayingBack = 0x128926092432d0f9,
		CSceneEntity_m_bMultiplayer = 0xcf10c477f6f412fa,
		CSceneEntity_m_bPaused = 0x2927b9f2a88ec536,
		CSceneEntity_m_flForceClientTime = 0xfff7a4d2c0b352e0,
		CSceneEntity_m_nSceneStringIndex = 0x1b64802dfd25c852,
		CShadowControl_m_bDisableShadows = 0x1b1d8460450fb6c,
		CShadowControl_m_bEnableLocalLightShadows = 0x595c082aea00dbc2,
		CShadowControl_m_flShadowMaxDist = 0xb024f69d6872d24a,
		CShadowControl_m_shadowColor = 0xebcbc13296a228f4,
		CShadowControl_m_shadowDirection = 0xc07269b40bf10b28,
		CSlideshowDisplay_m_bEnabled = 0xa3db1793e4f42506,
		CSlideshowDisplay_m_bNoListRepeats = 0xed49461366a2ad67,
		CSlideshowDisplay_m_chCurrentSlideLists = 0x499e1dfa8e327049,
		CSlideshowDisplay_m_fMaxSlideTime = 0xbcae45cb40d0f6ef,
		CSlideshowDisplay_m_fMinSlideTime = 0xa091f458fe838b40,
		CSlideshowDisplay_m_iCycleType = 0xda00d9193b633bb3,
		CSlideshowDisplay_m_szDisplayText = 0xdb256aada30ff254,
		CSlideshowDisplay_m_szSlideshowDirectory = 0x7dbfbea642d588b,
		CSmokeGrenadeProjectile_m_bDidSmokeEffect = 0x176d487b9670354e,
		CSmokeGrenadeProjectile_m_nSmokeEffectTickBegin = 0x7e3859f7a3f7021b,
		CSmokeStack_m_AmbientLight_m_flIntensity = 0x9b0351126abdcef2,
		CSmokeStack_m_AmbientLight_m_vColor = 0xf4d703b2fee35423,
		CSmokeStack_m_AmbientLight_m_vPos = 0x9a396fc47fdd8f9a,
		CSmokeStack_m_DirLight_m_flIntensity = 0xa20123e000b5041a,
		CSmokeStack_m_DirLight_m_vColor = 0x4f51f37d91ec5eb,
		CSmokeStack_m_DirLight_m_vPos = 0x259ba63a00450eed,
		CSmokeStack_m_EndSize = 0x90cd19f49b21bfe5,
		CSmokeStack_m_JetLength = 0x7c54f89afa454ae8,
		CSmokeStack_m_Rate = 0x75ed894135efdcb0,
		CSmokeStack_m_Speed = 0x553b65e8c5b06782,
		CSmokeStack_m_SpreadSpeed = 0xd7b8c2a694d51fb1,
		CSmokeStack_m_StartSize = 0xf36305aa947ecae3,
		CSmokeStack_m_bEmit = 0x9871745979c007f4,
		CSmokeStack_m_flBaseSpread = 0x2c612777b1ef5cfa,
		CSmokeStack_m_flRollSpeed = 0x9b321e6cf2b261e9,
		CSmokeStack_m_flTwist = 0x303b18bc81b8456e,
		CSmokeStack_m_iMaterialModel = 0x87625bd3b0f2a538,
		CSmokeStack_m_vWind = 0xbd9b2312ab99d78e,
		CSpatialEntity_m_bEnabled = 0xf2f1ee6e44dee9e4,
		CSpatialEntity_m_flCurWeight = 0x7f4f9c102d3e822,
		CSpatialEntity_m_maxFalloff = 0x588b7dca0ee5e8ae,
		CSpatialEntity_m_minFalloff = 0x8439d0277e52b16d,
		CSpatialEntity_m_vecOrigin = 0xab385a0552038e72,
		CSpotlightEnd_m_Radius = 0x4422c86ddc4b1a93,
		CSpotlightEnd_m_flLightScale = 0x50ef21f0df7dd251,
		CSpriteTrail_m_flEndWidth = 0xa1bc785846ddef76,
		CSpriteTrail_m_flLifeTime = 0xf419304e62d51add,
		CSpriteTrail_m_flMinFadeLength = 0x510f0e274c50b6c0,
		CSpriteTrail_m_flSkyboxScale = 0x193d7a917f29dce9,
		CSpriteTrail_m_flStartWidth = 0x65964ba55f8a2218,
		CSpriteTrail_m_flStartWidthVariance = 0x20a4d5376cc91cde,
		CSpriteTrail_m_flTextureRes = 0x79d4577cadac16a4,
		CSpriteTrail_m_vecSkyboxOrigin = 0x64f367a7bcd6bc54,
		CSprite_m_bWorldSpaceScale = 0xd551154b3b56be69,
		CSprite_m_flBrightnessTime = 0x81bf5479fe9b4b4,
		CSprite_m_flFrame = 0x4a595be44d171832,
		CSprite_m_flGlowProxySize = 0xba1a9f3af0e6960a,
		CSprite_m_flHDRColorScale = 0x3bf8906c558516a1,
		CSprite_m_flScaleTime = 0x14d51a40d029205b,
		CSprite_m_flSpriteFramerate = 0x3b9682de31c6c799,
		CSprite_m_flSpriteScale = 0x178c837245f1af0b,
		CSprite_m_hAttachedToEntity = 0x315cf014bbe38703,
		CSprite_m_nAttachment = 0xb5bd9f523a4b407e,
		CSprite_m_nBrightness = 0xa7444715129beb11,
		CStatueProp_m_bShatter = 0xfb7c7a6048277ab1,
		CStatueProp_m_hInitBaseAnimating = 0xf4047c51f5935611,
		CStatueProp_m_nShatterFlags = 0xa7e49635915bdbce,
		CStatueProp_m_vShatterForce = 0xeafc6c63a8e2e108,
		CStatueProp_m_vShatterPosition = 0x92bc28b2b2f28858,
		CSteamJet_m_EndSize = 0x3f25466fb061e81d,
		CSteamJet_m_JetLength = 0x3804dd9d33972bb7,
		CSteamJet_m_Rate = 0x220523bf2dce690e,
		CSteamJet_m_Speed = 0x546c752d76a36303,
		CSteamJet_m_SpreadSpeed = 0xc63d2ddfc66c9ce1,
		CSteamJet_m_StartSize = 0x2bd7b4f62aa88d2f,
		CSteamJet_m_bEmit = 0x70a4a972795a5727,
		CSteamJet_m_bFaceLeft = 0x6100e9f23bf98c53,
		CSteamJet_m_flRollSpeed = 0x17a5c376372d11b5,
		CSteamJet_m_nType = 0x1a961485d00ccab5,
		CSteamJet_m_spawnflags = 0x953cf5141c3e642d,
		CSun_m_bOn = 0xb63a59b110d1e939,
		CSun_m_clrOverlay = 0xb95bcf2b8f229e10,
		CSun_m_clrRender = 0xba0fb02d483aba0f,
		CSun_m_nMaterial = 0x5a7babd2435e4f41,
		CSun_m_nOverlayMaterial = 0xd449292989489ed9,
		CSun_m_nOverlaySize = 0x985f2193f973618f,
		CSun_m_nSize = 0x5737c0747df0e1c6,
		CSun_m_vDirection = 0xab5686d70a25d2ce,
		CSunlightShadowControl_m_LightColor = 0x5f89bea6cdac292d,
		CSunlightShadowControl_m_TextureName = 0x9773a5884e9c37bd,
		CSunlightShadowControl_m_bEnableShadows = 0xefb23ea66cb7d1a8,
		CSunlightShadowControl_m_bEnabled = 0xa5a11d0128dd3f87,
		CSunlightShadowControl_m_flColorTransitionTime = 0xf3482ebfcbaf47af,
		CSunlightShadowControl_m_flFOV = 0xf6db475753842aa8,
		CSunlightShadowControl_m_flNearZ = 0x5429c9aa63a47d24,
		CSunlightShadowControl_m_flNorthOffset = 0xc179a31f0120c0ac,
		CSunlightShadowControl_m_flSunDistance = 0x230e056abd91e30,
		CSunlightShadowControl_m_shadowDirection = 0x9be848c4d2aab60e,
		CSurvivalSpawnChopper_m_vecOrigin = 0x265a169ba464d77a,
		CSurvivalSpawnChopper_m_vecOrigin_2_ = 0x5c0a901027dcd44,
		CTEBSPDecal_m_nEntity = 0xe75ac1f00eb25741,
		CTEBSPDecal_m_nIndex = 0xb1d425116829b2a8,
		CTEBSPDecal_m_vecOrigin = 0x2e3fdba90e2469eb,
		CTEBaseBeam_a = 0x803b117f7b583edd,
		CTEBaseBeam_b = 0xe82eb88ca3a69e0c,
		CTEBaseBeam_g = 0xa25a30ffb91a4f9a,
		CTEBaseBeam_m_fAmplitude = 0x3ea01fae79fb1cc1,
		CTEBaseBeam_m_fEndWidth = 0x3c4b29d4aa7d3e8c,
		CTEBaseBeam_m_fLife = 0xbd00a224ef56c457,
		CTEBaseBeam_m_fWidth = 0x7297a8e9a178b155,
		CTEBaseBeam_m_nFadeLength = 0x257ce0025ad52584,
		CTEBaseBeam_m_nFlags = 0x57c34090d89dd81f,
		CTEBaseBeam_m_nFrameRate = 0x1e8aa9cf57143f72,
		CTEBaseBeam_m_nHaloIndex = 0xb9ae5856c5e5340f,
		CTEBaseBeam_m_nModelIndex = 0x7109ea13c37daf3,
		CTEBaseBeam_m_nSpeed = 0x95620cf37cc572ac,
		CTEBaseBeam_m_nStartFrame = 0x85e15e5a8be2e6ce,
		CTEBaseBeam_r = 0xed2f917e61e34380,
		CTEBeamEntPoint_m_nEndEntity = 0x78ee153250c41a3f,
		CTEBeamEntPoint_m_nStartEntity = 0x4a54c1bca14ee1a5,
		CTEBeamEntPoint_m_vecEndPoint = 0xcfc159a32e4cd6a2,
		CTEBeamEntPoint_m_vecStartPoint = 0x52bc4b6d86c9f2f8,
		CTEBeamEnts_m_nEndEntity = 0xb0032b342ef32185,
		CTEBeamEnts_m_nStartEntity = 0x31956b165e961520,
		CTEBeamFollow_m_iEntIndex = 0x43807652e07a59e0,
		CTEBeamLaser_m_nEndEntity = 0x8f1e784b29a02eb3,
		CTEBeamLaser_m_nStartEntity = 0x13899a7d38f38e3,
		CTEBeamPoints_m_vecEndPoint = 0xef3f44677d7cd4a6,
		CTEBeamPoints_m_vecStartPoint = 0xce1c5e22490f23fa,
		CTEBeamRingPoint_m_flEndRadius = 0x191548a9d586cc41,
		CTEBeamRingPoint_m_flStartRadius = 0xc3396e76749760dc,
		CTEBeamRingPoint_m_vecCenter = 0xd74788730bf3dbb7,
		CTEBeamRing_m_nEndEntity = 0xbf2dffa7f64ceea9,
		CTEBeamRing_m_nStartEntity = 0xb86427f894358dcb,
		CTEBeamSpline_m_nPoints = 0x72620b595bea37d5,
		CTEBeamSpline_m_vecPoints_0_ = 0x94de1a932fc2a213,
		CTEBloodSprite_a = 0x6f93df5844df1bd6,
		CTEBloodSprite_b = 0x4da3a572f05cd361,
		CTEBloodSprite_g = 0x2c38de7c24a00fdd,
		CTEBloodSprite_m_nDropModel = 0xc87f205ccd13ee0d,
		CTEBloodSprite_m_nSize = 0xfc632b094b906089,
		CTEBloodSprite_m_nSprayModel = 0x2c01338d692db7a2,
		CTEBloodSprite_m_vecDirection = 0x17b12508de69f0f3,
		CTEBloodSprite_m_vecOrigin = 0x20494727753dd679,
		CTEBloodSprite_r = 0x8830d3ff7c3195b7,
		CTEBloodStream_a = 0xa3feda619969513c,
		CTEBloodStream_b = 0xc838fb87d1e7bf24,
		CTEBloodStream_g = 0x59e4885151b3c4b,
		CTEBloodStream_m_nAmount = 0xa5259904afd4bb5d,
		CTEBloodStream_m_vecDirection = 0xec0fc3851679b91e,
		CTEBloodStream_r = 0xa4f778630c78c68e,
		CTEBreakModel_m_angRotation_0_ = 0x3a8b6572275a7cf2,
		CTEBreakModel_m_angRotation_1_ = 0xd932f42685f9aecb,
		CTEBreakModel_m_angRotation_2_ = 0x4e67452c6deae601,
		CTEBreakModel_m_fTime = 0xdb6c09e51b182578,
		CTEBreakModel_m_nCount = 0xfc5c9c1bdcc10aa1,
		CTEBreakModel_m_nFlags = 0x8b04f95763c9fc51,
		CTEBreakModel_m_nModelIndex = 0x2139e6453ace89aa,
		CTEBreakModel_m_nRandomization = 0xb73f0aeb75d798fe,
		CTEBreakModel_m_vecOrigin = 0x18dcba67f7257262,
		CTEBreakModel_m_vecSize = 0xcf9efc8bfe7dc704,
		CTEBreakModel_m_vecVelocity = 0xbf62aa97cf554c6,
		CTEBubbleTrail_m_fSpeed = 0xf6dfbcd6ff009b60,
		CTEBubbleTrail_m_flWaterZ = 0xecdf9627a4fd2445,
		CTEBubbleTrail_m_nCount = 0xbb83c242c1aa6eba,
		CTEBubbleTrail_m_nModelIndex = 0xf2d924730e7dd9a7,
		CTEBubbleTrail_m_vecMaxs = 0x249b422465f04af4,
		CTEBubbleTrail_m_vecMins = 0x29453a1f8c5c0db6,
		CTEBubbles_m_fHeight = 0xd1f8e74d52633257,
		CTEBubbles_m_fSpeed = 0x64d9f1c0495a1725,
		CTEBubbles_m_nCount = 0xbac6a88624252152,
		CTEBubbles_m_nModelIndex = 0x28e6fc8a73c6aa02,
		CTEBubbles_m_vecMaxs = 0x30f0bce4be214f41,
		CTEBubbles_m_vecMins = 0x9d0eeb219837d168,
		CTEClientProjectile_m_hOwner = 0xf9a7d8f651218e43,
		CTEClientProjectile_m_nLifeTime = 0xed22dbb79880314b,
		CTEClientProjectile_m_nModelIndex = 0x2a071755445f4481,
		CTEClientProjectile_m_vecOrigin = 0x3149913f5e4f5c15,
		CTEClientProjectile_m_vecVelocity = 0x93c80c78ea331f49,
		CTEDecal_m_nEntity = 0x883db7234a1af072,
		CTEDecal_m_nHitbox = 0x8fd2bc7c37f7ca51,
		CTEDecal_m_nIndex = 0xc6629c49166b9594,
		CTEDecal_m_vecOrigin = 0x8763036f43b98c78,
		CTEDecal_m_vecStart = 0xcb88ba915abb2217,
		CTEDust_m_flSize = 0x725ae4d54fa69206,
		CTEDust_m_flSpeed = 0xf230306226cc9209,
		CTEDust_m_vecDirection = 0xe14783416111dbd4,
		CTEDynamicLight_b = 0x58557ce8b26f6308,
		CTEDynamicLight_exponent = 0xb2a4e5de6aafd6b6,
		CTEDynamicLight_g = 0x6315b8f4621236de,
		CTEDynamicLight_m_fDecay = 0x96edd688265b1e13,
		CTEDynamicLight_m_fRadius = 0xe1507325053f4aca,
		CTEDynamicLight_m_fTime = 0x1b3805f3688ba8de,
		CTEDynamicLight_m_vecOrigin = 0x42a80de57c788f2e,
		CTEDynamicLight_r = 0xd99b048e5da6494c,
		CTEEffectDispatch_entindex = 0x792b93e59a6f096a,
		CTEEffectDispatch_m_EffectData = 0x1245cc7952461273,
		CTEEffectDispatch_m_bPositionsAreRelativeToEntity = 0x9b27cff60121f71d,
		CTEEffectDispatch_m_fFlags = 0x3a7828a12dbb9d2,
		CTEEffectDispatch_m_flMagnitude = 0xdda4694c42d6fa09,
		CTEEffectDispatch_m_flRadius = 0xe235331f7d831e60,
		CTEEffectDispatch_m_flScale = 0x656776e6fe6dba00,
		CTEEffectDispatch_m_iEffectName = 0x9428ffa158d1eb4b,
		CTEEffectDispatch_m_nAttachmentIndex = 0xa666852cfecb9a1e,
		CTEEffectDispatch_m_nColor = 0x2bcd0c168170cdcc,
		CTEEffectDispatch_m_nDamageType = 0x1e7a3d2efb4aaca2,
		CTEEffectDispatch_m_nHitBox = 0xb5309a5344c5dbda,
		CTEEffectDispatch_m_nMaterial = 0xade4d3c370a10bf5,
		CTEEffectDispatch_m_nOtherEntIndex = 0x4277434e221833b7,
		CTEEffectDispatch_m_nSurfaceProp = 0xbebee4f7a7479055,
		CTEEffectDispatch_m_vAngles = 0x8b5158f4adda6ce2,
		CTEEffectDispatch_m_vNormal = 0x39831144abc0bf58,
		CTEEffectDispatch_m_vOrigin_x = 0xeb96fd9b5bc0c0c5,
		CTEEffectDispatch_m_vOrigin_y = 0x65eaafe6e624168,
		CTEEffectDispatch_m_vOrigin_z = 0x24a218c7a45ba7d0,
		CTEEffectDispatch_m_vStart_x = 0xb62b63930e6d6933,
		CTEEffectDispatch_m_vStart_y = 0xb27984db1a4ac429,
		CTEEffectDispatch_m_vStart_z = 0xeee3d23af0a5730a,
		CTEEnergySplash_m_bExplosive = 0x69e470ae655b01b8,
		CTEEnergySplash_m_vecDir = 0x4f48de4e46bd1173,
		CTEEnergySplash_m_vecPos = 0xc8b6e54f7308a55a,
		CTEExplosion_m_chMaterialType = 0x22170e462e6d6b44,
		CTEExplosion_m_fScale = 0x7b98bc955f9e5532,
		CTEExplosion_m_nFlags = 0xdd9a9f8721092403,
		CTEExplosion_m_nFrameRate = 0x6d06f6b8f15de20c,
		CTEExplosion_m_nMagnitude = 0xdf5f360edf9a6fd1,
		CTEExplosion_m_nModelIndex = 0xecf2d92e71de300f,
		CTEExplosion_m_nRadius = 0x68bfee7ac46db74e,
		CTEExplosion_m_vecNormal = 0xefb56f7e8cfe8ba4,
		CTEFireBullets_m_fInaccuracy = 0x5cf5901351bf6328,
		CTEFireBullets_m_fSpread = 0x5fa19c2df21aad4f,
		CTEFireBullets_m_flRecoilIndex = 0x400e1f489257a717,
		CTEFireBullets_m_iMode = 0x57f4afdec43851aa,
		CTEFireBullets_m_iPlayer = 0xca03f21ae012cefb,
		CTEFireBullets_m_iSeed = 0x577e2cd01e5c05c5,
		CTEFireBullets_m_iSoundType = 0x30ca744e23bc5e80,
		CTEFireBullets_m_iWeaponID = 0x6c7eb86226eab4bb,
		CTEFireBullets_m_nItemDefIndex = 0xfb15b334ce40a762,
		CTEFireBullets_m_vecAngles_0_ = 0x13dfd73b387350c6,
		CTEFireBullets_m_vecAngles_1_ = 0x6e39b23421afca11,
		CTEFireBullets_m_vecOrigin = 0x991c59d8cbf9bf1c,
		CTEFireBullets_m_weapon = 0x830059f72fc1115f,
		CTEFizz_m_nCurrent = 0x173d8ba5872c506e,
		CTEFizz_m_nDensity = 0xa95cdcaa8bc811c7,
		CTEFizz_m_nEntity = 0xa431ffebf6ca23c,
		CTEFizz_m_nModelIndex = 0xbfebb7daefe57c9e,
		CTEFootprintDecal_m_chMaterialType = 0xb1679c7b23bbc814,
		CTEFootprintDecal_m_nEntity = 0xf246dda610c88733,
		CTEFootprintDecal_m_nIndex = 0x3c2b4869ec5f0649,
		CTEFootprintDecal_m_vecDirection = 0x8ccf55828daaf800,
		CTEFootprintDecal_m_vecOrigin = 0xfbfb31256922ff4d,
		CTEFoundryHelpers_m_iEntity = 0xadb5296efe1ec935,
		CTEGaussExplosion_m_nType = 0x3c9d0799e2c9788a,
		CTEGaussExplosion_m_vecDirection = 0x645d10142cf54006,
		CTEGlowSprite_m_fLife = 0x3d777d5cc3a2287,
		CTEGlowSprite_m_fScale = 0x6926256a602cc134,
		CTEGlowSprite_m_nBrightness = 0x1e31973ef61ea877,
		CTEGlowSprite_m_nModelIndex = 0x4671c6b464b342c2,
		CTEGlowSprite_m_vecOrigin = 0x6989879bb9ae38b4,
		CTEImpact_m_iType = 0x9cba74b1e9a29443,
		CTEImpact_m_ucFlags = 0x2da6d654b9b8c886,
		CTEImpact_m_vecNormal = 0x19df801b7aeeb5f7,
		CTEImpact_m_vecOrigin = 0x9952ef1953a73762,
		CTEKillPlayerAttachments_m_nPlayer = 0x6a20120c63da6acc,
		CTELargeFunnel_m_nModelIndex = 0x9a5f52ce5c3e556c,
		CTELargeFunnel_m_nReversed = 0xac676142325a3f7,
		CTEMetalSparks_m_vecDir = 0x13984836b519fad0,
		CTEMetalSparks_m_vecPos = 0xc8f7ee756cf6d376,
		CTEMuzzleFlash_m_flScale = 0x4eee10424b316e29,
		CTEMuzzleFlash_m_nType = 0x1b771205b185a44e,
		CTEMuzzleFlash_m_vecAngles = 0x361ab90c58e0950,
		CTEMuzzleFlash_m_vecOrigin = 0x5362cb611f559b91,
		CTEParticleSystem_m_vecOrigin_0_ = 0xc8e3db8988844462,
		CTEParticleSystem_m_vecOrigin_1_ = 0x486dbc68693c8980,
		CTEParticleSystem_m_vecOrigin_2_ = 0xe7e8752b28a84982,
		CTEPhysicsProp_m_angRotation_0_ = 0x7cbf0194c50ebacd,
		CTEPhysicsProp_m_angRotation_1_ = 0xdfda73a37ec7b0f8,
		CTEPhysicsProp_m_angRotation_2_ = 0x23048c65193da0e1,
		CTEPhysicsProp_m_clrRender = 0xb6fc7c21f0f2c1c3,
		CTEPhysicsProp_m_nEffects = 0x1389db5321e850ff,
		CTEPhysicsProp_m_nFlags = 0xccab4b3facf4879e,
		CTEPhysicsProp_m_nModelIndex = 0xb2ee0db3c53edcc6,
		CTEPhysicsProp_m_nSkin = 0x3efe6c91a5948b1b,
		CTEPhysicsProp_m_vecOrigin = 0x86358611bdc3c303,
		CTEPhysicsProp_m_vecVelocity = 0xc757609d5f43ebcb,
		CTEPlantBomb_m_iPlayer = 0xb0eb1ed6c8dceab9,
		CTEPlantBomb_m_option = 0x9a4dbe1ae21758ab,
		CTEPlantBomb_m_vecOrigin = 0x55a14e1b03bf3215,
		CTEPlayerAnimEvent_m_hPlayer = 0xfa93078637dfa9ce,
		CTEPlayerAnimEvent_m_iEvent = 0x51bc2758c7ebb43,
		CTEPlayerAnimEvent_m_nData = 0x9bd152e873c8034a,
		CTEPlayerDecal_m_nEntity = 0x22dbbf1505ea5a14,
		CTEPlayerDecal_m_nHitbox = 0xbf9485b17e75691b,
		CTEPlayerDecal_m_nPlayer = 0x5245043f3ae9f27,
		CTEPlayerDecal_m_vecOrigin = 0x5431b0409e71949d,
		CTEPlayerDecal_m_vecRight = 0x18e72e78ccac9c8,
		CTEPlayerDecal_m_vecStart = 0x70fe392316857f6c,
		CTEProjectedDecal_m_angRotation = 0x4f50b93886b6f4f4,
		CTEProjectedDecal_m_flDistance = 0xaf18bc205e6b70d8,
		CTEProjectedDecal_m_nIndex = 0xa3f3d62b72a79e83,
		CTEProjectedDecal_m_vecOrigin = 0x69cebbcd1e071c27,
		CTERadioIcon_m_iAttachToClient = 0x13d90f5efc2f06a1,
		CTEShatterSurface_m_flHeight = 0x2f774aa6fc4c3ea8,
		CTEShatterSurface_m_flShardSize = 0x69a0124a656e0364,
		CTEShatterSurface_m_flWidth = 0xb22ebbd25601f66b,
		CTEShatterSurface_m_nSurfaceType = 0x9b6a4d3cc4eae4c9,
		CTEShatterSurface_m_uchBackColor_0_ = 0x3d32ec2422ac306,
		CTEShatterSurface_m_uchBackColor_1_ = 0xb1206d2cda285874,
		CTEShatterSurface_m_uchBackColor_2_ = 0x174b7611847ca47f,
		CTEShatterSurface_m_uchFrontColor_0_ = 0xc08f45eef74c1100,
		CTEShatterSurface_m_uchFrontColor_1_ = 0xbe3cdaafd8f0e3dc,
		CTEShatterSurface_m_uchFrontColor_2_ = 0x59484a2f61b0a760,
		CTEShatterSurface_m_vecAngles = 0x24f08318d08a537c,
		CTEShatterSurface_m_vecForce = 0xe1827e478bdbd254,
		CTEShatterSurface_m_vecForcePos = 0x5607ac0d30e05307,
		CTEShatterSurface_m_vecOrigin = 0x341660443aaefaa,
		CTEShowLine_m_vecEnd = 0x85a732dc897f5f21,
		CTESmoke_m_fScale = 0xaecf3187da8dc21a,
		CTESmoke_m_nFrameRate = 0x967d9da3dffd3fb4,
		CTESmoke_m_nModelIndex = 0xeded141269ffe256,
		CTESmoke_m_vecOrigin = 0x62b70e18a10bbc82,
		CTESparks_m_nMagnitude = 0x194b1e2b6c577296,
		CTESparks_m_nTrailLength = 0x3589c67dd0aed560,
		CTESparks_m_vecDir = 0x98bb042df86e0cf,
		CTESpriteSpray_m_fNoise = 0x487faefdece4e0d8,
		CTESpriteSpray_m_nCount = 0x9e47a58f634aa51,
		CTESpriteSpray_m_nModelIndex = 0x393f94d8eebadf95,
		CTESpriteSpray_m_nSpeed = 0x41d8b73e9ff3c63b,
		CTESpriteSpray_m_vecDirection = 0xd2327d34d57dcf14,
		CTESpriteSpray_m_vecOrigin = 0x850665f97f498fd8,
		CTESprite_m_fScale = 0x2cd04089a6960d21,
		CTESprite_m_nBrightness = 0xa11b1f3eac187c83,
		CTESprite_m_nModelIndex = 0x2487d1e547546d2d,
		CTESprite_m_vecOrigin = 0xeb51726e748778b6,
		CTEWorldDecal_m_nIndex = 0x4a78095b44d05693,
		CTEWorldDecal_m_vecOrigin = 0xf658bd987c23da50,
		CTablet_m_bTabletReceptionIsBlocked = 0x2461808e3d03542e,
		CTablet_m_flBootTime = 0xb1badc9c543a237d,
		CTablet_m_flScanProgress = 0x69b068e540a79bac,
		CTablet_m_flShowMapTime = 0x3452fa0c53a69c3d,
		CTablet_m_flUpgradeExpirationTime_0_ = 0x7bfad4891758b1fa,
		CTablet_m_nContractKillGridHighResIndex = 0x35635fd9005f7fec,
		CTablet_m_nContractKillGridIndex = 0xb95cafca9743209d,
		CTablet_m_nLastPurchaseIndex = 0x33ce730717b8d7fd,
		CTablet_m_vecLocalHexFlags_0_ = 0x6c56b654c7c55da3,
		CTablet_m_vecNearestMetalCratePos = 0x59a1ecdd4ac284ce,
		CTablet_m_vecNotificationIds_0_ = 0x69052887a7d3360f,
		CTablet_m_vecNotificationTimestamps_0_ = 0xbcaef5eb051302de,
		CTablet_m_vecPlayerPositionHistory_0_ = 0x18857281440bf4e4,
		CTeam_m_bSurrendered = 0x361088b51fe30413,
		CTeam_m_iClanID = 0x20cefd0f628561da,
		CTeam_m_iTeamNum = 0x598f2fa9096f5750,
		CTeam_m_nGGLeaderEntIndex_CT = 0xed47eff57faa8088,
		CTeam_m_nGGLeaderEntIndex_T = 0xe5de73625676cc1d,
		CTeam_m_numMapVictories = 0x2db841fdf07b8d35,
		CTeam_m_scoreFirstHalf = 0x8e6bf4cd7c09c93a,
		CTeam_m_scoreOvertime = 0xfe19fe8cc4b99d92,
		CTeam_m_scoreSecondHalf = 0xfa8c2a206f8df686,
		CTeam_m_scoreTotal = 0x34113fa76ed1f9b,
		CTeam_m_szClanTeamname = 0xd902b1804e224f24,
		CTeam_m_szTeamFlagImage = 0x24ab9363902b67c4,
		CTeam_m_szTeamLogoImage = 0xf22b1bf5c1f1f043,
		CTeam_m_szTeamMatchStat = 0x83c9797b567abaf0,
		CTeam_m_szTeamname = 0x1b5ac04a96caf291,
		CTeamplayRoundBasedRulesProxy_m_TeamRespawnWaveTimes = 0xfe2f0a2d7ab692e,
		CTeamplayRoundBasedRulesProxy_m_bAwaitingReadyRestart = 0xd8e3914205a5ab94,
		CTeamplayRoundBasedRulesProxy_m_bInOvertime = 0x6606622c925ab465,
		CTeamplayRoundBasedRulesProxy_m_bInSetup = 0x401a31b1da6ef51,
		CTeamplayRoundBasedRulesProxy_m_bInWaitingForPlayers = 0x6951626a02c9b69,
		CTeamplayRoundBasedRulesProxy_m_bStopWatch = 0x1f889cbe12438fb5,
		CTeamplayRoundBasedRulesProxy_m_bSwitchedTeamsThisRound = 0x2bb964f831df979b,
		CTeamplayRoundBasedRulesProxy_m_bTeamReady = 0xb518e985db811f15,
		CTeamplayRoundBasedRulesProxy_m_flMapResetTime = 0x74c54b67ce0f583d,
		CTeamplayRoundBasedRulesProxy_m_flNextRespawnWave = 0x6e0558a6841c6502,
		CTeamplayRoundBasedRulesProxy_m_flRestartRoundTime = 0x3039ab05f70ba3aa,
		CTeamplayRoundBasedRulesProxy_m_iRoundState = 0x8257aec3d96927b5,
		CTeamplayRoundBasedRulesProxy_m_iWinningTeam = 0x315e0245959c040e,
		CTesla_m_SoundName = 0xa5b9cc7e408058c,
		CTesla_m_iszSpriteName = 0x2ada6c0a2c7e8903,
		CTestTraceline_m_angRotation_0_ = 0x776117d766dbf107,
		CTestTraceline_m_angRotation_1_ = 0x43f146dcfa7d30fb,
		CTestTraceline_m_angRotation_2_ = 0x5b12862581d8ca96,
		CTestTraceline_m_clrRender = 0xefe8ecc6984894e9,
		CTestTraceline_m_vecOrigin = 0x5d232f1059528efd,
		CTestTraceline_moveparent = 0x657d331bc77f5df4,
		CTest_ProxyToggle_Networkable_m_WithProxy = 0xaec4a34b164f29a6,
		CTriggerSoundOperator_m_nSoundOperator = 0xfe952738bcc9378e,
		CVGuiScreen_m_fScreenFlags = 0x55f09eacc83b7f6c,
		CVGuiScreen_m_flHeight = 0x4d5218948519cbf7,
		CVGuiScreen_m_flWidth = 0xc854a47a4d535692,
		CVGuiScreen_m_hPlayerOwner = 0xa47c44d30bd6f045,
		CVGuiScreen_m_nAttachmentIndex = 0xac1c1cdc473712df,
		CVGuiScreen_m_nOverlayMaterial = 0x3db6100463de932e,
		CVGuiScreen_m_nPanelName = 0x53009476f2cd619,
		CVoteController_m_bIsYesNoVote = 0x2a9ab7eccbac29b4,
		CVoteController_m_iActiveIssueIndex = 0xb35a6ca23957009a,
		CVoteController_m_iOnlyTeamToVote = 0x7305d8a314b6d7f1,
		CVoteController_m_nPotentialVotes = 0xff808f47b3f89415,
		CVoteController_m_nVoteOptionCount = 0xc14db084380927fc,
		CWaterLODControl_m_flCheapWaterEndDistance = 0xf941f58001c2785e,
		CWaterLODControl_m_flCheapWaterStartDistance = 0x627b1fe1ce844c0c,
		CWeaponBaseItem_m_bRedraw = 0x300dca9d7a50e6c6,
		CWeaponCSBaseGun_m_iBurstShotsRemaining = 0x334ba8ae78967fa0,
		CWeaponCSBaseGun_m_zoomLevel = 0xa5a361f02977da2,
		CWeaponCSBase_m_bBurstMode = 0x8869271a20309516,
		CWeaponCSBase_m_bReloadVisuallyComplete = 0xc2ca19388d7d3e38,
		CWeaponCSBase_m_bSilencerOn = 0x2c247064f77e460a,
		CWeaponCSBase_m_fAccuracyPenalty = 0xe59404b7fffd6610,
		CWeaponCSBase_m_fLastShotTime = 0x5b7cf4b76fabb6d9,
		CWeaponCSBase_m_flDoneSwitchingSilencer = 0xbd750a9a30e795ac,
		CWeaponCSBase_m_flPostponeFireReadyTime = 0xccba599f38b3ef3c,
		CWeaponCSBase_m_flRecoilIndex = 0x198cc435a1055c8f,
		CWeaponCSBase_m_hPrevOwner = 0xeac4691e129b1a92,
		CWeaponCSBase_m_iIronSightMode = 0x6458f840df8101a7,
		CWeaponCSBase_m_iOriginalTeamNumber = 0x164ccadd1b1b563c,
		CWeaponCSBase_m_iRecoilIndex = 0x19de318720e4ad3d,
		CWeaponCSBase_m_weaponMode = 0x15993d7a1334b2a7,
		CWeaponM3_m_reloadState = 0x8744ea80f114bdb9,
		CWeaponNOVA_m_reloadState = 0xeae6b71a493df03e,
		CWeaponSawedoff_m_reloadState = 0x5d099ff1a5b4c8d8,
		CWeaponShield_m_flDisplayHealth = 0x32fc0320a3bdf53a,
		CWeaponTaser_m_fFireTime = 0x934d1d1dd91fa5c3,
		CWeaponXM1014_m_reloadState = 0xa88e1a4ea3cdd999,
		CWorldVguiText_m_bEnabled = 0x44e3115572b819f5,
		CWorldVguiText_m_clrText = 0xb59d47560bae25d7,
		CWorldVguiText_m_iTextPanelWidth = 0xbb1411958a68cd33,
		CWorldVguiText_m_szDisplayText = 0x13b4a8a9d996682b,
		CWorldVguiText_m_szDisplayTextOption = 0xe64fb605ee37950a,
		CWorldVguiText_m_szFont = 0xd0b7563d455f5ee6,
		CWorld_m_WorldMaxs = 0xe60477fb19c205f7,
		CWorld_m_WorldMins = 0xf405bdf3cd445669,
		CWorld_m_bColdWorld = 0xf5ff9d813e7c4c4f,
		CWorld_m_bStartDark = 0x516276f6ac0cb64b,
		CWorld_m_flMaxOccludeeArea = 0xc57aaa1585172095,
		CWorld_m_flMaxPropScreenSpaceWidth = 0x5c30910aa44c25f6,
		CWorld_m_flMinOccluderArea = 0x1a0d43c54a7c9a7f,
		CWorld_m_flMinPropScreenSpaceWidth = 0xec6f8f04a9c7c809,
		CWorld_m_flWaveHeight = 0x3b91dc9042ec3b35,
		CWorld_m_iTimeOfDay = 0x30b489c7a70566,
		CWorld_m_iszDetailSpriteMaterial = 0x126e51af2f4ee0,
		DustTrail_m_Color = 0x9dc662f492a5c9c4,
		DustTrail_m_EndSize = 0x4d1be212d06b75ad,
		DustTrail_m_MaxDirectedSpeed = 0xad58d91f02bee701,
		DustTrail_m_MaxSpeed = 0x822cd9aa8f037b5b,
		DustTrail_m_MinDirectedSpeed = 0x49021b308ded89fa,
		DustTrail_m_MinSpeed = 0x17664c1582141773,
		DustTrail_m_Opacity = 0xcd6918903d065b19,
		DustTrail_m_ParticleLifetime = 0x252bff61e148cde5,
		DustTrail_m_SpawnRadius = 0xea2b160b7a2406eb,
		DustTrail_m_SpawnRate = 0xce795cd0823f61db,
		DustTrail_m_StartSize = 0xce44ebbda973dd5,
		DustTrail_m_StopEmitTime = 0xb5cf9d531d0a07ca,
		DustTrail_m_bEmit = 0x207facf0ea97b625,
		ParticleSmokeGrenade_m_CurrentStage = 0xd42b1abb34047a9b,
		ParticleSmokeGrenade_m_FadeEndTime = 0x1c31dcb537770483,
		ParticleSmokeGrenade_m_FadeStartTime = 0x7bd100ca99ff7224,
		ParticleSmokeGrenade_m_MaxColor = 0x3ca9123a5f603ce6,
		ParticleSmokeGrenade_m_MinColor = 0xa56503e03b05608a,
		ParticleSmokeGrenade_m_flSpawnTime = 0x354fd9122c36e46c,
		RocketTrail_m_EndColor = 0xbef9a0e7efd94372,
		RocketTrail_m_EndSize = 0x63cd4625177e6055,
		RocketTrail_m_MaxSpeed = 0xdc1efda9f5920955,
		RocketTrail_m_MinSpeed = 0x1facae033a1e5527,
		RocketTrail_m_Opacity = 0xe69769b09128fe55,
		RocketTrail_m_ParticleLifetime = 0xea648e457224487,
		RocketTrail_m_SpawnRadius = 0x76ac5307fa2d346,
		RocketTrail_m_SpawnRate = 0x9668b05728122b64,
		RocketTrail_m_StartColor = 0xbdb3f99ebb87fc3a,
		RocketTrail_m_StartSize = 0xfb3d503e86f52b82,
		RocketTrail_m_StopEmitTime = 0x1af383bed80e25cd,
		RocketTrail_m_bDamaged = 0xe3454e0e4d4b4cc7,
		RocketTrail_m_bEmit = 0x408d97a9966acf08,
		RocketTrail_m_flFlareScale = 0xb25d13abc1bd781f,
		RocketTrail_m_nAttachment = 0x198248160599ad6e,
		SmokeTrail_m_EndColor = 0x73f22472f9120ccf,
		SmokeTrail_m_EndSize = 0x1146422c9970fb7c,
		SmokeTrail_m_MaxDirectedSpeed = 0x1a2fda9a7546a6ce,
		SmokeTrail_m_MaxSpeed = 0xa278d95e49cd9d90,
		SmokeTrail_m_MinDirectedSpeed = 0x189e183c4e1f525d,
		SmokeTrail_m_MinSpeed = 0xc49d9681d2baeafe,
		SmokeTrail_m_Opacity = 0xdbd5889f6910d4d2,
		SmokeTrail_m_ParticleLifetime = 0x4084819ec96b13b6,
		SmokeTrail_m_SpawnRadius = 0x21863073992e73ec,
		SmokeTrail_m_SpawnRate = 0xc05a9c609337963e,
		SmokeTrail_m_StartColor = 0xcf0b814419565998,
		SmokeTrail_m_StartSize = 0x7bfbf11f66b4f76f,
		SmokeTrail_m_StopEmitTime = 0xc1860a00a1db37b,
		SmokeTrail_m_bEmit = 0x57fe68186df3b78f,
		SmokeTrail_m_nAttachment = 0x7f50c6d8e4917c86,
		SporeExplosion_m_bDontRemove = 0x19d8aa5321d2c559,
		SporeExplosion_m_bEmit = 0x58e82619351b43ea,
		SporeExplosion_m_flEndSize = 0xaa3d930f2c416906,
		SporeExplosion_m_flParticleLifetime = 0xe0ff7f50ae6fb81c,
		SporeExplosion_m_flSpawnRadius = 0x46d0c3b0f64fc54e,
		SporeExplosion_m_flSpawnRate = 0x3e3f47f93a3e8b46,
		SporeExplosion_m_flStartSize = 0xf6b3333a934a2868,
		SporeTrail_m_bEmit = 0x1794221d3c307c8b,
		SporeTrail_m_flEndSize = 0x640a43a31e95bcf3,
		SporeTrail_m_flParticleLifetime = 0xed4df615a1a80f84,
		SporeTrail_m_flSpawnRadius = 0xec58d7c03914f76f,
		SporeTrail_m_flSpawnRate = 0xd2dc6af7e986f5b5,
		SporeTrail_m_flStartSize = 0xe37e4ee1ab291072,
		SporeTrail_m_vecEndColor = 0x4482e3d5303efa73,
		string_CCSPlayer_m_bWaitForNoAttack = 0xda811616ad7dedcd,
		string_CCSPlayer_m_iMoveState = 0x180ea4410af59856,
		string_CCSPlayer_m_bStrafing = 0xee1abc1fa5bb3711,
		string_CCSPlayer_m_flVelocityModifier = 0x2d42d42cbc7a2d33,
		string_CCSPlayer_m_flThirdpersonRecoil = 0xc9cad953b1441565,
		string_CBaseCSGrenade_m_fThrowTime = 0x3edefe4582a7b24f,
		string_CBaseCSGrenade_m_bIsHeldByPlayer = 0xcdf1ebdbc4451787,
		COUNT
	};

	enum class module_names : int32_t
	{
		NONE = -1,
		client,
		engine,
		materialsystem,
		vstdlib,
		vphysics,
		vguimatsurface,
		tier0,
		vgui2,
		localize,
		datacache,
		studiorender,
		filesystem_stdio,
		panorama,
		gameoverlayrenderer,
		server,
		v8,
		inputsystem,
		shaderapidx9,
		COUNT
	};
}
