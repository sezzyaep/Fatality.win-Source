class INetMessage;

class KeyValuesSystem
{
public:
	VFUNC( 2, allocate_key_values_memory( int size ), void* ( __thiscall* )( void*, int ) )( size );
	VFUNC( 3, free_key_values_memory( void* mem ), void ( __thiscall* )( void*, void* ) )( mem );
	VFUNC( 4, get_symbol_for_string( const char* name, bool create ), int( __thiscall* )( void*, const char*, bool ) )( name, create );
	VFUNC( 5, get_symbol_for_string( int symbol ), const char* ( __thiscall* )( void*, int ) )( symbol );
};

class KeyValues
{
public:
	void init( const char* name );
	void set_string( const char* keyName, const char* value );
	static bool load_from_buffer( KeyValues* kv, char const* name, const char* buffer );

	enum types_t
	{
		TYPE_NONE = 0,
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_PTR,
		TYPE_WSTRING,
		TYPE_COLOR,
		TYPE_UINT64,
		TYPE_COMPILED_INT_BYTE,
		TYPE_COMPILED_INT_0,
		TYPE_COMPILED_INT_1,
		TYPE_NUMTYPES,
	};

public:
	static KeyValues* create( char const* name );
	static KeyValues* from_netsmg( INetMessage* msg );

	~KeyValues();

	KeyValues* find_key( const char* key_name );

	const char* get_name() const;

	inline KeyValues* get_first_value() const
	{
		auto ret = sub;
		while ( ret && ret->data_type == TYPE_NONE )
			ret = ret->peer;
		return ret;
	}

	inline KeyValues* get_next_value() const
	{
		auto ret = peer;
		while ( ret && ret->data_type == TYPE_NONE )
			ret = ret->peer;
		return ret;
	}

	inline void* get_ptr( const char* key = nullptr, void* def = nullptr )
	{
		if ( auto dat = find_key( key ); dat )
		{
			switch ( dat->data_type )
			{
				case TYPE_FLOAT:
				case TYPE_INT:
				case TYPE_PTR:
					return dat->p_value;
				case TYPE_WSTRING:
					return dat->ws_value;
				case TYPE_STRING:
					return dat->s_value;
				case TYPE_UINT64:
				default:
					break;
			};
		}

		return def;
	}

	KeyValues() = delete;

	uint32_t key_name : 24;
	uint32_t key_name_case_sensitive1 : 8;

	char* s_value = nullptr;
	wchar_t* ws_value = nullptr;

	union
	{
		int i_value;
		float f_value;
		void* p_value;
		uint8_t color[ 4 ];
	};

	char data_type = 0;
	char has_escape_sequences = 0;
	uint16_t  key_name_case_sensitive2 = 0;

	char pad[ 8 ]{};
	KeyValues* peer = nullptr;
	KeyValues* sub = nullptr;
	KeyValues* chain = nullptr;

	uintptr_t expression_get_symbol_proc = 0;
};