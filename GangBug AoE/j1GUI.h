#pragma once
#include <list>
#include "j1Module.h"

class GUIElement;

class j1GUI :
	public j1Module
{
public:
	j1GUI();
	virtual ~j1GUI();

	void Init() override;
	bool Awake(pugi::xml_node&) override;
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;

	bool UpdateGuiList();
	bool UpdateDebug_guiList();

	GUIElement* FindMouseHover();
	void ManageEvents(GUIElement* mouseHover, GUIElement* focus = nullptr);
	void BroadcastEventToListeners(GUIElement* element, GuiEvents event);

	void DrawDebug() override;
	//Getters & Setters

public:
	std::list<GUIElement*> guiList;
	std::list<GUIElement*> debug_guiList;

private:
	GUIElement* mouseHover = nullptr;
	GUIElement* focus = nullptr;

};

