#pragma once
#ifdef ALPHA
#include "StackTracer.h"
#endif

#include "networking/include/data.h"

__declspec( dllexport ) extern uint32_t rdatastart;
__declspec( dllexport ) extern uint32_t rdataend;
__declspec( dllexport ) extern uint32_t textstart;
__declspec( dllexport ) extern uint32_t textend;

namespace protection
{
	inline bool CheckDebugOutDbgStr( const char* szMsg )
	{
		ULONG_PTR args[ 2 ];
		args[ 0 ] = ( ULONG_PTR ) strlen( szMsg ) + 1;
		args[ 1 ] = ( ULONG_PTR ) szMsg;
		bool bResult = false;

		__try
		{
			RaiseException( 0x40010006, 0, 2, args );
			bResult = true;
		}
		__except ( EXCEPTION_EXECUTE_HANDLER )
		{

		}
		return bResult;
	}

	static inline std::unordered_map<uint32_t, uint64_t> detoured_map;
	static inline std::unordered_map<HANDLE, ULONG> protected_threads;

	static __int64 __fastcall hkDbgUiIssueRemoteBreakin( __int64 a1 )
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;


		//nlohmann::json root;
		//root[ _r( "id" ) ] = _r( "DebuggerAttached" );
		//root[ _r( "extra" ) ] = _r( "DbgUiIssueRemoteBreakin" );

		util::send_report( networking::msg::report_type::TYPE1, 0xCD000001 );

		util::crash();

