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
#include "Entity.h"
#include "Boss.h"
#include "Units.h"
#include "Buildings.h"
#include "Object.h"
#include "j1SceneStartMenu.h"
#include "j1Gui.h"
#include "GUIImage.h"

#define ARCHER_COST 60
#define SAMURAI_COST 70
#define KNIGHT_COST 80

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
	quit = false;


	App->map->Start(); //Improve performance but can crash the game somethimes
	App->audio->Start();

	//Creation_Unit

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;

	if (inGame == true)
	{//checks if the player is ingame			
	
		App->render->camera->SetCenter({ -1500, 1500 }); //Camera Initial Pos

		MapLoader();
		ret = UILoader();
		//UnitFactory();
		AudioLoader();
		spawnArcher = 0;
		spawnKnight = 0;
		spawnSamurai = 0;

		boss = App->entity_manager->CreateBoss(fPoint(900, 750));
		App->entity_manager->CreateUnit(VILE, fPoint(500, 750));

		//Buildings creation
		archery = App->entity_manager->CreateBuilding(ARCHERY, fPoint(-1850, 1570));
		barracks = App->entity_manager->CreateBuilding(BARRACK, fPoint(-1560, 1760));
		stable = App->entity_manager->CreateBuilding(STABLE, fPoint(-1310, 1940));

		App->entity_manager->CreateObject(BANNERA, fPoint(600, 600));
		gold = 20000;
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

	

		// MOVEMENT OF THE CAMERA	 ------------------------------------

		int sCreenDivisionsY = 30;
		int sCreenDivisionsX = 20;

		/*
		Basically the screen is divided into x and e parts so that when rescaling
		the screen the surface is re-scaled, the magic numbers are simply numbers
		of divisions to establish the surface
		*/
		
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT ||
			(App->input->GetMousePosition().y < (App->render->camera->GetHeight() / sCreenDivisionsY) * 2/*Magic Numbers love <3*/) &&
			(App->input->GetMousePosition().y >(App->render->camera->GetHeight() / sCreenDivisionsY)))
		{
			App->render->camera->MoveUp(floor(300.0f * dt));
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT ||
			((App->input->GetMousePosition().y > (App->render->camera->GetHeight() / sCreenDivisionsY) * (sCreenDivisionsY - 1))) &&/*Magic Numbers love <3*/
			App->input->GetMousePosition().y < (App->render->camera->GetHeight() - 3) &&//make define border margin 3
			App->input->GetMousePosition().x >= (App->render->camera->GetWidth() / sCreenDivisionsX)*(sCreenDivisionsX * 0.5 + 1) ||
			((App->input->GetMousePosition().y >(App->render->camera->GetHeight() / sCreenDivisionsY) * (sCreenDivisionsY - 6))) &&/*Magic Numbers love <3*/
			App->input->GetMousePosition().y < (App->render->camera->GetHeight() / sCreenDivisionsY) * (sCreenDivisionsY - 5) &&
			App->input->GetMousePosition().x <= (App->render->camera->GetWidth() / sCreenDivisionsX)*(sCreenDivisionsX * 0.5 - 1))/*Magic Numbers love <3*/
		{
			App->render->camera->MoveDown(floor(300.0f * dt));
		}


		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT ||
			App->input->GetMousePosition().x < App->render->camera->GetWidth() / sCreenDivisionsX &&
			App->input->GetMousePosition().x > 0 &&
			(App->input->GetMousePosition().y <(App->render->camera->GetHeight() / sCreenDivisionsY) * (sCreenDivisionsY - 4.75f/*Magic Numbers love <3*/))
			//(App->input->GetMousePosition().y >(App->render->camera->GetHeight() / sCreenDivisionsY)* (sCreenDivisionsY+1/*Magic Numbers love <3*/)) //TODO move with out the botton up
			)
		{
			App->render->camera->MoveLeft(floor(300.0f * dt));
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT ||
			App->input->GetMousePosition().x >((App->render->camera->GetWidth() / sCreenDivisionsX) * --sCreenDivisionsX) &&
			App->input->GetMousePosition().x <= (App->render->camera->GetWidth() - 3) &&
			(App->input->GetMousePosition().y <(App->render->camera->GetHeight() / sCreenDivisionsY) * (sCreenDivisionsY - 3.5f/*Magic Numbers love <3*/)))
		{
			App->render->camera->MoveRight(floor(300.0f * dt));
		}
		
		// MOVEMENT OF THE CAMERA	------------------------------------
		
		if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
			App->render->camera->Zoom(1);

		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
			App->render->camera->Zoom(-1);

		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			App->map->SwitchDebug();

		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			dynamic_cast<Building*>(archery)->GenerateUnit(1);

		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			if (onMenuInGame)
			{
				App->gui->menuControl(false);
				onMenuInGame = false;
			}
			else 
			{
				App->gui->menuControl(true);
				onMenuInGame = true;
			}
		}

		App->map->Draw();
		DrawDebug();
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
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || quit == true)
		{
			ret = false;
		}
		//Drawing boss life quads
		////black
		//App->render->DrawQuad({ (int)boss->GetX() - (((BOSSHP * 100) / 2000 + 10) / 2)-1, (int)boss->GetY()-1, (BOSSHP * 100) / 2000+2, 12 }, 0, 0, 0, 255);
		////red
		//App->render->DrawQuad({ (int)boss->GetX() - ((BOSSHP * 100) / 2000 + 10) / 2, (int)boss->GetY(), (BOSSHP*100)/2000, 10 }, 255, 0, 0, 255);
		////green
		//App->render->DrawQuad({ (int)boss->GetX() - ((BOSSHP * 100) / 2000 + 10) / 2, (int)boss->GetY(), (boss->GetHP() * 100) / 2000, 10 }, 0, 255, 0, 255);

	}

	Selector();//SELECTION

	return ret; 
}

