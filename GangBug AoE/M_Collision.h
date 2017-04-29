#ifndef __M_COLLISION_H__
#define __M_COLLISION_H__

#include "Module.h"

class Unit;

class M_Collision : public Module
{
public:

	M_Collision(bool startEnabled = true);
	~M_Collision();

	update_status Update(float dt);

	void DrawDebug()override;

private:

	bool DoUnitsIntersect(Unit* unit1, Unit* unit2);
	iPoint FindClosestWalkable(Unit* unit);
	void SplitUnits(Unit* unit1);
};
#endif 
