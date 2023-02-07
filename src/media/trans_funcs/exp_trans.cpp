/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/trans_func.h>

FEIGN_BEGIN()

Color3f ExpTrans::surfaceSurface(Color3f tau) const
{
    return Exp(-tau);
}

Color3f ExpTrans::surfaceMedium(Color3f tau) const
{
    return Exp(-tau);
}

Color3f ExpTrans::mediumSurface(Color3f tau) const
{
    return Exp(-tau);
}

Color3f ExpTrans::mediumMedium(Color3f tau) const
{
    return Exp(-tau);
}

Float ExpTrans::sigmaBar() const
{
    return 1.0f;
}

Float ExpTrans::sampleSurface(Sampler *sampler) const
{
    return -std::log(1.0f - sampler->next1D());
}

Float ExpTrans::sampleMedium(Sampler *sampler) const
{
    return -std::log(1.0f - sampler->next1D());
}

FEIGN_END()
