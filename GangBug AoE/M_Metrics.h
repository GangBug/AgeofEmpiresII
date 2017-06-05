#ifndef __M_METRICS_H__
#define __M_METRICS_H__

#include <vector>
#include "Module.h"
#include "j1Timer.h"
#include "GUIElement.h"
#define METRICS_REFRESH 5

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

#define PERCENTAGEOFCHARTX 50
#define PERCENTAGEOFCHARTY 50
#define NUMBEROFDTCHART 10
#define NUMBEROFSCORELBLCHART 10

#define CHARTSIZEX 400
#define CHARTSIZEY 150

class M_Metrics : public Module
{
public:
	//constructor destructor
	M_Metrics(bool startEnabled = true);
	virtual ~M_Metrics();
	bool CleanUp()override;
	//override
	bool Start();
	update_status Update(float dt)override;
	void DrawDebug()override;

	// create score chart
	void CreateChart(int PosX, int PosY);

	//calculate the score;
	float GetScore();
	void SetInitialUnits(uint _initial);
	void StartMetricsTime();
	int getmetrics();

	//-----add
	//--totals
	void AddTotalUnit();
	// add resources
	void AddTotalGold(uint);
	void AddTotalWood(uint);
	void AddTotalFood(uint);
	void AddTotalRepaired(uint);
	void SetTotalTime();

	void AddDeadViles();
	void AddDeadWitch();
	void AddBoss();

	void AddArcherAlive();
	void AddTarkanAlive();
	void AddSamuraiAlive();
	void AddVillagersAlive();
	void KillArcherAlive();
	void KillTarkanAlive();
	void KillSamuraiAlive();

	uint GetTotalGold();
	uint GetTotalWood();
	uint GetTotalFood();
	uint GetTotalResources();
	uint GetFinalScore();
	uint GetTotalUnits();
	uint GetTotalkills();
	uint GetTotalVilesKilled();
	uint GetTotalWitchKilled();

	uint GetArchersAlive();
	uint GetTarkanAlive();
	uint GetSamuraisAlive();
	uint GetTotalUnitsAlive();
	float GetTotalTime() { return totalTime; }
	
private:

	//global
	uint totalUnits;
	uint totalResources;
	float totalTime;

	uint totalGold;
	uint totalWood;
	uint totalFood;
	uint totalRepaired;

	//in time
	uint deadUnits;
	uint deadEnemies;
	uint unitsAlive;
	uint deadBoss;
	uint villagers;
	
	uint samuraiAlive;
	uint archersAlive;
	uint tarkanAlive;

	//units
	uint deadViles;
	uint deadWitch;

	//score
	float score;
	float finalScore;
	std::vector<float> timeScore;

	//timing
	int metricsCounter;
	Timer MetricTimeCycle;//20 sec to reestart;
	Timer MetricTimeCounter;// Global counter

public:
	std::vector<uint> timechart;
	std::vector<uint> ScoreChart;
	std::list<GUIElement*> ChartList;

	void chartvisible(bool =true);
};
#endif //__M_METRICS_H__