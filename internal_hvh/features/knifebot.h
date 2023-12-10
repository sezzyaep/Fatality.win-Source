#pragma once

namespace knifebot
{
	bool trace_knife( Vector spot, lag_record_t* record, bool& retstab );
	bool rules_knife( lag_record_t* record );
	bool is_behind(const lag_record_t* record);
	float get_damage(const lag_record_t* record, bool stab);
};