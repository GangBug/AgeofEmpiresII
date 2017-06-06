#pragma once
#include "Module.h"
struct sKey
{
	SDL_Scancode scancode;
};

class M_KeyBinding :
	public Module
{
public:
	M_KeyBinding(bool enabled = true);
	~M_KeyBinding();

	bool Awake(pugi::xml_node&) override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;
	bool Load(pugi::xml_node&) override;
	bool Save(pugi::xml_node&) const override;
	void GuiEvent(GUIElement* element, int64_t event) override;
	void DrawDebug() override;

private:
	static SDL_Scancode charToScancode(const char* c);
public:
	sKey cameraUp;
	sKey cameraDown;
	sKey cameraLeft;
	sKey cameraRight;

	sKey selectAllUnitsOfTypeSelected;

	sKey selectArchery;
	sKey selectStable;
	sKey selectBarracks;
	sKey selectCastle;

};

