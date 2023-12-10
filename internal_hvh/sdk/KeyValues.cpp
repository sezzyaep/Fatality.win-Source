#include "../include_cheat.h"

KeyValues* KeyValues::create( char const* name )
{
	auto kv = reinterpret_cast< KeyValues* >( interfaces::key_values_system()->allocate_key_values_memory( 44 ) );
	kv->init( name );

	return kv;
}

KeyValues* KeyValues::from_netsmg( INetMessage* msg )
{
	return reinterpret_cast< KeyValues * ( __fastcall* )( INetMessage * msg ) >( make_offset( "engine.dll", sig_keyvalues_from_netsmg ) )( msg );
}

KeyValues::~KeyValues()
{
	interfaces::key_values_system()->free_key_values_memory( this );
}

bool KeyValues::load_from_buffer( KeyValues* kv, char const* name, const char* buffer )
{
	return reinterpret_cast< bool( __thiscall* )( void*, const char*, const char*, void*, void*, void*, void* ) >( make_offset( "client.dll", sig_load_from_buffer ) )( kv, name, buffer, nullptr, nullptr, nullptr, nullptr );
}

void KeyValues::init( const char* name )
{
	reinterpret_cast< void( __thiscall* )( void*, const char*, KeyValues*/*ker values system override*/, KeyValues*/*first subkey*/ ) >( make_offset( "client.dll", sig_keyvalues_init ) )( this, name, nullptr, nullptr );
}

void KeyValues::set_string( const char* keyName, const char* value )
{
	auto key = find_key( keyName );
	if ( key )
		reinterpret_cast< void( __thiscall* )( void*, const char* ) >( make_offset( "client.dll", sig_kv_set_string ) )( key, value );
}

const char* KeyValues::get_name() const
{
	return interfaces::key_values_system()->get_symbol_for_string( ( uint16_t( key_name_case_sensitive2 ) << 8 ) | uint8_t( key_name_case_sensitive1 ) );
}

KeyValues* KeyValues::find_key( const char* key_name )
{
	if ( !key_name || !key_name[ 0 ] )
		return this;

	auto sub_str = strchr( key_name, '/' );
	auto search_str = key_name;

	if ( sub_str )
	{
		int size = sub_str - key_name;
		auto buf = reinterpret_cast< char* >( alloca( size + 1 ) );
		memcpy( buf, key_name, size );
		buf[ size ] = 0;
		if ( strlen( key_name ) > 1 )
			search_str = buf;
	}

	int32_t search_str_idx = interfaces::key_values_system()->get_symbol_for_string( search_str, false );
	if ( search_str_idx == -1 )
		return nullptr;

	KeyValues* last_item = nullptr;
	auto dat = sub;
	for ( ; dat != nullptr; dat = dat->peer )
	{
		last_item = dat;

		if ( dat->key_name == uint32_t( search_str_idx ) )
			break;
	}

	if ( !dat && chain )
		dat = chain->find_key( key_name );

	if ( !dat )
		return nullptr;

	if ( sub_str )
		return dat->find_key( sub_str + 1 );

	return dat;
}

template <class T, class I>
void CUtlMemory<T, I>::Purge()
{
	if ( m_nGrowSize >= 0 )
	{
		if ( m_pMemory )
		{
			interfaces::memalloc()->Free( m_pMemory );
			m_pMemory = nullptr;
		}
		m_nAllocationCount = 0;
	}
}

template <class T, class I>
CUtlMemory<T, I>::CUtlMemory( int nGrowSize, int nInitSize ) : m_pMemory( 0 ),
m_nAllocationCount( nInitSize ), m_nGrowSize( nGrowSize )
{
	if ( m_nAllocationCount )
		m_pMemory = ( T* ) interfaces::memalloc()->Alloc( m_nAllocationCount * sizeof( T ) );
}

template <class T, class I>
void CUtlMemory<T, I>::Grow( int num )
{
		// Make sure we have at least numallocated + num allocations.
		// Use the grow rules specified for this memory (in m_nGrowSize)
	int nAllocationRequested = m_nAllocationCount + num;

	int nNewAllocationCount = UtlMemory_CalcNewAllocationCount( m_nAllocationCount, m_nGrowSize, nAllocationRequested, sizeof( T ) );

	// if m_nAllocationRequested wraps index type I, recalculate
	if ( ( int ) ( I ) nNewAllocationCount < nAllocationRequested )
	{
		if ( ( int ) ( I ) nNewAllocationCount == 0 && ( int ) ( I ) ( nNewAllocationCount - 1 ) >= nAllocationRequested )
		{
			--nNewAllocationCount; // deal w/ the common case of m_nAllocationCount == MAX_USHORT + 1
		}
		else
		{
			if ( ( int ) ( I ) nAllocationRequested != nAllocationRequested )
			{
				// we've been asked to grow memory to a size s.t. the index type can't address the requested amount of memory
				assert( 0 );
				return;
			}
			while ( ( int ) ( I ) nNewAllocationCount < nAllocationRequested )
			{
				nNewAllocationCount = ( nNewAllocationCount + nAllocationRequested ) / 2;
			}
		}
	}

	m_nAllocationCount = nNewAllocationCount;

	if ( m_pMemory )
		m_pMemory = ( T* ) interfaces::memalloc()->Realloc( m_pMemory, m_nAllocationCount * sizeof( T ) );
	else
		m_pMemory = ( T* ) interfaces::memalloc()->Alloc( m_nAllocationCount * sizeof( T ) );
}