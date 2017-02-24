#include "M_GUI.h"
#include "GUIElement.h"
#include "GUIImage.h"
#include "App.h"
#include "M_Input.h"
#include "M_Render.h"

M_GUI::M_GUI() : Module()
{
	name.create("GUI");
	active = true;
}
M_GUI::~M_GUI()
{
}

void M_GUI::Init()
{
	
}

bool M_GUI::Awake(pugi::xml_node &)
{
	GUIImage* img = new GUIImage();
	img->SetRectangle(20, 20, 100, 30);
	img->SetInteractive(true);
	img->SetCanFocus(true);

	guiList.push_back(img);
	return true;
}



bool M_GUI::PreUpdate()
{
	bool ret = true;	

	ManageEvents();
	
	// TODO do Update()
	std::list<GUIElement*>::iterator it;
	for (it = guiList.begin(); it != guiList.end(); it++)
	{
		(*it)->Update(mouseHover, focus);
	}
	for (it = debugGuiList.begin(); it != debugGuiList.end(); it++)
	{
		(*it)->Update(mouseHover, focus);
	}
	return ret;
}
bool M_GUI::Update(float dt)
{
	bool ret = true;

	return ret;
}
bool M_GUI::PostUpdate()
{
	DrawDebug();
	return true;
}
bool M_GUI::UpdateGuiList()
{
	return true;
}
bool M_GUI::UpdateDebugGuiList()
{
	return true;
}
//Checks if cursor is inside an element | returns null if anything found
GUIElement * M_GUI::FindMouseHover()
{
	GUIElement* ret = nullptr;
	std::list<GUIElement*>::reverse_iterator it;

	for (it = debugGuiList.rbegin(); it != debugGuiList.rend(); it++)
	{
		if ((*it)->CheckMouseOver())
		{
			ret = (*it);
		}
	}
	for (it = guiList.rbegin(); it != guiList.rend(); it++)
	{
		if ((*it)->CheckMouseOver())
		{
			ret = (*it);
		}
	}	

	return ret;
}
//Manages the events on hover and focus
void M_GUI::ManageEvents()
{
	std::list<GUIElement*>::iterator it;
	GUIElement* newMouseHover = nullptr;

	//Find the element that is hovered actually
	newMouseHover = FindMouseHover();
	//If the hover is null it gets the new element to hover
	if (mouseHover == nullptr && newMouseHover != nullptr)
	{
		mouseHover = newMouseHover;
		mouseHover->SetMouseInside(true);
		BroadcastEventToListeners(mouseHover, mouse_enters);
	}
	//If the hovered elements are diferent events ant status are managed here
	if (mouseHover != newMouseHover && newMouseHover != nullptr)
	{
		//Send leaving event
		BroadcastEventToListeners(mouseHover, mouse_leaves);
		//Set the new hover
		mouseHover->SetMouseInside(false);
		mouseHover = newMouseHover;
		mouseHover->SetMouseInside(true);
		//send entering event
		BroadcastEventToListeners(mouseHover, mouse_enters);
	}
	if (newMouseHover == nullptr && mouseHover != nullptr) //This is maybe unnecessary, but i think this check here helps to a better readability
	{
		BroadcastEventToListeners(mouseHover, mouse_leaves);
		mouseHover->SetMouseInside(false);
		mouseHover = nullptr;				
	}
	// TODO trobar quin element te el focus
	// TODO manage the input & events
	if (mouseHover != nullptr && mouseHover->GetCanFocus() == true && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_state::KEY_DOWN)
	{
		if (focus != mouseHover && focus != nullptr)
		{
			BroadcastEventToListeners(focus, lost_focus);
		}
		focus = mouseHover;
		BroadcastEventToListeners(mouseHover, mouse_lclick_down);
		BroadcastEventToListeners(mouseHover, gain_focus);
	}
	if (focus != nullptr && mouseHover != nullptr && mouseHover->GetCanFocus() == true && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_state::KEY_UP)
	{
		BroadcastEventToListeners(mouseHover, mouse_lclick_up);
	}
	if (focus != nullptr && mouseHover != nullptr && mouseHover->GetCanFocus() == true && app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_state::KEY_DOWN)
	{
		if (focus != mouseHover)
		{
			BroadcastEventToListeners(focus, lost_focus);
		}
		focus = mouseHover;
		BroadcastEventToListeners(mouseHover, mouse_rclick_down);
		BroadcastEventToListeners(mouseHover, gain_focus);
	}
	if (focus != nullptr && mouseHover != nullptr && mouseHover->GetCanFocus() == true && app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_state::KEY_UP)
	{
		BroadcastEventToListeners(mouseHover, mouse_rclick_up);
	}
}
//Broadcast an event to all GUIElement listeners
void M_GUI::BroadcastEventToListeners(GUIElement * element, GuiEvents event)
{
	if (event != mouse_enters)
		SDL_Log("Event: %d", event);
	//First we get listeners list of previous element hovered
	std::list<Module*> tmpListeners = element->GetListeners();
	//Iterate over listeners list to send them hover is lost
	for (std::list<Module*>::iterator it = tmpListeners.begin(); it != tmpListeners.end(); it++)
	{
		(*it)->GuiEvent(element, event);		
	}
}
void M_GUI::DrawDebug()
{
	for (std::list<GUIElement*>::iterator it = guiList.begin(); it != guiList.end(); it++)
	{
		rectangle rect = (*it)->GetRectangle();
		app->render->DrawQuad({ rect.x, rect.y, rect.w, rect.h }, 0, 255, 0, 255, false, false);
	}
	
}

