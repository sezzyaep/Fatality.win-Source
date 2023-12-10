#pragma once

#include "../includes.h"
#include "d3dfont.h"


namespace fonts
{
	extern std::shared_ptr<c_font> esp;
	extern std::shared_ptr<c_font> lby;

	extern std::shared_ptr<c_font> title_font;
	extern std::shared_ptr<c_font> glitch_font;
	extern std::shared_ptr<c_font> tab_font;
	extern std::shared_ptr<c_font> child_title;
	extern std::shared_ptr<c_font> control;
	extern std::shared_ptr<c_font> control_unicode;
}

class Color;

namespace render
{
	struct vertex
	{
		float x, y, z, rhw;
		uint32_t color;
	};

	class vertex_dx9
	{
	public:
		vertex_dx9( const vertex& vtx )
		{
			const auto p = Vector2D( vtx.x, vtx.y ) - 0.5f;

			pos[ 0 ] = p.x;
			pos[ 1 ] = p.y;
			uv[ 0 ] = vtx.z;
			uv[ 1 ] = vtx.rhw;
			col = vtx.color;
		}

	private:
		float pos[ 3 ]{};
		uint32_t col{};
		float uv[ 2 ]{};
	};

	struct command;
	struct layer
	{
		std::vector<command> 	commands{};
		std::vector<vertex> 	vertices{};
		std::vector<uint16_t> 	indices{};
	};

	struct command
	{
		command( layer* l );

		bool 					anti_alias{};	// Should enforce anti-aliasing
		uint32_t 				vertices{};		// Vertex count
		uint32_t 				indices{};		// Index count
		void* texture{};		// Texture to be applied
		RECT 					clip{};			// Clip rect
		layer* buffer{};

		/**
		 * Adds vertex to layer
		 * @param vtx Vertex info
		 */
		void add_vertex( const vertex& vtx )
		{
			if ( !buffer )
				return;

			vertices++;
			buffer->vertices.emplace_back( vtx );
		}

		template<class... V>
		void add_vertex( const vertex& vtx, const V... vtxs )
		{
			add_vertex( vtx );
			add_vertex( vtxs... );
		}

		/**
		 * Adds index to layer
		 * @param idx Index number
		 */
		void add_index( const uint16_t idx )
		{
			if ( !buffer )
				return;

			indices++;

			buffer->indices.emplace_back( idx );
		}
		template<typename... N>
		void add_index( uint16_t idx, const N... idxs )
		{
			add_index( idx );
			add_index( idxs... );
		}
	};

	inline layer render_layer{};
	inline std::deque<bool> aa_state{};

