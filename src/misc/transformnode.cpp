#include <feign/misc/transformnode.h>

TransformNode::TransformNode() : Node()
{
    // TODO
}

TransformNode::TransformNode(Node* parent) : Node(parent)
{
    // TODO
}

TransformNode::~TransformNode()
{
    // TODO
}

void TransformNode::preProcess()
{
    throw new NotImplementedException("preprocess transf");

    preProcessChildren();
}

string TransformNode::getName() const
{
    return "transform";
}

NodeType TransformNode::getNodeType() const
{
    return NT_Transform;
}
