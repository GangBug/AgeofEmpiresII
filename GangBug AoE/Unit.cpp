#include "Unit.h"
#include "M_Animation.h"
#include "M_Map.h"
#include "M_Pathfinding.h"
#include "M_FogOfWar.h"
#include "M_Audio.h"
#include "M_EntityManager.h"
#include "M_DialogueManager.h"
#include "M_MisionManager.h"
//TMP
#include "M_Input.h"
#include "App.h"
#include "Log.h"

Unit::Unit(unit_type type, Entity* parent) : unitType(type), Entity(ENTITY_UNIT, parent)
{
	name.assign("unit_");
	action = IDLE;
	unitDirection = SOUTH;
	entityTexture = app->animation->GetTexture(unitType);
	target = nullptr;


	switch (type)
	{
		//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	case TARKAN_KNIGHT:
		SetHp(160);
		attack = 15;
		speed = 1.6f;
		rate_of_fire = 1;
		range = 0;
		unitClass = CAVALRY;
		unitRadius = 4;
		horde = false;
		unitState = NO_STATE;
		break;

	case SAMURAI:
		SetHp(100);
		attack = 15;
		speed = 1.2f;
		rate_of_fire = 1;
		range = 0;
		unitClass = INFANTRY;
		unitRadius = 5;
		horde = true;
		unitState = NO_STATE;
		break;

	case ARCHER:
		SetHp(30);
		attack = 15;
		speed = 1.2f;
		rate_of_fire = 2;
		range = 5;
		unitClass = RANGED;
		unitRadius = 8;
		horde = false;
		unitState = NO_STATE;
		break;

	case VILE:
		SetHp(50);
		attack = 5;
		speed = 2.0f;
		rate_of_fire = 1;
		range = 0;
		unitClass = INFANTRY;
		unitRadius = 7;
		horde = true;
		unitState = NO_STATE;
		break;

	default:
		LOG("ERROR: NOT A CORRECT UNIT TYPE");
		unitClass = NO_CLASS;
		break;
	}

	visionArea.SetRad(300);
	renderArea.SetRad(300 + RENDER_MARGIN);
}

void Unit::OnUpdate(float dt)
{
	if (!app->dialogueManager->onDialogue)
	{
		if (GetHP() > 0) {
			switch (unitState)
			{
			case NO_STATE:
				if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
				{
					iPoint objective;
					app->input->GetMouseMapPosition(objective.x, objective.y);
					GoTo(objective);
				}
				if (target != nullptr && target->GetHP() > 0)
				{
					SetFightingArea();
				}
				break;
			case MOVING:
				if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
				{
					iPoint objective;
					app->input->GetMouseMapPosition(objective.x, objective.y);
					GoTo(objective);
				}
				else if (Move() == false)
				{
					unitState = NO_STATE;
					action = IDLE;
				}
				break;
			case MOVING_TO_ATTACK:
				if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
				{
					iPoint objective;
					app->input->GetMouseMapPosition(objective.x, objective.y);
					GoTo(objective);
				}
				else if (Move() == false)
				{
					unitState = ATTACKING;
					action = ATTACK;
				}
				break;
			case ATTACKING:
				if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
				{
					iPoint objective;
					app->input->GetMouseMapPosition(objective.x, objective.y);
					GoTo(objective);
				}
				if (!AttackUnit())
				{
					target = nullptr;
					unitState = NO_STATE;
					action = IDLE;
				}
				break;
			}
			CheckSurroundings();
		}

		else
		{
			Die();
		}
		iPoint p;
		app->animation->GetFrame(drawQuad, p, this);
		SetPivot(p);

		//FOG OF WAR
		float x, y;
		GetGlobalPosition(x, y);
		visionArea.SetPosition(iPoint(x, y));
		renderArea.SetPosition(iPoint(x, y));

		//app->fogOfWar->ClearAlphaLayer(visionArea, 200, true);
		//app->fogOfWar->ClearAlphaLayer(visionArea, MID_ALPHA);
		//app->fogOfWar->ClearFogLayer(renderArea, GRAY_FOG);
	}
}

unit_type Unit::GetType() const
{
	return unitType;
}

action_type Unit::GetAction() const
{
	return action;
}

direction Unit::GetDir() const
{
	return unitDirection;
}

unit_type Unit::GetUnitType() const
{
	return unitType;
}

unit_class Unit::GetUnitClass() const
{
	return unitClass;
}

int Unit::GetUnitRadius() const
{
	return unitRadius;
}

