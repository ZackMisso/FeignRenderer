/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/vector.h>

FEIGN_BEGIN()


struct DiscretePDF1D
{
    DiscretePDF1D(int siz);

    // normalizes the cdf (why should it not be normalized??)
    void normalize();
    void convert_pdf_to_cdf();

    // samples a location in the discrete pdf based on the value
    int sample(Float value) const;
    int sample(Float value, Float& pdf) const;

    int sample_reuse(Float& value) const;
    int sample_reuse(Float& value, Float& pdf) const;

    Float get_pmf(int entry) const;
    void set_pmf(int entry, Float value);

    // returns the pdf
    Float operator[](int entry) const;

    // sets the pdf
    Float& operator[](int entry);

    std::vector<Float> cdf;
    Float sum;
    Float normalization;
    bool is_normalized;
};

struct DiscretePDF2D
{
    DiscretePDF2D();
    ~DiscretePDF2D();
};

FEIGN_END()
