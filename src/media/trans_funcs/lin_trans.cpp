/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/trans_func.h>

FEIGN_BEGIN()

LinearTrans::LinearTrans(Float maxT_t) : maxT(maxT_t)
{
    // maxT = 3.0;
}

Color3f LinearTrans::surfaceSurface(Color3f tau) const
{
    return Color3f(1.0f) - Min(tau / maxT, 1.0f);
}

Color3f LinearTrans::surfaceMedium(Color3f tau) const
{
    return Step(tau, maxT) * (1.0f / maxT);
}

Color3f LinearTrans::mediumSurface(Color3f tau) const
{
    return Step(tau, maxT);
}

Color3f LinearTrans::mediumMedium(Color3f tau) const
{
    return Boxcar(tau, maxT - 1e-3f, maxT + 1e-3f);
}

Float LinearTrans::sigmaBar() const
{
    return 1.0f / maxT;
}

Float LinearTrans::sampleSurface(Sampler *sampler) const
{
    return maxT * (1.0f - 1e-5f) * sampler->next1D();
}

Float LinearTrans::sampleMedium(Sampler *sampler) const
{
    return maxT;
}

FEIGN_END()
