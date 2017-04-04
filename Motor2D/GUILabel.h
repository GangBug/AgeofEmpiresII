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
	GUILabel(std::string name, int flags = STANDARD_PRESET);
	GUILabel(const char* text, label_size _size, std::string name, int flags = STANDARD_PRESET);
	virtual ~GUILabel();
	const SDL_Texture* GetTexture() const;
	void Draw() const;
	void Serialize(pugi::xml_node root)override;
	void Deserialize(pugi::xml_node root)override;
	void SetText(const char* text, label_size _size);
	void OnUpdate(const GUIElement* mouseHover, const GUIElement* focus, float dt) override;
	label_size GetLabelSize() const { return lbSize; }
	std::string GetText() const { return text; }
private:
	SDL_Texture* texture;
	std::string text;
	label_size lbSize;
	SDL_Color color;
};
