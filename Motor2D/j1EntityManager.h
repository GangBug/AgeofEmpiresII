#ifndef _j1EntityManager_
#define _j1EntityManager_

#include "j1Module.h"
#include "j1Textures.h"
#include "p2Point.h"
#include "Entity.h"
#include "Units.h"
#include "Buildings.h"
#include "SDL/include/SDL_rect.h"

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Awake();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	Entity* CreateUnit(UNIT_TYPE u_type, fPoint pos);
	Entity* CreateBuilding(BUILDING_TYPE, fPoint pos);

	void SelectInQuad(const SDL_Rect& select_rect);
	void SelectInClick(int x, int y);
	void UnselectEverything();

	void DeleteEntity(Entity* ptr); // will call other delete methods
	void DeleteUnit(Entity* ptr);
	void DeleteBuilding(Entity* ptr);

	std::list<Entity*> entity_list;
	bool IsUnitInTile(const Unit* unit, iPoint tile)const;
	Unit* GetUnitInTile(iPoint tile) const;

	bool archerySelected = false;
	bool barracksSelected = false;
	bool stableSelected = false;

private:
	int unitID = 0;
	int buildingID = 0;
};
#endif //_j1EntityManager_

