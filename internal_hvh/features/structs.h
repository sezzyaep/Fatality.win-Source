#pragma once

struct sound_data_t
{
	sound_data_t()
	{
		index = -1;
		valid = true;
		origin = {};
		valid_origin = true;
		updated = false;
		player = nullptr;
		observerindex = 0;
		last_update_tick = 0;
		lerped_origin = {};
		lerp_start = 0.f;
		weapon_id = 0;
		has_zeus = false;
		zeus_active = false;
	}

	bool valid;
	bool valid_origin;
	Vector origin{};
	Vector lerped_origin{};
	float lerp_start;
	int index;
	C_CSPlayer* player;
	bool updated;
	int observerindex;
	int last_update_tick;
	int weapon_id;
	bool has_zeus;
	bool zeus_active;
};

struct world_data_t
{
	ClassId classid;
	float alpha;
	bool scoped;
	Vector pos;
	std::string name;
	std::string icon_name;
	float explode_time;
	int clip;
	int maxclip;
	bool defusing;
	int32 defuser;
	float defusecountdown;
	float timer_start;
	bool is_friendly;
};

struct visual_world_data_t
{
	std::vector<world_data_t> world_data;
};

struct record_shot_info_t
{
	bool has_info = false;
	bool delay_shot = false;
	bool breaking_lc = false;
	bool extrapolated = false;
	int backtrack_ticks = 0;
	float hitchance = 0.f;
	float target_dmg = 0.f;
	int safety = 0;
	std::string extra_info{};
};

enum class resolver_side
{
	resolver_invalid = -1,
	resolver_left = 0,
	resolver_right,
	resolver_side_max
};

enum class resolver_mode
{
	resolver_invalid = -1,
	resolver_default = 0,
	resolver_flip,
	resolver_shot,
	resolver_mode_max
};

enum class resolver_direction
{
	resolver_invalid = -1,
	resolver_networked = 0,
	resolver_max,
	resolver_zero,
	resolver_min,
	resolver_max_extra,
	resolver_max_max,
	resolver_min_min,
	resolver_min_extra,
	resolver_direction_max,
};

inline resolver_direction& operator++( resolver_direction& s, int )
{
#ifndef RELEASE
	if ( s == resolver_direction::resolver_direction_max )
		__debugbreak();
#endif
	s = static_cast< resolver_direction >( static_cast< int >( s ) + 1 );

	return s;
}

inline resolver_mode& operator++( resolver_mode& s, int )
{
#ifndef RELEASE
	if ( s == resolver_mode::resolver_mode_max )
		__debugbreak();
#endif
	s = static_cast< resolver_mode >( static_cast< int >( s ) + 1 );

	return s;
}

struct addons_t
{
	bool in_jump = false;
	bool in_idle = false;
	bool swing_left = false;
	std::vector<uint16_t> activity_modifiers{};
	float next_lby_update = 0.f;
	bool in_deploy_rate_limit = false;
	float previous_action_weight = 0.f;
	float previous_action_delta = 0.f;
	float adjust_weight = 0.f;
	int vm = 0;
};

struct state_info
{
	CCSGOPlayerAnimState m_animstate = {};
	ALIGN16 matrix3x4_t m_pristine_matrix[ 128 ] = {};
	ALIGN16 matrix3x4_t m_matrix[ 128 ] = {};
	ALIGN16 matrix3x4_t m_extra_matrix[ 128 ] = {};
	QAngle m_abs_ang = {};
	std::array<float, 24> m_poses = {};
	float m_simulated_yaw = {};
	std::array<C_AnimationLayer, 13> m_own_layers = {};
	int m_setup_tick = -1;
	addons_t addon = {};
};

template<typename E, class T, E N>
class enum_array : public std::array<T, static_cast< std::size_t >( N )> {
public:
	T& operator[] ( E e )
	{
		return std::array<T, static_cast< std::size_t >( N )>::operator[]( static_cast< std::size_t >( e ) );
	}

	const T& operator[] ( E e ) const
	{
		return std::array<T, static_cast< std::size_t >( N )>::operator[]( static_cast< std::size_t >( e ) );
	}

	E distance( T e ) const
	{
		return static_cast< E >( std::distance( std::array< T, static_cast< std::size_t >( N ) >::begin(), e ) );
	}
};

template<typename E, class T>
class enum_vector : public std::vector<T>
{
public:
	enum_vector() : std::vector<T>() {}
	enum_vector( E size ) : std::vector<T>( static_cast< std::size_t >( size ) ) {}
	T& operator[] ( E e )
	{
		return std::vector<T>::operator[]( static_cast< std::size_t >( e ) );
	}

	const T& operator[] ( E e ) const
	{
		return std::vector<T>::operator[]( static_cast< std::size_t >( e ) );
	}
};

