#include "p2Defs.h"
#include "p2Log.h"
#include "j1SceneStartMenu.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Scene.h"

j1SceneStartMenu::j1SceneStartMenu()
{
	name.assign("scene");
}

j1SceneStartMenu::~j1SceneStartMenu() {

}

bool j1SceneStartMenu::Awake()
{
	LOG("Loading Scene Start Menu");
	inMenu = true;

	return true;
}


bool j1SceneStartMenu::Start()
{
	bool ret = true;


	if(inMenu==true){

		AudioLoader();

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
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
			inMenu = false;
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
	return true;
}

void j1SceneStartMenu::AudioLoader()
{
	App->audio->Init();

	bso_scene_menu = App->audio->LoadAudioMusic("Sounds/BSO/BSO_Menu.ogg");
	bso_scene_menu.Play(-1);

}

void j1SceneStartMenu::SetInMenu()
{
	inMenu = true;
}


