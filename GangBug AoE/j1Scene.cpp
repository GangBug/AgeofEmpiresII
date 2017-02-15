#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "Selector.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	//EXERCISE 1 ---
	Element* background = App->gui->create(default, { 0, 1600, 1024, 576 }, { 0,0 });

	Element* window = App->gui->create(default, { 0, 512, 483, 512 }, { 10,40 });

	p2SString* string1 = new p2SString("Select Your Character");
	Element* text = App->gui->create(Text, { 0,0,0,0 }, { 128,50 }, string1);

	Element* button1 = App->gui->create(button, { 645,165,229,69 }, { 128, 360 });

	p2SString* string2 = new p2SString("Continue");
	Element* buttontext = App->gui->create(Text, { 0,0,0,0 }, { 55,18 }, string2);

	window->AddSon(text);
	button1->AddSon(buttontext);
	window->AddSon(button1);

	//---

	//EXERCISE 2 ---
	Element* thumbnail = App->gui->create(default, { 0, 290, 300, 169 }, { 90, 170 });

	Element* lens = App->gui->create(guiSelector, { 900, 314, 68, 54 }, { 1,1 });

	window->AddSon(thumbnail);
	thumbnail->AddSon(lens);

	//---

	Element* character = App->gui->create(default, { 0, 1024, 1025, 574 }, { 0,0 });

	lens->AddSon(character);

	delete string1;
	delete string2;


	

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	// -------
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");	


	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}


void j1Scene::GuiEvent(Element* element, Event state) {
	switch (element->mytype) {
	case button:
		if (state == away) {
			element->setRect_xy(645, 165);
		}
		if (state == idle) {
			element->setRect_xy(0, 111);
		}
		if (state == leftclick) {
			element->setRect_xy(410, 169);
		}
		break;
	case guiSelector:
		if (state == leftclick) {
			((Selector*)element)->Move();
		}
		//EXERCISE 3---
		if (state == away) {
			((Selector*)element)->Stick();
		}
		break;
	}
}