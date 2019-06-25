#include <feign/samplers/latin.h>

Latin::Latin() : Sampler() { }

Latin::Latin(Node* parent) : Sampler(parent) { }

Latin::Latin(Node* parent,
             uint32_t seed,
             uint32_t sample_cnt)
{
}

void Latin::preProcess()
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

Sampler* Latin::copy()
{
    Latin* newSamp = new Latin();
    newSamp->reseed(sample_seed);

    return newSamp;
}

Sampler* Latin::copy(uint32_t seed)
{
    Latin* newSamp = new Latin();
    newSamp->reseed(seed);

    return newSamp;
}

void Latin::nextSample()
{
    currentSample++;

    throw new NotImplementedException("latin next sample");
}

std::string Latin::getName() const
{
    return Sampler::getName() + "latin";
}
