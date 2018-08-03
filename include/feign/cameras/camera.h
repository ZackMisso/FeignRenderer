#pragma once

#include <feign/node.h>

class Camera : public SceneNode
{
public:
    Camera();
    Camera(SceneNode* parent);
    virtual ~Camera();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
