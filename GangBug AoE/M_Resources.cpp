#include "App.h"
#include "M_Resources.h"
#include "M_DialogueManager.h"
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

	food = 1000;
	wood = 1000;
	gold = 1000;

	miners = 5;
	lumberjacks = 3;
	farmers = 4;
	constructors = 0;
	totalVillagers = 12;
	unemployedVillagers = 0;


	return ret;
}

//Get current amount of each resource
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

//Add certain amount to existent resources
void M_Resources::AddFood(int amount)
{
	food += amount;
}
void M_Resources::AddWood(int amount)
{
	wood += amount;
}
void M_Resources::AddGold(int amount)
{
	gold += amount;
}

//Substracts certain amount to existent resources
void M_Resources::SubstractFood(int amount)
{
	food -= amount;
}
void M_Resources::SubstractWood(int amount)
{
	wood -= amount;
}
void M_Resources::SubstractGold(int amount)
{
	gold -= amount;
}

//Set existent resources to a certain amount
void M_Resources::SetCurrentFood(int amount)
{
	food = amount;
}
void M_Resources::SetCurrentWood(int amount)
{
	wood = amount;
}
void M_Resources::SetCurrentGold(int amount)
{
	gold = amount;
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
	if (!app->dialogueManager->onDialogue)
	{
		//Resources update
		if (updateResources.ReadSec() > UPDATE_TIMER)
		{
			if (GetCurrentFood() < MAX_RESOURCES)
			{
				AddFood(FOOD_AMOUNT*farmers);
			}
			if (GetCurrentWood() < MAX_RESOURCES)
			{
				AddWood(WOOD_AMOUNT*lumberjacks);
			}
			if (GetCurrentGold() < MAX_RESOURCES)
			{
				AddGold(GOLD_AMOUNT*miners);
			}
			updateResources.Start();
		}
	}
	return ret;
}

void M_Resources::DrawDebug()
{
}

uint M_Resources::GetConstructors()
{
	return uint(constructors);
}

uint M_Resources::GetVillagers()
{
	return uint(unemployedVillagers);
}

uint M_Resources::GetTotalVillagers()
{
	return uint(totalVillagers);
}

uint M_Resources::GetMiners()
{
	return uint(miners);
}

uint M_Resources::GetLumberjacks()
{
	return uint(lumberjacks);
}

uint M_Resources::GetFarmers()
{
	return uint(farmers);
}

void M_Resources::AddConstructors()
{
	if (unemployedVillagers > 0) {
		unemployedVillagers--;
		constructors++;
	}
}

void M_Resources::AddVillager()
{
	if (MAX_VILLAGERS >= totalVillagers) {
		totalVillagers++;
		unemployedVillagers++;
	}
}

void M_Resources::AddMiners()
{
	if (unemployedVillagers > 0) {
		unemployedVillagers--;
		miners++;
	}
}

void M_Resources::AddLumberjack()
{
	if (unemployedVillagers > 0) {
		unemployedVillagers--;
		lumberjacks++;
	}
}

void M_Resources::AddFarmers()
{
	if (unemployedVillagers > 0) {
		unemployedVillagers--;
		farmers++;
	}
}

void M_Resources::RemoveConstructors()
{
	if (constructors > 0) {
		constructors--;
		unemployedVillagers++;
	}
}

void M_Resources::RemoveMiners()
{
	if (miners > 0) {
		miners--;
		unemployedVillagers++;
	}

}

void M_Resources::RemoveLumberjacks()
{
	if (lumberjacks > 0) {
		lumberjacks--;
		unemployedVillagers++;
	}
}

void M_Resources::RemoveFarmers()
{
	if (farmers > 0) {
		farmers--;
		unemployedVillagers++;
	}
}

void M_Resources::GuiEvent(GUIElement * element, int64_t event)
{

	if (event & MOUSE_LCLICK_UP)
	{
		if (event & ADD_MINER)
		{
			AddMiners();
		}
		if (event & ADD_CONSTRUCTOR)
		{
			AddConstructors();
		}
		if (event & ADD_LUMBERJACK)
		{
			AddLumberjack();
		}
		if (event & ADD_FARMER)
		{
			AddFarmers();
		}

		if (event & REMOVE_MINER)
		{
			RemoveMiners();
		}
		if (event & REMOVE_CONSTRUCTOR)
		{
			RemoveConstructors();
		}
		if (event & REMOVE_LUMBERJACK)
		{
			RemoveLumberjacks();
		}
		if (event & REMOVE_FARMER)
		{
			RemoveFarmers();
		}
	
	}

}
