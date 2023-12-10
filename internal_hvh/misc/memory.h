#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <mutex>
#include <wintrust.h>
#include <softpub.h>
#include "defs.h"

#pragma comment(lib, "Wintrust.lib")

namespace memory
{
#undef Module32Next
#undef Module32First
#undef MODULEENTRY32
	inline uintptr_t get_module_handle( const uint32_t mod_name, const uint32_t process = 0 )
	{
		auto tries = 0;
		do
		{
			MODULEENTRY32 entry;
			entry.dwSize = sizeof( MODULEENTRY32 );

			const auto snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, process );

			if ( Module32First( snapshot, &entry ) )
				while ( Module32Next( snapshot, &entry ) )
				{
					std::string name = entry.szModule;
					std::ranges::transform( name, name.begin(), ::tolower );

					if ( fnv1a_rt( name.c_str() ) == mod_name )
					{
						CloseHandle( snapshot );
						return reinterpret_cast< uintptr_t >( entry.hModule );
					}
				}

			CloseHandle( snapshot );
		} while ( tries++ < 10 );

		_( logfile, "flog.log" );
		std::ofstream of( logfile, std::ofstream::out | std::ofstream::app );
		of << xorstr_( "failed to initialize: " ) << std::hex << mod_name << std::endl;
		of.close();
		return 0;
	}

	inline uint8_t* find_sig( const uint32_t offset, const char* signature, const uint32_t range = 0u )
	{
		static auto pattern_to_bytes = [] ( const char* pattern ) -> std::vector<int>
		{
			auto bytes = std::vector<int32_t>{};
			const auto start = const_cast< char* >( pattern );
			const auto end = const_cast< char* >( pattern ) + strlen( pattern );

			for ( auto current = start; current < end; ++current )
			{
				if ( *current == '?' )
				{
					current++;

					if ( *current == '?' )
						current++;

					bytes.push_back( -1 );
				}
				else
					bytes.push_back( static_cast< int32_t >( strtoul( current, &current, 0x10 ) ) );
			}

			return bytes;
		};

		const auto scan_bytes = reinterpret_cast< std::uint8_t* >( offset );
		auto pattern_bytes = pattern_to_bytes( signature );
		const auto s = pattern_bytes.size();
		const auto d = pattern_bytes.data();

		for ( auto i = 0ul; i < range - s; ++i )
		{
			auto found = true;

			for ( auto j = 0ul; j < s; ++j )
				if ( scan_bytes[ i + j ] != d[ j ] && d[ j ] != -1 )
				{
					found = false;
					break;
				}

			if ( found )
				return &scan_bytes[ i ];
		}

		return nullptr;
	}

	inline int32_t rva_2_offset( const uint32_t rva, PIMAGE_NT_HEADERS nt_headers, const bool in_memory = false )
	{
		if ( rva == 0 || !in_memory )
			return rva;

		auto sec = IMAGE_FIRST_SECTION( nt_headers );
		for ( size_t i = 0; i < nt_headers->FileHeader.NumberOfSections; i++ )
		{
			if ( rva >= sec->VirtualAddress && rva < sec->VirtualAddress + sec->Misc.VirtualSize )
				break;
			sec++;
		}

		return rva - sec->VirtualAddress + sec->PointerToRawData;
	}

	__forceinline uintptr_t get_proc_address( const uintptr_t module, const uint32_t function, const bool in_memory = false )
	{
		const auto dos_headers = reinterpret_cast< IMAGE_DOS_HEADER* >( module );
		if ( dos_headers->e_magic != IMAGE_DOS_SIGNATURE )
			return 0;

		const auto nt_headers = reinterpret_cast< IMAGE_NT_HEADERS* >( dos_headers->e_lfanew + module );
		if ( nt_headers->Signature != IMAGE_NT_SIGNATURE )
			return 0;

		const auto exports = reinterpret_cast< IMAGE_EXPORT_DIRECTORY* >(
			rva_2_offset( nt_headers->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress,
			nt_headers, in_memory ) + module );

		const auto names = reinterpret_cast< uint32_t* >( rva_2_offset( exports->AddressOfNames, nt_headers, in_memory ) + module );

		auto ordinal_index = static_cast< uint32_t >( -1 );
		for ( uint32_t i = 0; i < exports->NumberOfFunctions; i++ )
		{
			const auto function_name = reinterpret_cast< const char* >( rva_2_offset( names[ i ], nt_headers, in_memory ) + module );

			if ( fnv1a_rt( function_name ) == function )
			{
				ordinal_index = i;
				break;
			}
		}

		if ( ordinal_index > exports->NumberOfFunctions )
			return 0;

		const auto ordinals = reinterpret_cast< uint16_t* >( rva_2_offset( exports->AddressOfNameOrdinals, nt_headers, in_memory ) + module );
		const auto addresses = reinterpret_cast< uint32_t* >( rva_2_offset( exports->AddressOfFunctions, nt_headers, in_memory ) + module );
		return rva_2_offset( addresses[ ordinals[ ordinal_index ] ], nt_headers, in_memory ) + module;
	}

	inline std::mutex imp_mutex;

	template <uint32_t module, uint32_t function>
	[[clang::optnone]] inline void* import()
	{
		std::lock_guard l( imp_mutex );

		static void* fn = reinterpret_cast< void* >( get_proc_address( get_module_handle( module ), function ) );

		return fn;
	}
}

