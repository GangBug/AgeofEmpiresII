
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "Units.h"
#include "p2Log.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1EntityManager.h"

Unit::Unit(UNIT_TYPE u_type, fPoint pos, int id): Entity(UNIT, pos), unit_type(u_type), direction(WEST), action_type(IDLE), id(id)
{

	switch (u_type)
	{
		//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	case TWOHANDEDSWORDMAN:
		SetHp(600);
		attack = 12;
		SetArmor(1);
		speed = 0.9;
		rate_of_fire = 2;
		range = 1;
		unit_class = INFANTRY;
		unit_radius = 5;
		AI = false;
		state = NONE;
		break;

	case TWOHANDEDSWORDMANENEMY:
		SetHp(80);
		attack = 12;
		SetArmor(1);
		speed = 0.9;
		rate_of_fire = 2;
		range = 1;
		unit_class = INFANTRY;
		unit_radius = 5;
		AI = true;
		state = NONE;
		direction = EAST;
		break;


	case CAVALRYARCHER:
		SetHp(50);
		attack = 8;
		SetArmor(1);
		speed = 1.8;
		rate_of_fire = 2;
		range = 4;
		unit_class = RANGED;
		unit_radius = 8;
		AI = false;
		state = NONE;
		break;

	
	case ARCHER:
		SetHp(40);
		attack = 10;
		SetArmor(1);
		speed = 0.9;
		rate_of_fire = 2;
		range = 4;
		unit_class = RANGED;
		unit_radius = 8;
		AI = false;
		state = NONE;
		break;

	case SIEGERAM:
		SetHp(270);
		attack = 4;
		SetArmor(-5);
		speed = 0.6;
		rate_of_fire = 5;
		range = 1;
		unit_class = SIEGE;
		AI = false;
		state = NONE;
		break;

	default:
		LOG("Error UNIT TYPE STATS NULL");
		unit_class = NO_CLASS;
		break;
	}
}

void Unit::Update()
{
	if (!AI)
	{
		if (state == NONE) 
		{
			CheckSurroundings();
		}

		if (App->input->GetMouseButtonDown(3) == KEY_DOWN && this->GetEntityStatus() == E_SELECTED)
		{
			this->path_list.clear();
			App->input->GetMousePosition(destination.x, destination.y);
			destination.x -=  App->render->camera->GetPosition().x;
			destination.y -=  App->render->camera->GetPosition().y;

			destin = { destination };

			if (this->GetPath({ destination.x, destination.y }) != -1)
			{
				arrived = false;
				path_list.pop_front();
				GetNextTile();
				this->action_type = WALK;
				if (state == NONE) 
				{
					state = MOVING;
				}
				else 
				{
					state = FLEEING;
					enemy = nullptr;
				}
			}
			else
			{
				state = NONE;
				this->action_type = IDLE;
			}
		}

		if (state == MOVING)
		{
			Move();
			CheckSurroundings();
		}

		if (state == MOVING_TO_ATTACK || state == FLEEING)
		{
			Move();
		}

		if (state == ATTACKING)
		{
			AttackUnit();
		}
	}

	else if (AI)
	{
		DoAI();
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && this->GetEntityStatus() == E_SELECTED)
	{
		debug = !debug;
	}

	DrawDebugRadius();
	Draw();

}

void Unit::PostUpdate()
{
	if (enemy != nullptr && enemy->state == DEAD)
	{
		enemy = nullptr;
	}

	if (GetHP() <= 0) {
		state = DEAD;
		this->action_type = DIE;
		App->entity_manager->DeleteUnit(this);
	}

	
}

void Unit::Move()
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

void Unit::DoAI()
{
	if (state == NONE)
	{
		CheckSurroundings();
	}

	if (state == ATTACKING) 
	{
		AttackUnit();
	}

	if (state == MOVING || state == MOVING_TO_ATTACK)
	{
		Move();
	}
}

