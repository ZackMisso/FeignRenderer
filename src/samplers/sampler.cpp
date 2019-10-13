/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/sampler.h>

Sampler::Sampler() : Node()
{
    // TODO
}

Sampler::Sampler(Node* parent) : Node(parent)
{
    // TODO
}

Sampler::Sampler(Node* parent,
                 uint32_t seed,
                 uint32_t sample_cnt)
    : Node(parent), sampleSeed(seed), sampleCnt(sample_cnt)
{
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
