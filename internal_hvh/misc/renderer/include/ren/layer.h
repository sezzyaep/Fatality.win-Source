#ifndef EV0LVE_RENDERER_LAYER_H
#define EV0LVE_RENDERER_LAYER_H

#include <vector>
#include <memory>
#include <optional>
#include <array>

#include <ren/texture.h>
#include <ren/types.h>
#include <ren/vertex.h>
#include <ren/font.h>
#include <ren/command.h>

namespace evo::ren
{
	class layer
	{
	public:
		enum
		{
			background = 0,
			foreground = 255
		};

		enum : char
		{
			dm_normal,
			dm_custom,
			dm_noflush
		};

		enum : char
		{
			align_left,
			align_top = align_left,
			align_center,
			align_right,
			align_bottom = align_right
		};

		enum : char
		{
			dir_up,
			dir_down,
			dir_left,
			dir_right
		};

		enum
		{
			rnd_tl = 1 << 0,
			rnd_tr = 1 << 1,
			rnd_bl = 1 << 2,
			rnd_br = 1 << 3,
			rnd_t = rnd_tl | rnd_tr,
			rnd_b = rnd_bl | rnd_br,
			rnd_l = rnd_tl | rnd_bl,
			rnd_r = rnd_tr | rnd_br,
			rnd_all = rnd_tl | rnd_tr | rnd_br | rnd_bl
		};

		layer& push_texture( const std::shared_ptr<texture>& tex )
		{
			stack_textures.emplace_back( tex );
			return *this;
		}

		layer& push_texture_raw( void* tex )
		{
			stack_textures.emplace_back( std::make_shared<texture>( tex ) );
			return *this;
		}

		layer& pop_texture()
		{
			if ( !stack_textures.empty() )
				stack_textures.pop_back();
			return *this;
		}

		layer& push_clip( const rect& r, const bool intersect = true )
		{
			stack_clips.emplace_back( intersect ? ( stack_clips.empty() ? r : stack_clips.back().intersect( r ) ) : r );
			return *this;
		}

		layer& pop_clip()
		{
			if ( !stack_clips.empty() )
				stack_clips.pop_back();
			return *this;
		}

		layer& push_uv( const rect& v )
		{
			stack_uvs.emplace_back( v );
			return *this;
		}

		layer& pop_uv()
		{
			if ( !stack_uvs.empty() )
				stack_uvs.pop_back();
			return *this;
		}

		layer& push_alpha( const float a )
		{
			stack_alphas.emplace_back( a );
			return *this;
		}

		layer& push_alpha( const uint8_t a )
		{
			stack_alphas.emplace_back( ( float ) a / 255.f );
			return *this;
		}

		layer& pop_alpha()
		{
			if ( !stack_alphas.empty() )
				stack_alphas.pop_back();
			return *this;
		}

		layer& push_rot( const float uvr )
		{
			stack_rots.emplace_back( uvr );
			return *this;
		}

		layer& pop_rot()
		{
			if ( !stack_rots.empty() )
				stack_rots.pop_back();
			return *this;
		}

		layer& push_aa( const bool b )
		{
			stack_aa.emplace_back( b );
			return *this;
		}

		layer& pop_aa()
		{
			if ( !stack_aa.empty() )
				stack_aa.pop_back();
			return *this;
		}

		layer& push_no_dpi( const bool b )
		{
			stack_no_dpi.emplace_back( b );
			return *this;
		}

		layer& pop_no_dpi()
		{
			if ( !stack_no_dpi.empty() )
				stack_no_dpi.pop_back();
			return *this;
		}

