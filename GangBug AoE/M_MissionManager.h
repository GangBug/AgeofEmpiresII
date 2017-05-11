#ifndef _M_MISION_MANAGER_
#define _M_MISION_MANAGER_

#include "j1Timer.h"
#include "Module.h"

enum Mission_State 
{
	M_INTRO,
	M_TOWNATTACK,
	M_TOWNREPAIR,
	M_WAVES,
	M_BOSS,
	M_VICTORY,
	M_DEFEAT,
	M_STANDBY
};
#define MISION_TIME 1

#define WAVES_W8_TIME 30
#define WAVES_W8_TIME_DEADUNITS 30

#define TOWNREPAIR_TIME 100

#define TROOPS_ONTOWN 28

#define ENEMIES_TO_DEFEAT_WAVES 100

#define ENEMYOBJECTIVE_X -2800
#define ENEMYOBJECTIVE_Y 2300

class M_MissionManager : public Module
{
public:
	M_MissionManager(bool startEnabled = true);
	virtual ~M_MissionManager();

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
	Mission_State State;

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