#ifndef EV0LVE_RENDERER_FONT_H
#define EV0LVE_RENDERER_FONT_H

#include <string>
#include <optional>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>
#include <unordered_map>
#include <ren/texture.h>
#include <ren/types.h>

struct stbtt_fontinfo;

namespace evo::ren
{
	class font;
	class font_base;

	enum font_flags
	{
		ff_none = 0,
		ff_shadow = 1 << 0,
		ff_outline = 1 << 1,
		ff_antialias = 1 << 3,
	};

	class glyph_base
	{
	public:
		glyph_base() = default;

		virtual void create() {}
		virtual void destroy() {}

		vec2 						size{};
		vec2 						offset{};
		vec2						extra_offset{};
		vec2						first_offset{};
		rect						bmp_uv{ 0.f, 0.f, 1.f, 1.f };
		std::shared_ptr<texture> 	bmp{};

	protected:
		struct pixel
		{
			pixel( uint8_t* data ) : pd( data )
			{}

			bool is_full_white()
			{
				return red() == 0xFF && green() == 0xFF && blue() == 0xFF && alpha() == 0xFF;
			}

			void write( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
			{
				red() = r;
				green() = g;
				blue() = b;
				alpha() = a;
			}

			void blend( pixel& px, pixel& o )
			{
				const auto a = std::clamp( 1.f - ( 1.f - px.p_a() ) * ( 1.f - p_a() ), 0.f, 1.f );
				if ( a < 1.0e-6f )
				{
					o.alpha() = 0;
					return;
				}

				const auto r = px.p_r() * px.p_a() / a + p_r() * p_a() * ( 1.f - px.p_a() ) / a;
				const auto g = px.p_g() * px.p_a() / a + p_g() * p_a() * ( 1.f - px.p_a() ) / a;
				const auto b = px.p_b() * px.p_a() / a + p_b() * p_a() * ( 1.f - px.p_a() ) / a;

				o.write( r * 255.f, g * 255.f, b * 255.f, a * 255.f );
			}

			float p_r()
			{
				return red() / 255.f;
			}

			float p_g()
			{
				return green() / 255.f;
			}

			float p_b()
			{
				return blue() / 255.f;
			}

			float p_a()
			{
				return alpha() / 255.f;
			}

			uint8_t& red()
			{
				return pd[ 0 ];
			}

			uint8_t& green()
			{
				return pd[ 1 ];
			}

			uint8_t& blue()
			{
				return pd[ 2 ];
			}

			uint8_t& alpha()
			{
				return pd[ 3 ];
			}

			uint8_t* pd;
		};

		void apply_effect_shadow( uint8_t* buffer, uint32_t w, uint32_t h );
		void apply_effect_outline( uint8_t* buffer, uint32_t w, uint32_t h );
	};

	/**
	 * Font glyph class
	 */
	class glyph : public glyph_base
	{
	public:
		glyph() = default;
		glyph( const uint32_t g, const std::shared_ptr<font>& f ) : glyph_index( g ), owner( f )
		{}

		void create() override;
		void destroy() override;

	private:
		uint32_t glyph_index{};
		std::shared_ptr<font> owner{};

		bool is_created{};
	};

	class font_base : public std::enable_shared_from_this<font_base>
	{
	public:
		virtual void create() = 0;
		virtual void destroy() = 0;

		/**
		 * Searches a glyph in
		 * @param u Codepoint
		 * @return Glyph reference
		 */
		virtual const glyph_base& find( const uint32_t u ) const = 0;

		/**
		 * Returns font advance value
		 * @param a Previous codepoint
		 * @param b Next codepoint
		 * @return Advance
		 */
		virtual float get_advance( uint32_t a, uint32_t b ) = 0;

		template<typename T>
		std::shared_ptr<T> as()
		{
			return std::static_pointer_cast< T >( shared_from_this() );
		}

		float height{};
		float scale{};

		uint32_t flags{};
		uint32_t weight{};
		uint32_t mins{};
		uint32_t maxs{};

		float ascent{};
		float descent{};
		float line_gap{};

	protected:
		uint64_t kerning_index( uint32_t a, uint32_t b )
		{
			struct ki
			{
				ki( uint32_t v1, uint32_t v2 ) : _1( v1 ), _2( v2 )
				{}

				union
				{
					uint64_t v{};
					struct
					{
						uint32_t _1;
						uint32_t _2;
					};
				};
			};

			return ki( a, b ).v;
		}

		std::unordered_map<uint64_t, float> kerning_cache{};
	};

	/**
	 * Font wrapper class
	 */
	class font : public font_base
	{
	public:
		/**
		 * Creates font from TTF/OTF file
		 * @param path Path to file
		 * @param size Size in pixels
		 * @param flags Font flags
		 * @param from Starting codepoint
		 * @param to Ending codepoint
		 */
		font( const std::string& path, float size, uint32_t flags = ff_none, uint32_t from = 0x00, uint32_t to = 0xFF );

		/**
		 * Creates font from memory (TTF/OTF format)
		 * @param mem Memory buffer
		 * @param sz Buffer size
		 * @param size Size in pixels
		 * @param flags Font flags
		 * @param from Starting codepoint
		 * @param to Ending codepoint
		 */
		font( void* mem, size_t sz, float size, uint32_t flags = ff_none, uint32_t from = 0x00, uint32_t to = 0xFF );

		~font();

		void create() override;
		void destroy() override;

		const glyph_base& find( const uint32_t u ) const override
		{
			return glyphs.find( u ) != glyphs.end() ? glyphs.at( u ) : fallback_glyph;
		}

		float get_advance( uint32_t a, uint32_t b ) override;

		/**
		 * Returns stb font into pointer
		 * @return Font info
		 */
		stbtt_fontinfo* get_font_info()
		{
			return fnt;
		}

	private:
		stbtt_fontinfo* fnt{};
		uint8_t* font_data{}; size_t font_size{};

		glyph fallback_glyph{};
		std::unordered_map<uint32_t, glyph> glyphs{};
	};
}

#endif //EV0LVE_RENDERER_FONT_H
