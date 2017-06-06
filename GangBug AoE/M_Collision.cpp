#include "App.h"
#include "M_Pathfinding.h"
#include "M_Map.h"
#include "M_Collision.h"
#include "M_EntityManager.h"

M_Collision::M_Collision(bool startEnabled) : Module(startEnabled)
{
	name.assign("collision");
}

M_Collision::~M_Collision() {}

update_status M_Collision::Update(float dt)
{
	bool ret = true;
	std::vector<Entity*> unit1 = app->entityManager->GetUnitVector();
	for (int i = 0; i < unit1.size(); i++)
	{
		float posX, posY;
		unit1[i]->GetGlobalPosition(posX, posY);
		iPoint pos = app->map->WorldToMap(posX, posY);
		Unit* unit_1 = (Unit*)unit1[i];
		//Check colisions between units
		std::vector<Entity*> unit2 = app->entityManager->GetUnitVector();
		for (int j = 0; j < unit2.size(); j++)
		{
			if (unit1[i] != unit2[j])
			{
				if (DoUnitsIntersect((Unit*)unit1[i], (Unit*)unit2[j]) == true)
				{
					//Collision detected
					Unit* unit_2 = (Unit*)unit2[j];
					if ((unit_1->GetAction() == IDLE || (unit_1->GetAction() == ATTACK && unit_1->GetUnitType() == ARCHER)) && (unit_2->GetAction() == IDLE || (unit_2->GetAction() == ATTACK)))
							SplitUnits((Unit*)unit1[i]);
				}
			}

		}
	}
	return UPDATE_CONTINUE;
}

bool M_Collision::DoUnitsIntersect(Unit* unit1, Unit* unit2)
{
	float posX1, posY1, posX2, posY2;
	unit1->GetGlobalPosition(posX1, posY1);
	unit2->GetGlobalPosition(posX2, posY2);
	float distanceX = posX1 - posX2;
	float distanceY = posY1 - posY2;
	return (sqrt(distanceX * distanceX + distanceY * distanceY) < unit1->GetUnitRadius() + unit2->GetUnitRadius());
}

iPoint M_Collision::FindClosestWalkable(Unit* unit)
{
	bool found = false;
	fPoint pos = unit->GetGlobalPosition();
	iPoint tile = app->map->WorldToMap(pos.x, pos.y);
	iPoint origin = tile;

	int dist = 1;

	while (!found && app->pathfinding->IsWalkable(tile))
	{
		tile.y += dist;
		if (app->pathfinding->IsWalkable({ tile.x, tile.y }) == true && app->entityManager->IsUnitInTile(unit, tile) == false && PrevPosition == false && app->pathfinding->IsWalkable(tile) == true)
		{
			found = true;
			PrevPosition = true;
			break;
		}

		tile.y -= dist;
		tile.x -= dist;

		if (app->pathfinding->IsWalkable({ tile.x, tile.y }) == true && app->entityManager->IsUnitInTile(unit, tile) == false && PrevPosition2 == false && app->pathfinding->IsWalkable(tile) == true)
		{
			found = true;
			PrevPosition2 = true;
			break;
		}

		tile.y += dist;
		if (app->pathfinding->IsWalkable({ tile.x, tile.y }) == true && app->entityManager->IsUnitInTile(unit, tile) == false && PrevPosition3 == false && app->pathfinding->IsWalkable(tile) == true)
		{
			found = true;
			PrevPosition3 = true;
			break;
		}

		tile.y -= dist;
		tile.x -= dist;
		if (app->pathfinding->IsWalkable({ tile.x, tile.y }) == true && app->entityManager->IsUnitInTile(unit, tile) == false && app->pathfinding->IsWalkable(tile) == true)
		{
			found = true;
			resetPrevPositions();
			break;
		}

		//diagonals
		tile.y -= dist;
		if (app->pathfinding->IsWalkable({ tile.x, tile.y }) == true && app->entityManager->IsUnitInTile(unit, tile) == false && app->pathfinding->IsWalkable(tile) == true)
		{
			found = true;
			break;
		}

		tile.y += dist;
		tile.y += dist;
		if (app->pathfinding->IsWalkable({ tile.x, tile.y }) == true && app->entityManager->IsUnitInTile(unit, tile) == false && app->pathfinding->IsWalkable(tile) == true)
		{
			found = true;
			break;
		}

		tile.x += dist;
		tile.x += dist;
		if (app->pathfinding->IsWalkable({ tile.x, tile.y }) == true && app->entityManager->IsUnitInTile(unit, tile) == false && app->pathfinding->IsWalkable(tile) == true)
		{
			found = true;
			break;
		}

		tile.y -= dist;
		tile.y -= dist;
		if (app->pathfinding->IsWalkable({ tile.x, tile.y }) == true && app->entityManager->IsUnitInTile(unit, tile) == false && app->pathfinding->IsWalkable(tile) == true)
		{
			found = true;
			break;
		}

		//back to origin and increment the tile distance
		tile.x--;
		tile.y++;
		dist++;
	}

	return app->map->MapToWorld(tile.x, tile.y);
}

void M_Collision::resetPrevPositions()
{
	PrevPosition = false;
	PrevPosition2 = false;
	PrevPosition3 = false;
}

void M_Collision::SplitUnits(Unit * unit1)
{
	unit1->GoTo(FindClosestWalkable(unit1));
}

void M_Collision::DrawDebug()
{
}