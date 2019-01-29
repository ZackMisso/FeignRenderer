#pragma once

#include <feign/samplers/sampler.h>
#include <pcg32.h>

class Halton : public Sampler
{
public:
    Halton();
    Halton(Node* parent);

    virtual void preProcess();
    virtual void reseed();
    virtual void reseed(uint32_t seed);

    virtual Float next1D();
    virtual Vec2f next2D();
    virtual Vec3f next3D();
    virtual Vec4f next4D();

    virtual Sampler* copy();
    virtual Sampler* copy(uint32_t seed);

    virtual void nextSample();
    virtual std::string getName() const;

protected:
    // TODO
    uint32_t sample_seed;
}
