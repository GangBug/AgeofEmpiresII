#include "Entity.h"
#include "Log.h"

#include "M_EntityManager.h"
//TMP
#include "App.h"
#include "M_Render.h"


Entity::Entity(Entity* parent) : parent(parent)
{
	name.assign("entity");
}


Entity::~Entity()
{
	for (uint i = 0; i < childs.size(); ++i)
		RELEASE(childs[i]);
}

bool Entity::Save()
{
	return false;
}

bool Entity::Load()
{
	return false;
}

Entity* Entity::AddChild()
{
	Entity* ret = nullptr;

	ret = new Entity(this);
	if (ret)
		childs.push_back(ret);
	else
		LOG("ERROR: Could not create a new entity child of [%s].", parent->GetName());

	return ret;
}

Entity* Entity::GetParent()const
{
	return parent;
}

bool Entity::RecRemoveFlagged()
{
	bool ret = false;

	for (std::vector<Entity*>::iterator it = childs.begin(); it != childs.end();)
	{
		Entity* tmp = *it;
		if (tmp && tmp->removeFlag)
		{
			tmp->OnFinish();
			//TODO: Erase from quadtree
			RELEASE(tmp);
			it = childs.erase(it);
			ret = true;
		}
		else
		{
			ret |= tmp->RecRemoveFlagged();
			++it;
		}
	}

	return ret;
}

void Entity::RecCalcTransform(iPoint parentGlobalPos, bool force)
{
	if (transformHasChanged == true || force == true)
	{
		force = true;
		dirty = true;

		//1.Recalc globalPosition
		globalPosition = parentGlobalPos + localPosition;
		//TODO: Box position must be updated 

		//2.Notify globalPosition has changed
		OnTransformUpdated();

		//3.Update from quadtree
		app->entityManager->EraseEntityFromTree(this); //TODO: Too brute... Define static entities???
		app->entityManager->InsertEntityToTree(this);
	}
	else
	{
		dirty = false;
	}

	for (std::vector<Entity*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if ((*it))
			(*it)->RecCalcTransform(globalPosition, force);
	}
}

void Entity::SetNewParent(Entity* newParent)
{
	if (newParent == parent)
		return;

	if (parent)
	{
		std::vector<Entity*>::iterator it = std::find(parent->childs.begin(), parent->childs.end(), this);
		if (it != parent->childs.end())
			parent->childs.erase(it);
	}

	parent = newParent;

	if (newParent)
		newParent->childs.push_back(this);

	//TODO: If force recalc transform do it
}

iPoint Entity::GetLocalPosition()const
{
	return localPosition;
}

void Entity::GetLocalPosition(int&x, int& y)const
{
	x = localPosition.x;
	y = localPosition.y;
}

void Entity::SetLocalPosition(iPoint pos) //TODO: Must recalc global pos
{
	localPosition = pos;
	transformHasChanged = true;
}

void Entity::SetLocalPosition(int x, int y) //TODO: Must recalc global pos
{
	localPosition.create(x, y);
	transformHasChanged = true;
}

iPoint Entity::GetGlobalPosition()const
{
	return globalPosition;
}

void Entity::GetGlobalPosition(int& x, int& y)const
{
	x = globalPosition.x;
	y = globalPosition.y;
}

void Entity::SetGlobalPosition(iPoint pos)
{
	globalPosition = pos;
}

void Entity::SetGlobalPosition(int x, int y)
{
	globalPosition.create(x, y);
}

bool Entity::IsActive()const
{
	return selfActive;
}

void Entity::SetActive(bool set)
{
	if (set != selfActive)
	{
		selfActive = set;
		if (set)
			OnEnable();
		else
			OnDisable();
	}
}

void Entity::Enable()
{
	if (!selfActive)
	{
		selfActive = true;
		OnEnable();
	}
}

void Entity::Disable()
{
	if (selfActive)
	{
		selfActive = false;
		OnDisable();
	}
}

const char* Entity::GetName()const
{
	return name.c_str();
}

void Entity::SetName(const char* str)
{
	if (str)
		name.assign(str);
}

void Entity::Draw()
{}

void Entity::DrawDebug()
{
	//TMP
	app->render->DrawCircle(globalPosition.x, globalPosition.y, 30, 255, 0, 0, 255);

	for (std::vector<Entity*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if ((*it))
			(*it)->DrawDebug();
	}
}

void Entity::Remove()
{
	removeFlag = true;
}

///------------------------------------------------

void Entity::OnStart()
{}

void Entity::OnFinish()
{}

void Entity::OnEnable()
{}

void Entity::OnDisable()
{}

void Entity::OnUpdate(float dt)
{}

void Entity::OnTransformUpdated()
{}