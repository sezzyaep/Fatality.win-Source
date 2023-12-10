
#ifndef LUA_API_DEF_H
#define LUA_API_DEF_H

#include <lua/state.h>

namespace lua
{
	template <typename T>
	T* extract_type( runtime_state& s, const std::string& error = "", int pos = 1 )
	{
		if ( !s.check_arguments( { { ltd::user_data } } ) )
		{
			s.error( error );
			return nullptr;
		}

		return *reinterpret_cast< T** >( s.get_user_data( 1 ) );
	}

	inline color extract_color( state& s, int pos = 5 )
	{
		return {
			s.get_field_integer( XOR_STR( "r" ), pos ),
			s.get_field_integer( XOR_STR( "g" ), pos ),
			s.get_field_integer( XOR_STR( "b" ), pos ),
			s.get_field_integer( XOR_STR( "a" ), pos )
		};
	}
}

namespace lua::api_def
{
	int panic( lua_State* l );
	int print( lua_State* l );
	int require( lua_State* l );
	int loadfile( lua_State* l );

	int stub_new_index( lua_State* l );
	int stub_index( lua_State* l );
	int fatality_index( lua_State* l );
	int server_index( lua_State* l );
	int global_vars_index( lua_State* l );
	int game_rules_index( lua_State* l );

	namespace math
	{
		// math.vec3(x, y = x, z = x)
		int vec3_new( lua_State* l );

		// vec3:length()
		int vec3_length( lua_State* l );

		// vec3:length2d()
		int vec3_length2d( lua_State* l );

		// vec3:dist(other)
		int vec3_dist( lua_State* l );

		// vec3:dist2d(other)
		int vec3_dist2d( lua_State* l );

		// vec3:to2d()
		int vec3_to2d( lua_State* l );

		// vec3:dot(other)
		int vec3_dot( lua_State* l );

		// vec3:cross(other)
		int vec3_cross( lua_State* l );

		// vec3:normalize()
		int vec3_normalize( lua_State* l );

		int vec3_index( lua_State* l );
		int vec3_new_index( lua_State* l );
		int vec3_add( lua_State* l );
		int vec3_sub( lua_State* l );
		int vec3_mul( lua_State* l );
		int vec3_div( lua_State* l );
		int vec3_calc_angle( lua_State* l );
		int vec3_unpack( lua_State* l );

		int vector_angles( lua_State* l );
		int angle_vectors( lua_State* l );
	}

	namespace fs
	{
		// fs.read(path: string): string
		int read( lua_State* l );

		// fs.read(path: string): table[char]
		int read_stream( lua_State* l );

		// fs.write(path: string, data: string)
		int write( lua_State* l );

		// fs.write(path: string, data: table[char])
		int write_stream( lua_State* l );

		// fs.remove(path: string)
		int remove( lua_State* l );

		// fs.exists(path: string): bool
		int exists( lua_State* l );

		// fs.is_file(path: string): bool
		int is_file( lua_State* l );

		// fs.is_dir(path: string): bool
		int is_dir( lua_State* l );

		// fs.create_dir(path: string)
		int create_dir( lua_State* l );
	}

	namespace zip
	{
		int gc( lua_State* l );

		// zip:read(path: string): string
		int read( lua_State* l );

		// zip:read_stream(path: string): table[char]
		int read_stream( lua_State* l );

		// zip:write(path: string, data: string)
		int write( lua_State* l );

		// zip:write_stream(path: string, data: table[char])
		int write_stream( lua_State* l );

		// zip:save(path: string)
		int save( lua_State* l );

		// zip:get_files(): table[...]
		int get_files( lua_State* l );

		// zip:exists(path: string): bool
		int exists( lua_State* l );

		// zip:extract(path: string, dest: string)
		int extract( lua_State* l );

		// zip:extract_all(dest: string)
		int extract_all( lua_State* l );

		// zip.create(): zip
		int create( lua_State* l );

		// zip.open(path: string): zip
		int open( lua_State* l );

		// zip.open_stream(data: table[char]): zip
		int open_stream( lua_State* l );
	}

	namespace timer
	{
		// tmr:start()
		int start( lua_State* l );

		// tmr:stop()
		int stop( lua_State* l );

		// tmr:run_once()
		int run_once( lua_State* l );

		// tmr:set_delay(time: float)
		int set_delay( lua_State* l );

		// tmr:is_active(): bool
		int is_active( lua_State* l );
	}

	namespace event_log
	{
		// add(color: integer, text: string)
		int add( lua_State* l );

