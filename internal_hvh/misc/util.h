#pragma once
#include <fstream>
#include <psapi.h>

#include "crc.h"
#include "json.hpp"
#include "netlistmgr.h"

#define log(str) util::print_log(_r("%s"), _r(str))
#define log_raw(str) util::print_log(_r("%s"), str)
#ifdef DEBUG_LOG
#define log_debug(str) log(str)
#define log_debug_fmt(str, ...)  util::print_log(str, __VA_ARGS__)
#else
#define log_debug(str) {}
#define log_debug_fmt(str, ...) {}
#endif

#ifdef RELEASE
#define add_hash() ^ (uint64_t)(runtime_basis ^ __readfsdword( 0x20 ))
#else
#define add_hash() ^ (uint64_t)runtime_basis
#endif


#ifdef NO_NETOFFSETS
#define make_offset_direct( modulename, offset ) []() -> uintptr_t { static volatile uintptr_t module_base = ( uintptr_t ) GetModuleHandleA( modulename ); return module_base + (uint32_t) ( offsets::offset ^ static_cast<uint64_t>( offsets::randomized_hash::offset ) add_hash() ); }()
#define make_offset_once( modulename, offset ) []() -> uintptr_t { static volatile uintptr_t module_base = ( uintptr_t ) GetModuleHandleA( modulename ); return module_base + (uint32_t) ( offsets::offset ^ static_cast<uint64_t>( offsets::randomized_hash::offset ) add_hash() ); }()
#define make_offset( modulename, offset ) []() -> uintptr_t { static volatile uintptr_t module_base =  ( uintptr_t ) GetModuleHandleA( modulename ); return module_base + (uint32_t) ( offsets::offset ^ static_cast<uint64_t>( offsets::randomized_hash::offset ) add_hash() ); }()
#define make_offset_multi( modulename, offset ) []() -> uintptr_t { static volatile uintptr_t module_base =  ( uintptr_t ) GetModuleHandleA( modulename ); return module_base + (uint32_t) ( offsets::offset ^ static_cast<uint64_t>( offsets::randomized_hash::offset ) add_hash() ); }()
#define server_offset( offset ) []() -> uintptr_t { return (uint32_t)(offsets::offset ^ static_cast<uint64_t>( offsets::randomized_hash::offset ) add_hash()); }()
#define server_offset_multi( offset ) []() -> uintptr_t { return (uint32_t)(offsets::offset ^ static_cast<uint64_t>( offsets::randomized_hash::offset ) add_hash()); }()
#else
#define make_offset_direct( modulename, offset ) []() -> uintptr_t { static volatile uintptr_t module_base = ( uintptr_t ) GetModuleHandleA( modulename ); return module_base + (uint32_t) ( offsets::offset ^ static_cast<uint64_t>( offsets::randomized_hash::offset ) add_hash() ); }()
#define make_offset_once( modulename, offset ) []() __attribute__((always_inline)) { return (uint32_t) ( networking::network_offsets->get_offset( ( ( uint64_t ) offsets::as_enum::offset ) ^ 0x0827343994827389 ) ^ static_cast<uint64_t>( offsets::randomized_hash::offset ) add_hash() ); }()
#define make_offset( modulename, offset ) []() __attribute__((always_inline)) { static volatile uint64_t result = networking::network_offsets->get_offset( ( ( uint64_t ) offsets::as_enum::offset ) ^ 0x0827343994827389 ); return (uint32_t) ( result ^ static_cast<uint64_t>( offsets::randomized_hash::offset ) add_hash() ); }()
#define make_offset_multi( modulename, offset ) []() __attribute__((always_inline)) { static volatile uint64_t result = networking::network_offsets->get_offset( ( ( uint64_t ) offsets::as_enum::offset ) ^ 0x0827343994827389, false ); return (uint32_t) ( result ^ static_cast<uint64_t>( offsets::randomized_hash::offset ) add_hash() ); }()
#define server_offset( offset ) []() __attribute__((always_inline)) { static volatile uint64_t result = networking::network_offsets->get_offset( ( ( uint64_t ) offsets::as_enum::offset ) ^ 0x0827343994827389 ); return (uint32_t) ( result ^ static_cast<uint64_t>( offsets::randomized_hash::offset ) add_hash() ); }()
#define server_offset_multi( offset ) []() __attribute__((always_inline)) { static volatile uint64_t result = networking::network_offsets->get_offset( ( ( uint64_t ) offsets::as_enum::offset ) ^ 0x0827343994827389, false ); return (uint32_t) ( result ^ static_cast<uint64_t>( offsets::randomized_hash::offset ) add_hash() ); }()
#endif

