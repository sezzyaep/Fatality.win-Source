#pragma once

#include <string>
#include <vector>

#include "../misc/json.hpp"

#define NLOHMANN_JSON_XOR_TO(v1) nlohmann_json_j[XOR(#v1)] = nlohmann_json_t.v1;
#define NLOHMANN_JSON_XOR_FROM(v1) nlohmann_json_j.at(XOR(#v1)).get_to(nlohmann_json_t.v1);

#define MSG_DATA(Type, ...)  \
	friend void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_XOR_TO, __VA_ARGS__)) } \
	friend void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_XOR_FROM, __VA_ARGS__)) }

namespace networking
{
	namespace msg
	{
		enum msg_id : char
		{
			msg_none,
			msg_hello,
			msg_reply,

			msg_client_key,
			msg_server_nonce,

			msg_report,
			msg_module_info,
			msg_offset_request,
			msg_offsets,
			msg_user_info,
			msg_heartbeat,
			msg_material_info,
			msg_patch,
			msg_font,
			msg_file,
			msg_status,

			msg_refresh_scripts,		// (hack only) refresh scripts list
			msg_refresh_configs,		// (hack only) refresh configs list
			msg_get_script_key,			// (hack only) get script encryption key
			msg_get_resource,			// (hack only) get resource by name
			msg_isp_restore,			// (hack only) restore isp
			msg_isp_purchase,			// (hack only) purchase isp
			msg_get_isp_info,			// (hack only) get isp info
			msg_report_crash,			// (hack only) report crash
		};

		NLOHMANN_JSON_SERIALIZE_ENUM( msg_id, {
			{ msg_none, FNV1A( "none" ) },
			{ msg_hello, FNV1A( "hello" ) },
			{ msg_reply, FNV1A( "reply" ) },

			{ msg_client_key, FNV1A( "msg_client_key" ) },
			{ msg_server_nonce, FNV1A( "msg_server_nonce" ) },

			{ msg_report, FNV1A( "msg_report" ) },
			{ msg_module_info, FNV1A( "msg_module_info" ) },
			{ msg_offset_request, FNV1A( "msg_offset_request" ) },
			{ msg_offsets, FNV1A( "msg_offsets" ) },
			{ msg_user_info, FNV1A( "msg_user_info" ) },
			{ msg_heartbeat, FNV1A( "msg_heartbeat" ) },
			{ msg_material_info, FNV1A( "msg_material_info" ) },
			{ msg_patch, FNV1A( "msg_patch" ) },
			{ msg_font, FNV1A( "msg_font" ) },
			{ msg_file, FNV1A( "msg_file" ) },
			{ msg_status, FNV1A( "msg_status" ) },

			{ msg_refresh_scripts, FNV1A( "msg_refresh_scripts" ) },
			{ msg_refresh_configs, FNV1A( "msg_refresh_configs" ) },
			{ msg_get_script_key, FNV1A( "msg_get_script_key" ) },
			{ msg_get_resource, FNV1A( "msg_get_resource" ) },
			{ msg_isp_restore, FNV1A( "msg_isp_restore" ) },
			{ msg_isp_purchase, FNV1A( "msg_isp_purchase" ) },
			{ msg_get_isp_info, FNV1A( "msg_get_isp_info" ) },
			{ msg_report_crash, FNV1A( "msg_report_crash" ) },
		} );

		class message
		{
		public:
			MSG_DATA( message, id, data, reply );

			message()
			{}
			message( msg_id id, const std::string& r = {} ) : id( id ), reply( r )
			{}

			msg_id id{};
			std::string reply{};
			nlohmann::json data{};

			template<typename T>
			T get_data()
			{
				return data.get<T>();
			}

			template<typename T>
			void set_data( const T& v )
			{
				data = v;
			}
		};

		inline message reply( const message& old )
		{
			return message( msg_reply, old.reply );
		}

		enum class report_type : uint8_t
		{
			NONE,
			TYPE1,
			TYPE2
		};

		struct offsets
		{
			MSG_DATA( offsets, offset_data );

			std::vector<std::pair<uint16_t, uint64_t>> offset_data{};
		};

		struct offset_request
		{
			MSG_DATA( offset_request, offset, crc );

			uint32_t offset;
			uint64_t crc{};
		};

		struct report
		{
			MSG_DATA( report, type, code );

			report_type type = report_type::NONE;
			uint32_t code{};
		};

		struct module_info
		{
			MSG_DATA( module_info, modules, _reserved );

			std::vector<uint32_t> modules{};
			uint64_t _reserved{};
		};

		struct user_info
		{
			MSG_DATA( user_info, name, expiry, img, alpha );

			std::string name;
			std::string expiry;
			std::vector<uint8_t> img;
			bool alpha{};
		};

		struct client_key
		{
			MSG_DATA( client_key, pk );

			uint8_t pk[ 32 ];
		};

		struct server_nonce
		{
			MSG_DATA( server_nonce, nonce );

			uint8_t nonce[ 24 ];
		};

		struct heartbeat
		{
			MSG_DATA( heartbeat, number, timestamp );

			uint64_t number;
			uint64_t timestamp;
		};

		struct material_info
		{
			MSG_DATA( material_info, idx, shade, material );

			int32_t idx;
			std::string shade;
			std::string material;
		};

		struct patch
		{
			MSG_DATA( patch, location, bytes );

			uint32_t location;
			std::vector<uint8_t> bytes;
		};

		struct font
		{
			MSG_DATA( font, data );

			std::vector<unsigned char> data;
		};

		struct file
		{
			MSG_DATA( file, path, data );

			std::string path;
			std::vector<unsigned char> data;
		};

		struct status
		{
			MSG_DATA( status, code, crc );

			uint16_t code;
			uint64_t crc;
		};
	}
}
