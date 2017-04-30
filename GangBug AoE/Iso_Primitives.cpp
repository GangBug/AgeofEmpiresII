#include "Iso_Primitives.h"

#include "App.h"
#include "M_Render.h"
#include <math.h>
#include "Log.h"

///Class Primitive ------------------------------
//Constructors ==============
Primitive::Primitive()
{

}

Primitive::Primitive(const iPoint& position, const iPoint& displacement, const SDL_Color& color) :position(position), displacement(displacement), color(color)
{

}

Primitive::Primitive(const Primitive& copy) : position(copy.position), displacement(copy.displacement), color(copy.color), x_angle(copy.x_angle)
{

}

//Destructors ===============
Primitive::~Primitive()
{

}

//Functionality =============
bool Primitive::Draw()
{
	return true;
}

void Primitive::SetPosition(const iPoint & pos)
{
	position = pos;
}

void Primitive::SetDisplacement(const iPoint & desp)
{
	displacement = desp;
}

void Primitive::SetXAngle(float angle)
{
	x_angle = angle;
}

void Primitive::SetColor(const SDL_Color & rgba)
{
	color = rgba;
}

iPoint Primitive::GetPosition() const
{
	return position;
}
iPoint Primitive::GetDisplacement() const
{
	return displacement;
}
float Primitive::GetXAngle() const
{
	return x_angle;
}
SDL_Color Primitive::GetColor() const
{
	return color;
}
/// ---------------------------------------------


///Class Circle ---------------------------------
//Constructors ==============
Circle::Circle(const iPoint& position, uint rad, const iPoint& desplacement) :Primitive(position, desplacement), rad(rad)
{

}

Circle::Circle(const Circle & copy) : Primitive(copy), rad(copy.rad)
{

}

//Destructors ===============
Circle::~Circle()
{

}

//Functionality =============
bool Circle::Draw()
{
	//Just draw the circle mark
	return app->render->DrawCircle(position.x + displacement.x, position.y + displacement.y, rad, color.r, color.g, color.b, color.a, x_angle, true);
}

bool Circle::IsIn(const fPoint* loc) const
{
	return ((((loc->x - position.x) * (loc->x - position.x)) / (rad * rad) + ((loc->y - position.y) * (loc->y - position.y)) / ((rad * sin(x_angle)) * (rad * sin(x_angle)))) <= 1);
}

bool Circle::Intersects(const SDL_Rect * rect) const
{
	float dx = abs(position.x - (rect->x + rect->w * 0.5));
	float dy = abs(position.y - (rect->y + rect->h * 0.5));

	//Case the rect is out the circle limits in axis
	if (dx > rect->w * 0.5 + rad) return false;
	if (dy > rect->h * (0.5 + rad * sin(x_angle))) return false;

	//Case the rect is in the circle limits in axis
	if (dx <= rect->w * 0.5) return true;
	if (dx <= rect->w * 0.5) return true;

	//Case the rect intersects or not in the circle mid curves
	float a = (dx - rect->w * 0.5);
	float b = (dy - rect->h * 0.5);
	float corner_distance = a * a + b * b;

	return corner_distance <= rad ^ 2;
}

bool Circle::Intersects(const Circle * target) const
{
	iPoint vec(target->position.x - position.x, target->position.y - position.y);
	fPoint cpy(vec.x, vec.y);
	cpy.Norm();
	vec.x -= ((rad * cpy.x) + target->rad * cpy.x);
	vec.y -= (((target->rad + rad) * sin(x_angle) * cpy.y));
	float vec_len = sqrt(vec.x * vec.x + vec.y * vec.y);
	return (rad >= vec_len);
}

bool Circle::Intersects(const iPoint* point) const
{
	iPoint loc(point->x - position.x, point->y - position.y);
	return (abs(loc.x) <= rad && abs(loc.y) <= rad);
}

iPoint Circle::NearestPoint(const Circle* target) const
{
	iPoint vec(target->position.x - position.x, target->position.y - position.y);
	fPoint cpy(vec.x, vec.y);
	cpy.Norm();
	vec.x -= ((rad * cpy.x) + target->rad * cpy.x);
	vec.y -= (((target->rad + rad) * sin(x_angle) * cpy.y));

	return iPoint(position.x + vec.x, position.y + vec.y);
}

void Circle::SetRad(uint r)
{
	rad = r;
}

uint Circle::GetRad() const
{
	return rad;
}
/// ---------------------------------------------

///Class Rectngle -------------------------------
//Constructors ==============
Rectngle::Rectngle(const iPoint & position, uint width, uint height) :rect({ position.x, position.y, (int)width, (int)height })
{

}

//Destructors ===============
Rectngle::~Rectngle()
{

}

//Functionality =============
bool Rectngle::Draw()
{
	/*App->render->*/
	return false;
}

void Rectngle::SetPosition(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void Rectngle::SetRect(const SDL_Rect & new_rect)
{
	rect = new_rect;
}

void Rectngle::SetColor(const SDL_Color & new_color)
{
	color = new_color;
}

SDL_Rect Rectngle::GetRect() const
{
	return rect;
}

SDL_Color Rectngle::GetColor() const
{
	return color;
}
/// ---------------------------------------------

/// Class Alpha Cell ----------------------------
//Constructors ==============
AlphaCell::AlphaCell(const iPoint& position, unsigned short alpha) :position(position), alpha(alpha)
{

}

//Destructors ===============
AlphaCell::~AlphaCell()
{

}
