#include "M_EntityManager.h"
#include "Entity.h"

#include "App.h"
#include "Log.h"

#include "M_FileSystem.h"

#include "GB_QuadTree.h"
// Entities
#include "EntityUi.h"
#include "PlayerManager.h"
#include "EntityMap.h"
#include "Building.h"
//

// Modules
#include "M_Audio.h"


//

// -------------- to delete after serialize
#include "M_Map.h"
#include "M_Pathfinding.h"
// -------------- to delete after serialize

#include <iostream> 
#include <sstream> 

//TMP
#include "M_Input.h"
#include "SDL\include\SDL.h"
#include "M_Textures.h"
#include "M_Render.h"
#include <time.h>

/**
	Contructor: Creates the module that manages all entities.

	Parameters: Boolean. Module creates enabled or not.
*/
M_EntityManager::M_EntityManager(bool startEnabled) : Module(startEnabled)
{
	LOG("Entity manager: Creation.");
	name.assign("entity_manager");
	srand(time(nullptr));//TMP
}

/**
	Destructor
*/
M_EntityManager::~M_EntityManager()
{
	LOG("Entity manager: Destroying.");
}

/**
	Awake: Called before the first frame. Creates the root entity of the scene.

	Return:
		-True if succes.
		-False if could not create the root.

	Parameters: xml_node with the config file section.
*/
bool M_EntityManager::Awake(pugi::xml_node&)
{
	LOG("Entity manager: Awake.");
	bool ret = true;

	sceneTree = new GB_QuadTree();
	sceneTree->SetRoot(GB_Rectangle<int>(-2000, -2000, 4000, 4000)); //TODO/NOTE: Position and sizze of this starting rectangle must be changed depending on the map size.

	root = new Entity(ENTITY_BASE, nullptr);
	if (!root)
	{
		ret = false;
		LOG("ERROR: Could not create scene root entity.");
	}
	else
	{
		root->SetName("root_entity");
	}

	return ret;
}

/**
	Start: Called before first frame if module is active. Sets the base scene.

	Return:
		-True if succes.
		-False if any error.

	Parameters: Void.
*/
bool M_EntityManager::Start()
{
	LOG("Entity manager: Start.");
	bool ret = true;
	
	
	LoadFXs();

	//TMP
	textTexture = app->tex->Load("textures/test.png");

	CreateEntity(ENTITY_PLAYER_MAN, nullptr);

	CreateEntity(ENTITY_MAP, nullptr);
	//et = CreateEntity(nullptr, 300, 100);
	//et->SetTexture(textTexture);
	//et2 = CreateEntity(et, 50, 50);
	//et2->SetTexture(textTexture, GB_Rectangle<int>(0, 0, 100, 100));
	//archer = CreateUnit(CAVALRY_ARCHER, nullptr, 1000, 300);


	//if (app->map->Load("0.1Map.tmx") == true)
	//{
		//int w, h;
		//uchar* data = NULL;
		//if (app->map->CreateWalkabilityMap(w, h, &data))
		//	app->pathfinding->SetMap(w, h, data);

		//RELEASE_ARRAY(data);
	//}

	//root->Start();

	return ret;
}

