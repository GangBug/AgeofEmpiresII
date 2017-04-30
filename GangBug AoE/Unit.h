#ifndef __UNIT_H__
#define __UNIT_H__

#include "Entity.h"
#include "Iso_Primitives.h"

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

enum unit_class
{
	NO_CLASS = 0,
	INFANTRY,
	RANGED,
	CAVALRY,
};

enum state
{
	NO_STATE,
	ATTACKING,
	MOVING_TO_ATTACK,
	VIGILANT,
	MOVING,
	FLEEING,
	DEAD
};

class Unit :public Entity
{
public:
	Unit(unit_type type, Entity* parent = nullptr);

	void OnUpdate(float dt)override;

	void SetHp(int newHP);

	unit_type GetType() const;
	action_type GetAction() const;

	direction GetDir() const;
	unit_type GetUnitType() const;
	unit_class GetUnitClass() const;
	int GetUnitRadius() const;
	bool GetPath(iPoint dest);
	int GetAttack() const;
	int GetRange() const;
	Circle GetVisionArea()const;
	bool IsMoving() const;
	bool Move();

	int GetPriority() const;
	void SetAction(action_type action);

	//TODO:this should be private?
	bool GetNextTile();


	void LookAt(iPoint pos);
	bool GoTo(iPoint destination);
	bool ChangeDirection(iPoint destination);

private:
	unit_type unitType;
	action_type action;
	unit_class unitClass;
	direction unitDirection;
	state unitState;

	int attack;
	int hp;
	int range;
	int unitRadius;
	float speed;
	float rate_of_fire;
	iPoint destination;
	iPoint pathObjective;
	fPoint moveVector;
	float angle;
	std::vector<iPoint> pathVec;

	Circle visionArea;
	Circle renderArea;

	bool haveADestination = false;

	int priority;
};

#endif //__UNIT_H__