		// output()
		int output( lua_State* l );
	}

	namespace render
	{
		// create_font(font_path, size, flags = none, from = 0, to = 255): font_id
		int create_font( lua_State* l );

		// create_font_stream(bytes_table, size, flags = none, from = 0, to = 255): font_id
		int create_font_stream( lua_State* l );

		// create_font_gdi(name, size, flags = none, from = 0, to = 255): font_id
		int create_font_gdi( lua_State* l );

		// create_texture(tex_path): number
		int create_texture( lua_State* l );

		// create_texture_svg(path/contents, target_height): number
		int create_texture_svg( lua_State* l );

		// create_texture_bytes(bytes, size): number
		int create_texture_bytes( lua_State* l );

		// create_texture_stream(bytes_table)
		int create_texture_stream( lua_State* l );

		// create_texture(bytes, w, h, stride)
		int create_texture_rgba( lua_State* l );

		// create_animator_float(initial, time, interpolation = linear): anim_float
		int create_animator_float( lua_State* l );

		// direct(to) or direct(from, to)
		int anim_float_direct( lua_State* l );

		// get_value(): float
		int anim_float_get_value( lua_State* l );

		// create_animator_color(initial, time, interpolation = linear, interp_hue = false): anim_color
		int create_animator_color( lua_State* l );

		// direct(to) or direct(from, to)
		int anim_color_direct( lua_State* l );

		// get_value(): table
		int anim_color_get_value( lua_State* l );

		// push_texture(tex_id)
		int push_texture( lua_State* l );

		// pop_texture()
		int pop_texture( lua_State* l );

		// push_clip_rect(x1, y1, x2, y2, intersect = true)
		int push_clip_rect( lua_State* l );

		// pop_clip_rect()
		int pop_clip_rect( lua_State* l );

		// push_uv(x1, y1, x2, y2)
		int push_uv( lua_State* l );

		// pop_uv()
		int pop_uv( lua_State* l );

		// get_texture_size(tex): number, number
		int get_texture_size( lua_State* l );

		// get_text_size(font, text): number, number
		int get_text_size( lua_State* l );

		// color(r, g, b, a = 255)
		// color(hex)
		int color( lua_State* l );

		// rect_filled(x1, y1, x2, y2, color: { r, g, b, a })
		int rect_filled( lua_State* l );

		// rect(x1, y1, x2, y2, color: { r, g, b, a })
		int rect( lua_State* l );

		// rect_filled_rounded(x1, y1, x2, y2, color: { r, g, b, a }, rounding, sides = all)
		int rect_filled_rounded( lua_State* l );

		// rect_filled_multicolor(x1, y1, x2, y2, tl, tr, br, bl)
		int rect_filled_multicolor( lua_State* l );

		// circle(x, y, radius, color: { r, g, b, a }, thickness = 1, segments = 12, fill = 1.0, rot = 0.0)
		int circle( lua_State* l );

		// circle_filled(x, y, radius, color: { r, g, b, a }, segments = 12, fill = 1.0, rot = 0.0)
		int circle_filled( lua_State* l );

		// line(x1, y1, x2, y2, color)
		int line( lua_State* l );

		// line_multicolor(x1, y1, x2, y2, col1, col2)
		int line_multicolor( lua_State* l );

		// triangle(x1, y2, x2, y2, x3, y3, color)
		int triangle( lua_State* l );

		// triangle_filled(x1, y2, x2, y2, x3, y3, color)
		int triangle_filled( lua_State* l );

		// triangle_filled_multicolor(x1, y2, x2, y2, x3, y3, col1, col2, col3)
		int triangle_filled_multicolor( lua_State* l );

		// text(font, x, y, text, color: { r, g, b, a }, align_h = left, align_v = top)
		int text( lua_State* l );

		// get_screen_size(): float, float
		int get_screen_size( lua_State* l );

		// esp_flag(text, color): esp_flag
		int esp_flag( lua_State* l );
	}

	namespace utils
	{
		// random_int(min, max)
		int random_int( lua_State* l );

		// random_float(min, max)
		int random_float( lua_State* l );

		// flags(num...)
		int flags( lua_State* l );

		// find_interface(module, name)
		int find_interface( lua_State* l );

		// find_pattern(module, pattern)
		int find_pattern( lua_State* l );

		// get_weapon_info(index)
		int get_weapon_info( lua_State* l );

		int create_timer( lua_State* l, bool run = false );

		// new_timer(delay, function)
		int new_timer( lua_State* l );

