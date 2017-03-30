#include "Buildings.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "Units.h"
#include "j1Render.h"
#include "j1Textures.h"

Building::Building(BUILDING_TYPE type, fPoint pos, int id) : Entity(BUILDING, pos), buildingType(type), id(id)
{
	unitsToAdd = 0;
}

void Building::Update()
{
	Draw();
}

void Building::PostUpdate()
{
	if (unitsToAdd > 0 && timer.ReadSec() >= 1)
	{
		switch (buildingType) {
		case ARCHERY:
			App->entity_manager->CreateUnit(ARCHER, { GetX(), GetY() + 100});
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
	App->render->Blit(App->tex->archeryTex, GetX(), GetY());
}

void Building::GenerateUnit(int num)
{
	unitsToAdd += num;
	timer.Start();
}