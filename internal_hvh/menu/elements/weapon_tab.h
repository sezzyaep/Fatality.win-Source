#pragma once

class weapon_tab : public sub_tab
{
public:
	weapon_tab( std::string name, std::vector<std::shared_ptr<child>> childs = {} );
	void run_render( evo::ren::vec2& pos ) override;
	void run_input() override;
	uint32_t texture_id;
};
