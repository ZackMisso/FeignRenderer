/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/trans_func.h>

FEIGN_BEGIN()

Color3f TransFunc::eval(Color3f tau,
                        bool start_on_surface,
                        bool end_on_surface) const
{
    if (start_on_surface && end_on_surface)
        return surface_surface(tau);

    else if (!start_on_surface && !end_on_surface)
    {
        return medium_medium(tau) / sigma_bar();
    }

    else
        return medium_surface(tau);
}

Float TransFunc::sample(Sampler *sampler,
                        bool start_on_surface) const
{
    return start_on_surface ? sample_surface(sampler) : sample_medium(sampler);
}

Color3f TransFunc::surface_probability(Color3f tau,
                                       bool start_on_surface) const
{
    return start_on_surface ? surface_surface(tau) : medium_surface(tau);
}

Color3f TransFunc::medium_pdf(Color3f tau,
                              bool start_on_surface) const
{
    return start_on_surface ? surface_medium(tau) : medium_medium(tau);
}

FEIGN_END()
