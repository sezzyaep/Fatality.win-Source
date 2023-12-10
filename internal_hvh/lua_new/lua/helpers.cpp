#include "../include_cheat.h"

namespace lua::helpers
{
	std::unordered_map<uint32_t, var> var_map;
	std::mutex var_mutex;

	var get_netvar_internal( uint32_t hash, RecvTable* table, size_t offset )
	{
		for ( auto i = 0; i < table->m_nProps; ++i )
		{
			const auto current = &table->m_pProps[ i ];
			if ( current->m_RecvType == DPT_DataTable )
			{
				const auto found = get_netvar_internal( hash, current->m_pDataTable, offset + current->m_Offset );
				if ( found.offset != 0 )
					return found;
			}

			if ( isdigit( current->m_pVarName[ 0 ] ) )
				continue;

			if ( hash == fnv1a_rt( current->m_pVarName ) )
			{
				auto type = current->m_RecvType;
				if ( current->m_RecvType == DPT_DataTable && current->m_pDataTable && current->m_pDataTable->m_nProps )
					type = current->m_pDataTable->m_pProps[ 0 ].m_RecvType;

				return { static_cast< size_t >( current->m_Offset ) + offset, type, DPT_Int };
			}
		}

		return { 0, DPT_Int, DPT_Int };
	}

	var get_netvar( const char* name, RecvTable* table, size_t offset )
	{
		const auto hash = fnv1a_rt( std::string( name ) + table->m_pNetTableName );
		{
			std::lock_guard lock_( var_mutex );
			if ( var_map.contains( hash ) )
				return var_map[ hash ];
		}

		const auto netvar = get_netvar_internal( fnv1a_rt( name ), table, offset );
		if ( netvar.offset != 0 )
		{
			std::lock_guard lock_( var_mutex );
			var_map[ hash ] = netvar;
		}

		return netvar;
	}

	uintptr_t get_interface( const uint32_t module, const uint32_t name )
	{
		const auto list = **reinterpret_cast< interface_reg*** >( find_pattern( find_module( module ), XOR( "55 8B EC 56 8B 35 ? ? ? ? 57 85 F6 74 38 8B" ), 6 ).address );

		for ( auto current = list; current; current = current->next )
			if ( fnv1a_rt( current->name ) == name )
				return reinterpret_cast< uintptr_t >( current->create() );

		return -1;
	}

	namespace scanner
	{
		enum element_type
		{
			el_byte,
			el_any,
			el_ref_open,
			el_ref_close,
			el_follow,
			el_offset,
			el_add,
			el_sub,
			el_max
		};

		struct element_t
		{
			size_t pos;
			element_type type;
			uintptr_t value;
		};

		struct deref_t
		{
			uintptr_t pos;
			uintptr_t offset;
			uintptr_t length;
			bool should_follow;
		};
	}

