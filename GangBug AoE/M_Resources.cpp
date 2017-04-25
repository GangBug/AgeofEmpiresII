#include "App.h"
#include "M_Resources.h"

#include "j1Timer.h"
#include "Log.h"

M_Resources::M_Resources(bool startEnabled) : Module(startEnabled)
{
	name.assign("resources");
}

M_Resources::~M_Resources()
{
}

bool M_Resources::Start()
{
	LOG("Resources: Start.");
	bool ret = true;

	food = 0;
	wood = 0;
	gold = 0;

	return ret;
}

uint M_Resources::GetCurrentFood()
{
	return food;
}

uint M_Resources::GetCurrentWood()
{
	return wood;
}

uint M_Resources::GetCurrentGold()
{
	return gold;
}

void M_Resources::SetCurrentFood(int amount)
{
	food += amount;
}

void M_Resources::SetCurrentWood(int amount)
{
	wood += amount;
}

void M_Resources::SetCurrentGold(int amount)
{
	gold += amount;
}

void M_Resources::SetCurrentResources(int amount)
{
	food += amount;
	wood += amount;
	gold += amount;
}

update_status M_Resources::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	//Maybe we could use dt instead of this timer
	updateResources.Start();

	//Resources update
	if (updateResources.ReadSec() > UPDATE_TIMER)
	{
		if (GetCurrentFood() < MAX_RESOURCES)
		{
			SetCurrentFood(FOOD_AMOUNT);
		}
		if (GetCurrentWood() < MAX_RESOURCES)
		{
			SetCurrentWood(WOOD_AMOUNT);
		}
		if (GetCurrentGold() < MAX_RESOURCES)
		{
			SetCurrentGold(GOLD_AMOUNT);
		}
		updateResources.Stop();
	}

	return ret;
}

void M_Resources::DrawDebug()
{
}

