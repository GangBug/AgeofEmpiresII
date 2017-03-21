#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "Module.h"

struct SDL_Texture;
class GuiImage;
class GuiText;

class j1Scene : public Module
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
	//bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//void GuiEvent(GUIElement* element, gui_events state);

private:
	SDL_Texture* debug_tex;
	//GuiImage* banner;
	//GuiText* text;
};

#endif // __j1SCENE_H__