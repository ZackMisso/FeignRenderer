/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/trans_func.h>

FEIGN_BEGIN()

DavisTrans::DavisTrans(Float h, Float c)
    : c(c)
{
    beta = 2.0f * clamp(h, 0.0f, 1.0f) - 1.0f;
}

Float DavisTrans::computeAlpha(Float tau) const
{
    return std::pow(tau, 1.f - beta) / (std::pow(c, 1.f + beta));
}

Color3f DavisTrans::surfaceSurface(Color3f tau) const
{
    Float t = tau[0];
    if (t < 1e-4f)
        return Color3f(1.0f);

    Float alpha = computeAlpha(t);
    Float Tr = std::pow(1.0f + t / alpha, -alpha);

    return Color3f(std::isnan(Tr) ? 0 : Tr);
}

Color3f DavisTrans::surfaceMedium(Color3f tau) const
{
    Float t = tau[0];
    if (t < 1e-4f)
        return Color3f(1.0f);

    Float alpha = computeAlpha(t);
    Float base = 1.0f + t / alpha;

    Float trSurface = std::pow(base, -alpha);

    Float Tr = trSurface * (beta / base - (beta - 1.0f) * alpha / t * std::log(base));

    return Color3f(std::isnan(Tr) ? 0 : Tr);
}

Color3f DavisTrans::mediumSurface(Color3f tau) const
{
    return surfaceMedium(tau);
}

Color3f DavisTrans::mediumMedium(Color3f tau) const
{
    Float t = tau[0];
    if (t < 1e-4f)
        return Color3f(1.0f);

    Float alpha = computeAlpha(t);
    Float base = 1.0f + t / alpha;
    Float logBase = std::log(base);
    Float trSurface = std::pow(base, -alpha);

    Float term1 = beta * (-1.0f + beta * (1.0f + t) + (-1.0f + 2.0f * beta) * t / alpha) / (t * base * base);
    Float term2 = ((-1.0f + beta) * beta * alpha / (t * t) * (2.0f * t + base) * logBase) / base;
    Float term3 = (beta - 1.0f) * alpha / t * logBase;

    Float Tr = trSurface * (term1 - term2 + term3 * term3);

    return Color3f(std::isnan(Tr) ? 0 : Tr);
}

Float DavisTrans::sigmaBar() const
{
    return 1.0f;
}

Float DavisTrans::sampleSurface(Sampler *sampler) const
{
    Float xi = sampler->next1D();
    auto cdf = [this](float tau)
    { return 1.0f - surfaceSurface(tau)[0]; };
    Float step = 1e6;
    Float result = step * 2;

    while (step > 1e-6)
    {
        if (cdf(result) > xi)
            result -= step;
        else
            result += step;
        step /= 2;
    }

    return result;
}

Float DavisTrans::sampleMedium(Sampler *sampler) const
{
    Float xi = sampler->next1D();
    auto cdf = [this](float tau)
    { return 1.0f - mediumSurface(tau)[0]; };
    Float step = 1e6;
    Float result = step * 2;

    while (step > 1e-6)
    {
        if (cdf(result) > xi)
            result -= step;
        else
            result += step;
        step /= 2;
    }

    return result;
}

FEIGN_END()