void Unit::Draw()
{
	SDL_Texture* tex = App->anim->GetTexture(unit_type);
	SDL_Rect rect;
	iPoint pivot;

	App->anim->GetAnimationFrame(rect, pivot, this);

	if (direction == NORTH_EAST || direction == EAST || direction == SOUTH_EAST)
		App->render->Blit(tex, GetX(), GetY(), &rect, SDL_FLIP_HORIZONTAL, pivot.x, pivot.y);
	else
		App->render->Blit(tex, GetX() - pivot.x, GetY() - pivot.y, &rect);

	if (this->GetEntityStatus() == E_SELECTED)
		App->render->DrawCircle(this->GetX() +  App->render->camera->GetPosition().x, this->GetY() + App->render->camera->GetPosition().y, this->unit_radius, 255, 255, 255);
}

const DIRECTION Unit::GetDir() const
{
	return direction;
}

const UNIT_TYPE Unit::GetUnitType() const
{
	return unit_type;
}

const ACTION_TYPE Unit::GetActionType() const
{
	return action_type;
}

void Unit::SetAction(const ACTION_TYPE action)
{
	action_type = action;
}

int Unit::GetPath(iPoint dest)
{
	iPoint ori = App->map->WorldToMap(GetX(), GetY());
	iPoint destinat = App->map->WorldToMap(dest.x, dest.y);
	return App->pathfinding->CreatePath(ori, destinat, path_list);
}

void Unit::PopFirstPath()
{
	path_list.pop_front();
}

void Unit::AddPath(iPoint new_goal)
{
	path_list.push_back(new_goal);
}

bool Unit::GetNextTile()
{
	bool ret = true;

	if (path_list.size() == 0)
		return false;

	path_objective = App->map->MapToWorld(path_list.front().x, path_list.front().y);
	path_list.pop_front();

	move_vector.x = (float)path_objective.x - GetX();
	move_vector.y = (float)path_objective.y - GetY();
	float module = (sqrt(move_vector.x*move_vector.x + move_vector.y * move_vector.y));
	move_vector.x = move_vector.x / module;
	move_vector.y = move_vector.y / module;
	float ang_test = (float)RAD_TO_DEG * atan2(-move_vector.y, move_vector.x);
	LOG("ang_test: %f", ang_test);

	iPoint direction_vec;
	direction_vec.x = path_objective.x - GetX();
	direction_vec.y = GetY() - path_objective.y;
	angle = (float)RAD_TO_DEG * atan2(direction_vec.y, direction_vec.x);

	if (angle < 0)
		angle += 360;


	if ((0 <= angle &&  angle <= 22.5) || (337.5 <= angle&& angle <= 360))
		this->direction = EAST;

	else if (22.5 <= angle &&  angle <= 67.5)
		this->direction = NORTH_EAST;

	else if (67.5 <= angle &&  angle <= 112.5)
		this->direction = NORTH;

	else if (112.5 <= angle &&  angle <= 157.5)
		this->direction = NORTH_WEST;

	else if (157.5 <= angle &&  angle <= 202.5)
		this->direction = WEST;

	else if (202.5 <= angle &&  angle <= 247.5)
		this->direction = SOUTH_WEST;

	else if (247.5 <= angle &&  angle <= 292.5)
		this->direction = SOUTH;

	else if (292.5 <= angle &&  angle <= 337.5)
		this->direction = SOUTH_EAST;

	else
		this->direction = NO_DIRECTION;

	return ret;
}

