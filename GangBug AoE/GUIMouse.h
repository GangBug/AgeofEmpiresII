#include "SDL\include\SDL.h"
#include "M_GUI.h"
#include "p2Point.h"
#include "GUIElement.h"

class GUICursor :public GUIElement
{
public:
	GUICursor(int margin_x, int margin_y);
	GUICursor(const SDL_Rect& section, int margin_x, int margin_y);
	~GUICursor();

	void SetSection(const SDL_Rect& section);
	SDL_Rect GetSection()const;
	bool draw();
	void Update(const GUIElement* mouse_hover, const GUIElement* focus);
	SDL_Texture* GetTexture()const;

private:
	SDL_Rect section;
	SDL_Texture* curs = nullptr;
	iPoint margin;
};
