#ifndef __M_MINIMAP_H__
#define __M_MINIMAP_H__

#include "Module.h"
#include "p2Point.h"
#include <vector>



struct minimap_tiles 
	{
	iPoint coords;
	int id;
};

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

	//Create Minimap terrain
	void CreateMinimap();

	//Update Minimap with units
	void DrawMinimap();

	void DrawDebug() override;

public:

	std::vector<minimap_tiles> mini_tiles;

	SDL_Texture* minimap_atlas;

	SDL_Rect green = { 0, 0, 4, 4 };
	SDL_Rect blue = { 4, 0, 4, 4 };
	SDL_Rect red = { 0, 4, 4, 4 };
	SDL_Rect yellow = { 4, 4, 4, 4 };

};


#endif //__MINIMAP_H__