#include "S_Score.h"
#include "S_Menu.h"
#include "M_Audio.h"
#include "M_GUI.h"
#include "M_MissionManager.h"
#include "M_Textures.h"
#include "M_Metrics.h"
#include "App.h"
#include "M_EntityManager.h"
S_Score::S_Score(bool startEnabled) : Module(startEnabled)
{
	name.assign("score");
	active = false;
}


S_Score::~S_Score()
{
}

bool S_Score::Awake(pugi::xml_node & config)
{
	app->audio->Start();
	return true;
}

bool S_Score::Start()
{
	//audio
	//app->audio->PlayTheme(app->audio->menuMusic);

	if (active)
	{
		app->gui->SetActiveScene(name);
		LoadUI();
	}

	return true;
}

update_status S_Score::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (active)
	{
		bg->Draw();
	}

	return ret;
}

update_status S_Score::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (active)
	{

	}

	return ret;
}

update_status S_Score::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (active)
	{
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
			GoToMenu();
	}

	return ret;
}

bool S_Score::CleanUp()
{
	bool ret = true;

	if (active)
	{

	}

	return ret;
}

void S_Score::LoadUI()
{
	
	bg = app->gui->CreateImage({ 0,0,(int)(1920 * app->gui->GetScaleX()),(int)(1080 * app->gui->GetScaleY()) }, { 0, 0, 1920, 1080 }, "background");
	bg->SetVisible(true);
	bg->SetInteractive(true);

	if (app->missionManager->GetState() == M_VICTORY) 
	{
		SDL_Texture* sdl_tex = app->tex->Load("gui/victory_background.png");
		bg->SetAtlas(sdl_tex);		
		app->gui->background.push_back(bg);
	}
	else if (app->missionManager->GetState() == M_DEFEAT)
	{
		SDL_Texture* sdl_tex = app->tex->Load("gui/defeat_background.png");
		bg->SetAtlas(sdl_tex);
		app->gui->background.push_back(bg);
	}
	//app->metrics->CreateChart(600, 400);


}

void S_Score::DrawDebug()
{
	if (active)
	{

	}
}

void S_Score::GoToMenu()
{
	active = false;
	app->score->bg->SetVisible(false);
	app->menu->active = true;
	app->menu->bg->SetVisible(true);
	app->gui->SetActiveScene("menu");
	app->missionManager->CleanUp();
	app->missionManager->Disable();
	app->audio->CleanData();
	app->metrics->CleanUp();
	pugi::xml_node		nullnode;
	app->entityManager->Awake(nullnode);
	app->entityManager->Start();
}

void S_Score::GuiEvent(GUIElement * element, int64_t event)
{
	if (event & MOUSE_LCLICK_UP)
	{
		if (event & START_GAME) 
		{
			app->gui->FindElement(app->gui->guiList, "GoMenu")->SetLClicked(false);
			GoToMenu();
		}
		if (event & CLOSE_APP)
		{
			app->quit = true;
		}
	}
}
