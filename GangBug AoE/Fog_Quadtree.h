#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <vector>

#define NODE_SUBDIVISION 4

#include "SDL/include/SDL_rect.h"
#include "p2Point.h"
#include "Log.h"
#include "Iso_Primitives.h"

/// Class TreeItem ------------------------------
//Class that contains the data & its location
template <class DATA_TYPE>
struct TreeItem
{

	TreeItem() {}
	TreeItem(const DATA_TYPE& data, const iPoint& location) : data(data), location(location) {}

	iPoint		location = { 0,0 };
	DATA_TYPE	data = NULL;

	bool operator == (const TreeItem& target)
	{
		return (location == location && data == data);
	}
};
/// ---------------------------------------------

/// Class m_TreeItem ------------------------------
//Class that contains the data & its location in float for mutable quad tree needs more precision
template <class DATA_TYPE>
struct m_TreeItem
{

	m_TreeItem() {}
	m_TreeItem(const DATA_TYPE& data, const fPoint& location) : data(data), location(location) {}

	fPoint location = { 0,0 };
	DATA_TYPE data = NULL;

	bool operator == (const m_TreeItem& target)
	{
		return (location == location && data == data);
	}
};
/// ---------------------------------------------

/// Class AABB ----------------------------------
// This are the rectangles used by the quad-tree to subdivide the space (not mutable)
template <class DATA_TYPE>
class AABB
{
public:

	//Constructors ========================
	AABB(const SDL_Rect& aabb, uint max_objects) : aabb(aabb), max_objects(max_objects)
	{
		for (uint i = 0; i < 4; i++)
			children[i] = nullptr;
	}

	//Destructors =========================
	~AABB()
	{
		if (full)
		{
			for (int i = 0; i < 4; ++i)
			{
				if (children[i] != nullptr)
				{
					delete children[i];
				}
			}
		}

		this->objects.clear();
	}

public:

	SDL_Rect							aabb = { 0,0,0,0 };
	std::vector<TreeItem<DATA_TYPE>>	objects;
	uint								max_objects = 0;
	bool								full = false;
	AABB*								root = nullptr;
	AABB*								children[NODE_SUBDIVISION];

public:

	// Functionality =======================
	void Draw(const SDL_Color& color)const
	{
		App->render->DrawQuad(aabb, color.r, color.g, color.b, color.a, false);


		if (!full)return;
		for (uint k = 0; k < NODE_SUBDIVISION; k++)
		{
			children[k]->Draw(color);
		}
	}

	bool Insert(DATA_TYPE data, const iPoint* point)
	{
		// If new point is not in the quad-tree AABB, return
		SDL_Point p = { point->x,point->y };
		if (!SDL_PointInRect(&p, &aabb))
		{
			return false;
		}

		if (full)
		{
			for (uint i = 0; i < NODE_SUBDIVISION; i++)
			{
				if (children[i]->Insert(data, point))
				{
					return true;
				}
			}
			return false;
		}

		// If in this node there is space for the point, pushback it
		uint size = objects.size();
		if (size < max_objects)
		{
			TreeItem<DATA_TYPE> item(data, *point);
			objects.push_back(item);
			if (size + 1 == max_objects)
			{
				full = true;
				Subdivide();
			}
			return true;
		}

		return false;
	}

	void Subdivide()
	{
		// Get subdivision size
		iPoint qSize = { (int)floor(aabb.w * 0.5), (int)floor(aabb.h * 0.5) };
		iPoint qCentre;

		//Calculate new AABB center for each child
		qCentre = { aabb.x,aabb.y };
		children[0] = new AABB({ qCentre.x,qCentre.y,qSize.x,qSize.y }, max_objects);
		children[0]->root = this;


		qCentre = { aabb.x + qSize.x,aabb.y };
		children[1] = new AABB({ qCentre.x,qCentre.y,qSize.x,qSize.y }, max_objects);
		children[1]->root = this;

		qCentre = { aabb.x,aabb.y + qSize.y };
		children[2] = new AABB({ qCentre.x,qCentre.y,qSize.x,qSize.y }, max_objects);
		children[2]->root = this;

		qCentre = { aabb.x + qSize.x,aabb.y + qSize.y };
		children[3] = new AABB({ qCentre.x,qCentre.y,qSize.x,qSize.y }, max_objects);
		children[3]->root = this;

		for (uint h = 0; h < max_objects; h++)
		{
			for (uint k = 0; k < NODE_SUBDIVISION; k++)
			{
				if (children[k]->Insert(objects[h].data, &objects[h].location)) break;
			}
		}
		objects.clear();
	}

