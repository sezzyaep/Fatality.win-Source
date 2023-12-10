#pragma once

namespace v8
{
	class isolate_t
	{
	public:
		MFUNC( enter(), void( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_isolate_enter ) )( );
		MFUNC( exit(), void( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_isolate_exit ) )( );
	};

	class context_t
	{
	public:
		MFUNC( enter(), void( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_context_enter ) )( );
		MFUNC( exit(), void( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_context_exit ) )( );
	};

	class try_catch_t
	{
	public:
		MFUNC( enter( isolate_t* isolate ), void( __thiscall* )( void*, void* ), make_offset( "v8.dll", sig_v8_try_catch_ctor ) )( isolate );
		MFUNC( exit(), void( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_try_catch_dtor ) )( );

	private:
		char pad[ 0x19 ]{};
	};

	class handle_scope_t
	{
	public:
		MFUNC( enter( isolate_t* isolate ), void( __thiscall* )( void*, void* ), make_offset( "v8.dll", sig_v8_handle_scope_ctor ) )( isolate );
		MFUNC( exit(), void( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_handle_scope_dtor ) )( );

		static context_t* create_handle( isolate_t* isolate, void* object )
		{
			return reinterpret_cast< context_t * ( * )( isolate_t*, void* ) >( make_offset( "v8.dll", sig_v8_handle_scope_create_handle ) )( isolate, object );
		}

	private:
		char pad[ 0xC ]{};
	};

	class local_t;
	class value_t
	{
	public:
		MFUNC( is_string(), bool( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_value_is_string ) )( );
		MFUNC( is_string_object(), bool( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_value_is_string_object ) )( );
		MFUNC( is_array(), bool( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_value_is_array ) )( );
		MFUNC( is_object(), bool( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_value_is_object ) )( );
		MFUNC( is_boolean(), bool( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_value_is_boolean ) )( );
		MFUNC( is_boolean_object(), bool( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_value_is_boolean_object ) )( );
		MFUNC( is_number(), bool( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_value_is_number ) )( );
		MFUNC( is_number_object(), bool( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_value_is_number_object ) )( );
		MFUNC( is_function(), bool( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_value_is_function ) )( );
		MFUNC( boolean_value(), bool( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_value_boolean_value ) )( );
		MFUNC( number_value(), double( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_value_number_value ) )( );
		MFUNC( to_object( void* r ), local_t* ( __thiscall* )( void*, void* ), make_offset( "v8.dll", sig_v8_value_to_object ) )( r );
	};

	class object_t : public value_t
	{
	public:
		MFUNC( get( void* r, value_t* k ), local_t* ( __thiscall* )( void*, void*, value_t* ), make_offset( "v8.dll", sig_v8_object_get ) )( r, k );
		MFUNC( get_property_names( void* r ), local_t* ( __thiscall* )( void*, void* ), make_offset( "v8.dll", sig_v8_object_get_property_names ) )( r );
	};

	class array_t : public value_t
	{
	public:
		MFUNC( length(), int( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_array_length ) )( );
		MFUNC( get( void* r, int index ), local_t* ( __thiscall* )( void*, void*, int ), make_offset( "v8.dll", sig_v8_array_get ) )( r, index );
	};

	class utf8_string_t
	{
	public:
		MFUNC( ctor( value_t* val ), void( __thiscall* )( void*, void* ), make_offset( "v8.dll", sig_v8_string_utf8_ctor ) )( val );
		MFUNC( dtor(), void( __thiscall* )( void* ), make_offset( "v8.dll", sig_v8_string_utf8_dtor ) )( );

		char* value{};
		int length{};
	};

	class local_t
	{
	public:
		value_t* value;
	};
}

class ui_panel_t
{
public:
	VFUNC( 9, get_id(), const char* ( __thiscall* )( void* ) )( );
	VFUNC( 218, get_js_context_parent(), ui_panel_t* ( __thiscall* )( void* ) )( );
};

struct ui_panel_info_t
{
	char _pad_16[ 16 ];
	ui_panel_t* panel;
	char _pad_18[ 4 ];
};

class ui_engine_t
{
public:
	VFUNC( 36, is_valid_panel_ptr( void* panel ), bool( __thiscall* )( void*, void* ) )( panel );
	VFUNC( 113, run_script( ui_panel_t* panel, const char* script ), void* ( __thiscall* )( void*, void*, const char*, const char*, int, int, int, bool ) )( panel, script, XOR_STR( "panorama/layout/base.xml" ), 8, 10, 0, false );
	VFUNC( 129, get_isolate(), v8::isolate_t* ( __thiscall* )( void* ) )( );

	MFUNC( run_compiled_script( void* unk, ui_panel_t* panel, void* script ), v8::local_t* ( __thiscall* )( void*, void*, void*, void*, int, bool ), make_offset( "panorama.dll", sig_ui_engine_run_compiled_script ) )( unk, panel, script, 0, false );
	MFUNC( compile( ui_panel_t* panel, const char* script ), v8::local_t* ( __thiscall* )( void*, void*, const char*, const char* ), make_offset( "panorama.dll", sig_ui_engine_compile ) )( panel, script, XOR_STR( "" ) );
	MFUNC( get_panel_context( ui_panel_t* panel ), v8::context_t*** ( __thiscall* )( void*, ui_panel_t* ), make_offset( "panorama.dll", sig_ui_engine_get_panel_context ) )( panel );

	char _pad_27[ 240 ];
	CUtlVector<ui_panel_info_t> panels;
};

class panorama_ui_engine_t
{
public:
	VFUNC( 11, get_ui_engine(), ui_engine_t* ( __thiscall* )( void* ) )( );
};