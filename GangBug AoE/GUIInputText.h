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
	GUIInputText();
	virtual ~GUIInputText();

	void Update(const GUIElement* mouseHover, const GUIElement* focus) override;

	std::string text;
	uint cursorPosition;
	GUIImage* image;
	GUILabel* label;
};

