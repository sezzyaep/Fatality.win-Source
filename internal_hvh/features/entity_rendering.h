#pragma once

namespace entity_rendering
{
	void paint();
	bool world_to_screen( const Vector& in, Vector& out );
	bool screen_transform( const Vector& in, Vector& out );

	inline ITexture* preview_texture = nullptr;
	inline ITexture* cubemap_texture = nullptr;
	inline C_MergedMDL* preview_model = nullptr;

	inline CViewSetup view_setup = {};

	inline matrix3x4_t preview_matrix[ 128 ]{};
	inline VMatrix world_to_projection{};
	inline Vector2D preview_pos{};

	inline bool player_render = false;
	inline bool is_setup = false;

	inline constexpr std::array white =
	{
		1.0f,
		1.0f,
		1.0f
	};

	inline std::array light_col1 =
	{
		Vector4D{ 0.68f, 1.53f, 1.7f, 1.f },
		Vector4D( 0.68f, 1.53f, 1.7f, 1.f ),
		Vector4D( 0.68f, 1.53f, 1.7f, 1.f ),
		Vector4D( 0.68f, 1.53f, 1.7f, 1.f ),
		Vector4D( 0.68f, 1.53f, 1.7f, 1.f ),
		Vector4D( 0.68f, 1.53f, 1.7f, 1.f ),
	};

	inline std::array light_col2 =
	{
		Vector4D( 0.68f, 0.68f, 0.68f, 3.7f ),
		Vector4D( 0.68f, 0.68f, 0.68f, 3.7f ),
		Vector4D( 0.68f, 0.68f, 0.68f, 3.7f ),
		Vector4D( 0.68f, 0.68f, 0.68f, 3.7f ),
		Vector4D( 0.68f, 0.68f, 0.68f, 3.7f ),
		Vector4D( 0.68f, 0.68f, 0.68f, 3.7f ),
	};
};