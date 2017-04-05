#ifndef __j1FONTS_H__
#define __j1FONTS_H__

#include "j1Module.h"
#include "SDL\include\SDL_pixels.h"
#include <list>
#include "j1Module.h"
#define DEFAULT_FONT "fonts/open_sans/OpenSans-Regular.ttf" //default font from Ric	
#define DEFAULT_FONT_SIZE 24
#define MEDIUM_FONT_SIZE 18
#define SMALL_FONT_SIZE 12
struct SDL_Texture;
struct _TTF_Font;

class j1Fonts : public j1Module
{
public:

	j1Fonts();

	// Destructor
	virtual ~j1Fonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, _TTF_Font* font = nullptr, SDL_Color color = { 0, 0, 0, 255 });

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = nullptr) const;

	void DrawDebug();

public:

	std::list<_TTF_Font*>	fonts;
	_TTF_Font*			defaultFont;
	_TTF_Font*			mediumFont;
	_TTF_Font*			smallFont;
	_TTF_Font*			WOW;
};


#endif // __j1FONTS_H__