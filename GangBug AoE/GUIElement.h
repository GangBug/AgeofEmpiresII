#pragma once
#include <list>
#include <vector>
#include "p2Point.h"
#include "M_Input.h"
#include "GB_Rectangle.h"
#include "App.h"
#include <map>
#include "Bezier.h"

class Module;

enum status_flags
{
	NO_FLAGS = 0,
	DRAGGABLE = (1 << 0),
	INTERACTIVE = (1 << 1),
	CAN_FOCUS = (1 << 2),
	ACTIVE = (1 << 3),
	STANDARD_PRESET = (1 << 4),
	VISIBLE = (1 << 5)
};
enum gui_events
{
	EVENT_NONE = 0,
	LISTENING_END = (1 << 0),
	MOUSE_ENTERS = (1 << 1),			// done
	MOUSE_LEAVES = (1 << 2),			// done
	MOUSE_LCLICK_DOWN = (1 << 3),			// done
	MOUSE_LCLICK_UP = (1 << 4),			// done
	MOUSE_RCLICK_DOWN = (1 << 5),			// done
	MOUSE_RCLICK_UP = (1 << 6),			// done
	GAIN_FOCUS = (1 << 7),			// done
	LOST_FOUCS = (1 << 8),			// done
	INPUT_CHANGED = (1 << 9),
	INPUT_SUBMIT = (1 << 10),
	VALUE_CHANGED = (1 << 11),
	RETURN_DOWN = (1 << 12),

	ENABLE = (1 << 13),
	DISABLE = (1 << 14),

	//UI-GAME-EVENTS ---> Put them on a better event system and they wont need to coexist with GUI-EVENTS
	//					  for now this is okay

	CLOSE_APP = (1 << 15),
	START_GAME = (1 << 16),
	ADD_MINER = (1 << 17),
	ADD_CONSTRUCTOR = (1 << 18),
	ADD_LUMBERJACK = (1 << 19),
	ADD_FARMER = (1 << 20),
	REMOVE_MINER = (1 << 21),
	REMOVE_CONSTRUCTOR = (1 << 22),
	REMOVE_LUMBERJACK = (1 << 23),
	REMOVE_FARMER = (1 << 24),
	OPEN_MENU = (1 << 25)
	
	//TODO
};

struct ElementStatus
{
	bool draggable = false;	// Can be moved?
	bool interactive = false;	// Is interactable?
	bool visible = false;
								//bool cut_childs	= false;	// I dont know a shit about this...
	bool canFocus = false;	// Can get the focus? --- For now this is not used...
	bool active = true;		// Is active? if not dont do uptade
	bool mouseInside = false;
	bool lClicked = false;	// Left clicked
	bool rClicked = false;	// Right clicked
	bool statusChanged = false;	// Element status changed

	gui_events onLClickUp = EVENT_NONE;
	gui_events onLClickDown = EVENT_NONE;
	gui_events onRClickUp = EVENT_NONE;
	gui_events onRClickDown = EVENT_NONE;
	gui_events onGainFocus = EVENT_NONE;
	gui_events onLooseFocus = EVENT_NONE;
	gui_events onMouseEnters = EVENT_NONE;
	gui_events onMouseLeaves = EVENT_NONE;

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
	T_SLIDE,
	T_MOVE_RIGHT,
	T_MOVE_LEFT,
	T_MOVE_UP,
	T_MOVE_DOWN
};

class GUIElement
{
	//Methods
public:
	GUIElement(std::string name, int flags = NO_FLAGS);
	virtual ~GUIElement();

	void Update(const GUIElement* mouseHover, const GUIElement* focus, float dt); //Do something	// must implement dt

	virtual void OnUpdate(const GUIElement* mouseHover, const GUIElement* focus, float dt) {}
	virtual void Draw() const {}// Print the element
	virtual void DebugDraw() const {} // Print debug things if the element
	void CheckInput(const GUIElement* mouseHover, const GUIElement* focus); //Getting the input
	bool CheckMouseOver() const;
	void Center();
	void CenterX();
	void CenterY();
	void AddListener(Module* moduleToAdd);
	void RemoveListener(Module* moduleToRemove);
	void AddScene(Module* scene);
	void RemoveScene(Module* scene);
	bool ContainsScene(std::string scene);

