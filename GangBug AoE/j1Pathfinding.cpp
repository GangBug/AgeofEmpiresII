#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include <algorithm>

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

//TODO change 
// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
std::list<PathNode>::iterator* PathList::Find(const iPoint& point)
{
	std::list<PathNode>::iterator item = list.begin();
	while(item != list.end())
	{
		if((*item).pos == point)
			return &item;
		item++;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
std::list<PathNode>::iterator* PathList::GetNodeLowestScore()
{
	std::list<PathNode>::iterator* ret = NULL;
	int min = 65535;

	for(std::list<PathNode>::reverse_iterator item = list.rbegin; item != list.rend(); item++)
	{
		if((*item).Score() < min)
		{
			min = (*item).Score();
			ret = &item.base();
		}
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.size();

	// north
	cell.create(pos.x, pos.y + 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	return list_to_fill.list.size();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	if (IsWalkable(origin) && IsWalkable(destination)) {
		last_path.Clear();
		PathList open;
		PathList close;
		PathList adjacents;
		PathNode pathorigin(0, origin.DistanceManhattan(destination), origin, NULL);
		int index = 0;
		open.list.push_back(pathorigin);

		while (open.list.size() != 0) {
			close.list.push_back(*(*open.GetNodeLowestScore()));
			open.list.erase(*open.GetNodeLowestScore());
			if (close.list.end()->pos == destination) {
				for (const PathNode* backtrack = close.list.end()->parent; backtrack; backtrack = backtrack->parent) {
					last_path.PushBack(backtrack->pos);
				}
				last_path.Flip();
				return index;
			}
			uint adjnum = close.list.end()->FindWalkableAdjacents(adjacents);
			index++;
			std::list<PathNode>::iterator it = adjacents.list.begin();
			for (int i = 0; i < adjnum; i++, ++it) {
				if (close.Find(it._Ptr->_Myval.pos) != NULL) {
					continue;
				}
				if (open.Find(it._Ptr->_Myval.pos) == NULL) {
					it._Ptr->_Myval.parent = &close.list.end()._Ptr->_Myval;
					it._Ptr->_Myval.h = it._Ptr->_Myval.pos.DistanceManhattan(destination);
					it._Ptr->_Myval.g = close.list.end()->g + 1;
					open.list.push_back(it._Ptr->_Myval);
				}
				else {
					std::list<PathNode>::iterator tocompare = *open.Find(it._Ptr->_Myval.pos);
					if (tocompare._Ptr->_Myval.g > it._Ptr->_Myval.g) {
						tocompare._Ptr->_Myval.parent = &close.list.end()._Ptr->_Myval;
					}
				}
			}
		}
	}
	else {
		return -1;
	}
}