/**
	PreUpdate: Called every frame. Does logic calcs on all entities.

	Return: 
		-UPDATE_CONTINUE if all is correct.
		-UPDATE_STOP if the engine has to stop correctly.
		-UPDATE_ERROR if there's any error.

		Parameters: Float delta time.
*/
update_status M_EntityManager::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	RemoveFlagged();

	if (root)
	{
		root->RecCalcTransform({ 0, 0 });
		root->RecCalcBox();
	}
	else
	{
		LOG("ERROR: Invalid root, is NULL.");
		ret = UPDATE_ERROR;
	}

	if (mustSaveScene && ret == UPDATE_CONTINUE)
	{
		SaveSceneNow();
		mustSaveScene = false;
	}

	if (mustLoadScene && ret == UPDATE_CONTINUE)
	{
		LoadSceneNow();
		mustLoadScene = false;
	}




	//TMP
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		int x, y;
		app->input->GetMouseScreenPosition(x, y);
		fPoint pos(x, y);
		et->SetLocalPosition(pos);
	}

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
	{
		int x, y;
		app->input->GetMouseScreenPosition(x, y);
		//et2->SetLocalPosition(pos);
		fPoint pos(x, y);
		et2->SetGlobalPosition(pos);
	}

	if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		iPoint mP;
		app->input->GetMouseScreenPosition(mP.x, mP.y);
		CreateRandomTestEntity()->SetGlobalPosition(mP.x, mP.y);
	}

	if (app->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		Entity* t = CreateEntity(ENTITY_UNIT, nullptr);
		iPoint mP;
		app->input->GetMouseMapPosition(mP.x, mP.y);
		t->SetGlobalPosition(mP.x, mP.y);
	}

	if (app->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
	{
		int x, y;
		app->input->GetMouseScreenPosition(x, y);
		Entity* et = CreateRandomTestEntity();
		fPoint pos(x, y);
		et->SetGlobalPosition(pos);
	}

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		int x, y;
		app->input->GetMouseMapPosition(x, y);
		CreateBuilding(building_type::BUILD_STABLES, nullptr, x, y);
	}

	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		SaveScene();
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		LoadScene();


	return ret;
}

/**
	Update: Called every frame. Calls all entities updates.
	
	Return:
		-UPDATE_CONTINUE if all is correct.
		-UPDATE_STOP if the engine has to stop correctly.
		-UPDATE_ERROR if there's any error.
	
	Parameters: Float delta time.
*/
update_status M_EntityManager::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (root)
	{
		//TODO: Only if play mode??
		root->Update(dt); //TODO: Test with tree??? If not drawn why update?? Add static and dynamic entities??
	}
	else
	{
		LOG("ERROR: Invalid root, is NULL.");
		ret = UPDATE_ERROR;
	}

	return ret;
}

/**
	PostUpdate: Called every frame.
	
	Return:
		-UPDATE_CONTINUE if all is correct.
		-UPDATE_STOP if the engine has to stop correctly.
		-UPDATE_ERROR if there's any error.
	
	Parameters: Float delta time.
*/
update_status M_EntityManager::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

/**
	CleanUp: Called At the end of the execution. Cleans all the scene.
	
	Return:
		-True if succes.
		-False if any error.
	
	Parameters: Void.
*/
bool M_EntityManager::CleanUp()
{
	LOG("Entity manager: CleanUp.");
	bool ret = true;

	//TODO: Autosave scene
	root->Remove();
	RemoveFlagged();
	RELEASE(root);
	RELEASE(sceneTree);

	return ret;
}

Entity* M_EntityManager::CreateEntity(entity_type type, Entity* parent)
{
	Entity* ret = nullptr;

	switch (type)
	{
	case ENTITY_BASE:
		ret = ((parent == nullptr) ? root->AddChild() : parent->AddChild());
		break;

	case ENTITY_UNIT:
		ret = new Unit(ARCHER, nullptr);
		unitVector.push_back(ret);
		break;

	case ENTITY_UI:
		ret = new EntityUi(nullptr);
		break;

	case ENTITY_PLAYER_MAN:
		ret = new PlayerManager(nullptr);
		break;

	case ENTITY_MAP:
		ret = new EntityMap(nullptr);
		break;
	case ENTITY_OBJECT:
		ret = new EntityMap(nullptr);
		break;
		//TODO: More entities cases

	default:
		LOG("ERROR: Could not create entity, invalid entity type.");
		break;
	}

	if (ret != nullptr)
	{
		if (parent != nullptr)
			ret->SetNewParent(parent);
		else
			ret->SetNewParent(root);
	}

	return ret;
}