bool Unit::GetPath(iPoint dest) 
{
	float posX, posY;
	GetGlobalPosition(posX, posY);
	iPoint ori = app->map->WorldToMap(posX, posY);
	iPoint destinat = app->map->WorldToMap(dest.x, dest.y);
	if (ori == destinat) 
	{
		return true;
	}
	if (app->pathfinding->CalculatePath(ori, destinat, pathVec) == false)
		return false;
	return true;
}

int Unit::GetAttack() const
{
	return attack;
}

int Unit::GetRange() const
{
	return range;
}

Circle Unit::GetVisionArea() const
{
	return visionArea;
}

bool Unit::IsMoving() const
{
	return action == WALK;
}

bool Unit::Move()
{
	float posX, posY;
	GetGlobalPosition(posX, posY);

	this->SetGlobalPosition(posX + moveVector.x*speed, posY + moveVector.y*speed);

	if (pathObjective.DistanceTo(iPoint(posX, posY)) < 3)
	{
		//center the unit to the tile
		if (!GetNextTile())
			return false;
	}

	//app->fogOfWar->ClearFogLayer(renderArea, NO_FOG);
	//app->fogOfWar->ClearFogLayer(visionArea, GRAY_FOG);

	return true;
}

int Unit::GetHP() const
{
	return hp;
}

void Unit::DoDamage(int dmg)
{
	hp -= dmg;
}

void Unit::Die()
{
	if (action != DIE && action != DISAPPEAR)
	{
		action = DIE;
		Animation* anim = app->animation->GetAnimation(GetUnitType(), action, unitDirection);
		anim->Reset();
	}
	//FIX: This deletes the building associated with the dying unit
	else if (app->animation->GetAnimation(GetUnitType(), action, unitDirection)->Finished() == true && action == DIE)
	{
		this->action = DISAPPEAR; //CURRENTLY NOT WORKING 
		Animation* anim = app->animation->GetAnimation(GetUnitType(), action, unitDirection);
		anim->Reset();
	}
	else if (app->animation->GetAnimation(GetUnitType(), action, unitDirection)->Finished() == true && action == DISAPPEAR)
	{
		if (this->horde == true) {
			app->misionManager->AddEnemyDeadUnit();
		}

		app->entityManager->DeleteUnit(this);
	}
}

int Unit::GetPriority() const
{
	return priority;
}

void Unit::SetAction(action_type action)
{
	this->action = action;
}

bool Unit::GetNextTile()
{
	bool ret = true;

	if (pathVec.size() <= 0)
		return false;

	pathObjective = app->map->MapToWorld(pathVec.back().x, pathVec.back().y);
	pathVec.pop_back();

	float posX, posY;
	GetGlobalPosition(posX, posY);

	moveVector.x = (float)pathObjective.x - posX;
	moveVector.y = (float)pathObjective.y - posY;

	float modul = (sqrt(moveVector.x*moveVector.x + moveVector.y * moveVector.y));

	moveVector.x = moveVector.x / modul;
	moveVector.y = moveVector.y / modul;

	LookAt(pathObjective);

	return ret;
}

Entity* Unit::CheckSurroundings()
{
	Entity* ret = nullptr;
	int distance = 0;
	std::vector<Entity*> vec = app->entityManager->GetUnitVector();

	iPoint unitPos = app->map->WorldToMap(GetGlobalPosition().x, GetGlobalPosition().y);


	for (std::vector<Entity*>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		if ((*it) != this)
		{
			iPoint itPos = app->map->WorldToMap((*it)->GetGlobalPosition().x, (*it)->GetGlobalPosition().y);
			if (itPos.x <= unitPos.x + unitRadius && itPos.x >= unitPos.x - unitRadius && itPos.y <= unitPos.y + unitRadius && itPos.y >= unitPos.y - unitRadius && (*it)->GetHP() > 0)
			{
				Unit* challenger = dynamic_cast<Unit*>(*it);
				if (challenger->horde != this->horde)
				{
					int itDistance = unitPos.DistanceTo(itPos);
					if (itDistance < distance || distance == 0)
					{
						ret = (*it);
						target = (*it);
					}
				}
			}
		}
	}
	return ret;
}