	int CollectCandidates(std::vector<DATA_TYPE>& nodes, const SDL_Rect& rect)
	{
		uint ret = 0;

		// If range is not in the quad-tree, return
		if (!SDL_HasIntersection(&rect, &aabb)) return 0;

		// See if the points of this node are in range and pushback them to the vector
		if (full)
		{
			// Otherwise, add the points from the children
			ret += children[0]->CollectCandidates(nodes, rect);
			ret += children[1]->CollectCandidates(nodes, rect);
			ret += children[2]->CollectCandidates(nodes, rect);
			ret += children[3]->CollectCandidates(nodes, rect);
		}
		else
		{
			uint size = objects.size();
			for (uint k = 0; k < size; k++)
			{
				SDL_Point pos = { objects[k].location.x,objects[k].location.y };
				if (SDL_PointInRect(&pos, &rect))
				{
					nodes.push_back(objects[k].data);
					ret++;
				}
			}
		}

		return ret;
	}

	int CollectCandidates(std::vector<DATA_TYPE>& nodes, const Circle& circle)
	{
		uint ret = 0;

		// If range is not in the quad-tree, return
		if (!circle.Intersects(&aabb))return 0;

		// See if the points of this node are in range and pushback them to the vector
		if (full)
		{
			// Otherwise, add the points from the children
			ret += children[0]->CollectCandidates(nodes, circle);
			ret += children[1]->CollectCandidates(nodes, circle);
			ret += children[2]->CollectCandidates(nodes, circle);
			ret += children[3]->CollectCandidates(nodes, circle);
		}
		else
		{
			uint size = objects.size();
			for (uint k = 0; k < size; k++)
			{
				fPoint loc(objects[k].location.x, objects[k].location.y);

				if (circle.IsIn(&loc))
				{
					nodes.push_back(objects[k].data);
					ret++;
				}

			}
		}

		return ret;
	}
};
/// ---------------------------------------------

/// Class m_AABB --------------------------------
// This are the rectangles used by the quad-tree to subdivide the space (mutable)
template <class DATA_TYPE>
class m_AABB
{
public:

	//Constructors ========================
	m_AABB(const SDL_Rect& aabb, uint max_objects) : aabb(aabb), max_objects(max_objects)
	{
		for (uint i = 0; i < 4; i++)
			children[i] = nullptr;
	}

	//Destructors =========================
	~m_AABB()
	{
		if (full)
		{
			for (int i = 0; i < 4; ++i)
			{
				if (children[i] != nullptr)
				{
					delete children[i];
				}
			}
		}

		this->objects.clear();
	}

public:

	//SDL_Rect							aabb = { 0,0,0,0 };
	//std::list<m_TreeItem<DATA_TYPE>> 	objects;
	uint								max_objects = 0;
	bool								full = false;
	m_AABB*								root = nullptr;
	m_AABB*								children[NODE_SUBDIVISION];

public:

	// Functionality =======================
	void Draw(const SDL_Color& color)const
	{
		App->render->DrawQuad(aabb, color.r, color.g, color.b, color.a, false);

		if (!full)return;

		for (uint k = 0; k < NODE_SUBDIVISION; k++)
		{
			children[k]->Draw(color);
		}
	}