/**
	CreateEntity: Creates a new entity and adds it to the scene.

	Return:
		-Entity pointer if succes.
		-Nullptr if failure.

	Parameters:
		-Entity* if force a parent. Root by default.
		-posX, posY to force a position. 0,0 by default.
		-rectX, rectY to force an enclosing box size. 1,1 by default.
*/
Entity* M_EntityManager::CreateEntity(Entity* parent, int posX, int posY, int rectX, int rectY)
{
	Entity* ret = nullptr; //First set to nullptr to assure we can check for errors later.

	if (parent)
		ret = parent->AddChild();
	else
		ret = root->AddChild();

	if (ret)
	{
		ret->SetLocalPosition(posX, posY);
		ret->SetEnclosingBox(posX, posY, rectX, rectY);
	}
	else
	{
		LOG("ERROR: Could not create a new entity.");
	}

	return ret;
}

//--------------------------
//TMP
Entity* M_EntityManager::CreateRandomTestEntity()
{
	Entity* ret = nullptr;

	ret = root->AddChild();

	if (ret != nullptr)
	{
		ret->SetEnclosingBoxSize(rand() % 10 + 30, rand() % 10 + 30);
	}
	else
	{
		LOG("ERROR: Could not create a new entity.");
	}

	return ret;
}

bool M_EntityManager::IsUnitInTile(const Unit* unit, iPoint tile)const
{
	for (int i = 0; i < unitVector.size(); i++)
	{
		{
			if (unitVector[i] != unit)
			{
				fPoint pos = unitVector[i]->GetGlobalPosition();
				if (tile == app->map->WorldToMap(pos.x, pos.y))
				{
					return true;
				}
			}
		}

		return false;
	}
}

//--------------------------

/**
	CreateUnit: Creates a new unit and adds it to scene.

	Return:
		-Entity pointer if succes.
		-Nullptr if failure.

	Parameters:
		-unit_type to determine the unit type.
		-Entity* if force a parent. Root by default.
		-posX, posY to force a position. 0,0 by default.
		-rectX, rectY to force an enclosing box size. 1,1 by default.
*/
Entity* M_EntityManager::CreateUnit(unit_type type, Entity* parent, int posX, int posY, int rectX, int rectY)
{
	Entity* ret = (Entity*) new Unit(type, parent);

	if (parent)
		parent->AddChild(ret);
	else
		root->AddChild(ret);

	if (ret)
	{
		ret->SetGlobalPosition(posX, posY);
		ret->SetEnclosingBox(posX, posY, rectX, rectY);
		unitVector.push_back(ret);
	}
	else
	{
		LOG("ERROR: Could not create a new unit.");
	}

	return ret;
}

Entity* M_EntityManager::CreateBuilding(building_type buildType, Entity* parent, int posx, int posy)
{
	Entity* ret = nullptr;

	if (!parent)
		parent = root;

	ret = (Entity*)new Building(buildType, parent);
	if (ret)
	{
		parent->AddChild(ret);
		ret->SetGlobalPosition(posx, posy);
	}

	return ret;
}

/**
	GetSceneRoot: Return the root entity of the scene.

	Return:
		-Constant entity.

	Parameters:
		-Void.
*/
Entity* M_EntityManager::GetSceneRoot()const
{
	return root;
}

Entity* M_EntityManager::FindEntity()
{
	return nullptr; //TODO
}

std::vector<Entity*> M_EntityManager::GetUnitVector()
{
	return unitVector;
}

void M_EntityManager::GetEntitiesOnRect(uint type, std::vector<Entity*>& vec, GB_Rectangle<int> rectangle)
{
	//TODO: Would be cool to be able to ask for several types just like layers (using bit operators | )
	if (type != ENTITY_UNKNOWN && sceneTree != nullptr)
	{
		//TODO: Change this, i dont like it at all...
		std::vector<Entity*> vec2;
		sceneTree->CollectCandidates(vec2, rectangle);
		for (std::vector<Entity*>::iterator it = vec2.begin(); it != vec2.end(); ++it)
			if (type & (*it)->type)
				vec.push_back((*it));
	}
}

