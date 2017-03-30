
#ifndef __M_GUI_H__
#define __M_GUI_H__

#include <list>
#include "j1Module.h"
#include "GB_Rectangle.h"
#include "GUIAutoLabel.h"

class GUIElement;
class GUIButton;
class GUILabel;
class GUIImage;
class GUIMouse;
class CBeizier;
enum label_size;


class j1GUI : public j1Module
{
public:
	j1GUI();
	virtual ~j1GUI();

	bool Awake(pugi::xml_node&) ;
	bool Start() ;
	bool PreUpdate(float dt) ;
	bool Update(float dt) ;
	bool PostUpdate() ;

	bool LoadLayout(); //TODO: LoadLayout needs lots of improvements 
					   //Define wich list it fills and wich gui.xml gets for example 
					   //FIX: Each element could load and save himself
	bool SaveLayout(); //TODO: SaveLayout needs lots of improvements 
					   //Define wich list it saves and wich gui.xml creates for example
					   //FIX: Each element could load and save himself

					   //Not implemented
	bool UpdateGuiList();
	//Not implemented
	bool UpdateDebugGuiList();
	//Not implemented
	bool UpdateEditorGuiList();

	GUIElement* FindMouseHover();
	void ManageEvents();
	void BroadcastEventToListeners(GUIElement* element, gui_events event);

	void Draw();
	void DrawEditor();
	void DrawDebug();

	GUIElement* FindElement(std::list<GUIElement*> list, std::string name);
	bool FindElement(std::list<GUIElement*> list, GUIElement* element);

	//Getters & Setters
	SDL_Texture* GetAtlas() const;
	void SetAtlas(SDL_Texture* texture);

	//This method is in testing phase
	//void IterateList(std::list<GUIElement*>* list, void (M_GUI::*method)(GUIElement*));
	//void DoElementUpdate(GUIElement* element, float dt);
	//void DoElementDraw(GUIElement* element);

	// UI factory
	// Any create adds the GUIElement into lists, this job
	// is for who is using this methods
	// This section is unfinished, so for now use this with caution
	GUIButton*	CreateButton(GB_Rectangle<int> _position,
		GB_Rectangle<int> _standBySection,
		GB_Rectangle<int> _hoverSection,
		GB_Rectangle<int> _clickedSection, std::string name); // From nothing
	GUIButton*	CreateButtonFromPreset(GB_Rectangle<int> _position, std::string preset, std::string name, const char* _text = nullptr); // From a preset
	GUILabel*	CreateLabel(GB_Rectangle<int> _position, label_size _size, std::string name, const char* _text = nullptr);
	GUIImage*	CreateImage(GB_Rectangle<int> _position, GB_Rectangle<int> _section, std::string name); // From nothing
	GUIImage*	CreateImageFromPreset(GB_Rectangle<int> _position, std::string preset, std::string name); // From a preset
	GUIMouse*	CreateMouse();

	GUIElement* GuiFactory();

	bool GetUIEditing() const;
	void SetUIEditing(bool edit);

public:
	std::list<GUIElement*> guiList;
	std::list<GUIElement*> debugGuiList;
	std::list<GUIElement*> editorGuiList;

	CBeizier* cBeizier = nullptr;

private:

	SDL_Texture* atlas;
	GUIElement* mouseHover = nullptr;
	GUIElement* focus = nullptr;
	std::map<std::string, GUIElement*> guiPresets; // A map of basic UI elements defined on the xml
	bool mustSaveScene = false; 
	bool mustLoadScene = false;


	//debuging things and some stuff
	GUIMouse* curs = nullptr; // this goes to scene entity
	GUIAutoLabel<uint32>* lastFrameMS;
	GUIAutoLabel<uint32>* fps;
	GUILabel* xMouse;
	GUILabel* yMouse;
	bool UIEditing = false;

	GUIImage* img2 = nullptr; //TMP

};

#endif // !__M_GUI_H__