//#define imp(module, name) []()  __attribute__((noinline)) { static void* fn = reinterpret_cast< void* >( memory::get_proc_address( memory::get_module_handle( FNV1A(module) ), FNV1A(#name) ) ); return reinterpret_cast< decltype( &( name ) ) >( fn ); }()

#define imp(module, name) (reinterpret_cast<decltype(&(name))>(memory::import<FNV1A(module), FNV1A(#name)>()))
#define import(module, name) (reinterpret_cast<name>(memory::import<FNV1A(module), FNV1A(#name)>()))



namespace memory
{
	struct syscall_fn
	{
		uint32_t index;
		uint16_t retn;
	};

	inline bool verify_integrity( const char* path )
	{
		std::string source( path );
		std::wstring file( source.begin(), source.end() );

		WINTRUST_FILE_INFO file_data;
		memset( &file_data, 0, sizeof( file_data ) );
		file_data.cbStruct = sizeof( WINTRUST_FILE_INFO );
		file_data.pcwszFilePath = file.c_str();
		file_data.hFile = nullptr;
		file_data.pgKnownSubject = nullptr;

		GUID guid = WINTRUST_ACTION_GENERIC_VERIFY_V2;
		WINTRUST_DATA wintrust;
		memset( &wintrust, 0, sizeof( wintrust ) );

		wintrust.cbStruct = sizeof( wintrust );
		wintrust.pPolicyCallbackData = nullptr;
		wintrust.pSIPClientData = nullptr;
		wintrust.dwUIChoice = WTD_UI_NONE;
		wintrust.fdwRevocationChecks = WTD_REVOKE_NONE;
		wintrust.dwUnionChoice = WTD_CHOICE_FILE;
		wintrust.dwStateAction = WTD_STATEACTION_VERIFY;
		wintrust.hWVTStateData = nullptr;
		wintrust.pwszURLReference = nullptr;
		wintrust.dwUIContext = 0;
		wintrust.pFile = &file_data;

		const uint32_t status = imp( "wintrust.dll", WinVerifyTrust )( nullptr, &guid, &wintrust );
		wintrust.dwStateAction = WTD_STATEACTION_CLOSE;
		WinVerifyTrust( nullptr, &guid, &wintrust );
		return status == ERROR_SUCCESS;
	}

	inline uint8_t* locate_wow64_transition()
	{
		static uint8_t* wow64_transition;
		if ( !wow64_transition )
			wow64_transition = reinterpret_cast< uint8_t* >( get_proc_address(
			get_module_handle( fnv1a_ct( "win32u.dll" ) ), fnv1a_ct( "Wow64Transition" ) ) );

		return wow64_transition;
	}

