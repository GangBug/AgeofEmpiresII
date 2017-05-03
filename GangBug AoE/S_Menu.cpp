#include "S_Menu.h"
#include "M_Audio.h"
#include "M_GUI.h"

#include "S_InGame.h"

S_Menu::S_Menu(bool startEnabled) : Module(startEnabled)
{
	name.assign("menu");
}


S_Menu::~S_Menu()
{
}

bool S_Menu::Awake(pugi::xml_node & config)
{
	return true;
}

bool S_Menu::Start()
{
	bool ret = true;
	//audio
	//app->audio->PlayTheme(app->audio->thirdMission);
	if (!app->menuOrIngame)
	{
		app->gui->SetActiveScene(name);
	}

	return ret;
}

update_status S_Menu::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (!app->menuOrIngame)
	{
		
	}

	return ret;
}

update_status S_Menu::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (!app->menuOrIngame)
	{

	}

	return ret;
}

update_status S_Menu::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (!app->menuOrIngame)
	{

		if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
			GoToIngame();

		if (app->menuOrIngame)
		{			
			app->gui->SetActiveScene(app->inGame->name);
		}
	}
	return ret;
}

bool S_Menu::CleanUp()
{
	bool ret = true;

	if (!app->menuOrIngame)
	{

	}

	return ret;
}

void S_Menu::DrawDebug()
{
	if (!app->menuOrIngame)
	{

	}
}

void S_Menu::GoToIngame()
{
	app->menuOrIngame = true;
}
