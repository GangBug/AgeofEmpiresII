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


#define WAVES_W8_TIME 30
#define WAVES_W8_TIME_DEADUNITS 30

#define TOWNREPAIR_TIME 100

#define TROOPS_ONTOWN 28

#define TROOPS_WAVE1 5
#define TROOPS_WAVE2 5
#define TROOPS_WAVE3 5
#define TROOPS_WAVE4 10

#define SPAWNPOINT_1_X  -2650
#define SPAWNPOINT_1_Y  1200

#define SPAWNPOINT_2_X  -2700
#define SPAWNPOINT_2_Y  3000

#define SPAWNPOINT_3_X  -150
#define SPAWNPOINT_3_Y  -50

#define SPAWNPOINT_4_X  3500
#define SPAWNPOINT_4_Y  2300

#define ENEMYOBJECTIVE_X -2800
#define ENEMYOBJECTIVE_Y 2300

class M_MisionManager : public Module
{
public:
	M_MisionManager(bool startEnabled = true);
	virtual ~M_MisionManager();

	bool Start();
	update_status Update(float dt)override;
	void DrawDebug()override{}
	bool CleanUp() override;


	uint GetEnemyUnits()const;
	std::string GetStateName()const;
	Timer GetMisionTime()const;
	float GetMisionTimeleftf()const;
	bool GetBossState()const;
	uint GetEnemyDeadUnits()const;

	void AddStartUnit();
	void AddEnemyDeadUnit();
	void SetBossState(bool state);
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
	std::string stateName;

	uint enemyDeadUnits;


};


#endif 