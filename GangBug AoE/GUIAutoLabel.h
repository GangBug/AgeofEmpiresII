#pragma once
#include "GUILabel.h"
#include "GB_Rectangle.h"
#include <string>

template <class t>
class GUIAutoLabel :
	public GUILabel
{
public:
	GUIAutoLabel(GB_Rectangle<int> position, t * _watcher)
	{
		watchingVar = _watcher;
		SetRectangle(position);
	}
	~GUIAutoLabel()
	{
	}
	void Update(const GUIElement * mouseHover, const GUIElement * focus)
	{
		std::string str = std::to_string(*watchingVar);
		SetText(str.c_str(), MEDIUM);
	}

	t* watchingVar;


};

