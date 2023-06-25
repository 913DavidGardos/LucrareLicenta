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
    float x; ///< coordonata x a vectorului.
    float y; ///< coordonata y a vectorului.

    /// \brief Suprascrie operatorul '+' pentru adunarea a doi vectori.
    /// \param other Vectorul care este adunat.
    /// \return Rezultatul adunarii celor doi vectori.
    Vertex operator+(const Vertex& other) const;

    /// \brief Suprascrie operatorul '-' pentru scaderea a doi vectori.
    /// \param other Vectorul care este scazut.
    /// \return Rezultatul scaderii celor doi vectori.
    Vertex operator-(const Vertex& other) const;

    /// \brief Suprascrie operatorul '=' pentru a copia un vector.
    /// \param other Vectorul care este copiat.
    /// \return O referinta la vectorul din stanga operatorului '='.
    Vertex& operator=(const Vertex& other);

    /// \brief Suprascrie operatorul '=' pentru a copia aceasi valoare pentru coordonatele x si y.
    /// \param other valoarea care este copiata.
    /// \return vectorul cu coordonatele x si y updatate.
    Vertex operator=(int other);
};

/// \struct Box
/// \brief Reprezinta un dreptunghi care are id, un punct minim si un punct maxim
struct Box
{
    int id; ///< ID ul de la Box
    Vertex vertex0; ///< Punctul minim.
    Vertex vertex1; ///< Punctul maxim.

    /// \brief Calculeaza centrul dreptunghiului.
    /// \return Vector spre centrul dreptunghiului.
    Vertex center();
};

/// \struct Node
/// \brief Reprezinta un nod care este folosit in algoritmul BVH.
struct Node
{
    Vertex aabbMin; ///< Punctul minim care constrange nodul.
    Vertex aabbMax; ///< Punctul maxim care constrange nodul.
    int leftChild; ///< Indexul copilului din partea stanga din arborele binar din algoritmul BVH.
    int firstBox; ///<  Indexul primului dreptunghi in lista de dreptunghiuri cuprinse de nod..
    int boxCount; ///< Numarul de dreptunghiuri pe care nodul le contine.

    /// \brief Verifica daca nodul este frunza.
    /// \return `true` daca nodul este frunza altfel `false`.
    bool isLeaf();
};

/// \class BvhContainer
/// \brief O clasa care are comportamentul unui algoritm BVH.
///
/// Clasa Bvh Container prezinta functii care efectueaza constructia, updatarea si functionarea algoritmului BVH.
template<typename T>
class BvhContainer
{
public:

    /// \brief Constructor pentru clasa BvhContainer.
    /// \param particleMap structura de date care contine ID ul Particulei si Particula.  
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

    /// \brief Calculeaza punctul minim dintre cei doi vectori
    /// \param a Primul vector.
    /// \param a Al doilea vector.
    /// \return Vector care reprezinta punctul minim dinre cei doi vectori a si b.
    Vertex Min(Vertex& a, Vertex& b)
    {
        Vertex result;

        result.x = (a.x > b.x) ? b.x : a.x;
        result.y = (a.y > b.y) ? b.y : a.y;

        return result;
    }

    /// \brief Calculeaza punctul maxim dintre cei doi vectori
    /// \param a Primul vector.
    /// \param a Al doilea vector.
    /// \return Vector care reprezinta punctul maxim dinre cei doi vectori a si b.
    Vertex Max(Vertex& a, Vertex& b)
    {
        Vertex result;

        result.x = (a.x < b.x) ? b.x : a.x;
        result.y = (a.y < b.y) ? b.y : a.y;

        return result;
    }

    /// \brief Inlocuirea lui a cu b
    /// \param a Primul Box.
    /// \param a Al doilea Box.
    void swap(Box& a, Box& b)
    {
        Box temp = a;
        a = b;
        b = temp;
    }

    /// \brief Updatarea dreptunghiului care face constrangerea pentru nod
    /// \param nodeIdx ID-ul nodului.
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

    /// \brief Divizeaza recursiv structura de date 
    /// \param nodeIdx ID-ul nodului.
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

    /// \brief Construieste structura de date pentru BVH
    void buildBVH()
    {
        Node& root = bvhNode[rootNodeIndex];
        root.leftChild = 0;
        root.firstBox = 0, root.boxCount = boxes.size();
        updateNodeBounds(rootNodeIndex);
        subdivide(rootNodeIndex);
    }

    /// \brief Updateaza structura de date cu valorile curente pe care le detin Particulele
    /// \param deltaT diferenta de timp
    /// \param particles structura de date care contine particulele.
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

    /// \brief Returneaza o lista cu nodurile din BVH
    /// \return Lista care contine noduri
    std::vector<Node>& getBvhNodes()
    {
        return bvhNode;
    }

    /// \brief Returneaza o lista cu Box-urile din BVH
   /// \return Lista care contine Box-uri
    std::vector<Box>& getBoxes()
    {
        return boxes;
    }

    /// \brief Reseteaza structurile de date ale lui BVH
    void reset()
    {
        boxes.clear();
        bvhNode.clear();
    }

    /// \brief Calculeaza locul ocupat in memorie de structurile de date folosite de algoritm
    /// \return Numarul de bytes care ocupa spatiu in memorie
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

    /// \brief Verifica daca exista coliziune intre doua obiecte care sunt stocate in BVH
    /// \return Lista care contine perechi de obiecte care sunt in coliziune
    std::vector<std::pair<int, int>> detectCollisions()
    {
        std::vector<std::pair<int, int>> collisions;

        // Traverse the BVH tree recursively
        traverseBVH(rootNodeIndex, collisions);

        return collisions;
    }

private:
    /// \brief Traverseaza arborele binar din BVH de la radacina la frunze
    /// \param nodeIdx ID-ul nodului
    /// \param collisions Lista care contine perechi de obiecte care sunt in coliziune
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

    /// \brief Verifica daca doua Box-uri sunt in coliziune una cu alta
    /// \param boxA Primul Box
    /// \param boxB Al doilea Box
    /// \return 'true' daca boxA se suprapune cu boxB altfel 'false'
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

    std::vector<Box> boxes; ///< Lista de dreptunghiuri/Box-uri.
    std::vector<Node> bvhNode; ///< Lista a nodurilor din BVH.
    int rootNodeIndex = 0; ///< Indexul nodului radacina din BVH.
    int nodesUsed = 1; ///< Numarul de noduri folosite in BVH.
};