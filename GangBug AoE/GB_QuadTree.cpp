#include "GB_QuadTree.h"
#include "Entity.h"

//-----------------------------------------------------
//-----------------GB_QuadTreeNode---------------------
//-----------------------------------------------------

GB_QuadTreeNode::GB_QuadTreeNode(const GB_Rectangle<int> _box) : box(_box)
{
	for (unsigned int i = 0; i < 4; ++i)
		childs[i] = nullptr;
	parent = nullptr;
}

GB_QuadTreeNode::~GB_QuadTreeNode()
{

}

void GB_QuadTreeNode::Insert(Entity* et)
{
	if (et != nullptr)
	{
		if (childs[0] == nullptr && entities.size() < MAX_NODE_ENTITIES)
		{
			entities.push_back(et);
		}
		else
		{
			if (childs[0] == nullptr)
				DivideNode();

			entities.push_back(et);
			AjustNode();
		}
	}
}

void GB_QuadTreeNode::Erase(Entity* et)
{
	std::list<Entity*>::iterator tmp = std::find(entities.begin(), entities.end(), et);

	if (tmp != entities.end())
	{
		entities.erase(tmp);
	}

	if (childs[0] != nullptr)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			childs[i]->Erase(et);
		}
	}
}

void GB_QuadTreeNode::CollectCandidates(std::vector<Entity*>& vec, const GB_Rectangle<int> test)
{
	if(box.Collides(test) == true)
	{
		for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		{
			vec.push_back((*it));
		}
	}

	if (childs[0] != nullptr)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			childs[i]->CollectCandidates(vec, test);
		}
	}
}

void GB_QuadTreeNode::CollectTreeBox(std::vector<GB_Rectangle<int>>& vec)
{
	vec.push_back(box);

	if (childs[0] != nullptr)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			childs[i]->CollectTreeBox(vec);
		}
	}
}

void GB_QuadTreeNode::DivideNode()
{
	iPoint newSize(box.w / 2, box.h / 2);
	GB_Rectangle<int> tmp;

	//TOP-LEFT
	tmp.Set(box.x, box.y, newSize.x, newSize.y);
	childs[0] = new GB_QuadTreeNode(tmp);

	//TOP-RIGHT
	tmp.Set(box.x + newSize.x, box.y, newSize.x, newSize.y);
	childs[1] = new GB_QuadTreeNode(tmp);

	//BOTTOM-RIGHT
	tmp.Set(box.x + newSize.x, box.y + newSize.y, newSize.x, newSize.y); //TODO: Must check coord system on Y axis, dont remember if must sum or rest the y
	childs[2] = new GB_QuadTreeNode(tmp);

	//BOTTOM-LEFT
	tmp.Set(box.x, box.y + newSize.y, newSize.x, newSize.y); //TODO: Must check coord system on Y axis, dont remember if must sum or rest the y
	childs[3] = new GB_QuadTreeNode(tmp);

	for (unsigned int i = 0; i < 4; ++i)
	{
		childs[i]->parent = this;
	}
}

void GB_QuadTreeNode::AjustNode()
{ 
	std::list<Entity*>::iterator it = entities.begin();

	while (it != entities.end())
	{
		Entity* tmp = *it;
		GB_Rectangle<int> b(tmp->GetEnclosingBox());

		bool intersections[4];

		for (unsigned int i = 0; i < 4; ++i)
			intersections[i] = childs[i]->box.Collides(b);

		if (intersections[0] && intersections[1] && intersections[2] && intersections[3])
		{
			//If collides with all childs will let it in parent.
			++it;
		}
		else
		{
			it = entities.erase(it);
			
			for (unsigned int i = 0; i < 4; ++i)
			{
				if (intersections[i])
					childs[i]->Insert(tmp);
			}
		}
	}
}



//=============================================

GB_QuadTree::GB_QuadTree()
{
}

GB_QuadTree::GB_QuadTree(const GB_Rectangle<int>& _box)
{
	root = new GB_QuadTreeNode(_box);
}

GB_QuadTree::~GB_QuadTree()
{
	Clear();
}

void GB_QuadTree::Insert(Entity* et)
{
	if (root != nullptr && et != nullptr)
		if(root->box.Collides(et->GetEnclosingBox()) == true)
			root->Insert(et);
}

void GB_QuadTree::Erase(Entity* et)
{
	if (root != nullptr && et != nullptr)
		root->Erase(et);
}

void GB_QuadTree::CollectCandidates(std::vector<Entity*>& vec, const GB_Rectangle<int> test)
{
	if (root != nullptr)
		root->CollectCandidates(vec, test);
}

void GB_QuadTree::CollectTreeBoxes(std::vector<GB_Rectangle<int>>& vec)
{
	if (root != nullptr)
		root->CollectTreeBox(vec);
}

void GB_QuadTree::SetRoot(const GB_Rectangle<int>& _box)
{
	if (root != nullptr)
		delete(root);

	root = new GB_QuadTreeNode(_box);
}

void GB_QuadTree::Clear()
{
	if (root != nullptr)
	{
		delete(root);
		root = nullptr;
	}
}