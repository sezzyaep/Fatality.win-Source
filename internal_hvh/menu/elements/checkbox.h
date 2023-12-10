#pragma once
#include <string>

class value;

class checkbox : public control
{
public:
	checkbox( std::string name, value* var );

	void run_render( evo::ren::vec2& pos ) override;
	void run_input() override;

	bool can_add_subcontrols() override
	{
		return true;
	}
private:

};
