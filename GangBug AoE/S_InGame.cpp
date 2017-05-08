#include "S_InGame.h"
#include "App.h"
#include "M_Map.h"
#include "M_Pathfinding.h"
#include "M_EntityManager.h"
#include "M_FogOfWar.h"
#include "M_Audio.h"
#include "M_EnemyWaves.h"
#include "Building.h"
#include "M_DialogueManager.h"
#include "S_Menu.h"
#include "M_MisionManager.h"

S_InGame::S_InGame(bool startEnabled) : Module(startEnabled)
{
	name.assign("inGame");
	active = false;
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
	if (active) 
	{

	app->pathfinding->Enable();
	app->entityManager->LoadObjects();
	SetGUI();

	app->dialogueManager->Enable();
	app->dialogueManager->Start();
	
	//audio
	//app->audio->PlayTheme(app->audio->thirdMission);

	app->entityManager->CreateBuilding(BUILD_STABLES, nullptr, -2115, 1850);
	app->entityManager->CreateBuilding(BUILD_ARCHERY, nullptr, -1385, 2170);
	app->entityManager->CreateBuilding(BUILD_BARRACK, nullptr, -2370, 1925);
	app->entityManager->CreateBuilding(BUILD_TOWNCENTER, nullptr, -2800, 2300);

	app->render->camera->SetCenter({ -2500, 2200 });

	if (app->map->Load("Map.tmx") == true)
	{	
		int w, h;
		uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data))
			app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

		//app->fogOfWar->GenerateFogOfWar();
	}
	app->entityManager->PlaceObjects();

	}
	return true;
}

update_status S_InGame::PreUpdate(float dt)
{
	if (active)
	{
		if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
			app->gui->SetActiveScene(name);
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
			GoToMenu();
		if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
			app->gui->SetActiveScene("\0");

		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera->Move(10.0, UP);
		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera->Move(10.0, LEFT);
		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera->Move(10.0, DOWN);
		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera->Move(10.0, RIGHT);


		if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		{
			int x, y;
			app->input->GetMouseMapPosition(x, y);
			app->enemyWaves->SpawnEnemies(5, 0, x, y);
		}
		if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		{
			int x, y;
			app->input->GetMouseMapPosition(x, y);
			enemyDestination = { x, y };
		}
		if (app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
		{
			int x, y;
			app->input->GetMouseMapPosition(x, y);
			app->entityManager->CreateUnit(TARKAN_KNIGHT, nullptr, x, y);
			app->audio->PlayFx(app->entityManager->fxCreateUnit);
		}
		if (app->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
		{
			int x, y;
			app->input->GetMouseMapPosition(x, y);
			app->entityManager->CreateUnit(VILE, nullptr, x, y);
			app->audio->PlayFx(app->entityManager->fxCreateUnit);
		}
		if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
		{
			return UPDATE_STOP;
		}

		//What is this: A tool to debug pathfinding
		if (app->debug)
		{
			if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
			{
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_state::KEY_UP)
				{
					iPoint point;
					app->input->GetMouseMapPosition(point.x, point.y);
					origin.x = app->map->WorldToMap(point.x, point.y).x;
					origin.y = app->map->WorldToMap(point.x, point.y).y;
				}
				if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_state::KEY_UP)
				{
					iPoint point;
					app->input->GetMouseMapPosition(point.x, point.y);
					destiny.x = app->map->WorldToMap(point.x, point.y).x;
					destiny.y = app->map->WorldToMap(point.x, point.y).y;

					app->pathfinding->CalculatePath(origin, destiny, path);
					SDL_Log("-------------\nOrigin: (%d, %d)\nDestiniy: (%d,%d)\n-------------",
							origin.x,
							origin.y,
							destiny.x,
							destiny.y);
				}
			}
		}
	}	
	return UPDATE_CONTINUE;
}

update_status S_InGame::Update(float dt)
{
	if (active)
	{

	}
	return UPDATE_CONTINUE;
}

update_status S_InGame::PostUpdate(float dt)
{
	if (active)
	{

	}
	return UPDATE_CONTINUE;
}

void S_InGame::Draw()
{
	if (active)
	{
		
	}
}

void S_InGame::DrawDebug()
{
	if (active)
	{
		for (auto node : path)
		{
			iPoint point;
			point = app->map->MapToWorld(node.x, node.y);
			app->render->DrawQuad({ point.x - 5, point.y - 5, 10, 10 }, 255, 255, 0, 255);
		}

	}
}

