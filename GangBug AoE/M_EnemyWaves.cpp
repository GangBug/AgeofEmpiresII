#include "App.h"

#include "M_EnemyWaves.h"
#include "M_EntityManager.h"
#include "Unit.h"

#include "Log.h"


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

	firstWave = false;

	return ret;
}

update_status M_EnemyWaves::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (waveTimer.ReadSec() > FIRSTWAVETIMER && firstWave == false)
	{
		SpawnWave(5, 2);

		firstWave = true;
	}

	//TODO: Not working
	if (firstWave == true)
	{
		for (std::vector<Entity*>::iterator it = waveEntities.begin(); it != waveEntities.end(); ++it)
		{
			if (dynamic_cast<Unit*>(*it)->GetType() == VILE)
			{
				dynamic_cast<Unit*>(*it)->GoTo(iPoint(200, 250));
			}
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


void M_EnemyWaves::SpawnWave(int vileAmount, int diabloAmount)
{
	for (int i = 0; i < vileAmount; i++)
	{
		app->entityManager->CreateUnit(VILE, nullptr, 300, 200);
	}
}

void M_EnemyWaves::DrawDebug()
{
}