	bool Save(pugi::xml_node& node) const;
	//bool Load(pugi::xml_node& node);

	//virtual bool OnSave(pugi::xml_node& node)const;
	//virtual bool OnLoad(pugi::xml_node* node);

	void OnGuiEvent(int64_t eventToReact);

	GUIElement* operator==(GUIElement* element); //TODO: needed to compare elements

												 //Getters & Setters ·········································· START ···················
	GB_Rectangle<int> GetScreenRect() const;
	GB_Rectangle<int> GetLocalRect() const;
	iPoint GetScreenPos() const;
	iPoint GetLocalPos() const;
	iPoint GetGlobalPos() const;
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
	iPoint GetDrawPosition()const;
	GB_Rectangle<float> GetDrawRect()const;
	std::string GetPresetType() const;
	std::string GetName() const;
	std::list<Module*> GetListeners_noconst();
	bool GetVisible() const;

	virtual void SetLocalPos(int x, int y);
	virtual void SetGlobalPos(int x, int y);
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
	void SetDrawPosition(float x, float y);
	void SetPresetType(std::string str);
	void SetName(std::string str);
	void SetVisible(bool visible);

	void SetOnLClickUp(gui_events _event);
	void SetOnLClickDown(gui_events _event);
	void SetOnRClickUp(gui_events _event);
	void SetOnRClickDown(gui_events _event);
	void SetOnGainFocus(gui_events _event);
	void SetOnLooseFocus(gui_events _event);
	void SetOnMouseEnters(gui_events _event);
	void SetOnMouseLeaves(gui_events _event);


	void Enable();
	void Disable();

	void AddAnimationOrTransition(gui_events eventToReact, staticAnim_or_transition animOrTransition);
	void RemoveAnimationOrTransitionReaction(gui_events eventToReact);
	void GetAllAnimationAndTransitions(std::vector<std::pair<gui_events, staticAnim_or_transition>>& animsAndTrans);
	bool HasEventReactionSet(gui_events eventToReact);
	staticAnim_or_transition GetAnimOrTransitionForEvent(gui_events eventToReact);

	//Getters & Setters ·········································· END ···················

	virtual void Serialize(pugi::xml_node root) {}
	virtual void Deserialize(pugi::xml_node root) {}

private:
	void resize(fPoint newScale);

	void FlashSA(float dt);
	void ShakeSA(float dt);
	void PulseSA(float dt);
	void BounceSA(float dt);

	void ScaleT(float dt);
	void FadeT(float dt);
	void DropT(float dt);
	void FlyT(float dt);
	void SlideT(float dt);
	void MoveRightT(float dt);
	void MoveLeftT(float dt);
	void MoveUpT(float dt);
	void MoveDownT(float dt);

	//protected:
	//	void SetSize(int w, int h);

	//Attributes
private:
	GUIElement* parent = nullptr;			 // Parent element
	std::list<GUIElement*> childs;			 // Child elements
	gui_types type = gui_types::GUI_UNKNOWN; // Gui Type	
	GB_Rectangle<int> rect;
	fPoint scale = fPoint(1.f, 1.f);
	iPoint localPosition = iPoint(0, 0);
	ElementStatus status;

	std::string presetName;
	std::string name;

	staticAnim_or_transition currentStaticAnimation = SAT_NONE;
	staticAnim_or_transition currentTransition = SAT_NONE;
	std::map<gui_events, staticAnim_or_transition> transAndAnimations;
	gui_events eventsToReact = EVENT_NONE;
	GB_Rectangle<float> drawRect;

	cbezier_type curveType = cbezier_type::CB_EASE_INOUT_BACK;

	bool mustDisable = false;

	Timer transTimer;
	iPoint transOrigin = iPoint(0, 0);
	iPoint transDestination = iPoint(0, 0);
	int currentTransTime = 0;
	bool doingTransition = false;

	int currentAnimTim = 0;
	int animDuraton = 1000;
	bool doingAnimation = false;
	int animTime = 500;



protected:
	std::list<Module*> listeners;
	std::map<std::string, Module*> scenes;

	bool haveFocus = false; // TODO implement it on event management
	int alpha = 255;

};

