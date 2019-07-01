#include <feign/filters/gauss_filter.h>

GaussFilter::GaussFilter() : ReconstructionFilter()
{
    // does nothing
}

GaussFilter::GaussFilter(Vec2f size) : ReconstructionFilter(size)
{
    // does nothing
}

void GaussFilter::preProcess()
{
    preProcessChildren();

    float xrad = size[0];
    float yrad = size[1];
    alpha = 2.f;

    expCoeff[0] = std::exp(-alpha * xrad * xrad);
    expCoeff[1] = std::exp(-alpha * yrad * yrad);
}

Float GaussFilter::evaluate(const Point2f& p) const
{
    Float xval = 0.f;
    Float yval = 0.f;

    xval = std::max(0.0, double(exp(-alpha * p(0) * p(0)) - expCoeff(0)));
    yval = std::max(0.0, double(exp(-alpha * p(1) * p(1)) - expCoeff(1)));

    return xval * yval;
}

std::string GaussFilter::getName() const
{
    return ReconstructionFilter::getName() + "gaussian";
}
