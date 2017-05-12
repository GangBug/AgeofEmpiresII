#include "Log.h"
#include "App.h"
#include "M_PathFinding.h"
#include <algorithm>

#include "Defs.h"
#include "App.h"
#include "M_Pathfinding.h"
#include "M_Render.h"
#include "M_Input.h"
#include "M_Map.h"

M_Pathfinding::M_Pathfinding(bool startEnabled) : Module(startEnabled), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.assign("pathfinding");
}

// Destructor
M_Pathfinding::~M_Pathfinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool M_Pathfinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.clear();
	if (map != nullptr) RELEASE_ARRAY(map);
	if (node_map != nullptr) RELEASE_ARRAY(node_map);
	if (constructible_map_ally != nullptr) RELEASE_ARRAY(constructible_map_ally);
	if (constructible_map_neutral != nullptr) RELEASE_ARRAY(constructible_map_neutral);
	return true;
}

void M_Pathfinding::CleanUpJPS()
{
	for (std::vector<PathNode*>::iterator i = visited.begin(); i != visited.end(); ++i)
		DELETE_PTR(*i)

		visited.clear();
	frontier.clear();
}

// Sets up the walkability map
void M_Pathfinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	//TODO1
	//create a node_map
	//RELEASE_ARRAY(node_map);
	node_map = new PathNode[width*height];

	memcpy(map, data, width*height);
}

void M_Pathfinding::SetConstructibleMaps(uint width, uint height, uchar* data, uchar* data2)
{
	constructible_map_ally = new uchar[width*height];
	memcpy(constructible_map_ally, data, width*height);

	constructible_map_neutral = new uchar[width*height];
	memcpy(constructible_map_neutral, data2, width*height);

}

// Utility: return true if pos is inside the map boundaries
bool M_Pathfinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool M_Pathfinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

bool M_Pathfinding::IsConstructible_ally(const iPoint& pos) const
{
	uchar t = GetTileAtConstructible_ally(pos);
	return t != INVALID_WALK_CODE && t > 1;
}

bool M_Pathfinding::IsConstructible_neutral(const iPoint& pos) const
{
	uchar t = GetTileAtConstructible_neutral(pos);
	return t != INVALID_WALK_CODE && t > 1;
}

void M_Pathfinding::MakeNoConstruible_ally(const iPoint& pos)
{
	constructible_map_ally[pos.y*width + pos.x] = INVALID_WALK_CODE;
}

void M_Pathfinding::MakeNoConstruible_neutral(const iPoint& pos)
{
	constructible_map_neutral[pos.y*width + pos.x] = INVALID_WALK_CODE;
}

void M_Pathfinding::MakeConstruible_neutral(const iPoint& pos)
{
	constructible_map_neutral[pos.y*width + pos.x] = 10;
}

void M_Pathfinding::MakeConstruible_ally(const iPoint& pos)
{
	constructible_map_ally[pos.y*width + pos.x] = 10;
}

void M_Pathfinding::MakeNoWalkable(const iPoint& pos)
{
	map[pos.y*width + pos.x] = INVALID_WALK_CODE;
}

void M_Pathfinding::MakeWalkable(const iPoint& pos)
{
	map[pos.y*width + pos.x] = 34;
}

// Utility: return the walkability value of a tile
uchar M_Pathfinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

