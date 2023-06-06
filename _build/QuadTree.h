#pragma once
#include <raylib.h>
#include <vector>
#include <array>
#include <memory>
#include <list>

template <typename T>
class StaticQuadTree
{
public:
	StaticQuadTree(const Rectangle& rectangle = { 0.f, 0.f, 100.f, 100.f }, const size_t depth = 0) :
		 parent(this), rectangle(rectangle), depth(depth)
	{
		resize(rectangle);
	}

	void resize(const Rectangle& rArea)
	{
		clear();
		rectangle = rArea;
		float childWidth = rectangle.width / 2.0f;
		float childHeight = rectangle.height / 2.0f;

		childRec =
		{
			Rectangle {rectangle.x, rectangle.y, childWidth, childHeight},								// top left
			Rectangle {rectangle.x + childWidth, rectangle.y, childWidth, childHeight},					// top right
			Rectangle {rectangle.x, rectangle.y + childHeight, childWidth, childHeight},				// bottom left
			Rectangle {rectangle.x + childWidth, rectangle.y + childHeight, childWidth, childHeight}	// bottom right
		};
	}

	void clear()
	{
		items.clear();
		for (int i = 0; i < 4; i++)
		{
			if (childPtr[i])
				childPtr[i]->clear();
			childPtr[i].reset();
		}
	}

	size_t size() const
	{
		size_t count = items.size();
		for (int i = 0; i < 4; i++)
			if (childPtr[i])
				count += childPtr[i]->size();

		return count;
	}

	void insert(const T& item, const Rectangle& itemSize)
	{

		for (int i = 0; i < 4; i++)
		{
			if (firstContainsSecond(childRec[i], itemSize))
			{
				// did we reach depth limit?
				if (depth + 1 < maxDepth)
				{
					// does child exist?
					if (!childPtr[i])
					{
						// No, make one
						childPtr[i] = std::make_shared<StaticQuadTree<T>>(childRec[i], depth + 1);
						childPtr[i]->parent = this;
					}

					childPtr[i]->insert(item, itemSize);
					return;
				}
			}
		}

		items.push_back({ itemSize, item });
	}

	std::list<T> _search(const Rectangle& rArea) const
	{
		std::list<T> listItems;
		search(rArea, listItems);
		return listItems;
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
	
	void search(const Rectangle& rArea, std::list<T>& listItems) const
	{

		for (const auto& p : items)
		{
			if (CheckCollisionRecs(rArea, p.first))
				listItems.push_back(p.second);
		}

		for (int i = 0; i < 4; i++)
		{
			if (childPtr[i])
			{
				if (firstContainsSecond(rArea, childRec[i]))
				{
					childPtr[i]->retriveItems(listItems);
				}
				else if (CheckCollisionRecs(childRec[i], rArea))
				{
					childPtr[i]->search(rArea, listItems);
				}

			}
		}
	}

	void traverse()
	{
		DrawRectangleLines(rectangle.x, rectangle.y, rectangle.width, rectangle.height, GRAY);

		for (int i = 0; i < 4; i++)
		{
			if (childPtr[i])
				childPtr[i]->traverse();
		}
	}

	Rectangle getParentRec()
	{
		return parent->rectangle;
	}

	Rectangle getRec()
	{
		return rectangle;
	}
	
	StaticQuadTree<T>* getParent()
	{
		return parent;
	}

	std::shared_ptr <StaticQuadTree<T>> getChildPtr(int i)
	{
		return childPtr[i];
	}

	std::vector<std::pair<Rectangle, T>> getItems()
	{
		return items;
	}


protected:

	void retriveItems(std::list<T>& listItems) const
	{
		for (const auto& p : items)
			listItems.push_back(p.second);

		for (int i = 0; i < 4; i++)
			if (childPtr[i])
				childPtr[i]->retriveItems(listItems);
	}

	StaticQuadTree<T>* parent = nullptr;

	size_t depth = 0;

	Rectangle rectangle;

	std::array<Rectangle, 4> childRec{};

	std::array<std::shared_ptr<StaticQuadTree<T>>, 4> childPtr{};

	// container of the objects stored at this level
	std::vector<std::pair<Rectangle, T>> items;

	static const int maxDepth = 7;
};