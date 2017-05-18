#include "App.h"

#include "M_EnemyWaves.h"
#include "M_EntityManager.h"
#include "S_InGame.h"
#include "Building.h"
#include "M_Map.h"
#include "M_Audio.h"
#include "Log.h"
#include "M_MissionManager.h"

M_EnemyWaves::M_EnemyWaves(bool startEnabled) : Module(startEnabled)
{
	name.assign("enemy_waves");
}

M_EnemyWaves::~M_EnemyWaves()
{
}

bool M_EnemyWaves::Start()
{
	LOG("EnemyWaves: Start.");
	bool ret = true;

	waveTimer.Start();

	vilesSpawn, vilesSpawned = 0;
	diablosSpawn, diablosSpawned = 0;

	spawnPosition = { 0, 0 };

	waveSpawn = false;
	secondWave = false;
	spawnTimerStarted = false;

	return ret;
}

update_status M_EnemyWaves::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (!app->pause)
	{
		//TODO: Use IsStopped instead of spawnTimerStarted
		if (waveTimer.ReadSec() > 10)
		{
			app->entityManager->CreateBuilding(BUILD_PORTAL, iPoint(8, 57), nullptr, 192, 2590);

			waveSpawn = true;
			ResetWaveTimer();
		}

		if (waveSpawn == true && waveTimer.ReadSec() > 5)
		{
			checkCurrentPortals();
			ResetWaveTimer();
		}

		//Working now
		if (waveTimer.ReadSec() > 5)
		{
			for (std::vector<Entity*>::iterator it = waveEntities.begin(); it != waveEntities.end(); ++it)
			{
				if (dynamic_cast<Unit*>(*it)->GetType() == VILE || dynamic_cast<Unit*>(*it)->GetType() == HELL_WITCH)
				{
					dynamic_cast<Unit*>(*it)->GoTo(iPoint(-2221, 2524));
				}
			}
			waveEntities.clear();
		}
	}
	return ret;
}

uint M_EnemyWaves::GetCurrentWaveTimer()
{
	return waveTimer.ReadSec();
}

void M_EnemyWaves::ResetWaveTimer()
{
	waveTimer.Stop();
	waveTimer.Start();
}

void M_EnemyWaves::SpawnWave(unit_type type, int posX, int posY, Entity* entityParent)
{
	waveEntities.push_back(app->entityManager->CreateUnit(type, entityParent, posX, posY));
}

void M_EnemyWaves::SpawnEnemies(int vileSpawn, int diabloSpawn, int pos1X, int pos1Y)
{
	vilesSpawn += vileSpawn;
	diablosSpawn = diabloSpawn;

	spawnPosition.x = pos1X;
	spawnPosition.y = pos1Y;
}

void M_EnemyWaves::DrawDebug()
{
}

bool M_EnemyWaves::CleanUp()
{
	waveEntities.clear();
	return true;
}

void M_EnemyWaves::checkCurrentPortals()
{
	std::vector<Entity*> buildingVec = app->entityManager->GetBuildingVector();

	for (std::vector<Entity*>::iterator it = buildingVec.begin(); it != buildingVec.end(); it++)
	{
		if ((*it)->GetHP() > 0 &&  dynamic_cast<Building*>(*it)->buildType == BUILD_PORTAL)
		{
			iPoint spawnPos = app->map->MapToWorld(dynamic_cast<Building*>(*it)->tileAttack.x, dynamic_cast<Building*>(*it)->tileAttack.y);
			if (fxCount < 1)
			{
				app->audio->PlayFx(app->entityManager->fxPortalCast01);
				fxCount++;
			}
			
			int randNum = rand() % 2;
			if (randNum == 0)
			{
				SpawnWave(VILE, spawnPos.x, spawnPos.y, nullptr);
			}
			else
			{
				SpawnWave(HELL_WITCH, spawnPos.x, spawnPos.y, nullptr);
			}
		}
	}
	fxCount = 0;
}

void M_EnemyWaves::activatePortals()
{
	std::vector<Entity*> buildingVec = app->entityManager->GetBuildingVector();

	for (std::vector<Entity*>::iterator it = buildingVec.begin(); it != buildingVec.end(); it++)
	{
		if ((*it)->GetHP() == 0 && dynamic_cast<Building*>(*it)->buildType == BUILD_PORTAL)
		{
			dynamic_cast<Building*>(*it)->HP = 100;
		}
	}
}

int M_EnemyWaves::checkActivePortals()
{
	int ret = 0;
	std::vector<Entity*> buildingVec = app->entityManager->GetBuildingVector();

	for (std::vector<Entity*>::iterator it = buildingVec.begin(); it != buildingVec.end(); it++)
	{
		if ((*it)->GetHP() > 0 && dynamic_cast<Building*>(*it)->buildType == BUILD_PORTAL)
		{
			ret++;
		}
	}

	return ret;
}