/**
	Draw: Collect all enitities that must be drawn on screen by testing the camera with the quadtree and fill a vector with them.

	Return: 
		-Void.

	Parameters:
		-Entiti* vector by reference that will be filled.
*/
void M_EntityManager::Draw(std::vector<Entity*>& entitiesToDraw, GB_Rectangle<int> camToTest)
{
	if (root)
	{
		PerfTimer timer;
		if (culling == true)
		{
			sceneTree->CollectCandidates(entitiesToDraw, camToTest);
			lastFrameDrawDuration = timer.ReadMs();
			//LOG("Culling lasted: %f ms.", lastFrameDrawDuration);
		}
		else
		{
			for (std::vector<Entity*>::iterator it = root->childs.begin(); it != root->childs.end(); ++it)
			{
				RecColectEntitiesToDraw(entitiesToDraw, (*it));
			}
			lastFrameDrawDuration = timer.ReadMs();
			//LOG("Collecting all entities lasted: %f ms.", lastFrameDrawDuration);
		}
	}
	else
	{
		LOG("ERROR: Invalid root, is NULL.");
	}
}

/**
	DrawDebug: Called every frame if debug mode is enabled. Calls all active entities DrawDebug.

	Return: 
		-Void.

	Parameters:
		-Void.
*/
void M_EntityManager::DrawDebug()
{
	if (root)
	{
		//TODO: Might add a flag in order to have specific control on entities debug and module debug.
		for (std::vector<Entity*>::iterator it = root->childs.begin(); it != root->childs.end(); ++it)
		{
			if ((*it) && (*it)->IsActive())
				(*it)->DrawDebug();
		}

		if (sceneTree != nullptr)
		{
			std::vector<GB_Rectangle<int>> treeQuads;
			sceneTree->CollectTreeBoxes(treeQuads);

			for (std::vector<GB_Rectangle<int>>::iterator it = treeQuads.begin(); it != treeQuads.end(); ++it)
			{
				app->render->DrawQuad((*it).GetSDLrect(), 255, 255, 0, 255, false);
				//TODO: Would be cool move debug functions to another place in order to avoid having dependencies with renderer.
			}
		}

	}
	else
	{
		LOG("ERROR: Invalid root, is NULL.");
	}
}

/**
	InsertEntityToTree: Adds an entity to the scene quadtree.

	Return: 
		-Void.

	Parameters:
		-Entity to add.
*/
void M_EntityManager::InsertEntityToTree(Entity* et)
{
	if (et != nullptr && sceneTree != nullptr)
		sceneTree->Insert(et);
}

/**
	EraseEntityFromTree: Remove an entity to the scene quadtree.

	Return:
		-Void.

	Parameters:
		-Entity to remove.
*/
void M_EntityManager::EraseEntityFromTree(Entity* et)
{
	if (et != nullptr && sceneTree != nullptr)
		sceneTree->Erase(et);
}

/**
	LoadScene: Mark to load an scene on next frame.
*/
void M_EntityManager::LoadScene()
{
	mustLoadScene = true;
}

/**
	SaveScene: Mark to save an scene on next frame.
*/
void M_EntityManager::SaveScene()
{
	mustSaveScene = true;
}

/**
	RemoveFlagged: Iterate all marked to remove entities and deletes them. Must be called on a save moment.
*/
void M_EntityManager::RemoveFlagged()
{
	if (root)
	{
		if (root->removeFlag)
		{
			for (uint i = 0; i < root->childs.size(); ++i)
			{
				root->childs[i]->Remove();
			}
			root->removeFlag = false;
		}

		if (root->RecRemoveFlagged())
		{
			//TODO: Send event
		}
	}
	else
	{
		LOG("ERROR: Invalid root, is NULL.");
	}
}

/**
	RecColectEntitiesToDraw: Recursively iterate all entities and collects them to later draw.

	Return: 
		-Void.

	Parameters:
		-Enitiy vector by reference to fill.
		-Current entity iterating.
*/
void M_EntityManager::RecColectEntitiesToDraw(std::vector<Entity*>& entitiesToDraw, Entity* et)
{
	if (et != nullptr)
	{
		if(et->HasTexture())
			entitiesToDraw.push_back(et);

		for (std::vector<Entity*>::iterator it = et->childs.begin(); it != et->childs.end(); ++it)
		{
			RecColectEntitiesToDraw(entitiesToDraw, (*it));
		}
	}
}

