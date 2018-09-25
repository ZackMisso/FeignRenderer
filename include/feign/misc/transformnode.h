#pragma once

#include <feign/common.h>
#include <feign/node.h>
#include <feign/math/transform.h>

class TransformNode : public Node
{
public:
    TransformNode();
    TransformNode(Node* parent);
    ~TransformNode();

    virtual void preProcess();

    virtual string getName() const;
    virtual NodeType getNodeType() const;
protected:
    Transform transform;
};
