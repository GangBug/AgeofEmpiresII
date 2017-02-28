#pragma once
#include "GUIElement.h"
class GUILabel :
	public GUIElement
{
public:
	GUILabel();
	GUILabel(const char* text);
	virtual ~GUILabel();

	void SetText(const char* text);
	const SDL_Texture* GetTexture() const;
	void Draw() const;

	SDL_Texture* texture;
	std::string text;
};

