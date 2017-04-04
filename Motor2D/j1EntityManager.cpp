#include "j1App.h"
#include "j1EntityManager.h"
#include "Units.h"
#include "Boss.h"
#include "j1Map.h"
#include "GB_Rectangle.h"
#include "j1Gui.h"
#include "j1Scene.h"

j1EntityManager::j1EntityManager() : j1Module()
{
	name.assign("entity_manager");
}

j1EntityManager::~j1EntityManager() {}


bool j1EntityManager::Awake() 
{ 
	archerySelected = false;
	barracksSelected = false;
	stableSelected = false;

	return true; 
}

bool j1EntityManager::CleanUp() 
{
	std::list<Entity*>::iterator item = entity_list.begin();
	while (item != entity_list.end())
	{
		RELEASE(item._Ptr->_Myval);
		item++;
	}
	entity_list.clear();

	return true;
}

Entity * j1EntityManager::CreateUnit(UNIT_TYPE u_type, fPoint pos)
{
	unitID++;
	Entity* new_entity = (Entity*) new Unit(u_type, pos, unitID);
	entity_list.push_back(new_entity);
	return new_entity;
}

Entity * j1EntityManager::CreateBuilding(BUILDING_TYPE type, fPoint pos)
{
	buildingID++;
	Entity* new_entity = (Entity*) new Building(type, pos, buildingID);
	entity_list.push_back(new_entity);
	return new_entity;
}

Entity * j1EntityManager::CreateBoss(fPoint pos)
{
	unitID++;
	Entity* new_entity = (Entity*) new Boss(BOSS, pos, unitID);
	entity_list.push_back(new_entity);
	return new_entity;
}

void j1EntityManager::SelectInQuad(const SDL_Rect& select_rect)
{
	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		int unit_x = it._Ptr->_Myval->GetX();
		int unit_y = it._Ptr->_Myval->GetY();

		if (it._Ptr->_Myval->GetEntityType() != BUILDING)
		{
			if (unit_x > select_rect.x && unit_x < select_rect.w && unit_y > select_rect.y && unit_y < select_rect.h)
			{
				it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
			}
			else if (unit_x < select_rect.x && unit_x > select_rect.w && unit_y < select_rect.y && unit_y > select_rect.h)
			{
				it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
			}
			else if (unit_x > select_rect.x && unit_x < select_rect.w && unit_y < select_rect.y && unit_y > select_rect.h)
			{
				it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
			}
			else if (unit_x < select_rect.x && unit_x > select_rect.w && unit_y > select_rect.y && unit_y < select_rect.h)
			{
				it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
			}
		}
		if (it._Ptr->_Myval->GetEntityType() == BUILDING)
		{
			unit_x += 100;
			unit_y += 100;
			if (unit_x > select_rect.x && unit_x < select_rect.w && unit_y > select_rect.y && unit_y < select_rect.h)
			{
				it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
			}
			else if (unit_x < select_rect.x && unit_x > select_rect.w && unit_y < select_rect.y && unit_y > select_rect.h)
			{
				it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
			}
			else if (unit_x > select_rect.x && unit_x < select_rect.w && unit_y < select_rect.y && unit_y > select_rect.h)
			{
				it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
			}
			else if (unit_x < select_rect.x && unit_x > select_rect.w && unit_y > select_rect.y && unit_y < select_rect.h)
			{
				it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
			}
		}
	}
}

void j1EntityManager::SelectInClick(int x, int y)
{
	GB_Rectangle<int> mouseClick = { x-15, y, 30, 55 };

	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		int unit_x = it._Ptr->_Myval->GetX();
		int unit_y = it._Ptr->_Myval->GetY();
		
		if (mouseClick.Contains(unit_x, unit_y))
		{
			it._Ptr->_Myval->SetEntityStatus(E_SELECTED);
		}
	}
}

void j1EntityManager::UnselectEverything()
{
	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
			it._Ptr->_Myval->SetEntityStatus(E_NON_SELECTED);
	}

	if(App->scene->IsInGame())
	{
		GUIElement* tmpElement = nullptr;

		if (App->entity_manager->archerySelected == true)
		{
			tmpElement = App->gui->FindElement(App->gui->guiList, "ArcherCreatorButton");
			tmpElement->SetActive(false);
			tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonArchery");
			tmpElement->SetActive(false);
		}
		if (App->entity_manager->barracksSelected == true)
		{
			tmpElement = App->gui->FindElement(App->gui->guiList, "SamuraiCreatorButton");
			tmpElement->SetActive(false);
			tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonBarrack");
			tmpElement->SetActive(false);
		}
		if (App->entity_manager->stableSelected == true)
		{
			tmpElement = App->gui->FindElement(App->gui->guiList, "TarkanCreatorButton");
			tmpElement->SetActive(false);
			tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonStable");
			tmpElement->SetActive(false);
		}
	}

}

void j1EntityManager::DeleteEntity(Entity * ptr)
{
	switch (ptr->GetEntityType())
	{
		case UNIT:
			DeleteUnit(ptr);
			break;
	}
}

void j1EntityManager::DeleteUnit(Entity * ptr)
{
	entity_list.remove(ptr);
	delete ptr;
}


//TODO: Delete this

bool j1EntityManager::IsUnitInTile(const Unit* unit, const iPoint tile)const
{
	for (std::list<Entity*>::iterator units = App->entity_manager->entity_list.begin(); units != App->entity_manager->entity_list.end(); units++)
	{
		if (units._Ptr->_Myval != unit)
		{
			if (tile == App->map->WorldToMap(units._Ptr->_Myval->GetX(), units._Ptr->_Myval->GetY()))
				return true;
		}
	}

	return false;
}

Unit* j1EntityManager::GetUnitInTile(iPoint tile)const
{
	for (std::list<Entity*>::iterator units = App->entity_manager->entity_list.begin(); units != App->entity_manager->entity_list.end(); units++)
	{
			if (tile == App->map->WorldToMap((*units)->GetX(), (*units)->GetY()))
				return ((Unit*)*units);
		}
}

bool j1EntityManager::Update(float dt)
{

	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); ++it)
	{
		it._Ptr->_Myval->Update();
	}

	return true;
}

bool j1EntityManager::PostUpdate()
{
	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); ++it)
	{
		if (it._Ptr->_Myval->ToDelete() == true)
		{
			DeleteEntity(it._Ptr->_Myval);
			entity_list.erase(it);
		}
		else
		{
			(*it)->PostUpdate();
		}
	}
	return true;
}