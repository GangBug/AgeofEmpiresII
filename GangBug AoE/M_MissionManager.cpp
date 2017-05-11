#include "M_MissionManager.h"
#include "Log.h"
#include "M_EnemyWaves.h"
#include "M_DialogueManager.h"
#include "App.h"
M_MissionManager::M_MissionManager(bool startEnabled) : Module(startEnabled)
{

	name.assign("missionManager");	
}

M_MissionManager::~M_MissionManager()
{
}

bool M_MissionManager::Start()
{
	LOG("Resources: Start.");
	bool ret = true;

	enemyStartTroops = 0;
	enemyTroopCounter = 0;
	aliveTroopWave = 0;
	bossIsAlive = false;
	townCenterIsAlive = true;
	stateName.assign("Intro");
	enemyDeadUnits = 0;
	active = false;

	State = M_INTRO;
	

	return ret;
}

update_status M_MissionManager::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (active)
	{
		switch (State)
		{
		case M_INTRO:
		app->dialogueManager->PlayDialogue(D_EVENT_FIRST_MISSION);
			if (misionTimer.ReadSec() > MISION_TIME && app->dialogueManager->onDialogue == false)
			{
				misionTimer.Start();
				State = M_TOWNATTACK;
				stateName.assign("Attack the town");
			}
			break;

		case M_TOWNATTACK:
			if (enemyDeadUnits >= TROOPS_ONTOWN && app->dialogueManager->onDialogue == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_TOWN_REPAIR);
				State = M_TOWNREPAIR;
				misionTimer.Start();
				stateName.assign("Repair the town");
			}

			break;

		case M_TOWNREPAIR:
			
			if (misionTimer.ReadSec() > TOWNREPAIR_TIME && app->dialogueManager->onDialogue == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_WAVES_START);
				State = M_WAVE1;
				misionTimer.Start();
				stateName.assign("Defend the town! Wave 1");
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_1_X, SPAWNPOINT_1_Y);

			}

			break;

		case M_WAVE1:

			if (misionTimer.ReadSec() > WAVES_W8_TIME)
			{
				State = M_WAVE2;
				misionTimer.Start();
				stateName.assign("Defend the town! Wave 2");
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_2_X, SPAWNPOINT_2_Y);

			}

			if (enemyDeadUnits >= TROOPS_ONTOWN + TROOPS_WAVE1 && misionTimer.ReadSec() > WAVES_W8_TIME_DEADUNITS)
			{
				State = M_WAVE2;
				misionTimer.Start();
				stateName.assign("Defend the town! Wave 2");
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_2_X, SPAWNPOINT_2_Y);
			}

			break;

		case M_WAVE2:

			if (misionTimer.ReadSec() > WAVES_W8_TIME)
			{
				State = M_WAVE3;
				misionTimer.Start();
				stateName.assign("Defend the town! Wave 3");
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_3_X, SPAWNPOINT_3_Y);

			}

			if (enemyDeadUnits >= TROOPS_ONTOWN + TROOPS_WAVE1 + TROOPS_WAVE2 && misionTimer.ReadSec() > WAVES_W8_TIME_DEADUNITS)
			{
				State = M_WAVE3;
				misionTimer.Start();
				stateName.assign("Defend the town! Wave 3");
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_3_X, SPAWNPOINT_3_Y);
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

			if (enemyDeadUnits >= TROOPS_ONTOWN + TROOPS_WAVE1 + TROOPS_WAVE2 + TROOPS_WAVE3 && misionTimer.ReadSec() > WAVES_W8_TIME_DEADUNITS)
			{
				State = M_WAVE4;
				misionTimer.Start();
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_1_X, SPAWNPOINT_1_Y);
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_2_X, SPAWNPOINT_2_Y);
				stateName.assign("Defend the town! Wave 4");
			}

			break;

		case M_WAVE4:
			app->dialogueManager->PlayDialogue(D_EVENT_DIABLO_SPAWN);
			if (misionTimer.ReadSec() > WAVES_W8_TIME && app->dialogueManager->onDialogue == false)
			{
				// ----------- // ---------- //  //TODO add spawn boss
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_1_X, SPAWNPOINT_1_Y);
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_2_X, SPAWNPOINT_2_Y);
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_3_X, SPAWNPOINT_3_Y);
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_4_X, SPAWNPOINT_4_Y);

				app->enemyWaves->SpawnEnemies(0, 1, SPAWNPOINT_4_X, SPAWNPOINT_4_Y);

				State = M_BOSSWAVE;
				misionTimer.Start();
				stateName.assign("Defend the town! The diablo is comming!");
			}

			if (enemyDeadUnits >= TROOPS_ONTOWN + TROOPS_WAVE1 + TROOPS_WAVE2 + TROOPS_WAVE3 + TROOPS_WAVE4 && misionTimer.ReadSec() > WAVES_W8_TIME_DEADUNITS && app->dialogueManager->onDialogue == false)
			{
				// ----------- // ---------- //  //TODO add spawn boss
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_1_X, SPAWNPOINT_1_Y);
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_2_X, SPAWNPOINT_2_Y);
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_3_X, SPAWNPOINT_3_Y);
				app->enemyWaves->SpawnEnemies(5, 0, SPAWNPOINT_4_X, SPAWNPOINT_4_Y);

				app->enemyWaves->SpawnEnemies(0, 1, SPAWNPOINT_4_X, SPAWNPOINT_4_Y);
				State = M_BOSSWAVE;
				misionTimer.Start();
				stateName.assign("Defend the town! The diablo is comming!");
			}
			break;

		case M_BOSSWAVE:

			if (bossIsAlive == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_VICTORY);
				stateName.assign("CONGRATS!  YOU WIN!");
				State = M_VICTORY;
			}

			if (townCenterIsAlive == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_DEFEAT);
				stateName.assign("NEXT TIME WILL BE B ETTER");
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
	}
	return update_status(ret);
}

bool M_MissionManager::CleanUp()
{
	bool ret = true;
	State = M_INTRO;
	enemyStartTroops = 0;
	enemyTroopCounter = 0;
	aliveTroopWave = 0;
	bossIsAlive = false;
	townCenterIsAlive = true;
	stateName.assign("Intro");
	enemyDeadUnits = 0;


	return ret;
}

uint M_MissionManager::GetEnemyUnits()const
{
	return enemyTroopCounter;
}

std::string M_MissionManager::GetStateName() const
{
	return std::string(stateName);
}

Timer M_MissionManager::GetMisionTime() const
{
	return Timer(misionTimer);
}

float M_MissionManager::GetMisionTimeleftf() const
{
	return float(misionTimer.ReadSec());
}

void M_MissionManager::AddStartUnit()
{
	enemyStartTroops++;
}

void M_MissionManager::AddEnemyDeadUnit()
{
	enemyDeadUnits++;
}

bool M_MissionManager::GetBossState()const
{
	return bossIsAlive;
}

uint M_MissionManager::GetEnemyDeadUnits() const
{
	return uint(enemyDeadUnits);
}

void M_MissionManager::SetBossState(bool state)
{
	bossIsAlive = state;
}

void M_MissionManager::TheTownCenterIsDead()
{
	townCenterIsAlive = false;
}