uchar M_Pathfinding::GetTileAtConstructible_ally(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return constructible_map_ally[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}
uchar M_Pathfinding::GetTileAtConstructible_neutral(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return constructible_map_neutral[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const std::vector<iPoint>& M_Pathfinding::GetLastPath() const
{
	return last_path;
}

PathNode* M_Pathfinding::GetPathNode(int x, int y)
{
	return &node_map[(y*width) + x];
}

iPoint M_Pathfinding::FindNearestWalkable(const iPoint & origin)
{
	iPoint ret(origin);

	// dx -> direction x  | dy -> direction y  
	// search_in_radius -> finds the nearest walkable tile in a radius (max radius in FIND_RADIUS) 

	int search_in_radius = 1;
	while (search_in_radius != FIND_RADIUS)
	{
		for (int dx = -search_in_radius; dx < search_in_radius; dx++)
		{
			for (int dy = -search_in_radius; dy < search_in_radius; dy++)
			{
				ret.x = origin.x + dx;
				ret.y = origin.y + dy;
				if (IsWalkable(ret))
					return ret; // Found the nearest walkable tile
			}
		}

		++search_in_radius;
	}
	return ret.create(-1, -1);
}

     //----------------||----------------||----------------\\
	//----------------||A* + JPS algorithm||----------------\\
   //----------------||____________________||----------------\\

bool M_Pathfinding::CalculatePath(iPoint start, const iPoint & end, std::vector<iPoint>& vec_to_fill)
{
	CleanUpJPS();

	float max_destination_cost = app->map->data.height * app->map->data.width;
	origin = new PathNode(0.0f, 0.0f, start, iPoint(-1, -1));
	destination = new PathNode(max_destination_cost, 0.0f, end, iPoint(-2, -2));
	PathNode* check = new PathNode(*origin);

	if (IsWalkable(start))
	{
		visited.push_back(origin);
		visited.push_back(destination);
	}
	else
	{	
		LOG("Non-Walkable origin");	
		start = FindNearestWalkable(start);
		visited.push_back(origin);
		visited.push_back(destination);
		//return false;
	}

	if (!IsWalkable(end))
	{
		LOG("Non-Walkable Destination");
		return false;
	}

	if (origin->pos == destination->pos)
	{
		LOG("NOT MOVING");
		return false;
	}

	X_DIRECTION dx = X_NO_DIR;
	Y_DIRECTION dy = Y_NO_DIR;

	OpenOrigin();

	//open all forced neighbours
	ForcedNeighbour forced_neighbour;

	while (GetLowestFN(forced_neighbour))
	{
		if (GetNodeFromVisited(forced_neighbour.after)->cost_so_far >= destination->cost_so_far && destination_reached == true)
			break;

		check = (PathNode*)GetNodeFromVisited(forced_neighbour.after);

		if (forced_neighbour.after.x - forced_neighbour.before.x > 0)
			dx = X_RIGHT;
		else if (forced_neighbour.after.x - forced_neighbour.before.x < 0)
			dx = X_LEFT;

		if (forced_neighbour.after.y - forced_neighbour.before.y > 0)
			dy = Y_DOWN;
		else if (forced_neighbour.after.y - forced_neighbour.before.y < 0)
			dy = Y_UP;

		if (CheckForTiles(check, dx, dy))
			destination_reached = true;
	}

	if (destination_reached)
		FillPathVec(vec_to_fill);

	CleanUpJPS();

	if (destination_reached)
	{
		destination_reached = false;
		return true;
	}

	return false;
}

void M_Pathfinding::DrawDebug()
{
}

bool M_Pathfinding::CheckForTiles(const PathNode* start, X_DIRECTION dx, Y_DIRECTION dy) //true when destination reached
{
	bool diagonal_end = false;
	PathNode* diagonal = new PathNode(*start);
	diagonal->parent = start->pos;

	while (diagonal_end == false)
	{
		//initial pos check
		if (diagonal->pos == start->pos)
		{
			if (CheckForForcedNeighbours(start, dx, dy))
				return true;
		}
		else
			if (CheckForForcedNeighbours(diagonal, dx, dy))
				return true;
		//X
		if (diagonal->pos == start->pos)
		{
			if (CheckX(start, dx) == true)
				return true;
		}
		else
			if (CheckX(diagonal, dx) == true)
				return true;
		//Y
		if (diagonal->pos == start->pos)
		{
			if (CheckY(start, dy) == true)
				return true;
		}
		else
			if (CheckY(diagonal, dy) == true)
				return true;
		//Diagonal
		MoveDiagonal(diagonal, dx, dy, diagonal_end);

	}
	DELETE_PTR(diagonal);
	return false;
}

const PathNode* M_Pathfinding::PushToVisited(const PathNode* node)
{
	PathNode* ret = nullptr;
	bool found = false;

	for (std::vector<PathNode*>::iterator it = visited.begin(); it != visited.end(); ++it)
	{
		if ((*it)->pos == node->pos)
		{
			if (node->cost_so_far < (*it)->cost_so_far && node->pos != origin->pos) //put = for debug
			{
				(*it)->parent = node->parent;
				ChangeCosts(it, node->cost_so_far);
			}
			found = true;
			ret = *it;
			break;
		}
	}

	if (found == false)
	{
		PathNode* copy = new PathNode(*node);
		visited.push_back(copy);
		ret = copy;
	}
	return (const PathNode*)ret;
}

void M_Pathfinding::PushToFrontier(ForcedNeighbour & fn)
{
	bool found = false;

	for (std::list<ForcedNeighbour>::iterator it = frontier.begin(); it != frontier.end(); it++)
		if (fn.before == it->before && fn.after == it->after)
			found = true;

	if (!found)
		frontier.push_back(fn);
}

bool M_Pathfinding::FoundForcedNeighbour(const PathNode* before, iPoint after_pos, const PathNode* path_to)
{
	PushToVisited(path_to);

	PathNode* after = new PathNode();
	after->pos = after_pos;
	after->parent = before->pos;
	after->cost_so_far = before->cost_so_far + DIAGONAL_COST;
	after->CalculatePriority(destination->pos);

	PushToVisited(before);
	PushToVisited(after);

	ForcedNeighbour corner(before->pos, after->pos, after->priority);

	PushToFrontier(corner);

	if (after->pos == destination->pos)
	{
		DELETE_PTR(after);
		return true;
	}
	DELETE_PTR(after);
	return false;
}

bool M_Pathfinding::FoundForcedNeighbour(const PathNode* before, iPoint after_pos)
{
	PathNode* after = new PathNode();
	after->pos = after_pos;
	after->parent = before->pos;
	after->cost_so_far = before->cost_so_far + DIAGONAL_COST;
	after->CalculatePriority(destination->pos);

	PushToVisited(before);
	PushToVisited(after);

	ForcedNeighbour corner(before->pos, after->pos, after->priority);

	PushToFrontier(corner);

	if (after->pos == destination->pos)
	{
		DELETE_PTR(after);
		return true;
	}
	DELETE_PTR(after);
	return false;
}

bool M_Pathfinding::CheckX(const PathNode* start, X_DIRECTION dir)
{
	PathNode* line = new PathNode(start->cost_so_far, start->distance, start->pos, start->parent);
	line->parent = start->pos;

	if (dir == X_RIGHT)
		while (IsWalkable(iPoint(line->pos.x + 1, line->pos.y)))
		{
			line->pos.x++;
			line->cost_so_far += STRAIGHT_COST;
			line->CalculatePriority(destination->pos);

			//Check destination
			if (line->pos == destination->pos)
			{
				DestinationReached(line, start);
				DELETE_PTR(line);
				return true;
			}

			//UP
			if (IsWalkable(iPoint(line->pos.x, line->pos.y - 1)) == false)
				if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y - 1)) == true)
					if (FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y - 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
			//DOWN
			if (IsWalkable(iPoint(line->pos.x, line->pos.y + 1)) == false)
				if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y + 1)) == true)
					if (FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y + 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
		}

	if (dir == X_LEFT)
		while (IsWalkable(iPoint(line->pos.x - 1, line->pos.y)))
		{
			line->pos.x--;
			line->cost_so_far += STRAIGHT_COST;
			line->CalculatePriority(destination->pos);

			if (line->pos == destination->pos)
			{
				DestinationReached(line, start);
				DELETE_PTR(line);
				return true;
			}

			//UP
			if (IsWalkable(iPoint(line->pos.x, line->pos.y - 1)) == false)
				if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y - 1)) == true)
					if (FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y - 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
			//DOWN
			if (IsWalkable(iPoint(line->pos.x, line->pos.y + 1)) == false)
				if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y + 1)) == true)
					if (FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y + 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
		}
	DELETE_PTR(line);
	return false;
}

