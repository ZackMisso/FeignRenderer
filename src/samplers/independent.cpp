#include <feign/samplers/independent.h>

Independent::Independent() : Sampler() { }

Independent::Independent(Node* parent) : Sampler(parent) { }

Independent::Independent(Node* parent,
                         uint32_t seed,
                         uint32_t sample_cnt)
    : Sampler(parent, seed, sample_cnt)
{
}

void Independent::preProcess(bool use_prims)
{
    preProcessChildren(use_prims);

    if (use_prims)
    {
        int samples = 0;
        primitives->findInt("sampleCount", samples, 16);
        sampleCnt = samples;

        int seed = 0;
        primitives->findInt("seed", seed, 0x9486a5);

        sampleSeed = seed;
    }

    currentSample = 0;

    reseed(sampleSeed);
}

void Independent::reseed()
{
    // TODO: fix this seeding
    srand(sampleSeed);
    uint64_t r1 = rand();
    uint64_t r2 = rand();
    rng = pcg32(r1, r2);
}

void Independent::reseed(uint32_t seed)
{
    sampleSeed = seed;
    reseed();
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
    Independent* newSamp = new Independent();
    newSamp->reseed(seed);

    return newSamp;
}

void Independent::nextSample()
{
    currentSample++;
}

std::string Independent::getName() const
{
    return Sampler::getName() + "independent";
}
