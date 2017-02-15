#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1FileSystem.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1Fonts.h"
#include "j1App.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	fs = new j1FileSystem();
	map = new j1Map();
	pathfinding = new j1PathFinding();
	font = new j1Fonts();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(fs);
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(pathfinding);
	AddModule(font);

	// render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
j1App::~j1App()
{
	std::list<j1Module*>::iterator it = modules.end();
	it._Ptr = it._Ptr->_Prev;
	for (; it != modules.begin(); --it) {
		RELEASE(it._Ptr->_Myval);
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

	bool ret = false;
		
	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());

		int cap = app_config.attribute("framerate_cap").as_int(-1);

		if(cap > 0)
		{
			capped_ms = 1000 / cap;
		}
	}

	if(ret == true)
	{

		std::list<j1Module*>::iterator it = modules.begin();
		while (it!=modules.end() && ret == true) {
			it._Ptr->_Myval->Awake(config.child(it._Ptr->_Myval->name.GetString()));
			it._Ptr = it._Ptr->_Next;
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
	std::list<j1Module*>::iterator item;
	item = modules.begin();

	while(item != modules.end() && ret == true)
	{
		ret = item._Ptr->_Myval->Start();
		item._Ptr = item._Ptr->_Next;
	}
	startup_time.Start();

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
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

	if(result == NULL)
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
	if(want_to_save == true)
		SavegameNow();

	if(want_to_load == true)
		LoadGameNow();

	// Framerate calculations --

	if(last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu ",
			  avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);
	App->win->SetTitle(title);

	if(capped_ms > 0 && last_frame_ms < capped_ms)
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
	item = modules.begin();
	j1Module* pModule = NULL;

	for(item = modules.begin(); item!=modules.end() && ret == true; item._Ptr = item._Ptr->_Next)
	{
		pModule = item._Ptr->_Myval;

		if(pModule->active == false) {
			continue;
		}

		ret = item._Ptr->_Myval->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	std::list<j1Module*>::iterator item;
	item = modules.begin();
	j1Module* pModule = NULL;

	for(item = modules.begin(); item!=modules.end() && ret == true; item._Ptr = item._Ptr->_Next)
	{
		pModule = item._Ptr->_Myval;

		if(pModule->active == false) {
			continue;
		}

		ret = item._Ptr->_Myval->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	std::list<j1Module*>::iterator item;
	j1Module* pModule = NULL;

	for(item = modules.begin(); item != modules.end() && ret == true; item._Ptr = item._Ptr->_Next)
	{
		pModule = item._Ptr->_Myval;

		if(pModule->active == false) {
			continue;
		}

		ret = item._Ptr->_Myval->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	PERF_START(ptimer);
	bool ret = true;
	std::list<j1Module*>::iterator item;
	item = modules.end();
	item._Ptr = item._Ptr->_Prev;

	while(item != modules.begin() && ret == true)
	{
		ret = item._Ptr->_Myval->CleanUp();
		item._Ptr = item._Ptr->_Prev;
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
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
float j1App::GetDT() const
{
	return dt;
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void j1App::LoadGame(const char* file)
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
	load_game.create("%s%s", fs->GetSaveDirectory(), file);
}

// ---------------------------------------
void j1App::SaveGame(const char* file) const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
	save_game.create(file);
}

// ---------------------------------------
void j1App::GetSaveGames(std::list<p2SString>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool j1App::LoadGameNow()
{
	bool ret = false;

	char* buffer;
	uint size = fs->Load(load_game.GetString(), &buffer);

	if(size > 0)
	{
		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		if(result != NULL)
		{
			LOG("Loading new Game State from %s...", load_game.GetString());

			root = data.child("game_state");

			std::list<j1Module*>::iterator item = modules.begin();
			ret = true;

			while(item._Ptr != NULL && ret == true)
			{
				ret = item._Ptr->_Myval->Load(root.child(item._Ptr->_Myval->name.GetString()));
				item._Ptr = item._Ptr->_Next;
			}

			data.reset();
			if(ret == true)
				LOG("...finished loading");
			else
				LOG("...loading process interrupted with error on module %s", (item._Ptr != NULL) ? item._Ptr->_Myval->name.GetString() : "unknown");
		}
		else
			LOG("Could not parse game state xml file %s. pugi error: %s", load_game.GetString(), result.description());
	}
	else
		LOG("Could not load game state xml file %s", load_game.GetString());

	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.GetString());

	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	std::list<j1Module*>::const_iterator item;
	item = modules.begin();

	while(item._Ptr != NULL && ret == true)
	{
		ret = item._Ptr->_Myval->Save(root.append_child(item._Ptr->_Myval->name.GetString()));
		item._Ptr = item._Ptr->_Next;
	}

	if(ret == true)
	{
		std::stringstream stream;
		data.save(stream);

		fs->Save(save_game.GetString(), stream.str().c_str(), stream.str().length());
		LOG("... finished saving", save_game.GetString());
	}
	else
		LOG("Save process halted from an error in module %s", (item._Ptr != NULL) ? item._Ptr->_Myval->name.GetString() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}