/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>

FEIGN_BEGIN()

// TODO: convert these to spectrum once spectrums are implemented
static inline Color3f Min(const Color3f &a, Float b)
{
    Color3f result;
    for (int i = 0; i < 3; ++i)
        result[i] = std::min(a(i), b);
    return result;
}

static inline Color3f Max(const Color3f &a, Float b)
{
    Color3f result;
    for (int i = 0; i < 3; ++i)
        result[i] = std::max(a(i), b);
    return result;
}

static inline Color3f Step(Float edge, const Color3f &x)
{
    Color3f result;
    for (int i = 0; i < 3; ++i)
        result[i] = x(i) > edge ? 1.0 : 0.0;
    return result;
}

static inline Color3f Step(const Color3f &edge, Float x)
{
    Color3f result;
    for (int i = 0; i < 3; ++i)
        result[i] = x > edge(i) ? 1.0 : 0.0;
    return result;
}

static inline Color3f Boxcar(const Color3f &x, Float a, Float b)
{
    Color3f result;
    for (int i = 0; i < 3; ++i)
        result[i] = x(i) >= a && x(i) <= b ? 1.0 : 0.0;
    return result;
}

static inline Float Sum(const Color3f &x)
{
    Float result = x(0);
    for (int i = 1; i < 3; ++i)
        result += x(i);
    return result;
}

static inline Float Avg(const Color3f &x)
{
    return Sum(x) / 3.0;
}

// static inline Color3f Exp(const Color3f &a)
// {
//     Color3f result;
//     for (int i = 0; i < 3; ++i)
//         result[i] = std::exp(a(i));
//     return result;
// }

FEIGN_END()
