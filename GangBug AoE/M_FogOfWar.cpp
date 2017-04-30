#include "M_FogOfWar.h"
#include "Iso_Primitives.h"
#include "App.h"
#include "M_Map.h"
#include "M_Render.h"
#include "M_Input.h"
#include "M_EntityManager.h"

//Constructors ========================
//Constructors ========================
M_FogOfWar::M_FogOfWar(bool startEnabled) : Module(startEnabled)
{
	name = "fog_of_war";
}

//Destructors =========================
M_FogOfWar::~M_FogOfWar()
{

}


update_status M_FogOfWar::PostUpdate(float dt)
{
	//Paste fog zone at the screen surface to render it
	//fog_quadtree.Draw();


	//if (app->debug)return UPDATE_CONTINUE;

	std::vector<AlphaCell*> in_view_cells;
	uint size = fog_quadtree.CollectCandidates(in_view_cells, app->render->camera->GetRect());
	for (uint k = 0; k < size; k++)
	{
		app->render->FogBlit(in_view_cells[k]->position, alpha_cell_size, 0);

		if (in_view_cells[k]->alpha < MID_ALPHA)in_view_cells[k]->alpha = MID_ALPHA;
	}

	return UPDATE_CONTINUE;
}

//Functionality =======================
void M_FogOfWar::GenerateFogOfWar()
{
	//Calculate alpha cells size
	alpha_cell_size = ceil(1000 / DIVISIONS_PER_PIXELS);

	//Check if alpha cell size is a multiple of 1000 to don't have pixels without fog
	while (1000 % (alpha_cell_size - MARGIN) != 0)alpha_cell_size++;

	//Calculate fog alpha layer size with the map data & alpha cells size
	alpha_layer_width = ceil((app->map->data.width * (app->map->data.tileWidth + MARGIN)) / alpha_cell_size);

	alpha_layer_height = ceil((app->map->data.height * (app->map->data.tileHeight + MARGIN)) / (alpha_cell_size - MARGIN));

	int mid_map_lenght = ceil(app->map->data.width * (app->map->data.tileWidth)) * -0.5;

	//Build fog quadtree boundaries & limit
	fog_quadtree.SetBoundaries({ (int)mid_map_lenght,0,(int)alpha_cell_size * (int)alpha_layer_width,(int)alpha_cell_size * (int)alpha_layer_height });
	fog_quadtree.SetMaxObjects(DIVISIONS_PER_PIXELS * 10);
	fog_quadtree.SetDebugColor({ 255,255,0,255 });

	//Build fog alpha layer
	//Allocate alpha layer cells
	alpha_layer = new AlphaCell[alpha_layer_width * alpha_layer_height];

	for (uint y = 0; y < alpha_layer_height; y++)
	{
		for (uint x = 0; x < alpha_layer_width; x++)
		{
			AlphaCell* current_cell = &alpha_layer[y * alpha_layer_width + x];

			current_cell->position = { (int)ceil(mid_map_lenght + (int)alpha_cell_size * (int)x), (int)ceil((int)alpha_cell_size * (int)y) };
			current_cell->alpha = ALPHA_LIMIT;
			if (!fog_quadtree.Insert(current_cell, &iPoint(current_cell->position.x + alpha_cell_size * 0.5, current_cell->position.y + alpha_cell_size * 0.5)))
			{
				LOG("Fail");
			}
		}
	}

	//Build fog type layer
	fog_layer = new FOG_TYPE[app->map->data.width * app->map->data.height];

	for (uint y = 0; y < app->map->data.height; y++)
	{
		for (uint x = 0; x < app->map->data.width; x++)
		{
			fog_layer[y * app->map->data.width + x] = DARK_FOG;
		}
	}
}

FOG_TYPE M_FogOfWar::GetFogID(int x, int y) const
{
	return fog_layer[y * app->map->data.width + x];
}

void M_FogOfWar::ClearAlphaLayer(const Circle zone, unsigned short alpha, bool radiancy)
{
	std::vector<AlphaCell*> fog_cells;
	uint size = fog_quadtree.CollectCandidates(fog_cells, zone);

	//Clear fog in vision area
	if (!radiancy)
	{
		for (uint k = 0; k < size; k++)
		{
			if (fog_cells[k]->alpha > alpha)fog_cells[k]->alpha = alpha;
		}
	}
	else
	{
		for (uint k = 0; k < size; k++)
		{
			float x_rad_percent = abs(((float)(fog_cells[k]->position.x) - (float)(zone.GetPosition().x))) / zone.GetRad();
			float y_rad_percent = abs(((fog_cells[k]->position.y) - (zone.GetPosition().y))) / (zone.GetRad() * sin(zone.GetXAngle()));
			float rad_percent = (x_rad_percent + y_rad_percent) * 0.5;

			if (fog_cells[k]->alpha > ALPHA_LIMIT * rad_percent)fog_cells[k]->alpha = ALPHA_LIMIT * rad_percent;
		}
	}

}

void M_FogOfWar::ClearFogLayer(const Circle zone, FOG_TYPE type)
{

	std::vector<iPoint> tiles_in;
	uint size = app->map->mapQuadtree.CollectCandidates(tiles_in, zone);

	for (uint k = 0; k < size; k++)
	{
		fog_layer[tiles_in[k].y * app->map->data.width + tiles_in[k].x] = type;
	}

}

void M_FogOfWar::DrawDebug()
{
}