	uint CountChildrenObjects()const
	{
		if (!full)return objects.size();

		uint total_size = 0;
		for (uint k = 0; k < NODE_SUBDIVISION; k++)
		{
			total_size += children[k]->CountChildrenObjects();
		}
		return total_size;
	}

	bool Insert(DATA_TYPE data, const fPoint* point)
	{
		// If new point is not in the quad-tree AABB, return
		SDL_Point p = { point->x,point->y };
		if (!SDL_PointInRect(&p, &aabb))
		{
			return false;
		}

		if (full)
		{
			for (uint i = 0; i < NODE_SUBDIVISION; i++)
			{
				if (children[i]->Insert(data, point))
				{
					return true;
				}
			}
			return false;
		}

		// If in this node there is space for the point, pushback it
		uint size = objects.size();
		if (size < max_objects)
		{
			m_TreeItem<DATA_TYPE> item(data, *point);
			objects.push_back(item);
			if (size + 1 == max_objects)
			{
				full = true;
				Subdivide();
			}
			return true;
		}

		return false;
	}

	bool Extract(const fPoint* loc)
	{
		bool found = false;

		if (full)
		{
			for (uint i = 0; i < NODE_SUBDIVISION; i++)
			{
				if (found)break;
				if (children[i]->full)
				{
					if (children[i]->Extract(loc))
					{
						found = true;
						break;
					}
				}
				else
				{
					std::list<m_TreeItem<DATA_TYPE>>::iterator object = children[i]->objects.begin();
					while (object != children[i]->objects.end())
					{
						if (object._Ptr->_Myval.location == *loc)
						{
							children[i]->objects.erase(object);
							found = true;
							break;
						}
						object++;
					}
				}
			}
			if (found)
			{
				if (CountChildrenObjects() < max_objects)
				{
					Fuse();
				}
				return true;
			}
		}
		else
		{
			std::list<m_TreeItem<DATA_TYPE>>::iterator object = objects.begin();
			while (object != objects.end())
			{
				if (object._Ptr->_Myval.location == *loc)
				{
					objects.erase(object);
					return true;
				}
				object++;
			}
		}

		return false;
	}

	bool Extract(const DATA_TYPE data, const fPoint* loc)
	{
		bool found = false;

		if (full)
		{
			for (uint i = 0; i < NODE_SUBDIVISION; i++)
			{
				if (found)break;
				if (children[i]->full)
				{
					if (children[i]->Extract(data, loc))
					{
						found = true;
						break;
					}
				}
				else
				{
					std::list<m_TreeItem<DATA_TYPE>>::iterator object = children[i]->objects.begin();
					while (object != children[i]->objects.end())
					{
						if (object._Ptr->_Myval.location == *loc && object._Ptr->_Myval.data == data)
						{
							children[i]->objects.erase(object);
							found = true;
							break;
						}
						object++;
					}
				}
			}
			if (found)
			{
				if (CountChildrenObjects() < max_objects)
				{
					Fuse();
				}
				return true;
			}
		}
		else
		{
			std::list<m_TreeItem<DATA_TYPE>>::iterator object = objects.begin();
			while (object != objects.end())
			{
				if (object._Ptr->_Myval.location == *loc)
				{
					objects.erase(object);
					return true;
				}
				object++;
			}
		}

		return false;
	}

