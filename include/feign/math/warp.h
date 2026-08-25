/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/core/sampler.h>

FEIGN_BEGIN()

// TODO: maybe redo all of this to also return the pdf with the sample methods???

class WarpSpace
{
public:
    static Point2f sqr_to_uni_disk(const Point2f &sample);
    static Float sqr_to_uni_disk_pdf(const Point2f &p);

    static Vector3f sqr_to_uni_sph(const Point2f &sample);
    static Float sqr_to_uni_sph_pdf(const Vector3f &v);

    static Vector3f sqr_to_uni_sph_cap(const Point2f &sample, Float cos_theta_max);
    static Float sqr_to_uni_sph_cap_pdf(const Vector3f &v, Float cos_theta_max);

    static Vector3f sqr_to_uni_hemi(const Point2f &sample);
    static Float sqr_to_uni_hemi_pdf(const Vector3f &v);

    static Vector3f sqr_to_cos_hemi(const Point2f &sample);
    static Float sqr_to_cos_hemi_pdf(const Vector3f &v);

    static Vector3f sqr_to_cos_pow_hemi(const Point2f &sample, Float n);
    static Float sqr_to_cos_pow_hemi_pdf(const Vector3f &v, Float n);

    static Point2f sqr_to_uni_tri(const Point2f &sample);
    static Float sqr_to_uni_tri_pdf(const Point2f &sample);

    static Vector3f sqr_to_beck(const Point2f &sample, Float alpha);
    static Float sqr_to_beck_pdf(const Vector3f &m, Float alpha);
};

FEIGN_END()
