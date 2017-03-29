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
	bool Awake();

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

	//load the map
	void MapLoader();

	//load The audio
	void AudioLoader();

	void Selector();
	void SetInGame();
private:
	SDL_Texture* debug_tex;
	bool inGame = false;
	
	SDL_Rect select_rect;
	AudioMusic bso_scene;
private:

};

#endif // __j1SCENE_H__