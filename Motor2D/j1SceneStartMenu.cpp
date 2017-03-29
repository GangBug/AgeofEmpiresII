#include "p2Defs.h"
#include "p2Log.h"
#include "j1SceneStartMenu.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
j1SceneStartMenu::j1SceneStartMenu()
{
	name.assign("scene");
}

j1SceneStartMenu::~j1SceneStartMenu() {

}

bool j1SceneStartMenu::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene Start Menu");
	inMenu = true;

	bso_scene_menu = App->audio->LoadAudioMusic("Sounds/BSO/BSO_Menu.ogg");

	return true;
}


bool j1SceneStartMenu::Start()
{
	bool ret = true;
	App->audio->Init();

	if(inMenu==true){
		AudioLoader();
		//App->entity_manager->CreateUnit(ARCHER, fPoint(300, 310));
	}


	return ret;
}
bool j1SceneStartMenu::PreUpdate()
{
	return true;
}

bool j1SceneStartMenu::Update(float dt)
{
	if(inMenu == true){
		if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
			inMenu = false;
			CleanUp();
			App->scene->SetInGame();
			
		}

	}

	return true;
}

bool j1SceneStartMenu::PostUpdate()
{
	bool ret = true;
	if (inMenu == true) {
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			ret = false;
	}
	return ret;
}

bool j1SceneStartMenu::CleanUp()
{
	LOG("Free Start Menu");

	bso_scene_menu.Stop();
	App->audio->CleanUp();

	return true;
}

void j1SceneStartMenu::AudioLoader()
{
	
	bso_scene_menu.Play(-1);

}

void j1SceneStartMenu::SetInMenu()
{
	inMenu = true;
	Start();
}


