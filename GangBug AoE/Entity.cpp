#include "Entity.h"
#include "Log.h"
#include "M_EntityManager.h"

#include "SDL\include\SDL.h"

//TMP
#include "App.h"
#include "M_Render.h"
#include "M_Textures.h"

/**
	Constructor: Fill some entity data.

	Parameters:
		-Entity* parent of the new entity.
		-SDL_Texture* entity texture if it has. Nullptr by default.
		-GB_Rectangle<int> drawRect: Is the texture section that must be drawn. ZeroRectangle(0, 0, 0, 0) by default.
*/
Entity::Entity(Entity* parent, SDL_Texture* texture, GB_Rectangle<int> drawRect) : parent(parent), entityTexture(texture), drawQuad(drawRect)
{
	name.assign("entity");
	enclosingRect.Set(0, 0, 0, 0);
	if (texture != nullptr && drawRect == ZeroRectangle)
	{
		app->tex->GetSize(texture, (uint&)enclosingRect.w, (uint&)enclosingRect.h); //TODO: Enclosing box should not be the full texture size.
		drawQuad = enclosingRect;
	}

	scale.create(1.0f, 1.0f);
}

/**
	Destructor: Destroy the entity and all its childs.
*/
Entity::~Entity()
{
	for (uint i = 0; i < childs.size(); ++i)
		RELEASE(childs[i]);
}

/**
	Save: Saves the entity information.
*/
bool Entity::Save()
{
	return false;
}

/**
	Load: Loads the entity information.
*/
bool Entity::Load()
{
	return false;
}

/**
	AddChild: Creates a new child and adds it to childs vector.
*/
Entity* Entity::AddChild()
{
	Entity* ret = nullptr;

	ret = new Entity(this);
	if (ret != nullptr)
		childs.push_back(ret);
	else
		LOG("ERROR: Could not create a new entity child of [%s].", parent->GetName());

	return ret;
}

/**
	AddChild: Add a passed entity to child vector.

	Parameters:
		-Entity to add.
*/
void Entity::AddChild(Entity* child) //NOTE!!! : The old AddChild was usefull to abstract a bit of system complexity.
{
	childs.push_back(child);
}

/**
	GetParent: Return the entity parent.

	Return: 
		-Entity*.
*/
Entity* Entity::GetParent()const
{
	return parent;
}

/**
	RecRemoveFlagged: Actually deletes all marked childs after calling theirs OnFinish.

	Return:
		-True if any child has been deleted.
		-False if any child has been deletes.
*/
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

