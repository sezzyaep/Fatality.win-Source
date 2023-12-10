#pragma once

class gamemovement
{
public:
	void walk_move( QAngle viewangles, float forwardmove, float sidemove );
	void accelerate( Vector& wishdir, float wishspeed, float accel, Vector& velocity );

	float maxspeed, surface_friction, encumberance, healthshot_expiration, time, duckamount;
	Vector velocity, origin;
	bool scoped, heavy_armor, hostage, walking;
	
};