	inline bool verify_call_integrity( const uint8_t* shell )
	{
		const auto jmp_pad_cmp = *reinterpret_cast< uint8_t** >( &const_cast< uint8_t* >( shell )[ 6 ] );
		const auto transition_cmp = *reinterpret_cast< uint8_t** >( &jmp_pad_cmp[ 2 ] );

		return transition_cmp == locate_wow64_transition();
	}

	static std::mutex extract_syscall_mutex;


	inline syscall_fn extract_syscall( const uint32_t function, const bool special_offset = false )
	{
		std::lock_guard l( extract_syscall_mutex );

		syscall_fn fn = {};

		_( ntdll_ext, "\\SysWOW64\\ntdll.dll" );

		char win_path[ MAX_PATH ] = {};
		imp( "kernel32.dll", GetWindowsDirectoryA )( win_path, MAX_PATH );

		const std::string ntdll = win_path + ntdll_ext;

		if ( !verify_integrity( ntdll.c_str() ) )
			return fn;

		std::ifstream file( ntdll, std::ios::binary | std::ios::ate );
		const std::streamsize size = static_cast< uint32_t >( file.tellg() );
		file.seekg( 0, std::ios::beg );

		std::vector<char> buffer( static_cast< uint32_t >( size ) );
		if ( file.read( buffer.data(), size ) )
		{
			const auto proc = get_proc_address( reinterpret_cast< uintptr_t >( buffer.data() ), function, true );
			const auto id = *reinterpret_cast< uint32_t* >( proc + 0x1 );
			const auto ret = *reinterpret_cast< uint16_t* >( proc + ( special_offset ? 0x27 : 0xD ) );
			fn = { id, ret };
		}

		return fn;
	}

