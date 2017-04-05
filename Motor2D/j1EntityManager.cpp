#include "j1App.h"
#include "j1EntityManager.h"
#include "Units.h"
#include "Boss.h"
#include "j1Map.h"
#include "GB_Rectangle.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1FileSystem.h"
#include "p2Log.h"

j1EntityManager::j1EntityManager() : j1Module()
{
	name.assign("entity_manager");
}

j1EntityManager::~j1EntityManager() {}


bool j1EntityManager::Awake(pugi::xml_node& node) 
{ 
	archerySelected = false;
	barracksSelected = false;
	stableSelected = false;

	LoadObjects();

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

bool j1EntityManager::LoadObjects()
{
	std::string obj_folder = "objects/Objects_data.xml";	//Load Objects data from object folder
	char* buff = nullptr;
	int size = App->fs->Load(obj_folder.c_str(), &buff);
	pugi::xml_document obj_data;
	pugi::xml_parse_result result = obj_data.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading objects data: %s", result.description());
		return false;
	}

	//Loading Objects Sprites
	pugi::xml_node spriteNode = obj_data.child("TextureAtlas").first_child();
	while (spriteNode != NULL)
	{
		ObjectTexture newObject(OBJECT_NONE, {});
		newObject.SetType(spriteNode);
		newObject.SetRect(spriteNode);

		if (newObject.type != OBJECT_NONE)
		{
			objectTextures.push_back(newObject);
		}
		spriteNode = spriteNode.next_sibling();
	}

	return true;
}

bool j1EntityManager::PlaceObjects()
{
	std::string obj_folder = "objects/Objects_creator.xml";	//Load Objects data from object folder
	char* buff = nullptr;
	int size = App->fs->Load(obj_folder.c_str(), &buff);
	pugi::xml_document obj_data;
	pugi::xml_parse_result result = obj_data.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading objects data: %s", result.description());
		return false;
	}

	//Loading Objects Sprites
	pugi::xml_node objectNode = obj_data.child("Objects").first_child();
	while (objectNode != NULL)
	{
		CreateObject(OBJECT_TYPE(objectNode.attribute("type").as_int()), { objectNode.attribute("x").as_float(),objectNode.attribute("y").as_float() });
		objectNode = objectNode.next_sibling();
	}

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

Entity* j1EntityManager::CreateObject(OBJECT_TYPE type, fPoint pos)
{
	objectID++;
	Entity* new_entity = (Entity*) new Object(type, pos);
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

void j1EntityManager::DispawnEnemies()
{
	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		if ((*it)->GetEntityType() == UNIT) {
			if (dynamic_cast<Unit*>(*it)->GetUnitType() == VILE)
				dynamic_cast<Unit*>(*it)->SetHp(0);
		}
	}
}

