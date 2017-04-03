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


	return true;
}


bool j1SceneStartMenu::Start()
{
	bool ret = true;
	if (inMenu == true) {
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
	if (inMenu == true) {
		if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
			inMenu = false;
			CleanUp();
			App->scene->SetInGame();
		}
		if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
			App->audio->PlayFx(menuSelect);


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



	return true;
}

void j1SceneStartMenu::AudioLoader()
{
	App->audio->PlayMusic("audio/BSO/BSO_Menu.ogg");

	menuHover = App->audio->LoadFx("audio/fx/Menu_Select.wav.wav");
	menuSelect = App->audio->LoadFx("audio/fx/Menu_Hover.wav.wav");


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
