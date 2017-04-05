#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Timer.h"

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
	bool IsInGame();

	int GetGold() const;
	int GetBossLife() const;
	bool IsBossNull() const;
	void DrawDebug();

	int currentFriendlyUnits;

private:
	SDL_Texture* debug_tex = nullptr;
	bool inGame;
	Entity* archery = nullptr;
	Entity* barracks = nullptr;
	Entity* stable = nullptr;
	SDL_Rect select_rect;

private:
	int gold;
	int spawnArcher;
	int spawnSamurai;
	int spawnKnight;
	Entity* boss = nullptr;

	bool preGame;
	bool onMenuInGame=true;
	bool quit = true;

	j1Timer victoryTimer;

public:// Audio
	   //FX
	   //Units -- 
	   //dead
	uint fxUnitDead1;
	uint fxUnitDead2;
	uint fxUnitDead3;
	uint fxUnitDead4;
	uint fxUnitDead5;
	uint fxUnitDead6;
	//attack	
	uint fxFight1;
	uint fxFight2;
	uint fxFight3;
	uint fxFight4;
	uint fxFight5;
	uint fxFight6;
	uint fxFight7;
	uint fxFight8;
	//move
	uint fxCMove;
	uint fxSMove;
	uint fxAMove;
	//create
	uint fxUnitCreate;
	//Buldings --
	uint fxArchery;
	uint fxBarrack;
	uint fxStable;
	// UI -- 
	uint menuHover;
	uint menuSelect;

};

#endif // __j1SCENE_H__