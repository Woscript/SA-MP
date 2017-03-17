//projectile.inc - By Gammix (originally by PeppeAC) - v1.2 - updated: 17 Mar,2017
#if defined projectile_included
	#endinput
#endif
#define projectile_included

#include <colandreas>

#if !defined MAX_PROJECTILES
	#define MAX_PROJECTILES \
		100
#endif

#if !defined PROJECTILE_TIMER_INTERVAL
	#define PROJECTILE_TIMER_INTERVAL \
		20
#endif

#if !defined FLOAT_INFINITY
	#define FLOAT_INFINITY \
		Float:0x7F800000
#endif

#if defined OnProjectileUpdate
	forward OnProjectileUpdate(projid);
#endif

#if defined OnProjectileStop
	forward OnProjectileStop(projid);
#endif

#if defined OnProjectileCollide
	forward OnProjectileCollide(projid, type);
#endif

/*
native Projectile(Float:x, Float:y, Float:z, Float:vx, Float:vy, Float:vz, Float:sphere_radius = 1.0, Float:acceleration = 0.0, Float:friction = 10.0, Float:air_resistance = 0.5, Float:gravity = 10.0, bool:playercol = true);
native StopProjectile(projid);
native GetProjectilePos(projid, &Float:x, &Float:y, &Float:z);
native GetProjectileRot(projid, &Float:rx, &Float:ry, &Float:rz);
native GetProjectileVelocity(projid, &Float:vx, &Float:vy, &Float:vz);
*/

enum ProjectileCollision:(<<= 1)
{
	TEST
};

enum e_PROJECTILE
{
    Float:e_PROJECTILE_X,
    Float:e_PROJECTILE_Y,
	Float:e_PROJECTILE_Z,
    Float:e_PROJECTILE_RX,
    Float:e_PROJECTILE_RY,
	Float:e_PROJECTILE_RZ,
	Float:e_PROJECTILE_VX,
	Float:e_PROJECTILE_VY,
	Float:e_PROJECTILE_VZ,
	Float:e_PROJECTILE_RADIUS,
	Float:e_PROJECTILE_ACCELERATION,
	Float:e_PROJECTILE_MASS,
	Float:e_PROJECTILE_GROUND_FRICTION,
	Float:e_PROJECTILE_COLLISION_FRICTION,
	Float:e_PROJECTILE_GRAVITY,
	Float:e_PROJECTILE_AIR_RESISTANCE,
	bool:e_PROJECTILE_PLAYER_COL,
	e_PROJECTILE_TIMER
};

static eProjectile[MAX_PROJECTILES][e_PROJECTILE];
static iProjectileCount;

stock Projectile(Float:x, Float:y, Float:z, Float:vx, Float:vy, Float:vz, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, Float:sphere_radius = 1.0, Float:acceleration = 0.0, Float:ground_friction = 10.0, Float:collision_friction = 1.0, Float:air_resistance = 0.5, Float:gravity = 10.0, bool:playercol = true)
{
	if (iProjectileCount == MAX_PROJECTILES)
	    return -1;

	new i = iProjectileCount++;
    eProjectile[i][e_PROJECTILE_X] = x;
    eProjectile[i][e_PROJECTILE_Y] = y;
    eProjectile[i][e_PROJECTILE_Z] = z;
    eProjectile[i][e_PROJECTILE_RX] = rx;
    eProjectile[i][e_PROJECTILE_RY] = ry;
    eProjectile[i][e_PROJECTILE_RZ] = rz;
    eProjectile[i][e_PROJECTILE_VX] = vx;
    eProjectile[i][e_PROJECTILE_VY] = vy;
    eProjectile[i][e_PROJECTILE_VZ] = vz;
    eProjectile[i][e_PROJECTILE_RADIUS] = sphere_radius;
    eProjectile[i][e_PROJECTILE_ACCELERATION] = acceleration;
    eProjectile[i][e_PROJECTILE_GROUND_FRICTION] = ground_friction;
    eProjectile[i][e_PROJECTILE_COLLISION_FRICTION] = collision_friction;
    eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] = air_resistance;
    eProjectile[i][e_PROJECTILE_GRAVITY] = gravity;
    eProjectile[i][e_PROJECTILE_PLAYER_COL] = playercol;
    eProjectile[i][e_PROJECTILE_TIMER] = SetTimerEx("Internal_OnProjectileUpdate", PROJECTILE_TIMER_INTERVAL, true, "i", i);
	return i;
}

