#include "Building.h"
#include "M_EntityManager.h"
#include "App.h"
#include "M_Input.h"
#include "M_Textures.h"
#include "M_Audio.h"
#include "M_GUI.h"
#include "M_Resources.h"
#include "M_DialogueManager.h"
#include "Log.h"



Building::Building(building_type buildType, Entity* parent) : Entity(ENTITY_BUILDING, parent), buildType(buildType)
{
	
	SetEnclosingBoxSize(100, 100);

	switch (buildType)
	{
	case BUILD_ARCHERY:
		unitType = unit_type::ARCHER;
		unitCost = 60;
		entityTexture = app->tex->archeryTexture;
		SetEnclosingBoxSize(320, 293);
		creatorButton = app->gui->FindElement(app->gui->guiList, "ArcherCreatorButton");
		break;
	case BUILD_STABLES:
		unitType = unit_type::TARKAN_KNIGHT;
		unitCost = 70;
		entityTexture = app->tex->stableTexture;
		SetEnclosingBoxSize(323, 226);
		creatorButton = app->gui->FindElement(app->gui->guiList, "TarkanCreatorButton");
		break;
	case BUILD_BARRACK:
		unitType = SAMURAI;
		unitCost = 50;
		entityTexture = app->tex->barracksTexture;
		SetEnclosingBoxSize(310, 266);
		creatorButton = app->gui->FindElement(app->gui->guiList, "SamuraiCreatorButton");
		break;

	case 	BUILD_TOWNCENTER:
		unitType = VILLAGER;
		unitCost = 90;
		entityTexture = app->tex->townCenterTexture;
		SetEnclosingBoxSize(382, 399);
		creatorButton = app->gui->FindElement(app->gui->guiList, "VillagerCreatorButton");
		break;

	}

	HP = 100;
}


Building::~Building()
{
}

void Building::OnUpdate(float dt)
{
	if (!app->dialogueManager->onDialogue)
	{
		iPoint mPos;
		app->input->GetMouseMapPosition(mPos.x, mPos.y);

		if (selected == false && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) && GetEnclosingBox().Contains(mPos.x, mPos.y))
		{
			selected = true;
			creatorButton->SetVisible(true);

			if (buildType == BUILD_ARCHERY)
			{
				app->audio->PlayFx(app->entityManager->fxArcherySelection);
			}
			else if (buildType == BUILD_BARRACK)
			{
				app->audio->PlayFx(app->entityManager->fxBarrackSelection);
			}
			else if (buildType == BUILD_STABLES)
			{
				app->audio->PlayFx(app->entityManager->fxStableSelection);
			}
	/*		else if (buildType == BUILD_TOWNCENTER)
			{
				app->audio->PlayFx(app->entityManager->fxTownCenterSelection);
			}*/
		}

		else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && !GetEnclosingBox().Contains(mPos.x, mPos.y))
		{
			app->input->GetMouseScreenPosition(mPos.x, mPos.y);
			if (creatorButton->GetDrawRect().Contains(mPos.x, mPos.y))
			{
				app->dialogueManager->PlayDialogue(D_EVENT_FIRST_ENCOUNTER);
				BuyUnit();
			}
			else
			{
				selected = false;
				creatorButton->SetVisible(false);
			}
		}
	}
}

bool Building::OnSave(pugi::xml_node & node) const
{
	return false;
}

bool Building::OnLoad(pugi::xml_node * node)
{
	return false;
}

void Building::BuyUnit()
{
	//If theres money create a unit
	if (app->resources->GetCurrentGold() > unitCost)
	{
		fPoint pos = GetGlobalPosition();

		app->entityManager->CreateUnit(unitType, this, pos.x, pos.y + 10.0f);

		app->audio->PlayFx(app->entityManager->fxCreateUnit);

		app->resources->SubstractGold(unitCost);

	}
}

int Building::GetHP() const
{
	return HP;
}

void Building::DoDamage(int dmg)
{
	HP -= dmg;
}

void Building::PlaySelectFx()
{
	switch (this->buildType)
	{
	case BUILD_ARCHERY:
		app->audio->PlayFx(app->entityManager->fxArcherySelection);
		break;
	case BUILD_STABLES:
		app->audio->PlayFx(app->entityManager->fxStableSelection);
		break;
	case BUILD_BARRACK:
		app->audio->PlayFx(app->entityManager->fxBarrackSelection);
		break;
	case BUILD_TOWNCENTER:
		app->audio->PlayFx(app->entityManager->fxTownCenterSelection);
		break;

	default:
		break;
	}
}
