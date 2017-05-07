#include "S_Menu.h"
#include "M_Audio.h"
#include "M_GUI.h"

#include "S_InGame.h"

S_Menu::S_Menu(bool startEnabled) : Module(startEnabled)
{
	name.assign("menu");
	active = false;
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
	if (active)
	{
		app->gui->SetActiveScene(name);
	}

	return ret;
}

update_status S_Menu::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (active)
	{
		
	}

	return ret;
}

update_status S_Menu::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (active)
	{

	}

	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		return UPDATE_STOP;
	}

	return ret;
}

update_status S_Menu::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (active)
	{
		if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
			GoToIngame();			
	}
	return ret;
}

bool S_Menu::CleanUp()
{
	bool ret = true;

	if (active)
	{

	}

	return ret;
}

void S_Menu::DrawDebug()
{
	if (active)
	{

	}
}

void S_Menu::GoToIngame()
{
	this->active = false;
	app->inGame->active = true;
	
	app->gui->SetActiveScene(app->inGame->name);
	app->inGame->Start();
}

void S_Menu::GuiEvent(GUIElement* element, int64_t event)
{
	if (event & MOUSE_LCLICK_UP)
	{
		if (event & START_GAME)
		{
			GoToIngame();
		}
	}
}