struct C_AnimationLayer;
struct lag_record_t
{
	lag_record_t() {}
	lag_record_t( C_CSPlayer* player );

	void setup_matrices( resolver_direction direction = resolver_direction::resolver_invalid, bool extrapolated = false );

	matrix3x4_t* matrix( resolver_direction direction )
	{
		return m_state[ direction ].m_matrix;
	}

	matrix3x4_t* extra_matrix( resolver_direction direction )
	{
		if ( !m_last_ang_differs )
			return nullptr;

		return m_state[ direction ].m_extra_matrix;
	}

	static float get_resolver_roll( const resolver_direction state )
	{
		switch ( state )
		{
			case resolver_direction::resolver_min_min:
			case resolver_direction::resolver_max_extra:
				return 50.f;
			case resolver_direction::resolver_max_max:
			case resolver_direction::resolver_min_extra:
				return -50.f;
			default:
				return 0.f;
		}
	}

	bool can_delay_shot() const;
	int ticks_behind( int lag = -1 ) const;
	bool delay_shot() const;
	bool breaking_lc() const;

	matrix3x4_t m_visual_matrix[ 128 ] = {};

	int m_index = {};
	enum_array<resolver_direction, state_info, resolver_direction::resolver_direction_max> m_state = {};
	int m_mode = {};

	record_shot_info_t m_shot_info = {};

	bool m_last_ang_differs = false;
	bool m_pitch_jitter = false;
	bool has_animstate = false;
	bool do_not_set = false;
	bool m_valid = false;
	bool m_dormant = {};
	bool m_did_wall_detect = {};
	Vector m_velocity = {};
	Vector m_calculated_velocity = {};
	Vector m_origin = {};
	Vector m_abs_origin = {};
	Vector m_obb_mins = {};
	Vector m_obb_maxs = {};
	CBaseHandle m_ground_entity = {};

	float m_net_time_deviation = {};
	float m_old_simtime = {};
	float m_simtime = {};
	float m_duckamt = {};
	float m_lby = {};
	bool m_strafing = {};
	int m_server_tick = {};

	QAngle m_eye_angles = {};
	float m_yaw_modifier = {};

	resolver_direction m_shot_dir = {};
	resolver_direction m_setup_dir = {};
	int m_flags = {};
	float m_velocity_modifier = {};
	int m_lagamt = {};
	int m_real_lag = {};
	float m_recv_time = {};
	int m_extrapolate_amt = {};
	bool m_extrapolated = {};
	bool m_to_server_position = {};
	bool m_ignore = false;
	bool m_unknown = false;
	resolver_mode m_resolver_mode = resolver_mode::resolver_default;
	resolver_side m_resolver_side = resolver_side::resolver_left;
	int m_pitch_cycle = 0;
	bool m_tickbase_shift = false;
	bool m_breaking_lc = false;

	float m_collision_viewheight = {};
	float m_collision_change_time = {};

	addons_t addon{};

	std::array<C_AnimationLayer, 13> m_layers = {};
	std::array<float, 24> m_poses = {};
	QAngle m_abs_ang = {};

	bool m_has_visual_matrix = false;

	bool m_shot = {};

	int m_cmdnum = {}; //lp only
};

template <class T, size_t N = 0>
struct circular_buffer
{
	circular_buffer()
	{
		if ( ( max_size_ = N ) > 0 )
			elem_.resize( max_size_ );
	}

	circular_buffer( const size_t sz ) : max_size_( sz )
	{
		elem_.resize( max_size_ );
	}

	circular_buffer( const circular_buffer& other )
	{
		max_size_ = other.max_size_;
		current_ = other.current_;
		size_ = other.size_;
		elem_ = other.elem_;

	}

	circular_buffer& operator=( const circular_buffer& other )
	{
		max_size_ = other.max_size_;
		current_ = other.current_;
		size_ = other.size_;
		elem_ = other.elem_;

		return *this;
	}

	circular_buffer( circular_buffer&& other ) noexcept
	{
		max_size_ = other.max_size_;
		current_ = other.current_;
		size_ = other.size_;
		elem_ = other.elem_;

		other.elem_.clear();
		other.size_ = other.max_size_ = 0u;
	}

	~circular_buffer() = default;

	[[nodiscard]] inline T& operator[] ( size_t idx ) { return elem_[ ( current_ + idx ) % max_size_ ]; }
	[[nodiscard]] inline const T& operator[] ( size_t idx ) const { return elem_[ ( current_ + idx ) % max_size_ ]; }
	[[nodiscard]] inline int size() const { return size_; }
	[[nodiscard]] inline bool empty() const { return size_ == 0; }

