#pragma once
#include <list>
#include "p2Point.h"
#include "M_Input.h"
#include "GB_Rectangle.h"
#include "App.h"


class Module;

struct ElementStatus
{
//Private acces
	bool draggable		= false;				// Can be moved?
	bool interactive	= false;			// Is interactable?
    //bool cut_childs	= false;			// I dont know a shit about this...
	bool can_focus		= false;				// Can get the focus?
	bool active			= true;					// Is active? if not dont do uptade
	bool mouseInside	= false;
//Public access
	bool lClicked		= false;				// Left clicked
	bool rClicked		= false;				// Right clicked
	bool statusChanged	= false;			// Element status changed

};

enum gui_events
{
	LISTENING_END,
	MOUSE_ENTERS, // done
	MOUSE_LEAVES, // done
	MOUSE_LCLICK_DOWN,// done
	MOUSE_LCLICK_UP,// done
	MOUSE_RCLICK_DOWN,// done
	MOUSE_RCLICK_UP,// done
	GAIN_FOCUS, // done
	LOST_FOUCS,  // done
	INPUT_CHANGED,
	INPUT_SUBMIT,
	VALUE_CHANGED,
	RETURN_DOWN
};
enum gui_types
{
	GUI_UNKNOWN,
	GUI_IMAGE,
	GUI_LABEL,
	GUI_BUTTON,
	GUI_INPUT_TEXT,
	GUI_LOAD_BAR,
	GUI_H_SLIDER,
	GUI_V_SLIDER,
	GUI_MOUSE_CURSOR,
	GUI_RECT
};

class GUIElement
{
	//Methods
public:
	GUIElement();
	virtual ~GUIElement();
	
	virtual void Update(const GUIElement* mouseHover, const GUIElement* focus) {} //Do something	
	virtual void Draw() const {}// Print the element
	virtual void DebugDraw() const {} // Print debug things if the element
	void CheckInput(const GUIElement* mouseHover, const GUIElement* focus); //Getting the input
	bool CheckMouseOver() const;
	void Center();
	void CenterX();
	void CenterY();
	void AddListener(Module* moduleToAdd);
	void RemoveListener(Module* moduleToRemove);

	//Getters & Setters ·········································· START ···················
	GB_Rectangle<int> GetScreenRect() const;
	GB_Rectangle<int> GetLocalRect() const;
	GB_Rectangle<int> GetRectangle() const;
	iPoint GetScreenPos() const;
	iPoint GetLocalPos() const;
	bool GetDraggable() const;
	bool GetInteractive() const;
	bool GetCanFocus() const;
	bool GetActive() const;
	GUIElement* GetParent() const;
	const std::list<GUIElement*> GetChilds() const;
	gui_types GetType() const;
	std::list<Module*> GetListeners() const;
	bool GetMouseInside() const;
	fPoint GetScale() const;
	bool GetLClicked() const;
	bool GetRClicked() const;
	ElementStatus GetElementStatus() const;


	void SetLocalPos(int x, int y);
	void SetDraggable(bool _draggable);
	void SetInteractive(bool _interactive);
	void SetCanFocus(bool _focus);
	void SetActive(bool _active);
	void SetParent(GUIElement* _parent);
	void SetType(gui_types _type);
	void SetRectangle(GB_Rectangle<int> _rect);
	void SetRectangle(int x, int y, int w, int h);
	void SetMouseInside(bool ins);
	void SetScale(fPoint _scale);
	void SetScale(float _scaleX, float _scaleY);
	void SetLClicked(bool l);
	void SetRClicked(bool r);
	void SetStatusChanged(bool changed);

	//Getters & Setters ·········································· END ···················

private:
	void resize(fPoint newScale);

protected:
	void SetSize(int w, int h);

	//Attributes
private:
	GUIElement* parent = nullptr;			 // Parent element
	std::list<GUIElement*> childs;			 // Child elements
	gui_types type = gui_types::GUI_UNKNOWN; // Gui Type	
	GB_Rectangle<int> rect;
	fPoint scale = fPoint(1.f, 1.f);
	ElementStatus status;

protected:
	std::list<Module*> listeners;
	bool haveFocus = false; // TODO implement it on event management

};

