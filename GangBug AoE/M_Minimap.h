#ifndef __M_MINIMAP_H__
#define __M_MINIMAP_H__

#include "Module.h"
#include "p2Point.h"
#include <vector>

class M_Minimap : public Module
{
public:

	M_Minimap(bool startEnabled = true);

	//Destructor
	virtual ~M_Minimap();

	//Called before render is available
	bool Awake(pugi::xml_node&);

	//Called before quitting
	bool CleanUp();

	//Update Minimap with units
	void DrawMinimap();

	void DrawDebug() override;

public:
	SDL_Texture* minimap_atlas;

	SDL_Rect red = { 0, 0, 4, 4 };
	SDL_Rect green = { 4, 0, 4, 4 };

};


#endif //__MINIMAP_H__