#pragma once

namespace hitmarker
{
	struct hitmarker_t
	{
		hitmarker_t( const float& time, const int& index, const int& damage, const int& hitgroup, const Vector& pos )
		{
			this->time = time;
			this->index = index;
			this->damage = damage;
			this->hitgroup = hitgroup;
			this->pos = pos;
			moved = 0.f;
			alpha = 255.f;
		}
		float time;
		int index;
		int damage;
		int hitgroup;
		float moved;
		float alpha;
		Vector pos{};
	};

	void listener( IGameEvent * game_event );
	void draw_hits();
	void add_hit( hitmarker_t hit );
	void save_files();
	void render_hitmarker( hitmarker_t& hit, const Vector & screen_pos );
	void render_screen_hitmarker( hitmarker_t& hit );

	inline std::deque<hitmarker_t> hits = {};
	inline std::vector soundfiles = {

		_w( "*bell.wav" ),
		_w( "*cod.wav" ),
		_w( "*fatality.wav" ),
		_w( "buttons/arena_switch_press_02.wav" ),
		_w( "*bepis.wav" ),
		_w( "*phonk.wav" ),
		_w( "*sparkle.wav" ),
		_w( "*cock.wav" ),
	};
};