bool M_Pathfinding::CheckY(const PathNode* start, Y_DIRECTION dir)
{
	PathNode* line = new PathNode(*start);
	line->parent = start->pos;

	if (dir == Y_DOWN)
		while (IsWalkable(iPoint(line->pos.x, line->pos.y + 1)))
		{
			line->pos.y++;
			line->cost_so_far += STRAIGHT_COST;
			line->CalculatePriority(destination->pos);

			if (line->pos == destination->pos)
			{
				DestinationReached(line, start);
				DELETE_PTR(line);
				return true;
			}

			//Left
			if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y)) == false)
				if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y + 1)) == true)
					if (FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y + 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
			//Right
			if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y)) == false)
				if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y + 1)) == true)
					if (FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y + 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
		}

	//Up
	if (dir == Y_UP)
		while (IsWalkable(iPoint(line->pos.x, line->pos.y - 1)))
		{
			line->pos.y--;
			line->cost_so_far += STRAIGHT_COST;
			line->CalculatePriority(destination->pos);

			if (line->pos == destination->pos)
			{
				DestinationReached(line, start);
				DELETE_PTR(line);
				return true;
			}

			//Left
			if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y)) == false)
				if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y - 1)) == true)
					if (FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y - 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
			//Right
			if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y)) == false)
				if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y - 1)) == true)
					if (FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y - 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
		}
	DELETE_PTR(line);
	return false;
}

