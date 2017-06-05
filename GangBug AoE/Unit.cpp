#include "Unit.h"
#include "M_Animation.h"
#include "M_Map.h"
#include "M_Pathfinding.h"
#include "M_FogOfWar.h"
#include "M_Audio.h"
#include "Building.h"
#include "M_EntityManager.h"
#include "M_DialogueManager.h"
#include "M_MissionManager.h"
#include "M_Collision.h"
#include "M_Metrics.h"
//TMP
#include "M_Input.h"
#include "App.h"
#include "Log.h"

#define U_LIFE_BAR_WIDTH 30
#define U_LIFE_BAR_Y_OFFSET 45

Unit::Unit(unit_type type, Entity* parent) : unitType(type), Entity(ENTITY_UNIT, parent)
{
	name.assign("unit_");
	action = IDLE;
	unitDirection = SOUTH;
	entityTexture = app->animation->GetTexture(unitType);
	target = nullptr;

	//EASY MODE
	if (app->missionManager->getHardModeStatus() == false)
	{
		switch (type)
		{
			//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
		case TARKAN_KNIGHT:
			SetFullHp(130 * EASY_MODE);
			attack = 15 * EASY_MODE;
			speed = 1.6f;
			rate_of_fire = 1;
			range = 1;
			unitClass = CAVALRY;
			unitRadius = 4;
			horde = false;
			unitState = NO_STATE;
			break;

		case SAMURAI:
			SetFullHp(100 * EASY_MODE);
			attack = 17 * EASY_MODE;
			speed = 1.3f;
			rate_of_fire = 1;
			range = 1;
			unitClass = INFANTRY;
			unitRadius = 5;
			horde = false;
			unitState = NO_STATE;
			break;

		case ARCHER:
			SetFullHp(40 * EASY_MODE);
			attack = 8 * EASY_MODE;
			speed = 1.5f;
			rate_of_fire = 1.3f;
			range = 5;
			unitClass = RANGED;
			unitRadius = 8;
			horde = false;
			unitState = NO_STATE;
			break;

		case VILE:
			SetFullHp(70 / EASY_MODE);
			attack = 15;
			speed = 2.0f;
			rate_of_fire = 1;
			range = 1;
			unitClass = INFANTRY;
			unitRadius = 7;
			horde = true;
			unitState = NO_STATE;
			break;

		case HELL_WITCH:
			SetFullHp(100 / EASY_MODE);
			attack = 12;
			speed = 2.0f;
			rate_of_fire = 1;
			range = 1;
			unitClass = INFANTRY;
			unitRadius = 7;
			horde = true;
			unitState = NO_STATE;
			break;
		}
	}
	//HARD MODE
	else
	{
		switch (type)
		{
			//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
		case TARKAN_KNIGHT:
			SetFullHp(130);
			attack = 15;
			speed = 1.6f;
			rate_of_fire = 1;
			range = 1;
			unitClass = CAVALRY;
			unitRadius = 4;
			horde = false;
			unitState = NO_STATE;
			break;

		case SAMURAI:
			SetFullHp(100);
			attack = 17;
			speed = 1.3f;
			rate_of_fire = 1;
			range = 1;
			unitClass = INFANTRY;
			unitRadius = 5;
			horde = false;
			unitState = NO_STATE;
			break;

		case ARCHER:
			SetFullHp(40);
			attack = 8;
			speed = 1.5f;
			rate_of_fire = 1.3f;
			range = 5;
			unitClass = RANGED;
			unitRadius = 8;
			horde = false;
			unitState = NO_STATE;
			break;

		case VILE:
			SetFullHp(70);
			attack = 15;
			speed = 2.0f;
			rate_of_fire = 1;
			range = 1;
			unitClass = INFANTRY;
			unitRadius = 7;
			horde = true;
			unitState = NO_STATE;
			break;

		case HELL_WITCH:
			SetFullHp(100);
			attack = 12;
			speed = 2.0f;
			rate_of_fire = 1;
			range = 1;
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
	}	

	visionArea.SetRad(300);
	renderArea.SetRad(300 + RENDER_MARGIN);
}

void Unit::OnUpdate(float dt)
{
	if (!app->dialogueManager->onDialogue)
	{
		if (GetHP() > 0)
		{
			if (this->horde == false)
			{
				iPoint mPos;
				app->input->GetMouseMapPosition(mPos.x, mPos.y);

				GB_Rectangle<float> unitRect = { this->GetGlobalPosition().x - 20 , this->GetGlobalPosition().y - 35, 30, 42 };
			
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && unitRect.Contains(mPos.x, mPos.y) && selected == false)
				{
					selected = true;
				}
				if (selected == true && app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
				{
					std::vector<Entity*> vec = app->entityManager->GetUnitVector();
					for (std::vector<Entity*>::iterator it = vec.begin(); it != vec.end(); it++)
					{
						if (dynamic_cast<Unit*>(*it)->GetUnitType() == this->GetUnitType())
						{
							(*it)->selected = true;
						}
					}
				}
			}
	
			switch (unitState)
			{
			case NO_STATE:
				if (this->horde == false)
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
					{
						PlayMoveSound();
						//Colision
						app->collision->resetPrevPositions();

						iPoint objective;
						app->input->GetMouseMapPosition(objective.x, objective.y);
						GoTo(objective);
					}
				}
				if (target != nullptr && target->GetHP() > 0)
				{
					if (buildingToAttack == true)
					{
						SetBuildingFightingArea();
					}
					else if (buildingToAttack == false)
					{
						SetFightingArea();
					}
				}
				CheckSurroundings();
				break;
			case MOVING:
				if (this->horde == false)
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
					{
						PlayMoveSound();
						//Colision
						app->collision->resetPrevPositions();

						iPoint objective;
						app->input->GetMouseMapPosition(objective.x, objective.y);
						GoTo(objective);
					}
				}
				if (Move() == false)
				{
					unitState = NO_STATE;
					action = IDLE;
				}
				break;
			case FLEEING:
				if (this->horde == false)
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
					{
						PlayMoveSound();
						//Colision
						app->collision->resetPrevPositions();

						iPoint objective;
						app->input->GetMouseMapPosition(objective.x, objective.y);
						GoTo(objective);
					}
				}
				if (Move() == false)
				{
					unitState = NO_STATE;
					action = IDLE;
				}
				CheckSurroundings();
				break;
			case MOVING_TO_ATTACK:
				if (this->horde == false)
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
					{
						PlayMoveSound();
						//Colision
						app->collision->resetPrevPositions();

						iPoint objective;
						app->input->GetMouseMapPosition(objective.x, objective.y);
						GoTo(objective);
					}
				}
				if (Move() == false)
				{
					unitState = ATTACKING;
					action = ATTACK;
				}
				break;
			case ATTACKING:
				if (this->horde == false)
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
					{
						PlayMoveSound();

						//Colision
						app->collision->resetPrevPositions();

						iPoint objective;
						app->input->GetMouseMapPosition(objective.x, objective.y);
						unitState = FLEEING;
						GoTo(objective);
					}
				}
				if (!AttackUnit())
				{
					target = nullptr;
					unitState = NO_STATE;
					action = IDLE;
				}
				break;
			}
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

	if (target != nullptr)
	{
		if (target->selfActive == false || target->removeFlag == true || target->GetHP() <= 0)
		{
			target = nullptr;
			unitState = NO_STATE;
			action = IDLE;
		}
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
	if (app->pathfinding->CalculatePath(ori, destinat, pathVec) == false)
	{
		unitState = NO_STATE;
		return false;
	}
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
	if (app->god)
	{
		if (horde)
		{
			hp -= dmg;
		}
	}
	else
	{
		hp -= dmg;
	}
}

