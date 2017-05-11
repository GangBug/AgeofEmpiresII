#include <iostream> 
#include <sstream> 

#include "Log.h"

#include "M_Window.h"
#include "M_Input.h"
#include "M_Render.h"
#include "M_Textures.h"
#include "M_Audio.h"
#include "M_EnemyWaves.h"
#include "M_FileSystem.h"
#include "M_Map.h"
#include "M_Pathfinding.h"
#include "M_Fonts.h"
#include "M_EntityManager.h"
#include "M_GUI.h"
#include "M_Animation.h"
#include "M_Resources.h"
#include "M_Collision.h"
#include "M_FogOfWar.h"
#include "M_DialogueManager.h"
#include "M_MissionManager.h"
#include "M_Minimap.h"
#include "M_ParticleSystem.h"

#include "S_InGame.h"
#include "S_Menu.h"

#include "App.h"
#include "Brofiler/Brofiler.h"

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
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
		if (str == "-editor")
		{
			editor = true;
		}
		if (str == "-brofiler")
		{
			brofiler = true;
		}
	}
	
	if (brofiler)
	{
		std::cout << "Press any character to start profiling";
		getchar();
	}

	input = new M_Input();
	win = new M_Window();
	render = new M_Render();
	tex = new M_Textures();
	audio = new M_Audio();
	enemyWaves = new M_EnemyWaves();
	fs = new M_FileSystem();
	map = new M_Map();
	pathfinding = new M_Pathfinding();
	font = new M_Fonts();
	entityManager = new M_EntityManager();
	gui = new M_GUI();
	animation = new M_Animation();
	resources = new M_Resources();
	collision = new M_Collision();
	dialogueManager = new M_DialogueManager();
	minimap = new M_Minimap();
	//fogOfWar = new M_FogOfWar();
	missionManager = new M_MissionManager();
	particleSystem = new M_ParticleSystem();

	menu = new S_Menu(true);
	inGame = new S_InGame(true);

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(fs);
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(enemyWaves);
	AddModule(map);
	AddModule(pathfinding);
	AddModule(font);
	AddModule(entityManager);
	AddModule(resources);
	AddModule(collision);
	AddModule(dialogueManager);
	AddModule(minimap);
	//AddModule(fogOfWar);
	AddModule(animation);
	AddModule(gui);
	AddModule(missionManager);
	AddModule(particleSystem);
	AddModule(menu);
	AddModule(inGame);

	// render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
App::~App()
{
	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); it++) 
	{
		RELEASE(*it);
	}

	modules.clear();
}

void App::AddModule(Module* module)
{
	modules.push_back(module);
}

