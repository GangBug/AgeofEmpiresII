#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Defs.h"
#include "p2Point.h"
#include <string>
#include <vector>
#include "GB_Rectangle.h"

#include "PugiXml\src\pugixml.hpp"

class SDL_Texture;
class GB_QuadTreeNode;

enum entity_type
{
	ENTITY_UNKNOWN = 0,
	ENTITY_BASE = (1 << 0),
	ENTITY_UNIT = (1 << 2),
	ENTITY_BUILDING = (1 << 3),
	ENTITY_UI = ( 1 << 4),
	ENTITY_MAP = (1 << 5),
	ENTITY_PLAYER_MAN = (1 << 6)
};

class Entity
{
public:
	Entity(entity_type type, Entity* parent, SDL_Texture* texture = nullptr, GB_Rectangle<int> drawRect = ZeroRectangle); //TODO: Might be usefull to implement a UID system
	virtual ~Entity();

	Entity* AddChild();
	void AddChild(Entity* child); //TODO: Maybe add a parameter to force ID?
	Entity* GetParent()const;

	bool RecRemoveFlagged();
	void RecCalcTransform(fPoint parentGlobalPos, bool force = false);
	void RecCalcBox();
	void SetNewParent(Entity* newParent, bool forceRecalcTransform = false);

	/** Local transform */
	fPoint GetLocalPosition()const;
	void GetLocalPosition(float& x, float& y)const;
	void SetLocalPosition(fPoint pos);
	void SetLocalPosition(int x, int y);

	/** Global transform */
	fPoint GetGlobalPosition()const;
	void GetGlobalPosition(float& x, float& y)const;
	void SetGlobalPosition(fPoint pos);
	void SetGlobalPosition(float x, float y);

	/** Enclosing box methods */
	void SetEnclosingBoxPosition(int x, int y);
	void SetEnclosingBoxSize(int w, int h);
	void SetEnclosingBox(int x, int y, int w, int h);
	void SetEnclosingBox(GB_Rectangle<int> r);

	/** Size/Scale methods */
	fPoint GetScale()const;
	void GetScale(float& w, float& h)const;
	void SetScale(fPoint scl);            
	void SetScale(float w, float h);
	void SetScale(float scl);

	/** Pivot methods */
	iPoint GetPivot()const;
	void GetPivot(int& x, int& y)const;
	void SetPivot(iPoint piv);
	void SetPivot(int x, int y);

	bool IsActive()const;
	void SetActive(bool set);
	void Enable();
	void Disable();

	const char* GetName()const;
	void SetName(const char* str);

	void Remove();

	void Update(float dt);
	void Start();

	//---------------------------------------------------------------

	virtual void OnStart();
	virtual void OnFinish();
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnTransformUpdated(); //Called whenever transform has changed and recalculed.

	virtual void OnUpdate(float dt);

	virtual bool OnSave(pugi::xml_node& node)const;
	virtual bool OnLoad(pugi::xml_node* node);

	bool Save(pugi::xml_node& node)const;
	bool Load(pugi::xml_node* node);

	virtual void DrawDebug();

	//---------------------------------------------------------------

	bool HasTexture();
	SDL_Texture* GetTexture()const;
	void SetTexture(SDL_Texture* texture, GB_Rectangle<int> drawRect = ZeroRectangle);

	GB_Rectangle<int> GetDrawQuad()const;
	GB_Rectangle<int> GetEnclosingBox()const;

protected:
	Entity* parent = nullptr;
	bool selfActive = true;
	std::string name;
	/** Is a rectangle the enclose the entity and will be used to collide with the quadtree and might be and aproximation to simulate simple physics collisions. */
	GB_Rectangle<int> enclosingRect;

	SDL_Texture* entityTexture = nullptr; //TODO: Instead of a SDL_Testure directly would be better to have a structure that holds it with a UID or the texture path in order to load it later.
	/** It's a rectangle that determine the section of the texture that must be drawn. If an enclosing box is not provided this rect will be used to set it. */
	GB_Rectangle<int> drawQuad;

private:  //Set position to private because I want to modify position with methods to control global position change
	fPoint localPosition; //NOTE: Float point would be better??? For sure velocities must be used with floats
	fPoint globalPosition; //Actually draw position.

	/**
		GlobalPosition is the position in the world representation system. It's the position where the entity will be draw.
		Local position is the position in relation with its parent. 
		
		If parents position is 5, 5 and this entity local position is 1, 1 it will be drawn on 6, 6.
	*/

	bool transformHasChanged = false;
	bool dirty = true;


public:
	std::vector<Entity*> childs;
	bool removeFlag = false;

	/** Scale of entity on each axis. */
	fPoint scale;

	iPoint pivot = { 0, 0 };

	entity_type type = ENTITY_UNKNOWN;


	GB_QuadTreeNode* currentQuadTreeNode = nullptr;
};

#endif // !__ENTITY_H__