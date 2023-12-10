#pragma once

class CFlashLightEffect
{
public:
	bool m_bIsOn; //0x0000 
	char pad_0x0001[ 0x3 ]; //0x0001
	int m_nEntIndex; //0x0004 
	WORD m_FlashLightHandle; //0x0008 
	char pad_0x000A[ 0x2 ]; //0x000A
	float m_flMuzzleFlashBrightness; //0x000C 
	float m_flFov; //0x0010 
	float m_flFarZ; //0x0014 
	float m_flLinearAtten; //0x0018 
	bool m_bCastsShadows; //0x001C 
	char pad_0x001D[ 0x3 ]; //0x001D
	float m_flCurrentPullBackDist; //0x0020 
	DWORD m_MuzzleFlashTexture; //0x0024 
	DWORD m_FlashLightTexture; //0x0028 
	char m_szTextureName[ 64 ]; //0x1559888 
}; //Size=0x006C

namespace flashlight
{
	void run();
	void create( int ent_index, const char* name, float fov, float far_z, float lin_atten );
	void destroy();
	void update( const Vector& pos, const Vector& forward, const Vector& right, const Vector& up );

	inline bool m_active = {};
	inline CFlashLightEffect* m_flashlight = {};
};