//Only use this for enemy units
bool Unit::SetFightingArea()
{
	bool ret;

	if (target != nullptr && unitState != FLEEING)
	{
		unitState = MOVING_TO_ATTACK;

		Unit* enemy = dynamic_cast<Unit*>(target);

		iPoint enemyPos = app->map->WorldToMap(enemy->GetGlobalPosition().x, enemy->GetGlobalPosition().y);

		iPoint Pos = app->map->WorldToMap(GetGlobalPosition().x, GetGlobalPosition().y);

		iPoint distance = Pos - enemyPos;

		//UNIT

		if ((enemy->target == nullptr || enemy->target == this) && enemy->unitState != FLEEING) {//If the enemy's enemy is... YOU or a nullptr 

			enemy->target = this;
			if (unitClass == RANGED && enemy->unitClass == RANGED)
			{
				unitState = ATTACKING; //If you're ranged, just attack

				enemy->unitState = ATTACKING;
			}

			else if (unitClass == RANGED && enemy->unitClass != RANGED) //If you're ranged, attack and set the enemy next to you.
			{
				unitState = ATTACKING;
				//Then calculate new enemy pos
				iPoint newEnemyPos;

				if (this->GetFreeAdjacent(newEnemyPos))
				{
					enemy->pathVec.clear();
					if (enemy->GoTo(app->map->MapToWorld(newEnemyPos.x, newEnemyPos.y)) != false)
					{
						ret = false;
					}

					else
					{
						enemy->unitState = NO_STATE;
					}

				}
				else

				{
					enemy->unitState = NO_STATE;
				}
			}

			else if (unitClass != RANGED && enemy->unitClass == RANGED)
			{

				enemy->unitState = ATTACKING;
				iPoint newPos;
				enemy->GetFreeAdjacent(newPos);
				pathVec.clear();

				if (GoTo(app->map->MapToWorld(newPos.x, newPos.y)) != false)

				{
					ret = false;
				}

				else
				{
					unitState = NO_STATE;
				}

			}

			else if (unitClass != RANGED && enemy->unitClass != RANGED) //If both units are melee, the first one will set a tile in between them and the other one will go to and adjacent one
			{
				//YOUR PATH
				iPoint newPos;
				newPos.x = Pos.x - distance.x*0.5;
				newPos.y = Pos.y - distance.y*0.5;
				pathVec.clear();

				if (this->GoTo(app->map->MapToWorld(newPos.x, newPos.y)) != -1)
				{
					ret = false;
				}
				else
				{
					unitState = NO_STATE;
				}

				//ENEMY PATH
				iPoint newEnemyPos;
				enemy->GetAdjacentTile(newPos, newEnemyPos);
				enemy->pathVec.clear();

				if (enemy->GoTo(app->map->MapToWorld(newEnemyPos.x, newEnemyPos.y)) != -1)
				{
					ret = false;
				}

				else
				{
					enemy->unitState = NO_STATE;
				}
			}
		}

		if (enemy->unitState == FLEEING) {//If the enemy's fleeing

			if (unitClass == RANGED)
			{
				unitState = ATTACKING; //If you're ranged, just attack
			}
			else if (unitClass != RANGED) // If not we'll go next to his tile
			{
				unitState = MOVING_TO_ATTACK;

				iPoint newPos;
				enemy->GetFreeAdjacent(newPos);
				pathVec.clear();

				if (GoTo(app->map->MapToWorld(newPos.x, newPos.y)) != -1)
				{
					ret = false;
				}
				else
				{
					unitState = NO_STATE;
				}
			}
		}

		else if (enemy->target != nullptr && enemy->target != this) //Enemy is already in combat with someone else, WE WON'T MOVE HIM!
		{
			if (unitClass == RANGED)
			{
				unitState = ATTACKING;
			}

			else
			{
				if (enemy->unitState == ATTACKING)  //If he's attacking we'll move to a tile next to him if possible.
				{
					iPoint newPos;

					if (enemy->GetFreeAdjacent(newPos))
					{
						pathVec.clear();

						if (GoTo(app->map->MapToWorld(newPos.x, newPos.y)) != -1)
						{
							unitState = MOVING_TO_ATTACK;
							ret = false;
						}
						else
						{
							unitState = NO_STATE;
						}
					}

					else
					{
						action = IDLE;
						unitState = NO_STATE;
					}
				}

				else if (enemy->unitState == MOVING_TO_ATTACK || enemy->unitState == FLEEING) //If he's moving, we'll move next to the tile where he's going
				{
					iPoint newPos;
					if (this->GetAdjacentTile(enemy->destination, newPos))
					{
						pathVec.clear();

						if (GoTo(app->map->MapToWorld(newPos.x, newPos.y)) != -1)
						{
							unitState = MOVING_TO_ATTACK;
							ret = false;
						}

						else
						{
							unitState = NO_STATE;
						}
					}

					else
					{
						action = IDLE;
						unitState = NO_STATE;
					}
				}
			}
		}
	}
	return true;
}

