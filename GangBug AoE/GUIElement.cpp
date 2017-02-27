#include "GUIElement.h"
#include "App.h"
#include "M_Render.h"

GUIElement::GUIElement() : rect(0,0,0,0)
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
void GUIElement::CheckInput(const GUIElement * mouseHover, const GUIElement * focus)
{
}
bool GUIElement::CheckMouseOver() const
{
	iPoint mousePosition;
	app->input->GetMousePosition(mousePosition.x, mousePosition.y);
	
	return rect.Contains(mousePosition.x, mousePosition.y);
}
void GUIElement::Center()
{
	int frame_w = (parent) ? parent->GetLocalRect().w : app->render->camera.w;
	int frame_h = (parent) ? parent->GetLocalRect().h : app->render->camera.h;

	SetLocalPos(frame_w / 2 - rect.w / 2, frame_h / 2 - rect.h / 2);
}
void GUIElement::CenterX()
{
	int frame_w = (parent) ? parent->GetLocalRect().w : app->render->camera.w;

	SetLocalPos(frame_w / 2 - rect.w / 2, rect.h);
}
void GUIElement::CenterY()
{
	int frame_h = (parent) ? parent->GetLocalRect().h : app->render->camera.h;

	SetLocalPos(rect.w, frame_h / 2 - rect.h / 2);
}
GB_Rectangle<int> GUIElement::GetScreenRect() const
{
	if (parent != nullptr)
	{
		iPoint p = GetScreenPos();
		return{ p.x, p.y, rect.w, rect.h };
	}
	return rect;
}
GB_Rectangle<int> GUIElement::GetLocalRect() const
{
	return rect;
}
GB_Rectangle<int> GUIElement::GetRectangle() const
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
gui_types GUIElement::GetType() const
{
	return type;
}
std::list<Module*> GUIElement::GetListeners() const
{
	return listeners;
}
bool GUIElement::GetMouseInside() const
{
	return mouseInside;
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
void GUIElement::SetType(gui_types _type)
{
}
void GUIElement::SetRectangle(GB_Rectangle<int> _rect)
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
	mouseInside = ins;
}
void GUIElement::SetSize(int w, int h)
{
	rect.w = w;
	rect.h = h;
}
