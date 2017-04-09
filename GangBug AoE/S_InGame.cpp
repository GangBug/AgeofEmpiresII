#include "S_InGame.h"



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
	app->gui->SetActiveScene(name);

	return true;
}

update_status S_InGame::PreUpdate(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		app->gui->SetActiveScene(name);
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		app->gui->SetActiveScene("menu");


		

	return UPDATE_CONTINUE;
}

void S_InGame::Draw()
{

}

void S_InGame::DrawDebug()
{
}
