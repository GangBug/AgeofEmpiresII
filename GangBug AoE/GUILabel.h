#pragma once
#include "GUIElement.h"

enum label_size
{
	DEFAULT,
	MEDIUM,
	SMALL
};

class GUILabel :
	public GUIElement
{
public:
	GUILabel(int flags = STANDARD_PRESET);
	GUILabel(const char* text, label_size _size, int flags = STANDARD_PRESET);
	virtual ~GUILabel();	
	const SDL_Texture* GetTexture() const;
	void Draw() const;
	void SetText(const char* text, label_size _size);
	label_size GetLabelSize() const { return lbSize; }
	std::string GetText() const { return text; }
private:
	SDL_Texture* texture;
	std::string text;
	label_size lbSize;
};

