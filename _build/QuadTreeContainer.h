#pragma once
#include <iostream>
#include "QuadTree.h"

/// \brief A container that uses a static quadtree to store items of type T.
///
/// The StaticQuadTreeContainer class template is a container that uses a static quadtree
/// data structure to store and manage items of type T. It allows efficient insertion, removal,
/// and searching of items based on their position in a 2D space.
template <typename T>
class StaticQuadTreeContainer
{
    using QuadTreeContainer = std::list<std::shared_ptr<T>>;  ///< The underlying container type.

protected:
    QuadTreeContainer allItems; ///< The list of all items in the container.
    StaticQuadTree<typename QuadTreeContainer::iterator> root; ///< The root of the static quadtree.
    std::list<std::shared_ptr<StaticQuadTree<typename QuadTreeContainer::iterator>>> leaves; ///< The nodes from the quadtree.

public:
    /// \brief Codnstructs a StaticQuadTreeContainer object with the specified rectangle and depth.
    /// \param rectangle The bounding rectangle of the quadtree.
    /// \param depth The maximum depth of the quadtree.
    StaticQuadTreeContainer(const Rectangle& rectangle, const size_t depth) : root(rectangle, depth)
    {

    }

    /// \brief Resizes the quadtree to fit the specified area.
    /// \param rArea The new bounding rectangle of the quadtree.
    void resize(const Rectangle& rArea)
    {
        root.resize(rArea);
    }

    /// \brief Returns the number of items in the container.
    /// \return The number of items in the container.
    size_t size() const
    {
        return allItems.size();
    }

    size_t sizeOfDataStructure() const
    {
        return root.sizeOfDataStructure();
    }

    /// \brief Checks if the container is empty.
    /// \return `true` if the container is empty, `false` otherwise.
    bool empty() const
    {
        return allItems.empty();
    }

    /// \brief Clears the container, removing all items.
    void clear()
    {
        root.clear();
        allItems.clear();
    }

    /// \brief Inserts an item into the container at the specified position.
    /// \param item A shared pointer to the item to insert.
    /// \param itemSize The size of the item's bounding rectangle.
    void insert(const std::shared_ptr<T> item, const Rectangle& itemSize)
    {
        allItems.push_back(item);
        root.insert(std::prev(allItems.end()), itemSize);
    }


    /// \brief Searches for items within the specified area.
    /// \param rArea The area to search within.
    /// \return A list of iterators pointing to the items found within the area.
    std::list<typename QuadTreeContainer::iterator> search(const Rectangle& rArea) const
    {
        return root._search(rArea);
    }

    /// \brief Returns an iterator pointing to the beginning of the container.
    /// \return An iterator pointing to the beginning of the container.
    typename QuadTreeContainer::iterator begin()
    {
        return allItems.begin();
    }

    /// \brief Returns an iterator pointing to the end of the container.
    /// \return An iterator pointing to the end of the container.
    typename QuadTreeContainer::iterator end()
    {
        return allItems.end();
    }

    /// \brief Updates the quadtree by reinserting all items.
    ///
    /// This function updates the quadtree by reinserting all items into it. It is called
    /// when the positions or sizes of the items have changed.
    void update()
    {
        root.clear();

        for (auto iter = allItems.begin(); iter != allItems.end(); ++iter)
        {
            auto it = *iter;
            root.insert(iter, Rectangle{ it->getX() - it->getRadius(), it->getY() - it->getRadius(), it->getRadius() * 2.f, it->getRadius() * 2.f });
        }
    }

    /// \brief Draws the lines of the quadtree for visualization purposes.
    ///
    /// This function draws the lines of the quadtree for visualization purposes. It can be
    /// used to visualize the structure of the quadtree.
    void drawLines()
    {
        root.traverse();
    }

    /// \brief Checks if the first rectangle contains the second rectangle.
    /// \param first The first rectangle.
    /// \param second The second rectangle.
    /// \return `true` if the first rectangle contains the second rectangle, `false` otherwise.
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