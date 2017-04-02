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
	name.assign("menu");
}

j1SceneStartMenu::~j1SceneStartMenu() {

}

bool j1SceneStartMenu::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene Start Menu");
	inMenu = true;
	quit = false;

	AudioLoader();

	return true;
}


bool j1SceneStartMenu::Start()
{
	bool ret = true;	
		menuSelect.Play();
	if(inMenu==true){

		App->audio->Init();
		bso_scene_menu.Play(-1);
		AudioLoader();
		ret = UILoader();
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
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || quit)
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
	bso_scene_menu = App->audio->LoadAudioMusic("Sounds/BSO/BSO_Menu.ogg");
	menuHover = App->audio->LoadAudioFX("Sounds/FX/UI/Menu_Hover.wav");
	menuSelect = App->audio->LoadAudioFX("Sounds/FX/UI/Menu_Select.wav");
}

bool j1SceneStartMenu::UILoader()
{
	bool ret = true;

	ret = App->gui->LoadLayout("gui/menu.xml");
	//App->entity_manager->CreateUnit(ARCHER, fPoint(300, 310));

	GUIImage* bg = App->gui->CreateImage({ 0,0,1024,575 }, { 0, 0, 1920, 1080 }, "background");
	SDL_Texture* sdl_tex = App->tex->Load("gui/startmenu_background.png");
	bg->SetAtlas(sdl_tex);
	App->gui->background.push_back(bg);


	return ret;

}

void j1SceneStartMenu::SetInMenu()
{
	inMenu = true;
	Start();
}

void j1SceneStartMenu::GuiEvent(GUIElement* element, int64_t event)
{
	if (event & MOUSE_LCLICK_UP)
	{
	

		if (event & NEW_GAME)
		{
		
			inMenu = false;
			CleanUp();
			App->scene->SetInGame();
		}
		if (event & CLOSE_APP)
		{
		
			quit = true;
		}
	}
}
