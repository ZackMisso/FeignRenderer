/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/sampler.h>

// TODO: maybe keep track of dimensions
// TODO: add next1U for use in making copies of samplers

Independent::Independent() : Sampler() { }

Independent::Independent(uint32_t seed,
                         int sample_cnt)
    : Sampler(seed, sample_cnt)
{
}

void Independent::preProcess()
{
    // preProcessChildren();

    currentSample = 0;

    reseed(sampleSeed);
}

void Independent::reseed()
{
    rng = pcg32(sampleSeed, 1);
}

void Independent::reseed(uint32_t seed)
{
    sampleSeed = seed;
    rng = pcg32(sampleSeed, 1);
}

Float Independent::next1D()
{
    return rng.nextFloat();
}

Vec2f Independent::next2D()
{
    return Vec2f(next1D(), next1D());
}

Vec3f Independent::next3D()
{
    return Vec3f(next1D(), next1D(), next1D());
}

Vec4f Independent::next4D()
{
    return Vec4f(next1D(), next1D(), next1D(), next1D());
}

// is this copy architecture really desired?
Sampler* Independent::copy()
{
    Independent* newSamp = new Independent();
    newSamp->reseed(sampleSeed);

    return newSamp;
}

Sampler* Independent::copy(uint32_t seed)
{
    Independent* newSamp = new Independent(seed, sampleCnt);
    newSamp->reseed(seed);

    return newSamp;
}

void Independent::nextSample()
{
    currentSample++;
}
