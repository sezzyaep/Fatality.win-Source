#pragma once

class CBoneAccessor
{
public:
	matrix3x4_t* get_bone_array_for_write( void ) const
	{
		return m_pBones;
	}
	void set_bone_array_for_write( matrix3x4_t* bonearray )
	{
		m_pBones = bonearray;
	}
	alignas( 16 ) matrix3x4_t* m_pBones;

	int m_ReadableBones;		// Which bones can be read.
	int m_WritableBones;		// Which bones can be written.
};

namespace bone_setup
{
	bool handle_bone_setup( lag_record_t& record, resolver_direction state, bool lean = false, bool extrapolated = false );
	bool perform_bone_setup( lag_record_t& record, resolver_direction state, bool lean = false );
};

