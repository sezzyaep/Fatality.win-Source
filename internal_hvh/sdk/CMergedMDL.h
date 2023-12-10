#pragma once

struct MDLSquenceLayer_t
{
	int		nSequenceIndex;
	float	flWeight;
};

#define MAXSTUDIOBONECTRLS			4
#define MAXSTUDIOPOSEPARAM			24
#define MAXSTUDIOSKINS				32		// total textures
#define MAXSTUDIOFLEXCTRL			96		// maximum number of flexcontrollers (input sliders)
#define MAXSTUDIOANIMBLOCKS			256
#define MAXSTUDIOFLEXDESC			1024	// maximum number of low level flexes (actual morph targets)

class CMDL
{
public:
	std::byte	pad[ 0x3C ]; // 0x0000
	MDLHandle_t	hModelHandle; // 0x003C
	std::byte	pad0[ 0x8 ]; // 0x003E
	Color		Color; // 0x0046
	std::byte	pad1[ 0x2 ]; // 0x004A
	int			nSkin; // 0x004C
	int			nBody; // 0x0050
	int			nSequence; // 0x0054
	int			nLOD; // 0x0058
	float		flPlaybackRate; // 0x005C
	float		flTime; // 0x0060
	float		flCurrentAnimEndTime; // 0x0064
	float		arrFlexControls[ MAXSTUDIOFLEXCTRL * 4 ]; // 0x0068
	Vector		vecViewTarget; // 0x0668
	bool		bWorldSpaceViewTarget; // 0x0674
	bool		bUseSequencePlaybackFPS; // 0x0675
	std::byte	pad2[ 0x2 ]; // 0x0676
	void* pProxyData; // 0x0678
	float		flTimeBasisAdjustment; // 0x067C
	std::byte	pad3[ 0x4 ]; // 0x0680 --isn't correct after this point iirc
	CUtlVector<int> arrSequenceFollowLoop; // 0x0684
	matrix3x4_t	matModelToWorld; // 0x0698
	bool		bRequestBoneMergeTakeover; // 0x06C8
}; // Size: 0x06C9 // 0x6D0?

class CCustomMaterialOwner
{
public:
	virtual ~CCustomMaterialOwner() {}
	virtual void SetCustomMaterial( void* pCustomMaterial, int nIndex = 0 ) = 0;	// either replaces and existing material (releasing the old one), or adds one to the vector
	virtual void OnCustomMaterialsUpdated() {}
	virtual void DuplicateCustomMaterialsToOther( CCustomMaterialOwner* pOther ) const = 0;

public:
	CUtlVector<void*> vecCustomMaterials;
}; // Size: 0x0014

class IRenderToRTHelperObject
{
public:
	virtual void Draw( const matrix3x4_t& matRootToWorld ) = 0;
	virtual bool BoundingSphere( Vector& vecCenter, float& flRadius ) = 0;
	virtual ITexture* GetEnvCubeMap() = 0;
};

class C_MergedMDL : public IRenderToRTHelperObject
{
public:
	virtual ~C_MergedMDL() {}
	virtual void SetMDL( MDLHandle_t hModelHandle, CCustomMaterialOwner* pCustomMaterialOwner = nullptr, void* pProxyData = nullptr ) = 0;
	virtual void SetMDL( const char* szModelPath, CCustomMaterialOwner* pCustomMaterialOwner = nullptr, void* pProxyData = nullptr ) = 0;

	MFUNC( SetMergedMDL( const char* szModelPath, CCustomMaterialOwner* pCustomMaterialOwner = nullptr, void* pProxyData = nullptr ), void( __thiscall* )( void*, const char*, CCustomMaterialOwner*, void*, bool ), make_offset( "client.dll", sig_set_merged_mdl ) )( szModelPath, pCustomMaterialOwner, pProxyData, false );
	MFUNC( SetupBonesForAttachmentQueries(), void( __thiscall* )( void* ), make_offset( "client.dll", sig_setup_bones_attachment_queries ) )( );

	void SetSequence( const int nSequence, const bool bUseSequencePlaybackFPS )
	{
		this->RootMDL.nSequence = nSequence;
		this->RootMDL.bUseSequencePlaybackFPS = bUseSequencePlaybackFPS;
		this->RootMDL.flTimeBasisAdjustment = this->RootMDL.flTime;
	}

	void create()
	{
		reinterpret_cast< void( __thiscall* )( void* ) >( make_offset( "client.dll", sig_mdl_create ) )( this );
	}

public:
	CMDL RootMDL; // 0x0000
	CUtlVector<CMDL> vecMergedModels; // 0x069C
	float arrPoseParameters[ MAXSTUDIOPOSEPARAM ]; // 0x06E9
	int	nSequenceLayers; // 0x0749
	MDLSquenceLayer_t sequenceLayers[ 8 ]; // 0x074D -> // end: 0x78D
}; // Expected Size: 0x075C