#include "Building.h"
#include "M_EntityManager.h"
#include "App.h"
#include "M_Input.h"
#include "M_Textures.h"



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
		break;
	case BUILD_STABLES:
		unitType = unit_type::TARKAN_KNIGHT;
		unitCost = 70;
		entityTexture = app->tex->stableTexture;
		SetEnclosingBoxSize(323, 226);
		break;
	case BUILD_BARRACK:
		entityTexture = app->tex->barracksTexture;
		SetEnclosingBoxSize(310, 266);
		break;
	}
}


Building::~Building()
{
}

void Building::OnUpdate(float dt)
{
	iPoint mPos;
	app->input->GetMousePosition(mPos.x, mPos.y);

	if (selected && GetEnclosingBox().Contains(mPos.x, mPos.y) && app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		BuyUnit();
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
	//Check gold

	//If theres money crate a unit
	
	fPoint pos = GetGlobalPosition();

	app->entityManager->CreateUnit(unitType, this, pos.x, pos.y + 10.0f);
}
