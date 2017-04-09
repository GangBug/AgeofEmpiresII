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

void S_InGame::Draw()
{

}

void S_InGame::DrawDebug()
{
}
