#ifndef EV0LVE_RENDERER_ANIM_H
#define EV0LVE_RENDERER_ANIM_H

#include <cmath>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <ren/types.h>

#define INTERP(n) inline static float interp_##n(float a, float b, float u)
#define LINEAR(u) interp_linear(a, b, u);

namespace evo::ren
{
	constexpr auto int_pi = 3.14159265358979323846f;
	constexpr auto int_pi2 = 1.57079632679489661923f;

	enum : char
	{
		i_linear,
		i_ease_in,
		i_ease_out,
		i_ease_in_out,
		i_elastic_in,
		i_elastic_out,
		i_elastic_in_out,
		i_bounce_in,
		i_bounce_out,
		i_bounce_in_out,
		i_max
	};

	INTERP(linear)
	{
		return std::clamp((1.f - u) * a + u * b, a > b ? b : a, b > a ? b : a);
	}

	INTERP(ease_in)
	{
		return LINEAR( u * u * u );
	}

	INTERP(ease_out)
	{
		return LINEAR( 1.f - powf( 1.f - u, 3.f ) );
	}

	INTERP(ease_in_out)
	{
		return LINEAR( u < 0.5f ? 4.f * u * u * u : 1.f - powf( -2.f * u + 2.f, 3.f ) / 2.f );
	}

	INTERP(elastic_in)
	{
		const auto t = u * u;
		return LINEAR(t * t * sinf(u * 3.1415926 * 4.5f));
	}

	INTERP(elastic_out)
	{
		const auto t = (u - 1.f) * (u - 1.f);
		return LINEAR(1.f - t * t * cosf(u * 3.1415926 * 4.5f));
	}

	INTERP(elastic_in_out)
	{
		if (u < 0.45f)
		{
			const auto t = u * u;
			return LINEAR(8.f * t * t * sinf(u * 3.1415926 * 9.f));
		}
		else if (u < 0.55f)
		{
			return LINEAR(0.5f + 0.75f * sinf(u * 3.1415926 * 4.f));
		}
		else
		{
			const auto t = (u - 1.f) * (u - 1.f);
			return LINEAR(1.f - 8.f * t * t * sinf(u * 3.1415926 * 9.f));
		}
	}

	INTERP(bounce_in)
	{
		return LINEAR(powf(2.f, 6.f * (u - 1.f)) * abs(sinf(u * 3.1415926 * 3.5f)));
	}

	INTERP(bounce_out)
	{
		return LINEAR(1.f - powf(2.f, -6.f * u) * abs(cosf(u * 3.1415926 * 3.5f)));
	}

	INTERP(bounce_in_out)
	{
		if (u < 0.5f)
			return LINEAR(8.f * powf(2.f, 8.f * (u - 1.f)) * abs(sinf(u * 3.1415926 * 7.f)));

		return LINEAR(1.f - 8.f * powf(2.f, -8.f * u) * abs(sinf(u * 3.1415926 * 7.f)));
	}

	inline std::unordered_map<char, std::function<float(float, float, float)>> interp_functions{
			{ i_linear,  			interp_linear },
			{ i_ease_in, 			interp_ease_in },
			{ i_ease_out, 		interp_ease_out },
			{ i_ease_in_out, 		interp_ease_in_out },
			{ i_elastic_in, 		interp_elastic_in },
			{ i_elastic_out, 		interp_elastic_out },
			{ i_elastic_in_out, 	interp_elastic_in_out },
			{ i_bounce_in, 		interp_bounce_in },
			{ i_bounce_out, 		interp_bounce_out },
			{ i_bounce_in_out, 	interp_bounce_in_out },
	};

	/**
	 * Animation base class
	 */
	class anim_base : public std::enable_shared_from_this<anim_base>
	{
	public:
		/**
		 * Interpolates variable based on frametime
		 * @param ft Frametime
		 */
		virtual void animate(float ft){}

		template<typename T>
		std::shared_ptr<T> as()
		{
			return std::static_pointer_cast<T>(shared_from_this());
		}
	};

	template<typename T>
	class anim : public anim_base
	{
	public:
		/**
		 * Animation constructor
		 * @param v Base value
		 * @param d Animation time (in seconds)
		 * @param i Interpolation type (i_*)
		 */
		anim(const T& v, float d, char i = i_linear) : value(v), end(v), start(v), duration(d), interpolation(i)
		{}

		void animate(float ft) override
		{
			if (on_started && time == 0.f)
				on_started(as<anim<T>>());

			if (on_finished)
			{
				if (time == 1.f)
				{
					if (!did_call_finish)
						on_finished(as<anim<T>>());

					did_call_finish = true;
				} else
					did_call_finish = false;
			}

			if (duration > 0.f && ft > 0.f)
			{
				time += 1.f / duration * ft;
				if (time > 1.f)
					time = 1.f;
			} else
				time = 1.f;
		}

		/**
		 * Directs interpolation into new value (offsets from current value)
		 * @param t New value
		 */
		virtual void direct(const T& t)
		{
			start = value;
			end = t;

			time = 0.f;
		}

		/**
		 * Directs interpolation into new value (offsets from a custom value)
		 * @param a New start
		 * @param t New value
		 */
		virtual void direct(const T& a, const T& t)
		{
			start = a;
			end = t;

			time = 0.f;
		}

		T value{};
		T end{};
		T start{};

		std::function<void(std::shared_ptr<anim<T>>)> on_started{};		// Called when interpolation has been started
		std::function<void(std::shared_ptr<anim<T>>)> on_finished{};	// Called when interpolation has been ended

		char interpolation{};
		float duration{};

	protected:
		float i(float a, float b) const
		{
			return interp_functions[interpolation](a, b, time);
		}

		float time{};

	private:
		bool did_call_finish{};
	};

	/**
	 * Shorthand for float values
	 */
	class anim_float : public anim<float>
	{
	public:
		anim_float(const float& v, float d, char i = i_linear) : anim<float>(v, d, i)
		{}

		void animate(float ft) override
		{
			anim::animate(ft);
			value = i(start, end);
		}
	};

	/**
	 * Shorthand for vec2 values
	 */
	class anim_vec2 : public anim<vec2>
	{
	public:
		anim_vec2(const vec2& v, float d, char i = i_linear) : anim<vec2>(v, d, i)
		{}

		void animate(float ft) override
		{
			anim::animate(ft);
			value.x = i(start.x, end.x);
			value.y = i(start.y, end.y);
		}
	};

	enum anim_color_type : char
	{
		act_rgba,
		act_hsva
	};

	/**
	 * Shorthand for color values
	 */
	class anim_color : public anim<color>
	{
	public:
		anim_color(const color& v, float d, char i = i_linear) : anim<color>(v, d, i)
		{}

		void animate(float ft) override
		{
			anim::animate(ft);

			if (type == act_rgba)
			{
				value.value.x = i(start.value.x, end.value.x);
				value.value.y = i(start.value.y, end.value.y);
				value.value.z = i(start.value.z, end.value.z);
				value.value.w = i(start.value.w, end.value.w);
			} else
			{
				value.hsv(i(start.h(), end.h()), i(start.s(), end.s()), i(start.v(), end.v()));
				value.a(i(start.value.w, end.value.w));
			}
		}

		char type{};	// Color animation type. act_rgba interpolates RGB and alpha values, act_hsva - HSV and alpha
	};
}

#undef INTERP
#undef LINEAR

#endif //EV0LVE_RENDERER_ANIM_H
