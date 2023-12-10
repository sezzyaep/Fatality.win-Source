#pragma once

class c_hook
{
public:
	virtual uintptr_t apply( uintptr_t dest ) { return 0; }
	virtual uintptr_t apply( const uint32_t index, uintptr_t func ) { return 0; }
	virtual bool is_detour() { return false; }
	virtual ~c_hook() {}
};

class c_detour : public c_hook
{
	void* original = nullptr;
#ifndef RELEASE
	std::vector<std::uint8_t> original_bytes;
#endif
	std::uint8_t* src{};

public:
	c_detour() = default;
	__forceinline c_detour( uintptr_t ent ) : src( reinterpret_cast< std::uint8_t* >( ent ) )
	{}

	~c_detour() override
	{
#ifndef RELEASE
		DWORD old_protect;
		VirtualProtect( src, original_bytes.size(), PAGE_EXECUTE_READWRITE, &old_protect );
		memcpy( src, original_bytes.data(), original_bytes.size() );
		VirtualProtect( src, original_bytes.size(), old_protect, &old_protect );
#endif
		if ( original )
		{
			syscall( NtFreeVirtualMemory )( current_process, &original, nullptr, MEM_RELEASE );
			sysunlock();
		}
	}

	__forceinline bool is_detour() override { return true; }

	__forceinline static uint32_t length_disasm( uint8_t* op )
	{
		uint32_t size, code;
		return reinterpret_cast< bool( * )( uint8_t*, uint32_t*, uint32_t* ) >( make_offset_direct( xorstr_( "gameoverlayrenderer.dll" ), sig_length_disasm_game ) )( op, &size, &code ) ? size : 0xFFFF;
	}

	__forceinline uintptr_t apply( uintptr_t dest ) override
	{
		auto add = 0;
		DWORD len = 0;
		auto opcode = src;
		while ( opcode - src < 5 )
		{
			if ( *opcode == 0xE8 )
				add = len + 1;
			if ( *opcode == 0x3B )
				len += 2;
			else if ( *opcode == 0x66 )
				len += 5;
			else
				len += length_disasm( opcode );
			opcode = src + len;
		}

		const auto op_code = std::make_unique<std::uint8_t[]>( len + 5 );
		//auto return_mem = VirtualAlloc( nullptr, len + 5, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
		auto return_mem = memory::alloc_mem( len + 5 );
		for ( std::uint32_t i = 0; i < len; i++ )
		{
			op_code[ i ] = src[ i ];
#ifndef RELEASE
			original_bytes.push_back( src[ i ] );
#endif
		}

		op_code[ len ] = 0xE9;
		*reinterpret_cast< std::uint32_t* >( op_code.get() + len + 1 ) = reinterpret_cast<
			std::uint32_t
		>( src ) - ( reinterpret_cast< std::uint32_t >( return_mem ) + 5 );

		std::memcpy( return_mem, op_code.get(), len + 5 );

		if ( add )
		{
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( return_mem ) + add ) -= reinterpret_cast< uintptr_t >( return_mem );
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( return_mem ) + add ) += reinterpret_cast< uintptr_t >( src );
		}

		/*DWORD old_protect;
		VirtualProtect( src, len, PAGE_EXECUTE_READWRITE, &old_protect );

		*src = 0xE9;
		*reinterpret_cast< std::uint32_t* >( src + 1 ) = ( uintptr_t ) ( dest ) -reinterpret_cast< std::uint32_t >( src ) - 5;
		for ( std::uint32_t i = 5; i < len; ++i )
			src[ i ] = 0x90;

		VirtualProtect( src, len, old_protect, &old_protect );

		return reinterpret_cast< Fn >( original = return_mem );*/

		DWORD old_protect;
		memory::protect_mem( src, len, PAGE_EXECUTE_READWRITE, old_protect );

		*src = 0xE9;
		*reinterpret_cast< std::uint32_t* >( src + 1 ) = ( uintptr_t ) ( dest ) -reinterpret_cast< std::uint32_t >( src ) - 5;
		for ( std::uint32_t i = 5; i < len; ++i )
			src[ i ] = 0x90;

		memory::protect_mem( src, len, old_protect, old_protect );

		return reinterpret_cast< uintptr_t >( original = return_mem );
	}
};


class c_vtable_hook : public c_hook
{
public:
	explicit c_vtable_hook( uintptr_t ent )
	{
		base = reinterpret_cast< uintptr_t* >( ent );
		original = *base;

		const auto l = length() + 1;
		current = std::make_unique<uint32_t[]>( l );
		std::memcpy( current.get(), reinterpret_cast< void* >( original - sizeof( uint32_t ) ), l * sizeof( uint32_t ) );

		patch_pointer( base );
	}

	~c_vtable_hook() override
	{
#ifndef RELEASE
		DWORD old;
		memory::protect_mem( base, sizeof( uintptr_t ), PAGE_READWRITE, old );
		*base = original;
		memory::protect_mem( base, sizeof( uintptr_t ), old, old );
#endif
	}

	__forceinline uintptr_t apply( const uint32_t index, uintptr_t func ) override
	{
		auto old = reinterpret_cast< uintptr_t* >( original )[ index ];
		current.get()[ index + 1 ] = func;
		return old;
	}

	void patch_pointer( uintptr_t* location ) const
	{
		if ( !location )
			return;

		DWORD old;
		memory::protect_mem( location, sizeof( uintptr_t ), PAGE_READWRITE, old );
		*location = reinterpret_cast< uint32_t >( current.get() ) + sizeof( uint32_t );
		memory::protect_mem( location, sizeof( uintptr_t ), old, old );
	}

private:
	uint32_t length() const
	{
		uint32_t index;
		const auto vmt = reinterpret_cast< uint32_t* >( original );

		for ( index = 0; vmt[ index ]; index++ )
			if ( IS_INTRESOURCE( vmt[ index ] ) )
				break;

		return index;
	}

	std::uintptr_t* base;
	std::uintptr_t original;
	std::unique_ptr<uint32_t[]> current;
};