namespace networking
{
	namespace msg
	{
		enum class report_type : uint8_t;
	}
}

class Color;
class CEconItem;
namespace util
{
	uintptr_t get_sig( const std::string& module_name, const std::string& pattern, bool backwards = false );
	float get_hue( D3DCOLOR col );
	float get_saturation( D3DCOLOR col );
	float get_brightness( D3DCOLOR col );
	float get_fraction( float val, float min, float max );
	D3DCOLOR hsb_to_rgb( float hue, float saturation, float brightness, int alpha = 255 );
	Color hsb_to_rgbcol( float _h, float s, float b, int alpha = 255 );
	void print_dev_console( bool printincon, const Color& color, const char* format, ... );
	uint8_t* load_text_file( const char* name, size_t* len );
	template <typename T> T get_vfunc( void* v_table, const int i_index ) { return ( *static_cast< T** >( v_table ) )[ i_index ]; }
	template <typename T> T* get_interface( const HMODULE module, std::string szinterface )
	{
		using create_interface_fn = T * ( * )( const char* szname, int ret );
		_( CreateInterface, "CreateInterface" );
		const create_interface_fn create_interface = reinterpret_cast< create_interface_fn >( GetProcAddress( module, CreateInterface.c_str() ) );

		return create_interface( szinterface.c_str(), 0 );
	}
	inline uint8_t* find_sig_ext( uint32_t offset, const char* signature, uint32_t range = 0u )
	{
		static auto pattern_to_bytes = [] ( const char* pattern ) -> std::vector<int>
		{
			auto bytes = std::vector<int32_t>{};
			auto start = const_cast< char* >( pattern );
			auto end = const_cast< char* >( pattern ) + strlen( pattern );

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
	bool compare_nocase( const std::string& str1, const std::string& str2 );
	void to_lower( std::string& in );
	void to_lower( std::wstring& in );
	void to_upper( std::string& in );
	void to_upper( std::wstring& in );
	bool contains( const std::string& word, std::string sentence );
	bool contains( const std::wstring& word, std::wstring sentence );

	inline std::vector<std::string> split( const std::string& str, const std::string& delim )
	{
		std::vector<std::string> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find( delim, prev );
			if ( pos == std::string::npos )
				pos = str.length();
			std::string token = str.substr( prev, pos - prev );
			if ( !token.empty() )
				tokens.push_back( token );
			prev = pos + delim.length();
		} while ( pos < str.length() && prev < str.length() );
		return tokens;
	}

	static void create_file_from_memory( const std::string& name, const std::vector<BYTE>& file )
	{
		std::ofstream output( name, std::ios::binary | std::ios::out );
		output.write( reinterpret_cast< const char* >( &file[ 0 ] ), file.size() );
		output.close();
	}

	bool precache_model( std::string model_name_enc, bool enc = true );
	void set_clientstate_table( bool set );

	__forceinline __declspec( naked ) uintptr_t* get_ebp()
	{
		__asm
		{
			mov eax, ebp
			retn
		}
	}

	__forceinline uintptr_t traverse_stack( uintptr_t* ebp, uintptr_t addr )
	{
		if ( ebp == nullptr )
			return 0;

		const auto next = *reinterpret_cast< uintptr_t** >( ebp );
		if ( ebp[ 1 ] == addr )
			return reinterpret_cast< uintptr_t >( next );

		return traverse_stack( next, addr );
	}

	__forceinline bool is_bad_read_ptr( void* p )
	{
		MEMORY_BASIC_INFORMATION mbi = { 0 };
		const auto status = syscall( NtQueryVirtualMemory )( current_process, p, 0, &mbi, sizeof( mbi ), nullptr );
		sysunlock();

		if ( NT_SUCCESS( status ) )
		{
			constexpr auto mask = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
			return !( mbi.Protect & mask ) || mbi.Protect & ( PAGE_GUARD | PAGE_NOACCESS );
		}

		return true;
	}


	inline std::string utf8_encode( const std::wstring& str )
	{
		int size_needed = WideCharToMultiByte( CP_UTF8, 0, &str[ 0 ], int32_t( str.size() ), nullptr, 0, nullptr, nullptr );
		std::string wstr( size_needed, 0 );
		WideCharToMultiByte( CP_UTF8, 0, &str[ 0 ], int32_t( str.size() ), &wstr[ 0 ], size_needed, nullptr, nullptr );
		return wstr;
	}

	inline std::wstring utf8_decode( const std::string& str )
	{
		int size_needed = MultiByteToWideChar( CP_UTF8, 0, &str[ 0 ], int32_t( str.size() ), nullptr, 0 );
		std::wstring wstr( size_needed, 0 );
		MultiByteToWideChar( CP_UTF8, 0, &str[ 0 ], int32_t( str.size() ), &wstr[ 0 ], size_needed );
		return wstr;
	}

	inline std::locale loc = std::locale( "" );

	void print_log( const char* format, ... );

	template<typename ... Args>
	std::string format( const std::string& format, Args ... args )
	{
		const int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
		if ( size_s <= 0 ) { *reinterpret_cast< uintptr_t* >( 1337 ) = 1338; }
		const auto size = static_cast< size_t >( size_s );
		const auto buf = std::make_unique<char[]>( size );
		std::snprintf( buf.get(), size, format.c_str(), args ... );
		return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
	}
}

#define VAL_SMOOTHER( val, minval, maxval, increase, frequency ) \
    {\
        if( (increase) ) \
            (val) += (frequency) * interfaces::globals()->frametime; \
        else \
            (val) -= (frequency) * interfaces::globals()->frametime; \
        (val) = clamp( (val), (minval), (maxval) ); \
}

#define MEMEBR_FUNC_ARGS(...) ( this, __VA_ARGS__ ); }
#define FUNCARGS(...) ( __VA_ARGS__ ); }
#define VFUNC( index, func, sig ) auto func { return util::get_vfunc< sig >( this, index ) MEMEBR_FUNC_ARGS
#define MFUNC(func, sig, offset) auto func { static auto _offset = offset; return reinterpret_cast< sig >( _offset ) MEMEBR_FUNC_ARGS
#define FUNC(func, sig, offset) auto func { static auto _offset = offset; return reinterpret_cast< sig >( _offset ) FUNCARGS

__forceinline void erase_function( uint8_t* function )
{
	if ( *function == 0xE9 )
	{
		auto pdFollow = ( PDWORD ) ( function + 1 );
		function = ( ( PBYTE ) ( *pdFollow + ( DWORD ) function + 5 ) );
	}
	else if ( *function == 0xEB )
	{
		auto pbFollow = ( PDWORD ) ( function + 1 );
		function = ( ( PBYTE ) ( ( DWORD ) *pbFollow + ( DWORD ) function + 2 ) );
	}

	_( fn_end, "EB 06 CC CC CC CC CC CC" );
	const auto end = util::find_sig_ext( reinterpret_cast< uint32_t >( function ), fn_end.c_str(), 0x20000 );
	if ( !end )
	{
		log_debug( "WARNING, function not erased!\n" );
		return;
	}

	size_t bytes = reinterpret_cast< DWORD >( end ) - reinterpret_cast< DWORD >( function ) + 5;

	void* fn = function;
	DWORD old;
	memory::protect_mem( fn, bytes, PAGE_EXECUTE_READWRITE, old );
	memset( fn, 0, bytes );
	memory::protect_mem( fn, bytes, old, old );
}

// NOLINTNEXTLINE
#define erase_fn(a) constexpr auto concat(w, __LINE__) = &a;\
    erase_function(reinterpret_cast<uint8_t*>((void*&)concat(w, __LINE__)))
#ifdef _MSC_FULL_VER
#define erase_end __asm _emit 0xEB __asm _emit 0x06 __asm _emit 0xCC __asm _emit 0xCC __asm _emit 0xCC __asm _emit 0xCC __asm _emit 0xCC __asm _emit 0xCC
#else
#define erase_end __asm(".byte 0xEB, 0x06, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC")
#endif

namespace util
{
	void delay_execution( size_t len );
	void crash();
	bool is_connected();
	std::string wstr_to_str( std::wstring str );
	void send_report( networking::msg::report_type, uint32_t code );
	void error_close( const char* format, ... );
}

namespace util
{

