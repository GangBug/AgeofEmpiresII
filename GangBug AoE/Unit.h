#ifndef __UNIT_H__
#define __UNIT_H__

#include "Entity.h"

enum unit_type
{
	DEFAULT_UNIT,
	ARCHER,
	TARKAN_KNIGHT,
	SAMURAI,
	BOSS,
	VILE
};

enum action_type
{
	ATTACK,
	IDLE,
	WALK,
	DIE,
	DISAPPEAR,
	NO_ACTION
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

	void OnUpdate(float dt)override;

	unit_type GetType() const;
	action_type GetAction() const;
	direction GetDirection() const;

	void SetDestination(int x, int y);
	void SetDestination(iPoint dst);

protected:
	direction GetDirectionFromVelocity(fPoint vel);

public:
	iPoint destination;

private:
	unit_type unitType;
	action_type action;
	direction unitDirection;

	fPoint velocity = {0.f, 0.f};
	float speed = 100.f;
	int arriveRadius = 5; //This radius is a distance to detect the arriving because an exact point is almost impossible to check

	bool haveADestination = false;

	//STATS?
};

#endif //__UNIT_H__