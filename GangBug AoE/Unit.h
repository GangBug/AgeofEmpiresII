#ifndef __UNIT_H__
#define __UNIT_H__

#include "Entity.h"
#include "Iso_Primitives.h"
#include "j1Timer.h"

#define EASY_MODE 1.2

enum unit_type
{
	DEFAULT_UNIT,
	ARCHER,
	TARKAN_KNIGHT,
	SAMURAI,
	VILE,
	VILLAGER,
	HELL_WITCH,
	DIABLO
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

	void SetFullHp(int newHP);

	void SetHpandFullHp(int currentHP, int fullHP);

	void SetAttack(int amount);

	unit_type GetType() const;
	action_type GetAction() const;

	direction GetDir() const;
	unit_type GetUnitType() const;
	unit_class GetUnitClass() const;

	int GetUnitRadius() const;
	int GetAttack() const;
	int GetRange() const;
	int GetHP() const;

	Circle GetVisionArea()const;

	bool GetPath(iPoint dest);
	bool IsMoving() const;
	bool Move();

	void DoDamage(int dmg);
	void Die();
	void PrintLife();

	int GetPriority() const;
	void SetAction(action_type action);

	//TODO:this should be private?
	bool GetNextTile();

	Entity* CheckSurroundings();

	bool SetFightingArea();
	bool SetBuildingFightingArea();

	void LookAt(iPoint pos);
	bool GoTo(iPoint destination);
	bool ChangeDirection(iPoint destination);
	bool AttackUnit();

	bool GetFreeAdjacent(iPoint& adjacent, iPoint currentPos = iPoint(0,0)) const;
	bool GetAdjacentTile(iPoint tile, iPoint & adjacent) const;

	void Serialize(pugi::xml_node&);

public:// ADD FX
	void PlayDeathSound() const; //called when the unit dies
	void PlayAttackSound() const; //called when the unit attack
	void PlaySelectSound()const; // called when the unit is selected
	void PlayMoveSound() const; //called when the unit is selected 

	bool horde;
protected:
	unit_type unitType;
	action_type action;
	unit_class unitClass;
	direction unitDirection;
	state unitState;

	int attack;
	int hp;
	int totalhp;
	int range;
	int unitRadius;
	float speed;
	float rate_of_fire;
	iPoint destination;
	iPoint pathObjective;
	fPoint moveVector;
	float angle;
	std::vector<iPoint> pathVec;

	Entity* target;
	Timer attackTimer;

	Circle visionArea;
	Circle renderArea;

	bool haveADestination = false;
	bool buildingToAttack = false;
	bool godMode = false;

	int priority;
};

#endif //__UNIT_H__