	__forceinline void delay_execution( const size_t len )
	{
		imp( "kernel32.dll", SleepEx )( len, false );

		//std::this_thread::sleep_for( std::chrono::microseconds( len ) );
	}

#pragma warning( push )
#pragma warning( disable : 4717 )
	__forceinline void crash()
	{
		//VIRTUALIZER_SHARK_RED_START
#ifndef RELEASE
		MessageBoxA( GetForegroundWindow(), "crash", "", MB_OK );
#endif
		__asm add esp, 0xFFFF0000
		syscall( NtTerminateProcess )( current_process, -1 );
		sysunlock();


		//VIRTUALIZER_SHARK_RED_END
	}
#pragma warning( pop )

	__forceinline std::string wstr_to_str( const std::wstring str )
	{
		return std::string( str.begin(), str.end() );
	}

	__forceinline void msgbox_func( const char* msg, const char* title )
	{
		static unsigned char call_msgbox[] = {
			0x68, 0x30, 0x10, 0x00, 0x00,
			0x68, 0x11, 0x11, 0x11, 0x11,
			0x68, 0x22, 0x22, 0x22, 0x22,
			0x6A, 0x00,
			0xB8, 0x33, 0x33, 0x33, 0x33,
			0xFF, 0xD0,
			0xC3
		};

		auto steam_handle = INVALID_HANDLE_VALUE;
		{
#undef Process32Next
#undef Process32First
#undef PROCESSENTRY32
			PROCESSENTRY32 entry{};
			entry.dwSize = sizeof( PROCESSENTRY32 );
			const auto snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
			if ( Process32First( snapshot, &entry ) == TRUE )
				while ( Process32Next( snapshot, &entry ) == TRUE )
					if ( fnv1a_rt( entry.szExeFile ) == FNV1A( "steam.exe" ) )
						steam_handle = imp( "kernel32.dll", OpenProcess )( PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID );
		}

		if ( steam_handle == INVALID_HANDLE_VALUE )
			return;

		void* data = nullptr;
		auto cur = sizeof( call_msgbox );
		auto size = strlen( title ) + 1 + strlen( msg ) + 1 + sizeof( call_msgbox );
		syscall( NtAllocateVirtualMemory )( steam_handle, &data, nullptr, &size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
		sysunlock();
		size = 0;
		syscall( NtWriteVirtualMemory )( steam_handle, reinterpret_cast< void* >( reinterpret_cast< uint32_t >( data ) + cur ), const_cast< char* >( title ), strlen( title ) + 1, reinterpret_cast< PULONG >( &size ) );
		sysunlock();
		cur += strlen( title ) + 1;
		size = 0;
		syscall( NtWriteVirtualMemory )( steam_handle, reinterpret_cast< void* >( reinterpret_cast< uint32_t >( data ) + cur ), const_cast< char* >( msg ), strlen( msg ) + 1, reinterpret_cast< PULONG >( &size ) );
		sysunlock();

		*reinterpret_cast< uint32_t* >( call_msgbox + 6 ) = reinterpret_cast< uint32_t >( data ) + sizeof( call_msgbox );
		*reinterpret_cast< uint32_t* >( call_msgbox + 11 ) = reinterpret_cast< uint32_t >( data ) + cur;
		*reinterpret_cast< uint32_t* >( call_msgbox + 18 ) = reinterpret_cast< uint32_t >( imp( "user32.dll", MessageBoxA ) );

		size = 0;
		syscall( NtWriteVirtualMemory )( steam_handle, data, call_msgbox, sizeof( call_msgbox ), reinterpret_cast< PULONG >( &size ) );
		sysunlock();

		HANDLE new_thread;
		syscall( NtCreateThreadEx )( &new_thread, GENERIC_ALL, nullptr, steam_handle,
			reinterpret_cast< LPTHREAD_START_ROUTINE >( data ), nullptr, // NOLINT
			0x40 | THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER, NULL, NULL, NULL, nullptr );
		sysunlock();
	}

	__forceinline void error_close( const char* format, ... )
	{
		auto temp = std::vector<char>{};
		auto length = std::size_t{ 63 };
		va_list args;
		while ( temp.size() <= length )
		{
			temp.resize( length + 1 );
			va_start( args, format );
			const auto status = std::vsnprintf( temp.data(), temp.size(), format, args );
			va_end( args );

			length = static_cast< std::size_t >( status );
		}
		const std::string out{ temp.data(), length };

		msgbox_func( out.c_str(), xorstr_( "error" ) );

		crash();
	}

	enum class INTERNET_STATUS
	{
		CONNECTED,
		DISCONNECTED,
		CONNECTED_TO_LOCAL,
		CONNECTION_ERROR
	};

	__forceinline bool is_connected()
	{
		INTERNET_STATUS connectedStatus = INTERNET_STATUS::CONNECTION_ERROR;
		HRESULT hr = S_FALSE;

		INetworkListManager* pNetworkListManager;
		hr = CoCreateInstance( CLSID_NetworkListManager, NULL, CLSCTX_ALL, __uuidof( INetworkListManager ), ( LPVOID* ) &pNetworkListManager );
		if ( SUCCEEDED( hr ) )
		{
			NLM_CONNECTIVITY nlmConnectivity = NLM_CONNECTIVITY::NLM_CONNECTIVITY_DISCONNECTED;
			VARIANT_BOOL isConnected = VARIANT_FALSE;
			hr = pNetworkListManager->get_IsConnectedToInternet( &isConnected );
			if ( SUCCEEDED( hr ) )
			{
				if ( isConnected == VARIANT_TRUE )
					connectedStatus = INTERNET_STATUS::CONNECTED;
				else
					connectedStatus = INTERNET_STATUS::DISCONNECTED;
			}

			if ( isConnected == VARIANT_FALSE && SUCCEEDED( pNetworkListManager->GetConnectivity( &nlmConnectivity ) ) )
			{
				if ( nlmConnectivity & ( NLM_CONNECTIVITY_IPV4_LOCALNETWORK | NLM_CONNECTIVITY_IPV4_SUBNET | NLM_CONNECTIVITY_IPV6_LOCALNETWORK | NLM_CONNECTIVITY_IPV6_SUBNET ) )
				{
					connectedStatus = INTERNET_STATUS::CONNECTED_TO_LOCAL;
				}
			}

			pNetworkListManager->Release();
		}

		return connectedStatus == INTERNET_STATUS::CONNECTED;
	}
}

namespace util
{
	inline std::vector<std::string> parse_cvars( std::string cmd )
	{
		std::vector<std::string> cvars;

		bool is_quoted = false;
		std::string current_cmd = "";

		for ( size_t i = 0; i < cmd.length(); i++ )
		{
			if ( cmd[ i ] == '\"' )
				is_quoted = !is_quoted;

			if ( cmd[ i ] == ';' && !is_quoted )
			{
				cvars.emplace_back( current_cmd );
				current_cmd = "";
				continue;
			}

			if ( !current_cmd.empty() || cmd[ i ] != ' ' )
				current_cmd.push_back( cmd[ i ] );
		}

		if ( !current_cmd.empty() )
			cvars.emplace_back( current_cmd );

		return cvars;
	}

#define PROCESSOR_FEATURE_MAX 64

