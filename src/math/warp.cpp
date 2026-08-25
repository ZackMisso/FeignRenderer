/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/math/warp.h>
#include <tgmath.h>

FEIGN_BEGIN()

Point2f WarpSpace::sqr_to_uni_disk(const Point2f &sample)
{
    Float r = std::sqrt(sample(1));
    Float sin_phi, cos_phi;
    feign_sincos(2.0 * M_PI * (sample(0)), &sin_phi, &cos_phi);

    return Point2f(Float(cos_phi) * r, Float(sin_phi) * r);
}

Float WarpSpace::sqr_to_uni_disk_pdf(const Point2f &p)
{
    return p.sqr_norm() <= 1 ? INV_PI : 0.0f;
}

Vector3f WarpSpace::sqr_to_uni_sph(const Point2f &sample)
{
    Float z = 1.0f - 2.0f * sample(1);
    Float r = std::sqrt(std::max((Float)0.0f, 1.0f - z * z));
    Float sin_phi, cos_phi;
    feign_sincos(2.0 * M_PI * sample(0), &sin_phi, &cos_phi);
    return Vector3f(r * Float(cos_phi), r * Float(sin_phi), z);
}

Float WarpSpace::sqr_to_uni_sph_pdf(const Vector3f &v)
{
    return INV_FOURPI;
}

Vector3f WarpSpace::sqr_to_uni_sph_cap(const Point2f &sample, Float cos_theta_max)
{
    Float z = sample(1) + (1.f - sample(1)) * cos_theta_max;
    Float r = std::sqrt(1 - z * z);
    Float sin_phi, cos_phi;
    feign_sincos(2.0 * M_PI * sample(0), &sin_phi, &cos_phi);
    return Vector3f(r * Float(cos_phi), r * Float(sin_phi), z);
}

Float WarpSpace::sqr_to_uni_sph_cap_pdf(const Vector3f &v, Float cos_theta_max)
{
    if (v(2) < cos_theta_max)
        return 0.0f;
    return INV_TWOPI / (1.0f - cos_theta_max);
}

Vector3f WarpSpace::sqr_to_uni_hemi(const Point2f &sample)
{
    Float cos_theta = 1.0 - sample(1);
    Float sin_theta = std::sqrt(std::max((Float)0, 1 - cos_theta * cos_theta));

    Float sin_phi, cos_phi;
    feign_sincos(2.0 * M_PI * sample(0), &sin_phi, &cos_phi);

    return Vector3f(Float(cos_phi) * sin_theta, Float(sin_phi) * sin_theta, cos_theta);
}

Float WarpSpace::sqr_to_uni_hemi_pdf(const Vector3f &v)
{
    return v(2) > 0 ? INV_TWOPI : 0.0f;
}

Vector3f WarpSpace::sqr_to_cos_hemi(const Point2f &sample)
{
    Point2f p = sqr_to_uni_disk(sample);
    Float z = std::sqrt(std::max((Float)0, (Float)1.0 - p(0) * p(0) - p(1) * p(1)));

    return Vector3f(p(0), p(1), z);
}

Float WarpSpace::sqr_to_cos_hemi_pdf(const Vector3f &v)
{
    return v(2) > 0 ? INV_PI * v(2) : 0.0f;
}

Vector3f WarpSpace::sqr_to_cos_pow_hemi(const Point2f &sample, Float n)
{
    Float cosTheta = std::pow(sample(1), 1.0f / (n + 1.0f));
    Float sinTheta = std::sqrt(std::max((Float)0.0f, 1.0f - cosTheta * cosTheta));

    Float sin_phi, cos_phi;
    feign_sincos(2.0 * M_PI * sample(0), &sin_phi, &cos_phi);

    return Vector3f(Float(cos_phi) * sinTheta, Float(sin_phi) * sinTheta, cosTheta);
}

Float WarpSpace::sqr_to_cos_pow_hemi_pdf(const Vector3f &v, Float n)
{
    return v(2) > 0 ? 0.5f * (n + 1.0f) * std::pow(v(2), n) * INV_PI : 0.0f;
}

Point2f WarpSpace::sqr_to_uni_tri(const Point2f &sample)
{
    Float u = 1 - sqrt(sample(0));
    Float v = sample(1) * sqrt(sample(0));

    return Point2f(u, v);
}

Float WarpSpace::sqr_to_uni_tri_pdf(const Point2f &sample)
{
    if (sample(0) + sample(1) > 1.0)
        return 0.0f;
    if (sample(0) + sample(1) < 0.0)
        return 0.0f;

    return 2.0f;
}

Vector3f WarpSpace::sqr_to_beck(const Point2f &sample, Float alpha)
{
    throw new NotImplementedException("square to beckmann");

    return Vector3f(0.f);
}

Float WarpSpace::sqr_to_beck_pdf(const Vector3f &m, Float alpha)
{
    throw new NotImplementedException("square to beckmann pdf");

    return 0.f;
}

FEIGN_END()
