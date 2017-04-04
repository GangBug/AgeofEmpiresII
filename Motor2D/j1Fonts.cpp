#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1FileSystem.h"
#include "j1Fonts.h"
#include "j1Module.h"
#include "j1Gui.h"

#include "SDL\include\SDL.h"
#include "SDL_TTF\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

j1Fonts::j1Fonts()
{
	name.assign("fonts");
}

// Destructor
j1Fonts::~j1Fonts()
{}

// Called before render is available
bool j1Fonts::Awake(pugi::xml_node& conf)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
		defaultFont = Load(path, size*WINDOWSCALE);
		//char* path = conf.child("medium_font").attribute("file").as_string(DEFAULT_FONT);
		size = conf.child("medium_font").attribute("size").as_int(MEDIUM_FONT_SIZE);
		mediumFont = Load(path, size*WINDOWSCALE);
		//char* path = conf.child("small_font").attribute("file").as_string(DEFAULT_FONT);
		size = conf.child("small_font").attribute("size").as_int(SMALL_FONT_SIZE);
		smallFont = Load(path, size*WINDOWSCALE);

	}

	return ret;
}

// Called before quitting
bool j1Fonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");
	std::list<TTF_Font*>::iterator item;

	for (item = fonts.begin(); item != fonts.end(); item++)
	{
		TTF_CloseFont((*item));
	}

	fonts.clear();
	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const j1Fonts::Load(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFontRW(App->fs->Load(path), 1, size);

	if (font == nullptr)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.push_back(font);
	}

	return font;
}

// Print text using font
SDL_Texture* j1Fonts::Print(const char* text, _TTF_Font* font, SDL_Color color)
{
	SDL_Texture* ret = nullptr;
	SDL_Surface* surface = TTF_RenderText_Blended((font) ? font : defaultFont, text, color);

	if (surface == nullptr)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = App->tex->LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return ret;
}

// calculate size of a text
bool j1Fonts::CalcSize(const char* text, int& width, int& height, _TTF_Font* font) const
{
	bool ret = false;

	if (TTF_SizeText((font) ? font : defaultFont, text, &width, &height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}

void j1Fonts::DrawDebug()
{
}