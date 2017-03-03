#include "M_EntityManager.h"
#include "Entity.h"

#include "App.h"
#include "Log.h"

//TMP
#include "M_Input.h"
#include "SDL\include\SDL.h"
#include "M_Textures.h"

M_EntityManager::M_EntityManager(bool startEnabled) : Module(startEnabled)
{
	LOG("Entity manager: Creation.");
	name.assign("entity_manager");
}


M_EntityManager::~M_EntityManager()
{
	LOG("Entity manager: Destroying.");
}

bool M_EntityManager::Awake(pugi::xml_node&)
{
	LOG("Entity manager: Awake.");
	bool ret = true;

	root = new Entity(nullptr);
	if (!root)
	{
		ret = false;
		LOG("ERROR: Could not create scene root entity.");
	}

	return ret;
}

bool M_EntityManager::Start()
{
	LOG("Entity manager: Start.");
	bool ret = true;

	//TMP
	textTexture = app->tex->Load("textures/test.png");

	et = CreateEntity(nullptr, 300, 100);
	et->SetTexture(textTexture);
	//et2 = CreateEntity(et, 50, 50);
	//et2->SetTexture(textTexture, GB_Rectangle<int>(0, 0, 100, 100));
	archer = CreateUnit(CAVALRY_ARCHER, nullptr, 1000, 300);

	return ret;
}

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
		iPoint pos;
		app->input->GetMousePosition(pos.x, pos.y);
		et->SetLocalPosition(pos);
	}

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
	{
		iPoint pos;
		app->input->GetMousePosition(pos.x, pos.y);
		//et2->SetLocalPosition(pos);
		et2->SetGlobalPosition(pos);
	}

	return ret;
}

update_status M_EntityManager::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (root)
	{
		//TODO: Only if play mode??
		root->OnUpdate(dt);
	}
	else
	{
		LOG("ERROR: Invalid root, is NULL.");
		ret = UPDATE_ERROR;
	}

	return ret;
}

update_status M_EntityManager::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

bool M_EntityManager::CleanUp()
{
	LOG("Entity manager: CleanUp.");
	bool ret = true;

	root->Remove();
	//TODO: Autosave scene

	RELEASE(root);

	return ret;
}

Entity* M_EntityManager::CreateEntity(Entity* parent, int posX, int posY, int rectX, int rectY)
{
	Entity* ret = new Entity(parent);

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
		LOG("ERROR: Could not create a new entity.");
	}

	return ret;
}

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

Entity* M_EntityManager::GetSceneRoot()const
{
	return root;
}

Entity* M_EntityManager::FindEntity()
{
	return nullptr; //TODO
}

void M_EntityManager::Draw(std::vector<Entity*>& entitiesToDraw)
{
	if (root)
	{
		//TODO: Must check with quadtree
		for (std::vector<Entity*>::iterator it = root->childs.begin(); it != root->childs.end(); ++it)
		{
			RecColectEntitiesToDraw(entitiesToDraw, (*it));
		}
	}
	else
	{
		LOG("ERROR: Invalid root, is NULL.");
	}
}

void M_EntityManager::DrawDebug()
{
	if (root)
	{
		for (std::vector<Entity*>::iterator it = root->childs.begin(); it != root->childs.end(); ++it)
		{
			if ((*it))
				(*it)->DrawDebug();
		}
	}
	else
	{
		LOG("ERROR: Invalid root, is NULL.");
	}
}

void M_EntityManager::InsertEntityToTree(Entity* et)
{

}

void M_EntityManager::EraseEntityFromTree(Entity* et)
{

}

void M_EntityManager::LoadScene()
{
	mustLoadScene = true;
}

void M_EntityManager::SaveScene()
{
	mustSaveScene = true;
}

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

bool M_EntityManager::LoadSceneNow()
{
	return false;
}

bool M_EntityManager::SaveSceneNow()
{
	return false;
}