	void load_fonts();
	void remove_fonts();
	void init( IDirect3DDevice9* dev );
	void limit( Vector2D pos, Vector2D size, bool override = false );
	void limit_lua( Vector2D pos, Vector2D size, bool intersect = true );
	void reset_limit();
	void line( Vector2D from, Vector2D to, Color color );
	void line_gradient( Vector2D from, Vector2D to, Color color, Color color2 );
	void rect( Vector2D from, Vector2D size, Color color );
	void rect_linear_gradient( Vector2D from, Vector2D size, Color color1, Color color2, bool horizontal = false );
	void rect_filled( Vector2D from, Vector2D size, Color color );
	void rect_filled_linear_gradient( Vector2D from, Vector2D size, Color color1, Color color2, bool horizontal = false );
	void rect_filled_radial_gradient_test( Vector2D from, Vector2D size, Color color1, Color color2 );
	void rect_filled_radial_gradient( Vector2D from, Vector2D size, Color color1, Color color2 );
	void rect_filled_diamond_gradient( Vector2D from, Vector2D size, Color color1, Color color2 );
	void rect_filled_hue( Vector2D from, Vector2D size );
	void triangle( Vector2D pos1, Vector2D pos2, Vector2D pos3, Color color );
	void triangle_filled( Vector2D pos1, Vector2D pos2, Vector2D pos3, Color color );
	void triangle_filled_linear_gradient( Vector2D pos1, Vector2D pos2, Vector2D pos3, Color color1, Color color2, Color color3 );
	void circle( Vector2D center, float radius, Color color );
	void circle_progress( Vector2D center, float radius, Color color, float progress );
	void circle_filled( Vector2D center, float radius, Color color );
	void circle_filled_radial_gradient( Vector2D center, float radius, Color color1, Color color2 );
	void circle_filled_rainbow( Vector2D center, float radius, float* hue, float brightness = 1.f, float alpha = 255.f, float alpha2 = 255.f );
	void filled_triangle( POINT pos1, POINT pos2, POINT pos3, Color color );
	void filled_box( const int x, const int y, const int w, const int h, const Color color );
	void filled_box_outlined( const int x, const int y, const int w, const int h, const Color color, const Color outline, const int thickness = 1 );
	void bordered_box( const int x, const int y, const int w, const int h, const Color color, const int thickness = 1 );
	void bordered_box_outlined( const int x, const int y, const int w, const int h, const Color color, const Color outline, const int thickness = 1 );
	void filled_cube( const Vector origin, const int width, const int height, Color outline, Color filling );
	void rect_filled_multi_gradient( Vector2D from, Vector2D size, Color col_top_left, Color col_top_right, Color col_bot_left, Color col_bot_right );
	void render_texture( LPDIRECT3DTEXTURE9 texture, const Vector2D& pos, std::size_t width, std::size_t height, const Color& color, uint8_t flags = 0 );
	void space_texture( const Vector2D& pos, const Vector2D& size );
	Color get_pixel_color( const Vector2D& pos, bool render = false );
	void text( Vector2D pos, std::wstring string, Color color, std::shared_ptr<c_font>& font, uint8_t flags = 0 );
	void text( Vector2D pos, std::string string, Color color, std::shared_ptr<c_font>& font, uint8_t flags = 0 );
	void text_radial( Vector2D pos, std::wstring string, Color color1, Color color2, std::shared_ptr<c_font>& font, uint8_t flags = 0 );
	void text_radial( Vector2D pos, std::string string, Color color1, Color color2, std::shared_ptr<c_font>& font, uint8_t flags = 0 );
	Vector2D get_text_size( std::string string, std::shared_ptr<c_font>& font );
	Vector2D get_text_size( std::wstring string, std::shared_ptr<c_font>& font );
	void scale( Vector2D& vec );
	Vector2D get_center();
	D3DVIEWPORT9 get_dimensions();
	void init_device_objects( IDirect3DDevice9* dev );
	void invalidate_device_objects();
	void setup_render_state( IDirect3DDevice9* device );
	void build_lookup_table();

	void draw_stream();

	inline std::mutex mtx{};
	inline void swap_buffers( layer& buffer )
	{
		std::lock_guard _lock( mtx );
		std::swap( render_layer, buffer );
	}

	inline IDirect3DVertexBuffer9* vertex_buffer{};
	inline IDirect3DIndexBuffer9* index_buffer{};
	inline IDirect3DTexture9* texture_object{};

	inline size_t vb_size{ 5000 };
	inline size_t ib_size{ 10000 };

	inline static constexpr auto points = 64;
	inline IDirect3DDevice9* dev{};
	inline D3DVIEWPORT9 port{};
	inline std::vector<Vector2D> lookup_table{};
	inline std::vector<std::shared_ptr<c_font>> fonts{};
	inline std::deque<RECT> m_scissor_records{};
	inline bool m_init = false;
	inline LPDIRECT3DTEXTURE9 override_texture{};
	inline RECT get_current_scissor_record() { return m_scissor_records.front(); }
	inline std::vector<HANDLE> mem_font_hadles = {};
	static LPD3DXSPRITE m_sprite_texture = {};

	inline std::vector<std::pair<uint32_t, std::shared_ptr<anim_base>>> lua_animators{};
	inline std::mutex lua_animators_mutex;
	inline std::vector<std::pair<uint32_t, std::shared_ptr<c_font>>> lua_fonts{};
	inline std::mutex lua_font_idx_mutex;
	inline uint32_t get_lua_font_idx()
	{
		lua_font_idx_mutex.lock();
		lua_fonts.push_back( {} );
		const auto ret = lua_fonts.size() - 1;
		lua_fonts.back().first++;
		lua_font_idx_mutex.unlock();
		return ret;
	}
	inline uint32_t get_lua_animators_idx()
	{
		lua_animators_mutex.lock();
		lua_animators.push_back( {} );
		const auto ret = lua_animators.size() - 1;
		lua_animators.back().first++;
		lua_animators_mutex.unlock();
		return ret;
	}

