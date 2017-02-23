#include "GUIElement.h"
#include "j1App.h"
#include "j1Render.h"

GUIElement::GUIElement() : rect({0,0,0,0})
{
}
GUIElement::~GUIElement()
{
	for (std::list<GUIElement*>::iterator it = childs.begin(); it != childs.end(); it++)
	{
		RELEASE(*it);
	}
	childs.clear();
}
void GUIElement::CheckInput(const GUIElement * mouse_hover, const GUIElement * focus)
{
}
bool GUIElement::CheckMouseOver() const
{
	iPoint mousePosition;
	App->input->GetMousePosition(mousePosition.x, mousePosition.y);
	
	return rect.Contains(mousePosition.x, mousePosition.y);
}
void GUIElement::Center()
{
	int frame_w = (parent) ? parent->GetLocalRect().w : App->render->camera.w;
	int frame_h = (parent) ? parent->GetLocalRect().h : App->render->camera.h;

	SetLocalPos(frame_w / 2 - rect.w / 2, frame_h / 2 - rect.h / 2);
}
void GUIElement::CenterX()
{
	int frame_w = (parent) ? parent->GetLocalRect().w : App->render->camera.w;

	SetLocalPos(frame_w / 2 - rect.w / 2, rect.h);
}
void GUIElement::CenterY()
{
	int frame_h = (parent) ? parent->GetLocalRect().h : App->render->camera.h;

	SetLocalPos(rect.w, frame_h / 2 - rect.h / 2);
}
rectangle GUIElement::GetScreenRect() const
{
	if (parent != nullptr)
	{
		iPoint p = GetScreenPos();
		return{ p.x, p.y, rect.w, rect.h };
	}
	return rect;
}
rectangle GUIElement::GetLocalRect() const
{
	return rect;
}
rectangle GUIElement::GetRectangle() const
{
	return rect;
}
iPoint GUIElement::GetScreenPos() const
{
	if (parent != nullptr)
		return parent->GetScreenPos() + iPoint(rect.x, rect.y);
	else
		return iPoint(rect.x, rect.y);
}
iPoint GUIElement::GetLocalPos() const
{
	return iPoint(rect.x, rect.y);
}
bool GUIElement::GetDraggable() const
{
	return draggable;
}
bool GUIElement::GetInteractive() const
{
	return interactive;
}
bool GUIElement::GetCanFocus() const
{
	return can_focus;
}
bool GUIElement::GetActive() const
{
	return active;
}
GUIElement * GUIElement::GetParent() const
{
	if (parent != nullptr)
	{
		return parent;
	}
	return nullptr;
}
const std::list<GUIElement*> GUIElement::GetChilds() const
{
	return childs;
}
GuiTypes GUIElement::GetType() const
{
	return type;
}
std::list<j1Module*> GUIElement::GetListeners() const
{
	return listeners;
}
bool GUIElement::GetMouseInside() const
{
	return mouse_inside;
}
void GUIElement::SetLocalPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}
void GUIElement::SetDraggable(bool _draggable) 
{
	draggable = _draggable;
}
void GUIElement::SetInteractive(bool _interactive)
{
	interactive = _interactive;
}
void GUIElement::SetCanFocus(bool _focus)
{
	can_focus = _focus;
}
void GUIElement::SetActive(bool _active) 
{
	active = _active;
}
void GUIElement::SetParent(GUIElement * _parent) 
{
	parent = _parent;
}
void GUIElement::SetType(GuiTypes _type) 
{
}
void GUIElement::SetRectangle(rectangle _rect) 
{
	rect = _rect;
}
void GUIElement::SetRectangle(int x, int y, int w, int h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}
void GUIElement::SetMouseInside(bool ins)
{
	mouse_inside = ins;
}
void GUIElement::SetSize(int w, int h)
{
	rect.w = w;
	rect.h = h;
}