bool Unit::AttackUnit()
{
	bool ret = false;

	if (enemy != nullptr && enemy->GetHP() > 0) 
	{

		ret = true;

		iPoint enemyPos = App->map->WorldToMap(enemy->GetX(), enemy->GetY());
		iPoint Pos = App->map->WorldToMap(GetX(), GetY());

		if (Pos.DistanceTo(enemyPos) > 1 && unit_class != RANGED)
		{
				SetFightingArea();
		}

		else 
		{
			action_type = ATTACK;
			LookAtEnemy();

			if (attackingTimer.ReadSec() > 1) //Attack every second
			{
				enemy->SetHp(enemy->GetHP() - attack);
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

bool Unit::CheckSurroundings()
{
	if (logicTimer.ReadSec() > 1) //Calculates each 0.5s
	{ 
		frontier.clear();
		visited.clear();//Reset frontier and visited
		logicTimer.Start();//Start timer again

		frontier.push_back(App->map->WorldToMap(GetX(), GetY()));
		visited.push_back(App->map->WorldToMap(GetX(), GetY()));

		iPoint current;

		for (int j = 0; j < unit_radius; j++) 
		{
			int frontierNum = frontier.size();
			for (int i = 0; i < frontierNum;) 
			{
				current = *frontier.begin();
				frontier.pop_front();

				iPoint neighbors[4];
				neighbors[0].x = current.x + 1;
				neighbors[0].y = current.y;
				neighbors[1].x = current.x - 1;
				neighbors[1].y = current.y;
				neighbors[2].x = current.x;
				neighbors[2].y = current.y - 1;
				neighbors[3].x = current.x;
				neighbors[3].y = current.y + 1;

				for (int n = 0; n <= 3; n++)
				{
					if (App->entity_manager->IsUnitInTile(this, neighbors[n]))
					{
						Unit* unit2 = App->entity_manager->GetUnitInTile(neighbors[n]);
						if (unit2->AI != this->AI && state!=FLEEING)//If we find a unit and he's not of your army, clear your lists and set him as your enemy! Now you have to find where' you'll fight him 
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

bool Unit::SetFightingArea()
{
	bool ret;

	if (enemy != nullptr && arrived == true && state != FLEEING)
	{
		state = MOVING_TO_ATTACK;

		iPoint enemyPos = App->map->WorldToMap(enemy->GetX(), enemy->GetY());
		iPoint Pos = App->map->WorldToMap(GetX(), GetY());

		iPoint distance = Pos - enemyPos;

		//UNIT
		if ((enemy->enemy == nullptr || enemy->enemy == this) && enemy->state != FLEEING) {//If the enemy's enemy is... YOU or a nullptr 

			enemy->enemy = this;

			if (unit_class == RANGED && enemy->unit_class == RANGED)
			{
				state = ATTACKING; //If you're ranged, just attack
				enemy->state = ATTACKING;
			}
			else if (unit_class == RANGED && enemy->unit_class != RANGED) //If you're ranged, attack and set the enemy next to you.
			{
				state = ATTACKING;
				//Then calculate new enemy pos
				iPoint newEnemyPos;
				if (this->GetFreeAdjacent(newEnemyPos))
				{
					enemy->path_list.clear();
					if (enemy->GetPath(App->map->MapToWorld(newEnemyPos.x, newEnemyPos.y)) != -1)
					{
						enemy->path_list.pop_front();
						enemy->GetNextTile();
						enemy->action_type = WALK;
						ret = false;
					}
					else
					{
						enemy->state = NONE;
					}
				}
				else
				{
					enemy->state = NONE;
				}
			}
			else if (unit_class != RANGED && enemy->unit_class == RANGED)
			{
				enemy->state = ATTACKING;

				iPoint newPos;
				enemy->GetFreeAdjacent(newPos);
				path_list.clear();
				if (GetPath(App->map->MapToWorld(newPos.x, newPos.y)) != -1)
				{
					path_list.pop_front();
					GetNextTile();
					action_type = WALK;
					ret = false;
				}
				else
				{
					state = NONE;
				}
			}
			else if (unit_class != RANGED && enemy->unit_class != RANGED) //If both units are melee, the first one will set a tile in between them and the other one will go to and adjacent one
			{
				//YOUR PATH
				iPoint newPos;
				newPos.x = Pos.x - distance.x*0.5;
				newPos.y = Pos.y - distance.y*0.5;
				this->path_list.clear();
				if (this->GetPath(App->map->MapToWorld(newPos.x, newPos.y)) != -1)
				{
					path_list.pop_front();
					GetNextTile();
					this->action_type = WALK;
					ret = false;
				}
				else
				{
					state = NONE;
				}

				//ENEMY PATH
				iPoint newEnemyPos;
				enemy->GetAdjacentTile(newPos, newEnemyPos);
				enemy->path_list.clear();
				if (enemy->GetPath(App->map->MapToWorld(newEnemyPos.x, newEnemyPos.y)) != -1)
				{
					enemy->state = MOVING_TO_ATTACK;
					enemy->path_list.pop_front();
					enemy->GetNextTile();
					enemy->action_type = WALK;
					ret = false;
				}
				else
				{
					enemy->state = NONE;
				}
			}
		}

		if (enemy->state == FLEEING) {//If the enemy's fleeing

			if (unit_class == RANGED)
			{
				state = ATTACKING; //If you're ranged, just attack
			}
			else if (unit_class != RANGED) // If not we'll go next to his tile
			{
				state = MOVING_TO_ATTACK;
				iPoint newPos;
				enemy->GetFreeAdjacent(newPos);
				path_list.clear();
				if (GetPath(App->map->MapToWorld(newPos.x, newPos.y)) != -1)
				{
					path_list.pop_front();
					GetNextTile();
					action_type = WALK;
					ret = false;
				}
				else
				{
					state = NONE;
				}
			}
		}

		else if (enemy->enemy != nullptr && enemy->enemy != this) //Enemy is already in combat with someone else, WE WON'T MOVE HIM!
		{
			if (unit_class == RANGED)
			{
				state = ATTACKING;
			}
			else
			{
				if (enemy->state == ATTACKING)  //If he's attacking we'll move to a tile next to him if possible.
				{
					iPoint newPos;
					if (enemy->GetFreeAdjacent(newPos))
					{
						path_list.clear();
						if (GetPath(App->map->MapToWorld(newPos.x, newPos.y)) != -1)
						{
							state = MOVING_TO_ATTACK;
							path_list.pop_front();
							GetNextTile();
							action_type = WALK;
							ret = false;
						}
						else
						{
							state = NONE;
						}
					}
					else
					{
						action_type = IDLE;
						state = NONE;
					}
				}

				else if (enemy->state == MOVING_TO_ATTACK || enemy->state == FLEEING) //If he's moving, we'll move next to the tile where he's going
				{
					iPoint newPos;
					if (this->GetAdjacentTile(enemy->destination, newPos))
					{
						path_list.clear();
						if (GetPath(App->map->MapToWorld(newPos.x, newPos.y)) != -1)
						{
							state = MOVING_TO_ATTACK;
							path_list.pop_front();
							GetNextTile();
							action_type = WALK;
							ret = false;
						}
						else
						{
							state = NONE;
						}
					}
					else
					{
						action_type = IDLE;
						state = NONE;
					}
				}
			}
		}
	}
	return true;
}

//Returns the direction to face the enemy 
void Unit::LookAtEnemy()
{
	if (enemy != nullptr)
	{

		iPoint direction_vec;
		direction_vec.x = enemy->GetX() - GetX();
		direction_vec.y = GetY() - enemy->GetY();

		angle = (float)RAD_TO_DEG * atan2(direction_vec.y, direction_vec.x);

		if (angle < 0)
			angle += 360;


		if ((0 <= angle &&  angle <= 22.5) || (337.5 <= angle&& angle <= 360))
			this->direction = EAST;

		else if (22.5 <= angle &&  angle <= 67.5)
			this->direction = NORTH_EAST;

		else if (67.5 <= angle &&  angle <= 112.5)
			this->direction = NORTH;

		else if (112.5 <= angle &&  angle <= 157.5)
			this->direction = NORTH_WEST;

		else if (157.5 <= angle &&  angle <= 202.5)
			this->direction = WEST;

		else if (202.5 <= angle &&  angle <= 247.5)
			this->direction = SOUTH_WEST;

		else if (247.5 <= angle &&  angle <= 292.5)
			this->direction = SOUTH;

		else if (292.5 <= angle &&  angle <= 337.5)
			this->direction = SOUTH_EAST;
	}
}

//Returns a tile unnocupied next to the unit.
bool Unit::GetFreeAdjacent(iPoint& Adjacent) const
{
	iPoint ret = App->map->WorldToMap(GetX(), GetY());

	if ((!App->entity_manager->IsUnitInTile(this, { ret.x + 1, ret.y })) && App->pathfinding->IsWalkable({ ret.x + 1, ret.y }))
	{
		Adjacent = { ret.x + 1, ret.y };
		return true;
	}
	else if ((!App->entity_manager->IsUnitInTile(this, { ret.x - 1, ret.y })) && App->pathfinding->IsWalkable({ ret.x - 1, ret.y }))
	{
		Adjacent = { ret.x - 1, ret.y };
		return true;
	}
	else if ((!App->entity_manager->IsUnitInTile(this, { ret.x, ret.y + 1 })) && App->pathfinding->IsWalkable({ ret.x, ret.y + 1 }))
	{
		Adjacent = { ret.x, ret.y + 1};
		return true;
	}
	else if ((!App->entity_manager->IsUnitInTile(this, { ret.x, ret.y - 1 })) && App->pathfinding->IsWalkable({ ret.x, ret.y - 1 }))
	{
		Adjacent = { ret.x, ret.y - 1};
		return true;
	}

	else
	{
		return false;
	}
}

//Gets an unnocupied tile next to the "tile"
bool Unit::GetAdjacentTile(iPoint tile, iPoint& Adjacent) const
{

	if ((!App->entity_manager->IsUnitInTile(this, { tile.x + 1, tile.y })) && App->pathfinding->IsWalkable({ tile.x + 1, tile.y }))
	{
		Adjacent = { tile.x + 1, tile.y };
		return true;
	}
	else if ((!App->entity_manager->IsUnitInTile(this, { tile.x - 1, tile.y })) && App->pathfinding->IsWalkable({ tile.x - 1, tile.y }))
	{
		Adjacent = { tile.x - 1, tile.y };
		return true;
	}
	else if ((!App->entity_manager->IsUnitInTile(this, { tile.x, tile.y + 1 })) && App->pathfinding->IsWalkable({ tile.x, tile.y + 1 }))
	{
		Adjacent = { tile.x, tile.y + 1 };
		return true;
	}
	else if ((!App->entity_manager->IsUnitInTile(this, { tile.x, tile.y - 1 })) && App->pathfinding->IsWalkable({ tile.x, tile.y - 1 }))
	{
		Adjacent = { tile.x, tile.y - 1 };
		return true;
	}

	else
	{
		return false;
	}
}

void Unit::DrawDebugRadius()
{
	// Print DEBUG 
	if (debug)
	{
		// Draw Visited
		iPoint point;
		std::list<iPoint>::iterator item = visited.begin();

		while (item != visited.end())
		{
			point = (*item);
			TileSet* tileset = App->map->GetTilesetFromTileId(129);

			SDL_Rect r = tileset->GetTileRect(129);
			iPoint pos = App->map->MapToWorld(point.x, point.y);

			App->render->Blit(App->tex->debugTex, pos.x - 32, pos.y - 32, &r);

			item++;
		}

		// Draw frontier
		for (std::list<iPoint>::iterator item2 = frontier.begin(); item2 != frontier.end(); item2++)
		{
			point = (*item2);
			TileSet* tileset = App->map->GetTilesetFromTileId(130);

			SDL_Rect r = tileset->GetTileRect(130);
			iPoint pos = App->map->MapToWorld(point.x, point.y);

			App->render->Blit(App->tex->debugTex, pos.x - 32, pos.y - 32, &r);
		}
	}
}