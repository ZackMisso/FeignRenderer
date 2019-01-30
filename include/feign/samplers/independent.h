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

    virtual Float next1D();
    virtual Vec2f next2D();
    virtual Vec3f next3D();
    virtual Vec4f next4D();
    // maybe add VecX

    virtual Sampler* copy();
    virtual Sampler* copy(uint32_t seed);

    virtual void nextSample();

    virtual std::string getName() const;

protected:
    pcg32 rng;
    uint32_t sample_seed;
};
