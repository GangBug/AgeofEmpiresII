#include "S_InGame.h"
#include "App.h"
#include "M_Map.h"
#include "M_Pathfinding.h"



S_InGame::S_InGame(bool startEnabled) : Module(startEnabled)
{
	name.assign("inGame");

}


S_InGame::~S_InGame()
{
}

bool S_InGame::Awake(pugi::xml_node & config)
{
	return true;
}

bool S_InGame::Start()
{
	//app->gui->SetActiveScene(name);
	if (app->map->Load("0.1Map.tmx") == true)
	//if (app->map->Load("testingMap.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data))
			app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	return true;
}

update_status S_InGame::PreUpdate(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		app->gui->SetActiveScene(name);
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		app->gui->SetActiveScene("menu");
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		app->gui->SetActiveScene("\0");

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera->Move(10.0, UP);
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera->Move(10.0, LEFT);
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera->Move(10.0, DOWN);
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera->Move(10.0, RIGHT);

	if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_state::KEY_UP)
		{		
			app->input->GetMousePosition(origin.x, origin.y);
			origin.x = app->render->ScreenToWorld(origin.x, origin.y).x;
			origin.y = app->render->ScreenToWorld(origin.x, origin.y).y;
			origin.x = app->map->WorldToMap(origin.x, origin.y).x;
			origin.y = app->map->WorldToMap(origin.x, origin.y).y;
		}	
		if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_state::KEY_UP)
		{
			app->input->GetMousePosition(destiny.x, destiny.y);
			destiny.x = app->render->ScreenToWorld(destiny.x, destiny.y).x;
			destiny.y = app->render->ScreenToWorld(destiny.x, destiny.y).y;
			destiny.x = app->map->WorldToMap(destiny.x, destiny.y).x;
			destiny.y = app->map->WorldToMap(destiny.x, destiny.y).y;

			app->pathfinding->CreatePath(origin, destiny);
			path = app->pathfinding->GetLastPath();

		}

	}


	return UPDATE_CONTINUE;
}

void S_InGame::Draw()
{

}

void S_InGame::DrawDebug()
{
	for (std::vector<iPoint>::iterator node = path.begin(); node != path.end(); ++node)
	{
		app->render->DrawLine((*node).x, (*node).y, (*(node + 1)).x, (*(node + 1)).y, 255, 0, 0, 255, false);
	}
}
