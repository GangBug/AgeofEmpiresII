#pragma once
#include <list>
#include "p2Point.h"
#include "j1Input.h"

class j1Module;

enum GuiEvents
{
	listening_ends,
	mouse_enters, // done
	mouse_leaves, // done
	mouse_lclick_down,// done
	mouse_lclick_up,// done
	mouse_rclick_down,// done
	mouse_rclick_up,// done
	gain_focus, // done
	lost_focus,  // done
	input_changed,
	input_submit,
	value_changed,
	return_down
};
enum GuiTypes
{
	unknown,
	image,
	label,
	button,
	input_text,
	load_bar,
	h_slider,
	v_slider,
	mouse_cursor,
	gui_rect
};

class GUIElement
{
	//Methods
public:
	GUIElement();
	virtual ~GUIElement();
	
	virtual void Update(const GUIElement* mouse_hover, const GUIElement* focus) {} //Do something	
	virtual void Draw() const {};// Print the element
	virtual void DebugDraw() const {}; // Print debug things if the element
	void CheckInput(const GUIElement* mouse_hover, const GUIElement* focus); //Getting the input
	bool CheckMouseOver() const;
	void Center();
	void CenterX();
	void CenterY();
	void AddListener(j1Module moduleToAdd);
	void RemoveListener(j1Module moduleToRemove);

	//Getters & Setters ------------------------------------------ START -------------------
	rectangle GetScreenRect() const;
	rectangle GetLocalRect() const;
	rectangle GetRectangle() const;
	iPoint GetScreenPos() const;
	iPoint GetLocalPos() const;
	bool GetDraggable() const;
	bool GetInteractive() const;
	bool GetCanFocus() const;
	bool GetActive() const;
	GUIElement* GetParent() const;
	const std::list<GUIElement*> GetChilds() const;
	GuiTypes GetType() const;
	std::list<j1Module*> GetListeners() const;
	bool GetMouseInside() const;


	void SetLocalPos(int x, int y);
	void SetDraggable(bool _draggable);
	void SetInteractive(bool _interactive);
	void SetCanFocus(bool _focus);
	void SetActive(bool _active);
	void SetParent(GUIElement* _parent);
	void SetType(GuiTypes _type);
	void SetRectangle(rectangle _rect);
	void SetRectangle(int x, int y, int w, int h);
	void SetMouseInside(bool ins);
	//Getters & Setters ------------------------------------------ END -------------------

protected:
	void SetSize(int w, int h);

	//Attributes
private:
	bool draggable = false;			// Can be moved?
	bool interactive = false;		// Is interactable?
	//bool cut_childs = false;		// I dont know a shit about this...
	bool can_focus = false;			// Can get the focus?
	bool active = true;				// Is active? if not dont do uptade
	GUIElement* parent = nullptr;	// Parent element
	std::list<GUIElement*> childs;	// Child elements
	GuiTypes type = GuiTypes::unknown; // Gui Type

protected:
	std::list<j1Module*> listeners;
	bool have_focus = false; // TODO implement it on event management

private:
	bool mouse_inside = false;
	rectangle rect;
};

