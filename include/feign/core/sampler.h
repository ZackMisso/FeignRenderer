/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/math/vector.h>
#include <pcg32.h>

FEIGN_BEGIN()

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
            int sample_cnt);

    virtual ~Sampler() {}

    virtual void reseed() = 0;
    virtual void reseed(uint32_t seed) = 0;

    virtual Float next_1d() = 0;
    virtual Vec2f next_2d() = 0;
    virtual Vec3f next_3d() = 0;
    virtual Vec4f next_4d() = 0;
    // maybe add VecX

    virtual Sampler *copy() = 0;
    virtual Sampler *copy(uint32_t seed) = 0;

    virtual void next_sample() = 0;

    uint32_t get_sample_cnt() const;

protected:
    int sample_cnt;
    uint32_t sample_seed;
    uint32_t current_sample;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Independent Sampler
/////////////////////////////////////////////////
class Independent : public Sampler
{
public:
    struct Params
    {
        Params(int sample_cnt, uint32_t seed)
            : sample_cnt(sample_cnt), seed(seed) {}

        uint32_t seed;
        int sample_cnt;
    };

    Independent();
    Independent(uint32_t seed,
                int sample_cnt);

    virtual void pre_process();

    virtual void reseed();
    virtual void reseed(uint32_t seed);

    virtual Float next_1d();
    virtual Vec2f next_2d();
    virtual Vec3f next_3d();
    virtual Vec4f next_4d();
    // maybe add VecX

    virtual Sampler *copy();
    virtual Sampler *copy(uint32_t seed);

    virtual void next_sample();

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
          int sample_cnt);

    virtual void pre_process();
    virtual void reseed();
    virtual void reseed(uint32_t seed);

    virtual Float next_1d();
    virtual Vec2f next_2d();
    virtual Vec3f next_3d();
    virtual Vec4f next_4d();

    virtual Sampler *copy();
    virtual Sampler *copy(uint32_t seed);

    virtual void next_sample();

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
           int sample_cnt);

    virtual void pre_process();
    virtual void reseed();
    virtual void reseed(uint32_t seed);

    virtual Float next_1d();
    virtual Vec2f next_2d();
    virtual Vec3f next_3d();
    virtual Vec4f next_4d();

    virtual Sampler *copy();
    virtual Sampler *copy(uint32_t seed);

    virtual void next_sample();

protected:
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Sampler Node structure
/////////////////////////////////////////////////
struct SamplerNode : public Node
{
public:
    SamplerNode() : sampler(nullptr) {}
    SamplerNode(std::string name) : Node(name), sampler(nullptr) {}
    SamplerNode(Sampler *sampler) : sampler(sampler) {}

    ~SamplerNode() { delete sampler; }

    Sampler *sampler;
};
/////////////////////////////////////////////////

FEIGN_END()
