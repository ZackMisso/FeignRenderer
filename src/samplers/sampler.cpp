#include <feign/samplers/sampler.h>

Sampler::Sampler() : Node()
{
    // TODO
}

Sampler::Sampler(Node* parent) : Node(parent)
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
