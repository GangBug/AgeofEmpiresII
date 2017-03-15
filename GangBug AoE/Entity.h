#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Defs.h"
#include "p2Point.h"
#include <string>
#include <vector>
#include "GB_Rectangle.h"

class SDL_Texture;

class Entity
{
public:
	Entity(Entity* parent, SDL_Texture* texture = nullptr, GB_Rectangle<int> drawRect = ZeroRectangle); //TODO: Might be usefull to implement a UID system
	virtual ~Entity();

	Entity* AddChild();
	void AddChild(Entity* child); //TODO: Maybe add a parameter to force ID?
	Entity* GetParent()const;

	bool RecRemoveFlagged();
	void RecCalcTransform(iPoint parentGlobalPos, bool force = false);
	void RecCalcBox();
	void SetNewParent(Entity* newParent, bool forceRecalcTransform = false);

	/** Local transform */
	iPoint GetLocalPosition()const;
	void GetLocalPosition(int& x, int& y)const;
	void SetLocalPosition(iPoint pos);
	void SetLocalPosition(int x, int y);

	/** Global transform */
	iPoint GetGlobalPosition()const;
	void GetGlobalPosition(int& x, int& y)const;
	void SetGlobalPosition(iPoint pos);
	void SetGlobalPosition(int x, int y);

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

	bool IsActive()const;
	void SetActive(bool set);
	void Enable();
	void Disable();

	const char* GetName()const;
	void SetName(const char* str);

	void Remove();

	void Update(float dt);

	//---------------------------------------------------------------

	virtual void OnStart();
	virtual void OnFinish();
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnTransformUpdated(); //Called whenever transform has changed and recalculed.

	virtual void OnUpdate(float dt);

	virtual bool Save(); //TODO: xml_node as parameter
	virtual bool Load(); //TODO: xml_node as parameter

	virtual void DrawDebug();		//TODO: Const?? Probably

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

	SDL_Texture* entityTexture = nullptr;
	/** It's a rectangle that determine the section of the texture that must be drawn. If an enclosing box is not provided this rect will be used to set it. */
	GB_Rectangle<int> drawQuad;

private:  //Set position to private because I want to modify position with methods to control global position change
	iPoint localPosition; //NOTE: Float point would be better??? For sure velocities must be used with floats
	iPoint globalPosition; //Actually draw position.

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
};

#endif // !__ENTITY_H__