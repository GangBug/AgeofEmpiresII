// ----------------------------------------------------
// Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __MODULE_H__
#define __MODULE_H__

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"
#include "Defs.h"

class App;
class GUIElement;
enum gui_events;

class Module
{
public:

	Module(bool startEnabled) : active(startEnabled)
	{}

	virtual ~Module()
	{}

	void Enable()
	{
		active = true;
	}

	void Disable()
	{
		active = false;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	// Called each loop iteration
	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	// Called each loop iteration
	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void GuiEvent(GUIElement* element, gui_events event) {

	}

	virtual void DrawDebug() = 0;
public:

	p2SString	name;
	bool		active;

};

#endif // __MODULE_H__