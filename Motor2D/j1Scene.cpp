#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Scene.h"
#include "j1Animation.h"
#include "j1EntityManager.h"
#include "Units.h"
#include "Buildings.h"
#include "j1SceneStartMenu.h"
#include "j1Gui.h"
#include "GUIImage.h"


j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	bool ret = true;

	inGame = false;
	preGame = true;

	//Creation_Unit

	//Buildings creation
	archery = App->entity_manager->CreateBuilding(ARCHERY, fPoint(610, 210));
	barracks = App->entity_manager->CreateBuilding(BARRACK, fPoint(380, 90));
	stable = App->entity_manager->CreateBuilding(STABLE, fPoint(100, 0));

	gold = 2000;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;

	if (inGame == true)
	{//checks if the player is ingame			
		AudioLoader();
		
		MapLoader();
		ret = UILoader();
		//UnitFactory();

		spawnArcher = 0;
		spawnKnight = 0;
		spawnSamurai = 0;
	}


	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (inGame == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
			App->LoadGame("save_game.xml");

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
			App->SaveGame("save_game.xml");

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT ||
			(App->input->GetMousePosition().y < App->render->camera->GetHeight() / 7))
		{
			App->render->camera->MoveUp(floor(200.0f * dt));
		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT ||
			((App->input->GetMousePosition().y > (App->render->camera->GetHeight() / 6.2f) * 6)))
		{
			App->render->camera->MoveDown(floor(200.0f * dt));
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT ||
			App->input->GetMousePosition().x < App->render->camera->GetWidth() / 18)
		{
			App->render->camera->MoveLeft(floor(200.0f * dt));
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT ||
			App->input->GetMousePosition().x > ((App->render->camera->GetWidth() / 9) * 9))
		{
			App->render->camera->MoveRight(floor(200.0f * dt));
		}
		if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
			App->render->camera->Zoom(1);

		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
			App->render->camera->Zoom(-1);

		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			App->map->SwitchDebug();

		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			dynamic_cast<Building*>(archery)->GenerateUnit(1);

		App->map->Draw();
	}
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (inGame == true) 
	{

		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) 
		{
			inGame = false;
			CleanUp();
			App->sceneStart->SetInMenu();	
		
		}	
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			ret = false;
		}
	}

	Selector();//SELECTION

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	App->entity_manager->CleanUp();
	App->audio->CleanUp();



	LOG("Freeing scene");
	return true;
}

void j1Scene::UnitFactory()
{
	LOG("Creating units");

	dynamic_cast<Building*>(archery)->GenerateUnit(spawnArcher);
	dynamic_cast<Building*>(barracks)->GenerateUnit(spawnSamurai);
	dynamic_cast<Building*>(stable)->GenerateUnit(spawnKnight);

	spawnArcher = 0;
	spawnKnight = 0;
	spawnSamurai = 0;
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMANENEMY, fPoint(-500, 300));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMANENEMY, fPoint(-500, 350));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMANENEMY, fPoint(-500, 400));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMANENEMY, fPoint(-500, 450));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMANENEMY, fPoint(-500, 500));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMANENEMY, fPoint(-500, 550));

	/*
	App->entity_manager->CreateUnit(ARCHER, fPoint(500, 310));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(500, 360));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(600, 350));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(650, 415));
	App->entity_manager->CreateUnit(CAVALRYARCHER, fPoint(340, 415));
	App->entity_manager->CreateUnit(TARKANKNIGHT, fPoint(350, 400));
	*/
}

