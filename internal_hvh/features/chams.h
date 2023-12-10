#pragma once

typedef unsigned short ModelInstanceHandle_t;
namespace chams
{
	struct ghost_t
	{
		int index{};
		IClientRenderable* renderable{};
		state_info state;
		Vector origin{};
		QAngle abs_ang{};
		float time{};
		int skin{};
		int body{};
		int hitboxset{};
		model_t* model{};
	};

	void dormant_chams();
	void extend_drawing( ClientFrameStage_t stage );
	bool run( IMatRenderContext* context, DrawModelState_t& state, ModelRenderInfo_t& info, matrix3x4_t* custom_bone_to_world );
	void add_material( material_index_t idx, const std::string& shade, const std::string& material );
	void add_material( material_index_t idx, const std::string& material );
	bool get_interp_matrix( const player_log_t& log, matrix3x4_t* out );
	bool draw_hst( const player_log_t& log, matrix3x4_t* out );
	bool draw_ragdoll_chams( IMatRenderContext* context, DrawModelState_t& state, ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld );
	bool draw_arm_chams( IMatRenderContext* context, DrawModelState_t& state, ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld );
	bool draw_attachment_chams( IMatRenderContext* context, DrawModelState_t& state, ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld );
	void apply_material( int type, Color& col1, Color& col2, float transparency, bool hidden );
	void handle_player( C_CSPlayer* player, bool hidden = false, bool fake = false, int override_type = -1 );
	int get_type( C_CSPlayer* player, bool fake = false );
	int get_type( int preview_type );
	int get_attachment_type( int preview_type );
	bool draw_player_chams( IMatRenderContext* context, DrawModelState_t& state, ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld );
	void draw_death_ghost();
	void add_ghost( C_CSPlayer* player, lag_record_t* record );
	void dark_mode( const int stage );
	void handle_preview( const bool hidden, int override_type = -1 );
	void handle_preview_attachments( int override_type = -1 );

	void set_skybox();
	void set_vgui( bool on );

	inline bool m_rerun_nightmode = true;
	inline IMaterial* materials[ material_index_t::NUM_MATERIALS ]{};
	inline std::vector<ghost_t>ghosts = {};

	inline draw_model_ent_t draw_model_ent = {};
	inline bool in_glow;

	inline std::vector skyboxes = {

		_w( "sky_csgo_cloudy01" ),
		_w( "sky_csgo_night02" ),
		_w( "sky_csgo_night02b" ),
		_w( "cs_baggage_skybox_" ),
		_w( "cs_tibet" ),
		_w( "vietnam" ),
		_w( "sky_lunacy" ),
		_w( "embassy" ),
		_w( "italy" ),
		_w( "jungle" ),
		_w( "office" ),
		_w( "sky_cs15_daylight01_hdr" ),
		_w( "sky_cs15_daylight02_hdr" ),
		_w( "sky_day02_05" ),
		_w( "nukeblank" ),
		_w( "dustblank" ),
		_w( "sky_venice" ),
		_w( "sky_cs15_daylight03_hdr" ),
		_w( "sky_cs15_daylight04_hdr" ),
		_w( "vertigo" ),
		_w( "vertigoblue_hdr" ),
		_w( "sky_dust" ),
		_w( "sky_hr_aztec" ),
	};
};