	scan_result find_pattern( const uint8_t* module, const char* pat, const ptrdiff_t off )
	{
		const auto module_size = PIMAGE_NT_HEADERS( module + PIMAGE_DOS_HEADER( module )->e_lfanew )->OptionalHeader.SizeOfImage;
		std::string pattern{ pat };

		// parse pattern.
		std::vector<scanner::element_t> elements; int open_count{}, close_count{}; size_t pos{};
		for ( auto it = pattern.begin(); it != pattern.end(); ++it )
		{
			switch ( *it )
			{
				case ' ':
					continue;
				case '?':
					if ( it + 1 != pattern.end() && *( it + 1 ) == '?' )
						++it;

					elements.push_back( { pos++, scanner::el_any } );
					continue;
				case '[':
					++open_count;
					elements.push_back( { pos++, scanner::el_ref_open } );
					continue;
				case ']':
					++close_count;
					elements.push_back( { pos++, scanner::el_ref_close } );
					continue;
				case '*':
					elements.push_back( { pos++, scanner::el_follow } );
					continue;
				case '+':
					elements.push_back( { pos++, scanner::el_add } );
					continue;
				case '-':
					elements.push_back( { pos++, scanner::el_sub } );
					continue;
			}

			if ( !elements.empty() )
			{
				const auto& last = elements.back();
				if ( isdigit( *it ) && ( last.type == scanner::el_add || last.type == scanner::el_sub ) )
				{
					uintptr_t offset{};
					while ( it != pattern.end() && isdigit( *it ) )
					{
						offset *= 10;
						offset += *it - '0';
						++it;
					}

					if ( last.type == scanner::el_sub )
						offset = -offset;

					--it;
					elements.push_back( { pos++, scanner::el_offset, offset } );
					continue;
				}
			}

			if ( isxdigit( *it ) )
			{
				uintptr_t value{};
				while ( it != pattern.end() && isxdigit( *it ) )
				{
					value *= 16;
					value += isdigit( *it ) ? *it - '0' : tolower( *it ) - 'a' + 10;
					++it;
				}

				--it;
				elements.push_back( { pos++, scanner::el_byte, value } );
				continue;
			}

			return { 0, scan_result::parse_error, pos };
		}

		if ( open_count != close_count )
			return { 0, scan_result::parse_error };

		if ( elements.empty() )
			return { 0, scan_result::not_found };

		// build dereference stack.
		std::vector<scanner::deref_t> deref_stack; bool had_follow{}; int skip_closes, old_open_count{ open_count }, length_delta{ open_count * 2 - 1 }; size_t last_byte{};
		for ( const auto& elf : elements )
		{
			if ( elf.type == scanner::el_byte || elf.type == scanner::el_any )
				last_byte = elf.pos + 1;

			if ( elf.type == scanner::el_follow )
			{
				had_follow = true;
				continue;
			}

			if ( elf.type != scanner::el_ref_open )
			{
				if ( had_follow )
					return { 0, scan_result::parse_error, elf.pos };

				continue;
			}

			scanner::deref_t cur_deref{};
			cur_deref.should_follow = had_follow;
			cur_deref.pos = last_byte;
			last_byte = 0;

			skip_closes = old_open_count - open_count;

			for ( auto it = elements.rbegin(); it != elements.rend(); ++it )
			{
				auto& elb = *it;
				if ( elb.type != scanner::el_ref_close || skip_closes-- )
					continue;

				cur_deref.length = elb.pos - elf.pos - length_delta;

				if ( elb.pos + 2 < elements.size() )
				{
					const auto& elc = elements[ elb.pos + 1 ];
					if ( elc.type == scanner::el_add || elc.type == scanner::el_sub )
						cur_deref.offset = elements[ elb.pos + 2 ].value;
				}

				break;
			}

			if ( !cur_deref.length )
				return { 0, scan_result::too_small_deref, elf.pos };

			had_follow = false;
			deref_stack.push_back( cur_deref );
			--open_count;
			length_delta -= 2;
		}

		// build scan pattern.
		std::vector<scanner::element_t> scan_pattern;
		for ( const auto& el : elements )
		{
			if ( el.type == scanner::el_byte || el.type == scanner::el_any )
				scan_pattern.push_back( el );
		}

		scan_result result{ 0, scan_result::not_found };
		for ( auto i = ( uintptr_t ) module; i < ( uintptr_t ) module + module_size - scan_pattern.size(); ++i )
		{
			bool match{ true }; size_t cur{};
			for ( const auto& el : scan_pattern )
			{
				++cur;
				if ( el.type != scanner::el_any && *( uint8_t* ) ( i + ( cur - 1 ) ) != ( uint8_t ) el.value )
				{
					match = false;
					break;
				}
			}

			if ( match )
			{
				result.address = ( uintptr_t ) i;
				result.error = scan_result::found;
			}
		}

		if ( result.error != scan_result::found )
			return result;

		// apply the stack.
		for ( auto it = deref_stack.rbegin(); it != deref_stack.rend(); ++it )
		{
			auto& deref = *it;
			if ( deref.should_follow )
			{
				if ( deref.length < 4 )
				{
					result.error = scan_result::too_small_deref;
					result.error_pos = deref.pos;
					return result;
				}

				try
				{
					result.address = *reinterpret_cast< uintptr_t* >( result.address + deref.pos ) + result.address + sizeof( uintptr_t ) + 1 + deref.offset;
				}
				catch ( ... )
				{
					result.error = scan_result::exception;
					return result;
				}

				continue;
			}

			try
			{
				switch ( deref.length )
				{
					case 1:
						result.address = *reinterpret_cast< uint8_t* >( result.address + deref.pos );
						break;
					case 2:
						result.address = *reinterpret_cast< uint16_t* >( result.address + deref.pos );
						break;
					case 4:
						result.address = *reinterpret_cast< uint32_t* >( result.address + deref.pos );
						break;
					default:
						result.error = scan_result::not_a_power;
						return result;
				}
			}
			catch ( ... )
			{
				result.error = scan_result::exception;
				return result;
			}
		}

		result.address += off;
		return result;
	}

