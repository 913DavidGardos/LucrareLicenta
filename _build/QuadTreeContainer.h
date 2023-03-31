#pragma once
#include <iostream>
#include "QuadTree.h"

template <typename T>
class StaticQuadTreeContainer
{
	using QuadTreeContainer = std::list<T>;

protected:
	QuadTreeContainer allItems;
	std::list<typename QuadTreeContainer::iterator> allItemsPointers;
	StaticQuadTree<typename QuadTreeContainer::iterator> root;

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
	void insert(const T& item, const Rectangle& itemSize)
	{
		allItems.push_back(item);
		allItemsPointers.push_back(std::prev(allItems.end()));

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
		for (auto it = allItems.begin(); it != allItems.end(); ++it)
		{
			root.insert(it, Rectangle{ it->getX() - it->getRadius(), it->getY() - it->getRadius(), it->getRadius() * 2.f, it->getRadius() * 2.f });
		}
	}

	void drawLines()
	{
		root.traverse();

	}
};