#pragma once
#include "App.h"
#include "Module.h"
#include "M_GUI.h"

class S_InGame :
	public Module
{
public:
	S_InGame(bool startEnabled);
	~S_InGame();

	// Called before render is available
	bool Awake(pugi::xml_node& config) override;
	// Called before the first frame
	bool Start() override;
	//// Called each loop iteration
	//update_status PreUpdate(float dt) override;
	//// Called each loop iteration
	//update_status Update(float dt) override;
	//// Called each loop iteration
	//update_status PostUpdate(float dt) override;
	//// Called before quitting
	//bool CleanUp() override;

	void Draw();

	//bool Load(pugi::xml_node&) override;
	//bool Save(pugi::xml_node&) const override;
	//void GuiEvent(GUIElement* element, int64_t event) override;
	void DrawDebug() override;

};

