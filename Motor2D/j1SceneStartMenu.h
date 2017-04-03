#ifndef __j1SCENESTARTMENU_H__
#define __j1SCENESTARTMENU_H__

#include "j1Module.h"
#include "j1Audio.h"
class AudioMusic;
class j1SceneStartMenu : public j1Module
{
public:

	j1SceneStartMenu();
	virtual ~j1SceneStartMenu();

	bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();


	//Loaders

	void AudioLoader();
	bool 	UILoader();

	void SetInMenu();

	// Gui Event
	void GuiEvent(GUIElement* element, int64_t event);

private:


	uint menuHover;
	uint menuSelect;
	uint select;
	uint select2;

	bool inMenu;
	bool quit;
};




#endif //__j1SCENESTARTMENU_H__