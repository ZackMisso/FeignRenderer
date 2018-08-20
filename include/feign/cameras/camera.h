#pragma once

#include <feign/node.h>

class Camera : public Node
{
public:
    Camera();
    Camera(Node* parent);
    virtual ~Camera();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