void Unit::Die()
{
	if (action != DIE && action != DISAPPEAR)
	{
		PlayDeathSound();
	
		action = DIE;
		Animation* anim = app->animation->GetAnimation(GetUnitType(), action, unitDirection);
		anim->Reset();
	}
	else if (app->animation->GetAnimation(GetUnitType(), action, unitDirection)->Finished() == true && action == DIE)
	{
		if (unitType != HELL_WITCH && unitType != DIABLO)
		{

			this->action = DISAPPEAR; //CURRENTLY NOT WORKING 
			Animation* anim = app->animation->GetAnimation(GetUnitType(), action, unitDirection);
			anim->Reset();
		}
		else if(unitType == HELL_WITCH || unitType == DIABLO )
		{
			if (this->horde == true)
			{
				app->missionManager->AddEnemyDeadUnit();
			}

			app->entityManager->DeleteUnit(this);
		}
	}
	else if (app->animation->GetAnimation(GetUnitType(), action, unitDirection)->Finished() == true && action == DISAPPEAR)
	{
		if (this->horde == true) 
		{
			app->missionManager->AddEnemyDeadUnit();

			if (unitType == VILE)
				app->metrics->AddDeadViles();
			if (unitType == HELL_WITCH)
				app->metrics->AddDeadWitch();
			else if (unitType == DIABLO)
				app->metrics->AddBoss();
		}
		else if (unitType == SAMURAI)
			app->metrics->KillSamuraiAlive();
		else if (unitType == TARKAN_KNIGHT)
			app->metrics->KillTarkanAlive();
		else if (unitType == ARCHER)
			app->metrics->KillArcherAlive();
	
	
		app->entityManager->DeleteUnit(this);
	}
}

