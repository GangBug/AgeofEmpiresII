#include "M_MissionManager.h"
#include "Log.h"
#include "M_EnemyWaves.h"
#include "M_DialogueManager.h"
#include "App.h"
#include "S_InGame.h"
#include "M_EntityManager.h"
#include "M_Audio.h"
#include "M_Metrics.h"
#include "M_Window.h"
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
	enemyDeadUnits = 0;

	bossIsAlive = false;
	townCenterIsAlive = true;
	active = false;
	isHardModeActive = false;

	stateName.assign("Intro");

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
				Unit* tmpBoss = (Unit*)app->entityManager->CreateUnit(DIABLO, nullptr, -144, 1300);
				tmpBoss->GoTo({ -2221, 2524 });
			}		

			if (townCenterIsAlive == false)
			{
				app->dialogueManager->PlayDialogue(D_EVENT_DEFEAT);
				stateName.assign("NEXT TIME WILL BE B ETTER");
				State = M_DEFEAT;
			//	app->inGame->GoToMenu();
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
			app->inGame->GoToScore();

			}
			break;

		case M_DEFEAT:
			if (app->dialogueManager->onDialogue == false)
			{
				app->inGame->GoToScore();

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

void M_MissionManager::SetState(Mission_State _newstate)
{
	State = _newstate;
}

bool M_MissionManager::getHardModeStatus()
{
	return isHardModeActive;
}

void M_MissionManager::setGameToHardMode(bool status)
{
	isHardModeActive = status;
}

bool M_MissionManager::Save(pugi::xml_node& node) const
{
	/*pugi::xml_node entitiesNode = node.append_child("entities");

	pugi::xml_node buildings = entitiesNode.append_child("buildings");

	for()*/

	pugi::xml_node missionNode = node.append_child("mission");

	pugi::xml_node saveNode = missionNode.append_child("currentState");
	
	switch (State)
	{
	case M_INTRO: saveNode.append_attribute("status") = "Intro"; break;
	case M_TOWNATTACK: saveNode.append_attribute("status") = "Town Attack"; break;
	case M_TOWNREPAIR: saveNode.append_attribute("status") = "Town Repair"; break;
	case M_WAVES: saveNode.append_attribute("status") = "Waves"; break;
	case M_BOSS: saveNode.append_attribute("status") = "Boss"; break;
	}

	saveNode.append_attribute("enemyDeadUnits") = enemyDeadUnits;
	saveNode.append_attribute("missionTimer") = misionTimer.ReadSec();
	saveNode.append_attribute("bossStatus") = bossIsAlive;
	saveNode.append_attribute("townStatus") = townCenterIsAlive;
	saveNode.append_attribute("difficulty") = isHardModeActive;

	return true;
}

bool M_MissionManager::Load(pugi::xml_node& node)
{
	pugi::xml_node missionNode = node.child("mission");

	pugi::xml_node loadNode = missionNode.child("currentState");

	std::string status = loadNode.attribute("status").as_string();
	if (strcmp(status.c_str(), "Intro") == 0)
	{
		State = M_INTRO;
	}
	else if (strcmp(status.c_str(), "Town Attack") == 0)
	{
		State = M_TOWNATTACK;
	}
	else if (strcmp(status.c_str(), "Town Repair") == 0)
	{
		State = M_TOWNREPAIR;
	}
	else if (strcmp(status.c_str(), "Waves") == 0)
	{
		State = M_WAVES;
	}
	else if (strcmp(status.c_str(), "Boss") == 0)
	{
		State = M_BOSS;
	}

	enemyDeadUnits = loadNode.attribute("enemyDeadUnits").as_uint();
	misionTimer.SetSec(loadNode.attribute("missionTimer").as_float());
	bossIsAlive = loadNode.attribute("bossStatus").as_bool();
	townCenterIsAlive = loadNode.attribute("townStatus").as_bool();
	isHardModeActive = loadNode.attribute("difficulty").as_bool();

	return true;
}