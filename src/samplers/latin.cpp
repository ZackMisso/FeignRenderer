/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/sampler.h>

FEIGN_BEGIN()

Latin::Latin() : Sampler() {}

Latin::Latin(uint32_t seed,
             int sample_cnt)
    : Sampler(seed, sample_cnt)
{
}

void Latin::preProcess()
{
    currentSample = 0;

    reseed(sampleSeed);
}

void Latin::reseed()
{
    throw new NotImplementedException("latin reseed");

    // TODO: fix this seeding
    // srand(sample_seed);
    // uint64_t r1 = rand();
    // uint64_t r2 = rand();
    // rng = pcg32(r1, r2);
}

void Latin::reseed(uint32_t seed)
{
    sampleSeed = seed;
    reseed();
}

Float Latin::next1D()
{
    throw new NotImplementedException("latin next1D");

    return 0.0;
    // return rng.nextFloat();
}

Vec2f Latin::next2D()
{
    return Vec2f(next1D(), next1D());
}

Vec3f Latin::next3D()
{
    return Vec3f(next1D(), next1D(), next1D());
}

Vec4f Latin::next4D()
{
    return Vec4f(next1D(), next1D(), next1D(), next1D());
}

Sampler *Latin::copy()
{
    Latin *newSamp = new Latin();
    newSamp->reseed(sampleSeed);

    return newSamp;
}

Sampler *Latin::copy(uint32_t seed)
{
    Latin *newSamp = new Latin();
    newSamp->reseed(seed);

    return newSamp;
}

void Latin::nextSample()
{
    currentSample++;

    throw new NotImplementedException("latin next sample");
}

FEIGN_END()
