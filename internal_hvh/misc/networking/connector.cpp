//#include "../../include_cheat.h"
//#include "connector.h"
//#include <chrono>
//#include <sstream>
//#include <variant>
//
//namespace networking
//{
//	[[clang::optnone]] void __stdcall connector::worker( connector* conn )
//	{
//		VIRTUALIZER_DOLPHIN_BLACK_START;
//
//		log_debug( "starting worker\n" );
//
//		try
//		{
//			while ( !conn->exit_flag_.load() )
//			{
//				static bool once = true;
//				if ( once )
//					log( "--\n" );
//
//				if ( !conn->session_ )
//				{
//					SleepEx( 50, true );
//					continue;
//				}
//
//				if ( once )
//					log( "-1\n" );
//
//				std::unique_lock lock( network_offsets->queue_mtx_ );
//				network_offsets->queue_cond_.wait( lock, [conn] { return network_offsets->in_request.load() > 0 || network_offsets->heartbeat || !conn->queued_data_out.empty() || !conn->offsets_ready_.empty() || !conn->is_ready_.load(); } );
//				const auto was_heartbeat = conn->is_ready_.load() && network_offsets->heartbeat;
//				network_offsets->heartbeat = false;
//
//				if ( was_heartbeat )
//					conn->send_heartbeat();
//
//				if ( once )
//					log( "-2\n" );
//
//				conn->prepare_queued_data();
//				
//				if ( once )
//					log( "-1\n" );
//
//				if ( !conn->process() )
//				{
//#ifdef SOCKET_RECONNECT
//					_u( ip, conn->enc_ip );
//					_u( token, conn->enc_token );
//					_u( port, conn->enc_port );
//					conn->queued_data_out.clear();
//					conn->inc_data_.clear();
//					conn->session_.reset( new gr_connector::session( ip.c_str(), static_cast< uint16_t >( std::stoi( port ) ), token ) );
//					conn->is_ready_.store( false );
//					continue;
//#endif
//				/*imp( "user32.dll", MessageBoxA )( nullptr, xorstr_( "unresolved error: 0xC000002A" ), xorstr_( "error" ), MB_OK );
//				import( "ntdll.dll", RtlExitUserProcess )( -1 );
//				syscall( NtTerminateProcess )( current_process, -1 );
//				sysunlock();*/
//					util::error_close( xorstr_( "connection lost" ) );
//				}
//
//
//				if ( conn->queued_data_out.empty() )
//					conn->processed_ = true;
//
//				if ( was_heartbeat )
//					SleepEx( 50, false );
//
//				if ( once )
//					log( "-2\n" );
//
//				conn->resolve_incoming_messages();
//				if ( once )
//					log( "-1\n" );
//
//				conn->push_queued_offsets();
//
//				if ( !conn->is_ready_.load() )
//				{
//					if ( conn->session_->key.has_key )
//						conn->is_ready_ = true;
//#ifdef SOCKET_RECONNECT
//					for ( auto i = 0u; i < net_offsets::received.size(); i++ )
//					{
//						if ( net_offsets::requested[ i ] && !net_offsets::received[ i ] )
//							conn->request_offset( i );
//					}
//#endif
//				}
//
//				once = false;
//
//				SleepEx( was_heartbeat ? 50 : 1, true );
//
//			}
//		}
//		catch ( const std::exception& ex )
//		{
//			util::error_close( ex.what() );
//		}
//
//		VIRTUALIZER_DOLPHIN_BLACK_END;
//	}
//}