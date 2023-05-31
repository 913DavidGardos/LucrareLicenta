#pragma once
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <map>
#include <utility>

#include "Particle.h"

struct Vertex
{
    float x, y;
    Vertex operator+(const Vertex& other) const;
    Vertex operator-(const Vertex& other) const;
    Vertex& operator=(const Vertex& other);
    Vertex operator=(int other);
};

struct Box 
{ 
    int id;
    Vertex vertex0, vertex1; 
    Vertex center();
};

struct Node
{
    Vertex aabbMin, aabbMax;
    int leftChild, firstBox, boxCount;
    bool isLeaf();
};

class BvhContainer
{
public:
    BvhContainer(std::map<int, std::shared_ptr<Particle>>& particleVec);

    Vertex Min(Vertex& a, Vertex& b);
    Vertex Max(Vertex& a, Vertex& b);

    void swap(Box& a, Box& b);
    void updateNodeBounds(int nodeIdx);
    void subdivide(int nodeIdx);
    void buildBVH();

    void update(int deltaT, std::map<int, std::shared_ptr<Particle>>& particles);

    std::vector<Node>& getBvhNodes();

    std::vector<Box>& getBoxes();


private:
    std::vector<Box> boxes;
    std::vector<Node> bvhNode;
    int rootNodeIndex = 0;
    int nodesUsed = 1;
};

