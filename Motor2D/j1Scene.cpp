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

	bso_scene = App->audio->LoadAudioMusic("Sounds/BSO/BSO_ThirdMision.ogg");

	spawnArcher = 0;
	spawnKnight = 0;
	spawnSamurai = 0;

	archery = App->entity_manager->CreateBuilding(ARCHERY, fPoint(610, 210));
	barracks = App->entity_manager->CreateBuilding(BARRACK, fPoint(-610, 210));
	stable = App->entity_manager->CreateBuilding(STABLE, fPoint(1000, 460));

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;

	if (inGame == true)
	{//checks if the player is ingame			
	//	AudioLoader();
	
		MapLoader();
		ret = UILoader();
		//UnitFactory();
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
		if (preGame == false) 
		{
			if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
				App->LoadGame("save_game.xml");

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
				App->SaveGame("save_game.xml");

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
				App->render->camera->MoveUp(floor(200.0f * dt));

				for (std::list<GUIElement*>::iterator it = App->gui->guiList.begin(); it != App->gui->guiList.end(); it++)
					(*it)->MoveNorth();

			}


			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
				App->render->camera->MoveDown(floor(200.0f * dt));

				for (std::list<GUIElement*>::iterator it = App->gui->guiList.begin(); it != App->gui->guiList.end(); it++)
					(*it)->MoveSouth();
			}


			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
				App->render->camera->MoveLeft(floor(200.0f * dt));

				for (std::list<GUIElement*>::iterator it = App->gui->guiList.begin(); it != App->gui->guiList.end(); it++)
					(*it)->MoveWest();
			}


			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				App->render->camera->MoveRight(floor(200.0f * dt));

				for (std::list<GUIElement*>::iterator it = App->gui->guiList.begin(); it != App->gui->guiList.end(); it++)
					(*it)->MoveEast();
			}


			if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
				App->render->camera->Zoom(1);

			if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
				App->render->camera->Zoom(-1);

			if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
				App->map->SwitchDebug();

			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
				dynamic_cast<Building*>(archery)->GenerateUnit(1);
			Selector();//SELECTION
		}
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

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	App->entity_manager->CleanUp();
	bso_scene.Stop();
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
	/*App->entity_manager->CreateUnit(TWOHANDEDSWORDMANENEMY, fPoint(-500, 300));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMANENEMY, fPoint(-500, 350));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMANENEMY, fPoint(-500, 400));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMANENEMY, fPoint(-500, 450));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMANENEMY, fPoint(-500, 500));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMANENEMY, fPoint(-500, 550));

	App->entity_manager->CreateUnit(ARCHER, fPoint(500, 310));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(500, 360));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(600, 350));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(650, 415));
	App->entity_manager->CreateUnit(CAVALRYARCHER, fPoint(340, 415));
	App->entity_manager->CreateUnit(TARKANKNIGHT, fPoint(350, 400));

	
	archery = App->entity_manager->CreateBuilding(ARCHERY, fPoint(610, 210));*/

		App->entity_manager->CreateUnit(SAMURAI, fPoint(350, 400));
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

	bso_scene.Play();
}

bool j1Scene::UILoader()
{
	bool ret = true;

	ret = App->gui->LoadLayout("gui/gui.xml");



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
		if (event & ADD_ARCHER)
		{
			spawnArcher++;
		}
		if (event & ERASE_ARCHER)
		{
			spawnArcher--;
		}
		if (event & ADD_SAMURAI)
		{
			spawnSamurai++;
		}
		if (event & ERASE_SAMURAI)
		{
			spawnSamurai--;
		}
		if (event & ADD_KNIGHT)
		{
			spawnKnight++;
		}
		if (event & ERASE_KNIGHT)
		{
			spawnKnight--;
		}
		if (event & START_GAME)
		{
			preGame = false;
			UnitFactory();
		}
	}
}

