#pragma once

#include <feign/shapes/shape.h>

class ObjMesh : public Shape
{
public:
    ObjMesh();
    ObjMesh(SceneNode* parent);

    // virtual NodeType getNodeType() const;
    // bool isPrimitive() const;
};
