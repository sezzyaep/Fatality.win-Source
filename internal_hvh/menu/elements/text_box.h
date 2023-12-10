#pragma once

namespace textbox_style
{
	enum flags
	{
		none = 1 << 0,
		search_bar = 1 << 1,
	};
}

class value;

class text_box : public control
{
public:
	text_box( std::string name, std::wstring& in, uint8_t flags = textbox_style::flags::none );
	text_box( std::string name, value* val, uint8_t flags = textbox_style::flags::none );

	void run_render( evo::ren::vec2& pos ) override;
	void run_input() override;
private:
	bool m_focus;

	std::wstring* m_text;
	value* m_value;

	uint8_t m_flags;
};