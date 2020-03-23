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

Blinn::Blinn(const Blinn::Params* params)
    : BSDF(),
      kd(params->kd),
      ks(params->ks),
      exponent(params->exponent) { }

// TODO: the speed of this could probably be improved
void Blinn::sample(MaterialClosure& closure) const
{
    if (CoordinateFrame::cosTheta(closure.wi) <= 0)
    {
        closure.albedo = COLOR_BLACK;
        closure.pdf = 0.f;
        return;
    }

    Point2f sample = closure.sampler->next2D();

    if (sample[0] < ks.maxValue()) {
        sample[0] /= ks.maxValue();

        if (sample[0] > 1.0001) assert(false);

        Vector3f wh = WarpSpace::sqrToCosPowHemi(sample, exponent);
        closure.wo = wh * 2.f * (wh % closure.wi) - closure.wi;

        // is_specular stores if this is a delta function, while this is technically
        // a specularity, it is not a delta function
        closure.is_specular = false;
        closure.eta = 1.f;
    } else {
        sample[0] = (sample(0) - ks.maxValue()) / (1.f - ks.maxValue());

        /* Warp a uniformly distributed sample on [0,1]^2
           to a direction on a cosine-weighted hemisphere */
        closure.wo = WarpSpace::sqrToCosHemi(sample);

        closure.is_specular = false;
    }

    Vector3f wh = closure.wi + closure.wo;
    if (wh.norm() < Epsilon)
    {
        LOG("wh norm:", wh.norm());
        closure.albedo = COLOR_BLACK;
        closure.pdf = 0.f;
        return;
    }
    
    wh = wh.normalized();

    CoordinateFrame frame(wh);
    Vector3f val = frame.toLocal(closure.wo);

    Float dotProd = std::max(wh % closure.wo, Epsilon);

    float diffPdf = (1.f - ks.maxValue()) * WarpSpace::sqrToCosHemiPdf(val);
    float specPdf = (ks.maxValue()) * WarpSpace::sqrToCosPowHemiPdf(val, exponent) * (1.f / (4.f * dotProd));

    float expVal = std::max(wh(2), 0.f);

    closure.pdf = diffPdf + specPdf;
    closure.albedo = kd * INV_PI +
                     ks * (exponent + 2.f) * INV_TWOPI * powf(expVal, exponent);
}

void Blinn::evaluate(MaterialClosure& closure) const
{
    if (CoordinateFrame::cosTheta(closure.wi) <= 0)
    {
        closure.albedo = COLOR_BLACK;
        return;
    }

    Vector3f wh = closure.wo + closure.wi;
    wh = wh.normalized();

    float expVal = std::max(wh(2), 0.f);

    closure.albedo = kd * INV_PI + ks * (exponent + 2.f) * INV_TWOPI * powf(expVal, exponent);
}
