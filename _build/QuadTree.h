#pragma once
#include <raylib.h>
#include <vector>
#include <array>
#include <memory>
#include <list>

/// \file
/// This file contains the definition of the StaticQuadTree class template.

/// \class StaticQuadTree
/// \brief A static implementation of a quadtree data structure.
/// \details StaticQuadTree is a templated class that represents a quadtree data structure. It is used to partition a 2D space into smaller regions, allowing for efficient spatial queries.
template <typename T>
class StaticQuadTree
{
public:
    /// \brief Constructs a StaticQuadTree with an optional initial rectangle and depth.
    /// \param rectangle The initial rectangle representing the bounds of the quadtree.
    /// \param depth The initial depth of the quadtree.
    StaticQuadTree(const Rectangle& rectangle = { 0.f, 0.f, 100.f, 100.f }, const size_t depth = 0) :
        parent(this), rectangle(rectangle), depth(depth)
    {
        resize(rectangle);
    }

    /// \brief Resizes the quadtree with a new rectangle.
    /// \param rArea The new rectangle representing the bounds of the quadtree.
    void resize(const Rectangle& rArea)
    {
        clear();
        rectangle = rArea;
        float childWidth = rectangle.width / 2.0f;
        float childHeight = rectangle.height / 2.0f;

        childRec =
        {
            Rectangle {rectangle.x, rectangle.y, childWidth, childHeight},                                // top left
            Rectangle {rectangle.x + childWidth, rectangle.y, childWidth, childHeight},                    // top right
            Rectangle {rectangle.x, rectangle.y + childHeight, childWidth, childHeight},                // bottom left
            Rectangle {rectangle.x + childWidth, rectangle.y + childHeight, childWidth, childHeight}    // bottom right
        };
    }

    /// \brief Clears the quadtree, removing all items and child nodes.
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

    /// \brief Returns the number of items stored in the quadtree.
    /// \return The number of items stored in the quadtree.
    size_t size() const
    {
        size_t count = items.size();
        for (int i = 0; i < 4; i++)
            if (childPtr[i])
                count += childPtr[i]->size();

        return count;
    }

    size_t sizeOfDataStructure() const
    {
        size_t count = items.size();
        count += sizeof(depth);
        count += sizeof(rectangle);
        count += sizeof(parent);

        for (int i = 0; i < 4; i++)
            count += sizeof(childRec[i]);

        for (int i = 0; i < 4; i++)
            if (childPtr[i])
                count += childPtr[i]->sizeOfDataStructure();

        return count;
    }


    /// \brief Inserts an item with its corresponding size into the quadtree.
    /// \param item The item to insert.
    /// \param itemSize The size (rectangle) of the item.
    void insert(const T& item, const Rectangle& itemSize)
    {
        for (int i = 0; i < 4; i++)
        {
            if (firstContainsSecond(childRec[i], itemSize))
            {
                // Did we reach the depth limit?
                if (depth + 1 < maxDepth)
                {
                    // Does the child exist?
                    if (!childPtr[i])
                    {
                        // No, create one
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

    /// \brief Searches for items within the specified area.
    /// \param rArea The area to search for items.
    /// \return A list of items within the specified area.
    std::list<T> _search(const Rectangle& rArea) const
    {
        std::list<T> listItems;
        search(rArea, listItems);
        return listItems;
    }

    /// \brief Checks if the first rectangle contains the second rectangle.
    /// \param first The first rectangle.
    /// \param second The second rectangle.
    /// \return True if the first rectangle contains the second rectangle, false otherwise.
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

    /// \brief Traverses the quadtree, drawing its boundaries.
    void traverse()
    {
        DrawRectangleLines(rectangle.x, rectangle.y, rectangle.width, rectangle.height, GRAY);

        for (int i = 0; i < 4; i++)
        {
            if (childPtr[i])
                childPtr[i]->traverse();
        }
    }

    /// \brief Returns the rectangle of the parent quadtree.
    /// \return The rectangle of the parent quadtree.
    Rectangle getParentRec()
    {
        return parent->rectangle;
    }

    /// \brief Returns the rectangle of the current quadtree.
    /// \return The rectangle of the current quadtree.
    Rectangle getRec()
    {
        return rectangle;
    }

    /// \brief Returns the parent quadtree.
    /// \return A pointer to the parent quadtree.
    StaticQuadTree<T>* getParent()
    {
        return parent;
    }

    /// \brief Returns the child quadtree at the specified index.
    /// \param i The index of the child quadtree.
    /// \return A shared pointer to the child quadtree.
    std::shared_ptr<StaticQuadTree<T>> getChildPtr(int i)
    {
        return childPtr[i];
    }

    /// \brief Returns a vector of items stored in the quadtree.
    /// \return A vector of items stored in the quadtree.
    std::vector<std::pair<Rectangle, T>> getItems()
    {
        return items;
    }

protected:
    /// \brief Retrieves items within the specified area and adds them to the provided list.
    /// \param rArea The area to search for items.
    /// \param listItems The list to store the found items.
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

    /// \brief Retrieves items stored in the quadtree and adds them to the provided list.
    /// \param listItems The list to store the retrieved items.
    void retriveItems(std::list<T>& listItems) const
    {
        for (const auto& p : items)
            listItems.push_back(p.second);

        for (int i = 0; i < 4; i++)
            if (childPtr[i])
                childPtr[i]->retriveItems(listItems);
    }

    StaticQuadTree<T>* parent = nullptr;                             ///< Pointer to the parent quadtree.
    size_t depth = 0;                                                ///< The depth of the quadtree.
    Rectangle rectangle;                                             ///< The rectangle representing the bounds of the quadtree.
    std::array<Rectangle, 4> childRec{};                              ///< The child rectangles of the quadtree.
    std::array<std::shared_ptr<StaticQuadTree<T>>, 4> childPtr{};     ///< Pointers to the child quadtrees.
    std::vector<std::pair<Rectangle, T>> items;                       ///< Container of items stored at this level.
    static const int maxDepth = 7;                                    ///< The maximum depth of the quadtree.
};