void j1EntityManager::UnselectEverything()
{
	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); it++)
	{
		if ((*it)->GetEntityType() == BUILDING && (*it)->GetEntityStatus() == E_SELECTED)
		{
			GUIElement* tmpElement = nullptr;

			if (App->entity_manager->archerySelected == true)
			{
				tmpElement = App->gui->FindElement(App->gui->guiList, "ArcherCreatorButton");

				if (tmpElement->GetMouseInside() == false)
				{
					it._Ptr->_Myval->SetEntityStatus(E_NON_SELECTED);
					tmpElement->SetActive(false);
					tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonArchery");
					tmpElement->SetActive(false);
				}
			}
			if (App->entity_manager->barracksSelected == true)
			{
				tmpElement = App->gui->FindElement(App->gui->guiList, "SamuraiCreatorButton");

				if (tmpElement->GetMouseInside() == false)
				{
					it._Ptr->_Myval->SetEntityStatus(E_NON_SELECTED);
					tmpElement->SetActive(false);
					tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonBarrack");
					tmpElement->SetActive(false);
				}
			}
			if (App->entity_manager->stableSelected == true)
			{
				tmpElement = App->gui->FindElement(App->gui->guiList, "TarkanCreatorButton");

				if (tmpElement->GetMouseInside() == false)
				{
					it._Ptr->_Myval->SetEntityStatus(E_NON_SELECTED);
					tmpElement->SetActive(false);
					tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonStable");
					tmpElement->SetActive(false);
				}
			}
		}
		else
		{
			it._Ptr->_Myval->SetEntityStatus(E_NON_SELECTED);
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
		if ((*it)->GetX() > 10000 || (*it)->GetY() > 10000)
		{
			(*it)->GetX();
		}
		(*it)->Update();
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

SDL_Rect j1EntityManager::getObjectRect(OBJECT_TYPE type)
{
	for (std::vector<ObjectTexture>::iterator it = objectTextures.begin(); it != objectTextures.end(); it++)
	{
		if ((*it).type == type)
		{
			return (*it).section;
		}
	}
	return{ 0,0,0,0 };
}

void ObjectTexture::SetType(pugi::xml_node node)
{
	if (strcmp(node.attribute("n").as_string(), "asianWallH") == 0) {
		type = ASIAN_WALL_H;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallL") == 0) {
		type = ASIAN_WALL_L;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallR") == 0) {
		type = ASIAN_WALL_R;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallT") == 0) {
		type = ASIAN_WALL_T;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallV") == 0) {
		type = ASIAN_WALL_V;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallRdmg") == 0) {
		type = ASIAN_WALL_R_DMG;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallLdmg") == 0) {
		type = ASIAN_WALL_L_DMG;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallTdmg") == 0) {
		type = ASIAN_WALL_T_DMG;
	}
	else if (strcmp(node.attribute("n").as_string(), "asianWallVdmg") == 0) {
		type = ASIAN_WALL_V_DMG;
	}
	else if (strcmp(node.attribute("n").as_string(), "Bamboo1") == 0) {
		type = BAMBOO1;
	}
	else if (strcmp(node.attribute("n").as_string(), "Bamboo2") == 0) {
		type = BAMBOO2;
	}
	else if (strcmp(node.attribute("n").as_string(), "Bamboo3") == 0) {
		type = BAMBOO3;
	}
	else if (strcmp(node.attribute("n").as_string(), "Bamboo4") == 0) {
		type = BAMBOO4;
	}
	else if (strcmp(node.attribute("n").as_string(), "BannerA") == 0) {
		type = BANNERA;
	}
	else if (strcmp(node.attribute("n").as_string(), "BannerB") == 0) {
		type = BANNERB;
	}
	else if (strcmp(node.attribute("n").as_string(), "Charriot") == 0) {
		type = CHARRIOT;
	}
	else if (strcmp(node.attribute("n").as_string(), "PalisadeR") == 0) {
		type = PALISADE_R;
	}
	else if (strcmp(node.attribute("n").as_string(), "PalisadeL") == 0) {
		type = PALISADE_L;
	}
	else if (strcmp(node.attribute("n").as_string(), "PalisadeT") == 0) {
		type = PALISADE_T;
	}
	else if (strcmp(node.attribute("n").as_string(), "PalisadeH") == 0) {
		type = PALISADE_H;
	}
	else if (strcmp(node.attribute("n").as_string(), "PalisadeV") == 0) {
		type = PALISADE_V;
	}
	else if (strcmp(node.attribute("n").as_string(), "SkeletonsGroup") == 0) {
		type = SKELETONS_GROUP;
	}
	else if (strcmp(node.attribute("n").as_string(), "SkullPile") == 0) {
		type = SKULL_PILE;
	}
	else if (strcmp(node.attribute("n").as_string(), "TorchFloorSquare") == 0) {
		type = TORCH_FLOOR_SQUARE;
	}
	else if (strcmp(node.attribute("n").as_string(), "TorchFloorTwisted") == 0) {
		type = TORCH_FLOOR_TWISTED;
	}
	else if (strcmp(node.attribute("n").as_string(), "TrashLarge") == 0) {
		type = TRASH_LARGE;
	}
	else if (strcmp(node.attribute("n").as_string(), "TrashMedium") == 0) {
		type = TRASH_MEDIUM;
	}
	else if (strcmp(node.attribute("n").as_string(), "TrashSmall") == 0) {
		type = TRASH_SMALL;
	}
	else if (strcmp(node.attribute("n").as_string(), "Altar") == 0) {
		type = ALTAR;
	}
	else
	{
		type = OBJECT_NONE;
		LOG("ERROR: XML Node OBJECT TYPE does not match");
	}
}

void ObjectTexture::SetRect(pugi::xml_node node)
{
	section.x = node.attribute("x").as_int();
	section.y = node.attribute("y").as_int();
	section.w = node.attribute("w").as_int();
	section.h = node.attribute("h").as_int();
}

ObjectTexture::ObjectTexture(OBJECT_TYPE type, SDL_Rect rect)
{
	this->type = type;
	section = rect;
}