/**
	LoadSceneNow: Actually load a scene in a save moment.

	Return:
		-True if succes.
		-False if failure.
*/
bool M_EntityManager::LoadSceneNow()
{
	bool ret = false;

	LOG("Loading scene.");

	char* buffer = nullptr;
	char path[256];
	sprintf_s(path, 256, "%sscene1.xml", app->fs->GetSaveDirectory());

	uint size = app->fs->Load(path, &buffer);
	
	if (size > 0 && buffer != nullptr)
	{
		LOG("Loading scene.");

		pugi::xml_document data;
		pugi::xml_node rootN;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		if (result != NULL)
		{
			rootN = data.child("root_entity");

			ret = root->Load(&rootN);

			data.reset();

			if (ret == true)
			{
				LOG("... scene loaded with succes.");
			}
			else
			{
				LOG("ERROR: ...loading scene process had an error.");
			}
		}
		else
		{
			LOG("Could not parse scene xml.");
		}
	}
	else
	{
		LOG("Could not load scene1.xml");
	}

	return ret;
}

/**
	SaveSceneNow: Actually save a scene in a save moment.

	Return:
		-True if succes.
		-False if failure.
*/
bool M_EntityManager::SaveSceneNow()
{
	bool ret = true;

	LOG("Saving scene.");

	pugi::xml_document data;
	pugi::xml_node rootN;

	rootN = data.append_child(root->GetName());

	ret = root->Save(rootN);

	if (ret == true)
	{
		std::stringstream stream;
		data.save(stream);

		app->fs->Save("scene1.xml", stream.str().c_str(), stream.str().length());
		LOG("... just saved the scene: scene1.xml");
	}
	else
	{
		LOG("Could not save the scene.");
	}

	data.reset();

	return ret;
}

//---------------------- objects

SDL_Rect M_EntityManager::getObjectRect(object_type type)
{
	for (std::vector<ObjectTexture>::iterator it = objectTextures.begin(); it != objectTextures.end(); it++)
	{
		if ((*it).type == type)
		{
			return (*it).section;
		}
	}
	return{ 0,0,0,0 };
}

Entity* M_EntityManager::CreateObject(object_type type, Entity* parent, int posX, int posY, int rectX, int rectY)
{

	Entity* ret = (Entity*) new Object(type, parent);

	if (parent)
		parent->AddChild(ret);
	else
		root->AddChild(ret);

	if (ret)
	{
		ret->SetLocalPosition(posX, posY);
		ret->SetEnclosingBox(posX, posY, rectX, rectY);
	}
	else
	{
		LOG("ERROR: Could not create a new unit.");
	}

	return ret;
}

bool M_EntityManager::DeleteUnit(Entity* toDelete)
{
	for (int i = 0; i < unitVector.size(); i++)
	{
		if (unitVector[i] == toDelete)
		{
			unitVector.erase(unitVector.begin() + i);
			return true;
		}
	}
	return false;
}

bool M_EntityManager::LoadObjects()
{
	std::string obj_folder = "objects/Objects_data.xml";	//Load Objects data from object folder
	char* buff = nullptr;
	int size = app->fs->Load(obj_folder.c_str(), &buff);
	pugi::xml_document obj_data;
	pugi::xml_parse_result result = obj_data.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading objects data: %s", result.description());
		return false;
	}

	//Loading Objects Sprites
	pugi::xml_node spriteNode = obj_data.child("TextureAtlas").first_child();
	while (spriteNode != NULL)
	{
		ObjectTexture newObject(OBJECT_NONE, {});
		newObject.SetType(spriteNode);
		newObject.SetRect(spriteNode);

		if (newObject.type != OBJECT_NONE)
		{
			objectTextures.push_back(newObject);
		}
		spriteNode = spriteNode.next_sibling();
	}

	return true;
}

