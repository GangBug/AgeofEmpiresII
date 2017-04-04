#include "GUIButton.h"
#include "j1Render.h"
#include "j1Gui.h"

GUIButton::GUIButton(GB_Rectangle<int> _position,
	GB_Rectangle<int> _standBySection,
	GB_Rectangle<int> _hoverSection,
	GB_Rectangle<int> _clickedSection,
	std::string name,
	int flags,
	char* text, label_size _size) : GUIElement(name, flags)
{
	SetType(GUI_BUTTON);
	//SetRectangle(0, 100, 231, 71);
	SetRectangle(_position);
	clickedSection = _clickedSection;
	hoverSection = _hoverSection;
	standBySection = _standBySection;
	SetInteractive(true);
	SetCanFocus(true);

	image = new GUIImage(GetName());
	image->SetParent(this);
	image->SetRectangle(_position);
	image->SetInteractive(false);
	image->SetCanFocus(false);
	image->SetSection(standBySection);

	label = new GUILabel(GetName());
	label->SetParent(this);

	label->SetInteractive(false);
	label->SetCanFocus(false);

	SetGlobalPos(_position.x, _position.y);

	if (text != nullptr)
	{
		label->SetText(text, _size);
	}
	label->SetLocalPos(_position.x-50, _position.y);
	//label->Center();

}
GUIButton::GUIButton(const GUIButton & btn, std::string name, int flags) : GUIElement(name, flags)
{
	SetType(GUI_BUTTON);
	SetRectangle(btn.GetLocalRect());
	standBySection = btn.standBySection;
	hoverSection = btn.hoverSection;
	clickedSection = btn.clickedSection;
	image = new GUIImage(*btn.image);
	image->SetParent(this);
	image->SetRectangle(btn.image->GetLocalRect());
	label = new GUILabel(*btn.label);
	label->SetParent(this);
	label->SetRectangle(btn.label->GetLocalRect());
	//label->Center();

}
GUIButton::~GUIButton()
{
}
void GUIButton::OnUpdate(const GUIElement * mouseHover, const GUIElement * focus, float dt)
{
	if (GetElementStatus().active)
	{
		if (GetElementStatus().statusChanged)
		{
			if (GetElementStatus().lClicked || GetElementStatus().rClicked)
			{
				image->SetSection(clickedSection);
			}
			else if (mouseHover == this)
			{
				image->SetSection(hoverSection);
			}
			else
			{
				image->SetSection(standBySection);
			}
		}

		//This goes the last 4ever
		SetStatusChanged(false);
	}
}
void GUIButton::Draw() const
{
	image->Draw();
	label->Draw();
}

void GUIButton::Serialize(pugi::xml_node root)
{
	pugi::xml_attribute atr;
	pugi::xml_node position;
	pugi::xml_node size;
	pugi::xml_node n_events;
	pugi::xml_node n_event;
	pugi::xml_node n_listeners;
	pugi::xml_node n_listener;
	pugi::xml_node element;

	element = root.append_child("button");
	//Create atributes in button
	atr = element.append_attribute("type");
	atr.set_value(GetPresetType().c_str());
	atr = element.append_attribute("name");
	atr.set_value(GetName().c_str());
	atr = element.append_attribute("text");
	atr.set_value(label->GetText().c_str());
	//Create node events
	n_events = element.append_child("events");
	if (GetElementStatus().onLClickUp & ~EVENT_NONE)
	{
		n_event = n_events.append_child("event");
		atr = n_event.append_attribute("action");
		atr.set_value(GetElementStatus().onLClickUp);
		atr = n_event.append_attribute("origin");
		atr.set_value(MOUSE_LCLICK_UP);
	}
	//gui_events onLClickDown = EVENT_NONE;
	//gui_events onRClickUp = EVENT_NONE;
	//gui_events onRClickDown = EVENT_NONE;
	//gui_events onGainFocus = EVENT_NONE;
	//gui_events onLooseFocus = EVENT_NONE;
	//gui_events onMouseEnters = EVENT_NONE;
	//gui_events onMouseLeaves = EVENT_NONE;

	//Create Node Listeners
	n_listeners = element.append_child("listeners");
	for (std::list<j1Module*>::iterator it = listeners.begin(); it != listeners.end(); it++)
	{
		n_listener = n_listeners.append_child("event");
		atr = n_listener.append_attribute("name");
		atr.set_value((*it)->name.c_str());
	}
	//Create node button/position
	position = element.append_child("position");
	//Create atributes in button/position
	atr = position.append_attribute("x");
	atr.set_value(GetLocalRect().x*WINDOWSCALEINV);
	atr = position.append_attribute("y");
	atr.set_value(GetLocalRect().y*WINDOWSCALEINV);
	//Create node button/size
	size = element.append_child("size");
	//Create atributes in button/size
	atr = size.append_attribute("w");
	atr.set_value(GetLocalRect().w*WINDOWSCALEINV);
	atr = size.append_attribute("h");
	atr.set_value(GetLocalRect().h*WINDOWSCALEINV);
}

void GUIButton::Deserialize(pugi::xml_node layout_element)
{
	std::string text = layout_element.attribute("text").as_string("");
	GB_Rectangle<int> rect;
	rect.x = layout_element.child("position").attribute("x").as_int()*WINDOWSCALE;
	rect.y = layout_element.child("position").attribute("y").as_int()*WINDOWSCALE;
	rect.w = layout_element.child("size").attribute("w").as_int()*WINDOWSCALE;
	rect.h = layout_element.child("size").attribute("h").as_int()*WINDOWSCALE;
	SetRectangle(rect);
	image->SetRectangle(rect);
	label->Center();
	SetText(text);
	//Load button actions and assign them to an action
	//TODO: try to find a better system of assigning button actions to gui events
	for (pugi::xml_node it_event = layout_element.child("events").first_child(); it_event; )
	{
		pugi::xml_node next = it_event.next_sibling();
		if (it_event.attribute("origin").as_int(0) & MOUSE_LCLICK_UP)
		{
				SetOnLClickUp((gui_events)it_event.attribute("action").as_int(0));
		}
		else if (it_event.attribute("origin").as_int(0) & MOUSE_LCLICK_DOWN)
		{
				SetOnLClickUp((gui_events)it_event.attribute("action").as_int(0));
		}
		it_event = next;
	}
	//Load button listeners
	//TODO: try to find a better system of assigning button actions to gui events
	for (pugi::xml_node it_listener = layout_element.child("listeners").first_child(); it_listener; )
	{
		pugi::xml_node next = it_listener.next_sibling();
		if (j1Module* module = App->FindModule(it_listener.attribute("name").as_string("")))
		{
			AddListener(module);
		}
		it_listener = next;
	}


}

void GUIButton::SetText(std::string txt, label_size size)
{
	label->SetText(txt.c_str(), size);
}

