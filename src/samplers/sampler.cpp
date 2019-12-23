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
}

Sampler::Sampler(uint32_t seed,
                 int sample_cnt)
    : Node(), sampleSeed(seed), sampleCnt(sample_cnt)
{
}

uint32_t Sampler::getSampleCnt() const
{
    return sampleCnt;
}
