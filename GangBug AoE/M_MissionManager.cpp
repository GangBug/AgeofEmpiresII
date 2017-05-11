#include "M_MissionManager.h"
#include "Log.h"
#include "M_EnemyWaves.h"
#include "M_DialogueManager.h"
#include "App.h"
#include "S_InGame.h"
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
		if (townCenterIsAlive == false)
		{
			app->dialogueManager->PlayDialogue(D_EVENT_DEFEAT);
			stateName.assign("NEXT TIME WILL BE B ETTER");
			State = M_DEFEAT;
			app->inGame->GoToMenu();
		}

		switch (State)
		{
		case M_INTRO:
		app->dialogueManager->PlayDialogue(D_EVENT_FIRST_MISSION);
			if (misionTimer.ReadSec() > MISION_TIME && app->dialogueManager->onDialogue == false)
			{
				misionTimer.Start();
				State = M_TOWNATTACK;
				stateName.assign("Save the town");
			}
			break;

		case M_TOWNATTACK:
			if (enemyDeadUnits >= TROOPS_ONTOWN && app->dialogueManager->onDialogue == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_TOWN_REPAIR);
				enemyDeadUnits = 0;
				State = M_TOWNREPAIR;
				misionTimer.Start();
				stateName.assign("Repair the town");
			}

			break;

		case M_TOWNREPAIR:			
			if (misionTimer.ReadSec() > TOWNREPAIR_TIME && app->dialogueManager->onDialogue == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_WAVES_START);
				State = M_WAVES;
				misionTimer.Start();
				stateName.assign("Defend the town! Waves incoming.");
				app->enemyWaves->createPortals();

			}

			break;

		case M_WAVES:

			if (enemyDeadUnits >= ENEMIES_TO_DEFEAT_WAVES || app->enemyWaves->checkActivePortals() == 0)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_DIABLO_SPAWN);
				State = M_BOSS;
				stateName.assign("Last fight! Defeat Diablo!");
			}			

		case M_BOSS:

			if (bossIsAlive == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_VICTORY);
				stateName.assign("CONGRATS!  YOU WIN!");
				State = M_VICTORY;
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
