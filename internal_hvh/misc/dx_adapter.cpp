#include "../include_cheat.h"

using namespace evo;

std::shared_ptr<evo::ren::texture> dx_adapter::load_svg_to_texture( const uint8_t* buffer, size_t size, uint32_t w, uint32_t h )
{
	std::vector<uint32_t> texture( 0xFFFFFF );
	image_data img( texture );

	if ( img.load_svg( buffer, size, &w, &h ) )
		return std::make_shared<evo::ren::texture>( texture.data(), w, h, w * 4 );

	return nullptr;
}

std::optional<dx_adapter::icon_texture> dx_adapter::icon_to_texture( const char* name, uint32_t target_height )
{
	const auto hash = fnv1a_rt( name ) + target_height;

	std::lock_guard<std::mutex> lock( draw_icon_mutex );

	auto ico = icon_textures.find( hash );

	// lazy icon initialization.
	if ( ico == icon_textures.end() )
	{
		// load file from virtual file system.
		size_t size{};
		const auto file = util::load_text_file( name, &size );
		if ( !file )
			return std::nullopt;

		// load in-place to get dimension.
		std::vector<uint32_t> texture( 0xFFFFFF );
		image_data img( texture );
		uint32_t w{}, h{};
		if ( !img.load_svg( file, size, &w, &h ) )
		{
			interfaces::memalloc()->Free( file );
			return std::nullopt;
		}

		// rasterize svg to texture.
		const auto width_to_height = static_cast< float >( w ) / static_cast< float >( h );
		const auto svg = load_svg_to_texture( file, size, target_height ?
				static_cast< uint32_t >( width_to_height * target_height ) : 0, target_height );
		interfaces::memalloc()->Free( file );
		if ( !svg )
			return std::nullopt;

		// insert new entry.
		ico = icon_textures.insert_or_assign( hash, icon_texture{ svg, width_to_height } ).first;
	}

	return ico->second;
}

std::optional<dx_adapter::icon_texture> dx_adapter::custom_icon_to_texture( const char* name, std::string data, uint32_t target_height )
{
	const auto hash = fnv1a_rt( name ) + target_height;

	std::lock_guard<std::mutex> lock( draw_icon_mutex );

	auto ico = icon_textures.find( hash );

	// lazy icon initialization.
	if ( ico == icon_textures.end() )
	{
		// load in-place to get dimension.
		std::vector<uint32_t> texture( 0xFFFFFF );
		image_data img( texture );
		uint32_t w{}, h{};
		if ( !img.load_svg( ( uint8_t* ) data.c_str(), data.size(), &w, &h ) )
			return std::nullopt;

		// rasterize svg to texture.
		const auto width_to_height = static_cast< float >( w ) / static_cast< float >( h );
		const auto svg = load_svg_to_texture( ( uint8_t* ) data.c_str(), data.size(), target_height ?
				static_cast< uint32_t >( width_to_height * target_height ) : 0, target_height );

		if ( !svg )
			return std::nullopt;

		// insert new entry.
		ico = icon_textures.insert_or_assign( hash, icon_texture{ svg, width_to_height } ).first;
	}

	return ico->second;
}

void dx_adapter::weapon( evo::ren::layer& b, const vec2& pos, const float height, const color& clr, const char* name, char ah, char av, bool dropshadow )
{
	std::string complete_path;
	complete_path += _r( "materials\\panorama\\images\\icons\\equipment\\" );
	complete_path += name;
	complete_path += _r( ".svg" );

	// find icon.
	const auto wpn = icon_to_texture( complete_path.c_str(), height );

	if ( !wpn )
		return;

	const auto width = static_cast< size_t >( wpn->width_to_height * height );

	auto adjusted_pos = pos;
	if ( ah > 0 || av > 0 )
	{
		if ( ah == layer::align_center )
			adjusted_pos -= vec2( width * 0.5f, 0.f );
		else if ( ah == layer::align_right )
			adjusted_pos -= vec2( width, 0.f );

		if ( av == layer::align_center )
			adjusted_pos -= vec2( 0.f, height * 0.5f );
		else if ( av == layer::align_bottom )
			adjusted_pos -= vec2( 0.f, height );
	}

	const auto from = vec2{ adjusted_pos.x, adjusted_pos.y };
	const auto to = vec2{ adjusted_pos.x + width, adjusted_pos.y + height };

	b.push_texture( wpn->texture );

	if ( dropshadow )
		b.add_rect_filled( ren::rect( { from.x + 1, from.y + 1 }, { to.x + 1, to.y + 1 } ).floor(), ren::color( 0, 0, 0, static_cast< uint8_t >( .6f * static_cast< float >( clr.a() ) ) ) );
	b.add_rect_filled( ren::rect( { from.x, from.y }, { to.x, to.y } ).floor(), clr );
	b.pop_texture();
}

