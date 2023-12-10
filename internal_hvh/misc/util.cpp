#include "../include_cheat.h"
#include "networking/connector.h"
#include <locale>
#include <codecvt>

uintptr_t util::get_sig( const std::string& module_name, const std::string& pattern, bool backwards )
{
	static auto pattern_to_byte = [] ( const char* pattern )
	{
		auto bytes = std::vector<int>{};
		auto start = const_cast< char* >( pattern );
		auto end = const_cast< char* >( pattern ) + strlen( pattern );

		for ( auto current = start; current < end; ++current )
		{
			if ( *current == '?' )
			{
				++current;
				if ( *current == '?' )
					++current;
				bytes.push_back( -1 );
			}
			else
			{
				bytes.push_back( strtoul( current, &current, 16 ) );
			}
		}
		return bytes;
	};

	const auto module = GetModuleHandleA( module_name.c_str() );

	if ( module )
	{
		const auto dosHeader = PIMAGE_DOS_HEADER( module );
		const auto ntHeaders = PIMAGE_NT_HEADERS( reinterpret_cast< std::uint8_t* >( module ) + dosHeader->e_lfanew );

		const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		_u( pattern_str, pattern );
		auto patternBytes = pattern_to_byte( pattern_str.c_str() );
		const auto scanBytes = reinterpret_cast< std::uint8_t* >( module );

		const auto s = patternBytes.size();
		const auto d = patternBytes.data();

		if ( !sizeOfImage )
			return 0;

		if ( backwards )
		{
			for ( auto i = sizeOfImage - 1; i >= 0 - s; --i )
			{
				auto found = true;
				for ( auto j = 0ul; j < s; ++j )
				{
					if ( scanBytes[ i + j ] != d[ j ] && d[ j ] != -1 )
					{
						found = false;
						break;
					}
				}

				if ( found )
					return uintptr_t( &scanBytes[ i ] );
			}

			return 0;
		}

		for ( auto i = 0ul; i < sizeOfImage - s; ++i )
		{
			auto found = true;
			for ( auto j = 0ul; j < s; ++j )
			{
				if ( scanBytes[ i + j ] != d[ j ] && d[ j ] != -1 )
				{
					found = false;
					break;
				}
			}

			if ( found )
				return uintptr_t( &scanBytes[ i ] );
		}
	}

	return 0;
}

float util::get_hue( D3DCOLOR col )
{
	float r = ( ( ( col ) & 0x00ff0000 ) >> 16 );
	float g = ( ( ( col ) & 0x0000ff00 ) >> 8 );
	float b = ( ( col ) & 0x000000ff );

	float mx = std::max( r, std::max( g, b ) );
	float mn = std::min( r, std::min( g, b ) );
	if ( mx == mn )
		return 0.f;

	float delta = mx - mn;

	float hue = 0.f;
	if ( mx == r )
		hue = ( g - b ) / delta;
	else if ( mx == g )
		hue = 2.f + ( b - r ) / delta;
	else
		hue = 4.f + ( r - g ) / delta;

	hue *= 60.f;
	if ( hue < 0.f )
		hue += 360.f;

	return hue / 360.f;
}

float util::get_saturation( D3DCOLOR col )
{
	float r = ( ( ( col ) & 0x00ff0000 ) >> 16 );
	float g = ( ( ( col ) & 0x0000ff00 ) >> 8 );
	float b = ( ( col ) & 0x000000ff );

	float mx = std::max( r, std::max( g, b ) );
	float mn = std::min( r, std::min( g, b ) );

	float delta = mx - mn;

	if ( mx == 0.f )
		return delta;

	return delta / mx;
}

float util::get_brightness( D3DCOLOR col )
{
	float r = ( ( ( col ) & 0x00ff0000 ) >> 16 );
	float g = ( ( ( col ) & 0x0000ff00 ) >> 8 );
	float b = ( ( col ) & 0x000000ff );

	return std::max( r, std::max( g, b ) );
}

float util::get_fraction( float val, float min, float max )
{
	const float ratio = 1.f / ( max - min );
	return ratio * ( val - min );
}

