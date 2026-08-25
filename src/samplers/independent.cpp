/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/sampler.h>

// TODO: maybe keep track of dimensions
// TODO: add next1U for use in making copies of samplers

FEIGN_BEGIN()

Independent::Independent() : Sampler() {}

Independent::Independent(uint32_t seed,
                         int sample_cnt)
    : Sampler(seed, sample_cnt)
{
}

void Independent::pre_process()
{
    current_sample = 0;

    reseed(sample_seed);
}

void Independent::reseed()
{
    rng = pcg32(sample_seed, 1);
}

void Independent::reseed(uint32_t seed)
{
    sample_seed = seed;
    rng = pcg32(sample_seed, 1);
}

Float Independent::next_1d()
{
    return rng.nextFloat();
}

Vec2f Independent::next_2d()
{
    return Vec2f(next_1d(), next_1d());
}

Vec3f Independent::next_3d()
{
    return Vec3f(next_1d(), next_1d(), next_1d());
}

Vec4f Independent::next_4d()
{
    return Vec4f(next_1d(), next_1d(), next_1d(), next_1d());
}

// is this copy architecture really desired?
Sampler *Independent::copy()
{
    Independent *new_samp = new Independent();
    new_samp->reseed(sample_seed);

    return new_samp;
}

Sampler *Independent::copy(uint32_t seed)
{
    Independent *new_samp = new Independent(seed, sample_cnt);
    new_samp->reseed(seed);

    return new_samp;
}

void Independent::next_sample()
{
    current_sample++;
}

FEIGN_END()
