#pragma once

namespace offsets
{
	enum class as_enum;
}

namespace networking
{
	class net_offsets
	{
	public:
		void init();
		uint64_t get_offset( uint64_t offset_hashed, bool remove = true );
		static void precache_map_load();
		void precache_init();

		__forceinline bool set_offsets()
		{
			if ( !mutex.try_lock() )
				return false;

			for ( auto& [id, offset] : vec_offsets )
			{
				if ( static_cast< offsets::as_enum >( id ) > offsets::as_enum::INVALID && static_cast< offsets::as_enum >( id ) < offsets::as_enum::COUNT )
					if ( !requested[ static_cast< size_t >( id ) ] )
						offset = id * offset ^ 0x1937282717263726;

				offsets[ id ] = offset;
			}

			mutex.unlock();
			return true;
		}

		std::mutex queue_mtx_ = {};
		std::condition_variable queue_cond_ = {};
		std::atomic_int in_request = 0;
		bool heartbeat = false;
		std::vector<bool> requested{};
		std::vector<bool> received{};
		std::shared_mutex mutex{};
		std::vector<uint64_t> offsets{};
		std::vector<std::pair<uint16_t, uint64_t>> vec_offsets{};

		static constexpr uint64_t deleted_val = 0x1947265784932001;
		static constexpr uint64_t init_val = 0x9273628581726547;
		static constexpr uint64_t invalid_val = 0x9273819408473928;
	};

	inline net_offsets* network_offsets;
}

