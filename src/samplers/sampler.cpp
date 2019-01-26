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

std::string Sampler::getName() const
{
    return "sampler: ";
}

NodeType Sampler::getNodeType() const
{
    return NT_Sampler;
}

uint32_t Sampler::getSampleCnt() const
{
    return sampleCnt;
}
