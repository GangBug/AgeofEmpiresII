#ifndef _j1Collision_
#define _j1Collision_

#include "j1Module.h"
#include "j1EntityManager.h"

class j1Collision : public j1Module
{
public:

	j1Collision();
	~j1Collision();

	bool Update(float dt);


private:

	bool DoUnitsIntersect(Unit* unit1, Unit* unit2);
	iPoint FindClosestWalkable(Unit* unit);
	void SplitUnits(Unit* unit1);
};
#endif //_j1Collision_