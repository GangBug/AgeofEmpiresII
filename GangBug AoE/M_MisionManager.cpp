#include "M_MisionManager.h"
#include "Log.h"

M_MisionManager::M_MisionManager(bool startEnabled) : Module(startEnabled)
{

	name.assign("misionManager");
	enemyStartTroops = 0;
	enemyTroopCounter = 0;
	aliveTroopWave = 0;
	bossIsAlive = true;
	townCenterIsAlive = true;
}

M_MisionManager::~M_MisionManager()
{
}

bool M_MisionManager::Start()
{
	LOG("Resources: Start.");
	bool ret = true;

	State = M_INTRO;
	

	return ret;
}

update_status M_MisionManager::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;


	switch (State)
	{
	case M_INTRO:

		if (misionTimer.ReadSec() > MISION_TIME) 
		{
			misionTimer.Start();
			State = M_TOWNATTACK;
		}

		break;

	case M_TOWNATTACK:

		if (GetEnemyUnits() <= enemyStartTroops + TROOPS_ONTOWN) 
		{
			State = M_TOWNREPAIR;
			misionTimer.Start();
		}

		break;

	case M_TOWNREPAIR:

		if (misionTimer.ReadSec() > TOWNREPAIR_TIME) 
		{
			State = M_WAVE1;
			misionTimer.Start();
		}

		break;

	case M_WAVE1:

		if (misionTimer.ReadSec() > WAVES_W8_TIME)
		{
			State = M_WAVE2;
			misionTimer.Start();
		}
		if (GetEnemyUnits() <= aliveTroopWave + TROOPS_WAVE1 && misionTimer.ReadSec() > WAVES_W8_TIME_DEADUNITS) 
		{
			State = M_WAVE2;
			misionTimer.Start();
		}

		break;

	case M_WAVE2:

		if (misionTimer.ReadSec() > WAVES_W8_TIME)
		{
			State = M_WAVE3;
			misionTimer.Start();

		}
		if (GetEnemyUnits() <= aliveTroopWave + TROOPS_WAVE2 && misionTimer.ReadSec() > WAVES_W8_TIME_DEADUNITS) 
		{
			State = M_WAVE3;
			misionTimer.Start();
		}

		break;

	case M_WAVE3:

		if (misionTimer.ReadSec() > WAVES_W8_TIME) {
			State = M_WAVE4;
			misionTimer.Start();
		}
		if (GetEnemyUnits() <= aliveTroopWave + TROOPS_WAVE3 && misionTimer.ReadSec() > WAVES_W8_TIME_DEADUNITS) 
		{
			State = M_WAVE4;
			misionTimer.Start();
		}

		break;

	case M_WAVE4:

		if (misionTimer.ReadSec() > WAVES_W8_TIME) {
			State = M_BOSSWAVE;
			misionTimer.Start();
		}
		if (GetEnemyUnits() <= aliveTroopWave + M_WAVE4 && misionTimer.ReadSec() > WAVES_W8_TIME_DEADUNITS)
		{
			State = M_BOSSWAVE;
			misionTimer.Start();
		}

		break;

	case M_BOSSWAVE:

		if (bossIsAlive == false)
		{
			State = M_VICTORY;
		}
		if (townCenterIsAlive == false) {
			State = M_DEFEAT;
		}

		break;

	case M_VICTORY:
		//TODO i dont know whats is going next  maybe return to menu
		break;

	case M_DEFEAT:
		//TODO i dont know whats is going next maybe return to menu
		break;
	}
	return update_status(ret);
}

uint M_MisionManager::GetEnemyUnits()
{
	return enemyTroopCounter;
}

void M_MisionManager::AddStartUnit()
{
	enemyStartTroops++;
}

void M_MisionManager::TheBossIsDead()
{
	bossIsAlive = false;
}

void M_MisionManager::TheTownCenterIsDead()
{
	townCenterIsAlive = false;
}
