#pragma once
#include <memory>
#include <cstdarg>
#include <list>
#include <vector>
#include "raylib.h"

template <typename T>
class KDTree
{
public:
	KDTree(int dimensions) : dimensions(dimensions), dimensionCoord() {}
	KDTree(int dimensions, std::vector<float>& dimensionCoord, T& item) : dimensions(dimensions), dimensionCoord(dimensionCoord), item(std::make_shared<T>(item)) {}

	void clear()
	{
		item.reset();
		if(left)
			left->clear();
		if (right)
			right->clear();
	}

	void insert(T& newItem, std::vector<float> coords, int d = 0)
	{
		if (dimensionCoord.empty()) // root node
		{
			dimensionCoord = coords;
			item = std::make_shared<T>(newItem);
			return;
		}

		if (d >= dimensions) d = 0;

		if (d < dimensions)
			if (dimensionCoord[d] > coords[d])
				if (!left) // if there is nothing to the left insert
				{
					left = std::make_shared<KDTree<T>>(dimensions, coords, newItem);	
					return;
				}
				else // parse the left branch
				{
					left->insert(newItem, coords, ++d);
				}
			else
				if (!right) // if there is nothing to the right insert
				{
					right = std::make_shared<KDTree<T>>(dimensions, coords, newItem);
					return;
				}
				else // parse the r branch
				{
					right->insert(newItem, coords, ++d);
				}
	}

	std::list<T> _search(const Rectangle& rArea) const
	{
		std::list<T> listItems;
		search(rArea, listItems);
		return listItems;
	}

	void search(const Rectangle& rArea, std::list<T>& listItems) const
	{
		if(item)
			if (CheckCollisionRecs(rArea, (*item)->getRectangle()))
				listItems.push_back((*item));

		if (item)
			if (left)
			{
			
				if (firstContainsSecond(rArea, (*left->item)->getRectangle()))
				{
					left->retriveItems(listItems);
				}
				else if (CheckCollisionRecs((*left->item)->getRectangle(), rArea))
				{
					left->search(rArea, listItems);
				}
			}
			else if (right)
			{
				if (firstContainsSecond(rArea, (*right->item)->getRectangle()))
				{
					right->retriveItems(listItems);
				}
				else if (CheckCollisionRecs((*right->item)->getRectangle(), rArea))
				{
					right->search(rArea, listItems);
				}
			}
	}

protected:

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

	void retriveItems(std::list<T>& listItems) const
	{
		listItems.push_back((*item));

		if(left)
			left->retriveItems(listItems);
		if (right)
			right->retriveItems(listItems);
	}

	int dimensions;

	std::shared_ptr<KDTree<T>> left;

	std::shared_ptr<KDTree<T>> right;

	std::vector<float> dimensionCoord{};

	std::shared_ptr<T> item;
};