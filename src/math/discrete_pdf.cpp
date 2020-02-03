/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/math/discrete_pdf.h>

///////////////////////
//  Discrete PDF 1D  //
///////////////////////

DiscretePDF1D::DiscretePDF1D(int siz)
{
    cdf = std::vector<Float>(siz + 1);
    cdf[0] = 0.f;
    sum = 0.0;
    normalization = 0.0;
    is_normalized = false;
}

void DiscretePDF1D::normalize()
{
    // assert(false);
    sum = cdf[cdf.size() - 1];
    normalization = 1.0 / sum;

    for (int i = 0; i < cdf.size(); ++i)
    {
        cdf[i] *= normalization;
    }
}

// TODO: make this faster
int DiscretePDF1D::sample(Float value) const
{
    int i = 0;

    while(cdf[i] < value && i < cdf.size()-1) ++i;

    return cdf[std::max(i - 1, 0)];
}

int DiscretePDF1D::sample(Float value, Float& pdf) const
{
    int index = sample(value);

    pdf = operator[](index);

    return index;
}

int DiscretePDF1D::sample_reuse(Float& value) const
{
    int index = sample(value);

    value = (value - cdf[index]) / operator[](index);

    return index;
}

int DiscretePDF1D::sample_reuse(Float& value, Float& pdf) const
{
    int index = sample(value, pdf);
    // LOG("value:", value);
    // LOG("min:", cdf[index]);

    value = (value - cdf[index]) / pdf;

    return index;
}

Float DiscretePDF1D::operator[](int entry) const
{
    return cdf[entry + 1] - cdf[entry];
}

///////////////////////
//  Discrete PDF 2D  //
///////////////////////

DiscretePDF2D::DiscretePDF2D()
{
    throw new NotImplementedException("discrete pdf 2d");
}

DiscretePDF2D::~DiscretePDF2D()
{
    throw new NotImplementedException("discrete pdf 2d");
}