	void Subdivide()
	{
		// Get subdivision size
		iPoint qSize = { (int)floor(aabb.w * 0.5), (int)floor(aabb.h * 0.5) };
		iPoint qCentre;

		//Calculate new AABB center for each child
		qCentre = { aabb.x,aabb.y };
		children[0] = new m_AABB({ qCentre.x,qCentre.y,qSize.x,qSize.y }, max_objects);
		children[0]->root = this;


		qCentre = { aabb.x + qSize.x,aabb.y };
		children[1] = new m_AABB({ qCentre.x,qCentre.y,qSize.x,qSize.y }, max_objects);
		children[1]->root = this;

		qCentre = { aabb.x,aabb.y + qSize.y };
		children[2] = new m_AABB({ qCentre.x,qCentre.y,qSize.x,qSize.y }, max_objects);
		children[2]->root = this;

		qCentre = { aabb.x + qSize.x,aabb.y + qSize.y };
		children[3] = new m_AABB({ qCentre.x,qCentre.y,qSize.x,qSize.y }, max_objects);
		children[3]->root = this;


		std::list<m_TreeItem<DATA_TYPE>>::const_iterator object = objects.begin();
		while (object != objects.end())
		{
			for (uint h = 0; h < NODE_SUBDIVISION; h++)
			{
				if (children[h]->Insert(object._Ptr->_Myval.data, &object._Ptr->_Myval.location))break;
			}
			object++;
		}
		objects.clear();
	}

	void Fuse()
	{
		this->full = false;
		for (uint k = 0; k < NODE_SUBDIVISION; k++)
		{
			std::list<m_TreeItem<DATA_TYPE>>::const_iterator object = children[k]->objects.begin();
			while (object != children[k]->objects.end())
			{
				objects.push_back(object._Ptr->_Myval);
				object++;
			}

			children[k]->objects.clear();
			delete children[k];
			children[k] = nullptr;
		}

	}

	int CollectCandidates(std::vector<DATA_TYPE>& nodes, const SDL_Rect& rect)
	{
		uint ret = 0;

		// If range is not in the quad-tree, return
		if (!SDL_HasIntersection(&rect, &aabb)) return 0;

		// See if the points of this node are in range and pushback them to the vector
		if (full)
		{
			// Otherwise, add the points from the children
			ret += children[0]->CollectCandidates(nodes, rect);
			ret += children[1]->CollectCandidates(nodes, rect);
			ret += children[2]->CollectCandidates(nodes, rect);
			ret += children[3]->CollectCandidates(nodes, rect);
		}
		else
		{
			std::list<m_TreeItem<DATA_TYPE>>::const_iterator object = objects.begin();
			while (object != objects.end())
			{
				fPoint loc = object._Ptr->_Myval.location;
				SDL_Point point = { loc.x,loc.y };

				if (SDL_PointInRect(&point, &rect))
				{
					nodes.push_back(object._Ptr->_Myval.data);
					ret++;
				}

				object++;
			}
		}

		return ret;
	}

	int CollectCandidates(std::vector<DATA_TYPE>& nodes, const Circle& circle)
	{
		uint ret = 0;

		// If range is not in the quad-tree, return
		if (!circle.Intersects(&aabb))return 0;

		// See if the points of this node are in range and pushback them to the vector
		if (full)
		{
			// Otherwise, add the points from the children
			ret += children[0]->CollectCandidates(nodes, circle);
			ret += children[1]->CollectCandidates(nodes, circle);
			ret += children[2]->CollectCandidates(nodes, circle);
			ret += children[3]->CollectCandidates(nodes, circle);
		}
		else
		{
			std::list<m_TreeItem<DATA_TYPE>>::const_iterator object = objects.begin();
			while (object != objects.end())
			{
				fPoint loc = object._Ptr->_Myval.location;

				if (circle.IsIn(&loc))
				{
					nodes.push_back(object._Ptr->_Myval.data);
					ret++;
				}

				object++;
			}
		}

		return ret;
	}

	void Reset()
	{
		if (full)
		{
			for (uint k = 0; k < 4; k++)
			{
				if (children[k]->full)
				{
					children[k]->Reset();
				}
				else delete children[k];
			}
		}
		this->full = false;
		objects.clear();
	}
};
/// ---------------------------------------------

/// Class QuadTree ------------------------------
// This class contains all the tree information & functionality (not mutable)
template <class DATA_TYPE>
class QuadTree
{
public:

	//Constructors ========================
	QuadTree(const SDL_Rect& rect = { 0,0,0,0 }, uint max_objects = 0)
	{
		SetBoundaries(rect);
	}

	//Destructors =========================
	~QuadTree()
	{
		Clear();
	}

private:

