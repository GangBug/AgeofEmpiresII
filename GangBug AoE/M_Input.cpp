#include "Log.h"
#include "App.h"
#include "M_Input.h"
#include "M_Window.h"
#include "SDL/include/SDL.h"

#define MAX_KEYS 300
#define _CRT_SECURE_NO_WARNINGS

M_Input::M_Input(bool startEnabled) : Module(startEnabled)
{
	name.assign("input");

	keyboard = new key_state[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(key_state) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(key_state) * NUM_MOUSE_BUTTONS);

}

// Destructor
M_Input::~M_Input()
{
	delete[] keyboard;
}

// Called before render is available
bool M_Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool M_Input::Start()
{
	//SDL_StartTextInput();
	return true;
}

// Called each loop iteration
update_status M_Input::PreUpdate(float dt)
{
	lastText = "";
	static SDL_Event event;
	
	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if(mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	//SDL_StartTextInput();

	while(SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			app->quit = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;
		case SDL_KEYUP:
			if (textInput)
			{
				printf("a");
			}
			break;
		case SDL_TEXTINPUT:
			if (textInput)
			{
				lastText += event.text.text;

				cursor += strlen(event.text.text);

				LOG("Input event: %s", event.edit.text);
			}
			break;
		/*case SDL_KEYUP:

			// Special case of micro controlling text input

			if (text_input)

			{

				switch (event.key.keysym.sym)

				{

				case SDLK_BACKSPACE:

					lastText.Cut(cursor - 1);

					if (cursor > 0)

						cursor--;

					break;

				case SDLK_DELETE:

					if (cursor < lastText.Length())

						lastText.Cut(cursor);

					break;

				case SDLK_KP_ENTER:

				case SDLK_RETURN2:

				case SDLK_RETURN:

					selection_len = 1;

					break;

				case SDLK_LEFT:

					if (cursor > 0)

						cursor--;

					break;

				case SDLK_RIGHT:

					if (cursor < lastText.Length())

						cursor++;

					break;

				case SDLK_HOME:

					cursor = 0;

					break;

				case SDLK_END:

					cursor = lastText.Length();

					break;

				}
			}
			break;

			case SDL_TEXTINPUT:

			lastText.operator+=(event.text.text);

			cursor += strlen(event.text.text);

			LOG("Input event: %s", event.edit.text);

			break;



			case SDL_TEXTEDITING:

			//lastText = event.edit.text;

			//cursor_text_input = event.edit.start;

			//selection_text_input = event.edit.length;

			LOG("Edit event: %s cursor %d selection %d", event.edit.text, event.edit.start, event.edit.length);

			break;*/

			case SDL_MOUSEBUTTONDOWN:
				mouse_buttons[event.button.button - 1] = KEY_DOWN;
				/*mouse_motion_x = 0;
				mouse_motion_y = 0;*/
				//LOG("Mouse button %d down", event.button.button-1);
			break;

			case SDL_MOUSEBUTTONUP:
				mouse_buttons[event.button.button - 1] = KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
			break;

			case SDL_MOUSEMOTION:
				int scale = app->win->GetScale();
				mouseMotionX = event.motion.xrel / scale;
				mouseMotionY = event.motion.yrel / scale;
				mouseX = event.motion.x / scale;
				mouseY = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;

		}
	}


	return UPDATE_CONTINUE;
}

void M_Input::StartTyping() {
	if (!textInput) {
		textInput = true;
		SDL_StartTextInput();
	}
}
void M_Input::StopTyping() {
	if (textInput) {
		textInput = false;
		SDL_StopTextInput();
	}
}
const char* M_Input::GetText() {
	return lastText.c_str();
}

// Called before quitting
bool M_Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}



// ---------
bool M_Input::GetWindowEvent(event_window ev)
{
	return windowEvents[ev];
}

void M_Input::GetMousePosition(int& x, int& y)
{
	x = mouseX;
	y = mouseY;
}

void M_Input::GetMouseMotion(int& x, int& y)
{
	x = mouseMotionX;
	y = mouseMotionY;
}

void M_Input::DrawDebug()
{
}
