#include <feign/media/trans_func.h>

FEIGN_BEGIN()

Color3f TransFunc::eval(Color3f tau,
                        bool startOnSurface,
                        bool endOnSurface) const
{
    if (startOnSurface && endOnSurface)
        return surfaceSurface(tau);

    else if (!startOnSurface && !endOnSurface)
    {
        return mediumMedium(tau) / sigmaBar();
    }

    else
        return mediumSurface(tau);
}

Float TransFunc::sample(Sampler *sampler,
                        bool startOnSurface) const
{
    return startOnSurface ? sampleSurface(sampler) : sampleMedium(sampler);
}

Color3f TransFunc::surfaceProbability(Color3f tau,
                                      bool startOnSurface) const
{
    return startOnSurface ? surfaceSurface(tau) : mediumSurface(tau);
}

Color3f TransFunc::mediumPdf(Color3f tau,
                             bool startOnSurface) const
{
    return startOnSurface ? surfaceMedium(tau) : mediumMedium(tau);
}

FEIGN_END()
