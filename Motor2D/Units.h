#ifndef _UNITS
#define _UNITS

#include "Entity.h"
#include "j1Timer.h"

#define XY_TILES_RELATION 2
#define RAD_TO_DEG 57.29577951

struct PathList;
struct PathNode;

enum UNIT_TYPE
{
	NO_UNIT = 0,
	TWOHANDEDSWORDMAN,
	TWOHANDEDSWORDMANENEMY,
	CAVALRYARCHER,
	ARCHER,
	SIEGERAM,
	SAMURAI,
	TARKANKNIGHT,
	VILE,
	BOSS
};

enum UNIT_CLASS
{
	NO_CLASS = 0,
	INFANTRY,
	RANGED,
	CAVALRY,
	SIEGE,
};

enum ACTION_TYPE
{
	NO_ACTION = 0,
	ATTACK,
	DIE,
	DISAPPEAR,
	IDLE,
	WALK
};

enum DIRECTION
{
	NO_DIRECTION = 0,
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST
};

enum STATE
{
	NONE,
	ATTACKING,
	MOVING_TO_ATTACK,
	VIGILANT,
	MOVING,
	FLEEING,
	DEAD
};

class Unit : public Entity
{
protected:
	enum UNIT_TYPE unit_type;
	enum ACTION_TYPE action_type; 
	enum DIRECTION direction;

	int attack;
	int range;
	float speed;
	float rate_of_fire;
	iPoint destination;
	UNIT_CLASS unit_class;
	iPoint path_objective;
	fPoint move_vector;
	float angle;
	std::list<iPoint> path_list;

	//TACTIC
	bool AI;
	Unit* enemy = nullptr;
	j1Timer attackingTimer;
	j1Timer logicTimer;
	std::list<iPoint> frontier;
	std::list<iPoint> visited;
	

public:

	bool GetNextTile();
	//should be private gotta change TODO


	Unit(UNIT_TYPE u_type, fPoint pos, int id);

	int unit_radius;
	int id;

	STATE state;

	void Update(); // defines order
	void PostUpdate();

	virtual void Move();
	virtual void DoAI();
	virtual void Draw();	

	const DIRECTION GetDir() const;
	const UNIT_TYPE GetUnitType() const;
	const ACTION_TYPE GetActionType() const;
	void SetAction(const ACTION_TYPE action);
	int GetPath(iPoint dest);
	void PopFirstPath();
	void AddPath(iPoint new_goal);

	//TACTIC
	bool AttackUnit();
	bool CheckSurroundings();
	bool debug = false;
	bool SetFightingArea();
	void LookAtEnemy();
	bool GetFreeAdjacent(iPoint& Adjacent) const;
	bool GetAdjacentTile(iPoint tile, iPoint& Adjacent) const;
	void DrawDebugRadius();
	void Die();
	bool GetAI() const;

	bool arrived = true;
	iPoint destin;

	j1Timer deathTimer;
};

#endif