	inline void clear() { size_ = 0; current_ = 0; }
	inline void clear_all_but_last() { current_ += size_ - 1; size_ = 1; }
	inline void pop_back() { --size_; }
	inline void pop_front() { ++current_; --size_; }

	inline void reserve( size_t newsize )
	{
		if ( newsize == max_size_ )
			return;

		elem_.resize( max_size_ = newsize );
		size_ = 0;
		current_ = 0;
	}

	inline void resize( size_t newsize )
	{
		elem_.resize( max_size_ = newsize );
		size_ = max_size_;
	}

	template<class... Args>
	inline T* emplace_back( Args&&... args ) { assert( size_ < max_size_ ); elem_[ ( current_ + size_++ ) % max_size_ ] = T( std::forward<Args>( args )... ); return &elem_[ ( current_ + size_ - 1 ) % max_size_ ]; }
	inline T* push_back( T&& item ) { return emplace_back( std::move( item ) ); }
	[[nodiscard]] inline T* push_back() { return size_ >= max_size_ ? nullptr : &elem_[ ( current_ + size_++ ) % max_size_ ]; }

	template<class... Args>
	inline void emplace_front( Args&&... args ) { assert( size_ < max_size_ ); elem_[ --current_ % max_size_ ] = T( std::forward<Args>( args )... ); ++size_; }
	inline void push_front( T&& item ) { emplace_front( std::move( item ) ); }
	[[nodiscard]] inline T* push_front() { if ( size_ >= max_size_ ) return nullptr; ++size_; return &elem_[ --current_ % max_size_ ]; }

	[[nodiscard]] inline T& back() { return elem_[ ( current_ + size_ - 1 ) % max_size_ ]; }
	[[nodiscard]] inline T& front() { return elem_[ current_ % max_size_ ]; }

	inline void sort( std::function<bool( const T&, const T& )> pred )
	{
		current_ = current_ % max_size_;
		assert( current_ % max_size_ + size_ <= max_size_ );
		std::sort( elem_.begin() + current_ % max_size_, elem_.begin() + current_ % max_size_ + size_, pred );
	}

	std::vector<T> elem_{};
	size_t current_ = 0;
	size_t size_ = 0;
	size_t max_size_ = 0;
};

struct queue_element;
struct mstudiobone_t;
struct player_log_t
{
	player_log_t() : record( 0 )
	{
		for ( auto& mode : m_mode )
			mode = {};
	}

	void reset( C_CSPlayer* player )
	{
		record.clear();
		m_shots_spread = 0;
		last_server_tick = 0;
		m_unknown_shot = true;
		m_last_unusual_pitch = 0.f;
		m_last_zero_pitch = 0.f;
		m_shots = 0;
		m_history_origin_lerp = {};
		m_highest_simtime = 0.f;
		m_simtime_int = -1;
		m_enter_pvs = false;
		pitch_prev = 0;
		m_dormant_misses = 0;
		m_unknown_misses = 0;
		m_last_eye_angle = {};
		m_last_valid = 0;

		for ( auto& mode : m_mode )
			mode.m_side[ resolver_side::resolver_left ].m_blacklist = mode.m_side[ resolver_side::resolver_right ].m_blacklist = {};

		if ( player && player->get_player_info().xuid != xuid )
		{
			for ( auto& mode : m_mode )
				mode = {};

			m_unknown = true;
			nospread.m_pitch_cycle = 0;
			memset( &state, 0, sizeof( state ) );
			m_has_state = false;
			m_last_flip_tick = 0;
		}
	}

	C_CSPlayer* player = nullptr;
	int64_t xuid = 0;
	CBaseHandle handle{};
	CCSGOPlayerAnimState state;
	bool m_has_state = false;
	bool m_has_matrix = false;

	int m_lastvalid = 0;
	int m_lastvalid_vis = 0;

	float m_spawntime = 0.f;
	float m_highest_simtime = 0.f;

	float m_view_delta = 54.f;
	float m_maxs_z = FLT_MAX;
	float m_unpred_change_time = 0.f;

	Vector m_history_origin_lerp{};
	float m_history_lerp_time{};
	float m_history_ang_lerp{};
	int m_last_valid{};

	bool m_was_reset{};

	int m_last_flip_tick = 0.f;

	float m_add = 0.f;

	float m_last_unusual_pitch = 0.f;
	float m_last_zero_pitch = 0.f;

	circular_buffer<lag_record_t> record;

	lag_record_t extrapolated_record;
	lag_record_t dormant_record;
	bool need_extrapolate = false;
	bool is_hittable = false;

	int pitch_prev = 0;
	int pitch_alt = 0;

	int m_shots_spread = 0;
	int m_last_brute_mode = 0;
	int last_server_tick = 0;
	int last_recv_data = 0;
	int m_simtime_int = -1;
	int adjust_next = 0;

