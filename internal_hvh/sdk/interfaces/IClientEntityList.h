#pragma once

class IClientEntity;
class IClientUnknown;
class IClientNetworkable;
using CBaseHandle = unsigned long;

struct EntityCacheInfo_t
{
	// Cached off because GetClientNetworkable is called a *lot*
	IClientNetworkable* m_pNetworkable;
	unsigned short m_BaseEntitiesIndex;	// Index into m_BaseEntities (or m_BaseEntities.InvalidIndex() if none).
	unsigned short m_bDormant;	// cached dormant state - this is only a bit
};

#define	MAX_EDICT_BITS				11
#define NUM_ENT_ENTRY_BITS		(MAX_EDICT_BITS + 2)
#define NUM_ENT_ENTRIES			(1 << NUM_ENT_ENTRY_BITS)

union full_handle
{
	struct
	{
		uint16_t index;
		uint16_t serial;
	};
	uint32_t full;
};

class C_CSPlayer;

struct ent_info_t
{
	IClientEntity* entity;
	uint32_t serial;
	ent_info_t* prev;
	ent_info_t* next;
};

struct ent_info_list_t
{
	ent_info_t* head;
	ent_info_t* tail;
};

class IClientEntityList
{
	inline static auto constexpr num_ent_entries = 8192;

	struct entity_iterator_t
	{
		using value_type = C_BaseEntity*;

		ent_info_t* ptr;
		ent_info_t* tail;

		inline entity_iterator_t& operator++()
		{
			if ( ptr == tail || ptr->next > tail )
				ptr = nullptr;
			else
				ptr = ptr->next;
			return *this;
		}

		inline C_BaseEntity* operator->() const
		{
			return ( C_BaseEntity* ) ptr->entity;
		}

		inline C_BaseEntity* operator*() const
		{
			return ( C_BaseEntity* ) ptr->entity;
		}

		inline bool operator==( entity_iterator_t const& other ) const
		{
			return ptr == other.ptr;
		}

		inline bool operator!=( entity_iterator_t const& other ) const
		{
			return ptr != other.ptr;
		}
	};

	struct player_iterator_t : public entity_iterator_t
	{
		inline C_CSPlayer* operator->() const
		{
			return ( C_CSPlayer* ) ptr->entity;
		}

		inline C_CSPlayer* operator*() const
		{
			return ( C_CSPlayer* ) ptr->entity;
		}
	};

	struct players_t
	{
		IClientEntityList* parent;

		players_t( IClientEntityList* parent ) : parent( parent ) {}

		[[nodiscard]] __forceinline player_iterator_t begin()
		{
			auto world = &( ( ent_info_t* ) parent )[ 0 - ( num_ent_entries + 1 ) ];
			return { { world->next && world->next->entity ? world->next : nullptr, &( ( ent_info_t* ) parent )[ 64 - ( num_ent_entries + 1 ) ] } };
		}

		[[nodiscard]] __forceinline player_iterator_t end()
		{
			// begin->prev holds the last entity reachable.
			return { { nullptr , nullptr } };
		}
	};

public:
	[[nodiscard]] __forceinline int32_t get_highest_entity_index()
	{
		return *( uint32_t* ) ( uintptr_t( this ) + 0x24 );
	}

	[[nodiscard]] __forceinline ent_info_list_t* get_active_list()
	{
		return ( ent_info_list_t* ) ( uintptr_t( this ) - 0x10 );
	}

	[[nodiscard]] __forceinline IClientEntity* get_client_entity( uint32_t ent )
	{
		if ( ent >= 0 && ent < num_ent_entries )
			return ( ( ent_info_t* ) this )[ ent - ( num_ent_entries + 1 ) ].entity;

		return nullptr;
	}

	[[nodiscard]] __forceinline IClientEntity* get_client_entity_from_handle( CBaseHandle e )
	{
		full_handle ent{ .full = e };
		if ( ent.full == -1 )
			return nullptr;

		auto& inf = ( ( ent_info_t* ) this )[ ent.index - ( num_ent_entries + 1 ) ];
		if ( inf.serial == ent.serial )
			return inf.entity;

		return nullptr;
	}

	[[nodiscard]] __forceinline entity_iterator_t begin()
	{
		auto active = get_active_list();
		return { active->head ? active->head : nullptr, active->tail };
	}

	[[nodiscard]] __forceinline entity_iterator_t end()
	{
		// begin->prev holds the last entity reachable.
		return { nullptr, nullptr };
	}

	[[nodiscard]] __forceinline ent_info_t* get_next_info( ent_info_t* info )
	{
		auto list = get_active_list()->head;
		while ( list )
		{
			if ( list > info && list->entity )
				return list;

			list = list->next;
		}
		return nullptr;
	}

	inline players_t get_players()
	{
		return players_t( this );
	}

	template <typename f>
	inline void for_each( f fn )
	{
		for ( auto ent : *this )
			fn( ent );
	}

	void for_each_z( std::function<void( C_BaseEntity* )> fn );
	void for_each_player_z( std::function<void( C_CSPlayer* )> fn );

	template <typename f>
	inline void for_each_player( f fn )
	{
		for ( auto ent : get_players() )
			fn( reinterpret_cast< C_CSPlayer* >( ent ) );
	}
};
