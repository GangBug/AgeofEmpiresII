#pragma once
#ifndef __BOSS_H__
#define __BOSS_H__

#include "Unit.h"

#define BOSSHP 2250

#define EASY_MODE_BOSS 1.15

class Timer;

class Boss : public Unit
{
public:
	Boss(fPoint pos, Entity* parent = nullptr);

	void OnUpdate(float dt);

	bool getBossControllableStatus();
	void setBossControllable(bool status);

private:
	bool isBossControllable = false;
	Timer diabloAttackSoundTimer; 

};

#endif