		layer& add_empty();
		layer& add_quad_filled( const vec2& a, const vec2& b, const vec2& c, const vec2& d, const color& col );
		layer& add_rect_filled( const rect& r, const color& c );
		layer& add_text( const std::shared_ptr<font_base>& f, const vec2& p, const std::string& text, const color& col, char ah = align_left, char av = align_top );
		layer& add_line( const vec2& a, const vec2& b, const color& col, float thickness = 1.f );
		layer& add_rect( const rect& r, const color& c, float thickness = 1.f );
		layer& add_circle_filled( const vec2& c, float r, const color& col, int segments = 64, float fill = 1.f, float rot = 0.f );
		layer& add_circle( const vec2& c, float r, const color& col, float thickness = 1.f, int segments = 64, float fill = 1.f, float rot = 0.f );
		layer& add_rect_filled_linear_gradient( const rect& r, color color1, color color2, bool horizontal = false );
		layer& add_rect_filled_radial_gradient( const rect& r, color color1, color color2 );
		layer& add_rect_filled_multicolor( const rect& r, const std::array<color, 4>& cs );
		layer& add_quad_filled_multicolor( const vec2& a, const vec2& b, const vec2& c, const vec2& d, const std::array<color, 4>& cs );
		layer& add_line_multicolor( const vec2& a, const vec2& b, const std::array<color, 2>& cs, float thickness = 1.f );
		layer& add_quad( const vec2& a, const vec2& b, const vec2& c, const vec2& d, const color& col, float thickness = 1.f );
		layer& add_quad_multicolor( const vec2& a, const vec2& b, const vec2& c, const vec2& d, const std::array<color, 4>& cs, float thickness = 1.f );
		layer& add_rect_multicolor( const rect& r, const std::array<color, 4>& cs, float thickness = 1.f );
		layer& add_circle_filled_multicolor( const vec2& c, float r, const std::array<color, 2>& cs, int segments = 64, float fill = 1.f, float rot = 0.f );
		layer& add_triangle( const vec2& a, const vec2& b, const vec2& c, const color& col, float thickness = 1.f );
		layer& add_triangle_filled( const vec2& a, const vec2& b, const vec2& c, const color& col );
		layer& add_triangle_multicolor( const vec2& a, const vec2& b, const vec2& c, const std::array<color, 3>& cs, float thickness = 1.f );
		layer& add_triangle_filled_multicolor( const vec2& a, const vec2& b, const vec2& c, const std::array<color, 3>& cs );
		layer& add_shadow( const rect& r, char dir, float a = 0.25f );
		layer& add_shadow( const rect& r, float radius, float alpha = 0.25f, bool bg = false );
		layer& add_rect_rounded( const rect& r, const color& c, float f, char rnd = rnd_all, float thickness = 1.f );
		layer& add_rect_filled_rounded( const rect& r, const color& c, float f, char rnd = rnd_all );

		static inline std::unordered_map<int, std::vector<vec2>> lookup_table{};
		static void build_lookup_table( int segments )
		{
			if ( !lookup_table[ segments ].empty() )
				return;

			for ( auto i = 0; i <= segments; i++ )
				lookup_table[ segments ].emplace_back(
					static_cast< float >( cos( 2.f * 3.14159265358979323846 * ( i / static_cast< float >( segments ) ) ) ),
					static_cast< float >( sin( 2.f * 3.14159265358979323846 * ( i / static_cast< float >( segments ) ) ) )
				);
		}

		std::optional<std::shared_ptr<texture>> get_texture() const
		{
			return stack_textures.empty() ?
				std::nullopt :
				std::optional{ stack_textures.back() };
		}

		std::optional<rect> get_clip() const
		{
			return stack_clips.empty() ? std::nullopt : std::optional<rect>{ stack_clips.back() };
		}

		std::optional<rect> get_uv() const
		{
			return stack_uvs.empty() ? std::nullopt : std::optional<rect>{ stack_uvs.back() };
		}

		std::optional<float> get_alpha() const
		{
			return stack_alphas.empty() ? std::nullopt : std::optional<float>{ stack_alphas.back() };
		}

		std::optional<float> get_rot() const
		{
			return stack_rots.empty() ? std::nullopt : std::optional<float>{ stack_rots.back() };
		}

		std::optional<bool> get_aa() const
		{
			return stack_aa.empty() ? std::nullopt : std::optional<bool>{ stack_aa.back() };
		}

		std::optional<bool> get_no_dpi() const
		{
			return stack_no_dpi.empty() ? std::nullopt : std::optional<bool>{ stack_no_dpi.back() };
		}

		void add_command( const command& cmd );

		inline void clear()
		{
			auto cap_com = commands.capacity();
			auto cap_vert = vertices.capacity();
			auto cap_idx = indices.capacity();
			commands.clear();
			vertices.clear();
			indices.clear();
			commands.reserve( cap_com );
			vertices.reserve( cap_vert );
			indices.reserve( cap_idx );
		}

		inline void swap( layer& o )
		{
			commands.swap( o.commands );
			vertices.swap( o.vertices );
			indices.swap( o.indices );
		}

		inline void set_draw_mode( char dm )
		{
			draw_mode = dm;
		}

		inline char get_draw_mode() const
		{
			return draw_mode;
		}

		std::vector<command> 	commands{};
		std::vector<vertex> 	vertices{};
		std::vector<uint16_t> 	indices{};

	private:
		char draw_mode{};

		std::vector<std::shared_ptr<texture>> 	stack_textures{};
		std::vector<rect> 	stack_clips{};
		std::vector<rect> 	stack_uvs{};
		std::vector<float> 	stack_alphas{};
		std::vector<float>	stack_rots{};
		std::vector<bool>	stack_aa{};
		std::vector<bool>	stack_no_dpi{};
	};
}

#endif //EV0LVE_RENDERER_LAYER_H
