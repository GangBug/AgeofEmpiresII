#include "M_Metrics.h"
#include "M_Resources.h"
#include "App.h"
#include "M_MissionManager.h"
#include "p2Point.h"
#include "GUIImage.h"
#include "M_GUI.h"

M_Metrics::M_Metrics(bool startEnabled) : Module(startEnabled)
{
	name.assign("Metrics");

	totalUnits = 0;
	unitsAlive = 0;
	samuraiAlive = 0;
	archersAlive = 0;
	tarkanAlive = 0;

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
	finalScore = 0;
	metricsCounter = 0;
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

		if (app->missionManager->GetState() != M_INTRO || app->missionManager->GetState() != M_DEFEAT || app->missionManager->GetState() != M_VICTORY || app->missionManager->GetState() != M_STANDBY) 
		{

			if (MetricTimeCycle.ReadSec() > METRICS_REFRESH) 
			{
				metricsCounter++;

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

		}
	}
	return update_status(ret);
}

void M_Metrics::DrawDebug()
{
}

void M_Metrics::CreateChart(int PosX, int PosY)
{
	uint size = timeScore.size();
	float imgsizex = 0;
	float imgsizey = 0;

	finalScore = timeScore[0];
	imgsizex = CHARTSIZEX / size;

	//window
	GUIImage* img;
	img = app->gui->CreateImage({ (int)(PosX-(CHARTSIZEX*0.1)),(int)(PosY - (CHARTSIZEY*0.1)) ,(int)(CHARTSIZEX+ (CHARTSIZEX*0.2)),(int)(CHARTSIZEY+ CHARTSIZEY*0.2) }, { 600, 100, 20, 20 },"window");
	img->SetVisible(true);
	img->SetInteractive(true);
	app->gui->background.push_back(img);


	//barras
	for (int i = 0, tmp; i < (size); i++) {
		tmp = timeScore[i];
		imgsizey = (CHARTSIZEY / finalScore)*tmp;

		img = app->gui->CreateImage({ PosX + (int)imgsizex *i,PosY ,(int)imgsizex,(int)imgsizey }, { 15, 530, 20, 20 }, std::to_string(i));
	
		img->SetVisible(true);
		img->SetInteractive(true);
		app->gui->background.push_back(img);

	}

	SetTotalTime();

}

float M_Metrics::GetScore()
{
	float ret = 0;
	float military = 0, economy = 0, society = 0;

	military += (deadViles*VILEPRICE) + (deadWitch*WICHPRICE) + (deadBoss*DAEMONPRICE);

	economy += samuraiAlive*SAMURAIPRICE + archersAlive*ARCHERPRICE + tarkanAlive*TARKANPRICE + villagers*VILLAGERPRICE;

	society += totalRepaired;

	ret = (military*MILITARPERCENTAGE) + (economy*ECONOMYPERCENTAGE) + society*(SOCIETYPERCENTAGE);

	return float(ret);
}

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

void M_Metrics::AddBoss()
{
	deadBoss++;
}

void M_Metrics::AddTotalGold(uint add)
{
	totalGold += add;
}

void M_Metrics::AddTotalWood(uint add)
{
	totalWood += add;
}

void M_Metrics::AddTotalFood(uint add)
{
	totalFood += add;
}

void M_Metrics::AddTotalRepaired(uint add)
{
	totalRepaired += add;
}

void M_Metrics::SetTotalTime()
{
	totalTime = MetricTimeCounter.ReadSec();
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

int M_Metrics::getmetrics()
{
	return metricsCounter;
}

void M_Metrics::AddArcherAlive()
{
	archersAlive++;
}

void M_Metrics::AddTarkanAlive()
{
	tarkanAlive++;
}

void M_Metrics::AddSamuraiAlive()
{
	samuraiAlive++;
}

void M_Metrics::AddVillagersAlive()
{
	villagers++;
}

void M_Metrics::KillArcherAlive()
{
	archersAlive--;
}

void M_Metrics::KillTarkanAlive()
{
	tarkanAlive--;
}

void M_Metrics::KillSamuraiAlive()
{
	samuraiAlive--;
}

uint M_Metrics::GetTotalGold()
{
	return uint(totalGold);
}

uint M_Metrics::GetTotalWood()
{
	return uint(totalWood);
}

uint M_Metrics::GetTotalFood()
{
	return uint(totalFood);
}

uint M_Metrics::GetTotalResources()
{
	return uint(totalResources);
}

uint M_Metrics::GetFinalScore()
{
	return uint(finalScore);
}

uint M_Metrics::GetTotalUnits()
{
	return uint(totalUnits);
}

uint M_Metrics::GetTotalkills()
{
	return uint(deadUnits);
}

uint M_Metrics::GetTotalVilesKilled()
{
	return uint(deadViles);
}

uint M_Metrics::GetTotalWitchKilled()
{
	return uint(deadWitch);
}

uint M_Metrics::GetArchersAlive()
{
	return uint(archersAlive);
}

uint M_Metrics::GetTarkanAlive()
{
	return uint(tarkanAlive);
}

uint M_Metrics::GetSamuraisAlive()
{
	return uint(samuraiAlive);
}

uint M_Metrics::GetTotalUnitsAlive()
{
	return uint(tarkanAlive+samuraiAlive+archersAlive);
}