void Unit::LookAt(iPoint pos)
{

	float posX, posY;
	GetGlobalPosition(posX, posY);
	iPoint directionVec;
	directionVec.x = pos.x - posX;
	directionVec.y = posY - pos.y;
	angle = (float)57.29577951 * atan2(directionVec.y, directionVec.x);

	if (angle < 0)
		angle += 360;

	if ((0 <= angle &&  angle <= 22.5) || (337.5 <= angle&& angle <= 360))
	{
		this->unitDirection = EAST;
	}

	else if (22.5 <= angle &&  angle <= 67.5)
	{
		this->unitDirection = NORTH_EAST;
	}

	else if (67.5 <= angle &&  angle <= 112.5)
	{
		this->unitDirection = NORTH;
	}

	else if (112.5 <= angle &&  angle <= 157.5)
	{
		this->unitDirection = NORTH_WEST;
	}

	else if (157.5 <= angle &&  angle <= 202.5)
	{
		this->unitDirection = WEST;
	}

	else if (202.5 <= angle &&  angle <= 247.5)
	{
		this->unitDirection = SOUTH_WEST;
	}

	else if (247.5 <= angle &&  angle <= 292.5)
	{
		this->unitDirection = SOUTH;
	}

	else if (292.5 <= angle &&  angle <= 337.5)
	{
		this->unitDirection = SOUTH_EAST;
	}
}

bool Unit::GoTo(iPoint destination)
{
	if (this->GetPath({ destination.x, destination.y }) != false)
	{
		GetNextTile();
		this->action = WALK;
		if (unitState != MOVING_TO_ATTACK)
		{
			this->unitState = MOVING;
		}
		this->destination.x = destination.x;
		this->destination.y = destination.y;
		return true;
	}
	return false;
}

bool Unit::ChangeDirection(iPoint destination)
{
	if (this->GetPath(destination) != true)
	{
		GetNextTile();
		this->destination.x = destination.x;
		this->destination.y = destination.y;
		return true;
	}
	return false;
}

bool Unit::AttackUnit()
{
	bool ret = false;

	if (target != nullptr && target->GetHP() > 0)
	{
		if (attackTimer.ReadSec() > 1)
		{
			LookAt(iPoint(target->GetGlobalPosition().x, target->GetGlobalPosition().y));
			action = ATTACK;
			unitState = ATTACKING;
			target->DoDamage(attack);
			attackTimer.Start();
		}
		ret = true;
	}

	return ret;
}

bool Unit::GetFreeAdjacent(iPoint& Adjacent) const
{
	iPoint ret = app->map->WorldToMap(GetGlobalPosition().x, GetGlobalPosition().y);

	if ((!app->entityManager->IsUnitInTile(this, { ret.x + 1, ret.y })) && app->pathfinding->IsWalkable({ ret.x + 1, ret.y }))
	{
		Adjacent = { ret.x + 1, ret.y };
		return true;
	}

	else if ((!app->entityManager->IsUnitInTile(this, { ret.x - 1, ret.y })) && app->pathfinding->IsWalkable({ ret.x - 1, ret.y }))
	{
		Adjacent = { ret.x - 1, ret.y };
		return true;
	}

	else if ((!app->entityManager->IsUnitInTile(this, { ret.x, ret.y + 1 })) && app->pathfinding->IsWalkable({ ret.x, ret.y + 1 }))
	{
		Adjacent = { ret.x, ret.y + 1 };
		return true;
	}

	else if ((!app->entityManager->IsUnitInTile(this, { ret.x, ret.y - 1 })) && app->pathfinding->IsWalkable({ ret.x, ret.y - 1 }))
	{
		Adjacent = { ret.x, ret.y - 1 };
		return true;
	}

	else
	{
		return false;
	}
}

bool Unit::GetAdjacentTile(iPoint tile, iPoint& Adjacent) const
{
	if ((!app->entityManager->IsUnitInTile(this, { tile.x + 1, tile.y })) && app->pathfinding->IsWalkable({ tile.x + 1, tile.y }))
	{
		Adjacent = { tile.x + 1, tile.y };
		return true;
	}

	else if ((!app->entityManager->IsUnitInTile(this, { tile.x - 1, tile.y })) && app->pathfinding->IsWalkable({ tile.x - 1, tile.y }))
	{
		Adjacent = { tile.x - 1, tile.y };
		return true;
	}

	else if ((!app->entityManager->IsUnitInTile(this, { tile.x, tile.y + 1 })) && app->pathfinding->IsWalkable({ tile.x, tile.y + 1 }))
	{
		Adjacent = { tile.x, tile.y + 1 };
		return true;
	}

	else if ((!app->entityManager->IsUnitInTile(this, { tile.x, tile.y - 1 })) && app->pathfinding->IsWalkable({ tile.x, tile.y - 1 }))
	{
		Adjacent = { tile.x, tile.y - 1 };
		return true;
	}

	else
	{
		return false;
	}
}