void M_Pathfinding::MoveDiagonal(PathNode* diagonal, X_DIRECTION dx, Y_DIRECTION dy, bool& diagonal_end)
{
	if (dx == X_RIGHT && dy == Y_DOWN)
		if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y + 1)))
		{
			diagonal->pos.x++;
			diagonal->pos.y++;
			diagonal->cost_so_far += DIAGONAL_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dx == X_LEFT && dy == Y_UP)
		if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y - 1)))
		{
			diagonal->pos.x--;
			diagonal->pos.y--;
			diagonal->cost_so_far += DIAGONAL_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dx == X_RIGHT && dy == Y_UP)
		if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y - 1)))
		{
			diagonal->pos.x++;
			diagonal->pos.y--;
			diagonal->cost_so_far += DIAGONAL_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dx == X_LEFT && dy == Y_DOWN)
		if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y + 1)))
		{
			diagonal->pos.x--;
			diagonal->pos.y++;
			diagonal->cost_so_far += DIAGONAL_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dx == X_LEFT)
		if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y)))
		{
			diagonal->pos.x--;
			diagonal->cost_so_far += STRAIGHT_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dx == X_RIGHT)
		if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y)))
		{
			diagonal->pos.x++;
			diagonal->cost_so_far += STRAIGHT_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dy == Y_UP)
		if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y - 1)))
		{
			diagonal->pos.y--;
			diagonal->cost_so_far += STRAIGHT_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dy == Y_DOWN)
		if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y + 1)))
		{
			diagonal->pos.y++;
			diagonal->cost_so_far += STRAIGHT_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;
}

bool M_Pathfinding::GetLowestFN(ForcedNeighbour& fn)
{
	bool ret = false;
	bool all_opened = true;
	if (!frontier.empty())
	{
		fn = frontier.front();

		for (std::list<ForcedNeighbour>::iterator it = frontier.begin(); it != frontier.end(); ++it)
			if ((it->opened == false))
			{
				fn = *it;
				all_opened = false;
				break;
			}

		if (all_opened)
			return false;

		for (std::list<ForcedNeighbour>::iterator it = frontier.begin(); it != frontier.end(); ++it)
			if (it->opened == false && it->priority < fn.priority)
				fn = *it;

		for (std::list<ForcedNeighbour>::iterator it = frontier.begin(); it != frontier.end(); ++it)
			if (it->after == fn.after && it->before == fn.before && it->opened == false)
			{
				it->opened = true;
				return true;
			}
	}
	return false;
}

