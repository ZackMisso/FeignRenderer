/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/trans_func.h>

FEIGN_BEGIN()

LinearTrans::LinearTrans(Float max_t) : max_t(max_t)
{
    // maxT = 3.0;
}

Color3f LinearTrans::surface_surface(Color3f tau) const
{
    return Color3f(1.0f) - min(tau / max_t, 1.0f);
}

Color3f LinearTrans::surface_medium(Color3f tau) const
{
    return step(tau, max_t) * (1.0f / max_t);
}

Color3f LinearTrans::medium_surface(Color3f tau) const
{
    return step(tau, max_t);
}

Color3f LinearTrans::medium_medium(Color3f tau) const
{
    return boxcar(tau, max_t - 1e-3f, max_t + 1e-3f);
}

Float LinearTrans::sigma_bar() const
{
    return 1.0f / max_t;
}

Float LinearTrans::sample_surface(Sampler *sampler) const
{
    return max_t * (1.0f - 1e-5f) * sampler->next_1d();
}

Float LinearTrans::sample_medium(Sampler *sampler) const
{
    return max_t;
}

FEIGN_END()
