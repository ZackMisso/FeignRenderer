/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/emitter.h>

FEIGN_BEGIN()

// TODO: this entire emitter will probably have to be rewritten

EnvironmentEmitter::EnvironmentEmitter()
{
    throw new NotImplementedException("emitter environment");
}

Color3f EnvironmentEmitter::sample_li(EmitterQuery& rec,
                                  const Point2f& sample,
                                  Float* pdf) const
{
    throw new NotImplementedException("emitter environment");
}

Color3f EnvironmentEmitter::sample_pos(EmitterQuery& rec,
                                   const Point2f& sample,
                                   Float* pdf) const
{
    throw new NotImplementedException("emitter environment");
}

void EnvironmentEmitter::preProcess()
{
    throw new NotImplementedException("emitter environment");
}

Color3f EnvironmentEmitter::evaluate(EmitterQuery& rec) const
{
    throw new NotImplementedException("environment evaluate");

    return Color3f(0.f);
}

FEIGN_END()
