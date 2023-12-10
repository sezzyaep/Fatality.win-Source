
#ifndef GUI_RENDERER_BITFONT_H
#define GUI_RENDERER_BITFONT_H

#include <ren/font.h>

namespace gui
{
	class bitfont;

	class bitglyph : public evo::ren::glyph_base
	{
	public:
		bitglyph() = default;
		bitglyph( const uint32_t g, const std::shared_ptr<bitfont>& f, ABC& abc ) : glyph_index( g ), owner( f ), abc( abc )
		{}

		void create() override;
		void destroy() override;

	private:
		ABC abc{};
		uint32_t glyph_index{};
		std::shared_ptr<bitfont> owner{};

		bool is_created{};

		friend class bitfont;
	};

	class bitfont : public evo::ren::font_base
	{
	public:
		/**
		 * Creates font through GDI
		 * @param name Name of the font
		 * @param size Size in pixels
		 * @param flags Font flags
		 * @param from Starting codepoint
		 * @param to Ending codepoint
		 */
		bitfont( const std::string& name, float size, uint32_t weight = 400, uint32_t flags = evo::ren::ff_none, uint32_t from = 0x00, uint32_t to = 0xFF );
		~bitfont();

		void create() override;
		void destroy() override;

		const evo::ren::glyph_base& find( const uint32_t u ) const override
		{
			return glyphs.find( u ) != glyphs.end() ? glyphs.at( u ) : fallback_glyph;
		}

		float get_advance( uint32_t a, uint32_t b ) override;

	private:
		inline bool glyph_exits( uint32_t g );

		std::string name;

		HDC device{};
		HFONT win_font{};
		HBITMAP dib{};
		TEXTMETRICA metric{};

		uint32_t bitmap_size[ 2 ]{};
		void* buffer{};

		bitglyph fallback_glyph{};
		std::unordered_map<uint32_t, bitglyph> glyphs{};

		friend class bitglyph;
	};
}

#endif // GUI_RENDERER_BITFONT_H
