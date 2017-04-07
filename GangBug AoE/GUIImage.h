#pragma once
#include "GUIElement.h"

class GUIImage :
	public GUIElement
{
public:
	GUIImage(std::string name, int flags = STANDARD_PRESET);
	GUIImage(const GUIImage &img, std::string name, int flags = STANDARD_PRESET);
	virtual ~GUIImage();

	void OnUpdate(const GUIElement* mouseHover, const GUIElement* focus, float dt) override;
	void Draw() const;
	void Serialize(pugi::xml_node root)override;
	void Deserialize(pugi::xml_node root)override;

	GB_Rectangle<int> GetSection() const;
	void SetSection(GB_Rectangle<int> _section);
	void SetSection(int _sectionX, int _sectionY, int _sectionW, int _sectionH);

private:
	GB_Rectangle<int> section;
	SDL_Texture* atlas;
};

