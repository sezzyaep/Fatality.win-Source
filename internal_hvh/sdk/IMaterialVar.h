#pragma once

class IMaterialVar
{
public:
	VFUNC( 1, get_tex_value(), ITexture* ( __thiscall* )( void* ) )( )
		VFUNC( 4, set_float_vaue( float val ), void( __thiscall* )( void*, float ) )( val )
		VFUNC( 5, set_int_value( int val ), void( __thiscall* )( void*, int ) )( val )
		VFUNC( 6, set_string_value( char const* val ), void( __thiscall* )( void*, char const* ) )( val )
		VFUNC( 11, set_vec_val( float x, float y, float z ), void( __thiscall* )( void*, float, float, float ) )( x, y, z )
		VFUNC( 15, set_tex_val( ITexture* tex ), void( __thiscall* )( void*, ITexture* ) )( tex )
		VFUNC( 26, set_vec_component_calue( float value, int component ), void( __thiscall* )( void*, float, int ) ) ( value, component )

		VFUNC( 6, set_string( const char* val ), void( __thiscall* )( void*, const char* ) )( val )
		VFUNC( 7, get_string(), const char* ( __thiscall* )( void* ) )( )
		VFUNC( 20, set_matrix( VMatrix& matrix ), void( __thiscall* )( void*, VMatrix& ) )( matrix )
		VFUNC( 26, set_vector_component( const float val, const int comp ), void( __thiscall* )( void*, float, int ) )( val, comp )
		VFUNC( 27, get_int(), int( __thiscall* )( void* ) )( )
		VFUNC( 28, get_float(), float( __thiscall* )( void* ) )( )
		VFUNC( 29, get_vector(), float* ( __thiscall* )( void* ) )( )
		VFUNC( 31, get_vector_size(), int( __thiscall* )( void* ) )( )
};
