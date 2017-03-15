#include "GUIButton.h"
#include "M_Render.h"
#include "M_GUI.h"

GUIButton::GUIButton(GB_Rectangle<int> _standBySection, GB_Rectangle<int> _hoverSection, GB_Rectangle<int> _clickedSection)
{
	SetType(GUI_BUTTON);
	SetRectangle(0, 100, 231, 71);
	clickedSection = _clickedSection;
	hoverSection = _hoverSection;
	standBySection = _standBySection;
	SetInteractive(true);
	SetCanFocus(true);

	image = new GUIImage();
	image->SetParent(this);
	image->SetRectangle(0, 100, 231, 71);	
	image->SetInteractive(true);
	image->SetCanFocus(true);
	image->SetSection(standBySection);
}
GUIButton::~GUIButton()
{
}
void GUIButton::Update(const GUIElement * mouseHover, const GUIElement * focus)
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
void GUIButton::Draw() const
{
	image->Draw();
}