const bool M_Pathfinding::FrontierFinished() const
{
	if (frontier.empty())
		return true;

	for (std::list<ForcedNeighbour>::const_iterator i = frontier.begin(); i != frontier.end(); ++i)
		if (i->opened != true)
			return false;

	return true;
}

void M_Pathfinding::FillPathVec(std::vector<iPoint>& vec)
{
	vec.clear();

	const PathNode* it = destination;

	while (it->parent != origin->parent)
	{
		vec.push_back(it->pos);
		it = GetNodeFromVisited(it->parent);
	}

	last_path = vec;
}

void M_Pathfinding::ChangeCosts(std::vector<PathNode*>::iterator from, float new_cost)
{
	float difference = (*from)->cost_so_far - new_cost;

	(*from)->cost_so_far = new_cost;
	(*from)->CalculatePriority(destination->pos);

	for (std::vector<PathNode*>::iterator it = visited.begin(); it != visited.end(); ++it)
		if ((*it)->parent == (*from)->pos)
		{
			float cost = (*it)->cost_so_far - difference;
			ChangeCosts(it, cost);
		}
}

void M_Pathfinding::OpenOrigin()
{
	//open origin in 4 diagonals
	if (CheckForTiles(origin, X_RIGHT, Y_DOWN) == true
		|| CheckForTiles(origin, X_RIGHT, Y_UP) == true
		|| CheckForTiles(origin, X_LEFT, Y_DOWN) == true
		|| CheckForTiles(origin, X_LEFT, Y_UP) == true)
		destination_reached = true;
}

const PathNode * M_Pathfinding::GetNodeFromVisited(const iPoint & pos)
{
	for (std::vector<PathNode*>::iterator it = visited.begin(); it != visited.end(); ++it)
	{
		if ((*it)->pos == pos)
			return *it;
	}
	return nullptr;
}

void M_Pathfinding::DestinationReached(const PathNode* destination, const PathNode* path_to)
{
	PushToVisited(path_to);
	PushToVisited(destination);
}

void M_Pathfinding::DestinationReached(const PathNode* destination)
{
	PushToVisited(destination);
}

bool M_Pathfinding::CheckForForcedNeighbours(const PathNode * node, X_DIRECTION dx, Y_DIRECTION dy)
{
	//Check if in Destination
	if (node->pos == destination->pos)
	{
		DestinationReached(node);
		return true;
	}

	if (dx == X_LEFT)
	{
		//up
		if (IsWalkable(iPoint(node->pos.x, node->pos.y - 1)) == false)
			if (IsWalkable(iPoint(node->pos.x - 1, node->pos.y - 1)))
				if (FoundForcedNeighbour(node, iPoint(node->pos.x - 1, node->pos.y - 1)))
					return true;
		//down
		if (IsWalkable(iPoint(node->pos.x, node->pos.y + 1)) == false)
			if (IsWalkable(iPoint(node->pos.x - 1, node->pos.y + 1)))
				if (FoundForcedNeighbour(node, iPoint(node->pos.x - 1, node->pos.y + 1)))
					return true;
	}
	if (dx == X_RIGHT)
	{
		//up
		if (IsWalkable(iPoint(node->pos.x, node->pos.y - 1)) == false)
			if (IsWalkable(iPoint(node->pos.x + 1, node->pos.y - 1)))
				if (FoundForcedNeighbour(node, iPoint(node->pos.x + 1, node->pos.y - 1)))
					return true;
		//down
		if (IsWalkable(iPoint(node->pos.x, node->pos.y + 1)) == false)
			if (IsWalkable(iPoint(node->pos.x + 1, node->pos.y + 1)))
				if (FoundForcedNeighbour(node, iPoint(node->pos.x + 1, node->pos.y + 1)))
					return true;
	}
	if (dy == Y_UP)
	{
		//right
		if (IsWalkable(iPoint(node->pos.x + 1, node->pos.y)) == false)
			if (IsWalkable(iPoint(node->pos.x + 1, node->pos.y - 1)))
				if (FoundForcedNeighbour(node, iPoint(node->pos.x + 1, node->pos.y - 1)))
					return true;
		//left
		if (IsWalkable(iPoint(node->pos.x - 1, node->pos.y)) == false)
			if (IsWalkable(iPoint(node->pos.x - 1, node->pos.y - 1)))
				if (FoundForcedNeighbour(node, iPoint(node->pos.x - 1, node->pos.y - 1)))
					return true;
	}
	if (dy == Y_DOWN)
	{
		//right
		if (IsWalkable(iPoint(node->pos.x + 1, node->pos.y)) == false)
			if (IsWalkable(iPoint(node->pos.x + 1, node->pos.y + 1)))
				if (FoundForcedNeighbour(node, iPoint(node->pos.x + 1, node->pos.y + 1)))
					return true;
		//left
		if (IsWalkable(iPoint(node->pos.x - 1, node->pos.y)) == false)
			if (IsWalkable(iPoint(node->pos.x - 1, node->pos.y + 1)))
				if (FoundForcedNeighbour(node, iPoint(node->pos.x - 1, node->pos.y + 1)))
					return true;
	}
	return false;
}

