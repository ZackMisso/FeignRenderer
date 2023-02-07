/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/math/coord_frame.h>
#include <feign/math/warp.h>

FEIGN_BEGIN()

class PhaseFunction : public Node
{
public:
    PhaseFunction();
    virtual ~PhaseFunction();

    virtual Float eval(const Vector3f &wo, const Vector3f &wi) const = 0;
    virtual Float sample(Point2f sample,
                         const Vector3f &wo,
                         Vector3f &wi) const = 0;

    virtual std::string getName() const;
};

class IsotropicPhase : public PhaseFunction
{
public:
    IsotropicPhase() {}

    virtual Float eval(const Vector3f &wo, const Vector3f &wi) const
    {
        return 1.f / (4.f * PI);
    }

    virtual Float sample(Point2f sample,
                         const Vector3f &wo,
                         Vector3f &wi) const
    {
        wi = WarpSpace::sqrToUniSph(sample);
        return INV_FOURPI;
    }
};

class HenyeyGreenstein : public PhaseFunction
{
public:
    HenyeyGreenstein(Float g) : g(g) {}

    Float hg(Float cosTheta, Float g) const
    {
        Float denom = 1.f + g * g - 2.f * g * cosTheta;
        denom *= std::sqrt(denom);
        return INV_FOURPI * (1.f - g * g) / denom;
    }

    virtual Float eval(const Vector3f &wo, const Vector3f &wi) const
    {
        return hg(wo % wi, g);
    }

    virtual Float sample(Point2f sample,
                         const Vector3f &wo,
                         Vector3f &wi) const
    {
        Float cos_theta;
        if (std::abs(g) < Epsilon)
        {
            cos_theta = 1.f - 2.f * sample[0];
        }
        else
        {
            Float numer = -2.f * sample[0] * sample[0] * (g * g * g + g);
            numer += 2.f * sample[0] * (g - 1.f) * (g * g + 1.f);
            numer += (g - 1.f) * (g - 1.f);
            Float denom = (2.f * sample[0] - 1.f) * g + 1.f;
            denom *= denom;
            cos_theta = -numer / denom;
        }

        Float sin_theta = std::sqrt(std::max(Float(0.f), 1.f - cos_theta * cos_theta));
        Float phi = 2.f * PI * sample[1];

        Vector3f v1;
        Vector3f v2;

        CoordinateFrame::coordinateSystem(wo, v1, v2);

        wi = CoordinateFrame::sphericalDirection(cos_theta,
                                                 sin_theta,
                                                 phi,
                                                 v1,
                                                 v2,
                                                 -wo);

        return hg(-cos_theta, g);
    }

    virtual std::string getName() const { return "henyey_green"; }

    Float g;
};

// linearly anisotropic phase
class EddingtonPhase : public PhaseFunction
{
public:
    EddingtonPhase(Float b) : b(b) {}

    // NOTE: this assumes that both wo and wi are unit vectors
    virtual Float eval(const Vector3f &wo, const Vector3f &wi) const
    {
        return INV_FOURPI * (1 + 4.f * (wo % wi));
    }

    virtual Float sample(Point2f sample,
                         const Vector3f &wo,
                         Vector3f &wi) const
    {
        Float cos_theta;

        if (std::abs(b) < Epsilon)
        {
            cos_theta = 1.f - 2.f * sample[0];
        }
        else
        {
            Float tmp = std::sqrt(b * b + 4.f * b * sample[0] - 2.f * b + 1.f);
            cos_theta = (tmp - 1.f) / b;
        }

        Float sin_theta = std::sqrt(std::max(Float(0.f), 1.f - cos_theta * cos_theta));
        Float phi = 2.f * PI * sample[1];

        Vector3f v1;
        Vector3f v2;

        CoordinateFrame::coordinateSystem(wo, v1, v2);

        wi = CoordinateFrame::sphericalDirection(cos_theta,
                                                 sin_theta,
                                                 phi,
                                                 v1,
                                                 v2,
                                                 -wo);

        return eval(wo, wi);
    }

    Float b;
};

class RayleighPhase : public PhaseFunction
{
public:
    RayleighPhase() {}

    virtual Float eval(const Vector3f &wo, const Vector3f &wi) const
    {
        Float cos_theta = wo % wi;
        return 3.f * (cos_theta * cos_theta + 1.f) * INV_FOURPI / 4.f;
    }

