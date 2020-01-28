/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/emitter.h>

InterpEnvironmentEmitter::InterpEnvironmentEmitter(Color3f top,
                                                   Color3f bot)
    : top(top), bot(bot)
{
    // does nothing
}

Color3f InterpEnvironmentEmitter::sample_li(EmitterQuery& rec,
                                            const Point2f& sample,
                                            Float* pdf) const
{
    throw new NotImplementedException("interp env emitter sample_li");

    return Color3f(0.f);
}

Color3f InterpEnvironmentEmitter::sample_pos(EmitterQuery& rec,
                                             const Point2f& sample,
                                             Float* pdf) const
{
    throw new NotImplementedException("interp env emitter sample_pos");

    return Color3f(0.f);
}

Color3f InterpEnvironmentEmitter::evaluate(EmitterQuery& rec) const
{
    throw new NotImplementedException("interp env emitter evaluate");

    return Color3f(0.f);
}
