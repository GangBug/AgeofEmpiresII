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
#include "j1Timer.h" 

#define ATTACK_TIMER 1

Boss::Boss(fPoint pos, Entity* parent) : Unit(DIABLO, parent)
{
	if (app->missionManager->getHardModeStatus() == false)
	{
		SetFullHp(BOSSHP / EASY_MODE_BOSS);
		attack = 25;
		speed = 1.5f;
		rate_of_fire = 2;
		range = 4;
		unitClass = INFANTRY;
		unitRadius = 10;
		horde = true;
		unitState = NO_STATE;
		unitDirection = SOUTH_WEST;
		action = IDLE;
	}
	if (app->missionManager->getHardModeStatus() == true)
	{
		SetFullHp(BOSSHP);
		attack = 25;
		speed = 1.5f;
		rate_of_fire = 2;
		range = 4;
		unitClass = INFANTRY;
		unitRadius = 10;
		horde = true;
		unitState = NO_STATE;
		unitDirection = SOUTH_WEST;
		action = IDLE;
	}
	diabloAttackSoundTimer.Start();
	app->missionManager->SetBossState(true);
}

void Boss::OnUpdate(float dt)
{
	app->missionManager->SetBossState(true);
	if (!app->pause)
	{
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			this->SetFullHp(GetHP() - 200);
		}

		/*if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		{
			if (getBossControllableStatus() == true)
			{
				LOG("Boss set to -> NOT controllable");
				setBossControllable(false);
			}
			else if (getBossControllableStatus() == false)
			{
				LOG("Boss set to -> YES controllable");
				setBossControllable(true);
			}
		}*/

		if (GetHP() > 0)
		{
			switch (unitState)
			{
			case NO_STATE:
				if (getBossControllableStatus() == true)
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
					{
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
				if (getBossControllableStatus() == true)
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
					{
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
				if (getBossControllableStatus() == true)
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
					{
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
				if (getBossControllableStatus() == true)
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && selected == true)
					{
						//Colision
						app->collision->resetPrevPositions();

						iPoint objective;
						app->input->GetMouseMapPosition(objective.x, objective.y);
						GoTo(objective);
					}
				}
				if (!AttackUnit())
				{
					target = nullptr;
					unitState = NO_STATE;
					action = IDLE;
				}
				if (diabloAttackSoundTimer.ReadSec() > (this->rate_of_fire - 0.5f))
				{
					if (this->GetUnitType() == DIABLO && (this->GetGlobalPosition().x > (-1)*app->render->camera->GetPosition().x && this->GetGlobalPosition().x < (-1)*(app->render->camera->GetPosition().x - app->render->camera->GetRect().w))
						&& (this->GetGlobalPosition().y > (-1)*app->render->camera->GetPosition().y && this->GetGlobalPosition().y < (-1)*(app->render->camera->GetPosition().y - app->render->camera->GetRect().h)))
					{
						this->PlayAttackSound();
					}
					diabloAttackSoundTimer.Start();
				}
				break;
			}
		}

		else
		{
			Die();
			app->missionManager->SetBossState(false);
		}
	}

	iPoint p;
	app->animation->GetFrame(drawQuad, p, this);
	SetPivot(p);
}

bool Boss::getBossControllableStatus()
{
	return isBossControllable;
}

void Boss::setBossControllable(bool status)
{
	isBossControllable = status;
}