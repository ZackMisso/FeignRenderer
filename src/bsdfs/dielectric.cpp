/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/bsdf.h>
#include <feign/math/coord_frame.h>

Dielectric::Dielectric(Float int_ior, Float ext_ior) : BSDF() { }

// the idea is that sample returns eval() / pdf()
Color3f Dielectric::sample(BSDFQuery& rec, const Point2f& sample) const
{
    float cos_theta = rec.wi[2];

    if (CoordinateFrame::cosTheta(rec.wi) < 0.f)
    {
        // refracts out of object
        rec.eta = int_ior / ext_ior;
    }
    else
    {
        // refracts into object
        rec.eta = ext_ior / int_ior;
    }

    float prob = fresnel(CoordinateFrame::cosTheta(rec.wi),
                         ext_ior,
                         int_ior);

    if (sample(0) < prob)
    {
        // reflects
        Vector3f wr = Vector3f(-rec.wi[0],
                               -rec.wi[1],
                               rec.wi[2]);

        rec.wo = wr;
        rec.eta = 1.f;

        return Color3f(1.f);
    }
    else
    {
        // refracts
        Vector3f wr = Vector3f(-rec.wi[0],
                               -rec.wi[1],
                               0.0);

        Vector3f wtperp = wr * rec.eta;
        Vector3f wtpara = Vector3f(0.f,
                                   0.f,
                                   -sqrt(1.0 - wtperp.sqrNorm()));

        rec.wo = wtperp + wtpara;

        if (cos_theta < -Epsilon) rec.wo[2] = -rec.wo[2];

        return Color3f(1.f);
    }
}

Color3f Dielectric::eval(const BSDFQuery& rec) const
{
    return Color3f(0.0f);
}

Float Dielectric::pdf(const BSDFQuery& rec) const
{
    return 0.0f;
}
