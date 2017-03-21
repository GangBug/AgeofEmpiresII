#include "GUIMouse.h"
#include "App.h"
#include "M_Render.h"
#include "M_GUI.h"
#include "M_Input.h"
#include "M_Textures.h"
#include "j1Scene.h"


// class GuiMCursor ---------------------------------------------------
GUICursor::GUICursor(int margin_x, int margin_y) : GUIElement(), margin(margin_x, margin_y)
{
	section.x = section.y = 0;

	curs = app->gui->GetAtlas();
	app->tex->GetSize(curs, (uint&)section.w, (uint&)section.h);

	SetSize(section.w, section.h);
	iPoint p;
	app->input->GetMousePosition(p.x, p.y);
	SetLocalPos(p.x - margin.x, p.y - margin.y);


	SetType(GUI_MOUSE_CURSOR);
}

// --------------------------
GUICursor::GUICursor(const SDL_Rect& section, int margin_x, int margin_y) : GUIElement(), section(section), margin(margin_x, margin_y)
{
	curs = app->gui->GetAtlas();
	SetSize(section.w, section.h);
	iPoint p;
	app->input->GetMousePosition(p.x, p.y);
	SetLocalPos(p.x - margin.x, p.y - margin.y);
	SetType(GUI_MOUSE_CURSOR);
}

// --------------------------
GUICursor::~GUICursor()
{}

//---------------------------
SDL_Rect GUICursor::GetSection()const
{
	return section;
}

// --------------------------
void GUICursor::SetSection(const SDL_Rect& section)
{
	this->section = section;
}

// --------------------------
bool GUICursor::draw()
{
	SDL_ShowCursor(SDL_DISABLE);

	iPoint p;
	app->input->GetMousePosition(p.x, p.y);
	app->render->Blit(curs, p.x - margin.x, p.y - margin.y, (SDL_Rect*)&section, 0.0f);
	return true;
}

//----------------------------

SDL_Texture* GUICursor::GetTexture()const
{
	return curs;
}

//----------------------------

void GUICursor::Update(const GUIElement* mouse_hover, const GUIElement* focus)
{

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
