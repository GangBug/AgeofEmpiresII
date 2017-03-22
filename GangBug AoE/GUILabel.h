#pragma once
#include "GUIElement.h"

enum size
{
	DEFAULT,
	MEDIUM,
	SMALL
};

class GUILabel :
	public GUIElement
{
public:
	GUILabel();
	GUILabel(const char* text, size _size);
	virtual ~GUILabel();

	void SetText(const char* text, size _size);
	const SDL_Texture* GetTexture() const;
	void Draw() const;

	SDL_Texture* texture;
	std::string text;
};

