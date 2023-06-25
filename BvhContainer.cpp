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
