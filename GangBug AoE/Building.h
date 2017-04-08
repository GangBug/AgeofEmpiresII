#pragma once
#include "Entity.h"
#include "Unit.h"
#include "j1Timer.h"

enum building_type
{
	BUILD_ARCHERY,
	BUILD_STABLES,
	BUILD_BARRACK
};

class Building :
	public Entity
{
public:
	Building(building_type buildType, Entity* parent);
	virtual ~Building();

	void OnUpdate(float dt)override;

	bool OnSave(pugi::xml_node& node)const override;
	bool OnLoad(pugi::xml_node* node)override;

	void BuyUnit();


public:
	Timer buyTimer;

	enum building_type buildType;
	enum unit_type unitType;
	bool selected = false;

	int unitCost;
};

