#include <feign/samplers/independent.h>

Independent::Independent() : Sampler() { }

Independent::Independent(Node* parent) : Sampler(parent) { }

void Independent::preProcess()
{
    preProcessChildren();

    int samples = 0;
    primitives->findInt("sampleCount", samples, 16);
    sampleCnt = samples;

    currentSample = 0;
}

void Independent::reseed()
{
    throw new NotImplementedException("independent reseed");
}

void Independent::reseed(uint32_t seed)
{
    throw new NotImplementedException("independent reseed");
}

void Independent::reset()
{
    throw new NotImplementedException("independent reset");
}

Float Independent::next1D()
{
    throw new NotImplementedException("independent next1D");

    return 0;
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
    throw new NotImplementedException("independent copy");

    return this;
}

Sampler* Independent::copy(uint32_t seed)
{
    throw new NotImplementedException("independent copy seed");

    return this;
}

void Independent::nextSample()
{
    currentSample++;
}

string Independent::getName() const
{
    return Sampler::getName() + "independent";
}