		VIRTUALIZER_DOLPHIN_WHITE_END;
		return 0;
	}

	static __int64 __fastcall hkDbgUiConnectToDbg()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		/*nlohmann::json root;
		root[ _r( "id" ) ] = _r( "DebuggerAttached" );
		root[ _r( "extra" ) ] = _r( "DbgUiConnectToDbg" );*/

		util::send_report( networking::msg::report_type::TYPE1, 0xCD000002 );

		util::crash();

		VIRTUALIZER_DOLPHIN_WHITE_END;
		return 0;
	}

	static inline uintptr_t oRtlGetFullPathName_U;
	static ULONG NTAPI hkRtlGetFullPathName_U( IN PCWSTR FileName, IN ULONG Size, IN PWSTR Buffer, OUT PWSTR* ShortName )
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		if (
			wcsstr( FileName, _wc( "HookLibraryx64" ).c_str() ) ||
			wcsstr( FileName, _wc( "vehdebug-x86_64" ).c_str() ) ||
			wcsstr( FileName, _wc( "winhook-x86_64" ).c_str() )
			)
		{

			auto szFilePath = util::wstr_to_str( FileName );

			/*nlohmann::json root;
			root[ _r( "id" ) ] = _r( "BlacklistedModuleLoaded" );
			root[ _r( "extra" ) ] = szFilePath.c_str();*/

			util::send_report( networking::msg::report_type::TYPE1, 0xCB000001 );

			util::crash();

			return 0;
		}

		VIRTUALIZER_DOLPHIN_WHITE_END;
		return reinterpret_cast< decltype( &hkRtlGetFullPathName_U ) >( oRtlGetFullPathName_U )( FileName, Size, Buffer, ShortName );
	}

	static inline DWORD exception_magic_code;

	static ULONG NTAPI AntiDebugHandler( _EXCEPTION_POINTERS* p )
	{
		if ( p->ExceptionRecord->ExceptionCode == 0xC000001D )//&& *( BYTE *)( p->ContextRecord->Rip + 1 ) == 0xC3 )
		{
			/*nlohmann::json root;
			root[ _r( "id" ) ] = _r( "AntiDebug" );
			root[ _r( "extra" ) ] = _r( "Loader was attached to debugger" );*/

			//VIRTUALIZER_DOLPHIN_WHITE_START;
			util::send_report( networking::msg::report_type::TYPE1, 0xCD000003 );
			util::crash();
			//VIRTUALIZER_DOLPHIN_WHITE_END;

			return EXCEPTION_CONTINUE_EXECUTION;
		}

		if ( p->ExceptionRecord->ExceptionCode == 0xCA001337 )
		{
			exception_magic_code = 0xDEADBEEF;
			return EXCEPTION_CONTINUE_EXECUTION;
		}

		if ( p->ExceptionRecord->ExceptionCode == 0xC0000096 )
			return EXCEPTION_CONTINUE_EXECUTION;

#ifdef ALPHA
		const auto Exception = p->ExceptionRecord;
		const auto code = Exception->ExceptionCode;
		if ( code < 0x80000000 || code == 0xE06D7363 )
			return EXCEPTION_CONTINUE_SEARCH;

		const auto write = Exception->ExceptionInformation[ 0 ];
		const auto to = Exception->ExceptionInformation[ 1 ];

		StackTracer::ExceptionFilter( p );

		log( "----------------------------- CRASH -----------------------------\n" );
		_( READ, "READ" );
		_( WRITE, "WRITE" );
		util::print_log( "%d %s : %d\n", code, write ? WRITE.c_str() : READ.c_str(), to );
		util::print_log( "eax : %d\n", p->ContextRecord->Eax );
		util::print_log( "ebx : %d\n", p->ContextRecord->Ebx );
		util::print_log( "ecx : %d\n", p->ContextRecord->Ecx );
		util::print_log( "edx : %d\n", p->ContextRecord->Edx );
		util::print_log( "esi : %d\n", p->ContextRecord->Esi );
		util::print_log( "edi : %d\n", p->ContextRecord->Edi );
		util::print_log( "%s", StackTracer::GetExceptionMsg().c_str() );
		log( "-----------------------------------------------------------------" );
#endif
		return EXCEPTION_CONTINUE_SEARCH;
	}

	static inline std::atomic_bool isDebuggerPresentPatched{};

	enum thread_identifiers : int
	{
		id_CheckCodeSection = ( 1 << 0 ),
		id_CheckAntiDebugIntegrity = ( 1 << 1 ),
		id_CheckBlacklistedDrivers = ( 1 << 2 ),
		id_CheckDebugger = ( 1 << 3 ),
		id_HasTitanHide = ( 1 << 4 ),
		id_AntiVirtualMachine = ( 1 << 5 ),
		id_CheckHosts = ( 1 << 6 ),
		id_CheckThreads = ( 1 << 7 ),
		id_CheckInternetConnection = ( 1 << 8 ),
		id_ran_base = id_CheckCodeSection | id_CheckAntiDebugIntegrity | id_CheckBlacklistedDrivers | id_CheckDebugger/* | id_HasTitanHide*/ /*| id_AntiVirtualMachine*/ | id_CheckHosts /*| id_CheckInternetConnection*/,
		id_ran_all = id_CheckCodeSection | id_CheckAntiDebugIntegrity | id_CheckBlacklistedDrivers | id_CheckDebugger/* | id_HasTitanHide*/ /*| id_AntiVirtualMachine*/ | id_CheckHosts /*| id_CheckInternetConnection*/ | id_CheckThreads,
	};


	static __declspec( noinline ) void replaceIsDbgPresent()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		static const uint8_t IsDbgPresent_shell[] = { 0xB8, 0x39, 0x05, 0x00, 0x00, 0xC3 };

		ULONG Old{};

		auto IsDbgPresent = PBYTE( imp( "kernel32.dll", IsDebuggerPresent ) );
		if ( IsDbgPresent )
		{
			memory::protect_mem( IsDbgPresent, 0x1000, PAGE_EXECUTE_READWRITE, Old );
			RtlCopyMemory( IsDbgPresent, IsDbgPresent_shell, sizeof( IsDbgPresent_shell ) );
			memory::protect_mem( IsDbgPresent, 0x1000, Old, Old );

			detoured_map.insert( { uint32_t( IsDbgPresent ), crc64::calculate( uint32_t( IsDbgPresent ), 5 ) } );
			if ( memcmp( IsDbgPresent, IsDbgPresent_shell, sizeof( IsDbgPresent_shell ) ) == 0 )
				isDebuggerPresentPatched.store( true );
			else
			{
				/*char extra_info[ 200 ]{};
				sprintf_s( extra_info, _r( "IsDebuggerPresent patch failed!" ) );*/

				/*nlohmann::json root;
				root[ _r( "id" ) ] = _r( "AntiDetourPatchFailed" );
				root[ _r( "extra" ) ] = extra_info;*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCF000000 );

				util::crash();
			}
		}
		else
		{
			/*nlohmann::json root;
			root[ _r( "id" ) ] = _r( "AntiDebugHook" );
			root[ _r( "extra" ) ] = _r( "IsDebuggerPresent not found!" );*/

			util::send_report( networking::msg::report_type::TYPE1, 0xCF000001 );
		}

		VIRTUALIZER_DOLPHIN_WHITE_END;
	}

	static __declspec( noinline ) void AntiDebugDetours()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		log_debug( "ex start\n" );

		ULONG Old{};

		log_debug( "MH_Initialize\n" );
		auto mhs = MH_Initialize();
		if ( mhs != MH_OK )
		{
			/*char extra_info[ 200 ]{};
			sprintf_s( extra_info, _r( "MH_Initialize returned %d" ), mhs );

			nlohmann::json root;
			root[ _r( "id" ) ] = _r( "MinHookFailure" );
			root[ _r( "extra" ) ] = extra_info;*/

			util::send_report( networking::msg::report_type::TYPE1, 0xCF000002 );

			util::crash();
		}

		log_debug( "fnDbgUiIssueRemoteBreakin\n" );
		auto fnDbgUiIssueRemoteBreakin = import( "ntdll.dll", DbgUiIssueRemoteBreakin );
		if ( fnDbgUiIssueRemoteBreakin )
		{
			PVOID Dummy;
			mhs = MH_CreateHook( ( void* ) fnDbgUiIssueRemoteBreakin, ( void* ) &hkDbgUiIssueRemoteBreakin, &Dummy );
			if ( mhs == MH_OK )
				mhs = MH_EnableHook( fnDbgUiIssueRemoteBreakin );
			if ( mhs != MH_OK )
			{
				/*char extra_info[ 200 ]{};
				sprintf_s( extra_info, _r( "MH_CreateHook(DbgUiIssueRemoteBreakin) returned %d" ), mhs );

				nlohmann::json root;
				root[ _r( "id" ) ] = _r( "MinHookFailure" );
				root[ _r( "extra" ) ] = extra_info;*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCF000003 );

				util::crash();
			}

			detoured_map.insert( { uint32_t( fnDbgUiIssueRemoteBreakin ), crc64::calculate( uint32_t( fnDbgUiIssueRemoteBreakin ), 5 ) } );
		}
		else
		{
			/*nlohmann::json root;
			root[ _r( "id" ) ] = _r( "AntiDebugHook" );
			root[ _r( "extra" ) ] = _r( "DbgUiIssueRemoteBreakin not found!" );*/

			util::send_report( networking::msg::report_type::TYPE1, 0xCF000004 );

			util::crash();
		}

		//CreateMutex( nullptr, TRUE, _wc( "$ IDA registry mutex $" ).c_str() );
		//CreateMutex( nullptr, TRUE, _wc( "$ IDA trusted_idbs" ).c_str() );

		log_debug( "fnDbgUiRemoteBreakin\n" );
		auto fnDbgUiRemoteBreakin = import( "ntdll.dll", DbgUiRemoteBreakin );
		if ( fnDbgUiRemoteBreakin )
		{
			memory::protect_mem( ( void* ) fnDbgUiRemoteBreakin, 0x1000, PAGE_EXECUTE_READWRITE, Old );
			RtlCopyMemory( ( void* ) fnDbgUiRemoteBreakin, "\x0F\x0B", 2 );
			memory::protect_mem( ( void* ) fnDbgUiRemoteBreakin, 0x1000, Old, Old );

			detoured_map.insert( { uint32_t( fnDbgUiRemoteBreakin ), crc64::calculate( uint32_t( fnDbgUiRemoteBreakin ), 5 ) } );
		}
		else
		{
			/*nlohmann::json root;
			root[ _r( "id" ) ] = _r( "AntiDebugHook" );
			root[ _r( "extra" ) ] = _r( "DbgUiRemoteBreakin not found!" );*/

			util::send_report( networking::msg::report_type::TYPE1, 0xCF000005 );

			util::crash();
		}

		log_debug( "fnDbgUserBreakPoint\n" );
		auto fnDbgUserBreakPoint = import( "ntdll.dll", DbgUserBreakPoint );
		if ( fnDbgUserBreakPoint )
		{
			memory::protect_mem( ( void* ) fnDbgUserBreakPoint, 0x1000, PAGE_EXECUTE_READWRITE, Old );
			RtlCopyMemory( ( void* ) fnDbgUserBreakPoint, "\x0F\x0B", 2 );
			memory::protect_mem( ( void* ) fnDbgUserBreakPoint, 0x1000, Old, Old );

			detoured_map.insert( { uint32_t( fnDbgUserBreakPoint ), crc64::calculate( uint32_t( fnDbgUserBreakPoint ), 5 ) } );
		}
		else
		{
			/*nlohmann::json root;
			root[ _r( "id" ) ] = _r( "AntiDebugHook" );
			root[ _r( "extra" ) ] = _r( "DbgUserBreakPoint not found!" );*/

			util::send_report( networking::msg::report_type::TYPE1, 0xCF000006 );

			util::crash();
		}

		log_debug( "fnDbgUiConnectToDbg\n" );
		auto fnDbgUiConnectToDbg = import( "ntdll.dll", DbgUiConnectToDbg );
		if ( fnDbgUiConnectToDbg )
		{
			PVOID Dummy;
			mhs = MH_CreateHook( ( void* ) fnDbgUiConnectToDbg, &hkDbgUiConnectToDbg, &Dummy );
			if ( mhs == MH_OK )
				mhs = MH_EnableHook( fnDbgUiConnectToDbg );
			if ( mhs != MH_OK )
			{
				/*char extra_info[ 200 ]{};
				sprintf_s( extra_info, _r( "MH_CreateHook(DbgUiConnectToDbg) returned %d" ), mhs );

				nlohmann::json root;
				root[ _r( "id" ) ] = _r( "MinHookFailure" );
				root[ _r( "extra" ) ] = extra_info;*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCF000007 );

				util::crash();
			}

			detoured_map.insert( { uint32_t( fnDbgUiConnectToDbg ), crc64::calculate( uint32_t( fnDbgUiConnectToDbg ), 5 ) } );
		}
		else
		{
			/*nlohmann::json root;
			root[ _r( "id" ) ] = _r( "AntiDebugHook" );
			root[ _r( "extra" ) ] = _r( "DbgUiConnectToDbg not found!" );*/

			util::send_report( networking::msg::report_type::TYPE1, 0xCF000008 );

			util::crash();
		}

		log_debug( "fnRtlGetFullPathName_U\n" );
		auto fnRtlGetFullPathName_U = import( "ntdll.dll", RtlGetFullPathName_U );
		if ( fnRtlGetFullPathName_U )
		{
			oRtlGetFullPathName_U = ( uintptr_t ) fnRtlGetFullPathName_U;

			mhs = MH_CreateHook( reinterpret_cast< PVOID >( fnRtlGetFullPathName_U ), ( void* ) &hkRtlGetFullPathName_U, reinterpret_cast< PVOID* >( &oRtlGetFullPathName_U ) );
			if ( mhs == MH_OK )
				mhs = MH_EnableHook( fnRtlGetFullPathName_U );
			if ( mhs != MH_OK )
			{
				/*char extra_info[ 200 ]{};
				sprintf_s( extra_info, _r( "MH_CreateHook(RtlGetFullPathName_U) returned %d" ), mhs );

				nlohmann::json root;
				root[ _r( "id" ) ] = _r( "MinHookFailure" );
				root[ _r( "extra" ) ] = extra_info;*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCF000009 );

				util::crash();

			}

			detoured_map.insert( { uint32_t( fnRtlGetFullPathName_U ), crc64::calculate( uint32_t( fnRtlGetFullPathName_U ), 5 ) } );
		}
		else
		{
			/*nlohmann::json root;
			root[ _r( "id" ) ] = _r( "AntiDebugHook" );
			root[ _r( "extra" ) ] = _r( "RtlGetFullPathName_U not found!" );*/

			util::send_report( networking::msg::report_type::TYPE1, 0xCF00000A );

			util::crash();
		}

		log_debug( "end\n" );
		VIRTUALIZER_DOLPHIN_WHITE_END;
	}

	static __declspec( noinline ) void CheckCodeSection()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		static bool once = true;

		while ( true )
		{
			if ( once )
			{
				once = false;

				globals::secure_info.initial_checksum = crc64::calculate( textstart, textend, rdatastart, rdataend );
				if ( !globals::secure_info.initial_checksum )
					break;
			}

			const auto cursum = crc64::calculate( textstart, textend, rdatastart, rdataend );

			if ( !( globals::secure_info.values.threads_ran & static_cast< int >( id_CheckCodeSection ) ) )
				log( "0.36\n" );
			globals::secure_info.values.threads_ran |= static_cast< unsigned char >( id_CheckCodeSection );

			if ( globals::secure_info.initial_checksum != cursum )
			{
				/*nlohmann::json root;
				root[ _r( "id" ) ] = _r( "CrcCheckFailed" );
				root[ _r( "extra" ) ] = extra_info;*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCC000001 );

				util::crash();

				break;
			}

			util::delay_execution( 2500 );
		}

		VIRTUALIZER_DOLPHIN_WHITE_END;
	}

	volatile inline bool noopt = false;

	static __declspec( noinline ) void CheckAntiDebugIntegrity()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		log_debug( "detours start\n" );
		AntiDebugDetours();
		log_debug( "detours done\n" );

		while ( true )
		{
			for ( auto& p : detoured_map )
			{
				const auto cursum = crc64::calculate( p.first, 5 );
				if ( cursum != p.second )
				{

					/*char extra_info[ 200 ]{};
					sprintf_s( extra_info, _r( "Expected: 0x%llX - Got: 0x%llX" ), p.second, cursum );

					nlohmann::json root;
					root[ _r( "id" ) ] = _r( "DetoursIntegrityFail" );
					root[ _r( "extra" ) ] = extra_info;*/

					util::send_report( networking::msg::report_type::TYPE1, 0xCC000002 );

					util::crash();

					break;
				}
			}

			exception_magic_code = NULL;
			imp( "kernel32.dll", RaiseException )( 0xCA001337, 0ul, 0ul, nullptr );

			if ( exception_magic_code == 0xc0000022 ) // STATUS_ACCESS_DENIED 
			{
				log_debug( "STATUS_ACCESS_DENIED\n" );
				continue;
			}


			if ( exception_magic_code != 0xDEADBEEF )
			{
				/*char extra_info[ 200 ]{};
				sprintf_s( extra_info, _r( "Code 0x%X" ), exception_magic_code );

				nlohmann::json root;
				root[ _r( "id" ) ] = _r( "ExpHandlerIntegrityFail" );
				root[ _r( "extra" ) ] = extra_info;*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCC000003 );

				util::crash();

				break;
			}

			if ( !( globals::secure_info.values.threads_ran & static_cast< int >( id_CheckAntiDebugIntegrity ) ) )
				log( "0.37\n" );
			globals::secure_info.values.threads_ran |= static_cast< unsigned char >( id_CheckAntiDebugIntegrity );

			util::run_heartbeat();

			util::delay_execution( 5 * 1000 );
		}

		VIRTUALIZER_DOLPHIN_WHITE_END;
	}

	static __declspec( noinline ) void CheckBlacklistedDrivers()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		while ( true )
		{
			static std::unique_ptr< uint8_t[] > buffer;
			static ULONG buflen = 0x10000;
			static NTSTATUS status = 0xC0000001; // STATUS_UNSUCCESSFUL

			do
			{
				buffer.reset( new uint8_t[ buflen ] );

				status = sys( NtQuerySystemInformation )( ( SYSTEM_INFORMATION_CLASS ) 0x0B/*SystemModuleInformation*/, buffer.get(), buflen, &buflen );
				sysunlock();

				if ( !NT_SUCCESS( status ) && status != 0xC0000004 )
				{
					/*nlohmann::json root;
					root[ _r( "id" ) ] = _r( "NtQuerySystemInformation" );
					root[ _r( "extra" ) ] = std::to_string( status );*/

					util::send_report( networking::msg::report_type::TYPE1, 0xCB000002 );

					util::crash();
					continue;
				}
			} while ( !NT_SUCCESS( status ) );


			const auto ModuleInfo = PRTL_PROCESS_MODULES( &buffer[ 0 ] );
			for ( auto i = 0ul; i < ModuleInfo->NumberOfModules; i++ )
			{
				const auto cur_module = &ModuleInfo->Modules[ i ];

				std::string driver_name( PCHAR( cur_module->FullPathName ) );
				std::transform( driver_name.begin(), driver_name.end(), driver_name.begin(), ::tolower );

				if (
					driver_name.find( _r( "kprocesshacker" ) ) != driver_name.npos ||
					driver_name.find( _r( "procmon" ) ) != driver_name.npos ||
					driver_name.find( _r( "kdcom" ) ) != driver_name.npos ||
					driver_name.find( _r( "dbgv" ) ) != driver_name.npos ||
					driver_name.find( _r( "dbk64" ) ) != driver_name.npos
					)
				{
					/*nlohmann::json root;
					root[ _r( "id" ) ] = _r( "DriverBlackList" );
					root[ _r( "extra" ) ] = driver_name;

					util::send_report( root );*/

					util::send_report( networking::msg::report_type::TYPE1, 0xCB000000 );

					util::error_close( _r( "Please open a ticket for more information" ), driver_name.c_str() );
					goto end;
				}
			}

			if ( !( globals::secure_info.values.threads_ran & static_cast< int >( id_CheckBlacklistedDrivers ) ) )
				log( "0.38\n" );
			globals::secure_info.values.threads_ran |= static_cast< unsigned char >( id_CheckBlacklistedDrivers );

			util::delay_execution( 2255 );

		}
