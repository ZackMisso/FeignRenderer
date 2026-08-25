/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/trans_func.h>

FEIGN_BEGIN()

Color3f ExpTrans::surface_surface(Color3f tau) const
{
    return exponential(-tau);
}

Color3f ExpTrans::surface_medium(Color3f tau) const
{
    return exponential(-tau);
}

Color3f ExpTrans::medium_surface(Color3f tau) const
{
    return exponential(-tau);
}

Color3f ExpTrans::medium_medium(Color3f tau) const
{
    return exponential(-tau);
}

Float ExpTrans::sigma_bar() const
{
    return 1.0f;
}

Float ExpTrans::sample_surface(Sampler *sampler) const
{
    return -std::log(1.0f - sampler->next_1d());
}

Float ExpTrans::sample_medium(Sampler *sampler) const
{
    return -std::log(1.0f - sampler->next_1d());
}

FEIGN_END()
