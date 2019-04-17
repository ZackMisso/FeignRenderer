#pragma once

#include <feign/node.h>

// TODO: rewrite sampler abstraction to allow for different samplers for
//       different portions of the render

class Sampler : public Node
{
public:
    Sampler();
    Sampler(Node* parent);
    virtual ~Sampler();

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

    virtual std::string getName() const;
    virtual NodeType getNodeType() const;

    uint32_t getSampleCnt() const;

protected:
    uint32_t sampleCnt;
    uint32_t currentSample;
};