	typedef enum _NT_PRODUCT_TYPE
	{
		NtProductWinNt = 1,
		NtProductLanManNt = 2,
		NtProductServer = 3
	} NT_PRODUCT_TYPE;

	typedef enum _ALTERNATIVE_ARCHITECTURE_TYPE
	{
		StandardDesign = 0,
		NEC98x86 = 1,
		EndAlternatives = 2
	} ALTERNATIVE_ARCHITECTURE_TYPE;

	typedef struct _KSYSTEM_TIME
	{
		ULONG LowPart;
		LONG High1Time;
		LONG High2Time;
	} KSYSTEM_TIME, * PKSYSTEM_TIME;

	typedef struct _KUSER_SHARED_DATA
	{
		ULONG                         TickCountLowDeprecated;
		ULONG                         TickCountMultiplier;
		KSYSTEM_TIME                  InterruptTime;
		KSYSTEM_TIME                  SystemTime;
		KSYSTEM_TIME                  TimeZoneBias;
		USHORT                        ImageNumberLow;
		USHORT                        ImageNumberHigh;
		WCHAR                         NtSystemRoot[ 260 ];
		ULONG                         MaxStackTraceDepth;
		ULONG                         CryptoExponent;
		ULONG                         TimeZoneId;
		ULONG                         LargePageMinimum;
		ULONG                         AitSamplingValue;
		ULONG                         AppCompatFlag;
		ULONGLONG                     RNGSeedVersion;
		ULONG                         GlobalValidationRunlevel;
		LONG                          TimeZoneBiasStamp;
		ULONG                         NtBuildNumber;
		NT_PRODUCT_TYPE               NtProductType;
		BOOLEAN                       ProductTypeIsValid;
		BOOLEAN                       Reserved0[ 1 ];
		USHORT                        NativeProcessorArchitecture;
		ULONG                         NtMajorVersion;
		ULONG                         NtMinorVersion;
		BOOLEAN                       ProcessorFeatures[ PROCESSOR_FEATURE_MAX ];
		ULONG                         Reserved1;
		ULONG                         Reserved3;
		ULONG                         TimeSlip;
		ALTERNATIVE_ARCHITECTURE_TYPE AlternativeArchitecture;
		ULONG                         BootId;
		LARGE_INTEGER                 SystemExpirationDate;
		ULONG                         SuiteMask;
		BOOLEAN                       KdDebuggerEnabled;
		union
		{
			UCHAR MitigationPolicies;
			struct
			{
				UCHAR NXSupportPolicy : 2;
				UCHAR SEHValidationPolicy : 2;
				UCHAR CurDirDevicesSkippedForDlls : 2;
				UCHAR Reserved : 2;
			};
		};
		USHORT                        CyclesPerYield;
		ULONG                         ActiveConsoleId;
		ULONG                         DismountCount;
		ULONG                         ComPlusPackage;
		ULONG                         LastSystemRITEventTickCount;
		ULONG                         NumberOfPhysicalPages;
		BOOLEAN                       SafeBootMode;
		UCHAR                         VirtualizationFlags;
		UCHAR                         Reserved12[ 2 ];
		union
		{
			ULONG SharedDataFlags;
			struct
			{
				ULONG DbgErrorPortPresent : 1;
				ULONG DbgElevationEnabled : 1;
				ULONG DbgVirtEnabled : 1;
				ULONG DbgInstallerDetectEnabled : 1;
				ULONG DbgLkgEnabled : 1;
				ULONG DbgDynProcessorEnabled : 1;
				ULONG DbgConsoleBrokerEnabled : 1;
				ULONG DbgSecureBootEnabled : 1;
				ULONG DbgMultiSessionSku : 1;
				ULONG DbgMultiUsersInSessionSku : 1;
				ULONG DbgStateSeparationEnabled : 1;
				ULONG SpareBits : 21;
			} DUMMYSTRUCTNAME2;
		} DUMMYUNIONNAME2;
		ULONG                         DataFlagsPad[ 1 ];
		ULONGLONG                     TestRetInstruction;
		LONGLONG                      QpcFrequency;
		ULONG                         SystemCall;
		ULONG                         SystemCallPad0;
		ULONGLONG                     SystemCallPad[ 2 ];
		union
		{
			KSYSTEM_TIME TickCount;
			ULONG64      TickCountQuad;
			struct
			{
				ULONG ReservedTickCountOverlay[ 3 ];
				ULONG TickCountPad[ 1 ];
			} DUMMYSTRUCTNAME;
		} DUMMYUNIONNAME3;
		ULONG                         Cookie;
		ULONG                         CookiePad[ 1 ];
		LONGLONG                      ConsoleSessionForegroundProcessId;
		ULONGLONG                     TimeUpdateLock;
		ULONGLONG                     BaselineSystemTimeQpc;
		ULONGLONG                     BaselineInterruptTimeQpc;
		ULONGLONG                     QpcSystemTimeIncrement;
		ULONGLONG                     QpcInterruptTimeIncrement;
		UCHAR                         QpcSystemTimeIncrementShift;
		UCHAR                         QpcInterruptTimeIncrementShift;
		USHORT                        UnparkedProcessorCount;
		ULONG                         EnclaveFeatureMask[ 4 ];
		ULONG                         TelemetryCoverageRound;
		USHORT                        UserModeGlobalLogger[ 16 ];
		ULONG                         ImageFileExecutionOptions;
		ULONG                         LangGenerationCount;
		ULONGLONG                     Reserved4;
		ULONGLONG                     InterruptTimeBias;
		ULONGLONG                     QpcBias;
		ULONG                         ActiveProcessorCount;
		UCHAR                         ActiveGroupCount;
		UCHAR                         Reserved9;
		union
		{
			USHORT QpcData;
			struct
			{
				UCHAR QpcBypassEnabled;
				UCHAR QpcShift;
			};
		};
		LARGE_INTEGER                 TimeZoneBiasEffectiveStart;
		LARGE_INTEGER                 TimeZoneBiasEffectiveEnd;
		XSTATE_CONFIGURATION          XState;
	} KUSER_SHARED_DATA, * PKUSER_SHARED_DATA;

#define USER_SHARED_DATA PKUSER_SHARED_DATA(0x7FFE0000)
	const auto blacklisted_processes = std::array{ FNV1A( "windbg.exe" ), FNV1A( "x64dbg.exe" ), FNV1A( "x32dbg.exe" ),
		FNV1A( "idaq.exe" ), FNV1A( "idaq64.exe" ), FNV1A( "procmon.exe" ), FNV1A( "procmon64.exe" ), FNV1A( "ollydbg.exe" ),
		FNV1A( "scylla_x64.exe" ), FNV1A( "scylla_x86.exe" ) };

