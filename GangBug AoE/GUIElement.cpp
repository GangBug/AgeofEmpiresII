#include "GUIElement.h"
#include "App.h"
#include "M_Render.h"

#include "M_Window.h"

GUIElement::GUIElement(int flags) : rect(0,0,0,0), drawRect(0.f, 0.f, 0.f, 0.f)
{
	if (flags & DRAGGABLE)
		SetDraggable(true);
	else
		SetDraggable(false);

	if (flags & INTERACTIVE)
		SetInteractive(true);
	else
		SetInteractive(false);

	if (flags & CAN_FOCUS)
		SetCanFocus(true);
	else
		SetCanFocus(false);

	if (flags & ACTIVE)
		SetActive(true);
	else
		SetActive(false);
	
	if (flags & STANDARD_PRESET)
	{
		SetActive(true);
		SetCanFocus(true);
		SetInteractive(true);
		SetDraggable(false);
	}

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
	int frame_w = (parent) ? parent->GetLocalRect().w : app->render->camera.w - (GetLocalRect().w / 2);
	int frame_h = (parent) ? parent->GetLocalRect().h : app->render->camera.h - (GetLocalRect().h / 2);

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
void GUIElement::AddListener(Module * moduleToAdd)
{
	std::list<Module*>::iterator it = std::find(listeners.begin(), listeners.end(), moduleToAdd);
	if (it == listeners.end())
		listeners.push_back(moduleToAdd);
}
void GUIElement::RemoveListener(Module * moduleToRemove)
{
	std::list<Module*>::iterator it = std::find(listeners.begin(), listeners.end(), moduleToRemove);
	if (it != listeners.end())
	{
		listeners.erase(it);
	}	
}
bool GUIElement::Save(pugi::xml_node & node) const
{
	bool ret = true;

	return false;
}
void GUIElement::OnGuiEvent(gui_events eventToReact)
{
	if (this->eventsToReact & eventToReact)
	{
		std::map<gui_events, staticAnim_or_transition>::iterator it = transAndAnimations.find(eventToReact);
		if (it != transAndAnimations.end())
		{
			staticAnim_or_transition tmp = it->second;
			if (tmp < SAT_SEPARATOR)
				currentStaticAnimation = tmp;
			else if (tmp > SAT_SEPARATOR)
				currentTransition = tmp;
		}
	}
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
iPoint GUIElement::GetSize() const
{
	return iPoint(rect.w, rect.h);
}
bool GUIElement::GetDraggable() const
{
	return status.draggable;
}
bool GUIElement::GetInteractive() const
{
	return status.interactive;
}
bool GUIElement::GetCanFocus() const
{
	return status.canFocus;
}
bool GUIElement::GetActive() const
{
	return status.active;
}
GUIElement * GUIElement::GetParent() const
{
	return parent;
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
	return status.mouseInside;
}
fPoint GUIElement::GetScale() const
{
	return scale;
}
bool GUIElement::GetLClicked() const
{
	return status.lClicked;
}
bool GUIElement::GetRClicked() const
{
	return status.rClicked;
}
ElementStatus GUIElement::GetElementStatus() const
{
	return status;
}
iPoint GUIElement::GetDrawPosition()const
{
	return iPoint(drawRect.x, drawRect.y);
}
GB_Rectangle<float> GUIElement::GetDrawRect()const
{
	return drawRect;
}

void GUIElement::SetLocalPos(int x, int y)
{
	//Changes this item position and its childs.
	if (parent)
	{
		rect.x = x + parent->GetLocalPos().x;
		rect.y = y + parent->GetLocalPos().y;
	}
	else
	{
		rect.x = x;
		rect.y = y;
	}
	drawRect.x = rect.x;
	drawRect.y = rect.y;
	
	for (std::list<GUIElement*>::iterator it = childs.begin(); it != childs.end(); ++it)
		if ((*it)) (*it)->SetLocalPos((*it)->GetLocalPos().x + x, (*it)->GetLocalPos().y + y);
			
}
void GUIElement::SetDraggable(bool _draggable) 
{
	status.draggable = _draggable;
	status.statusChanged = true;
}
void GUIElement::SetInteractive(bool _interactive)
{
	status.interactive = _interactive;
	status.statusChanged = true;
}
void GUIElement::SetCanFocus(bool _focus)
{
	status.canFocus = _focus;
	status.statusChanged = true;
}
void GUIElement::SetActive(bool _active) 
{
	if (_active)
		Enable();
	else
		Disable();
}
void GUIElement::SetParent(GUIElement * _parent) 
{
	parent = _parent;
	status.statusChanged = true;
}
void GUIElement::SetType(gui_types _type)
{
	type = _type;
	status.statusChanged = true;
}
void GUIElement::SetRectangle(GB_Rectangle<int> _rect)
{
	//Only modifies this element but doesnt change childs.
	rect = _rect;
	drawRect.Set(_rect.x, _rect.y, _rect.w, _rect.h);
	status.statusChanged = true;
}
void GUIElement::SetRectangle(int x, int y, int w, int h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	drawRect.Set(x, y, w, h);
	status.statusChanged = true;
}
void GUIElement::SetMouseInside(bool ins)
{
	if (status.interactive && status.active)
	{
		status.mouseInside = ins;
		status.statusChanged = true;
	}

}
void GUIElement::SetScale(fPoint _scale)
{
	scale = _scale;
	resize(_scale);
	status.statusChanged = true;
}
void GUIElement::SetScale(float _scaleX, float _scaleY)
{
	scale.x = _scaleX;
	scale.y = _scaleY;
	resize(fPoint(_scaleX, _scaleY));
	status.statusChanged = true;
}
void GUIElement::SetLClicked(bool l)
{
	if (status.interactive && status.active)
	{
		status.lClicked = l;
		status.statusChanged = true;
	}
}
void GUIElement::SetRClicked(bool r)
{
	if (status.interactive && status.active)
	{
		status.rClicked = r;
		status.statusChanged = true;
	}
}
void GUIElement::SetStatusChanged(bool changed)
{
	status.statusChanged = changed;
}
void GUIElement::Enable()
{
	if (!status.active)
	{
		if (eventsToReact & ENABLE)
		{
			OnGuiEvent(ENABLE);
		}
		else
		{
			drawRect.Set(rect.x, rect.y, rect.w, rect.h);
		}
		status.active = true; 
		status.statusChanged = true;
	}
}
void GUIElement::Disable()
{
	if (status.active)
	{
		//If we have a reaction to use at disable dont set active to false directly, 
		//send the event and later in the transition set it to false.
		//If there is no reaction just set active to false
		if (eventsToReact & DISABLE)
		{
			OnGuiEvent(DISABLE);
		}
		else
		{
			status.active = false;
		}
		status.statusChanged = true;
	}
}
void GUIElement::AddAnimationOrTransition(gui_events eventToReact, staticAnim_or_transition animOrTransition)
{
	std::map<gui_events, staticAnim_or_transition>::iterator it = transAndAnimations.find(eventToReact);
	if (it != transAndAnimations.end())
	{
		//If we find that that event has already been told to react we overide it.
		it->second = animOrTransition;
	}
	else
	{
		//If we doont find that event reaction setted already we just set it.
		transAndAnimations.insert(std::pair<gui_events, staticAnim_or_transition>(eventToReact, animOrTransition));
		this->eventsToReact = (gui_events)(this->eventsToReact | eventToReact);
	}
}
void GUIElement::RemoveAnimationOrTransitionReaction(gui_events eventToReact)
{
	if (this->eventsToReact & eventToReact)
	{
		std::map<gui_events, staticAnim_or_transition>::iterator it = transAndAnimations.find(eventToReact);
		if (it != transAndAnimations.end())
		{
			transAndAnimations.erase(it);
			this->eventsToReact = (gui_events)(this->eventsToReact & ~eventToReact);
		}
	}
}
void GUIElement::GetAllAnimationAndTransitions(std::vector<std::pair<gui_events, staticAnim_or_transition>>& animsAndTrans)
{
	std::map<gui_events, staticAnim_or_transition>::iterator it = transAndAnimations.begin();
	for (; it != transAndAnimations.end(); ++it)
	{
		animsAndTrans.push_back((*it));
	}
}
bool GUIElement::HasEventReactionSet(gui_events eventToReact)
{
	return (eventsToReact & eventToReact);
}
staticAnim_or_transition GUIElement::GetAnimOrTransitionForEvent(gui_events eventToReact)
{
	if (this->eventsToReact & eventToReact)
	{
		std::map<gui_events, staticAnim_or_transition>::iterator it = transAndAnimations.find(eventToReact);
		if (it != transAndAnimations.end())
		{
			return it->second;
		}
	}

	return SAT_NONE;
}
void GUIElement::resize(fPoint newScale)
{
	fPoint variation = fPoint(scale.x / newScale.x, scale.y / newScale.y);
	rect.x *= variation.x;
	rect.y *= variation.y;
	drawRect.x *= variation.x;
	drawRect.y *= variation.y;
	status.statusChanged = true;
}
void GUIElement::SetSize(int w, int h)
{
	rect.w = w;
	rect.h = h;
	drawRect.w = w;
	drawRect.h = h;
	status.statusChanged = true;
}
void GUIElement::SetDrawPosition(float x, float y)
{
	drawRect.Move(x, y);

	for (std::list<GUIElement*>::iterator it = childs.begin(); it != childs.end(); ++it)
		if ((*it)) (*it)->SetDrawPosition((*it)->drawRect.x + x, (*it)->drawRect.y + y);
}

void GUIElement::Update(const GUIElement* mouseHover, const GUIElement* focus, float dt)
{
	//When updateing first do the element particular update overrided in each subtype.
	OnUpdate(mouseHover, focus);

	//Then process the animations and transitions.
	if (currentStaticAnimation != SAT_NONE)
	{
		//Do here the animation according the active one.

		switch (currentStaticAnimation)
		{
		case SA_FLASH:
			FlashSA();
			break;
		case SA_SHAKE:
			ShakeSA();
			break;
		case SA_PULSE:
			PulseSA();
			break;
		case SA_BOUNCE:
			BounceSA();
			break;
		}
		
	}

	if (currentTransition != SAT_NONE)
	{
		//DO here the transition logic according the one active.

		switch (currentTransition)
		{
		case T_SCALE:
			ScaleT();
			break;
		case T_FADE:
			FadeT();
			break;
		case T_DROP:
			DropT();
			break;
		case T_FLY:
			FlyT();
			break;
		case T_SLIDE:
			SlideT();
			break;
		case T_MOVE_TO_RIGHT:
			MoveToRightT(dt);
			break;
		}

	}
}



void GUIElement::FlashSA()
{
	currentStaticAnimation = SAT_NONE;
}
void GUIElement::ShakeSA()
{
	currentStaticAnimation = SAT_NONE;
}
void GUIElement::PulseSA()
{
	currentStaticAnimation = SAT_NONE;
}
void GUIElement::BounceSA()
{
	currentStaticAnimation = SAT_NONE;
}

void GUIElement::ScaleT()
{
	currentTransition = SAT_NONE;
}
void GUIElement::FadeT()
{
	currentTransition = SAT_NONE;
}
void GUIElement::DropT()
{
	currentTransition = SAT_NONE;
}
void GUIElement::FlyT()
{
	currentTransition = SAT_NONE;
}
void GUIElement::SlideT()
{
	currentTransition = SAT_NONE;
}
void GUIElement::MoveToRightT(float dt)
{
	static int screenRXBorderPos = app->win->GetWindowSize().x;

	int dif = screenRXBorderPos - (drawRect.x + rect.w);

	if (dif <= 0)
	{
		currentTransition = SAT_NONE;
		drawRect.x = rect.x;
		status.active = false;
		return;
	}

	float speed = 600 * dt;
	SetDrawPosition(drawRect.x += speed, drawRect.y);
	
}