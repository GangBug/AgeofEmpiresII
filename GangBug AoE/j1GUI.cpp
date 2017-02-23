#include "j1GUI.h"
#include "GUIElement.h"
#include "j1App.h"
#include "j1Input.h"


j1GUI::j1GUI()
{
}


j1GUI::~j1GUI()
{
}

bool j1GUI::PreUpdate()
{
	bool ret = true;
	std::list<GUIElement*>::iterator it;
	GUIElement* mouseHover = nullptr;
	GUIElement* focus = nullptr;

	// TODO trobar sobre quin element esta el ratolí
	mouseHover = FindMouseHover();

	// TODO trobar quin element te el focus
	// TODO manage the input
	if (mouseHover && mouseHover->GetCanFocus() == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == j1KeyState::KEY_DOWN)
		focus = mouseHover;
	if (true)
	{

	}
	// TODO do Update()
	for (it = guiList.begin(); it != guiList.end(); it++)
	{
		(*it)->Update(mouseHover, focus);
	}

	return ret;
}

bool j1GUI::Update()
{
	bool ret = true;

	return ret;
}

bool j1GUI::PostUpdate()
{
	return true;
}

bool j1GUI::UpdateGuiList()
{
	return true;
}

bool j1GUI::UpdateDebug_guiList()
{
	return true;
}

GUIElement * j1GUI::FindMouseHover()
{
	GUIElement* ret = nullptr;
	std::list<GUIElement*>::reverse_iterator it;

	for (it = debug_guiList.rbegin(); it != debug_guiList.rend() && ret != nullptr; it++)
	{
		if ((*it)->CheckMouseOver())
		{
			ret = (*it);
		}
	}
	for (it = guiList.rbegin(); it != guiList.rend() && ret != nullptr; it++)
	{
		if ((*it)->CheckMouseOver())
		{
			ret = (*it);
		}
	}	

	return ret;
}

