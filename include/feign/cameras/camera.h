#pragma once

#include <feign/node.h>

class Camera : public Node
{
public:
    Camera();
    Camera(Node* parent);
    virtual ~Camera();

    virtual string getName() const;

    virtual NodeType getNodeType() const;

    // TODO
};