void dx_adapter::icon( evo::ren::layer& b, const vec2& pos, const float height, const color& clr, const char* name, char ah, char av, bool dropshadow )
{
	std::string complete_path;
	complete_path += _r( "materials\\panorama\\images\\" );
	complete_path += name;
	complete_path += _r( ".svg" );

	// find icon.
	const auto wpn = icon_to_texture( complete_path.c_str(), height );

	if ( !wpn )
		return;

	const auto width = static_cast< size_t >( wpn->width_to_height * height );

	auto adjusted_pos = pos;
	if ( ah > 0 || av > 0 )
	{
		if ( ah == layer::align_center )
			adjusted_pos -= vec2( width * 0.5f, 0.f );
		else if ( ah == layer::align_right )
			adjusted_pos -= vec2( width, 0.f );

		if ( av == layer::align_center )
			adjusted_pos -= vec2( 0.f, height * 0.5f );
		else if ( av == layer::align_bottom )
			adjusted_pos -= vec2( 0.f, height );
	}

	const auto from = vec2{ adjusted_pos.x, adjusted_pos.y };
	const auto to = vec2{ adjusted_pos.x + width, adjusted_pos.y + height };

	b.push_texture( wpn->texture );

	if ( dropshadow )
		b.add_rect_filled( ren::rect( { from.x + 1, from.y + 1 }, { to.x + 1, to.y + 1 } ).floor(), ren::color( 0, 0, 0, static_cast< uint8_t >( .6f * static_cast< float >( clr.a() ) ) ) );
	b.add_rect_filled( ren::rect( { from.x, from.y }, { to.x, to.y } ).floor(), clr );
	b.pop_texture();
}

void dx_adapter::custom_icon( evo::ren::layer& b, const vec2& pos, const float height, const color& clr, const char* name, char ah, char av, bool dropshadow )
{

	_u( data, icon_alias.at( fnv1a_rt( name ) ) );
	// find icon.
	const auto wpn = custom_icon_to_texture( name, data, height );

	if ( !wpn )
		return;

	const auto width = static_cast< size_t >( wpn->width_to_height * height );

	auto adjusted_pos = pos;
	if ( ah > 0 || av > 0 )
	{
		if ( ah == layer::align_center )
			adjusted_pos -= vec2( width * 0.5f, 0.f );
		else if ( ah == layer::align_right )
			adjusted_pos -= vec2( width, 0.f );

		if ( av == layer::align_center )
			adjusted_pos -= vec2( 0.f, height * 0.5f );
		else if ( av == layer::align_bottom )
			adjusted_pos -= vec2( 0.f, height );
	}

	const auto from = vec2{ adjusted_pos.x, adjusted_pos.y };
	const auto to = vec2{ adjusted_pos.x + width, adjusted_pos.y + height };

	b.push_texture( wpn->texture );

	if ( dropshadow )
		b.add_rect_filled( ren::rect( { from.x + 1, from.y + 1 }, { to.x + 1, to.y + 1 } ).floor(), ren::color( 0, 0, 0, static_cast< uint8_t >( .6f * static_cast< float >( clr.a() ) ) ) );
	b.add_rect_filled( ren::rect( { from.x, from.y }, { to.x, to.y } ).floor(), clr );
	b.pop_texture();
}

float dx_adapter::get_weapon_width( const float height, const char* name )
{
	std::string complete_path;
	complete_path += _r( "materials\\panorama\\images\\icons\\equipment\\" );
	complete_path += name;
	complete_path += _r( ".svg" );

	// find icon.
	const auto wpn = icon_to_texture( complete_path.c_str(), height );

	if ( !wpn )
		return 0.f;

	return static_cast< size_t >( wpn->width_to_height * height );
}

float dx_adapter::get_icon_width( const float height, const char* name )
{
	std::string complete_path;
	complete_path += _r( "materials\\panorama\\images\\" );
	complete_path += name;
	complete_path += _r( ".svg" );

	// find icon.
	const auto wpn = icon_to_texture( complete_path.c_str(), height );

	if ( !wpn )
		return 0.f;

	return static_cast< size_t >( wpn->width_to_height * height );
}

void dx_adapter::swap_buffers()
{
	buf.set_draw_mode( ren::layer::dm_noflush );
	buf.swap( ren::draw.get_layer( esp_layer ) );
	ren::draw.get_layer( esp_layer ).set_draw_mode( buf.get_draw_mode() );
}

void dx_adapter::clear_buffer()
{
	buf.clear();
}