/**
	RecCalcTransform: Update the global position according to its parent global position and its own local position if this one has changed. Adter this recusecly calls all childs.

	Parameters: 
		-iPoint parentGlobalPosition.
		-bool force if want to force the calc when position have not changed.
*/
void Entity::RecCalcTransform(iPoint parentGlobalPos, bool force)
{
	if (transformHasChanged == true || force == true)
	{
		force = true;
		dirty = true;

		//1.Recalc globalPosition
		globalPosition = parentGlobalPos + localPosition;
		enclosingRect.Move(globalPosition.x, globalPosition.y);

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

/**
	RecCalcBox: If entity is dirty, recalculate the enclosing box and recursively recalc childs.
*/
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

/**
	SetNewParent: Set a new parent to the entity.

	Parameters:
		-Entity* the new parent.
		-Bool if want to force to recalculate the position.
*/
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

/**
	GetLocalPosition: Returns an iPoint with the local position in relation with its parent.
*/
iPoint Entity::GetLocalPosition()const
{
	return localPosition;
}

/**
	GetLocalPosition: Fill tow ints with the local position in relation with its parent.
*/
void Entity::GetLocalPosition(int&x, int& y)const
{
	x = localPosition.x;
	y = localPosition.y;
}

/**
	SetLocalPos: Sets the local position of the entity.
*/
void Entity::SetLocalPosition(iPoint pos)
{
	localPosition = pos;
	transformHasChanged = true;
}

/**
	SetLocalPos: Sets the local position of the entity.
*/
void Entity::SetLocalPosition(int x, int y)
{
	localPosition.create(x, y);
	transformHasChanged = true;
}

/**
	GetGlobalPos: Return the global position in world coords reference.
*/
iPoint Entity::GetGlobalPosition()const
{
	return globalPosition;
}

/**
	GetGlobalPos: Return the global position in world coords reference.
*/
void Entity::GetGlobalPosition(int& x, int& y)const
{
	x = globalPosition.x;
	y = globalPosition.y;
}

/**
	SetGlobalPos: Sets the global position of the entity.
*/
void Entity::SetGlobalPosition(iPoint pos) //TODO: Must change local pos to make it fit later when parent is moved
{
	globalPosition = pos;
	iPoint tmp = { 0, 0 };
	tmp = globalPosition - ((parent != nullptr) ? parent->GetGlobalPosition() : iPoint(0, 0));
	SetLocalPosition(tmp); //TODO: Must test this.
	dirty = true;
}

/**
	SetGlobalPos: Sets the global position of the entity.
*/
void Entity::SetGlobalPosition(int x, int y) //TODO: Must change local pos to make it fit later when parent is moved
{
	globalPosition.create(x, y);
	iPoint tmp = { 0, 0 };
	tmp = globalPosition - ((parent != nullptr) ? parent->GetGlobalPosition() : iPoint(0, 0));
	SetLocalPosition(tmp); //TODO: Must test this.
	dirty = true;
}

/**
	SetEnclosingBoxPosition: Moves the enclosing box to a certain point. This coords will be treated on world reference system.
*/
void Entity::SetEnclosingBoxPosition(int x, int y)
{
	enclosingRect.Move(x, y);
}

/**
	SetEnclosingBoxSize: Sets the size of the enclosing box.
*/
void Entity::SetEnclosingBoxSize(int w, int h)
{
	enclosingRect.w = w;
	enclosingRect.h = h;
}

/**
	SetEnclosingBox: Sets a new enclosing box.
*/
void Entity::SetEnclosingBox(int x, int y, int w, int h)
{
	enclosingRect.Set(x, y, w, h);
}

/**
	SetEnclosingBox: Sets a new enclosing box.
*/
void Entity::SetEnclosingBox(GB_Rectangle<int> r)
{
	enclosingRect.Set(r.x, r.y, r.w, r.h);
}

/**
	GetScale: Returns the scale of the entity as a fPoint.
*/
fPoint Entity::GetScale()const
{
	return scale;
}

/**
	GetScale: Returns the scale of the entity by two floats as reference.
*/
void Entity::GetScale(float& w, float& h)const
{
	w = scale.x;
	h = scale.y;
}

/**
	SetScale: Set the scale from a fPoint.
*/
void Entity::SetScale(fPoint scl)
{
	fPoint tmp(scl.x / scale.x, scl.y / scale.y);
	scale = scl;
	enclosingRect.w *= tmp.x;
	enclosingRect.h *= tmp.y;
}

/**
	SetScale: Set the scale from two floats, one for each axis.
*/
void Entity::SetScale(float w, float h)
{
	fPoint tmp(w / scale.x, h / scale.y);
	scale.create(w, h);
	enclosingRect.w *= tmp.x;
	enclosingRect.h *= tmp.y;
}

/**
	SetScale: Set the scale from one float, the same will be applied to both axis to keep the scale ratio.
*/
void Entity::SetScale(float scl)
{
	SetScale(scl, scl);
}

/**
	IsActive: Return true if entity is active and false if not.
*/
bool Entity::IsActive()const
{
	return selfActive;
}

/**
	SetActive: Sets entity self active. If it has changed calls OnEnable or OnDisable.
*/
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

/**
	Enable: Set entity active to true and calls OnEnable if it wasnt active before.
*/
void Entity::Enable()
{
	if (!selfActive)
	{
		selfActive = true;
		OnEnable();
	}
}

/**
	Disable: Set entity active to false and calls OnDisable if it was active before.
*/
void Entity::Disable()
{
	if (selfActive)
	{
		selfActive = false;
		OnDisable();
	}
}

/** GetName: Returns entity name. */
const char* Entity::GetName()const
{
	return name.c_str();
}

/** SetsName: Sets entity name. */
void Entity::SetName(const char* str)
{
	if (str)
		name.assign(str);
}

/** DrawDebug: Draw all debug info about the entity. */
void Entity::DrawDebug()
{
	app->render->DrawQuad({enclosingRect.x, enclosingRect.y, enclosingRect.w, enclosingRect.h}, 255, 0, 0, 255, false);

	for (std::vector<Entity*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if ((*it))
			(*it)->DrawDebug();
	}
}

/** 
	Remove: Mark the entity to delete it in a save moment.
*/
void Entity::Remove()
{
	removeFlag = true;
}

/**
	Update: Called every frame. Calls OnUpdate where logic is placed and recursively calls childs update.
*/
void Entity::Update(float dt)
{
	if (selfActive == true)
	{
		OnUpdate(dt);

		for (std::vector<Entity*>::iterator it = childs.begin(); it != childs.end(); ++it)
		{
			Entity* tmp = (*it);

			if (tmp != nullptr && tmp->IsActive())
			{
				tmp->Update(dt);
			}
		}

	}
	
	//No longer done like this but keep until I understand dynamic_cast
	/*for (std::vector<Entity*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if((*it)->IsActive())
		{ 
			if (strcmp((*it)->name.c_str(), "unit") == 0)
			{
				dynamic_cast<Unit*>(*it)->Update(dt);
			}
			else
			{
				(*it)->Update(dt);
			}
		}
	}*/
}

/**
	HasTexture: Return true if has a texture or false if not.
*/
bool Entity::HasTexture()
{
	return entityTexture != nullptr;
}

/**
	GetTexture: Return the texture or nullptr.
*/
SDL_Texture* Entity::GetTexture()const
{
	return entityTexture;
}

/**
	SetTexture: Change the enitity texture.

	Parameters:
		-SDL_Texture* the new texture.
		-GB_Rectangle<int> the section of the texture to draw. Will be also used to set the enclosing box. ZeroRect by default. If ZeroRect, it usesthe texture size.
*/
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

/** GetDrawQuad: Returns the section of the texture to draw. */
GB_Rectangle<int> Entity::GetDrawQuad()const
{
	return drawQuad;
}

/** GetEnclosingBox: Returns the box that enclose the entity. */
GB_Rectangle<int> Entity::GetEnclosingBox()const
{
	return enclosingRect;
}

///------------------------------------------------

/**
	OnStart: Virtual method called after entity creation.
*/
void Entity::OnStart()
{}

/**
	OnFinish: Virtual method called before entity delete.
*/
void Entity::OnFinish()
{}

/**
	OnEnable: Virtual method called when entity is enabled.
*/
void Entity::OnEnable()
{}

/**
	OnDisable: Virtual method called when entity is disabled.
*/
void Entity::OnDisable()
{}

/**
	OnUpdate: Virtual method called every frame and must containt the logic of the entity.
*/
void Entity::OnUpdate(float dt)
{}

/**
	OnTransformUpdated: Virtual method called when transform/global position has changed.
*/
void Entity::OnTransformUpdated()
{}