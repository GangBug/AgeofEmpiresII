#include "M_MisionManager.h"
#include "Log.h"
#include "M_EnemyWaves.h"
#include "App.h"
M_MisionManager::M_MisionManager(bool startEnabled) : Module(startEnabled)
{

	name.assign("misionManager");
	enemyStartTroops = 0;
	enemyTroopCounter = 0;
	aliveTroopWave = 0;
	bossIsAlive = false;
	townCenterIsAlive = true;
	stateName.assign("Intro");
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
			stateName.assign("Attack the town");
		}
		//WAARNING
		State = M_WAVE1;
		break;

	case M_TOWNATTACK:

		if (GetEnemyUnits() <= enemyStartTroops + TROOPS_ONTOWN) 
		{
			State = M_TOWNREPAIR;
			misionTimer.Start();
			stateName.assign("Repair the town");
		}

		break;

	case M_TOWNREPAIR:

		if (misionTimer.ReadSec() > TOWNREPAIR_TIME) 
		{
			State = M_WAVE1;
			misionTimer.Start();
			app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_1_X, SPAWNPOINT_1_Y);	
			stateName.assign("Defend the town! Wave 1");
		}

		break;

	case M_WAVE1:

		if (misionTimer.ReadSec() > WAVES_W8_TIME)
		{
			State = M_WAVE2;
			misionTimer.Start();
			app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_2_X, SPAWNPOINT_2_Y);
			stateName.assign("Defend the town! Wave 2");
		}

		if (GetEnemyUnits() <= aliveTroopWave + TROOPS_WAVE1 && misionTimer.ReadSec() > WAVES_W8_TIME_DEADUNITS) 
		{
			State = M_WAVE2;
			misionTimer.Start();
			app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_2_X, SPAWNPOINT_2_Y);
			stateName.assign("Defend the town! Wave 2");
		}

		break;

	case M_WAVE2:

		if (misionTimer.ReadSec() > WAVES_W8_TIME)
		{
			State = M_WAVE3;
			misionTimer.Start();
			app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_3_X, SPAWNPOINT_3_Y);
			stateName.assign("Defend the town! Wave 3");
		}

		if (GetEnemyUnits() <= aliveTroopWave + TROOPS_WAVE2 && misionTimer.ReadSec() > WAVES_W8_TIME_DEADUNITS) 
		{
			State = M_WAVE3;
			misionTimer.Start();
			app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_3_X, SPAWNPOINT_3_Y);
			stateName.assign("Defend the town! Wave 3");
		}

		break;

	case M_WAVE3:

		if (misionTimer.ReadSec() > WAVES_W8_TIME) 
		{
			State = M_WAVE4;
			misionTimer.Start();
			app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_1_X, SPAWNPOINT_1_Y);
			app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_2_X, SPAWNPOINT_2_Y);
			stateName.assign("Defend the town! Wave 4");
		}

		if (GetEnemyUnits() <= aliveTroopWave + TROOPS_WAVE3 && misionTimer.ReadSec() > WAVES_W8_TIME_DEADUNITS) 
		{
			State = M_WAVE4;
			misionTimer.Start();
			app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_1_X, SPAWNPOINT_1_Y);
			app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_2_X, SPAWNPOINT_2_Y);
			stateName.assign("Defend the town! Wave 4");
		}

		break;

	case M_WAVE4:

		if (misionTimer.ReadSec() > WAVES_W8_TIME) 
		{
			// ----------- // ---------- //  //TODO add spawn boss
			State = M_BOSSWAVE;
			misionTimer.Start();
			stateName.assign("Defend the town! The diablo is comming!");
		}

		if (GetEnemyUnits() <= aliveTroopWave + M_WAVE4 && misionTimer.ReadSec() > WAVES_W8_TIME_DEADUNITS)
		{
			// ----------- // ---------- //  //TODO add spawn boss
			State = M_BOSSWAVE;
			misionTimer.Start();
			stateName.assign("Defend the town! The diablo is comming!");
		}
		

		break;

	case M_BOSSWAVE:

		if (bossIsAlive == false)
		{
			stateName.assign("CONGRATS!  YOU WIN!");
			State = M_VICTORY;
		}

		if (townCenterIsAlive == false) {
			stateName.assign("NEXT TIME WILL BE BETTER");
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

uint M_MisionManager::GetEnemyUnits()const
{
	return enemyTroopCounter;
}

std::string M_MisionManager::GetStateName() const
{
	return std::string(stateName);
}

Timer M_MisionManager::GetMisionTime() const
{
	return Timer(misionTimer);
}

void M_MisionManager::AddStartUnit()
{
	enemyStartTroops++;
}

bool M_MisionManager::GetBossState()
{
	return bossIsAlive;
}

void M_MisionManager::SetBossState(bool state)
{
	bossIsAlive = state;
}

void M_MisionManager::TheTownCenterIsDead()
{
	townCenterIsAlive = false;
}
