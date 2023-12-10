#ifndef EV0LVE_RENDERER_VERTEX_H
#define EV0LVE_RENDERER_VERTEX_H

#include <ren/types.h>

namespace evo::ren
{
	class vertex
	{
	public:
		vertex(const vec2& p, const color& c, const vec2& uv = {0.f, 0.f}) : pos(p), col(c), uv(uv)
		{}

		vertex set_uv(const vec2& u) const
		{
			return vertex(pos, col, u);
		}
		vertex set_color(const color& c) const
		{
			return vertex(pos, c, uv);
		}
		vertex set_pos(const vec2& p) const
		{
			return vertex(p, col, uv);
		}

		inline vec2 get_uv() const
		{
			return uv;
		}
		inline color get_color() const
		{
			return col;
		}
		inline vec2 get_pos() const
		{
			return pos;
		}

	private:
		vec2 pos{};
		vec2 uv{};
		uint32_t col{};
	};

	class vertex_constant_buffer
	{
	public:
		float mvp[4][4]{};
	};
}

#endif //EV0LVE_RENDERER_VERTEX_H
