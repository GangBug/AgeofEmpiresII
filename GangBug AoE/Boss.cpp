#include "App.h"
#include "M_Input.h"
#include "M_Render.h"
#include "M_Animation.h"
#include "Boss.h"
#include "Log.h"
#include "M_Pathfinding.h"
#include "M_Map.h"
#include "M_EntityManager.h"
#include "M_MissionManager.h"
#include "M_Collision.h"

#define ATTACK_TIMER 1

Boss::Boss(fPoint pos, Entity* parent) : Unit(DIABLO, parent)
{
		SetHp(BOSSHP);
		attack = 25;
		speed = 1.5f;
		rate_of_fire = 2;
		range = 4;
		unitClass = INFANTRY;
		unitRadius = 12;
		horde = true;
		unitState = NO_STATE;
		unitDirection = SOUTH_WEST;
		action = IDLE;
}

void Boss::OnUpdate(float dt)
{
	app->missionManager->SetBossState(true);
	if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		this->SetHp(GetHP() - 200);
	}

	if (GetHP() > 0) 
	{
		switch (unitState)
		{
		case NO_STATE:
			if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
			{
				//Colision
				app->collision->resetPrevPositions();

				iPoint objective;
				app->input->GetMouseMapPosition(objective.x, objective.y);
				GoTo(objective);
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
			if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
			{
				//Colision
				app->collision->resetPrevPositions();

				iPoint objective;
				app->input->GetMouseMapPosition(objective.x, objective.y);
				GoTo(objective);
			}
			else if (Move() == false)
			{
				unitState = NO_STATE;
				action = IDLE;
			}
			CheckSurroundings();
			break;
		case MOVING_TO_ATTACK:
			if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
			{
				//Colision
				app->collision->resetPrevPositions();

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
				//Colision
				app->collision->resetPrevPositions();

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
	}

	else
	{
		Die();
		app->missionManager->SetBossState(false);
	}


	iPoint p;
	app->animation->GetFrame(drawQuad, p, this);
	SetPivot(p);
}