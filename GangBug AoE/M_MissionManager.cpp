#include "M_MissionManager.h"
#include "Log.h"
#include "M_EnemyWaves.h"
#include "M_DialogueManager.h"
#include "App.h"
#include "S_InGame.h"
#include "M_EntityManager.h"
#include "M_Audio.h"
#include "M_Metrics.h"

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
		app->audio->PlayTheme(app->audio->firstMission);
			if (misionTimer.ReadSec() > MISION_TIME && app->dialogueManager->onDialogue == false)
			{			
				misionTimer.Start();
				State = M_TOWNATTACK;
				stateName.assign("Save the town"); 
			}
			break;

		case M_TOWNATTACK:
			
			if (bossIsAlive == false && app->dialogueManager->onDialogue == false)
			{
				app->audio->PlayTheme(app->audio->secondMission);
				app->dialogueManager->PlayDialogue(D_EVENT_FIRST_MISSION_FINISH);
				
				enemyDeadUnits = 0;
				bossIsAlive = true;
				State = M_TOWNREPAIR;
				misionTimer.Start();
				stateName.assign("Repair the town");
			}
			break;

		case M_TOWNREPAIR:	
			if (app->dialogueManager->onDialogue == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_TOWN_REPAIR);
			}
			if (misionTimer.ReadSec() > TOWNREPAIR_TIME && app->dialogueManager->onDialogue == false)
			{
				app->audio->PlayTheme(app->audio->thirdMission);
				app->dialogueManager->PlayDialogue(D_EVENT_WAVES_START);
				State = M_WAVES;
				misionTimer.Start();
				stateName.assign("Defend the town! Waves incoming.");
				app->enemyWaves->activatePortals();
				enemyDeadUnits = 0;
			}

			break;

		case M_WAVES:

			if (enemyDeadUnits >= ENEMIES_TO_DEFEAT_WAVES || app->enemyWaves->checkActivePortals() == 0)
			{
				app->audio->PlayTheme(app->audio->finalMission);
				app->dialogueManager->PlayDialogue(D_EVENT_DIABLO_SPAWN_SAMURAI);
				State = M_BOSS;
				stateName.assign("Last fight! Defeat Diablo!");
				bossIsAlive = true;
				app->entityManager->CreateUnit(DIABLO, nullptr, -144, 1300);
			}		

			if (townCenterIsAlive == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_DEFEAT);
				stateName.assign("NEXT TIME WILL BE B ETTER");
				State = M_DEFEAT;
				app->inGame->GoToMenu();
			}

			break;

		case M_BOSS:
			if (app->dialogueManager->onDialogue == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_DIABLO_SPAWN_DIABLO);
			}
			if (bossIsAlive == false && app->dialogueManager->onDialogue == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_VICTORY_DIABLO);

				app->metrics->deadBoss = true;

				stateName.assign("CONGRATS! YOU WIN!");
				State = M_VICTORY;
			}

			if (townCenterIsAlive == false && app->dialogueManager->onDialogue == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_DEFEAT);
				stateName.assign("NEXT TIME WILL BE BETTER");
				State = M_DEFEAT;
			}

			break;

		case M_VICTORY:
			if (app->dialogueManager->onDialogue == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_VICTORY_SAMURAI);
			}
			if (app->dialogueManager->onDialogue == false)
			{
				app->inGame->GoToMenu();
			}
			break;

		case M_DEFEAT:
			if (app->dialogueManager->onDialogue == false)
			{
				app->inGame->GoToMenu();
			}
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

Mission_State M_MissionManager::GetState() const
{
	return Mission_State(State);
}

bool M_MissionManager::getHardModeStatus()
{
	return isHardModeActive;
}

void M_MissionManager::setGameToEasyMode()
{
	isHardModeActive = false;
}

void M_MissionManager::setGameToHardMode()
{
	isHardModeActive = true;
}