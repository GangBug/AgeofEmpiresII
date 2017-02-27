#include "Entity.h"
#include "Log.h"
#include "M_EntityManager.h"

#include "SDL\include\SDL.h"

//TMP
#include "App.h"
#include "M_Render.h"
#include "M_Textures.h"


Entity::Entity(Entity* parent, SDL_Texture* texture, GB_Rectangle<int> drawRect) : parent(parent), entityTexture(texture), drawQuad(drawRect)
{
	name.assign("entity");
	enclosingRect.Set(0, 0, 0, 0);
	if (texture != nullptr && drawRect == ZeroRectangle)
	{
		app->tex->GetSize(texture, (uint&)enclosingRect.w, (uint&)enclosingRect.h);
		drawQuad = enclosingRect;
	}
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
			app->entityManager->EraseEntityFromTree(this);
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

		//2.Notify globalPosition has changed
		OnTransformUpdated();

		//3.Update from quadtree
		app->entityManager->EraseEntityFromTree(this); //TODO: Too brute... Define static entities???
		app->entityManager->InsertEntityToTree(this);

		transformHasChanged = false;
	}

	for (std::vector<Entity*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if ((*it))
			(*it)->RecCalcTransform(globalPosition, force);
	}
}

void Entity::RecCalcBox()
{
	if (selfActive == true && dirty == true)
	{
		//Actualy recalc box pos/size
		enclosingRect.Move(globalPosition.x, globalPosition.y);
		dirty = false;
	}

	for (std::vector<Entity*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if ((*it))
			(*it)->RecCalcBox();
	}
}

void Entity::SetNewParent(Entity* newParent, bool forceRecalcTransform)
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

	dirty = true;

	if (forceRecalcTransform == true && newParent)
	{
		globalPosition = newParent->GetGlobalPosition() + localPosition;
	}
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

void Entity::SetLocalPosition(iPoint pos)
{
	localPosition = pos;
	transformHasChanged = true;
}

void Entity::SetLocalPosition(int x, int y)
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

void Entity::SetGlobalPosition(iPoint pos) //TODO: Must change local pos to make it fit later when parent is moved
{
	globalPosition = pos;
	dirty = true;
}

void Entity::SetGlobalPosition(int x, int y) //TODO: Must change local pos to make it fit later when parent is moved
{
	globalPosition.create(x, y);
	dirty = true;
}

void Entity::SetEnclosingBoxPosition(int x, int y)
{
	enclosingRect.Move(x, y);
}

void Entity::SetEnclosingBoxSize(int w, int h)
{
	enclosingRect.w = w;
	enclosingRect.y = h;
}

void Entity::SetEnclosingBox(int x, int y, int w, int h)
{
	enclosingRect.Set(x, y, w, h);
}

void Entity::SetEnclosingBox(GB_Rectangle<int> r)
{
	enclosingRect.Set(r.x, r.y, r.w, r.h);
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
	app->render->DrawQuad({enclosingRect.x, enclosingRect.y, enclosingRect.w, enclosingRect.h}, 255, 0, 0, 255, false);

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

bool Entity::HasTexture()
{
	return entityTexture != nullptr;
}

SDL_Texture* Entity::GetTexture()const
{
	return entityTexture;
}

void Entity::SetTexture(SDL_Texture* texture, GB_Rectangle<int> drawRect)
{
	entityTexture = texture;

	if (texture != nullptr)
	{
		if (drawRect == ZeroRectangle)
		{
			app->tex->GetSize(texture, (uint&)enclosingRect.w, (uint&)enclosingRect.h);
			drawQuad.w = enclosingRect.w;
			drawQuad.h = enclosingRect.h;
		}
		else
		{
			drawQuad = drawRect;
			enclosingRect.w = drawQuad.w;
			enclosingRect.h = drawQuad.h;
		}
	}
}

GB_Rectangle<int> Entity::GetDrawQuad()const
{
	return drawQuad;
}

GB_Rectangle<int> Entity::GetEnclosingBox()const
{
	return enclosingRect;
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