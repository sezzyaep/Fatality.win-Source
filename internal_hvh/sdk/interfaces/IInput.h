#pragma once

typedef unsigned int CRC32_t;
class IInput
{
private:
	uint8_t             pad[ 0xA9 ];
public:
	bool                m_fCameraInThirdPerson;       //0x9D
	bool                m_fCameraMovingWithMouse;     //0x9E
	Vector				m_vecCameraOffset;            //0xA0
	bool                m_fCameraDistanceMove;        //0xAC
	int                 m_nCameraOldX;                //0xB0
	int                 m_nCameraOldY;                //0xB4
	int                 m_nCameraX;                   //0xB8
	int                 m_nCameraY;                   //0xBC
	bool                m_CameraIsOrthographic;       //0xC0
	Vector              m_angPreviousViewAngles;      //0xC4
	Vector              m_angPreviousViewAnglesTilt;  //0xD0
	float               m_flLastForwardMove;          //0xDC
	int                 m_nClearInputState;           //0xE0
	CUserCmd*           m_pCommands;                  //0xEC
	CVerifiedUserCmd*   m_pVerifiedCommands;		  //0xF0
	VFUNC( 8, GetUserCmd( int slot, int seq ), CUserCmd*( __thiscall* )( void*, int, int ) )( slot, seq )
		CUserCmd* get_user_cmd_vfunc( int slot, int sequence_number )
	{
		return util::get_vfunc< CUserCmd*( __thiscall* )( void*, int, int )>( this, 8 )( this, slot, sequence_number );
	}
};
