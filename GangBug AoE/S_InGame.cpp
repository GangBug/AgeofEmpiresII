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
	//if (app->map->Load("0.1Map.tmx") == true)
	if (app->map->Load("testingMap.tmx") == true)
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

	GB_Rectangle<int> rect;
	app->input->GetMousePosition(rect.x, rect.y);
	rect.w = 10;
	rect.h = 10;

	if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_state::KEY_UP)
	{
		app->render->camera->InsideRenderTarget(rect);
	}
	
	
	

	return UPDATE_CONTINUE;
}

void S_InGame::Draw()
{

}

void S_InGame::DrawDebug()
{
}
