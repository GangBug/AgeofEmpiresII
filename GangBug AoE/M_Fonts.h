#ifndef __M_FONTS_H__
#define __M_FONTS_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"
#include <list>

#define DEFAULT_FONT "fonts/open_sans/OpenSans-Regular.ttf" //default font from Ric	
#define DEFAULT_FONT_SIZE 24
#define MEDIUM_FONT_SIZE 12
#define SMALL_FONT_SIZE 8


struct SDL_Texture;
struct _TTF_Font;

class M_Fonts : public Module
{
public:

	M_Fonts(bool startEnabled = true);

	// Destructor
	virtual ~M_Fonts();

	// Called before render is available
	bool Awake(pugi::xml_node&)override;

	// Called before quitting
	bool CleanUp()override;

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, _TTF_Font* font = nullptr, SDL_Color color = { 255, 255, 255, 255 });

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = nullptr) const;

	void DrawDebug()override;

public:

	std::list<_TTF_Font*>	fonts;
	_TTF_Font*			defaultFont;
	_TTF_Font*			mediumFont;
	_TTF_Font*			smallFont;
	_TTF_Font*			WOW;
};


#endif // __M_FONTS_H__