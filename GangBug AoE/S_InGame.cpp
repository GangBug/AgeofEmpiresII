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



		

	return UPDATE_CONTINUE;
}

void S_InGame::Draw()
{

}

void S_InGame::DrawDebug()
{
}
