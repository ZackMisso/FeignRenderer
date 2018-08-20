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

string Sampler::getName() const
{
    return "sampler: ";
}

NodeType Sampler::getNodeType() const
{
    return NT_Sampler;
}