	__forceinline uint8_t* allocate_proxy()
	{
		static std::vector<uint8_t> pad =
		{
			0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 // far jmp wow64_transition
		};

		static std::vector<uint8_t> sys =
		{
			0xB8, 0x00, 0x00, 0x00, 0x00, // mov eax, index
			0xBA, 0x00, 0x00, 0x00, 0x00, // mov edx, wow64_jumppad
			0xFF, 0xD2, // call wow64_transition
			0xC2, 0x00, 0x00 // retn error_code
		};

		static uint8_t* proxy = nullptr;
		static uint8_t* jmp_pad = nullptr;

		if ( proxy )
			return proxy;

		// Allocate jumppad
		auto size = pad.size();
		import( "ntdll.dll", NtAllocateVirtualMemory )( current_process, reinterpret_cast<PVOID*>( &jmp_pad ), nullptr,
			&size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

		size = pad.size();
		*reinterpret_cast< uint32_t* >( &pad[ 2 ] ) = reinterpret_cast< uint32_t >( locate_wow64_transition() );
		memcpy( jmp_pad, &pad[ 0 ], size );

		// Allocate shellcode
		uint8_t* sys_code = nullptr;
		size = sys.size();
		import( "ntdll.dll", NtAllocateVirtualMemory )( current_process, reinterpret_cast<PVOID*>( &sys_code ), nullptr,
			&size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

		size = sys.size();
		*reinterpret_cast< uint32_t* >( &sys[ 6 ] ) = reinterpret_cast< uint32_t >( jmp_pad );
		memcpy( sys_code, &sys[ 0 ], size );

		return proxy = sys_code;
	}

	/*
	 * Not thread safe, make sure to implement a mutex in the future.
	 * We need to make sure that two threads never override the shellcode at the same time.
	 * Considering the fact these are native functions, the result might be catastrophic.
	 *
	 * This also only allows for immediate calls right now.
	 */
	static std::mutex syscall_mutex;

	template <uint32_t function>
	__declspec( noinline ) static void* syscall( const bool lock = false, const bool special_offset = false )
	{
		if ( lock )
			syscall_mutex.lock();

		static auto proxy = allocate_proxy();

		static auto fn = extract_syscall( function, special_offset );

		if ( !verify_call_integrity( proxy ) )
		{
			_( seperator, "-" );

			fn.index = 169;
			fn.retn = 19;
		}

		*reinterpret_cast< uint32_t* >( &proxy[ 1 ] ) = fn.index;
		*reinterpret_cast< uint32_t* >( &proxy[ 0xD ] ) = fn.retn;

		return proxy;
	}
}
#define syscall(name) (reinterpret_cast<name>(memory::syscall<FNV1A(#name)>(true)))
#define sys(name) (reinterpret_cast<decltype(&(name))>(memory::syscall<FNV1A(#name)>(true)))
#define query_info_process (reinterpret_cast<decltype(&(NtQueryInformationProcess))>(memory::syscall<FNV1A("NtQueryInformationProcess")>(true, true)))
#define sysunlock() (memory::syscall_mutex.unlock())

namespace memory
{
	__forceinline std::string lookup_library( const std::string& subdir, const uint32_t hash )
	{
		char win_path[ MAX_PATH ] = {};
		imp( "kernel32.dll", GetWindowsDirectoryA )( win_path, MAX_PATH );
		std::string path( win_path );
		path.append( subdir );

		WIN32_FIND_DATAA find_data;
		const auto find = imp( "kernel32.dll", FindFirstFileA )( path.c_str(), &find_data );
		std::string name;

		do
		{
			std::string file_name = find_data.cFileName;
			std::transform( file_name.begin(), file_name.end(), file_name.begin(), ::tolower );

			if ( fnv1a_rt( file_name.c_str() ) == hash )
				name = find_data.cFileName;
		} while ( imp( "kernel32.dll", FindNextFileA )( find, &find_data ) != 0 );

		imp( "kernel32.dll", FindClose )( find );
		return name;
	}

	__forceinline std::string lookup_apiset( const uint32_t hash )
	{
		const auto& peb = NtCurrentTeb()->ProcessEnvironmentBlock;
		auto api_set_map = reinterpret_cast< PAPI_SET_NAMESPACE >( peb->Reserved9[ 0 ] );

		auto entry = reinterpret_cast< PAPI_SET_NAMESPACE_ENTRY >( api_set_map->EntryOffset + reinterpret_cast< ULONG_PTR >( api_set_map ) );
		for ( uint32_t i = 0; i < api_set_map->Count; i++ )
		{
			std::wstring w_name( PWCHAR( reinterpret_cast< ULONG_PTR >( api_set_map ) + entry->NameOffset ) );
			std::string name( w_name.begin(), w_name.end() );
			std::transform( name.begin(), name.end(), name.begin(), ::tolower );

			if ( fnv1a_rt( name.append( ".dll" ).c_str() ) == hash )
				return name;

			entry++;
		}

		return std::string();
	}

	inline uint32_t force_remote_load_library( const HANDLE process, const std::string& name )
	{
		void* module = nullptr;
		auto size = sizeof( char ) * ( name.size() + 1 );
		syscall( NtAllocateVirtualMemory )( process, &module, nullptr, &size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
		sysunlock();
		syscall( NtWriteVirtualMemory )( process, module, const_cast< char* >( name.c_str() ), size, reinterpret_cast< PULONG >( &size ) );
		sysunlock();

		HANDLE thread;
		DWORD retn;
		syscall( NtCreateThreadEx )( &thread, GENERIC_ALL, nullptr, process,
			reinterpret_cast< LPTHREAD_START_ROUTINE >( imp( "kernel32.dll", LoadLibraryA ) ), module, // NOLINT
			0x40 | THREAD_CREATE_FLAGS_SKIP_THREAD_ATTACH | THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER, NULL, NULL, NULL, nullptr );
		sysunlock();
		imp( "kernel32.dll", WaitForSingleObject )( thread, INFINITE );
		imp( "kernel32.dll", GetExitCodeThread )( thread, &retn );
		syscall( NtFreeVirtualMemory )( process, &module, nullptr, MEM_RELEASE );
		sysunlock();

		return static_cast< uint32_t >( retn );
	}

	inline uint32_t force_remote_load_library( const HANDLE process, const uint32_t hash )
	{
		_( syswow, "\\SysWOW64\\*" );

		auto name = lookup_library( syswow, hash );

		if ( name.empty() )
			name = lookup_apiset( hash );

		if ( name.empty() )
		{
			_( runtime, "Runtime Error" );
			imp( "user32.dll", MessageBoxA )( nullptr, std::to_string( hash ).c_str(), runtime.c_str(), MB_ICONERROR );
			return 0;
		}

		void* module = nullptr;
		const auto string_len = sizeof( char ) * ( name.size() + 1 );
		auto size = string_len;
		syscall( NtAllocateVirtualMemory )( process, &module, nullptr, &size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
		sysunlock();
		size = 0;
		syscall( NtWriteVirtualMemory )( process, module, const_cast< char* >( name.c_str() ), string_len, reinterpret_cast< PULONG >( &size ) );
		sysunlock();

		HANDLE thread;
		DWORD retn;
		syscall( NtCreateThreadEx )( &thread, GENERIC_ALL, nullptr, process,
			reinterpret_cast< LPTHREAD_START_ROUTINE >( imp( "kernel32.dll", LoadLibraryA ) ), module, // NOLINT
			0x40 | THREAD_CREATE_FLAGS_SKIP_THREAD_ATTACH | THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER, NULL, NULL, NULL, nullptr );
		sysunlock();
		imp( "kernel32.dll", WaitForSingleObject )( thread, INFINITE );
		imp( "kernel32.dll", GetExitCodeThread )( thread, &retn );
		syscall( NtFreeVirtualMemory )( process, &module, nullptr, MEM_RELEASE );
		sysunlock();

		return static_cast< uint32_t >( retn );
	}

	inline uint32_t static_load_library( const uint32_t hash )
	{
		_( syswow, "\\SysWOW64\\*" );

		auto name = lookup_library( syswow, hash );

		if ( name.empty() )
			name = lookup_apiset( hash );

		if ( name.empty() )
		{
			_( runtime, "Runtime Error" );
			imp( "user32.dll", MessageBoxA )( nullptr, std::to_string( hash ).c_str(), runtime.c_str(), MB_ICONERROR );
			return 0;
		}

		return reinterpret_cast< uint32_t >( imp( "kernel32.dll", LoadLibraryExA )( name.c_str(), nullptr, DONT_RESOLVE_DLL_REFERENCES ) );
	}

	inline uintptr_t get_static_proc_address( const uintptr_t module, const uint32_t function, const bool ordinal = false )
	{
		if ( ordinal )
			return reinterpret_cast< uint32_t >( imp( "kernel32.dll", GetProcAddress )
					( reinterpret_cast< HMODULE >( module ), MAKEINTRESOURCEA( function ) ) );

		const auto dos_headers = reinterpret_cast< IMAGE_DOS_HEADER* >( module );
		if ( dos_headers->e_magic != IMAGE_DOS_SIGNATURE )
			return 0;

		const auto nt_headers = reinterpret_cast< IMAGE_NT_HEADERS* >( module + dos_headers->e_lfanew );
		if ( nt_headers->Signature != IMAGE_NT_SIGNATURE )
			return 0;

		const auto exports = reinterpret_cast< IMAGE_EXPORT_DIRECTORY* >( module +
			nt_headers->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress );

		const auto names = reinterpret_cast< uint32_t* >( module + exports->AddressOfNames );

		for ( uint32_t i = 0; i < exports->NumberOfFunctions; i++ )
		{
			const auto function_name = reinterpret_cast< const char* >( module + names[ i ] );

			if ( fnv1a_rt( function_name ) == function )
				return reinterpret_cast< uint32_t >( imp( "kernel32.dll", GetProcAddress )
					( reinterpret_cast< HMODULE >( module ), function_name ) );
		}

		return 0;
	}
}

namespace memory
{
	inline void protect_mem( void* addr, const uint32_t len, const DWORD new_protect, DWORD& old_protect )
	{
		auto target = addr;
		auto target_len = static_cast< DWORD >( len );
		syscall( NtProtectVirtualMemory )( current_process, &target, &target_len, new_protect, &old_protect );
		sysunlock();
	}

	inline void* alloc_mem( const uint32_t len )
	{
		auto target_len = len;
		void* ret = nullptr;
		import( "ntdll.dll", NtAllocateVirtualMemory )( current_process, &ret, nullptr, &target_len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
		return ret;
	}
}