	const uint8_t* find_module( const uint32_t hash )
	{
		static auto file_name_w = [] ( wchar_t* path )
		{
			wchar_t* slash = path;

			while ( path && *path )
			{
				if ( ( *path == '\\' || *path == '/' || *path == ':' ) &&
					path[ 1 ] && path[ 1 ] != '\\' && path[ 1 ] != '/' )
					slash = path + 1;
				path++;
			}

			return slash;
		};

		const auto peb = NtCurrentTeb()->ProcessEnvironmentBlock;

		if ( !peb )
			return nullptr;

		const auto ldr = reinterpret_cast< PPEB_LOADER_DATA >( peb->Ldr );

		if ( !ldr )
			return nullptr;

		const auto head = &ldr->InLoadOrderModuleList;
		auto current = head->Flink;

		while ( current != head )
		{
			const auto module = CONTAINING_RECORD( current, LDR_MODULE, InLoadOrderModuleList );
			std::wstring wide( file_name_w( module->FullDllName.Buffer ) );
			std::string name( wide.begin(), wide.end() );
			std::transform( name.begin(), name.end(), name.begin(), ::tolower );

			if ( fnv1a_rt( name.c_str() ) == hash )
				return reinterpret_cast< uint8_t* >( module->BaseAddress );

			current = current->Flink;
		}

		return nullptr;
	}

