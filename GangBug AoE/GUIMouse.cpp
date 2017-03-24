#include "GUIMouse.h"
#include "App.h"
#include "M_Render.h"
#include "M_GUI.h"
#include "M_Input.h"
#include "M_Textures.h"
#include "j1Scene.h"


// class GuiMCursor ---------------------------------------------------
GUIMouse::GUIMouse(iPoint margin, GB_Rectangle<int> _section, int flags) : GUIElement(flags), margin(margin)
{
	section = _section;
	
	SetType(GUI_MOUSE_CURSOR);
}

// --------------------------
GUIMouse::~GUIMouse()
{}

//---------------------------
GB_Rectangle<int> GUIMouse::GetSection()const
{
	return section;
}

// --------------------------
void GUIMouse::SetSection(const GB_Rectangle<int> section)
{
	this->section = section;
}

// --------------------------
void GUIMouse::Draw() const
{
	curs->Draw();
}

//----------------------------

void GUIMouse::OnUpdate(const GUIElement* mouse_hover, const GUIElement* focus)
{
	SDL_ShowCursor(SDL_DISABLE);
	app->input->GetMousePosition(position.x, position.y);
	curs->SetLocalPos(position.x, position.y);
	//if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	//	app->gui->ManageEvents();
	//	app->scene->behaviour(this, UIEvents::mouse_lclick_down);

	//if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	//	app->scene->behaviour(this, UIEvents::mouse_lclick_up);

	//if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	//	app->scene->behaviour(this, UIEvents::mouse_rclick_down);

	//if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
	//	app->scene->behaviour(this, UIEvents::mouse_lclick_up);

}
