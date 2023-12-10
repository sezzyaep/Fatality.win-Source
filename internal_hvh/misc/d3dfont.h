#pragma once

#include "../includes.h"
#include "../sdk/datatypes/Vector2D.h"

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(x) { if(x) delete x; x = NULL; }

#define D3DFONT_ITALIC (1 << 0)
#define D3DFONT_ZENABLE (1 << 1)
#define D3DFVF_FONT2DVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class c_font
{
	struct font2dvertex
	{
		D3DXVECTOR4 p;
		uint32_t color{};
		float tu{}, tv{};
	};

	static constexpr auto max_vertices = 50 * 6;
	static constexpr auto points = 64;

	std::vector<Vector2D> lookup_table;
	void build_lookup_table();

	std::string font;
	uint32_t height;
	uint32_t flags;
	uint32_t weight;
	uint32_t heighest_char;

	IDirect3DDevice9* dev;
	IDirect3DTexture9* texture;
	IDirect3DVertexBuffer9* vertex_buffer;
	uint32_t text_width;
	uint32_t text_height;
	float text_scale;
	std::vector<std::vector<float>> text_coords;
	uint32_t spacing;

	IDirect3DStateBlock9* saved;
	IDirect3DStateBlock9* draw;

	bool m_use_draft_quality{};

public:
	enum font_flags
	{
		centered_x = 1 << 0,
		centered_y = 1 << 1,
		left_aligned = 1 << 2,
		right_aligned = 1 << 3,
		top_aligned = 1 << 4,
		bottom_aligned = 1 << 5,
		outline = 1 << 6,
		drop_shadow = 1 << 7
	};

	HRESULT draw_string( float x, float y, uint32_t color, const wchar_t* text, uint16_t flags = 0L );
	HRESULT draw_radial( float x, float y, uint32_t color1, uint32_t color2, const wchar_t* text, uint16_t flags = 0L );
	HRESULT get_text_extent( const wchar_t* text, SIZE* size );

	HRESULT init_device_objects( IDirect3DDevice9* dev );
	HRESULT restore_device_objects();
	HRESULT invalidate_device_objects();
	HRESULT delete_device_objects();

	c_font();
	c_font( std::string font, uint32_t height, uint32_t weight, bool use_draft_quality = false, uint32_t heighest_char = 0x80, uint32_t flags = 0 );
	~c_font();

	void destroy()
	{
		invalidate_device_objects();
		delete_device_objects();
		text_coords.clear();
		lookup_table.clear();
	}

	float line_height;
};