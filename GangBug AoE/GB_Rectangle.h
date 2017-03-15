#ifndef __GB_RECTANGLE__
#define __GB_RECTANGLE__

#include "SDL\include\SDL_rect.h"

template <class type>
class GB_Rectangle
{
public:
	GB_Rectangle(type x, type y, type w, type h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	GB_Rectangle(const GB_Rectangle<type>& copy)
	{
		//(*this) = copy;
		this->x = copy.x;
		this->y = copy.y;
		this->w = copy.w;
		this->h = copy.h;
	}

	GB_Rectangle(const SDL_Rect& r) : x(r.x), y(r.y), w(r.w), h(r.h)
	{}
	
	GB_Rectangle() : x(0), y(0), w(0), h(0)
	{

	}

	virtual ~GB_Rectangle()
	{}

	GB_Rectangle& operator=(const GB_Rectangle& copy)
	{
		this->x = copy.x;
		this->y = copy.y;
		this->w = copy.w;
		this->h = copy.h;
		return (*this);
	}

	bool operator== (const GB_Rectangle& rect)
	{
		return (x == rect.x && y == rect.y && w == rect.w && h == rect.h);
	}

	bool operator!= (const GB_Rectangle& rect)
	{
		return (x != rect.x || y != rect.y || w != rect.w || h != rect.h);
	}

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

	void GetCenter(type& x, type& y)
	{
		x = this->x + w / 2;
		y = this->y + h / 2;
	}
	
	SDL_Rect GetSDLrect()
	{
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		return rect;
	}

	bool Collides(GB_Rectangle rect)
	{
		return (x < rect.x + rect.w &&
			x + w > rect.x &&
			y < rect.y + rect.h &&
			h + y > rect.y);
	}

public:
	int x, y, w, h;
};

extern GB_Rectangle<int> ZeroRectangle;
extern GB_Rectangle<int> OneRectangle;

#endif // !__GB_RECTANGLE__