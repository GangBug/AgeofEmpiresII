#ifndef __M_RESOURCES_H__
#define __M_RESOURCES_H__

#include "Module.h"

#define UPDATE_TIMER 2
#define MAX_RESOURCES 600

#define VILLAGERS_REDUCTION 0.2

#define REPAIR_AMOUNT 10
#define MAX_VILLAGERS 10

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

	//Total Resources
	uint foodAmount;
	uint woodAmount;
	uint goldAmount;

	//Resource +
	uint food;
	uint wood;
	uint gold; 

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

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

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