	enum tex_flags
	{
		centered_x = 1 << 0,
		centered_y = 1 << 1,
		left_aligned = 1 << 2,
		right_aligned = 1 << 3,
		top_aligned = 1 << 4,
		bottom_aligned = 1 << 5,
		outline = 1 << 6,
		drop_shadow = 1 << 7
	};
};

namespace render
{
	struct image_data_t
	{
	public:
		[[nodiscard]] auto allocate( std::vector<uint32_t>& v, const void* file_data, std::size_t file_size, std::size_t& width, std::size_t& height, float scale = 1.0f ) noexcept -> bool
		{
			static auto root = make_offset( "panorama.dll", sig_construct_image );
			memset( data.data(), 0, data.size() );
			*reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) ) = reinterpret_cast< uintptr_t >( v.data() );
			*reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x4 ) = v.size() * sizeof( uint32_t );
			*reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x24 ) = *reinterpret_cast< uintptr_t* >( root - 0xC );
			*reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( this ) + 0x28 ) = *reinterpret_cast< uintptr_t* >( root - 0x4 );

			//reinterpret_cast< void( __thiscall* )( void*, size_t ) >( *reinterpret_cast< uintptr_t* >( root + 0x1 ) + root + 0x5 ) ( this, data.size() );

			return parse( file_data, file_size, &width, &height, scale );
		}

	private:
		[[nodiscard]] auto parse( const void* file_data, std::size_t file_size, std::size_t* const width, std::size_t* const height, float scale = 1.f ) noexcept -> bool
		{
			auto fn = make_offset( "panorama.dll", sig_parse_svg );
			uint16_t magic[] = { 0x33, 0x34, 0x35, 0x5c, 0x37, 0x33, 0x30, 0x5c, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x5c, 0x70, 0x61, 0x6e, 0x6f, 0x72, 0x61, 0x6d, 0x61, 0x00 };
			auto rval = false;
			auto thisptr = this;

			__asm
			{
				mov edx, file_size
				lea ecx, magic
				push ecx
				push scale
				push height
				push width
				mov ecx, thisptr
				push ecx
				mov ecx, file_data
				call fn
				add esp, 0x14
				mov rval, al
			}

			return rval;
		}

		std::array<uint8_t, 0x2C> data{};
	};

	struct texture_t
	{
		[[nodiscard]] constexpr auto build( const unsigned char* src ) noexcept -> bool
		{
			if ( ptexture )
				return true;

			if ( dev->CreateTexture( width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &ptexture, nullptr ) != D3D_OK )
				return false;

			D3DLOCKED_RECT lockedRect;
			if ( ptexture->LockRect( 0, &lockedRect, nullptr, D3DLOCK_DISCARD ) != D3D_OK )
				return false;

			std::uint8_t* dst{ reinterpret_cast< std::uint8_t* >( lockedRect.pBits ) };
			for ( decltype( height ) y{}; y < height; ++y )
			{
				const auto step{ width * sizeof( width ) };
				std::copy( src, src + step, dst );
				src += step;
				dst += lockedRect.Pitch;
			}

			if ( ptexture->UnlockRect( 0 ) != D3D_OK )
				return false;

			return true;
		}

		std::size_t width{}, height{};
		LPDIRECT3DTEXTURE9 ptexture{};
	};

	struct weapon_icon_t
	{
		constexpr weapon_icon_t( const std::string& name, const std::size_t height ) noexcept
		{
			std::string complete_path;
			complete_path += _r( "materials\\panorama\\images\\icons\\equipment\\" );
			complete_path += name;
			complete_path += _r( ".svg" );

			if ( const auto file{ interfaces::file_system()->Open( complete_path.c_str(), _r( "r" ), _r( "GAME" ) ) } )
			{
				if ( const auto file_size{ interfaces::file_system()->Size( file ) } )
				{
					const auto file_data = interfaces::memalloc()->Alloc( file_size );
					if ( interfaces::file_system()->Read( file_data, file_size, file ) )
					{
						std::vector<uint32_t> v1( 0xFFFFFF );
						if ( image.allocate( v1, file_data, file_size, texture.width, texture.height ) )
						{
							const auto scale = height / static_cast< float >( texture.height );

							v1.clear();
							v1.resize( 0xFFFFFF );
							if ( image.allocate( v1, file_data, file_size, texture.width, texture.height, scale ) )
							{
								if ( !texture.build( reinterpret_cast< uint8_t* >( v1.data() ) ) )
									SAFE_RELEASE( texture.ptexture )
							}
							else
								SAFE_RELEASE( texture.ptexture )
						}
						else
							SAFE_RELEASE( texture.ptexture )
					}
					interfaces::memalloc()->Free( file_data );
				}
				interfaces::file_system()->Close( file );
			}
		}

		~weapon_icon_t() noexcept
		{
			SAFE_RELEASE( texture.ptexture );
		}

		image_data_t image{};
		texture_t texture{};
	};

	struct custom_icon_t
	{
		custom_icon_t( const std::string& data, const std::size_t height ) noexcept
		{
			std::vector<uint32_t> v1( 0xFFFFFF );
			if ( image.allocate( v1, data.c_str(), data.size(), texture.width, texture.height ) )
			{
				const auto scale = height / static_cast< float >( texture.height );

				v1.clear();
				v1.resize( 0xFFFFFF );
				if ( image.allocate( v1, data.c_str(), data.size(), texture.width, texture.height, scale ) )
				{
					if ( !texture.build( reinterpret_cast< uint8_t* >( v1.data() ) ) )
						SAFE_RELEASE( texture.ptexture )
				}
				else
					SAFE_RELEASE( texture.ptexture )
			}
			else
				SAFE_RELEASE( texture.ptexture )
		}

		~custom_icon_t() noexcept
		{
			SAFE_RELEASE( texture.ptexture );
		}

		image_data_t image{};
		texture_t texture{};
	};

	inline tsl::robin_map<uint32_t, std::shared_ptr<weapon_icon_t>> m_weapon_icons;
	inline tsl::robin_map<uint32_t, std::shared_ptr<custom_icon_t>> m_custom_icons;

	inline std::mutex m_draw_icon_mutex{};

	Vector2D weapon_size( std::string& name, const std::size_t height );
	void draw_weapon( const Vector2D& pos, const std::size_t height, const Color& color, const std::string& name, uint8_t flags = 0 );
	void draw_custom( const Vector2D& pos, const std::size_t height, const Color& color, const std::string& name, uint8_t flags = 0 );

	inline tsl::robin_map<uint32_t, std::string> m_icon_alias =
	{
		{ FNV1A( "taser" ), _w( "<svg id=\"svg\" version=\"1.1\" width=\"608\" height=\"689\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" ><g id=\"svgg\"><path id=\"path0\" d=\"M185.803 18.945 C 184.779 19.092,182.028 23.306,174.851 35.722 C 169.580 44.841,157.064 66.513,147.038 83.882 C 109.237 149.365,100.864 163.863,93.085 177.303 C 88.686 184.901,78.772 202.072,71.053 215.461 C 63.333 228.849,53.959 245.069,50.219 251.505 C 46.480 257.941,43.421 263.491,43.421 263.837 C 43.421 264.234,69.566 264.530,114.025 264.635 L 184.628 264.803 181.217 278.618 C 179.342 286.217,174.952 304.128,171.463 318.421 C 167.974 332.714,160.115 364.836,153.999 389.803 C 147.882 414.770,142.934 435.254,143.002 435.324 C 143.127 435.452,148.286 428.934,199.343 364.145 C 215.026 344.243,230.900 324.112,234.619 319.408 C 238.337 314.704,254.449 294.276,270.423 274.013 C 286.397 253.750,303.090 232.582,307.519 226.974 C 340.870 184.745,355.263 166.399,355.263 166.117 C 355.263 165.937,323.554 165.789,284.798 165.789 C 223.368 165.789,214.380 165.667,214.701 164.831 C 215.039 163.949,222.249 151.366,243.554 114.474 C 280.604 50.317,298.192 19.768,298.267 19.444 C 298.355 19.064,188.388 18.576,185.803 18.945 \" stroke=\"none\" fill=\"#ffffff\" fill-rule=\"evenodd\"></path></g></svg>" ) },
	};

}