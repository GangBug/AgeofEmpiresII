#include "M_EntityManager.h"
#include "Entity.h"

#include "Log.h"


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

	Entity* et = CreateEntity(nullptr, 100, 100);
	CreateEntity(nullptr, 800, 600);
	CreateEntity(nullptr, 100, 900);
	CreateEntity(nullptr, 700, 400);

	return ret;
}

update_status M_EntityManager::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	RemoveFlagged();

	if (root)
	{
		//TODO: Update rans
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
	Entity* ret = nullptr;

	if (parent)
		ret = parent->AddChild();
	else
		ret = root->AddChild();

	if (ret)
	{
		ret->SetLocalPosition(posX, posY);
		//ret->SetRect(rectX, rectY);
	}
	else
	{
		LOG("ERROR: Could not create a new entity.");
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

void M_EntityManager::Draw()
{
	if (root)
	{
		//TODO: Must order here or on renderer the list. Also must test with quadtree the drawable entites
		for (std::vector<Entity*>::iterator it = root->childs.begin(); it != root->childs.end(); ++it)
		{
			if ((*it))
				(*it)->Draw();
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

bool M_EntityManager::LoadSceneNow()
{
	return false;
}

bool M_EntityManager::SaveSceneNow()
{
	return false;
}
