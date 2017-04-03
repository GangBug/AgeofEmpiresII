#include "Boss.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Map.h"

#define ATTACK_TIMER 1
//TODO MARC: CARGA LA ANIMACION, METELO EN EL SPRITEORDER COMO CON LAS UNIDADES. EL XML YA TIENE EL DIABLO. ARREGLA ERRORES.

Boss::Boss(UNIT_TYPE u_type, fPoint pos, int id) : Unit(u_type, pos, id)
{

}

void Boss::Update()
{
	if (state == NONE)
	{

	}

	else if (state == ATTACKING || enemy != nullptr)
	{
		AttackUnit();
	}
}

void Boss::PostUpdate()
{
	if (GetHP() <= 0)
	{
		state = DEAD;
		this->action_type = DIE;
		App->entity_manager->DeleteUnit(this);
	}
}

bool Boss::CheckSurroundings()
{
	if (logicTimer.ReadSec() > 1) //Calculates each 1s
	{
		frontier.clear();
		visited.clear();//Reset frontier and visited
		logicTimer.Start();//Start timer again

		frontier.push_back(App->map->WorldToMap(GetX(), GetY()));
		visited.push_back(App->map->WorldToMap(GetX(), GetY()));

		iPoint current;

		for (int j = 0; j < range; j++)
		{
			int frontierNum = frontier.size();
			for (int i = 0; i < frontierNum;)
			{
				current = *frontier.begin();
				frontier.pop_front();

				iPoint neighbors[4];
				neighbors[0].x = current.x + unit_radius + 1;
				neighbors[0].y = current.y;
				neighbors[1].x = current.x + unit_radius - 1;
				neighbors[1].y = current.y;
				neighbors[2].x = current.x;
				neighbors[2].y = current.y + unit_radius - 1;
				neighbors[3].x = current.x;
				neighbors[3].y = current.y + unit_radius + 1;

				for (int n = 0; n <= 3; n++)
				{
					if (App->entity_manager->IsUnitInTile(this, neighbors[n]))
					{
						Unit* unit2 = App->entity_manager->GetUnitInTile(neighbors[n]); //TODO MARC: ARREGLA ESTO
						if (unit2->GetAI() != this->AI && state != FLEEING)//If we find a unit and he's not of your army, clear your lists and set him as your enemy! Now you have to find where' you'll fight him 
						{
							frontier.clear();
							visited.clear();
							enemy = unit2;
							SetFightingArea();
							attackingTimer.Start();
							return true;
						}
					}
				}

				bool alreadyVisited = false;
				for (int i = 0; i <= 3; i++)
				{
					if (frontierNum == 1)
					{
						frontier.push_back(iPoint(neighbors[i]));
						visited.push_back(iPoint(neighbors[i]));
					}
					else
					{
						std::list<iPoint>::iterator tmp = visited.begin();
						int visitedNum = visited.size();
						alreadyVisited = false;
						for (int p = 0; p < visitedNum; tmp++, p++)
						{
							if ((*tmp).x == neighbors[i].x && (*tmp).y == neighbors[i].y)
							{
								alreadyVisited = true;
							}
						}
						if (alreadyVisited == false)
						{
							frontier.push_back(iPoint(neighbors[i]));
							visited.push_back(iPoint(neighbors[i]));
						}
					}
				}
				i++;
			}
		}
	}
	return true;
}

bool Boss::AttackUnit()
{
	bool ret = false;

	if (enemy != nullptr && enemy->GetHP() > 0)
	{

		ret = true;

		iPoint enemyPos = App->map->WorldToMap(enemy->GetX(), enemy->GetY());
		iPoint Pos = App->map->WorldToMap(GetX(), GetY());

		if (Pos.DistanceTo(enemyPos) > unit_radius && unit_class != RANGED)
		{
			CheckSurroundings();
		}

		else
		{
			action_type = ATTACK;
			LookAtEnemy();

			if (attackingTimer.ReadSec() > ATTACK_TIMER) //Attack every second
			{
				enemy->SetHp(enemy->GetHP() - attack);//TODO MARC: ARREGLA ESTO
				attackingTimer.Start();
			}
		}
	}

	else
	{
		enemy = nullptr;
		state = NONE;
		action_type = IDLE;
	}

	return ret;
}