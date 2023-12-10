#include "include_cheat.h"

#include "misc/MinHook.h"
#include "misc/protection.h"

extern "C" _PIFV * __xi_a;
extern "C" _PIFV * __xi_z;
extern "C" _PVFV * __xc_a;
extern "C" _PVFV * __xc_z;

void __stdcall init_main()
{
	init::on_startup();
}

thread_local static std::string test = "---------- ";

BOOL APIENTRY DllMain( HMODULE h_module, uintptr_t  dw_reason_for_call, LPVOID lp_reserved )
{
	switch ( dw_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:
		{
			_( logfile, "flog.log" );
			std::ofstream of( logfile, std::ofstream::out | std::ofstream::trunc );
			of.close();
			log( "start --------" );

			util::print_log( test.c_str() );

			log( "\n" );

			textstart = reinterpret_cast< uint32_t >( globals::module_base = h_module );
			imp( "kernel32.dll", DisableThreadLibraryCalls )( h_module );

#ifdef RELEASE
			HANDLE thread;
			syscall( NtCreateThreadEx )( &thread, MAXIMUM_ALLOWED, nullptr, current_process,
				&init_main, nullptr, 0x40 | THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER, NULL, NULL, NULL, nullptr );
			sysunlock();
#else
			DWORD tid;
			CloseHandle( CreateThread( nullptr, 0, reinterpret_cast< LPTHREAD_START_ROUTINE >( init_main ), nullptr, 0, &tid ) );
#endif
			return true;
		}
		default:
			return true;
	}
}