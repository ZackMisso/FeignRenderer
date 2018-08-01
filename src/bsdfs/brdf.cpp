#include <feign/bsdfs/brdf.h>

BRDF::BRDF() : SceneNode()
{
    // TODO
}

BRDF::BRDF(SceneNode* parent) : SceneNode(parent)
{
    // TODO
}

BRDF::~BRDF()
{
    // does nothing
}

NodeType BRDF::getNodeType() const
{
    return NT_BSDF;
}

bool BRDF::isPrimitive() const
{
    return false;
}
