#ifndef __M_RESOURCES_H__
#define __M_RESOURCES_H__

#include "Module.h"

#define UPDATE_TIMER 5
#define MAX_RESOURCES 2000

#define REPAIR_AMOUNT 10
#define MAX_VILLAGERS 20

class M_Resources : public Module
{
public:
	M_Resources(bool startEnabled = true);
	virtual ~M_Resources();

	bool Start();

	update_status Update(float dt)override;

	void DrawDebug()override;

private:
	Timer updateResources;

	//Resources
	uint foodAmount;
	uint woodAmount;
	uint goldAmount;

public:
	uint GetCurrentGold();
	uint GetCurrentWood();
	uint GetCurrentFood();

	void AddFood(int amount);
	void AddWood(int amount);
	void AddGold(int amount);

	void SubstractFood(int amount);
	void SubstractWood(int amount);
	void SubstractGold(int amount);

	void SetCurrentFood(int amount);
	void SetCurrentWood(int amount);
	void SetCurrentGold(int amount);

	void SetCurrentResources(int amount);

private:
	uint miners;
	uint lumberjacks;
	uint farmers;

	uint constructors;
	uint totalVillagers;
	uint unemployedVillagers;

	uint totalUnits;

public:
	uint GetConstructors();
	uint GetVillagers();
	uint GetTotalVillagers();

	uint GetMiners();
	uint GetLumberjacks();
	uint GetFarmers();
	uint GetTotalUnits();

	void AddConstructors();
	void AddVillager();
	void AddMiners();
	void AddLumberjack();
	void AddFarmers();

	void RemoveConstructors();
	void RemoveMiners();
	void RemoveLumberjacks();
	void RemoveFarmers();

	void GuiEvent(GUIElement* element, int64_t event);

};

#endif // __M_RESOURCES_H__