	json parse_table( lua_State* L, int index )
	{
		runtime_state s( L );

		json root;

		// Push another reference to the table on top of the stack (so we know
		// where it is, and this function can work for negative, positive and
		// pseudo indices
		lua_pushvalue( L, index );
		// stack now contains: -1 => table
		lua_pushnil( L );
		// stack now contains: -1 => nil; -2 => table
		while ( lua_next( L, -2 ) )
		{
			// stack now contains: -1 => value; -2 => key; -3 => table
			// copy the key so that lua_tostring does not modify the original
			lua_pushvalue( L, -2 );
			// stack now contains: -1 => key; -2 => value; -3 => key; -4 => table

			json sub;

			// set value by type
			switch ( lua_type( L, -2 ) )
			{
				case LUA_TNIL:
					sub = nullptr;
					break;
				case LUA_TNUMBER:
					sub = lua_tonumber( L, -2 );
					break;
				case LUA_TBOOLEAN:
					sub = lua_toboolean( L, -2 );
					break;
				case LUA_TSTRING:
					sub = lua_tostring( L, -2 );
					break;
				case LUA_TTABLE:
					sub = parse_table( L, -2 );
					break;
				default:
					break;
			}

			// set value to key
			if ( lua_isnumber( L, -1 ) )
				root.push_back( sub );
			else
				root[ lua_tostring( L, -1 ) ] = sub;


			// pop value + copy of key, leaving original key
			lua_pop( L, 2 );
			// stack now contains: -1 => key; -2 => table
		}
		// stack now contains: -1 => table (when lua_next returns 0 it pops the key
		// but does not push anything.)
		// Pop table
		lua_pop( L, 1 );
		// Stack is now the same as it was on entry to this function

		return root;
	}
	int load_table( lua_State* L, json& j, bool array )
	{
		runtime_state s( L );

		s.create_table();
		{
			if ( j.is_array() )
				array = true;

			// array iterator (lua arrays start at 1)
			auto i = 1;
			for ( auto& kv : j.items() )
			{
				if ( kv.value().is_string() )
					array ? s.set_i( i, kv.value().get<std::string>().c_str() ) : s.set_field( kv.key().c_str(), kv.value().get<std::string>().c_str() );
				else if ( kv.value().is_number() )
					array ? s.set_i( i, kv.value().get<double>() ) : s.set_field( kv.key().c_str(), kv.value().get<double>() );
				else if ( kv.value().is_boolean() )
					array ? s.set_i( i, kv.value().get<bool>() ) : s.set_field( kv.key().c_str(), kv.value().get<bool>() );
				else if ( kv.value().is_object() || kv.value().is_array() )
				{
					load_table( L, kv.value(), kv.value().is_array() );
					array ? s.set_i( i ) : s.set_field( kv.key().c_str() );
				}

				i++;
			}
		}

		return 1;
	}

	void print( const char* info_string )
	{
		interfaces::cvar()->ConsoleColorPrintf( Color( 171, 34, 210, 255 ), XOR_STR( "[lua] " ) );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), info_string );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), XOR_STR( "\n" ) );
	}

	void warn( const char* info_string )
	{
		interfaces::cvar()->ConsoleColorPrintf( Color( 171, 34, 210, 255 ), XOR_STR( "[lua] " ) );
		util::print_dev_console( true, Color( 255, 128, 0, 255 ), XOR_STR( "warning: " ) );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), info_string );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), XOR_STR( "\n" ) );
	}

	void error( const char* error_type, const char* error_string )
	{
		interfaces::cvar()->ConsoleColorPrintf( Color( 171, 34, 210, 255 ), XOR_STR( "[lua] " ) );
		util::print_dev_console( true, Color( 255, 0, 0 ), error_type );
		util::print_dev_console( true, Color( 255, 0, 0 ), XOR_STR( ": " ) );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), error_string );
		util::print_dev_console( true, Color( 255, 255, 255, 255 ), XOR_STR( "\n" ) );
		EMIT_ERROR_SOUND();
	}

	std::optional<uri> parse_uri( const std::string& u )
	{
		if ( u.find( XOR_STR( "http" ) ) != 0 || u.size() < 9 )
			return std::nullopt;

		uri res{};
		res.is_secure = u.find( XOR_STR( "https:" ) ) == 0;

		const auto host_offset = 7 + ( int ) res.is_secure;
		res.host = u.substr( host_offset, u.find( XOR_STR( "/" ), 8 ) - host_offset );
		res.path = u.substr( host_offset + res.host.size() );

		if ( const auto port_p = res.host.find( XOR_STR( ":" ) ); port_p != std::string::npos )
		{
			const auto port_s = res.host.substr( port_p + 1 );
			res.host = res.host.substr( 0, port_p );
			res.port = std::stoi( port_s );
		}
		else
			res.port = res.is_secure ? 443 : 80;

		if ( res.path.empty() )
			res.path = XOR_STR( "/" );

		return res;
	}

	inline std::unordered_map<uint32_t, std::vector<std::shared_ptr<timed_callback>>> timers{};
	inline std::unordered_map<std::shared_ptr<control>, int> hidden_elements{};

	timed_callback::timed_callback( float delay, std::function<void()> callback )
	{
		this->delay = delay;
		this->callback = std::move( callback );
	}
}