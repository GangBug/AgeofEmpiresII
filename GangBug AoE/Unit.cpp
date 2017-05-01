#include "Unit.h"
#include "M_Animation.h"
#include "M_Map.h"
#include "M_Pathfinding.h"
#include "M_FogOfWar.h"
#include "M_Audio.h"
#include "M_EntityManager.h"
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
		//AI = false;
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
		//AI = false;
		unitState = NO_STATE;
		break;

	case VILE:
		SetHp(20);
		attack = 5;
		speed = 2.0f;
		rate_of_fire = 1;
		range = 0;
		unitClass = INFANTRY;
		unitRadius = 7;
		//AI = true;
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
	switch (unitState)
	{
	case NO_STATE:
		if (app->input->GetMouseButtonDown(3) == KEY_DOWN && selected == true)
		{
			iPoint objective;
			app->input->GetMouseMapPosition(objective.x, objective.y);
			GoTo(objective);
		}
		break;
	case MOVING:
		if (app->input->GetMouseButtonDown(3) == KEY_DOWN && selected == true)
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
		this->unitState = MOVING;
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


}




void Unit::SetHp(int newHP)
{
	hp = newHP;
}