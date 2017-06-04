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
	GUILabel(std::string name, int flags = STANDARD_PRESET, bool resize = false);
	GUILabel(const char* text, label_size _size, std::string name, bool resize = false, int flags = STANDARD_PRESET, SDL_Color color = { 255,255,255,255});

	virtual void OnUpdate(const GUIElement* mouseHover, const GUIElement* focus, float dt);

	virtual ~GUILabel();
	SDL_Texture* GetTexture() const;
	void Draw() const;
	void Serialize(pugi::xml_node root)override;
	void Deserialize(pugi::xml_node root)override;
	void SetText(const char* text, label_size _size);
	label_size GetLabelSize() const { return lbSize; }
	std::string GetText() const { return text; }
	SDL_Color GetColor() const { return color; }
	void SetResize(bool resize) { this->resize = resize; }

	void SetColor(SDL_Color _color) { color = _color; std::string str = text; SetText("   ", lbSize); SetText(str.c_str(), lbSize); }
	void FollowScreen(bool _followScreen) { _followScreen ? followScreen = true : followScreen = false; }
private:
	SDL_Texture* texture;
	std::string text;
	label_size lbSize;
	SDL_Color color;
	bool followScreen = true;
	bool resize;
};