end:
		VIRTUALIZER_DOLPHIN_WHITE_END;
	}

	static __declspec( noinline ) void CheckDebugger()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		while ( true )
		{
			NTSTATUS status;

			util::delay_execution( 15 );

			//MessageBoxA( nullptr, "2", "yep", MB_OK );
#ifndef _DEBUG
			//
			// Debug Prevention checking some flags
			//
			DWORD_PTR isDebuggerPresent = FALSE;

			status = query_info_process( current_process, ProcessDebugPort, &isDebuggerPresent, sizeof( isDebuggerPresent ), nullptr );
			sysunlock();

			//MessageBoxA( nullptr, "3", "yep", MB_OK );

			if ( NT_SUCCESS( status ) && isDebuggerPresent != 0 )
			{
				/*nlohmann::json root;
				root[ _r( "id" ) ] = _r( "DebuggerFound" );
				root[ _r( "extra" ) ] = _r( "ProcessDebugPort" );*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCD000004 );

				util::crash();

				break;
			}

			util::delay_execution( 550 );

			//MessageBoxA( nullptr, "4", "yep", MB_OK );

			ULONG hDebugObj = 0;
			status = query_info_process( current_process, static_cast< PROCESSINFOCLASS >( 0x1e ) /*ProcessDebugObjectHandle*/, &hDebugObj, sizeof( hDebugObj ), nullptr );
			sysunlock();

			if ( NT_SUCCESS( status ) && hDebugObj )
			{

				/*nlohmann::json root;
				root[ _r( "id" ) ] = _r( "DebuggerFound" );
				root[ _r( "extra" ) ] = _r( "ProcessDebugObjectHandle" );*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCD000005 );

				util::crash();

				break;
			}

			util::delay_execution( 550 );

			ULONG ulDebugFlags{};
			status = query_info_process( current_process, static_cast< PROCESSINFOCLASS >( 0x1f ) /*ProcessDebugFlags*/, &ulDebugFlags, sizeof( ulDebugFlags ), nullptr );
			sysunlock();

			if ( NT_SUCCESS( status ) && ulDebugFlags == NULL )
			{

				/*nlohmann::json root;
				root[ _r( "id" ) ] = _r( "DebuggerFound" );
				root[ _r( "extra" ) ] = _r( "ProcessDebugFlags" );*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCD000006 );

				util::crash();

				break;
			}
#endif

			util::delay_execution( 550 );

			typedef struct
			{
				BOOLEAN DebuggerEnabled;
				BOOLEAN DebuggerNotPresent;
			} SYSTEM_KERNEL_DEBUGGER_INFORMATION;

			SYSTEM_KERNEL_DEBUGGER_INFORMATION Info{};

			status = sys( NtQuerySystemInformation )( static_cast< SYSTEM_INFORMATION_CLASS >( 0x23 ) /*SystemKernelDebuggerInformation*/, &Info, ULONG( sizeof( Info ) ), 0ul );
			sysunlock();

			if ( NT_SUCCESS( status ) )
			{
				if ( Info.DebuggerEnabled )
				{
					if ( !Info.DebuggerNotPresent )
					{

						/*nlohmann::json root;
						root[ _r( "id" ) ] = _r( "DebuggerFound" );
						root[ _r( "extra" ) ] = _r( "SystemKernelDebugger" );*/

						util::send_report( networking::msg::report_type::TYPE1, 0xCD000007 );

						util::crash();

						break;
					}
				}
			}

			util::delay_execution( 550 );

			//
			// Check shared user data
			//
			unsigned char b = *( unsigned char* ) 0x7ffe02d4;
			if ( ( b & 0x01 ) || ( b & 0x02 ) )
			{

				/*nlohmann::json root;
				root[ _r( "id" ) ] = _r( "DebuggerFound" );
				root[ _r( "extra" ) ] = _r( "SharedUserData" );*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCD000008 );

				util::crash();

				break;
			}

			//
			// Check system integrity
			//
			SYSTEM_CODEINTEGRITY_INFORMATION Integrity = { sizeof( SYSTEM_CODEINTEGRITY_INFORMATION ), 0 };

			status = sys( NtQuerySystemInformation )( static_cast< SYSTEM_INFORMATION_CLASS >( 103 ), &Integrity, ULONG( sizeof( Integrity ) ), 0ul );
			sysunlock();

			if ( NT_SUCCESS( status ) )
			{
				if ( !( Integrity.CodeIntegrityOptions & CODEINTEGRITY_OPTION_ENABLED ) )
				{
					/*nlohmann::json root;
					root[ _r( "id" ) ] = _r( "CodeIntegrity" );
					root[ _r( "extra" ) ] = _r( "Disabled" );*/

					util::send_report( networking::msg::report_type::TYPE1, 0xCD000009 );

					util::crash();

					//util::error_close( _r( "Please enable Code Integrity in order to use the loader." ) );
					break;
				}

				if ( Integrity.CodeIntegrityOptions & CODEINTEGRITY_OPTION_TESTSIGN )
				{
					/*nlohmann::json root;
					root[ _r( "id" ) ] = _r( "CodeIntegrity" );
					root[ _r( "extra" ) ] = _r( "TestSignOn" );*/

					util::send_report( networking::msg::report_type::TYPE1, 0xCD00000A );

					util::crash();

					//util::error_close( _r( "Please disable Windows Test mode in order to use the load" ) );
					break;
				}

				if ( Integrity.CodeIntegrityOptions & CODEINTEGRITY_OPTION_DEBUGMODE_ENABLED )
				{
					/*nlohmann::json root;
					root[ _r( "id" ) ] = _r( "CodeIntegrity" );
					root[ _r( "extra" ) ] = _r( "DebugModeOn" );*/

					util::send_report( networking::msg::report_type::TYPE1, 0xCD00000B );

					util::crash();

					//util::error_close( _r( "Please disable Windows Debug mode in order to use the loader." ) );
					break;
				}
			}

			static bool once = true;

			HKEY hKey{};
			if ( once && imp( "advapi32.dll", RegOpenKeyA )( HKEY_LOCAL_MACHINE, _r( "SYSTEM\\CurrentControlSet\\Control" ), &hKey ) == NO_ERROR )
			{
				once = false;
				char szRegData[ 1024 ]{};
				DWORD dwRegCb = sizeof( szRegData ) / sizeof( *szRegData );
				DWORD Type = REG_SZ;

				if ( imp( "advapi32.dll", RegQueryValueExA )( hKey, _r( "SystemStartOptions" ), nullptr, &Type, reinterpret_cast< PBYTE >( szRegData ), &dwRegCb ) == NO_ERROR )
				{
					imp( "advapi32.dll", RegCloseKey )( hKey );

					if (
						strstr( szRegData, _r( "TESTSIGNING" ) ) ||
						strstr( szRegData, _r( "DISABLE_INTEGRITY_CHECKS" ) ) )
					{
						/*nlohmann::json root;
						root[ _r( "id" ) ] = _r( "SystemStartOptions" );
						root[ _r( "extra" ) ] = szRegData;*/

						util::send_report( networking::msg::report_type::TYPE1, 0xCD00000C );

						util::crash();

						//util::error_close( _r( "This application cannot run under a modified kernel environment!" ) );
						break;
					}
				}
				else
					imp( "advapi32.dll", RegCloseKey )( hKey );
			}

			/*static auto LuidsMatch = [] ( LUID l1, LUID l2 )
			{
				return l1.LowPart == l2.LowPart && l1.HighPart == l2.HighPart;
			};

			HANDLE hToken{};
			if ( OpenProcessToken( current_process, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
			{
				//Find the LUID for the debug privilege token
				LUID luidDebugPrivilege;
				if ( LookupPrivilegeValueA(
					NULL,            // lookup privilege on local system
					_r( "SeDebugPrivilege" ),   // privilege to lookup
					&luidDebugPrivilege ) )        // receives LUID of privilege
				{

					//Find if the "debug programs" privilege is already assigned to this process
					DWORD dwReturnedDataSize;
					GetTokenInformation(
						hToken,
						TokenPrivileges,
						NULL,
						0,
						&dwReturnedDataSize );

					BYTE* pData = new BYTE[ dwReturnedDataSize ];
					GetTokenInformation(
						hToken,
						TokenPrivileges,
						pData,
						dwReturnedDataSize,
						&dwReturnedDataSize );

					TOKEN_PRIVILEGES* pPrivileges = ( TOKEN_PRIVILEGES* ) pData;

					bool bResult = false;

					for ( unsigned int count = 0; count < pPrivileges->PrivilegeCount; count++ )
					{
						LUID_AND_ATTRIBUTES& luidAndAttrs = pPrivileges->Privileges[ count ];

						if ( LuidsMatch( luidAndAttrs.Luid, luidDebugPrivilege ) )
						{
							if ( ( luidAndAttrs.Attributes & SE_PRIVILEGE_ENABLED ) == SE_PRIVILEGE_ENABLED )
							{
								bResult = true;
								break;
							}
							else
							{
								bResult = false;
								break;
							}
						}
					}

					if ( bResult == false )
					{
						nlohmann::json root;
						root[ _r( "id" ) ] = _r( "PrivilegeCheck" );
						root[ _r( "extra" ) ] = _r( "NoDebugPrivileges" );

						util::send_report( root );

						util::crash();
						return;
					}
				}
			}*/

			static bool once2 = true;
			if ( once2 )
			{
				once2 = false;
				UNICODE_STRING LicenseValue;
				imp( "ntdll.dll", RtlInitUnicodeString )( &LicenseValue, _wc( "CodeIntegrity-AllowConfigurablePolicy-CustomKernelSigners" ).c_str() );

				ULONG Result = 0, ReturnLength;
				const auto ql_val = syscall( NtQueryLicenseValue )( &LicenseValue, nullptr, reinterpret_cast< PVOID >( &Result ), 4ul, &ReturnLength );
				sysunlock();

				if ( NT_SUCCESS( ql_val ) )
				{
					if ( Result != 0 )
					{
						/*nlohmann::json root;
						root[ _r( "id" ) ] = _r( "SystemStartOptions" );
						root[ _r( "extra" ) ] = _r( "CustomKernelSignOn" );*/

						util::send_report( networking::msg::report_type::TYPE1, 0xCD00000D );

						util::crash();

						//util::error_close( _r( "This application cannot run under a non secure kernel environment!" ) );
						break;
					}
				}
			}

