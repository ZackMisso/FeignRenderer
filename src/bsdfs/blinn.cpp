/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/bsdf.h>
#include <feign/core/sampler.h>
#include <feign/math/coord_frame.h>
#include <feign/math/warp.h>

FEIGN_BEGIN()

Blinn::Blinn(const Blinn::Params *params)
    : BSDF(),
      kd(params->kd),
      ks(params->ks),
      exponent(params->exponent) {}

// TODO: the speed of this could probably be improved
void Blinn::sample(MaterialClosure &closure) const
{
    if (CoordinateFrame::cosTheta(closure.wi) <= 0)
    {
        closure.albedo = COLOR_BLACK;
        closure.pdf = ZERO;
        return;
    }

    Point2f sample = closure.sampler->next2D();

    if (sample[0] < ks.maxValue())
    {
        sample[0] /= ks.maxValue();

        Vector3f wh = WarpSpace::sqrToCosPowHemi(sample, exponent);
        closure.wo = wh * TWO * (wh % closure.wi) - closure.wi;

        // is_specular stores if this is a delta function, while this is technically
        // a specularity, it is not a delta function
        closure.is_specular = false;
        closure.eta = ONE;
    }
    else
    {
        sample[0] = (sample(0) - ks.maxValue()) / (ONE - ks.maxValue());

        /* Warp a uniformly distributed sample on [0,1]^2
           to a direction on a cosine-weighted hemisphere */
        closure.wo = WarpSpace::sqrToCosHemi(sample);

        closure.is_specular = false;
    }

    Vector3f wh = closure.wi + closure.wo;

    // sometimes the norm is extremely small causing fireflies, this probably isn't
    // correct, but this material is not going to be used on the realistic side of
    // this renderer and more for the animation side so I don't mind all that much
    if (wh.norm() < Epsilon)
    {
        closure.albedo = COLOR_BLACK;
        closure.pdf = ZERO;
        return;
    }

    wh = wh.normalized();

    CoordinateFrame frame(wh);
    Vector3f val = frame.toLocal(closure.wo);

    Float dotProd = std::max(wh % closure.wo, Epsilon);

    float diffPdf = (ONE - ks.maxValue()) * WarpSpace::sqrToCosHemiPdf(val);
    float specPdf = (ks.maxValue()) * WarpSpace::sqrToCosPowHemiPdf(val, exponent) * (ONE / (FOUR * dotProd));

    float expVal = std::max(wh(2), ZERO);

    closure.pdf = diffPdf + specPdf;
    closure.albedo = kd * INV_PI +
                     ks * (exponent + TWO) * INV_TWOPI * powf(expVal, exponent);
}

void Blinn::evaluate(MaterialClosure &closure) const
{
    if (CoordinateFrame::cosTheta(closure.wi) <= 0)
    {
        closure.albedo = COLOR_BLACK;
        return;
    }

    Vector3f wh = closure.wo + closure.wi;
    wh = wh.normalized();

    float expVal = std::max(wh(2), ZERO);

    closure.albedo = kd * INV_PI + ks * (exponent + TWO) * INV_TWOPI * powf(expVal, exponent);
}

FEIGN_END()
