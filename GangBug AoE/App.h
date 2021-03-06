#ifndef __j1APP_H__
#define __j1APP_H__

#include <iostream>
#include <list>
#include <string>

#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"
#include "Defs.h"

// Modules
class Module;

class M_Window;
class M_Input;
class M_Render;
class M_Textures;
class M_Audio;
class M_EnemyWaves;
class M_FileSystem;
class M_Map;
class M_Pathfinding;
class M_Fonts;
class M_EntityManager;
class M_GUI;
class M_Animation;
class M_Resources;
class M_Collision;
class M_DialogueManager;
class M_FogOfWar;
class S_Menu;
class S_InGame;
class S_Score;
class M_MissionManager;
class M_Minimap;
class M_ParticleSystem;
class M_Metrics;
class M_Video;
class M_KeyBinding;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	update_status Update();

	// Called before quitting
	bool CleanUp();

	//Called if debug mode and iterate all modules
	void DrawDebug();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	float GetDT() const;

	void LoadGame(const char* file);
	void SaveGame(const char* file) const;
	void GetSaveGames(std::list<std::string>& listToFill) const;
	Module* FindModule(std::string name);

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	update_status PreUpdate();

	// Call modules on each loop iteration
	update_status DoUpdate();

	// Call modules after each loop iteration
	update_status PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	M_Window*			win = nullptr;
	M_Input*			input = nullptr;
	M_Render*			render = nullptr;
	M_Textures*			tex = nullptr;
	M_Audio*			audio = nullptr;
	M_EnemyWaves*		enemyWaves = nullptr;
	M_FileSystem*		fs = nullptr;
	M_Map*				map = nullptr;
	M_Pathfinding*		pathfinding = nullptr;
	M_Fonts*			font = nullptr;
	M_EntityManager*	entityManager = nullptr;
	M_GUI*				gui = nullptr;
	M_Animation*		animation = nullptr;
	M_Resources*		resources = nullptr;
	M_Collision*		collision = nullptr;
	M_DialogueManager*	dialogueManager = nullptr;
	//M_FogOfWar*			fogOfWar = nullptr;
	M_MissionManager*	missionManager = nullptr;
	M_Minimap*			minimap = nullptr;
	M_ParticleSystem*	particleSystem = nullptr;
	M_Metrics*			metrics = nullptr;
	M_Video*			video = nullptr;
	M_KeyBinding*		keyBinding = nullptr;

	S_Menu*				menu = nullptr;
	S_InGame*			inGame = nullptr;
	S_Score *			score = nullptr;

	
	bool debug = false;
	bool editor = false;
	bool quit = false;
	bool brofiler = false;
	bool pause = false;
	bool god = false;

	uint32 last_frame_ms;
	uint32 frames_on_last_update;
private:

	std::list<Module*>	modules;
	int					argc;
	char**				args;

	std::string			title;
	std::string			organization;

	mutable bool		wantToSave = false;
	bool				wantToLoad = false;
	std::string			loadGame;
	mutable std::string	saveGame;

	PerfTimer			ptimer;
	uint64				frameCount = 0;
	Timer				startupTime;
	Timer				frameTime;
	Timer				lastSecFrameTime;
	uint32				lastSecFrameCount = 0;
	uint32				prevLastSecFrameCount = 0;
	float				dt = 0.0f;
	int					cappedMs = -1;


	
};

extern App* app;

#endif