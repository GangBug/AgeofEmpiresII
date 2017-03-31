#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1FileSystem.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1Animation.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Collision.h"
#include "j1SceneStartMenu.h"
#include "j1Gui.h"
#include "j1Fonts.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	//TODO this is a prototype of how to get command line parameters and do something with them, i'll enhance it when i get some more time.
	for (int i = 0; i < argc; i++)
	{
		std::string str(args[i]);
		//std::cout << "\narg " << i << ": " << str << "\n";
		if (str == "-debug")
		{
			debug = true;
		}
	}


	input = new j1Input();
	win = new j1Window();
	audio = new M_Audio();
	render = new j1Render();
	tex = new j1Textures();
	fs = new j1FileSystem();
	pathfinding = new j1PathFinding();
	entity_manager = new j1EntityManager();
	anim = new j1Animation();
	map = new j1Map();
	sceneStart = new j1SceneStartMenu();
	scene = new j1Scene();
	gui = new j1GUI();
	font = new j1Fonts();

	collision = new j1Collision();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(fs);
	AddModule(input);	
	AddModule(win);
	AddModule(audio);
	AddModule(tex);
	AddModule(gui); //ADD WHEN READY! TOOK OUT FOR 0.1.5
	AddModule(font);

	AddModule(pathfinding);
	AddModule(map);
	AddModule(anim);
	AddModule(collision);

	// scene last
	AddModule(sceneStart);
	AddModule(scene);
	AddModule(entity_manager);

	
	// render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
j1App::~j1App()
{
	// release modules
	std::list<j1Module*>::reverse_iterator item = modules.rbegin();

	while (item != modules.rend())
	{
		RELEASE(*item);
		item++;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool j1App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = true;

	config = LoadConfig(config_file);

	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.assign(app_config.child("title").child_value());
		organization.assign(app_config.child("organization").child_value());

		int cap = app_config.attribute("framerate_cap").as_int(-1);

		if (cap > 0)
		{
			capped_ms = 1000 / cap;
		}
	}

	if (ret == true)
	{
		std::list<j1Module*>::iterator item = modules.begin();

		while (item != modules.end() && ret == true)
		{
			ret = (*item)->Awake(config.child((*item)->name.c_str()));
			item++;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	PERF_START(ptimer);
	bool ret = true;
	std::list<j1Module*>::iterator item = modules.begin();

	while (item != modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}
	startup_time.Start();

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;
	if (input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		gui->SetUIEditing(!gui->GetUIEditing());


	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = App->fs->Load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();
	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	if (want_to_save == true)
		SavegameNow();

	if (want_to_load == true)
		LoadGameNow();

	// Framerate calculations --

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	last_frame_ms = frame_time.Read();
	frames_on_last_update = prev_last_sec_frame_count;


	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera->GetPosition().x, y - App->render->camera->GetPosition().y);
	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);
	mouse.x -= App->render->camera->GetPosition().x;
	mouse.y -= App->render->camera->GetPosition().y;
	static char title[256];
	sprintf_s(title, 256, "Mouse: %d, %d    Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d,         Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu ", 
		mouse.x, mouse.y,
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.size(),
		map_coordinates.x, map_coordinates.y,
		avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);
	App->win->SetTitle(title);

	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		j1PerfTimer t;
		SDL_Delay(capped_ms - last_frame_ms);
		LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
	}
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	std::list<j1Module*>::iterator item;
	j1Module* pModule = NULL;

	for (item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = (*item);

		if (pModule->active == false) {
			continue;
		}

		ret = (*item)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	std::list<j1Module*>::iterator item;
	j1Module* pModule = NULL;

	for (item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = (*item);

		if (pModule->active == false) {
			continue;
		}

		ret = (*item)->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	std::list<j1Module*>::iterator item;
	j1Module* pModule = NULL;

	for (item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = (*item);

		if (pModule->active == false) {
			continue;
		}

		ret = (*item)->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	PERF_START(ptimer);
	bool ret = true;
	std::list<j1Module*>::reverse_iterator item = modules.rbegin();

	while (item != modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}

	PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.c_str();
}

// ---------------------------------------
float j1App::GetDT() const
{
	return dt;
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.c_str();
}

// Load / Save
void j1App::LoadGame(const char* file)
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
	char dir[512];
	sprintf_s(dir, "%s%s", fs->GetSaveDirectory(), file);
	load_game.assign(dir);
}

// ---------------------------------------
void j1App::SaveGame(const char* file) const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
	save_game.assign(file);
}

// ---------------------------------------
void j1App::GetSaveGames(std::list<std::string>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool j1App::LoadGameNow()
{
	bool ret = false;

	char* buffer;
	uint size = fs->Load(load_game.c_str(), &buffer);

	if (size > 0)
	{
		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		if (result != NULL)
		{
			LOG("Loading new Game State from %s...", load_game.c_str());

			root = data.child("game_state");

			std::list<j1Module*>::iterator item = modules.begin();
			ret = true;

			while (item != modules.end() && ret == true)
			{
				ret = (*item)->Load(root.child((*item)->name.c_str()));
				item++;
			}

			data.reset();
			if (ret == true)
				LOG("...finished loading");
			else
				LOG("...loading process interrupted with error on module %s", (*item != NULL) ? (*item)->name.c_str() : "unknown");
		}
		else
			LOG("Could not parse game state xml file %s. pugi error: %s", load_game.c_str(), result.description());
	}
	else
		LOG("Could not load game state xml file %s", load_game.c_str());

	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.c_str());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	std::list<j1Module*>::const_iterator item = modules.begin();

	while (item != modules.end() && ret == true)
	{
		ret = (*item)->Save(root.append_child((*item)->name.c_str()));
		item++;
	}

	if (ret == true)
	{
		std::stringstream stream;
		data.save(stream);

		// we are done, so write data to disk
		fs->Save(save_game.c_str(), stream.str().c_str(), stream.str().length());
		LOG("... finished saving", save_game.c_str());
	}
	else
		LOG("Save process halted from an error in module %s", (*item != NULL) ? (*item)->name.c_str() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}

j1Module * j1App::FindModule(std::string _name)
{
	j1Module* ret = nullptr;
	for (std::list<j1Module*>::iterator it = App->modules.begin(); it != App->modules.end(); it++)
	{
		if (!strcmp((*it)->name.c_str(), _name.c_str()))
		{
			ret = (*it);
		}
	}
	return ret;
}
