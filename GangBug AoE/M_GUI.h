#ifndef __M_GUI_H__
#define __M_GUI_H__

#include <list>
#include "Module.h"
#include "GB_Rectangle.h"
#include "GUIAutoLabel.h"

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

	//This method is in testing phase
	void IterateList(std::list<GUIElement>* list, void* method);

	// UI factory
	// Any create adds the GUIElement into lists, this job
	// is for who is using this methods
	// This section is unfinished, so for now use this with caution
	GUIButton*	CreateButton(GB_Rectangle<int> _position, 
							 GB_Rectangle<int> _standBySection, 
							 GB_Rectangle<int> _hoverSection, 
							 GB_Rectangle<int> _clickedSection);
	GUILabel*	CreateLabel(GB_Rectangle<int> _position, size _size, const char* _text = nullptr);
	GUIImage*	CreateImage(GB_Rectangle<int> _position, GB_Rectangle<int> _section);
	GUIMouse*	CreateMouse();
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
	
	//debuging things and some stuff
	GUIMouse* curs = nullptr; // this goes to scene entity
	GUIAutoLabel<uint32>* lastFrameMS;
	GUIAutoLabel<uint32>* fps;
	GUILabel* xMouse;
	GUILabel* yMouse;


};

#endif // !__M_GUI_H__