	AABB<DATA_TYPE>*	root = nullptr;
	uint				max_objects = 0;
	SDL_Color			color = { 255,255,255,255 };

public:

	// Functionality =======================
	void SetBoundaries(const SDL_Rect& r)
	{
		if (root != NULL)
			delete root;

		root = new AABB<DATA_TYPE>(r, max_objects);
	}

	void SetMaxObjects(uint max)
	{
		max_objects = max;
		root->max_objects = max;
	}

	void SetDebugColor(const SDL_Color& new_color)
	{
		color = new_color;
	}

	bool Insert(DATA_TYPE data, const iPoint* newpoint)
	{
		if (root != NULL)
		{
			return root->Insert(data, newpoint);
		}
		return false;
	}

	void Draw()const
	{
		root->Draw(color);
	}

	int	CollectCandidates(std::vector<DATA_TYPE>& nodes, const SDL_Rect& r) const
	{
		int tests = 0;

		if (root != NULL && SDL_HasIntersection(&r, &root->aabb))
			tests = root->CollectCandidates(nodes, r);
		return tests;
	}

	void Clear()
	{
		if (root != NULL)
		{
			delete root;
			root = NULL;
		}
	}

	int	CollectCandidates(std::vector<DATA_TYPE>& nodes, const Circle& circle) const
	{
		int tests = 0;

		if (root != NULL && circle.Intersects(&root->aabb))
			tests = root->CollectCandidates(nodes, circle);

		return tests;
	}

};
/// ---------------------------------------------

/// Class m_QuadTree ----------------------------
// This class contains all the tree information & functionality (mutable)
template <class DATA_TYPE>
class m_QuadTree
{
public:

	//Constructors ========================
	m_QuadTree(const SDL_Rect& rect = { 0,0,0,0 }, uint max_objects = 0)
	{
		SetBoundaries(rect);
	}

	//Destructors =========================
	~m_QuadTree()
	{
		Clear();
	}

private:

	m_AABB<DATA_TYPE>*	root = nullptr;
	uint				max_objects = 0;
	SDL_Color			color = { 255,255,255,255 };

public:

	// Functionality =======================
	void SetBoundaries(const SDL_Rect& r)
	{
		if (root != NULL)
			delete root;

		root = new m_AABB<DATA_TYPE>(r, max_objects);
	}

	void SetMaxObjects(uint max)
	{
		max_objects = max;
		root->max_objects = max;
	}

	void SetDebugColor(const SDL_Color& new_color)
	{
		color = new_color;
	}

	bool Insert(DATA_TYPE data, const fPoint* newpoint)
	{
		if (root != NULL)
		{
			return root->Insert(data, newpoint);
		}
		return false;
	}

	bool Exteract(const fPoint* loc)
	{
		if (root != NULL)
		{
			return root->Extract(loc);
		}
		return false;
	}

	bool Exteract(const DATA_TYPE data, const fPoint* loc)
	{
		if (root != NULL)
		{
			return root->Extract(data, loc);
		}
		return false;
	}

	void Draw()const
	{
		root->Draw(color);
	}

	int	CollectCandidates(std::vector<DATA_TYPE>& nodes, const SDL_Rect& r) const
	{
		int cand = 0;

		if (root != NULL && SDL_HasIntersection(&r, &root->aabb))
			cand = root->CollectCandidates(nodes, r);
		return cand;
	}

	int	CollectCandidates(std::vector<DATA_TYPE>& nodes, const Circle& circle) const
	{
		int tests = 0;

		if (root != NULL && circle.Intersects(&root->aabb))
			tests = root->CollectCandidates(nodes, circle);

		return tests;
	}

	void Clear()
	{
		if (root != NULL)
		{
			delete root;
			root = NULL;
		}
	}

	void Reset()
	{
		if (root != NULL)
		{
			root->Reset();
		}
	}
};
/// ---------------------------------------------
#endif // !_QUADTREE_
