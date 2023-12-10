#pragma once

class CCLCMsg_Move
{
private:
	virtual ~CCLCMsg_Move();
	char pad[ 8 ]{};
public:
	int backup_commands{};
	int new_commands{};
};

namespace tickbase
{
	void reset();
	bool is_ready();
	void adjust_limit_dynamic( bool finalize = false );
	bool holds_tick_base_weapon();
	void attempt_shift_back( bool& send_packet );
	void revert_shift_back();
	void fill_fake_commands();
	void apply_static_configuration();
	int determine_optimal_shift();
	int determine_optimal_limit();
	float get_adjusted_time();
	int compute_current_limit( int command_number = globals::current_cmd ? globals::current_cmd->command_number : 0 );
	void on_send_command( const CUserCmd* cmd = globals::current_cmd );
	void on_finish_command( bool sendpacket, const CUserCmd* cmd = globals::current_cmd );
	void on_recharge( const CUserCmd* cmd );
	void on_runcmd( const CUserCmd* cmd, int& tickbase );

	inline int clock_drift = {}, server_limit = {}, to_recharge = {}, to_shift = {}, to_adjust = {}, to_correct = {};
	inline bool force_choke = {}, force_unchoke = {}, force_drop = {}, skip_next_adjust = {}, fast_fire = {}, hide_shot = {}, post_shift = {};
};