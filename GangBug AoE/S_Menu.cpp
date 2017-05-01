#include "S_Menu.h"
#include "M_Audio.h"


S_Menu::S_Menu(bool startEnabled) : Module(startEnabled)
{
	name.assign("menu");
}


S_Menu::~S_Menu()
{
}

bool S_Menu::Awake(pugi::xml_node & config)
{
	return true;
}

bool S_Menu::Start()
{
	bool ret = true;
	//audio
	//app->audio->PlayTheme(app->audio->thirdMission);


	return ret;
}

update_status S_Menu::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;



	return ret;
}

update_status S_Menu::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;



	return ret;
}

update_status S_Menu::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;



	return ret;
}

bool S_Menu::CleanUp()
{
	bool ret = true;



	return ret;
}

void S_Menu::DrawDebug()
{
}
