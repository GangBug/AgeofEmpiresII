#pragma once
#ifndef __BOSS_H__
#define __BOSS_H__

#include "Units.h"
#define BOSSHP 2000


class Boss : public Unit
{
public:
	Boss(UNIT_TYPE u_type, fPoint pos, int id);

	void Update(); // defines order
	void PostUpdate();


	void Move();
	bool ZergCreate(int i = 1)const;
	bool AttackUnit();
	bool CheckSurroundings();

	int wave = 4;
};

#endif#pragma once