// Called before quitting
bool j1Scene::CleanUp()
{
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
	if (App->map->Load("MAPTILED.tmx") == true)
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

	GUILabel* currentGold = nullptr;
	//currentGold = App->gui->CreateLabel({ 0,0,1024,768 }, SMALL, "Gold", "");

	//desactive all 


	GUIElement* tmpElement = nullptr;


	//buildings start buttons

	tmpElement = App->gui->FindElement(App->gui->guiList, "ArcherCreatorButton");
	if (tmpElement)
	{
		tmpElement->SetActive(false);
	}
	tmpElement = App->gui->FindElement(App->gui->guiList, "SamuraiCreatorButton");
	tmpElement->SetActive(false);
	tmpElement = App->gui->FindElement(App->gui->guiList, "TarkanCreatorButton");
	tmpElement->SetActive(false);

	tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonArchery");
	tmpElement->SetActive(false);
	tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonStable");
	tmpElement->SetActive(false);
	tmpElement = App->gui->FindElement(App->gui->guiList, "RepairButtonBarrack");
	tmpElement->SetActive(false);

	//window menu

	tmpElement = App->gui->FindElement(App->gui->guiList, "MenuWindow");
	tmpElement->SetActive(false);
	tmpElement = App->gui->FindElement(App->gui->guiList, "MenuButtonInGame_Resume");
	tmpElement->SetActive(false);
	tmpElement = App->gui->FindElement(App->gui->guiList, "MenuButtonInGame_Load");
	tmpElement->SetActive(false);
	tmpElement = App->gui->FindElement(App->gui->guiList, "MenuButtonInGame_Save");
	tmpElement->SetActive(false);
	tmpElement = App->gui->FindElement(App->gui->guiList, "MenuButtonInGame_Options");
	tmpElement->SetActive(false);
	tmpElement = App->gui->FindElement(App->gui->guiList, "MenuButtonInGame_Start");
	tmpElement->SetActive(false);
	tmpElement = App->gui->FindElement(App->gui->guiList, "MenuButtonInGame_exit");
	tmpElement->SetActive(false);

	//window resources
	tmpElement = App->gui->FindElement(App->gui->guiList, "ResourceWindow");
	tmpElement->SetActive(false);


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
	App->audio->CleanUp();
	inGame = true;
	Start();
}

bool j1Scene::IsInGame()
{
	return inGame;
}

int j1Scene::GetGold() const
{
	return gold;
}

int j1Scene::GetBossLife() const
{
	return boss->GetHP();
}

bool j1Scene::IsBossNull() const
{
	if (boss == nullptr)
	{
		return true;
	}
	return false;
}

void j1Scene::DrawDebug()
{
	if (App->debug)
	{
		App->render->DrawQuad({ (int)boss->GetX() - 5, (int)boss->GetY(), 10, 10 }, 255, 0, 0, 255);
	}	
}

void j1Scene::GuiEvent(GUIElement* element, int64_t event)
{
	if (event & MOUSE_LCLICK_UP)
	{
		if (event & ADD_ARCHER && App->entity_manager->archerySelected == true && gold >= ARCHER_COST)
		{
			spawnArcher++;
			gold -= ARCHER_COST;
			UnitFactory();
		}
		if (event & ERASE_ARCHER && App->entity_manager->archerySelected == true && spawnArcher > 0)
		{
			spawnArcher--;
			gold += ARCHER_COST;
		}
		if (event & ADD_SAMURAI && App->entity_manager->barracksSelected == true && gold >= SAMURAI_COST)
		{
			spawnSamurai++;
			gold -= SAMURAI_COST;
			UnitFactory();
		}
		if (event & ERASE_SAMURAI && App->entity_manager->barracksSelected == true && spawnSamurai > 0)
		{
			spawnSamurai--;
			gold += SAMURAI_COST;
		}
		if (event & ADD_KNIGHT && App->entity_manager->stableSelected == true && gold >= KNIGHT_COST)
		{
			spawnKnight++;
			gold -= KNIGHT_COST;
			UnitFactory();
		}
		if (event & ERASE_KNIGHT && App->entity_manager->stableSelected == true && spawnKnight > 0)
		{
			spawnKnight--;
			gold += KNIGHT_COST;
		}
		if (event & OPEN_SCENE_MENU)
		{
			App->gui->menuControl(true);
		}
		if (event & CLOSE_APP)
		{
			quit = true;
		}
		if (event & RETURN_TO_MENU)
		{
			inGame = false;
			CleanUp();
			App->sceneStart->SetInMenu();
		}
		if (event & RESUME_GAME)
		{
			App->gui->menuControl(false);
		}
	}
}

