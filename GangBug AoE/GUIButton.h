#pragma once
#include "GUIElement.h"
#include "GUIImage.h"
#include "GUILabel.h"
#include "GB_Rectangle.h"

class GUIButton :
	public GUIElement
{
public:
	GUIButton(GB_Rectangle<int> _position, 
			  GB_Rectangle<int> _standBySection, 
			  GB_Rectangle<int> _hoverSection, 
			  GB_Rectangle<int> _clickedSection, 
			  char* text = nullptr, 
			  label_size _size = DEFAULT);
	~GUIButton();

	void Update(const GUIElement* mouseHover, const GUIElement* focus) override;
	void Draw() const override;
	GUIImage* image = nullptr;
	GUILabel* label = nullptr;

	GB_Rectangle<int> standBySection;
	GB_Rectangle<int> hoverSection;
	GB_Rectangle<int> clickedSection;
};

