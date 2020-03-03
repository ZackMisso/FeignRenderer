/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/bsdf.h>
#include <feign/core/sampler.h>
#include <feign/math/coord_frame.h>
#include <feign/math/warp.h>

Phong::Phong(Color3f kd, Float ks, Float exponent)
    : BSDF(), kd(kd), ks(ks), exponent(exponent)
{
    ks *= 1.f - kd.maxValue();
}

// TODO: the speed of this could probably be improved
void Phong::sample(MaterialClosure& closure) const
{
    if (CoordinateFrame::cosTheta(closure.wi) <= 0)
    {
        closure.albedo = COLOR_BLACK;
        closure.pdf = 0.f;
        return;
    }

    Point2f sample = closure.sampler->next2D();

    if (sample(0) < ks)
    {
        sample[0] /= ks;
        sample[0] = (sample(0) - ks) / (1.f - ks);

        if (CoordinateFrame::cosTheta(closure.wi) <= 0)
        {
            closure.albedo = COLOR_BLACK;
            closure.pdf = 0.f;
            return;
        }

        /* Warp a uniformly distributed sample on [0,1]^2
           to a direction on a cosine-weighted hemisphere */
        Vector3f wr = Vector3f(0.f, 0.f, 2.f * closure.wi(2)) - closure.wi;
        CoordinateFrame frame(wr);
        Vector3f val = frame.toWorld(WarpSpace::sqrToCosPowHemi(sample, exponent));

        closure.wo = val;
        closure.is_specular = true;
    }
    else
    {
        sample[0] = (sample(0) - ks) / (1.f - ks);

        if (CoordinateFrame::cosTheta(closure.wi) <= 0)
        {
            closure.albedo = COLOR_BLACK;
            closure.pdf = 0.f;
            return;
        }

        /* Warp a uniformly distributed sample on [0,1]^2
           to a direction on a cosine-weighted hemisphere */
        closure.wo = WarpSpace::sqrToCosHemi(sample);

        closure.is_specular = false;
    }

    /* Relative index of refraction: no change */
    closure.eta = 1.0f;

    Vector3f wr = Vector3f(0.f, 0.f, 2.f * closure.wi(2)) - closure.wi;
    CoordinateFrame frame(wr);
    Vector3f val = frame.toLocal(closure.wo);

    float diffPdf = (1.f - ks) * WarpSpace::sqrToCosHemiPdf(closure.wo);
    float specPdf = (ks) * WarpSpace::sqrToCosPowHemiPdf(val, exponent);

    closure.pdf = diffPdf + specPdf;
    closure.albedo = kd * INV_PI +
                     ks * (exponent + 2.f) * INV_TWOPI * powf(closure.wo % wr, exponent);
}

void Phong::evaluate(MaterialClosure& closure) const
{
    Vector3f wr = Vector3f(0.f, 0.f, 2.f * closure.wi(2)) - closure.wi;

    closure.albedo = kd * INV_PI + ks * (exponent + 2.f) * INV_TWOPI * powf(closure.wo % wr, exponent);
}