    virtual Float sample(Point2f sample,
                         const Vector3f &wo,
                         Vector3f &wi) const
    {
        Float cos_theta;

        Float tmp = -4.f * sample[0] + sqrt(16.f * (sample[0] - 1.f) * sample[0] + 5.f) + 2.f;

        cos_theta = (1.f - powf(tmp, 2.f / 3.f)) /
                    (powf(tmp, 1.f / 3.f));

        Float sin_theta = std::sqrt(std::max(Float(0.f), 1.f - cos_theta * cos_theta));
        Float phi = 2.f * PI * sample[1];

        Vector3f v1;
        Vector3f v2;

        CoordinateFrame::coordinateSystem(wo, v1, v2);

        wi = CoordinateFrame::sphericalDirection(cos_theta,
                                                 sin_theta,
                                                 phi,
                                                 v1,
                                                 v2,
                                                 -wo);

        return eval(wo, wi);
    }
};

class EllipsoidalPhase : public PhaseFunction
{
public:
    EllipsoidalPhase(Float b) : b(b) {}

    virtual Float eval(const Vector3f &wo, const Vector3f &wi) const
    {
        Float cos_theta = wo % wi;
        return b * INV_TWOPI / ((1.f - b * cos_theta) * log((b + 1.f) / (1.f - b)));
    }

    virtual Float sample(Point2f sample,
                         const Vector3f &wo,
                         Vector3f &wi) const
    {
        Float cos_theta;

        cos_theta = (1.f - (b + 1.f) * powf((b + 1.f) / (1.f - b), -sample[0])) / b;

        Float sin_theta = std::sqrt(std::max(Float(0.f), 1.f - cos_theta * cos_theta));
        Float phi = 2.f * PI * sample[1];

        Vector3f v1;
        Vector3f v2;

        CoordinateFrame::coordinateSystem(wo, v1, v2);

        wi = CoordinateFrame::sphericalDirection(cos_theta,
                                                 sin_theta,
                                                 phi,
                                                 v1,
                                                 v2,
                                                 -wo);

        return eval(wo, wi);
    }

    Float b;
};

// TODO: continue from here
class BinomialPhase : public PhaseFunction
{
public:
    BinomialPhase(Float n) : n(n) {}

    virtual Float eval(const Vector3f &wo, const Vector3f &wi) const
    {
        Float cos_theta = wo % wi;
        return INV_PI * powf(2.f, -n - 2.f) * (n + 1.f) * powf(cos_theta + 1.f, n);
    }

    virtual Float sample(Point2f sample,
                         const Vector3f &wo,
                         Vector3f &wi) const
    {
        throw new NotImplementedException("");
        // TODO
        return 0.f;
    }

    Float n;
};

class GegenbaurPhase : public PhaseFunction
{
public:
    GegenbaurPhase() {}

    virtual Float eval(const Vector3f &wo, const Vector3f &wi) const
    {
        throw new NotImplementedException("");
        // TODO
        return 0.f;
    }

    virtual Float sample(Point2f sample,
                         const Vector3f &wo,
                         Vector3f &wi) const
    {
        throw new NotImplementedException("");
        // TODO
        return 0.f;
    }
};

class LiuPhase : public PhaseFunction
{
public:
    LiuPhase() {}

    virtual Float eval(const Vector3f &wo, const Vector3f &wi) const
    {
        throw new NotImplementedException("");
        // TODO
        return 0.f;
    }

    virtual Float sample(Point2f sample,
                         const Vector3f &wo,
                         Vector3f &wi) const
    {
        throw new NotImplementedException("");
        // TODO
        return 0.f;
    }
};

class SphericalGaussianPhase : public PhaseFunction
{
public:
    SphericalGaussianPhase() {}

    virtual Float eval(const Vector3f &wo, const Vector3f &wi) const
    {
        throw new NotImplementedException("");
        // TODO
        return 0.f;
    }

    virtual Float sample(Point2f sample,
                         const Vector3f &wo,
                         Vector3f &wi) const
    {
        throw new NotImplementedException("");
        // TODO
        return 0.f;
    }
};

/////////////////////////////////////////////////
// PhaseFunction Node structure
/////////////////////////////////////////////////
struct PhaseFunctionNode : public Node
{
public:
    PhaseFunctionNode() : phase(nullptr) {}
    PhaseFunctionNode(std::string name) : Node(name), phase(nullptr) {}
    PhaseFunctionNode(PhaseFunction *phase) : phase(phase) {}

    ~PhaseFunctionNode() { delete phase; }

    PhaseFunction *phase;
};
/////////////////////////////////////////////////

FEIGN_END()
