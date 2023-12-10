#pragma once

class pad : public control
{
public:
	pad( const int pad ) : m_pad( pad ) { m_size.y = pad; }

	void run_render( evo::ren::vec2& pos ) override
	{
		if ( !should_render() )
			return;
		pos.y += m_pad;
	}
	void run_input() override {}
private:
	int m_pad = 0;
};