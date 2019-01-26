#include <feign/samplers/independent.h>

Independent::Independent() : Sampler() { }

Independent::Independent(Node* parent) : Sampler(parent) { }

void Independent::preProcess()
{
    preProcessChildren();

    int samples = 0;
    primitives->findInt("sampleCount", samples, 16);
    sampleCnt = samples;

    int seed = 0;
    primitives->findInt("seed", seed, 0x9486a5);

    currentSample = 0;

    reseed(seed);
}

void Independent::reseed()
{
    srand(sample_seed);
    uint64_t r1 = rand();
    uint64_t r2 = rand();
    rng = pcg32(r1, r2);
}

void Independent::reseed(uint32_t seed)
{
    sample_seed = seed;
    reseed();
}

void Independent::reset()
{
    currentSample = 0;
    reseed();
}

Float Independent::next1D()
{
    return rng.nextFloat();
}

Vec2<Float> Independent::next2D()
{
    return Vec2<Float>(next1D(), next1D());
}

Vec3<Float> Independent::next3D()
{
    return Vec3<Float>(next1D(), next1D(), next1D());
}

Vec4<Float> Independent::next4D()
{
    return Vec4<Float>(next1D(), next1D(), next1D(), next1D());
}

Sampler* Independent::copy()
{
    Independent* newSamp = new Independent();
    newSamp->reseed(sample_seed);

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
