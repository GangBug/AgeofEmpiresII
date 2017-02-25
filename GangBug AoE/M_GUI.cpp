#include "M_GUI.h"
#include "GUIElement.h"
#include "GUIImage.h"
#include "App.h"
#include "M_Input.h"
#include "M_Render.h"

M_GUI::M_GUI(bool startEnabled) : Module(startEnabled)
{
	name.create("GUI");
	active = true;
}
M_GUI::~M_GUI()
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



update_status M_GUI::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

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
update_status M_GUI::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}
update_status M_GUI::PostUpdate(float dt)
{
	DrawDebug();
	return UPDATE_CONTINUE;
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
		BroadcastEventToListeners(mouseHover, MOUSE_ENTERS);
	}
	//If the hovered elements are diferent events ant status are managed here
	if (mouseHover != newMouseHover && newMouseHover != nullptr)
	{
		//Send leaving event
		BroadcastEventToListeners(mouseHover, MOUSE_LEAVES);
		//Set the new hover
		mouseHover->SetMouseInside(false);
		mouseHover = newMouseHover;
		mouseHover->SetMouseInside(true);
		//send entering event
		BroadcastEventToListeners(mouseHover, MOUSE_ENTERS);
	}
	if (newMouseHover == nullptr && mouseHover != nullptr) //This is maybe unnecessary, but i think this check here helps to a better readability
	{
		BroadcastEventToListeners(mouseHover, MOUSE_LEAVES);
		mouseHover->SetMouseInside(false);
		mouseHover = nullptr;				
	}
	// TODO trobar quin element te el focus
	// TODO manage the input & events
	if (mouseHover != nullptr && mouseHover->GetCanFocus() == true && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_state::KEY_DOWN)
	{
		if (focus != mouseHover && focus != nullptr)
		{
			BroadcastEventToListeners(focus, LOST_FOUCS);
		}
		focus = mouseHover;
		BroadcastEventToListeners(mouseHover, MOUSE_LCLICK_DOWN);
		BroadcastEventToListeners(mouseHover, GAIN_FOCUS);
	}
	if (focus != nullptr && mouseHover != nullptr && mouseHover->GetCanFocus() == true && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_state::KEY_UP)
	{
		BroadcastEventToListeners(mouseHover, MOUSE_LCLICK_UP);
	}
	if (focus != nullptr && mouseHover != nullptr && mouseHover->GetCanFocus() == true && app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_state::KEY_DOWN)
	{
		if (focus != mouseHover)
		{
			BroadcastEventToListeners(focus, LOST_FOUCS);
		}
		focus = mouseHover;
		BroadcastEventToListeners(mouseHover, MOUSE_RCLICK_DOWN);
		BroadcastEventToListeners(mouseHover, GAIN_FOCUS);
	}
	if (focus != nullptr && mouseHover != nullptr && mouseHover->GetCanFocus() == true && app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_state::KEY_UP)
	{
		BroadcastEventToListeners(mouseHover, MOUSE_RCLICK_UP);
	}
}
//Broadcast an event to all GUIElement listeners
void M_GUI::BroadcastEventToListeners(GUIElement * element, gui_events event)
{
	if (event != MOUSE_ENTERS)
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