void Unit::PlayDeathSound() const// ADD FX
{
	int rand_num = rand() % 6;

	if (this->unitType == SAMURAI||this->unitType == ARCHER) {

		switch (rand_num)
		{
		case 0:
			app->audio->PlayFx(app->entityManager->fxDieSoldier001);
			break;
		case 1:
			app->audio->PlayFx(app->entityManager->fxDieSoldier002);
			break;
		case 2:
			app->audio->PlayFx(app->entityManager->fxDieSoldier003);
			break;
		case 3:
			app->audio->PlayFx(app->entityManager->fxDieSoldier004);
			break;
		case 4:
			app->audio->PlayFx(app->entityManager->fxDieSoldier005);
			break;
		case 5:
			app->audio->PlayFx(app->entityManager->fxDieSoldier006);
			break;
		}
	}

	rand_num = rand() % 3;

	if (this->unitType == TARKAN_KNIGHT) {

		switch (rand_num)
		{
		case 0:
			app->audio->PlayFx(app->entityManager->fxDieHorse001);
			break;
		case 1:
			app->audio->PlayFx(app->entityManager->fxDieHorse002);
			break;
		case 2:
			app->audio->PlayFx(app->entityManager->fxDieHorse003);
			break;
	
		}
	}

}

void Unit::PlayAttackSound() const
{
	int rand_num = rand() % 8;

	if(this->unitType== SAMURAI || this->unitType == TARKAN_KNIGHT)
	switch (rand_num)
	{
	case 0:
		app->audio->PlayFx(app->entityManager->fxFight001);
		break;
	case 1:
		app->audio->PlayFx(app->entityManager->fxFight002);
		break;
	case 2:
		app->audio->PlayFx(app->entityManager->fxFight003);
		break;
	case 3:
		app->audio->PlayFx(app->entityManager->fxFight004);
		break;
	case 4:
		app->audio->PlayFx(app->entityManager->fxFight005);
		break;
	case 5:
		app->audio->PlayFx(app->entityManager->fxFight006);
		break;
	case 6:
		app->audio->PlayFx(app->entityManager->fxFight007);
		break;
	case 7:
		app->audio->PlayFx(app->entityManager->fxFight008);
		break;
	}

	rand_num = rand() % 7;

	if (this->unitType == ARCHER) {
		switch (rand_num)
		{
		case 0:
			app->audio->PlayFx(app->entityManager->fxArrow001);
			break;
		case 1:
			app->audio->PlayFx(app->entityManager->fxArrow002);
			break;
		case 2:
			app->audio->PlayFx(app->entityManager->fxArrow003);
			break;
		case 3:
			app->audio->PlayFx(app->entityManager->fxArrow004);
			break;
		case 4:
			app->audio->PlayFx(app->entityManager->fxArrow005);
			break;
		case 5:
			app->audio->PlayFx(app->entityManager->fxArrow006);
			break;
		case 6:
			app->audio->PlayFx(app->entityManager->fxArrow007);
			break;
		}
	}
}

void Unit::PlaySelectSound() const
{
	int rand_num = rand() % 4;

	if (this->unitType == ARCHER || this->unitType == SAMURAI) {

		switch (rand_num)
		{
		case 0:
			app->audio->PlayFx(app->entityManager->fxUnitSelect001);
			break;
		case 1:
			app->audio->PlayFx(app->entityManager->fxUnitSelect002);
			break;
		case 2:
			app->audio->PlayFx(app->entityManager->fxUnitSelect003);
			break;
		case 3:
			app->audio->PlayFx(app->entityManager->fxUnitSelect004);
			break;
		}
	}

	rand_num = rand() % 3;

	if (this->unitType == ARCHER || this->unitType == SAMURAI) {
		switch (rand_num)
		{
		case 0:
			app->audio->PlayFx(app->entityManager->fxHorseSelect001);
			break;
		case 1:
			app->audio->PlayFx(app->entityManager->fxHorseSelect002);
			break;
		case 2:
			app->audio->PlayFx(app->entityManager->fxHorseSelect003);
			break;
		}
	}


}

void Unit::PlayMoveSound() const
{
	if (this->unitType == ARCHER || this->unitType == SAMURAI)
		app->audio->PlayFx(app->entityManager->fxUnitSelect001);

	if (this->unitType == ARCHER || this->unitType == SAMURAI)
		app->audio->PlayFx(app->entityManager->fxUnitSelect001);

}


void Unit::SetHp(int newHP)
{
	hp = newHP;
}