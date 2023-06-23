#pragma once
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <map>
#include <utility>
#include "Particle.h"

/// \struct Vertex
/// \brief Reprezinta un vector
struct Vertex
{
    float x; ///< The x-coordinate of the vertex.
    float y; ///< The y-coordinate of the vertex.

    /// \brief Overloads the '+' operator to perform vector addition.
    /// \param other The vertex to be added.
    /// \return The result of the addition operation.
    Vertex operator+(const Vertex& other) const;

    /// \brief Overloads the '-' operator to perform vector subtraction.
    /// \param other The vertex to be subtracted.
    /// \return The result of the subtraction operation.
    Vertex operator-(const Vertex& other) const;
    
    /// \brief Overloads the assignment operator '=' to copy the values from another vertex.
    /// \param other The vertex to be copied from.
    /// \return A reference to the updated vertex.
    Vertex& operator=(const Vertex& other);

    /// \brief Overloads the assignment operator '=' to set both x and y coordinates to a given value.
    /// \param other The value to be assigned to both x and y coordinates.
    /// \return A reference to the updated vertex.
    Vertex operator=(int other);
};

/// \struct Box
/// \brief Represents a box with an ID and two vertices defining its boundaries.
struct Box 
{ 
    int id; ///< The ID of the box.
    Vertex vertex0; ///< The first vertex defining the box.
    Vertex vertex1; ///< The second vertex defining the box.
    
    /// \brief Calculates and returns the center vertex of the box.
    /// \return The center vertex.
    Vertex center();
};

/// \struct Node
/// \brief Represents a node in a bounding volume hierarchy (BVH).
struct Node
{
    Vertex aabbMin; ///< The minimum point of the axis-aligned bounding box (AABB) enclosing the node.
    Vertex aabbMax; ///< The maximum point of the AABB enclosing the node.
    int leftChild; ///< The index of the left child node in the BVH structure.
    int firstBox; ///< The index of the first box in the list of boxes enclosed by the node.
    int boxCount; ///< The number of boxes enclosed by the node.

    /// \brief Checks if the node is a leaf node.
    /// \return `true` if the node is a leaf, `false` otherwise.
    bool isLeaf();
};

/// \class BvhContainer
/// \brief A class template for constructing and updating a bounding volume hierarchy (BVH) container.
///
/// The BvhContainer class is used to build and update a BVH structure based on a map of particles.
/// It provides methods to compute the bounding boxes, subdivide the nodes, and update the BVH.
template<typename T>
class BvhContainer
{
public:

    /// \brief Constructor for the BvhContainer class.
    /// \param particleMap The map of particles used to construct the BVH.  
    BvhContainer(std::map<int, std::shared_ptr<T>>& particleMap)
    {
        for (int i = 0; i < particleMap.size(); i++)
        {
            float xMin = particleMap[i]->getX() - particleMap[i]->getRadius();
            float yMin = particleMap[i]->getY() - particleMap[i]->getRadius();
            float xMax = particleMap[i]->getX() + particleMap[i]->getRadius();
            float yMax = particleMap[i]->getY() + particleMap[i]->getRadius();
            boxes.push_back(Box{ particleMap[i]->getId(), Vertex{xMin, yMin}, Vertex{xMax, yMax} });
        }
        bvhNode.resize(2 * boxes.size());
    }

    Vertex Min(Vertex& a, Vertex& b)
    {
        Vertex result;

        result.x = (a.x > b.x) ? b.x : a.x;
        result.y = (a.y > b.y) ? b.y : a.y;

        return result;
    }

    Vertex Max(Vertex& a, Vertex& b)
    {
        Vertex result;

        result.x = (a.x < b.x) ? b.x : a.x;
        result.y = (a.y < b.y) ? b.y : a.y;

        return result;
    }

    void swap(Box& a, Box& b)
    {
        Box temp = a;
        a = b;
        b = temp;
    }

    void updateNodeBounds(int nodeIdx)
    {
        // computes/updates the bounds of the node
        Node& node = bvhNode[nodeIdx];
        node.aabbMin = 9999;
        node.aabbMax = -9999;
        for (int first = node.firstBox, i = 0; i < node.boxCount; i++)
        {
            Box& leftBox = boxes[first + i];
            node.aabbMin = Min(node.aabbMin, leftBox.vertex0);
            node.aabbMax = Max(node.aabbMax, leftBox.vertex1);
        }
    }

