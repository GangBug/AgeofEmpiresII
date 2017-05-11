#include "App.h"

#include "M_EnemyWaves.h"
#include "M_EntityManager.h"
#include "S_InGame.h"
#include "M_Audio.h"
#include "Log.h"
#include "M_MisionManager.h"

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

	//TODO: Use IsStopped instead of spawnTimerStarted
	if (vilesSpawn > 0)
	{
		if (spawnTimerStarted == false && vilesSpawned != vilesSpawn)
		{
			spawnTimer.Start();
			spawnTimerStarted = true;
		}
		if (spawnTimer.ReadSec() > 1)
		{
			SpawnWave(VILE, spawnPosition.x, spawnPosition.y, nullptr);
			vilesSpawned++;
			spawnTimer.Stop();
			spawnTimerStarted = false;
		}
		if (vilesSpawned == vilesSpawn)
		{
			vilesSpawned = 0;
			vilesSpawn = 0;
			waveSpawn = true;
		}
	}

	if (diablosSpawn > 0)
	{
		if (spawnTimerStarted == false)
		{
			spawnTimer.Start();
			spawnTimerStarted = true;
		}
		if (spawnTimer.ReadSec() > 1)
		{
			SpawnWave(DIABLO, spawnPosition.x, spawnPosition.y, nullptr);
			diablosSpawned++;
			spawnTimer.Stop();
			spawnTimerStarted = false;
		}
		if (diablosSpawned == diablosSpawn)
		{
			diablosSpawn = 0;
			waveSpawn = true;
		}
	}

	//Working now
	if (waveSpawn == true)
	{
		app->audio->PlayFx(app->entityManager->fxAlert01);
	
		for (std::vector<Entity*>::iterator it = waveEntities.begin(); it != waveEntities.end(); ++it)
		{
			if (dynamic_cast<Unit*>(*it)->GetType() == VILE)
			{
				dynamic_cast<Unit*>(*it)->GoTo({ app->inGame->enemyDestination });
			}
		}
		waveSpawn = false;
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
