#include "Log.h"
#include "App.h"
#include "M_Window.h"

#include "SDL/include/SDL.h"
#include "GUIElement.h"


M_Window::M_Window(bool startEnabled) : Module(startEnabled)
{
	window = nullptr;
	screenSurface = nullptr;
	name.assign("window");
}

// Destructor
M_Window::~M_Window()
{
}

// Called before render is available
bool M_Window::Awake(pugi::xml_node& config)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN;
		bool fullscreen = config.child("fullscreen").attribute("value").as_bool();
		bool borderless = config.child("borderless").attribute("value").as_bool();
		bool resizable = config.child("resizable").attribute("value").as_bool();
		bool fullscreen_window = config.child("fullscreen_window").attribute("value").as_bool();
		bool start_maximized = config.child("start_maximized").attribute("value").as_bool();

		if (fullscreen)
		{
			SDL_DisplayMode DM;
			SDL_GetCurrentDisplayMode(0, &DM);
			width = DM.w;
			height = DM.h;
		}
		else
		{
			width = config.child("resolution").attribute("width").as_int();
			height = config.child("resolution").attribute("height").as_int();
		}		
		scale = config.child("resolution").attribute("scale").as_int(1);

		if(fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(fullscreen_window == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (start_maximized)
		{
			flags |= SDL_WINDOW_MAXIMIZED;
		}

		window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == nullptr)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool M_Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void M_Window::SetTitle(const char* newTitle)
{
	//title.create(new_title);
	SDL_SetWindowTitle(window, newTitle);
}

void M_Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}

iPoint M_Window::GetWindowSize() const
{
	return iPoint(width, height);
}

uint M_Window::GetScale() const
{
	return scale;
}

void M_Window::DrawDebug()
{
}