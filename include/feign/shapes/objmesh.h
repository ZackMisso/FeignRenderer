#pragma once

#include <feign/shapes/shape.h>

class ObjMesh : public Shape
{
public:
    ObjMesh();
    ObjMesh(Node* parent);

    virtual string getName() const;

    // TODO
};
