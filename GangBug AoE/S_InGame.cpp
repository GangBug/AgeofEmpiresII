#include "S_InGame.h"
#include "App.h"
#include "M_Map.h"
#include "M_Pathfinding.h"
#include "M_EntityManager.h"
#include "M_FogOfWar.h"
#include "M_Audio.h"
#include "M_EnemyWaves.h"
#include "S_Menu.h"

S_InGame::S_InGame(bool startEnabled) : Module(startEnabled)
{
	name.assign("inGame");
	active = false;
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
	if (active)
	{
		if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
			app->gui->SetActiveScene(name);
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
			GoToMenu();
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

		/*if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		{
			app->enemyWaves->SpawnEnemies(5, 0, 300, 200);
		}*/
		if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		{
			int x, y;
			app->input->GetMouseMapPosition(x, y);
			app->entityManager->CreateUnit(ARCHER, nullptr, x, y);
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



		//What is this: A tool to debug pathfinding
		if (app->debug)
		{
			if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
			{
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_state::KEY_UP)
				{
					iPoint point;
					app->input->GetMouseMapPosition(point.x, point.y);
					origin.x = app->map->WorldToMap(point.x, point.y).x;
					origin.y = app->map->WorldToMap(point.x, point.y).y;
				}
				if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_state::KEY_UP)
				{
					iPoint point;
					app->input->GetMouseMapPosition(point.x, point.y);
					destiny.x = app->map->WorldToMap(point.x, point.y).x;
					destiny.y = app->map->WorldToMap(point.x, point.y).y;

					app->pathfinding->CalculatePath(origin, destiny, path);
					SDL_Log("-------------\nOrigin: (%d, %d)\nDestiniy: (%d,%d)\n-------------",
							origin.x,
							origin.y,
							destiny.x,
							destiny.y);
				}
			}
		}


	}	
	return UPDATE_CONTINUE;
}

update_status S_InGame::Update(float dt)
{
	if (active)
	{

	}
	return UPDATE_CONTINUE;
}

update_status S_InGame::PostUpdate(float dt)
{
	if (active)
	{

	}
	return UPDATE_CONTINUE;
}

void S_InGame::Draw()
{
	if (active)
	{
		
	}
}

void S_InGame::DrawDebug()
{
	if (active)
	{
		for (auto node : path)
		{
			iPoint point;
			point = app->map->MapToWorld(node.x, node.y);
			app->render->DrawQuad({ point.x - 5, point.y - 5, 10, 10 }, 255, 255, 0, 255);
		}

	}
}

void S_InGame::GoToMenu()
{
	active = false;
	app->menu->active = true;
	app->gui->SetActiveScene("menu");
}