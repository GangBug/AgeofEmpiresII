#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class UILabel;
class UIImage;
class UICheckbutton;
class UISelectOption;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//create initial units
	void UnitFactory();

	/// Loadders----------

	//load the map
	void MapLoader();

	//load The audio
	void AudioLoader();

	//load UI
	bool UILoader();

	// Gui Event
	void GuiEvent(GUIElement* element, int64_t event);

	void Selector();
	void SetInGame();
private:
	SDL_Texture* debug_tex;
	bool inGame;
	Entity* archery;
	Entity* barracks;
	Entity* stable;
	SDL_Rect select_rect;



private:
	int spawnArcher;
	int spawnSamurai;
	int spawnKnight;

	bool preGame;

public:// Audio

	//FX
	//Units -- 
	//dead
	AudioFX fxUnitDead1;
	AudioFX fxUnitDead2;
	AudioFX fxUnitDead3;
	AudioFX fxUnitDead4;
	AudioFX fxUnitDead5;
	AudioFX fxUnitDead6;
	//attack	
	AudioFX fxFight1;
	AudioFX fxFight2;
	AudioFX fxFight3;
	AudioFX fxFight4;
	AudioFX fxFight5;
	AudioFX fxFight6;
	AudioFX fxFight7;
	AudioFX fxFight8;
	//move
	AudioFX fxCMove;
	AudioFX fxSMove;
	AudioFX fxAMove;
	//create
	AudioFX fxUnitCreate;
	//Buldings --
	AudioFX fxArchery;
	AudioFX fxBarrack;
	AudioFX fxStable;
	// UI -- 
	AudioFX menuHover;
	AudioFX menuSelect;
	//BSO
	AudioMusic bso_scene;


};

#endif // __j1SCENE_H__