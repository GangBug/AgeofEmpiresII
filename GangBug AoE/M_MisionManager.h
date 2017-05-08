#ifndef _M_MISION_MANAGER_
#define _M_MISION_MANAGER_

#include "j1Timer.h"
#include "Module.h"

enum Mision_State {
	M_INTRO,
	M_TOWNATTACK,
	M_TOWNREPAIR,
	M_WAVE1,
	M_WAVE2,
	M_WAVE3,
	M_WAVE4,
	M_WAVE5,
	M_BOSSWAVE,
	M_VICTORY,
	M_DEFEAT,
	M_STANDBY
};
#define MISION_TIME 1


#define WAVES_W8_TIME 200
#define WAVES_W8_TIME_DEADUNITS 50

#define TOWNREPAIR_TIME 100

#define TROOPS_ONTOWN 25

#define TROOPS_WAVE1 10
#define TROOPS_WAVE2 20
#define TROOPS_WAVE3 30
#define TROOPS_WAVE4 40

class M_MisionManager : public Module
{
public:
	M_MisionManager(bool startEnabled = true);
	virtual ~M_MisionManager();

	bool Start();

	update_status Update(float dt)override;
	void DrawDebug()override{}

	uint GetEnemyUnits();
	void AddStartUnit();
	void TheBossIsDead();
	void TheTownCenterIsDead();
private:
	Timer misionTimer;
	Mision_State State;

private://troops states

	uint enemyTroopCounter;
	uint enemyStartTroops;
	uint aliveTroopWave;
	bool bossIsAlive;
	bool townCenterIsAlive;
};


#endif 