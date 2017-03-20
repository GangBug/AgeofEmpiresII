#pragma once
#include "GUIElement.h"
#include "GUIButton.h"
#include "GUILabel.h"
#include "j1Timer.h"

class GUIInputBox :
	public GUIElement
{
public:
	GUIInputBox();
	~GUIInputBox();

private:
	GUILabel label;
	std::string text;
	uint cursorPosition;
	bool inputEnabled;
	bool hideText = false;
	uint maxCharacters = 28;

	//Methods
	void Update(const GUIElement* mouseHover, const GUIElement* focus) override; // must implement dt
	void Draw() const override;
	void DebugDraw() const override;

	void ManageWritting();
	void ManageCursor();
};

