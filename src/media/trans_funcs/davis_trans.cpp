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

Float DavisTrans::compute_alpha(Float tau) const
{
    return std::pow(tau, 1.f - beta) / (std::pow(c, 1.f + beta));
}

Color3f DavisTrans::surface_surface(Color3f tau) const
{
    Float t = tau[0];
    if (t < 1e-4f)
        return Color3f(1.0f);

    Float alpha = compute_alpha(t);
    Float Tr = std::pow(1.0f + t / alpha, -alpha);

    return Color3f(std::isnan(Tr) ? 0 : Tr);
}

Color3f DavisTrans::surface_medium(Color3f tau) const
{
    Float t = tau[0];
    if (t < 1e-4f)
        return Color3f(1.0f);

    Float alpha = compute_alpha(t);
    Float base = 1.0f + t / alpha;

    Float trSurface = std::pow(base, -alpha);

    Float Tr = trSurface * (beta / base - (beta - 1.0f) * alpha / t * std::log(base));

    return Color3f(std::isnan(Tr) ? 0 : Tr);
}

Color3f DavisTrans::medium_surface(Color3f tau) const
{
    return surface_medium(tau);
}

Color3f DavisTrans::medium_medium(Color3f tau) const
{
    Float t = tau[0];
    if (t < 1e-4f)
        return Color3f(1.0f);

    Float alpha = compute_alpha(t);
    Float base = 1.0f + t / alpha;
    Float logBase = std::log(base);
    Float trSurface = std::pow(base, -alpha);

    Float term1 = beta * (-1.0f + beta * (1.0f + t) + (-1.0f + 2.0f * beta) * t / alpha) / (t * base * base);
    Float term2 = ((-1.0f + beta) * beta * alpha / (t * t) * (2.0f * t + base) * logBase) / base;
    Float term3 = (beta - 1.0f) * alpha / t * logBase;

    Float Tr = trSurface * (term1 - term2 + term3 * term3);

    return Color3f(std::isnan(Tr) ? 0 : Tr);
}

Float DavisTrans::sigma_bar() const
{
    return 1.0f;
}

Float DavisTrans::sample_surface(Sampler *sampler) const
{
    Float xi = sampler->next_1d();
    auto cdf = [this](float tau)
    { return 1.0f - surface_surface(tau)[0]; };
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

Float DavisTrans::sample_medium(Sampler *sampler) const
{
    Float xi = sampler->next_1d();
    auto cdf = [this](float tau)
    { return 1.0f - medium_surface(tau)[0]; };
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
