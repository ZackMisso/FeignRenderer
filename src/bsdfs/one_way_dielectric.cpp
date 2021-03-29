/**
* Author:    Zackary Misso
* Version:   0.2.0
*
* Anyone has permission to use the following code as long as proper
* acknowledgement is provided to the original author(s).
**/

// This material is not based off of any real material, it is simply a hacky
// BSDF to create a cool scene idea.

#include <feign/core/bsdf.h>
#include <feign/core/sampler.h>
#include <feign/math/coord_frame.h>

FEIGN_BEGIN()

OneWayDielectric::OneWayDielectric(Float int_ior, Float ext_ior, Color3f albedo)
    : BSDF(), int_ior(int_ior), ext_ior(ext_ior), albedo(albedo) { }

void OneWayDielectric::sample(MaterialClosure& closure) const
{
    Float sample = closure.sampler->next1D();

    float cos_theta = closure.wi[2];

    bool exiting = false;
    if (CoordinateFrame::cosTheta(closure.wi) < 0.f)
    {
        // refracts out of object
        closure.eta = int_ior / ext_ior;
        exiting = true;
    }
    else
    {
        // refracts into object
        closure.eta = ext_ior / int_ior;
    }

    float prob = fresnel(CoordinateFrame::cosTheta(closure.wi),
                         ext_ior,
                         int_ior);

    // The insides of this object are a one-way mirror. So it will always have
    // total internal reflection
    if (sample < prob || exiting)
    {
        // reflects
        Vector3f wr = Vector3f(-closure.wi[0],
                               -closure.wi[1],
                               closure.wi[2]);

        closure.wo = wr;
        closure.eta = 1.f;
    }
    else
    {
        // refracts
        Vector3f wr = Vector3f(-closure.wi[0],
                               -closure.wi[1],
                               0.0);

        Vector3f wtperp = wr * closure.eta;
        Vector3f wtpara = Vector3f(0.f,
                                   0.f,
                                   -sqrt(1.0 - wtperp.sqrNorm()));

        closure.wo = wtperp + wtpara;

        if (cos_theta < -Epsilon) closure.wo[2] = -closure.wo[2];
    }

    closure.pdf = 1.f;
    closure.albedo = albedo;
}

void OneWayDielectric::evaluate(MaterialClosure& closure) const
{
    closure.albedo = albedo;
}

FEIGN_END()
