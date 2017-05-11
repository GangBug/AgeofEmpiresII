#include "App.h"
#include "M_Minimap.h"
#include "M_Render.h"
#include "M_FileSystem.h"
#include "M_Textures.h"
#include "M_Map.h"
#include "M_EntityManager.h"
#include "Log.h"
#include "Entity.h"
#include <math.h>

#include "SDL\include\SDL.h"
#include "SDL_TTF\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

#define MAP_POS_X 1239 - app->render->camera->GetPosition().x + (mini_tiles[i].coords.x * 0.027)
#define MAP_POS_Y 640 - app->render->camera->GetPosition().y + (mini_tiles[i].coords.y * 0.027)
#define UNITS_POS_X 1239 - app->render->camera->GetPosition().x + (coords.x * 0.027)
#define UNITS_POS_Y 640 - app->render->camera->GetPosition().y + (coords.y * 0.027)

M_Minimap::M_Minimap(bool startEnabled) : Module(startEnabled)
{
	name.assign("minimap");
}

//Destructor
M_Minimap::~M_Minimap()
{}

bool M_Minimap::Awake(pugi::xml_node& config)
{
	return true;
}

bool M_Minimap::CleanUp()
{
	app->tex->UnLoad(minimap_atlas);
	mini_tiles.clear();
	return true;
}

void M_Minimap::CreateMinimap()
{
	minimap_atlas = app->tex->Load("maps/minimap.png");
	if (app->map->active == false)
		return;

	for (std::list<MapLayer*>::iterator it = app->map->data.layers.begin(); it != app->map->data.layers.end(); it++)
	{
		MapLayer* layer = *it;

		if (layer->properties.Get("Nodraw") == 0)
			continue;

		for (int y = 0; y < app->map->data.height; ++y)
		{
			for (int x = 0; x < app->map->data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					minimap_tiles tile;

					tile.coords = app->map->MapToWorld(x, y);
					tile.id = tile_id;

					mini_tiles.push_back(tile);
				}
			}
		}
	}
}

void M_Minimap::DrawMinimap() 
{
	if (this != nullptr)
	{
		for (uint i = 0; i < mini_tiles.size(); i++) {
			if (mini_tiles[i].id == 297)

				app->render->Blit(minimap_atlas, MAP_POS_X, MAP_POS_Y, &green);

			else

				app->render->Blit(minimap_atlas, MAP_POS_X, MAP_POS_Y, &blue);
		}

		std::vector<Entity*> uVector = app->entityManager->GetUnitVector();
		for (std::vector<Entity*>::iterator it = uVector.begin(); it != uVector.end(); it++) {

			iPoint coords;
			coords.x = (*it)->GetGlobalPosition().x;
			coords.y = (*it)->GetGlobalPosition().y;
			if (dynamic_cast<Unit*>(*it)->horde == true)
			{
				app->render->Blit(minimap_atlas, UNITS_POS_X, UNITS_POS_Y, &red);
			}
			else
			{
				app->render->Blit(minimap_atlas, UNITS_POS_X, UNITS_POS_Y, &yellow);
			}
		}
	}
}

void M_Minimap::DrawDebug()
{
}