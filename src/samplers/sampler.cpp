#include <feign/samplers/sampler.h>

Sampler::Sampler() : SceneNode()
{
    // TODO
}

Sampler::Sampler(SceneNode* parent) : SceneNode(parent)
{
    // TODO
}

Sampler::~Sampler()
{
    // TODO
}

NodeType Sampler::getNodeType() const
{
    return NT_Sampler;
}

bool Sampler::isPrimitive() const
{
    return false;
}
