#pragma once

class IGameEvent;
namespace listeners
{
	class event_listeners : public IGameEventListener2
	{
	public:
		event_listeners()
		{
			interfaces::game_event_manager()->AddListenerGlobal( this, false );
		};
		~event_listeners() { interfaces::game_event_manager()->RemoveListener( this ); };
		virtual void	FireGameEvent( IGameEvent* p_event ) override;
		int				GetEventDebugID() override { return 0x2A; };
	};

	inline event_listeners * event_listener = {};

	__declspec( noinline ) inline void setup_listeners() { if ( !event_listener ) { event_listener = new event_listeners(); } erase_end }
	inline void remove_listeners() { delete event_listener; }
};

