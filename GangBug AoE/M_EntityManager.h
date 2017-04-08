#ifndef __M_ENTITY_MANAGER_H__
#define __M_ENTITY_MANAGER_H__

#include "Module.h"
#include <vector>
#include "Unit.h"

class Entity;
class SDL_Texture;
enum building_type;

class GB_QuadTree;

class M_EntityManager : public Module
{
public:
	M_EntityManager(bool startEnabled = true);
	virtual ~M_EntityManager();

	bool Awake(pugi::xml_node&)override;
	bool Start()override;
	update_status PreUpdate(float dt)override;
	update_status Update(float dt)override;
	update_status PostUpdate(float dt)override;
	bool CleanUp()override;


	Entity* CreateEntity(entity_type type, Entity* parent);
	Entity* CreateEntity(Entity* parent = nullptr, int posX = 0, int posY = 0, int rectX = 1, int rectY = 1);
	Entity* CreateUnit(unit_type type = DEFAULT_UNIT, Entity* parent = nullptr, int posX = 0, int posY = 0, int rectX = 1, int rectY = 1);

	Entity* CreateBuilding(building_type buldType, Entity* parent, uint posx, uint posy);

	Entity* GetSceneRoot()const;
	Entity* FindEntity(); //TODO: Used if UID are in use

	void GetEntitiesOnRect(uint type, std::vector<Entity*>& vec, GB_Rectangle<int> rectangle);

	void Draw(std::vector<Entity*>& entitiesToDraw, GB_Rectangle<int> camToTest);
	void DrawDebug()override;

	void InsertEntityToTree(Entity* et);
	void EraseEntityFromTree(Entity* et);

	void LoadScene();
	void SaveScene();

	//-------------------------------
	Entity* CreateRandomTestEntity();

private:
	void RemoveFlagged();
	bool SaveSceneNow();
	bool LoadSceneNow();

	void RecColectEntitiesToDraw(std::vector<Entity*>& entitiesToDraw, Entity* et);

public:
	bool showQauds = false;
	bool culling = true;
	GB_QuadTree* sceneTree = nullptr;

	double lastFrameDrawDuration = 0;

private:
	Entity* root = nullptr;
	bool mustSaveScene = false;
	bool mustLoadScene = false;

	//TMP
	Entity* et = nullptr;
	Entity* et2 = nullptr;
	Entity* archer = nullptr;
	SDL_Texture* textTexture = nullptr;
};

#endif // !__M_ENTITY_MANAGER_H__