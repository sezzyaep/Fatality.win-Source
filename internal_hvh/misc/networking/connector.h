//#pragma once
//
//#include <thread>
//#include <mutex>
//#include <atomic>
//#include <iostream>
//
//#include "data.h"
//#include "net_offsets.h"
//#include "../../monocypher.h"
//
//__declspec( dllexport ) extern uint32_t rdatastart;
//__declspec( dllexport ) extern uint32_t rdataend;
//__declspec( dllexport ) extern uint32_t textstart;
//__declspec( dllexport ) extern uint32_t textend;
//
//namespace networking
//{
//	class connector
//	{
//	public:
//		__forceinline connector( const char* ip, uint16_t port, const char* token ) : exit_flag_( false ), is_ready_( false ), processed_( false )
//		{
//#ifdef SOCKET_RECONNECT
//			enc_token = _w2( std::string( token ) );
//			enc_ip = _w2( std::string( ip ) );
//			enc_port = _w2( std::to_string( port ) );
//#endif
//			session_.reset( new session( ip, port, token ) );
//
//			msg::client_key key;
//			generate_keys( key.pk );
//
//			msg::message m( msg::msg_id::msg_client_key );
//			m.set_data( key );
//
//			queue_message( m );
//
//			syscall( NtCreateThreadEx )( &main_thread_, MAXIMUM_ALLOWED, nullptr, current_process,
//				&worker, this, 0x40 | THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER, NULL, NULL, NULL, nullptr );
//			sysunlock();
//
//			imp( "kernel32.dll", SetThreadPriority )( main_thread_, THREAD_MODE_BACKGROUND_BEGIN );
//		}
//
//		__forceinline ~connector()
//		{
//			exit_flag_ = true;
//			network_offsets->queue_cond_.notify_all();
//			imp( "kernel32.dll", WaitForSingleObject )( main_thread_, INFINITE );
//		}
//
//#pragma comment( lib, "Bcrypt" )
//#include <../../monocypher.h>
//
//		__forceinline void generate_keys( uint8_t pk[ 32 ] ) const
//		{
//			uint8_t sk[ 32 ];
//
//			BCRYPT_ALG_HANDLE prov = nullptr;
//			if ( !BCRYPT_SUCCESS( BCryptOpenAlgorithmProvider( &prov, BCRYPT_RNG_ALGORITHM, nullptr, 0 ) ) || !BCRYPT_SUCCESS( BCryptGenRandom( prov, sk, 16, 0 ) ) )
//			{
//				if ( prov )
//					BCryptCloseAlgorithmProvider( prov, 0 );
//				session_->close();
//				return;
//			}
//
//			BCryptCloseAlgorithmProvider( prov, 0 );
//
//			crypto_x25519_public_key( pk, sk );
//
//			uint8_t other_pk[ 32 ] = { 242, 131, 134, 134, 5, 91, 45, 226, 210, 138, 65, 6, 23, 47, 66, 183, 5, 67, 50, 14, 147, 29, 123, 219, 244, 232, 209, 204, 78, 203, 26, 10 };
//
//			uint8_t shared_secret[ 32 ];
//			crypto_x25519( shared_secret, sk, other_pk );
//			crypto_wipe( sk, 32 );
//
//			uint8_t shared_keys[ 64 ]; /* Two shared session keys */
//			crypto_blake2b_ctx ctx;
//			crypto_blake2b_init( &ctx );
//			crypto_blake2b_update( &ctx, shared_secret, 32 );
//			crypto_blake2b_update( &ctx, pk, 32 );
//			crypto_blake2b_update( &ctx, other_pk, 32 );
//			crypto_blake2b_final( &ctx, shared_keys );
//
//			crypto_wipe( other_pk, 32 );
//
//			const uint8_t* key_2 = shared_keys + 32; /* Shared key 2 */
//
//			memcpy( session_->key.shared_key, key_2, 32 );
//		}
//
//		__forceinline void queue_message( msg::message& m )
//		{
//			std::vector<unsigned char> data_;
//			auto size_ = 0u;
//
//			auto pack = json::to_msgpack( json( m ) );
//			data_.resize( data_.size() + ( session_->key.has_key ? 16 : 0 ) + pack.size() );
//
//			if ( session_->key.has_key )
//			{
//				uint8_t mac[ 16 ];
//				crypto_lock( mac, pack.data(), session_->key.shared_key, session_->key.nonce, pack.data(), pack.size() );
//				memcpy( data_.data() + size_, mac, 16 );
//				size_ += 16;
//			}
//
//			memcpy( data_.data() + size_, pack.data(), pack.size() );
//
//
//			std::lock_guard<std::mutex> lock( queue_mutex_out );
//			queued_data_out.emplace_back( data_ );
//		}
//
//		__forceinline void send_report( msg::report_type type, uint32_t code )
//		{
//			msg::report report;
//			report.type = type;
//			report.code = code;
//
//			msg::message m( msg::msg_id::msg_report );
//			m.set_data( report );
//
//			queue_message( m );
//		}
//
//		__forceinline void send_modules( const std::vector<uint32_t>& modules )
//		{
//			msg::module_info info;
//			info.modules = modules;
//			info._reserved = 0;
//
//			msg::message m( msg::msg_id::msg_module_info );
//			m.set_data( info );
//
//			queue_message( m );
//
//			network_offsets->queue_cond_.notify_all();
//		}
//
//		__forceinline void send_heartbeat()
//		{
//			msg::heartbeat heartbeat;
//			heartbeat.number = crc64::calculate( textstart, textend, rdatastart, rdataend );
//
//			msg::message m( msg::msg_id::msg_heartbeat );
//			m.set_data( heartbeat );
//
//			queue_message( m );
//		}
//
//		__forceinline void send_status( const int code )
//		{
//			msg::status status;
//			status.crc = crc64::calculate( textstart, textend, rdatastart, rdataend );
//			status.code = code;
//
//			msg::message m( msg::msg_id::msg_status );
//			m.set_data( status );
//
//			queue_message( m );
//
//			++network_offsets->in_request;
//			network_offsets->queue_cond_.notify_all();
//		}
//
//		std::vector<msg::offsets> offsets_ready_;
//		std::atomic_bool exit_flag_;
//		std::atomic_bool is_ready_;
//		std::atomic_bool processed_;
//
//		std::vector<std::vector<unsigned char>> queued_data_out;
//		std::mutex queue_mutex_out;
//	private:
//
//		__forceinline void prepare_queued_data()
//		{
//			log_debug( "try_lock\n" );
//			if ( queue_mutex_out.try_lock() )
//			{
//				log_debug( "for\n" );
//				for ( auto& iter : queued_data_out )
//				{
//					log_debug( "push_data\n" );
//					push_data( iter );
//					log_debug( "push_data done\n" );
//				}
//				log_debug( "clear\n" );
//				queued_data_out.clear();
//				log_debug( "unlock\n" );
//				queue_mutex_out.unlock();
//			}
//		}
//
//		__forceinline void push_queued_offsets()
//		{
//			if ( offsets_ready_.empty() )
//				return;
//
//			network_offsets->vec_offsets = offsets_ready_.back().offset_data;
//			if ( network_offsets->set_offsets() )
//				offsets_ready_.pop_back();
//		}
//
//		__declspec( noinline ) bool process()
//		{
//			VIRTUALIZER_DOLPHIN_WHITE_START;
//			std::lock_guard<std::mutex> lock( session_mutex );
//			inc_data_.clear();
//			auto ret = false;
//			inc_data_ = session_->process( ret );
//
//			VIRTUALIZER_DOLPHIN_WHITE_END;
//			return ret;
//		}
//
//		__forceinline void resolve_incoming_messages()
//		{
//			for ( auto data : inc_data_ )
//			{
//				if ( session_->key.has_key )
//				{
//					auto dec = std::vector<unsigned char>( data.size() - 16 );
//					const auto mac = std::vector<unsigned char>( data.begin(), data.begin() + 16 );
//					if ( crypto_unlock( dec.data(), session_->key.shared_key, session_->key.nonce, mac.data(), data.data() + 16, dec.size() ) != 0 )
//					{
//						session_->close();
//						return;
//					}
//
//					data = dec;
//				}
//
//				const auto pack = json::from_msgpack( data.begin(), data.end() );
//
//				auto m = pack.get<msg::message>();
//				if ( !session_->key.has_key )
//				{
//					if ( m.id != msg::msg_id::msg_server_nonce )
//					{
//						session_->close();
//						return;
//					}
//
//					const auto nonce = m.get_data<msg::server_nonce>();
//					memcpy( session_->key.nonce, nonce.nonce, 24 );
//					session_->key.has_key = true;
//
//					continue;
//				}
//
//				log_debug_fmt( "received msg with id: %d\n", m.id );
//
//				if ( m.id == msg::msg_id::msg_offsets )
//					handle_offset_updates( m );
//				if ( m.id == msg::msg_id::msg_user_info )
//					handle_user_info( m );
//				if ( m.id == msg::msg_id::msg_heartbeat )
//					handle_heartbeat( m );
//				if ( m.id == msg::msg_id::msg_status )
//					handle_status( m );
//				if ( m.id == msg::msg_id::msg_patch )
//					handle_patch( m );
//				if ( m.id == msg::msg_id::msg_material_info )
//					handle_material( m );
//				if ( m.id == msg::msg_id::msg_font )
//					handle_font( m );
//				if ( m.id == msg::msg_id::msg_file )
//					handle_file( m );
//			}
//		}
//
//		__forceinline void handle_offset_updates( msg::message& data )
//		{
//			const auto offsets = data.get_data<msg::offsets>();
//			offsets_ready_.push_back( offsets );
//		}
//
//		__forceinline void handle_user_info( msg::message& data ) const
//		{
//			log_debug( "get_data\n" );
//			const auto info = data.get_data<msg::user_info>();
//			log_debug( "get_data success\n" );
//			globals::user_info.name = info.name;
//			globals::user_info.expiry = info.expiry;
//			globals::user_info.img = info.img;
//			globals::user_info.alpha = info.alpha;
//		}
//
//		__forceinline void handle_heartbeat( msg::message& data ) const
//		{
//			const auto heartbeat = data.get_data<msg::heartbeat>();
//
//			const auto current_time = duration_cast< std::chrono::minutes >( std::chrono::system_clock::now().time_since_epoch() ).count();
//
//			if ( heartbeat.number == fnv1a_rt( std::to_string( globals::secure_info.initial_checksum ) ) )
//			{
//				if ( abs( current_time - static_cast< int >( heartbeat.timestamp ) ) < 10 )
//					util::last_heartbeat_tick = util::get_current_tick();
//				else
//					log( "failure 0x5040\n" );
//			}
//			else
//			{
//				log( "failure 0x5050\n" );
//				util::crash();
//			}
//				
//		}
//
//		__forceinline void handle_status( msg::message& data ) const
//		{
//			const auto status = data.get_data<msg::status>();
//
//			log_debug_fmt( "handeled status code: %d\n", status.code );
//
//			if ( status.code == 6 )
//				init_status = 1;
//			if ( status.code == 7 )
//				init_status = 2;
//
//			--network_offsets->in_request;
//		}
//
//		__forceinline void handle_patch( msg::message& data ) const
//		{
//			const auto patch = data.get_data<msg::patch>();
//
//			DWORD old{};
//			auto addr = reinterpret_cast< uint32_t* >( patch.location );
//			memory::protect_mem( addr, patch.bytes.size(), PAGE_EXECUTE_READWRITE, old );
//			for ( auto i = 0u; i < patch.bytes.size(); i++ )
//				*reinterpret_cast< uint8_t* >( reinterpret_cast< uintptr_t >( addr ) + i ) = patch.bytes[ i ];
//			memory::protect_mem( addr, patch.bytes.size(), old, old );
//
//		}
//
//		__forceinline void handle_material( msg::message& data ) const
//		{
//			const auto material = data.get_data<msg::material_info>();
//
//			chams::add_material( static_cast< material_index_t >( material.idx ), material.shade, material.material );
//		}
//
//		__forceinline void handle_font( msg::message& data ) const
//		{
//			auto font = data.get_data<msg::font>();
//
//			DWORD n_fonts;
//			imp( "gdi32.dll", AddFontMemResourceEx )( font.data.data(), font.data.size(), nullptr, &n_fonts );
//		}
//
//		__forceinline void handle_file( msg::message& data ) const
//		{
//			const auto file = data.get_data<msg::file>();
//
//			util::create_file_from_memory( file.path, file.data );
//		}
//
//		static void __stdcall worker( connector* connector );
//
//		__forceinline void push_data( std::vector<unsigned char>& data )
//		{
//			log_debug( "session_mutex.lock\n" );
//			session_mutex.lock();
//			log_debug( "session_->update\n" );
//			session_->update( data );
//			log_debug( "session_mutex.unlock\n" );
//			session_mutex.unlock();
//		}
//
//	protected:
//		HANDLE main_thread_;
//		std::vector<std::vector<unsigned char>> inc_data_;
//		std::shared_ptr<session> session_;
//		std::mutex session_mutex;
//
//		std::string enc_token, enc_ip, enc_port;
//	};
//
//	inline connector* conn;
//}