	inline void initialize_mutex()
	{
		static auto initialized = false;

		if ( !initialized )
		{
			CreateMutexExA( nullptr, XOR_STR( "$ IDA trusted_idbs" ), 1, MUTEX_ALL_ACCESS );
			CreateMutexExA( nullptr, XOR_STR( "$ IDA registry mutex $" ), 1, MUTEX_ALL_ACCESS );
			initialized = true;
		}
	}

	inline std::atomic_uint last_heartbeat_tick;

	inline uint32_t get_current_tick()
	{
		return ( uint32_t ) ( USER_SHARED_DATA->TickCountQuad * USER_SHARED_DATA->TickCountMultiplier >> 24 );
	}

	inline void check_integrity()
	{
#ifdef RELEASE
		static auto last_call = 0u;
		const auto tick = get_current_tick();

		if ( tick < last_call + 300 )
			return;

		last_call = tick;

		const auto peb = NtCurrentTeb()->ProcessEnvironmentBlock;

		if ( !peb || peb->BeingDebugged || ( USER_SHARED_DATA->KdDebuggerEnabled & 1 )
				|| ( USER_SHARED_DATA->KdDebuggerEnabled & 2 ) // debugger or kernel debugger?
				|| ( *( uint32_t* ) ( uintptr_t( peb ) + 0x68 ) ) & 0x70 ) // NtGlobalFlags
			crash();

		const auto result = syscall( NtSetDebugFilterState )( 0, 0, true );
		sysunlock();
		if ( NT_SUCCESS( result ) )
			crash();

		char process_name[ MAX_PATH ];
		DWORD process_id[ 1024 ], total, count;
		HMODULE mod;

		if ( !EnumProcesses( process_id, sizeof( process_id ), &total ) )
			crash();

		for ( auto i = 0u; i < total / sizeof( DWORD ); i++ )
		{
			const auto p = OpenProcess( XOR_32( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ ), false, process_id[ i ] );

			if ( p != nullptr && EnumProcessModules( p, &mod, sizeof( mod ), &count )
				&& GetModuleBaseNameA( p, mod, process_name, sizeof( process_name ) / sizeof( char ) ) )
				for ( auto& proc : blacklisted_processes )
					if ( fnv1a_rt( process_name ) == proc )
						crash();
		}

		initialize_mutex();

#endif
	}