bool M_EntityManager::PlaceObjects()
{
	std::string obj_folder = "objects/Objects_creator.xml";	//Load Objects data from object folder
	char* buff = nullptr;
	int size = app->fs->Load(obj_folder.c_str(), &buff);
	pugi::xml_document obj_data;
	pugi::xml_parse_result result = obj_data.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading objects data: %s", result.description());
		return false;
	}

	//Loading Objects Sprites
	pugi::xml_node objectNode = obj_data.child("Objects").first_child();
	while (objectNode != NULL)
	{
		if (objectNode.attribute("type").as_int() == 30)
			Setforest(objectNode, objectNode.attribute("r").as_int());
		else
		CreateObject(object_type(objectNode.attribute("type").as_int()),nullptr, objectNode.attribute("x").as_int(),objectNode.attribute("y").as_int(), getObjectRect(object_type(objectNode.attribute("type").as_int())).w, getObjectRect(object_type(objectNode.attribute("type").as_int())).h);
		objectNode = objectNode.next_sibling();
	}

	return true;
}



void M_EntityManager::CreateForest(pugi::xml_node  &objectNode, int x, int y) {


	int iSecret = 0;
	iSecret = rand() % 4 + 1;

	switch (iSecret)
	{
	case 1:
		CreateObject(BAMBOO1, nullptr, (objectNode.attribute("x").as_int() + (x*(app->map->data.tileWidth / 2))), objectNode.attribute("y").as_int() + y*(app->map->data.tileHeight / 2), getObjectRect(object_type(objectNode.attribute("type").as_int())).w, getObjectRect(object_type(objectNode.attribute("type").as_int())).h);
		break;

	case 2:
		CreateObject(BAMBOO2, nullptr, (objectNode.attribute("x").as_int() + (x*(app->map->data.tileWidth / 2))), objectNode.attribute("y").as_int() + y*(app->map->data.tileHeight / 2), getObjectRect(object_type(objectNode.attribute("type").as_int())).w, getObjectRect(object_type(objectNode.attribute("type").as_int())).h);
		break;

	case 3:
		CreateObject(BAMBOO3, nullptr, (objectNode.attribute("x").as_int() + (x*(app->map->data.tileWidth / 2))), objectNode.attribute("y").as_int() + y*(app->map->data.tileHeight / 2), getObjectRect(object_type(objectNode.attribute("type").as_int())).w, getObjectRect(object_type(objectNode.attribute("type").as_int())).h);
		break;

	default:
		CreateObject(BAMBOO4, nullptr, (objectNode.attribute("x").as_int() + (x*(app->map->data.tileWidth / 2))), objectNode.attribute("y").as_int() + y*(app->map->data.tileHeight / 2), getObjectRect(object_type(objectNode.attribute("type").as_int())).w, getObjectRect(object_type(objectNode.attribute("type").as_int())).h);
		break;
	}
}

void M_EntityManager::Setforest(pugi::xml_node objectNode, int radious)
{
	srand(time(NULL));

	int k = app->map->data.tileWidth;
	int k2 = app->map->data.tileHeight;

	for (int counterh = 0, counter; counterh <= radious; counterh++) {
		for (int i = 1; i <= radious; i++) {
			for (int j = 1; j < i * 2; j++)
				CreateForest(objectNode, j, j + ((counterh - 1) * 3));
		}
		/*	for (int i = 1; i <= radious; i++) {
		for (int j = 1; j < i * 2; j++)
		CreateForest(objectNode, j, j - ((counterh - 1) * 3));
		}*/
		radious--;
	}
}

