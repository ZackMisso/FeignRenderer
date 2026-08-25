/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/sampler.h>

FEIGN_BEGIN()

Halton::Halton() : Sampler()
{
}

Halton::Halton(uint32_t seed,
               int sample_cnt)
    : Sampler(seed, sample_cnt)
{
}

void Halton::pre_process()
{
    throw new NotImplementedException("halton preProcess");

    // preProcessChildren();
    //
    // int samples = 0;
    // primitives->findInt("sampleCount", samples, 16);
    // sampleCnt = samples;
    //
    // int seed = 0;
    // primitives->findInt("seed", seed, 0x9486a5);
    //
    // currentSample = 0;
    //
    // reseed(seed);
}

void Halton::reseed()
{
    throw new NotImplementedException("halton reseed");

    // TODO: fix this seeding
    // srand(sample_seed);
    // uint64_t r1 = rand();
    // uint64_t r2 = rand();
    // rng = pcg32(r1, r2);
}

void Halton::reseed(uint32_t seed)
{
    throw new NotImplementedException("halton reseed");

    // sample_seed = seed;
    // reseed();
}

Float Halton::next_1d()
{
    throw new NotImplementedException("halton next1D");

    return 0.0;
    // return rng.nextFloat();
}

Vec2f Halton::next_2d()
{
    return Vec2f(next_1d(), next_1d());
}

Vec3f Halton::next_3d()
{
    return Vec3f(next_1d(), next_1d(), next_1d());
}

Vec4f Halton::next_4d()
{
    return Vec4f(next_1d(), next_1d(), next_1d(), next_1d());
}

Sampler *Halton::copy()
{
    Halton *new_samp = new Halton();
    new_samp->reseed(sample_seed);

    return new_samp;
}

Sampler *Halton::copy(uint32_t seed)
{
    Halton *new_samp = new Halton();
    new_samp->reseed(seed);

    return new_samp;
}

void Halton::next_sample()
{
    current_sample++;

    throw new NotImplementedException("halton next_sample");
}

FEIGN_END()
