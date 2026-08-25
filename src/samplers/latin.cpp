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

void Latin::pre_process()
{
    current_sample = 0;

    reseed(sample_seed);
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
    sample_seed = seed;
    reseed();
}

Float Latin::next_1d()
{
    throw new NotImplementedException("latin next1D");

    return 0.0;
    // return rng.nextFloat();
}

Vec2f Latin::next_2d()
{
    return Vec2f(next_1d(), next_1d());
}

Vec3f Latin::next_3d()
{
    return Vec3f(next_1d(), next_1d(), next_1d());
}

Vec4f Latin::next_4d()
{
    return Vec4f(next_1d(), next_1d(), next_1d(), next_1d());
}

Sampler *Latin::copy()
{
    Latin *new_samp = new Latin();
    new_samp->reseed(sample_seed);

    return new_samp;
}

Sampler *Latin::copy(uint32_t seed)
{
    Latin *new_samp = new Latin();
    new_samp->reseed(seed);

    return new_samp;
}

void Latin::next_sample()
{
    current_sample++;

    throw new NotImplementedException("latin next sample");
}

FEIGN_END()