#ifndef _DEBUG

			util::delay_execution( 550 );
			int expectedValue = isDebuggerPresentPatched.load() == false ? 0 : 1337;
			if ( imp( "kernel32.dll", IsDebuggerPresent )( ) != expectedValue )
			{

				/*nlohmann::json root;
				root[ _r( "id" ) ] = _r( "DebuggerFound" );
				root[ _r( "extra" ) ] = _r( "IsDebuggerPresent" );*/


				util::send_report( networking::msg::report_type::TYPE1, 0xCD00000E );

				util::crash();

				break;
			}

			util::delay_execution( 550 );

			BOOL bIsDebuggerPresent = FALSE;
			imp( "kernel32.dll", CheckRemoteDebuggerPresent )( current_process, &bIsDebuggerPresent );
			if ( bIsDebuggerPresent )
			{
				/*nlohmann::json root;
				root[ _r( "id" ) ] = _r( "DebuggerFound" );
				root[ _r( "extra" ) ] = _r( "CheckRemoteDebuggerPresent" );*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCD000010 );

				util::crash();

				break;
			}

			//
			// PEB->BeingDebugged
			//
			const auto PEB = reinterpret_cast< _PEB32* >( __readfsdword( 0x30 ) );
			if ( PEB->BeingDebugged )
			{
				/*nlohmann::json root;
				root[ _r( "id" ) ] = _r( "DebuggerFound" );
				root[ _r( "extra" ) ] = _r( "PEB->BeingDebugged" );*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCD000011 );

				util::crash();

				break;
			}

			//
			// Check NtGlobalFlag
			//
			if ( PEB->NtGlobalFlag & NT_GLOBAL_FLAG_DEBUGGED )
			{
				/*nlohmann::json root;
				root[ _r( "id" ) ] = _r( "DebuggerFound" );
				root[ _r( "extra" ) ] = _r( "NtGlobalFlag" );*/

				util::send_report( networking::msg::report_type::TYPE1, 0xCD000012 );

				util::crash();

				break;
			}

			util::delay_execution( 550 );

			//
			// Detect Hardware breakpoints
			//
			ULONG buflen = 0x10000;
			std::unique_ptr< uint8_t[] > buffer;
			status = 0xC000009AL;

			do
			{
				buffer.reset( new uint8_t[ buflen ] );

				status = sys( NtQuerySystemInformation )( SystemProcessInformation, buffer.get(), buflen, &buflen );
				sysunlock();

				if ( !NT_SUCCESS( status ) && status != 0xC0000004 )
				{
					/*nlohmann::json root;
					root[ _r( "id" ) ] = _r( "NtQuerySystemInformation2" );
					root[ _r( "extra" ) ] = std::to_string( status );*/

					util::send_report( networking::msg::report_type::TYPE1, 0xCD000013 );

					util::crash();
					continue;
				}
			} while ( !NT_SUCCESS( status ) );

			util::delay_execution( 550 );

			_SYSTEM_PROCESS_INFORMATION_T< DWORD_PTR >* info = nullptr;

			unsigned i = 0;
			do
			{
				info = reinterpret_cast< _SYSTEM_PROCESS_INFORMATION_T< DWORD_PTR >* >( &buffer[ i ] );

				if ( info->UniqueProcessId == ULONG_PTR( reinterpret_cast< _TEB32* >( NtCurrentTeb() )->ClientId.UniqueProcess ) )
				{
					for ( unsigned int j = 0; j < info->NumberOfThreads; j++ )
					{
						auto thread = &info->Threads[ j ];

						OBJECT_ATTRIBUTES oa{};
						InitializeObjectAttributes( &oa, NULL, 0, NULL, NULL );

						HANDLE hThread{};

						status = syscall( NtOpenThread )( &hThread, THREAD_GET_CONTEXT, &oa, reinterpret_cast< CLIENT_ID* >( &thread->ThreadInfo.ClientId ) );
						sysunlock();
						if ( NT_SUCCESS( status ) )
						{
							CONTEXT ctx{};
							ctx.ContextFlags = CONTEXT_FULL;

							status = syscall( NtGetContextThread )( hThread, &ctx );
							sysunlock();
							imp( "kernel32.dll", CloseHandle )( hThread );

							if ( NT_SUCCESS( status ) )
							{
								if ( ctx.Dr0 || ctx.Dr1 || ctx.Dr2 || ctx.Dr3 || ctx.Dr6 || ctx.Dr7 )
								{

									/*nlohmann::json root;
									root[ _r( "id" ) ] = _r( "DebuggerFound" );
									root[ _r( "extra" ) ] = _r( "DebugDrFlagsSet" );*/

									util::send_report( networking::msg::report_type::TYPE1, 0xCD000014 );

									util::crash();
									break;
								}
							}
						}
					}
				}

				i += info->NextEntryOffset;
			} while ( info->NextEntryOffset != 0 );
