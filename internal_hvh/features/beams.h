#pragma once

namespace beams
{
	struct impact_info_t
	{
		Vector			start{};
		Vector			pos{};
		Color			color;
		int				index;
		float			time;

		impact_info_t( const float time, const Vector start, const Vector pos, const int index, const Color col = Color::White() )
		{
			this->index = index;
			this->start = start;
			this->pos = pos;
			this->color = col;
			this->time = time;
		}
		float length() const
		{
			return start.Dist( this->pos );
		}
		bool equal( const impact_info_t& info ) const
		{
			if ( this->index != info.index )
				return false;

			if ( this->time != info.time )
				return false;

			return true;
		}
	};

	void draw_impacts();
	void listener( IGameEvent * game_event );
	void add_local_beam( impact_info_t info );
	bool beam_exists( C_CSPlayer* player, const float curtime );
	void add_beacon( const Vector & origin );

	inline std::deque<impact_info_t> impacts = {};
	inline std::deque<Vector> beacons = {};
};
