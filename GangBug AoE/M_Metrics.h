#ifndef __M_METRICS_H__
#define __M_METRICS_H__

#include <vector>
#include "Module.h"
#include "j1Timer.h"

#define METRICS_REFRESH 20

#define VILEPRICE 45
#define WICHPRICE 70
#define DAEMONPRICE 2000

#define SAMURAIPRICE 60+30
#define ARCHERPRICE  25+45
#define TARKANPRICE 60+75
#define VILLAGERPRICE 50

#define MILITARPERCENTAGE 20
#define ECONOMYPERCENTAGE 10
#define SOCIETYPERCENTAGE 20


class M_Metrics : public Module
{
public:

	M_Metrics(bool startEnabled = true);
	virtual ~M_Metrics();

	bool Start();
	update_status Update(float dt)override;
	void DrawDebug()override;

	//print the metrics
	void ShowMetrics();
	
	// create score chart
	//void CreateChart(iPoint Pos);

	//add dead units
	void AddTotalUnit();
	void AddDeadViles();
	void AddDeadWitch();
	// add resources
	void AddTotalGold();
	void AddTotalWood();
	void AddTotalFood();

	//calculate the score;
	float GetScore();

	void SetInitialUnits(uint _initial);
	void StartMetricsTime();
	bool deadBoss = false;

private:

	uint totalUnits;
	uint deadUnits;

	uint deadEnemies;
	uint deadViles;
	uint deadWitch;

	uint unitsAlive;

	uint samuraiAlive;
	uint archersAlive;
	uint tarkanAlive;

	float score;

	uint totalResources;
	uint totalGold;
	uint totalWood;
	uint totalFood;
	uint totalRepaired;

	uint totalTime;
	Timer MetricTimeCycle;//20 sec to reestart;
	Timer MetricTimeCounter;// Global counter
	
	std::vector<float> timeScore;

	uint villagers;

};


#endif //__M_METRICS_H__