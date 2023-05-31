#include "BvhContainer.h"

// Vertex
Vertex Vertex::operator+(const Vertex& other) const
{
    Vertex result;
    result.x = x + other.x;
    result.y = y + other.y;
    return result;
}
Vertex Vertex::operator-(const Vertex& other) const
{
    Vertex result;
    result.x = x - other.x;
    result.y = y - other.y;
    return result;
}
Vertex& Vertex::operator=(const Vertex& other)
{
    x = other.x;
    y = other.y;
    return *this;
}
Vertex Vertex::operator=(int other)
{
    x = other;
    y = other;
    return *this;
}

bool Node::isLeaf() { return boxCount > 0; }

Vertex Box::center() { return Vertex{ (vertex1 + vertex0).x / 2, (vertex1 + vertex0).y / 2 }; }

// Bvh Container
BvhContainer::BvhContainer(std::map<int, std::shared_ptr<Particle>>& particleMap)
{
    for (int i = 0; i < particleMap.size(); i++)
    {
        float xMin = particleMap[i]->getX() - particleMap[i]->getRadius();
        float yMin = particleMap[i]->getY() - particleMap[i]->getRadius();
        float xMax = particleMap[i]->getX() + particleMap[i]->getRadius();
        float yMax = particleMap[i]->getY() + particleMap[i]->getRadius();
        boxes.push_back(Box{ particleMap[i]->getId(), Vertex{xMin, yMin}, Vertex{xMax, yMax} });
    }
    bvhNode.resize(2*boxes.size());

}

Vertex BvhContainer::Min(Vertex& a, Vertex& b)
{
    Vertex result;

    result.x = (a.x > b.x) ? b.x : a.x;
    result.y = (a.y > b.y) ? b.y : a.y;

    return result;
}

Vertex BvhContainer::Max(Vertex& a, Vertex& b)
{
    Vertex result;

    result.x = (a.x < b.x) ? b.x : a.x;
    result.y = (a.y < b.y) ? b.y : a.y;

    return result;
}

void BvhContainer::swap(Box& a, Box& b)
{
    Box temp = a;
    a = b;
    b = temp;
}

void BvhContainer::updateNodeBounds(int nodeIdx)
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

    //std::cout << "updateNodeBounds" << "\n";

}

void BvhContainer::subdivide(int nodeIdx)
{
    //std::cout << "subdivide" << "\n";

    Node& node = bvhNode[nodeIdx];
    if (node.boxCount <= 2) return;

    // split plane axis and position
    Vertex extent = node.aabbMax - node.aabbMin;
    int axis = 0;
    if (extent.y > extent.x)
        axis = 1;

    //if(axis == 0)
    //    std::cout << "split axis =  x" << "\n";
    //else
    //    std::cout << "split axis =  y" << "\n";

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

void BvhContainer::buildBVH()
{

    Node& root = bvhNode[rootNodeIndex];
    root.leftChild = 0;
    root.firstBox = 0, root.boxCount = boxes.size();
    updateNodeBounds(rootNodeIndex);
    subdivide(rootNodeIndex);

}

void BvhContainer::update(int deltaT, std::map<int, std::shared_ptr<Particle>>& particles)
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

std::vector<Node>& BvhContainer::getBvhNodes()
{
    return bvhNode;
}


std::vector<Box>& BvhContainer::getBoxes()
{
    return boxes;
}