void M_EntityManager::LoadFXs()// ADD FX
{
	//fight

	fxFight001 = app->audio->LoadFx("audio/fx/Units/Fight_1.wav");
	fxFight002 = app->audio->LoadFx("audio/fx/Units/Fight_2.wav");
	fxFight003 = app->audio->LoadFx("audio/fx/Units/Fight_3.wav");
	fxFight004 = app->audio->LoadFx("audio/fx/Units/Fight_4.wav");
	fxFight005 = app->audio->LoadFx("audio/fx/Units/Fight_5.wav");
	fxFight006 = app->audio->LoadFx("audio/fx/Units/Fight_6.wav");
	fxFight007 = app->audio->LoadFx("audio/fx/Units/Fight_7.wav");
	fxFight008 = app->audio->LoadFx("audio/fx/Units/Fight_8.wav");
	fxArrow001 = app->audio->LoadFx("audio/fx/Other/Arrow_1.wav");
	fxArrow002 = app->audio->LoadFx("audio/fx/Other/Arrow_2.wav");
	fxArrow003 = app->audio->LoadFx("audio/fx/Other/Arrow_3.wav");
	fxArrow004 = app->audio->LoadFx("audio/fx/Other/Arrow_4.wav");
	fxArrow005 = app->audio->LoadFx("audio/fx/Other/Arrow_5.wav");
	fxArrow006 = app->audio->LoadFx("audio/fx/Other/Arrow_6.wav");
	fxArrow007 = app->audio->LoadFx("audio/fx/Other/Arrow_7.wav");
	
	//die
	fxDieSoldier001 = app->audio->LoadFx("audio/fx/Units/Dead_1.wav");
	fxDieSoldier002 = app->audio->LoadFx("audio/fx/Units/Dead_2.wav");
	fxDieSoldier003 = app->audio->LoadFx("audio/fx/Units/Dead_3.wav");
	fxDieSoldier004 = app->audio->LoadFx("audio/fx/Units/Dead_4.wav");
	fxDieSoldier005 = app->audio->LoadFx("audio/fx/Units/Dead_5.wav");
	fxDieSoldier006 = app->audio->LoadFx("audio/fx/Units/Dead_6.wav");
	fxDieHorse001 = app->audio->LoadFx("audio/fx/Units/Dead_Horse_1.wav");
	fxDieHorse002 = app->audio->LoadFx("audio/fx/Units/Dead_Horse_2.wav");
	fxDieHorse003 = app->audio->LoadFx("audio/fx/Units/Dead_Horse_3.wav");

	//select
	fxHorseSelect001 = app->audio->LoadFx("audio/fx/Units/Horse_Select_1.wav");
	fxHorseSelect002 = app->audio->LoadFx("audio/fx/Units/Horse_Select_2.wav");
	fxHorseSelect003 = app->audio->LoadFx("audio/fx/Units/Horse_Select_3.wav");
	fxUnitSelect001 = app->audio->LoadFx("audio/fx/Units/Select_Unit_1.wav");
	fxUnitSelect002 = app->audio->LoadFx("audio/fx/Units/Select_Unit_2.wav");
	fxUnitSelect003 = app->audio->LoadFx("audio/fx/Units/Select_Unit_3.wav");
	fxUnitSelect004 = app->audio->LoadFx("audio/fx/Units/Select_Unit_4.wav");

	//create
	fxCreateUnit = app->audio->LoadFx("audio/fx/Units/Creation_Unit.wav");

	//buldings
	fxStableSelection = app->audio->LoadFx("audio/fx/Buildings/Click_Archery.wav");
	fxBarrackSelection = app->audio->LoadFx("audio/fx/Buildings/Click_Barrack.wav");
	fxArcherySelection = app->audio->LoadFx("audio/fx/Buildings/Click_Ball.wav");
	fxArcherySelection = app->audio->LoadFx("audio/fx/Buildings/Town_Center.wav");

	//ambient
	fxBirds001 = app->audio->LoadFx("audio/fx/Other/tf1.wav");
	fxBirds002 = app->audio->LoadFx("audio/fx/Other/tf2.wav");
	fxBirds003 = app->audio->LoadFx("audio/fx/Other/tf3.wav");
	fxBirds004 = app->audio->LoadFx("audio/fx/Other/tf4.wav");
	fxBirds005 = app->audio->LoadFx("audio/fx/Other/tf5.wav");
	fxBirds006 = app->audio->LoadFx("audio/fx/Other/tf6.wav");
	fxBirds007 = app->audio->LoadFx("audio/fx/Other/tf7.wav");
	fxBirds008 = app->audio->LoadFx("audio/fx/Other/tf8.wav");
	fxBirds009 = app->audio->LoadFx("audio/fx/Other/Bird.wav");

	//alert
	fxAlert01 = app->audio->LoadFx("audio/fx/Other/Warning_Attack_1.wav");
	fxAlert02 = app->audio->LoadFx("audio/fx/Other/Warning_Attack_2.wav");

}




