#ifndef __M_GUI_H__
#define __M_GUI_H__

#include <list>
#include "Module.h"
#include "GB_Rectangle.h"

class GUIElement;
class GUIButton;
class GUILabel;
class GUIImage;
class GUIMouse;

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
	bool UpdateEditorGuiList();

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
	GUIButton*	CreateButton(GB_Rectangle<int> _position, 
							 GB_Rectangle<int> _standBySection, 
							 GB_Rectangle<int> _hoverSection, 
							 GB_Rectangle<int> _clickedSection);
	GUILabel*	CreateLabel(GB_Rectangle<int> _position, const char* _text = nullptr);
	GUIImage*	CreateImage(GB_Rectangle<int> _position, GB_Rectangle<int> _section);
	GUIMouse*	CreateMouse ()
	//GUIImage* createImage();

	GUIElement* GuiFactory();

public:
	std::list<GUIElement*> guiList;
	std::list<GUIElement*> debugGuiList;
	std::list<GUIElement*> editorGuiList;

private:

	SDL_Texture* atlas;
	GUIElement* mouseHover = nullptr;
	GUIElement* focus = nullptr;


	GUIElement* curs = NULL;
};

#endif // !__M_GUI_H__