void S_InGame::GuiEvent(GUIElement* element, int64_t event)
{
	if (event & MOUSE_LCLICK_UP)
	{
		if (event & OPEN_MENU)
		{
			OpenMenu(!menuOpen);
			menuOpen = !menuOpen;
			app->audio->PlayFx(app->gui->fxSelect);
		}
		if (event & OPEN_RESOURCE_MANAGER)
		{
			OpenResources(!resourceManager);
			resourceManager = !resourceManager;
			app->audio->PlayFx(app->gui->fxSelect);
		}
	}

}

void S_InGame::GoToMenu()
{
	active = false;
	app->menu->active = true;
	app->dialogueManager->CleanUp();
	app->dialogueManager->Disable();
	app->gui->SetActiveScene("menu");
	app->entityManager->CleanUp();
	app->audio->CleanData();
	app->misionManager->CleanUp();
	pugi::xml_node		nullnode;

	app->entityManager->Awake(nullnode);
	app->entityManager->Start();
}

void S_InGame::OpenMenu(bool visible)
{
	app->gui->FindElement(app->gui->guiList, "MenuWindow")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Resume")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Load")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Save")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Options")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Start")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Exit")->SetVisible(visible);

	app->gui->FindElement(app->gui->guiList, "MenuWindow")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Resume")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Load")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Save")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Options")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Start")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Exit")->SetInteractive(visible);


}

void S_InGame::OpenResources(bool visible)
{
	app->gui->FindElement(app->gui->guiList, "ResourceWindow")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_TotalVillager")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_TotalVillager_n")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_UnployedVillager")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_UnployedVillager_n")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners_n")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners_plus")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners_minus")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers_n")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers_plus")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers_minus")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks_n")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks_plus")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks_minus")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen_n")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen_plus")->SetVisible(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen_minus")->SetVisible(visible);


	app->gui->FindElement(app->gui->guiList, "ResourceWindow")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_TotalVillager")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_TotalVillager_n")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_UnployedVillager")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_UnployedVillager_n")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners_n")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners_plus")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners_minus")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers_n")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers_plus")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers_minus")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks_n")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks_plus")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks_minus")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen_n")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen_plus")->SetInteractive(visible);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen_minus")->SetInteractive(visible);




}


void S_InGame::SetGUI()
{
	app->gui->FindElement(app->gui->guiList, "MenuWindow")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Resume")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Load")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Save")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Options")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Start")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Exit")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "RepairButtonBarrack")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "RepairButtonStable")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "RepairButtonArchery")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "SamuraiCreatorButton")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "TarkanCreatorButton")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "ArcherCreatorButton")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "VillagerCreatorButton")->SetVisible(false);

	//----resource window
	
	app->gui->FindElement(app->gui->guiList, "ResourceWindow")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_TotalVillager")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_TotalVillager_n")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_UnployedVillager")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_UnployedVillager_n")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners_n")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners_plus")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners_minus")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers_n")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers_plus")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers_minus")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks_n")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks_plus")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks_minus")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen_n")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen_plus")->SetVisible(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen_minus")->SetVisible(false);

	//----
	app->gui->FindElement(app->gui->guiList, "MenuWindow")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Resume")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Load")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Save")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Options")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Start")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "MenuButtonInGame_Exit")->SetInteractive(false);
	
	app->gui->FindElement(app->gui->guiList, "RepairButtonBarrack")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "RepairButtonStable")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "RepairButtonArchery")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "SamuraiCreatorButton")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "TarkanCreatorButton")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "ArcherCreatorButton")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "VillagerCreatorButton")->SetInteractive(false);

	//----resource window
	app->gui->FindElement(app->gui->guiList, "ResourceWindow")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_TotalVillager")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_TotalVillager_n")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_UnployedVillager")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_UnployedVillager_n")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners_n")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners_plus")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Miners_minus")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers_n")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers_plus")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Farmers_minus")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks_n")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks_plus")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Lumberjacks_minus")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen_n")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen_plus")->SetInteractive(false);
	app->gui->FindElement(app->gui->guiList, "label_ResourceWindow_Repairmen_minus")->SetInteractive(false);

	// RepairButton - PlusButton - MinusButton
	
	//----
}
