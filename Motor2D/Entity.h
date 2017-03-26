#ifndef _ENTITY
#define _ENTITY

#include "p2Point.h"

enum ENTITY_TYPE
{
	NO_ENTITY = 0,
	UNIT
};
enum ENTITY_STATUS
{
	E_SELECTED = 0,
	E_NON_SELECTED
};

class Entity
{
private:
	bool to_delete;
	fPoint position;
	int hp;
	int armor;
	enum ENTITY_TYPE entity_type;
	enum ENTITY_STATUS entity_status = E_NON_SELECTED;

public:
	Entity(ENTITY_TYPE entity_type, fPoint pos);
	~Entity();

	virtual void Update() = 0;
	virtual void PostUpdate() = 0;
	virtual void DoAI() = 0;
	virtual void Draw() = 0;

	void Die();//to_delete = true

	//Geters
	bool ToDelete() const;
	ENTITY_TYPE GetEntityType() const;
	ENTITY_STATUS GetEntityStatus();
	void SetEntityStatus(ENTITY_STATUS status);
	void SetPosition(float x, float y);
	int GetHP() const;
	const float GetX() const;
	const float GetY() const;

protected:
	//Seters
	void SetArmor(int new_armor);
	void SetHp(int new_hp);
};
#endif
