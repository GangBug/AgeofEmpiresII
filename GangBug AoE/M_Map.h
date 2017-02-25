#ifndef __M_MAP_H__
#define __M_MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "Module.h"
#include <list>
#include "SDL\include\SDL.h"

// ----------------------------------------------------
struct Properties
{
	struct Property
	{
		std::string name;
		int value;
	};

	~Properties()
	{
		std::list<Property*>::iterator item;
		item = list.begin();

		while(item != list.end())
		{
			RELEASE(item._Ptr->_Myval);
			item++;
		}

		list.clear();
	}

	int Get(const char* name, int defaultValue = 0) const;

	std::list<Property*>	list;
};

// ----------------------------------------------------
struct MapLayer
{
	std::string	name;
	int			width;
	int			height;
	uint*		data;
	Properties	properties;

	MapLayer() : data(nullptr)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tileWidth;
	int					tileHeight;
	SDL_Texture*		texture;
	int					texWidth;
	int					texHeight;
	int					numTilesWidth;
	int					numTilesHeight;
	int					offsetX;
	int					offsetY;
};

enum map_types
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tileWidth;
	int					tileHeight;
	SDL_Color			backgroundColor;
	map_types			type;
	std::list<TileSet*>	tilesets;
	std::list<MapLayer*>	layers;
};

// ----------------------------------------------------
class M_Map : public Module
{
public:

	M_Map(bool startEnabled = true);

	// Destructor
	virtual ~M_Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf)override;

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp()override;

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	void DrawDebug()override;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tilesetNode, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tilesetNode, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;

private:

	pugi::xml_document	mapFile;
	std::string			folder;
	bool				mapLoaded;
};

#endif // __M_MAP_H__