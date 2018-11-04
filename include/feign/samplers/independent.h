#pragma once

#include <feign/samplers/sampler.h>
#include <pcg32.h>

class Independent : public Sampler
{
public:
    Independent();
    Independent(Node* parent);

    virtual void preProcess();

    virtual void reseed();
    virtual void reseed(uint32_t seed);

    virtual void reset();

    virtual Float next1D();
    virtual Vec2<Float> next2D();
    virtual Vec3<Float> next3D();
    virtual Vec4<Float> next4D();
    // maybe add VecX

    virtual Sampler* copy();
    virtual Sampler* copy(uint32_t seed);

    virtual void nextSample();

    virtual string getName() const;

public:
    pcg32 rng;
    uint32_t sample_seed;
};
