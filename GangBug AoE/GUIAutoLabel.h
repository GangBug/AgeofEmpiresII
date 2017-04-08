#pragma once
#include "GUILabel.h"
#include "GB_Rectangle.h"
#include <string>

//WARNING Use this label with caution
template <class t>
class GUIAutoLabel : public GUILabel
{
public:
	GUIAutoLabel(GB_Rectangle<int> position, t * _watcher, std::string name, int flags = STANDARD_PRESET) : GUILabel(name)
	{
		watchingVar = _watcher;
		SetRectangle(position);
	}
	~GUIAutoLabel()
	{
	}

	void OnUpdate(const GUIElement * mouseHover, const GUIElement * focus, float dt) override
	{
		std::string str = std::to_string(*watchingVar);
		SetText(str.c_str(), MEDIUM);
	}

	t* watchingVar;


};