		// run_delayed(delay, function)
		int run_delayed( lua_State* l );

		// world_to_screen(x, y, z): x, y | nil
		int world_to_screen( lua_State* l );

		// get_rtt(): number
		int get_rtt( lua_State* l );

		// get_time(): string
		int get_time( lua_State* l );

		// http_get(url, headers, cbk)
		int http_get( lua_State* l );

		// http_post(url, headers, body, cbk)
		int http_post( lua_State* l );

		// json_decode(json)
		int json_decode( lua_State* l );

		// json_encode(table)
		int json_encode( lua_State* l );

		// set_clan_tag(tag)
		int set_clan_tag( lua_State* l );

		// trace(from, to): trace_t
		int trace( lua_State* l );

		// trace_bullet(item_definition_index, from, to): damage, trace_t
		int trace_bullet( lua_State* l );

		// scale_damage(damage, item_definition_index, hit_group, armor, heavy_armor, helmet): damage
		int scale_damage( lua_State* l );

		// load_file(path): string
		int load_file( lua_State* l );

		// print_console(string, color = white)
		int print_console( lua_State* l );

		// print_dev_console(string)
		int print_dev_console( lua_State* l );

		// error_print(string)
		int error_print( lua_State* l );

		// aes256_encrypt(key, data)
		int aes256_encrypt( lua_State* l );

		// aes256_decrypt(key, data)
		int aes256_decrypt( lua_State* l );

		// base64_encode(data)
		int base64_encode( lua_State* l );

		// base64_decode(data)
		int base64_decode( lua_State* l );

		// get_unix_time()
		int get_unix_time( lua_State* l );
	}

	namespace database
	{
		int save( lua_State* l );
		int load( lua_State* l );
	}

	namespace gui
	{
		// is_menu_open(): bool
		int is_menu_open( lua_State* l );

		// get_menu_rect(): number, number, number, number
		int get_menu_rect( lua_State* l );

		// add_checkbox(name, path): cfg.value
		int add_checkbox( lua_State* l );

		// add_slider(name, min, max, step, path): cfg.value
		int add_slider( lua_State* l );

		// add_combo(name, path, {item, ...}): cfg.value
		int add_combo( lua_State* l );

		// add_multi_combo(name, path, {item, ...}): cfg.value...
		int add_multi_combo( lua_State* l );

		// add_button(name, function, path)
		int add_button( lua_State* l );

		// add_textbox(name, path): cfg.value
		int add_textbox( lua_State* l );

		// add_listbox(name, path, items_to_show, search_bar, {item, ...}): cfg.value
		int add_listbox( lua_State* l );

		// add_keybind(path)
		int add_keybind( lua_State* l );

		// add_colorpicker(path, alpha_bar): cfg.value
		int add_colorpicker( lua_State* l );

		// 
		int get_keybind( lua_State* l );

		// get_listbox_items(path): {item, ...}
		int get_listbox_items( lua_State* l );

		// set_listbox_items(path, {item, ...})
		int set_listbox_items( lua_State* l );

		// get_combo_items(path): {item, ...}
		int get_combo_items( lua_State* l );

		// set_combo_items(path, {item, ...})
		int set_combo_items( lua_State* l );

		// get_config_item(path): cfg.value
		int get_config_item( lua_State* l );

		// set_visible(bool, path)
		int set_visible( lua_State* l );
	}

	namespace input
	{
		// is_key_down(k)
		int is_key_down( lua_State* l );

		// get_cursor_pos(): int, int
		int get_cursor_pos( lua_State* l );
	}

	namespace entities
	{
		// entity_list[id]
		int index( lua_State* l );

		// get_entity(id): ent
		int get_entity( lua_State* l );

		// get_entity_from_handle(id): ent
		int get_entity_from_handle( lua_State* l );

		// for_each(func)
		int for_each( lua_State* l );

		// for_each_z(func)
		int for_each_z( lua_State* l );

		// for_each_player(func)
		int for_each_player( lua_State* l );

		// for_each_player_z(func)
		int for_each_player_z( lua_State* l );
	}

	namespace entity
	{
		bool is_sane( C_BaseEntity* ent );

		// ent:index(): int
		int get_index( lua_State* l );

		// ent:is_valid(): bool
		int is_valid( lua_State* l );

		// ent:is_alive(): bool
		int is_alive( lua_State* l );

		// ent:is_dormant(): bool
		int is_dormant( lua_State* l );

		// ent:is_player(): bool
		int is_player( lua_State* l );

