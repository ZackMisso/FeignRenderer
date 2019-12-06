/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <pcg32.h>

// TODO: rewrite sampler abstraction to allow for different samplers for
//       different portions of the render

/////////////////////////////////////////////////
// Sampler
/////////////////////////////////////////////////
class Sampler : public Node
{
public:
    Sampler();
    Sampler(uint32_t seed,
            uint32_t sample_cnt);

    virtual ~Sampler() { }

    virtual void reseed() = 0;
    virtual void reseed(uint32_t seed) = 0;

    virtual Float next1D() = 0;
    virtual Vec2f next2D() = 0;
    virtual Vec3f next3D() = 0;
    virtual Vec4f next4D() = 0;
    // maybe add VecX

    virtual Sampler* copy() = 0;
    virtual Sampler* copy(uint32_t seed) = 0;

    virtual void nextSample() = 0;

    uint32_t getSampleCnt() const;

protected:
    uint32_t sampleCnt;
    uint32_t sampleSeed;
    uint32_t currentSample;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Independent Sampler
/////////////////////////////////////////////////
class Independent : public Sampler
{
public:
    Independent();
    Independent(uint32_t seed,
                uint32_t sample_cnt);

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
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Latin Hypercubes Sampler
/////////////////////////////////////////////////
class Latin : public Sampler
{
public:
    Latin();
    Latin(uint32_t seed,
          uint32_t sample_cnt);

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
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Halton Sampler
/////////////////////////////////////////////////
class Halton : public Sampler
{
public:
    Halton();
    Halton(uint32_t seed,
           uint32_t sample_cnt);

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
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Sampler Node structure
/////////////////////////////////////////////////
struct SamplerNode : public Node
{
public:
    SamplerNode() : sampler(nullptr) { }
    SamplerNode(Sampler* sampler) : sampler(sampler) { }

    Sampler* sampler;
};
/////////////////////////////////////////////////
