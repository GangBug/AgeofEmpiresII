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
#include "M_ParticleSystem.h"
#include "M_MissionManager.h"

#define BUY_TIMER 2
#define PROGRESS_WIDTH 100


Building::Building(building_type buildType, iPoint tileAttack, Entity* parent) : Entity(ENTITY_BUILDING, parent), buildType(buildType), tileAttack(tileAttack)
{
	SetEnclosingBoxSize(100, 100);
	unitsToAdd = 0;

	switch (buildType)
	{
	case BUILD_ARCHERY:
		unitType = unit_type::ARCHER;

		unitGoldCost = 45;
		unitWoodCost = 25;
		unitFoodCost = 0;
		horde = false;

		entityTexture = app->tex->archeryTexture;
		SetEnclosingBoxSize(320, 293);
		creatorButton = app->gui->FindElement(app->gui->guiList, "ArcherCreatorButton");
		unitInfoLabel = app->gui->FindElement(app->gui->guiList, "label_ArcherCost");

		break;

	case BUILD_STABLES:
		unitType = unit_type::TARKAN_KNIGHT;

		unitGoldCost = 75;
		unitWoodCost = 0;
		unitFoodCost = 60;
		horde = false;

		entityTexture = app->tex->stableTexture;
		SetEnclosingBoxSize(323, 226);
		creatorButton = app->gui->FindElement(app->gui->guiList, "TarkanCreatorButton");
		unitInfoLabel = app->gui->FindElement(app->gui->guiList, "label_TarkanCost");

		break;

	case BUILD_BARRACK:

		unitType = SAMURAI;

		unitGoldCost = 30;
		unitWoodCost = 0;
		unitFoodCost = 60;
		horde = false;

		entityTexture = app->tex->barracksTexture;
		SetEnclosingBoxSize(310, 266);
		creatorButton = app->gui->FindElement(app->gui->guiList, "SamuraiCreatorButton");
		unitInfoLabel = app->gui->FindElement(app->gui->guiList, "label_SamuraiCost");

		break;

	case BUILD_TOWNCENTER:
		unitType = VILLAGER;

		unitGoldCost = 0;
		unitWoodCost = 0;
		unitFoodCost = 50;
		horde = false;

		entityTexture = app->tex->townCenterTexture;
		SetLocalPosition(10, 10);
		SetEnclosingBoxSize(382, 399);
		creatorButton = app->gui->FindElement(app->gui->guiList, "VillagerCreatorButton");
		unitInfoLabel = app->gui->FindElement(app->gui->guiList, "label_VillagerCost");

		break;

	case BUILD_PORTAL:
		unitType = VILE;
		horde = true;
		entityTexture = app->tex->portalTexture;
		SetEnclosingBoxSize(155, 233);

		break;
	}
	selected = false;
	HP = 100;
	fullHP = 100;
}


Building::~Building()
{
}

