#include <feign/bsdfs/bsdf.h>

BSDF::BSDF() : Node()
{
    // TODO
}

BSDF::BSDF(Node* parent) : Node(parent)
{
    // TODO
}

BSDF::~BSDF()
{
    // does nothing
}

NodeType BSDF::getNodeType() const
{
    return NT_BSDF;
}

bool BSDF::isPrimitive() const
{
    return false;
}