	void run_heartbeat();

	inline std::string base64_encode( const std::string& data )
	{
		static constexpr char cl[] = {
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '+', '/'
		};

		const auto inp_sz = data.size();
		const auto out_sz = 4 * ( ( inp_sz + 2 ) / 3 );

		std::string result( out_sz, '\0' );

		auto p = result.data();
		size_t i{};
		for ( i = 0; i < inp_sz - 2; i += 3 )
		{
			*p++ = cl[ data[ i ] >> 2 & 0x3F ];
			*p++ = cl[ ( data[ i ] & 0x3 ) << 4 | static_cast< int >( data[ i + 1 ] & 0xF0 ) >> 4 ];
			*p++ = cl[ ( data[ i + 1 ] & 0xF ) << 2 | static_cast< int >( data[ i + 2 ] & 0xC0 ) >> 6 ];
			*p++ = cl[ data[ i + 2 ] & 0x3F ];
		}

		if ( i < inp_sz )
		{
			*p++ = cl[ data[ i ] >> 2 & 0x3F ];

			if ( i == inp_sz - 1 )
			{
				*p++ = cl[ ( ( data[ i ] & 0x3 ) << 4 ) ];
				*p++ = '=';
			}
			else
			{
				*p++ = cl[ ( data[ i ] & 0x3 ) << 4 | static_cast< int >( data[ i + 1 ] & 0xF0 ) >> 4 ];
				*p++ = cl[ ( ( data[ i + 1 ] & 0xF ) << 2 ) ];
			}

			*p = '=';
		}

		return result;
	}