void M_Pathfinding::DeleteIfNotPushed(PathNode *& ptr)
{
	bool in_vec = false;

	for (int i = 0; i < visited.size(); i++)
		if (ptr == visited[i])
		{
			in_vec = true;
			break;
		}

	if (!in_vec)
	{
		delete ptr;
		ptr = nullptr;
	}
}


// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------

PathNode::PathNode() : cost_so_far(0.0f), distance(0.0f), priority(0.0f), pos(-1, -1), parent(iPoint(0, 0))
{}

PathNode::PathNode(iPoint pos) : cost_so_far(0.0f), distance(0.0f), priority(0.0f), pos(pos), parent(iPoint(0, 0))
{}

PathNode::PathNode(float cost, float dist, const iPoint& pos, iPoint parent) : cost_so_far(cost), distance(dist), priority(cost + dist), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : cost_so_far(node.cost_so_far), distance(node.distance), priority(node.priority), pos(node.pos), parent(node.parent)
{}

PathNode::~PathNode()
{}

float PathNode::CalculateDistance(iPoint destination)
{
	float sr = (destination.x - pos.x) * (destination.x - pos.x) + (destination.y - pos.y) * (destination.y - pos.y);
	distance = sqrtf(sr);
	return distance;
}

float PathNode::CalculatePriority(const iPoint& destination)
{
	priority = CalculateDistance(destination) + cost_so_far;
	return priority;
}

const PathNode & PathNode::operator=(const PathNode & node)
{
	this->cost_so_far = node.cost_so_far;
	this->distance = node.distance;
	this->parent = node.parent;
	this->pos = node.pos;

	return *this;
}

inline bool PathNode::operator>(const PathNode & rhs) const
{
	if (this->priority > rhs.priority)
		return true;
	return false;
}

inline bool PathNode::operator<(const PathNode & rhs) const
{
	if (this->distance + this->cost_so_far < rhs.cost_so_far + rhs.distance)
		return true;
	return false;
}

inline bool PathNode::operator==(const PathNode & rhs) const
{
	if (this->distance == rhs.distance && this->cost_so_far == rhs.cost_so_far && this->parent == rhs.parent && this->pos == rhs.pos)
		return true;
	return false;
}

//ForcedNeighbour
ForcedNeighbour::ForcedNeighbour(const iPoint& before, const iPoint& after, float priority) : before(before), after(after), opened(false), priority(priority)
{}

ForcedNeighbour::ForcedNeighbour() : before(iPoint(0, 0)), after(iPoint(0, 0)), opened(false), priority(0.0f)
{}

const ForcedNeighbour & ForcedNeighbour::operator=(const ForcedNeighbour & rhs)
{
	before = rhs.before;
	after = rhs.after;
	opened = rhs.opened;
	priority = rhs.priority;

	return *this;
}
