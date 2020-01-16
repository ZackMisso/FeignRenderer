/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/ray.h>
#include <feign/misc/intersection.h>

// should this really be a thing???

// this struct represents what is returned from the material shader when it
// gets evaluated
struct MaterialClosure
{
    MaterialClosure()
    {
        throw new FeignRendererException("empty constructor should not be used");
    }
    
    MaterialClosure(Point2f sample,
                    const Intersection* its,
                    const Ray3f* ray)
        : sample(sample),
          its(its),
          ray(ray),
          albedo(Color3f(0.f)),
          emission(Color3f(0.f)),
          wo(Vector3f(1.f)),
          pdf(1.f),
          eta(1.f),
          is_specular(false) { }

    // i do not think this is needed
    // MaterialClosure(const Intersection* its,
    //                 const Ray3f* ray)
    //     : sample(Point2f(0.f)),
    //       its(its),
    //       ray(ray),
    //       bsdf_sample(Color3f(0.f)),
    //       emission(Color3f(0.f)),
    //       wo(Vector3f(1.f)),
    //       bsdf_pdf(1.f),
    //       is_specular(false) { }

    // always inputs
    Point2f sample;          // the random numbers to use
    const Intersection* its; // a reference to the intersection info
    const Ray3f* ray;        // a reference to the incoming ray

    // sometimes inputs or outputs depending on sampling
    Vector3f wo;             // the outgoing ray

    // always outputs
    Color3f albedo;          // the bsdf sample of the material
    Color3f emission;        // the emissive term from the material
    Float pdf;               // the pdf of the bsdf sample
    Float eta;               // how the ior changes
    bool is_specular;        // whether or not this material is discretely evaluated
};

// this struct represents what is returned from a media shader when it gets
// evaluated
struct MediaClosure
{
    // TODO
};