stock StopProjectile(projid)
{
    if (projid < 0 || projid >= iProjectileCount)
        return 0;

	KillTimer(eProjectile[projid][e_PROJECTILE_TIMER]);

	for (new i = projid; i < (iProjectileCount - 1); i++)
	{
	    eProjectile[i][e_PROJECTILE_X] = eProjectile[i + 1][e_PROJECTILE_X];
	    eProjectile[i][e_PROJECTILE_Y] = eProjectile[i + 1][e_PROJECTILE_Y];
	    eProjectile[i][e_PROJECTILE_Z] = eProjectile[i + 1][e_PROJECTILE_Z];
	    eProjectile[i][e_PROJECTILE_RX] = eProjectile[i + 1][e_PROJECTILE_RX];
	    eProjectile[i][e_PROJECTILE_RY] = eProjectile[i + 1][e_PROJECTILE_RY];
	    eProjectile[i][e_PROJECTILE_RZ] = eProjectile[i + 1][e_PROJECTILE_RZ];
	    eProjectile[i][e_PROJECTILE_VX] = eProjectile[i + 1][e_PROJECTILE_VX];
	    eProjectile[i][e_PROJECTILE_VY] = eProjectile[i + 1][e_PROJECTILE_VY];
	    eProjectile[i][e_PROJECTILE_VZ] = eProjectile[i + 1][e_PROJECTILE_VZ];
	    eProjectile[i][e_PROJECTILE_RADIUS] = eProjectile[i + 1][e_PROJECTILE_RADIUS];
	    eProjectile[i][e_PROJECTILE_ACCELERATION] = eProjectile[i + 1][e_PROJECTILE_ACCELERATION];
	    eProjectile[i][e_PROJECTILE_GROUND_FRICTION] = eProjectile[i + 1][e_PROJECTILE_GROUND_FRICTION];
	    eProjectile[i][e_PROJECTILE_COLLISION_FRICTION] = eProjectile[i + 1][e_PROJECTILE_COLLISION_FRICTION];
	    eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] = eProjectile[i + 1][e_PROJECTILE_AIR_RESISTANCE];
	    eProjectile[i][e_PROJECTILE_GRAVITY] = eProjectile[i + 1][e_PROJECTILE_GRAVITY];
	    eProjectile[i][e_PROJECTILE_PLAYER_COL] = eProjectile[i + 1][e_PROJECTILE_PLAYER_COL];
	    eProjectile[i][e_PROJECTILE_TIMER] = eProjectile[i + 1][e_PROJECTILE_TIMER];
	}
	iProjectileCount--;
	return 1;
}

stock GetProjectilePos(projid, &Float:x, &Float:y, &Float:z)
{
    if (projid < 0 || projid >= iProjectileCount)
        return 0;

    x = eProjectile[projid][e_PROJECTILE_X];
    y = eProjectile[projid][e_PROJECTILE_Y];
    z = eProjectile[projid][e_PROJECTILE_Z];
	return 1;
}

stock GetProjectileRot(projid, &Float:rx, &Float:ry, &Float:rz)
{
    if (projid < 0 || projid >= iProjectileCount)
        return 0;

    rx = eProjectile[projid][e_PROJECTILE_RX];
    ry = eProjectile[projid][e_PROJECTILE_RY];
    rz = eProjectile[projid][e_PROJECTILE_RZ];
	return 1;
}

stock GetProjectileVelocity(projid, &Float:vx, &Float:vy, &Float:vz)
{
    if (projid < 0 || projid >= iProjectileCount)
        return 0;

    vx = eProjectile[projid][e_PROJECTILE_VX];
    vy = eProjectile[projid][e_PROJECTILE_VY];
    vz = eProjectile[projid][e_PROJECTILE_VZ];
	return 1;
}

