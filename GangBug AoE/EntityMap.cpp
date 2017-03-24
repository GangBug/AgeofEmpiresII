#include "EntityMap.h"
#include "M_Map.h"
#include "App.h"
#include "M_Pathfinding.h"

EntityMap::EntityMap(Entity* parent) : Entity(ENTITY_MAP, parent)
{
	name.assign("entity_map");

}


EntityMap::~EntityMap()
{
}

void EntityMap::OnStart()
{
	//if (app->map->Load("0.1Map.tmx") == true)
	//{
	//	int w, h;
	//	uchar* data = NULL;
	//	if (app->map->CreateWalkabilityMap(w, h, &data))
	//		app->pathfinding->SetMap(w, h, data);

	//	RELEASE_ARRAY(data);
	//}
}

void EntityMap::OnFinish()
{
	//TODO: Destroy all ui elements.
}

void EntityMap::OnEnable()
{
	//TODO: Enable all ui elements.
}

void EntityMap::OnDisable()
{
	//TODO: Disable all ui elements.
}

bool EntityMap::OnSave(pugi::xml_node& node)const
{
	//TODO: Save all ui elements.
	return true;
}

bool EntityMap::OnLoad(pugi::xml_node* node)
{
	//TODO: Load all ui elements.
	return true;
}
