#pragma once
#include "SDL\include\SDL_rect.h"

template <class type>
class GB_Rectangle
{
public:
	GB_Rectangle(type x, type y, type w, type h);
	GB_Rectangle(const GB_Rectangle<type> &copy);
	GB_Rectangle();
	virtual ~GB_Rectangle();

	GB_Rectangle<type>& operator=(const GB_Rectangle<type>& copy);

	inline bool Contains(type x, type y) const
	{
		return ((x >= this->x) && (x < (this->x + w)) && (y >= this->y) && (y < (this->y + h)));
	}	
	inline void Move(type x, type y)
	{
		this->x = x;
		this->y = y;
	}
	inline void Set(type x, type y, type w, type h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
	
	SDL_Rect getSDLrect();
	bool Collides(GB_Rectangle rect);

public:
	int x, y, w, h;
};

