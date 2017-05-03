#pragma once
#include "App.h"
#include "Module.h"
class S_Menu :
	public Module
{
public:
	S_Menu(bool startEnabled);
	~S_Menu();

	//// Called before render is available
	bool Awake(pugi::xml_node& config) override;
	//// Called before the first frame
	bool Start() override;
	//// Called each loop iteration
	update_status PreUpdate(float dt) override;
	//// Called each loop iteration
	update_status Update(float dt) override;
	//// Called each loop iteration
	update_status PostUpdate(float dt) override;
	//// Called before quitting
	bool CleanUp() override;
	//bool Load(pugi::xml_node&) override;
//	bool Save(pugi::xml_node&) const override;

//	void GuiEvent(GUIElement* element, int64_t event) override;
	void DrawDebug() override;

	void GoToIngame();

};