#endif

			if ( !( globals::secure_info.values.threads_ran & static_cast< int >( id_CheckDebugger ) ) )
				log( "0.39\n" );
			globals::secure_info.values.threads_ran |= static_cast< unsigned char >( id_CheckDebugger );
		}

		VIRTUALIZER_DOLPHIN_WHITE_END;
	}

	static __declspec( noinline ) void DummyFunction()
	{
		util::crash();
	}

	static __declspec( noinline ) void HasTitanHide()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		const auto dummy_breakpoint = reinterpret_cast< DWORD >( &DummyFunction );

		while ( true )
		{
			util::delay_execution( 20000 );

			CONTEXT ctx{};
			ctx.Dr0 = dummy_breakpoint;
			ctx.Dr7 = 1;
			ctx.ContextFlags = 0x10;

			NTSTATUS Status = syscall( NtSetDebugFilterState )( 0ul, 0ul, TRUE );
			sysunlock();
			if ( !NT_SUCCESS( Status ) )
			{
				globals::secure_info.values.threads_ran |= static_cast< unsigned char >( id_HasTitanHide );
				continue;
			}

			Status = syscall( NtSetContextThread )( NtCurrentThread(), &ctx );
			sysunlock();
			if ( !NT_SUCCESS( Status ) )
			{
				globals::secure_info.values.threads_ran |= static_cast< unsigned char >( id_HasTitanHide );
				continue;
			}

			Status = syscall( NtGetContextThread )( NtCurrentThread(), &ctx );
			sysunlock();
			if ( !NT_SUCCESS( Status ) )
			{
				globals::secure_info.values.threads_ran |= static_cast< unsigned char >( id_HasTitanHide );
				continue;
			}

			if ( ctx.Dr0 == dummy_breakpoint )
			{
				globals::secure_info.values.threads_ran |= static_cast< unsigned char >( id_HasTitanHide );
				continue;
			}

			/*nlohmann::json root;
			root[ _r( "id" ) ] = _r( "AntiDebug" );
			root[ _r( "extra" ) ] = _r( "TitanHideFound" );*/


			util::send_report( networking::msg::report_type::TYPE1, 0xCE00000 );

			util::crash();

			break;
		}

		VIRTUALIZER_DOLPHIN_WHITE_END;
	}

	static __declspec( noinline ) void SuspendedThread()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		util::delay_execution( 2000 );

		util::send_report( networking::msg::report_type::TYPE1, 0xCE0000B3 );

		util::crash();

		VIRTUALIZER_DOLPHIN_WHITE_END;
	}

	__forceinline HANDLE suspended_thread()
	{
		HANDLE hThread{};
		SIZE_T nullData = 0;
		const auto res = syscall( NtCreateThreadEx )(
				&hThread,
				MAXIMUM_ALLOWED,
				nullptr,
				current_process,
				&SuspendedThread,
				nullptr,
				THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER,
				nullData, nullData, nullData,
				nullptr );
		sysunlock();

		if ( !NT_SUCCESS( res ) )
		{
			util::error_close( _r( "Unknown error NT0x%X" ), res );
			return nullptr;
		}

		sys( NtSetInformationThread )(
			hThread,
			static_cast< THREADINFOCLASS >( 0x11 ),
			nullptr,
			0ul );
		sysunlock();

		for ( size_t i = 0; i < 128; ++i )
		{
			syscall( NtSuspendThread )( hThread, nullptr );
			sysunlock();
		}

		protected_threads.insert( { hThread, 127 } );

		return hThread;
	}

	__forceinline HANDLE unstoppable_thread( void* Routine )
	{
		HANDLE hThread{};
		SIZE_T nullData = 0;

		const auto res = syscall( NtCreateThreadEx )(
			&hThread,
			MAXIMUM_ALLOWED,
			nullptr,
			current_process,
			Routine,
			nullptr,
			0x40 | THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER,
			nullData, nullData, nullData,
			nullptr );
		sysunlock();

		if ( !NT_SUCCESS( res ) )
		{
			util::error_close( _r( "Unknown error NT0x%X" ), res );
			return nullptr;
		}

		sys( NtSetInformationThread )(
			NtCurrentThread(),
			static_cast< THREADINFOCLASS >( 0x11 ),
			nullptr,
			0ul );
		sysunlock();

#ifndef _DEBUG
		sys( NtSetInformationThread )(
			hThread,
			static_cast< THREADINFOCLASS >( 0x11 ),
			nullptr,
			0ul );
		sysunlock();
#endif

		ULONG SuspendCount = 0;
		syscall( NtResumeThread )( hThread, &SuspendCount );
		sysunlock();

		imp( "kernel32.dll", SetThreadPriority )( hThread, THREAD_MODE_BACKGROUND_BEGIN );

		protected_threads.insert( { hThread, SuspendCount } );

		return hThread;
	}

	static __declspec( noinline ) void AntiVirtualMachine()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		//
		// Check for vm driver presence
		//
		std::vector<std::wstring> vm_devices;
		vm_devices.push_back( _wc( "\\Global??\\VBoxMiniRdrDN" ) );
		vm_devices.push_back( _wc( "\\Global??\\VBoxMiniRdDN" ) );
		vm_devices.push_back( _wc( "\\Global??\\VBoxTrayIPC" ) );
		vm_devices.push_back( _wc( "\\Global??\\HGFS" ) );
		vm_devices.push_back( _wc( "\\Global??\\vmci" ) );

		for ( int i = 0; i < 5; ++i )
		{
			auto device_str = vm_devices[ i ];

			UNICODE_STRING us{};
			us.Buffer = PWSTR( device_str.c_str() );
			us.Length = USHORT( wcslen( device_str.c_str() ) );
			us.MaximumLength = us.Length + sizeof( wchar_t );

			OBJECT_ATTRIBUTES oa{};
			InitializeObjectAttributes( &oa, &us, OBJ_CASE_INSENSITIVE, NULL, NULL );

			IO_STATUS_BLOCK io{};

			HANDLE hFile{};
			const auto res = sys( NtCreateFile )( &hFile, GENERIC_READ | GENERIC_WRITE, &oa, &io, nullptr, 0ul, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, 0ul, nullptr, 0ul );
			sysunlock();

			if ( NT_SUCCESS( res ) && hFile )
			{
				CloseHandle( hFile );

				util::send_report( networking::msg::report_type::TYPE1, 0xCE000001 );

				util::crash();
			}
		}

		//
		// Try to load VirtualBox .dll
		//
		std::vector<std::wstring> vm_modules;
		vm_devices.push_back( _wc( "vboxhook.dll" ) );
		vm_devices.push_back( _wc( "vmclientcore.dll" ) );
		vm_devices.push_back( _wc( "vmwarewui.dll" ) );
		vm_devices.push_back( _wc( "virtualbox.dll" ) );
		vm_devices.push_back( _wc( "vboxvmm.dll" ) );

		for ( int i = 0; i < 5; ++i )
		{
			const auto hModule = imp( "kernel32.dll", LoadLibraryExW )( vm_modules[ i ].c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_SEARCH_SYSTEM32 );
			if ( hModule )
			{
				util::send_report( networking::msg::report_type::TYPE1, 0xCE000002 );

				util::crash();
			}
		}

		globals::secure_info.values.threads_ran |= static_cast< int >( id_AntiVirtualMachine );

		VIRTUALIZER_DOLPHIN_WHITE_END;
	}

	static __declspec( noinline ) void CheckHosts()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		char szSystem[ MAX_PATH ]{};
		imp( "kernel32.dll", GetSystemDirectoryA )( szSystem, sizeof( szSystem ) );

		while ( true )
		{
			std::ifstream ifs( std::string( szSystem ) + xorstr_( "\\drivers\\etc\\hosts" ), std::ios::in );
			if ( ifs.good() )
			{
				for ( std::string hosts; std::getline( ifs, hosts ); )
				{
					std::ranges::transform( hosts, hosts.begin(), ::tolower );
					if ( hosts.find( xorstr_( "34.226.138.50" ) ) != hosts.npos || hosts.find( xorstr_( "fatality.win" ) ) != hosts.npos )
					{
						util::send_report( networking::msg::report_type::TYPE1, 0xCE0000A0 );

						util::crash();
						goto end;
					}

					util::delay_execution( 100 );
				}
			}

			if ( !( globals::secure_info.values.threads_ran & static_cast< int >( id_CheckHosts ) ) )
				log( "0.392\n" );
			globals::secure_info.values.threads_ran |= static_cast< int >( id_CheckHosts );

			util::delay_execution( 10000 );
		}

