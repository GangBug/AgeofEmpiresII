#include "Entity.h"
#include "Log.h"
#include "M_EntityManager.h"
#include "GB_QuadTree.h"

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
Entity::Entity(entity_type type, Entity* parent, SDL_Texture* texture, GB_Rectangle<int> drawRect) : type(type), parent(parent), entityTexture(texture), drawQuad(drawRect)
{
	name.assign("entity");
	objectTypee = 0;
	enclosingRect.Set(0, 0, 1, 1);
	if (texture != nullptr && drawRect == ZeroRectangle)
	{
		app->tex->GetSize(texture, (uint&)enclosingRect.w, (uint&)enclosingRect.h); //TODO: Enclosing box should not be the full texture size.
		drawQuad = enclosingRect;
	}

	scale.create(1.0f, 1.0f);
	app->entityManager->InsertEntityToTree(this);
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
	Save: Saves the entity basic information, call OnSave and iterate all childs saving them.
*/
bool Entity::Save(pugi::xml_node& node)const
{
	bool ret = true;

	//Name
	node.append_child("entity_name").append_attribute("value") = name.c_str();
	//Self active
	node.append_child("entity_self_active").append_attribute("value") = selfActive;
	//TYPE
	node.append_child("entity_type").append_attribute("value") = (int)type;

	//Global position
	pugi::xml_node gPos = node.append_child("global_position");
	gPos.append_attribute("x") = globalPosition.x;
	gPos.append_attribute("y") = globalPosition.y;

	//Local position
	pugi::xml_node lPos = node.append_child("local_position");
	lPos.append_attribute("x") = localPosition.x;
	lPos.append_attribute("y") = localPosition.y;

	//Enclosing box
	pugi::xml_node eBox = node.append_child("enclosing_box");
	eBox.append_attribute("x") = enclosingRect.x;
	eBox.append_attribute("y") = enclosingRect.y;
	eBox.append_attribute("w") = enclosingRect.w;
	eBox.append_attribute("h") = enclosingRect.h;

	//Draw quad
	pugi::xml_node dQuad = node.append_child("draw_quad");
	dQuad.append_attribute("x") = drawQuad.x;
	dQuad.append_attribute("y") = drawQuad.y;
	dQuad.append_attribute("w") = drawQuad.w;
	dQuad.append_attribute("h") = drawQuad.h;

	//Scale
	pugi::xml_node scl = node.append_child("scale");
	scl.append_attribute("x") = scale.x;
	scl.append_attribute("y") = scale.y;

	//TODO: Saving texture
	//TODO: Add UID and save the hierarchy with UIDs

	pugi::xml_node text = node.append_child("entity_texture");
	if (entityTexture != nullptr)
	{
		text.append_attribute("has_texture") = true;
		//Texture path/UID
		text.append_attribute("texture_path") = "???";
	}
	else
	{
		text.append_attribute("has_texture") = false;
	}

	//-----------------------------------

	ret = OnSave(node);

	//Childs
	pugi::xml_node childsN = node.append_child("childs");

	for (std::vector<Entity*>::const_iterator it = childs.begin(); it != childs.end() && ret == true; ++it)
	{
		Entity* tmp = *it;
		if (tmp != nullptr)
		{
			pugi::xml_node child = childsN.append_child(tmp->GetName());
			ret = tmp->Save(child);
		}
	}

	return ret;
}

/**
	Load: Loads the entity information, call OnLoad and iterate all childs loading them.
*/
bool Entity::Load(pugi::xml_node* node)
{
	bool ret = true;

	if (node == nullptr)
		return false;

	//Name
	SetName(node->child("entity_name").attribute("value").as_string("_entity"));
	//SelfActive
	SetActive(node->child("entity_self_active").attribute("value").as_bool(true));

	//Gloal position
	fPoint gPos;
	pugi::xml_node gPosN = node->child("global_position");
	gPos.x = gPosN.attribute("x").as_float(0);
	gPos.y = gPosN.attribute("y").as_float(0);
	SetGlobalPosition(gPos);

	//Local position
	fPoint lPos;
	pugi::xml_node lPosN = node->child("local_position");
	lPos.x = lPosN.attribute("x").as_float(0);
	lPos.y = lPosN.attribute("y").as_float(0);
	SetLocalPosition(lPos);

	//Enclosing box
	GB_Rectangle<int> eBox;
	pugi::xml_node eBoxN = node->child("enclosing_box");
	eBox.x = eBoxN.attribute("x").as_int(0);
	eBox.y = eBoxN.attribute("y").as_int(0);
	eBox.w = eBoxN.attribute("w").as_int(1);
	eBox.h = eBoxN.attribute("h").as_int(1);
	SetEnclosingBox(eBox);

	//DrawQuad
	pugi::xml_node dQuadN = node->child("draw_quad");
	drawQuad.x = dQuadN.attribute("x").as_int(0);
	drawQuad.y = dQuadN.attribute("y").as_int(0);
	drawQuad.w = dQuadN.attribute("w").as_int(1);
	drawQuad.h = dQuadN.attribute("h").as_int(1);

	//Scale
	fPoint scl;
	pugi::xml_node sclN = node->child("scale");
	scl.x = sclN.attribute("x").as_float(1.0f);
	scl.y = sclN.attribute("y").as_float(1.0f);
	SetScale(scl);

	//TODO: Loading texture
	//TODO: Add UID and load the hierarchy with UIDs

	pugi::xml_node text = node->child("entity_texture");
	if (text.attribute("has_texture").as_bool(false) == true)
	{
		//Load texture
	}

	ret = OnLoad(node);

	if (ret == true && selfActive == true)
		OnStart();

	pugi::xml_node childsN = node->child("childs");
	for (pugi::xml_node it = childsN.first_child(); it != NULL; it = it.next_sibling())
	{
		//Get the child type
		entity_type typ = (entity_type)it.child("entity_type").attribute("value").as_int(-1);
		Entity* tmp = app->entityManager->CreateEntity(typ, this);
		if (tmp != nullptr)
		{
			ret = tmp->Load(&it);
		}
	}

	return ret;
}

/**
	AddChild: Creates a new child and adds it to childs vector.
*/
Entity* Entity::AddChild()
{
	Entity* ret = nullptr;

	ret = new Entity(ENTITY_BASE, this);
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
			app->entityManager->EraseEntityFromTree(tmp);
			RELEASE(tmp);
			it = childs.erase(it);
			return true;
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
void Entity::RecCalcTransform(fPoint parentGlobalPos, bool force)
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
		if (currentQuadTreeNode != nullptr)
		{
			if (currentQuadTreeNode->box.Collides(enclosingRect) == false)
			{
				app->entityManager->EraseEntityFromTree(this); //TODO: Too brute... Define static entities???
				app->entityManager->InsertEntityToTree(this);
			}
		}

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
fPoint Entity::GetLocalPosition()const
{
	return localPosition;
}

/**
	GetLocalPosition: Fill tow ints with the local position in relation with its parent.
*/
void Entity::GetLocalPosition(float&x, float& y)const
{
	x = localPosition.x;
	y = localPosition.y;
}

/**
	SetLocalPos: Sets the local position of the entity.
*/
void Entity::SetLocalPosition(fPoint pos)
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
fPoint Entity::GetGlobalPosition()const
{
	return globalPosition;
}

/**
	GetGlobalPos: Return the global position in world coords reference.
*/
void Entity::GetGlobalPosition(float& x, float& y)const
{
	x = globalPosition.x;
	y = globalPosition.y;
}

/**
	SetGlobalPos: Sets the global position of the entity.
*/
void Entity::SetGlobalPosition(fPoint pos)
{
	globalPosition = pos;
	fPoint tmp = { 0.f, 0.f };
	tmp = globalPosition - ((parent != nullptr) ? parent->GetGlobalPosition() : fPoint(0, 0));
	SetLocalPosition(tmp);
	dirty = true;
}

/**
	SetGlobalPos: Sets the global position of the entity.
*/
void Entity::SetGlobalPosition(float x, float y)
{
	globalPosition.create(x, y);
	fPoint tmp = { 0.f, 0.f };
	tmp = globalPosition - ((parent != nullptr) ? parent->GetGlobalPosition() : fPoint(0, 0));
	SetLocalPosition(tmp);
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

iPoint Entity::GetPivot()const
{
	return pivot;
}

void Entity::GetPivot(int& x, int& y)const
{
	x = pivot.x;
	y = pivot.y;
}

void Entity::SetPivot(iPoint piv)
{
	pivot = piv;
}

void Entity::SetPivot(int x, int y)
{
	pivot.create(x, y);
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
	app->render->DrawQuad({enclosingRect.x - pivot.x, enclosingRect.y - pivot.y, enclosingRect.w, enclosingRect.h}, 255, 0, 0, 255, false);

	for (std::vector<Entity*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if ((*it))
			(*it)->DrawDebug();
	}
}

int Entity::GetHP() const
{
	return 0;
}

void Entity::DoDamage(int dmg)
{
}

direction Entity::GetDir()const
{
	return NORTH;
}

entity_type Entity::GetEntityType() const
{
	return type;
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

		//this is because units (who'll never have childs anyways...) get deleted when they die.
		if (selfActive == true)
		{
			if (childs.size() > 0)
			{
				for (std::vector<Entity*>::iterator it = childs.begin(); it != childs.end(); ++it)
				{
					Entity* tmp = (*it);

					if (tmp != nullptr && tmp->IsActive())
					{
						tmp->Update(dt);
					}
				}
			}
		}
	}
}

void Entity::Start()
{

	if (selfActive == true)
	{
		OnStart();

		for (std::vector<Entity*>::iterator it = childs.begin(); it != childs.end(); ++it)
		{
			Entity* tmp = (*it);

			if (tmp != nullptr && tmp->IsActive())
			{
				tmp->Start();
			}
		}

	}

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
	return GB_Rectangle<int>{(int)globalPosition.x, (int)globalPosition.y, enclosingRect.w, enclosingRect.h};
}

///------------------------------------------------

/**
	OnStart: Virtual method called after entity creation.
*/
void Entity::OnStart()
{
}

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

/**
	OnSave: Virtual method called when entity is saved. This method is supposed to be override to save especific entity data.
*/
bool Entity::OnSave(pugi::xml_node& node)const
{
	return true;
}

/**
	OnLoade: Virtual method called when entity is loaded. This method is supposed to be override to load especific entity data.
*/
bool Entity::OnLoad(pugi::xml_node* node)
{
	return true;
}



void Entity::Serialize(pugi::xml_node& root)
{
	if (this->type == ENTITY_OBJECT) {

		pugi::xml_attribute atr;
		pugi::xml_node element;

		element = root.append_child("Object");

		atr = element.append_attribute("type");
		atr.set_value(objectTypee);
		atr = element.append_attribute("x");
		atr.set_value(GetLocalPosition().x);
		atr = element.append_attribute("y");
		atr.set_value(GetGlobalPosition().y);
	}
}