    void subdivide(int nodeIdx)
    {

        Node& node = bvhNode[nodeIdx];
        if (node.boxCount <= 2) return;

        // split plane axis and position
        Vertex extent = node.aabbMax - node.aabbMin;
        int axis = 0;
        if (extent.y > extent.x)
            axis = 1;

        int splitPos;
        if (axis == 0)
            splitPos = node.aabbMin.x + extent.x / 2;
        else
            splitPos = node.aabbMin.y + extent.y / 2;
        // split the 2 groups in two halves
        int i = node.firstBox;
        int j = i + node.boxCount - 1;
        while (i <= j)
        {

            if (axis == 0)
            {
                if (boxes[i].center().x < splitPos)
                {
                    i++;

                }
                else
                    swap(boxes[i], boxes[j--]);
            }
            else
            {
                if (boxes[i].center().y < splitPos)
                {
                    i++;

                }
                else
                    swap(boxes[i], boxes[j--]);
            }
        }

        // creating child nodes for each half
        int leftCount = i - node.firstBox;

        if (leftCount == 0 || leftCount == node.boxCount) return;

        int leftChildIdx = nodesUsed++;
        int rightChildIdx = nodesUsed++;
        node.leftChild = leftChildIdx;
        bvhNode[leftChildIdx].firstBox = node.firstBox;
        bvhNode[leftChildIdx].boxCount = leftCount;
        bvhNode[rightChildIdx].firstBox = i;
        bvhNode[rightChildIdx].boxCount = node.boxCount - leftCount;

        node.boxCount = 0;

        updateNodeBounds(leftChildIdx);
        updateNodeBounds(rightChildIdx);

        subdivide(leftChildIdx);
        subdivide(rightChildIdx);
    }

    void buildBVH()
    {
        Node& root = bvhNode[rootNodeIndex];
        root.leftChild = 0;
        root.firstBox = 0, root.boxCount = boxes.size();
        updateNodeBounds(rootNodeIndex);
        subdivide(rootNodeIndex);
    }

    /// \brief Updates the BVH with the given delta time and map of particles.
    /// \param deltaT The time difference since the last update.
    /// \param particles The map of particles used to update the BVH.
    void update(int deltaT, std::map<int, std::shared_ptr<Particle>>& particles)
    {
        boxes.clear();
        for (int i = 0; i < particles.size(); i++)
        {
            float xMin = particles[i]->getX() - particles[i]->getRadius();
            float yMin = particles[i]->getY() - particles[i]->getRadius();
            float xMax = particles[i]->getX() + particles[i]->getRadius();
            float yMax = particles[i]->getY() + particles[i]->getRadius();
            boxes.push_back(Box{ particles[i]->getId(), Vertex{xMin, yMin}, Vertex{xMax, yMax} });
        }

        rootNodeIndex = 0;
        nodesUsed = 1;

        bvhNode.clear();
        bvhNode.resize(2 * boxes.size());

        buildBVH();
    }

    /// \brief Retrieves the list of BVH nodes.
    /// \return The list of BVH nodes.
    std::vector<Node>& getBvhNodes()
    {
        return bvhNode;
    }

    /// \brief Retrieves the list of boxes.
    /// \return The list of boxes.
    std::vector<Box>& getBoxes()
    {
        return boxes;
    }

    /// \brief Resets the BVH container.
    void reset()
    {
        boxes.clear();
        nvhNode.clear();
    }

    size_t sizeOfDataStructure()
    {
        size_t count = 0;
        for (auto elem : boxes)
        {
            count += sizeof(elem.id);
            count += sizeof(elem.vertex0);
            count += sizeof(elem.vertex1);
        }
        for (auto elem : bvhNode)
        {
            count += sizeof(elem.aabbMax);
            count += sizeof(elem.aabbMin);
            count += sizeof(elem.boxCount);
            count += sizeof(elem.firstBox);
            count += sizeof(elem.leftChild);
        }
        count += sizeof(rootNodeIndex);
        count += sizeof(nodesUsed);
    
        return count;
    }

    std::vector<std::pair<int, int>> detectCollisions()
    {
        std::vector<std::pair<int, int>> collisions;

        // Traverse the BVH tree recursively
        traverseBVH(rootNodeIndex, collisions);

        return collisions;
    }

private:
    void traverseBVH(int nodeIdx, std::vector<std::pair<int, int>>& collisions)
    {
        Node& node = bvhNode[nodeIdx];

        if (node.isLeaf()) 
        {
            // Leaf node, check for collisions between boxes
            int boxStartIdx = node.firstBox;
            int boxEndIdx = boxStartIdx + node.boxCount - 1;

            for (int i = boxStartIdx; i < boxEndIdx; ++i) 
            {
                for (int j = i + 1; j <= boxEndIdx; ++j)
                {
                    if (areBoxesColliding(boxes[i], boxes[j])) 
                    {
                        // Collision detected, store the box pair
                        collisions.push_back(std::make_pair(boxes[i].id, boxes[j].id));
                    }
                }
            }
        }
        else 
        {
            // Internal node, recursively traverse child nodes
            traverseBVH(node.leftChild, collisions);
            traverseBVH(node.leftChild + 1, collisions);
        }
    }

    bool areBoxesColliding(const Box& boxA, const Box& boxB)
    {
        // Perform collision detection logic between the two boxes
        // You can use whatever collision detection algorithm is suitable for your scenario

        // Example: AABB collision detection
        if (boxA.vertex0.x <= boxB.vertex1.x && boxA.vertex1.x >= boxB.vertex0.x &&
            boxA.vertex0.y <= boxB.vertex1.y && boxA.vertex1.y >= boxB.vertex0.y) 
        {
            return true; // Boxes are colliding
        }

        return false; // Boxes are not colliding
    }

    std::vector<Box> boxes; ///< The list of boxes in the BVH.
    std::vector<Node> bvhNode; ///< The list of BVH nodes.
    int rootNodeIndex = 0; ///< The index of the root node in the BVH.
    int nodesUsed = 1; ///< The number of nodes used in the BVH.
};