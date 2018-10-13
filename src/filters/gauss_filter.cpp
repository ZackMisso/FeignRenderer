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
    // TODO:
    // set exp Coeff and alpha
    throw new NotImplementedException("gauss_filter preprocess");
}

float GaussFilter::evaluate(const Point2f p) const
{
    double xval = 0.0;
    double yval = 0.0;

    xval = max(0.0, double(exp(-alpha * p(0) * p(0)) - expCoeff(0)));
    yval = max(0.0, double(exp(-alpha * p(1) * p(1)) - expCoeff(1)));

    return xval * yval;
}
