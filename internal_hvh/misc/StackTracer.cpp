/*
//
// Description: This class is used to get the call stack when there is an exception being caught use SEH
//
// Author: Baiyan Huang
// Date: 8/30/2009
//
*/

#include <vector>
#include <map>
#include <Windows.h>
#include "StackTracer.h"
#include <DbgHelp.h>
#include <sstream>
#include <psapi.h>
#include "../cthash.h"
#include "../include_cheat.h"

#pragma comment(lib, "dbghelp.lib")

StackTracer StackTracer::s_StackTracer;

// Generally, call stack with depth of 6 should be enough for our diagnosing.
const int CALLSTACK_DEPTH = 20;

// Translate exception code to description
#define CODE_DESCR(code) CodeDescMap::value_type(code, #code)

StackTracer::StackTracer( void )
	:m_dwExceptionCode( 0 )
{
	// Get machine type
	m_dwMachineType = 0;
	m_dwMachineType = IMAGE_FILE_MACHINE_I386;

	// Exception code description
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_ACCESS_VIOLATION ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_DATATYPE_MISALIGNMENT ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_BREAKPOINT ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_SINGLE_STEP ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_ARRAY_BOUNDS_EXCEEDED ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_FLT_DENORMAL_OPERAND ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_FLT_DIVIDE_BY_ZERO ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_FLT_INEXACT_RESULT ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_FLT_INVALID_OPERATION ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_FLT_OVERFLOW ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_FLT_STACK_CHECK ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_FLT_UNDERFLOW ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_INT_DIVIDE_BY_ZERO ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_INT_OVERFLOW ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_PRIV_INSTRUCTION ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_IN_PAGE_ERROR ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_ILLEGAL_INSTRUCTION ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_NONCONTINUABLE_EXCEPTION ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_STACK_OVERFLOW ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_INVALID_DISPOSITION ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_GUARD_PAGE ) );
	m_mapCodeDesc.insert( CODE_DESCR( EXCEPTION_INVALID_HANDLE ) );
	//m_mapCodeDesc.insert(CODE_DESCR(EXCEPTION_POSSIBLE_DEADLOCK));
	// Any other exception code???
}

StackTracer::~StackTracer( void )
{}


LONG StackTracer::ExceptionFilter( LPEXCEPTION_POINTERS e )
{
	return s_StackTracer.HandleException( e );
}


std::string StackTracer::GetExceptionMsg()
{
	std::ostringstream  m_ostringstream;

	// Exception Code
	auto itc = s_StackTracer.m_mapCodeDesc.find( s_StackTracer.m_dwExceptionCode );

	_( newl, "\n" );

	if ( itc != s_StackTracer.m_mapCodeDesc.end() )
		m_ostringstream << itc->second << newl;
	else
		m_ostringstream << std::to_string( s_StackTracer.m_dwExceptionCode ) << newl;

	// Call Stack
	auto itbegin = s_StackTracer.m_vecCallStack.begin();
	auto itend = s_StackTracer.m_vecCallStack.end();
	std::vector<FunctionCall>::iterator it;
	for ( it = itbegin; it < itend; it++ )
	{
		std::string strFunctionName = it->FunctionName.empty() ? "-" : it->FunctionName;

		_( minus, "------------------" );

		m_ostringstream << minus << newl;
		m_ostringstream << strFunctionName << newl;
		m_ostringstream << it->Address << newl;
		m_ostringstream << it->Module << newl;
	}

	return m_ostringstream.str();
}

DWORD StackTracer::GetExceptionCode()
{
	return s_StackTracer.m_dwExceptionCode;
}

std::vector<FunctionCall> StackTracer::GetExceptionCallStack()
{
	return s_StackTracer.m_vecCallStack;
}

LONG __stdcall StackTracer::HandleException( LPEXCEPTION_POINTERS e )
{
	m_dwExceptionCode = e->ExceptionRecord->ExceptionCode;
	m_vecCallStack.clear();

	HANDLE hProcess = INVALID_HANDLE_VALUE;

	// Initializes the symbol handler
	if ( !SymInitialize( GetCurrentProcess(), NULL, TRUE ) )
	{
		SymCleanup( hProcess );
		return EXCEPTION_EXECUTE_HANDLER;
	}

	// Work through the call stack upwards.
	TraceCallStack( e->ContextRecord );

	SymCleanup( hProcess );

	return( EXCEPTION_EXECUTE_HANDLER );
}

// Work through the stack to get the entire call stack
void StackTracer::TraceCallStack( CONTEXT* pContext )
{
	// Initialize stack frame
	STACKFRAME sf;
	memset( &sf, 0, sizeof( STACKFRAME ) );
	sf.AddrPC.Offset = pContext->Eip;
	sf.AddrStack.Offset = pContext->Esp;
	sf.AddrFrame.Offset = pContext->Ebp;
	sf.AddrPC.Mode = AddrModeFlat;
	sf.AddrStack.Mode = AddrModeFlat;
	sf.AddrFrame.Mode = AddrModeFlat;

	if ( 0 == m_dwMachineType )
		return;

	// Walk through the stack frames.
	HANDLE hProcess = GetCurrentProcess();
	HANDLE hThread = GetCurrentThread();
	while ( StackWalk( m_dwMachineType, hProcess, hThread, &sf, pContext, 0, SymFunctionTableAccess, SymGetModuleBase, 0 ) )
	{
		if ( sf.AddrFrame.Offset == 0 || m_vecCallStack.size() >= CALLSTACK_DEPTH )
			break;

		DWORD64 functionAddress;
		std::string moduleName;
		std::string functioName;
		std::string file;
		unsigned int line = -1;

		functionAddress = sf.AddrPC.Offset;

		DWORD moduleBase = SymGetModuleBase( hProcess, sf.AddrPC.Offset );
		char moduleBuff[ MAX_PATH ];
		if ( moduleBase && GetModuleFileNameA( ( HINSTANCE ) moduleBase, moduleBuff, MAX_PATH ) )
		{
			moduleName = moduleBuff;
		}
		else
			moduleBase = ( DWORD )globals::module_base;

		char symbolBuffer[ sizeof( IMAGEHLP_SYMBOL ) + 255 ];
		PIMAGEHLP_SYMBOL symbol = ( PIMAGEHLP_SYMBOL ) symbolBuffer;
		symbol->SizeOfStruct = ( sizeof( IMAGEHLP_SYMBOL ) ) + 255;
		symbol->MaxNameLength = 254;

		if ( SymGetSymFromAddr( hProcess, sf.AddrPC.Offset, NULL, symbol ) )
		{
			functioName = symbol->Name;
		}

		DWORD  offset = 0;
		IMAGEHLP_LINE line_help;
		line_help.SizeOfStruct = sizeof( IMAGEHLP_LINE );

		if ( SymGetLineFromAddr( hProcess, sf.AddrPC.Offset, &offset, &line_help ) )
		{
			file = line_help.FileName;
			line = line_help.LineNumber;
		}
		FunctionCall curCall = { functioName, file, ( int ) line, ( uint32_t ) functionAddress - moduleBase, moduleName };
		// Call stack stored
		m_vecCallStack.push_back( curCall );

	}
}
