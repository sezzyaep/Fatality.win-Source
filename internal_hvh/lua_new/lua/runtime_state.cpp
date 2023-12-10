#include <lua/state.h>

bool lua::runtime_state::check_argument_type( int pos, lua_type_def expected_type )
{
	auto result = false;
	switch ( expected_type )
	{
		case lua_type_def::nil:
			result = lua_isnil( l, pos );
			break;
		case lua_type_def::boolean:
			result = lua_isboolean( l, pos );
			break;
		case lua_type_def::light_user_data:
			result = lua_islightuserdata( l, pos );
			break;
		case lua_type_def::number:
			result = lua_isnumber( l, pos );
			break;
		case lua_type_def::string:
			result = lua_isstring( l, pos );
			break;
		case lua_type_def::table:
			result = lua_istable( l, pos );
			break;
		case lua_type_def::function:
			result = lua_isfunction( l, pos );
			break;
		case lua_type_def::user_data:
			result = lua_isuserdata( l, pos );
			break;
		case lua_type_def::thread:
			result = lua_isthread( l, pos );
			break;
		default: break;
	}

	return result;
}

bool lua::runtime_state::check_arguments( const std::vector<argument_desc>& desc, bool relaxed )
{
	const auto arg_count = get_stack_top();

	// we don't want more arguments than needed
	if ( !relaxed && arg_count > desc.size() )
		return false;

	int i{};
	for ( const auto& arg : desc )
	{
		// increment stack counter
		++i;

		// check if we have enough args
		if ( arg_count < i )
		{
			// check if it's a required argument or skip optional arguments if they are not present
			if ( !arg.is_optional ) return false;
			else continue;
		}

		// check corresponding type
		if ( !check_argument_type( i, arg.expected_type ) )
			return false;
	}

	return true;
}