void Unit::PrintLife()
{
	if (selected == true && horde == false)
	{
		if (GetHP() < 0)
		{
			hp = 0;
		}

		fPoint pos = GetGlobalPosition();
		int halfWidth = GetEnclosingBox().w * 0.5;
		iPoint progressPos(pos.x + halfWidth - (U_LIFE_BAR_WIDTH * 0.5), pos.y - U_LIFE_BAR_Y_OFFSET);

		int gwbar;
		gwbar = ((hp * 100) / totalhp);
		gwbar = (gwbar * U_LIFE_BAR_WIDTH) / 100;
		//red
		app->render->DrawQuad({ progressPos.x, progressPos.y, U_LIFE_BAR_WIDTH, 4 }, 255, 0, 0, 255);
		//green
		app->render->DrawQuad({ progressPos.x, progressPos.y, gwbar, 4 }, 0, 255, 0, 255);
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
	std::vector<Entity*> buildingVec = app->entityManager->GetBuildingVector();

	iPoint unitPos = app->map->WorldToMap(GetGlobalPosition().x, GetGlobalPosition().y);

	for (std::vector<Entity*>::iterator it = buildingVec.begin(); it != buildingVec.end(); it++)
	{
		if ((*it) != this)
		{
			Building* challenger = dynamic_cast<Building*>(*it);
			if (challenger->horde != this->horde) {
				iPoint itPos = challenger->tileAttack;
				if (itPos.x <= unitPos.x + unitRadius && itPos.x >= unitPos.x - unitRadius && itPos.y <= unitPos.y + unitRadius && itPos.y >= unitPos.y - unitRadius && (*it)->GetHP() > 0)
				{
					int itDistance = unitPos.DistanceTo(itPos);
					if (itDistance < distance || distance == 0)
					{
						distance = itDistance;
						ret = (*it);
						target = (*it);

						buildingToAttack = true;
					}
				}
			}
		}
	}

	for (std::vector<Entity*>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		if ((*it) != this)
		{
			Unit* challenger = dynamic_cast<Unit*>(*it);
			if (challenger->horde != this->horde)
			{
				iPoint itPos = app->map->WorldToMap((*it)->GetGlobalPosition().x, (*it)->GetGlobalPosition().y);
				if (itPos.x <= unitPos.x + unitRadius && itPos.x >= unitPos.x - unitRadius && itPos.y <= unitPos.y + unitRadius && itPos.y >= unitPos.y - unitRadius && (*it)->GetHP() > 0)
				{
					int itDistance = unitPos.DistanceTo(itPos);
					if (itDistance < distance || distance == 0)
					{
						distance = itDistance;
						ret = (*it);
						target = (*it);

						buildingToAttack = false;
					}
				}
			}
		}
	}
	return ret;
}

//Only use this for enemy units attacking buildings
bool Unit::SetBuildingFightingArea()
{
	bool ret;

	if (target != nullptr && unitState != FLEEING &&  target->GetEntityType() == ENTITY_BUILDING)
	{
		unitState = MOVING_TO_ATTACK;

		Building* enemy = dynamic_cast<Building*>(target);

		//BUILDING
		if (unitClass != RANGED && unitState != ATTACKING)
		{
			iPoint newPos = app->map->MapToWorld(enemy->tileAttack.x, enemy->tileAttack.y);

			pathVec.clear();

			if (GoTo(newPos) != false)
			{
				ret = false;
			}

			else
			{
				unitState = NO_STATE;
			}

		}
		else if (unitClass == RANGED)
		{
			unitState = ATTACKING;
		}

	}
	else 
	{
		buildingToAttack = false;
	}

	return true;
}


//Only use this for enemy units
bool Unit::SetFightingArea()
{
	bool ret;

	if (target != nullptr && unitState != FLEEING)
	{
		Unit* enemy = dynamic_cast<Unit*>(target);

		iPoint enemyPos = app->map->WorldToMap(enemy->GetGlobalPosition().x, enemy->GetGlobalPosition().y);

		iPoint Pos = app->map->WorldToMap(GetGlobalPosition().x, GetGlobalPosition().y);

		iPoint distance = Pos - enemyPos;

		//UNIT

		if ((enemy->target == nullptr || enemy->target == this) && enemy->unitState != FLEEING && enemy->unitState != MOVING_TO_ATTACK) 
		{//If the enemy's enemy is... YOU or a nullptr 

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

				if (this->GetFreeAdjacent(newEnemyPos, enemyPos))
				{
					enemy->pathVec.clear();
					if (enemy->GoTo(app->map->MapToWorld(newEnemyPos.x, newEnemyPos.y)) != false)
					{
						enemy->unitState = MOVING_TO_ATTACK;
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

				if (enemy->GetFreeAdjacent(newPos, Pos))
				{
					pathVec.clear();

					if (GoTo(app->map->MapToWorld(newPos.x, newPos.y)) != false)

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

				if (this->GoTo(app->map->MapToWorld(newPos.x, newPos.y)) != false)
				{
					unitState = MOVING_TO_ATTACK;
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
				if (enemy->GoTo(app->map->MapToWorld(newEnemyPos.x, newEnemyPos.y)) != false)
				{		
					enemy->unitState = MOVING_TO_ATTACK;
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
				Entity* tmp = CheckSurroundings();

				if (tmp == target)
				{
					unitState = MOVING_TO_ATTACK;

					iPoint newPos;
					enemy->GetFreeAdjacent(newPos, Pos);
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

					if (enemy->GetFreeAdjacent(newPos, Pos))
					{
						if (newPos != Pos)
						{
							pathVec.clear();

							if (GoTo(app->map->MapToWorld(newPos.x, newPos.y)) != false)
							{
								unitState = MOVING_TO_ATTACK;
								ret = false;
							}
							else
							{
								unitState = NO_STATE;
							}
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
					action = IDLE;
					unitState = NO_STATE;
					/*iPoint newPos;
					iPoint enemyDestination = app->map->WorldToMap(enemy->pathObjective.x, enemy->pathObjective.y);
					if (this->GetAdjacentTile(enemy->pathObjective, newPos))
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
					}*/
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
		if (unitState != MOVING_TO_ATTACK && unitState != FLEEING)
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

	if (target == nullptr || target->selfActive == false || target->GetHP() <= 0)
	{
		target = nullptr;
		unitState = NO_STATE;
		action = IDLE;
	}

	else if (target != nullptr && target->GetHP() > 0)
	{
		iPoint enemyPos = app->map->WorldToMap(target->GetGlobalPosition().x, target->GetGlobalPosition().y);
		iPoint myPos = app->map->WorldToMap(GetGlobalPosition().x, GetGlobalPosition().y);
		if (myPos.DistanceTo(enemyPos) > unitRadius && target->GetEntityType() == ENTITY_UNIT)
		{
			target = nullptr;
			unitState = NO_STATE;
			action = IDLE;
		}
		else if (myPos.DistanceTo(enemyPos) > range && target->GetEntityType() == ENTITY_UNIT)
		{
			SetFightingArea();
		}
		else if (target->GetEntityType() == ENTITY_BUILDING && myPos != dynamic_cast<Building*>(target)->tileAttack && unitClass != RANGED)
		{
			SetBuildingFightingArea();
		}
		else if (myPos.DistanceTo(enemyPos) > unitRadius && target->GetEntityType() == ENTITY_UNIT)
		{
			target = nullptr;
			unitState = NO_STATE;
			action = IDLE;
		}
		else if (attackTimer.ReadSec() > (this->rate_of_fire + 0.1f))
		{
			LookAt(iPoint(target->GetGlobalPosition().x, target->GetGlobalPosition().y));
			action = ATTACK;
		
			if (this->GetUnitType() != DIABLO)
			{
				this->PlayAttackSound();
			}
		
			unitState = ATTACKING;
			{
				target->DoDamage(attack);
			}
			

			attackTimer.Start();
		}
		ret = true;
	}

	return ret;
}

bool Unit::GetFreeAdjacent(iPoint& Adjacent, iPoint currentPos) const
{
	iPoint ret = app->map->WorldToMap(GetGlobalPosition().x, GetGlobalPosition().y);
	bool retB = false;
	int distance = 0;

	if (currentPos == iPoint(0, 0))
	{
		if ((!app->entityManager->IsUnitInTile(this, { ret.x + 1, ret.y })) && app->pathfinding->IsWalkable({ ret.x + 1, ret.y }))
		{
			Adjacent = { ret.x + 1, ret.y };
			retB = true;
		}

		else if ((!app->entityManager->IsUnitInTile(this, { ret.x - 1, ret.y })) && app->pathfinding->IsWalkable({ ret.x - 1, ret.y }))
		{
			Adjacent = { ret.x - 1, ret.y };
			retB = true;
		}

		else if ((!app->entityManager->IsUnitInTile(this, { ret.x, ret.y + 1 })) && app->pathfinding->IsWalkable({ ret.x, ret.y + 1 }))
		{
			Adjacent = { ret.x, ret.y + 1 };
			retB = true;
		}

		else if ((!app->entityManager->IsUnitInTile(this, { ret.x, ret.y - 1 })) && app->pathfinding->IsWalkable({ ret.x, ret.y - 1 }))
		{
			Adjacent = { ret.x, ret.y - 1 };
			retB = true;
		}
	}
	else
	{
		if ((!app->entityManager->IsUnitInTile(this, { ret.x + 1, ret.y })) && app->pathfinding->IsWalkable({ ret.x + 1, ret.y }))
		{
			if (distance == 0 || currentPos.DistanceTo({ ret.x + 1, ret.y }) < distance)
			{
				Adjacent = { ret.x + 1, ret.y };
				distance = currentPos.DistanceTo(Adjacent);
				retB = true;
			}
		}

		if ((!app->entityManager->IsUnitInTile(this, { ret.x - 1, ret.y })) && app->pathfinding->IsWalkable({ ret.x - 1, ret.y }))
		{
			if (distance == 0 || currentPos.DistanceTo({ ret.x - 1, ret.y }) < distance)
			{
				Adjacent = { ret.x - 1, ret.y };
				distance = currentPos.DistanceTo(Adjacent);
				retB = true;
			}
		}

		if ((!app->entityManager->IsUnitInTile(this, { ret.x, ret.y + 1 })) && app->pathfinding->IsWalkable({ ret.x, ret.y + 1 }))
		{
			if (distance == 0 || currentPos.DistanceTo({ ret.x, ret.y + 1 }) < distance)
			{
				Adjacent = { ret.x, ret.y + 1 };
				distance = currentPos.DistanceTo(Adjacent);
				retB = true;
			}
		}

		if ((!app->entityManager->IsUnitInTile(this, { ret.x, ret.y - 1 })) && app->pathfinding->IsWalkable({ ret.x, ret.y - 1 }))
		{
			if (distance == 0 || currentPos.DistanceTo({ ret.x, ret.y - 1 }) < distance)
			{
				Adjacent = { ret.x, ret.y - 1 };
				distance = currentPos.DistanceTo(Adjacent);
				retB = true;
			}
		}
	}

	return retB;
}

bool Unit::GetAdjacentTile(iPoint tile, iPoint& Adjacent) const
{
	bool ret = false;
	int distance = 0;
	iPoint currentPos = app->map->WorldToMap(GetGlobalPosition().x, GetGlobalPosition().y);

	if ((!app->entityManager->IsUnitInTile(this, { tile.x + 1, tile.y })) && app->pathfinding->IsWalkable({ tile.x + 1, tile.y }))
	{
		if (distance == 0 || currentPos.DistanceTo({ tile.x + 1, tile.y }) < distance)
		{
			Adjacent = { tile.x + 1, tile.y };
			distance = currentPos.DistanceTo(Adjacent);
			ret = true;
		}
	}

	if ((!app->entityManager->IsUnitInTile(this, { tile.x - 1, tile.y })) && app->pathfinding->IsWalkable({ tile.x - 1, tile.y }))
	{
		if (distance == 0 || currentPos.DistanceTo({ tile.x - 1, tile.y }) < distance)
		{
			Adjacent = { tile.x - 1, tile.y };
			distance = currentPos.DistanceTo(Adjacent);
			ret = true;
		}
	}

	if ((!app->entityManager->IsUnitInTile(this, { tile.x, tile.y + 1 })) && app->pathfinding->IsWalkable({ tile.x, tile.y + 1 }))
	{
		if (distance == 0 || currentPos.DistanceTo({ tile.x, tile.y + 1}) < distance)
		{
			Adjacent = { tile.x, tile.y + 1};
			distance = currentPos.DistanceTo(Adjacent);
			ret = true;
		}
	}

	if ((!app->entityManager->IsUnitInTile(this, { tile.x, tile.y - 1 })) && app->pathfinding->IsWalkable({ tile.x, tile.y - 1 }))
	{
		if (distance == 0 || currentPos.DistanceTo({ tile.x, tile.y - 1 }) < distance)
		{
			Adjacent = { tile.x, tile.y - 1 };
			distance = currentPos.DistanceTo(Adjacent);
			ret = true;
		}
	}

	return ret;
}


void Unit::PlayDeathSound() const// ADD FX
{
	int rand_num = rand() % 6;

	if (this->unitType == SAMURAI || this->unitType == ARCHER) {

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

	if (this->unitType == VILE) {

		rand_num = rand() % 5;

		switch (rand_num)
		{
		case 0:
			app->audio->PlayFx(app->entityManager->fxDieVile001);
			break;
		case 1:
			app->audio->PlayFx(app->entityManager->fxDieVile002);
			break;
		case 2:
			app->audio->PlayFx(app->entityManager->fxDieVile003);
			break;
		case 3:
			app->audio->PlayFx(app->entityManager->fxDieVile004);
			break;
		case 4:
			app->audio->PlayFx(app->entityManager->fxDieVile005);
			break;
		}
	}

	if (this->unitType == HELL_WITCH) {

		rand_num = rand() % 4;

		switch (rand_num)
		{
		case 0:
			app->audio->PlayFx(app->entityManager->fxDieHellW001);
			break;
		case 1:
			app->audio->PlayFx(app->entityManager->fxDieHellW002);
			break;
		case 2:
			app->audio->PlayFx(app->entityManager->fxDieHellW003);
			break;
		case 3:
			app->audio->PlayFx(app->entityManager->fxDieHellW004);
			break;
		}
	}

	if (this->unitType == DIABLO)
		app->audio->PlayFx(app->entityManager->fxDieDiablo001);

	if (this->unitType == TARKAN_KNIGHT) {

		rand_num = rand() % 3;

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

	if (this->unitType == SAMURAI || this->unitType == TARKAN_KNIGHT) {

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
	}
	if (this->unitType == ARCHER)
	{
		rand_num = rand() % 7;

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
	if (this->unitType == VILE)
	{
		rand_num = rand() % 5;

		switch (rand_num)
		{
		case 0:
			app->audio->PlayFx(app->entityManager->fxFightVile001);
			break;
		case 1:
			app->audio->PlayFx(app->entityManager->fxFightVile002);
			break;
		case 2:
			app->audio->PlayFx(app->entityManager->fxFightVile003);
			break;
		case 3:
			app->audio->PlayFx(app->entityManager->fxFightVile004);
			break;
		case 4:
			app->audio->PlayFx(app->entityManager->fxFightVile005);
			break;
		}
	}
	if (this->unitType == HELL_WITCH) 
	{
		rand_num = rand() % 5;

		switch (rand_num)
		{
		case 0:
			app->audio->PlayFx(app->entityManager->fxFightHellW001);
			break;

		case 1:
			app->audio->PlayFx(app->entityManager->fxFightHellW002);
			break;

		case 2:
			app->audio->PlayFx(app->entityManager->fxFightHellW003);
			break;

		case 3:
			app->audio->PlayFx(app->entityManager->fxFightHellW004);
			break;

		case 4:
			app->audio->PlayFx(app->entityManager->fxFightHellW005);
			break;

		}
	}
	if (this->unitType == DIABLO) {

		rand_num = rand() % 5;

		switch (rand_num)
		{
		case 0:
			app->audio->PlayFx(app->entityManager->fxFightDiabloW001);
			break;

		case 1:
			app->audio->PlayFx(app->entityManager->fxFightDiabloW002);
			break;

		case 2:
			app->audio->PlayFx(app->entityManager->fxFightDiabloW003);
			break;

		case 3:
			app->audio->PlayFx(app->entityManager->fxFightDiabloW004);
			break;

		case 4:
			app->audio->PlayFx(app->entityManager->fxFightDiabloW005);
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
	if (this->unitType == TARKAN_KNIGHT) 
	{
		rand_num = rand() % 3;
		switch (rand_num)
		{
		case 0:
			//	app->audio->PlayFx(app->entityManager->fxHorseSelect001);
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
	int rand_num = rand() % 8;

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
			//	app->audio->PlayFx(app->entityManager->fxUnitSelect003);  WARNING OMG I  I've been deaf to this sound
			break;
		case 3:
			app->audio->PlayFx(app->entityManager->fxUnitSelect004);
			break;
		}
	}

	if (this->unitType == TARKAN_KNIGHT) {
		rand_num = rand() % 8;
		switch (rand_num)
		{
		case 0:
			//	app->audio->PlayFx(app->entityManager->fxHorseSelect001); WARNING OMG I  I've been deaf to this sound
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

void Unit::SetFullHp(int newHP)
{
	totalhp = newHP;
	hp = newHP;
}

void Unit::SetHpandFullHp(int currentHP, int fullHP)
{
	hp = currentHP;
	totalhp = fullHP;
}

void Unit::SetAttack(int amount)
{
	attack = amount;
}

void Unit::Serialize(pugi::xml_node& node)
{
	pugi::xml_node uNode = node.append_child("unit");

	uNode.append_attribute("hp") = hp;
	uNode.append_attribute("totalhp") = totalhp;
	uNode.append_attribute("posX") = GetGlobalPosition().x;
	uNode.append_attribute("posY") = GetGlobalPosition().y;
	switch (unitType)
	{
	case ARCHER:	uNode.append_attribute("type") = "archer"; break;
	case SAMURAI:	uNode.append_attribute("type") = "samurai"; break;
	case TARKAN_KNIGHT:	uNode.append_attribute("type") = "tarkanKnight"; break;
	case VILE:	uNode.append_attribute("type") = "vile"; break;
	case HELL_WITCH:	uNode.append_attribute("type") = "hellWitch"; break;
	case DIABLO:	uNode.append_attribute("type") = "diablo"; break;
	}
}