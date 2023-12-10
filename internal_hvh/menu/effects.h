class particle
{
public:
	particle( const Vector& pos, const Vector& velocity, const bool golden ) : pos( pos ), velocity( velocity ), golden( golden ) {}

	Vector pos;
	Vector velocity;
	bool golden;
};

class particle_drawer
{
public:
	particle_drawer( const int width, const int height, const int particles, const float foreground_plane ) :
		max_particles{ particles },
		screen_width{ width },
		screen_height{ height },
		focus_point{ width / 2.f, height / 2.f },
		foreground_plane{ foreground_plane } { spawn_particles(); }

	~particle_drawer() = default;

	void draw( int alpha );
	void draw_foreground( const int alpha );
private:
	void spawn_particles();
	void handle_respawn( particle& particle ) const;

	void update( particle& particle ) const;
	static void parallax( particle& particle, const evo::ren::vec2& delta );
	void draw_particle( particle& particle, const int alpha, const bool foreground ) const;

	int max_particles = -1;
	int screen_width = -1;
	int screen_height = -1;
	evo::ren::vec2 focus_point{};
	float foreground_plane = 0.f;
	std::vector<particle> particles{};
};