// Called before render is available
bool App::Awake()
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
		title.assign(app_config.child("title").child_value());
		organization.assign(app_config.child("organization").child_value());

		int cap = app_config.attribute("framerate_cap").as_int(-1);

		if(cap > 0)
		{
			cappedMs = 1000 / cap;
		}
	}

	if(ret == true)
	{
		std::list<Module*>::iterator it = modules.begin();
		while (it!=modules.end() && ret == true) 
		{
			(*it)->Awake(config.child((*it)->name.c_str()));
			it++;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(ptimer);
	bool ret = true;
	std::list<Module*>::iterator item = modules.begin();

	while(item != modules.end() && ret == true)
	{
		ret = (*item)->Start();
		++item;
	}
	startupTime.Start();

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
update_status App::Update()
{
	BROFILER_CATEGORY("App Update", Profiler::Color::Blue);
	update_status ret = UPDATE_CONTINUE;

	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;
	if (input->GetKey(SDL_SCANCODE_PAUSE) == KEY_DOWN)
		pause = !pause;
	if (input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		gui->SetUIEditing(!gui->GetUIEditing()); 

	PrepareUpdate();


	if(ret == UPDATE_CONTINUE)
		ret = PreUpdate();

	if(ret == UPDATE_CONTINUE)
		ret = DoUpdate();

	if(ret == UPDATE_CONTINUE)
		ret = PostUpdate();

	FinishUpdate();

	if(quit)
		ret = UPDATE_STOP;

	return ret;
}

// ---------------------------------------------
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	char* buf = nullptr;
	int size = fs->Load("config.xml", &buf);
	pugi::xml_parse_result result = configFile.load_buffer(buf, size);
	RELEASE(buf);

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	dt = frameTime.ReadSec();
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	if(wantToSave == true)
		SavegameNow();

	if(wantToLoad == true)
		LoadGameNow();

	// Framerate calculations --

	if(lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	float avg_fps = float(frameCount) / startupTime.ReadSec();
	float seconds_since_startup = startupTime.ReadSec();
	last_frame_ms = frameTime.Read();
	frames_on_last_update = prevLastSecFrameCount;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu ",
			  avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frameCount);
	win->SetTitle(title);

	if(cappedMs > 0 && last_frame_ms < cappedMs)
	{
		PerfTimer t;
		SDL_Delay(cappedMs - last_frame_ms);
		LOG("We waited for %d milliseconds and got back in %f", cappedMs - last_frame_ms, t.ReadMs());
	}
}

// Call modules before each loop iteration
update_status App::PreUpdate()
{
	update_status ret = UPDATE_CONTINUE;
	std::list<Module*>::iterator item;
	item = modules.begin();
	Module* pModule = nullptr;

	for(item = modules.begin(); item!=modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		pModule = *item;

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->PreUpdate(dt);
	}

	return ret;
}

// Call modules on each loop iteration
update_status App::DoUpdate()
{
	update_status ret = UPDATE_CONTINUE;
	std::list<Module*>::iterator item;
	item = modules.begin();
	Module* pModule = nullptr;

	for(item = modules.begin(); item!=modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		pModule = (*item);

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
update_status App::PostUpdate()
{
	update_status ret = UPDATE_CONTINUE;
	std::list<Module*>::iterator item;
	Module* pModule = nullptr;

	for(item = modules.begin(); item != modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		pModule = *item;

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->PostUpdate(dt);
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	PERF_START(ptimer);
	bool ret = true;

	for (std::list<Module*>::reverse_iterator item = modules.rbegin(); item != modules.rend(); item++)
	{
		ret = (*item)->CleanUp();
	}

	PERF_PEEK(ptimer);
	return ret;
}

void App::DrawDebug()
{
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		if ((*it)->active)
			(*it)->DrawDebug();
	}
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return nullptr;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.c_str();
}

// ---------------------------------------
float App::GetDT() const
{
	return dt;
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.c_str();
}

// Load / Save
void App::LoadGame(const char* file)
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	wantToLoad = true;
	loadGame.assign(fs->GetSaveDirectory(), file);
}

// ---------------------------------------
void App::SaveGame(const char* file) const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?
	wantToSave = true;
	saveGame.assign(file);
}

// ---------------------------------------
void App::GetSaveGames(std::list<std::string>& listToFill) const
{
	// need to add functionality to file_system module for this to work
}

bool App::LoadGameNow()
{
	bool ret = false;

	char* buffer;
	uint size = fs->Load(loadGame.c_str(), &buffer);

	if(size > 0)
	{
		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		if(result != NULL)
		{
			LOG("Loading new Game State from %s...", loadGame.c_str());

			root = data.child("game_state");

			std::list<Module*>::iterator item = modules.begin();
			ret = true;

			while(item._Ptr != nullptr && ret == true)
			{
				ret = item._Ptr->_Myval->Load(root.child(item._Ptr->_Myval->name.c_str()));
				item._Ptr = item._Ptr->_Next;
			}

			data.reset();
			if(ret == true)
				LOG("...finished loading");
			else
				LOG("...loading process interrupted with error on module %s", (item._Ptr != nullptr) ? item._Ptr->_Myval->name.c_str() : "unknown");
		}
		else
			LOG("Could not parse game state xml file %s. pugi error: %s", loadGame.c_str(), result.description());
	}
	else
		LOG("Could not load game state xml file %s", loadGame.c_str());

	wantToLoad = false;
	return ret;
}

bool App::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", saveGame.c_str());

	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	std::list<Module*>::const_iterator item;
	for(item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		ret = (*item)->Save(root.append_child((*item)->name.c_str()));
		item++;
	}

	if(ret == true)
	{
		std::stringstream stream;
		data.save(stream);

		fs->Save(saveGame.c_str(), stream.str().c_str(), stream.str().length());
		LOG("... finished saving", saveGame.c_str());
	}
	else //This LOG coul have some bugs on the (item != modules.end())
		LOG("Save process halted from an error in module %s", (item != modules.end()) ? (*item)->name.c_str() : "unknown");

	data.reset();
	wantToSave = false;
	return ret;
}
Module* App::FindModule(std::string _name)
{
	Module* ret = nullptr;
	for (std::list<Module*>::iterator it = app->modules.begin(); it != app->modules.end(); it++)
	{
		if (!strcmp((*it)->name.c_str(), _name.c_str()))
		{
			ret = (*it);
		}
	}
	return ret;
}