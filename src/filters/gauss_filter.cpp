/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/recon_filter.h>

GaussFilter::GaussFilter() : ReconstructionFilter()
{
    std_dev = 0.5;
}

GaussFilter::GaussFilter(Vec2f size) : ReconstructionFilter(size)
{
    std_dev = 0.5;
}

GaussFilter::GaussFilter(Vec2f size, Float std_dev)
    : ReconstructionFilter(size), std_dev(std_dev)
{
    // does nothing
}

void GaussFilter::preProcess()
{
    // preProcessChildren();

    Float xrad = size[0];
    Float yrad = size[1];
    std_dev = 0.5;
    alpha = -1.0 / (2.0 * std_dev * std_dev);

    expCoeff[0] = std::exp(alpha * xrad * xrad);
    expCoeff[1] = std::exp(alpha * yrad * yrad);
}

Float GaussFilter::evaluate(const Point2f& p) const
{
    Float xval = 0.f;
    Float yval = 0.f;

    // TODO: maybe replace exp with fastexp or precalculate filter
    xval = std::max(0.0, double(exp(alpha * p(0) * p(0)) - expCoeff(0)));
    yval = std::max(0.0, double(exp(alpha * p(1) * p(1)) - expCoeff(1)));

    return xval * yval;
}
