#include <feign/bsdfs/bsdf.h>

BSDF::BSDF() : SceneNode()
{
    // TODO
}

BSDF::BSDF(SceneNode* parent) : SceneNode(parent)
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
