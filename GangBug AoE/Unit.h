#ifndef __UNIT_H__
#define __UNIT_H__

#include "Entity.h"

enum unit_type
{
	DEFAULT_UNIT,
	CAVALRY_ARCHER
};

enum action_type
{
	ATTACK,
	IDLE,
	WALK
};

enum direction
{
	NORTH,
	NORTH_WEST,
	NORTH_EAST,
	EAST,
	WEST,
	SOUTH,
	SOUTH_EAST,
	SOUTH_WEST
};

class Unit :public Entity
{
public:
	Unit(unit_type type, Entity* parent = nullptr);

	void OnUpdate(float dt);

	unit_type GetType() const;
	action_type GetAction() const;
	direction GetDirection() const;

private:
	unit_type type;
	action_type action;
	direction unitDirection;

	//STATS?
};

#endif //__UNIT_H__