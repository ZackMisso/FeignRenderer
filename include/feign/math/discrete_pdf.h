/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/vector.h>

struct DiscretePDF1D
{
    DiscretePDF1D();
    ~DiscretePDF1D();

    // normalizes the cdf (why should it not be normalized??)
    void normalize();

    // samples a location in the discrete pdf based on the value
    int sample(Float value) const;
    int sample(Float value, Float& pdf) const;

    // returns the pdf
    Float operator[](int entry) const;

protected:
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
