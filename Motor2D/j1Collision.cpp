#include "j1Collision.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"

#include "j1Map.h"

j1Collision::j1Collision()
{
	name.assign("collision");
}

j1Collision::~j1Collision() {}

bool j1Collision::Update(float dt)
{
	bool ret = true;

	for (std::list<Entity*>::iterator unit1 = App->entity_manager->entity_list.begin(); unit1 != App->entity_manager->entity_list.end(); unit1++)
	{
		if ((*unit1)->GetEntityType() == UNIT)
		{
			iPoint pos = App->map->WorldToMap(unit1._Ptr->_Myval->GetX(), unit1._Ptr->_Myval->GetY());
			Unit* unit_1 = (Unit*)unit1._Ptr->_Myval;
			//this shouldn't happen, but if any case:
			if (!App->pathfinding->IsWalkable(pos) && unit_1->state != MOVING && unit_1->state != MOVING_TO_ATTACK)
			{
				iPoint tile = FindClosestWalkable((Unit*)unit1._Ptr->_Myval);
				unit1._Ptr->_Myval->SetPosition(tile.x, tile.y);
			}
			else
			{
				//Check colisions between units
				for (std::list<Entity*>::iterator unit2 = App->entity_manager->entity_list.begin(); unit2 != App->entity_manager->entity_list.end(); unit2++)
				{
					if (unit1 != unit2)
					{
						if (DoUnitsIntersect((Unit*)unit1._Ptr->_Myval, (Unit*)unit2._Ptr->_Myval) == true)
						{
							//Collision detected
							Unit* unit_2 = (Unit*)unit2._Ptr->_Myval;
							if (unit_1->id > unit_2->id && unit_1->state != MOVING && unit_1->state != MOVING_TO_ATTACK)
							{
								SplitUnits((Unit*)unit1._Ptr->_Myval, (Unit*)unit2._Ptr->_Myval);
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

bool j1Collision::DoUnitsIntersect(Unit* unit1, Unit* unit2)
{
	float distance_x = unit1->GetX() - unit2->GetX();
	float distance_y = unit1->GetY() - unit2->GetY();
	return (sqrt(distance_x * distance_x + distance_y * distance_y) < unit1->unit_radius + unit2->unit_radius);
}

iPoint j1Collision::FindClosestWalkable(Unit* unit)
{
	bool found = false;

	iPoint tile = App->map->WorldToMap(unit->GetX(), unit->GetY());
	iPoint origin = tile;

	int dist = 1;

	while (!found)
	{
		tile.y += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.y -= dist;
		tile.y -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.y += dist;
		tile.x += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.x -= dist;
		tile.x -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		//diagonals
		tile.y -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.y += dist;
		tile.y += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.x += dist;
		tile.x += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.y -= dist;
		tile.y -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		//back to origin and increment the tile distance
		tile.x--;
		tile.y++;
		dist++;
	}

	return App->map->MapToWorld(tile.x, tile.y);
}

void j1Collision::SplitUnits(Unit * unit1, Unit * unit2)
{
	unit1->GetPath(FindClosestWalkable(unit1));
	unit1->PopFirstPath();
	unit1->GetNextTile();
	unit1->SetAction(WALK);
	unit1->state = MOVING;
	unit1->state = MOVING_TO_ATTACK;

}

