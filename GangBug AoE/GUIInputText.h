#pragma once
#include "GUIElement.h"
#include "App.h"
#include "M_Input.h"
#include "GUIImage.h"
#include "GUILabel.h"

class GUIInputText :
	public GUIElement
{
public:
	GUIInputText(std::string name, int flags = STANDARD_PRESET);
	virtual ~GUIInputText();

	void OnUpdate(const GUIElement* mouseHover, const GUIElement* focus, float dt) override;

	std::string text;
	uint cursorPosition;
	GUIImage* image;
	GUILabel* label;
};

