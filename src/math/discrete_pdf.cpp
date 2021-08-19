/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/math/discrete_pdf.h>

///////////////////////
//  Discrete PDF 1D  //
///////////////////////

FEIGN_BEGIN()

DiscretePDF1D::DiscretePDF1D(int siz)
{
    cdf = std::vector<Float>(siz + 1);
    cdf[0] = 0.f;
    sum = 0.0;
    normalization = 0.0;
    is_normalized = false;
}

void DiscretePDF1D::convert_pdf_to_cdf()
{
    std::vector<Float> pdf = std::vector<Float>(cdf.size());

    sum = 0.0;
    for (int i = 0; i < cdf.size(); ++i)
    {
        sum += cdf[i];
    }

    for (int i = 0; i < cdf.size(); ++i)
    {
        pdf[i] = cdf[i] / sum;
    }

    normalization = 1.f / sum;

    Float total = 0.f;
    for (int i = 0; i < pdf.size(); ++i)
    {
        cdf[i] = total;
        total += pdf[i];
    }

    assert(std::abs(total - 1.0) < Epsilon);

    cdf.push_back(total);
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

    while (cdf[i] < value && i < cdf.size() - 1)
        ++i;

    return cdf[std::max(i - 1, 0)];
}

int DiscretePDF1D::sample(Float value, Float &pdf) const
{
    int index = sample(value);

    pdf = operator[](index);

    return index;
}

int DiscretePDF1D::sample_reuse(Float &value) const
{
    int index = sample(value);

    value = (value - cdf[index]) / operator[](index);

    return index;
}

int DiscretePDF1D::sample_reuse(Float &value, Float &pdf) const
{
    int index = sample(value, pdf);

    value = (value - cdf[index]) / pdf;

    return index;
}

Float DiscretePDF1D::get_pmf(int entry) const
{
    return cdf[entry + 1] - cdf[entry];
}

// this assumes the cdf is temporarily storing the pmf,
// and a call to convert_pdf_to_cdf will be made after all
// pdf/pmf values have been set
void DiscretePDF1D::set_pmf(int entry, Float value)
{
    cdf[entry] = value;
}

Float DiscretePDF1D::operator[](int entry) const
{
    // LOG("AHHH");
    return cdf[entry + 1] - cdf[entry];
}

Float &DiscretePDF1D::operator[](int entry)
{
    // LOG("AHHH");
    return cdf[entry];
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

FEIGN_END()
