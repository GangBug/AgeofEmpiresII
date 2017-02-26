#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Defs.h"
#include "p2Point.h"
#include <string>
#include <vector>

class Entity
{
public:
	Entity(Entity* parent); //TODO: Might be usefull to implement a UID system
	virtual ~Entity();

	virtual bool Save(); //TODO: xml_node as parameter
	virtual bool Load(); //TODO: xml_node as parameter

	Entity* AddChild(); //TODO: Maybe add a parameter to force ID?
	Entity* GetParent()const;

	bool RecRemoveFlagged();
	void RecCalcTransform(iPoint parentGlobalPos, bool force = false);
	void SetNewParent(Entity* newParent);

	iPoint GetLocalPosition()const;
	void GetLocalPosition(int& x, int& y)const;
	void SetLocalPosition(iPoint pos);
	void SetLocalPosition(int x, int y);

	iPoint GetGlobalPosition()const;
	void GetGlobalPosition(int& x, int& y)const;
	void SetGlobalPosition(iPoint pos);
	void SetGlobalPosition(int x, int y);

	//TODO: Rectangle enclosing box and methods...

	bool IsActive()const;
	void SetActive(bool set);
	void Enable();
	void Disable();

	const char* GetName()const;
	void SetName(const char* str);

	virtual void Draw();			//TODO: Const?? Probably
	virtual void DrawDebug();		//TODO: Const?? Probably

	void Remove();

	virtual void OnStart();
	virtual void OnFinish();
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnTransformUpdated(); //Called whenever transform has changed and recalculed.

	virtual void OnUpdate(float dt);

protected:
	Entity* parent = nullptr;
	bool selfActive = true;
	std::string name;

private:  //Set position to private because I want to modify position with methods to control global position change
	iPoint localPosition; //NOTE: Float point would be better??? For sure velocities must be used with floats
	iPoint globalPosition; //Actually draw position.

	//TODO: Rectangle enclosing box

	bool transformHasChanged = false;
	bool dirty = true;

public:
	std::vector<Entity*> childs;
	bool removeFlag = false;

};

#endif // !__ENTITY_H__