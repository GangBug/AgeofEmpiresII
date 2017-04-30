
#include"Defs.h"
#include "p2Point.h"
#include"SDL_image/include/SDL_image.h"

///Class Primitive ------------------------------
//Base primitive class
class Primitive
{
public:

	Primitive();
	Primitive(const iPoint& position, const iPoint& displacement = { 0,0 }, const SDL_Color& color = { 0,0,0,255 });
	Primitive(const Primitive& copy);
	~Primitive();

protected:

	iPoint		position = { 0,0 };
	iPoint		displacement = { 0,0 };
	float		x_angle = 0.5f;
	SDL_Color	color = { 0,0,0,255 };

public:

	//Functionality ---------
	//Draw
	virtual bool	Draw();
	//Set Methods
	void			SetPosition(const iPoint& pos);
	void			SetDisplacement(const iPoint& desp);
	void			SetXAngle(float angle);
	void			SetColor(const SDL_Color& rgba);
	//Get Methods
	iPoint			GetPosition()const;
	iPoint			GetDisplacement()const;
	float			GetXAngle()const;
	SDL_Color		GetColor()const;
};
/// ---------------------------------------------


///Class Circle ---------------------------------
//Circle isometric primitive
class Circle : public Primitive
{
public:

	Circle(const iPoint& position = { 0,0 }, uint rad = 0, const iPoint& displacement = { 0,0 });
	Circle(const Circle& copy);
	~Circle();

private:

	uint rad = 0;

public:

	//Functionality ---------
	//Draw
	bool	Draw();
	//Shape functions
	bool	IsIn(const fPoint* loc) const;
	bool	Intersects(const SDL_Rect* rect) const;
	bool	Intersects(const Circle* target)const;
	bool	Intersects(const iPoint* point)const;
	iPoint  NearestPoint(const Circle* target)const;

	//Set Methods
	void	SetRad(uint r);
	//Get Methods
	uint	GetRad()const;
};
/// ---------------------------------------------

///Class Rectngle ------------------------------
class Rectngle
{
public:

	Rectngle(const iPoint& position = { 0,0 }, uint width = 0, uint height = 0);
	~Rectngle();

private:

	SDL_Rect	rect = { 0,0,0,0 };
	SDL_Color	color = { 0,0,0,0 };

public:

	//Functionality ---------
	bool Draw();

	void	SetPosition(int x, int y);
	void	SetRect(const SDL_Rect& new_rect);
	void	SetColor(const SDL_Color& new_color);

	SDL_Rect	GetRect()const;
	SDL_Color	GetColor()const;

};
/// ---------------------------------------------

/// Class Alpha Cell ----------------------------
class AlphaCell
{
public:

	AlphaCell(const iPoint& position = { 0,0 }, unsigned short alpha = 255);
	~AlphaCell();

public:

	iPoint position = { 0,0 };
	unsigned short alpha = 255;

};
/// ---------------------------------------------
#endif // _ISO_PRIMITVES_
