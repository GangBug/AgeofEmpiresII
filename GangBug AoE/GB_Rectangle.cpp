#include "GB_Rectangle.h"

template<class type>
GB_Rectangle<type>::GB_Rectangle(type x, type y, type w, type h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

template<class type>
GB_Rectangle<type>::GB_Rectangle(const GB_Rectangle<type> &copy)
{
	(*this) = copy;
}

template <class type>
GB_Rectangle<type>::GB_Rectangle()
{
}
template <class type>
GB_Rectangle<type>::~GB_Rectangle()
{
}

template<class type>
GB_Rectangle<type> & GB_Rectangle<type>::operator=(const GB_Rectangle<type> & copy)
{
	x = copy.x;
	y = copy.y;
	w = copy.w;
	h = copy.h;
}

template <class type>
SDL_Rect GB_Rectangle<type>::getSDLrect()
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}
template <class type>
bool GB_Rectangle<type>::Collides(GB_Rectangle rect) //TODO: This is really ugly, fix it
{
	SDL_Rect r;
	r.x = x; r.y = y; r.w = w; r.h = h;
	SDL_Rect r2;
	r2.x = rect.x; r2.y = rect.y; r2.w = rect.w; r2.h = rect.h;
	return (bool)SDL_HasIntersection((const SDL_Rect*)&r, (const SDL_Rect*)&r2);
}