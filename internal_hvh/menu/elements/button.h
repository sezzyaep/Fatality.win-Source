#pragma once

class value;

namespace luabridge
{
	class LuaRef;
}

namespace button_style
{
	enum flags
	{
		none = 1 << 0,
		lua = 1 << 1,
	};
}

class button : public control
{
public:
	button( std::string name, std::function<void()> func, uint8_t flags = button_style::flags::none );

	void run_render( evo::ren::vec2& pos ) override;
	void run_input() override;

	void set_lua_callback( luabridge::LuaRef func );
private:
	std::function<void()> m_func = nullptr;
	std::thread m_func_thread = {};
	bool m_held = false;
	bool m_running = false;
	bool m_cancel_callback = false;

	uint8_t m_flags;
};