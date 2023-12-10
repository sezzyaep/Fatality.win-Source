
#ifndef LUA_HELPERS_H
#define LUA_HELPERS_H

#include <lua/state.h>

class control;
using json = nlohmann::json;

namespace lua::helpers
{
	struct var
	{
		size_t offset;
		int type;
		int child_type;
	};

	struct esp_flag
	{
		std::string text;
		evo::ren::color color;
	};

	inline static const std::vector<uint32_t> blocked_vars = {
		FNV1A( "cl_csm_shadows" ),
		FNV1A( "cl_bob_lower_amt" ),
		FNV1A( "cl_interp" ),
		FNV1A( "cl_interp_ratio" ),
		FNV1A( "cl_cmdrate" ),
		FNV1A( "cl_updaterate" ),
		FNV1A( "cl_extrapolate" ),
		FNV1A( "mat_queue_mode" ),
		FNV1A( "net_earliertempents" ),
		FNV1A( "r_jiggle_bones" ),
		FNV1A( "host_writeconfig" ),
		FNV1A( "plugin_load" ),
	};

	inline static const std::vector<uint32_t> risky_vars = {
		FNV1A( "cl_SetupAllBones" ),
		FNV1A( "sv_cheats" ),
		FNV1A( "mat_wireframe" ),
		FNV1A( "r_drawothermodels" ),
		FNV1A( "enable_skeleton_draw" ),
		FNV1A( "r_drawbeams" ),
		FNV1A( "r_drawbrushmodels" ),
		FNV1A( "r_drawdetailprops" ),
		FNV1A( "r_drawstaticprops" ),
		FNV1A( "r_modelwireframedecal" ),
		FNV1A( "r_shadowwireframe" ),
		FNV1A( "r_slowpathwireframe" ),
		FNV1A( "r_visocclusion" ),
		FNV1A( "vcollide_wireframe" ),
		FNV1A( "mp_radar_showall" ),
		FNV1A( "radarvisdistance" ),
		FNV1A( "mat_proxy" ),
		FNV1A( "mat_drawflat" ),
		FNV1A( "mat_norendering" ),
		FNV1A( "mat_drawgray" ),
		FNV1A( "mat_showmiplevels" ),
		FNV1A( "mat_showlowresimage" ),
		FNV1A( "mat_measurefillrate" ),
		FNV1A( "mat_fillrate" ),
		FNV1A( "mat_reversedepth" ),
		FNV1A( "fog_override" ),
		FNV1A( "r_drawentities" ),
		FNV1A( "r_drawdisp" ),
		FNV1A( "r_drawfuncdetail" ),
		FNV1A( "r_drawworld" ),
		FNV1A( "r_drawmodelstatsoverlay" ),
		FNV1A( "r_drawopaqueworld" ),
		FNV1A( "r_drawtranslucentworld" ),
		FNV1A( "r_drawopaquerenderables" ),
		FNV1A( "r_drawtranslucentrenderables" ),
		FNV1A( "mat_normals" ),
		FNV1A( "sv_allow_thirdperson" ),
	};

	json parse_table( lua_State* L, int index );
	int load_table( lua_State* L, json& j, bool array = false );

	var get_netvar( const char* name, RecvTable* table, size_t offset = 0 );

	typedef void* ( *create_t )( );
	class interface_reg
	{
	public:
		create_t create;
		const char* name;
		interface_reg* next;
	};

	uintptr_t get_interface( const uint32_t module, const uint32_t name );

	struct scan_result
	{
		enum scan_error
		{
			not_found,
			found,
			too_large_deref,
			too_small_deref,
			not_a_power,
			exception,
			parse_error,
		};

		uintptr_t address{};
		scan_error error{};
		size_t error_pos{};
	};

	scan_result find_pattern( const uint8_t* module, const char* pattern, const ptrdiff_t off = 0 );

	const uint8_t* find_module( const uint32_t hash );

	void error( const char* error_type, const char* error_string );
	void warn( const char* info_string );
	void print( const char* info_string );

	struct uri
	{
		bool is_secure;
		std::string host;
		int port;
		std::string path;
	};

	std::optional<uri> parse_uri( const std::string& u );

	// will run a callback timing based. Also used to delay a function call (run_once)
	class timed_callback
	{
	public:
		timed_callback() = default;
		~timed_callback()
		{
			delete_after_run = running = once = false;
			delay = t_start = t_end = 0.f;
			callback = {};
		}
		timed_callback( float delay, std::function<void()> callback );

		void start()
		{
			t_start = interfaces::globals()->realtime;
			running = true;
		}
		void stop()
		{
			running = false;
		}
		void run_once()
		{
			t_start = interfaces::globals()->realtime;
			once = delete_after_run = running = true;
		}

		void set_delay( float delay ) { this->delay = delay; }

		[[nodiscard]] bool is_active() const { return running; }
		[[nodiscard]] bool should_delete() const { return !once && delete_after_run; }
		bool cycle_completed()
		{
			if ( !running && !once )
				return false;

			t_end = interfaces::globals()->realtime;

			const bool completed = t_end - t_start >= delay / 1000.f;

			if ( completed )
			{
				t_start = interfaces::globals()->realtime;

				// we finished our first cycle
				once = false;
			}

			return completed;
		}

		[[nodiscard]] std::function<void()> get_callback() const { return callback; }
	private:
		float t_start{};
		float t_end{};

		std::function<void()> callback{};
		float delay{};
		bool running{};
		bool once{};
		bool delete_after_run{};
	};

	extern std::unordered_map<uint32_t, std::vector<std::shared_ptr<timed_callback>>> timers;
	extern std::unordered_map<std::shared_ptr<control>, int> hidden_elements;
}

#endif // LUA_HELPERS_H
