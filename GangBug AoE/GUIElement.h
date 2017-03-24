#pragma once
#include <list>
#include <vector>
#include "p2Point.h"
#include "M_Input.h"
#include "GB_Rectangle.h"
#include "App.h"
#include <map>


class Module;

enum status_flags
{
	NO_FLAGS		= 0,
	DRAGGABLE		= (1 << 0),
	INTERACTIVE		= (1 << 1),
	CAN_FOCUS		= (1 << 2),
	ACTIVE			= (1 << 3),
	STANDARD_PRESET = (1 << 4)
};

struct ElementStatus
{
	bool draggable		= false;	// Can be moved?
	bool interactive	= false;	// Is interactable?
    //bool cut_childs	= false;	// I dont know a shit about this...
	bool canFocus		= false;	// Can get the focus? --- For now this is not used...
	bool active			= true;		// Is active? if not dont do uptade
	bool mouseInside	= false;
	bool lClicked		= false;	// Left clicked
	bool rClicked		= false;	// Right clicked
	bool statusChanged	= false;	// Element status changed

};

enum gui_events
{
	LISTENING_END,
	MOUSE_ENTERS,			// done
	MOUSE_LEAVES,			// done
	MOUSE_LCLICK_DOWN,		// done
	MOUSE_LCLICK_UP,		// done
	MOUSE_RCLICK_DOWN,		// done
	MOUSE_RCLICK_UP,		// done
	GAIN_FOCUS,				// done
	LOST_FOUCS,				// done
	INPUT_CHANGED,
	INPUT_SUBMIT,
	VALUE_CHANGED,
	RETURN_DOWN
};
enum gui_types
{
	GUI_UNKNOWN,
	GUI_IMAGE,				//done
	GUI_LABEL,				//done
	GUI_BUTTON,				//done
	GUI_INPUT_TEXT,
	GUI_LOAD_BAR,
	GUI_H_SLIDER,
	GUI_V_SLIDER,
	GUI_MOUSE_CURSOR,		//done
	GUI_RECT
};

enum staticAnim_or_transition
{
	SAT_NONE = 0,

	SA_FLASH,
	SA_SHAKE,
	SA_PULSE,
	SA_BOUNCE,

	SAT_SEPARATOR, //Dont really like this must change it but for now i use this to know if it is a transition or a 

	T_SCALE,
	T_FADE,
	T_DROP,
	T_FLY,
	T_SLIDE
};

class GUIElement
{
	//Methods
public:
	GUIElement(int flags = NO_FLAGS);
	virtual ~GUIElement();
	
	void Update(const GUIElement* mouseHover, const GUIElement* focus); //Do something	// must implement dt
	
	virtual void OnUpdate(const GUIElement* mouseHover, const GUIElement* focus){}
	virtual void Draw() const {}// Print the element
	virtual void DebugDraw() const {} // Print debug things if the element
	void CheckInput(const GUIElement* mouseHover, const GUIElement* focus); //Getting the input
	bool CheckMouseOver() const;
	void Center();
	void CenterX();
	void CenterY();
	void AddListener(Module* moduleToAdd);
	void RemoveListener(Module* moduleToRemove);

	void OnGuiEvent(gui_events eventToReact);

	//Getters & Setters ·········································· START ···················
	GB_Rectangle<int> GetScreenRect() const;
	GB_Rectangle<int> GetLocalRect() const;
	iPoint GetScreenPos() const;
	iPoint GetLocalPos() const;
	iPoint GetSize() const;
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
	void SetSize(int w, int h);
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

	void AddAnimationOrTransition(gui_events eventToReact, staticAnim_or_transition animOrTransition);
	void RemoveAnimationOrTransitionReaction(gui_events eventToReact);
	void GetAllAnimationAndTransitions(std::vector<std::pair<gui_events, staticAnim_or_transition>>& animsAndTrans);
	bool HasEventReactionSet(gui_events eventToReact);
	staticAnim_or_transition GetAnimOrTransitionForEvent(gui_events eventToReact);

	//Getters & Setters ·········································· END ···················

private:
	void resize(fPoint newScale);

//protected:
//	void SetSize(int w, int h);

	//Attributes
private:
	GUIElement* parent = nullptr;			 // Parent element
	std::list<GUIElement*> childs;			 // Child elements
	gui_types type = gui_types::GUI_UNKNOWN; // Gui Type	
	GB_Rectangle<int> rect;
	fPoint scale = fPoint(1.f, 1.f);
	ElementStatus status;

	staticAnim_or_transition currentStaticAnimation = SAT_NONE;
	staticAnim_or_transition currentTransition = SAT_NONE;
	std::map<gui_events, staticAnim_or_transition> transAndAnimations;
	//TODO: Thought i could have a gui_events variable that stores all events i have a reaction to in order to not searching with map.find
	
protected:
	std::list<Module*> listeners;
	bool haveFocus = false; // TODO implement it on event management

};

