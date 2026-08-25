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
    if (CoordinateFrame::cos_theta(closure.wi) <= 0)
    {
        closure.albedo = COLOR_BLACK;
        closure.pdf = ZERO;
        return;
    }

    Point2f sample = closure.sampler->next_2d();

    if (sample[0] < ks.max_value())
    {
        sample[0] /= ks.max_value();

        Vector3f wh = WarpSpace::sqr_to_cos_pow_hemi(sample, exponent);
        closure.wo = wh * TWO * (wh % closure.wi) - closure.wi;

        // is_specular stores if this is a delta function, while this is technically
        // a specularity, it is not a delta function
        closure.is_specular = false;
        closure.eta = ONE;
    }
    else
    {
        sample[0] = (sample(0) - ks.max_value()) / (ONE - ks.max_value());

        /* Warp a uniformly distributed sample on [0,1]^2
           to a direction on a cosine-weighted hemisphere */
        closure.wo = WarpSpace::sqr_to_cos_hemi(sample);

        closure.is_specular = false;
    }

    Vector3f wh = closure.wi + closure.wo;

    // sometimes the norm is extremely small causing fireflies, this probably isn't
    // correct, but this material is not going to be used on the realistic side of
    // this renderer and more for the animation side so I don't mind all that much
    if (wh.norm() < EPSILON)
    {
        closure.albedo = COLOR_BLACK;
        closure.pdf = ZERO;
        return;
    }

    wh = wh.normalized();

    CoordinateFrame frame(wh);
    Vector3f val = frame.to_local(closure.wo);

    Float dot_prod = std::max(wh % closure.wo, EPSILON);

    float diff_pdf = (ONE - ks.max_value()) * WarpSpace::sqr_to_cos_hemi_pdf(val);
    float spec_pdf = (ks.max_value()) * WarpSpace::sqr_to_cos_pow_hemi_pdf(val, exponent) * (ONE / (FOUR * dot_prod));

    float exp_val = std::max(wh(2), ZERO);

    closure.pdf = diff_pdf + spec_pdf;
    closure.albedo = kd * INV_PI +
                     ks * (exponent + TWO) * INV_TWOPI * powf(exp_val, exponent);
}

void Blinn::evaluate(MaterialClosure &closure) const
{
    if (CoordinateFrame::cos_theta(closure.wi) <= 0)
    {
        closure.albedo = COLOR_BLACK;
        return;
    }

    Vector3f wh = closure.wo + closure.wi;
    wh = wh.normalized();

    float exp_val = std::max(wh(2), ZERO);

    closure.albedo = kd * INV_PI + ks * (exponent + TWO) * INV_TWOPI * powf(exp_val, exponent);
}

FEIGN_END()
