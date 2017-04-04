#ifndef _j1EntityManager_
#define _j1EntityManager_

#include "j1Module.h"
#include "j1Textures.h"
#include "p2Point.h"
#include "Entity.h"
#include "Units.h"
#include "Buildings.h"
#include "Object.h"
#include "SDL/include/SDL_rect.h"
#include <vector>

struct ObjectTexture
{
	ObjectTexture(OBJECT_TYPE b, SDL_Rect rect);
	~ObjectTexture() {}

	OBJECT_TYPE type;
	SDL_Rect section;

	void SetType(pugi::xml_node node);
	void SetRect(pugi::xml_node node);
};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool LoadObjects();

	Entity* CreateUnit(UNIT_TYPE u_type, fPoint pos);
	Entity* CreateBuilding(BUILDING_TYPE, fPoint pos);
	Entity* CreateBoss(fPoint pos);
	Entity* CreateObject(OBJECT_TYPE, fPoint pos);

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

	SDL_Rect getObjectRect(OBJECT_TYPE bType);

private:
	int unitID = 0;
	int buildingID = 0;
	int objectID = 0;
	std::vector<ObjectTexture> objectTextures;
};
#endif //_j1EntityManager_

