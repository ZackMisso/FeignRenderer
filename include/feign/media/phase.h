/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/math/coord_frame.h>

class PhaseFunction : public Node
{
public:
    PhaseFunction();
    virtual ~PhaseFunction();

    virtual Float eval(const Vector3f &wo, const Vector3f& wi) const = 0;
    virtual Float sample(Point2f sample,
                         const Vector3f &wo,
                         Vector3f& wi) const = 0;

    virtual std::string getName() const;
};

class HenyeyGreenstein : public PhaseFunction
{
public:
    HenyeyGreenstein(Float g) : g(g) { }

    Float hg(Float cosTheta, Float g) const
    {
        Float denom = 1.f + g * g + 2.f * g * cosTheta;;
        denom *= std::sqrt(denom);
        return INV_FOURPI * (1.f - g * g) / denom;
    }

    virtual Float eval(const Vector3f &wo, const Vector3f& wi) const
    {
        return hg(wo % wi, g);
    }

    virtual Float sample(Point2f sample,
                         const Vector3f &wo,
                         Vector3f& wi) const
    {
        Float cos_theta;
        if (std::abs(g) < Epsilon)
        {
            cos_theta = 1.f - 2.f * sample[0];
        }
        else
        {
            Float tmp = (1.f - g * g) / (1.f - g + 2.f * g * sample[0]);
            cos_theta = (1.f + g * g - tmp * tmp) / (2.f * g);
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

// TODO: need to get this implemented right

/////////////////////////////////////////////////
// PhaseFunction Node structure
/////////////////////////////////////////////////
struct PhaseFunctionNode : public Node
{
public:
    PhaseFunctionNode() : phase(nullptr) { }
    PhaseFunctionNode(std::string name) : Node(name), phase(nullptr) { }
    PhaseFunctionNode(PhaseFunction* phase) : phase(phase) { }

    ~PhaseFunctionNode() { delete phase; }

    PhaseFunction* phase;
};
/////////////////////////////////////////////////
