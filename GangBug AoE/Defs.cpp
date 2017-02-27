#include "Defs.h"
#include "SDL\include\SDL_rect.h"

bool rectangle::Collides(rectangle rect) //TODO: This is really ugly, fix it
{
	SDL_Rect r;
	r.x = x; r.y = y; r.w = w; r.h = h;
	SDL_Rect r2;
	r2.x = rect.x; r2.y = rect.y; r2.w = rect.w; r2.h = rect.h;
	return (bool)SDL_HasIntersection((const SDL_Rect*)&r, (const SDL_Rect*)&r2);
}