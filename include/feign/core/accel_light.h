/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/emitter.h>
#include <feign/core/sampler.h>
#include <feign/math/discrete_pdf.h>

FEIGN_BEGIN();

/////////////////////////////////////////////////
// Light Sampling Acceleration Structure
/////////////////////////////////////////////////
class LightAccel
{
public:
    LightAccel() {}
    virtual ~LightAccel() {}

    virtual void clear() = 0;
    virtual void build(const BBox3f &scene_bounds,
                       const std::vector<Emitter *> &emitters) = 0;

    virtual void sampleEmitter(Point3f pos,
                               Sampler *sampler,
                               int &index,
                               Float &pdf) = 0;

    virtual void sampleEmitters(Point3f pos,
                                Sampler *sampler,
                                std::vector<int> &indices,
                                std::vector<Float> &pdf) = 0;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Naive Uniform Light Sampling Acceleration Structure
/////////////////////////////////////////////////
class NaiveLightAccel : public LightAccel
{
public:
    NaiveLightAccel();

    virtual void clear();
    virtual void build(const BBox3f &scene_bounds,
                       const std::vector<Emitter *> &emitters);

    virtual void sampleEmitter(Point3f pos,
                               Sampler *sampler,
                               int &index,
                               Float &pdf);

    virtual void sampleEmitters(Point3f pos,
                                Sampler *sampler,
                                std::vector<int> &indices,
                                std::vector<Float> &pdf);

protected:
    int number_of_emitters;
    float pmf;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Spatial Partitioning Light Sampling Acceleration Structure
/////////////////////////////////////////////////
// TODO: this causes fireflies along the spatial partitioning boundries
class SpatialLightAccel : public LightAccel
{
public:
    struct Params
    {
        Params(int width, int height, int depth)
            : width(width), height(height), depth(depth) {}

        int width;
        int height;
        int depth;
    };

    struct SpatialLightBounds
    {
        ~SpatialLightBounds()
        {
            delete emitter_pdf;
        }

        BBox3f bbox;
        DiscretePDF1D *emitter_pdf;
    };

    SpatialLightAccel(int width, int height, int depth);

    virtual void clear();
    virtual void build(const BBox3f &scene_bounds,
                       const std::vector<Emitter *> &emitters);

    virtual void sampleEmitter(Point3f pos,
                               Sampler *sampler,
                               int &index,
                               Float &pdf);

    virtual void sampleEmitters(Point3f pos,
                                Sampler *sampler,
                                std::vector<int> &indices,
                                std::vector<Float> &pdf);

protected:
    SpatialLightBounds *bounds;
    BBox3f light_area_bounds;

    int width;  // x
    int height; // y
    int depth;  // z
};
/////////////////////////////////////////////////

// TODO: maybe add more in the future

FEIGN_END()
