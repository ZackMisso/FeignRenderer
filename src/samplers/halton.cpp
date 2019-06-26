#include <feign/samplers/halton.h>

Halton::Halton() : Sampler() { }

Halton::Halton(Node* parent) : Sampler(parent) { }

Halton::Halton(Node* parent,
               uint32_t seed,
               uint32_t sample_cnt)
    : Sampler(parent, seed, sample_cnt)
{
}

void Halton::preProcess(bool use_prims)
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

Float Halton::next1D()
{
    throw new NotImplementedException("halton next1D");

    return 0.0;
    // return rng.nextFloat();
}

Vec2f Halton::next2D()
{
    return Vec2f(next1D(), next1D());
}

Vec3f Halton::next3D()
{
    return Vec3f(next1D(), next1D(), next1D());
}

Vec4f Halton::next4D()
{
    return Vec4f(next1D(), next1D(), next1D(), next1D());
}

Sampler* Halton::copy()
{
    Halton* newSamp = new Halton();
    newSamp->reseed(sampleSeed);

    return newSamp;
}

Sampler* Halton::copy(uint32_t seed)
{
    Halton* newSamp = new Halton();
    newSamp->reseed(seed);

    return newSamp;
}

void Halton::nextSample()
{
    currentSample++;

    throw new NotImplementedException("halton nextSample");
}

std::string Halton::getName() const
{
    return Sampler::getName() + "halton";
}
