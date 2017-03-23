#include "GUILabel.h"
#include "GUIElement.h"
#include "M_Textures.h"
#include "M_Fonts.h"
#include "M_GUI.h"
#include "App.h"

//TEMP
#include "M_Render.h"

GUILabel::GUILabel() : GUIElement()
{
	SetType(GUI_LABEL);
	texture = nullptr;
}
GUILabel::GUILabel(const char * text, label_size _size) : GUIElement()
{
	SetText(text, _size);
	SetType(gui_types::GUI_LABEL);
}

GUILabel::~GUILabel()
{
	if (texture != nullptr && texture != app->gui->GetAtlas())
		app->tex->UnLoad(texture);
}

void GUILabel::SetText(const char* text, label_size _size)
{
	if (texture != nullptr)
		SDL_DestroyTexture(texture);

	this->text = text;
	switch (_size)
	{
		case DEFAULT:
			texture = app->font->Print(text, app->font->defaultFont);
			break;
		case MEDIUM:
			texture = app->font->Print(text, app->font->mediumFont);
			break;
		case SMALL:
			texture = app->font->Print(text, app->font->smallFont);
			break;
		default:
			break;
	}
	
	int w, h;
	app->tex->GetSize(texture, (uint&)w, (uint&)h);
	SetSize(w, h);
}

const SDL_Texture * GUILabel::GetTexture() const
{
	return texture;
}

void GUILabel::Draw() const
{
	if (texture != nullptr)
	{
		GB_Rectangle<int> rect = GetLocalRect();
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
		app->render->Blit(texture, rect.x, rect.y, NULL, 0.0f);
	}
}