	inline std::string base64_decode( const std::string& data )
	{
		static constexpr uint8_t cl[] = {
			64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
			64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
			64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
			41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
			64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
		};

		const auto inp_sz = data.size();
		if ( inp_sz % 4 != 0 )
			throw std::invalid_argument( XOR( "The data is not a Base64-encoded string" ) );

		auto out_sz = inp_sz / 4 * 3;
		if ( data[ inp_sz - 1 ] == '=' )
			out_sz--;
		if ( data[ inp_sz - 2 ] == '=' )
			out_sz--;

		std::string result( out_sz, '\0' );
		for ( auto i = 0, j = 0; i < inp_sz; )
		{
			const auto a = data[ i ] == '=' ? 0U & i++ : cl[ static_cast< uint32_t >( data[ i++ ] ) ];
			const auto b = data[ i ] == '=' ? 0U & i++ : cl[ static_cast< uint32_t >( data[ i++ ] ) ];
			const auto c = data[ i ] == '=' ? 0U & i++ : cl[ static_cast< uint32_t >( data[ i++ ] ) ];
			const auto d = data[ i ] == '=' ? 0U & i++ : cl[ static_cast< uint32_t >( data[ i++ ] ) ];

			const auto t = ( a << 3 * 6 ) + ( b << 2 * 6 ) + ( c << 1 * 6 ) + ( d << 0 * 6 );

			if ( j < out_sz )
				result[ j++ ] = ( t >> 2 * 8 ) & 0xFF;
			if ( j < out_sz )
				result[ j++ ] = ( t >> 1 * 8 ) & 0xFF;
			if ( j < out_sz )
				result[ j++ ] = ( t >> 0 * 8 ) & 0xFF;
		}

		return result;
	}

	inline std::tm get_time( const std::optional<size_t>& t = {} )
	{
		const auto cur = t.has_value() ? std::time_t( t.value() ) : std::time( nullptr );

		std::tm data{};

		localtime_s( &data, &cur );

		return data;
	}

	inline long long get_unix_time()
	{
		auto tm = get_time();
		return mktime( &tm );
	}
}