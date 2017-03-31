#include "p2Defs.h"
#include "p2Log.h"
#include "j1SceneStartMenu.h"
#include "j1App.h"
#include "j1Gui.h"
#include "GUIImage.h"
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
		ret = App->gui->LoadLayout("gui/menu.xml");
		//App->entity_manager->CreateUnit(ARCHER, fPoint(300, 310));

		GUIImage* bg = App->gui->CreateImage({ 0,0,1024,768 }, { 0, 0, 1920, 1080 }, "background");
		SDL_Texture* sdl_tex = App->tex->Load("gui/startmenu_background.png");
		bg->SetAtlas(sdl_tex);
		App->gui->background.push_back(bg);

		bg = App->gui->CreateImage({ 0,0,1024,768 }, { 0, 0, 1920, 1080 }, "shader");
		sdl_tex = App->tex->Load("gui/UI_Shadder.png");
		bg->SetAtlas(sdl_tex);
		App->gui->background.push_back(bg);



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
			App->gui->background.clear(); //TODO: esto es una guarrada, debo mejorarlo, lo he dejado para que la UI de momento no moleste
			App->gui->guiList.clear();
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


