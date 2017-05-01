#include "S_InGame.h"
#include "App.h"
#include "M_Map.h"
#include "M_Pathfinding.h"
#include "M_EntityManager.h"
#include "M_FogOfWar.h"
#include "M_Audio.h"

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
	app->pathfinding->Enable();
	app->entityManager->LoadObjects();


	//audio
	//app->audio->PlayTheme(app->audio->thirdMission);


	//app->gui->SetActiveScene(name);
	if (app->map->Load("0.1Map.tmx") == true)
	//if (app->map->Load("testingMap.tmx") == true)
	{
		
		int w, h;
		uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data))
			app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

		//app->fogOfWar->GenerateFogOfWar();
	}
	app->entityManager->PlaceObjects();

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


	//What is this: ?
	if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_state::KEY_UP)
		{		
			app->input->GetMouseScreenPosition(origin.x, origin.y);
			origin.x = app->map->WorldToMap(origin.x, origin.y).x;
			origin.y = app->map->WorldToMap(origin.x, origin.y).y;
		}	
		if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_state::KEY_UP)
		{
			app->input->GetMouseScreenPosition(destiny.x, destiny.y);
			destiny.x = app->map->WorldToMap(destiny.x, destiny.y).x;
			destiny.y = app->map->WorldToMap(destiny.x, destiny.y).y;

			//app->pathfinding->CreatePath(origin, destiny);
			//path = app->pathfinding->GetLastPath();

		}
	}   
	if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		int x, y;
		app->input->GetMouseMapPosition(x, y);
		app->entityManager->CreateUnit(SAMURAI, nullptr, x, y);
		app->audio->PlayFx(app->entityManager->fxCreateUnit);
	}
	if (app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
	{
		int x, y;
		app->input->GetMouseMapPosition(x, y);
		app->entityManager->CreateUnit(TARKAN_KNIGHT, nullptr, x, y);
		app->audio->PlayFx(app->entityManager->fxCreateUnit);
	}
	if (app->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
	{
		int x, y;
		app->input->GetMouseMapPosition(x, y);
		app->entityManager->CreateUnit(VILE, nullptr, x, y);
		app->audio->PlayFx(app->entityManager->fxCreateUnit);
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
