/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/emitters/distant.h>

DistantEmitter::DistantEmitter()
{
    throw new NotImplementedException("emitter distant");
}

DistantEmitter::DistantEmitter(Node* parent)
{
    throw new NotImplementedException("emitter distant");
}

Color3f DistantEmitter::sample_li(EmitterQuery& rec,
                                  const Point2f& sample,
                                  Float* pdf) const
{
    throw new NotImplementedException("emitter distant");
}

Color3f DistantEmitter::sample_pos(EmitterQuery& rec,
                                   const Point2f& sample,
                                   Float* pdf) const
{
    throw new NotImplementedException("emitter distant");
}

void DistantEmitter::preProcess()
{
    throw new NotImplementedException("emitter distant");
}

std::string DistantEmitter::getName() const
{
    throw new NotImplementedException("emitter distant");
}
