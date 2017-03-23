#include "GUIImage.h"
#include "M_Render.h"
#include "M_GUI.h"

GUIImage::GUIImage()
{
	SetType(GUI_IMAGE);
	atlas = app->gui->GetAtlas();
	//texture = nullptr;
}


GUIImage::~GUIImage()
{
}

void GUIImage::Draw() const
{
	app->render->Blit(atlas, GetLocalRect().x, GetLocalRect().y, &GetSection().GetSDLrect());
}

GB_Rectangle<int> GUIImage::GetSection() const
{
	return section;
}

void GUIImage::SetSection(GB_Rectangle<int> _section)
{
	section = _section;
}

void GUIImage::SetSection(int _sectionX, int _sectionY, int _sectionW, int _sectionH)
{
	section.x = _sectionX;
	section.y = _sectionY;
	section.w = _sectionW;
	section.h = _sectionH;
}
