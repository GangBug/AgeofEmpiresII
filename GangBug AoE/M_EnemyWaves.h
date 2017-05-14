#ifndef __M_ENEMYWAVES_H__
#define __M_ENEMYWAVES_H__

#include "Module.h"

#include "j1Timer.h"
#include "Entity.h"
#include "Unit.h"

#include <vector>

class Entity;

class M_EnemyWaves : public Module
{
public:
	M_EnemyWaves(bool startEnabled = true);

	virtual ~M_EnemyWaves();

	bool Start();

	update_status Update(float dt)override;

	void DrawDebug()override;

	bool CleanUp();

private:
	Timer waveTimer;
	Timer spawnTimer;

	bool waveSpawn;
	bool secondWave;
	bool spawnTimerStarted;

	iPoint spawnPosition;

	int fxCount;

public:
	int vilesSpawn, vilesSpawned;
	int diablosSpawn, diablosSpawned;

	uint GetCurrentWaveTimer();
	void ResetWaveTimer();

	void SpawnWave(unit_type type, int posX, int posY, Entity* entityParent);
	void SpawnEnemies(int vileSpawn, int diabloSpawn, int posX, int posY);

	void checkCurrentPortals();

	void activatePortals();

	int checkActivePortals();

	std::vector<Entity*> waveEntities;
};

#endif // __M_ENEMYWAVES_H__
