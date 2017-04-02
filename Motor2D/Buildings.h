#ifndef _BUILDINGS
#define _BUILDINGS

#include "Entity.h"
#include "j1Timer.h"

enum BUILDING_TYPE
{
	NO_BUILDING = 0,
	ARCHERY,
	BARRACK,
	STABLE
};

class Building : public Entity
{
public:
	Building(BUILDING_TYPE type, fPoint pos, int id);
	~Building();

	void Update();
	void PostUpdate();
	void Draw();

	void GenerateUnit(int num);

	enum BUILDING_TYPE buildingType;
	int id;
	int unitsToAdd;
	int buildingRadius;
	int buildingWidth;
	int buildingHeight;
	j1Timer timer;
};
#endif