#include "p2Defs.h"
#include "p2Log.h"
#include "j1VictoryScene.h"
#include "j1App.h"
#include "j1Gui.h"
#include "GUIImage.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1SceneStartMenu.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1Map.h"


j1VictoryScene::j1VictoryScene()
{
	name.assign("victory_menu");
}

j1VictoryScene::~j1VictoryScene()
{

}

bool j1VictoryScene::Awake(pugi::xml_node& node)
{
	LOG("Loading Victory Menu");
	inVictoryMenu = true;
	quit = false;

	return true;
}

bool j1VictoryScene::Start()
{
	bool ret = true;
	if (inVictoryMenu == true)
	{
		AudioLoader();
		ret = UILoader();
	}


	return ret;
}
bool j1VictoryScene::PreUpdate()
{
	return true;
}

bool j1VictoryScene::Update(float dt)
{
	if (inVictoryMenu == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		{
			inVictoryMenu = false;
			CleanUp();
			App->scene->SetInGame();
		}

		if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
			App->audio->PlayFx(menuSelect);
	}

	return true;
}

bool j1VictoryScene::PostUpdate()
{
	bool ret = true;
	if (inVictoryMenu == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || quit)
			ret = false;
	}
	return ret;
}

bool j1VictoryScene::CleanUp()
{
	LOG("Free Start Menu");

	active = false;

	return true;
}

void j1VictoryScene::AudioLoader()
{
	App->audio->PlayTheme(App->audio->menuMusic); 

	menuHover = App->audio->LoadFx("audio/fx/Menu_Select.wav.wav");
	menuSelect = App->audio->LoadFx("audio/fx/Menu_Hover.wav.wav");
}

bool j1VictoryScene::UILoader()
{
	bool ret = true;

	ret = App->gui->LoadLayout("gui/victory.xml");
	//App->entity_manager->CreateUnit(ARCHER, fPoint(300, 310));

	GUIImage* bg = App->gui->CreateImage({ 0,0,(int)(1920 * App->gui->GetScaleX()),(int)(1080 * App->gui->GetScaleY()) }, { 0, 0, 1920, 1080 }, "background");

	if (playerWon == true)
	{
		SDL_Texture* sdl_tex = App->tex->Load("gui/victory_background.png");
		bg->SetAtlas(sdl_tex);
		App->gui->background.push_back(bg);
	}
	if (playerWon == false)
	{
		SDL_Texture* sdl_tex = App->tex->Load("gui/defeat_background.png");
		bg->SetAtlas(sdl_tex);
		App->gui->background.push_back(bg);
	}

	return ret;
}

void j1VictoryScene::SetInMenu()
{
	App->entity_manager->CleanUp();
	App->map->CleanUp();
	App->audio->CleanData();
	inVictoryMenu = true;
	active = true;
	Start();
}

void j1VictoryScene::GuiEvent(GUIElement* element, int64_t event)
{
	if (event & MOUSE_LCLICK_UP)
	{
		if (event & RETURN_TO_MENU)
		{
			inVictoryMenu = false;
			CleanUp();
			App->sceneStart->SetInMenu();
		}
		if (event & CLOSE_APP)
		{
			quit = true;
		}
	}
}
