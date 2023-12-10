#pragma once

enum emittype_t
{
	emit_surface,		// 90 degree spotlight
	emit_point,			// simple point light source
	emit_spotlight,		// spotlight with penumbra
	emit_skylight,		// directional light with no falloff (surface must trace to SKY texture)
	emit_quakelight,	// linear falloff, non-lambertian
	emit_skyambient,	// spherical light source with no falloff (surface must trace to SKY texture)
};

struct dworldlight_t
{
	Vector origin; //0x0000 
	Vector intensity; //0x000C 
	Vector normal; //0x0018 
	int cluster; //0x0024 
	emittype_t type; //0x0028 
	int style; //0x002C 
	float stopdot; //0x0030 
	float stopdot2; //0x0034 
	float exponent; //0x0038 
	float radius; //0x003C 
	float constant_attn; //0x0040 
	float linear_attn; //0x0044 
	float quadratic_attn; //0x0048 
	int flags; //0x004C 
	int texinfo; //0x0050 
	int owner; //0x0054 
	char pad_0x0058[ 0xC ]; //0x0058
};

class CHostState
{
public:
	char pad_0x0000[ 0xA8 ]; //0x0000
	int numworldlights;
	dworldlight_t *worldlights;

}; //Size=0x00B0

struct ColorRGBExp32
{
	byte r, g, b;
	signed char exponent;
};

struct dlight_t
{
	int		flags;
	Vector	origin;
	float	radius;
	ColorRGBExp32	color;
	float	die;
	float	decay;
	float	minlight;
	int		key;
	int		style;
	Vector	m_Direction;
	float	m_InnerAngle;
	float	m_OuterAngle;
};
class IVEffects
{
public:
	VFUNC( 4, alloc_dlight( int key ), dlight_t*( __thiscall* )( void*, int ) )( key )
		VFUNC( 5, alloc_elight( int key ), dlight_t*( __thiscall* )( void*, int ) )( key )
		VFUNC( 8, get_elight( int key ), dlight_t*( __thiscall* )( void*, int ) )( key )
};