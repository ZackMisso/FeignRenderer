#pragma once

#include <feign/shapes/shape.h>

class Mesh : public Shape
{
public:
    Mesh();
    Mesh(SceneNode* parent);

    // virtual NodeType getNodeType() const;
    // bool isPrimitive() const;
};
