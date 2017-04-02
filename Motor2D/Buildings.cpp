#include "Buildings.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "Units.h"
#include "j1Render.h"
#include "j1Textures.h"

#define ARCHERY_DOOR_X 28
#define ARCHER_DOOR_Y 300 //NEEDS POLISHING

Building::Building(BUILDING_TYPE type, fPoint pos, int id) : Entity(BUILDING, pos), buildingType(type), id(id)
{
	unitsToAdd = 0;
}

void Building::Update()
{
	App->render->SpriteOrdering(this);
}

void Building::PostUpdate()
{
	if (unitsToAdd > 0 && timer.ReadSec() >= 1)
	{
		switch (buildingType) {
		case ARCHERY:
			App->entity_manager->CreateUnit(ARCHER, { GetX(), GetY() + ARCHER_DOOR_Y});
			unitsToAdd--;
			if (unitsToAdd > 0)
			{
				timer.Start();
			}
			else
			{
				timer.Stop();
			}
			break;
		case BARRACK:
			App->entity_manager->CreateUnit(SAMURAI, { GetX(), GetY() + ARCHER_DOOR_Y });
			unitsToAdd--;
			if (unitsToAdd > 0)
			{
				timer.Start();
			}
			else
			{
				timer.Stop();
			}
			break;
		case STABLE:
			App->entity_manager->CreateUnit(TARKANKNIGHT, { GetX(), GetY() + ARCHER_DOOR_Y });
			unitsToAdd--;
			if (unitsToAdd > 0)
			{
				timer.Start();
			}
			else
			{
				timer.Stop();
			}
			break;
		case NO_BUILDING:
			break;
		}
	}
}

void Building::Draw()
{
	switch (buildingType) {
	case ARCHERY:
		App->render->Blit(App->tex->archeryTex, GetX(), GetY());
		break;
	case BARRACK:
		App->render->Blit(App->tex->barracsTex, GetX(), GetY());
		break;
	case STABLE:
		App->render->Blit(App->tex->stableTex, GetX(), GetY());
		break;
	}
}

void Building::GenerateUnit(int num)
{
	unitsToAdd += num;
	timer.Start();
}