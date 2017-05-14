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
#include "M_Metrics.h"

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
		unitLimitCount = 6;
		currentUnitCount = 0;

		HP = 150;
		fullHP = 150;
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
		unitLimitCount = 10;
		currentUnitCount = 0;

		HP = 150;
		fullHP = 150;
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
		unitLimitCount = 20;
		currentUnitCount = 0;

		HP = 150;
		fullHP = 150;
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
		unitFoodCost = 40;
		unitLimitCount = 10;
		currentUnitCount = 0;

		HP = 450;
		fullHP = 450;
		horde = false;

		entityTexture = app->tex->townCenterTexture;
		SetEnclosingBoxSize(382, 399);
		creatorButton = app->gui->FindElement(app->gui->guiList, "VillagerCreatorButton");
		unitInfoLabel = app->gui->FindElement(app->gui->guiList, "label_VillagerCost");

		break;

	case BUILD_PORTAL:
		unitType = VILE;
		horde = true;

		HP = 100;
		fullHP = 100;

		entityTexture = app->tex->portalTexture;
		SetEnclosingBoxSize(155, 233);
		HP = 0;
		break;
	}

	selected = false;
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

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && GetEnclosingBox().Contains(mPos.x, mPos.y))
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
					if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
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
					currentUnitCount++;

					unitsToAdd--;
					if (unitsToAdd > 0)
					{
						buyTimer.Start();
					}
					else
					{
						buyTimer.Stop();
					}

					if (unitType == SAMURAI)
						app->metrics->AddSamuraiAlive();
					if (unitType == TARKAN_KNIGHT)
						app->metrics->AddTarkanAlive();
					if (unitType == ARCHER)
						app->metrics->AddArcherAlive();

					app->metrics->AddTotalUnit();
				}
				else
				{
					app->metrics->AddVillagersAlive();
					app->resources->AddVillager();
					app->audio->PlayFx(app->entityManager->fxCreateVillager);
					currentUnitCount++;
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
			if (portalParticle == nullptr && HP > 0)
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
	if (app->resources->GetCurrentGold() > unitGoldCost && app->resources->GetCurrentFood() > unitFoodCost && app->resources->GetCurrentWood() > unitWoodCost && HP == fullHP && currentUnitCount < unitLimitCount)
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

void Building::SetHP(int HP)
{
	this->HP = HP;
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
	if (selected == true)
	{
		fPoint pos = GetGlobalPosition();
		int halfWidth = GetEnclosingBox().w * 0.5;
		int halfHeight = GetEnclosingBox().h * 0.5;

		if (unitsToAdd >= 1)
		{
			iPoint progressPos(pos.x + halfWidth - (PROGRESS_WIDTH * 0.5), pos.y);

			int gwbar;
			gwbar = ((buyTimer.ReadSec() * 100) / BUY_TIMER);
			gwbar = (gwbar * PROGRESS_WIDTH) / 100;
			//red
			app->render->DrawQuad({ progressPos.x, progressPos.y, PROGRESS_WIDTH, 10 }, 255, 0, 0, 255);
			//green
			app->render->DrawQuad({ progressPos.x, progressPos.y, gwbar, 10 }, 0, 255, 0, 255);
		}

		iPoint lifePos(pos.x + halfWidth - 35, pos.y + halfHeight - 50);

		int glbar;
		if (HP < 0)
		{
			HP = 0;
		}
		glbar = ((HP * 100) / fullHP);
		glbar = (glbar*PROGRESS_WIDTH) / 100;
		//red
		app->render->DrawQuad({ lifePos.x, lifePos.y, PROGRESS_WIDTH, 7 }, 255, 0, 0, 255);
		//green
		app->render->DrawQuad({ lifePos.x, lifePos.y, glbar, 7 }, 0, 255, 0, 255);
	}
}

void Building::Serialize(pugi::xml_node& node)
{
	pugi::xml_node bNode = node.append_child("building");

	bNode.append_attribute("posX") = GetGlobalPosition().x;
	bNode.append_attribute("posY") = GetGlobalPosition().y;
	bNode.append_attribute("hp") = HP;
	bNode.append_attribute("tileAttackX") = tileAttack.x;
	bNode.append_attribute("tileAttackY") = tileAttack.y;

	switch (buildType)
	{
	case BUILD_ARCHERY: bNode.append_attribute("type") = "archery"; break;
	case BUILD_BARRACK: bNode.append_attribute("type") = "barrack"; break;
	case BUILD_STABLES: bNode.append_attribute("type") = "stables"; break;
	case BUILD_TOWNCENTER: bNode.append_attribute("type") = "townCenter"; break;
	case BUILD_PORTAL: bNode.append_attribute("type") = "portal"; break;
	}
}