forward Internal_OnProjectileUpdate(i);
public Internal_OnProjectileUpdate(i)
{
	// calculate next position at timestep
	// distance = velocity * time
    new Float:new_x = eProjectile[i][e_PROJECTILE_X] + eProjectile[i][e_PROJECTILE_VX] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
	new Float:new_y = eProjectile[i][e_PROJECTILE_Y] + eProjectile[i][e_PROJECTILE_VY] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
	new Float:new_z = eProjectile[i][e_PROJECTILE_Z] + eProjectile[i][e_PROJECTILE_VZ] * (PROJECTILE_TIMER_INTERVAL / 1000.0);

	// calculate max & min height the sphere can reach
	new Float:unused;
	new Float:min_height;
	CA_RayCastLineAngle(eProjectile[i][e_PROJECTILE_X], eProjectile[i][e_PROJECTILE_Y], eProjectile[i][e_PROJECTILE_Z], eProjectile[i][e_PROJECTILE_X], eProjectile[i][e_PROJECTILE_Y], eProjectile[i][e_PROJECTILE_Z] - 1000.0, unused, unused, min_height, unused, unused, unused);
	if (min_height != 0.0)
		min_height += eProjectile[i][e_PROJECTILE_RADIUS];

	new Float:max_height;
	CA_RayCastLineAngle(eProjectile[i][e_PROJECTILE_X], eProjectile[i][e_PROJECTILE_Y], eProjectile[i][e_PROJECTILE_Z], eProjectile[i][e_PROJECTILE_X], eProjectile[i][e_PROJECTILE_Y], eProjectile[i][e_PROJECTILE_Z] + 1000.0, unused, unused, max_height, unused, unused, unused);
	if (max_height > min_height)
		max_height -= eProjectile[i][e_PROJECTILE_RADIUS];
	else
		max_height = FLOAT_INFINITY;

    if (new_z > max_height)
	{
		if (eProjectile[i][e_PROJECTILE_VZ] > 0)
		{
			eProjectile[i][e_PROJECTILE_VZ] = 0.0;
			#if defined OnProjectileCollide
				OnProjectileCollide(i, 1); // COLLIDE_HIGH_Z_BOUND
			#endif
		}
		new_z = max_height;
	}
	else if (new_z < min_height)
	{
		if (eProjectile[i][e_PROJECTILE_VZ] < 0)
		{
            eProjectile[i][e_PROJECTILE_VZ] = 0.0;
			#if defined OnProjectileCollide
				OnProjectileCollide(i, 2); // COLLIDE_LOW_Z_BOUND
			#endif
		}
		new_z = min_height;
	}

	// if velocity is 0, stop simulation (KillTimer)
	if (eProjectile[i][e_PROJECTILE_VX] == 0.0 && eProjectile[i][e_PROJECTILE_VY] == 0.0 && eProjectile[i][e_PROJECTILE_VZ] == 0.0)
	{
	    StopProjectile(i);
	    #if defined OnProjectileStop
			OnProjectileStop(i);
		#endif
		return;
	}

	// collision check with SA-World
	new Float:cx,
		Float:cy,
		Float:cz,
		Float:crx,
		Float:cry,
		Float:crz;
	if (CA_RayCastLineAngle(new_x, new_y, new_z, (new_x + eProjectile[i][e_PROJECTILE_RADIUS]), (new_y + eProjectile[i][e_PROJECTILE_RADIUS]), (new_x + eProjectile[i][e_PROJECTILE_RADIUS]), cx, cy, cz, crx, cry, crz) != 0)
	{
		new Float:angle = atan2(-cry, crx);
		
		new Float:new_vx = ((eProjectile[i][e_PROJECTILE_VX] * floatcos(angle, degrees)) - (eProjectile[i][e_PROJECTILE_VY] * floatsin(angle, degrees)));
		new Float:new_vy = -((eProjectile[i][e_PROJECTILE_VX] * floatsin(angle, degrees)) + (eProjectile[i][e_PROJECTILE_VY] * floatcos(angle, degrees)));

		angle *= -1;
		eProjectile[i][e_PROJECTILE_VX] = ((new_vx * floatcos(angle, degrees)) - (new_vy * floatsin(angle, degrees)));
		eProjectile[i][e_PROJECTILE_VY] = ((new_vx * floatsin(angle, degrees)) + (new_vy * floatcos(angle, degrees)));
		
		// apply collision friction
		if (eProjectile[i][e_PROJECTILE_COLLISION_FRICTION] != 0.0)
		{
			new Float:speed = floatsqroot((eProjectile[i][e_PROJECTILE_VX] * eProjectile[i][e_PROJECTILE_VX]) + (eProjectile[i][e_PROJECTILE_VY] * eProjectile[i][e_PROJECTILE_VY]));
			speed -= eProjectile[i][e_PROJECTILE_COLLISION_FRICTION] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
			if (speed < 0.001)
				speed = 0;

			eProjectile[i][e_PROJECTILE_VX] = speed * floatsin(-angle, degrees);
			eProjectile[i][e_PROJECTILE_VY] = speed * floatcos(-angle, degrees);
		}
		//
		
		angle += ((new_vy > 0) ? (90.0) : (-90.0));
		new_x = (cx + ((eProjectile[i][e_PROJECTILE_RADIUS] + 0.001) * floatcos(angle, degrees)));
		new_y = (cy + ((eProjectile[i][e_PROJECTILE_RADIUS] + 0.001) * floatsin(angle, degrees)));

		#if defined OnProjectileCollide
			OnProjectileCollide(i, 3); // COLLIDE_SA_WORLD
		#endif
	}

	// collision check with players
	if (eProjectile[i][e_PROJECTILE_PLAYER_COL])
	{
	 	new Float:px,
		    Float:py,
		    Float:pz,
		    Float:dx,
		    Float:dy,
			Float:new_vx,
			Float:new_vy,
			Float:angle;
		for (new a, b = GetPlayerPoolSize(); a <= b; a++)
		{
		    if (IsPlayerConnected(a))
		    {
		        GetPlayerPos(a, px, py, pz);
		        if ((pz - (min_height - eProjectile[i][e_PROJECTILE_RADIUS])) < new_z < (pz + (max_height + eProjectile[i][e_PROJECTILE_RADIUS])))
				{
					dx = new_x - px;
					dy = new_y - py;
					if (((dx * dx) + (dy * dy)) < ((eProjectile[i][e_PROJECTILE_RADIUS] + 0.8) * (eProjectile[i][e_PROJECTILE_RADIUS] + 0.8)))
					{
						if (((eProjectile[i][e_PROJECTILE_VX] * dx) + (eProjectile[i][e_PROJECTILE_VY] * dy)) < 0.0)
						{
							angle = -atan2(dy, dx);
							new_vx = -1 * ((eProjectile[i][e_PROJECTILE_VX] * floatcos(angle, degrees)) - (eProjectile[i][e_PROJECTILE_VY] * floatsin(angle, degrees)));
							new_vy = ((eProjectile[i][e_PROJECTILE_VX] * floatsin(angle, degrees)) + (eProjectile[i][e_PROJECTILE_VY] * floatcos(angle, degrees)));

							angle *= -1;
							eProjectile[i][e_PROJECTILE_VX] = ((new_vx * floatcos(angle, degrees)) - (new_vy * floatsin(angle, degrees)));
							eProjectile[i][e_PROJECTILE_VY] = ((new_vx * floatsin(angle, degrees)) + (new_vy * floatcos(angle, degrees)));

							// apply collision friction
							if (eProjectile[i][e_PROJECTILE_COLLISION_FRICTION] != 0.0)
							{
								new Float:speed = floatsqroot((eProjectile[i][e_PROJECTILE_VX] * eProjectile[i][e_PROJECTILE_VX]) + (eProjectile[i][e_PROJECTILE_VY] * eProjectile[i][e_PROJECTILE_VY]));
								speed -= eProjectile[i][e_PROJECTILE_COLLISION_FRICTION] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
								if (speed < 0.001)
									speed = 0;

								eProjectile[i][e_PROJECTILE_VX] = speed * floatsin(-angle, degrees);
								eProjectile[i][e_PROJECTILE_VY] = speed * floatcos(-angle, degrees);
							}
							//

							#if defined OnProjectileCollide
								OnProjectileCollide(i, 4); // COLLIDE_PLAYER
							#endif
						}
					}
				}
		    }
		}
	}

	// apply gravitation force
	if (eProjectile[i][e_PROJECTILE_GRAVITY] != 0.0)
	{
	    if (eProjectile[i][e_PROJECTILE_VZ] > 0)
		{
			eProjectile[i][e_PROJECTILE_VZ] -= (eProjectile[i][e_PROJECTILE_GRAVITY] * (PROJECTILE_TIMER_INTERVAL / 1000.0));
			if (eProjectile[i][e_PROJECTILE_VZ] < 0)
				eProjectile[i][e_PROJECTILE_VZ] = 0;
		}
		else
			eProjectile[i][e_PROJECTILE_VZ] -= (eProjectile[i][e_PROJECTILE_GRAVITY] * (PROJECTILE_TIMER_INTERVAL / 1000.0));
	}

	// apply friction
	new Float:moveangle = (atan2(eProjectile[i][e_PROJECTILE_VY], eProjectile[i][e_PROJECTILE_VX]) - 90.0);
	new Float:speed = floatsqroot((eProjectile[i][e_PROJECTILE_VX] * eProjectile[i][e_PROJECTILE_VX]) + (eProjectile[i][e_PROJECTILE_VY] * eProjectile[i][e_PROJECTILE_VY]));
	if (eProjectile[i][e_PROJECTILE_GROUND_FRICTION] != 0.0 && new_z == min_height)
	{
		speed -= eProjectile[i][e_PROJECTILE_GROUND_FRICTION] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
		if (speed < 0.001)
			speed = 0;

		eProjectile[i][e_PROJECTILE_VX] = speed * floatsin(-moveangle, degrees);
		eProjectile[i][e_PROJECTILE_VY] = speed * floatcos(-moveangle, degrees);
	}
	
	// apply air resistance
	if (eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] != 0.0)
	{
		eProjectile[i][e_PROJECTILE_VX] -= eProjectile[i][e_PROJECTILE_VX] * eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
		eProjectile[i][e_PROJECTILE_VY] -= eProjectile[i][e_PROJECTILE_VY] * eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
		eProjectile[i][e_PROJECTILE_VZ] -= eProjectile[i][e_PROJECTILE_VZ] * eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
	}
	
	// update rotation
	if (speed > 0.0)
	{
	    eProjectile[i][e_PROJECTILE_RX] -= ((speed * (PROJECTILE_TIMER_INTERVAL / 1000.0)) * ((180.0 / 3.14159) / eProjectile[i][e_PROJECTILE_RADIUS]));
		if (eProjectile[i][e_PROJECTILE_RX] < 0.0)
		{
			eProjectile[i][e_PROJECTILE_RX] += 360.0;
		}
		eProjectile[i][e_PROJECTILE_RZ] = moveangle;
	}
	
	// update position
	eProjectile[i][e_PROJECTILE_X] = new_x;
	eProjectile[i][e_PROJECTILE_Y] = new_y;
	eProjectile[i][e_PROJECTILE_Z] = new_z;

	// update velocity
	eProjectile[i][e_PROJECTILE_VX] += eProjectile[i][e_PROJECTILE_ACCELERATION];
	eProjectile[i][e_PROJECTILE_VY] += eProjectile[i][e_PROJECTILE_ACCELERATION];
	eProjectile[i][e_PROJECTILE_VZ] += eProjectile[i][e_PROJECTILE_ACCELERATION];

	#if defined OnProjectileUpdate
		OnProjectileUpdate(i);
	#endif
}