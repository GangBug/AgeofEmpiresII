#pragma once
#include "GUIElement.h"
class GUIBox :
	public GUIElement
{
public:
	GUIBox(std::string name, int flags);
	~GUIBox();
};

