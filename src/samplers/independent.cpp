#include <feign/samplers/independent.h>

Independent::Independent() : Sampler()
{
    // TODO
}

Independent::Independent(Node* parent) : Sampler(parent)
{
    // TODO
}

void Independent::preProcess()
{
    preProcessChildren();

    int samples = 0;
    if (!primitives->findInt("sampleCount", samples))
    {
        throw new MissingPrimitiveException("sampleCount");
    }
    sampleCnt = samples;
    currentSample = 0;
}

void Independent::reseed()
{
    throw new NotImplementedException("independent");
}

void Independent::reseed(uint32_t seed)
{
    throw new NotImplementedException("independent");
}

void Independent::reset()
{
    throw new NotImplementedException("independent");
}

Float Independent::next1D()
{
    throw new NotImplementedException("independent");

    return 0;
}

Vec2<Float> Independent::next2D()
{
    throw new NotImplementedException("independent");

    return Vec2<Float>(0);
}

Vec3<Float> Independent::next3D()
{
    throw new NotImplementedException("independent");

    return Vec3<Float>(0);
}

Vec4<Float> Independent::next4D()
{
    throw new NotImplementedException("independent");

    return Vec4<Float>(0);
}

Sampler* Independent::copy()
{
    throw new NotImplementedException("independent");

    return this;
}

Sampler* Independent::copy(uint32_t seed)
{
    throw new NotImplementedException("independent");

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
