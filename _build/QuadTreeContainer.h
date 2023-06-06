#pragma once
#include <iostream>
#include "QuadTree.h"

template <typename T>
class StaticQuadTreeContainer
{
	using QuadTreeContainer = std::list<std::shared_ptr<T>>;

protected:
	QuadTreeContainer allItems;
	StaticQuadTree<typename QuadTreeContainer::iterator> root;
	std::list<std::shared_ptr<StaticQuadTree<typename QuadTreeContainer::iterator>>> leaves; // Nodes from the QuadTree


public:
	StaticQuadTreeContainer(const Rectangle& rectangle, const size_t depth) : root(rectangle, depth)
	{

	}
	void resize(const Rectangle& rArea)
	{
		root.resize(rArea);
	}
	size_t size() const
	{
		return allItems.size();
	}
	bool empty() const
	{
		return allItems.empty();
	}
	void clear()
	{
		root.clear();
		allItems.clear();
	}
	void insert(const std::shared_ptr<T> item, const Rectangle& itemSize)
	{
		allItems.push_back(item);
		root.insert(std::prev(allItems.end()), itemSize);
	}

	void remove(const T& item, const Rectangle& itemSize)
	{
		allItems.erase(item);
		root.remove(std::prev(allItems.end()), itemSize);
	}

	std::list<typename QuadTreeContainer::iterator> search(const Rectangle& rArea) const
	{
		return root._search(rArea);
	}

	typename QuadTreeContainer::iterator begin()
	{
		return allItems.begin();
	}
	
	typename QuadTreeContainer::iterator end()
	{
		return allItems.end();
	}

	void update()
	{
		root.clear();

		for (auto iter = allItems.begin(); iter != allItems.end(); ++iter)
		{
			auto it = *iter;
			root.insert(iter, Rectangle{ it->getX() - it->getRadius(), it->getY() - it->getRadius(), it->getRadius() * 2.f, it->getRadius() * 2.f });
		}
	}

	void drawLines()
	{
		root.traverse();

	}

	bool firstContainsSecond(const Rectangle& first, const Rectangle& second) const
	{
		if (first.x < second.x &&
			first.y < second.y &&
			first.x + first.width  > second.x + second.width &&
			first.y + first.height > second.y + second.height)
		{
			return true;

		}
		return false;
	}
};