		// ent:is_enemy(): bool
		int is_enemy( lua_State* l );

		// ent:get_class(): string
		int get_class( lua_State* l );

		// ent:get_prop(prop): type
		int get_prop( lua_State* l );

		// ent:set_prop(prop, value)
		int set_prop( lua_State* l );

		// ent:get_hitbox_position(hb): vec3
		int get_hitbox_position( lua_State* l );

		// ent:get_eye_position(): vec3
		int get_eye_position( lua_State* l );

		// ent:get_player_info(): player_info
		int get_player_info( lua_State* l );

		// ent:get_move_type(): int
		int get_move_type( lua_State* l );

		// ent:get_bbox(): number, number, number, number
		int get_bbox( lua_State* l );

		// ent:get_weapon(): entity
		int get_weapon( lua_State* l );

		// ent:get_esp_alpha(): float
		int get_esp_alpha( lua_State* l );
	}

	namespace engine
	{
		// is_in_game(): bool
		int is_in_game( lua_State* l );

		// exec(cmd)
		int exec( lua_State* l );

		// get_local_player(): int
		int get_local_player( lua_State* l );

		// get_view_angles(): float, float, float
		int get_view_angles( lua_State* l );

		// get_player_for_user_id(user_id): int
		int get_player_for_user_id( lua_State* l );

		// get_player_info(id): table
		int get_player_info( lua_State* l );

		// set_view_angles(x, y)
		int set_view_angles( lua_State* l );
	}

	namespace cvar
	{
		// cvar['name']: csgo.cvar
		int index( lua_State* l );

		int get_int( lua_State* l );
		int set_int( lua_State* l );
		int get_float( lua_State* l );
		int set_float( lua_State* l );
		int get_string( lua_State* l );
		int set_string( lua_State* l );
	}

	namespace event
	{
		int index( lua_State* l );
		int get_name( lua_State* l );
		int get_bool( lua_State* l );
		int get_int( lua_State* l );
		int get_float( lua_State* l );
		int get_string( lua_State* l );
	}

	namespace user_cmd
	{
		int get_command_number( lua_State* l );
		int get_view_angles( lua_State* l );
		int set_view_angles( lua_State* l );
		int get_move( lua_State* l );
		int set_move( lua_State* l );
		int get_buttons( lua_State* l );
		int set_buttons( lua_State* l );
	}

	namespace panorama
	{
		int eval( lua_State* l );
	}

	namespace mat
	{
		int gc( lua_State* l );
		int index( lua_State* l );

		// material_var:get_float()
		int get_float( lua_State* l );

		// material_var:set_float(value: number)
		int set_float( lua_State* l );

		// material_var:get_int()
		int get_int( lua_State* l );

		// material_var:set_int(value: number)
		int set_int( lua_State* l );

		// material_var:get_string()
		int get_string( lua_State* l );

		// material_var:set_string(value: string)
		int set_string( lua_State* l );

		// material_var:get_vector()
		int get_vector( lua_State* l );

		// material_var:set_vector(value: table)
		int set_vector( lua_State* l );

		// material:modulate(color: table)
		int modulate( lua_State* l );

		// material:set_flag(flag: enum, val: bool)
		int set_flag( lua_State* l );

		// material:get_flag(flag: enum)
		int get_flag( lua_State* l );

		// material:find_var(name: string): material_var
		int find_var( lua_State* l );

		// material:get_name(): string
		int get_name( lua_State* l );

		// material:get_group(): string
		int get_group( lua_State* l );

		// mat.create(name: string, type: string, kv: string): material
		int create( lua_State* l );

		// mat.find(nam: stringe, group: string): material
		int find( lua_State* l );

		// mat.for_each_material(callback: function(material))
		int for_each_material( lua_State* l );

		// mat.override_material(mat/nil)
		int override_material( lua_State* l );
	}

	namespace cfg_value
	{
		// get_int(): int
		int get_int( lua_State* l );

		// get_bool(): bool
		int get_bool( lua_State* l );

		// get_float(): number
		int get_float( lua_State* l );

		// get_color(): color
		int get_color( lua_State* l );

		// get_string(): string
		int get_string( lua_State* l );

		// set_int(int)
		int set_int( lua_State* l );

		// set_bool(bool)
		int set_bool( lua_State* l );

		// set_float(number):
		int set_float( lua_State* l );

		// set_color(color)
		int set_color( lua_State* l );

		// set_string(string)
		int set_string( lua_State* l );
	}
}

#endif // LUA_API_DEF_H
