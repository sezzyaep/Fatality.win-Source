#pragma once
#include "interfaces/ICVar.h"

struct convar_value_t
{
	char* string{};
	int32_t string_length{};
	float value{};
	int32_t n_val{};
};

class ConVar
{
public:
	static inline bool cl_lagcompensation = true;
	static inline bool cl_predict = true;

	void enforce_sent_value( bool& current, bool val );

	VFUNC( 5, get_name(), char* ( __thiscall* )( void* ) )( );
	VFUNC( 14, set_value_internal( const char* value ), void( __thiscall* )( void*, const char* ) )( value );
	VFUNC( 15, set_value_internal( float value ), void( __thiscall* )( void*, float ) )( value );
	VFUNC( 16, set_value_internal( int value ), void( __thiscall* )( void*, int ) )( value );
	VFUNC( 17, set_value_internal( Color value ), void( __thiscall* )( void*, Color ) )( value );
	VFUNC( 12, get_float(), float( __thiscall* )( void* ) )( );
	VFUNC( 13, get_int(), int( __thiscall* )( void* ) )( );

	template <typename T>
	inline void set_value( T v )
	{
		set_value_internal( v );
		orig_value = value;
	}

	bool get_bool() { return !!get_int(); }

	void unlock()
	{
		flags &= ~FCVAR_CHEAT;
		orig_flags = flags | 1;
	}

	void unarchive()
	{
		flags &= ~FCVAR_ARCHIVE;
		orig_flags = flags | 1;
	}

	void set_string( const char* value ) { set_value( value ); }
	void set_float( float value ) { set_value( value ); }
	void set_int( int value ) { set_value( value ); }
	void set_color( Color value ) { set_value( value ); }

	char pad0[ 0x4 ]{};
	ConVar* next{};
	int32_t orig_flags{};
	char* name{};
	char* help_string{};
	int32_t flags{};
	char pad1[ 0x4 ]{};
	ConVar* parent{};
	char* default_value{};

	convar_value_t value{};
	convar_value_t orig_value{};

	int32_t has_min{};
	float min_val{};
	int32_t has_max{};
	float max_val{};
	void* change_callback{};
};//Size=0x0048