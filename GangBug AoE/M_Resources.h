#ifndef __M_RESOURCES_H__
#define __M_RESOURCES_H__

#include "Module.h"

#define UPDATE_TIMER 20
#define MAX_RESOURCES 2000

#define FOOD_AMOUNT 10
#define WOOD_AMOUNT 10
#define GOLD_AMOUNT 10
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
	uint food;
	uint wood;
	uint gold;

	Timer updateResources;

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

	uint totalVillagers;
	uint unemployedVillagers;

public:
	uint GetVillagers();
	uint GetTotalVillagers();
	uint GetMiners();
	uint GetLumberjacks();
	uint GetFarmers();


	void AddVillager();
	void AddMiners();
	void AddLumberjack();
	void AddFarmers();

	void RemoveMiners();
	void RemoveLumberjacks();
	void RemoveFarmers();
};

#endif // __M_RESOURCES_H__