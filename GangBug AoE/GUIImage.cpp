#include "GUIImage.h"
#include "M_Render.h"
#include "M_GUI.h"

GUIImage::GUIImage()
{
	SetType(GUI_IMAGE);
	//texture = nullptr;
}


GUIImage::~GUIImage()
{
}

void GUIImage::Draw() const
{
	app->render->Blit(app->gui->GetAtlas(), 0, 0, &GetRectangle().GetSDLrect());
}
