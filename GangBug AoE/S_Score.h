#pragma once
#include "App.h"
#include "Module.h"
#include "GUIImage.h"

class S_Score :
	public Module
{
public:
	S_Score(bool startEnabled);
	~S_Score();

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
	//bool Save(pugi::xml_node&) const override;

	void LoadUI();
	void DrawDebug() override;

	void GoToMenu();

	void GuiEvent(GUIElement* element, int64_t event);

	GUIImage* bg = nullptr;
};

