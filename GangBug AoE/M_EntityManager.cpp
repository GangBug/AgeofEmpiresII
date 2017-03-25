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
	
	


	//TMP
	textTexture = app->tex->Load("textures/test.png");

	CreateEntity(ENTITY_PLAYER_MAN, nullptr);

	CreateEntity(ENTITY_MAP, nullptr);
	//et = CreateEntity(nullptr, 300, 100);
	//et->SetTexture(textTexture);
	//et2 = CreateEntity(et, 50, 50);
	//et2->SetTexture(textTexture, GB_Rectangle<int>(0, 0, 100, 100));
	//archer = CreateUnit(CAVALRY_ARCHER, nullptr, 1000, 300);


	/*if (app->map->Load("0.1Map.tmx") == true)
	{
		//int w, h;
		//uchar* data = NULL;
		//if (app->map->CreateWalkabilityMap(w, h, &data))
		//	app->pathfinding->SetMap(w, h, data);

		//RELEASE_ARRAY(data);
	}*/

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
		app->input->GetMousePosition(x, y);
		fPoint pos(x, y);
		et->SetLocalPosition(pos);
	}

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
	{
		int x, y;
		app->input->GetMousePosition(x, y);
		//et2->SetLocalPosition(pos);
		fPoint pos(x, y);
		et2->SetGlobalPosition(pos);
	}

	if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		iPoint mP;
		app->input->GetMousePosition(mP.x, mP.y);
		CreateRandomTestEntity()->SetGlobalPosition(mP.x, mP.y);
	}

	if (app->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		Entity* t = CreateEntity(ENTITY_UNIT, nullptr);
		iPoint mP;
		app->input->GetMousePosition(mP.x, mP.y);
		t->SetGlobalPosition(mP.x, mP.y);
	}

	if (app->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
	{
		int x, y;
		app->input->GetMousePosition(x, y);
		Entity* et = CreateRandomTestEntity();
		fPoint pos(x, y);
		et->SetGlobalPosition(pos);
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
		ret = new Unit(CAVALRY_ARCHER, nullptr);
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
		ret->SetLocalPosition(posX, posY);
		ret->SetEnclosingBox(posX, posY, rectX, rectY);
	}
	else
	{
		LOG("ERROR: Could not create a new unit.");
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

void M_EntityManager::GetEntitiesOnRect(entity_type type, std::vector<Entity*>& vec, GB_Rectangle<int> rectangle)
{
	//TODO: Would be cool to be able to ask for several types just like layers (using bit operators | )
	if (type != ENTITY_UNKNOWN && sceneTree != nullptr)
	{
		//TODO: Change this, i dont like it at all...
		std::vector<Entity*> vec2;
		sceneTree->CollectCandidates(vec2, rectangle);
		for (std::vector<Entity*>::iterator it = vec2.begin(); it != vec2.end(); ++it)
			if ((*it)->type == type)
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
