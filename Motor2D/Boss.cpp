#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "Boss.h"
#include "p2Log.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1EntityManager.h"

#define ATTACK_TIMER 1
//TODO MARC: CARGA LA ANIMACION, METELO EN EL SPRITEORDER COMO CON LAS UNIDADES. EL XML YA TIENE EL DIABLO. ARREGLA ERRORES.

Boss::Boss(UNIT_TYPE u_type, fPoint pos, int id) : Unit(u_type, pos, id)
{
	switch (u_type)
	{
	case BOSS:
		SetHp(BOSSHP);
		attack = 12;
		SetArmor(1);
		speed = 0.9;
		rate_of_fire = 2;
		range = 1;
		unit_class = INFANTRY;
		unit_radius = 2;
		AI = true;
		state = NONE;
		action_type = IDLE;
		break;

	default:
		LOG("Error UNIT TYPE STATS NULL");
		unit_class = NO_CLASS;
		break;
	}
}

void Boss::Update()
{
	DoAI();

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && this->GetEntityStatus() == E_SELECTED)
	{
		debug = !debug;
	}
	switch (wave)
	{
	case 1:
		if (this->GetHP() <= BOSSHP*0.2)
		{
			ZergCreate();
			ZergCreate(2);
			wave--;
		}
		break;
	case 2:
		if (this->GetHP() <= BOSSHP*0.4) {
			ZergCreate();
			wave--;
		}

		break;
	case 3:
		if (this->GetHP() <= BOSSHP*0.6) {
			ZergCreate(2);
			wave--;
		}

		break;
	case 4:
		if (this->GetHP() <= BOSSHP*0.8) {
			ZergCreate();
			wave--;
		}

		break;
	default:
		break;
	}



	DrawDebugRadius();
	App->render->SpriteOrdering(this);
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

void Boss::Move()
{
	this->SetPosition(GetX() + move_vector.x*speed, GetY() + move_vector.y*speed);

	iPoint unit_world;
	unit_world.x = GetX();
	unit_world.y = GetY();

	if (destin.x - 35 < unit_world.x && destin.x + 35 > unit_world.x && destin.y - 35 < unit_world.y && destin.y + 35 > unit_world.y && arrived == false)
	{
		LOG("TRUE");
		arrived = true;
	}

	if (path_objective.DistanceTo(unit_world) < 3)
	{
		//center the unit to the tile
		this->SetPosition(path_objective.x, path_objective.y);
		if (!GetNextTile())
		{
			if (state == MOVING_TO_ATTACK)
			{
				state = ATTACKING;
				this->action_type = IDLE;
			}
			else
			{
				state = NONE;
				this->action_type = IDLE;
			}
		}
	}
}

bool Boss::ZergCreate(int i)const
{
	bool ret = true;

	App->entity_manager->CreateUnit(VILE, fPoint(this->GetX() + 0, this->GetY() + 150 * i));
	App->entity_manager->CreateUnit(VILE, fPoint(this->GetX() + 0, this->GetY() - 150 * i));
	App->entity_manager->CreateUnit(VILE, fPoint(this->GetX() + 150 * i, this->GetY() + 0));
	App->entity_manager->CreateUnit(VILE, fPoint(this->GetX() - 150 * i, this->GetY() - 0));

	App->entity_manager->CreateUnit(VILE, fPoint(this->GetX() + 100 * i, this->GetY() + 100 * i));
	App->entity_manager->CreateUnit(VILE, fPoint(this->GetX() - 100 * i, this->GetY() - 100 * i));
	App->entity_manager->CreateUnit(VILE, fPoint(this->GetX() - 100 * i, this->GetY() + 100 * i));
	App->entity_manager->CreateUnit(VILE, fPoint(this->GetX() + 100 * i, this->GetY() - 100 * i));

	return true;
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
						if (unit2->GetAI() != this->AI && state != FLEEING && enemy != unit2)//If we find a unit and he's not of your army, clear your lists and set him as your enemy! Now you have to find where' you'll fight him 
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
	return false;
}

bool Boss::AttackUnit()
{
	bool ret = false;

	if (enemy != nullptr && enemy->GetHP() > 0)
	{

		ret = true;

		iPoint enemyPos = App->map->WorldToMap(enemy->GetX(), enemy->GetY());
		iPoint Pos = App->map->WorldToMap(GetX(), GetY());

		if (Pos.DistanceTo(enemyPos) > unit_radius)
		{
			if (!CheckSurroundings())
			{
				//SetFightingArea();
			}
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