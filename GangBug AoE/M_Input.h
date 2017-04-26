#ifndef __M_INPUT_H__
#define __M_INPUT_H__

#include "Module.h"
#include "SDL/include/SDL_scancode.h"
#include "SDL/include/SDL_mouse.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50


struct SDL_Rect;

enum event_window
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum key_state
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class M_Input : public Module
{

public:

	M_Input(bool startEnabled = true);

	// Destructor
	virtual ~M_Input();

	// Called before render is available
	bool Awake(pugi::xml_node&)override;

	// Called before the first frame
	bool Start()override;

	// Called each loop iteration
	update_status PreUpdate(float dt)override;

	// Called before quitting
	bool CleanUp()override;

	// Gather relevant win events
	bool GetWindowEvent(event_window ev);

	// Check key states (includes mouse and joy buttons)
	key_state GetKey(int id) const
	{
		return keyboard[id];
	}

	key_state GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	void StartTyping();
	void StopTyping();
	const char* GetText();

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMouseMapPosition(int &x, int &y);
	void GetMouseScreenPosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	void DrawDebug();

private:
	bool		windowEvents[WE_COUNT];
	key_state*	keyboard;
	key_state	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouseMotionX = 0;
	int			mouseMotionY = 0;
	int			mouseX = 0;
	int			mouseY = 0;

	//text
	bool textInput;
	std::string lastText;
	int cursor = 0;
	int selectionLen = 0;
};

#endif // __M_INPUT_H__