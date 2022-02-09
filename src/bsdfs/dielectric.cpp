/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/bsdf.h>
#include <feign/core/sampler.h>
#include <feign/math/coord_frame.h>

FEIGN_BEGIN()

Dielectric::Dielectric(Float int_ior, Float ext_ior, Color3f albedo)
    : BSDF(), int_ior(int_ior), ext_ior(ext_ior), albedo(albedo) {}

void Dielectric::sample(MaterialClosure &closure) const
{
    Float sample = closure.sampler->next1D();

    float cos_theta = closure.wi[2];

    if (CoordinateFrame::cosTheta(closure.wi) < 0.f)
    {
        // refracts out of object
        closure.eta = int_ior / ext_ior;
    }
    else
    {
        // refracts into object
        closure.eta = ext_ior / int_ior;
    }

    float prob = fresnel(CoordinateFrame::cosTheta(closure.wi),
                         ext_ior,
                         int_ior);

    if (sample < prob)
    {
        // reflects
        Vector3f wr = Vector3f(-closure.wi[0],
                               -closure.wi[1],
                               closure.wi[2]);

        closure.wo = wr;
        closure.eta = 1.f;
        LOG("reflect");
    }
    else
    {
        LOG("ETA: " + std::to_string(closure.eta));
        LOG("WI X: " + std::to_string(closure.wi[0]));
        LOG("WI Y: " + std::to_string(closure.wi[1]));

        // refracts
        Vector3f wr = Vector3f(-closure.wi[0],
                               -closure.wi[1],
                               0.0);

        Vector3f wtperp = wr * closure.eta;
        Vector3f wtpara = Vector3f(0.f,
                                   0.f,
                                   -SQRT(1.0 - wtperp.sqrNorm()));

        LOG("SQR NORM: " + std::to_string(wtperp.sqrNorm()));
        LOG("ONE MINUS: " + std::to_string(1.0 - wtperp.sqrNorm()));
        LOG("SQRT: " + std::to_string(sqrt(1.0 - wtperp.sqrNorm())));

        closure.wo = wtperp + wtpara;

        if (cos_theta < -Epsilon)
            closure.wo[2] = -closure.wo[2];
        LOG("refract");
    }

    LOG("COS THETA: " + std::to_string(cos_theta));
    LOG("X: " + std::to_string(closure.wo[0]));
    LOG("Y: " + std::to_string(closure.wo[1]));
    LOG("Z: " + std::to_string(closure.wo[2]));

    closure.pdf = 1.f;
    closure.albedo = albedo;
}

void Dielectric::evaluate(MaterialClosure &closure) const
{
    closure.albedo = albedo;
}

FEIGN_END()
