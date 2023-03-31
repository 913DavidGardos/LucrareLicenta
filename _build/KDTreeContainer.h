#pragma once
#include <iostream>
#include "KDTree.h"

template <typename T>
class KDTreeContainer
{
	using TreeContainer = std::list<T>;

protected:
	TreeContainer allItems;
	KDTree<typename TreeContainer::iterator> root;

public:
	KDTreeContainer(int dimension) : root(dimension)
	{

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

	void insert(const T& item, std::vector<float>dims)
	{
		allItems.push_back(item);
		root.insert(std::prev(allItems.end()), dims);
	}

	std::list<typename TreeContainer::iterator> search(const Rectangle& rArea) const
	{
		return root._search(rArea);
	}

	typename TreeContainer::iterator begin()
	{
		return allItems.begin();
	}

	typename TreeContainer::iterator end()
	{
		return allItems.end();
	}

	void update()
	{
		root.clear();
		for (auto it = allItems.begin(); it != allItems.end(); ++it)
		{
			root.insert(it, std::vector<float>{it->getX(), it->getY()});
		}
	}
};