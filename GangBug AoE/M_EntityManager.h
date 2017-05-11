#ifndef __M_ENTITY_MANAGER_H__
#define __M_ENTITY_MANAGER_H__

#include "Module.h"
#include <vector>
#include "Unit.h"
#include "Object.h"
#include <list>

class Entity;
class SDL_Texture;
enum building_type;
class Boss;

class GB_QuadTree;



struct ObjectTexture
{
	ObjectTexture(object_type b, SDL_Rect rect);
	~ObjectTexture() {}

	object_type type;
	SDL_Rect section;

	void SetType(pugi::xml_node node);
	void SetRect(pugi::xml_node node);
};



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
	Entity* CreateBuilding(building_type buldType, iPoint tileAttack, Entity* parent, int posx, int posy);
	Entity* CreateObject(object_type type = OBJECT_NONE, Entity* parent = nullptr, int posX = 0, int posY = 0, int rectX = 1, int rectY = 1);

	bool DeleteUnit(Entity* toDelete);


	Entity* GetSceneRoot()const;
	Entity* FindEntity(); //TODO: Used if UID are in use
	std::vector<Entity*> GetUnitVector();
	std::vector<Entity*> GetBuildingVector();

	void GetEntitiesOnRect(uint type, std::vector<Entity*>& vec, GB_Rectangle<int> rectangle);

	void Draw(std::vector<Entity*>& entitiesToDraw, GB_Rectangle<int> camToTest);
	void DrawDebug()override;

	void InsertEntityToTree(Entity* et);
	void EraseEntityFromTree(Entity* et);

	void LoadScene();
	void SaveScene();

	//-------------------------------
	Entity* CreateRandomTestEntity();

	//
	bool IsUnitInTile(const Unit* unit, iPoint tile)const;

public: // ------------------------------- Objects
	SDL_Rect getObjectRect(object_type bType);
	bool LoadObjects();
	bool PlaceObjects();
	void CreateForest(pugi::xml_node &, int x, int y);
	void Setforest(pugi::xml_node node, int radious = 0);

	//----Adri
	Unit* GetBoss();
public:

	void LoadFXs();// ADD FX

	//fight  

	uint fxFight001;
	uint fxFight002;
	uint fxFight003;
	uint fxFight004;
	uint fxFight005;
	uint fxFight006;
	uint fxFight007;
	uint fxFight008;
	uint fxArrow001;
	uint fxArrow002;
	uint fxArrow003;
	uint fxArrow004;
	uint fxArrow005;
	uint fxArrow006;
	uint fxArrow007;

	//die
	uint fxDieSoldier001;
	uint fxDieSoldier002;
	uint fxDieSoldier003;
	uint fxDieSoldier004;
	uint fxDieSoldier005;
	uint fxDieSoldier006;
	uint fxDieHorse001;
	uint fxDieHorse002;
	uint fxDieHorse003;

	//select
	uint fxHorseSelect001;
	uint fxHorseSelect002;
	uint fxHorseSelect003;
	uint fxUnitSelect001;
	uint fxUnitSelect002;
	uint fxUnitSelect003;
	uint fxUnitSelect004;

	//create
	uint fxCreateUnit;
	uint fxCreateVillager;
	uint fxLimitVillager;

	//buldings
	uint fxStableSelection;
	uint fxBarrackSelection;
	uint fxArcherySelection;
	uint fxTownCenterSelection;

	//ambient

	uint fxBirds001;
	uint fxBirds002;
	uint fxBirds003;
	uint fxBirds004;
	uint fxBirds005;
	uint fxBirds006;
	uint fxBirds007;
	uint fxBirds008;
	uint fxBirds009;

	//alert
	uint fxAlert01;
	uint fxAlert02;

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

	// Objects
	std::vector<ObjectTexture> objectTextures;
	std::vector<Entity*> unitVector;
	std::vector<Entity*> buildingVector;
	//TMP
	Entity* et = nullptr;
	Entity* et2 = nullptr;
	Entity* archer = nullptr;
	SDL_Texture* textTexture = nullptr;

	public: //carlos

		void SerializeObjects();
		std::list<Entity*> Objectslist;
};

#endif // !__M_ENTITY_MANAGER_H__