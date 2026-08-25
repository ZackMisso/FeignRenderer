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

Phong::Phong(Color3f kd, Float ks, Float exponent)
    : BSDF(), kd(kd), ks(ks), exponent(exponent)
{
    ks *= 1.f - kd.max_value();
}

// TODO: the speed of this could probably be improved
void Phong::sample(MaterialClosure &closure) const
{
    if (CoordinateFrame::cos_theta(closure.wi) <= 0)
    {
        closure.albedo = COLOR_BLACK;
        closure.pdf = 0.f;
        return;
    }

    Point2f sample = closure.sampler->next_2d();

    if (sample(0) < ks)
    {
        sample[0] /= ks;

        /* Warp a uniformly distributed sample on [0,1]^2
           to a direction on a cosine-weighted hemisphere */
        Vector3f wr = Vector3f(0.f, 0.f, 2.f * closure.wi(2)) - closure.wi;
        CoordinateFrame frame(wr);
        Vector3f val = frame.to_world(WarpSpace::sqr_to_cos_pow_hemi(sample, exponent));

        closure.wo = val;

        // is_specular stores if this is a delta function, while this is technically
        // a specularity, it is not a delta function
        closure.is_specular = false;
    }
    else
    {
        sample[0] = (sample(0) - ks) / (1.f - ks);

        /* Warp a uniformly distributed sample on [0,1]^2
           to a direction on a cosine-weighted hemisphere */
        closure.wo = WarpSpace::sqr_to_cos_hemi(sample);

        closure.is_specular = false;
    }

    /* Relative index of refraction: no change */
    closure.eta = 1.0f;

    Vector3f wr = Vector3f(0.f, 0.f, 2.f * closure.wi(2)) - closure.wi;
    CoordinateFrame frame(wr);
    Vector3f val = frame.to_local(closure.wo);

    Float dot_prod = std::max(wr % closure.wo, EPSILON);

    float diff_pdf = (1.f - ks) * WarpSpace::sqr_to_cos_hemi_pdf(closure.wo);
    // I forget, what is this 1.f / 4 * cos term again?
    float spec_pdf = (ks)*WarpSpace::sqr_to_cos_pow_hemi_pdf(val, exponent); // * (1.f / (4.f * dotProd));

    closure.pdf = diff_pdf + spec_pdf;
    closure.albedo = kd * INV_PI +
                     ks * (exponent + 2.f) * INV_TWOPI * powf(dot_prod, exponent);
}

void Phong::evaluate(MaterialClosure &closure) const
{
    Vector3f wr = Vector3f(0.f, 0.f, 2.f * closure.wi(2)) - closure.wi;

    closure.albedo = kd * INV_PI + ks * (exponent + 2.f) * INV_TWOPI * powf(closure.wo % wr, exponent);
}

FEIGN_END()
