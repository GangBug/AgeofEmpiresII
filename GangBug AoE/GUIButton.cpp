#include "GUIButton.h"
#include "M_Render.h"
#include "M_GUI.h"

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
	image->SetVisible(true);
	image->SetSection(standBySection);

	label = new GUILabel(GetName());
	label->SetParent(this);
	//label->SetLocalPos(_position.x, _position.y);
	label->SetInteractive(false);
	label->SetCanFocus(false);
	label->SetVisible(true);

	SetGlobalPos(_position.x, _position.y);

	if (text != nullptr)
	{
		label->SetText(text, _size);
	}
	label->Center();
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
	//label->SetRectangle(btn.label->GetLocalRect());
	label->Center();

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
	if(GetVisible())
	{
		image->Draw();
		label->Draw();
	}
	
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

	GB_Rectangle<float> xmlRect = app->gui->ScreenToXml(GetLocalRect());

	element = root.append_child("button");
	//Create atributes in button
	atr = element.append_attribute("type");
	atr.set_value(GetPresetType().c_str());
	atr = element.append_attribute("name");
	atr.set_value(GetName().c_str());
	atr = element.append_attribute("text");
	atr.set_value(label->GetText().c_str());
	//atr = element.append_attribute("draggable");
	//atr.set_value(GetDraggable());
	atr = element.append_attribute("interactive");
	atr.set_value(GetInteractive());
	atr = element.append_attribute("canFocus");
	atr.set_value(GetCanFocus());
	atr = element.append_attribute("active");
	atr.set_value(GetActive());
	atr = element.append_attribute("visible");
	atr.set_value(GetVisible());
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
	for (std::list<Module*>::iterator it = listeners.begin(); it != listeners.end(); it++)
	{
		n_listener = n_listeners.append_child("event");
		atr = n_listener.append_attribute("name");
		atr.set_value((*it)->name.c_str());
	}
	//Create Node Scenes
	n_listeners = element.append_child("scenes");
	for (std::map<std::string, Module*>::iterator it = scenes.begin(); it != scenes.end(); it++)
	{
		n_listener = n_listeners.append_child("scene");
		atr = n_listener.append_attribute("name");
		atr.set_value((it->first.c_str()));
	}
	
	//Create node button/position
	position = element.append_child("position");
	//Create atributes in button/position
	atr = position.append_attribute("x");
	atr.set_value(xmlRect.x);
	atr = position.append_attribute("y");
	atr.set_value(xmlRect.y);
	//Create node button/size
	size = element.append_child("size");
	//Create atributes in button/size
	atr = size.append_attribute("w");
	atr.set_value(xmlRect.w);
	atr = size.append_attribute("h");
	atr.set_value(xmlRect.h);
}

void GUIButton::Deserialize(pugi::xml_node layout_element)
{
	std::string text = layout_element.attribute("text").as_string("");
	SetActive(layout_element.attribute("active").as_bool(false));
	//SetDraggable(layout_element.attribute("draggable").as_bool(false));
	SetInteractive(layout_element.attribute("interactive").as_bool(false));
	SetCanFocus(layout_element.attribute("canFocus").as_bool(false));
	SetVisible(layout_element.attribute("visible").as_bool(false));
	GB_Rectangle<float> xmlRect;
	GB_Rectangle<int>	rect;
	xmlRect.x = layout_element.child("position").attribute("x").as_float();
	xmlRect.y = layout_element.child("position").attribute("y").as_float();
	xmlRect.w = layout_element.child("size").attribute("w").as_float();
	xmlRect.h = layout_element.child("size").attribute("h").as_float();

	rect = app->gui->XmlToScreen(xmlRect);
	
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
			if (it_event.attribute("action").as_int(0) & CLOSE_APP)
				SetOnLClickUp((gui_events)it_event.attribute("action").as_int(0));
		}
		else if (it_event.attribute("origin").as_int(0) & MOUSE_LCLICK_DOWN)
		{
			if (it_event.attribute("action").as_int(0) & CLOSE_APP)
				SetOnLClickUp((gui_events)it_event.attribute("action").as_int(0));
		}
		it_event = next;
	}
	//Load button listeners
	//TODO: try to find a better system of assigning button actions to gui events
	for (pugi::xml_node it_listener = layout_element.child("listeners").first_child(); it_listener; )
	{
		pugi::xml_node next = it_listener.next_sibling();
		if (Module* module = app->FindModule(it_listener.attribute("name").as_string("")))
		{
			AddListener(module);
		}
		it_listener = next;
	}
	//Load scenes
	for (pugi::xml_node it = layout_element.child("scenes").first_child(); it; )
	{
		pugi::xml_node next = it.next_sibling();
		if (Module* module = app->FindModule(it.attribute("name").as_string("")))
		{
			AddScene(module);
		}
		it = next;
	}
	
}

void GUIButton::SetText(std::string txt, label_size size)
{
	label->SetText(txt.c_str(), size);
}

