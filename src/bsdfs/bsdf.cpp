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

string BSDF::getName() const
{
    return "bsdf: ";
}

NodeType BSDF::getNodeType() const
{
    return NT_BSDF;
}
