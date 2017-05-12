#include "M_Metrics.h"
#include "M_Resources.h"
#include "App.h"
#include "M_MissionManager.h"
#include "p2Point.h"

M_Metrics::M_Metrics(bool startEnabled) : Module(startEnabled)
{
	name.assign("Metrics");

	totalUnits = 15;
	deadUnits = 0;
	deadEnemies = 0;
	deadViles = 0;
	deadWitch = 0;

	score = 0.0f;
	
	totalResources = 0;

	totalGold = 0;
	totalWood = 0;
	totalFood = 0;
	totalTime = 0;

}

M_Metrics::~M_Metrics()
{
}

bool M_Metrics::Start()
{
	bool ret = true;
	
	return ret;
}

update_status M_Metrics::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (active)
	{

		if (app->missionManager->GetState() != M_INTRO || app->missionManager->GetState() != M_DEFEAT || app->missionManager->GetState() != M_VICTORY || app->missionManager->GetState() != M_STANDBY) {

			if (MetricTimeCycle.ReadSec() > METRICS_REFRESH){

				//units
				deadUnits = app->missionManager->GetEnemyDeadUnits();

				unitsAlive = app->resources->GetTotalUnits();


				//resources
				totalResources = totalGold + totalWood + totalFood;

				//score
				timeScore.push_back(this->GetScore());

				//timer
				MetricTimeCycle.Start();
			}
				totalTime = MetricTimeCounter.ReadSec();
		}
	}
	return update_status(ret);
}

void M_Metrics::DrawDebug()
{
}

void M_Metrics::ShowMetrics()
{
}

/*void M_Metrics::CreateChart(iPoint Pos)
{
}*/

void M_Metrics::AddTotalUnit()
{
	totalUnits++;
}

void M_Metrics::AddDeadViles()
{
	deadWitch++;
}

void M_Metrics::AddDeadWitch()
{
	deadWitch++;
}

void M_Metrics::AddTotalGold()
{
	totalGold++;
}

void M_Metrics::AddTotalWood()
{
	totalWood++;
}

void M_Metrics::AddTotalFood()
{
	totalFood++;
}

float M_Metrics::GetScore()
{
	float ret = 0;
	float military=0, economy =0 ,society=0;

	military += (deadViles*VILEPRICE)+ (deadWitch*WICHPRICE);
	if (deadBoss)
		military += DAEMONPRICE;

	economy += samuraiAlive*SAMURAIPRICE + archersAlive*ARCHERPRICE  + tarkanAlive*TARKANPRICE + villagers*VILLAGERPRICE;

	society += totalRepaired;


	ret = (military*MILITARPERCENTAGE) + (economy*ECONOMYPERCENTAGE) + society*(SOCIETYPERCENTAGE);

	return float(ret);
}

void M_Metrics::SetInitialUnits(uint _initial)
{
	totalUnits = _initial;
}

void M_Metrics::StartMetricsTime()
{
	MetricTimeCounter.Start();
	MetricTimeCycle.Start();
}
