#pragma once

#include <feign/node.h>

class Shape : public SceneNode
{
public:
    Shape();
    Shape(SceneNode* parent);
    virtual ~Shape();

    virtual NodeType getNodeType() const;
    bool isPrimitive() const;  
};
