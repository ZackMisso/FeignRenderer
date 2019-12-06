/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/emitter.h>

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
