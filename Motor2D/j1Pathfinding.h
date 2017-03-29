#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include <vector>
#include <queue>
#include <list>

#define DEFAULT_PATH_LENGTH 0
#define INVALID_WALK_CODE 255

// --------------------------------------------------
// Recommended reading:
// Intro: http://www.raywenderlich.com/4946/introduction-to-a-pathfinding
// Details: http://theory.stanford.edu/~amitp/GameProgramming/
// --------------------------------------------------
struct PathNode;

class j1PathFinding : public j1Module
{
public:

	j1PathFinding();

	// Destructor
	~j1PathFinding();

	// Called before quitting
	bool CleanUp();

	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);

	// Main function to request a path from A to B
	int CreatePath(const iPoint& origin, const iPoint& destination);

	int CreatePath(iPoint origin, iPoint destination, std::list<iPoint>& list);
	// To request all tiles involved in the last generated path
	const std::vector<iPoint>* GetLastPath() const;

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& pos) const;

	// Utility: return the walkability value of a tile
	uchar GetTileAt(const iPoint& pos) const;

	PathNode* GetPathNode(int x, int y);

private:

	// size of the map
	uint width;
	uint height;
	// all map walkability values [0..255]
	uchar* map;

	PathNode* node_map;

	// we store the created path here
	std::vector<iPoint> last_path;
};

// ---------------------------------------------------------------------
// Pathnode: Helper struct to represent a node in the path creation
// ---------------------------------------------------------------------
struct PathNode
{
	// Convenient constructors
	PathNode();
	PathNode(int g, int h, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	// Fills a list (PathList) of all valid adjacent pathnodes
	uint FindWalkableAdjacents(std::list<PathNode*>* list_to_fill) const;

	// Calculates this tile score
	float Score() const;
	// Calculate the F for a specific destination tile
	int CalculateF(const iPoint& destination);
	void SetPosition(iPoint& value);

	// -----------
	float g;
	int h;
	iPoint pos;
	bool operator ==(const PathNode& node)const;
	bool operator !=(const PathNode& node)const;
	bool on_close = false;
	bool on_open = false;
	const PathNode* parent; // needed to reconstruct the path in the end
};

struct compare
{
	bool operator()(const PathNode* l, const PathNode* r)
	{
		return l->Score() >= r->Score();
	}
};


#endif // __j1PATHFINDING_H__