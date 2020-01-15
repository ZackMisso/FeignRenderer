/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/emitter.h>

DirectionalEmitter::DirectionalEmitter()
{
    throw new NotImplementedException("emitter directional");
}

Color3f DirectionalEmitter::sample_li(EmitterQuery& rec,
                                  const Point2f& sample,
                                  Float* pdf) const
{
    throw new NotImplementedException("emitter directional");
}

Color3f DirectionalEmitter::sample_pos(EmitterQuery& rec,
                                   const Point2f& sample,
                                   Float* pdf) const
{
    throw new NotImplementedException("emitter directional");
}

void DirectionalEmitter::preProcess()
{
    throw new NotImplementedException("emitter directional");
}
