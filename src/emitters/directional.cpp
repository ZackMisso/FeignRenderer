#include <feign/emitters/directional.h>

DirectionalEmitter::DirectionalEmitter()
{
    throw new NotImplementedException("emitter directional");
}

DirectionalEmitter::DirectionalEmitter(Node* parent)
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

std::string DirectionalEmitter::getName() const
{
    throw new NotImplementedException("emitter directional");
}