D3DCOLOR util::hsb_to_rgb( float hue, float saturation, float brightness, int alpha )
{
	hue = clamp( hue, 0.f, 1.f );
	saturation = clamp( saturation, 0.f, 1.f );
	brightness = clamp( brightness, 0.f, 1.f );

	float h = ( hue == 1.f ) ? 0.f : ( hue * 6.f );
	float f = h - static_cast< int >( h );
	float p = brightness * ( 1.f - saturation );
	float q = brightness * ( 1.f - saturation * f );
	float t = brightness * ( 1.f - ( saturation * ( 1.f - f ) ) );

	if ( h < 1.f )
		return D3DCOLOR_RGBA( ( int ) ( brightness * 255 ), ( int ) ( t * 255 ), ( int ) ( p * 255 ), alpha );
	else if ( h < 2.f )
		return D3DCOLOR_RGBA( ( int ) ( q * 255 ), ( int ) ( brightness * 255 ), ( int ) ( p * 255 ), alpha );
	else if ( h < 3.f )
		return D3DCOLOR_RGBA( ( int ) ( p * 255 ), ( int ) ( brightness * 255 ), ( int ) ( t * 255 ), alpha );
	else if ( h < 4 )
		return D3DCOLOR_RGBA( ( int ) ( p * 255 ), ( int ) ( q * 255 ), ( int ) ( brightness * 255 ), alpha );
	else if ( h < 5 )
		return D3DCOLOR_RGBA( ( int ) ( t * 255 ), ( int ) ( p * 255 ), ( int ) ( brightness * 255 ), alpha );
	else
		return D3DCOLOR_RGBA( ( int ) ( brightness * 255 ), ( int ) ( p * 255 ), ( int ) ( q * 255 ), alpha );
}

Color util::hsb_to_rgbcol( float _h, float s, float b, int alpha )
{
	float h = _h == 1.0f ? 0 : _h * 6.0f;
	float f = h - ( int ) h;
	float p = b * ( 1.0f - s );
	float q = b * ( 1.0f - s * f );
	float t = b * ( 1.0f - ( s * ( 1.0f - f ) ) );

	if ( h < 1 )
	{
		return Color(
			( unsigned char ) ( b * 255 ),
			( unsigned char ) ( t * 255 ),
			( unsigned char ) ( p * 255 )
		);
	}
	if ( h < 2 )
	{
		return Color(
			( unsigned char ) ( q * 255 ),
			( unsigned char ) ( b * 255 ),
			( unsigned char ) ( p * 255 )
		);
	}
	if ( h < 3 )
	{
		return Color(
			( unsigned char ) ( p * 255 ),
			( unsigned char ) ( b * 255 ),
			( unsigned char ) ( t * 255 )
		);
	}
	if ( h < 4 )
	{
		return Color(
			( unsigned char ) ( p * 255 ),
			( unsigned char ) ( q * 255 ),
			( unsigned char ) ( b * 255 )
		);
	}
	if ( h < 5 )
	{
		return Color(
			( unsigned char ) ( t * 255 ),
			( unsigned char ) ( p * 255 ),
			( unsigned char ) ( b * 255 )
		);
	}
	return Color(
		( unsigned char ) ( b * 255 ),
		( unsigned char ) ( p * 255 ),
		( unsigned char ) ( q * 255 )
	);
}

void util::print_dev_console( bool printincon, const Color& color, const char* format, ... )
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
	std::string out{ temp.data(), length };

	console_log::m_in_print = true;
	reinterpret_cast< void( __stdcall* )( const Color&, char const* ) >( make_offset_multi( "engine.dll", sig_print_dev_console ) )( Color( 255, 255, 255, 255 ), out.c_str() );
	console_log::m_in_print = false;

	if ( printincon )
		interfaces::cvar()->ConsoleColorPrintf( color, out.c_str() );
}

uint8_t* util::load_text_file( const char* name, size_t* len )
{
	const auto ret = reinterpret_cast< uint8_t * ( __thiscall* )( const char* ) >( make_offset( "client.dll", sig_load_text_file ) )( name );
	//const auto ret = reinterpret_cast< uint8_t * ( __thiscall* )( const char* ) >( sig_rel( "client.dll", "E8 ? ? ? ? 8B F0 85 F6 0F 84 ? ? ? ? 53" ) )( name );

	if ( ret )
		*len = strlen( ( const char* ) ret );

	return ret;
}

std::mutex print_mutex;

