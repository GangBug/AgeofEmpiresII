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
	void AudioLoader();


	void SetInMenu();
private:
	AudioMusic bso_scene_menu;
	bool inMenu;
};




#endif //__j1SCENESTARTMENU_H__