ObjectTexture::ObjectTexture(object_type type, SDL_Rect rect)
{
	this->type = type;
	section = rect;
}

void ObjectTexture::SetRect(pugi::xml_node node)
{
	section.x = node.attribute("x").as_int();
	section.y = node.attribute("y").as_int();
	section.w = node.attribute("w").as_int();
	section.h = node.attribute("h").as_int();
}

void ObjectTexture::SetType(pugi::xml_node node)
{
	if (strcmp(node.attribute("n").as_string(), "asianWallH") == 0) {
		type = ASIAN_WALL_H;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallL") == 0) {
		type = ASIAN_WALL_L;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallR") == 0) {
		type = ASIAN_WALL_R;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallT") == 0) {
		type = ASIAN_WALL_T;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallV") == 0) {
		type = ASIAN_WALL_V;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallRdmg") == 0) {
		type = ASIAN_WALL_R_DMG;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallLdmg") == 0) {
		type = ASIAN_WALL_L_DMG;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallTdmg") == 0) {
		type = ASIAN_WALL_T_DMG;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallVdmg") == 0) {
		type = ASIAN_WALL_V_DMG;
	}
	else if (strcmp(node.attribute("n").as_string(), "Bamboo1") == 0) {
		type = BAMBOO1;
	}
	else if (strcmp(node.attribute("n").as_string(), "Bamboo2") == 0) {
		type = BAMBOO2;
	}
	else if (strcmp(node.attribute("n").as_string(), "Bamboo3") == 0) {
		type = BAMBOO3;
	}
	else if (strcmp(node.attribute("n").as_string(), "Bamboo4") == 0) {
		type = BAMBOO4;
	}
	else if (strcmp(node.attribute("n").as_string(), "BannerA") == 0) {
		type = BANNERA;
	}
	else if (strcmp(node.attribute("n").as_string(), "BannerB") == 0) {
		type = BANNERB;
	}
	else if (strcmp(node.attribute("n").as_string(), "Charriot") == 0) {
		type = CHARRIOT;
	}
	else if (strcmp(node.attribute("n").as_string(), "PalisadeR") == 0) {
		type = PALISADE_R;
	}
	else if (strcmp(node.attribute("n").as_string(), "PalisadeL") == 0) {
		type = PALISADE_L;
	}
	else if (strcmp(node.attribute("n").as_string(), "PalisadeT") == 0) {
		type = PALISADE_T;
	}
	else if (strcmp(node.attribute("n").as_string(), "PalisadeH") == 0) {
		type = PALISADE_H;
	}
	else if (strcmp(node.attribute("n").as_string(), "PalisadeV") == 0) {
		type = PALISADE_V;
	}
	else if (strcmp(node.attribute("n").as_string(), "SkeletonsGroup") == 0) {
		type = SKELETONS_GROUP;
	}
	else if (strcmp(node.attribute("n").as_string(), "SkullPile") == 0) {
		type = SKULL_PILE;
	}
	else if (strcmp(node.attribute("n").as_string(), "Bamboo") == 0) {
		type = BAMBOO;
	}
	else if (strcmp(node.attribute("n").as_string(), "TorchFloorSquare") == 0) {
		type = TORCH_FLOOR_SQUARE;
	}
	else if (strcmp(node.attribute("n").as_string(), "TorchFloorTwisted") == 0) {
		type = TORCH_FLOOR_TWISTED;
	}
	else if (strcmp(node.attribute("n").as_string(), "TrashLarge") == 0) {
		type = TRASH_LARGE;
	}
	else if (strcmp(node.attribute("n").as_string(), "TrashMedium") == 0) {
		type = TRASH_MEDIUM;
	}
	else if (strcmp(node.attribute("n").as_string(), "TrashSmall") == 0) {
		type = TRASH_SMALL;
	}
	else if (strcmp(node.attribute("n").as_string(), "Altar") == 0) {
		type = ALTAR;
	}
	else
	{
		type = OBJECT_NONE;
		LOG("ERROR: XML Node OBJECT TYPE does not match");
	}
}
