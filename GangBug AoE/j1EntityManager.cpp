#include "j1EntityManager.h"
#include "Entity.h"

#include "p2Log.h"


j1EntityManager::j1EntityManager()
{
	LOG("Entity manager: Creation.");
}


j1EntityManager::~j1EntityManager()
{
	LOG("Entity manager: Destroying.");
}

bool j1EntityManager::Awake(pugi::xml_node&)
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

bool j1EntityManager::Start()
{
	LOG("Entity manager: Start.");
	bool ret = true;

	Entity* et = CreateEntity(nullptr, 100, 100);
	CreateEntity(nullptr, 800, 600);
	CreateEntity(nullptr, 100, 900);
	CreateEntity(nullptr, 700, 400);

	return ret;
}

bool j1EntityManager::PreUpdate()
{
	bool ret = true;

	RemoveFlagged();

	if (root)
	{
		//TODO: Update rans
	}
	else
	{
		LOG("ERROR: Invalid root, is NULL.");
	}

	if (mustSaveScene)
	{
		SaveSceneNow();
		mustSaveScene = false;
	}

	if (mustLoadScene)
	{
		LoadSceneNow();
		mustLoadScene = false;
	}

	return ret;
}

bool j1EntityManager::Update(float dt)
{
	bool ret = true;

	if (root)
	{
		//TODO: Only if play mode??
		root->OnUpdate(dt);
	}
	else
	{
		LOG("ERROR: Invalid root, is NULL.");
	}

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool j1EntityManager::CleanUp()
{
	LOG("Entity manager: CleanUp.");
	bool ret = true;

	root->Remove();
	//TODO: Autosave scene

	RELEASE(root);

	return ret;
}

Entity* j1EntityManager::CreateEntity(Entity* parent, int posX, int posY, int rectX, int rectY)
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

Entity* j1EntityManager::GetSceneRoot()const
{
	return root;
}

Entity* j1EntityManager::FindEntity()
{
	return nullptr; //TODO
}

void j1EntityManager::Draw()
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

void j1EntityManager::DrawDebug()
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

void j1EntityManager::InsertEntityToTree(Entity* et)
{

}

void j1EntityManager::EraseEntityFromTree(Entity* et)
{

}

void j1EntityManager::LoadScene()
{
	mustLoadScene = true;
}

void j1EntityManager::SaveScene()
{
	mustSaveScene = true;
}

void j1EntityManager::RemoveFlagged()
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

bool j1EntityManager::LoadSceneNow()
{
	return false;
}

bool j1EntityManager::SaveSceneNow()
{
	return false;
}
