#ifndef __GB_QUADTREE__
#define __GB_QUADTREE__

#define MAX_NODE_ENTITIES 8

#include <list>
#include <vector>
#include "GB_Rectangle.h"

class Entity;

class GB_QuadTreeNode
{
public:
	GB_QuadTreeNode(const GB_Rectangle<int> _box);
	~GB_QuadTreeNode();

	void Insert(Entity* et);
	void Erase(Entity* et);

	void CollectCandidates(std::vector<Entity*>& vec, const GB_Rectangle<int> test);
	void CollectTreeBox(std::vector<GB_Rectangle<int>>& vec);

private:
	void DivideNode();
	void AjustNode();

public:
	GB_Rectangle<int> box;
	std::list<Entity*> entities;
	GB_QuadTreeNode* parent = nullptr;
	GB_QuadTreeNode* childs[4];
	
};

class GB_QuadTree
{
public:
	GB_QuadTree();
	GB_QuadTree(const GB_Rectangle<int>& _box);
	virtual ~GB_QuadTree();

	void Insert(Entity* et);
	void Erase(Entity* et);

	void CollectCandidates(std::vector<Entity*>& vec, const GB_Rectangle<int> test);
	void CollectTreeBoxes(std::vector<GB_Rectangle<int>>& vec);

	void SetRoot(const GB_Rectangle<int>& _box);
	void Clear();

public:
	GB_QuadTreeNode* root = nullptr;
};


#endif // !__GB_QUADTREE__