	std::array<C_AnimationLayer, 13> server_layers{};
	bool m_enter_pvs = false;
	int m_dormant_misses = 0;
	int m_unknown_misses = 0;

	float m_ground_accel_linear_frac_last_time = 0.f;
	int m_ground_accel_linear_frac_last_time_stamp = 0;

	struct resolver_modes
	{
		struct wall_sides
		{
			resolver_direction m_current_dir = resolver_direction::resolver_networked;
			enum_array<resolver_direction, bool, resolver_direction::resolver_direction_max> m_blacklist = {};
		};

		enum_array<resolver_side, wall_sides, resolver_side::resolver_side_max> m_side{};
	};

	enum_array<resolver_mode, resolver_modes, resolver_mode::resolver_mode_max> m_mode;
	resolver_mode m_current_mode = resolver_mode::resolver_default;
	resolver_side m_current_side = resolver_side::resolver_left;
	int m_shots = 0;

	float m_proxy_pitch = 0.f;

	resolver_direction get_dir( const lag_record_t* record = nullptr )
	{
		if ( !record )
			return m_mode[ !this->record.empty() && this->record.back().m_shot ? resolver_mode::resolver_shot : m_current_mode ].m_side[ m_current_side ].m_current_dir;

		return m_mode[ record->m_resolver_mode ].m_side[ record->m_resolver_side ].m_current_dir;
	}

	struct
	{
		C_BaseCombatWeapon* m_weapon;
		float m_shot_time;
	} last_shot{};

	bool m_unknown = true;
	bool m_unknown_shot = true;
	QAngle m_last_non_shot_angles = {};
	QAngle m_last_eye_angle = {};
	float m_wall_detect_ang = {};

	struct
	{
		int m_pitch_cycle = 0;
		bool m_can_fake = false;
	}nospread;

	struct
	{
		bool hittable;
		bool unhittable;
	} hitinfo{};
};

enum material_index_t : int
{
	MAT_PADDING, // for menu alignment
	MAT_NORMAL,
	MAT_FLAT,
	MAT_RIM_GLOW,
	MAT_CONTOUR,
	MAT_FLOW,
	MAT_ACRYLIC,
	MAT_LEGACY,

	MAT_SPRITE,

	NUM_MATERIALS
};


struct FireBulletData
{
	Vector           src;
	trace_t          enter_trace;
	Vector           direction;
	ITraceFilter     filter;
	CCSWeaponData* wpn_data;
	float           trace_length;
	float           trace_length_remaining;
	float           length_to_end;
	float           current_damage;
	int             penetrate_count;
};


struct visual_data_t
{
	visual_data_t()
	{
		this->index = -1;
		this->had_info = true;
	}

	C_CSPlayer* player{};
	bool		valid{};
	int			index{};
	float		fade_time{};
	float		alpha{};
	float		alpha_lerp{};
	float		model_alpha{};
	Vector		top{};
	Vector		bot{};
	float		width{};
	float		height{};
	float		spawntime{};
	bool had_info{};
	bool oof{};
	bool not_lagcompensated{};
	Vector pos{};

	struct
	{
		bool alive{};
		Vector pos{};
		QAngle viewangles{};
	}local{};

	//extra
	float vel{};
	int act{};
	float l1_weight{};
	float l1_cycle{};
	bool dormant{};
	int maxclip{};
	int clip{};
	int armor{};
	int health{};
	float health_interpolated{};
	bool scoped{};
	float ping{};
	bool bomb{};
	float animtime{};
	bool lc{};
	bool fd{};
	bool xp{};
	bool kevlar{};
	bool helmet{};
	bool can_hit{};
	bool kit{};
	bool has_zeus{};
	bool zeus_active{};
	int money{};

#if defined(ALPHA) || !defined(RELEASE)
	struct
	{
		int hc{};
		int target_hc{};
		int dmg{};
		int target_dmg{};
		bool sp{};
		bool vsp{};
		bool target_sp{};
		bool target_vsp{};
		bool extrapolate{};
		bool scan{};
		std::string reason{};
	}debug{};
#endif

	//record
	int shots{};
	int shots_spread{};
	bool fakewalking{};
	bool is_unicode{};
	uint32_t name_hash{};
	uint32_t weapon_name_hash{};
	std::string name{};
	int weapon_id{};
	std::string weapon_name{};
	std::string icon_name{};
	std::string rmode{};

	bool has_matrix = false;
	std::vector<std::pair< Vector, Vector>> skeleton_poses{};
	std::vector<std::pair< Vector, Vector>> history_skeleton_poses{};
	Vector aimpoints[ 128 ]{};
};