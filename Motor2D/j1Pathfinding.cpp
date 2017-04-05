#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name.assign("pathfinding");
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

	last_path.clear();
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
	//RELEASE_ARRAY(node_map);
	node_map = new PathNode[width*height];

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
	else
	{
		return map[0];
	}

	//return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const std::vector<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
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
uint PathNode::FindWalkableAdjacents(std::list<PathNode*>* list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill->size();
	bool northClose = false, southClose = false, eastClose = false, westClose = false;
	// south
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	else
	{
		southClose = true;
	}
	// north
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	else
	{
		northClose = true;
	}
	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell) && cell.x != 25)
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	// south-east
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell) && southClose == false && eastClose == false)
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	// south-west
	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell) && southClose == false && westClose == false)
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	// north-east
	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell) && northClose == false && eastClose == false)
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	// north-west
	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell) && northClose == false && westClose == false)
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	return list_to_fill->size();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
float PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	if (parent->pos.DistanceManhattan(pos) == 2){
		g = parent->g + 11;
	}
	else if (parent->pos.DistanceManhattan(pos) == 4) {
		g = parent->g + 16;
	}
	else {
		g = parent->g + 10;
	}
	h = pos.DistanceManhattan(destination)*10;
	return  g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------

int j1PathFinding::CreatePath(iPoint origin, iPoint destination, std::list<iPoint>& list)
{
	int size = width*height;
	//TODO 2
	// Fill the node_map with with null PathNodes 
	std::fill(node_map, node_map + size, PathNode(-1, -1, iPoint(-1, -1), nullptr));
	int ret = -1;

	if (IsWalkable(origin) && IsWalkable(destination))
	{
		ret = 1;
		//TODO 3 
		//Create a priority_queue open that compares a PathNode* in a vector of PathNodes* using the compare struct
		std::priority_queue<PathNode*, std::vector<PathNode*>, compare> open;
		//TODO 5
		//Inicialize firstNode getting its node and setting its position, g and h
		PathNode* firstNode = GetPathNode(origin.x, origin.y);
		firstNode->SetPosition(origin);
		firstNode->g = 0;
		firstNode->h = origin.DistanceH(destination);

		open.push(firstNode);
		PathNode* current = nullptr;
		while (open.size() != 0)
		{
			//TODO 6 
			//Get the top of the queue as the current node, set it on_close and pop the top node.
			current = open.top();
			open.top()->on_close = true;
			open.pop();
			if (current->pos == destination)
			{

				std::vector<iPoint>* path = new std::vector<iPoint>;
				last_path.clear();
				//TODO 8
				// make a look for current, and until its parent is nullptr, make current = ParentNode
				for (; current->parent != nullptr; current = GetPathNode(current->parent->pos.x, current->parent->pos.y))
				{
					last_path.push_back(current->pos);
					list.push_front(current->pos);
				}
				list.push_front(current->pos);
				last_path.push_back(current->pos);
				;
				return last_path.size();
			}
			else
			{
				std::list<PathNode*> neighbours;
				current->FindWalkableAdjacents(&neighbours);
				for (std::list<PathNode*>::iterator item = neighbours.begin(); item != neighbours.end(); item++) {
					PathNode* temp = item._Mynode()->_Myval;
					//TODO 7
					//change if an else if for on_open, on_close booleans 
					if (temp->on_close == true)
					{
						continue;
					}
					else if (temp->on_open == true)
					{
						int last_g_value = temp->g;
						temp->CalculateF(destination);
						if (last_g_value < temp->g)
						{
							temp->parent = GetPathNode(current->pos.x, current->pos.y);
						}
						else {
							temp->g = last_g_value;
						}
					}
					else
					{
						temp->on_open = true;
						temp->CalculateF(destination);
						open.push(temp);
					}
				}

				neighbours.clear();
			}
		}
	}
	return -1;

}

PathNode* j1PathFinding::GetPathNode(int x, int y)
{
	return &node_map[(y*width) + x];
}

bool PathNode::operator==(const PathNode & node) const
{
	return pos == node.pos;
}

bool PathNode::operator!=(const PathNode & node) const
{
	return !operator==(node);
}

void PathNode::SetPosition(iPoint& value)
{
	pos = value;
}