#include "SDL\include\SDL.h"
#include "M_GUI.h"
#include "p2Point.h"
#include "GUIElement.h"
#include "GUIImage.h"

class GUIMouse :public GUIElement
{
public:
	GUIMouse(iPoint margin, GB_Rectangle<int> _section);
	~GUIMouse();

	void Draw() const override;
	void Update(const GUIElement* mouse_hover, const GUIElement* focus) override;


	GB_Rectangle<int> GUIMouse::GetSection() const;
	void SetSection(const GB_Rectangle<int> section);

private:
	GB_Rectangle<int> section;
	GUIImage* curs = nullptr;
	iPoint margin;
	iPoint position;
};