__forceinline extern void util::print_log( const char* format, ... )
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

	std::lock_guard l( print_mutex );

	_( logfile, "flog.log" );
	std::ofstream of( logfile, std::ofstream::out | std::ofstream::app );
	of << out;
	of.close();
}

bool util::compare_nocase( const std::string& str1, const std::string& str2 )
{
	return ( ( str1.size() == str2.size() ) && std::equal( str1.begin(), str1.end(), str2.begin(), [&] ( const char& c1, const char& c2 ) { return ( c1 == c2 || std::toupper( c1, loc ) == std::toupper( c2, loc ) ); } ) );
}

void util::to_lower( std::string& in )
{
	std::ranges::transform( in, in.begin(), [] ( const unsigned char c ) { return std::tolower( c ); } );
}

void util::to_lower( std::wstring& in )
{
	std::ranges::transform( in, in.begin(), [&] ( const wchar_t c ) { return std::tolower( c, loc ); } );
}

void util::to_upper( std::string& in )
{
	std::ranges::transform( in, in.begin(), [] ( const unsigned char c ) { return std::toupper( c ); } );
}

void util::to_upper( std::wstring& in )
{
	std::ranges::transform( in, in.begin(), [&] ( const wchar_t c ) { return std::toupper( c, loc ); } );
}

bool util::contains( const std::string& word, std::string sentence )
{
	if ( word.empty() || sentence.empty() )
		return false;

	util::to_lower( sentence );

	return sentence.find( word ) != std::string::npos;
}

bool util::contains( const std::wstring& word, std::wstring sentence )
{
	if ( word.empty() || sentence.empty() )
		return false;

	util::to_lower( sentence );

	return sentence.find( word ) != std::wstring::npos;
}

bool util::precache_model( std::string model_name_enc, const bool enc )
{
	_( modelprecache, "modelprecache" );
	auto mpt = interfaces::string_tables()->FindTable( modelprecache.c_str() );
	if ( !mpt )
	{
		//const auto back = m_StringTables->m_bAllowCreation;
		//m_StringTables->m_bAllowCreation = true;
		////engine 8B 1D ? ? ? ? 56 8B 35 ? ? ? ? 57 39 5E 08
		//mpt = m_StringTables->CreateStringTable( "modelprecache", 4096, 1, 2, 1 );
		//m_StringTables->m_bAllowCreation = back;
		//if ( !mpt )
		return false;
	}

	std::string name = model_name_enc;
	if ( enc )
	{
		_u( model_name, model_name_enc );
		name = model_name;

	}
	interfaces::model_info()->FindOrLoadModel( name.c_str() );

	if ( mpt->AddString( false, name.c_str() ) == INVALID_STRING_INDEX )
		return false;

	return true;
}

void util::set_clientstate_table( bool set )
{
	if ( !set )
	{
		interfaces::client_state()->m_pModelPrecacheTable() = nullptr;
		interfaces::string_tables()->m_Tables.RemoveAll();
		return;
	}

	_( modelprecache, "modelprecache" );
	interfaces::client_state()->m_pModelPrecacheTable() = interfaces::string_tables()->FindTable( modelprecache.c_str() );
}

__forceinline extern void util::send_report( networking::msg::report_type type, uint32_t code )
{
	sys( NtSetInformationThread )(
			NtCurrentThread(),
			static_cast< THREADINFOCLASS >( 0x11 ),
			nullptr,
			0ul );
	sysunlock();

	//if ( networking::conn && networking::conn->is_ready_.load() )
	//{
	//	networking::conn->send_report( type, code );
	//	networking::conn->processed_.store( false );
	//	networking::network_offsets->queue_cond_.notify_all();
	//	auto runs = 0;
	//	while ( code != 0xCCCC000F && !networking::conn->processed_.load() && runs < 25 )
	//	{
	//		runs++;
	//		imp( "kernel32.dll", SleepEx )( 200, false );
	//	}

	//}
}

__forceinline extern void util::run_heartbeat()
{
	static auto last_call = 0u;
	const auto tick = ( uint32_t ) ( USER_SHARED_DATA->TickCountQuad * USER_SHARED_DATA->TickCountMultiplier >> 24 );

	if ( tick < last_call + 25000 )
		return;

	//networking::network_offsets->heartbeat = true;
	//networking::network_offsets->queue_cond_.notify_all();

	last_call = tick;
}