end:
		VIRTUALIZER_DOLPHIN_WHITE_END;
	}

	static __declspec( noinline ) void CheckThreads()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		NTSTATUS status;

		while ( true )
		{
			for ( auto& threads : protected_threads )
			{
				if ( threads.second == 127 )
				{
					status = syscall( NtSuspendThread )( threads.first, nullptr );
					sysunlock();

					if ( NT_SUCCESS( status ) )
					{
						util::send_report( networking::msg::report_type::TYPE1, 0xCE0000B4 );
						break;
					}

					continue;
				}

				ULONG SuspendCount = 0;
				status = syscall( NtResumeThread )( threads.first, &SuspendCount );
				sysunlock();

				if ( threads.second != SuspendCount )
				{
					util::send_report( networking::msg::report_type::TYPE1, 0xCE0000B2 );

					util::crash();
					break;
				}
			}

			sys( NtSetInformationThread )(
				NtCurrentThread(),
				static_cast< THREADINFOCLASS >( 0x11 ),
				nullptr,
				0ul );
			sysunlock();

			if ( protected_threads.empty() )
				break;

			if ( !( globals::secure_info.values.threads_ran & static_cast< int >( id_CheckThreads ) ) )
				log( "0.41\n" );
			globals::secure_info.values.threads_ran |= static_cast< int >( id_CheckThreads );

			util::delay_execution( 5000 );
		}

		VIRTUALIZER_DOLPHIN_WHITE_END;
	}

	static __declspec( noinline ) void CheckInternetConnection()
	{
		VIRTUALIZER_DOLPHIN_WHITE_START;

		while ( true )
		{
			if ( !util::is_connected() )
			{
				util::error_close( xorstr_( "An active internet connection is reuqired." ) );
				break;
			}

			globals::secure_info.values.threads_ran |= static_cast< int >( id_CheckInternetConnection );

			util::delay_execution( 15 * 1000 );
		}

		VIRTUALIZER_DOLPHIN_WHITE_END;
	}

}
