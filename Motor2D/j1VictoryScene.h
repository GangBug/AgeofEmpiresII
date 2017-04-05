#ifndef __j1VICTORYMENU_H__
#define __j1VICTORYMENU_H__

#include "j1Module.h"
#include "j1Audio.h"

class AudioMusic;
class j1VictoryScene : public j1Module
{
public:
	j1VictoryScene();
	virtual ~j1VictoryScene();

	bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	//Loaders
	void AudioLoader();
	bool UILoader();

	void SetInMenu();

	// Gui Event
	void GuiEvent(GUIElement* element, int64_t event);

private:
	uint menuHover;
	uint menuSelect;
	uint select;
	uint select2;

	bool inVictoryMenu;
	bool quit;
};




#endif //__j1VICTORYMENU_H__