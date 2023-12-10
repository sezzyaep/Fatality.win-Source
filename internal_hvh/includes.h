#pragma once

#ifdef RELEASE
namespace std
{
	inline const char* _Syserror_map( int ) { return nullptr; }
	[[noreturn]] inline void _Throw_C_error( int ) { while ( true ); }
	[[noreturn]] inline void _Throw_Cpp_error( int ) { while ( true ); }
	inline int _Winerror_map( int ) { return 0; }
	[[noreturn]] inline void _Xbad_function_call() { while ( true ); }
	[[noreturn]] inline void _Xinvalid_argument( char const* ) { while ( true ); }
	[[noreturn]] inline void _Xlength_error( char const* ) { while ( true ); }
	[[noreturn]] inline void _Xout_of_range( char const* ) { while ( true ); }
}
#endif

#pragma warning( disable : 4530 )

#define WIN32_NO_STATUS
#define _WINSOCKAPI_
#define NOCRYPT
#include <Windows.h>
#undef WIN32_NO_STATUS
#include <minwindef.h>
#include <memory>
#include <chrono>
#include <mutex>
#include <string>
#include <vector>
#include <array>
#include <deque>
#include <cmath>
#include <ranges>
#include <unordered_map>
#include <d3d9.h>
#include <d3dx9.h>
#include "threadsafe_deque.h"
#include <shared_mutex>

#ifdef NO_VIRT
#define VIRTUALIZER_TIGER_WHITE_START {}
#define VIRTUALIZER_TIGER_WHITE_END {}
#define VIRTUALIZER_SHARK_RED_START {}
#define VIRTUALIZER_SHARK_RED_END {}
#define VIRTUALIZER_LION_WHITE_START {}
#define VIRTUALIZER_LION_WHITE_END {}
#define VIRTUALIZER_LION_RED_START {}
#define VIRTUALIZER_LION_RED_END {}
#define VIRTUALIZER_LION_BLACK_START {}
#define VIRTUALIZER_LION_BLACK_END {}
#define VIRTUALIZER_DOLPHIN_WHITE_START {}
#define VIRTUALIZER_DOLPHIN_WHITE_END {}
#define VIRTUALIZER_DOLPHIN_WHITE_START {}
#define VIRTUALIZER_DOLPHIN_WHITE_END {}
#define VIRTUALIZER_DOLPHIN_BLACK_START  {}
#define VIRTUALIZER_DOLPHIN_BLACK_END  {}
#else
#include <VirtualizerSDK.h>
#endif

template <typename T> __forceinline int signum( T val )
{
	return ( T( 0 ) < val ) - ( val < T( 0 ) );
}

template <typename T> __forceinline T clamp( const T& n, const T& lower, const T& upper )
{
	return max( lower, min( n, upper ) );
};

#ifndef __clang__
#define __attribute__(x)
#endif

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma warning( disable : 4616 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4838 )
#pragma warning( disable : 4018 )
#pragma warning( disable : 1251 )
#pragma warning( disable : 3291 )
#pragma warning( disable : 1491 )
#pragma warning( disable : 1563 )
#pragma warning( disable : 4750 )