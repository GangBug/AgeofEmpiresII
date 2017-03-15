#ifndef __M_GUI_H__
#define __M_GUI_H__

#include <list>
#include "Module.h"

class GUIElement;

class M_GUI : public Module
{
public:
	M_GUI(bool startEnabled = true);
	virtual ~M_GUI();

	bool Awake(pugi::xml_node&) override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;

	bool UpdateGuiList();
	bool UpdateDebugGuiList();

	GUIElement* FindMouseHover();
	void ManageEvents();
	void BroadcastEventToListeners(GUIElement* element, gui_events event);

	void Draw();
	void DrawEditor();
	void DrawDebug() override;
	
	//Getters & Setters
	SDL_Texture* GetAtlas() const;
	void SetAtlas(SDL_Texture* texture);

	// UI factory
	//GUIImage* createImage();

public:
	std::list<GUIElement*> guiList;
	std::list<GUIElement*> debugGuiList;
	std::list<GUIElement*> editorGuiList;

private:

	SDL_Texture* atlas;
	GUIElement* mouseHover = nullptr;
	GUIElement* focus = nullptr;

};

#endif // !__M_GUI_H__