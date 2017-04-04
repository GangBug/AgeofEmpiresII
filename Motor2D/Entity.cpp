#include "Entity.h"
#include "Buildings.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1EntityManager.h"

Entity::Entity(ENTITY_TYPE entity_type, fPoint pos): to_delete (false), entity_type(entity_type), position(pos)
{
}

Entity::~Entity()
{
}

void Entity::Die()
{
	to_delete = true;
}

bool Entity::ToDelete() const
{
	return to_delete;
}

ENTITY_TYPE Entity::GetEntityType() const
{
	return entity_type;
}

ENTITY_STATUS Entity::GetEntityStatus()
{
	return entity_status;
}

void Entity::SetEntityStatus(ENTITY_STATUS status)
{
	entity_status = status;

	if (entity_type == BUILDING && status == E_SELECTED)
	{
		GUIElement* tmpElement = nullptr;

		switch (dynamic_cast<Building*>(this)->buildingType)
		{
		case ARCHERY:
			App->entity_manager->archerySelected = true;
			App->entity_manager->barracksSelected = false;
			App->entity_manager->stableSelected = false;

			tmpElement = App->gui->FindElement(App->gui->guiList, "ArcherCreatorButton");
			tmpElement->SetActive(true);
			tmpElement = App->gui->FindElement(App->gui->guiList, "SamuraiCreatorButton");
			tmpElement->SetActive(false);
			tmpElement = App->gui->FindElement(App->gui->guiList, "TarkanCreatorButton");
			tmpElement->SetActive(false);

			tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonArchery");
			tmpElement->SetActive(true);
			tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonStable");
			tmpElement->SetActive(false);
			tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonBarrack");
			tmpElement->SetActive(false);


			break;
		case BARRACK:
			App->entity_manager->archerySelected = false;
			App->entity_manager->barracksSelected = true;
			App->entity_manager->stableSelected = false;

			tmpElement = App->gui->FindElement(App->gui->guiList, "ArcherCreatorButton");
			tmpElement->SetActive(false);
			tmpElement = App->gui->FindElement(App->gui->guiList, "SamuraiCreatorButton");
			tmpElement->SetActive(true);
			tmpElement = App->gui->FindElement(App->gui->guiList, "TarkanCreatorButton");
			tmpElement->SetActive(false);

			tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonArchery");
			tmpElement->SetActive(false);
			tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonStable");
			tmpElement->SetActive(false);
			tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonBarrack");
			tmpElement->SetActive(true);


			break;
		case STABLE:
			App->entity_manager->archerySelected = false;
			App->entity_manager->barracksSelected = false;
			App->entity_manager->stableSelected = true;

			tmpElement = App->gui->FindElement(App->gui->guiList, "ArcherCreatorButton");
			tmpElement->SetActive(false);
			tmpElement = App->gui->FindElement(App->gui->guiList, "SamuraiCreatorButton");
			tmpElement->SetActive(false);
			tmpElement = App->gui->FindElement(App->gui->guiList, "TarkanCreatorButton");
			tmpElement->SetActive(true);

			tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonArchery");
			tmpElement->SetActive(false);
			tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonStable");
			tmpElement->SetActive(true);
			tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonBarrack");
			tmpElement->SetActive(false);


			break;
		}
	}
}

void Entity::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

int Entity::GetHP() const
{
	return hp;
}
const float Entity::GetX() const
{
	return position.x;
}

const float Entity::GetY() const
{
	return position.y;
}

void Entity::SetArmor(int new_armor)
{
	armor = new_armor;
}

void Entity::SetHp(int new_hp)
{
	hp = new_hp;
}
