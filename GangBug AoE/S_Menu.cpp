#include "S_Menu.h"
#include "M_Audio.h"
#include "M_GUI.h"
#include "M_Textures.h"
#include "S_InGame.h"
#include "GUIImage.h"
#include "M_EntityManager.h"
#include "M_MissionManager.h"

S_Menu::S_Menu(bool startEnabled) : Module(startEnabled)
{
	name.assign("menu");
	active = true;
}

S_Menu::~S_Menu()
{
}

bool S_Menu::Awake(pugi::xml_node & config)
{
	app->audio->Start();

	return true;
}

bool S_Menu::Start()
{
	bool ret = true;
	//audio
	//app->audio->PlayTheme(app->audio->menuMusic);

	if (active)
	{
		app->gui->SetActiveScene(name);
		LoadUI();
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
		bg->Draw();
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
		/*if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
			GoToIngame();			*/
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

void S_Menu::LoadUI()
{
	bg = app->gui->CreateImage({ 0,0,(int)(1920 * app->gui->GetScaleX()),(int)(1080 * app->gui->GetScaleY()) }, { 0, 0, 1920, 1080 }, "background");

	bg->SetAtlas(app->tex->Load("gui/Menu_BackGround.png"));
	bg->SetVisible(true);

	app->gui->background.push_back(bg);
}

void S_Menu::DrawDebug()
{
	if (active)
	{

	}
}

void S_Menu::GoToIngame()
{
	CleanUp();
	this->active = false;	
	app->gui->SetActiveScene(app->inGame->name);

	bg->SetVisible(false);
	app->inGame->active = true;
	app->inGame->Start();
}

void S_Menu::GuiEvent(GUIElement* element, int64_t event)
{
	if (event & MOUSE_LCLICK_UP)
	{
		if (event & START_GAME)
		{
			app->gui->FindElement(app->gui->guiList, "easyMode")->SetActive(true);
			app->gui->FindElement(app->gui->guiList, "hardMode")->SetActive(true);
		}
		if (event & CLOSE_APP)
		{
			app->quit = true;
		}
		if (event & EASY_MODE_GAME)
		{
			app->missionManager->setGameToHardMode(false);
			app->gui->FindElement(app->gui->guiList, "easyMode")->SetLClicked(false);
			GoToIngame();
		}
		if (event & HARD_MODE_GAME)
		{
			app->missionManager->setGameToHardMode(true);
			app->gui->FindElement(app->gui->guiList, "hardMode")->SetLClicked(false);
			GoToIngame();
		}
	}
}