#include "App.h"
#include "M_Input.h"
#include "M_Render.h"
#include "M_Animation.h"
#include "Boss.h"
#include "Log.h"
#include "M_Pathfinding.h"
#include "M_Map.h"
#include "M_EntityManager.h"

#define ATTACK_TIMER 1

Boss::Boss(fPoint pos, Entity* parent) : Unit(DIABLO, parent)
{
		SetHp(BOSSHP);
		attack = 25;
		speed = 1.5f;
		rate_of_fire = 2;
		range = 6;
		unitClass = INFANTRY;
		unitRadius = 7;
		horde = true;
		unitState = NO_STATE;
		unitDirection = SOUTH_WEST;
		action = IDLE;
}

void Boss::Update()
{
	if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		this->SetHp(GetHP() - 200);
	}
	CheckSurroundings();
	if (GetHP() <= 0)
	{
		unitState = DEAD;
		action = DIE;
		app->entityManager->DeleteUnit(this);
	}
}