#ifndef __M_ENEMYWAVES_H__
#define __M_ENEMYWAVES_H__

#include "Module.h"

#include "j1Timer.h"
#include "Entity.h"

#include <vector>

//Maybe we could use one wave timer and then multiply it for the wave we need it
#define FIRSTWAVETIMER 10

class Entity;

class M_EnemyWaves : public Module
{
public:
	M_EnemyWaves(bool startEnabled = true);

	virtual ~M_EnemyWaves();

	bool Start();

	update_status Update(float dt)override;

	void DrawDebug()override;

private:
	Timer waveTimer;

	bool firstWave;

public:
	uint GetCurrentWaveTimer();
	void ResetWaveTimer();

	void SpawnWave(int vileAmount, int diabloAmount);

	std::vector<Entity*> waveEntities;
};

#endif // __M_ENEMYWAVES_H__