void j1Scene::MapLoader()
{
	LOG("Loading Map");
	if (App->map->Load("0.1Map.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");
}

void j1Scene::AudioLoader()
{
	LOG("Loading Audio");


	App->audio->PlayMusic("audio/BSO/BSO_ThirdMision.ogg");

	//FX UNITS
	//dead
	fxUnitDead1 = App->audio->LoadFx("audio/FX/Units/Dead_1.wav");
	fxUnitDead2 = App->audio->LoadFx("audio/FX/Units/Dead_2.wav");
	fxUnitDead3 = App->audio->LoadFx("audio/FX/Units/Dead_3.wav");
	fxUnitDead4 = App->audio->LoadFx("audio/FX/Units/Dead_4.wav");
	fxUnitDead5 = App->audio->LoadFx("audio/FX/Units/Dead_5.wav");
	fxUnitDead6 = App->audio->LoadFx("audio/FX/Units/Dead_6.wav");
	//attack
	fxFight1 = App->audio->LoadFx("audio/FX/Units/Attack_6.wav");
	fxFight2 = App->audio->LoadFx("audio/FX/Units/Attack_7.wav");
	fxFight3 = App->audio->LoadFx("audio/FX/Units/Attack_8.wav");
	//create
	fxUnitCreate = App->audio->LoadFx("audio/FX/Units/Creation_Unit.wav");
	//move
	fxCMove = App->audio->LoadFx("audio/FX/Units/Attack_8.wav");
	fxSMove = App->audio->LoadFx("audio/FX/Units/Attack_8.wav");
	fxAMove = App->audio->LoadFx("audio/FX/Units/Attack_8.wav");
	//FX BULDINGS	
	fxArchery = App->audio->LoadFx("audio/FX/Buildings/Click_Archery.wav");
	fxBarrack = App->audio->LoadFx("audio/FX/Buildings/Click_Barrack.wav");
	fxStable = App->audio->LoadFx("audio/FX/Buildings/Click_stable.wav");
	//UI
	menuHover = App->audio->LoadFx("audio/FX/Menu_Hover.wav");
	menuSelect = App->audio->LoadFx("audio/FX/Menu_Select.wav");
	//BSO
	


}

bool j1Scene::UILoader()
{
	bool ret = true;

	ret = App->gui->LoadLayout("gui/gui.xml");


	GUIImage* bg = App->gui->CreateImage({ 0,0,1024,768 }, { 0, 0, 1920, 1080 }, "shader");
	SDL_Texture* sdl_tex = App->tex->Load("gui/UI_Shadder.png");

	return ret;
}

void j1Scene::Selector()
{

	int x, y;
	App->input->GetMousePosition(x, y);

	if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		App->entity_manager->UnselectEverything();

		select_rect.x = x - App->render->camera->GetPosition().x;
		select_rect.y = y - App->render->camera->GetPosition().y;
		select_rect.w = select_rect.x;
		select_rect.h = select_rect.y;
	}

	else if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
	{
		select_rect.w = x - App->render->camera->GetPosition().x;
		select_rect.h = y - App->render->camera->GetPosition().y;
		App->render->DrawQuad({ select_rect.x, select_rect.y, select_rect.w - select_rect.x, select_rect.h - select_rect.y }, 255, 255, 255, 255, false);
	}

	if (App->input->GetMouseButtonDown(1) == KEY_UP)
	{
		App->entity_manager->SelectInQuad(select_rect);
		App->entity_manager->SelectInClick(x,y);
	}
}

void j1Scene::SetInGame()
{	
	inGame = true;
	Start();
}

void j1Scene::GuiEvent(GUIElement* element, int64_t event)
{
	if (event & MOUSE_LCLICK_UP)
	{
		
		if (event & ADD_ARCHER && App->entity_manager->archerySelected == true && gold >= 60)
		{
			spawnArcher++;
			gold -= 60;
		}
		if (event & ERASE_ARCHER && App->entity_manager->archerySelected == true && spawnArcher > 0)
		{
			spawnArcher--;
			gold += 60;
		}
		if (event & ADD_SAMURAI && App->entity_manager->barracksSelected == true && gold >= 70)
		{
			spawnSamurai++;
			gold -= 70;
		}
		if (event & ERASE_SAMURAI && App->entity_manager->barracksSelected == true && spawnSamurai > 0)
		{
			spawnSamurai--;
			gold += 70;
		}
		if (event & ADD_KNIGHT && App->entity_manager->stableSelected == true && gold >= 80)
		{
			spawnKnight++;
			gold -= 80;
		}
		if (event & ERASE_KNIGHT && App->entity_manager->stableSelected == true && spawnKnight > 0)
		{
			spawnKnight--;
			gold += 80;
		}
		if (event & START_GAME)
		{
			App->entity_manager->archerySelected = false;
			App->entity_manager->barracksSelected = false;
			App->entity_manager->stableSelected = false;
			preGame = false;
			UnitFactory();
		}
	}
}

