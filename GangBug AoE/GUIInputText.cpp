#include "GUIInputText.h"



GUIInputText::GUIInputText(std::string name, int flags) : GUIElement(name, flags)
{
	SetType(GUI_INPUT_TEXT);
	SetRectangle(0, 200, 231, 71);
	SetInteractive(true);
	SetCanFocus(true);

	image = new GUIImage(GetName());
	image->SetParent(this);
	image->SetRectangle(0, 200, 231, 71);
	image->SetInteractive(true);
	image->SetCanFocus(true);
	image->SetSection(GB_Rectangle<int>(0, 110, 231, 71));

	label = new GUILabel(GetName());
	label->SetLocalPos(0, 200);
	label->SetRectangle(GB_Rectangle<int>(0, 200, 231, 71));
}


GUIInputText::~GUIInputText()
{
}

void GUIInputText::OnUpdate(const GUIElement * mouseHover, const GUIElement * focus, float dt)
{
	if (focus == this)
	{
		text += app->input->GetText();
	}
	label->SetText(text.c_str(), MEDIUM);
	if (label->GetLocalRect().w > GetLocalRect().w)
		label->SetRectangle(GetLocalRect());
	//label->SetRectangle(GB_Rectangle<int>(0, 200, 231, 71));
}


