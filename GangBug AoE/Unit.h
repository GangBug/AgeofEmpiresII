#ifndef __UNIT_H__
#define __UNIT_H__

#include "Entity.h"

enum unit_type
{
	DEFAULT_UNIT,
	CAVALRY_ARCHER
};

class Unit :public Entity
{
public:
	Unit(unit_type type, Entity* parent = nullptr);

	void OnUpdate(float dt);

	unit_type GetType() const;

private:
	unit_type type;

	//STATS?
};

#endif //__UNIT_H__