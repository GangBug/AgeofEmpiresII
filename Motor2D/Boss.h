#pragma once
#ifndef __BOSS_H__
#define __BOSS_H__

#include "Units.h"

class Boss : public Unit
{
public:
	Boss(UNIT_TYPE u_type, fPoint pos, int id);

	void Update(); // defines order
	void PostUpdate();

	void Move();

	bool AttackUnit();
	bool CheckSurroundings();
	bool SetFightingArea();
};

#endif#pragma once