void Building::OnUpdate(float dt)
{
	if (!app->dialogueManager->onDialogue)
	{
		if (buildType != BUILD_PORTAL)
		{
			iPoint mPos;
			app->input->GetMouseMapPosition(mPos.x, mPos.y);

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && GetEnclosingBox().Contains(mPos.x, mPos.y))
			{
				selected = true;
				creatorButton->SetVisible(true);
				creatorButton->SetInteractive(true);

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
				else if (buildType == BUILD_TOWNCENTER)
				{
					app->audio->PlayFx(app->entityManager->fxTownCenterSelection);
				}
			}
			else if (selected == true)
			{
				app->input->GetMouseScreenPosition(mPos.x, mPos.y);
				if (creatorButton->GetDrawRect().Contains(mPos.x, mPos.y))
				{
					unitInfoLabel->SetVisible(true);
					unitInfoLabel->SetActive(true);
					if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
					{
						BuyUnit();
					}
				}
				else if (!creatorButton->GetDrawRect().Contains(mPos.x, mPos.y))
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
					{
						selected = false;
						creatorButton->SetVisible(false);
						creatorButton->SetInteractive(false);
					}
					else
					{
						unitInfoLabel->SetVisible(false);
						unitInfoLabel->SetActive(false);
					}
				}
			}

			//UNIT CREATION
			if (unitsToAdd > 0 && buyTimer.ReadSec() >= BUY_TIMER)
			{
				fPoint pos = GetGlobalPosition();

				if (unitType != VILLAGER)
				{
					app->entityManager->CreateUnit(unitType, this, pos.x + 20, pos.y + 300.0f);
					app->audio->PlayFx(app->entityManager->fxCreateUnit);

					unitsToAdd--;
					if (unitsToAdd > 0)
					{
						buyTimer.Start();
					}
					else
					{
						buyTimer.Stop();
					}
				}
				else
				{
					app->resources->AddVillager();
					app->audio->PlayFx(app->entityManager->fxCreateVillager);
					unitsToAdd--;
					if (unitsToAdd > 0)
					{
						buyTimer.Start();
					}
					else
					{
						buyTimer.Stop();
					}
				}
			}

			//BUILDING PARTICLES
			if (HP <= 0 && fire2 == nullptr)
			{
				fPoint firePos2(GetEnclosingBox().x + 160, GetEnclosingBox().y + 20);
				fire2 = app->particleSystem->CreateStaticBucle(firePos2, true, FIRE);
				if (buildType == BUILD_TOWNCENTER)
				{
					app->missionManager->TheTownCenterIsDead();
				}
			}
			else if (HP >= fullHP * 0.5 && HP != fullHP)
			{
				if (fire == nullptr)
				{
					fPoint firePos(GetEnclosingBox().x + 60, GetEnclosingBox().y + 30);
					fire = app->particleSystem->CreateStaticBucle(firePos, true, FIRE);
				}
				else if (fire && fire2)
				{
					app->particleSystem->DestroyParticle(fire2);
					fire2 = nullptr;
				}
			}
			else if (HP == fullHP && fire)
			{
				app->particleSystem->DestroyParticle(fire);
				fire = nullptr;
			}
		}
		else if (buildType == BUILD_PORTAL)
		{
			if (portalParticle == nullptr && HP == fullHP)
			{
				fPoint particlePos(GetEnclosingBox().x + 10, GetEnclosingBox().y);
				portalParticle = app->particleSystem->CreateStaticBucle(particlePos, false, PORTAL);
			}
			else if (portalParticle != nullptr && HP <= 0)
			{
				app->particleSystem->DestroyParticle(portalParticle);
				portalParticle = nullptr;
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
	if (app->resources->GetCurrentGold() > unitGoldCost && app->resources->GetCurrentFood() > unitFoodCost && app->resources->GetCurrentWood() > unitWoodCost)
	{
		app->resources->SubstractGold(unitGoldCost);
		app->resources->SubstractFood(unitFoodCost);
		app->resources->SubstractWood(unitWoodCost);
		unitsToAdd++;
		buyTimer.Start();

	}
	else {
		app->audio->PlayFx(app->entityManager->fxError);
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

void Building::Repair(int amount)
{
	if (HP < fullHP && buildType != BUILD_PORTAL)
	{
		if (!app->entityManager->IsUnitInTile(nullptr, tileAttack)) {
			HP += amount;
			if (HP > fullHP)
			{
				HP = fullHP;
			}
		}
	}
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

void Building::PrintProgression()
{
	if (selected == true && unitsToAdd >= 1)
	{
		fPoint pos = GetGlobalPosition();
		int halfWidth = GetEnclosingBox().w * 0.5;
		iPoint progressPos(pos.x + halfWidth - (PROGRESS_WIDTH * 0.5), pos.y);

		int gwbar;
		gwbar = ((buyTimer.ReadSec() * 100) / BUY_TIMER);
		gwbar = (gwbar * PROGRESS_WIDTH) / 100;
		//red
		app->render->DrawQuad({ progressPos.x, progressPos.y, PROGRESS_WIDTH, 10 }, 255, 0, 0, 255);
		//green
		app->render->DrawQuad({ progressPos.x, progressPos.y, gwbar, 10 }, 0, 255, 0, 255);
	}
}